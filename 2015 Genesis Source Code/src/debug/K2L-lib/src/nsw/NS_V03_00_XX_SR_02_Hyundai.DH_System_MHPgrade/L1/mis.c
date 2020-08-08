/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Implementation of Message Interface Service (MIS)
File:           mis.c
Version:        3.0.x-SR-2  
Language:       C
Author(s):      S.Semmler, S.Kerber
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
  * \brief      Implementation of the Message Interface Service (MIS)
  */

#include "mbm.h"
#include "pms.h"
#include "mdm.h"
#include "mns.h"

#ifdef GAS_MIN
    #include "gas.h"
#endif

#include "mis.h"
#include "mis_pv.h"

#ifdef ADS_MIN
    #include "wads.h"
#endif

#ifndef PACKET_COMMUNICATION_ONLY
    #include "wmcs.h"
    #include "ams.h"

    #ifdef VMSV_MIN
        #include "vmsv.h"
    #endif

    #ifdef SCM_MIN
        #include "wscm.h"
    #endif
#endif


/*
==============================================================================
    macros
==============================================================================
*/

#if (MNS_DEBUG & NST_C_FUNC_ENTRIES)
    #define T_API_ENTRY(func)   MNS_TRACE((MNS_P_SRV_MIS, NST_E_FUNC_ENTRY_API, 1, func))
    #define T_LIB_ENTRY(func)   MNS_TRACE((MNS_P_SRV_MIS, NST_E_FUNC_ENTRY_LIB, 1, func))
    #define T_MOD_ENTRY(func)   MNS_TRACE((MNS_P_SRV_MIS, NST_E_FUNC_ENTRY_MOD, 1, func))
#else
    #define T_API_ENTRY(func)
    #define T_LIB_ENTRY(func)
    #define T_MOD_ENTRY(func)
#endif

#if (MNS_DEBUG & NST_C_FUNC_EXITS)
    #define T_API_EXIT(func)    MNS_TRACE((MNS_P_SRV_MIS, NST_E_FUNC_EXIT_API, 1, func))
    #define T_LIB_EXIT(func)    MNS_TRACE((MNS_P_SRV_MIS, NST_E_FUNC_EXIT_LIB, 1, func))
    #define T_MOD_EXIT(func)    MNS_TRACE((MNS_P_SRV_MIS, NST_E_FUNC_EXIT_MOD, 1, func))
#else
    #define T_API_EXIT(func)
    #define T_LIB_EXIT(func)
    #define T_MOD_EXIT(func)
#endif

#if (MNS_DEBUG & NST_C_INIT)
    #define T_INIT()            MNS_TRACE((MNS_P_SRV_MIS, NST_E_INIT, 0))
#else
    #define T_INIT()
#endif

#if (MNS_DEBUG & NST_C_SERVICE)
    #define T_SERVICE(event)    MNS_TRACE((MNS_P_SRV_MIS, NST_E_SERVICE, 1, event))
#else
    #define T_SERVICE(event)
#endif

#if (MNS_DEBUG & NST_C_REQUESTS)
    #define T_REQUEST(event)    MNS_TRACE((MNS_P_SRV_MIS, NST_E_REQUEST, 1, event))
#else
    #define T_REQUEST(event)
#endif

#if (MNS_DEBUG & NST_C_RX)
    #define T_RX(src, fb, inst, func, op, len)  MNS_TRACE((MNS_P_SRV_MIS, NST_E_RX, 6, src, fb, inst, func, op, len))
    #define T_RX_PACKET(src, len)  MNS_TRACE((MNS_P_SRV_MIS, NST_E_RX_PACKET, 2, src, len))
#else
    #define T_RX(src, fb, inst, func, op, len)
    #define T_RX_PACKET(src, len)
#endif

#if (MNS_DEBUG & NST_C_ASSERTS)
    #define FAILED_ASSERT()     MNS_TRACE((MNS_P_SRV_MIS, NST_E_ASSERT, 1, __LINE__))
    #define ASSERT(exp)         if(!(exp)) FAILED_ASSERT()
#else
    #define FAILED_ASSERT()
    #define ASSERT(exp)
#endif

#ifdef MIS_MIN

    #define TAKE_EVENTS()       WAIT4MX(MX_MIS_PE)
    #define GIVE_EVENTS()        REL_MX(MX_MIS_PE)
#endif

/*
================================================================================
    Module Internal Variables
================================================================================
*/

