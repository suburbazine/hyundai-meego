/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Public API of the Message Buffer Management (MBM)
File:           mbm_pb.h
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
  * \brief     Public API of the Message Buffer Management (MBM)
  */

#ifndef _MBM_PB_H
#define _MBM_PB_H

#include "mostdef1.h"

/*
==============================================================================
    Public definitions
==============================================================================
*/
#define MBM_TYPE_MASK       ((word)0x000F)    /* bit 0..3 */
#define MBM_TYPE_TX_BIT     ((word)0x0001)
#define MBM_TYPE_FIFO_BIT   ((word)0x0002)    /* FIFO Status/Command */

#define MBM_TYPE_MSG_MASK   ((word)0x0004 | MBM_TYPE_FIFO_BIT)

#define MBM_TYPE_DATA       ((word)0x0000)
#define MBM_TYPE_CTRL       ((word)0x0004)

#define MBM_TYPE_STATUS     ((word)0x0000 | MBM_TYPE_FIFO_BIT)
#define MBM_TYPE_CMD        ((word)0x0004 | MBM_TYPE_FIFO_BIT)

#define MBM_TYPE_SHADOW     ((word)0x0008)    /* This is a shadow of a real message (reference to) */

#define MBM_TYPE_CTRL_TX    (MBM_TYPE_CTRL | MBM_TYPE_TX_BIT)
#define MBM_TYPE_CTRL_RX    MBM_TYPE_CTRL
#define MBM_TYPE_DATA_TX    (MBM_TYPE_DATA | MBM_TYPE_TX_BIT)
#define MBM_TYPE_DATA_RX    MBM_TYPE_DATA
#define MBM_TYPE_CMD_TX     (MBM_TYPE_CMD | MBM_TYPE_TX_BIT)
#define MBM_TYPE_CMD_RX     MBM_TYPE_CMD
#define MBM_TYPE_STATUS_TX  (MBM_TYPE_STATUS | MBM_TYPE_TX_BIT)
#define MBM_TYPE_STATUS_RX  MBM_TYPE_STATUS

/* Status of a buffer */
#define MBM_STAT_LSB            4
#define MBM_STAT_MASK           ((word)((word)0xFFF << MBM_STAT_LSB)) /* Bits 4..11 */
#define MBM_STAT_NONE           ((word)((word)0x000 << MBM_STAT_LSB)) /* Not prepared                 */
#define MBM_STAT_TX_SEG         ((word)((word)0x001 << MBM_STAT_LSB)) /* Message is segmented by PMS  */
#define MBM_STAT_TX_SENT        ((word)((word)0x002 << MBM_STAT_LSB)) /* Message sent (at least once) */
#define MBM_STAT_TX_LAST_SEG    ((word)((word)0x004 << MBM_STAT_LSB)) /* Last segment                 */
#define MBM_STAT_TX_APPEND      ((word)((word)0x008 << MBM_STAT_LSB)) /* Append data to payload       */
#define MBM_STAT_TX_BYPASS      ((word)((word)0x010 << MBM_STAT_LSB)) /* Message bypasses normal Q    */

#define MBM_STAT_RX_GARBAGE_BIT ((word)((word)0x020 << MBM_STAT_LSB))
#define MBM_STAT_RSVD           ((word)((word)0x040 << MBM_STAT_LSB)) /* Static buffer reserved/in use */
#define MBM_STAT_NO_USER_FREE   ((word)((word)0x080 << MBM_STAT_LSB)) /* Do not call the user TX free  */
#define MBM_STAT_USE_BACKUP     ((word)((word)0x100 << MBM_STAT_LSB)) /* "msg_backup" was used for a message conversion */

/*! Bits that are set by higher layers and should be protected from clearing when
  * a message is compressed (TX only). */
#define MBM_STAT_PROTECTED_MASK     (MBM_STAT_TX_APPEND | MBM_STAT_RSVD | MBM_STAT_NO_USER_FREE | MBM_STAT_USE_BACKUP )

