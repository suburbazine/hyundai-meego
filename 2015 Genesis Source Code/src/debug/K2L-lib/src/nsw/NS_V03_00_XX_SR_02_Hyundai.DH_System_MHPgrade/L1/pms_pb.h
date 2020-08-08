/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Public API of the Port Message Service (PMS)
File:           pms_pb.h
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
Date            By      Description

==============================================================================
*/

/*! \file
  * \brief      Public API of the Port Message Service (PMS)
  */

#ifndef _PMS_PB_H
#define _PMS_PB_H

/*
==============================================================================
    Includes
==============================================================================
*/

#include "mostdef1.h"
#include "mbm_pb.h"

/*
==============================================================================
    Mapping of external events to the correspondent MIS service flags
==============================================================================
*/

#define PMS_E_SYNC_CMD                 ((word) 0x0100)  /* Successful SyncC */
#define PMS_E_SYNC_RESET               ((word) 0x0200)  /* INIC has been reset/started */
#define PMS_E_SYNC_FAILED              ((word) 0x0400)  /* Synchronization failed */
#define PMS_E_SYNC_NOT_SUPPORTED       ((word) 0x0800)  /* PMS Protocol ID not supported by the INIC */


/*
==============================================================================
    PMS Service flags (public only for trace purposes)
==============================================================================
*/
#define PMS_P_NONE                     ((word) 0x0000) /* general 'none'      */

#define PMS_P_RX                       ((word) 0x0001) /* Rx to process       */
#define PMS_P_TIMEOUT                  ((word) 0x0002) /* Timeout             */
#define PMS_P_TX_TRIG                  ((word) 0x0008) /* Trigger sending     */
#define PMS_P_BUF_FREED                ((word) 0x0010) /* A buffer was freed  */
#define PMS_P_MDP_TXSTATUS             ((word) 0x0040) /* TxStatus of MLB MDP */
#define PMS_P_RX_ACK                   ((word) 0x0080) /* Acknowledge SlotAv  */
#define PMS_P_RX_STATUS                ((word) 0x0100) /* Process Status      */
#define PMS_P_GBG_COL                  ((word) 0x0200) /* Garbage collector   */
#define PMS_P_GO_PROTECTED             ((word) 0x0400) /* Protected mode      */
#define PMS_P_RETRIGGER_GBG            ((word) 0x0800) /* Retrigger garbage collector */
#define PMS_P_PROCESS_WAITING_Q        ((word) 0x1000) /* Process the Q waiting for user buffer */
#define PMS_P_TIMEOUT_RX_PREALLOC      ((word) 0x2000) /* Timeout for the RX pre-allocation */
#define PMS_P_GO_NET_OFF               ((word) 0x4000) /* Transition to state NetOff */
#define PMS_P_GO_SEMI_PROTECTED        ((word) 0x8000) /* SemiProtected mode  */

#define PMS_P_FIRST                    PMS_P_RX
#define PMS_P_LAST                     PMS_P_GO_SEMI_PROTECTED

/*
--------------------------------------------------------------------------------
    FIFO masks, used by the set/get interface functions
--------------------------------------------------------------------------------
*/
#define PMS_M_FIFO_MCM                  ((byte) 0x80)   /* intended only for internal use */
#define PMS_M_FIFO_ICM                  ((byte) 0x40)   /* intended only for internal use */
#define PMS_M_FIFO_MDP                  ((byte) 0x20)   /* intended only for internal use */
#define PMS_M_FIFO_CTRL                 (PMS_M_FIFO_MCM | PMS_M_FIFO_ICM)   /* Control FIFOs */
#define PMS_M_FIFO_DATA                 PMS_M_FIFO_MDP                      /* Async FIFO    */
#define PMS_M_FIFO_ALL                  (PMS_M_FIFO_CTRL | PMS_M_FIFO_DATA) /* intended only for internal use */
#define PMS_M_FIFO_NONE                 ((byte) 0x00)                       /* intended only for internal use */
#define PMS_M_FIFO_STAT                 ((byte) 0x10)   /* Status FIFO - intended only for internal use */

