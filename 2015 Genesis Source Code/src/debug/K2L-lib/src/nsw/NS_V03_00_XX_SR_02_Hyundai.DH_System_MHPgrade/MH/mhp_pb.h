/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Public API of the MOST High Protocol Service (MHP)
File:           mhp_pb.h
Version:        3.0.x-SR-2  
Language:       C
Author(s):      R.Hanke
Date:           31.January.2012

FileGroup:      Layer II Extension: MOST High Protocol Service
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
  * \brief      Public API of the MOST High Protocol Service (MHP)
  */

#ifndef _MHP_PB_H
#define _MHP_PB_H


#include "mhp_ad.h"
#ifdef PMHT_MIN
  #include "pmht_ad.h"
#endif

/*
==============================================================================
    Module Global Definitions
==============================================================================
*/

/*--------------------------------------- */
/* Return values of function MhpOpenCon() */
/*--------------------------------------- */
#define MHP_OC_SUCCESS              ((byte)0x00)    /* Open connection triggered successfully */
#define MHP_OC_AH_BUSY              ((byte)0x01)    /* Connection not opend, Address Handler is busy*/
#define MHP_OC_NCS_NOT_OK           ((byte)0x02)    /* Connection is not opened, NWM.ConfigurationState is NotOK */
#define MHP_OC_ALREADY_OPEN         ((byte)0x03)    /* Connection already open */


/*-------------------------------------*/
/* Return values of function MhpSend() */
/*-------------------------------------*/
#define MHP_SEND_SUCCESS            ((byte)0x00)    /* Sending of Data triggered successfully */
#define MHP_SEND_AH_BUSY            ((byte)0x01)    /* Data is not send, Address Handler is busy   */
#define MHP_SEND_NCS_NOT_OK         ((byte)0x02)    /* Data is not send, NWM.ConfigurationState is NotOK */
#define MHP_SEND_TX_LEN_ZERO        ((byte)0x03)    /* Data is not send, Length is zero*/
#define MHP_SEND_INV_TOTAL_PL       ((byte)0x04)    /* Data is not send, Total Packet Length is to big */
#define MHP_SEND_TX_NO_DATA         ((byte)0x05)    /* Data is not send, data pointer is NULL */

/*-----------------------------------------*/
/* Return values of function MhpTxStatus() */
/*-----------------------------------------*/
#define MHP_STATUS_NO_DATA          ((byte)0x00)    /* No more data at the moment, hold connection */
#define MHP_STATUS_SEND_NEXT        ((byte)0x01)    /* Data is ready to to be transmitted */
#define MHP_STATUS_CLOSE_CON        ((byte)0x02)    /* Close connection */

/*-------------------------------------------- */
/* Default values for parameter TMhpTx.Options */
/*-------------------------------------------- */
#define MHP_HOLD_TO                 ((byte)0x00)            /* Connection is closed after timeout of t_Delay_End  (Default) */
#define MHP_HOLD_INF                ((byte)0x01)            /* Connection is not closed automatically by MHP Module */


/*------------------------------ */
/* Flags of MnsPendingHigh       */
/*------------------------------ */
#define MNSH_P_TX_STATE             ((word)0x0001)          /* MHP Tx Section: State changed */
#define MNSH_P_TX_TIMEOUT           ((word)0x0002)          /* MHP Tx Section: Timeout event */
#define MNSH_P_RX_STATE             ((word)0x0004)          /* MHP Rx Section: State changed */
#define MNSH_P_RX_TIMEOUT           ((word)0x0008)          /* MHP Rx Section: Timeout event */
#define MNSH_P_TX_NOBUF             ((word)0x0010)          /* MHP Tx Section: No telegram buffer available */
#define MNSH_P_RX_NOBUF             ((word)0x0020)          /* MHP Rx Section: No telegram buffer available */
#define MNSH_P_MASK                 ((word)0x003F)          /* mask all used flags */


/*------------------------------- */
/* Events for MostServiceHigh()   */
/*------------------------------- */
#define MNSH_E_TIMER_TX             (MNSH_P_TX_TIMEOUT)     /* event forced by MnsRequestHighTimerTx() */
#define MNSH_E_TIMER_RX             (MNSH_P_RX_TIMEOUT)     /* event forced by MnsRequestHighTimerRx() */
#define MNSH_E_TX_EVENT             (MNSH_P_TX_STATE)       /* event forced by alternative packet service, when transmission has been completed */
#define MNSH_E_MASK                 ((MNSH_E_TIMER_TX)|(MNSH_E_TIMER_RX)|(MNSH_E_TX_EVENT))
                                                            /* Mask for events, which must be captured by "MnsPendingHigh"               */

