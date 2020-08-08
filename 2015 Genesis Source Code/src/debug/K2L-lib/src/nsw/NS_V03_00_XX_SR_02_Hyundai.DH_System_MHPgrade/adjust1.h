/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Sample Adjustment for Layer I
File:           adjust1.h
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


#ifndef _ADJUST1_H
#define _ADJUST1_H


//#define MOST_INIC_ID          0x03      /* Select the used INIC derivative:   */
                                        /*     0x02:  INIC50 (e.g.,OS81082)   */
                                        /*     0x03:  INIC150(e.g.,OS81110)   */

#ifndef MOST_INIC_ID
#error "MOST_INIC_ID has to be defined"
#endif

#define PACKET_COMMUNICATION_ONLY

/* -----------------------------------------------------------------------------
 * Interface to Application Layer
 * -----------------------------------------------------------------------------
*/

#ifndef PACKET_COMMUNICATION_ONLY
   #define SERVICE_LAYER_II         /* Enable MOST NetServices Layer II */
/* #define NS_AMS_AH        */      /* Enable AH support for the AMS        */
   #define NS_MNS_MNS2              /* Enable Kernel support            */
   #define NS_MSV_NB                /* Enable VMSV support of the NB    */
/* #define NS_AMS_MSV2      */      /* Enable interface between AMS and MSV2*/
/* #define NS_MSV_ET        */      /* Enable interface between MSV and ET  */
#endif

/* -----------------------------------------------------------------------------
 * Interface to MOST High Protocol Service
 * -----------------------------------------------------------------------------
 */

#define MOST_HIGH_PROTOCOL   /* Enable MOSThigh Protocol */
#define NS_MNS_MHP           /* Enable Kernel support    */
#define NS_WADS_MHP          /* Enable wADS support      */
/* #define NS_PMS_PMHT        */    /* Enable PMS support for PMHT  */


/*
------------------------------------------------------------------------------
 Interface to Packetizer for MOST High Protocol Service
------------------------------------------------------------------------------
*/

#define DISABLE_CFG_MSG_FILTER
/* #define PACKETIZER_FOR_MOST_HIGH */
/* #define NS_MNS_PMHS              */


#if (defined SERVICE_LAYER_II) || (defined MOST_HIGH_PROTOCOL)
#define DECLARE_LEGACY_SWITCHES
#define DECLARE_LEGACY_API
#endif


/*
------------------------------------------------------------------------------
 Message Buffer Module Configuration
------------------------------------------------------------------------------
*/

/* The unit size defines the smallest amount of payload the MBM will allocate.
 * Valid range: 1..128 (bytes)
 */
 #define MBM_MEM_UNIT_SIZE       4 


 /* The unit number is multiplied by unit size to determine the amount of memory
  * to reserve for payload buffers. The unit number may not exceed 65535.
 */
 #define MBM_MEM_UNIT_NUM        10000 


/* The message number is the number of buffer handles allocated for use.
 */
 #define MBM_MSG_NUM             150 


/* Defines the number of buffers which are exclusively reserved for RX.
 * The sum of RX and TX must never exceed MBM_MSG_NUM.
 */
 #define MBM_MSG_RSVD_RX         35 


/* Defines the number of buffers which are exclusively reserved for TX.
 * The sum of RX and TX must never exceed MBM_MSG_NUM.
 */
 #define MBM_MSG_RSVD_TX         35 


/* The following macro may solve compiler specific misalignment of data payload.
 * The defined expression will extend the definition of the internal payload 
 * memory pool. Consider to set the option if MbmGetBufData() returns an
 * address which is not 4 byte aligned (MBM_MEM_UNIT_SIZE = 4) and the controller
 * does not support direct access of unaligned memory.
 */
/* #define MBM_ALIGN_OPT1      e.g. __attribute__ ((aligned (MBM_MEM_UNIT_SIZE))) */


/* Define the following macro to force an alignment of the message extension fields
 * ExtData and TxHandle.
 */
/* #define MBM_ALIGN_OPT2 */


/*
------------------------------------------------------------------------------
 Application Message Service Configuration
------------------------------------------------------------------------------
*/

/* Define this macro to enable Byte order functions 
 */
#define AMS_ADD1 */


/* Time to wait until next mid level retry (in ms)
 */
 #define TIME_MSG_TX_RETRY       20 


/* Default number of mid level retries for messages sent from application
 */
 #define DEF_MID_LEVEL_RETRIES   3 


