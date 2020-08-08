/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Definition File for MOST NetServices (Basic Layer)
File:           mostdef1.h
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
  * \brief      Definition File for MOST NetServices (Basic Layer)
  */

#ifndef _MOSTDEF1_H
#define _MOSTDEF1_H
/* lint -save -e537 repeated include file */

#include "rules1_ad.h"


#include "mdm_ad.h"     /* Adjustment for Debug Messages
                         * Attention! This header file must be included
                         * first of all.
                         */
#include "mbm_ad.h"     /* Message Buffer Management Service                */
#include "pms_ad.h"     /* Adjustment for Port Message Service              */
#include "mis_ad.h"     /* Adjustment for Message Interface Service         */
#include "mns_ad.h"     /* Adjustment for MOST NetService Kernel            */
#include "ams_ad.h"     /* Adjustment for Application Message Service       */
#include "wmcs_ad.h"    /* Adjustment for MOST Processor Ctl Srv. Wrapper   */

#ifdef ADS_MIN
    #include "wads_ad.h"    /* Asynchronous Data Transmission Service Wrapper */
#endif
#ifdef VMSV_MIN
    #include "vmsv_ad.h"    /* Adjustment for Virtual MOST Supervisor         */
#endif

#ifdef SCM_MIN
    #include "wscm_ad.h"    /* Socket Connection Manager Wrapper              */
#endif
#ifdef SCMPM_MIN
    #include "scmpm_ad.h"   /* SCM Automatic Packet Mode                      */
#endif
#ifdef AAM_MIN
    #include "aam_ad.h"
#endif
#ifdef FSM_MIN
    #include "fsm_ad.h"
#endif
#ifdef GAS_MIN
    #include "gas_ad.h"
#endif

/* lint -restore */



/*
==============================================================================
    NetServices constants
==============================================================================
*/


/*
-------------------------------------------------------------------
    General
-------------------------------------------------------------------
*/

#define MNS_TRUE                            ((bool) 0x01)
#define MNS_FALSE                           ((bool) 0x00)
#define MNS_ON                              ((bool) 0x01)
#define MNS_OFF                             ((bool) 0x00)

#ifndef NULL
   #define NULL                             ((void *)0)
#endif

#define MNS_NET_ON                          ((byte) 0x04)
#define MNS_NET_NOTAVAIL                    ((byte) 0xFF)
#define MNS_SLAVE                           ((byte) 0x00)
#define MNS_MASTER                          ((byte) 0x01)
#define MNS_RETIMEDBYPASS_SLAVE             ((byte) 0x04)
#define MNS_RETIMEDBYPASS_MASTER            ((byte) 0x05)
#define MNS_DEVMODE_AUTO                    ((byte) 0xFF)
#define DIAGNOSIS                           ((byte) 0x02)
#define MNS_DEFAULT                         ((byte) 0x00)

#define DIAG_OK                             ((byte) 0xFF)
#define DIAG_POOR                           ((byte) 0xFC)
#define DIAG_POS0_SIGNAL                    ((byte) 0x40)

#define RBD_ACTIVITY_LOCK                   ((byte) 0x00)
#define RBD_ACTIVITY_NO_LOCK                ((byte) 0x01)
#define RBD_NO_ACTIVITY                     ((byte) 0x02)

/*
-------------------------------------------------------------------
    Errors on function call (Return values)
-------------------------------------------------------------------
*/
#define ERR_NO              ((byte) 0x00)
#define ERR_PARAM           ((byte) 0x01)
#define ERR_BUFOV           ((byte) 0x04)
#define ERR_NOTAVAIL        ((byte) 0x08)
#define ERR_NOT_SUPPORTED   ((byte) 0x10)
#define ERR_INVALID_SHADOW  ((byte) 0x20)
#define ERR_ALREADY_SET     ((byte) 0x40)
/*
-------------------------------------------------------------------
    Buffer Status Flags
-------------------------------------------------------------------
*/
#define BUF_FREE            ((byte) 0x00)
#define BUF_F_LOCK          ((byte) 0x01)
#define BUF_F_READY         ((byte) 0x02)
#define BUF_F_ACTIVE        ((byte) 0x04)
#define BUF_F_RXTRIGGER     ((byte) 0x08)
#define BUF_F_TXRETRY       ((byte) 0x08)

#define BUF_F_GBGCOL        ((byte) 0x10)
#define BUF_F_TXINTTGT      ((byte) 0x20)

/*
-------------------------------------------------------------------------------
 Thread Synchronization
-------------------------------------------------------------------------------
*/

#define MX_INVALID              ((word) 0xFFFF)

#define MX_MBM_CS               ((word) 0)

#define MX_PMS_PE               ((word) 1)
#define MX_PMS_CS               ((word) 2)
#define MX_PMS_Q                ((word) 3)

#define MX_MIS_PE               ((word) 4)

#define MX_MNS_SRV              ((word) 5)
#define MX_MNS_PS               ((word) 6)
#define MX_MNS_PE               ((word) 7)
#define MX_MNS_CS               ((word) 8)
#define MX_MNS_TM               ((word) 9)

#define MX_AMS_PE               ((word)10)
#define MX_AMS_CS               ((word)11)
#define MX_AMS_TX_Q             ((word)12)
#define MX_AMS_RX_Q             ((word)13)

#define MX_ADS_PE               ((word)14)
#define MX_ADS_RX_Q             ((word)15)

#define MX_VMSV_PE              ((word)16)
#define MX_VMSV_CS              ((word)17)

#define MX_WMCS_PE              ((word)18)
#define MX_WMCS_CS              ((word)19)

#define MX_WSCM_PE              ((word)20)
#define MX_WSCM_CS              ((word)21)

#define MX_AAM_PE               ((word)22)
#define MX_AAM_TX_Q             ((word)23)

#define MX_MDM_Q                ((word)24)
#define MX_MDM_CS               ((word)25)

#define MX_GAS_PE               ((word)26)

#define MNS_REQUIRED_MUTEXES    ((word)27)




/*
==============================================================================
    Legacy NetServices constants
==============================================================================
*/

