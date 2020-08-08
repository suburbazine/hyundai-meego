/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Implementation of the Message Buffer Management (MBM)
File:           mbm.c
Version:        3.0.x-SR-2  
Language:       C
Author(s):      R. Lundstrom, T. Jahnke
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
  * \brief      Implementation of the Message Buffer Management (MBM)
  */

/*
==============================================================================
    Includes
==============================================================================
*/

#include "pms.h" /* Size of headers and payload */
#include "mns.h"
#include "mdm.h"
#include "mbm.h"
#include "mbm_pv.h"


#ifdef MBM_MLOG_STORE
    #include "mlog.h"
    #ifdef MLOG_ENABLED
        #define MBM_LOG(data_ptr)   MLogAddLine(MBM_MLOG_STORE, 3, data_ptr)
    #endif
#endif

#ifndef MBM_LOG
    #define MBM_LOG(data_ptr)
#endif


/*
==============================================================================
    NetServices trace: module trace macros
==============================================================================
*/

#if (MNS_DEBUG & NST_C_FUNC_ENTRIES)
    #define T_API_ENTRY(func)   MNS_TRACE((MNS_P_SRV_MBM, NST_E_FUNC_ENTRY_API, 1, func))
    #define T_LIB_ENTRY(func)   MNS_TRACE((MNS_P_SRV_MBM, NST_E_FUNC_ENTRY_LIB, 1, func))
    #define T_MOD_ENTRY(func)   MNS_TRACE((MNS_P_SRV_MBM, NST_E_FUNC_ENTRY_MOD, 1, func))
#else
    #define T_API_ENTRY(func)
    #define T_LIB_ENTRY(func)
    #define T_MOD_ENTRY(func)
#endif

#if (MNS_DEBUG & NST_C_FUNC_EXITS)
    #define T_API_EXIT(func)    MNS_TRACE((MNS_P_SRV_MBM, NST_E_FUNC_EXIT_API, 1, func))
    #define T_LIB_EXIT(func)    MNS_TRACE((MNS_P_SRV_MBM, NST_E_FUNC_EXIT_LIB, 1, func))
    #define T_MOD_EXIT(func)    MNS_TRACE((MNS_P_SRV_MBM, NST_E_FUNC_EXIT_MOD, 1, func))
#else
    #define T_API_EXIT(func)
    #define T_LIB_EXIT(func)
    #define T_MOD_EXIT(func)
#endif

#if (MNS_DEBUG & NST_C_INIT)
    #define T_INIT()            MNS_TRACE((MNS_P_SRV_MBM, NST_E_INIT, 0))
#else
    #define T_INIT()
#endif

#if (MNS_DEBUG & NST_C_BUF_USAGE)
    #define T_ALLOC(h,tx,rx)    MNS_TRACE((MNS_P_SRV_MBM, NST_E_ALLOC_HANDLE, 3, h, tx, rx))
    #define T_FREE(h,tx,rx)     MNS_TRACE((MNS_P_SRV_MBM, NST_E_FREE_HANDLE, 3, h, tx, rx))
    #define T_CONVERT(h,tx,rx)  MNS_TRACE((MNS_P_SRV_MBM, NST_E_CONVERT_HANDLE, 3, h, tx, rx))
#else
    #define T_ALLOC(h,tx,rx)
    #define T_FREE(h,tx,rx)
    #define T_CONVERT(h,tx,rx)
#endif

#if (MNS_DEBUG & NST_C_OPT_INFO)
    #define T_MALLOC_FAIL(size) MNS_TRACE((MNS_P_SRV_MBM, NST_E_MALLOC_FAIL, 1, size))
#else
    #define T_MALLOC_FAIL(size)
#endif

#ifdef _lint
    /*lint -emacro(*,*ASSERT)*/
    #define FAILED_ASSERT()     return
    #define ASSERT(exp)         if(!(exp)) FAILED_ASSERT()
#else
  #if (MNS_DEBUG & NST_C_ASSERTS)
    #define FAILED_ASSERT()     MNS_TRACE((MNS_P_SRV_MBM, NST_E_ASSERT, 1, __LINE__))
    #define ASSERT(exp)         if(!(exp)) FAILED_ASSERT()
  #else
    #define FAILED_ASSERT()
    #define ASSERT(exp)
  #endif
#endif


/*
================================================================================
    Global variables
================================================================================
*/

/*
================================================================================
    Module internal variables
================================================================================
*/
#ifdef MBM_MIN

    static MbmBuf   msgBufArray[MBM_MSG_NUM];   /*!< Array of message buffers */

    static MbmQueue msgFreeQ;                   /*!< Free control buffers */

    static MbmPool  msgPool;                    /*!< Administrates message buffers */

    /*! Initialization structure containing the administrative structures */
    static _CONST MbmPoolInitStruct msgPoolInitData =
    {
        &msgPool,              /*!< MbmPool  *pool_ptr;        */
        &msgFreeQ,             /*!< MbmQueue *q_ptr;           */
        &msgBufArray[0],       /*!< MbmBuf   *firstMbmBuf_ptr; */
        (word)MBM_MSG_NUM,     /*!< word      numDataBufs;     */
        (word)MBM_MSG_RSVD_TX, /*!< word      txRsvd;          */
        (word)MBM_MSG_RSVD_RX  /*!< word      rxRsvd;          */
    };

    /*! Internal memory available for message payload */
    byte mbmMemArray[MBM_MEM_UNIT_SIZE * MBM_MEM_UNIT_NUM] MBM_ALIGN_OPT1;
    /*! Free memory segments, includes 1 head element */
    static MbmMemInfo  memInfoArray[MBM_MEM_MAX_SEG + 1];
    /*! Unused memory segments point to this element */
    static MbmMemInfo  memInfoFree;

    #ifdef MBM_USAGE_API
    /*! The current usage of memory and handles  */
    static MbmUsageCollection mbmUsage;
    #endif

