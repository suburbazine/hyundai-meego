/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Implementation of the MOST NetServices Kernel (Basic Layer)
File:           mns.c
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
Date        By      		Description
2013-10-02	hanhong.keum	Fix Errata Bug

==============================================================================
*/


/*! \file
  * \brief      Implementation of the MOST NetServices Kernel (Basic Layer)
  * \details    This module contains the kernel functions of Layer I. I controls all services of
  *             Layer I and contains the internal timer management. Id additions it is responsible
  *             for the correct initialization of all services, keeping the EGC attached to the INIC,
  *             common task used by all services and driving the application socket layer (Layer II)
  *             as well as the MOST High Protocol Service and Packetizer 4 MOST High Service implementation
  *             if present.
  */

#include "mbm.h"
#include "pms.h"
#include "mis.h"
#include "mdm.h"
#include "mns.h"
#include "mns_pv.h"

#ifdef ADS_MIN
    #include "wads.h"
#endif

#ifdef AMS_MIN
    #include "ams.h"
#endif
#ifdef MCS_MIN
    #include "wmcs.h"
#endif

#ifdef VMSV_MIN
    #include "vmsv.h"
#endif

#ifdef SCM_MIN
    #include "wscm.h"
#endif
#ifdef SCMPM_MIN
    #include "scmpm.h"
#endif
#ifdef GAS_MIN
    #include "gas.h"
#endif
#ifdef AAM_MIN
    #include "aam.h"
#endif


/*
==============================================================================
    NetServices trace: module trace macros
==============================================================================
*/

#if (MNS_DEBUG & NST_C_FUNC_ENTRIES)
    #define T_API_ENTRY(func)   MNS_TRACE((MNS_P_SRV_MNS, NST_E_FUNC_ENTRY_API, 1, func))
    #define T_LIB_ENTRY(func)   MNS_TRACE((MNS_P_SRV_MNS, NST_E_FUNC_ENTRY_LIB, 1, func))
    #define T_MOD_ENTRY(func)   MNS_TRACE((MNS_P_SRV_MNS, NST_E_FUNC_ENTRY_MOD, 1, func))
#else
    #define T_API_ENTRY(func)
    #define T_LIB_ENTRY(func)
    #define T_MOD_ENTRY(func)
#endif

#if (MNS_DEBUG & NST_C_FUNC_EXITS)
    #define T_API_EXIT(func)    MNS_TRACE((MNS_P_SRV_MNS, NST_E_FUNC_EXIT_API, 1, func))
    #define T_LIB_EXIT(func)    MNS_TRACE((MNS_P_SRV_MNS, NST_E_FUNC_EXIT_LIB, 1, func))
    #define T_MOD_EXIT(func)    MNS_TRACE((MNS_P_SRV_MNS, NST_E_FUNC_EXIT_MOD, 1, func))
#else
    #define T_API_EXIT(func)
    #define T_LIB_EXIT(func)
    #define T_MOD_EXIT(func)
#endif

#if (MNS_DEBUG & NST_C_INIT)
    #define T_INIT()            MNS_TRACE((MNS_P_SRV_MNS, NST_E_INIT, 0))
#else
    #define T_INIT()
#endif

#if (MNS_DEBUG & NST_C_SERVICE)
    #define T_SERVICE(event)    MNS_TRACE((MNS_P_SRV_MNS, NST_E_SERVICE, 1, event))
#else
    #define T_SERVICE(event)
#endif

#if (MNS_DEBUG & NST_C_REQUESTS)
    #define T_REQUEST(event)    MNS_TRACE((MNS_P_SRV_MNS, NST_E_REQUEST, 1, event))
#else
    #define T_REQUEST(event)
#endif

#if (MNS_DEBUG & NST_C_INIT)
    #define T_MOST_INIT(srv, cfg) MNS_TRACE((MNS_P_SRV_MNS, NST_E_MOST_INIT, 6, mns.shadow.version.public.ns[0], mns.shadow.version.public.ns[1], mns.shadow.version.public.ns[2], mns.shadow.version.public.ns[3], srv, cfg))
#else
    #define T_MOST_INIT(srv, cfg)
#endif

#if (MNS_DEBUG & NST_C_INIT)
    #define T_INIC_INIT() MNS_TRACE((MNS_P_SRV_MNS, NST_E_INIC_INIT, 4, mns.shadow.version.public.hw, mns.shadow.version.public.fw[0], mns.shadow.version.public.fw[1], mns.shadow.version.public.fw[2]))
#else
    #define T_INIC_INIT()
#endif

#if (MNS_DEBUG & NST_C_MISC)
    #define T_TM_EV(e)           MNS_TRACE((MNS_P_SRV_MNS, NST_E_TM_EV,          1, e))
    #define T_TM_OP(o)           MNS_TRACE((MNS_P_SRV_MNS, NST_E_TM_OP,          1, o))
    #define T_TM_MIN_TIMEOUT(t)  MNS_TRACE((MNS_P_SRV_MNS, NST_E_TM_MIN_TIMEOUT, 1, t))
    #define T_TM_DIFF(d, s)      MNS_TRACE((MNS_P_SRV_MNS, NST_E_TM_DIFF,        2, d, s))
    #define T_TM_FLAGS(f)        MNS_TRACE((MNS_P_SRV_MNS, NST_E_TM_FLAGS,       1, f))
    #define T_TM_TBOFF(d)        MNS_TRACE((MNS_P_SRV_MNS, NST_E_TM_TBOFF,       1, d))
#else
    #define T_TM_EV(e)
    #define T_TM_OP(o)
    #define T_TM_MIN_TIMEOUT(t)
    #define T_TM_DIFF(d, s)
    #define T_TM_FLAGS(f)
    #define T_TM_TBOFF(d)
#endif

#if (MNS_DEBUG & NST_C_SERVICE)
    #define T_MOST_SERVICE(event)   MNS_TRACE((MNS_P_SRV_MNS, NST_E_MOST_SERVICE, 1, event))
    #define T_L2_SERVICE(event)     MNS_TRACE((MNS_P_SRV_MNS, NST_E_L2_SERVICE, 1, event))
    #define T_MHP_SERVICE(event)    MNS_TRACE((MNS_P_SRV_MNS, NST_E_MHP_SERVICE, 1, event))
    #define T_PMHS_SERVICE(event)   MNS_TRACE((MNS_P_SRV_MNS, NST_E_PMHS_SERVICE, 1, event))
    #define T_START_L2()            MNS_TRACE((MNS_P_SRV_MNS, NST_E_START_L2, 0))
    #define T_STOP_L2()             MNS_TRACE((MNS_P_SRV_MNS, NST_E_STOP_L2, 0))
    #define T_START_PL()            MNS_TRACE((MNS_P_SRV_MNS, NST_E_START_PL, 0))
    #define T_STOP_PL()             MNS_TRACE((MNS_P_SRV_MNS, NST_E_STOP_PL, 0))
#else
    #define T_MOST_SERVICE(event)
    #define T_L2_SERVICE(event)
    #define T_MHP_SERVICE(event)
    #define T_PMHS_SERVICE(event)
    #define T_START_L2()
    #define T_STOP_L2()
    #define T_START_PL()
    #define T_STOP_PL()
#endif

#if (MNS_DEBUG & NST_C_REQUESTS)
    #define T_MOST_REQUEST(event)   MNS_TRACE((MNS_P_SRV_MNS, NST_E_MOST_REQUEST, 1, event))
    #define T_L2_REQUEST(event)     MNS_TRACE((MNS_P_SRV_MNS, NST_E_L2_REQUEST, 1, event))
    #define T_MHP_REQUEST(event)    MNS_TRACE((MNS_P_SRV_MNS, NST_E_MHP_REQUEST, 1, event))
    #define T_PMHS_REQUEST(event)   MNS_TRACE((MNS_P_SRV_MNS, NST_E_PMHS_REQUEST, 1, event))
#else
    #define T_MOST_REQUEST(event)
    #define T_L2_REQUEST(event)
    #define T_MHP_REQUEST(event)
    #define T_PMHS_REQUEST(event)
#endif

#if (MNS_DEBUG & NST_C_STATES)
    #define T_EHCISTATE_CHANGE(state)   MNS_TRACE((MNS_P_SRV_MNS, NST_E_STATE_CHANGE, 2, NST_P1_EHCISTATE, state))
    #define T_INIT_PHASE_CHANGE(state)  MNS_TRACE((MNS_P_SRV_MNS, NST_E_STATE_CHANGE, 2, NST_P1_INIT_PHASE, state))
    #define T_RESET_PHASE_CHANGE(state) MNS_TRACE((MNS_P_SRV_MNS, NST_E_STATE_CHANGE, 2, NST_P1_RESET_PHASE, state))
    #define T_TRANSITION_CAUSE(length,cause) if (1 < length) MNS_TRACE((MNS_P_SRV_MNS, NST_E_TRANSITION_CAUSE, 1, cause))
#else
    #define T_EHCISTATE_CHANGE(state)
    #define T_INIT_PHASE_CHANGE(state)
    #define T_RESET_PHASE_CHANGE(state)
    #define T_TRANSITION_CAUSE(length,cause)
#endif

#if (MNS_DEBUG & NST_C_MISC)
    #define T_TM_WATCHDOG_TIMEOUT() MNS_TRACE((MNS_P_SRV_MNS, NST_E_TM_WATCHDOG_TIMEOUT, 3, mns.config.general.watchdogmode.reset, mns.config.general.watchdogmode.timeout, mns.config.general.watchdogmode.auto_shutdown_delay))
#else
    #define T_TM_WATCHDOG_TIMEOUT()
#endif

#if (MNS_DEBUG & NST_C_STATES)
    #define T_NTF_STATE_CHANGE(state)  MNS_TRACE((MNS_P_SRV_MNS, NST_E_STATE_CHANGE, 2, NST_P1_NTF_STATE, mns.events.ntf_state))
#else
    #define T_NTF_STATE_CHANGE(state)
#endif

#if (MNS_DEBUG & NST_C_ASSERTS)
    #define FAILED_ASSERT()     MNS_TRACE((MNS_P_SRV_MNS, NST_E_ASSERT, 1, __LINE__))
    #define ASSERT(exp)         if(!(exp)) FAILED_ASSERT()
#else
    #define FAILED_ASSERT()
    #define ASSERT(exp)
#endif

#define TAKE_MS()       WAIT4MX(MX_MNS_SRV)
#define GIVE_MS()       REL_MX(MX_MNS_SRV)
#define TAKE_SERVICES() WAIT4MX(MX_MNS_PS)
#define GIVE_SERVICES() REL_MX(MX_MNS_PS)
#define TAKE_EVENTS()   WAIT4MX(MX_MNS_PE)
#define GIVE_EVENTS()   REL_MX(MX_MNS_PE)
#define TAKE_MNS()      WAIT4MX(MX_MNS_CS)
#define GIVE_MNS()      REL_MX(MX_MNS_CS)
#define TAKE_TM()       WAIT4MX(MX_MNS_TM)
#define GIVE_TM()       REL_MX(MX_MNS_TM)

#define SCHEDULE_RETRY(flag) { TAKE_MNS(); mns.retry |= flag; GIVE_MNS(); }
#define RETRY_DONE(flag)     { TAKE_MNS(); mns.retry &= (word)(~flag); GIVE_MNS(); }
#define PROCESS_RETRY(retry_flag, retry_func) { if ((ERR_NO == result) && (retry & retry_flag)) { RETRY_DONE(retry_flag); result = retry_func(); }}

/*
================================================================================
    Module Internal Variables
================================================================================
*/

#ifdef MNS_MIN
    /*! Data variable of the MNS module */
    static MnsData mns;

    #include "events.tab"

  #if (!defined PACKET_COMMUNICATION_ONLY) && (!defined DISABLE_ES_0201XX_025)
    static _CONST byte mns_disable_scerror[12] = {0x00, 0x00, 0x00, 0x0B, 
                                                   0xEC, 0x04, 0x00, 0x00, 
                                                   0x00, 0x00, 0xFF, 0x01};
  #endif

  #ifdef ENABLE_HF_340
    static _CONST  byte MnsWA340Cmd0[2] = {0x01,0x00};
    static _CONST  byte MnsWA340Cmd1[6] = {0x00,0x00,0x02,0x51,0x01,0x00};

    static HMBMBUF MnsWA340Hndl1; 
    static HMBMBUF MnsWA340Hndl2; 
    static HMBMBUF MnsWA340Hndl3;
    static bool MnsWA340Enabled;
    static bool MnsWA340PatchBusy;
    static bool MnsWA340PatchRetry;
  #endif  /* #ifdef ENABLE_HF_340 */


#endif  /* #ifdef MNS_MIN */


#include "versions.tab"

/*
================================================================================
    Function Implementations
================================================================================
*/


#ifdef MNS_112
/*!
  * \brief      Gets the current NetService version
  * \param[out] mns_ver Pointer to a byte array with a minimum size of 4, which
  *                     contains the current NetServices version.
  *                     Format: major, minor, release, step
  */
void MnsGetNetServicesVersion(byte *mns_ver)
{
    MNS_MEM_CPY(mns_ver, (byte *)mns_valid_versions[0].ns_ver,
              (word)sizeof(mns_valid_versions[0].ns_ver));
}
#endif


#ifdef MNS_0