/* Default number of mid level retries for internal messages 
 */
 #define DEF_MID_LEVEL_RETRIES_INT_PROC  2 


/* Size of optional field that can be used e.g. as transmission handle
 */
 #define MAX_TX_HANDLE           4 


/* Size of optional field that can be used e.g.for a timestamp, etc.
 */
 #define MAX_EXT_DATA            4 


/* Define this macro to disable TX segementation
 */
/* #define AMS_TX_NOSEG */


/* Define this macro to enable the application to bypass the AMS buffer queue 
 * in case of single telegram messages.
 */
/* #define AMS_TX_BYPASS_FILTER */


/* Applicable for MOST50 only:
 * Define one of 3 possible macros to determine the Rx segmentation mode. 
 * If none is selected, MSG_RX_SEG_COOPERATIVE is chosen as default value.
 */
/* #define MSG_RX_SEG_COOPERATIVE */
/* #define MSG_RX_SEG_INIC_ONLY */
/* #define MSG_RX_SEG_EHC_ONLY */


/* User supplies the payload buffer for incoming messages to where the received
 * payload is copied before handed to the application. In this case it is not allowed 
 * to define PMS_RX_SEG_PREALLOC_SIZE.
 */
/* #define MSG_RX_USER_PAYLOAD */


/* Define this if user allocated payload is intended to be used, i.e. if the
 * tx_ptr->Data is set by the application.
 */
/* #define MSG_TX_USER_PAYLOAD */


/* Define this macro to enable an advanced callback function for freeing control Tx
 * user payload.
 */
/* #define MSG_TX_USER_PAYLOAD_EXT_CB */


/*
------------------------------------------------------------------------------
 Application Message Arbitration Manager Service Configuration
------------------------------------------------------------------------------
*/

/*#define AAM_MIN*/                         /* This macro enables the AAM module */


#define AAM_MAX_NUM_JOBS    4               /* determines the maximum number of simultaneous
                                             * TX jobs, which can be handled by the Application
                                             * Message Arbitration Manager
                                             */

#define AAM_INTERLEAVE_LEVEL 0              /* Determines if targets for AAM messages are distinguished
                                             * on DeviceID, logical address (FBlockID/InstID) or FktID level.
                                             */

#define AAM_NO_COPY_ENABLE                  /* If this macro is defined, the AAM service provides
                                             * the payload of the message by a pointer instead of
                                             * copying it to an MBM buffer. This may increase the
                                             * message throughput depending on the system environment.
                                             */


/*
------------------------------------------------------------------------------
 Application Data Service Configuration
------------------------------------------------------------------------------
*/

/* This macro enables the Application Data Service Wrapper(wADS)
 */
 #define ADS_MIN 

/* Number of initially preallocated RX buffers
 * Packet data transmission via I2C:     0
 * Packet data transmission via MediaLB: 1 or higher
 * Default value: 1
 */
 #define ADS_RX_PREALLOC        1 

/* Size of optional field that can be used e.g. as transmission handle, etc.
 */
 #define MAX_DATA_TX_HANDLE     0 


/* Size of optional field that can be used e.g.for a timestamp, etc.
 */
 #define MAX_DATA_EXT_DATA      0 


/* Define this if user allocated payload is intended to be used, i.e. if the
 * tx_ptr->Data is set by the application. 
 */
/* #define DATA_TX_USER_PAYLOAD */


/* Define this macro to enable an advanced callback function for freeing data Tx
 * user payload.
 */
/* #define DATA_TX_USER_PAYLOAD_EXT_CB */


/* Default Retry value for MDP packets. Only used for OS81110
 */
 #define MDP_DEFAULT_RETRY      0x1F 

/* Enables optional internal call of PmsTxStarted 
 * if LLD does not support the NS_PMS_PMHT feature
 */
/* #define ADS_OPT1 */    /* only to be used, if LLD does not support PmsTxStarted() */



/*
------------------------------------------------------------------------------
 MOST NetServices Kernel Configuration
------------------------------------------------------------------------------
*/

/*  Packet communication is on this host the other layers on the EHC
 */
/* #define PACKET_COMMUNICATION_ONLY */


/*  This host has a PACKET_COMMUNICATION_ONLY counter part
 */
/* #define PACKET_ON_SECOND_HOST */