#endif /* #ifdef MBM_MIN */


/*
================================================================================
    API function implementations
================================================================================
*/

#ifdef MBM_24
/*!
  * \brief      Returns a pointer to the data in the buffer
  * \param[in]  handle  Pointer to the memory buffer
  * \details    If the buffer has non-continuous data, the returned pointer
  *             is always to the initial segment. The second, payload,
  *             segement is passed to the LLD by the PMS.
  * \return     Pointer to initial or only data segment
  */
byte *MbmGetBufData(HMBMBUF handle)         /*lint -esym( 818, handle ) no use of HCMBMBUF in API functions*/
{
    ASSERT(handle);
    if(!handle)
    {
        return(NULL);
    }

    T_API_ENTRY(MBM_24);
    T_API_EXIT(MBM_24);
    return( handle->hdr_ptr );
}                                           /*lint +esym( 818, handle ) no use of HCMBMBUF in API functions*/
#endif

#ifdef MBM_25
/*!
  * \brief      Determines the CONTINUOUS length of a buffer
  * \param[in]  handle  Pointer to the memory buffer
  * \details    The length is counted from the start of what MbmGetBufData() 
  *             returns to the end of continuous data.
  * \return     The CONTINUOUS length of the buffer
  */
word MbmGetBufLength(HMBMBUF handle)    /*lint -esym( 818, handle ) no use of HCMBMBUF in API functions*/
{
    word len;

    ASSERT(handle);

    if(!handle)
    {
        return (word)0;
    }

    len = handle->hdr_len;

    T_API_ENTRY(MBM_25);
    /* Add length of payload if it's continuous with headers */
    if( (handle->hdr_ptr + handle->hdr_len) == handle->payload_ptr )
    {
        len += handle->payload_len;
    }
    T_API_EXIT(MBM_25);
    return( len );
}                                       /*lint +esym( 818, handle ) no use of HCMBMBUF in API functions*/
#endif

#ifdef MBM_29
HMBMBUF MbmGetBuf(word size)
{
    HMBMBUF handle;
    T_LIB_ENTRY(MBM_29);
    if( size < MBM_RX_MIN_SIZE )
    {
        size = MBM_RX_MIN_SIZE;
    }
    handle = MbmAllocate(size, (word)0, (word)0);
    T_LIB_EXIT(MBM_29);
    return( handle );
}
#endif



/*
================================================================================
    Library function implementations
================================================================================
*/

#ifdef MBM_0
void MbmInit(void)
{
    T_LIB_ENTRY(MBM_0);

    T_INIT();

    WAIT4MX(MX_MBM_CS);
    #ifdef MBM_28
    _MbmMemInit();
    #endif
    _MbmInitPool(&msgPoolInitData);

    _MBM_USAGE_INIT();

    REL_MX(MX_MBM_CS);

    #ifdef MBM_USAGE_API
    MBM_USAGE_INIT_CALLBACK(&mbmUsage.threshold);
    #endif

    T_LIB_EXIT(MBM_0);
}
#endif

#ifdef MBM_1
HMBMBUF MbmAllocate(word size, word reserved, word type)
{
    MbmBuf  *alloc_ptr = NULL;
    word      rsvd;            /* Reserved buffers                       */
    word      used;            /* Used buffers of possibly reserved kind */
    word     *cnt_ptr;         /* Pointer to allocation counter          */

    T_LIB_ENTRY(MBM_1);

    WAIT4MX(MX_MBM_CS);
    if( type & MBM_TYPE_TX_BIT )
    {
        rsvd    = msgPool.rxRsvd;
        used    = msgPool.rxAlloc;
        cnt_ptr = &msgPool.txAlloc;
    }
    else
    {
        rsvd    = msgPool.txRsvd;
        used    = msgPool.txAlloc;
        cnt_ptr = &msgPool.rxAlloc;
    }

    /* How many buffers needs to stay reserved? */
    if( used < rsvd )
    {
        rsvd -= used;
    }
    else
    {
        rsvd = (word)0;
    }

    /* Check if any unreserved buffer is available */
    if( MBM_QUEUE_LENGTH(msgPool.q_ptr) > rsvd )
    {
        alloc_ptr = _MbmDequeue(msgPool.q_ptr);
        if( alloc_ptr )
        {
            word needed_size = size;

            alloc_ptr->start_ptr = _MbmMemAlloc(&size);

            if( alloc_ptr->start_ptr || !needed_size )
            {
                /*
                 * hdr_ptr, hdr_len, payload_ptr & payload_len is set in
                 * MbmReserve() (or subfunctions).
                 */
                alloc_ptr->size = size;
                alloc_ptr->type = type;

                #ifndef PACKET_COMMUNICATION_ONLY
                alloc_ptr->msg.msgRx.UsageCnt = (byte)0;
                #endif

                MbmReserve(alloc_ptr, reserved);
                (*cnt_ptr)++;

                #ifdef MBM_USAGE_API
                if( &msgPool.txAlloc == cnt_ptr )
                {
                    _MBM_USAGE_UPDATE_TX();
                }
                else
                {
                    _MBM_USAGE_UPDATE_RX();
                }
                _MBM_USAGE_UPDATE_NUM();
                #endif

                T_ALLOC((int)alloc_ptr, (int)msgPool.txAlloc, (int)msgPool.rxAlloc);
            }
            else
            {
                T_MALLOC_FAIL(size);
                _MbmEnqueue(msgPool.q_ptr, alloc_ptr);
                alloc_ptr = NULL;
            }
        }
    }
    REL_MX(MX_MBM_CS);

    #ifdef MBM_43
    if(NULL == alloc_ptr)                               /* No free buffer available ? */
    {
        MbmSendMdmMbmBufferAllocationFailed(type);      /* Send debug message */
    }
    #endif

    T_LIB_EXIT(MBM_1);
    return( alloc_ptr );
}
#endif