/*! All status bits except protected ones */
#define MBM_STAT_NONPROTECTED_MASK  (MBM_STAT_MASK & (word)~MBM_STAT_PROTECTED_MASK)

/*
==============================================================================
    Public macros
==============================================================================
*/

#define MBM_GET_TYPE(h)           ((h)->type & MBM_TYPE_MASK)
#define MBM_GET_CTRL_TX_PTR(h)    (&(h)->msg.msgTx)
#define MBM_GET_CTRL_RX_PTR(h)    (&(h)->msg.msgRx)
#define MBM_GET_DATA_TX_PTR(h)    (&(h)->msg.dataTx)
#define MBM_GET_DATA_RX_PTR(h)    (&(h)->msg.dataRx)
#define MBM_SET_STATUS_CB(h, c)   ((h)->cbTxStatus_fptr = c)
#define MBM_GET_STATUS_CB(h)      ((h)->cbTxStatus_fptr)
#define MBM_SET_STATUS_BITS(h, b)    ((h)->type |= (b))
#define MBM_CLR_STATUS_BITS(h, b)    ((h)->type &= (word)(~(b)))


#define MBM_TYPE_IS_DATA(h)       (MBM_TYPE_DATA == ((h)->type & (MBM_TYPE_MSG_MASK | MBM_TYPE_SHADOW)))
#define MBM_TYPE_IS_CTRL(h)       (MBM_TYPE_CTRL == ((h)->type & (MBM_TYPE_MSG_MASK | MBM_TYPE_SHADOW)))


#if defined(MSG_TX_USER_PAYLOAD) || defined(DATA_TX_USER_PAYLOAD)
    /*! 
      * \brief Marks a buffer so the user tx free callback won't be called for the
      *        payload when the buffer is freed.
      * \param[in]  h   handle to buffer
      */
    #define MBM_NO_USER_TX_FREE(h)  MBM_SET_STATUS_BITS(h,MBM_STAT_NO_USER_FREE)
#else
    #define MBM_NO_USER_TX_FREE(h)
#endif


/*! 
  * \brief  Sets the telid that the message should use
  * \param[in]  m   Pointer to the message
  * \param[in]  id  Segment counter
  */
#define MBM_SET_TELID(m, id)      ((MbmGetHandleByMsgPtr(m))->tel_id = (id))

/*! 
  * \brief  Sets the segment count that should be appended to a segmented control
  *         message.
  * \param[in]  m   Pointer to the control message
  * \param[in]  c   Segment counter
  */
#define MBM_SET_SEGCNT(m, c)              {                                             HMBMBUF _h = MbmGetHandleByMsgPtr(m);     _h->append.segCnt = (c);                  _h->type |= MBM_STAT_TX_APPEND;       }

/*! 
  * \brief  Sets the segment count and the telid that the message should use
  * \param[in]  m   Pointer to the control message
  * \param[in]  id  Segment counter
  * \param[in]  c   Segment counter
  */
#define MBM_SET_CTRLSEG(m, id, c)         {                                             HMBMBUF _h = MbmGetHandleByMsgPtr(m);     _h->tel_id        = (id);                 _h->append.segCnt = (c);                  _h->type |= MBM_STAT_TX_APPEND;       }

#define MBM_SET_ADD_PAYLOAD(m, p_ptr, p_len)  {                                                 HMBMBUF _h = MbmGetHandleByMsgPtr(m);         _h->append.payloadPtr = (p_ptr);              _h->append.payloadLen = (p_len);              _h->type |= MBM_STAT_TX_APPEND;           }

/*
==============================================================================
    Public structures
==============================================================================
*/