/*--------------------------- */
/* Parameter of MhpTxStatus() */
/*--------------------------- */
#define MHP_TX_SUCCESS              ((byte)0x00)            /* Segment successfully transmitted */
#define MHP_TX_ERROR_NAC            ((byte)0x01)            /* Transmission and connection canceled, since any frames were not acknowledged */
#define MHP_TX_ERROR_KILLED         ((byte)0x02)            /* Transmission and connection canceled, since connection was killed by receiver */
#define MHP_TX_ERROR_BUILDCON       ((byte)0x03)            /* Connection failed, since no "Start Connection" received */
#define MHP_TX_ERROR_PRIO           ((byte)0x04)            /* Connection failed, since Priority rejected */
#define MHP_TX_ERROR_SCALE          ((byte)0x05)            /* Connection failed, since "Start Connection" was received with operand "scale" = 0 */
#define MHP_TX_ERROR_NOBUF          ((byte)0x06)            /* Connection failed, internal error, no telegram buffer available */

#define MHP_TX_KILLED_TX            ((byte)0x08)            /* Transmission failed, since connection aborted by application of sender */
#define MHP_TX_ERROR_NDF            ((byte)0x09)            /* Connection failed, since "Start Connection" was received with operand "NDF" = 0 */
#define MHP_TX_LEN_ZERO             ((byte)0x21)            /* Connection is cancelled, since packet length is zero */
#define MHP_TX_END_READY            ((byte)0x22)            /* Entry from connection list is removed, due all "EndConnection" telegrams sent */
#define MHP_TX_KILLED_RX            ((byte)0x23)            /* Entry from connection list is removed, due to "EndConnection" telegram by receiver, */
                                                            /* but all pending data packets have been already transmitted */
#define MHP_TX_CONFIG_NOTOK         ((byte)0x25)            /* NWM.ConfigurationState is NotOK */
#define MHP_TX_LEN_TOO_BIG          ((byte)0x26)            /* Total size of packet exceeds upper limit */
#define MHP_TX_CLOSE                ((byte)0x27)            /* Transmission and connection is ordinary finished by transmitter */


/*----------------------------- */
/* Parameter of MhpRxComplete() */
/*----------------------------- */
#define MHP_RX_SUCCESS              ((byte)0x00)            /* Block was received successfully */
#define MHP_RX_CLOSE                ((byte)0x10)            /* Transmission and connection is ordinary finished by transmitter or by receiver */
#define MHP_RX_ERROR_NFRAME         ((byte)0x01)            /* Block transmission failed, since Null-Frame not received     --> Connection canceled */
#define MHP_RX_ERROR_FRAME          ((byte)0x02)            /* Block transmission failed, since Data-Frame(s) missed        --> Connection canceled */
#define MHP_RX_ERROR_NOREADY        ((byte)0x03)            /* Transmission failed, since no "Ready for Data" received      --> Connection canceled */
#define MHP_RX_ERROR_KILLED         ((byte)0x04)            /* Transmission failed, since connection killed by transmitter  --> Connection canceled */
#define MHP_RX_ERROR_NOBUF          ((byte)0x05)            /* Connection failed, internal error, no telegram buffer available --> Connection canceled */
#define MHP_RX_KILLED_RX            ((byte)0x06)            /* Receiption is canceled by application or service, since no buffer established */
                                                            /* after timeout */

/*-------------------------- */
/* Parameter when requesting */                             /* Callback function is called via the tables from Command Interpreter */
/* a receive buffer          */
/*-------------------------- */
#define MHP_GETRXPTR_REQUEST        ((byte)0x01)            /* New conection is requested (first block) */
#define MHP_GETRXPTR_NEXT           ((byte)0x02)            /* Buffer is required for next block */


/*---------------------------------- */
/* Hold Events of MOST High Protocol */
/*---------------------------------- */
#define MHP_HOLD_EVENT_TX           ((byte)0x80)            /* Conncetion is forced to be in hold state by application (sender) */
#define MHP_HOLD_EVENT_BUSY_RX      ((byte)0x81)            /* Receiver busy since processing another connection */
#define MHP_HOLD_EVENT_BUF          ((byte)0x82)            /* Buffer of application has not been established */
#define MHP_HOLD_EVENT_ENDDELAY     ((byte)0x83)            /* Hold Connection after segment was successfully sent */
#define MHP_HOLD_EVENT_BUSY_TX      ((byte)0x84)            /* Transmitter busy since processing another connection */