/* This macro should only be enabled during migration from MNS V2.x to V3.x */
#ifdef MNSV3_ENABLE_LEGACY_MACROS
    #define MASTER       MNS_MASTER
    #define SLAVE        MNS_SLAVE
    #define NET_ON       MNS_NET_ON
    #define NET_NOTAVAIL MNS_NET_NOTAVAIL
    #ifdef TRUE
        #if (TRUE != 0x01)
            #error Use MNS_TRUE instead of TRUE
        #endif
    #else
        #define TRUE      MNS_TRUE
    #endif

    #ifdef FALSE
        #if (FALSE != 0x00)
            #error Use MNS_FALSE instead of FALSE
        #endif
    #else
        #define FALSE      MNS_FALSE
    #endif

#endif



/*
==============================================================================
    INIC constants
==============================================================================
*/
#define CTRL_MAX_PAYLOAD_OS81050            ((byte) 12)
#define CTRL_MAX_PAYLOAD_OS81110            ((byte) 45)


/*
==============================================================================
    FBlock INIC constants
==============================================================================
*/

#define FBLOCK_INIC                         ((byte) 0x00)
#define FBLOCK_NETBLOCK                     ((byte) 0x01)
#define FBLOCK_NETWORKMASTER                ((byte) 0x02)
#define FBLOCK_CONNECTMASTER                ((byte) 0x03)
#define FBLOCK_POWERMASTER                  ((byte) 0x04)
#define FBLOCK_DEBUG                        ((byte) 0x09)
#define FBLOCK_ET                           ((byte) 0x0F)
#define FBLOCK_ALL                          ((byte) 0xFF)

#define INST_ANY                            ((byte) 0x00)
#define INST_ALL                            ((byte) 0xFF)
#define INST_ID_UNUSED                      ((byte) 0x00)
#define INIC_SHADOW_INVALID_BYTE            ((byte) 0xEE)
#define INIC_SHADOW_INVALID_WORD            ((word) 0xEEEE)
#define INIC_SHADOW_INVALID_DWORD           ((dword) 0xEEEEEEEE)

#define FUNCID_INIC_FKTIDS                  ((word) 0x000)
#define FUNCID_INIC_NOTIFICATION            ((word) 0x001)
#define FUNCID_INIC_NOTIFICATIONCHECK       ((word) 0x002)
#define FUNCID_INIC_VERSIONINFO             ((word) 0x200)
#define FUNCID_INIC_RMCK                    ((word) 0x201)
#define FUNCID_INIC_TIMER                   ((word) 0x202)
#define FUNCID_INIC_RESET                   ((word) 0x203)
#define FUNCID_INIC_TMCLOCKSOURCE           ((word) 0x204)
#define FUNCID_INIC_TMCLOCKFREQUENCY        ((word) 0x205)
#define FUNCID_INIC_BIST                    ((word) 0x206)
#define FUNCID_INIC_EHCISTATE               ((word) 0x300)
#define FUNCID_INIC_FBLOCKIDSINTERNAL       ((word) 0x301)
#define FUNCID_INIC_ALTPACKETADDRESS        ((word) 0x302)
#define FUNCID_INIC_MIDLEVELRETRY           ((word) 0x303)
#define FUNCID_INIC_MCMRXBUFSIZE            ((word) 0x305)
#define FUNCID_INIC_ABILITYTOSEGMENT        ((word) 0x306)
#define FUNCID_INIC_TIMESTAMPENABLE         ((word) 0x307)
#define FUNCID_INIC_REMOTEACCESS            ((word) 0x308)
#define FUNCID_INIC_WATCHDOG                ((word) 0x309)
#define FUNCID_INIC_WATCHDOGMODE            ((word) 0x30A)
#define FUNCID_INIC_RETRYOPTIONS            ((word) 0x30B)
#define FUNCID_INIC_MEPFILTERMODE           ((word) 0x30C)
#define FUNCID_INIC_MEPHASHTABLE            ((word) 0x30D)
#define FUNCID_INIC_SETEUI48                ((word) 0x30E)
#define FUNCID_INIC_PACKETRETRYTIME         ((word) 0x30F)
#define FUNCID_INIC_OPENPORT                ((word) 0x400)
#define FUNCID_INIC_CLOSEPORT               ((word) 0x401)
#define FUNCID_INIC_REMOTEGETSOURCE         ((word) 0x402)
#define FUNCID_INIC_CREATESOCKET            ((word) 0x403)
#define FUNCID_INIC_DESTROYSOCKET           ((word) 0x404)
#define FUNCID_INIC_CONNECTSOCKETS          ((word) 0x405)
#define FUNCID_INIC_DISCONNECTSOCKETS       ((word) 0x406)
#define FUNCID_INIC_MUTEMODE                ((word) 0x407)
#define FUNCID_INIC_MEDIALBALLOCATEONLY     ((word) 0x408)
#define FUNCID_INIC_MEDIALBDEALLOCATEONLY   ((word) 0x409)
#define FUNCID_INIC_MUTECONNECTION          ((word) 0x40C)
#define FUNCID_INIC_DEMUTECONNECTION        ((word) 0x40D)
#define FUNCID_INIC_BANDWIDTH               ((word) 0x40E)
#define FUNCID_INIC_SOURCEDROP              ((word) 0x410)
#define FUNCID_INIC_SCERROR                 ((word) 0x411)
#define FUNCID_INIC_SCDEMUTE                ((word) 0x415)
#define FUNCID_INIC_SPDIF                   ((word) 0x416)
#define FUNCID_INIC_NWSTARTUP               ((word) 0x500)
#define FUNCID_INIC_NWSHUTDOWN              ((word) 0x501)
#define FUNCID_INIC_DEVICEMODE              ((word) 0x502)
#define FUNCID_INIC_NISTATE                 ((word) 0x503)
#define FUNCID_INIC_MAXIMUMPOSITION         ((word) 0x504)
#define FUNCID_INIC_NCEDELAYED              ((word) 0x505)
#define FUNCID_INIC_LOCKSTATE               ((word) 0x506)
#define FUNCID_INIC_RBDTRIGGER              ((word) 0x507)
#define FUNCID_INIC_RBDRESULT               ((word) 0x509)
#define FUNCID_INIC_NCSTATE                 ((word) 0x50A)
#define FUNCID_INIC_NIWAKEUPMODE            ((word) 0x50B)
#define FUNCID_INIC_NIEVENT                 ((word) 0x50C)
#define FUNCID_INIC_NUMCODINGERRORS         ((word) 0x50D)
#define FUNCID_INIC_PMISTATE                ((word) 0x50E)
#define FUNCID_INIC_SYSERRMONITOR           ((word) 0x511)
#define FUNCID_INIC_NETONTIMER              ((word) 0x512)
#define FUNCID_INIC_SSORESULT               ((word) 0x513)
#define FUNCID_INIC_MEMORY                  ((word) 0xE00)
#define FUNCID_INIC_CONF_STRING             ((word) 0xE10)
#define FUNCID_INIC_MEM_READ                ((word) 0xE12)
#define FUNCID_INIC_MEM_WRITE               ((word) 0xE13)
#define FUNCID_INIC_PMEM                    ((word) 0xE14)

