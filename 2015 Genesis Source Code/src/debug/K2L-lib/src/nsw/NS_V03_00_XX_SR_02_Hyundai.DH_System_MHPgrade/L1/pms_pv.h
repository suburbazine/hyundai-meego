/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Private declarations and definitions of the Port Message 
                Service (PMS)
File:           pms_pv.h
Version:        3.0.x-SR-2  
Language:       C
Author(s):      R.Lundstrom, T.Jahnke
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
  * \brief      Private declarations and definitions of the Port Message 
  *             Service (PMS)
  */

#ifndef _PMS_PV_H
#define _PMS_PV_H


/*
==============================================================================
    Includes
==============================================================================
*/

#include "mbm.h"
#include "pms.h"

/*
==============================================================================
    Rules
==============================================================================
*/
#ifdef PMS_MIN
    #ifndef PACKET_COMMUNICATION_ONLY

        #if PMS_RX_SEG_PREALLOC_SIZE > PMS_ALLOC_MAX_SIZE
           #error "PMS_RX_SEG_PREALLOC_SIZE must not be bigger than 64000."
        #endif

    #endif
#endif  /* #ifdef PMS_MIN */
/*
==============================================================================
    Module internal definitions
==============================================================================
*/


/*
--------------------------------------------------------------------------------
    Sizes (explicit casts)
--------------------------------------------------------------------------------
*/
#define PMS_CFG_RX_SEG_PREALLOC_SIZE    ((word)PMS_RX_SEG_PREALLOC_SIZE)
#define PMS_CFG_BUFSIZE_SMALL           ((word)PMS_BUFSIZE_SMALL)
#define PMS_CFG_BUFSIZE_DATA_LARGE      ((word)PMS_BUFSIZE_DATA_LARGE)
#define PMS_CFG_ADS_RX_PREALLOC         ((word)ADS_RX_PREALLOC)

#define PMS_MAX_CMS_PAYLOAD_MOST50      ((word)12)
#define PMS_MAX_CMS_PAYLOAD_MOST150     ((word)45)

/*
--------------------------------------------------------------------------------
    TelIDs
--------------------------------------------------------------------------------
*/
#define PMS_TELID_0                     ((byte)0x00)
#define PMS_TELID_1                     ((byte)0x01)
#define PMS_TELID_2                     ((byte)0x02)
#define PMS_TELID_3                     ((byte)0x03)
#define PMS_TELID_4                     ((byte)0x04)
#define PMS_TELID_80                    ((byte)0x80)
#define PMS_TELID_90                    ((byte)0x90)
#define PMS_TELID_UNKNOWN               ((byte)0xFF)

/*
--------------------------------------------------------------------------------
    Rx Segmentation
--------------------------------------------------------------------------------
*/
#define PMS_TELID4_MSGCNT_ALLOC_OK      ((byte)0xFF)
#define PMS_TELID4_MSGCNT_ALLOC_FAILED  ((byte)0xEE)

/*
----------------------------------------------------------------------------
    PMH ckecks
----------------------------------------------------------------------------
*/
/*
 * Marks a MOST Data Packet as invalid due to a failed plausibility check
 * of the PML against the data length
 */
#define PMS_INVALID_PM_LENGTH ((word)0xFFFF)

/*
--------------------------------------------------------------------------------
    PMS Protocol IDs
--------------------------------------------------------------------------------
*/
#define PMS_PID_OPTIMIZED      ((byte) 0x01)    /* optimized mode - PMS_RX_OPT3 */

/*
--------------------------------------------------------------------------------
    PmsSyncHandler bits
--------------------------------------------------------------------------------
*/

#define PMS_A_NONE             ((byte) 0x00)
#define PMS_A_RESET_CNT        ((byte) 0x01)
#define PMS_A_SYNCC            ((byte) 0x02)
#define PMS_A_GO_SYNCED        ((byte) 0x04)


