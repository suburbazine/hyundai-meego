/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Implementation of Port Message Service Async (PMSA)
File:           pmsa.c
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
  * \brief      Implementation of Port Message Service Async (PMSA)
  */


/*
==============================================================================
    Includes
==============================================================================
*/

#include "mns.h"

#ifndef PACKET_COMMUNICATION_ONLY
extern byte dummy; /* ANSI C requires that something be declared in the compilation unit. */
#else /* #ifndef PACKET_COMMUNICATION_ONLY */

#include "mbm.h"
#include "pms.h"
#include "pms_pv.h"





/*
==============================================================================
    NetServices trace: module trace macros
==============================================================================
*/
/*lint -esym(961,19.7)*/
#if (MNS_DEBUG & NST_C_FUNC_ENTRIES)
    #define T_API_ENTRY(func)   MNS_TRACE((MNS_P_SRV_PMS, NST_E_FUNC_ENTRY_API, 1, func))
    #define T_LIB_ENTRY(func)   MNS_TRACE((MNS_P_SRV_PMS, NST_E_FUNC_ENTRY_LIB, 1, func))
    #define T_MOD_ENTRY(func)   MNS_TRACE((MNS_P_SRV_PMS, NST_E_FUNC_ENTRY_MOD, 1, func))
#else
    #define T_API_ENTRY(func)
    #define T_LIB_ENTRY(func)
    #define T_MOD_ENTRY(func)
#endif

#if (MNS_DEBUG & NST_C_FUNC_EXITS)
    #define T_API_EXIT(func)    MNS_TRACE((MNS_P_SRV_PMS, NST_E_FUNC_EXIT_API, 1, func))
    #define T_LIB_EXIT(func)    MNS_TRACE((MNS_P_SRV_PMS, NST_E_FUNC_EXIT_LIB, 1, func))
    #define T_MOD_EXIT(func)    MNS_TRACE((MNS_P_SRV_PMS, NST_E_FUNC_EXIT_MOD, 1, func))
#else
    #define T_API_EXIT(func)
    #define T_LIB_EXIT(func)
    #define T_MOD_EXIT(func)
#endif

#if (MNS_DEBUG & NST_C_INIT)
    #define T_INIT()            MNS_TRACE((MNS_P_SRV_PMS, NST_E_INIT, 0))
#else
    #define T_INIT()
#endif

#if (MNS_DEBUG & NST_C_SERVICE)
    #define T_SERVICE(event)    MNS_TRACE((MNS_P_SRV_PMS, NST_E_SERVICE, 1, event))
#else
    #define T_SERVICE(event)
#endif

#if (MNS_DEBUG & NST_C_REQUESTS)
    #define T_REQUEST(event)    MNS_TRACE((MNS_P_SRV_PMS, NST_E_REQUEST, 1, event))
#else
    #define T_REQUEST(event)
#endif

#if (MNS_DEBUG & NST_C_ASSERTS)
    #define FAILED_ASSERT()     /*lint -esym(960,10.1)*/MNS_TRACE((MNS_P_SRV_PMS, NST_E_ASSERT, 1, __LINE__))/*lint +esym(960,10.1)*/
    #define ASSERT(exp)         if(!(exp)) FAILED_ASSERT()
#else
    #define FAILED_ASSERT()
    #define ASSERT(exp)
#endif
/*lint +esym(961,19.7)*/



/*
==============================================================================
    Module internal variables
==============================================================================
*/

#ifdef PMS_MIN

static PmsInitStruct *pmsInitData_ptr; /* Callbacks and configuration */

word pmsPending;

static PmsIface generalData;

static PmsFifo  mdpFifo;

static MbmQueue rxQ;

static HMBMBUF  cmdRsvd;                /* Reserved command msg */

static HMBMBUF  cmdFree;                /* Same message when free */

static PmsStruct pms;

#endif /* #ifdef PMS_MIN */


/*
----------------------------------------------------------------------------
    FIFO specific constant configurations
----------------------------------------------------------------------------
*/

/*
==============================================================================
    Implementation of API functions
==============================================================================
*/