#define INIC_NIEVENT_MPR_CHANGED            ((byte) 0x10)
#define INIC_NIEVENT_NET_ACTIVITY           ((byte) 0x80)
#define INIC_NIEVENT_SHORT_UNLOCK           ((byte) 0xE0)
#define INIC_NIEVENT_CRITICAL_UNLOCK        ((byte) 0xE1)
#define INIC_NIEVENT_TIMEOUT_CFG_STATUS     ((byte) 0xF1)
#define INIC_NIEVENT_NONE                   ((byte) 0xFF)

#define INIC_NOTIFICATION_SET_ALL       ((byte) 0x00)
#define INIC_NOTIFICATION_SET_FUNC      ((byte) 0x01)
#define INIC_NOTIFICATION_CLEAR_ALL     ((byte) 0x02)
#define INIC_NOTIFICATION_CLEAR_FUNC    ((byte) 0x03)

#define NISTATE_NET_OFF                 ((byte) 0x00)
#define NISTATE_NET_INIT                ((byte) 0x01)
#define NISTATE_NET_RBD                 ((byte) 0x02)
#define NISTATE_NET_ON                  ((byte) 0x03)
#define NISTATE_NET_RBDRES              ((byte) 0x04)

#define SDR_NO_RESULT                   ((byte) 0x00)
#define SDR_NO_FAULT_SAVED              ((byte) 0x01)
#define SDR_SUDDEN_SIGNAL_OFF           ((byte) 0x02)
#define SDR_CRITICAL_UNLOCK             ((byte) 0x03)


#define EHCISTATE_PROTECTED             ((byte) 0x00)
#define EHCISTATE_SEMI_PROTECTED        ((byte) 0x01)
#define EHCISTATE_ATTACHED              ((byte) 0x02)
#define EHCISTATE_UNKNOWN               ((byte) 0xFF)  /* NS internal only */
#define EHCISTATE_NTF_NO                ((byte) 0x00)
#define EHCISTATE_NTF_STD               ((byte) 0x01)
#define EHCISTATE_NTF_ALL               ((byte) 0x02)

#define NCS_NOT_OK                      ((byte) 0x00)
#define NCS_OK                          ((byte) 0x01)
#define NCS_INVALID_NWM                 ((word) 0xFFFF)
#define NCS_FBLOCKLIST_MASK             ((byte) 0x01)
#define NCS_SHUTDOWN_MASK               ((byte) 0x02)
#define NCS_NWMINST_MASK                ((byte) 0x04)

#define INIC_PARTNUMBER                 ((byte) 0x00)
#define INIC_HARDWARE_VERSION           ((byte) 0x01)
#define INIC_FIRMWARE_VERSION           ((byte) 0x02)
#define INIC_REVISION                   ((byte) 0x03)

#define NB_NOTIFICATION_SET_ALL         ((byte) 0x00)
#define NB_NOTIFICATION_SET_FUNC        ((byte) 0x01)
#define NB_NOTIFICATION_CLEAR_ALL       ((byte) 0x02)
#define NB_NOTIFICATION_CLEAR_FUNC      ((byte) 0x03)

#define FUNCID_NWM_CONFIGURATION        ((word) 0xA00)

#define INIC_ATS_NOEHCSEG               ((byte) 0x00)
#define INIC_ATS_ISSEHCSEG              ((byte) 0x01)
#define INIC_ATS_CMSEHCSEG              ((byte) 0x02)

#define INIC_MEM_CONF_STRING            ((dword) 0x01020000)    /* Configuration String Start Page */

#define INIC_MEM_PMEM_LEN               ((byte) 12)
#define INIC_MEM_CS_LEN                 ((byte) 32)

#define INIC_LOCKSTATE_UNLOCK           ((byte) 0x00)
#define INIC_LOCKSTATE_STABLELOCK       ((byte) 0x01)

/*------------------------------------------------------------------- */
/* FunctionIDs of FBlock NetBlock */
/*------------------------------------------------------------------- */
#define FUNC_FBLOCKIDS              ((word) 0x000)   /* FBlock IDs */
#define FUNC_DEVICEINFO             ((word) 0x001)   /* Device Info */
#define FUNC_NODEPOSITION           ((word) 0x002)   /* Node Position Address */
#define FUNC_NODEADDRESS            ((word) 0x003)   /* Node Address */
#define FUNC_GROUPADDRESS           ((word) 0x004)   /* Group Address */
#define FUNC_SHUTDOWN               ((word) 0x006)   /* Shut Down */
#define FUNC_RETRYPARAMETERS        ((word) 0x007)   /* Retry Parameters */
#define FUNC_SAMPLINGFREQUENCY      ((word) 0x008)   /* Sampling Frequency */
#define FUNC_SHUTDOWNREASON         ((word) 0x009)   /* Shut Down Reason*/
#define FUNC_VERSION                ((word) 0x010)   /* Version */
#define FUNC_FBLOCKINFO             ((word) 0x011)   /* FBlock Info */
#define FUNC_IMPLFBLOCKIDS          ((word) 0x012)   /* Implemented FBlocks */
#define FUNC_EUI48                  ((word) 0x013)   /* EUI48 identifier */
#define FUNC_NB_RBDRESULT           ((word) 0x405)   /* RBD Result */
#define FUNC_NB_BOUNDARY            ((word) 0xA03)   /* Boundary */
/*------------------------------------------------------------------- */

/*------------------------------------------------------------------- */
/* FunctionIDs of FBlock DebugMessages */
/*------------------------------------------------------------------- */
#define FUNC_ADJ_APP_DBG_MSG        ((word) 0x000)   /* Adjust_Application_DebugMessage */

/*
-------------------------------------------------------------------------------
 MOST Operation types
-------------------------------------------------------------------------------
*/