#define MHP_IMPLICIT_HOLD_FLAG      ((byte)0x01)            /* allows to issue an implicit HoldRx when a block is acknowledged */


/*---------------------------- */
/* Interface to Receive Buffer */
/*---------------------------- */
#define MHP_FLAGS_DATA              ((byte)0x01)            /* BufPtr points at a data buffer */
#define MHP_FLAGS_FUNC              ((byte)0x02)            /* BufPtr points at a service function, which supervises the data buffer */


/*--------------------------- */
/* Debug Events of Tx Section */
/*--------------------------- */
#define MHP_DBG_STATE_TX            ((byte)0x01)            /* Change of current state */
#define MHP_DBG_NEGACK_TX           ((byte)0x02)            /* Negative Acknowledge received  */
#define MHP_DBG_FINACK_TX           ((byte)0x03)            /* Acknowledge of last frame received */
#define MHP_DBG_NOTACK_TX           ((byte)0x04)            /* Not all frames were acknowledged */
#define MHP_DBG_ACKNOTACTIVE_TX     ((byte)0x05)            /* Acknowledge has been received, but not from expected function */
#define MHP_DBG_GETTXPTR_TX         ((byte)0x06)            /* Trace within function MhpGetTxPtrCon() */
#define MHP_DBG_STCON_TX            ((byte)0x07)            /* Start Connection is ignored or rejected by Sender */
#define MHP_DBG_CON_HOLD            ((byte)0x08)            /* Connection is hold by the DSO, while the MHP TX section is servicing another connection */


/*--------------------------- */
/* Debug Events of Rx Section */
/*--------------------------- */
#define MHP_DBG_STATE_RX            ((byte)0x01)            /* Change of current state */
#define MHP_DBG_NFRAME_RX           ((byte)0x02)            /* Null Frame received */
#define MHP_DBG_NEGACK_RX           ((byte)0x03)            /* Negative Acknowledge transmitted */
#define MHP_DBG_ACKNOBUF_RX         ((byte)0x04)            /* No buffer for frame acknowledge available */
#define MHP_DBG_DATAFR_RX           ((byte)0x05)            /* Data frame received without Null-Frame */
#define MHP_DBG_REQCON_RX           ((byte)0x06)            /* Request Connection is ignored or rejected by Receiver */

#define MHP_SIZE_DEBUG_INFO         3                       /* maximum number of debug information (size of info buffer) */


/*-------------------------------- */
/* Payload size of data link layer */
/*-------------------------------- */

#define MHP_PAYLOAD_ADS             ((word)MHP_MAX_PACKET_SIZE)
#define MHP_PAYLOAD_PCKT            MHP_PAYLOAD_ADS         /* ADS interface */


/*---------------------- */
/* MHP protocol overhead */
/*---------------------- */
#define MHP_OVERHEAD_PCKT           ((word)6)               /* MHP protocol overhead on asynchronous channel */
#define MHP_DF_OH_PCKT              ((word)2)               /* MHP data frame overhead on async channel (FrAck Field) */
#define MHP_ASYNC_DATA_OFFSET       (MHP_OVERHEAD_PCKT + MHP_DF_OH_PCKT)
                                    /* Difference between Raw Data and User Data on asynchronous channel */

#define MHP_ASYNC_DATA_PAYLOAD      ((MHP_PAYLOAD_PCKT) - (MHP_ASYNC_DATA_OFFSET))
                                    /* application payload =  MHP_PAYLOAD_PCKT -  MHP_OVERHEAD_PCKT - MHP_DF_OH_PCKT */
                                    /* e.g. 40 = 48 - 6 - 2 (when using 48 Byte data link layer pn asynchr. channel) */


/*------------------*/
/* Max. Packet size */
/*------------------*/
#define MHP_MAX_TOTAL_PACKET_LENGTH ((dword)0xFFFF0000)


/*---------------------------- */
/* Size of TX frame buffer     */
/* (payload of Mhp_TxTel_Type) */
/*---------------------------- */
#define MHP_NUM_DATA_TX         ((word)42)                              /* worst case for Multi-Frame-Request */


/*---------------------------- */
/* Size of RX frame buffer     */
/* (payload of Mhp_RxTel_Type) */
/*---------------------------- */
#ifdef MHP_RX_MIN                                                           /* Receiver Section available ? */
    #define MHP_NUM_DATA_RX         (MHP_PAYLOAD_PCKT - MHP_OVERHEAD_PCKT)  /* Async Interface available */