/* determines the watchdog timeout margin possible values are:
 * 1 - timeout margin is 50% of t_INIC_watchdog
 * 2 - timeout margin is 25% of t_INIC_watchdog
 * default value is 2
 */
/* #define MNS_TM_WATCHDOG_MARGIN 2 */


/* This macro allows to extend the max. value of the watchdog timeout (in ms).
 * The value must not exceed 5000 ms.
 */
/* #define MNS_TM_WD_EXTENDED_TIMEOUT 1000 */



/*
------------------------------------------------------------------------------
 Virtual MOST Supervisor Configuration
------------------------------------------------------------------------------
*/

#define VMSV_MIN

/* This macro enables the optional transmission of the Ring Break Diagnosis 
 * result message (NetBlock.RBDResult.Status) over the MOST network.
 */
/* #define MSV_DIAG_RESULT_MSG */



/*
------------------------------------------------------------------------------
 Socket Connection Manager Wrapper Configuration
------------------------------------------------------------------------------
*/

#ifndef PACKET_COMMUNICATION_ONLY
#define SCM_MIN
#define SCMPM_MIN                       /* Enables the SCM Automatic Packet Mode Feature */
#endif



/*
------------------------------------------------------------------------------
 Port Message Service Configuration
------------------------------------------------------------------------------
*/

/* Enables double buffering between PMS an INIC
 */
 #define PMS_DBL_BUFFER_ENABLE */


/* When the macro is defined a payload buffer of this size is pre-allocated 
 * when receiving the first segment of an incoming segmented control message.
 * The max. size for incoming control messages is then limited to 
 * PMS_RX_SEG_PREALLOC_SIZE.
 * If this macro is not defined MSG_RX_USER_PAYLOAD must be defined. In this 
 * case the application can give the intended size of payload.
 */
 #define PMS_RX_SEG_PREALLOC_SIZE   4000 


/* MOST150 feature, enables RX optimization III
 */
/* #define PMS_RX_OPT3 */


/* Specifies the MCM RX channel address which is used if
 * PMS_RX_OPT3 is enabled
 */
/* #define PMS_RX_OPT3_CHADDR   0x3E */


#ifdef offsetof

    /* (1) if an implementation of stdlib's offsetof is available */
    #define PMS_OFFSETOF    offsetof

#else

    /* (2) with NULL pointer */
    #define PMS_OFFSETOF(s,m)   ((unsigned int)&(((s *)0)->m))

    /* (3) without NULL pointer
     *
     * extern word pmsPending;
     * #define PMS_OFFSETOF(s,m)   ((unsigned int)( (byte *)&((s *)&pmsPending)->m - (byte *)&pmsPending ))
     */

#endif

/*
------------------------------------------------------------------------------
 MEP Configuration
------------------------------------------------------------------------------
*/

/* MOST150 feature, enable helper functions and macros to identify MEP packets and
 * build the transmit header.
 */
/* #define MEP_MIN */


/* Default Retry value for MEP packets Only used for OS81110
 */
 #define MEP_DEFAULT_RETRY      0x44 


/*
------------------------------------------------------------------------------
 Debug Messages Configuration
------------------------------------------------------------------------------
*/

/* Debug Level. Define one of the following values:
 * 0 - None       (Module disabled)
 * 1 - Error
 * 2 - Warning
 * 3 - Info
 */
#define MDM_LEVEL                   1


/* MDM_RSVD_MNS and MDM_RSVD_APP are used to tell how many buffers are exclusively
 * reserved for the MNS and the application respectively. The sum of both values must
 * never exceed MDM_NUM_BUF.
 */
#define MDM_RSVD_MNS                3
#define MDM_RSVD_APP                3


/* Total number of debug message buffers
 */
#define MDM_NUM_BUF                 10


/* Maximum number of bytes for case specific values of application debug messages.
 * (0..38 Byte)
 */
#define MDM_MAX_MSG_SIZE_APP        20


/* Maximum number of MOST function ids used for application debug messages.
 */
#define MDM_APP_NUM_FUNC_IDS        5



/*
------------------------------------------------------------------------------
 Memory Copy, Memory Set and Offset Functions
------------------------------------------------------------------------------
*/

/* This macro enables a user defined memory copy function. This function can be 
 * adapted to the used hardware (e.g. DMA, Cache Alignments, DWord instead of 
 * Byte copy loop). Projects including the C Standard Library may simply define 
 * "memcpy". The function signature must be:
 * void FunctionName(byte* targetPointer, const byte* sourcePointer, word size)
 */
