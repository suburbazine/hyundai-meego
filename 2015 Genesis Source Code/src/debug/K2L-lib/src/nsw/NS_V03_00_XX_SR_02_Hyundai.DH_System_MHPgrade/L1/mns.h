/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Internal API of the MOST NetServices Kernel (Basic Layer)
File:           mns.h
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
  * \brief      Internal API of the MOST NetServices Kernel (Basic Layer)
  */

#ifndef _MNS_H
#define _MNS_H
/*lint -e(537) repeated include file */
#include "mostns1.h"

/*
==============================================================================
    Library internal definitions
==============================================================================
*/
#define MNS_TM_EV_TIMEOUT_INIC  ((byte) 0x01)
#define MNS_TM_EV_TIMEOUT_API   ((byte) 0x02)
#define MNS_TM_EV_TIMEOUT       ((byte) 0x03)
#define MNS_TM_EV_SCHEDULE      ((byte) 0x04)
#define MNS_TM_EV_UPDATE        ((byte) 0x05)
#define MNS_TM_EV_SERVICE       ((byte) 0x06)

#define MNS_TM_OP_DIFF          ((byte) 0x01)
#define MNS_TM_OP_UPDATE        ((byte) 0x02)
#define MNS_TM_OP_MIN_TIMEOUT   ((byte) 0x04)
#define MNS_TM_OP_SCHEDULE      ((byte) 0x08)

/*
==============================================================================
    Library internal macros
==============================================================================
*/
/*! Macro to ease the very common query if EHCIState is "attached"
      * currently.
      *
      * @author     SSemmler
      * @date       last update:  4/5/2005
      * @version    2.0.3
      * @see        MnsGetEHCIState
      */
    #define EHCISTATE_IS_PROTECTED() ((EHCISTATE_PROTECTED == MnsGetEHCIState()) || (EHCISTATE_UNKNOWN == MnsGetEHCIState()))
    #define EHCISTATE_IS_ATTACHED()  (EHCISTATE_ATTACHED == MnsGetEHCIState())

/*
==============================================================================
    Library internal types
==============================================================================
*/

typedef _CONST struct MnsEventEntry
{
    word          source_event;
    TMostEventCB *cb_ptr;
    word          target_event;
    word           mutex_id;

} MnsEventEntry, *MnsEventPtr;

typedef _CONST struct MnsServiceEntry
{
    word        service;
    MnsEventPtr entry_table;

} MnsServiceEntry, *MnsServicePtr;

/*
==============================================================================
    Library internal function prototypes
==============================================================================
*/

#ifdef MNS_2
    void MnsSetPendingService(word service_flag);
#endif

#ifdef MNS_3
    void MnsSetPendingEventFlag(word event, word pending_mutex_id,
                                word *pending_ptr, word service_to_call);
#endif

#ifdef MNS_4
    word MnsGetNextEventFlagToCall(word pending_mutex_id, word *pending_ptr,
                                   word *latest_called_ptr, word first,
                                   word last);
#endif

#ifdef MNS_9
    void MnsDistribEvent(word service, word event);
#endif

#ifdef MNS_11
    void MnsServiceInitComplete(byte phase, word service);
#endif

#ifdef MNS_12
    bool MnsIsNSInitComplete(word mask);
#endif

#ifdef MNS_20
    void MnsReportError(word code);
#endif
#ifdef MNS_26
    void MnsHandleVersionInfo(const TMsgRx *msg_ptr);
#endif
#ifdef MNS_27
    void MnsHandleEHCIStateStatus(const TMsgRx *msg_ptr);
#endif

#ifdef MNS_29
    void MnsRequestLayer2(word flags);
#endif

#ifdef MNS_30
    void MnsRequestLayer2Timer(word flags);
#endif

#ifdef MNS_31
    void MnsRequestHigh(word flags);
#endif

#ifdef MNS_32
    void MnsRequestHighTimerTx(void);
#endif

#ifdef MNS_33
    void MnsRequestHighTimerRx(void);
#endif

#ifdef MNS_34
    void MnsRequestPacket(word flags);
#endif

#ifdef MNS_35
    void MnsRequestPacketTimerTx(void);
#endif

#ifdef MNS_36
    void MnsRequestPacketTimerRx(void);
#endif

#ifdef MNS_43
    byte MnsGetEHCIState(void);
#endif
#ifdef MNS_53
    void MnsHandleWatchdogModeStatus(const TMsgRx *msg_ptr);
#endif

#ifdef MNS_55
    void MnsNtfCheck(dword flag);
#endif

#ifdef MNS_59
    _INLINE void MnsMemSet(byte* tgt_ptr, byte value, word size);
#endif

#ifdef MNS_60
    byte MnsRequestEHCIState(void);
#endif

#ifdef MNS_61
    byte MnsForceProtected(void);
#endif


#ifdef MNS_68
    void MnsHandleError(const TMsgRx *msg_ptr);
#endif

#ifdef MNS_69
    bool MnsPMReady(void);
#endif
#ifdef MNS_73
    void MnsCheckHigherLayers(void);
#endif

#ifdef MNS_74
    void MnsStartLayer2(void);
#endif

#ifdef MNS_75
    void MnsStopLayer2(void);
#endif

#ifdef MNS_76
    void MnsStartPacketLayers(void);
#endif

#ifdef MNS_77
    void MnsStopPacketLayers(void);
#endif

#ifdef MNS_96
    void MnsTMHandleEvent(byte event);
#endif

#ifdef MNS_108
    word MnsGetWatchdogKickTimeout(void);
#endif

#ifdef MNS_109
    bool MostNtfIsRequired(dword feature);
#endif

#ifdef MNS_120
    void MnsForceInicReset(bool guarded);
#endif

#ifdef MNS_121
    byte MnsWA340Fix(void);
#endif



#endif /* _MNS_H */