/*
--------------------------------------------------------------------------------
    SCF bits
--------------------------------------------------------------------------------
*/
#define PMS_M_SCF0_TIMESTAMP   ((byte) 0x80)
#define PMS_M_SCF0_HANDLE      ((byte) 0x40)
#define PMS_M_SCF0_PRIO        ((byte) 0x20)
#define PMS_M_SCF0_RETRY       ((byte) 0x10)
#define PMS_M_SCF0_SCF1_EN     ((byte) 0x01)

#define PMS_M_SCF1_TGTDEVTYPE  ((byte) 0x80)
#define PMS_M_SCF1_SRCDEVID    ((byte) 0x40)
#define PMS_M_SCF1_TGTDEVID    ((byte) 0x20)
#define PMS_M_SCF1_FUNCADDR    ((byte) 0x10)

/*
--------------------------------------------------------------------------------
    Default and available fields
--------------------------------------------------------------------------------
*/

/* N.B. NEVER ENABLE BIT 0, THIS IS RESERVED FOR THE PROTOCOL, AND ENABLED */
/*      AUTOMATICALLY WHEN NECESSARY AND POSSIBLE, ENABLING THE BIT IN THESE */
/*      MASKS WOULD MEAN THAT THE USER COULD DECIDE IF SCF1 SHOULD BE USED */
/*      EVEN WHEN NOT NECESSARY. */

#define PMS_M_ICM_RX_SCF0_DEFAULT       ((byte) 0x00) /* -None- */
#define PMS_M_ICM_RX_SCF1_DEFAULT       ((byte) 0x00) /* -None- */

#define PMS_M_MCM_RX_SCF0_DEFAULT       ((byte) 0x00) /* -None- */
#define PMS_M_MCM_RX_SCF1_DEFAULT       (PMS_M_SCF1_TGTDEVTYPE | PMS_M_SCF1_SRCDEVID | PMS_M_SCF1_FUNCADDR)

#define PMS_M_MDP_RX_SCF0_DEFAULT       ((byte) 0x00) /* -None- */
#define PMS_M_MDP_RX_SCF1_DEFAULT       (PMS_M_SCF1_TGTDEVTYPE | PMS_M_SCF1_SRCDEVID)

#define PMS_PML_LEN         2
#define PMS_PMHL_LEN        1
#define PMS_FPH_LEN         1
#define PMS_SCF0_LEN        1
#define PMS_RETRIES_LEN     2
#define PMS_TGTDEVID_LEN    2
#define PMS_FBLOCKINST_LEN  2
#define PMS_FUNCOPTYPE_LEN  2
#define PMS_TELIDLEN_LEN    2
#define PMS_MSGSIZE_LEN     2
#define PMS_MSGCNT_LEN      1

#define PMS_PM_INT_BODY_LEN         ((byte)(PMS_FUNCOPTYPE_LEN + PMS_TELIDLEN_LEN))     /* 4 */
#define PMS_PM_PRE_FDH_LEN          ((byte)(PMS_PML_LEN + PMS_PMHL_LEN + PMS_FPH_LEN))  /* 4 */

/*
----------------------------------------------------------------------------
    Bits of the FIFO Command
----------------------------------------------------------------------------
*/
#define PMS_M_CMD_SYNCC            ((byte)0x80)
#define PMS_M_CMD_SLOTNTF          ((byte)0x40)
#define PMS_M_CMD_RETRY            ((byte)0x04)
#define PMS_M_CMD_CANCEL           ((byte)0x02)

/*
--------------------------------------------------------------------------------
    Bits of FIFO Status
--------------------------------------------------------------------------------
*/
#define PMS_M_STAT_SYNCS            PMS_M_CMD_SYNCC
#define PMS_M_STAT_SLOTAV           ((byte)0x40)
#define PMS_M_STAT_AUTOCAN          ((byte)0x02)
#define PMS_M_STAT_EXT              ((byte)0x01)

#define PMS_STATUS_NONE             ((byte)0x00)