void InitNetServices(void)
{
  #ifndef PACKET_COMMUNICATION_ONLY
    TMsgTx *temp_msg_ptr;
    HMBMBUF temp_msg_handle;
  #endif

    T_API_ENTRY(MNS_0);

  #ifndef PACKET_COMMUNICATION_ONLY
    temp_msg_ptr    = NULL;
    temp_msg_handle = NULL;
  #endif

    TAKE_MS();

    TAKE_SERVICES();
        mns.services.pending = MNS_P_NONE;
        mns.services.latest  = MNS_P_NONE;
    GIVE_SERVICES();

    MnsInit();
    MisInit(&(mns.config.lld), &(mns.config.pms), &(mns.config.msg)
            #if defined(DATA_TX_USER_PAYLOAD) && defined(ADS_MIN)
            , &(mns.config.wads)
            #endif
            #ifdef MDM_MIN
            , &(mns.config.mdm)
            #endif
            #ifdef GAS_MIN
            , &(mns.config.gas)
            #endif
            );

    #ifdef NS_MNS_MNS2
    mns.config.msg.rx_complete_fptr = MsgRxOutMsg;
    #endif

    #ifdef MNS_104
    temp_msg_ptr    = MsgGetTxPtrExt(0);
    /*lint -e{413} See ASSERT */
    temp_msg_handle = temp_msg_ptr ? MbmGetHandleByMsgPtr(temp_msg_ptr) : NULL;
    ASSERT(temp_msg_ptr && temp_msg_handle);
    if(temp_msg_ptr && temp_msg_handle)
    {
        TAKE_TM();
        mns.wd.msg_ptr = temp_msg_ptr;
        mns.wd.handle  = temp_msg_handle;
        mns.wd.msg_ptr->Tgt_Adr   = MSG_TGT_INIC;
        mns.wd.msg_ptr->FBlock_ID = FBLOCK_INIC;
        mns.wd.msg_ptr->Inst_ID   = 0;
        mns.wd.msg_ptr->Func_ID   = FUNCID_INIC_WATCHDOG;
        mns.wd.msg_ptr->Operation = OP_START;
        GIVE_TM();
    }
    #endif

    #ifdef ENABLE_HF_340
    temp_msg_ptr    = MsgGetTxPtrExt(0);
    temp_msg_handle = temp_msg_ptr ? MbmGetHandleByMsgPtr(temp_msg_ptr) : NULL;
    ASSERT(temp_msg_ptr && temp_msg_handle);
    if(temp_msg_ptr && temp_msg_handle)
    {
        MnsWA340Hndl1          = temp_msg_handle;
        temp_msg_ptr->Tgt_Adr   = MSG_TGT_INIC;
        temp_msg_ptr->FBlock_ID = FBLOCK_INIC;
        temp_msg_ptr->Inst_ID   = 0;
        temp_msg_ptr->Func_ID   = 0xE01;
        temp_msg_ptr->Operation = OP_SET;
        temp_msg_ptr->Data      = (byte *) &MnsWA340Cmd0[0];
        temp_msg_ptr->Length    = 1;
    }

    temp_msg_ptr    = MsgGetTxPtrExt(0);
    temp_msg_handle = temp_msg_ptr ? MbmGetHandleByMsgPtr(temp_msg_ptr) : NULL;
    ASSERT(temp_msg_ptr && temp_msg_handle);
    if(temp_msg_ptr && temp_msg_handle)
    {
        MnsWA340Hndl2          = temp_msg_handle;
        temp_msg_ptr->Tgt_Adr   = MSG_TGT_INIC;
        temp_msg_ptr->FBlock_ID = FBLOCK_INIC;
        temp_msg_ptr->Inst_ID   = 0;
        temp_msg_ptr->Func_ID   = FUNCID_INIC_MEMORY;
        temp_msg_ptr->Operation = OP_SET;
        temp_msg_ptr->Data      = (byte *) &MnsWA340Cmd1[0];
        temp_msg_ptr->Length    = 6;
    }

    temp_msg_ptr    = MsgGetTxPtrExt(0);
    temp_msg_handle = temp_msg_ptr ? MbmGetHandleByMsgPtr(temp_msg_ptr) : NULL;
    ASSERT(temp_msg_ptr && temp_msg_handle);
    if(temp_msg_ptr && temp_msg_handle)
    {
        MnsWA340Hndl3          = temp_msg_handle;
        temp_msg_ptr->Tgt_Adr   = MSG_TGT_INIC;
        temp_msg_ptr->FBlock_ID = FBLOCK_INIC;
        temp_msg_ptr->Inst_ID   = 0;
        temp_msg_ptr->Func_ID   = 0xE01;
        temp_msg_ptr->Operation = OP_SET;
        temp_msg_ptr->Data      = (byte *) &MnsWA340Cmd0[1];
        temp_msg_ptr->Length    = 1;
    }

    MnsWA340Enabled    = MNS_FALSE;
    MnsWA340PatchRetry = MNS_FALSE;
    MnsWA340PatchBusy  = MNS_FALSE;
    #endif

    mns.forced_reset   = MNS_FALSE;

    #ifdef AMS_MIN
        MsgInit(&(mns.config.msg));
    #endif
    #ifdef MCS_MIN
        WmcsInit();
    #endif
    #ifdef ADS_MIN
        DataInit(&(mns.config.wads));
    #endif
    #ifdef VMSV_MIN
        VmsvInit(&(mns.config.vmsv));
    #endif

    #ifdef SCM_MIN
        ScmInit(&(mns.config.scm));
    #endif

    #ifdef NS_MNS_MHP
        MhpInit();
    #endif
    #ifdef NS_MNS_PMHS
        PacketInit();
    #endif
    #ifdef AAM_MIN
        AamInit();
    #endif

    GIVE_MS();
    T_API_EXIT(MNS_0);
}
#endif

#ifdef MNS_1
/*!
  * \brief      The service function of MOST NetServices Kernel.
  * \details    The callback function \c general.on_most_service_request_fptr() will be fired as soon
  *             as the application shall asynchronously call MostService(). During the execution of MostService()
  *             the callback may also be fired to signal that the MOST NetServices need further calls of MostService()
  *             in order to work off it's internal events. By setting \c general.burst to a value greater than "one"
  *             MostService() can process multiple events while it is only called once.
  */
void MostService(void)
{
    word service_to_run;
    byte count;
    bool request_flag;
    bool ms_released;

    T_API_ENTRY(MNS_1);

    count = 0;
    ms_released = MNS_FALSE;

    TAKE_MS();

    TAKE_TM();
    MnsTMDiff();
    GIVE_TM();

    do
    {
        if (MNS_FALSE == MnsCheckTermination())
        {
            service_to_run = MnsGetNextServiceToCall();
            request_flag   = MNS_FALSE;

            T_MOST_SERVICE(service_to_run);

            switch (service_to_run)
            {
                #ifdef PMS_MIN
                    case MNS_P_SRV_PMS:
                        PmsService();
                        break;
                #endif

                #ifdef MIS_MIN
                case MNS_P_SRV_MIS:
                    MisService();
                    break;
                #endif

                #ifdef MNS_MIN
                case MNS_P_SRV_MNS:
                    ms_released = MnsService();
                    if (MNS_FALSE != ms_released)
                    {
                        T_API_EXIT(MNS_1);
                        return;
                    }
                    break;
                #endif

                #ifdef AMS_MIN
                case MNS_P_SRV_AMS:
                    MsgService();
                    break;
                #endif
                #ifdef MCS_MIN
                case MNS_P_SRV_WMCS:
                    WmcsService();
                    break;
                #endif
                #ifdef ADS_MIN
                    case MNS_P_SRV_WADS:
                        DataService();
                        break;
                #endif
                #ifdef VMSV_MIN
                    case MNS_P_SRV_VMSV:
                        VmsvService();
                        break;
                #endif

                #ifdef SCM_MIN
                    case MNS_P_SRV_WSCM:
                        ScmService();
                        break;
                #endif

                #ifdef GAS_MIN
                    case MNS_P_SRV_GAS:
                        GasService();
                        break;
                #endif

                #ifdef AAM_MIN
                    case MNS_P_SRV_AAM:
                        AamService();
                        break;
                #endif

                default:
                    /* unknown service */
                    ASSERT(!service_to_run);
                    break;
            }

            TAKE_SERVICES();
            request_flag = (MNS_P_NONE != mns.services.pending) ? MNS_TRUE : MNS_FALSE;
            GIVE_SERVICES();

            count++;
            if (MNS_FALSE != request_flag)
            {
                if (count >= mns.config.general.burst)
                {
                    ASSERT(mns.config.general.most_service_request_fptr);
                    if (mns.config.general.most_service_request_fptr)
                    {
                        mns.config.general.most_service_request_fptr();
                    }
                }
            }
            else
            {
                count = mns.config.general.burst;
            }
        }
        else
        {
            count = mns.config.general.burst;
        }

    } while (count < mns.config.general.burst);

    MnsTMHandleEvent(MNS_TM_EV_SCHEDULE);

    GIVE_MS();

    T_API_EXIT(MNS_1);
}
#endif

#ifdef MNS_2
void MnsSetPendingService(word service_flag)
{
    bool request_flag;

    T_LIB_ENTRY(MNS_2);

    request_flag = MNS_FALSE;

    TAKE_MNS();
    if((MNS_FALSE != mns.stopped) && (MNS_FALSE != mns.signaled_ns_stopped))
    {
        request_flag = MNS_TRUE;
    }
    GIVE_MNS();

    if (MNS_FALSE == request_flag)
    {
        T_MOST_REQUEST(service_flag);

        TAKE_SERVICES();
        request_flag = (MNS_P_NONE == mns.services.pending) ? MNS_TRUE : MNS_FALSE;
        mns.services.pending |= service_flag;
        GIVE_SERVICES();

        if (MNS_FALSE != request_flag)
        {
            ASSERT(mns.config.general.most_service_request_fptr);
            if (mns.config.general.most_service_request_fptr)
            {
                mns.config.general.most_service_request_fptr();
            }
        }
    }

    T_LIB_EXIT(MNS_2);
}
#endif

#ifdef MNS_3
void MnsSetPendingEventFlag(word event, word pending_mutex_id,
                            word *pending_ptr, word service_to_call)
{
    bool request_flag;

    T_LIB_ENTRY(MNS_3);

    request_flag = MNS_FALSE;

    ASSERT(pending_ptr);

    if(MX_INVALID != pending_mutex_id)
    {
        WAIT4MX(pending_mutex_id);
    }
    request_flag = (MNS_P_NONE == *pending_ptr) ? MNS_TRUE : MNS_FALSE;
    *pending_ptr |= event;
    if(MX_INVALID != pending_mutex_id)
    {
        REL_MX(pending_mutex_id);
    }

    if (MNS_FALSE != request_flag)
    {
        MnsSetPendingService(service_to_call);
    }

    T_LIB_EXIT(MNS_3);
}
#endif

#ifdef MNS_4
word MnsGetNextEventFlagToCall(word pending_mutex_id, word *pending_ptr,
                               word *latest_called_ptr, word first,
                               word last)
{
    word candidate;

    T_LIB_ENTRY(MNS_4);

    candidate = MNS_P_NONE;

    ASSERT(pending_ptr);
    ASSERT(latest_called_ptr);

    if(MX_INVALID != pending_mutex_id)
    {
        WAIT4MX(pending_mutex_id);
    }
    if (MNS_P_NONE != *pending_ptr)
    {
        bool match = MNS_FALSE;
        bool cycle = MNS_FALSE;

        candidate = (MNS_P_NONE != *latest_called_ptr) ?
                     *latest_called_ptr : last;

        while(MNS_FALSE == match)
        {
            if (last == candidate)
            {
                if(MNS_FALSE == cycle)
                {
                    candidate = first;
                    cycle = MNS_TRUE;
                }
                else
                {
                    candidate = MNS_P_NONE;
                    break;
                }
            }
            else
            {
                candidate = (word)((word)candidate << 1);
            }

            match = (candidate & (*pending_ptr)) ? MNS_TRUE : MNS_FALSE;
        }
    }

    if (MNS_P_NONE != candidate)
    {
        *latest_called_ptr = candidate;
        *pending_ptr &= (word) ~candidate;
    }
    if(MX_INVALID != pending_mutex_id)
    {
        REL_MX(pending_mutex_id);
    }

    T_LIB_EXIT(MNS_4);
    return(candidate);
}
#endif

#ifdef MNS_5
static word MnsGetNextServiceToCall(void)
{
    static bool run_high_prio = MNS_FALSE;
    word result;

    T_MOD_ENTRY(MNS_5);

    result = MNS_P_NONE;

    if (MNS_FALSE == run_high_prio)
    {
        word latest_called_dummy = MNS_P_NONE;

        result = MnsGetNextEventFlagToCall(MX_MNS_PS,
                                           &mns.services.pending,
                                           &latest_called_dummy,
                                           MNS_P_SRV_PMS,
                                           MNS_P_SRV_MNS);
        run_high_prio = (MNS_P_NONE != result) ? MNS_TRUE : MNS_FALSE;
    }
    else
    {
        run_high_prio = MNS_FALSE;
    }

    if (MNS_FALSE == run_high_prio)
    {
        result = MnsGetNextEventFlagToCall(MX_MNS_PS,
                                           &mns.services.pending,
                                           &mns.services.latest,
                                           MNS_P_SRV_FIRST,
                                           MNS_P_SRV_LAST);
    }

    T_MOD_EXIT(MNS_5);
    return(result);
}
#endif

#ifdef MNS_6
static bool MnsService(void)
{
    word event_to_handle;
    bool request_flag;

    T_MOD_ENTRY(MNS_6);

    event_to_handle = MnsGetNextEventToHandle();
    request_flag    = MNS_FALSE;

    T_SERVICE(event_to_handle);

    switch (event_to_handle)
    {
        case MNS_P_BUF_FREED:
            #ifndef PACKET_COMMUNICATION_ONLY
            if ((ERR_NO == MnsRetry()) &&
                mns.config.general.on_buf_freed_fptr)
            {
                if (EHCISTATE_IS_ATTACHED())
                {
                    mns.config.general.on_buf_freed_fptr();
                }
                else  /* can't call on_buf_freed callback */
                {
                    TAKE_MNS();
                    mns.buf_freed = MNS_TRUE;      /* need to re-trigger callback when attached */
                    GIVE_MNS();
                }
            }

            #else
            if (mns.config.general.on_buf_freed_fptr)
            {
                mns.config.general.on_buf_freed_fptr();
            }
            #endif
            break;

        case MNS_P_WATCHDOG_TIMER:
            {
                bool wd_active;

                TAKE_MNS();
                wd_active = (MNS_TM_WD_DISABLE != mns.config.general.watchdogmode.timeout)
                            ? MNS_TRUE : MNS_FALSE;
                GIVE_MNS();
                if (MNS_FALSE != wd_active)
                {
                    #ifndef PACKET_COMMUNICATION_ONLY
                    MnsFireWatchdogTrigger();
                    #endif
                }
            }
            break;

        case MNS_P_ALL_SERVICES_READY:
            #ifdef MNS_AVOID_ATTACH
                MnsReportInitComplete();
            #else
            (void)MnsEHCIGoSemiProtected();
            #endif
            break;

        #ifndef MNS_AVOID_ATTACH
        case MNS_P_INIT_TIMER:
        {
            byte count;
            bool mns_stopped;
            MostClearTimer(&(mns.timer.init));
            MnsReportError(NSR_E_INIT_TIMEOUT);
            MnsReportError(NSR_E_REATTACH);
            #ifdef MNS_115
            MnsCheckNbminNotification();
            #endif

            GIVE_MS();

            MnsPrepareReInit();

            count = MisGetResetCount();
            mns_stopped = mns.stopped;
            InitNetServices();
            mns.stopped = mns_stopped;
            MisSetResetCount(count);
            T_MOD_EXIT(MNS_6);

            return(MNS_TRUE);
        }
        #endif

        case MNS_P_AS_GO_PROTECTED:
            (void)MnsEHCIGoProtected();
            break;

        #ifndef MNS_AVOID_ATTACH
        case MNS_P_AS_GO_SEMI_PROTECTED:
            (void)MnsEHCIGoSemiProtected();
            break;

        case MNS_P_AS_GO_ATTACHED:
            MnsEHCIGoAttached();
            break;

        #endif /* MNS_AVOID_ATTACH */

        case MNS_P_L2:
            #ifdef NS_MNS_MNS2
            {
                word sub_events = mns.events.pending_l2;
                mns.events.pending_l2 = MNS_P_NONE;

                T_L2_SERVICE(sub_events);
                if (MostServiceLayer2(0, sub_events))
                {
                    TAKE_EVENTS();
                    mns.events.pending_l2 |= MNS2_E_PEN_EXTERN;
                    GIVE_EVENTS();
                    MnsSetPendingEvent(MNS_P_L2);
                }
            }
            #endif
            break;

        case MNS_P_MHP:
            #ifdef NS_MNS_MHP
            {
                word sub_events = mns.events.pending_mhp;
                mns.events.pending_mhp = MNS_P_NONE;

                T_MHP_SERVICE(sub_events);
                if (MostServiceHigh(sub_events))
                {
                    TAKE_EVENTS();
                    mns.events.pending_mhp |= MNSH_E_PEN_EXTERN;
                    GIVE_EVENTS();
                    MnsSetPendingEvent(MNS_P_MHP);
                }
            }
            #endif
            break;

        case MNS_P_PMHS:
            #ifdef NS_MNS_PMHS
            {
                word sub_events = mns.events.pending_pmhs;
                mns.events.pending_pmhs = MNS_P_NONE;

                T_PMHS_SERVICE(sub_events);
                if (MostServicePacket(sub_events))
                {
                    TAKE_EVENTS();
                    mns.events.pending_pmhs |= MNSP_E_PEN_EXTERN;
                    GIVE_EVENTS();
                    MnsSetPendingEvent(MNS_P_PMHS);
                }
            }
            #endif
            break;

        case MNS_P_SRV_CHECK:
            {
                #ifdef AMS_MIN
                bool signal_net_on;
                #endif

                #ifdef MNS_73
                MnsCheckHigherLayers();
                #endif

                #ifdef AMS_MIN
                TAKE_MNS();
                signal_net_on = mns.signal_net_on;
                GIVE_MNS();

                if (MNS_FALSE != signal_net_on)
                {
                    MsgNIStateNetOn(MNS_TRUE);
                    TAKE_MNS();
                    mns.signal_net_on = MNS_FALSE;
                    GIVE_MNS();
                }
                #endif
            }
            break;

        default:
            FAILED_ASSERT();
            /* event_to_handle = MNS_P_NONE; commented to avoid PC-Lint warning 438  */
            break;
    }

    TAKE_EVENTS();
    request_flag = (MNS_P_NONE != mns.events.pending) ? MNS_TRUE : MNS_FALSE;
    GIVE_EVENTS();

    if (MNS_FALSE != request_flag)
    {
        MnsSetPendingService(MNS_P_SRV_MNS);
    }

    T_MOD_EXIT(MNS_6);
    return(MNS_FALSE);
}
#endif