/*
--------------------------------------------------------------------------------
 Function:      PmsRx(...)

 Description:   Receives incoming messages of any type from the LLD.

 Input(s):      handle : Received message

 Return(s):     None.
--------------------------------------------------------------------------------
*/
#ifdef PMS_0
void PmsRx(HMBMBUF handle)
{
    word event = PMS_P_RX;

    ASSERT(handle);
    T_API_ENTRY(PMS_0);

    if (NULL != handle)
    {
        MbmEnqueue(&rxQ, handle);
    
        #ifdef PMS_RX_TRIGGERS_TX
        PmsSetPendingEvent((word)(event | PMS_P_TX_TRIG));
        #else
        PmsSetPendingEvent((word)event);
        #endif
    }
    T_API_EXIT(PMS_0);
}
#endif

#ifdef PMS_1
/*!
  * \brief      Called by the LLD when it has completed using a buffer.
  * \details    It can be done as soon as the LLD does not need the buffer anymore. It
                may not reference th buffer after this.
  * \param[in]  handle    Handle to buffer released by LLD
  */
void PmsTxRelease(HMBMBUF handle)
{
    word events;

    T_API_ENTRY(PMS_1);

    events = (word)0;
    ASSERT(handle);

    if (NULL != handle)
    {
        if (MBM_TYPE_FIFO_BIT == (MBM_GET_TYPE(handle) & MBM_TYPE_FIFO_BIT))
        {
            WAIT4MX(MX_PMS_CS);                         /* handle is cmd or status message */
            if (cmdRsvd == handle)
            {
                cmdFree = handle;                       /* this was the Command message */
            }
            generalData.active = NULL;                  /* free interface */
            REL_MX(MX_PMS_CS);

            if ((word)0 < MBM_QUEUE_LENGTH(&mdpFifo.msgQ))
            {
                events |= PMS_P_TX_TRIG;                /* trigger transmission of next message */
            }
        }
        else
        {
            WAIT4MX(MX_PMS_CS);                         /* handle is data packet */
            generalData.active = NULL;                  /* free interface */
            mdpFifo.txPending  = handle;
            events            |= PMS_P_MDP_TXSTATUS;
            REL_MX(MX_PMS_CS);
        }
    }

    if ((word)0 != events)
    {
        PmsSetPendingEvent(events);
    }

    T_API_EXIT(PMS_1);
}
#endif

#ifdef PMS_31
HMBMBUF PmsGetRxBuf(word size)
{
    HMBMBUF handle;
    T_API_ENTRY(PMS_31);

    handle = MbmGetBuf(size);

    if (NULL == handle)
    {
        PmsFireBufFreed();
    }

    T_API_EXIT(PMS_31);
    return( handle );
}
#endif

#ifdef PMS_48
void PmsPrepareMepHeaderExt(byte *header_ptr, word current_message_length, byte retry)
{
    T_API_ENTRY(PMS_48);

    ASSERT(header_ptr);
    if( header_ptr )
    {
        current_message_length += 6; /* Include this header, PML not counted */
        *header_ptr++ = HB(current_message_length); /* PML MSB */
        *header_ptr++ = LB(current_message_length); /* PML LSB */
        *header_ptr++ = (byte)0x05;                 /* PMHL    */
        *header_ptr++ = (PMS_FIFONO_MEP << FPH_B_FIFONO_LSB) | FPH_MSGTYPE_DATA; /* FPH */
                                                    /* High nibble Retry, low Priority */
        *header_ptr++ = ( (byte)((retry & (byte)0x0F) << 4) ) | (PMS_DEF_PRIO & (byte)0x0F);
        *header_ptr++ = (byte)0x00;                 /* stuffing byte 5 */
        *header_ptr++ = (byte)0x00;                 /* stuffing byte 6 */
        *header_ptr   = (byte)0x00;                 /* stuffing byte 7 */
    }

    T_API_EXIT(PMS_48);
}
#endif

/*
==============================================================================
    Implementation of Library functions
==============================================================================
*/
#ifdef PMS_8
void MbmFree(HMBMBUF handle)
{
    ASSERT(handle);
    T_LIB_ENTRY(PMS_8);

    if (NULL != handle)
    {
        MbmFree2(handle);
    
        if ((NULL != pmsInitData_ptr->cbBufFreed_fptr) && (MNS_FALSE != pms.fire_buf_freed))
        {
            pms.fire_buf_freed = MNS_FALSE;
            pmsInitData_ptr->cbBufFreed_fptr();
        }
    }

    T_LIB_EXIT(PMS_8);
}
#endif


