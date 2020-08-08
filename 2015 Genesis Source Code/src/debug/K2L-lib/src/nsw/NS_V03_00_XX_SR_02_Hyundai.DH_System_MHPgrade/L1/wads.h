/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Internal API of the Asynchronous Data Service Wrapper (WADS)
File:           wads.h
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
  * \brief      Internal API of the Asynchronous Data Service Wrapper (WADS)
  */

#ifndef _ADS_H
#define _ADS_H

#include "mostns1.h"

#ifdef ADS_0
    void DataInit(struct TDataConfig *cfg_ptr);
#endif

#ifdef ADS_1
    void DataService(void);
#endif

#ifdef ADS_8
    void DataRxTrigger(HMBMBUF handle);
#endif

#ifdef ADS_10
    void DataNIStateNetOn(bool on);
#endif

#endif  /* _ADS_H */