#ifdef MNS_7
static _INLINE void MnsSetPendingEvent(word event_flag)
{
    T_MOD_ENTRY(MNS_7);
    T_REQUEST(event_flag);

    MnsSetPendingEventFlag(event_flag, MX_MNS_PE,
                           &mns.events.pending, MNS_P_SRV_MNS);

    T_MOD_EXIT(MNS_7);
}
#endif

/*! Returns the next event that must be handled. The function is a specialized
  * call of MnsGetNextEventFlagToCall() with the parameters for MNS. An
  * additional check ensures that the higher layers are only served when the
  * EHCIState and the NIState are correct (Attached and Net On).
  *
  * @author     SSemmler
  * @date       last update  6/6/2003
  * @version    2.0.0
  * @see        MnsSetPendingEvent
  * @see        MnsGetNextEventFlagToCall
  *
  * @return the ID of the event to service next
  */
#ifdef MNS_8
static word MnsGetNextEventToHandle(void)
{
    word result;
    bool run;

    T_MOD_ENTRY(MNS_8);

    result = MNS_P_NONE;
    run    = MNS_FALSE;

    TAKE_EVENTS();
    if ((MNS_FALSE == mns.services.ms_layer2) &&
        (mns.events.pending & MNS_P_L2))
    {
        mns.events.pending &= (word)(~MNS_P_L2);
    }
    if ((MNS_FALSE == mns.services.ms_packet_layers) &&
        (mns.events.pending & MNS_P_PACKET_LAYERS))
    {
        mns.events.pending &= (word)(~MNS_P_PACKET_LAYERS);
    }
    run = mns.events.pending ? MNS_TRUE : MNS_FALSE;
    GIVE_EVENTS();

    if (run)
    {
        result = MnsGetNextEventFlagToCall(MX_MNS_PE,
                                           &mns.events.pending,
                                           &mns.events.latest,
                                           MNS_P_FIRST, MNS_P_LAST);
    }

    T_MOD_EXIT(MNS_8);
    return(result);
}
#endif

#ifdef MNS_9
void MnsDistribEvent(word service, word event)
{
    byte          index;
    bool          service_match;
    MnsServicePtr service_ptr;
    MnsEventPtr   event_ptr;

    T_LIB_ENTRY(MNS_9);

    index         = 0;
    service_match = MNS_FALSE;
    service_ptr   = (MnsServicePtr) &mns_events[index];
    event_ptr     = NULL;

    while ((MNS_FALSE == service_match) && service_ptr->service)
    {
        if (service == service_ptr->service)
        {
            service_match = MNS_TRUE;
            index         = 0;
            event_ptr     = &(service_ptr->entry_table[index]);

            while (event_ptr->source_event)
            {
                if (event == event_ptr->source_event)
                {
                    if (event_ptr->cb_ptr)
                    {
                        event_ptr->cb_ptr(event_ptr->target_event);
                    }
                }
                event_ptr = &(service_ptr->entry_table[++index]);
            }
        }
        else
        {
            service_ptr = (MnsServicePtr) &mns_events[++index];
        }
    }

    T_LIB_EXIT(MNS_9);
}
#endif

#ifdef MNS_10
static void MnsInit(void)
{
    static bool first_init = MNS_TRUE;
    word temp_tick_count;

    T_MOD_ENTRY(MNS_10);

    temp_tick_count = 0;

    T_INIT();

    TAKE_EVENTS();
    mns.events.pending     = MNS_P_NONE;
    mns.events.latest      = MNS_P_NONE;
    #ifdef NS_MNS_MNS2
    mns.events.pending_l2 = MNS_P_NONE;
    #endif

    #ifdef NS_MNS_MHP
    mns.events.pending_mhp = MNS_P_NONE;
    #endif

    #ifdef NS_MNS_PMHS
    mns.events.pending_pmhs = MNS_P_NONE;
    #endif
    GIVE_EVENTS();

    mns.stopped                 = MNS_FALSE;
    mns.signaled_ns_stopped     = MNS_FALSE;
    mns.signal_net_on           = MNS_FALSE;
    mns.services.ms_init_mask   = MNS_P_NONE;
    T_INIT_PHASE_CHANGE(MNS_P_NONE);
    mns.services.ms_reset_mask  = MNS_P_NONE;
    T_RESET_PHASE_CHANGE(MNS_P_NONE);

    if (mns.config.general.get_tick_count_fptr)
    {
        temp_tick_count = mns.config.general.get_tick_count_fptr();
    }
    else
    {
        /* get_tick_count_fptr is mandatory ! */
        FAILED_ASSERT();
    }

    TAKE_MNS();
    /* check for valid timeout */
    if (((MNS_TM_WD_MIN_TIMEOUT > mns.config.general.watchdogmode.timeout)  ||
         (MNS_TM_WD_MAX_TIMEOUT < mns.config.general.watchdogmode.timeout)) &&
        (MNS_TM_WD_DISABLE    != mns.config.general.watchdogmode.timeout))
    {
        mns.config.general.watchdogmode.timeout = MNS_TM_WD_MAX_TIMEOUT;
    }
    mns.watchdogmode.timeout_wdsm = mns.config.general.watchdogmode.timeout -
        (mns.config.general.watchdogmode.timeout >> MNS_TM_WDSM_POWER);

    mns.retry = 0;

    if(!mns.config.general.burst)
    {
        mns.config.general.burst = MNS_STD_BURST;
    }

    mns.shadow.ehci_state             = EHCISTATE_UNKNOWN;
    /* The version information is entered automatically by the
     * PrePreprocessor.
     */
    mns.shadow.version.public.ns[0]          = 0x03;
    mns.shadow.version.public.ns[1]          = 0x00;
    mns.shadow.version.public.ns[2]          = 0x05;
    mns.shadow.version.public.ns[3]          = 0xD0;

    mns.services.ms_comm_ready        = MNS_FALSE;
    mns.services.ms_base_ready        = MNS_FALSE;

#if (MNS_DEBUG & NST_C_INIT)
    {
        word config = 0;

        #ifdef PACKET_COMMUNICATION_ONLY
            config |= MNS_F_PACKET_COMMUNICATION_ONLY;
        #endif

        #ifdef MNS_IGNORE_VERSION_INFO
            config |= MNS_F_IGNORE_VERSION_CHECK;
        #endif

        T_MOST_INIT(MNS_P_SRV_ALL, config);
    }
#endif

    mns.services.ms_layer2          = MNS_FALSE;
    mns.services.ms_packet_layers   = MNS_FALSE;
    mns.buf_freed                   = MNS_FALSE;
    mns.remote_access               = MNS_DENY;
    if(MNS_FALSE != first_init)
    {
        mns.pending_mdm = MNS_FALSE;
        first_init      = MNS_FALSE;
    }
    GIVE_MNS();

    TAKE_TM();
    mns.tm.last_tick    = temp_tick_count;
    mns.tm.diff         = 0;
    mns.tm.min_timeout  = 0;
    mns.tm.scheduled_timeout = 0;
    #ifndef PACKET_COMMUNICATION_ONLY
    mns.tm.flags        = 0;
    #endif

    mns.wd.flags        = 0;
    mns.timer.first_ptr = NULL;

    #ifndef MNS_DISABLE_WD_SUPERVISION
    mns.watchdogmode.max_tx_busy_cnt = MNS_WD_SUPERVIS_CNT_DISABLE; /* disable counting during initialization */
    mns.watchdogmode.tx_busy_cnt     = MNS_WD_SUPERVIS_CNT_START;   /* reset counter */
    #endif
    GIVE_TM();

    MostRegisterTimer(&(mns.timer.init), MnsSetPendingEvent, MNS_P_INIT_TIMER);
    MostRegisterTimer(&(mns.timer.watchdog), MnsSetPendingEvent,
        MNS_P_WATCHDOG_TIMER);

    #ifndef PACKET_COMMUNICATION_ONLY
        MostSetTimer(&(mns.timer.init), MNS_INIT_TIMER, MNS_FALSE);
    #endif

    #ifdef PACKET_COMMUNICATION_ONLY
    (void)MnsEHCIGoProtected();
    #endif

    T_MOD_EXIT(MNS_10);
}
#endif

#ifdef MNS_11
/*lint -esym(438,old_state,new_state) avoid pc-lint warning 438: Last value assigned to variable not used*/
void MnsServiceInitComplete(byte phase, word service)
{
    word old_state;
    word new_state;

    T_LIB_ENTRY(MNS_11);

    switch (phase)
    {
        case MNS_PHASE_INIT:
            {
                bool comm_ready;
                bool base_ready;

                TAKE_MNS();
                old_state = mns.services.ms_init_mask;
                mns.services.ms_init_mask |= service;
                new_state = mns.services.ms_init_mask;
                comm_ready = mns.services.ms_comm_ready;
                base_ready = mns.services.ms_base_ready;
                GIVE_MNS();

                if(old_state != new_state)
                {
                    T_INIT_PHASE_CHANGE(new_state);

                    if ((MNS_FALSE == comm_ready) && (MNS_P_SRV_COMM ==
                                          (new_state & MNS_P_SRV_COMM)))
                    {
                        TAKE_MNS();
                        mns.services.ms_comm_ready = MNS_TRUE;
                        GIVE_MNS();

                        #ifdef MNS_60
                        (void)MnsRequestEHCIState();
                        #endif
                    }
                    if ((MNS_FALSE == base_ready) && (MNS_P_SRV_BASE ==
                                          (new_state & MNS_P_SRV_BASE)))
                    {
                        TAKE_MNS();
                        mns.services.ms_base_ready = MNS_TRUE;
                        GIVE_MNS();
                        #ifdef MNS_119
                        (void)MnsGASPhaseComplete();
                        #endif
                    }
                    if (MNS_P_SRV_ALL == (new_state & MNS_P_SRV_ALL))
                    {
                        T_INIT_PHASE_CHANGE(0xFFFF);
                        MnsSetPendingEvent(MNS_P_ALL_SERVICES_READY);
                    }
                }
            }
            break;

        case MNS_PHASE_RESET:
            TAKE_MNS();
            old_state = mns.services.ms_reset_mask;
            mns.services.ms_reset_mask |= service;
            new_state = mns.services.ms_reset_mask;
            GIVE_MNS();

            T_RESET_PHASE_CHANGE(new_state);

            #ifndef MNS_AVOID_ATTACH
            old_state &= MNS_P_SRV_ALL_RESET;       /* ignore services not relevant for transition */
            new_state &= MNS_P_SRV_ALL_RESET;

            if (old_state != new_state)
            {
                if (MNS_P_SRV_ALL_RESET == (new_state & MNS_P_SRV_ALL_RESET))
                {
                    T_RESET_PHASE_CHANGE(0xFFFF);
                    (void)MnsResetComplete();
                }
            }
            else
            {
                FAILED_ASSERT();                    /* reset is notified twice or by a service which is not relevant */
            }
            #endif
            break;

        default:
            FAILED_ASSERT();
            break;
    }


    T_LIB_EXIT(MNS_11);
}
/*lint +esym(438,old_state,new_state) avoid pc-lint warning 438: Last value assigned to variable not used*/
#endif

#ifdef MNS_12
bool MnsIsNSInitComplete(word mask)
{
    bool result;

    T_LIB_ENTRY(MNS_12);

    result = MNS_FALSE;

    TAKE_MNS();
    if ((mask & mns.services.ms_init_mask) == mask)
    {
        result = MNS_TRUE;
    }
    GIVE_MNS();

    T_LIB_EXIT(MNS_12);
    return(result);
}
#endif

#ifdef MNS_16
/*!
  * \brief      Schedule a timeout (event will be fired when timeout is reached).
  * \see        MostClearTimer
  * \see        MostGetTimer
  * \see        MostRegisterTimer
  * \see        MostUnregisterTimer
  * \relates    TMostTimer
  * \param[in]  timer_ptr      Pointer to the timer instance
  * \param[in]  milliseconds   Timeout to be set in milliseconds
  * \param[in]  periodic       MNS_TRUE if the timer is periodic
  */
void MostSetTimer(TMostTimer *timer_ptr, word milliseconds, bool periodic)
{
    T_API_ENTRY(MNS_16);
    MnsTMHandleEvent(MNS_TM_EV_UPDATE);
    TAKE_TM();
    timer_ptr->value = milliseconds;
    timer_ptr->periodic = periodic ? milliseconds : 0;

    if (milliseconds)
    {
        if ((milliseconds<mns.tm.min_timeout)|| (mns.tm.min_timeout==0))
        {
            mns.tm.min_timeout = milliseconds;
        }
    }

    GIVE_TM();
    MnsTMHandleEvent(MNS_TM_EV_SCHEDULE);

    T_API_EXIT(MNS_16);
}
#endif

#ifdef MNS_17
/*!
  * \brief      Get the current timeout value of a timer.
  * \see        MostClearTimer
  * \see        MostSetTimer
  * \see        MostRegisterTimer
  * \see        MostUnregisterTimer
  * \param[in]  timer_ptr    Pointer to the timer instance
  * \return     The remaining time in milliseconds until the timer elapses.
  */
word MostGetTimer(TMostTimer *timer_ptr)
{
    word temp;

    T_API_ENTRY(MNS_17);

    ASSERT(timer_ptr);

    TAKE_TM();
    temp = timer_ptr->value;
    GIVE_TM();
    T_API_EXIT(MNS_17);

    return(temp);
}
#endif