/* #define MNS_MEM_CPY      memcpy */


/* This macro enables a user defined memory set function. This function can be 
 * adapted to the used hardware (e.g. DMA, Cache Alignments, DWord instead of 
 * Byte copy loop). Projects including the C Standard Library may simply define 
 * "memset". The function signature must be:
 * void FunctionName(byte* tgt_ptr, byte value, word size)
 */
/* #define MNS_MEM_SET      memset */


/* The following macro retrieves the offset of a member from the beginning of its parent 
 * structure. Since there is no 100% portable and ANSI-compliant way to do this, it is
 * recommended to map this macro to an own implementation or the cstdlib macro "offsetof".
 * The macro "offsetof" should be defined in <stddef.h>.
 */
#ifdef offsetof
   #define PMS_OFFSETOF     offsetof
#endif



/*
------------------------------------------------------------------------------
 Data type definitions
------------------------------------------------------------------------------
*/

/* The NetServices use the type names bool, byte, word and dword to identify
 * boolean values, unsigned 8-bit integers, unsigned 16-bit integers and
 * unsigned 32-bit integers.
 * Note: Boolean values are written as 1 (MNS_TRUE) and 0 (MNS_FALSE)!
 */
#include <stdint.h>

#ifndef __cplusplus
#ifndef __bool_true_false_are_defined
    typedef unsigned char  bool; // boolean
#endif
#endif

typedef uint8_t   byte;  // 8 bit unsigned integer
typedef uint16_t  word;  // 16 bit unsigned integer
typedef uint32_t  dword; // 32 bit unsigned integer


/*
------------------------------------------------------------------------------
 Operating system abstraction
------------------------------------------------------------------------------
*/

/* The WAIT4MX() and REL_MX() macros are called to take and release mutexes.
 * The application shall assign these functions if MOST NetServices functions
 * are called from multiple threads or processes.
 * The constant MNS_REQUIRED_MUTEXES defines the amount of required mutexes and
 * the functions take zero-based indexes (0 .. MNS_REQUIRED_MUTEXES-1).
 */
extern void mns_take(int mutex_id);
extern void mns_give(int mutex_id);

#define WAIT4MX(n)  //mns_take(n)
#define REL_MX(n)   //mns_give(n)


/*
------------------------------------------------------------------------------
 Compiler & target specific qualifiers
------------------------------------------------------------------------------
*/

/* Rom data qualifier, this is used for constant tables that needs to be
 * valid after startup.
 */
 #define _CONST const 

/* This macro provides the posibility to enable "inline" handling as supported
 * by the C99 Standard or the GNU C compiler (and a lot others).
 */
 #define _INLINE 

/* high Byte of most significant Word  */
 #define MSW_HB(dw)      (byte)((dword)((dword)(dw) >> 24) & (dword)0xFF) 

/* low Byte of most significant Word   */
 #define MSW_LB(dw)      (byte)((dword)((dword)(dw) >> 16) & (dword)0xFF) 

/* high Byte of least significant Word */
 #define LSW_HB(dw)      (byte)((dword)((dword)(dw) >> 8) & (dword)0xFF) 

/* low Byte of least significant Word  */
 #define LSW_LB(dw)      (byte)((dword)(dw) & (dword)0xFF) 

/* high Byte of 16bit value  */
 #define HB(value)       (byte)((word)(value) >> 8) 

/* low Byte of 16bit value   */
 #define LB(value)       (byte)((word)(value) & (word)0xFF) 

/* Big-Endian to target 16 bit   */
 #define DECODE_WORD(w_ptr, msb_ptr) (*(w_ptr) = (word)((word)((word)(msb_ptr)[0] << 8) | (word)(msb_ptr)[1])) 

/* Big-Endian to target 32 bit   */
 #define DECODE_DWORD(dw_ptr, msb_ptr) (*(dw_ptr) = (dword)((dword)((dword)(msb_ptr)[0] << 24) | (dword)((dword)(msb_ptr)[1] << 16) | (dword)((dword)(msb_ptr)[2] << 8) | (dword)(msb_ptr)[3])) 



/*
==============================================================================
 DEBUG SECTION
==============================================================================
*/

/*
------------------------------------------------------------------------------
 Debug Interface
------------------------------------------------------------------------------
*/

/* External function which is called for debug output
 */
extern void mns_trace(int service, int event, int pcnt, ...);


