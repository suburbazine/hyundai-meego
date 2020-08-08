/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Implementation of the Asynchronous Data Service Wrapper (WADS)
File:           wads.c
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
  * \brief      Implementation of the Asynchronous Data Service Wrapper (WADS)
  * \details    This Service provides an API to transmit and receive
  *             MOST Data Packets on a lower protocol layer. This layer
  *             does not include any packet segmentation and does not
  *             ensure data integrity. MOST High Protocol or MAMAC sits
  *             on top of this service, which are responsible for
  *             segmentation and data integrity.
  */

#include "mbm.h"
#include "pms.h"
#include "mns.h"
#include "wads.h"
#include "wads_pv.h"



/*
==============================================================================
    NetServices trace: module trace macros
==============================================================================
*/

#if (MNS_DEBUG & NST_C_FUNC_ENTRIES)
    #define T_API_ENTRY(func)   MNS_TRACE((MNS_P_SRV_WADS, NST_E_FUNC_ENTRY_API, 1, func))
    #define T_LIB_ENTRY(func)   MNS_TRACE((MNS_P_SRV_WADS, NST_E_FUNC_ENTRY_LIB, 1, func))
    #define T_MOD_ENTRY(func)   MNS_TRACE((MNS_P_SRV_WADS, NST_E_FUNC_ENTRY_MOD, 1, func))
#else
    #define T_API_ENTRY(func)
    #define T_LIB_ENTRY(func)
    #define T_MOD_ENTRY(func)
#endif

#if (MNS_DEBUG & NST_C_FUNC_EXITS)
    #define T_API_EXIT(func)    MNS_TRACE((MNS_P_SRV_WADS, NST_E_FUNC_EXIT_API, 1, func))
    #define T_LIB_EXIT(func)    MNS_TRACE((MNS_P_SRV_WADS, NST_E_FUNC_EXIT_LIB, 1, func))
    #define T_MOD_EXIT(func)    MNS_TRACE((MNS_P_SRV_WADS, NST_E_FUNC_EXIT_MOD, 1, func))
#else
    #define T_API_EXIT(func)
    #define T_LIB_EXIT(func)
    #define T_MOD_EXIT(func)
#endif

#if (MNS_DEBUG & NST_C_INIT)
    #define T_INIT()            MNS_TRACE((MNS_P_SRV_WADS, NST_E_INIT, 0))
#else
    #define T_INIT()
#endif

#if (MNS_DEBUG & NST_C_SERVICE)
    #define T_SERVICE(event)    MNS_TRACE((MNS_P_SRV_WADS, NST_E_SERVICE, 1, event))
#else
    #define T_SERVICE(event)
#endif

#if (MNS_DEBUG & NST_C_REQUESTS)
    #define T_REQUEST(event)    MNS_TRACE((MNS_P_SRV_WADS, NST_E_REQUEST, 1, event))
#else
    #define T_REQUEST(event)
#endif

#if (MNS_DEBUG & NST_C_TX)
    #define T_TX_PACKET(tgt, len)  MNS_TRACE((MNS_P_SRV_WADS, NST_E_TX_PACKET, 2, tgt, len))
#else
    #define T_TX_PACKET(tgt, len)
#endif

#if (MNS_DEBUG & NST_C_RX_MORE)
    #define T_RX_PACKET(src, len)  MNS_TRACE((MNS_P_SRV_WADS, NST_E_RX_PACKET, 2, src, len))
#else
    #define T_RX_PACKET(src, len)
#endif

#if (MNS_DEBUG & NST_C_ASSERTS)
    #define FAILED_ASSERT()     MNS_TRACE((MNS_P_SRV_WADS, NST_E_ASSERT, 1, __LINE__))
    #define ASSERT(exp)         if(!(exp)) FAILED_ASSERT()
#else
    #define FAILED_ASSERT()
    #define ASSERT(exp)
#endif

/*
==============================================================================
    mutexes
==============================================================================
*/

#define TAKE_EVENTS()   WAIT4MX(MX_ADS_PE)
#define GIVE_EVENTS()   REL_MX(MX_ADS_PE)
#define WAIT4RX()    WAIT4MX(MX_ADS_RX_Q)
#define REL_RX()     REL_MX(MX_ADS_RX_Q)

/*
==============================================================================
    module internal variables
==============================================================================
*/

#ifdef ADS_MIN
    /*! Data variable of the WADS module */
    static TWadsData wads_data;
#endif  /* #ifdef ADS_MIN */