#ifdef MBM_2
void MbmFree2(HMBMBUF handle)
{
    T_API_ENTRY(MBM_2);

    ASSERT((NULL != handle) && (NULL == handle->q_ptr));

    if(MNS_FALSE != MbmIsFromPool(handle))
    {
        MbmMemFree(handle);

        WAIT4MX(MX_MBM_CS);
        _MbmEnqueue(msgPool.q_ptr, handle);
        if( MBM_GET_TYPE(handle) & MBM_TYPE_TX_BIT )
        {
            msgPool.txAlloc--;
            _MBM_USAGE_UPDATE_TX();
        }
        else
        {
            msgPool.rxAlloc--;
            _MBM_USAGE_UPDATE_RX();
        }
        _MBM_USAGE_UPDATE_NUM();
        T_FREE((int)handle, (int)msgPool.txAlloc, (int)msgPool.rxAlloc);
        REL_MX(MX_MBM_CS);
    }
    else
    {
        /* Statically allocated message, just clear RSVD bit to mark it available */
        MBM_CLR_STATUS_BITS(handle, MBM_STAT_RSVD);
    }

    T_API_EXIT(MBM_2);
}
#endif

#ifdef MBM_3
void MbmQueueInit(MbmQueue *q_ptr, word mutex_id)
{
    T_LIB_ENTRY(MBM_3);

    ASSERT(q_ptr);

    /* initialize the list with no elements */
    q_ptr->next_ptr = (struct MbmBuf *)q_ptr;
    q_ptr->prev_ptr = (struct MbmBuf *)q_ptr;
    q_ptr->length   = (word)0;
    q_ptr->mutex_id = mutex_id;

    T_LIB_EXIT(MBM_3);
}
#endif

#ifdef MBM_4
void MbmEnqueue(MbmQueue *q_ptr, HMBMBUF handle)
{
    T_LIB_ENTRY(MBM_4);

    ASSERT(q_ptr);

    WAIT4MX(q_ptr->mutex_id);
    ASSERT((NULL != handle) && (NULL == handle->q_ptr));
    _MbmEnqueue(q_ptr, handle);
    REL_MX(q_ptr->mutex_id);

    T_LIB_EXIT(MBM_4);
}
#endif

#ifdef MBM_5
void _MbmEnqueue(MbmQueue *q_ptr, HMBMBUF handle)
{
    T_LIB_ENTRY(MBM_5);

    ASSERT(q_ptr);

    /* Fix handle */
    handle->prev_ptr = q_ptr->prev_ptr;
    handle->next_ptr = (MbmBuf *)q_ptr;

    /* Fix node in front of handle (previously last) */
    handle->prev_ptr->next_ptr = handle;

    /* Fix q_ptr */
    q_ptr->prev_ptr = handle;

    handle->q_ptr = q_ptr;
    q_ptr->length++;
    T_LIB_EXIT(MBM_5);
}
#endif

#ifdef MBM_6
void MbmEnqueueFirst(MbmQueue *q_ptr, HMBMBUF handle)
{
    T_LIB_ENTRY(MBM_6);

    ASSERT(q_ptr);

    WAIT4MX(q_ptr->mutex_id);
    ASSERT((NULL != handle) && (NULL == handle->q_ptr));
    _MbmEnqueueFirst(q_ptr, handle);
    REL_MX(q_ptr->mutex_id);

    T_LIB_EXIT(MBM_6);
}
#endif

#ifdef MBM_7
void _MbmEnqueueFirst(MbmQueue *q_ptr, HMBMBUF handle)
{
    T_LIB_ENTRY(MBM_7);

    ASSERT(q_ptr);

    /* Fix handle */
    handle->prev_ptr = (MbmBuf *)q_ptr;
    handle->next_ptr = q_ptr->next_ptr;

    /* Fix node behind handle (previously behind q_ptr) */
    handle->next_ptr->prev_ptr = handle;

    /* Fix q_ptr */
    q_ptr->next_ptr = handle;

    handle->q_ptr = q_ptr;
    q_ptr->length++;
    T_LIB_EXIT(MBM_7);
}
#endif

#ifdef MBM_8
HMBMBUF MbmDequeue(MbmQueue *q_ptr)
{
    HMBMBUF handle;
    T_LIB_ENTRY(MBM_8);

    ASSERT(q_ptr);

    WAIT4MX(q_ptr->mutex_id);
    handle = _MbmDequeue(q_ptr);
    REL_MX(q_ptr->mutex_id);

    T_LIB_EXIT(MBM_8);
    return( handle );
}
#endif

#ifdef MBM_9
HMBMBUF _MbmDequeue(MbmQueue *q_ptr)        /*lint -esym( 818, q_ptr ) queue may be modified in _MbmUnlink*/
{
    HMBMBUF handle;

    T_LIB_ENTRY(MBM_9);

    ASSERT(q_ptr);

    /* Make pointer check, safer than checking count (in case someone has
     * linked more than length can store) */
    if( q_ptr->next_ptr != (struct MbmBuf *)q_ptr )
    {
        /* Get handle to first buffer */
        handle = q_ptr->next_ptr;
        _MbmUnlink(handle);
    }
    else
    {
        handle = NULL;    /* List was empty */
    }
    T_LIB_EXIT(MBM_9);
    return( handle );
}                                           /*lint +esym( 818, q_ptr ) queue may be modified in _MbmUnlink*/
#endif

