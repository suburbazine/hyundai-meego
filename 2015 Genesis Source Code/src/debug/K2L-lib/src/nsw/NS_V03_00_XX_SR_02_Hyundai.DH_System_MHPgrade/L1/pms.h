/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Internal API of the Port Message Service (PMS)
File:           pms.h
Version:        3.0.x-SR-2  
Language:       C
Author(s):      R.Lundstrom, T.Jahnke
Date:           31.January.2012

FileGroup:      Layer I
Customer ID:    34C40C060300xx.HYUNDAI.DH
FeatureCode:    FCR1
------------------------------------------------------------------------------

                (c) Copyright 1998-2012
                SMSC
                All Rights Reserved

------------------------------------------------------------------------------



Modifications
~~~~~~~~~~~~~

Date        By      		Description
2013-10-01	hanhong.keum	Fix Errata Bug

==============================================================================
*/

/*! \file
  * \brief      Internal API of the Port Message Service (PMS)
  */

#ifndef _PMS_H
#define _PMS_H

/*lint -e(537) repeated include file */
#include "mostns1.h"

/*
==============================================================================
    Library internal definitions
==============================================================================
*/
#define PMS_ALIGN(n,a)      (((((n)+(a))-1)/a)*a)

/*lint -emacro((506),PMS_LARGEST) Constant value boolean */
#define PMS_LARGEST(a,b)    (((a)>(b))?(a):(b))

/*
----------------------------------------------------------------------------
    Actions on TxStatus Callback
----------------------------------------------------------------------------
*/
#define PMS_RELEASE                     ((byte)0x00)
#define PMS_RETRY                       ((byte)0x01)

/* Internal, do not use */
#define PMS_RETAIN                      ((byte)0x02) /* N/A */
#define PMS_RETRIGGER                   ((byte)0x03) /* Retriggers a message without having to requeue */


/*
------------------------------------------------------------------------------
    Maximum sizes
------------------------------------------------------------------------------
*/
/*
*/
#define PMS_CTRL_HDR_MAX_SIZE       20

/*
*/
#define PMS_DATA_HDR_MAX_SIZE       10

/*!
 * The define below is used by the MBM to calculate a minimum size of RX
 * messages allocated so that they can be converted internally and used for
 * TX, as used by the RX segmentation.
 */
#define PMS_ERR_MSG_RSVD_PAYLOAD    4

/*! Size of command buffers */
#define PMS_CMD_SIZE       ((word) 10)

/*
----------------------------------------------------------------------------
    Buffer sizes
----------------------------------------------------------------------------
*/

/*
 * Max total buffer necessary
 */
#define PMS_BUFSIZE_DATA_SMALL  PMS_ALIGN(PMS_I2C_DATA_MAX_PAYLOAD + PMS_DATA_HDR_MAX_SIZE, MBM_MEM_UNIT_SIZE)
#define PMS_BUFSIZE_CTRL        PMS_ALIGN(PMS_CTRL_MAX_PAYLOAD + PMS_CTRL_HDR_MAX_SIZE, MBM_MEM_UNIT_SIZE)
#define PMS_BUFSIZE_DATA_LARGE  PMS_ALIGN(PMS_DATA_MAX_PAYLOAD + PMS_DATA_HDR_MAX_SIZE, MBM_MEM_UNIT_SIZE)

/*!
 * Set PMS_BUFSIZE_SMALL to the largest one of the two possible. This is used
 * to preallocate buffers of this size to be able to give a status for
 * SlotAvailable to the INIC.
 */
#define PMS_BUFSIZE_SMALL       PMS_LARGEST(PMS_BUFSIZE_CTRL, PMS_BUFSIZE_DATA_SMALL)

#define PMS_BUFSIZE_SEG_ERROR   (PMS_CTRL_HDR_MAX_SIZE + PMS_ERR_MSG_RSVD_PAYLOAD)

/*
--------------------------------------------------------------------------------
    Default values
--------------------------------------------------------------------------------
*/
#define PMS_DEF_TIMESTAMP      ((dword)0x00000000)
#define PMS_DEF_HANDLE         ((byte)0xFF)
#define PMS_DEF_PRIO           ((byte)0x00)
#define PMS_DEF_RETRY          ((word)0x0001)
#define PMS_DEF_RCVTYPE        ((byte)0x00)
#define PMS_DEF_TGTDEVTYPE     PMS_DEF_RCVTYPE
#define PMS_DEF_SRCADDR        ((word)0x0001)
#define PMS_DEF_TGTADDR        ((word)0x0001)
#define PMS_DEF_FUNCADDR       ((word)0x0000)