/* This macro defines the debug level. Define NST_L_NOTHING to disable trace outputs.
 * Define one of the following macros to enable debug outputs:
 * NST_L_BASIC, NST_L_COMM, NST_L_STANDARD, NST_L_ADVANCED (further values are possible)
 */
#define MNS_DEBUG           NST_L_NOTHING


/* Define this macro to map the debug interface onto a user defined function.
 */
#define MNS_TRACE(args)     mns_trace args



/*
--------------------------------------------------------------------------------
 MBM Usage API
--------------------------------------------------------------------------------
*/

/* Enable the MBM usage API to trace usage of  buffers (allocated TX & RX messages),
 * memory and fragmentation.
 */
/* #define MBM_USAGE_API */


/* The following macro defines the limit which is used to count "tiny" memory fragments.
 * Each memory fragment smaller than this size (in bytes) means "tiny". It is recommended
 * to define the MBM_USAGE_FRAG_* macros if further information about memory fragmentation 
 * is needed (refer to structure MbmFragmentation). The sizes shall be dividable by 
 * MBM_MEM_UNIT_NUM.
 */
/* #define MBM_USAGE_FRAG_SMALL     20 */


/* The following macro defines the limit which is used to count "little" memory fragments.
 * Each memory fragment bigger than "tiny" and smaller than this size (in bytes) means 
 * "little".
 */
/* #define MBM_USAGE_FRAG_MEDIUM    68 */


/* The following macro defines the limit which is used to count "medium" and "large" memory
 * fragments. Each memory fragment bigger than "little" and smaller than this size (in bytes)
 * means "medium". Each memory fragment bigger than "medium" is counted as "large".
 */
/* #define MBM_USAGE_FRAG_LARGE     500 */


/* Define this macro with the name of a callback function which is called every time 
 * memory is allocated or freed and a threshold is passed. Define only the name.
 * The callback should match this prototype: 
 * void my_usage_callback(MbmUsage *usage_ptr);
 */
/* #define MBM_USAGE_CALLBACK       MyUsageCallback */


/* By defining the following callback function the application can set a usage threshold during
 * the initialization of the MBM in a comfortable way. The application does not need to handle
 * certain events (e.g. resets) when the MBM is re-initialized and the threshold values are
 * reset to '0'. Define only the name. The callback should match this prototype:
 * void my_usage_init_callback(MbmUsage *usage_ptr);
 */
/* #define MBM_USAGE_INIT_CALLBACK  MyUsageInitCallback */



/*
--------------------------------------------------------------------------------
 MLog Memory Logger Configuration
--------------------------------------------------------------------------------
*/

/* Define the timestamp function to give each log entry one. Return at least 16-bit
 * timestamp.
 */
/* #define MLOG_TIMESTAMP()             MyGetTickCnt */


/* The following macro defines a Target specific memory copy. If it is not defined
 * it it is replaced by a copy loop.
 */
/* #define MLOG_MEMCPY(dest, src, n)    MyMemCpy(dest, src, n) */


/* The following macro defines a function that should be called for each logged line.
 */
/* #define MLOG_DUMP_LINE(u8_store, u16_timestamp, u8_id, u8_ptr_data, data_len)                   MyDumpLine(u8_store, u16_timestamp, u8_id, u8_ptr_data, data_len) */


/* A critical section is required if adding lines to a store can be done from
 * different contexts. Define the following functions to enter and leave the
 * crtical section.
 */
/* #define MLOG_ENTER_CRITICAL_SECTION()    MyMutexTake(MY_MTX_ID) */
/* #define MLOG_LEAVE_CRITICAL_SECTION()    MyMutexRelease(MY_MTX_ID) */


/* The following macro defines the number of separate stores.
 */
/* #define MLOG_NUM_STORES   2 */


/* The following macro defines the number of lines in each store.
 */
/* #define MLOG_NUM_LINES  200 */


/* The following macro defines the number of bytes per line (excluding timestamp).
 */
/* #define MLOG_LINE_SIZE   16 */


/* Define this macro to enable PMS to log messages to this store, it'll use 0 as ID 
 * for RX and 1 as ID for TX.
 */
/* #define PMS_MLOG_STORE    0 */


/* Define this macro to enable MBM to log usage to this store, it'll use 3 as ID.
 */
/* #define MBM_MLOG_STORE    1 */


#endif /* _ADJUST1_H */