/*
--------------------------------------------------------------------------------
    Sync Status definitions
--------------------------------------------------------------------------------
*/
#define PMS_SYNCS_LSB_POS                 2
#define PMS_M_SYNCS                       ((byte)((byte)0xF << PMS_SYNCS_LSB_POS))
#define PMS_SYNCS_POWER_UP                ((byte)((byte)0x0 << PMS_SYNCS_LSB_POS))
#define PMS_SYNCS_RESET_PIN               ((byte)((byte)0x1 << PMS_SYNCS_LSB_POS))
#define PMS_SYNCS_WATCHDOG                ((byte)((byte)0x2 << PMS_SYNCS_LSB_POS))
#define PMS_SYNCS_RESET_COMMAND           ((byte)((byte)0x3 << PMS_SYNCS_LSB_POS))
#define PMS_SYNCS_SYNC_CMD                ((byte)((byte)0x4 << PMS_SYNCS_LSB_POS))

/*
--------------------------------------------------------------------------------
    MsgStatus
--------------------------------------------------------------------------------
*/
#define PMS_XMIT_LSB_POS                  2

/*
-------------------------------------------
    A. General Masks
-------------------------------------------
*/
#define PMS_M_XMIT                        ((byte)((byte)0xF << PMS_XMIT_LSB_POS))    /* XMIT status mask */
#define PMS_M_XMIT_ERR                    ((byte)((byte)0xE << PMS_XMIT_LSB_POS))    /* Any of these bits indicates an error */
#define PMS_XMIT_INT                      ((byte)((byte)0x0 << PMS_XMIT_LSB_POS))    /* Internal Error          (Highest Bit cleared) */
#define PMS_XMIT_INT_MOST                 ((byte)((byte)0x4 << PMS_XMIT_LSB_POS))    /* Internal Most Error     (Highest Bit cleared, Bit 3 set) */
#define PMS_XMIT_EXT_MOST                 ((byte)((byte)0x8 << PMS_XMIT_LSB_POS))    /* External Most Error     (Highest Bit set) */

/*
-------------------------------------------
    B. Success
-------------------------------------------
*/
#define PMS_XMIT_SUCCESS                  ((byte)((byte)0x1 << PMS_XMIT_LSB_POS))    /* Success */

/*
-------------------------------------------
    C. Internal Errors
-------------------------------------------
*/
/* i. general */
#define PMS_XMIT_INT_FMAT_FAILURE         ((byte)((byte)0x2 << PMS_XMIT_LSB_POS))    /* Format Error of PMS message */

/* ii. MOST related */
#define PMS_XMIT_INT_MOST_NETWORK_OFF     ((byte)((byte)0x4 << PMS_XMIT_LSB_POS))    /* Most-Network is off, could not send */
#define PMS_XMIT_INT_MOST_TIMEOUT         ((byte)((byte)0x5 << PMS_XMIT_LSB_POS))    /* Timeout expiration */

/*
-------------------------------------------
    D. External Errors (Most Errors)
       Can be OR'ed in case of Group/Broadcast
-------------------------------------------
*/
#define PMS_XMIT_EXT_MOST_WRONG_TARGET    ((byte)((byte)0x8 << PMS_XMIT_LSB_POS))    /* No response from the target address */
#define PMS_XMIT_EXT_MOST_SUCCESS         ((byte)((byte)0x9 << PMS_XMIT_LSB_POS))    /* Most transmission successful */
#define PMS_XMIT_EXT_MOST_BAD_CRC         ((byte)((byte)0xC << PMS_XMIT_LSB_POS))    /* Most transmission failed because receiving node got bad CRC */
#define PMS_XMIT_EXT_MOST_RECV_BUF_FULL   ((byte)((byte)0xE << PMS_XMIT_LSB_POS))    /* Most transmission failed because the receiving node's buffer is full */