#define OP_SET              ((byte) 0x0)
#define OP_GET              ((byte) 0x1)
#define OP_SETGET           ((byte) 0x2)
#define OP_INC              ((byte) 0x3)
#define OP_DEC              ((byte) 0x4)
#define OP_GETINTERFACE     ((byte) 0x5)
/* OpTypes 0x6, 0x7 and 0x8 are not available for 'properties' since */
/* these OpTypes can only be used for 'methods' with sender handles ! */

#define OP_STATUS           ((byte) 0xC)
#define OP_INTERFACE        ((byte) 0xE)
#define OP_ERROR            ((byte) 0xF)

#define OP_START            ((byte) 0x0)
#define OP_ABORT            ((byte) 0x1)
#define OP_STARTRESULT      ((byte) 0x2)

#define OP_STARTRESULTACK   ((byte) 0x6)
#define OP_ABORTACK         ((byte) 0x7)
#define OP_STARTACK         ((byte) 0x8)

#define OP_ERRORACK         ((byte) 0x9)
#define OP_PROCESSINGACK    ((byte) 0xA)
#define OP_PROCESSING       ((byte) 0xB)
#define OP_RESULT           ((byte) 0xC)
#define OP_RESULTACK        ((byte) 0xD)

#define OP_REPORTS          ((byte) 0x9)   /* First OP_Type of a report msg */


/*
-------------------------------------------------------------------------------
 MOST Error codes
-------------------------------------------------------------------------------
*/

#define ERR_FBLOCKID            ((byte) 0x01)   /* FBlockID not available */
#define ERR_INSTID              ((byte) 0x02)   /* InstID not available */
#define ERR_FKTID               ((byte) 0x03)   /* FktID not available */
#define ERR_OPTYPE              ((byte) 0x04)   /* OPType not available */
#define ERR_LENGTH              ((byte) 0x05)   /* Invalid length */

#define ERR_PARAM_WRONG         ((byte) 0x06)   /* Param wrong/out of range */
#define ERR_PARAM_NOTAVAILABLE  ((byte) 0x07)   /* Parameter not available */
#define ERR_PARAM_MISSING       ((byte) 0x08)   /* Parameter missing */
#define ERR_PARAM_TOOMANY       ((byte) 0x09)   /* too many parameters */

#define ERR_SECONDARY_NODE      ((byte) 0x0A)   /* Secondary Node */
#define ERR_DEVICE_MALFUNC      ((byte) 0x0B)   /* Device Malfunction */
#define ERR_SEGMENTATION        ((byte) 0x0C)   /* Segmentation Error */

#define ERR_FUNC_SPECIFIC       ((byte) 0x20)   /* Function specific error */
#define ERR_BUSY                ((byte) 0x40)   /* Function busy */
#define ERR_NOTAVAILABLE        ((byte) 0x41)   /* Function not available at the moment*/
#define ERR_PROCESSING          ((byte) 0x42)   /* processing a method */
#define ERR_METHOD_ABORTED      ((byte) 0x43)   /* Method aborted */

/*
------------------------------------------------------------------------------
 TRACE
------------------------------------------------------------------------------
*/

#define NST_C_INIT              /*lint --e(960) */0x0001 /* Init runs            */
#define NST_C_STATES            /*lint --e(960) */0x0002 /* state changes        */
#define NST_C_TX                /*lint --e(960) */0x0004 /* TX messages          */
#define NST_C_RX                /*lint --e(960) */0x0008 /* RX messages (MIS)    */
#define NST_C_RX_MORE           /*lint --e(960) */0x0010 /* RX messages (AMS)    */
#define NST_C_SERVICE           /*lint --e(960) */0x0020 /* Service runs         */
#define NST_C_REQUESTS          /*lint --e(960) */0x0040 /* Service requests     */
#define NST_C_MOST_INIT         /*lint --e(960) */0x0080 /* InitNetServices      */
#define NST_C_MOST_SERVICE      /*lint --e(960) */0x0100 /* MostService runs     */
#define NST_C_MOST_REQUESTS     /*lint --e(960) */0x0200 /* MostService requests */
#define NST_C_BUF_USAGE         /*lint --e(960) */0x0400 /* Buffer usage info    */
#define NST_C_FUNC_ENTRIES      /*lint --e(960) */0x0800 /* Function entries     */
#define NST_C_FUNC_EXITS        /*lint --e(960) */0x1000 /* Function exits       */
#define NST_C_OPT_INFO          /*lint --e(960) */0x2000 /* Optimization infos   */
#define NST_C_MISC              /*lint --e(960) */0x4000 /* all the rest ...     */
#define NST_C_ASSERTS           /*lint --e(960) */0x8000 /* Enable ASSERTs       */

#define NST_L_NOTHING           /*lint --e(960) */0x0000
#define NST_L_BASIC             /*lint --e(960) */(NST_C_INIT | NST_C_STATES | NST_C_ASSERTS)
#define NST_L_COMM              /*lint --e(960) */(NST_L_BASIC | NST_C_TX | NST_C_RX)
#define NST_L_COMM_T            /*lint --e(960) */(NST_L_BASIC | NST_C_TX | NST_C_RX | NST_C_MISC)
#define NST_L_STANDARD          /*lint --e(960) */(NST_L_COMM | NST_C_SERVICE)
#define NST_L_STANDARD_T        /*lint --e(960) */(NST_L_COMM | NST_C_SERVICE | NST_C_REQUESTS | NST_C_MISC)
#define NST_L_ADVANCED          /*lint --e(960) */(NST_L_STANDARD | NST_C_BUF_USAGE)
#define NST_L_HEAVY             /*lint --e(960) */(NST_L_ADVANCED | NST_C_FUNC_ENTRIES)
#define NST_L_EVERYTHING        /*lint --e(960) */0xFFFF