#ifdef PMS_4
void PmsInit(PmsInitStruct *is_ptr)
{
    T_LIB_ENTRY(PMS_4);
    T_INIT();

    ASSERT(is_ptr);
    ASSERT(is_ptr->cbMdpRx_fptr);
    ASSERT(is_ptr->dataTx_fptr);
    ASSERT(is_ptr->startIfaces_fptr);

    pmsInitData_ptr = is_ptr;

    WAIT4MX(MX_PMS_PE);
    pmsPending        = PMS_P_NONE;
    REL_MX(MX_PMS_PE);
    pms.ifacesStarted = MNS_FALSE;

    WAIT4MX(MX_PMS_CS);

    cmdFree = MbmAllocate(PMS_CMD_SIZE, (word)0, MBM_TYPE_CMD_TX);  /* init rsvd buffers */
    cmdRsvd = cmdFree;
    ASSERT(cmdRsvd);                                                /* will not work without it */

    PMS_IFACE_INIT(generalData);                                    /* init Interfaces */
    generalData.tx_fptr = is_ptr->dataTx_fptr;
    REL_MX(MX_PMS_CS);

    MbmQueueInit(&rxQ, MX_PMS_Q);

    PmsFifoInit();

    PmsSetPendingEvent(PMS_P_RX_ACK);

    MnsServiceInitComplete(MNS_PHASE_INIT, MNS_P_SRV_PMS);

    T_LIB_EXIT(PMS_4);
}
#endif


#ifdef PMS_5
void PmsService(void)
{
    word event;
    bool check;

    T_LIB_ENTRY(PMS_5);

    WAIT4MX(MX_PMS_PE);
    event = pmsPending;
    pmsPending = PMS_P_NONE;
    REL_MX(MX_PMS_PE);

    T_SERVICE(event);

    if (MNS_FALSE == pms.ifacesStarted)
    {
        pms.ifacesStarted = MNS_TRUE;
        pmsInitData_ptr->startIfaces_fptr();
    }

    if( event & PMS_P_RX )
    {
        PmsProcessRx();
    }

    if( event & PMS_P_TX_TRIG )
    {
        #ifdef PMS_70
        if ((NULL == mdpFifo.txPending) && (((word)0 < MBM_QUEUE_LENGTH(&mdpFifo.msgQ)) || ((word)0 < MBM_QUEUE_LENGTH(&mdpFifo.cmdQ))))
        #else
        if ((NULL == mdpFifo.txPending) && ((word)0 < MBM_QUEUE_LENGTH(&mdpFifo.msgQ)))
        #endif
        {
            PmsFifoTxTrigger();
        }
    }

    if( event & PMS_P_MDP_TXSTATUS )
    {
        PmsFifoProcessStatus(&mdpFifo, PMS_M_STAT_SLOTAV | PMS_XMIT_SUCCESS, NULL);
    }

    WAIT4MX(MX_PMS_PE);
    check = (PMS_P_NONE != pmsPending) ? MNS_TRUE : MNS_FALSE;
    REL_MX(MX_PMS_PE);

    if( MNS_FALSE != check )
    {
        MnsSetPendingService(MNS_P_SRV_PMS);
    }

    T_LIB_EXIT(PMS_5);
}
#endif

#ifdef PMS_6
HMBMBUF PmsGetBuf(word size, word type)
{
    HMBMBUF handle;
    word    reserved;

    T_LIB_ENTRY(PMS_6);

    reserved = ((word)MBM_TYPE_DATA_TX == type) ? (word)PMS_DATA_HDR_MAX_SIZE : (word)0;

    size  += reserved;
    handle = MbmAllocate(size, reserved, type);

    if (NULL != handle)
    {
        PmsFillDefaultHeader(handle);
    }
    else
    {
        PmsFireBufFreed();
    }

    T_LIB_EXIT(PMS_6);
    return( handle );
}
#endif

#ifdef PMS_7
void PmsSend(HMBMBUF handle, PmsTxStatusHandler cbTxStatus_fptr)
{
    bool iface_ready;

    T_LIB_ENTRY(PMS_7);
    handle->cbTxStatus_fptr = cbTxStatus_fptr;

    if (MNS_FALSE != PmsCompressHeader(handle))                  /* Append the headers into the TX buffer */
    {
        MbmEnqueue(&mdpFifo.msgQ, handle);

        WAIT4MX(MX_PMS_CS);
        iface_ready = PMS_IFACE_READY(generalData) ? MNS_TRUE : MNS_FALSE;
        REL_MX(MX_PMS_CS);

        if (MNS_FALSE != iface_ready)
        {
            /*
             * Only necessary to trigger if the interface is ready for
             * another buffer, otherwise it will be retriggered when
             * completed.
             */
            PmsSetPendingEvent(PMS_P_TX_TRIG);
        }
    }
    else
    {
        MbmFree(handle);    /* packet exceeds the maximum length */
        handle = NULL;
    }

    T_LIB_EXIT(PMS_7);
} /*lint !e438 (Last value assigned to variable 'handle' not used)*/
#endif