#ifdef MBM_11
HMBMBUF MbmDequeueLast(MbmQueue *q_ptr)     /*lint -esym( 818, q_ptr ) queue may be modified in _MbmUnlink*/
{
    HMBMBUF handle;

    T_LIB_ENTRY(MBM_11);
    ASSERT(q_ptr);

    WAIT4MX(q_ptr->mutex_id);
    if( q_ptr->prev_ptr != (MbmBuf *)q_ptr )
    {
        handle = q_ptr->prev_ptr;
        _MbmUnlink(handle);
    }
    else
    {
        handle = NULL;
    }
    REL_MX(q_ptr->mutex_id);
    T_LIB_EXIT(MBM_11);

    return( handle );
}                                           /*lint +esym( 818, q_ptr ) queue may be modified in _MbmUnlink*/
#endif

#ifdef MBM_12
HMBMBUF _MbmPeek(const MbmQueue *q_ptr)
{
    HMBMBUF handle;

    T_LIB_ENTRY(MBM_12);

    ASSERT(q_ptr);

    /* Make pointer check, safer than checking count (in case someone has
     * linked more than what length can store) */
    if( q_ptr->next_ptr != (struct MbmBuf *)q_ptr )
    {
        /* Get handle to first buffer */
        handle = q_ptr->next_ptr;
    }
    else
    {
        handle = NULL;    /* List was empty */
    }
    T_LIB_EXIT(MBM_12);
    return( handle );
}
#endif

#ifdef MBM_13
void _MbmUnlink(HMBMBUF handle)
{
    T_MOD_ENTRY(MBM_13);

    /* Not allowed to unlink an unlinked buffer */
    ASSERT((NULL != handle) && (NULL != handle->q_ptr));

    /* Simply cross-link next and previous */
    handle->prev_ptr->next_ptr = handle->next_ptr;
    handle->next_ptr->prev_ptr = handle->prev_ptr;
    handle->q_ptr->length--;
    handle->next_ptr = handle->prev_ptr = NULL;
    handle->q_ptr    = NULL;
    T_MOD_EXIT(MBM_13);
}
#endif

#ifdef MBM_14
void MbmReserve(HMBMBUF handle, word size)
{
    MbmBuf *mb_ptr = handle;
    T_LIB_ENTRY(MBM_14);
    mb_ptr->hdr_ptr = mb_ptr->start_ptr + size;
    mb_ptr->hdr_len = (word)0;

    /**
     * hdr_ptr can be at start_ptr + size, this is a buffer with only reserved
     * space
     */
    ASSERT(mb_ptr->hdr_ptr <= (mb_ptr->start_ptr + mb_ptr->size));

    /*
     * byte *Data is the first member of each message structure, hence any union
     * can be used. (Size optimization).
     * This may very well overwrite a CMD or STATUS, but the structure can not
     * be in use yet, so it doesn't matter.
     * If there is no payload space left after reserving, set *Data to the start
     * of the memory array so a valid pointer always will be passed.
     */
    mb_ptr->msg.dataTx.Data = (mb_ptr->hdr_ptr < (mb_ptr->start_ptr + mb_ptr->size))
                              ? mb_ptr->hdr_ptr : &mbmMemArray[0];
    mb_ptr->payload_ptr = mb_ptr->hdr_ptr;
    mb_ptr->payload_len = (word)0;

    #ifdef PMS_APPENDABLE_PAYLOAD
    mb_ptr->append.payloadPtr = NULL;
    mb_ptr->append.payloadLen = (word)0;
    #ifdef PMS_APPENDABLE_SEG_CNT
    mb_ptr->append.segCnt = (byte)0;
    #endif
    #endif

    T_LIB_EXIT(MBM_14);
}
#endif

#ifdef MBM_15
byte *MbmPush(HMBMBUF handle, word size)
{
    T_LIB_ENTRY(MBM_15);
    handle->hdr_len += size;
    handle->hdr_ptr -= size;
    ASSERT(handle->hdr_ptr >= handle->start_ptr);
    T_LIB_EXIT(MBM_15);
    return( handle->hdr_ptr );
}
#endif