/*
--------------------------------------------------------------------------------
    Interfaces, for set/get interface functions
--------------------------------------------------------------------------------
*/
#define PMS_IFACE_I2C           ((byte) 0x00)
#define PMS_IFACE_GENERAL       ((byte) 0x01)
#define PMS_IFACE_MEDIALB       PMS_IFACE_GENERAL /* deprecated: kept for legacy reason */
#define PMS_IFACE_MLB           PMS_IFACE_GENERAL /* deprecated: kept for legacy reason */

#define MSG_TGT_EHC                     ((word) 0x0000)
#define MSG_TGT_INIC                    ((word) 0x0001)
#define MSG_TGT_BROADCAST               ((word) 0x03C8)
#define MSG_TGT_BROADCAST_BLOCKING      ((word) 0x03C8)
#define MSG_TGT_BROADCAST_UNBLOCKING    ((word) 0x03FF)
#define MSG_TGT_DEBUG                   ((word) 0x0FF0)
#define MSG_TGT_UNKNOWN                 ((word) 0xFFFF)
#define MSG_SRC_EHC                     ((word) 0x0000)
#define MSG_SRC_INIC                    ((word) 0x0001)

#define XMIT_WRONGTARGET        ((byte) 0x00)
#define XMIT_SUCCESS            ((byte) 0x10)
#define XMIT_CRC                ((byte) 0x20)
#define XMIT_BUF                ((byte) 0x21)
#define XMIT_TIMEOUT            ((byte) 0x80)
#define XMIT_FAILED             ((byte) 0x40)

#define XMIT_NET_OFF            ((byte) 0x81)
#define XMIT_FRMT               ((byte) 0x82)
#define XMIT_SYNC               ((byte) 0x83)

#define XMIT_INTERNAL           ((byte) 0x84)
#define XMIT_NO_ICM             ((byte) 0x85)
#define XMIT_PROTECTED_MODE     ((byte) 0x87)
#define XMIT_AR_FAILED          ((byte) 0x88)

#define XMIT_SUCCESS_INTERNAL   ((byte) 0x86)

/*
--------------------------------------------------------------------------------
    RX segmentation errors
--------------------------------------------------------------------------------
*/
#define MSG_ERR_1   ((byte)0x01)    /* > PMS            missing the first telegram of a segmented message */
#define MSG_ERR_2   ((byte)0x02)    /* > PMS            buffer overflow (bufferline not long enough) */
#define MSG_ERR_3   ((byte)0x03)    /* > PMS            invalid telegram (corresponding bufferline not found) */
                                    /*                  since wrong blockcounter or wrong source address */
#define MSG_ERR_4   ((byte)0x04)    /* > PMS            buffer overflow (no free bufferline) */
#define MSG_ERR_5   ((byte)0x05)    /* > PMS            missing last telegram(s), garbage collector will clean the bufferline */
#define MSG_ERR_6   ((byte)0x06)    /* > PMS            device is not able to receive a segmented message */
#define MSG_ERR_7   ((byte)0x07)    /* > PMS            missing last telegram(s); message is cleared, since new message from */
                                    /*                  same source address has been received */
#define MSG_ERR_8   ((byte)0x08)    /* > PMS            buffer overflow (report first part of message in AMS) */

/*
--------------------------------------------------------------------------------
    FPH-FirstByte Definitions
--------------------------------------------------------------------------------
*/
#define FPH_INDEX              3              /* Index to FPH in headers      */

#define FPH_M_FIFONO           ((byte) 0x38)  /* Bit 5-3: FIFO number         */
#define FPH_M_MSGTYPE          ((byte) 0x06)  /* Bit 2-1: Message Type        */
#define FPH_M_SCF0_ENABLED     ((byte) 0x01)  /* Bit 0: SCF0P                 */

#define FPH_B_FIFONO_LSB       3              /* FIFONo Least Significant Bit */

