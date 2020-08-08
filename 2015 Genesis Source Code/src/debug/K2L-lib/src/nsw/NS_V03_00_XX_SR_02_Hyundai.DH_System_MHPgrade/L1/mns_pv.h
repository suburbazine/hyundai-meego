/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Private declarations and definitions of MOST NetServices 
                Kernel (Basic Layer)
File:           mns.c
Version:        3.0.x-SR-2  
Language:       C
Author(s):      T.Jahnke, R.Hanke
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
  * \brief      Private declarations and definitions of the MOST NetServices 
  *             Kernel (Basic Layer)
  */

#ifndef _MNS_PV_H
#define _MNS_PV_H


/*
==============================================================================
    Includes
==============================================================================
*/
#include "mns.h"


/*
==============================================================================
    Module internal definitions
==============================================================================
*/

#define MNS_RETRY_TIMER                 ((word)0x0001)
#define MNS_RETRY_RESET_COMPLETE        ((word)0x0002)
#define MNS_RETRY_GO_SEMI_PROTECTED     ((word)0x0004)
#define MNS_RETRY_CHECK_WATCHDOGMODE    ((word)0x0008)
#define MNS_RETRY_REQ_EHCISTATE         ((word)0x0010)
#define MNS_RETRY_FORCE_PROTECTED       ((word)0x0020)
#define MNS_RETRY_REMOTE_ACCESS         ((word)0x0040)
#define MNS_RETRY_DISABLE_SCERROR       ((word)0x0080)
#define MNS_RETRY_REQ_VERSION_INFO      ((word)0x0100)
#define MNS_RETRY_GAS_PHASE_COMPLETE    ((word)0x0200)
#define MNS_RETRY_HF340                 ((word)0x0400) 

#define MNS_WD_SUPERVIS_CNT_START       ((word)0x0000)
#define MNS_WD_SUPERVIS_CNT_DISABLE     ((word)0xFFFF)

#define MNS_IS_VALID(a, b) (((a == (byte)0xFF) || (b == (byte)0xFF)) ? MNS_TRUE : ((a == b) ? MNS_TRUE : MNS_FALSE))
#define MNS_IS_NOT_VALID(a, b) ((MNS_FALSE != MNS_IS_VALID(a, b)) ? MNS_FALSE : MNS_TRUE)

#define MNS_WD_FLAG_BUSY_MASK           ((byte)1)
#define MNS_WD_FLAG_SCHEDULE_MASK       ((byte)2)


/*
==============================================================================
    Module Internal Definitions
==============================================================================
*/


/*
==============================================================================
    Module Internal Structures
==============================================================================
*/

#ifdef MNS_MIN

/*! This structure defines a line in the versions table file. During startup
  * the NetServices check whether there is a guilty line for the current system.
  *
  * @author     SSemmler
  * @date       last update  5/9/2005
  * @version    2.0.3
  */
typedef struct MnsVerCheckLine
{
    /*! The hardware version (aka part number and hardware version) */
    byte hw_ver[5];

    /*! The chip id assigned by the NetServices */
    byte chip_id;

    /*! The firmware version (major.minor.release) */
    byte fw_ver[3];

    /*! Notification mask (result messages needed till "init complete") */
    dword ntf_mask;

    /*! Supported features mask (e.g. ext. sockets, connection labels ...) */
    word feature_mask;

    /*! NetServices verion (major.minor.release.betacode) */
    byte ns_ver[4];

} MnsVerCheckLine;