#define NST_E_FUNC_ENTRY_API      ((word)  1)     /* function entry             */
#define NST_E_FUNC_EXIT_API       ((word)  2)     /* function exit              */
#define NST_E_FUNC_ENTRY_LIB      ((word)  3)     /* function entry             */
#define NST_E_FUNC_EXIT_LIB       ((word)  4)     /* function exit              */
#define NST_E_FUNC_ENTRY_MOD      ((word)  5)     /* function entry             */
#define NST_E_FUNC_EXIT_MOD       ((word)  6)     /* function exit              */
#define NST_E_INIT                ((word)  7)     /* init                       */
#define NST_E_SERVICE             ((word)  8)     /* service runs ...           */
#define NST_E_REQUEST             ((word)  9)     /* service request            */
#define NST_E_TX                  ((word) 10)     /* transmit                   */
#define NST_E_RX                  ((word) 11)     /* receive                    */
#define NST_E_MOST_INIT           ((word) 12)     /* InitNetServices            */
#define NST_E_MOST_SERVICE        ((word) 13)     /* MostService                */
#define NST_E_MOST_REQUEST        ((word) 14)     /* MostRequest                */
#define NST_E_STATE_CHANGE        ((word) 15)     /* a state changed            */
#define NST_E_ASSERT              ((word) 16)     /* Assert failed              */
#define NST_E_ALLOC_HANDLE        ((word) 17)     /* Allocate MBM               */
#define NST_E_FREE_HANDLE         ((word) 18)     /* Free MBM                   */
#define NST_E_CONVERT_HANDLE      ((word) 19)     /* Change type of MBM         */
#define NST_E_MALLOC_FAIL         ((word) 20)     /* Memory allocation failed   */
#define NST_E_TX_PACKET           ((word) 21)     /* transmit data packet       */
#define NST_E_RX_PACKET           ((word) 22)     /* receive data packet        */
#define NST_E_INIC_INIT           ((word) 23)     /* Chip & FW version          */
#define NST_E_TM_OP               ((word) 24)     /* Timer Management Event     */
#define NST_E_TM_EV               ((word) 25)     /* Timer Management Operation */
#define NST_E_TM_MIN_TIMEOUT      ((word) 26)     /* New minimum timeout        */
#define NST_E_TM_DIFF             ((word) 27)     /* Last time difference       */
#define NST_E_TM_FLAGS            ((word) 28)     /* Timer Management Flags     */
#define NST_E_TM_TBOFF            ((word) 29)
#define NST_E_TM_WATCHDOG_TIMEOUT ((word) 30)     /* WatchdogMode Timeout       */
#define NST_E_L2_SERVICE          ((word) 39)     /* MostServiceLayer2          */
#define NST_E_MHP_SERVICE         ((word) 40)     /* MostServiceHigh            */
#define NST_E_PMHS_SERVICE        ((word) 41)     /* MostServicePacket          */
#define NST_E_L2_REQUEST          ((word) 42)     /* MostRequestLayer2          */
#define NST_E_MHP_REQUEST         ((word) 43)     /* MostRequestHigh            */
#define NST_E_PMHS_REQUEST        ((word) 44)     /* MostRequestPacket          */
#define NST_E_START_L2            ((word) 45)     /* Start of higher layers     */
#define NST_E_STOP_L2             ((word) 46)     /* Stop of higher layers      */
#define NST_E_START_PL            ((word) 47)     /* Start of higher layers     */
#define NST_E_STOP_PL             ((word) 48)     /* Stop of higher layers      */
#define NST_E_TRANSITION_CAUSE    ((word) 49)     /* Cause of EHCI state        */
#define NST_E_MODULE_START        ((word) 50)     /* GAS module started         */
#define NST_E_ANALYSIS_START      ((word) 51)     /* Memory analysis started    */
#define NST_E_ANALYSIS_DONE       ((word) 52)     /* Memory analysis done       */
#define NST_E_ACTION_START        ((word) 53)     /* Action started             */
#define NST_E_ACTION_DONE         ((word) 54)     /* Action done                */
#define NST_E_MODULE_DONE         ((word) 55)     /* GAS module done            */
#define NST_E_MODULE_ERROR        ((word) 56)     /* GAS error occurred         */

#define NST_MAX_PCNT              6               /* max. no. of parameters     */

/*
------------------------------------------------------------------------------
 parameter level 1
------------------------------------------------------------------------------
*/

#define NST_P1_MOST_MSG      ((word) 0x0000)     /* MOST message (MCM)       */
#define NST_P1_DATA_PACKET   ((word) 0x0001)     /* MOST data packet (MDP)   */
#define NST_P1_INIC_MSG      ((word) 0x0002)     /* INIC message (ICM)       */
#define NST_P1_EHCISTATE     ((word) 0x0003)     /* MNS Attach state machine */
#define NST_P1_NTF_STATE     ((word) 0x0004)     /* Notification states      */
#define NST_P1_TM            ((word) 0x0005)     /* Timer management         */
#define NST_P1_INIT_PHASE    ((word) 0x0006)     /* MNS Init phase           */
#define NST_P1_RESET_PHASE   ((word) 0x0007)     /* MNS Reset phase          */
#ifdef MSV_DIAG_RESULT_MSG
    #define NST_P1_COMM      ((word) 0x0008)     /* RBD Phase 3              */
#endif

/*
------------------------------------------------------------------------------
 parameter level 2
------------------------------------------------------------------------------
*/

#define NST_P2_LEVEL_API    ((word) 0x0001)      /* API level        */
#define NST_P2_LEVEL_LIB    ((word) 0x0002)      /* library level    */
#define NST_P2_LEVEL_MOD    ((word) 0x0003)      /* module level     */

#define NST_P2_WAIT         ((word) 0x0001)      /* Wait state       */
#define NST_P2_ALT_WAIT     ((word) 0x0002)      /* Alternative Wait */
#define NST_P2_CHECK        ((word) 0x0003)      /* Check state      */


/*
------------------------------------------------------------------------------
 misc
------------------------------------------------------------------------------
*/

#define NST_PROP            ((byte) 0x10)

typedef unsigned int        MNS_UINT;


/*
==============================================================================
    Return value concept
==============================================================================
*/
typedef word TMnsResult;           /* may only change into > 16 bits ! */

#define NSR_FAILURE_BIT      ((TMnsResult)((TMnsResult)1 << 15))
#define NSR_SUCCESS(x)       (((TMnsResult)((TMnsResult)x & NSR_FAILURE_BIT) ? MNS_FALSE : MNS_TRUE))
#define NSR_FAILURE(x)       (((TMnsResult)((TMnsResult)x & NSR_FAILURE_BIT) ? MNS_TRUE : MNS_FALSE))
#define NSR_CODE_BYTE(x)     ((byte)((TMnsResult)((TMnsResult)x & (TMnsResult)(~NSR_FAILURE_BIT)) >> 8))
#define NSR_EXT_CODE_BYTE(x) ((byte)((TMnsResult)((TMnsResult)x & (TMnsResult)(~NSR_FAILURE_BIT)) >> 0))

