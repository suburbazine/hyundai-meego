/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Private declarations and definitions of the Asynchronous 
                Data Service Wrapper (WADS)
File:           wads_pv.h
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
  * \brief      Private declarations and definitions of the Asynchronous 
  *             Data Service Wrapper (WADS)
  */

#ifndef _WADS_PV_H
#define _WADS_PV_H


/*
==============================================================================
    Includes
==============================================================================
*/

#include "wads.h"
#include "pms.h"

/*
==============================================================================
    Module Internal Definitions
==============================================================================
*/

#define DATA_RSVD_MAX_SIZE    (42 + PMS_DATA_HDR_MAX_SIZE)   /*!< effective size of reserved buffer including PM header */

/*
==============================================================================
    Module Internal Structures
==============================================================================
*/

#ifdef ADS_MIN
    typedef struct WadsData
    {
        word     pending_events;
        word     latest_handled_event;

        #ifndef PACKET_COMMUNICATION_ONLY
            bool net_on;
        #endif

        #ifdef PACKET_COMMUNICATION_ONLY
            byte ni_state;

            byte sys_state;
        #endif

        MbmQueue rx_queue;

        #ifndef PACKET_COMMUNICATION_ONLY
        struct TDataRsvdBuf
        {
            MbmBuf mbm_buf;
            HMBMBUF handle;
            byte payload[DATA_RSVD_MAX_SIZE];
        } rsvd_buf;
        #endif

        struct TDataConfig *cfg_ptr;
    } TWadsData;
#endif  /* #ifdef ADS_MIN */

/*
==============================================================================
    Module Internal Function Prototypes
==============================================================================
*/

#ifdef ADS_2
    static void DataSetPendingEvent(word event_flag);
#endif

#ifdef ADS_3
    static word DataGetNextEventToHandle(void);
#endif

#ifdef ADS_6
    static byte DataTxFinal(HMBMBUF handle, byte status);
#endif

#ifdef ADS_9
    static void DataRxOutTrigger(HMBMBUF handle);
#endif



#endif /* #ifndef _WADS_PV_H */

/*
==============================================================================
    End Of File
==============================================================================
*/