#ifdef MIS_MIN
    /*! Data variable of the MIS module */
    static TMisData mis_data;
#endif  /* #ifdef MIS_MIN */


/*
================================================================================
    Module Internal Tables
================================================================================
*/

#if (defined MIS_MIN) && (!defined PACKET_COMMUNICATION_ONLY)
    #include "mis.tab"
#endif



/*
==============================================================================
==============================================================================
    Module Implementation
==============================================================================
==============================================================================
*/

#ifdef MIS_0
void MisInit(struct TLldConfig   *lld_cfg_ptr
             ,struct TPmsConfig  *pms_cfg_ptr
             ,struct TMsgConfig  *msg_cfg_ptr
             #if defined(ADS_MIN) && defined(DATA_TX_USER_PAYLOAD)
             ,struct TDataConfig *data_cfg_ptr
             #endif
             #ifdef MDM_MIN
             ,struct TMdmConfig *mdm_cfg_ptr
             #endif
             #ifdef GAS_MIN
             ,struct TGasConfig *gas_cfg_ptr
             #endif
             )
{
    T_LIB_ENTRY(MIS_0);
    T_INIT();

    #if (defined PACKET_COMMUNICATION_ONLY) || ((!defined AMS_TX_BYPASS_FILTER) && (!defined NS_AMS_MSV2)              && (!defined MSG_TX_USER_PAYLOAD) && (!defined MSG_RX_USER_PAYLOAD))
    (void)msg_cfg_ptr;
    #endif

    mis_data.lld_cfg_ptr          = lld_cfg_ptr;

    TAKE_EVENTS();
    mis_data.init_complete        = MNS_FALSE;
    mis_data.pending_events       = MIS_P_NONE;
    mis_data.latest_handled_event = MIS_P_NONE;
    GIVE_EVENTS();

    mis_data.pms_config.cbBufFreed_fptr  = MisHandleBufFreed;
    mis_data.pms_config.cbExtEvents_fptr = MisHandlePmsEvent;
#ifdef MIS_8
    mis_data.pms_config.cbMdpRx_fptr     = MisHandlePacketData;
#else
    mis_data.pms_config.cbMdpRx_fptr     = NULL;
#endif
    mis_data.pms_config.startIfaces_fptr = lld_cfg_ptr->start_interfaces_fptr;
    mis_data.pms_config.stopIfaces_fptr  = lld_cfg_ptr->stop_interfaces_fptr;
    mis_data.pms_config.dataTx_fptr      = lld_cfg_ptr->data_tx_fptr;

    ASSERT(pms_cfg_ptr->rx_burst);
    mis_data.pms_config.rx_burst         = pms_cfg_ptr->rx_burst ? pms_cfg_ptr->rx_burst : PMS_STD_RX_BURST;

#ifndef PACKET_COMMUNICATION_ONLY
    #ifdef MIS_6
        mis_data.pms_config.cbIcmRx_fptr     = MisHandleInicMsg;
    #else
        mis_data.pms_config.cbIcmRx_fptr     = NULL;
    #endif

    #ifdef MIS_7
        mis_data.pms_config.cbMcmRx_fptr     = MisHandleMostMsg;
    #else
        mis_data.pms_config.cbMcmRx_fptr     = NULL;
    #endif

    #ifdef AMS_12
        mis_data.pms_config.cbRxErr_fptr     = MsgRxError;
    #else
        mis_data.pms_config.cbRxErr_fptr     = NULL;
    #endif

    #ifdef AMS_TX_BYPASS_FILTER
    mis_data.pms_config.cbTxBypass_fptr  = msg_cfg_ptr->tx_bypass_fptr;
    #endif
    #ifdef NS_AMS_MSV2
    mis_data.pms_config.cbTxBufferFlushed_fptr = msg_cfg_ptr->tx_buffer_flushed_fptr;
    #endif
    #ifdef MSG_TX_USER_PAYLOAD
    mis_data.pms_config.cbFreeUserTxMsgPayload_fptr = msg_cfg_ptr->free_tx_payload_fptr;
    mis_data.pms_config.cbFreeUserTxMsgPayloadExt_fptr  = msg_cfg_ptr->free_tx_payload_ext_fptr;
    #endif

    #if defined(DATA_TX_USER_PAYLOAD) && defined(ADS_MIN)
    mis_data.pms_config.cbFreeUserTxDataPayload_fptr = data_cfg_ptr->free_data_tx_payload_fptr;
    mis_data.pms_config.cbFreeUserTxDataPayloadExt_fptr = data_cfg_ptr->free_data_tx_payload_ext_fptr;
    #endif

    #ifdef MSG_RX_USER_PAYLOAD
    mis_data.pms_config.cbGetPayloadBuf_fptr = msg_cfg_ptr->get_payload_buf_fptr;
    mis_data.pms_config.cbFreePayloadBuf_fptr = msg_cfg_ptr->free_payload_buf_fptr;
    mis_data.pms_config.cbReallocPayloadBuf_fptr = msg_cfg_ptr->reallocate_payload_buf_fptr;
    #endif
    mis_data.pms_config.i2cTx_fptr       = lld_cfg_ptr->i2c_tx_fptr;
    mis_data.pms_config.ctrlTx_fptr      = lld_cfg_ptr->ctrl_tx_fptr;
    mis_data.pms_config.icmIface         = lld_cfg_ptr->ctrl_interface_id;
    mis_data.pms_config.mcmIface         = lld_cfg_ptr->ctrl_interface_id;
    mis_data.pms_config.mdpIface         = lld_cfg_ptr->data_interface_id;
#endif

    ASSERT(mis_data.lld_cfg_ptr->reset_fptr);

    mis_data.reset_counter = (byte)0;
    MostRegisterTimer(&(mis_data.reset_timer), MisSetPendingEvent, MIS_P_RESET_TIMER);

    MbmInit();
    PmsInit(&(mis_data.pms_config));

    #ifdef MDM_MIN
        mis_data.mdm_config.app_config_fptr = mdm_cfg_ptr->app_config_fptr;
        MdmInit(&(mis_data.mdm_config));
    #endif  /* #ifdef MDM_MIN */

    #ifdef PACKET_COMMUNICATION_ONLY
        MnsServiceInitComplete(MNS_PHASE_INIT, MNS_P_SRV_MIS);
    #endif

    #ifdef GAS_MIN
        mis_data.gas_config.dbg_cb_fptr = gas_cfg_ptr->dbg_cb_fptr;
        GasInit(&(mis_data.gas_config));
    #endif  /* #ifdef GAS_MIN */


    T_LIB_EXIT(MIS_0);
}
#endif