/* Individual bytes in MSB order */
/*lint -emacro(572,PMS_DEF_*_0) Shifting is ok (lint uses binary representation as precision)*/
#define PMS_DEF_TIMESTAMP_0    ((byte)(PMS_DEF_TIMESTAMP >> 24)
#define PMS_DEF_TIMESTAMP_1    ((byte)(PMS_DEF_TIMESTAMP >> 16)
#define PMS_DEF_TIMESTAMP_2    ((byte)(PMS_DEF_TIMESTAMP >> 8)
#define PMS_DEF_TIMESTAMP_3    ((byte)(PMS_DEF_TIMESTAMP)
#define PMS_DEF_RETRY_0        ((byte)(PMS_DEF_RETRY >> 8))
#define PMS_DEF_RETRY_1        ((byte)(PMS_DEF_RETRY))
#define PMS_DEF_SRCADDR_0      ((byte)(PMS_DEF_SRCADDR >> 8))
#define PMS_DEF_SRCADDR_1      ((byte)(PMS_DEF_SRCADDR))
#define PMS_DEF_TGTADDR_0      ((byte)(PMS_DEF_TGTADDR >> 8))
#define PMS_DEF_TGTADDR_1      ((byte)(PMS_DEF_TGTADDR))
#define PMS_DEF_FUNCADDR_0     ((byte)(PMS_DEF_FUNCADDR >> 8))
#define PMS_DEF_FUNCADDR_1     ((byte)(PMS_DEF_FUNCADDR))
#define PMS_DEF_FBLOCK_ID      PMS_DEF_FUNCADDR_0
#define PMS_DEF_INST_ID        PMS_DEF_FUNCADDR_1

/*
--------------------------------------------------------------------------------
    Features
--------------------------------------------------------------------------------
*/
#define PMS_F_TELID4            ((byte)0x01)

/*
==============================================================================
    Library internal types
==============================================================================
*/
typedef struct PmsInitStruct
{
    void (*cbExtEvents_fptr)(word event);                   /*!< Notify external events */
#ifndef PACKET_COMMUNICATION_ONLY
    void (*cbIcmRx_fptr)(HMBMBUF handle);                   /*!< Callback on ICM rx messages */
    void (*cbMcmRx_fptr)(HMBMBUF handle);                   /*!< Callback on MCM rx messages */
#endif
    void (*cbMdpRx_fptr)(HMBMBUF handle);                   /*!< Callback on MDP rx messages */
#ifndef PACKET_COMMUNICATION_ONLY
    void (*cbRxErr_fptr)(byte error, MbmCtrlRx *rx_ptr);    /*!< Callback on RX errors (READ-ONLY!!!) */
#endif
    void (*startIfaces_fptr)(void);                         /*!< Starts LLD interfaces(RX) */
    void (*stopIfaces_fptr)(void);                          /*!< Stops LLD interfaces(RX/TX) */
#ifndef PACKET_COMMUNICATION_ONLY
    byte (*i2cTx_fptr)(HMBMBUF handle, byte *add_ptr, word add_size);   /*!< I2C TX function */
    byte (*ctrlTx_fptr)(HMBMBUF handle, byte *add_ptr, word add_size);  /*!< MLB/SPI ctrl TX function */
#endif
    byte (*dataTx_fptr)(HMBMBUF handle, byte *add_ptr, word add_size);  /*!< MLB/SPI async TX function */
#ifndef PACKET_COMMUNICATION_ONLY
    byte icmIface;                                          /*!< Default ICM interface */
    byte mcmIface;                                          /*!< Default MCM interface */
    byte mdpIface;                                          /*!< Default MDP interface */
#endif

    void (*cbBufFreed_fptr)(void);                          /*!< Notify about freed buffers */

#ifdef AMS_TX_BYPASS_FILTER
    bool (*cbTxBypass_fptr)(TMsgTx *msg_ptr);               /*!< TX bypass filter */
#endif

#ifdef NS_AMS_MSV2
    void (*cbTxBufferFlushed_fptr)(word num);   /*!< Notifies the number of messages filtered by transition to configuration NotOk */
#endif

#ifdef MSG_TX_USER_PAYLOAD
    void (*cbFreeUserTxMsgPayload_fptr)(byte *data_ptr);        /*!< Free external (TX) payload */
    void (*cbFreeUserTxMsgPayloadExt_fptr)(TMsgTx *msg_ptr);    /*!< Free external (TX) payload */
#endif

#ifdef DATA_TX_USER_PAYLOAD
    void (*cbFreeUserTxDataPayload_fptr)(byte *data_ptr);       /*!< Free external (TX) payload */
    void (*cbFreeUserTxDataPayloadExt_fptr)(TDataTx *msg_ptr);  /*!< Free external (TX) payload */
#endif

#ifdef MSG_RX_USER_PAYLOAD
    word (*cbGetPayloadBuf_fptr)(TMsgRx *rx_ptr, word required_size);       /*!< Allocate user (RX) payload */
    void (*cbFreePayloadBuf_fptr)(TMsgRx *rx_ptr);                          /*!< Free user (RX) payload */
    word (*cbReallocPayloadBuf_fptr)(TMsgRx *rx_ptr, word required_size);   /*!< Reallocate user (RX) payload */
#endif

    byte rx_burst;                                              /*!< Number of RX messages to be processed */

} PmsInitStruct;