#define FPH_MSGTYPE_CMD        ((byte) 0x00)  /* Command message              */
#define FPH_MSGTYPE_STAT       ((byte) 0x02)  /* Status message               */
#define FPH_MSGTYPE_DATA       ((byte) 0x04)  /* Data message                 */

/*
----------------------------------------------------------------------------
    FIFO numbers
----------------------------------------------------------------------------
*/
#define PMS_FIFONO_MCM                  ((byte) 0x00)
#define PMS_FIFONO_MDP                  ((byte) 0x01)
#define PMS_FIFONO_ICM                  ((byte) 0x02)
#define PMS_FIFONO_ALL                  ((byte) 0x03)
#define PMS_FIFONO_MEP                  ((byte) 0x04)

/*
----------------------------------------------------------------------------
    Maximum packet sizes (to be changed, at least for CTRL)
----------------------------------------------------------------------------
*/
#define PMS_CTRL_MAX_PAYLOAD            45
#define PMS_I2C_DATA_MAX_PAYLOAD        50

#ifdef _OS81110_MDP_MAX_PL
    #define PMS_DATA_MAX_PAYLOAD        1524
#else
    #define PMS_DATA_MAX_PAYLOAD        1014
#endif

#define PMS_ALLOC_MAX_SIZE          64000


/*
--------------------------------------------------------------------------------
    Helpful macros
--------------------------------------------------------------------------------
*/
#define PMS_IS_MEP(buf_ptr) ((PMS_FIFONO_MEP << FPH_B_FIFONO_LSB) == ((buf_ptr)[FPH_INDEX] & FPH_M_FIFONO))

#define PmsPrepareMepHeader(header_ptr, current_message_length)                 PmsPrepareMepHeaderExt(header_ptr, current_message_length, MEP_DEFAULT_RETRY)

/*
--------------------------------------------------------------------------------
    Public functions
--------------------------------------------------------------------------------
*/
#ifdef PMS_0
    /*lint -sem(PmsRx,1p)*/
    void PmsRx(HMBMBUF handle);
#endif

#ifdef PMS_1
    /*lint -sem(PmsTxRelease,1p)*/
    void PmsTxRelease(HMBMBUF handle);
#endif

#ifdef PMS_2
    void PmsSetFifoInterface(byte fifomask, byte iface);
#endif

#ifdef PMS_67
    #define PmsSetFifoInterface(a,b);   /* N/A */
#endif

#ifdef PMS_3
    byte PmsGetFifoInterface(byte fifomask);
#endif

#ifdef PMS_68
    #define PmsGetFifoInterface(a)  PMS_IFACE_GENERAL
#endif

#ifdef PMS_8
    void MbmFree(HMBMBUF handle);
#endif

#ifdef PMS_26
    HMBMBUF PmsGetRxBufMcm(word size);
#endif

#ifdef PMS_27
    HMBMBUF PmsGetRxBufMdp(word size);
#endif

#ifdef PMS_31
    /*lint -sem(PmsGetRxBuf,r_null)*/
    HMBMBUF PmsGetRxBuf(word size);
#endif

#ifdef PMS_41
    void PmsSetExternalBufferAvailable(void);
#endif

#ifdef PMS_44
    void PmsFlushMsgTxBuffer(void);
#endif

#ifdef PMS_45
    /*lint -sem(PmsTxStarted,1p)*/
    void PmsTxStarted(HMBMBUF handle);
#endif

#ifdef PMS_48
    /*lint -sem(PmsPrepareMepHeaderExt,1p)*/
    void PmsPrepareMepHeaderExt(byte *header_ptr, word current_message_length, byte retry);
#endif

#ifdef PMS_49
    void PmsTxReady(void);
#endif

#ifdef NS_PMS_PMHT
    void MhpTxStarted(pcTDataTx tx_ptr);
#endif

#ifdef PMS_71
    byte PmsTriggerMlbAnalyzer(void);
#endif


#endif /* _PMS_PB_H */