#ifdef MNS_20
void MnsReportError(word code)
{
    static bool inic_reset_case = MNS_FALSE;

    T_LIB_ENTRY(MNS_20);

    #ifndef PACKET_COMMUNICATION_ONLY
    if (NSR_E_REATTACH == code)
    {
        MostSetTimer(&(mns.timer.init), MNS_INIT_TIMER, MNS_FALSE);
    }
    #endif

    if ((MNS_FALSE != mns.stopped) && (NSR_E_NO_COMM == code))
    {

        TAKE_MNS();
        mns.shadow.ehci_state = EHCISTATE_UNKNOWN;
        GIVE_MNS();
    }

    if (NSR_EI_RESET == code)
    {
        inic_reset_case = MNS_TRUE;

        MnsSetPendingEvent(MNS_P_INIT_TIMER);

    }

    if ((MNS_FALSE != mns.stopped) && (NSR_E_REATTACH == code))
    {
        /* ignore reattach error when StopNetServices() was called before */
    }
    else if ((MNS_FALSE != inic_reset_case) && ((NSR_E_INIT_TIMEOUT == code) || (NSR_E_REATTACH == code) || (MNS_FALSE != mns.forced_reset)))
    {
        if (NSR_E_REATTACH == code)
        {
            inic_reset_case = MNS_FALSE;
        }

        mns.forced_reset = MNS_FALSE;               /* avoid error notification for an intentional INIC reset */
    }
    else if (mns.config.general.on_error_fptr)
    {
        mns.config.general.on_error_fptr(code);

        #ifdef MNS_113
        /* Not able to reach OnInitComplete ? */
        if(NSR_E_INIT_TIMEOUT == code)
        {
            mns.pending_mdm = MNS_TRUE;             /* Set pending flag for debug message */
        }
        else
        #endif  /* #ifdef MNS_113 */
        {
            #ifdef MNS_114
            /* Version conflict MNS-Version vs. INIC FW ? */
            if(NSR_E_INIC_VERSION == code)
            {
                MnsSendMdmMnsVersionConflict();     /* Send debug message */
            }
            #endif  /* #ifdef MNS_114 */
        }
    }
    T_LIB_EXIT(MNS_20);
}
#endif

#ifdef MNS_21
static void MnsReportInitComplete(void)
{
    T_LIB_ENTRY(MNS_21);

    #if (defined PACKET_COMMUNICATION_ONLY) && (defined NS_MNS_MHP)
        MnsStartPacketLayers();
    #endif

    if (mns.config.general.on_init_complete_fptr)
    {
        mns.config.general.on_init_complete_fptr();
    }
    T_LIB_EXIT(MNS_21);
}
#endif

#ifdef MNS_22
static byte MnsEHCIGoProtected(void)
{
    byte old_ehci_state;

    T_MOD_ENTRY(MNS_22);

    TAKE_EVENTS();
    mns.events.ntf_state = NTF_NONE;
    GIVE_EVENTS();

    TAKE_TM();
    mns.tm.scheduled_timeout = 0;
    GIVE_TM();

    TAKE_MNS();
    mns.shadow.version.public.hw = INIC_SHADOW_INVALID_BYTE;
    MNS_MEM_SET(mns.shadow.version.public.fw, INIC_SHADOW_INVALID_BYTE,
                (word)sizeof(mns.shadow.version.public.fw));
    MNS_MEM_SET(mns.shadow.version.public.fw_date, INIC_SHADOW_INVALID_BYTE,
                (word)sizeof(mns.shadow.version.public.fw_date));
    MNS_MEM_SET(mns.shadow.version.public.cs, INIC_SHADOW_INVALID_BYTE,
                (word)sizeof(mns.shadow.version.public.cs));
    MNS_MEM_SET(mns.shadow.version.public.api, INIC_SHADOW_INVALID_BYTE,
                (word)sizeof(mns.shadow.version.public.api));
    mns.shadow.version.ntf_mask = NTF_MASK;
    mns.shadow.version.feature_mask = INIC_SHADOW_INVALID_WORD;
    GIVE_MNS();

    TAKE_MNS();
    old_ehci_state = mns.shadow.ehci_state;
    mns.shadow.ehci_state = EHCISTATE_PROTECTED;
    mns.retry = 0;
    GIVE_MNS();
    T_EHCISTATE_CHANGE(EHCISTATE_PROTECTED);

    T_INIT_PHASE_CHANGE(mns.services.ms_init_mask);

    if (EHCISTATE_UNKNOWN != old_ehci_state)
    {
        MnsReportError(NSR_E_REATTACH);
    }

    MnsServiceInitComplete(MNS_PHASE_RESET, MNS_P_SRV_MNS);
    #ifdef PACKET_COMMUNICATION_ONLY
    MnsServiceInitComplete(MNS_PHASE_INIT, MNS_P_SRV_MNS);
    #endif

    #ifndef PACKET_COMMUNICATION_ONLY
    MostSetTimer(&(mns.timer.init), MNS_INIT_TIMER, MNS_FALSE);
    #endif

    T_MOD_EXIT(MNS_22);

    return(ERR_NO);
}
#endif


/*! Transition into Semi-Protected Mode.
  *
  * @author     SSemmler
  * @date       last update  2004
  * @version    2.0.2
  *
  * @return     ERR_NO or ERR_BUFOV (retry is scheduled)
  */
#ifdef MNS_23
static byte MnsEHCIGoSemiProtected(void)
{
    byte result;

    T_MOD_ENTRY(MNS_23);

    result = ERR_NO;

    if (MNS_FALSE != MnsIsNSInitComplete(MNS_P_SRV_ALL))
    {
        byte target_mode = MNS_GO_ATTACHED_MODE;

        if (mns.config.general.wakeup_query_fptr &&
            (MNS_FALSE != VmsvGetNCStateShutdown()))
        {
            target_mode = mns.config.general.wakeup_query_fptr();
        }

        if (MNS_GO_PROTECTED_MODE == target_mode)
        {
            StopNetServices();
        }
        else
        {
            TMsgTx *msg_ptr;

            msg_ptr = MsgGetTxPtrICM(1);	// Bug : Errata sheet 3.0.X-6 - 0300XX-016
            if (msg_ptr)
            {
                msg_ptr->Tgt_Adr   = MSG_TGT_INIC;
                msg_ptr->Func_ID   = FUNCID_INIC_EHCISTATE;
                msg_ptr->Inst_ID   = 0;
                msg_ptr->Operation = OP_SET;
                msg_ptr->Length    = 1;
                msg_ptr->Data[0]   = EHCISTATE_ATTACHED;

                MsgSend3(msg_ptr);
            }
            else
            {
                SCHEDULE_RETRY(MNS_RETRY_GO_SEMI_PROTECTED);
                result = ERR_BUFOV;
            }
        }
    }

    T_MOD_EXIT(MNS_23);

    return(result);
}
#endif

#ifdef MNS_24
static void MnsEHCIGoAttached(void)
{
    bool call_buf_freed_cb;

    T_MOD_ENTRY(MNS_24);

    call_buf_freed_cb = MNS_FALSE;

    if (MNS_FALSE != MnsIsNSInitComplete(MNS_P_SRV_ALL))
    {
        #ifndef PACKET_COMMUNICATION_ONLY
        MostClearTimer(&(mns.timer.init));
        #endif


        TAKE_MNS();
        if (MNS_FALSE != mns.buf_freed)  /* a buffer has been freed during re-attached */
        {
            mns.buf_freed = MNS_FALSE;
            call_buf_freed_cb = MNS_TRUE;
        }
        GIVE_MNS();

        if (MNS_FALSE != call_buf_freed_cb)
        {
            (void)MnsSetPendingEvent(MNS_P_BUF_FREED);   /* trigger callback general.on_buf_freed */
        }
    }
    T_MOD_EXIT(MNS_24);
}
#endif

#ifdef MNS_26
void MnsHandleVersionInfo(const TMsgRx *msg_ptr)
{
    bool okay;

    T_MOD_ENTRY(MNS_26);

    okay  = MNS_FALSE;

    ASSERT(msg_ptr);

    /* get the full info message ... index 0,0 */
    TAKE_MNS();
    if (!(msg_ptr->Data[0]) && !(msg_ptr->Data[1]))
    {
        _CONST MnsVerCheckLine *current_ptr = mns_valid_versions;
        byte count;
        bool next;

        while ((current_ptr->hw_ver[0] != (byte)0) && (MNS_FALSE == okay))
        {
            count = (byte)4;
            next = MNS_FALSE;

            while (count && (MNS_FALSE == next))
            {
                count--;
                next = MNS_IS_NOT_VALID(current_ptr->ns_ver[count], mns.shadow.version.public.ns[count]);
            }
            count = (byte)5;
            while (count && (MNS_FALSE == next))
            {
                count--;
                next = MNS_IS_NOT_VALID(msg_ptr->Data[count+(byte)2], current_ptr->hw_ver[count]);
            }
            if (MNS_FALSE == next)
            {
                mns.shadow.version.public.hw = current_ptr->chip_id;
            }
            count = (byte)3;
            while (count && (MNS_FALSE == next))
            {
                count--;
                next = MNS_IS_NOT_VALID(msg_ptr->Data[count+(byte)8], current_ptr->fw_ver[count]);
            }
            if (MNS_FALSE == next)
            {
                MNS_MEM_CPY(mns.shadow.version.public.prod_name, &(msg_ptr->Data[2]),(word)3);
                mns.shadow.version.public.chip_id    = msg_ptr->Data[5];
                mns.shadow.version.public.hw_release = msg_ptr->Data[6];
                MNS_MEM_CPY(mns.shadow.version.public.fw, &(msg_ptr->Data[8]),(word)3);
                MNS_MEM_CPY(mns.shadow.version.public.fw_date, &(msg_ptr->Data[11]),(word)3);
                MNS_MEM_CPY(mns.shadow.version.public.cs, &(msg_ptr->Data[14]),(word)3);
                MNS_MEM_CPY(mns.shadow.version.public.api, &(msg_ptr->Data[17]),(word)3);
                mns.shadow.version.ntf_mask = current_ptr->ntf_mask;
                #ifndef SCM_MIN
                mns.shadow.version.ntf_mask &= ~NTF_MASK_WSCM;
                #endif

                #ifndef VMSV_MIN
                mns.shadow.version.ntf_mask &= ~NTF_MASK_VMSV;
                #endif
                mns.shadow.version.feature_mask = current_ptr->feature_mask;
                okay = MNS_TRUE;
            }
            current_ptr++;
        }
    }
    GIVE_MNS();

    #ifndef MNS_IGNORE_VERSION_INFO
    /* MOST 50: support FW versions >= 1.6.4 */
    if (   (MNS_FALSE != okay)
        && (MNS_FALSE != MostIsSupported(NSF_MOST_50))
        && ((byte)0x08 == msg_ptr->Data[2])         /* OS81092 */
        && ((byte)0x10 == msg_ptr->Data[3])
        && ((byte)0x92 == msg_ptr->Data[4])
        && ((byte)0x01 == msg_ptr->Data[8])         /* FW 1.6.4 */
        && ((byte)0x06 == msg_ptr->Data[9])
        && ((byte)0x04 >  msg_ptr->Data[10]))
    {
        okay = MNS_FALSE;
    }
    #endif  /* #ifndef MNS_IGNORE_VERSION_INFO */

    if (MNS_FALSE != okay)
    {
        T_INIC_INIT();
        MnsNtfCheck(NTF_VERSIONINFO);

        MnsServiceInitComplete(MNS_PHASE_INIT, MNS_P_SRV_MNS);          /* VersionInfo is accessible by other services */

        #ifdef GAS_MIN
        if ((MNS_FALSE != MnsIsNSInitComplete(MNS_P_SRV_BASE & (word)(~MNS_P_SRV_GAS)))
            && (MNS_FALSE == MnsIsNSInitComplete(MNS_P_SRV_GAS)))       /* all base services but GOS were started */
        {
            MnsDistribEvent(MNS_P_SRV_MNS, MNS_P_START_GAS_MODE);       /* VersionInfo.Status prior to GAS mode (minimal notification) */
        }
        else if (MNS_FALSE == MnsIsNSInitComplete(MNS_P_SRV_BASE))
        {
            FAILED_ASSERT();                                            /* ignore unexpected VersionInfo.Status() */
        }
        else                                                            /* all base services are available now */
        #endif
        {
            MnsDistribEvent(MNS_P_SRV_MNS, MNS_P_AS_GO_SEMI_PROTECTED); /* VersionInfo.Status when GAS mode was finished */

            #ifdef ENABLE_HF_340
            if (  MostIsSupported(NSF_MOST_50) 
                && ((byte)0x08 == msg_ptr->Data[2])         /* OS81092 */
                && ((byte)0x10 == msg_ptr->Data[3])
                && ((byte)0x92 == msg_ptr->Data[4])
                && (WA340_VERSION_NUMBER_MAJOR  == msg_ptr->Data[8] ) 
                && (WA340_VERSION_NUMBER_MINOR  == msg_ptr->Data[9] ) 
                && (WA340_VERSION_NUMBER_BUILD >= msg_ptr->Data[10]) )       /* FW version <= 1.6.4 */
            {
                if (MNS_FALSE == MnsWA340Enabled)                          /* first attach ? */
                {
                    MnsWA340Enabled = MNS_TRUE;
                    if (NISTATE_NET_OFF == VmsvGetNIState())
                    {                                                      /* perform fix on first attach   */
                        (void)MnsWA340Fix();                               /* if device is in NET_OFF state */
                    }                                                      /* NIState was transmitted before version info */
                }

            }
            #endif

            #ifdef MNS_105
            if (MNS_FALSE != MostIsSupported(NSF_OS81050_V01_10_07))
            {
                (void)MnsDisableSCError();
            }
            #endif /* #ifdef MNS_105 */

            #ifdef MNS_113
            if (MNS_FALSE != mns.pending_mdm)       /* MOST Debug Message "Initialization Timeout" is pending */
            {                                       /* and GAS mode is already finished ? */
                MnsSendMdmMnsInitTimeout();         /* Send debug message */
                mns.pending_mdm = MNS_FALSE;        /* Reset pending flag */
            }
            #endif  /* #ifdef MNS_113 */
        }
    }
    else
    {
        {
            MnsReportError(NSR_E_INIC_VERSION);
        }
    }

    T_MOD_EXIT(MNS_26);
}
#endif