/*
==============================================================================
==============================================================================
    Module Implementation
==============================================================================
==============================================================================
*/

#ifdef ADS_0
void DataInit(struct TDataConfig *cfg_ptr)
{
    T_LIB_ENTRY(ADS_0);

    ASSERT(cfg_ptr);

    wads_data.cfg_ptr = cfg_ptr;

    if ((byte)0 == wads_data.cfg_ptr->rx_burst)
    {
        wads_data.cfg_ptr->rx_burst = DATA_STD_RX_BURST;
    }

    T_INIT();
    TAKE_EVENTS();
    wads_data.pending_events       = ADS_P_NONE;
    wads_data.latest_handled_event = ADS_P_NONE;
    #ifndef PACKET_COMMUNICATION_ONLY
        wads_data.net_on           = MNS_FALSE;
    #endif
    #ifdef PACKET_COMMUNICATION_ONLY
        wads_data.ni_state  = (byte)0x00;              /* NET_OFF */
        wads_data.sys_state = (byte)0x00;              /* NOT_OK */
    #endif

    GIVE_EVENTS();

    MbmQueueInit(&(wads_data.rx_queue), MX_ADS_RX_Q);

    #ifndef PACKET_COMMUNICATION_ONLY
    MNS_MEM_SET((byte *)&(wads_data.rsvd_buf.mbm_buf), (byte)0x00,(word)sizeof(MbmBuf));
    wads_data.rsvd_buf.handle = &wads_data.rsvd_buf.mbm_buf;
    PmsInitExternalBuf( wads_data.rsvd_buf.handle, 
                        wads_data.rsvd_buf.payload, 
                        (word)DATA_RSVD_MAX_SIZE, /* effective size incl. header*/
                        MBM_TYPE_DATA_TX);
    #endif

    MnsServiceInitComplete(MNS_PHASE_INIT, MNS_P_SRV_WADS);
    T_LIB_EXIT(ADS_0);
}
#endif

#ifdef ADS_1
void DataService(void)
{
    word event_to_handle;
    bool request_flag;

    T_LIB_ENTRY(ADS_1);

    event_to_handle = DataGetNextEventToHandle();
    request_flag    = MNS_FALSE;

    T_SERVICE(event_to_handle);

    switch (event_to_handle)
    {
        case ADS_P_RX:
            #ifdef ADS_9
                DataRxOutTrigger(NULL);
            #endif
            break;

        default:
            FAILED_ASSERT();
            /* event_to_handle = ADS_P_NONE; commented to avoid PC-Lint warning 438 */
            break;
    }

    TAKE_EVENTS();
    request_flag = (ADS_P_NONE != wads_data.pending_events) ? MNS_TRUE : MNS_FALSE;
    GIVE_EVENTS();

    if (MNS_FALSE != request_flag)
    {
        MnsSetPendingService(MNS_P_SRV_WADS);
    }

    T_LIB_EXIT(ADS_1);
}
#endif

#ifdef ADS_2
static void DataSetPendingEvent(word event_flag)
{
    T_MOD_ENTRY(ADS_2);

    T_REQUEST(event_flag);
    MnsSetPendingEventFlag(event_flag, MX_ADS_PE,
                           &(wads_data.pending_events), MNS_P_SRV_WADS);
    T_MOD_EXIT(ADS_2);
}
#endif

#ifdef ADS_3
static word DataGetNextEventToHandle(void)
{
    word result;

    T_MOD_ENTRY(ADS_3);
    result = MnsGetNextEventFlagToCall(MX_ADS_PE,
                                       &(wads_data.pending_events),
                                       &(wads_data.latest_handled_event),
                                       ADS_P_FIRST, ADS_P_LAST);
    T_MOD_EXIT(ADS_3);
    return(result);
}
#endif

#ifdef ADS_4
/*!
  * \brief      Gets a free message buffer for an asynchronous
  *             data message.
  * \details    When there is no free buffer the function returns
  *             \c NULL. The next attempt can be made after the
  *             callback \c general.on_buf_freed_fptr() was called.
  * \param[in]  size    The amount of payload that should be reserved.
  * \return     -# Pointer to a free message buffer.
  *             -# \c NULL if there is no free buffer.
  */