/*
--------------------------------------------------------------------------------
    Extended FIFO Status
--------------------------------------------------------------------------------
*/
#define PMS_EXT_MSG_ERR_LSB     4 /* Error ID LSBit */
#define PMS_EXT_MSG_ERR_MASK    ((byte)((byte)0xF << PMS_EXT_MSG_ERR_LSB))

#define PMS_EXT_MSG_ERR_1       ((byte)((byte)0x1 << PMS_EXT_MSG_ERR_LSB))
#define PMS_EXT_MSG_ERR_2       ((byte)((byte)0x2 << PMS_EXT_MSG_ERR_LSB))
#define PMS_EXT_MSG_ERR_3       ((byte)((byte)0x3 << PMS_EXT_MSG_ERR_LSB))
#define PMS_EXT_MSG_ERR_4       ((byte)((byte)0x4 << PMS_EXT_MSG_ERR_LSB))
#define PMS_EXT_MSG_ERR_5       ((byte)((byte)0x5 << PMS_EXT_MSG_ERR_LSB))
#define PMS_EXT_MSG_ERR_6       ((byte)((byte)0x6 << PMS_EXT_MSG_ERR_LSB))
#define PMS_EXT_MSG_ERR_7       ((byte)((byte)0x7 << PMS_EXT_MSG_ERR_LSB))

#define PMS_EXT_CLASS_LSB       1
#define PMS_EXT_CLASS_MASK      ((byte)((byte)0x3 << PMS_EXT_CLASS_LSB))
#define PMS_EXT_CLASS_SEG_ERR   ((byte)((byte)0x0 << PMS_EXT_CLASS_LSB))
#define PMS_EXT_CLASS_TOO_LARGE ((byte)((byte)0x1 << PMS_EXT_CLASS_LSB))
#define PMS_EXT_CLASS_WRONG_TGT ((byte)((byte)0x2 << PMS_EXT_CLASS_LSB))

/*
----------------------------------------------------------------------------
    General
----------------------------------------------------------------------------
*/
/*
 * Send FIFOStatus error to INIC on segmentation errors, unless we handle
 * segmentation completely
 */
#define STATUS_TO_INIC  ((byte)0x80)

/*
 * Keep failed segment it will be reused for error message
 *(EHC only segmentation)
 */
#define KEEP_MESSAGE    ((byte)0x40)

/*
 * Do not unlink the message from any list (pending segment),
 * implies KEEP_MESSAGE
 *(EHC only segmentation)
 */
#define NO_UNLINK       ((byte)0x20)


/*
----------------------------------------------------------------------------
    FIFO specific constant configurations
----------------------------------------------------------------------------
*/

/*!
* Indexes to preallocated buffers in the preallocated array.
* These HAS to be in the same order as the fifoPtrPrioTab, starting from 1.
*/

#define PMS_IDX_RSVD_RX_PREALLOC    ((word)0)
#define PMS_IDX_ICM_RX_PREALLOC     ((word)1)
#define PMS_IDX_MCM_RX_PREALLOC     ((word)2)
#define PMS_IDX_MDP_RX_PREALLOC     ((word)3)

/*
==============================================================================
    Module internal macros
==============================================================================
*/

/*
--------------------------------------------------------------------------------
 Macro:         PMS_SKIP_HEADER(...)

 Description:   Skips the header of a message and sets the supplied pointer
                immediately behind the filler bytes.

 Input(s):      ptr : Pointer to modify
                h   : Bufferhandle

 Return(s):     None, but modifies ptr
--------------------------------------------------------------------------------
*/
/* Set ptr to PMHL, add PMHL + 1 since size of PMHL is not included in lenght */
#define PMS_SKIP_HEADER(ptr, h) (ptr)  = &(h)->hdr_ptr[2]; (ptr) += *(ptr) + 1