/*
==============================================================================
    Implementation of Module functions
==============================================================================
*/
#ifdef PMS_9
static void PmsFifoInit(void)
{
    T_MOD_ENTRY(PMS_9);

    MbmQueueInit(&mdpFifo.msgQ, MX_PMS_Q);
    #ifdef PMS_70
    MbmQueueInit(&mdpFifo.cmdQ, MX_PMS_Q);
    #endif
    mdpFifo.txPending = NULL;

    T_MOD_EXIT(PMS_9);
}
#endif

#ifdef PMS_10
void PmsSetPendingEvent(word event)
{
    T_MOD_ENTRY(PMS_10);
    T_REQUEST(event);
    MnsSetPendingEventFlag(event, MX_PMS_PE, &pmsPending, MNS_P_SRV_PMS);
    T_MOD_EXIT(PMS_10);
}
#endif


#ifdef PMS_11
static void PmsFillDefaultHeader(HMBMBUF handle)
{
    #if (MAX_TX_HANDLE > 0) || (MAX_DATA_TX_HANDLE > 0)
    word i = (word)0;
    byte *d_ptr = NULL;
    #endif
    MbmMsgUnion *m_ptr = &handle->msg;

    T_MOD_ENTRY(PMS_11);
    if( MBM_TYPE_DATA_TX == MBM_GET_TYPE(handle) )
    {
        #ifdef _OS81110_PCK_LLR
        m_ptr->dataTx.Retry     = (byte)(MDP_DEFAULT_RETRY);
        #endif
        m_ptr->dataTx.Tgt_Adr_H = PMS_DEF_TGTADDR_0;
        m_ptr->dataTx.Tgt_Adr_L = PMS_DEF_TGTADDR_1;
        m_ptr->dataTx.Length    = (word)0;
        #if MAX_DATA_TX_HANDLE > 0
        d_ptr = &m_ptr->dataTx.TxHandle[0];
        i     = (word)MAX_DATA_TX_HANDLE;
        #endif
    }
    #if (MAX_TX_HANDLE > 0) || (MAX_DATA_TX_HANDLE > 0)
    /* If the actual structure doesn't have a handle i will be 0 */
    if( d_ptr )
    {
        while( i-- )
        {
            *d_ptr++ = (byte)0;
        }
    }
    #endif

    #ifdef PMS_USE_HANDLE
    handle->handle = PMS_DEF_HANDLE;
    #endif
    T_MOD_EXIT(PMS_11);
}
#endif