TDataTx * DataGetTxPtrExt(word size)
{
    HMBMBUF handle;
    TDataTx *msg_ptr;
    bool    size_is_okay;
    byte    current_interface;

    T_API_ENTRY(ADS_4);

    handle       = NULL;
    msg_ptr      = NULL;
    size_is_okay = MNS_FALSE;

    current_interface = PmsGetFifoInterface(PMS_M_FIFO_MDP);
    size_is_okay = (((PMS_IFACE_I2C == current_interface) &&
                     ((word)PMS_I2C_DATA_MAX_PAYLOAD >= size)      ) ||
                    ((PMS_IFACE_GENERAL == current_interface) &&
                     ((word)PMS_DATA_MAX_PAYLOAD >= size)          )) ? MNS_TRUE : MNS_FALSE;

    ASSERT(size_is_okay);
    if (MNS_FALSE != size_is_okay)
    {
        handle = PmsGetBuf(size, MBM_TYPE_DATA_TX);

        if (handle)
        {
            msg_ptr = MBM_GET_DATA_TX_PTR(handle);
            ASSERT(msg_ptr);
        }
        #ifndef PACKET_COMMUNICATION_ONLY
        else if (((word)DATA_RSVD_MAX_SIZE - (word)PMS_DATA_HDR_MAX_SIZE) >= size)
        {
            if (MBM_STAT_RSVD != (MBM_GET_STATUS(wads_data.rsvd_buf.handle) & MBM_STAT_RSVD))
            {
                PmsInitExternalBuf( wads_data.rsvd_buf.handle, 
                                    wads_data.rsvd_buf.payload, 
                                    (word)DATA_RSVD_MAX_SIZE,
                                    MBM_TYPE_DATA_TX);
                MBM_SET_STATUS_BITS(wads_data.rsvd_buf.handle, MBM_STAT_RSVD);
                msg_ptr = &(wads_data.rsvd_buf.handle->msg.dataTx);
            }
        }
        #endif
        else
        {
            /* msg_ptr = NULL;      see initialization of msg_ptr */
        }

        if (NULL != msg_ptr)
        {
            msg_ptr->Length = size;
            if ((word)0 == size)
            {
                msg_ptr->Data = NULL;
            }
        }
    }

    T_API_EXIT(ADS_4);
    return(msg_ptr);
}
#endif

#ifdef ADS_5
/*!
  * \brief      Sends a data packet.
  * \details    When the transmission has been completed the callback
  *             TNetServicesConfig.wads_data.tx_status_fptr() is called.
  * \param[in]  msg_ptr     Pointer to the message.
  */
void DataSend(TDataTx *msg_ptr)
{
    HMBMBUF handle;

    T_API_ENTRY(ADS_5);

    handle = NULL;

    ASSERT(msg_ptr);
    if (msg_ptr)
    {
        handle = MbmGetHandleByMsgPtr(msg_ptr);
        ASSERT(handle);

        T_TX_PACKET((msg_ptr->Tgt_Adr_H << 8) | msg_ptr->Tgt_Adr_L,
                    msg_ptr->Length);
        PmsSend(handle, DataTxFinal);
    }
    T_API_EXIT(ADS_5);
}
#endif

#ifdef ADS_6
static byte DataTxFinal(HMBMBUF handle, byte status)
{
    TDataTx *msg_ptr;
#if (defined NS_WADS_MHP) && (!defined NS_PMS_PMHT)
    byte     tel_id;
    bool     result;
#endif

    T_MOD_ENTRY(ADS_6);
    (void) status;
    msg_ptr = NULL;

    ASSERT(handle);
    msg_ptr = MBM_GET_DATA_TX_PTR(handle);
    ASSERT(msg_ptr);

#if (defined NS_WADS_MHP) && (!defined NS_PMS_PMHT)
    if ((word)4 < msg_ptr->Length)
    {
        tel_id  = msg_ptr->Data[4];
        tel_id &= (byte)0xF0;
    }
    else
    {
        tel_id = (byte)0x00;  /* do not call MhpTxFrameComplete */
        FAILED_ASSERT();      /* transmitted data packet is out of specification */
    }

    #ifdef DATA_TX_DBG_STATUS_ENABLED
    if (wads_data.cfg_ptr->tx_dbg_status_fptr)
    {
        result = wads_data.cfg_ptr->tx_dbg_status_fptr(msg_ptr);
    }
    else
    #endif
    {
        result = MNS_TRUE;
    }

    if ((MNS_FALSE != result) && ((byte)0x80 == tel_id))
    {
        MhpTxFrameComplete();
    }
    else
#endif
    {
        #ifdef ADS_OPT1
        PmsTxStarted(handle);
        #endif

        /*lint -e{960} right-hand operator has no side effects (read only)*/
        if (wads_data.cfg_ptr->tx_status_fptr && EHCISTATE_IS_ATTACHED())
        {
            wads_data.cfg_ptr->tx_status_fptr(msg_ptr);
        }
    }
    MbmFree(handle);

    T_MOD_EXIT(ADS_6);
    return(PMS_RELEASE);
}
#endif