#ifdef MBM_16
bool MbmChangeType(HMBMBUF handle, word newType)
{
    word oldType;
    bool retval;
    word rsvd;
    word used;

    T_LIB_ENTRY(MBM_16);

    ASSERT(handle && (MNS_FALSE != MbmIsFromPool(handle)));
    ASSERT((word)0 == (newType & (word)~MBM_TYPE_MASK)); /* parameter newtype must not contain status bits */

    oldType = MBM_GET_TYPE(handle);

    MBM_SET_TYPE(handle, newType);
    MBM_CLR_STATUS_BITS(handle, MBM_STAT_NONPROTECTED_MASK);

    handle->tel_id = (byte)0;

    if( (MBM_TYPE_CTRL_TX == oldType) && (MBM_TYPE_CTRL_RX == newType) )
    {
        MbmCtrlTx  tx     = *MBM_GET_CTRL_TX_PTR(handle); /* Copy current structure */
        MbmCtrlRx *rx_ptr = MBM_GET_CTRL_RX_PTR(handle);
        rx_ptr->Src_Adr   = tx.Tgt_Adr;
        rx_ptr->FBlock_ID = tx.FBlock_ID;
        rx_ptr->Inst_ID   = tx.Inst_ID;
        rx_ptr->Func_ID   = tx.Func_ID;
        rx_ptr->Operation = tx.Operation;
        rx_ptr->Length    = tx.Length;
        rx_ptr->Data      = tx.Data;
        rx_ptr->Rcv_Type  = PMS_DEF_RCVTYPE;
        rx_ptr->UsageCnt  = (byte)0;
        #ifdef CTRL_FILTER_ID
        rx_ptr->Filter_ID = tx.Filter_ID;
        #endif

        if( NULL == rx_ptr->Data )
        {
            /* Always a valid pointer for RX messages */
            rx_ptr->Data = &mbmMemArray[0];
        }
    }
    else if( (MBM_TYPE_CTRL_RX == oldType) && (MBM_TYPE_CTRL_TX == newType) )
    {
        MbmCtrlRx  rx     = *MBM_GET_CTRL_RX_PTR(handle);
        MbmCtrlTx *tx_ptr = MBM_GET_CTRL_TX_PTR(handle);
        tx_ptr->Tgt_Adr   = rx.Src_Adr;
        tx_ptr->FBlock_ID = rx.FBlock_ID;
        tx_ptr->Inst_ID   = rx.Inst_ID;
        tx_ptr->Func_ID   = rx.Func_ID;
        tx_ptr->Operation = rx.Operation;
        PmsInsertRetryValues(tx_ptr);

        /*
        */
        MbmReserve(handle, (word)PMS_CTRL_HDR_MAX_SIZE);
        tx_ptr->Length = (word)(handle->size - (tx_ptr->Data - handle->start_ptr));
    }

    WAIT4MX(MX_MBM_CS);
    if( (oldType ^ newType) & MBM_TYPE_TX_BIT ) /* Did the RX/TX type change? */
    {
        if( MBM_TYPE_TX_BIT & newType )         /* RX => TX? */
        {
            msgPool.rxAlloc--;
            msgPool.txAlloc++;
        }
        else                                    /* TX => RX? */
        {
            msgPool.txAlloc--;
            msgPool.rxAlloc++;
        }
    }

    /* Check if the conversion respects the reserved TX/RX */
    if( MBM_TYPE_TX_BIT & newType )
    {
        rsvd = msgPool.rxRsvd;
        used = msgPool.rxAlloc;
    }
    else
    {
        rsvd = msgPool.txRsvd;
        used = msgPool.txAlloc;
    }

    if( used < rsvd )
    {
        rsvd -= used;
    }
    else
    {
        rsvd = (word)0;
    }

    /*
     * Add 1 to the free Q, which "emulates" that the buffer passed through it
     * before checking. We want to check if this would have been granted by a
     * normal allocate.
     */
    if( (MBM_QUEUE_LENGTH(msgPool.q_ptr) + (word)1) > rsvd )
    {
        retval = MNS_TRUE;   /* OK, the reserved were respected */
    }
    else
    {
        retval = MNS_FALSE; /* The conversion did not respect reservation */
    }

    _MBM_USAGE_UPDATE_TX();
    _MBM_USAGE_UPDATE_RX();
    _MBM_USAGE_UPDATE_NUM();
    T_CONVERT((int)handle, (int)msgPool.txAlloc, (int)msgPool.rxAlloc);
    REL_MX(MX_MBM_CS);
    T_LIB_EXIT(MBM_16);

    return( retval );
}
#endif


#ifdef MBM_17
void MbmPullHeaders(HMBMBUF handle)
{
    MbmBuf *mbuf_ptr = handle;

    T_LIB_ENTRY(MBM_17);
    mbuf_ptr->hdr_ptr += mbuf_ptr->hdr_len;
    mbuf_ptr->hdr_len  = (word)0;
    T_LIB_EXIT(MBM_17);
}
#endif

#ifdef MBM_19
word MbmGetExtPayloadLen(HMBMBUF handle)
{
    MbmBuf *mb_ptr = handle;

    T_LIB_ENTRY(MBM_19);
    T_LIB_EXIT(MBM_19);

    return( (mb_ptr->payload_ptr != (mb_ptr->hdr_ptr + mb_ptr->hdr_len)) ?
             mb_ptr->payload_len : (word)0 );
}
#endif

#ifdef MBM_23
void MbmFlush(MbmQueue *q_ptr)
{
    HMBMBUF handle;
    T_LIB_ENTRY(MBM_23);

    ASSERT(q_ptr);

    do
    {
        handle = MbmDequeue(q_ptr);
        if( NULL != handle )
        {
            MbmFree(handle);
        }
    } while( NULL != handle );

    T_LIB_EXIT(MBM_23);
}
#endif

/*
==============================================================================
    Module internal implementations
==============================================================================
*/

#ifdef MBM_46
static void MbmMdmTimerCb(word event_flag)
{
    T_MOD_ENTRY(MBM_46);

    WAIT4MX(MX_MBM_CS);
    if(MBM_MDM_TX == event_flag)
    {
        msgPool.mdmTxLocked = MNS_FALSE;
    }
    else
    {
        msgPool.mdmRxLocked = MNS_FALSE;
    }
    REL_MX(MX_MBM_CS);

    T_MOD_EXIT(MBM_46);
}
#endif

#ifdef MBM_20
static void _MbmInitPool(const MbmPoolInitStruct *is_ptr)
{
    word    num;
    MbmBuf *mbuf_ptr;

    ASSERT(is_ptr);

    num      = is_ptr->numDataBufs;
    mbuf_ptr = is_ptr->firstMbmBuf_ptr;

    T_MOD_ENTRY(MBM_20);
    MbmQueueInit(is_ptr->q_ptr, (word)MX_MBM_CS);

    is_ptr->pool_ptr->q_ptr   = is_ptr->q_ptr;
    is_ptr->pool_ptr->txRsvd  = is_ptr->txRsvd;
    is_ptr->pool_ptr->rxRsvd  = is_ptr->rxRsvd;
    is_ptr->pool_ptr->txAlloc = (word)0;
    is_ptr->pool_ptr->rxAlloc = (word)0;

    #ifdef MDM_MBM_BUFFER_ALLOCATION_FAILED
    is_ptr->pool_ptr->mdmTxLocked = MNS_FALSE;
    is_ptr->pool_ptr->mdmRxLocked = MNS_FALSE;
    MostRegisterTimer(&(is_ptr->pool_ptr->mdmTxTimeout), MbmMdmTimerCb, MBM_MDM_TX);
    MostRegisterTimer(&(is_ptr->pool_ptr->mdmRxTimeout), MbmMdmTimerCb, MBM_MDM_RX);
    #endif

    while( num-- )
    {
        mbuf_ptr->start_ptr   = NULL;
        mbuf_ptr->hdr_ptr     = NULL;
        mbuf_ptr->payload_ptr = NULL;

        /* Enqueue and go to next */
        _MbmEnqueue(is_ptr->q_ptr, mbuf_ptr++);
    }
    T_MOD_EXIT(MBM_20);
}
#endif