#else                                                                       /* Only Transmitter Section */
    #define MHP_NUM_DATA_RX         ((word)10)                              /* (Only Flow Control Protocols must be received) */
#endif


/*------------------------------------ */
/*  Buffer sizes                       */
/*------------------------------------ */
#define MHP_MAX_NUM_MULTI_REQ_PCKT  ((byte)41)                              /* maxumum number of frames to request in one */
                                                                            /* "Multi Frame Request" Command (Pckt channel) */


/*
==============================================================================
    Module Global Structures
==============================================================================
*/

#ifndef _MOSTDEF2_H
typedef TMostTimer TMostTimerHL;
#endif


#if (defined MHP_TX_MIN) || (defined PMHT_MIN) || (defined MHP_RX_MIN)
typedef struct Mhp_Con_Type
{
    word Addr;
    byte FBlock_ID;
    byte Inst_ID;
    word Func_ID;
    byte Operation;
} TMhpCon, *pTMhpCon;

typedef const TMhpCon* pcTMhpCon;   /* Pointer to const object */

#endif

#if (defined MHP_TX_MIN) || (defined PMHT_MIN)
struct Mhp_Tx_Type;                         /* forward declaration, necessary for Mhp_Tx_TgtDev_Type */
#endif


#ifdef PMHT_MIN
typedef struct Mhp_Tx_TgtDev_Type
{
    word                DevID;              /* Target Address of connection */
    word                UsageCount;         /* number of connections to target device */
    bool                TxBusy;             /* common busy flag */
    struct Mhp_Tx_Type  *ActiveConPtr;      /* buffer which currently transmits data to DevID */
    struct Mhp_Tx_Type  *RfdWaitingPtr;     /* buffer which waits for transmitting READY_FOR_DATA */
    bool                RfdPending;
    #ifdef PMHT_ADD1
    TMostTimer          timer;
    #endif
} TMhpTgtDev, *pTMhpTgtDev;
#endif

#if (defined MHP_TX_MIN) || (defined PMHT_MIN)
typedef struct Mhp_Tx_Type
{
    /* private fields: */
    byte Status;                    /* Buffer Status */
    byte State;                     /* State Machine */
    byte StateAReq;                 /* State after serving request */
    byte StateBack;                 /* Backup State when returning from Hold Loop */
    byte Retry;                     /* Retry Counter */

    TMostTimerHL TimerTx1;          /* Timeout */

    word TimerTx1Back;              /* Backup of Timer since overwritten while Hold Loop */
                                    /* stores just the timer value, not the whole structure */

    #ifdef PMHT_MIN
    TMostTimerHL TxTimerG1;
    word         TxFrId[16];        /* Tx Frame Id Bitfield */
    word         TxFrAck[16];       /* TxAcknowledge Bitfield */
    pTMhpTgtDev  TgtDevPtr;
    bool         BlockToken;        /* buffer active ? */
    #endif

    byte HoldEvent;
    byte EndEvent;
    word HoldLoopTime;              /* Timeout for Hold Loop */
    word BufRetry;                  /* Retry if no buffer available */
    word BlockCnt;                  /* Current Block Counter */
    word Flags;
    word TxDelayMax;                /* Current Delay value (read before each data frame transmission)  */

    byte Scale;                     /* max. Frames/Block */
    byte FrameCurrent;              /* current FrameID within current Block */
    byte FrameLast;                 /* last FrameID within current Block */
    word FrameRemain;               /* Remaining frames in following blocks (current segment) */
    word NumDataFrame;              /* Number of user data bytes per frame */
    word MaxBlkSize;                /* actual size of the receiver's buffer */
    byte *DataPtrBlock;             /* Pointer at first byte in first frame of current block */
    byte VersionIDRx;               /* version id of receiver */
    byte SegIDTx;                   /* segment id to transmit */
    dword TotalPacketSent;          /* bytes sent in the current packet */
    word CurrentBlockSize;          /* size of current block */
    word LengthRem;                 /* remaining length of current segment */
    byte FramesPerBlock;            /* actual number of frames in current block */
    dword TotalPacketLengthInt;     /* allows to transmit packets > 64kByte */

    /* public fields: */
    byte Options;                   /* Single-Frame- or Block-Acknowledge */
    byte Priority;                  /* Tx Priority */
    #if (defined MHP_DELAY_RANGE_EN) && (defined MHP_TX_MIN)
    word MinDelay;                  /* Minimum Delay value (between two data frames) for a certain connection (set by application) */
    word MaxDelay;                  /* Maximum Delay value (set by application) */
    #endif
    word Tgt_Adr;
    byte FBlock_ID;
    byte Inst_ID;
    word Func_ID;
    byte Operation;
    word Length;
    dword TotalPacketLength;        /* allows to transmit packets > 64kByte */
    byte *DataPtr;                  /* Pointer at data to transmit */
    #if (MAX_MHP_TX_HANDLE >0)
    byte TxHandle[MAX_MHP_TX_HANDLE];
    #endif
    byte HoldConOpt;                /* Options for sending Hold Connection Tx Control Data */

} TMhpTx, *pTMhpTx;