#ifdef MBM_MIN

    /*! Fields of a MOST control message */
    typedef struct Most_Header_Type
    {
        word Device_ID; /*!< Target device id  */
        byte FBlock_ID; /*!< Target FBlock id */
        byte Inst_ID;   /*!< Target instance id */
        word Func_ID;   /*!< Target function id */
        byte Operation; /*!< Target operation type */
        word Length;    /*!< Payload length */

    } TMostHeader, *pTMostHeader;

    /*! 
      * \brief      Control TX Message structure used by the Application Message Service (AMS).
      * \remarks    *Data HAS to be the first member for ALL message structures, do NOT change this.
      */
    typedef struct Msg_Tx_Type
    {
        byte *Data;                     /*!< Points to the message payload */
        word Length;                    /*!< Size of the payload in number of bytes */
        word Tgt_Adr;                   /*!< Target address (DeviceID)*/
        word Func_ID;                   /*!< Function id. Address of property or function in the function block */
        byte FBlock_ID;                 /*!< Address of function block */
        byte Inst_ID;                   /*!< Instance id of the function block */
        byte Operation;                 /*!< Id of operation (OpType) */
    #ifdef CTRL_FILTER_ID
        byte Filter_ID;                 /*!< Optional field which can be used by the application to denote information
                                         *   to be evaluated in the filter functions. */
    #endif

    #if (MAX_TX_HANDLE > 0)
        #ifdef MBM_ALIGN_OPT2
        byte* rsvd;
        #endif
        byte TxHandle[MAX_TX_HANDLE];   /*!< Optional transmission handle */
    #endif
        byte MidLevelRetries;           /*!< Number of mid-level retries in case of failed transmission */
        byte LowLevelRetries;           /*!< Number of low-level retries in case of failed transmission */

    } TMsgTx, *pTMsgTx;

    /*!
      * \brief      Control RX Message structure used by the Application Message Service (AMS). 
      * \remarks    *Data HAS to be the first member for ALL message structures, do NOT change this.
      */
    typedef struct Msg_Rx_Type
    {
        byte *Data;                     /*!< Points to the message payload. */
        word Length;                    /*!< Size of the payload in number of bytes */
        word Src_Adr;                   /*!< Source address (DeviceID) */
        word Func_ID;                   /*!< Function id. Address of property or function in the function block */
        byte FBlock_ID;                 /*!< Address of function block */
        byte Inst_ID;                   /*!< Instance id of the function block */
        byte Operation;                 /*!< Id of operation (OpType) */
        byte Rcv_Type;                  /*!< Contains the type of address to which the message was sent */
        byte UsageCnt;                  /*!< Reference counter, the application is not allowed to change this value */
    #ifdef CTRL_FILTER_ID
        byte Filter_ID;                 /*!< Optional field which can be used by the application to denote information
                                         *   to be evaluated in the filter functions. */
    #endif

    #if  (MAX_EXT_DATA > 0)
        #ifdef MBM_ALIGN_OPT2
        byte* rsvd;
        #endif
        byte ExtData[MAX_EXT_DATA];     /*!< Optional data field which can be used by the application to store application
                                         *   specific data in addition to the ordinary payload */
    #endif
    } TMsgRx, *pTMsgRx;

    typedef const TMsgRx* cpTMsgRx;

    /*!
      * \brief      Data TX Packet structure used by the Asynchronous Data Service Wrapper (WADS). 
      * \remarks    *Data HAS to be the first member for ALL message structures, do NOT change this.
      */
    typedef struct Data_Tx_Type
    {
        byte *Data;                         /*!< Points to the message payload. */
        word Length;                        /*!< Size of the payload in number of bytes */
        byte Tgt_Adr_H;                     /*!< High byte of the targed address (DeviceID) */
        byte Tgt_Adr_L;                     /*!< Low byte of the targed address (DeviceID) */
    #ifdef _OS81110_PCK_LLR
        byte Retry;                         /*!< Number of low level retries (only MOST150) */
    #endif

    #if  (MAX_DATA_TX_HANDLE > 0)
        #ifdef MBM_ALIGN_OPT2
        byte* rsvd;
        #endif
        byte TxHandle[MAX_DATA_TX_HANDLE];  /*!< Optional transmission handle */
    #endif
    } TDataTx, *pTDataTx;

    typedef const TDataTx* pcTDataTx;       /*!< Pointer at constant Data TX Packet structure */

    /*!
      * \brief      Data RX Packet structure used by the Asynchronous Data Service Wrapper (WADS). 
      * \remarks    *Data HAS to be the first member for ALL message structures, do NOT change this.
      */
    typedef struct Data_Rx_Type
    {
        byte *Data;                         /*!< Points to the message payload. */
        word Length;                        /*!< Size of the payload in number of bytes */
        byte Rcv_Type;                      /*!< Contains the type of address to which the packet was sent */
        byte Src_Adr_H;                     /*!< High byte of the source address (DeviceID) */
        byte Src_Adr_L;                     /*!< Low byte of the source address (DeviceID) */
    #if (MAX_DATA_EXT_DATA > 0)
        #ifdef MBM_ALIGN_OPT2
        byte* rsvd;
        #endif
        byte ExtData[MAX_DATA_EXT_DATA];    /*!< Optional data field which can be used by the application to store application
                                             *   specific data in addition to the ordinary payload */
    #endif
    } TDataRx, *pTDataRx;

    typedef const TDataRx* pcTDataRx;       /*!< Pointer at constant Data RX Packet structure */

    /*
     * typedefed MBM types makes migration smoother, can define the types
     * and temprary have both.
     */

    typedef struct Msg_Tx_Type  MbmCtrlTx;  /*!< control TX message type */
    typedef struct Msg_Rx_Type  MbmCtrlRx;  /*!< control RX message type */
    typedef struct Data_Tx_Type MbmDataTx;  /*!< data TX packet type */
    typedef struct Data_Rx_Type MbmDataRx;  /*!< data RX packet type */

    /*! 
      * Message union comprising Tx/Rx control and data message structures as 
      * well as a byte field byte which is used by fifo status or command messages.
      * The type of a message handle defines which field has to contains valid data.
      */
    typedef union MbmMsgUnion
    {
    #ifndef PACKET_COMMUNICATION_ONLY
        MbmCtrlTx msgTx;                    /*!< control TX message structure */
        MbmCtrlRx msgRx;                    /*!< control RX message structure */
    #endif
        MbmDataTx dataTx;                   /*!< data TX packet structure */
        MbmDataRx dataRx;                   /*!< data RX packet structure */
    #ifndef PACKET_COMMUNICATION_ONLY
        byte      ctrl;                     /*!< FIFO status/command */
    #endif
    } MbmMsgUnion;

    typedef struct MbmQueue
    {
        struct MbmBuf *next_ptr;            /*!< First node in the queue */
        struct MbmBuf *prev_ptr;            /*!< Last node in the queue */
        word           mutex_id;            /*!< Mutex ID of this queue */
        word           length;              /*!< Length of queue */
    } MbmQueue;

    typedef struct MbmAppPayload
    {
        byte* payloadPtr;           /*!< Pointer to payload */
        word  payloadLen;           /*!< Payload length */
        #ifdef PMS_APPENDABLE_SEG_CNT
        byte  segCnt;               /*!< Segment counter */
        #endif
    } MbmAppPayload;

    /*!
      * The message buffer structure. Pointers to a message structure are commonly known 
      * as message handles.
      */
    typedef struct MbmBuf
    {
        struct MbmBuf    *next_ptr;                     /*!< Next node in queue       */
        struct MbmBuf    *prev_ptr;                     /*!< Previous node in queue   */
        struct MbmQueue  *q_ptr;                        /*!< Queue of this buffer     */
        word              type;                         /*!< Message type and status  */
        byte             *start_ptr;                    /*!< Start of actual buffer   */
        union MbmMsgUnion msg;                          /*!< Actual message structure */
        #ifdef MSG_TX_USER_PAYLOAD_EXT_CB
        union MbmMsgUnion msg_backup;                   /*!< Stores msg before type conversion    */
        #endif

        byte (*cbTxStatus_fptr)(struct MbmBuf *handle, byte status); /*!< Callback for tx status */
        byte             *hdr_ptr;                      /*!< Start of buffer content  */
        byte             *payload_ptr;                  /*!< Start of payload         */
        word              size;                         /*!< Total buffer size        */
        word              hdr_len;                      /*!< Length of headers        */
        word              payload_len;                  /*!< Length of payload        */

        #ifdef PMS_APPENDABLE_PAYLOAD
        MbmAppPayload     append;                       /*!< Appendable payload       */
        #endif

        byte              tel_id;                       /*!< Telegram ID              */

        #ifdef PMS_USE_HANDLE
        byte              handle;                       /*!< Buffer handle            */
        #endif
    } MbmBuf, *HMBMBUF;

    typedef const MbmBuf* HCMBMBUF;                     /*!< Handle of constant MBM buffer */

    /*! Structure used to reference real buffers (used for segmentation) */
    typedef struct MbmShadow
    {
        struct MbmBuf    *next_ptr;                     /*!< Next node in queue       */
        struct MbmBuf    *prev_ptr;                     /*!< Previous node in queue   */
        struct MbmQueue  *q_ptr;                        /*!< Queue of this buffer     */
        word              type;                         /*!< Message type and status  */
        struct MbmBuf    *real_ptr;                     /*!< Referenced real buffer   */
    } MbmShadow;

    #ifdef MBM_USAGE_API
    /*! 
      * \brief      Collects information about the usage of MBM resources.
      * \attention  All members have to be words (see _MbmUpdateUsageValue)
      */
    typedef struct MbmUsage
    {
        word tx;                                /*!< The number of used TX buffers */
        word rx;                                /*!< The number of used RX buffers */
        word num;                               /*!< The sum of tx and rx at a moment */
        word memory;                            /*!< Used memory in units (memory * MBM_MEM_UNIT_SIZE = n bytes) */
        word fragments;                         /*!< Number of fragments in MNS internal memory */
    } MbmUsage;

    /*! 
      * \brief      Details about the fragmentation 
      */
    typedef struct MbmFragmentation
    {
        word tiny;                              /*!< Size < MBM_USAGE_FRAG_SMALL */
        word little;                            /*!< MBM_USAGE_FRAG_SMALL <= size < MBM_USAGE_FRAG_MEDIUM */
        word medium;                            /*!< MBM_USAGE_FRAG_MEDIUM <= size < MBM_USAGE_FRAG_LARGE */
        word large;                             /*!< MBM_USAGE_FRAG_LARGE <= size */
    } MbmFragmentation;
    #endif