#ifdef MNS_27
void MnsHandleEHCIStateStatus(const TMsgRx *msg_ptr)
{
    byte current_state;

    T_MOD_ENTRY(MNS_27);

    ASSERT(msg_ptr);

    TAKE_MNS();
        current_state = mns.shadow.ehci_state;
    GIVE_MNS();

    if ((EHCISTATE_UNKNOWN == current_state) &&
        (EHCISTATE_PROTECTED != msg_ptr->Data[0]))
    {
        (void)MnsForceProtected();
    }
    else
    {
        switch (msg_ptr->Data[0])
        {
            case EHCISTATE_ATTACHED:
                if (EHCISTATE_ATTACHED != current_state)
                {
                    TAKE_MNS();
                    mns.shadow.ehci_state = EHCISTATE_ATTACHED;
                    GIVE_MNS();
                    T_EHCISTATE_CHANGE(EHCISTATE_ATTACHED);

                    /* MnsReportInitComplete() must be here, so it is reported
                     * BEFORE the delayed callbacks
                     */
                    MnsReportInitComplete();
                    MnsDistribEvent(MNS_P_SRV_MNS, MNS_P_AS_GO_ATTACHED);
                }
                break;

            case EHCISTATE_SEMI_PROTECTED:
                #ifdef MNS_118
                if (!MnsIsNSInitComplete(MNS_P_SRV_BASE))   /* EHCIState.Status response prior to GAS mode ? */
                {
                    (void)MnsRequestVersionInfo();
                }
                #endif
                break;

            default:
                ASSERT(EHCISTATE_PROTECTED == msg_ptr->Data[0]);
                T_TRANSITION_CAUSE(msg_ptr->Length, msg_ptr->Data[1]);
                TAKE_MNS();
                mns.services.ms_init_mask &= (word)(~(MNS_P_SRV_SHADOW | MNS_P_SRV_GAS | MNS_P_SRV_MNS));
                mns.services.ms_reset_mask = MNS_P_NONE;
                mns.services.ms_base_ready = MNS_FALSE;
                GIVE_MNS();
                T_RESET_PHASE_CHANGE(MNS_P_NONE);
                MnsDistribEvent(MNS_P_SRV_MNS, MNS_P_AS_GO_PROTECTED);
                PmsSetFifoInterface( PMS_M_FIFO_CTRL, mns.config.lld.ctrl_interface_id );
                PmsSetFifoInterface( PMS_M_FIFO_DATA, mns.config.lld.data_interface_id );
                break;
        }

        MnsSetPendingEvent(MNS_P_SRV_CHECK);

        if ((EHCISTATE_ATTACHED == msg_ptr->Data[0]) &&
            (MNS_NET_ON == MostGetState()))
        {
            #ifdef AMS_MIN
            #ifdef SCM_MIN
            byte boundary = ScmGetBoundary();
            bool bdvalid  = (INIC_SHADOW_INVALID_BYTE != boundary) &&
                            ((MNS_FALSE != MostIsSupported(NSF_MOST_25)) ? ((0x00 != boundary) ? MNS_TRUE : MNS_FALSE) : MNS_TRUE);
            #else
            bool bdvalid  = MNS_TRUE;
            #endif

            if (MNS_FALSE != bdvalid)
            {
                MnsSignalNetOn();
            }
            #endif
        }
        else
        {
            #ifdef AMS_MIN
                MsgNIStateNetOn(MNS_FALSE);
            #endif
        }

        if ((EHCISTATE_ATTACHED == msg_ptr->Data[0]) &&
            (MNS_NET_ON == MostGetState())           &&
            (MNS_FALSE != MnsPMReady()))
        {
            #ifdef ADS_10
                DataNIStateNetOn(MNS_TRUE);
            #endif
        }
        else
        {
            #ifdef ADS_10
                DataNIStateNetOn(MNS_FALSE);
            #endif
        }

    }

    T_MOD_EXIT(MNS_27);
}
#endif

#ifdef MNS_29
void MnsRequestLayer2(word flags)
{
    (void) flags;
    T_MOD_ENTRY(MNS_29);
    TAKE_EVENTS();
    mns.events.pending_l2 |= MNS2_E_REQ_EXTERN;
    GIVE_EVENTS();
    T_L2_REQUEST(MNS2_E_REQ_EXTERN);
    MnsSetPendingEvent(MNS_P_L2);
    T_MOD_EXIT(MNS_29);
}
#endif

#ifdef MNS_30
void MnsRequestLayer2Timer(word flags)
{
    (void) flags;
    T_MOD_ENTRY(MNS_30);
    TAKE_EVENTS();
    mns.events.pending_l2 |= MNS2_E_TIMER_EXTERN;
    GIVE_EVENTS();
    T_L2_REQUEST(MNS2_E_TIMER_EXTERN);
    MnsSetPendingEvent(MNS_P_L2);
    T_MOD_EXIT(MNS_30);
}
#endif

#ifdef MNS_31
void MnsRequestHigh(word flags)
{
    (void) flags;
    T_MOD_ENTRY(MNS_31);
    TAKE_EVENTS();
    mns.events.pending_mhp |= MNSH_E_REQ_EXTERN;
    GIVE_EVENTS();
    T_MHP_REQUEST(MNSH_E_REQ_EXTERN);
    MnsSetPendingEvent(MNS_P_MHP);
    T_MOD_EXIT(MNS_31);
}
#endif

#ifdef MNS_32
void MnsRequestHighTimerTx(void)
{
    T_MOD_ENTRY(MNS_32);
    TAKE_EVENTS();
    mns.events.pending_mhp |= MNSH_E_TIMER_TX_EXTERN;
    GIVE_EVENTS();
    T_MHP_REQUEST(MNSH_E_TIMER_TX_EXTERN);
    MnsSetPendingEvent(MNS_P_MHP);
    T_MOD_EXIT(MNS_32);
}
#endif

#ifdef MNS_33
void MnsRequestHighTimerRx(void)
{
    T_MOD_ENTRY(MNS_33);
    TAKE_EVENTS();
    mns.events.pending_mhp |= MNSH_E_TIMER_RX_EXTERN;
    GIVE_EVENTS();
    T_MHP_REQUEST(MNSH_E_TIMER_RX_EXTERN);
    MnsSetPendingEvent(MNS_P_MHP);
    T_MOD_EXIT(MNS_33);
}
#endif

#ifdef MNS_34
void MnsRequestPacket(word flags)
{
    (void)flags;
    T_MOD_ENTRY(MNS_34);
    TAKE_EVENTS();
    mns.events.pending_pmhs |= MNSP_E_REQ_EXTERN;
    GIVE_EVENTS();
    T_PMHS_REQUEST(MNSP_E_REQ_EXTERN);
    MnsSetPendingEvent(MNS_P_PMHS);
    T_MOD_EXIT(MNS_34);
}
#endif

#ifdef MNS_35
void MnsRequestPacketTimerTx(void)
{
    T_MOD_ENTRY(MNS_35);
    TAKE_EVENTS();
    mns.events.pending_pmhs |= MNSP_E_TIMER_TX_EXTERN;
    GIVE_EVENTS();
    T_PMHS_REQUEST(MNSP_E_TIMER_TX_EXTERN);
    MnsSetPendingEvent(MNS_P_PMHS);
    T_MOD_EXIT(MNS_35);
}
#endif

#ifdef MNS_36
void MnsRequestPacketTimerRx(void)
{
    T_MOD_ENTRY(MNS_36);
    TAKE_EVENTS();
    mns.events.pending_pmhs |= MNSP_E_TIMER_RX_EXTERN;
    GIVE_EVENTS();
    T_PMHS_REQUEST(MNSP_E_TIMER_RX_EXTERN);
    MnsSetPendingEvent(MNS_P_PMHS);
    T_MOD_EXIT(MNS_36);
}
#endif



/*! Returns the current EHCIState as the EHC knows it. Based on the shadowed
  * EHCIState received from the INIC. This information is important for a lot
  * state machines, callbacks and initalization sequences.
  *
  * @author     SSemmler
  * @date       last update  4/4/2005
  * @version    2.0.3
  * @see        VmsvHandleNIStateStatus
  * @see        MsgSend3
  *
  * @return     EHCISTATE_UNKNOWN, EHCISTATE_PROTECTED, EHCISTATE_SEMI_PROTECTED or
  *             EHCISTATE_ATTACHED
  */
#ifdef MNS_43
byte MnsGetEHCIState(void)
{
    byte result;

    T_LIB_ENTRY(MNS_43);
    TAKE_MNS();
    result = mns.shadow.ehci_state;
    GIVE_MNS();
    T_LIB_EXIT(MNS_43);

    return(result);
}
#endif

/*! Register a timer in the timer management table. Initalizes the structure
  * members and adds the timer as last entry to the timer management table
  * (linked list).
  *
  * @author     SSemmler
  * @date       last update  4/20/2005
  * @version    2.0.3
  * @see        MostClearTimer
  * @see        MostGetTimer
  * @see        MostSetTimer
  * @see        MostUnregisterTimer
  * @relates    TMostTimer
  *
  * @param      timer_ptr is a pointer to the timer instance
  * @param      cb_ptr function pointer of the callback to call if timeout is
  *                    reached.
  * @param      event parameter to be passed to the given callback
  */
#ifdef MNS_45
void MostRegisterTimer(TMostTimer *timer_ptr, TMostEventCB *cb_ptr, word event)
{
    T_API_ENTRY(MNS_45);

    ASSERT(timer_ptr);

    TAKE_TM();
    if (MNS_FALSE == MnsTimerRegistered(timer_ptr))
    {
        if (mns.timer.first_ptr)
        {
            TMostTimer *last_ptr = mns.timer.first_ptr;

            while (last_ptr->next_ptr)
            {
                last_ptr = last_ptr->next_ptr;
            }

            last_ptr->next_ptr = timer_ptr;
            timer_ptr->prev_ptr = last_ptr;
        }
        else
        {
            mns.timer.first_ptr = timer_ptr;
            timer_ptr->prev_ptr = NULL;
        }

        timer_ptr->next_ptr = NULL;
        timer_ptr->cb_ptr = cb_ptr;
        timer_ptr->event = event;
        timer_ptr->value = 0;
        timer_ptr->periodic = 0;
    }
    GIVE_TM();

    T_API_EXIT(MNS_45);
}
#endif

/*! Unregister timer from timer management table. Unlink timer entry.
  *
  * @author     SSemmler
  * @date       last update  4/20/2005
  * @version    2.0.3
  * @see        MostClearTimer
  * @see        MostGetTimer
  * @see        MostSetTimer
  * @see        MostRegisterTimer
  * @relates    TMostTimer
  *
  * @param      timer_ptr is a pointer to the timer instance
  */
#ifdef MNS_46
void MostUnregisterTimer(TMostTimer *timer_ptr)
{
    T_API_ENTRY(MNS_46);

    ASSERT(timer_ptr);

    TAKE_TM();
    if (MNS_FALSE != MnsTimerRegistered(timer_ptr))
    {
        if (timer_ptr->prev_ptr)
        {
            timer_ptr->prev_ptr->next_ptr = timer_ptr->next_ptr;
            if (timer_ptr->next_ptr)
            {
                timer_ptr->next_ptr->prev_ptr = timer_ptr->prev_ptr;
            }
        }
        else
        {
            mns.timer.first_ptr = timer_ptr->next_ptr;
            if (timer_ptr->next_ptr)
            {
                timer_ptr->next_ptr->prev_ptr = NULL;
            }
        }
    }
    GIVE_TM();

    T_API_EXIT(MNS_46);
}
#endif

#ifdef MNS_49
static void MnsFireWatchdogTrigger(void)
{
    bool send;
    #ifndef MNS_DISABLE_WD_SUPERVISION
    bool error;
    #endif

    T_MOD_ENTRY(MNS_49);
    send = MNS_FALSE;
    #ifndef MNS_DISABLE_WD_SUPERVISION
    error = MNS_FALSE;
    #endif

    TAKE_TM();
    ASSERT(mns.wd.msg_ptr);

    if(MNS_WD_FLAG_BUSY_MASK != (mns.wd.flags & MNS_WD_FLAG_BUSY_MASK)) /* flag not set */
    {
        mns.wd.flags |= MNS_WD_FLAG_BUSY_MASK;
        send = MNS_TRUE;
        #ifndef MNS_DISABLE_WD_SUPERVISION
        mns.watchdogmode.tx_busy_cnt = MNS_WD_SUPERVIS_CNT_START;
        #endif
    }
    else
    {
        mns.wd.flags |= MNS_WD_FLAG_SCHEDULE_MASK;

        #ifndef MNS_DISABLE_WD_SUPERVISION
        mns.watchdogmode.tx_busy_cnt++;
        if (mns.watchdogmode.tx_busy_cnt > mns.watchdogmode.max_tx_busy_cnt)
        {
            error = MNS_TRUE;
            mns.watchdogmode.tx_busy_cnt = MNS_WD_SUPERVIS_CNT_START;
        }
        #endif
    }
    GIVE_TM();

    if (MNS_FALSE != send)
    {
        PmsSend(mns.wd.handle, MnsWDTxComplete);
    }

    #ifndef MNS_DISABLE_WD_SUPERVISION
    if (MNS_FALSE != error)
    {
        MnsReportError(NSR_E_NO_COMM);
    }
    #endif

    T_MOD_EXIT(MNS_49);
}
#endif

#ifdef MNS_51
static byte MnsRetry(void)
{
    word retry;
    byte result;

    T_MOD_ENTRY(MNS_51);

    retry  = (word)0;
    result = ERR_NO;

    TAKE_MNS();
    retry = mns.retry;
    GIVE_MNS();

    #ifndef MNS_AVOID_ATTACH
    PROCESS_RETRY    (MNS_RETRY_RESET_COMPLETE,     MnsResetComplete);
    PROCESS_RETRY    (MNS_RETRY_GO_SEMI_PROTECTED,  MnsEHCIGoSemiProtected);
    PROCESS_RETRY    (MNS_RETRY_CHECK_WATCHDOGMODE, MnsCheckWatchdogMode);
    PROCESS_RETRY    (MNS_RETRY_REQ_EHCISTATE,      MnsRequestEHCIState);
    PROCESS_RETRY    (MNS_RETRY_FORCE_PROTECTED,    MnsForceProtected);
    PROCESS_RETRY    (MNS_RETRY_REMOTE_ACCESS,      MnsEnforceRemoteAccess);
    #ifdef MNS_105
    PROCESS_RETRY    (MNS_RETRY_DISABLE_SCERROR,    MnsDisableSCError);
    #endif
    #ifdef MNS_118
    PROCESS_RETRY    (MNS_RETRY_REQ_VERSION_INFO,   MnsRequestVersionInfo);
    #endif
    #ifdef MNS_119
    PROCESS_RETRY    (MNS_RETRY_GAS_PHASE_COMPLETE, MnsGASPhaseComplete);
    #endif
    #ifdef MNS_121                                                    
    PROCESS_RETRY    (MNS_RETRY_HF340,              MnsWA340Fix);           
    #endif                                                                  
    #endif /* #ifndef MNS_AVOID_ATTACH */

    T_MOD_EXIT(MNS_51);

    return(result);
}
#endif


#ifdef MNS_52
static byte MnsCheckWatchdogMode(void)
{
    byte result;
    TMsgTx *msg_ptr;

    T_MOD_ENTRY(MNS_52);

    result  = ERR_NO;
    msg_ptr = MsgGetTxPtrICM((word)7);	// Bug : Errata sheet 3.0.X-6 - 0300XX-016

    if (msg_ptr)
    {
        msg_ptr->Tgt_Adr   = MSG_TGT_INIC;
        msg_ptr->FBlock_ID = FBLOCK_INIC;
        msg_ptr->Inst_ID   = (byte)0;
        msg_ptr->Func_ID   = FUNCID_INIC_WATCHDOGMODE;

        TAKE_MNS();
        if (mns.config.general.watchdogmode.overwrite)
        {
            msg_ptr->Operation = OP_SETGET;
            msg_ptr->Data[0]   = (byte)mns.config.general.watchdogmode.reset;
            msg_ptr->Data[1]   = HB(mns.config.general.watchdogmode.timeout);
            msg_ptr->Data[2]   = LB(mns.config.general.watchdogmode.timeout);
            msg_ptr->Data[3]   = HB(mns.config.general.watchdogmode.auto_shutdown_delay);
            msg_ptr->Data[4]   = LB(mns.config.general.watchdogmode.auto_shutdown_delay);
            #ifdef _OS81110_WD_TH
            if (MNS_TM_WD_THRESHOLD_DISABLE != mns.config.general.watchdogmode.threshold)
            {
                msg_ptr->Data[5]   = HB(mns.config.general.watchdogmode.threshold);
                msg_ptr->Data[6]   = LB(mns.config.general.watchdogmode.threshold);
            }
            else
            {
                msg_ptr->Length = (word)5;
            }
            #else
            msg_ptr->Length = (word)5;
            #endif
        }
        else
        {
            msg_ptr->Operation = OP_GET;
            msg_ptr->Length = (word)0;
        }
        GIVE_MNS();

        MsgSend3(msg_ptr);
    }
    else
    {
        SCHEDULE_RETRY(MNS_RETRY_CHECK_WATCHDOGMODE);
        result = ERR_BUFOV;
    }

    if (ERR_NO == result)
    {
        bool wd_active;
        word timeout;

        #ifndef MNS_DISABLE_WD_SUPERVISION
        TAKE_TM();
        mns.watchdogmode.max_tx_busy_cnt = MNS_WD_SUPERVIS_CNT_DISABLE; /* disable watchdog supervision */
        GIVE_TM();
        #endif

        TAKE_MNS();
        wd_active = (MNS_TM_WD_DISABLE != mns.config.general.watchdogmode.timeout)
            ? MNS_TRUE : MNS_FALSE;
        timeout = mns.watchdogmode.timeout_wdsm;
        GIVE_MNS();

        if (MNS_FALSE != wd_active)
        {
            MostSetTimer(&(mns.timer.watchdog), timeout, MNS_TRUE);
        }
    }

    T_MOD_EXIT(MNS_52);

    return(result);
}
#endif