#ifdef MIS_1
void MisService(void)
{
    word event_to_handle;
    bool request_flag;
    bool init_complete;

    T_LIB_ENTRY(MIS_1);

    event_to_handle = MisGetNextEventToHandle();
    request_flag    = MNS_FALSE;
    TAKE_EVENTS();
    init_complete = mis_data.init_complete;
    GIVE_EVENTS();

    T_SERVICE(event_to_handle);

    switch (event_to_handle)
    {
        case MIS_P_SYNC:
        case MIS_P_SYNC_RESET:

            MostClearTimer(&(mis_data.reset_timer));

            if ((MNS_FALSE == init_complete) && (MIS_P_SYNC == event_to_handle))
            {
                TAKE_EVENTS();
                mis_data.init_complete = MNS_TRUE;
                GIVE_EVENTS();

                MnsServiceInitComplete(MNS_PHASE_INIT, MNS_P_SRV_MIS);
            }
            else
            {
                if (MIS_P_SYNC_RESET == event_to_handle)
                {
                    MnsReportError(NSR_EI_RESET);

                }
                else
                {
                    MnsReportError(NSR_EI_SYNC);
                }
            }
            break;

        case MIS_P_SYNC_FAILED:
            if (mis_data.reset_counter >= (byte)MIS_RESET_TRIES)
            {
                MnsReportError(NSR_E_NO_COMM);
            }
            else
            {
                PmsSetFifoInterface(PMS_M_FIFO_CTRL, mis_data.pms_config.icmIface);
                PmsSetFifoInterface(PMS_M_FIFO_MDP, mis_data.pms_config.mdpIface);

                MnsReportError(NSR_EI_SYNC_FAILED);

                mis_data.reset_counter++;
                MostSetTimer(&(mis_data.reset_timer), MIS_RESET_TIMEOUT, MNS_FALSE);
                if (mis_data.lld_cfg_ptr->reset_fptr)
                {
                    mis_data.lld_cfg_ptr->reset_fptr();
                }
            }
            break;

        case MIS_P_RESET_TIMER:
            if (mis_data.reset_counter >= (byte)MIS_RESET_TRIES)
            {
                MnsReportError(NSR_E_NO_COMM);
            }
            break;

        case MIS_P_BUF_FREED:
            if (mis_data.lld_cfg_ptr->on_buf_freed_fptr)
            {
                mis_data.lld_cfg_ptr->on_buf_freed_fptr();
            }
            break;

        case MIS_P_SYNC_NOT_SUPPORTED:
            MnsReportError(NSR_E_WRONG_PROTOCOL_CFG);
            break;

        default:
            FAILED_ASSERT();
            /* event_to_handle = MNS_P_NONE; commented to avoid PC-Lint warning 438  */
            break;
    }

    TAKE_EVENTS();
    request_flag = (MIS_P_NONE != mis_data.pending_events) ? MNS_TRUE : MNS_FALSE;
    GIVE_EVENTS();

    if (MNS_FALSE != request_flag)
    {
        MnsSetPendingService(MNS_P_SRV_MIS);
    }

    T_LIB_EXIT(MIS_1);
}
#endif