/*
--------------------------------------------------------------------------------
 Macro:         PMS_EXTERNAL_EVENT(...)

 Description:   Calls the External Event callback function if there is a
                function and a non-zero event.

 Input(s):      e : Event(s) to notify

 Return(s):     -
--------------------------------------------------------------------------------
*/
#define PMS_EXTERNAL_EVENT(e) /*lint -esym(960,14.9)*/ if( pmsInitData_ptr->cbExtEvents_fptr ) pmsInitData_ptr->cbExtEvents_fptr(e) /*lint +esym(960,14.9)*/

/*
--------------------------------------------------------------------------------
 Macro:         PMS_IFACE_READY(...)

 Description:   Checks if an interface is ready to transmit, i.e. is not busy
                transmitting. Shall be performed to check if the interface
                might accept a new transmission.

 Input(s):      iface : interface to poll.

 Return(s):     Non-zero if ready, else 0
--------------------------------------------------------------------------------
*/
#define PMS_IFACE_READY(iface)  (NULL == (iface).active)


/*
--------------------------------------------------------------------------------
 Macro:         PMS_IFACE_INIT(...)

 Description:   Removes any stored handles

 Input(s):      iface : interface

 Return(s):     -
--------------------------------------------------------------------------------
*/
#define PMS_IFACE_INIT(iface)   ((iface).active = NULL)


#ifdef PACKET_COMMUNICATION_ONLY

/*
==============================================================================
    Module internal structures (Packet communication only)
==============================================================================
*/
typedef struct PmsIface
{
    byte (*tx_fptr)(HMBMBUF, byte *, word); /* Function for transmitting      */
    HMBMBUF active;                         /* Buffer being transmitted       */
} PmsIface;

/*
*/
typedef struct PmsFifo
{
    MbmQueue msgQ;              /* Queue for TX messages                      */
    #ifdef PMS_70
    MbmQueue cmdQ;              /* Queue for TX commands                      */
    #endif
    HMBMBUF  txPending;         /* Buffer waiting for Status (only msg)       */
} PmsFifo;

typedef struct PmsStruct
{
    bool ifacesStarted;
    bool fire_buf_freed;
} PmsStruct;

/*!
  * typedef a function pointer for RX handlers to be nice with our readers,
  * otherwise PmsDecompressHeader() will be obscure
  */
typedef void (*PmsRxHandler)(HMBMBUF handle);

/*
==============================================================================
    Module internal function prototypes (Packet communication only)
==============================================================================
*/
#ifdef PMS_9
    static void PmsFifoInit(void);
#endif

#ifdef PMS_11
    static void PmsFillDefaultHeader(HMBMBUF handle);
#endif

#ifdef PMS_12
    static bool PmsCompressHeader(HMBMBUF handle);
#endif

#ifdef PMS_13
    static void PmsDecompressHeader(HMBMBUF handle);
#endif

#ifdef PMS_17
    static void PmsFifoTxTrigger(void);
#endif

#ifdef PMS_18
    static void PmsProcessRx(void);
#endif

#ifdef PMS_19
    static void PmsFifoProcessStatus(PmsFifo *fifo_ptr, byte status, HMBMBUF *h_ptr);
#endif

#ifdef PMS_70
    /*lint -sem(PmsSendCommand,1p)*/
    static void PmsSendCommand(HMBMBUF handle, byte cmd);
#endif

#ifdef PMS_63
    static _INLINE void PmsFireBufFreed(void);
#endif




#else   /* ifdef PACKET_COMMUNICATION_ONLY */

/*
==============================================================================
    Module internal macros
==============================================================================
*/

/*
--------------------------------------------------------------------------------
 Macro:         PMS_RX_IS_STATUS(...)

 Description:   Checks if a received buffer is a status message

 Input(s):      h : Handle to the buffer containing the message

 Return(s):     1/0
--------------------------------------------------------------------------------
*/
#define PMS_RX_IS_STATUS(h)             (FPH_MSGTYPE_STAT == (MBM_GET_HDR_PTR(h)[FPH_INDEX] & FPH_M_MSGTYPE))

#define PMS_TX_IS_CMD_OR_STATUS(h)      (MBM_GET_TYPE(h) & MBM_TYPE_FIFO_BIT)