#ifdef MBM_26
static byte *_MbmMemAlloc(word *size)
{
    byte *mem_ptr;

    T_MOD_ENTRY(MBM_26);

    if( *size )
    {
        MbmMemInfoIndex i;
        MbmMemInfoIndex last;
        word            units;

        units = (word)MBM_MEM_UNITS(*size);
        last  = (MbmMemInfoIndex)0;      /* If first == match, then last == head  */
        i     = memInfoArray[0].next;    /* Get first available segment from head */

        /* If i == 0 it's the Head and no match was found */
        while( i && (memInfoArray[i].size < units) )
        {
            last = i;
            i    = memInfoArray[i].next;
        }

        if( i )
        {
            mem_ptr = &mbmMemArray[MBM_MEM_BYTES(memInfoArray[i].start)];

            memInfoArray[i].size  -= units;
            memInfoArray[i].start += units;

            if( !memInfoArray[i].size )
            {
                /* Remove whole segment */
                MBM_MEM_UNLINK(memInfoArray[last], memInfoArray, i);
                MBM_MEM_LINK(memInfoFree, memInfoArray, i);
                _MBM_USAGE_FRAG_DEC();
            }
            *size = (word)MBM_MEM_BYTES(units);        /* Set real allocated size */

            _MBM_USAGE_MEM_ALLOC(units);
        }
        else
        {
            mem_ptr = NULL;                            /* No free segment         */
        }
    }
    else
    {
        mem_ptr = NULL;
    }
    T_MOD_EXIT(MBM_26);

    return( mem_ptr );
}
#endif

#ifdef MBM_27
void MbmMemFree(MbmBuf *mb_ptr)
{
    T_LIB_ENTRY(MBM_27);

    ASSERT(mb_ptr);

    if( mb_ptr->start_ptr && mb_ptr->size )
    {
        word units = (word)MBM_MEM_UNITS(mb_ptr->size);
        word start = (word)MBM_MEM_UNITS(mb_ptr->start_ptr - &mbmMemArray[0]);
        word end   = (word)(start + units);
        MbmMemInfoIndex i;
        MbmMemInfoIndex j = (MbmMemInfoIndex)0;

        WAIT4MX(MX_MBM_CS);

        i = memInfoArray[0].next;
        /* If i == 0 it's the Head and no match was found */
        while( i && (start > (memInfoArray[i].start + memInfoArray[i].size)) )
        {
            j = i; /* Preceding if a new segment shall be added */
            i = memInfoArray[i].next;
        }

        if( i && (start == (memInfoArray[i].start + memInfoArray[i].size)) )
        {
            /* Append freed segment to the end of the found one */
            memInfoArray[i].size += units;

            /* Check if the segment has grown to join the next one */
            j = memInfoArray[i].next;
            if( j && (end == memInfoArray[j].start) )
            {
                /* Yes, join the next one and free the info structure */
                memInfoArray[i].size += memInfoArray[j].size;
                MBM_MEM_UNLINK(memInfoArray[i], memInfoArray, j);
                MBM_MEM_LINK(memInfoFree, memInfoArray, j);
                _MBM_USAGE_FRAG_DEC();
            }
        }
        else if( i && (end == memInfoArray[i].start) )
        {
            /* Append freed segment to the front of the found one */
            memInfoArray[i].size  += units;
            memInfoArray[i].start -= units;
        }
        else
        {
            /* TODO: Remove assert when through testing */
            ASSERT(memInfoFree.next);
            /*
             * If the assert fails there is an error in calculating the number
             * of needed segments (unless there is some other serious error in
             * the system that has caused this)
             */

            /*
             * Get a new segment and link it in front of where the search ended.
             */
            i = memInfoFree.next;
            MBM_MEM_UNLINK(memInfoFree, memInfoArray, i);
            memInfoArray[i].start = start;
            memInfoArray[i].size  = units;

            /*
             * Link this behind 'j', because it is either the head or the
             * segment closest in front of this one
             */
            MBM_MEM_LINK(memInfoArray[j], memInfoArray, i);
            _MBM_USAGE_FRAG_INC();
        }
        _MBM_USAGE_MEM_FREE(units);
        REL_MX(MX_MBM_CS);

        mb_ptr->start_ptr = mb_ptr->hdr_ptr = mb_ptr->payload_ptr = NULL;
        mb_ptr->size      = mb_ptr->hdr_len = mb_ptr->payload_len = (word)0;
    }
    T_LIB_EXIT(MBM_27);
}
#endif

#ifdef MBM_28
static void _MbmMemInit(void)
{
    MbmMemInfoIndex i;

    T_MOD_ENTRY(MBM_28);
    /*
     * Note that the head is the element with index zero and thus not a valid
     * index, this is used to mark the end of the list.
     */
    memInfoArray[0].next = (MbmMemInfoIndex)1;          /* First free segment */
    memInfoArray[0].start = (word)0;
    memInfoArray[0].size = (word)0;                     /* Head has no size   */
    memInfoArray[1].next = (MbmMemInfoIndex)0;          /* Only 1 large seg   */
    memInfoArray[1].start = (word)0;
    memInfoArray[1].size = MBM_CFG_UNIT_NUM;

    /* Create the list of free memory information structures */
    memInfoFree.next = (MbmMemInfoIndex)0;
    memInfoFree.size = (word)0;
    /**
     * Index 0 is head of segment list. Index 1 is the first free segment (all)
     */
    i = (MbmMemInfoIndex)2;
    while( i < (MbmMemInfoIndex)(MBM_MEM_MAX_SEG + 1) )
    {
        MBM_MEM_LINK(memInfoFree, memInfoArray, i);
        i++;
    }
    T_MOD_EXIT(MBM_28);
}
#endif