/*! typedef a function pointer for Tx status handlers */
typedef byte(*PmsTxStatusHandler)(HMBMBUF handle, byte status);

/*
==============================================================================
    Library internal function prototypes
==============================================================================
*/

#ifdef PMS_4
    /*lint -sem(PmsInit,1p) */
    void PmsInit(PmsInitStruct *is_ptr);
#endif

#ifdef PMS_5
    void PmsService(void);
#endif

#ifdef PMS_6
    /*lint -sem(PmsGetBuf,r_null) */
    HMBMBUF PmsGetBuf(word size, word type);
#endif

#ifdef PMS_7
    /*lint -sem(PmsSend,1p,custodial(1)) */
    void PmsSend(HMBMBUF handle, PmsTxStatusHandler cbTxStatus_fptr);
#endif

#ifdef PMS_54
    /*lint -sem(PmsSendBypass,1p,custodial(1)) */
    void PmsSendBypass(HMBMBUF handle, PmsTxStatusHandler cbTxStatus_fptr, bool auto_bypass);
#endif

#ifdef PMS_10
    void PmsSetPendingEvent(word event);
#endif

#ifdef PMS_22
    void PmsSync(void);
#endif

#ifdef PMS_42
    /*lint -sem(PmsReleasePayload,1p)*/
    void PmsReleasePayload(TMsgRx *rx_ptr);
#endif

#ifdef PMS_43
    /*lint -sem(PmsInjectWaitingRx,1p,custodial(1)) */
    void PmsInjectWaitingRx(HMBMBUF handle);
#endif

#ifdef PMS_46
    void PmsPrepareReInit(void);
#endif

#ifdef PMS_47
    /*lint -sem(PmsFlushTxQueue,1p) */
    void PmsFlushTxQueue(MbmQueue *q_ptr);
#endif

#ifdef PMS_50
    /*lint -sem(PmsDiscardRx,1p,custodial(1)) */
    bool PmsDiscardRx(HMBMBUF handle);
#endif

#ifdef PMS_55
    void PmsExtendSyncTimeout(void);
#endif

#ifdef PMS_56
    void PmsHandleRetryParamsStatus(const TMsgRx *msg_ptr);
#endif

#ifdef PMS_57
    void PmsHandleMidLevelRetriesStatus(const TMsgRx *msg_ptr);
#endif

#ifdef PMS_58
    void PmsInsertRetryValues(TMsgTx* tx_ptr);
#endif

#ifdef PMS_62
    void PmsInitExternalBuf(HMBMBUF handle, byte *payload, word size, word type);
#endif

#ifdef PMS_69
    byte PmsTriggerNAOverMDP(void);
#endif

// Bug : Errata sheet 3.0.X-6 - 0300XX-016
#ifdef PMS_200 
/*lint -sem(PmsGetBufICM,r_null) */
HMBMBUF PmsGetBufICM(word size);
#endif

#endif /* _PMS_H */