#ifdef MNS_53
void MnsHandleWatchdogModeStatus(const TMsgRx *msg_ptr)
{
    word new_timeout;
    word new_wdsm;

    T_LIB_ENTRY(MNS_53);

    ASSERT(msg_ptr);

    TAKE_MNS();
    mns.config.general.watchdogmode.reset                = msg_ptr->Data[0];
    mns.config.general.watchdogmode.timeout              = (word)((word)msg_ptr->Data[1] << 8);
    mns.config.general.watchdogmode.timeout             |= msg_ptr->Data[2];
    mns.watchdogmode.timeout_wdsm = mns.config.general.watchdogmode.timeout -
        (mns.config.general.watchdogmode.timeout >> MNS_TM_WDSM_POWER);
    mns.config.general.watchdogmode.auto_shutdown_delay  = (word)((word)msg_ptr->Data[3] << 8);
    mns.config.general.watchdogmode.auto_shutdown_delay |= msg_ptr->Data[4];
    new_timeout = mns.config.general.watchdogmode.timeout;
    new_wdsm = mns.watchdogmode.timeout_wdsm;
    GIVE_MNS();

    #ifndef MNS_DISABLE_WD_SUPERVISION
    TAKE_TM();
    if ((MNS_TM_WD_MIN_TIMEOUT >> 1) <= new_wdsm)
    {
        mns.watchdogmode.max_tx_busy_cnt = (word)(MNS_WD_STATUS_TIMEOUT / new_wdsm)
                                           + (word)1;
        mns.watchdogmode.tx_busy_cnt     = MNS_WD_SUPERVIS_CNT_START;
    }
    else
    {
        mns.watchdogmode.max_tx_busy_cnt = MNS_WD_SUPERVIS_CNT_DISABLE; /* disable watchdog supervision */
        mns.watchdogmode.tx_busy_cnt     = MNS_WD_SUPERVIS_CNT_START;
        FAILED_ASSERT();                                                /* invalid wd security margin */
    }
    GIVE_TM();
    #endif

    if (MNS_TM_WD_DISABLE != new_timeout)
    {
        MnsFireWatchdogTrigger();
        MostSetTimer(&(mns.timer.watchdog), new_wdsm, MNS_TRUE);
    }
    else
    {
        MostClearTimer(&(mns.timer.watchdog));
    }

    if (!EHCISTATE_IS_ATTACHED())
    {
        MnsNtfCheck(NTF_WATCHDOGMODE);
    }

    T_TM_WATCHDOG_TIMEOUT();

    T_LIB_EXIT(MNS_53);
}
#endif

/*! Supplies the information whether a specific feature is provided on the
  * current platform or not.
  *
  * @author     SSemmler
  * @date       last update  5/9/2005
  * @version    2.0.3
  *
  * @param      feature is the feature to query  (NSF_* constants)
  * @return     whether it is supported (MNS_TRUE) or not (MNS_FALSE)
  */
#ifdef MNS_54
bool MostIsSupported(word feature)
{
    bool supported;
    word mask;

    T_API_ENTRY(MNS_54);

    supported = MNS_FALSE;

    TAKE_MNS();
    mask = mns.shadow.version.feature_mask;
    GIVE_MNS();

    if ((INIC_SHADOW_INVALID_WORD != mask) && (mask & feature))
    {
        supported = MNS_TRUE;
    }

    T_API_EXIT(MNS_54);

    return(supported);
}
#endif

/*! Central check for the notification till "on init complete". Fires an
  * internal event, so the shadow services go into "init complete".
  *
  * @author     SSemmler
  * @date       last update  5/9/2005
  * @version    2.0.3
  *
  * @param      flag is the result message reveived (NTF_* constants).
  */
#ifdef MNS_55
void MnsNtfCheck(dword flag)
{
    bool complete;
    dword mask;

    T_LIB_ENTRY(MNS_55);

    complete = MNS_FALSE;

    TAKE_MNS();
    mask = mns.shadow.version.ntf_mask;
    GIVE_MNS();

    TAKE_EVENTS();
    mns.events.ntf_state |= flag;
    if (mask == (mask & mns.events.ntf_state))
    {
        complete = MNS_TRUE;
    }
    GIVE_EVENTS();

    if (MNS_FALSE != complete)
    {
        MnsDistribEvent(MNS_P_SRV_MNS, MNS_P_NTF_COMPLETE);
    }

    T_LIB_EXIT(MNS_55);
}
#endif

/*! Internal memcpy replacement.
  *
  * @author     SSemmler
  * @date       last update  5/10/2005
  * @version    2.0.3
  *
  * @param      tgt_ptr is a byte pointer to the target buffer
  * @param      src_ptr is a byte pointer to the source buffer
  * @param      size is the amount of bytes to copy
  */
#ifdef MNS_56
_INLINE void MnsMemCpy(byte* tgt_ptr, const byte* src_ptr, word size)
{
    ASSERT(src_ptr);
    /* Trace macros disabled for performance. */
    /* T_LIB_ENTRY(MNS_56); */
    while (size--)
    {
        *tgt_ptr++ = *src_ptr++;
    }
    /* T_LIB_EXIT(MNS_56); */
}
#endif

#ifdef MNS_57
/*!
  * \brief      Delivers the VersionInformation of the INIC hardware, firmware and the EHC
  *             NetServices in use.
  * \param[out] info_ptr  Pointer to a TMostVersionInfo structure containing the current
  *                       version information
  */
void MostGetVersionInfo(TMostVersionInfo *info_ptr)
{
    T_API_ENTRY(MNS_57);
    TAKE_MNS();

    info_ptr->hw         = mns.shadow.version.public.hw;
    info_ptr->chip_id    = mns.shadow.version.public.chip_id;
    info_ptr->hw_release = mns.shadow.version.public.hw_release;
    MNS_MEM_CPY(info_ptr->prod_name, mns.shadow.version.public.prod_name,
                (word)sizeof(mns.shadow.version.public.prod_name));
    MNS_MEM_CPY(info_ptr->fw, mns.shadow.version.public.fw,
                (word)sizeof(mns.shadow.version.public.fw));
    MNS_MEM_CPY(info_ptr->fw_date, mns.shadow.version.public.fw_date,
                (word)sizeof(mns.shadow.version.public.fw_date));
    MNS_MEM_CPY(info_ptr->cs, mns.shadow.version.public.cs,
                (word)sizeof(mns.shadow.version.public.cs));
    MNS_MEM_CPY(info_ptr->ns, mns.shadow.version.public.ns,
                (word)sizeof(mns.shadow.version.public.ns));
    MNS_MEM_CPY(info_ptr->api, mns.shadow.version.public.api,
                (word)sizeof(mns.shadow.version.public.api));

    GIVE_MNS();
    T_API_EXIT(MNS_57);
}
#endif

/*! Delivers the V1-style firmware date (day-month-year).
  *
  * @author     SSemmler
  * @date       last update  5/10/2005
  * @version    2.0.3
  *
  * @param      tgt_ptr points to a byte array of the size 3 to contain the
  *                     result.
  */
#ifdef MNS_58
void MostGetRevision(byte *tgt_ptr)
{
    byte count;

    T_API_ENTRY(MNS_58);

    count = 3;

    TAKE_MNS();

    while (count--)
    {
        *tgt_ptr++ = mns.shadow.version.public.fw_date[count];
    }

    GIVE_MNS();

    T_API_EXIT(MNS_58);
}
#endif

/*! Internal memset replacement.
  *
  * @author     SSemmler
  * @date       last update  5/10/2005
  * @version    2.0.3
  *
  * @param      tgt_ptr is a byte pointer to the target buffer
  * @param      value is the value to set
  * @param      size is the amount of bytes to set
  */
#ifdef MNS_59
_INLINE void MnsMemSet(byte* tgt_ptr, byte value, word size)
{
    /* Trace macros disabled for performance. */
    /* T_LIB_ENTRY(MNS_59); */
    while (size--)
    {
        *tgt_ptr++ = value;
    }
    /* T_LIB_EXIT(MNS_59); */
}
#endif

/*! Sends an INIC.00.EHCIState.Get message.
  *
  * @author     SSemmler
  * @date       last update  5/10/2005
  * @version    2.0.3
  * @see        MostServiceInitComplete
  *
  * @return     ERR_NO or ERR_BUFOV (retry is scheduled)
  */
#ifdef MNS_60
byte MnsRequestEHCIState(void)
{
    byte result;
    TMsgTx *msg_ptr;

    T_MOD_ENTRY(MNS_60);

    result  = ERR_NO;
    msg_ptr = MsgGetTxPtrICM(0);	// Bug : Errata sheet 3.0.X-6 - 0300XX-016

    if (msg_ptr)
    {
        msg_ptr->Tgt_Adr   = MSG_TGT_INIC;
        msg_ptr->Func_ID   = FUNCID_INIC_EHCISTATE;
        msg_ptr->Inst_ID   = 0;
        msg_ptr->Operation = OP_GET;

        MsgSend3(msg_ptr);
    }
    else
    {
        SCHEDULE_RETRY(MNS_RETRY_REQ_EHCISTATE);
        result = ERR_BUFOV;
    }

    T_MOD_EXIT(MNS_60);
    return(result);
}
#endif

/*! Sends an INIC.00.EHCIState.Set(Protected) message.
  *
  * @author     SSemmler
  * @date       last update  5/10/2005
  * @version    2.0.3
  * @see        MnsHandleEHCIStateStatus
  *
  * @return     ERR_NO or ERR_BUFOV (retry is scheduled)
  */
#ifdef MNS_61
byte MnsForceProtected(void)
{
    byte result;
    TMsgTx *msg_ptr;

    T_MOD_ENTRY(MNS_61);

    result  = ERR_NO;
    msg_ptr = MsgGetTxPtrICM(1);	// Bug : Errata sheet 3.0.X-6 - 0300XX-016

    if (msg_ptr)
    {
        msg_ptr->Tgt_Adr   = MSG_TGT_INIC;
        msg_ptr->Func_ID   = FUNCID_INIC_EHCISTATE;
        msg_ptr->Inst_ID   = 0;
        msg_ptr->Operation = OP_SET;
        msg_ptr->Data[0]   = EHCISTATE_PROTECTED;

        MsgSend3(msg_ptr);
    }
    else
    {
        SCHEDULE_RETRY(MNS_RETRY_FORCE_PROTECTED);
        result = ERR_BUFOV;
    }

    T_MOD_EXIT(MNS_61);
    return(result);
}
#endif

#ifdef MNS_62
/*! \brief   Timer Management Trigger Function
 *  \details Used for FIFO Status check every 2 seconds
 *          and also as driver for the timer management.
 *
 */
void MostCheckTimers(void)
{
    MnsTMHandleEvent(MNS_TM_EV_TIMEOUT_API);
}
#endif

#ifdef MNS_63
/*!
  * \brief      Returns the NetServices init structure
  * \details    The function GetNetServiceConfig() returns a pointer to the
  *             central configuration structure of the type TNetServicesConfig.
  *             Its field need to be setup by the application before
  *             InitNetServices() is called.
  * \return     Pointer to the central configuration structure
  */
TNetServicesConfig * GetNetServicesConfig(void)
{
    T_API_ENTRY(MNS_63);

    MNS_MEM_SET((byte *)&mns.config, (byte)0, (word)sizeof(mns.config));

    mns.config.general.watchdogmode.timeout             = MNS_TM_WD_MAX_TIMEOUT;
    mns.config.general.watchdogmode.auto_shutdown_delay = MNS_TM_WD_DISABLE;
    mns.config.general.watchdogmode.threshold           = MNS_TM_WD_THRESHOLD_DISABLE;
    mns.config.general.burst                            = MNS_STD_BURST;
    mns.config.lld.ctrl_interface_id                    = PMS_IFACE_I2C;
    mns.config.lld.data_interface_id                    = PMS_IFACE_I2C;

    mns.config.pms.rx_burst                             = PMS_STD_RX_BURST;
    #ifdef AMS_MIN
      mns.config.msg.rx_burst                           = MSG_STD_RX_BURST;
    #endif

    #ifdef ADS_MIN
      mns.config.wads.rx_burst                          = DATA_STD_RX_BURST;
    #endif


    #ifdef SCM_MIN
      #ifdef _OS81110_SPDIF
      #endif
      #ifdef SCMPM_MIN
        mns.config.scm.packet.port_id                   = SCM_PM_MLB;
        mns.config.scm.packet.mode                      = SCM_PM_NONE;
        mns.config.scm.packet.clock_config              = SCM_PORT_CFG_MLB_512_FS;
        #ifdef _OS81110_SPI
          mns.config.scm.packet.interrupt_threshold     = (byte)0x0F;
        #endif
        mns.config.scm.packet.in.address                = (word)0x0008;
        mns.config.scm.packet.out.address               = (word)0x0006;
      #endif
    #endif



    T_API_EXIT(MNS_63);

    return(&(mns.config));
}
#endif

#ifdef MNS_64
/*!
  * \brief      Stops the execution of the MOST NetServices.
  * \details    The application must call InitNetServices() in order to start
  *             the MOST NetServices again.
  *
  *             <b>Please note:</b>
  *             The INIC will be commanded into protected mode. There will be
  *             no more requests a short time after calling this function. A
  *             few more are required to send the INIC.EHCIState.Set() message.
  *             The callback function \c general.on_stop_netservices_fptr() is
  *             called as soon as the the INIC is detached and the application
  *             does not need to call MostService() anymore.
  */
void StopNetServices(void)
{
    T_API_ENTRY(MNS_64);

    TAKE_MNS();
    mns.stopped = MNS_TRUE;
    GIVE_MNS();

    #ifndef MNS_AVOID_ATTACH
    (void)MnsForceProtected();
    #else
    TAKE_MNS();
    mns.shadow.ehci_state = EHCISTATE_UNKNOWN;
    GIVE_MNS();
    if (mns.config.general.most_service_request_fptr)
    {
        mns.config.general.most_service_request_fptr();
    }
    #endif

    T_API_EXIT(MNS_64);
}
#endif

#ifdef MNS_65
/*!
  * \brief      Enable or disable the remote access to the INIC by calling this function.
  * \details    "remote access" means the ability to send .Set and .Start
  *              messages from another node in the network to FB INIC.
  * \param[in]  access can be MNS_ALLOW or MNS_DENY
  */
byte MostAllowRemoteAccess(byte access)
{
    byte result;

    T_API_ENTRY(MNS_65);

    result = ERR_NO;

    if (    (MNS_DENY  != access)
        &&  (MNS_ALLOW != access))
    {
        result = ERR_PARAM;
    }
    else
    {
        TAKE_MNS();
        mns.remote_access = access;
        GIVE_MNS();

        (void)MnsEnforceRemoteAccess();
    }

    T_API_EXIT(MNS_65);
    return(result);
}
#endif