#ifdef MBM_31
void MbmEnqueueBehind(HMBMBUF hPrev, HMBMBUF hNew)
{
    T_LIB_ENTRY(MBM_31);
    WAIT4MX(hPrev->q_ptr->mutex_id);
    _MbmEnqueueBehind(hPrev->q_ptr, hPrev, hNew);
    REL_MX(hPrev->q_ptr->mutex_id);
    T_LIB_EXIT(MBM_31);
}
#endif

#ifdef MBM_33
void _MbmEnqueueBehind(MbmQueue *q_ptr, HMBMBUF hPrev, HMBMBUF hNew)
{
    T_LIB_ENTRY(MBM_33);
    ASSERT((NULL != q_ptr) && (NULL == hNew->q_ptr));
    hNew->q_ptr              = q_ptr;
    q_ptr->length++;
    hNew->next_ptr           = hPrev->next_ptr;
    hNew->prev_ptr           = hPrev;
    hNew->next_ptr->prev_ptr = hNew;
    hPrev->next_ptr          = hNew;
    T_LIB_EXIT(MBM_33);
}
#endif

#ifdef MBM_30
bool MbmIsFromPool(HCMBMBUF handle)
{
    bool retval;

    T_LIB_ENTRY(MBM_30);

    if((handle >= &msgBufArray[0]) && (handle <= &msgBufArray[MBM_MSG_NUM-1]))
    {
        retval = MNS_TRUE;
    }
    else
    {
        retval = MNS_FALSE;
    }

    T_LIB_EXIT(MBM_30);
    return (retval);
}
#endif

#ifdef MBM_35
word MbmGetUsedTxBuffers(void)
{
    word result;

    T_LIB_ENTRY(MBM_35);

    WAIT4MX(MX_MBM_CS);
    result = msgPool.txAlloc;
    REL_MX(MX_MBM_CS);

    T_LIB_EXIT(MBM_35);

    return (result);
}
#endif

#ifdef MBM_36
word MbmGetUsedRxBuffers(void)
{
    word result;

    T_LIB_ENTRY(MBM_36);

    WAIT4MX(MX_MBM_CS);
    result = msgPool.rxAlloc;
    REL_MX(MX_MBM_CS);

    T_LIB_EXIT(MBM_36);

    return (result);
}
#endif

#ifdef MBM_37
/*!
  * \brief      Gets the current MBM usage (not threadsafe)
  * \details    This function is not threadsafe and may only be called from inside
  *             of the MBM Usage callback function
  * \param[out] usage_ptr   Structure that will be filled with the current usage
  */
void _MbmGetUsage(MbmUsage *usage_ptr)
{
    T_API_ENTRY(MBM_37);
    *usage_ptr = mbmUsage.current;
    T_API_EXIT(MBM_37);
}

/*!
  * \brief      Gets the current MBM usage
  * \param[out] usage_ptr   Structure that will be filled with the current usage
  */
void MbmGetUsage(MbmUsage *usage_ptr)
{
    WAIT4MX(MX_MBM_CS);
    _MbmGetUsage(usage_ptr);
    REL_MX(MX_MBM_CS);
}
#endif

#ifdef MBM_38
/*!
  * \brief      Gets the top MBM usage (not threadsafe)
  * \details    This function is not threadsafe and may only be called from inside
  *             of the MBM Usage callback function
  * \param[out] usage_ptr   Structure that will be filled with the top usage
  */
void _MbmGetUsageTop(MbmUsage *usage_ptr)
{
    T_API_ENTRY(MBM_38);
    *usage_ptr = mbmUsage.top;
    T_API_EXIT(MBM_38);
}

/*!
  * \brief       Gets the top MBM usage
  * \param[out]  usage_ptr  Structure that will be filled with the top usage
  */
void MbmGetUsageTop(MbmUsage *usage_ptr)
{
    WAIT4MX(MX_MBM_CS);
    _MbmGetUsageTop(usage_ptr);
    REL_MX(MX_MBM_CS);
}
#endif

#ifdef MBM_45
/*!
  * \brief      Gets the top MBM usage and resets the top MBM usage values (not threadsafe)
  * \details    This function is not threadsafe and may only be called from inside
  *             of the MBM Usage callback function
  * \param[out] usage_ptr   Structure that will be filled with the top usage
  */
void _MbmGetUsageTopExt(MbmUsage *usage_ptr)
{

    T_API_ENTRY(MBM_45);
    *usage_ptr = mbmUsage.top;
    mbmUsage.top = mbmUsage.current;
    T_API_EXIT(MBM_45);
}

/*!
  * \brief       Gets the top MBM usage and resets the top MBM usage values
  * \param[out]  usage_ptr  Structure that will be filled with the top usage
  */
void MbmGetUsageTopExt(MbmUsage *usage_ptr)
{
    WAIT4MX(MX_MBM_CS);
    _MbmGetUsageTopExt(usage_ptr);
    REL_MX(MX_MBM_CS);
}
#endif

#ifdef MBM_39
/*!
  * \brief      Gets detailed information about fragmentation (not threadsafe)
  * \details    This function is not threadsafe and may only be called from inside
  *             of the MBM Usage callback function
  * \param[out] frag_ptr    Structure that will be filled with the fragmentation information
  */