#define IS_FAILED(s)                    ((PMS_M_STAT_SYNCS | PMS_M_XMIT_ERR) & (s))
#define IS_NOT_FAILED(s)                (!IS_FAILED(s))

#define IS_COMPLETE_STATUS(s)           ((PMS_M_STAT_SYNCS | PMS_M_XMIT) & (s))

/* NOT(AutoCancelled OR SyncS) AND Error => Still in INIC */
#define MESSAGE_IS_ENQUEUED_IN_INIC(s)  (!((PMS_M_STAT_AUTOCAN | PMS_M_STAT_SYNCS) & (s)) && (PMS_M_XMIT_ERR & (s)))

#define HAS_TX(f_ptr)    (MBM_QUEUE_LENGTH(&(f_ptr)->cmdQ) || MBM_QUEUE_LENGTH(&(f_ptr)->msgQ))

#define _PMS_ALLOC_SHADOW()             _MbmDequeue(&txSegShadowQ)
#define _PMS_FREE_SHADOW(s)             _PmsFreeShadow(s)
#define PMS_CHECK_SHADOW()              MBM_QUEUE_LENGTH(&txSegShadowQ)

#define HAS_SAME_SEG_ID(lh, rh)         (    (lh.Tgt_Adr   == rh.Tgt_Adr)                                              && (lh.FBlock_ID == rh.FBlock_ID)                                            && (lh.Inst_ID   == rh.Inst_ID)                                              && (lh.Func_ID   == rh.Func_ID)                                              && (lh.Operation == rh.Operation) )

/*
--------------------------------------------------------------------------------
    Check if the handle is a shadow of an unfailed segmented message
--------------------------------------------------------------------------------
*/
#define IS_LAST_SEGMENT(h)      ((h)->type & MBM_STAT_TX_LAST_SEG)


#define HAS_RX_USER_PAYLOAD(h)  (!(h)->start_ptr && (h)->size)

/*
==============================================================================
    Module internal structures
==============================================================================
*/
typedef struct PmsFifoDefCfg
{
    byte rxScf0Default;      /* Default fields on received messages without SCF0 */
    byte rxScf1Default;      /* Default fields on received messages without SCF1 */
} PmsFifoDefCfg;


typedef struct PmsIface
{
    byte (*tx_fptr)(HMBMBUF handle, byte *add_ptr, word add_size);  /* Function for transmitting */
    HMBMBUF active;                                                 /* Buffer being transmitted  */
} PmsIface;

/*
*/
typedef struct PmsFifo
{
    MbmQueue msgQ;              /* Queue for TX messages                        */
    MbmQueue cmdQ;              /* Queue of commands                            */
    MbmQueue pendingQ;          /* Pending messages (passed to LLD/inside INIC) */
    PmsIface *iface_ptr;        /* Current TX interface                         */
    TMostTimer timer;           /* Timer for status on messages                 */
    word     timeout;           /* Timeout on messages                          */
    #ifdef PMS_RX_SEND_ACK
    MbmBuf   ack;                   /* Buffer dedicated to acknowledge           */
    byte     ackData[PMS_CMD_SIZE]; /* Buffer for compressed acknowledge message */
    byte     rxStatus;              /* 0x00 = None                               */
    #endif
    byte     txSlotsAvailable;  /* Free slots in the INIC                       */
    byte     syncGuard;         /* Count of consecutive syncs                   */
} PmsFifo;

typedef enum
{
    PMS_S_INIT = 0,   /* Just Initialized */
    PMS_S_PENDING,    /* Expecting SyncS after SyncC */
    PMS_S_SYNCED,     /* Synchronized (normal case after initial SyncS */
    PMS_S_FAILED      /* Could not synchronize */
} PmsSyncState;