#define NSR_BUILD_S(s)       ((TMnsResult)((MNS_FALSE != s)/*lint -e{506} */ ? (TMnsResult)0 : NSR_FAILURE_BIT))
#define NSR_BUILD_C(c)       ((TMnsResult)((TMnsResult)(((TMnsResult)c) & (TMnsResult)(~(TMnsResult)((TMnsResult)1<<7))) << 8))
#define NSR_BUILD_E(e)       ((TMnsResult)(((TMnsResult)e) << 0))

#define NSR_BUILD(s, c, e)   ((TMnsResult)(NSR_BUILD_S(s) | NSR_BUILD_C(c) | NSR_BUILD_E(e)))


#define NSR_S_OK                            NSR_BUILD(MNS_TRUE,  0x00, 0x00)
#define NSR_S_CE_INIC                       NSR_BUILD(MNS_TRUE,  0x01, 0x01)
#define NSR_S_CE_EXT_ONGOING                NSR_BUILD(MNS_TRUE,  0x01, 0x02)
#define NSR_S_CE_EXT_RESULT                 NSR_BUILD(MNS_TRUE,  0x01, 0x03)

#define NSR_E_FAILED                        NSR_BUILD(MNS_FALSE, 0x00, 0x00)
#define NSR_E_BUSY                          NSR_BUILD(MNS_FALSE, 0x01, 0x00)
#define NSR_E_INIC_VERSION                  NSR_BUILD(MNS_FALSE, 0x02, 0x00)
#define NSR_E_REATTACH                      NSR_BUILD(MNS_FALSE, 0x03, 0x00)
#define NSR_E_NO_COMM                       NSR_BUILD(MNS_FALSE, 0x04, 0x00)
#define NSR_E_NOT_SUPPORTED                 NSR_BUILD(MNS_FALSE, 0x05, 0x00)
#define NSR_E_PARAM                         NSR_BUILD(MNS_FALSE, 0x06, 0x00)
#define NSR_E_WRONG_PROTOCOL_CFG            NSR_BUILD(MNS_FALSE, 0x07, 0x00)
#define NSR_E_MCM_RX_CHANNEL                NSR_BUILD(MNS_FALSE, 0x08, 0x00)

#define NSR_E_ERR_FBLOCKID                  NSR_BUILD(MNS_FALSE, 0xA0, 0x01)
#define NSR_E_ERR_INSTID                    NSR_BUILD(MNS_FALSE, 0xA0, 0x02)
#define NSR_E_ERR_FKTID                     NSR_BUILD(MNS_FALSE, 0xA0, 0x03)
#define NSR_E_ERR_OPTYPE                    NSR_BUILD(MNS_FALSE, 0xA0, 0x04)
#define NSR_E_ERR_LENGTH                    NSR_BUILD(MNS_FALSE, 0xA0, 0x05)
#define NSR_E_ERR_PARAMWRONG                NSR_BUILD(MNS_FALSE, 0xA0, 0x06)
#define NSR_E_ERR_PARAMNOTAVAILABLE         NSR_BUILD(MNS_FALSE, 0xA0, 0x07)
#define NSR_E_ERR_PARAM_MISSING             NSR_BUILD(MNS_FALSE, 0xA0, 0x08)
#define NSR_E_ERR_PARAM_TOOMANY             NSR_BUILD(MNS_FALSE, 0xA0, 0x09)
#define NSR_E_ERR_DEVICE_MALFUNC            NSR_BUILD(MNS_FALSE, 0xA0, 0x0B)
#define NSR_E_ERR_FUNC_SPECIFIC             NSR_BUILD(MNS_FALSE, 0xA0, 0x20)
#define NSR_E_ERR_BUSY                      NSR_BUILD(MNS_FALSE, 0xA0, 0x40)
#define NSR_E_ERR_NOTAVAILABLE              NSR_BUILD(MNS_FALSE, 0xA0, 0x41)
#define NSR_E_ERR_PROCESSING                NSR_BUILD(MNS_FALSE, 0xA0, 0x42)
#define NSR_E_ERR_METHOD_ABORTED            NSR_BUILD(MNS_FALSE, 0xA0, 0x43)

#define NSR_E_CRS_INVALID_PORT_ID           NSR_BUILD(MNS_FALSE, 0xA1, 0x01)
#define NSR_E_CRS_PORT_NOT_OPENED           NSR_BUILD(MNS_FALSE, 0xA1, 0x02)
#define NSR_E_CRS_NET_OFF                   NSR_BUILD(MNS_FALSE, 0xA1, 0x04)
#define NSR_E_CRS_INVALID_DATA_TYPE         NSR_BUILD(MNS_FALSE, 0xA1, 0x05)
#define NSR_E_CRS_INVALID_DIRECTION         NSR_BUILD(MNS_FALSE, 0xA1, 0x06)
#define NSR_E_CRS_SOCKET_TABLE_FULL         NSR_BUILD(MNS_FALSE, 0xA1, 0x07)
#define NSR_E_CRS_INVALID_BLOCKWIDTH        NSR_BUILD(MNS_FALSE, 0xA1, 0x08)
#define NSR_E_CRS_INVALID_PREALLOC          NSR_BUILD(MNS_FALSE, 0xA1, 0x09)
#define NSR_E_CRS_INVALID_PROALLOC          NSR_E_CRS_INVALID_PREALLOC
#define NSR_E_CRS_INVALID_REQCHANLIST       NSR_BUILD(MNS_FALSE, 0xA1, 0x0B)
#define NSR_E_CRS_BUF_PTR                   NSR_BUILD(MNS_FALSE, 0xA1, 0x0C)
#define NSR_E_CRS_INVALID_MEDIALB_ADDR      NSR_BUILD(MNS_FALSE, 0xA1, 0x0D)
#define NSR_E_CRS_INVALID_MEDIALB_POS       NSR_BUILD(MNS_FALSE, 0xA1, 0x0E)
#define NSR_E_CRS_INVALID_LIST              NSR_BUILD(MNS_FALSE, 0xA1, 0x0F)
#define NSR_E_CRS_EXISTS_DIFFERENT          NSR_BUILD(MNS_FALSE, 0xA1, 0x10)
#define NSR_E_CRS_INVALID_OFFSET_SERIAL     NSR_BUILD(MNS_FALSE, 0xA1, 0x11)
#define NSR_E_CRS_MASTER_BUSY               NSR_BUILD(MNS_FALSE, 0xA1, 0x12)
#define NSR_E_CRS_NOT_ENOUGH_CHANNELS       NSR_BUILD(MNS_FALSE, 0xA1, 0x13)
#define NSR_E_CRS_INVALID_OFFSET_MEDIALB    NSR_BUILD(MNS_FALSE, 0xA1, 0x14)
#define NSR_E_CRS_INVALID_BLOCKWIDTH_EXT    NSR_BUILD(MNS_FALSE, 0xA1, 0x15)
#define NSR_E_CRS_DATA_FRAME_BLOCKWIDTH     NSR_BUILD(MNS_FALSE, 0xA1, 0x17)
#define NSR_E_CRS_DATA_SAMPLE_FREQ_REF      NSR_BUILD(MNS_FALSE, 0xA1, 0x18)
#define NSR_E_CRS_INSUFFICIENT_ISOC_RES     NSR_BUILD(MNS_FALSE, 0xA1, 0x19)

