/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Internal API of the Message Buffer Management (MBM)
File:           mbm.h
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
  * \brief      Internal API of the Message Buffer Management (MBM)
  */

#ifndef _MBM_H
#define _MBM_H

/*lint -e(537) repeated include file */
#include "mostns1.h"

/*
==============================================================================
    Library internal macros
==============================================================================
*/


/*
--------------------------------------------------------------------------------
    Macros below are only intended for "friend" modules, e.g. PMS
--------------------------------------------------------------------------------
*/

#define MBM_SET_TYPE(h, t)           ((h)->type &= (word)~MBM_TYPE_MASK);                                      ((h)->type |= (t) & MBM_TYPE_MASK)

#define MBM_GET_STATUS(h)            ((h)->type & MBM_STAT_MASK)
#define MBM_SET_STATUS(h, s)         ((h)->type &= ~MBM_STAT_MASK);                                      ((h)->type |= (s) & MBM_STAT_MASK)

#define MBM_SET_HDR_LEN(h, len)      ((h)->hdr_len = (len))
#define MBM_SET_HDR_PTR(h, ptr)      ((h)->hdr_ptr = (ptr))
#define MBM_GET_HDR_LEN(h)           ((h)->hdr_len)
#define MBM_GET_HDR_PTR(h)           ((h)->hdr_ptr)
#define MBM_RESET_HDR_PTR(h)         ((h)->hdr_ptr = (h)->start_ptr)

#define MBM_SET_PAYLOAD_LEN(h, len)  ((h)->payload_len = (len))
#define MBM_SET_PAYLOAD_PTR(h, ptr)  ((h)->payload_ptr = (ptr))
#define MBM_GET_PAYLOAD_LEN(h)       ((h)->payload_len)
#define MBM_GET_PAYLOAD_PTR(h)       ((h)->payload_ptr)

#define MBM_NEXT(h)                  (((h)->next_ptr != (MbmBuf *)(h)->q_ptr)                                      ? (h)->next_ptr : NULL)

#define MBM_PREV(h)                  (((h)->prev_ptr != (MbmBuf *)(h)->q_ptr)                                      ? (h)->prev_ptr : NULL)

#define MBM_QUEUE_LENGTH(q_ptr)      ((q_ptr)->length)
#define MbmQueueLength(q_ptr)        ((q_ptr)->length)

/*!
  * \brief      Checks if a pointer is outside MBM memory space.
  * \param[in]  ptr     handle to buffer
  * \return     Non-zero if outside or NULL, else 0.
  */
#define MBM_PTR_IS_EXTERNAL(ptr)     (((ptr) < &mbmMemArray[0]) || ((ptr) >= &mbmMemArray[MBM_MEM_UNIT_SIZE * MBM_MEM_UNIT_NUM]))

/*!
  * \brief      Checks if a control TX message is from the MBM buffer pool and has
  *             a payload outside MBM memory.
  * \param[in]  h   handle
  * \return     Non-zero if buffer from pool, payload external and NOT NULL,
  *             else 0.
  */
#define MBM_CTRL_TX_PTR_HAS_EXT_PAYLOAD(h)     ((h)->msg.msgTx.Data && MBM_PTR_IS_EXTERNAL((h)->msg.msgTx.Data) && MbmIsFromPool(h))

/*!
  * \brief      Checks if a asynchronous TX message is from the MBM buffer poll and
  *             has a payload outside MBM memory.
  * \param[in]  h   handle
  * \return     Non-zero if buffer from pool, payload external and NOT NULL,
  *             else 0.
  */
#define MBM_DATA_TX_PTR_HAS_EXT_PAYLOAD(h)     ((h)->msg.dataTx.Data && MBM_PTR_IS_EXTERNAL((h)->msg.dataTx.Data) && MbmIsFromPool(h))



/*
==============================================================================
    Library variables
==============================================================================
*/

extern byte mbmMemArray[];

/*
==============================================================================
    Library internal function prototypes
==============================================================================
*/

#ifdef MBM_0
    void MbmInit(void);
#endif

#ifdef MBM_1
    /*lint -sem(MbmAllocate,r_null)*/
    HMBMBUF MbmAllocate(word size, word reserved, word type);
#endif