typedef enum 
{
    PMS_SHE_TIMEOUT,             /*! a sync status wasn't received within the expected time */
    PMS_SHE_SEND_SYNCC,          /*! forces a sync command to the INIC */
    PMS_SHE_RCV_CHIP_STARTUP,    /*! received sync status "ChipStartup" */
    PMS_SHE_RCV_SYNCS,           /*! received sync status "SyncC.Received" */
    PMS_SHE_RCV_SYNC_FORMAT_ERR  /*! wrong format of received sync status */
} PmsSyncHandlerEvent;           /*! events distributed to sync handler */

typedef struct PmsMcmHeaderCfg
{
    byte rsvd_header_sz;   /*! The used size of the reserved header */
    byte pmhl;             /*! The port message header length */
    byte stuffing_bytes;   /*! Number of stuffing bytes needed */
    byte shift;            /*! Number of bytes belonging to the payload */
} TPmsMcmHeaderCfg;

#ifdef PMS_71
typedef struct 
{
    MbmBuf mbm_buf;              /*! message buffer struct for internal messages */
    HMBMBUF handle;              /*! points to the respective message */
    byte payload[PMS_CMD_SIZE];  /*! the reserved payload */
} TPmsRsvdBuf;                   /*! specific message struct that can be used for internal messages */
#endif

typedef struct PmsStruct
{
    /* ANSI-C allows only signed or unsigned int for bit fields */
    bool initSync;
    bool ifacesStarted;
#ifndef PACKET_COMMUNICATION_ONLY
    #ifndef PMS_TX_NOSEG
    bool needShadow;
    #endif

    #ifdef MSG_RX_AUTOMATIC_ERR_MSG
    bool needSegErrMsg;
    #endif

    #ifdef PMS_RX_SEND_ACK
    bool needRetriggerAck;
    #endif
    bool sync_rc;
    bool needPrealloc;
    bool rxPreAllocHasTimedOut;
    bool rxPreAllocFailedTimerStarted;
    TMostTimer rxPreAllocFailedTimer;

    MbmBuf *rxPreAllocPtrTab[PMS_RX_PREALLOC];

    byte mid_level_retries;
    byte low_level_retries;

    bool fire_buf_freed;

    #ifdef MSG_RX_USER_PAYLOAD
    MbmQueue rxPayloadWaitingQ;         /* Waiting for payload buffer from the user */
    #endif
#endif
} PmsStruct;


/*!
 * typedef a function pointer for RX handlers to be nice with our readers,
 * otherwise PmsDecompressHeader() will be obscure
 */
typedef void (*PmsRxHandler)(HMBMBUF handle);

/*
==============================================================================
    Module internal function prototypes
==============================================================================
*/
#ifdef PMS_9
    /*lint -sem(PmsFifoInit,1p)*/
    static void PmsFifoInit(PmsFifo *fifo_ptr);
#endif

#ifdef PMS_11
    /*lint -sem(PmsFillDefaultHeader,1p)*/
    static void PmsFillDefaultHeader(HMBMBUF handle);
#endif

#ifdef PMS_12
    /*lint -sem(PmsCompressHeader,1p)*/
    static bool PmsCompressHeader(HMBMBUF handle);
#endif

#ifdef PMS_64
    /*lint -sem(PmsCompressHeader,1p)*/
    static bool PmsCompressHeaderICM(HMBMBUF handle);
#endif

#ifdef PMS_65
    /*lint -sem(PmsCompressHeader,1p)*/
    static bool _PmsCompressHeaderMCM(HMBMBUF handle, bool telId4Enabled);
#endif

#ifdef PMS_66
    /*lint -sem(PmsCompressHeader,1p)*/
    static bool PmsCompressHeaderMDP(HMBMBUF handle);
#endif

#ifdef PMS_13
    /*lint -sem(PmsDecompressHeader,1p)*/
    static void PmsDecompressHeader(HMBMBUF handle, const PmsFifoDefCfg *def_ptr);
#endif

#ifdef PMS_15
    /*lint -sem(PmsGetTxFifo,1p)*/
    static PmsFifo *PmsGetTxFifo(HMBMBUF tx_handle);