#define NSR_E_CRS_COMM                      NSR_BUILD(MNS_FALSE, 0xA1, 0x20)
#define NSR_E_CRS_CRC                       NSR_E_CRS_COMM
#define NSR_E_CRS_INIT                      NSR_BUILD(MNS_FALSE, 0xA1, 0x40)
#define NSR_E_CRS_BUF                       NSR_E_CRS_INIT
#define NSR_E_CRS_TIMEOUT                   NSR_BUILD(MNS_FALSE, 0xA1, 0x80)

#define NSR_E_DYS_BUF_PTR                   NSR_BUILD(MNS_FALSE, 0xA2, 0x02)
#define NSR_E_DYS_INVALID_HANDLE            NSR_BUILD(MNS_FALSE, 0xA2, 0x03)
#define NSR_E_DYS_HANDLE_USED               NSR_BUILD(MNS_FALSE, 0xA2, 0x04)
#define NSR_E_DYS_INTERNAL                  NSR_BUILD(MNS_FALSE, 0xA2, 0x05)
#define NSR_E_DYS_MEDIALB_BANDWIDTH         NSR_BUILD(MNS_FALSE, 0xA2, 0x06)
#define NSR_E_DYS_NET_UNLOCKED              NSR_BUILD(MNS_FALSE, 0xA2, 0x07)
#define NSR_E_DYS_MASTER_BUSY               NSR_BUILD(MNS_FALSE, 0xA2, 0x12)
#define NSR_E_DYS_WRONG_LABEL               NSR_BUILD(MNS_FALSE, 0xA2, 0x14)
#define NSR_E_DYS_COMM                      NSR_BUILD(MNS_FALSE, 0xA2, 0x20)
#define NSR_E_DYS_CRC                       NSR_E_DYS_COMM
#define NSR_E_DYS_INIT                      NSR_BUILD(MNS_FALSE, 0xA2, 0x40)
#define NSR_E_DYS_BUF                       NSR_E_DYS_INIT
#define NSR_E_DYS_TIMEOUT                   NSR_BUILD(MNS_FALSE, 0xA2, 0x80)

#define NSR_E_CNS_INVALID_HANDLE_IN         NSR_BUILD(MNS_FALSE, 0xA3, 0x01)
#define NSR_E_CNS_INVALID_HANDLE_OUT        NSR_BUILD(MNS_FALSE, 0xA3, 0x02)
#define NSR_E_CNS_INVALID_CON               NSR_BUILD(MNS_FALSE, 0xA3, 0x03)
#define NSR_E_CNS_INVALID_PTR               NSR_BUILD(MNS_FALSE, 0xA3, 0x04)
#define NSR_E_CNS_NET_OFF                   NSR_BUILD(MNS_FALSE, 0xA3, 0x05)
#define NSR_E_CNS_INVALID_BLOCKWIDTH        NSR_BUILD(MNS_FALSE, 0xA3, 0x06)
#define NSR_E_CNS_TABLE_FULL                NSR_BUILD(MNS_FALSE, 0xA3, 0x07)
#define NSR_E_CNS_DATA_SAMPLE_FREQ_REF      NSR_BUILD(MNS_FALSE, 0xA3, 0xB0)

#define NSR_E_DNS_INVALID_HANDLE            NSR_BUILD(MNS_FALSE, 0xA4, 0x01)
#define NSR_E_DNS_NET_OFF                   NSR_BUILD(MNS_FALSE, 0xA4, 0x02)

#define NSR_E_MAO_FAILED                    NSR_BUILD(MNS_FALSE, 0xA5, 0x01)
#define NSR_E_MAO_MEDIALB_FULL              NSR_BUILD(MNS_FALSE, 0xA5, 0x02)

#define NSR_E_MDO_INTERNAL                  NSR_BUILD(MNS_FALSE, 0xA6, 0x01)

#define NSR_E_NAO_NET_OFF                   NSR_BUILD(MNS_FALSE, 0xA7, 0x01)
#define NSR_E_NAO_TOO_MUCH                  NSR_BUILD(MNS_FALSE, 0xA7, 0x02)
#define NSR_E_NAO_INTERNAL                  NSR_BUILD(MNS_FALSE, 0xA7, 0x03)
#define NSR_E_NAO_MASTER_BUSY               NSR_BUILD(MNS_FALSE, 0xA7, 0x12)
#define NSR_E_NAO_FULL                      NSR_BUILD(MNS_FALSE, 0xA7, 0x13)
#define NSR_E_NAO_COMM                      NSR_BUILD(MNS_FALSE, 0xA7, 0x20)
#define NSR_E_NAO_CRC                       NSR_E_NAO_COMM
#define NSR_E_NAO_INIT                      NSR_BUILD(MNS_FALSE, 0xA7, 0x40)
#define NSR_E_NAO_BUF                       NSR_E_NAO_INIT
#define NSR_E_NAO_TIMEOUT                   NSR_BUILD(MNS_FALSE, 0xA7, 0x80)