#ifdef PMS_12
static bool PmsCompressHeader(HMBMBUF handle)
{
    word  len;
    MbmDataTx *dmsg_ptr = MBM_GET_DATA_TX_PTR(handle);
    byte *tgt_ptr;
    bool success;

    T_MOD_ENTRY(PMS_12);
    success = MNS_TRUE;

    /* No SCFs allowed, only default */

    /* length = PML + PMHL + *PMHL + Length = 10 */
    tgt_ptr = MbmPush(handle, (word)PMS_DATA_HDR_MAX_SIZE);

    /*
    ------------------------------------------------------------------------
        Start filling message
    ------------------------------------------------------------------------
    */
    #ifdef PMS_APPENDABLE_PAYLOAD
    if(handle->type & MBM_STAT_TX_APPEND)
    {
            len = (word)((PMS_DATA_HDR_MAX_SIZE-2)
                    + dmsg_ptr->Length                /*   the port message includes */
                    + handle->append.payloadLen);     /* additional external payload */
    }
    else
    #endif
    {
            len = ((word)(PMS_DATA_HDR_MAX_SIZE-2)
                          + dmsg_ptr->Length);    /* Header is static 8   */
    }

    *tgt_ptr++ = HB(len);                         /* PML                  */
    *tgt_ptr++ = LB(len);
    *tgt_ptr++ = (byte)5;                         /* Static PMHL          */

                                                  /* FPH */
    *tgt_ptr++ = (byte)(PMS_FIFONO_MDP << FPH_B_FIFONO_LSB) | FPH_MSGTYPE_DATA;

    #ifdef _OS81110_PCK_LLR
                                                  /* byte Retry;          */
                                                  /* byte Priority;       */
    *tgt_ptr++ = (byte)((dmsg_ptr->Retry & (byte)0x0F) << 4) | (PMS_DEF_PRIO & (byte)0x0F);
    #else
    *tgt_ptr++ = PMS_DEF_PRIO;                    /* byte Priority;       */
    #endif
    *tgt_ptr++ = dmsg_ptr->Tgt_Adr_H;             /* word TgtDeviceID;    */
    *tgt_ptr++ = dmsg_ptr->Tgt_Adr_L;
    *tgt_ptr++ = 0x00;                            /* Static filler        */

    #ifdef PMS_APPENDABLE_PAYLOAD
    if(handle->type & MBM_STAT_TX_APPEND)
    {
        len = dmsg_ptr->Length + handle->append.payloadLen;
    }
    else
    #endif
    {
        len = dmsg_ptr->Length;
    }

    *tgt_ptr++ = HB(len);
    *tgt_ptr++ = LB(len);

    if (len > (word)PMS_DATA_MAX_PAYLOAD)
    {
        success = MNS_FALSE;
        FAILED_ASSERT();
    }

    #ifdef PMS_APPENDABLE_PAYLOAD
    if(handle->type & MBM_STAT_TX_APPEND)
    {
        len = dmsg_ptr->Length;       /* restore the correct user payload length */
                                      /* skip length checks                      */
    }
    #endif

    tgt_ptr    = dmsg_ptr->Data;                         /* Payload              */

    MBM_SET_PAYLOAD_PTR(handle, tgt_ptr);
    MBM_SET_PAYLOAD_LEN(handle, len);
    T_MOD_EXIT(PMS_12);

    return success;
}
#endif


#ifdef PMS_13
static void PmsDecompressHeader(HMBMBUF handle)
{
    MbmMsgUnion *msg_ptr;
    word         len;
    #ifndef PMS_DISABLE_ADS_LEN_CHECKS
    word         pml;
    #endif
    byte        *src_ptr;

    T_MOD_ENTRY(PMS_13);

    msg_ptr = &handle->msg;
    MBM_SET_TYPE(handle, MBM_TYPE_DATA_RX);

    /*
     * Header is currently static
     */
    src_ptr = MbmGetBufData(handle);        /* PML */

    #ifndef PMS_DISABLE_ADS_LEN_CHECKS
    DECODE_WORD(&pml, src_ptr);
    #endif

    src_ptr += 4;                           /* Skip to TgtDevType */

    msg_ptr->dataRx.Rcv_Type  = *src_ptr++;
    msg_ptr->dataRx.Src_Adr_H = *src_ptr++;
    msg_ptr->dataRx.Src_Adr_L = *src_ptr++;

    PMS_SKIP_HEADER(src_ptr, handle);       /* Skips any fillers  */

    DECODE_WORD(&len, src_ptr);             /* Length             */
    src_ptr += (word)2;                     /* Skips length field */
    msg_ptr->dataRx.Length = len;
    msg_ptr->dataRx.Data   = src_ptr;

    MBM_SET_HDR_PTR(handle, src_ptr);
    MBM_SET_HDR_LEN(handle, len);           /* Length of data inside buffer */

    #ifndef PMS_DISABLE_ADS_LEN_CHECKS
    if (pml == (len + (word)8))                   /* plausibility check for a correct MDP message */
    {                                       /* pml must be at least data body length + 8 bytes(PMH+Length) */
        pmsInitData_ptr->cbMdpRx_fptr(handle);
    }
    else
    {
        MbmFree(handle);                    /* discard the message if length checks failed */
        handle = NULL;
        FAILED_ASSERT();
    }
    #else
    pmsInitData_ptr->cbMdpRx_fptr(handle);
    #endif

    PmsSetPendingEvent(PMS_P_RX_ACK);

    T_MOD_EXIT(PMS_13);
} /*lint !e438 (Last value assigned to variable 'handle' not used) */
#endif


