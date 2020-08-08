/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Public API of Application Message Service Wrapper (WAMS)
File:           ams_pb.h
Version:        3.0.x-SR-2  
Language:       C
Author(s):      S.Kerber, S.Semmler
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
  * \brief      Public API of Application Message Service Wrapper (WAMS)
  */

#ifndef _AMS_PB_H
#define _AMS_PB_H

#include "mostdef1.h"

/*
==============================================================================
    AMS Service flags (public only for trace purposes)
==============================================================================
*/

#define AMS_P_NONE              ((word) 0x0000)

#define AMS_P_TX                ((word) 0x0001)
#define AMS_P_RX                ((word) 0x0002)
#define AMS_P_GO_PROTECTED      ((word) 0x0004)
#define AMS_P_BUF_FREED         ((word) 0x0008)
#define AMS_P_GO_SEMI_PROTECTED ((word) 0x0010)
#define AMS_P_SHADOW_CHANGE     ((word) 0x0020)
/*
#define AMS_P_FREE7             ((word) 0x0040)
#define AMS_P_FREE8             ((word) 0x0080)
#define AMS_P_FREE9             ((word) 0x0100)
#define AMS_P_FREE10            ((word) 0x0200)
#define AMS_P_FREE11            ((word) 0x0400)
#define AMS_P_FREE12            ((word) 0x0800)
#define AMS_P_FREE13            ((word) 0x1000)
#define AMS_P_FREE14            ((word) 0x2000)
#define AMS_P_FREE15            ((word) 0x4000)
#define AMS_P_FREE16            ((word) 0x8000)
*/
#define AMS_P_FIRST             AMS_P_TX
#define AMS_P_LAST              AMS_P_SHADOW_CHANGE

#define MSG_TX_FILTER_DEFAULT   ((byte) 0x00)   /* normal TX */
#define MSG_TX_FILTER_CANCEL    ((byte) 0x01)   /* cancel TX */
#define MSG_TX_FILTER_BUSY      ((byte) 0x02)   /* re-call filter */

#define MSG_TX_FREE             ((byte)0x00)    /* done, free the tx ptr */
#define MSG_TX_RETRY            ((byte)0x01)    /* retry tx */
#define MSG_TX_POSTPONE         ((byte)0x02)    /* app takes ptr and re-calls */

#define MSG_RX_TAKE             ((byte) 0x01)   /* will call MsgFreeRxMsg() */
#define MSG_RX_BUSY             ((byte) 0x02)   /* currently busy, re-call  */
#define MSG_RX_INC_USAGE_CNT    ((byte) 0x03)   /* increase usage count     */
#define MSG_RX_FREE             ((byte) 0x04)   /* done, free the rx ptr    */

#define MSG_RCV_TYPE_LOGICAL    ((byte) 0x00)   /* aimed to logical node addr */
#define MSG_RCV_TYPE_NODE_POS   ((byte) 0x01)   /* aimed to node pos addr     */
#define MSG_RCV_TYPE_BROADCAST  ((byte) 0x02)   /* received via broadcast     */
#define MSG_RCV_TYPE_GROUPCAST  ((byte) 0x03)   /* received via groupcast     */

#define MSG_SEG_COOPERATIVE     ((byte) 0x01)
#define MSG_SEG_INIC_ONLY       ((byte) 0x02)
#define MSG_SEG_EHC_ONLY        ((byte) 0x03)

/*! \brief      Configuration structure used as a parameter to
  *             MsgSetRetryConfig() and MsgGetRetryConfig().
  */
typedef struct TMsgRetryConfig
{
    /*! \brief The total number of all low-level transmission attempts */
    byte total_attempts;

    /*! \brief The time between two retry attempts. Time unit = 16 MOST
     *         network fames */
    byte time;

} TMsgRetryConfig;


#ifdef AMS_4
    TMsgTx * MsgGetTxPtrExt(word size);
#endif

#ifndef AmsMsgSend 
    #define AmsMsgSend    MsgSend
#endif
#ifdef AMS_5
    void AmsMsgSend(TMsgTx *msg_ptr);
#endif

#ifdef AMS_6
    void MsgSend2(TMsgTx *msg_ptr);
#endif

#if (defined AMS_7) && (defined MNS_AVOID_ATTACH)
/* This function is only intended for debug and test purpose. */
    void MsgSend3(TMsgTx *msg_ptr);
#endif

#ifdef AMS_9
    void MsgTxUnused(TMsgTx *msg_ptr);
#endif

#ifdef AMS_13
    void MsgFreeRxMsg(TMsgRx *msg_ptr);
#endif
#ifdef AMS_14
    TMsgRx * MsgGetRxInPtrExt(word size);
#endif

#ifdef AMS_15
    void MsgRxInReady(TMsgRx *msg_ptr);
#endif

#ifdef AMS_16
    void MsgTxDataByte(byte **tgt_ptr_ptr, byte *src_ptr);
#endif

#ifdef AMS_17
    void MsgTxDataWord(byte **tgt_ptr_ptr, word *src_ptr);
#endif

#ifdef AMS_18
    void MsgTxDataLong(byte **tgt_ptr_ptr, dword *src_ptr);
#endif

#ifdef AMS_19
    void MsgTxBcdToAscII(byte **tgt_ptr_ptr, byte number);
#endif

#ifdef AMS_20
    void MsgRxDataByte(byte *tgt_ptr, byte **src_ptr_ptr);
#endif

#ifdef AMS_21
    void MsgRxDataWord(word *tgt_ptr, byte **src_ptr_ptr);
#endif

#ifdef AMS_22
    void MsgRxDataLong(dword *tgt_ptr, byte **src_ptr_ptr);
#endif

#ifdef AMS_24
    void AddrHSearchStart(TMsgTx *msg_ptr);
#endif

#ifdef AMS_31
    byte MsgGetSegMode(void);
#endif

#ifdef NS_AMS_MSV2
    byte ConfigStateFilterV2(TMsgTx *msg_ptr);
#endif

#ifdef AMS_40
    word MsgGetUsedTxBuffers(void);
#endif

#ifdef AMS_41
    word MsgGetUsedRxBuffers(void);
#endif

#ifdef AMS_43
    byte MsgBcdToStr(byte** tgt_ptr_ptr, byte bcd);
#endif

#ifdef AMS_44
    byte MsgVersionToISO8859(byte* version, byte *string);
#endif

#ifdef AMS_50
    byte MsgGetRetryConfig(TMsgRetryConfig *config_ptr);
#endif

#ifdef AMS_51
    byte MsgSetRetryConfig(TMsgRetryConfig *config_ptr);
#endif

#ifdef AMS_52
    void MsgReleasePayload(TMsgRx *rx_ptr);
#endif


#endif /* _AMS_PB_H */