#define NSR_E_NDO_INTERNAL                  NSR_BUILD(MNS_FALSE, 0xA8, 0x01)
#define NSR_E_NDO_USED                      NSR_BUILD(MNS_FALSE, 0xA8, 0x03)
#define NSR_E_NDO_INVALID_LABEL             NSR_BUILD(MNS_FALSE, 0xA8, 0x04)
#define NSR_E_NDO_NET_OFF                   NSR_BUILD(MNS_FALSE, 0xA8, 0x05)
#define NSR_E_NDO_MASTER_BUSY               NSR_BUILD(MNS_FALSE, 0xA8, 0x12)
#define NSR_E_NDO_WRONG_LABEL               NSR_BUILD(MNS_FALSE, 0xA8, 0x14)
#define NSR_E_NDO_COMM                      NSR_BUILD(MNS_FALSE, 0xA8, 0x20)
#define NSR_E_NDO_CRC                       NSR_E_NDO_COMM
#define NSR_E_NDO_INIT                      NSR_BUILD(MNS_FALSE, 0xA8, 0x40)
#define NSR_E_NDO_BUF                       NSR_E_NDO_INIT
#define NSR_E_NDO_TIMEOUT                   NSR_BUILD(MNS_FALSE, 0xA8, 0x80)

#define NSR_E_MUC_NET_OFF                   NSR_BUILD(MNS_FALSE, 0xA9, 0x01)

#define NSR_E_DMC_NET_OFF                   NSR_BUILD(MNS_FALSE, 0xAA, 0x01)

#define NSR_E_SOURCE_DROP                   NSR_BUILD(MNS_FALSE, 0xAB, 0x01)

#define NSR_E_MEDIALB_BANDWIDTH             NSR_BUILD(MNS_FALSE, 0xAC, 0x01)
#define NSR_E_MOST_BANDWIDTH                NSR_BUILD(MNS_FALSE, 0xAC, 0x02)
#define NSR_E_MOST_DEALLOCATED_ALL          NSR_BUILD(MNS_FALSE, 0xAC, 0x03)
#define NSR_E_PM_AUTO_MLB_BANDWIDTH         NSR_BUILD(MNS_FALSE, 0xAC, 0x06)
#define NSR_E_PM_AUTO_MLB_CH_ADDR           NSR_BUILD(MNS_FALSE, 0xAC, 0x07)
#define NSR_E_PM_AUTO_PORT_FAILED           NSR_BUILD(MNS_FALSE, 0xAC, 0x08)
#define NSR_E_PM_AUTO_SOCKET_FAILED         NSR_BUILD(MNS_FALSE, 0xAC, 0x09)
#define NSR_E_PM_IAUTO_NOT_SUPPORTED        NSR_BUILD(MNS_FALSE, 0xAC, 0x0A)

#define NSR_E_SRC_NET_OFF                   NSR_BUILD(MNS_FALSE, 0xAD, 0x01)
#define NSR_E_SRC_UNUSED                    NSR_BUILD(MNS_FALSE, 0xAD, 0x02)
#define NSR_E_SRC_INFO_TEMP_NOT_AVAIL       NSR_BUILD(MNS_FALSE, 0xAD, 0x03)

#define NSR_E_INVALID_RETRY_CONFIG          NSR_BUILD(MNS_FALSE, 0xC0, 0x02)

#define NSR_E_INVALID_REMOTE_ACCESS_CONFIG  NSR_BUILD(MNS_FALSE, 0xC1, 0x01)
#define NSR_E_INVALID_WATCHDOG_MODE_CONFIG  NSR_BUILD(MNS_FALSE, 0xC1, 0x02)

#define NSR_E_BIST_MEM_CONTENT              NSR_BUILD(MNS_FALSE, 0xC2, 0x11)
#define NSR_E_BIST_MEMORY                   NSR_BUILD(MNS_FALSE, 0xC2, 0x81)
#define NSR_E_BIST_CONF_STRING              NSR_BUILD(MNS_FALSE, 0xC2, 0x82)
#define NSR_E_BIST_PCODE                    NSR_BUILD(MNS_FALSE, 0xC2, 0x83)
#define NSR_E_BIST_DATA_MEMORY              NSR_BUILD(MNS_FALSE, 0xC2, 0x84)

#define NSR_EI_RESET                        NSR_BUILD(MNS_FALSE, 0xC3, 0x01)
#define NSR_EI_SYNC_FAILED                  NSR_BUILD(MNS_FALSE, 0xC3, 0x02)
#define NSR_EI_SYNC                         NSR_BUILD(MNS_FALSE, 0xC3, 0x03)
#define NSR_E_INIT_TIMEOUT                  NSR_BUILD(MNS_FALSE, 0xC3, 0x04)

#define NSR_E_INVALID_SEG_MODE_CONFIG           NSR_BUILD(MNS_FALSE, 0xC4, 0x01)
#define NSR_E_SEG_MODE_CONFLICT                 NSR_BUILD(MNS_FALSE, 0xC4, 0x02)
#define NSR_E_INVALID_MID_LEVEL_RETRIES_CONFIG  NSR_BUILD(MNS_FALSE, 0xC4, 0x03)

#define NSR_E_INIC_NOT_SUPPORTED            NSR_BUILD(MNS_FALSE, 0xC5, 0x01)
#define NSR_E_NO_FREE_INIC_MEMORY           NSR_BUILD(MNS_FALSE, 0xC5, 0x02)
#define NSR_E_INIC_GAS_ERROR                NSR_BUILD(MNS_FALSE, 0xC5, 0x03)

#define NSR_CODE_SYS_ERR_MONITOR            ((byte)0xC5)
#define NSR_EM_NONE                         NSR_BUILD(MNS_TRUE,  NSR_CODE_SYS_ERR_MONITOR, 0x00)
#define NSR_EM_CRITICAL_UNLOCK              NSR_BUILD(MNS_FALSE, NSR_CODE_SYS_ERR_MONITOR, 0x01)
#define NSR_EM_EHC_SHUTDOWN                 NSR_BUILD(MNS_FALSE, NSR_CODE_SYS_ERR_MONITOR, 0x02)
#define NSR_EM_EHC_SLO                      NSR_BUILD(MNS_FALSE, NSR_CODE_SYS_ERR_MONITOR, 0x04)
#define NSR_EM_WAKEUP                       NSR_BUILD(MNS_FALSE, NSR_CODE_SYS_ERR_MONITOR, 0x08)
#define NSR_EM_PROTECTED                    NSR_BUILD(MNS_FALSE, NSR_CODE_SYS_ERR_MONITOR, 0x10)
#define NSR_E_MEMORY_BUF_OV                 NSR_BUILD(MNS_FALSE, 0xC8, 0x01)



#define MNS_F_PACKET_COMMUNICATION_ONLY     ((word) 0x0002)
#define MNS_F_IGNORE_VERSION_CHECK          ((word) 0x0004)

#endif  /* _MOSTDEF1_H */