#ifdef PMS_17
static void PmsFifoTxTrigger(void)
{
    bool  ready;
    byte* add_ptr;
    word  add_len;

    T_MOD_ENTRY(PMS_17);

    add_ptr = NULL;
    add_len = (word)0;

    WAIT4MX(MX_PMS_CS);
    ready = PMS_IFACE_READY(generalData) ? MNS_TRUE : MNS_FALSE;
    REL_MX(MX_PMS_CS);

    /* First check if the Interface might accept this */
    if( MNS_FALSE != ready )
    {
        HMBMBUF handle = MbmDequeue(&mdpFifo.msgQ);

        #ifdef PMS_70
        if (NULL == handle)
        {
            handle = MbmDequeue(&mdpFifo.cmdQ);
        }
        #endif

        if (NULL != handle)
        {
            /* Check & possibly get extended payload */
            #ifdef PMS_APPENDABLE_PAYLOAD
            if(handle->type & MBM_STAT_TX_APPEND)
            {
                add_len = handle->append.payloadLen;
                add_ptr = handle->append.payloadPtr;
                ASSERT(0 == MbmGetExtPayloadLen(handle)); /* external payload is not allowed */
            }
            else
            #endif
            {
                add_len = MbmGetExtPayloadLen(handle);
                add_ptr = ((word)0 < add_len) ? MBM_GET_PAYLOAD_PTR(handle) : NULL;
            }

            /*
             * Hook this as active, because PmsTxRelease can be called from another
             * thread (or this) when the transmit function has been called.
             */
            generalData.active = handle;
            if( (byte)0 == generalData.tx_fptr((void *)handle, add_ptr, add_len) )
            {
                /*
                 * Unhook this from being the active one since it was hooked before
                 * calling the transmit function (above)
                 */
                generalData.active = NULL;

                #ifdef PMS_TX_FORCE_TRIG
                PmsSetPendingEvent(PMS_P_TX_TRIG);
                #else
                /*
                 * No forced retrigger on LLD busy, this will be retriggered on
                 * completed transmissions (and possibly rx, since I2C is half
                 * duplex)
                 */
                #endif
            }
        }
    }
    T_MOD_EXIT(PMS_17);
}
#endif

#ifdef PMS_18
static void PmsProcessRx(void)
{
    HMBMBUF handle;
    word burst_num;

    T_MOD_ENTRY(PMS_18);

    burst_num = pmsInitData_ptr->rx_burst;

    while( burst_num-- > (word)0 )
    {
        handle = MbmDequeue(&rxQ);
        if( NULL != handle )
        {
            /* Decode header and process message */
            PmsDecompressHeader(handle);
        }
        else
        {
            burst_num = (word)0;
        }
    }

    if( MBM_QUEUE_LENGTH(&rxQ) )
    {
        PmsSetPendingEvent(PMS_P_RX);
    }
    T_MOD_EXIT(PMS_18);
}
#endif


/*
--------------------------------------------------------------------------------
 Function:      PmsFifoProcessStatus(...)

 Description:   Processes a status message for the specific FIFO, might take
                control of the handle if needed for answering, in that case
                handle is set to NULL.

 Input(s):      fifo_ptr : Target FIFO
                status   : Status(/Command) to process
                handle   : Pointer to the handle holding the status. If it is
                           NULL the status has to be either SYNCC or SUCCESS.

 Return(s):     None, might use the handle though and set it to NULL.
--------------------------------------------------------------------------------
*/
#ifdef PMS_19
static void PmsFifoProcessStatus(PmsFifo *fifo_ptr, byte status, HMBMBUF *h_ptr)
{
    HMBMBUF txPending;

    T_MOD_ENTRY(PMS_19);

    (void)fifo_ptr;
    (void)status;
    (void)h_ptr;

    txPending = mdpFifo.txPending;
    mdpFifo.txPending = NULL;

    /*
     * Check if this FIFO has a message that has been transmitted and is
     * pending for its status, i.e. hooked to txPending with status
     * MBM_STAT_PEND (ongoing transmissions has status MBM_STAT_BUSY).
     */
    if( txPending )
    {
        byte action = PMS_RETAIN;
        PmsTxStatusHandler cbTxStatus_fptr = txPending->cbTxStatus_fptr;

        if( cbTxStatus_fptr )
        {
            /*
             * Remove headers, this resets the buffer to "user state" and makes
             * it reusable for transmitting.
             * TODO: Make a general resetting mechanism that will work for
             *       segmentation as well.
             */
            MbmPullHeaders(txPending);
            action = cbTxStatus_fptr(txPending, XMIT_SUCCESS);
        }
        else
        {
            /* No TxStatus function, free the pending message */
            MbmFree(txPending);
            action = PMS_RELEASE;
        }

        if( PMS_RETRY == action )
        {
            /* Cannot get here if there were no txPending */
            PmsSend(txPending, cbTxStatus_fptr);
        }
    }

    if (((word)0 < MBM_QUEUE_LENGTH(&mdpFifo.msgQ)) && PMS_IFACE_READY(generalData))
    {
        /* Prepare next */
        PmsSetPendingEvent(PMS_P_TX_TRIG);
    }

    T_MOD_EXIT(PMS_19);
}
#endif