#endif /* #ifdef MBM_MIN */

/*
==============================================================================
    Public function prototypes
==============================================================================
*/

#ifdef MBM_24
    /*lint -sem(MbmGetBufData,1p)*/
    byte *MbmGetBufData(HMBMBUF handle);
#endif

#ifdef MBM_25
    /*lint -sem(MbmGetBufLength,1p)*/
    word MbmGetBufLength(HMBMBUF handle);
#endif

#ifdef MBM_37
    void _MbmGetUsage(MbmUsage *usage_ptr);
    void MbmGetUsage(MbmUsage *usage_ptr);
#endif

#ifdef MBM_38
    void _MbmGetUsageTop(MbmUsage *usage_ptr);
    void MbmGetUsageTop(MbmUsage *usage_ptr);
#endif

#ifdef MBM_45
    void _MbmGetUsageTopExt(MbmUsage *usage_ptr);
    void MbmGetUsageTopExt(MbmUsage *usage_ptr);
#endif

#ifdef MBM_39
    void _MbmGetFragmentation(MbmFragmentation *frag_ptr);
    void MbmGetFragmentation(MbmFragmentation *frag_ptr);
#endif

#ifdef MBM_41
    void MbmSetUsageThreshold(const MbmUsage *threshold_ptr);
#endif

#ifdef MBM_44
    HMBMBUF MbmGetHandleByMsgPtr(void *msg);
#endif

#endif /* Header guard */