#ifdef MNS_66
static byte MnsEnforceRemoteAccess(void)
{
    byte result;
    byte access;
    TMsgTx *msg_ptr;

    T_MOD_ENTRY(MNS_66);

    result = ERR_NO;

    TAKE_MNS();
    access = mns.remote_access;
    GIVE_MNS();


    msg_ptr = MsgGetTxPtrICM(1);	// Bug : Errata sheet 3.0.X-6 - 0300XX-016
    if (msg_ptr)
    {
        msg_ptr->Tgt_Adr   = MSG_TGT_INIC;
        msg_ptr->Func_ID   = FUNCID_INIC_REMOTEACCESS;
        msg_ptr->Inst_ID   = 0;
        msg_ptr->Operation = OP_SET;
        msg_ptr->Data[0]   = (MNS_ALLOW == access) ? MNS_TRUE : MNS_FALSE;
        MsgSend3(msg_ptr);
    }
    else
    {
        SCHEDULE_RETRY(MNS_RETRY_REMOTE_ACCESS);
        result = ERR_BUFOV;
    }

    T_MOD_EXIT(MNS_66);
    return(result);
}
#endif

#ifdef MNS_68
/*!    Internal helper function to handle the INIC.RemoteAccess.Error
  *             messages received from the INIC.
  *
  * @author     SSemmler
  * @date       last update  2005-10-19
  * @version    2.0.4
  */
void MnsHandleError(const TMsgRx *msg_ptr)
{
    T_LIB_ENTRY(MNS_68);

    ASSERT(msg_ptr);

    switch (msg_ptr->Func_ID)
    {
        case FUNCID_INIC_REMOTEACCESS:
            MnsReportError(NSR_E_INVALID_REMOTE_ACCESS_CONFIG);
            break;

        case FUNCID_INIC_BIST:
            {
                TMnsResult result;
                result = NSR_BUILD(MNS_FALSE, 0xC2, msg_ptr->Data[0]);
                if (NSR_E_BIST_MEMORY <= result)
                {
                    MnsReportError(result);
                }
            }
            break;

        case FUNCID_INIC_WATCHDOGMODE:
            MnsReportError(NSR_E_INVALID_WATCHDOG_MODE_CONFIG);
            break;

        default:
            FAILED_ASSERT();
            break;
    }

    T_LIB_EXIT(MNS_68);
}
#endif

#ifdef MNS_69
/*!    Library internal helper function to query if it is okay to
  *             service the higher layers.
  *
  * @author     SSemmler
  * @date       last update  2005-11-03
  * @version    2.0.4
  *
  * @return     MNS_TRUE, if the MediaLB packet sockets are created or I2C is the
  *             default interface.
  */
bool MnsPMReady(void)
{
    bool result;

    T_LIB_ENTRY(MNS_69);
    result = MNS_FALSE;
#ifdef SCM_MIN
    if((PMS_IFACE_I2C == mns.config.lld.ctrl_interface_id) ||
       ((PMS_IFACE_GENERAL == mns.config.lld.ctrl_interface_id)
#ifdef SCMPM_MIN
        && (MNS_FALSE != ScmpmComplete())
#endif
       ))
    {
        result = MNS_TRUE;
    }
#else
    result = MNS_TRUE;
#endif
    T_LIB_EXIT(MNS_69);

    return(result);
}
#endif

#ifdef MNS_70
static byte MnsResetComplete(void)
{
    TMsgTx *msg_ptr;
    byte result;
    bool mns_stopped;

    T_MOD_ENTRY(MNS_70);

    msg_ptr = NULL;
    result  = ERR_NO;
    mns_stopped = MNS_FALSE;

    TAKE_MNS();
    if((MNS_FALSE != mns.stopped) && (MNS_FALSE == mns.signaled_ns_stopped))
    {
        mns_stopped = MNS_TRUE;
        mns.shadow.ehci_state = EHCISTATE_UNKNOWN;  /* signale on_stop_netservices_cb() in MostService() */
    }
    GIVE_MNS();

    if (MNS_FALSE == mns_stopped)     /* do not start to reattach if StopNetServices() was called  */
    {
        msg_ptr = MsgGetTxPtrICM(2);	// Bug : Errata sheet 3.0.X-6 - 0300XX-016
    }

    if (NULL != msg_ptr)
    {
        msg_ptr->Tgt_Adr   = MSG_TGT_INIC;
        msg_ptr->Func_ID   = FUNCID_INIC_EHCISTATE;
        msg_ptr->Inst_ID   = 0;
        msg_ptr->Operation = OP_SET;
        msg_ptr->Length    = 2;
        msg_ptr->Data[0]   = EHCISTATE_SEMI_PROTECTED;
        #ifdef GAS_MIN
        msg_ptr->Data[1]   = EHCISTATE_NTF_NO;
        #else
        msg_ptr->Data[1]   = EHCISTATE_NTF_ALL;
        #endif
        MsgSend3(msg_ptr);

        #ifndef GAS_MIN
        (void)MnsCheckWatchdogMode();
        #endif
    }
    else if (MNS_FALSE == mns_stopped)
    {
        result = ERR_BUFOV;
    }
    else
    {
        SCHEDULE_RETRY(MNS_RETRY_RESET_COMPLETE);
        result = ERR_BUFOV;
    }

    T_MOD_EXIT(MNS_70);
    return(result);
}
#endif

#ifdef MNS_71
void MhpTxDelayTimeout(void)
{
    T_API_ENTRY(MNS_71);
    TAKE_EVENTS();
    mns.events.pending_mhp |= MNSH_E_TX_EVENT_EXTERN;
    GIVE_EVENTS();
    T_MHP_REQUEST(MNSH_E_TX_EVENT_EXTERN);
    MnsSetPendingEvent(MNS_P_MHP);
    T_API_EXIT(MNS_71);
}
#endif




/*! This module function is called whenever we need to check if we have to check
  * if the higher layers have to be started or stopped
  *
  * @author     RWilhelm
  * @date       last update  2006-08-17
  * @version    2.1.1
  * @see        MnsStopLayer2
  * @see        VmsvHandleNIStateStatus
  * @see        MnsHandleEHCIStateStatus
  */
#ifdef MNS_73
void MnsCheckHigherLayers(void)
{
    bool attached;
    bool net_on;
    bool pm_ready;
    #ifdef SCM_MIN
    byte boundary;
    bool bdvalid;
    #else
    bool bdvalid;
    #endif

    T_LIB_ENTRY(MNS_73);

    attached = EHCISTATE_IS_ATTACHED() ? MNS_TRUE : MNS_FALSE;
    net_on   = (MNS_NET_ON == MostGetState()) ? MNS_TRUE : MNS_FALSE;
    pm_ready = MnsPMReady();
    #ifdef SCM_MIN
    boundary = ScmGetBoundary();
    bdvalid  = (INIC_SHADOW_INVALID_BYTE != boundary) &&
               ((MNS_FALSE != MostIsSupported(NSF_MOST_25)) ? ((0x00 != boundary) ? MNS_TRUE : MNS_FALSE) : MNS_TRUE);
    #else
    bdvalid  = MNS_TRUE;
    #endif

    /* check conditions for Layer2 */
    if ( (MNS_FALSE != net_on)   &&
         (MNS_FALSE != attached) &&
         (MNS_FALSE != bdvalid ) )
    {
        #ifdef MNS_74
        MnsStartLayer2();
        #endif
    }
    else
    {
        #ifdef MNS_75
        MnsStopLayer2();
        #endif
    }


    /* check conditions for MHP and Packetizer */
    if ( (MNS_FALSE != net_on)   &&
         (MNS_FALSE != attached) &&
         (MNS_FALSE != pm_ready) &&
         (MNS_FALSE != bdvalid) )
    {
        #ifdef MNS_76
        MnsStartPacketLayers();
        #endif
    }
    else
    {
        #ifdef MNS_77
        MnsStopPacketLayers();
        #endif
    }


    T_LIB_EXIT(MNS_73);
}
#endif

/*! This module function is called whenever we enter the combined state of
  * "EHC Attached" and "Net On". The init functions of Layer2 are called
  * and the service mechanisms are enabled.
  *
  * @author     RWilhelm
  * @date       last update  2006-08-17
  * @version    2.1.1
  * @see        MnsStopLayer2
  * @see        MnsCheckHigherLayers
  * @see        VmsvHandleNIStateStatus
  * @see        MnsHandleEHCIStateStatus
  */
#ifdef MNS_74
void MnsStartLayer2(void)
{
    T_LIB_ENTRY(MNS_74);

    T_START_L2();

    if (MNS_FALSE == mns.services.ms_layer2)
    {
        mns.services.ms_layer2 = MNS_TRUE;

        #ifdef NS_MNS_MNS2
            InitNetServicesLayer2();
        #endif
    }

    T_LIB_EXIT(MNS_74);
}
#endif

/*! This module function is called whenever we leave the combined state of
  * "EHC Attached" and "Net On". The service mechanism is stopped and the
  * pending events of Layer2 are cleared. Certain Higher Level callbacks
  * are called if needed ("Go Net Off" for NB and ET).
  *
  * @author     RWilhelm
  * @date       last update  2006-08-17
  * @version    2.1.1
  * @see        MnsStartLayer2
  * @see        VmsvHandleNIStateStatus
  * @see        MnsHandleEHCIStateStatus
  */
#ifdef MNS_75
void MnsStopLayer2(void)
{
    T_LIB_ENTRY(MNS_75);

    T_STOP_L2();

    if (MNS_FALSE != mns.services.ms_layer2)
    {
        mns.services.ms_layer2 = MNS_FALSE;

        WAIT4MX(MX_MNS_PE);
        mns.events.pending &= (word)(~MNS_P_L2);
        REL_MX(MX_MNS_PE);

        #ifdef NS_MSV_ET
            ET_Go_Net_Off();
        #endif

        #ifdef NS_MSV_NB
            NbGoNetOff();
        #endif
    }

    T_LIB_EXIT(MNS_75);
}
#endif





/*! This module function is called whenever we enter the combined state of
  * "EHC Attached" and "Net On" and Packet transmission is possible.
  * The init functions of the packet layers are called
  * and the service mechanisms are enabled.
  *
  * @author     RWilhelm
  * @date       last update  2006-08-17
  * @version    2.1.1
  * @see        MnsStopPacketLayers
  * @see        VmsvHandleNIStateStatus
  * @see        MnsHandleEHCIStateStatus
  */
#ifdef MNS_76
void MnsStartPacketLayers(void)
{
    T_LIB_ENTRY(MNS_76);

    T_START_PL();

    if (MNS_FALSE == mns.services.ms_packet_layers)
    {
        mns.services.ms_packet_layers = MNS_TRUE;

        #ifdef NS_MNS_MHP
            MhpInit();
        #endif

        #ifdef NS_MNS_PMHS
            PacketInit();
        #endif
    }

    T_LIB_EXIT(MNS_76);
}
#endif

/*! This module function is called whenever we leave the combined state of
  * "EHC Attached" and "Net On" and "packet transmission possible". The service
  *  mechanism is stopped and the pending events of thepacket layers are cleared.
  *
  * @author     RWilhelm
  * @date       last update  2006-08-17
  * @version    2.1.1
  * @see        MnsStartPacketLayers
  * @see        VmsvHandleNIStateStatus
  * @see        MnsHandleEHCIStateStatus
  */

#ifdef MNS_77
void MnsStopPacketLayers(void)
{
    T_LIB_ENTRY(MNS_76);

    T_STOP_PL();

    if (MNS_FALSE != mns.services.ms_packet_layers)
    {
        mns.services.ms_packet_layers = MNS_FALSE;

        WAIT4MX(MX_MNS_PE);
        mns.events.pending &= (word)(~MNS_P_PACKET_LAYERS);
        REL_MX(MX_MNS_PE);

        #ifdef NS_MNS_MHP
            MhpInit();
        #endif
    }

    T_LIB_EXIT(MNS_76);
}
#endif

#ifdef MNS_96
void MnsTMHandleEvent(byte event)
{
    T_LIB_ENTRY(MNS_96);
    T_TM_EV(event);

    TAKE_TM();
    switch(event)
    {
        case MNS_TM_EV_TIMEOUT_API:
            mns.tm.scheduled_timeout = 0;
            /*lint -fallthrough */
        case MNS_TM_EV_TIMEOUT:
        case MNS_TM_EV_UPDATE:
        case MNS_TM_EV_SERVICE:
            MnsTMDiff();
            if(mns.tm.diff)
            {
                MnsTMUpdate();

                if (MNS_TM_EV_UPDATE == event)
                {
                    break;
                }
            }
        /*lint -fallthrough */
        case MNS_TM_EV_SCHEDULE:
            if(mns.tm.min_timeout)
            {
                if(mns.tm.scheduled_timeout)
                {
                    if(mns.tm.min_timeout < mns.tm.scheduled_timeout)
                    {
                        MnsTMSchedule();
                    }
                }
                else
                {
                    MnsTMSchedule();
                }
            }
            break;

        default:
            FAILED_ASSERT();
            break;
    }
    GIVE_TM();

    T_MOD_EXIT(MNS_96);
}
#endif

#ifdef MNS_97
static void MnsTMUpdate(void)
{
    TMostTimer *current_ptr;

    T_MOD_ENTRY(MNS_97);

    current_ptr = mns.timer.first_ptr;

    T_TM_OP(MNS_TM_OP_UPDATE);
    ASSERT(mns.tm.diff);

    mns.tm.min_timeout = 0xFFFF;

    while (current_ptr)
    {
        if (current_ptr->value)
        {
            if (mns.tm.diff >= current_ptr->value)
            {
                current_ptr->value = current_ptr->periodic;
                if (current_ptr->cb_ptr)
                {
                    current_ptr->cb_ptr(current_ptr->event);
                }
            }
            else
            {
                current_ptr->value -= mns.tm.diff;
            }

            if (current_ptr->value && (current_ptr->value< mns.tm.min_timeout))
            {
                mns.tm.min_timeout = current_ptr->value;
            }
        }
        current_ptr = current_ptr->next_ptr;
    }
    mns.tm.diff = 0;

    T_MOD_EXIT(MNS_97);
}
#endif

#ifdef MNS_98
static void MnsTMDiff(void)
{
    T_MOD_ENTRY(MNS_98);
    T_TM_OP(MNS_TM_OP_DIFF);

    if (mns.config.general.get_tick_count_fptr)
    {
        word current_tick = mns.config.general.get_tick_count_fptr();
        word current_diff = current_tick - mns.tm.last_tick;

        mns.tm.last_tick = current_tick;
        mns.tm.diff += current_diff;

        if(mns.tm.scheduled_timeout)
        {
            mns.tm.scheduled_timeout = (current_diff >= mns.tm.scheduled_timeout) ?
                (word) 1 : mns.tm.scheduled_timeout - current_diff;
        }
    }
    else
    {
        FAILED_ASSERT();
    }

    T_TM_DIFF(mns.tm.diff, mns.tm.scheduled_timeout);
    T_MOD_EXIT(MNS_98);
}
#endif

#ifdef MNS_100
static void MnsTMSchedule(void)
{
    T_MOD_ENTRY(MNS_100);
    T_TM_OP(MNS_TM_OP_SCHEDULE);

    mns.tm.scheduled_timeout = mns.tm.min_timeout;

    if (mns.config.general.next_min_timeout_fptr)
    {
        mns.config.general.next_min_timeout_fptr(mns.tm.min_timeout);
    }
    T_MOD_EXIT(MNS_100);
}
#endif