#ifdef MIS_2
static word MisGetNextEventToHandle(void)
{
    word result = MIS_P_NONE;

    T_MOD_ENTRY(MIS_2);
    result = MnsGetNextEventFlagToCall(MX_MIS_PE,
                                       &(mis_data.pending_events),
                                       &(mis_data.latest_handled_event),
                                       MIS_P_FIRST, MIS_P_LAST);
    T_MOD_EXIT(MIS_2);
    return(result);
}
#endif


#ifdef MIS_3
void MisSetPendingEvent(word event_flag)
{
    T_MOD_ENTRY(MIS_3);

    T_REQUEST(event_flag);
    MnsSetPendingEventFlag(event_flag, MX_MIS_PE,
                           &(mis_data.pending_events), MNS_P_SRV_MIS);
    T_MOD_EXIT(MIS_3);
}
#endif


#ifdef MIS_4
static void MisHandleBufFreed(void)
{
    T_MOD_ENTRY(MIS_4);
    MnsDistribEvent(MNS_P_SRV_MIS, MIS_P_BUF_FREED);
    T_MOD_EXIT(MIS_4);
}
#endif


#ifdef MIS_5
static void MisHandlePmsEvent(word event)
{
    word mis_event = MIS_P_NONE;

    T_MOD_ENTRY(MIS_5);
    switch (event)
    {
        case PMS_E_SYNC_CMD:
            mis_event = MIS_P_SYNC;
            break;

        case PMS_E_SYNC_RESET:
            mis_event = MIS_P_SYNC_RESET;
            break;

        case PMS_E_SYNC_FAILED:
            mis_event = MIS_P_SYNC_FAILED;
            break;

        case PMS_E_SYNC_NOT_SUPPORTED:
            mis_event = MIS_P_SYNC_NOT_SUPPORTED;
            break;

        default:
            FAILED_ASSERT();
            break;
    }

    if (MIS_P_NONE != mis_event)
    {
        MisSetPendingEvent(mis_event);
    }
    T_MOD_EXIT(MIS_5);
}
#endif


