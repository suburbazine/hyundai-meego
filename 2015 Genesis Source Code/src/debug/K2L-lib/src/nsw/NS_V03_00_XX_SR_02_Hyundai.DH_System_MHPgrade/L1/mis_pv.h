/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Private declarations and definitions of the Message Interface 
                Service (MIS)
File:           mis_pv.h
Version:        3.0.x-SR-2  
Language:       C
Author(s):      T.Jahnke
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
  * \brief      Private declarations and definitions of the Message Interface 
  *             Service (MIS)
  */

#ifndef _MIS_PV_H
#define _MIS_PV_H


/*
==============================================================================
    Includes
==============================================================================
*/

#include "mis.h"


/*
==============================================================================
    Rules
==============================================================================
*/

#ifndef MIS_MAX_FORCED_RESETS
    #define MIS_MAX_FORCED_RESETS   5
#endif


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

#ifdef MIS_MIN

    typedef struct MisData
    {
        word                pending_events;
        word                latest_handled_event;

        PmsInitStruct       pms_config;
        struct TLldConfig  *lld_cfg_ptr;

        bool                init_complete;
        byte                reset_counter;
        TMostTimer          reset_timer;
        TMdmInitStruct      mdm_config;
        #ifdef GAS_MIN
        TGasInitStruct      gas_config;
        #endif
    } TMisData;

/*
==============================================================================
    MIS handler table
==============================================================================
*/

#ifndef PACKET_COMMUNICATION_ONLY
    typedef void TMisHandlerFunc(const TMsgRx *msg_ptr);
    typedef TMisHandlerFunc *TMisHandlerFuncPtr;


    typedef struct TMisOperation
    {
        _CONST byte                op_type;
        _CONST TMisHandlerFuncPtr *handler_fptr_table;

    } TMisOperation;


    typedef struct TMisFunction
    {
        _CONST word           func_id;
        _CONST TMisOperation *op_type_table;

    } TMisFunction;


    typedef struct TMisFBlock
    {
        _CONST byte          fblock_id;
        _CONST TMisFunction *func_table;

    } TMisFBlock;

#endif /* #ifndef PACKET_COMMUNICATION_ONLY */

#endif /* #ifdef MIS_MIN */


/*
==============================================================================
    Module Internal Function Prototypes
==============================================================================
*/

#ifdef MIS_2
    static word MisGetNextEventToHandle(void);
#endif

#ifdef MIS_4
    static void MisHandleBufFreed(void);
#endif

#ifdef MIS_5
    static void MisHandlePmsEvent(word event);
#endif

#ifdef MIS_6
    static void MisHandleInicMsg(HMBMBUF handle);
#endif

#ifdef MIS_7
    static void MisHandleMostMsg(HMBMBUF handle);
#endif

#ifdef MIS_8
    static void MisHandlePacketData(HMBMBUF handle);
#endif



#endif /* #ifndef _MIS_PV_H */

/*
==============================================================================
    End Of File
==============================================================================
*/