#ifdef MNS_102
/*!    Is called by VMSV to signal MNS_NET_ON to the AMS
  *
  *
  * @author     RWilhelm
  * @date       last update  2006-12-07
  * @version    2.1.1
  */
void MnsSignalNetOn(void)
{
    T_LIB_ENTRY(MNS_102);

    TAKE_MNS();
    mns.signal_net_on = MNS_TRUE;
    GIVE_MNS();

    T_LIB_EXIT(MNS_102);
}
#endif



/*!    Is called by PMS exclusively when the transmission of
  *             mns.wd.msg_ptr completed.
  *
  *             reads:      mns.wd.flags
  *             changes:    mns.wd.flags
  *
  * @author     RWilhelm
  * @date       last update  2007-11-28
  * @version    2.1.2
  */
#ifdef MNS_104
static byte MnsWDTxComplete(HMBMBUF handle, byte status)
{
    bool scheduled;

    T_MOD_ENTRY(MNS_104);
    scheduled = MNS_FALSE;
    (void) handle;
    ASSERT(XMIT_SUCCESS == status);
    (void) status;

    TAKE_TM();
    mns.wd.flags &= (byte)(~MNS_WD_FLAG_BUSY_MASK);

    if(mns.wd.flags & MNS_WD_FLAG_SCHEDULE_MASK)
    {
        mns.wd.flags &= (byte)(~MNS_WD_FLAG_SCHEDULE_MASK);
        scheduled = MNS_TRUE;
    }
    GIVE_TM();

    if (MNS_FALSE != scheduled)
    {
        MnsFireWatchdogTrigger();
    }

    T_MOD_EXIT(MNS_104);

    return(PMS_RELEASE);
}
#endif


#ifdef MNS_105
/*!
  *
  *
  * @author     RWilhelm
  * @date       last update  2009-02-27
  * @version    3.0.1
  */
byte MnsDisableSCError(void)
{
    TMsgTx *msg_ptr[3];
    byte count;
    byte result;
    bool retry;

    T_MOD_ENTRY(MNS_105);

    count  = 0;
    result = ERR_NO;
    retry  = MNS_FALSE;

    while (count < 3)
    {
        msg_ptr[count] = MsgGetTxPtrICM(0);	// Bug : Errata sheet 3.0.X-6 - 0300XX-016
        if(msg_ptr[count])
        {
            MBM_NO_USER_TX_FREE( MbmGetHandleByMsgPtr(msg_ptr[count]) );

            msg_ptr[count]->Tgt_Adr   = MSG_TGT_INIC;
            msg_ptr[count]->FBlock_ID = FBLOCK_INIC;
            msg_ptr[count]->Inst_ID   = 0x00;
            msg_ptr[count]->Func_ID   = 0xE01;
            msg_ptr[count]->Operation = OP_SET;
            msg_ptr[count]->Length    = 0x01;
            count++;
        }
        else
        {
            break;
        }
    }

    if(3 == count)
    {
        if (ERR_NO == WmcsSetINICMemPending(MNS_TRUE))
        {
            msg_ptr[0]->Data    = (byte *) & mns_disable_scerror[0];
            msg_ptr[1]->Func_ID = FUNCID_INIC_MEMORY;
            msg_ptr[1]->Data    = (byte *) & mns_disable_scerror[1];
            msg_ptr[1]->Length  = 0x09;
            msg_ptr[2]->Data    = (byte *) & mns_disable_scerror[11];

            while(count)
            {
               count--;
               MsgSend3(msg_ptr[count]);
            }
            (void)WmcsSetINICMemPending(MNS_FALSE);
        }
        else
        {
            retry = MNS_TRUE;
        }
    }
    else
    {
        retry = MNS_TRUE;
    }

    if (MNS_FALSE != retry)
    {
        while(count)
        {
            count--;
            MsgTxUnused(msg_ptr[count]);
        };

        SCHEDULE_RETRY(MNS_RETRY_DISABLE_SCERROR);
        result = ERR_BUFOV;
    }

    T_MOD_EXIT(MNS_105);
    return(result);
}
#endif

#ifdef MNS_106
/*!    Checks if the given timer is (already) registered
  *
  *
  * @author     REberhardt
  * @date       last update  2007-12-07
  * @version    2.1.4
  */
bool MnsTimerRegistered(TMostTimer* timer_ptr)
{
    TMostTimer* current_ptr;
    bool result;

    T_MOD_ENTRY(MNS_106);

    result = MNS_FALSE;

    current_ptr = mns.timer.first_ptr;

    while (current_ptr && !result)
    {
        if (current_ptr == timer_ptr)
        {
            result = MNS_TRUE;
        }
        current_ptr = current_ptr->next_ptr;
    }

    T_MOD_EXIT(MNS_106);

    return(result);
}
#endif

#ifdef MNS_107
/*!    Changes the properties of a already registered timer.
  *             Such as deleting the event callback...
  *
  *
  * @author     REberhardt
  * @date       last update  2007-12-10
  * @version    2.1.4
  */
void MnsReRegisterTimer(TMostTimer *timer_ptr, TMostEventCB *cb_ptr, word event)
{
    T_MOD_ENTRY(MNS_107);

    ASSERT(timer_ptr);
    if (timer_ptr)
    {
        TAKE_TM();
        timer_ptr->event = event;
        timer_ptr->cb_ptr = cb_ptr;
        GIVE_TM();
    }

    T_MOD_EXIT(MNS_107);
}
#endif

#ifdef MNS_108
word MnsGetWatchdogKickTimeout(void)
{
    word timeout;

    T_LIB_ENTRY(MNS_108);

    TAKE_MNS();
    if( MNS_TM_WD_DISABLE == mns.config.general.watchdogmode.timeout )
    {
        timeout = MNS_TM_WD_DISABLE;
    }
    else
    {
        timeout = mns.watchdogmode.timeout_wdsm;
    }
    GIVE_MNS();

    T_LIB_EXIT(MNS_108);

    return( timeout );
}
#endif



#ifdef MNS_109
/*! Supplies the information whether a specific notification is provided on the
  * current platform or not.
  *
  * @author     RWilhelm
  * @date       last update  2008-09-04
  * @version    2.5.0
  *
  * @param      feature is the feature to query  (NTF_* constants)
  * @return     whether it is supported (MNS_TRUE) or not (MNS_FALSE)
  */
bool MostNtfIsRequired(dword feature)
{
    bool supported;
    dword mask;

    T_LIB_ENTRY(MNS_109);

    supported = MNS_FALSE;

    TAKE_MNS();
    mask = mns.shadow.version.ntf_mask;
    GIVE_MNS();

    if ((INIC_SHADOW_INVALID_DWORD != mask) && (mask & feature))
    {
        supported = MNS_TRUE;
    }

    T_LIB_EXIT(MNS_109);

    return(supported);
}
#endif

#ifdef MNS_113
static _INLINE void MnsSendMdmMnsInitTimeout(void)
{
    T_MOD_ENTRY(MNS_113);

    MdmMsgSendMns(MDM_MNS_INIT_TIMEOUT, NULL, (byte)0);

    T_MOD_EXIT(MNS_113);
}
#endif  /* #ifdef MNS_113 */

#ifdef MNS_114
static _INLINE void MnsSendMdmMnsVersionConflict(void)
{
    byte tmp[7];

    T_MOD_ENTRY(MNS_114);

    TAKE_MNS();
    /* Convert MNS version into Byte stream (big endian) */
    tmp[0] = mns.shadow.version.public.ns[3];
    tmp[1] = mns.shadow.version.public.ns[2];
    tmp[2] = mns.shadow.version.public.ns[1];
    tmp[3] = mns.shadow.version.public.ns[0];

    /* Convert INIC FW into Byte stream (big endian) */
    tmp[4] = mns.shadow.version.public.fw[2];
    tmp[5] = mns.shadow.version.public.fw[1];
    tmp[6] = mns.shadow.version.public.fw[0];
    GIVE_MNS();

    MdmMsgSendMns(MDM_MNS_VERSION_CONFLICT, tmp, (byte)7);

    T_MOD_EXIT(MNS_114);
}
#endif  /* #ifdef MNS_114 */

#ifdef MNS_115
static _INLINE void MnsCheckNbminNotification(void)
{
    dword ntf_mask;
    dword ntf_state;

    T_MOD_ENTRY(MNS_115);

    TAKE_MNS();
    ntf_mask = mns.shadow.version.ntf_mask;
    GIVE_MNS();
    TAKE_EVENTS();
    ntf_state = mns.events.ntf_state;
    GIVE_EVENTS();

    /* no FBlock.NBMIN properties notified but all FBlock.INIC properties notified */
    if( ((dword)0 == (NTF_MASK_NBMIN & ntf_state)) && ((NTF_MASK_NBMIN | ntf_state) == ntf_mask) )
    {
        MnsReportError(NSR_E_MCM_RX_CHANNEL);
    }

    T_MOD_EXIT(MNS_115);
}
#endif

#ifdef MNS_116
static _INLINE void MnsPrepareReInit(void)
{
    T_MOD_ENTRY(MNS_116);

    MostClearTimer(&(mns.timer.watchdog));
    MostClearTimer(&(mns.timer.init));

    #ifdef PMS_46
    PmsPrepareReInit();
    #endif
    #ifdef AMS_39
    MsgPrepareReInit();
    #endif

    T_MOD_EXIT(MNS_116);
}
#endif

#ifdef MNS_117
static _INLINE bool MnsCheckTermination(void)
{
    bool fire_callback;
    bool terminated;

    T_MOD_ENTRY(MNS_117);

    fire_callback = MNS_FALSE;
    terminated = MNS_FALSE;

    TAKE_MNS();
    if(MNS_FALSE != mns.stopped)
    {
        if (MNS_FALSE != mns.signaled_ns_stopped)
        {
            terminated = MNS_TRUE;
        }
        else if (EHCISTATE_UNKNOWN == mns.shadow.ehci_state)
        {
            terminated = MNS_TRUE;
            fire_callback = MNS_TRUE;
            mns.signaled_ns_stopped = MNS_TRUE;
        }
    }
    GIVE_MNS();

    if (MNS_FALSE != fire_callback)
    {
        MnsPrepareReInit();

        if (NULL != mns.config.general.on_stop_netservices_fptr)
        {
            mns.config.general.on_stop_netservices_fptr();
        }
    }
    T_MOD_EXIT(MNS_117);

    return terminated;
}
#endif

#ifdef MNS_120
void MnsForceInicReset(bool guarded)
{
    T_LIB_ENTRY(MNS_120);

    mns.forced_reset = MNS_TRUE;        /* avoid NSR_EI_RESET, NSR_E_INIT_TIMEOUT */
    MisForceInicReset(guarded);         /* do not count resets, do not stop init timer */

    T_MOD_EXIT(MNS_120);
}
#endif

#ifdef MNS_118
static byte MnsRequestVersionInfo(void)
{
    TMsgTx *msg_ptr;
    byte result;

    T_MOD_ENTRY(MNS_118);

    msg_ptr = NULL;
    result  = ERR_NO;
    msg_ptr = MsgGetTxPtrICM((word)1);	// Bug : Errata sheet 3.0.X-6 - 0300XX-016

    if (NULL != msg_ptr)
    {
        msg_ptr->Tgt_Adr   = MSG_TGT_INIC;
        msg_ptr->Func_ID   = FUNCID_INIC_VERSIONINFO;
        msg_ptr->Inst_ID   = (byte)0;
        msg_ptr->Operation = OP_GET;
        msg_ptr->Length    = (word)1;
        msg_ptr->Data[0]   = (byte)0; 

        MsgSend3(msg_ptr);
    }
    else
    {
        SCHEDULE_RETRY(MNS_RETRY_REQ_VERSION_INFO);
        result = ERR_BUFOV;
    }

    T_MOD_EXIT(MNS_118);
    return(result);
}
#endif

#ifdef MNS_119
static byte MnsGASPhaseComplete(void)
{
    byte result;
    TMsgTx *msg_ptr;

    T_MOD_ENTRY(MNS_119);

    result  = ERR_NO;
    msg_ptr = MsgGetTxPtrICM((word)2);	// Bug : Errata sheet 3.0.X-6 - 0300XX-016

    if (NULL != msg_ptr)
    {
        msg_ptr->Tgt_Adr   = MSG_TGT_INIC;
        msg_ptr->Func_ID   = FUNCID_INIC_EHCISTATE;
        msg_ptr->Inst_ID   = (byte)0;
        msg_ptr->Operation = OP_SET;
        msg_ptr->Length    = (word)2;
        msg_ptr->Data[0]   = EHCISTATE_SEMI_PROTECTED;
        msg_ptr->Data[1]   = EHCISTATE_NTF_ALL;

        MsgSend3(msg_ptr);
    }
    else
    {
        SCHEDULE_RETRY(MNS_RETRY_GAS_PHASE_COMPLETE);
        result = ERR_BUFOV;
    }

    if (ERR_NO == result)
    {
        (void)MnsCheckWatchdogMode();
    }

    T_MOD_EXIT(MNS_119);

    return(result);
}
#endif

#ifdef MNS_121
byte MnsWA340Fix(void)
{
    byte result;

    T_LIB_ENTRY(MNS_121);

    result = ERR_NO;

    if (MNS_FALSE != MnsWA340Enabled)                       /* do not execute before FW version is checked */
    {
        if (MNS_FALSE == MnsWA340PatchBusy)                 /* patch must not be performed as long as the msg handles are in use */
        {
            if (ERR_NO == WmcsSetINICMemPending(MNS_TRUE))  
            {
                if (MnsWA340Hndl1 && MnsWA340Hndl2 && MnsWA340Hndl3)
                {
                    MnsWA340PatchBusy = MNS_TRUE;
                    PmsSend(MnsWA340Hndl1, MnsWA340FixComplete);
                    PmsSend(MnsWA340Hndl2, MnsWA340FixComplete);
                    PmsSend(MnsWA340Hndl3, MnsWA340FixComplete);
                }

                (void)WmcsSetINICMemPending(MNS_FALSE);
            }
            else
            {
                SCHEDULE_RETRY(MNS_RETRY_HF340); 
                result = ERR_BUFOV;
            }
        }
        else
        {
            MnsWA340PatchRetry = MNS_TRUE;                  /* perform the patch when the msg handles are no longer used */
        }
    }

    T_LIB_EXIT(MNS_121);

    return(result);
}
#endif

#ifdef MNS_122
static byte MnsWA340FixComplete(HMBMBUF handle, byte status)
{
    TMsgTx *msg_ptr;

    #if !(MNS_DEBUG & NST_C_ASSERTS)
    (void)status;
    #endif

    T_MOD_ENTRY(MNS_122);

    ASSERT(XMIT_SUCCESS == status);

    msg_ptr = MBM_GET_CTRL_TX_PTR(handle);

    if (    (0x01 == msg_ptr->Length)                       /* retry possible after last msg transmitted to INIC */
         && (0x00 == msg_ptr->Data[0]))
    {
        MnsWA340PatchBusy = MNS_FALSE;
        if (MnsWA340PatchRetry)
        {
            MnsWA340PatchRetry = MNS_FALSE;
            (void)MnsWA340Fix();
        }
    }

    T_MOD_EXIT(MNS_122);

    return(PMS_RELEASE);
}
#endif