#ifdef MBM_2
    /*lint -sem(MbmFree2,r_null,custodial(1))*/
    void MbmFree2(HMBMBUF handle);
#endif

#ifdef MBM_3
    /*lint -sem(MbmQueueInit,1p)*/
    void MbmQueueInit(MbmQueue *q_ptr, word mutex_id);
#endif

#ifdef MBM_4
    /*lint -sem(MbmEnqueue,1p,2p,custodial(2))*/
    void MbmEnqueue(MbmQueue *q_ptr, HMBMBUF handle);
#endif

#ifdef MBM_5
    /*lint -sem(_MbmEnqueue,1p,2p,custodial(2))*/
    void _MbmEnqueue(MbmQueue *q_ptr, HMBMBUF handle);
#endif

#ifdef MBM_6
    /*lint -sem(MbmEnqueueFirst,1p,2p,custodial(2))*/
    void MbmEnqueueFirst(MbmQueue *q_ptr, HMBMBUF handle);
#endif

#ifdef MBM_7
    /*lint -sem(_MbmEnqueueFirst,1p,2p,custodial(2))*/
    void _MbmEnqueueFirst(MbmQueue *q_ptr, HMBMBUF handle);
#endif

#ifdef MBM_8
    /*lint -sem(MbmDequeue,1p,r_null)*/
    HMBMBUF MbmDequeue(MbmQueue *q_ptr);
#endif

#ifdef MBM_9
    /*lint -sem(_MbmDequeue,1p,r_null)*/
    HMBMBUF _MbmDequeue(MbmQueue *q_ptr);
#endif

#ifdef MBM_11
    /*lint -sem(MbmDequeue,1p,r_null)*/
    HMBMBUF MbmDequeueLast(MbmQueue *q_ptr);
#endif

#ifdef MBM_12
    /*lint -sem(_MbmPeek,1p,r_null)*/
    HMBMBUF _MbmPeek(const MbmQueue *q_ptr);
#endif

#ifdef MBM_13
    /*lint -sem(_MbmUnlink,1p)*/
    void _MbmUnlink(HMBMBUF handle);
#endif

#ifdef MBM_14
    /*lint -sem(MbmReserve,1p)*/
    void MbmReserve(HMBMBUF handle, word size);
#endif

#ifdef MBM_15
    /*lint -sem(MbmPush,1p)*/
    byte *MbmPush(HMBMBUF handle, word size);
#endif

#ifdef MBM_16
    /*lint -sem(MbmChangeType,1p)*/
    bool MbmChangeType(HMBMBUF handle, word newType);
#endif

#ifdef MBM_17
    /*lint -sem(MbmPullHeaders,1p)*/
    void MbmPullHeaders(HMBMBUF handle);
#endif

#ifdef MBM_19
    /*lint -sem(MbmGetExtPayloadLen,1p)*/
    word MbmGetExtPayloadLen(HMBMBUF handle);
#endif

#ifdef MBM_23
    /*lint -sem(MbmFlush,1p)*/
    void MbmFlush(MbmQueue *q_ptr);
#endif

#ifdef MBM_27
    /*lint -sem(MbmMemFree,1p)*/
    void MbmMemFree(MbmBuf *mb_ptr);
#endif

#ifdef MBM_29
    /*lint -sem(MbmMemFree,r_null)*/
    HMBMBUF MbmGetBuf(word size);
#endif

#ifdef MBM_31
    /*lint -sem(MbmEnqueueBehind,1p,2p)*/
    void MbmEnqueueBehind(HMBMBUF hPrev, HMBMBUF hNew);
#endif

#ifdef MBM_33
    /*lint -sem(_MbmEnqueueBehind,1p,2p,3p)*/
    void _MbmEnqueueBehind(MbmQueue *q_ptr, HMBMBUF hPrev, HMBMBUF hNew);
#endif

#ifdef MBM_30
    /*lint -sem(MbmIsFromPool,1p)*/
    bool MbmIsFromPool(HCMBMBUF handle);
#endif

#ifdef MBM_35
    word MbmGetUsedTxBuffers(void);
#endif

#ifdef MBM_36
    word MbmGetUsedRxBuffers(void);
#endif

#ifdef MBM_42
    bool MbmAllocateRxMinPayload(HMBMBUF handle);
#endif

#endif /* Header guard */
