/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Internal API of Message Interface Service (MIS)
File:           mis.h
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
  * \brief      Internal API of Message Interface Service (MIS)
  */

#ifndef _MIS_H
#define _MIS_H

/*lint -e(537) repeated include file */
#include "mostns1.h"

#ifdef MIS_0
    void MisInit(struct TLldConfig  *lld_cfg_ptr
                 ,struct TPmsConfig  *pms_cfg_ptr
                 ,struct TMsgConfig  *msg_cfg_ptr
                 #if defined(DATA_TX_USER_PAYLOAD) && defined(ADS_MIN)
                 ,struct TDataConfig *data_cfg_ptr
                 #endif
                 #ifdef MDM_MIN
                 ,struct TMdmConfig *mdm_cfg_ptr
                 #endif
                 #ifdef GAS_MIN
                 ,struct TGasConfig *gas_cfg_ptr
                 #endif
                 );
#endif

#ifdef MIS_1
    void MisService(void);
#endif

#ifdef MIS_3
    void MisSetPendingEvent(word event_flag);
#endif

#ifdef MIS_9
    void MisResetInic(void);
#endif

#ifdef MIS_10
byte MisGetResetCount(void);
#endif

#ifdef MIS_11
void MisSetResetCount(byte count);
#endif

#ifdef MIS_12
void MisFilterMostMsg(HMBMBUF handle);
#endif

#ifdef MIS_13
void MisForceInicReset(bool guarded);
#endif

#endif /* _MIS_H */