#ifdef MIS_6
static void MisHandleInicMsg(HMBMBUF handle)
{
#ifdef MNS_AVOID_ATTACH
    MisHandleMostMsg(handle);
#else
    bool    func_done;
    word    func_idx;
    TMsgRx *msg_ptr;

    T_MOD_ENTRY(MIS_6);

    #ifdef GAS_MIN
    GasHandleInicMsg(handle);
    #endif  /* #ifdef GAS_MIN */

    func_done = MNS_FALSE;
    func_idx  = 0;
    msg_ptr   = MBM_GET_CTRL_RX_PTR(handle);

    if (mis_data.reset_counter)
    {
        mis_data.reset_counter = (byte)0;
    }

    if (MNS_FALSE != MnsIsNSInitComplete(MNS_P_SRV_PMS))
    {
        ASSERT(msg_ptr);
        T_RX(msg_ptr->Src_Adr, msg_ptr->FBlock_ID, msg_ptr->Inst_ID,
             msg_ptr->Func_ID, msg_ptr->Operation, msg_ptr->Length);

#ifndef DISABLE_PLT_FILTER
#ifdef VMSV_102
        if (MNS_FALSE == VmsvPhysicalLayerTestActive())
#endif
#endif
        {
            while ((MNS_FALSE == func_done) && (NULL != inic_handler_table[func_idx].op_type_table))
            {
                if (msg_ptr->Func_ID == inic_handler_table[func_idx].func_id)
                {
                    bool op_done = MNS_FALSE;
                    word op_idx  = 0;

                    func_done = MNS_TRUE;
                    while ((MNS_FALSE == op_done) &&
                           (NULL != inic_handler_table[func_idx].op_type_table[op_idx].handler_fptr_table))
                    {
                        if (msg_ptr->Operation == inic_handler_table[func_idx].op_type_table[op_idx].op_type)
                        {
                            word handler_idx = 0;
                            _CONST TMisHandlerFuncPtr *handler_fptr_table =
                                inic_handler_table[func_idx].
                                op_type_table[op_idx].handler_fptr_table;

                            op_done = MNS_TRUE;
                            while (NULL != handler_fptr_table[handler_idx])
                            {
                                handler_fptr_table[handler_idx](msg_ptr);
                                handler_idx++;
                            }
                        }
                        op_idx++;
                    }
                }
                func_idx++;
            }
        }
#ifndef DISABLE_PLT_FILTER
#ifdef VMSV_102
        else        /* only certain messages are allowed during PhysicalLayerTest */
        {
            #ifdef VMSV_97
            if ((FUNCID_INIC_NUMCODINGERRORS == msg_ptr->Func_ID  ) &&
                (OP_STATUS                   == msg_ptr->Operation))
            {
                VmsvHandleCodingErrorsStatus(msg_ptr);
            }
            else
            #endif
            if ((FUNCID_INIC_LOCKSTATE == msg_ptr->Func_ID  ) &&
                (OP_STATUS             == msg_ptr->Operation))
            {
                VmsvHandleLockStateStatus(msg_ptr);
            }
            else if ((FUNCID_INIC_EHCISTATE == msg_ptr->Func_ID  ) &&
                     (OP_STATUS             == msg_ptr->Operation))
            {
                MnsHandleEHCIStateStatus(msg_ptr);
            }
        }
#endif
#endif
    }
    MbmFree(handle);
    T_MOD_EXIT(MIS_6);
#endif
}
#endif


#ifdef MIS_12
void MisFilterMostMsg(HMBMBUF handle)
{
    TMsgRx *msg_ptr     = MBM_GET_CTRL_RX_PTR(handle);
    #ifndef MNS_AVOID_ATTACH
    word    fblock_idx  = 0;
    bool    fblock_done = (MSG_SRC_INIC != msg_ptr->Src_Adr) ? MNS_TRUE : MNS_FALSE;
    #endif

    T_LIB_ENTRY(MIS_12);

    if (mis_data.reset_counter)
    {
        mis_data.reset_counter = (byte)0;
    }

    #ifdef MCS_MIN
    if ((FBLOCK_NETBLOCK == msg_ptr->FBlock_ID) &&
        (((MSG_SRC_INIC == msg_ptr->Src_Adr)    &&
          (OP_REPORTS <= msg_ptr->Operation))   ||
         ((OP_REPORTS > msg_ptr->Operation)     &&
          (0 != msg_ptr->Inst_ID))
        ))
    {
        WmcsRefreshNodePos(msg_ptr->Inst_ID);
    }
    #endif

    #ifndef MNS_AVOID_ATTACH
    if (MNS_FALSE != MnsIsNSInitComplete(MNS_P_SRV_PMS))
    {
        T_RX(msg_ptr->Src_Adr, msg_ptr->FBlock_ID, msg_ptr->Inst_ID,
             msg_ptr->Func_ID, msg_ptr->Operation, msg_ptr->Length);

        #ifdef VMSV_MIN
        VmsvRxFilter(msg_ptr);
        #endif

        #ifdef MSV_DIAG_RESULT_MSG
        if ((FBLOCK_NETBLOCK         == msg_ptr->FBlock_ID) &&
            (FUNC_NB_RBDRESULT       == msg_ptr->Func_ID)   &&
            (OP_STATUS               == msg_ptr->Operation))
        {
            VmsvHandleDiagResult(msg_ptr);
        }
        #endif

        while ((MNS_FALSE == fblock_done) && mis_most_handler_table[fblock_idx].func_table)
        {
            if (msg_ptr->FBlock_ID == mis_most_handler_table[fblock_idx].
                                      fblock_id)
            {
                bool func_done = MNS_FALSE;
                word func_idx  = 0;

                fblock_done = MNS_TRUE;
                while ((MNS_FALSE == func_done) && mis_most_handler_table[fblock_idx].
                                     func_table[func_idx].
                                     op_type_table)
                {
                    if (msg_ptr->Func_ID ==
                        mis_most_handler_table[fblock_idx].
                        func_table[func_idx].func_id)
                    {
                        bool op_done = MNS_FALSE;
                        word op_idx  = 0;

                        func_done = MNS_TRUE;
                        while ((MNS_FALSE == op_done) && mis_most_handler_table[fblock_idx].
                                           func_table[func_idx].
                                           op_type_table[op_idx].
                                           handler_fptr_table)
                        {
                            if (msg_ptr->Operation ==
                                mis_most_handler_table[fblock_idx].
                                func_table[func_idx].
                                op_type_table[op_idx].op_type)
                            {
                                word handler_idx = 0;
                                _CONST TMisHandlerFuncPtr *handler_fptr_table =
                                        mis_most_handler_table[fblock_idx].
                                        func_table[func_idx].
                                        op_type_table[op_idx].
                                        handler_fptr_table;

                                op_done = MNS_TRUE;
                                while (handler_fptr_table[handler_idx])
                                {
                                    handler_fptr_table[handler_idx](msg_ptr);
                                    handler_idx++;
                                }
                            }
                            op_idx++;
                        }
                    }
                    func_idx++;
                }
            }
            fblock_idx++;
        }
    }
    #endif

    T_LIB_EXIT(MIS_12);
}
#endif