typedef const TMhpTx* pcTMhpTx;     /* Pointer to const object */

#endif


#ifdef MHP_RX_MIN
typedef struct Mhp_Rx_Type
{
    /* private fields: */
    byte Status;                    /* Buffer Status */
    byte State;                     /* State Machine */
    byte StateBack;                 /* Backup State when returning from Hold Loop */
    byte Retry1;                    /* Retry Counter */
    byte HoldEvent;
    byte HoldLoop;
    word BufRetry;                  /* Retry if no buffer available */
    byte BlockCnt;                  /* Current Block Counter (reset when connection is established) */
    TMostTimerHL TimerRx1;          /* Timeout */

    word TimerRx1Back;              /* Backup of Timer since overwritten while Hold Loop */
    byte VersionIDTx;               /* version id of sender */

    byte Options;                   /* FrAck or BlockAck  */
    byte Priority;                  /* Rx Priority */
    word NumDataFrame;              /* Number of user data bytes per frame */
    void *BufPtr;                   /* Buffer Interface: Ptr at Data Buffer or Service Function */
    word BufSize;                   /* Buffer Interface: Size of Data Buffer in byte */
    byte BufFlags;                  /* Buffer Interface: Flags */
    byte FrameIdLast;               /* Last/Max Frame Id */
    byte Scale;
    struct Mhp_Rx_Type *NextPtr;
    struct Mhp_Rx_Type *PreviousPtr;

    /* public fields: */
    byte SegIDRx;                   /* received segment id */
    word BlockCntRel;               /* relative block cnt (reset on each new packet) */
    word PacketCnt;                 /* packet cnt (reset on new connection) */
    word Src_Adr;
    byte FBlock_ID;
    byte Inst_ID;
    word Func_ID;
    byte Operation;
    word Length;
    byte *DataPtrBlock;             /* First byte of block */
    #if (MAX_MHP_RX_HANDLE >0)
    byte RxHandle[MAX_MHP_RX_HANDLE];
    #endif
} TMhpRx, *pTMhpRx;

typedef const TMhpRx* pcTMhpRx;     /* Pointer to const object */

#endif


#if (defined MHP_TX_MIN) || (defined PMHT_MIN) || (defined MHP_RX_MIN)
typedef struct Mhp_TxTel_Type
{
    byte Priority;
    byte UserHandle;
    word TelLen;
    byte Addr_H;
    byte Addr_L;
    byte FBlock_ID;
    byte Inst_ID;
    byte Func_ID_H;
    byte Func_ID_L_Op;
    byte Tel_ID_Len_H;
    byte Tel_Len_L;
    byte Data[MHP_NUM_DATA_TX];
    byte Status;
    byte TelType;
    byte RetryCnt;
    void *BufPtr;
    byte *DataPtr;

    word DataPtrLength;
    word DataLength;
    #ifdef _OS81110_PCK_LLR
    byte LowLevelRetries;
    #endif
} TMhpTxTel, *pTMhpTxTel;
#endif

#if (defined MHP_TX_MIN) || (defined PMHT_MIN) || (defined MHP_RX_MIN)
typedef struct Mhp_RxTel_Type
{
    dword TimeStamp;
    word TelLen;
    byte Tgt_Addr_H;
    byte Tgt_Addr_L;
    byte Src_Addr_H;
    byte Src_Addr_L;
    byte FBlock_ID;
    byte Inst_ID;
    byte Func_ID_H;
    byte Func_ID_L_Op;
    byte Tel_ID_Len_H;
    byte Tel_Len_L;
    byte Data[MHP_NUM_DATA_RX];
    byte Status;
    byte TelType;
} TMhpRxTel, *pTMhpRxTel;
#endif