/*! Data of the MNS module */
typedef struct MnsData
{
    /*! event masks for processing */
    struct MnsDataEvents
    {
        /*! 16-bit mask of pending events. */
        word pending;

        /*! 16-bit mask of the latest served event (1 or 0 bits set). */
        word latest;


        #ifdef NS_MNS_MNS2
        /*! 16-bit mask of pending events of Layer 2. */
        word pending_l2;
        #endif

        #ifdef NS_MNS_MHP
        /*! 16-bit mask of pending events of MHP. */
        word pending_mhp;
        #endif

        #ifdef NS_MNS_PMHS
        /*! 16-bit mask of pending events of Packetizer. */
        word pending_pmhs;
        #endif

        /*! Bit mask of the received result messages leading to "init complete" */
        dword ntf_state;

    } events;

    /*! service masks for processing */
    struct MnsDataServices
    {
        /*! 16-bit mask of pending services. */
        word pending;

        /*! 16-bit mask of the latest served service (1 or 0 bits set). */
        word latest;

        /*! Bit-mask to track the "complete" state of all services */
        word ms_init_mask;

        /*! Bit-mask to track the "reset" state of all services */
        word ms_reset_mask;

        /*! Flag if the "communication services" are ready (PMS, MIS, AMS) */
        bool ms_comm_ready;

        /*! Flag if the "basic services" are ready (PMS, MIS, AMS, MNS, WADS, GAS) */
        bool ms_base_ready;

        /*! Flag if the layer2 shall be serviced (Net On + Attached) */
        bool ms_layer2;

        /*! Flag if the packet layers (MHP and Packetizer) shall be serviced (Net On + Attached) */
        bool ms_packet_layers;

    } services;

    /*! Shadow values of INIC properties needed by MNS. */
    struct MnsDataShadow
    {
        /*! Shadow of INIC.EHCIState */
        byte ehci_state;

        /*! Version Information */
        struct MnsDataShadowVersion
        {
            /*! Holds the public part of the VersionInfo accessable via the
              * API.
              */
            TMostVersionInfo public;

            /*! Bit mask for the result messages needed to complete init */
            dword ntf_mask;

            /*! Bit mask of the supported features */
            word feature_mask;

        } version;

    } shadow;

    struct MnsDataTimeWatchdogMode
    {
        word timeout_wdsm;

        #ifndef MNS_DISABLE_WD_SUPERVISION
        word max_tx_busy_cnt;

        word tx_busy_cnt;
        #endif

    } watchdogmode;

    struct MnsDataTime
    {
        word last_tick;

        word diff;

        word min_timeout;

        word scheduled_timeout;

        #ifndef PACKET_COMMUNICATION_ONLY

        byte flags;

        #endif

    } tm;

    /* !message used for the watchdog message */
    struct MnsWatchdogMsg
    {
        /*! exclusive message pointer to the watchdog message, so we do not need to
          * mess with "no buffer" situations
          */
        TMsgTx *msg_ptr;

        /*! handle of mns.wd.msg_ptr */
        HMBMBUF handle;

        /*! Bit0 = busy */
        byte flags;

    } wd;

    /*! Timers used by the MNS. */
    struct MnsDataTimer
    {
        /*! Pointer to the first entry of the linked list (timer management) */
        TMostTimer *first_ptr;

        /*! Timer to monitor init phase (maximum timeout for semi-protected
          * mode).
          */
        TMostTimer init;

        /*! Implements the watchdog timeout (the presens of the timer alone
          * forced the necessary communication to prevent a timeout).
          */
        TMostTimer watchdog;

    } timer;




    /*! Retry flags used by the MNS. */
    word retry;


    /*! holds the configuration of the MOST NetServices as given
      * by the application before InitNetServices() was called.
      */
    TNetServicesConfig config;

    /*! is MNS_TRUE if the MOST NetServices shall go and remain in
      * EHCIState Protected
      */
    bool stopped;

    /*! is MNS_TRUE if the MOST NetServices have already called
      * the on_stop_netservices_fptr() callback function */
    bool signaled_ns_stopped;

    /*! is MNS_TRUE if MNS_NET_ON has to signalled to the AMS */
    bool signal_net_on;

    /*! is MNS_TRUE if a buf_freed event occurs during reattached */
    bool buf_freed;

    /*! stores the required value for remote access */
    byte remote_access;

    /*! is MNS_TRUE if MOST Debug Message "Initialization Timeout" is pending */
    bool pending_mdm;

    /*! Used to avoid error notifications when executing an INIC reset */
    bool forced_reset;

}MnsData;

#endif  /* #ifdef MNS_MIN */


/*
==============================================================================
    Module Internal Function Prototypes
==============================================================================
*/

#ifdef MNS_5
    static word MnsGetNextServiceToCall(void);
#endif

#ifdef MNS_6
    static bool MnsService(void);
#endif

#ifdef MNS_7
    static _INLINE void MnsSetPendingEvent(word event_flag);
#endif

#ifdef MNS_8
    static word MnsGetNextEventToHandle(void);
#endif

#ifdef MNS_10
    static void MnsInit(void);
#endif

#ifdef MNS_21
    static void MnsReportInitComplete(void);
#endif

#ifdef MNS_22
    static byte MnsEHCIGoProtected(void);
#endif
#ifdef MNS_23
    static byte MnsEHCIGoSemiProtected(void);
#endif

#ifdef MNS_24
    static void MnsEHCIGoAttached(void);
#endif
#ifdef MNS_49
    static void MnsFireWatchdogTrigger(void);
#endif

#ifdef MNS_51
    static byte MnsRetry(void);
#endif

#ifdef MNS_52
    static byte MnsCheckWatchdogMode(void);
#endif

#ifdef MNS_66
    static byte MnsEnforceRemoteAccess(void);
#endif

#ifdef MNS_70
    static byte MnsResetComplete(void);
#endif

#ifdef MNS_97
    static void MnsTMUpdate(void);
#endif

#ifdef MNS_98
    static void MnsTMDiff(void);
#endif

#ifdef MNS_100
    static void MnsTMSchedule(void);
#endif

#ifdef MNS_104
    static byte MnsWDTxComplete(HMBMBUF handle, byte status);
#endif

#ifdef MNS_105
    byte MnsDisableSCError(void);
#endif

#ifdef MNS_106
    bool MnsTimerRegistered(TMostTimer* timer_ptr);
#endif

#ifdef MNS_107
    void MnsReRegisterTimer(TMostTimer* timer_ptr, TMostEventCB *cb_ptr, word event);
#endif

#ifdef MNS_113
    static _INLINE void MnsSendMdmMnsInitTimeout(void);
#endif

#ifdef MNS_114
    static _INLINE void MnsSendMdmMnsVersionConflict(void);
#endif

#ifdef MNS_115
    static _INLINE void MnsCheckNbminNotification(void);
#endif

#ifdef MNS_116
    static _INLINE void MnsPrepareReInit(void);
#endif

#ifdef MNS_117
    static _INLINE bool MnsCheckTermination(void);
#endif

#ifdef MNS_118
    static byte MnsRequestVersionInfo(void);
#endif

#ifdef MNS_119
    static byte MnsGASPhaseComplete(void);
#endif

#ifdef MNS_122
    static  byte MnsWA340FixComplete(HMBMBUF handle, byte status);
#endif


#define WA340_VERSION_NUMBER_MAJOR   ((byte) 0x01)
#define WA340_VERSION_NUMBER_MINOR   ((byte) 0x06)
#define WA340_VERSION_NUMBER_BUILD   ((byte) 0x04)




#endif /* #ifndef _MNS_PV_H */

/*
==============================================================================
    End Of File
==============================================================================
*/