#ifdef ADS_8
void DataRxTrigger(HMBMBUF handle)
{
    T_MOD_ENTRY(ADS_8);
    ASSERT(handle);

    #if (MNS_DEBUG & NST_C_RX_MORE)
    if (handle)
    {
        TDataRx *msg_ptr = MBM_GET_DATA_RX_PTR(handle);
        ASSERT(msg_ptr);
        if(msg_ptr)
        {
            T_RX_PACKET((msg_ptr->Src_Adr_H << 8) | msg_ptr->Src_Adr_L,
                         msg_ptr->Length);
        }
    }
    #endif

    #ifndef PACKET_COMMUNICATION_ONLY
    if ((MNS_FALSE != wads_data.cfg_ptr->rx_direct) && (MNS_FALSE != wads_data.net_on) && ((word)0 == MbmQueueLength(&(wads_data.rx_queue))))
    #else
    if ((MNS_FALSE != wads_data.cfg_ptr->rx_direct) && ((word)0 == MbmQueueLength(&(wads_data.rx_queue))))
    #endif
    {
        DataRxOutTrigger(handle);
    }
    else
    {
        MbmEnqueue(&(wads_data.rx_queue), handle);
        #ifndef PACKET_COMMUNICATION_ONLY
        if(MNS_FALSE != wads_data.net_on)
        #endif
        {
            DataSetPendingEvent(ADS_P_RX);
        }
    }

    T_MOD_EXIT(ADS_8);
}
#endif