#ifdef MIS_7
static void MisHandleMostMsg(HMBMBUF handle)
{
    TMsgRx *msg_ptr     = MBM_GET_CTRL_RX_PTR(handle);

    T_MOD_ENTRY(MIS_7);

    if (MNS_FALSE != MnsIsNSInitComplete(MNS_P_SRV_ALL))
    {
        MsgRxTrigger(msg_ptr);
    }
    else
    {
        MbmFree(handle);
    }

    T_MOD_EXIT(MIS_7);
}
#endif


#ifdef MIS_8
static void MisHandlePacketData(HMBMBUF handle)
{
    T_MOD_ENTRY(MIS_8);

    #ifdef ADS_MIN
        if (MNS_FALSE != MnsIsNSInitComplete(MNS_P_SRV_ALL))
        {
            #if (MNS_DEBUG & NST_C_RX)
                TDataRx *msg_ptr = MBM_GET_DATA_RX_PTR(handle);
                ASSERT(msg_ptr);
                T_RX_PACKET((msg_ptr->Src_Adr_H << 8) | msg_ptr->Src_Adr_L,
                            msg_ptr->Length);
            #endif
            DataRxTrigger(handle);
        }
        else
        {
            MbmFree(handle);
        }
    #else
        MbmFree(handle);
    #endif

    T_MOD_EXIT(MIS_8);
}
#endif


#ifdef MIS_9
void MisResetInic(void)
{
    T_LIB_ENTRY(MIS_9);

    MisSetPendingEvent(MIS_P_SYNC_FAILED);

    T_LIB_EXIT(MIS_9);
}
#endif


#ifdef MIS_10
byte MisGetResetCount(void)
{
    byte ret;

    T_LIB_ENTRY(MIS_10);

    ret = mis_data.reset_counter;

    T_LIB_EXIT(MIS_10);

    return(ret);
}
#endif


#ifdef MIS_11
void MisSetResetCount(byte count)
{
    T_LIB_ENTRY(MIS_11);

    mis_data.reset_counter = count;

    T_LIB_EXIT(MIS_11);
}
#endif

#ifdef MIS_13
void MisForceInicReset(bool guarded)
{
    bool reset;
    static byte cnt = (byte)0;

    T_LIB_ENTRY(MIS_13);

    reset = MNS_FALSE;

    PmsSetFifoInterface(PMS_M_FIFO_CTRL, mis_data.pms_config.icmIface);
    PmsSetFifoInterface(PMS_M_FIFO_MDP, mis_data.pms_config.mdpIface);

    if (MNS_FALSE != guarded)
    {
        if (cnt < (byte)MIS_MAX_FORCED_RESETS)
        {
            cnt++;
            reset = MNS_TRUE;
        }
    }
    else
    {
        reset = MNS_TRUE;
    }

    if ((MNS_FALSE != reset) && (NULL != mis_data.lld_cfg_ptr->reset_fptr))
    {
        mis_data.lld_cfg_ptr->reset_fptr();
    }
    else
    {
        MnsReportError(NSR_E_NO_COMM);
    }

    T_LIB_EXIT(MIS_13);
}
#endif
