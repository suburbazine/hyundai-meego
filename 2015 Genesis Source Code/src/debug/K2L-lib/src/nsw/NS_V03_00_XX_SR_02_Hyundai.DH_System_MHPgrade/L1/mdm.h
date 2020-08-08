/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Internal API of the MOST Debug Message Module (MDM)
File:           mdm.h
Version:        3.0.x-SR-2  
Language:       C
Author(s):      R.Hanke
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
  * \brief      Internal API of the MOST Debug Message Module (MDM)
  */

#ifndef _MDM_H
#define _MDM_H


/*
==============================================================================
    Includes
==============================================================================
*/

#include "mostns1.h"


/*
==============================================================================
    Library Global Structures
==============================================================================
*/


/*
==============================================================================
    Library internal types
==============================================================================
*/

/*! \brief      Initialization structure for MDM module */
typedef struct
{
    void (*app_config_fptr)(void);          /*!< Callback pointer for application initialization */

} TMdmInitStruct;


/*
==============================================================================
    Library Function Prototypes
==============================================================================
*/

#ifdef MDM_0
    extern void MdmInit(const TMdmInitStruct *is_ptr);
#endif  /* #ifdef MDM_0 */

#ifdef MDM_1
    extern void MdmMsgSendMns(word case_id, const byte * const data_ptr, byte length);
#endif  /* #ifdef MDM_1 */

#endif /* #ifndef _MDM_H */


/*
==============================================================================
    End Of File
==============================================================================
*/