/*
==============================================================================
    Module Global API Prototypes
==============================================================================
*/


#ifdef MHP_0
    /*lint -esym( 762, MhpInit ) function also declared in file L1/mns_pb.h*/
    extern void MhpInit(void);
#endif

#ifdef MHP_01
    /*lint -esym( 762, MostServiceHigh ) function also declared in file L1/mns_pb.h*/
    extern word MostServiceHigh(word events);
#endif

#ifdef MHP_06
    extern void MhpSetTxBurstCnt(word cnt);
#endif

#ifdef MHP_T2
    extern pTMhpTx MhpGetTxPtrCon(pTMhpCon con_ptr);
#endif

#ifdef MHP_T3
    extern byte MhpSend(pTMhpTx tx_ptr);
#endif

#ifdef MHP_T8
    extern void MhpAddrHComplete(word device_id, byte fblock_id, byte inst_id);
#endif

#ifdef MHP_T102
    extern byte  MhpOpenCon(pTMhpTx tx_ptr);
#endif

#ifdef MHP_T10
    /*lint -esym( 762, MhpTxFrameComplete ) function also declared in file L1/wads_pb.h*/
    extern void MhpTxFrameComplete(void);
#endif

#ifdef MHP_T61
    extern byte MhpTxEndCon(pTMhpCon con_ptr, byte event);
#endif

#ifdef MHP_T62
    extern byte MhpTxHoldCon(pTMhpCon con_ptr, byte event );
#endif

#ifdef MHP_T63
    extern byte MhpTxContinueCon(pTMhpCon con_ptr);
#endif

#ifdef MHP_T64
    extern void MhpTxEndAllCon(void);
#endif

#ifdef MHP_T65
    extern void MhpSetConfigStateFilterStatus(byte on);
#endif

#ifdef MHP_R4
    extern void MhpSetBufIntf(byte flags, void *ptr, word buf_size);
#endif

#ifdef MHP_R6
    extern byte MhpOfferRxBufCon( pTMhpCon con_ptr, byte *data_ptr );
#endif

#ifdef MHP_R61
    extern byte MhpRxKillCon(pTMhpCon con_ptr);
#endif

#ifdef MHP_R62
    extern byte MhpRxHoldCon(pTMhpCon con_ptr, byte event );
#endif

#ifdef MHP_R63
    extern byte MhpRxContinueCon(pTMhpCon con_ptr);
#endif

#ifdef MHP_DBG_TX
    extern void MhpTxDbgPrintState(pTMhpTx buf_ptr, byte new_state);
#endif


/*
----------------------------------------------------------------------------
        Callback Functions of MOST High Protocol Service
----------------------------------------------------------------------------
*/

#ifdef MHP_CB1
    extern byte MhpTxStatus(byte status, pTMhpTx tx_ptr);
#endif

#ifdef MHP_DBG_TX
    extern void MhpDebugInfoTx(pTMhpTx buf_ptr, byte *info_ptr, byte info_length);
#endif

#ifdef MHP_CB2
    extern void MhpRxComplete(byte status, pTMhpRx rx_ptr);
#endif

#ifdef MHP_CB5
    extern bool MhpGetBufIntf(pTMhpRx buf_ptr);
#endif

#ifdef MHP_DBG_RX
    extern void MhpDebugInfoRx(pTMhpRx buf_ptr, byte *info_ptr, byte info_length);
#endif

#ifdef MHP_CB10
    extern void MnsRequestHigh(word flags);
#endif

#ifdef MHP_CB11
    extern void MnsRequestHighTimerTx(void);
#endif

#ifdef MHP_CB12
    extern void MnsRequestHighTimerRx(void);
#endif

#ifdef MHP_CB30
    extern bool MhpTxDelay(word delay, pTMhpTx tx_ptr);
#endif

#ifdef MHP_CB31
    extern word MhpTxEstablishDelay(word air_tx, word air_rx, word ratio, pTMhpTx tx_ptr);
#endif

#ifdef MHP_CB32
    extern bool MhpPerfTimerStart(word delay, word tgt_adr);
#endif

#ifdef MHP_DBG_TXTEL
    extern void MhpDebugInfoTxTel(byte status, pTMhpTxTel tel_ptr);
#endif

#ifdef MHP_DBG_RXTEL
    extern void MhpDebugInfoRxTel(pTMhpRxTel tel_ptr);
#endif



#endif /* #ifndef _MHP_PB_H */


/*
==============================================================================
    End Of File
==============================================================================
*/