#ifdef ADS_9
static void DataRxOutTrigger(HMBMBUF handle)
{
    byte rx_burst;

    T_MOD_ENTRY(ADS_9);

    /* if wads_data.cfg_ptr->rx_direct is MNS_TRUE, there shouldn't be any packet in the
       queue, except for BUSY operation and packets received before NET_ON */
    rx_burst = (MNS_FALSE != wads_data.cfg_ptr->rx_direct) ? (byte)0xFF : wads_data.cfg_ptr->rx_burst;

    #ifndef PACKET_COMMUNICATION_ONLY
    if(MNS_FALSE == wads_data.net_on)
    {
        return;
    }
    #endif

    if(NULL == handle)
    {
        handle = MbmDequeue(&(wads_data.rx_queue));
    }

    ASSERT(handle);

    while (handle && rx_burst)
    {
        TDataRx *msg_ptr = MBM_GET_DATA_RX_PTR(handle);
        byte     result  = DATA_RX_FILTER_DEFAULT;
        #if (defined NS_WADS_MHP) || ((defined PACKET_COMMUNICATION_ONLY) && (defined _OS81110_NA_MDP))
        byte     tel_id  = (byte)0;
        #endif

        ASSERT(msg_ptr);

        #ifdef DATA_RX_DBG_FILTER_ENABLED
        if (NULL != wads_data.cfg_ptr->rx_dbg_filter_fptr)
        {
            result = wads_data.cfg_ptr->rx_dbg_filter_fptr(msg_ptr);
        }
        #endif

        #ifdef DATA_RX_DBG_FILTER_ENABLED
        if (DATA_RX_FILTER_DEFAULT == result)
        #endif
        {
            #ifdef NS_WADS_MHP
            if ((word)4 < msg_ptr->Length)
            {
                tel_id  = msg_ptr->Data[4];
                tel_id &= (byte)0xF0;
            }

            if (((byte)0x80 == tel_id) || ((byte)0x90 == tel_id))
            {
                result = MhpRxTriggerAds(msg_ptr) ? DATA_RX_FILTER_FREE : DATA_RX_FILTER_BUSY;
            }
            else
            #endif
            {
                #if (defined PACKET_COMMUNICATION_ONLY) && (defined _OS81110_NA_MDP)
                if (((byte)0x00 == tel_id) && ((word)15 < msg_ptr->Length))                          /* check for Control message */
                {
                    if (   (FBLOCK_NETBLOCK == msg_ptr->Data[0])
                        && ((byte)0x00      == msg_ptr->Data[2])
                        && ((byte)0x3C      == msg_ptr->Data[3])
                        && ((byte)0x0A      == msg_ptr->Data[5]) )
                    {
                        TDataNAOverMDP state;
                        
                        byte i;

                        state.ni_state   = msg_ptr->Data[8];
                        state.sys_state  = msg_ptr->Data[15];
                        state.node_addr  = (word)((word)msg_ptr->Data[6] << 8) | (word)msg_ptr->Data[7];

                        for (i=(byte)0; i<(byte)6; ++i)
                        {
                            state.eui48[i] = msg_ptr->Data[9 + i];
                        }

                        if (wads_data.cfg_ptr->mdp_state_fptr)
                        {
                            wads_data.cfg_ptr->mdp_state_fptr(&state);
                        }

                        #ifdef NS_WADS_MHP
                        if (   (((byte)0x00 == wads_data.ni_state) && ((byte)0x01 == state.ni_state))      /* change to NET_ON  */
                            || (((byte)0x01 == wads_data.ni_state) && ((byte)0x00 == state.ni_state)) )    /* change to NET_OFF */
                        {
                            MhpInit();
                        }
                        else
                        if ((((byte)0x01 == wads_data.sys_state) && ((byte)0x00 == state.sys_state)))      /* ConfigState -> NotOk */
                        {
                            MhpTxEndAllCon();
                        }

                        /* store current states */
                        wads_data.ni_state  = state.ni_state;
                        wads_data.sys_state = state.sys_state; 
                        #endif
                    }
                }
                #endif

                #ifndef PACKET_COMMUNICATION_ONLY
                if (wads_data.cfg_ptr->rx_filter_fptr && EHCISTATE_IS_ATTACHED())
                #else
                if (wads_data.cfg_ptr->rx_filter_fptr)
                #endif
                {
                    result = wads_data.cfg_ptr->rx_filter_fptr(msg_ptr);
                }

            }
        }

        switch (result)
        {
            case DATA_RX_FILTER_FREE:
                MbmFree(handle);
                handle = NULL;
                break;

            case DATA_RX_FILTER_BUSY:
                MbmEnqueueFirst(&(wads_data.rx_queue), handle);
                rx_burst = (byte)1;
                break;

            default:
                FAILED_ASSERT();

            case DATA_RX_FILTER_DEFAULT: /*lint !e616 !e825 control flows intentionally into case default */
            #ifndef PACKET_COMMUNICATION_ONLY
                if (wads_data.cfg_ptr->rx_complete_fptr && EHCISTATE_IS_ATTACHED())
            #else
                if (wads_data.cfg_ptr->rx_complete_fptr)
            #endif
                {
                    result = wads_data.cfg_ptr->rx_complete_fptr(msg_ptr);
                }
                else
                {
                    result = DATA_RX_COMPLETE_FREE;
                }
                switch (result)
                {
                    case DATA_RX_COMPLETE_BUSY:
                        MbmEnqueueFirst(&(wads_data.rx_queue), handle);
                        rx_burst = (byte)1;
                        break;

                    default:
                        FAILED_ASSERT();

                    case DATA_RX_COMPLETE_FREE: /*lint !e616 !e825 control flows intentionally into case default */
                        MbmFree(handle);
                        handle = NULL;
                        break;
                }
                break;
        }

        rx_burst--;

        if (MbmQueueLength(&(wads_data.rx_queue)))
        {
            if((byte)0 == rx_burst)
            {
                DataSetPendingEvent(ADS_P_RX);
            }
            else
            {
                handle = MbmDequeue(&(wads_data.rx_queue));
                ASSERT(handle);
            }
        }
    }

    T_MOD_EXIT(ADS_9);
}
#endif

#ifdef ADS_10
void DataNIStateNetOn(bool on)
{
    T_LIB_ENTRY(ADS_10);

    wads_data.net_on = on;

    if(MNS_FALSE != on)
    {
        if (MbmQueueLength(&(wads_data.rx_queue)))
        {
            DataSetPendingEvent(ADS_P_RX);
        }
    }
    else
    {
        while (MbmQueueLength(&(wads_data.rx_queue)))
        {
            HMBMBUF handle = MbmDequeue(&(wads_data.rx_queue));
            MbmFree(handle);
        }
    }

    T_LIB_EXIT(ADS_10);
}
#endif

#ifdef ADS_11
byte DataRequestStatus(void)
{
    byte retval;

    T_API_ENTRY(ADS_11);

    retval = PmsTriggerNAOverMDP();

    T_API_EXIT(ADS_11);

    return retval;
}
#endif