#endif

#ifdef PMS_53
    /*lint -sem(PmsGetRxFifo,1p,r_null)*/
    static PmsFifo *PmsGetRxFifo(HCMBMBUF rx_handle);
#endif

#ifdef PMS_17
    /*lint -sem(PmsFifoTxTrigger,1p,2p)*/
    static void PmsFifoTxTrigger(PmsFifo *fifo_ptr, MbmQueue *q_ptr);
#endif

#ifdef PMS_18
    static void PmsProcessRxMsgQueue(void);
#endif

#ifdef PMS_19
    /*lint -sem(PmsFifoProcessStatus,1p)*/
    static void PmsFifoProcessStatus(PmsFifo *fifo_ptr, byte status, HMBMBUF *handle);
#endif

#ifdef PMS_20
    /*lint -sem(PmsSendCommand,1p,2p)*/
    static void PmsSendCommand(PmsFifo *fifo_ptr, HMBMBUF handle);
#endif

#ifdef PMS_21
    static void PmsSyncIn(const byte *status_ptr, byte size);
#endif

#ifdef PMS_23
    static void PmsSyncHandler(PmsSyncHandlerEvent syncEvent);
#endif
#ifdef PMS_24
    /*lint -sem(PmsRxSegProcess,1p,r_null)*/
    static HMBMBUF PmsRxSegProcess(HMBMBUF hNew);
#endif

#ifdef PMS_25
    /*lint -sem(PmsRxSegFindMatch,1p,r_null)*/
    static HMBMBUF PmsRxSegFindMatch(const MbmCtrlRx *new_ptr, bool direction);
#endif

#ifdef PMS_28
    static void PmsRxSegGarbageCollector(bool no_mark);
#endif

#ifdef PMS_29
    /*lint -sem(PmsRxSegError,1p)*/
    static void PmsRxSegError(HMBMBUF hErr, byte error);
#endif

#ifdef PMS_33
    /*lint -sem(PmsRxSegMatch,1p,2p)*/
    static bool PmsRxSegMatch(const MbmCtrlRx *left_ptr, const MbmCtrlRx *right_ptr);
#endif

#ifdef PMS_34
    /*lint -sem(PmsRxSegAppend,1p,2p)*/
    static bool PmsRxSegAppend(HMBMBUF hAssembled, HMBMBUF hSegment);
#endif

#ifdef PMS_35
    /*lint -sem(PmsSegErrorTxStatus,1p)*/
    static byte PmsSegErrorTxStatus(HMBMBUF handle, byte status);
#endif
#ifdef PMS_36
    static _INLINE void _PmsSetReferencesToNull(const MbmBuf *org_ptr);
#endif
#ifdef PMS_39
    /*lint -sem(PmsProcessCompressedRxMsg,1p)*/
    static void PmsProcessCompressedRxMsg(HMBMBUF handle);
#endif

#ifdef PMS_40
    /*lint -sem(PmsProcessRxMsgOut,2p)*/
    static bool PmsProcessRxMsgOut(HMBMBUF handle, const PmsFifo *fifo_ptr);
#endif

#ifdef PMS_30
    static void PmsProcessRxStatusQueue(void);
#endif

#ifdef PMS_32
    static void PmsRxHandleAck(void);
#endif

#ifdef PMS_51
    /*lint -sem(PmsFreeUserTxPayload,1p,2p)*/
    static void PmsFreeUserTxPayload(byte *data_ptr, HMBMBUF handle);
#endif

#ifdef PMS_52
    static bool PmsDiscardPendingRx(void);
#endif

#ifdef PMS_61
    static void PmsTxSyncSegmented(void);
#endif

#ifdef PMS_63
    static _INLINE void PmsFireBufFreed(void);
#endif



#endif /* #ifdef PACKET_COMMUNICATION_ONLY */


#endif /* #ifndef _PMS_PV_H */

/*
==============================================================================
    End Of File
==============================================================================
*/