void _MbmGetFragmentation(MbmFragmentation *frag_ptr)
{
    MbmMemInfoIndex i = memInfoArray[0].next;               /* First free memory segment */
    MbmMemInfoIndex max = (MbmMemInfoIndex)MBM_MEM_MAX_SEG; /* Prevent getting stuck on serious errors */

    T_API_ENTRY(MBM_39);

    MNS_MEM_SET((byte *)frag_ptr, (byte)0, (word)sizeof(MbmFragmentation));

    while( i && max )
    {
        if( memInfoArray[i].size < (word)MBM_MEM_UNITS(MBM_USAGE_FRAG_SMALL) )
        {
            frag_ptr->tiny++;
        }
        else if( memInfoArray[i].size < (word)MBM_MEM_UNITS(MBM_USAGE_FRAG_MEDIUM) )
        {
            frag_ptr->little++;
        }
        else if( memInfoArray[i].size < (word)MBM_MEM_UNITS(MBM_USAGE_FRAG_LARGE) )
        {
            frag_ptr->medium++;
        }
        else
        {
            frag_ptr->large++;
        }
        max--;
        i = memInfoArray[i].next;
    }
    T_API_EXIT(MBM_39);
}

/*!
  * \brief      Gets detailed information about fragmentation
  * \param[out] frag_ptr    Structure that will be filled with the fragmentation information
  */
void MbmGetFragmentation(MbmFragmentation *frag_ptr)
{
    WAIT4MX(MX_MBM_CS);
    _MbmGetFragmentation(frag_ptr);
    REL_MX(MX_MBM_CS);
}
#endif

#ifdef MBM_40
static void _MbmUpdateUsageValue(word value, word offset_to_member)
{
    word *top_ptr;
    word *current_ptr = (word *)((byte *)&mbmUsage.current + offset_to_member);

    T_MOD_ENTRY(MBM_40);

    *current_ptr = value; /* mbmUsage.current.??? = value; */

    top_ptr = (word *)((byte *)&mbmUsage.top + offset_to_member);
    if( value > *top_ptr )
    {
        *top_ptr = value; /* mbmUsage.top.??? = value; */
    }

    #if defined(MBM_USAGE_CALLBACK) || defined (MBM_MLOG_STORE)
    {
        word threshold = *((word *)((byte *)&mbmUsage.threshold + offset_to_member));

        if( value >= threshold )
        {
            MBM_LOG((unsigned char *)&mbmUsage.current);
            MBM_USAGE_CALLBACK(&mbmUsage.current);
        }
    }
    #endif
    T_MOD_EXIT(MBM_40);
}
#endif

#ifdef MBM_41
/*!
  * \brief      Sets the threshold when the callback function is called
  * \param[in]  threshold_ptr    Pointer to the MbmUsage structure
  */
void MbmSetUsageThreshold(const MbmUsage *threshold_ptr)
{
    T_API_ENTRY(MBM_41);
    WAIT4MX(MX_MBM_CS);
    mbmUsage.threshold = *threshold_ptr;
    REL_MX(MX_MBM_CS);
    T_API_EXIT(MBM_41);
}
#endif

#ifdef MBM_42
bool MbmAllocateRxMinPayload(HMBMBUF handle)
{
    bool result;
    word size;

    T_LIB_ENTRY(MBM_42);
    result = MNS_FALSE;
    size = MBM_RX_MIN_SIZE;

    WAIT4MX(MX_MBM_CS);
    if (!handle->start_ptr && !handle->size)
    {
        handle->start_ptr = _MbmMemAlloc(&size);

        if( handle->start_ptr )
        {
            handle->size = size;            /* we don't change the type */
            MbmReserve(handle, (word)0 );   /* no reserved size */
            result = MNS_TRUE;
        }
    }
    REL_MX(MX_MBM_CS);

    T_LIB_EXIT(MBM_42);
    return result;
}
#endif

#ifdef MBM_43
static _INLINE void MbmSendMdmMbmBufferAllocationFailed(word type)
{
    byte tmp;
    bool locked;
    bool *locked_ptr;
    TMostTimer *t_ptr;

    T_MOD_ENTRY(MBM_43);

    WAIT4MX(MX_MBM_CS);
    if(type & MBM_TYPE_TX_BIT)              /* Message from type Tx? */
    {
        locked = msgPool.mdmTxLocked;
        locked_ptr = &msgPool.mdmTxLocked;
        t_ptr = &msgPool.mdmTxTimeout;
    }
    else                                    /* Message from type Rx? */
    {
        locked = msgPool.mdmRxLocked;
        locked_ptr = &msgPool.mdmRxLocked;
        t_ptr = &msgPool.mdmRxTimeout;
    }
    REL_MX(MX_MBM_CS);

    if(MNS_FALSE == locked)                 /* Transmission is unlocked? */
    {
        tmp = (byte)(type & (word)0x00FF);  /* Grab only low Byte of message type */
        MdmMsgSendMns(MDM_MBM_BUFFER_ALLOCATION_FAILED, &tmp, (byte)1); /* Transmit MDM */
        WAIT4MX(MX_MBM_CS);
        *locked_ptr = MNS_TRUE;             /* Lock transmission */
        REL_MX(MX_MBM_CS);
        MostSetTimer(t_ptr, MBM_MDM_TIMEOUT, MNS_FALSE);    /* Start timer to unlock transmission after timeout */
    }

    T_MOD_EXIT(MBM_43);
}
#endif

#ifdef MBM_44
HMBMBUF MbmGetHandleByMsgPtr(void *msg)
{
    MbmBuf *handle;

    /* Intended use of the function offsetof(). This is implemented by calculating
     * the offset of a member to NULL (which is defined as the start of it's 
     * parent structure */

    /*lint -e{413} suppress 'likely use of null pointer'*/
    /*lint -e{923} suppress 'cast from pointer to unsigned int'*/
    /*lint -e{970} suppress 'use of modifier unsigned outside a typedef'*/
    /*lint -e{826} suppress 'Suspicious pointer-to-pointer conversion'*/
    handle = (HMBMBUF)((byte *)(msg) - PMS_OFFSETOF(MbmBuf, msg.dataTx));

    return handle;
}
#endif