#ifdef PMS_45
void PmsTxStarted(HMBMBUF handle)
{
    TDataTx *msg_ptr;
    byte     tel_id;

    T_API_ENTRY(PMS_45);

    msg_ptr = NULL;
    tel_id  = PMS_TELID_0;

    ASSERT(handle);
    msg_ptr = MBM_GET_DATA_TX_PTR(handle);
    ASSERT(msg_ptr);


    tel_id  = msg_ptr->Data[4];
    tel_id &= 0xF0;

    if ((PMS_TELID_80 == tel_id) || (PMS_TELID_90 == tel_id))
    {
        MhpTxStarted(msg_ptr);
    }

    T_API_EXIT(PMS_45);
    return;
}
#endif

#ifdef PMS_63
static _INLINE void PmsFireBufFreed(void)
{
    pms.fire_buf_freed = MNS_TRUE;
}
#endif

#ifdef PMS_46
void PmsPrepareReInit(void)
{
    T_LIB_ENTRY(PMS_46);

    /* notify LLD to stop RX/TX */
    if (MNS_FALSE != pms.ifacesStarted)
    {
        pms.ifacesStarted = MNS_FALSE;

        if (pmsInitData_ptr->stopIfaces_fptr)
        {
            pmsInitData_ptr->stopIfaces_fptr();
        }
    }

    T_LIB_EXIT(PMS_46);
}
#endif

#ifdef PMS_69
byte PmsTriggerNAOverMDP(void)
{
    byte result;
    HMBMBUF handle;

    T_API_ENTRY(PMS_69);

    result = ERR_BUFOV;
    handle = NULL;

    WAIT4MX(MX_PMS_CS);
    if (NULL != cmdFree)    /* is reserved buffer available */
    {
        handle  = cmdFree;  /* take reserved buffer */
        cmdFree = NULL;
    }
    REL_MX(MX_PMS_CS);

    if (NULL != handle)
    {
        result  = ERR_NO;
        PmsSendCommand(handle,(byte)0x40);  /* send trigger command */
    }

    T_API_EXIT(PMS_69);

    return result;
}
#endif


#ifdef PMS_70
static void PmsSendCommand(HMBMBUF handle, byte cmd)
{
    byte *tgt_ptr;
    byte fph;
    bool ready;

    T_MOD_ENTRY(PMS_70);

    ready = MNS_FALSE;
    tgt_ptr = MBM_RESET_HDR_PTR(handle);
    fph = (byte)(PMS_FIFONO_MDP << FPH_B_FIFONO_LSB);

    fph |= FPH_MSGTYPE_CMD; /*lint !e835 (Information -- A zero has been given as right argument to operator '|') */

    /*lint -esym(960,17.4) (Elective Note -- Violates MISRA 2004 Required Rule 17.4, pointer arithmetic other than array indexing used) */
    tgt_ptr[0] = (byte)0x00; /* PML(HB) */
    tgt_ptr[1] = (byte)0x03; /* PML(LB) */
    tgt_ptr[2] = (byte)2;    /* PMHL */
    tgt_ptr[3] = fph;
    tgt_ptr[4] = cmd;
    /*lint +esym(960,17.4) (Elective Note -- Violates MISRA 2004 Required Rule 17.4, pointer arithmetic other than array indexing used) */

    MBM_SET_HDR_LEN(handle, (word)5);

    MbmEnqueue(&mdpFifo.cmdQ, handle);

    WAIT4MX(MX_PMS_CS);
    ready = PMS_IFACE_READY(generalData) ? MNS_TRUE : MNS_FALSE;
    REL_MX(MX_PMS_CS);

    if (MNS_FALSE != ready)
    {
        PmsSetPendingEvent(PMS_P_TX_TRIG);  /* process command queue */
    }

    T_MOD_EXIT(PMS_70);
}
#endif

#endif /* #ifndef PACKET_COMMUNICATION_ONLY */
