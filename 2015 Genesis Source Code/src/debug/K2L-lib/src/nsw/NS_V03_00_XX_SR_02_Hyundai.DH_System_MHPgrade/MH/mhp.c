/*
==============================================================================

Project:        MOST NetServices V3 for INIC
Module:         Implementation of the MOST High Protocol Service (MHP)
File:           mhp.c
Version:        3.0.x-SR-2  
Language:       C
Author(s):      S. Kerber, R.Wilhelm, R.Hanke
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
Date        By      		Description
2013-10-01	hanhong.keum	Fix Errata Bug

==============================================================================
*/

/*! \file
  * \brief      Implementation of MOST High Protocol Service (MHP)
  * \details    In addition to Basic Layer and Application Socket of the MOST 
  *             NetServices, the MOST High Protocol Service provides an 
  *             alternative transmission path, including an additional data 
  *             integrity layer. By using this service, large data packets as 
  *             well as stream data can be sent and received. An acknowledge
  *             mechanism provides data integrity.
  *             MOST High Protocol Service interfaces to the Asynchronous Data 
  *             Transmission Service Wrapper (wADS). So, the transmission is 
  *             done over the asynchronous channel of a MOST network.
  */


/*
==============================================================================
    Includes
==============================================================================
*/

#include "mostns.h"
#include "mdm.h"
#ifdef PMHT_MIN
  #include "pmht.h"
#endif
#include "mhp.h"
#include "mhp_pv.h"




#ifdef NS_INC_TGT_SPEC
    #include NS_INC_TGT_SPEC        /* include the file which is defined in adjust.h or in adjustmh.h */
#endif

#ifdef NS_INC_MHP
    #include NS_INC_MHP             /* include the file which is defined in adjustmh.h  */
#endif


/*
================================================================================
    Module Internal Variables
================================================================================
*/




#ifdef MHP_TX_MIN
    pTMhpTx MhpTxBPtrAct;                                               /* Pointer at active tx block buffer entry */
                                                                        /* (this entry transmitts current block)   */
    TMostTimerHL MhpTxTimerG1;
    TMostTimerHL MhpTxTimerG2;

    byte    MhpTxSpeedDir;
    word    MhpTxFrId[16];                                              /* TxFrameId Bitfield */
    word    MhpTxFrAck[16];                                             /* TxAcknowledge Bitfield */
    word    MhpTxDelayCounter;
  #ifdef MHP_TX_BURST
    byte    MhpTxBurstCnt;
    byte    MhpTxBurstCntMax;
  #endif
#endif


#if (defined MHP_TX_MIN) || (defined PMHT_MIN)
    TMhpTx  MhpTxBuf[MAX_MHP_TX];                                       /* MOST High Protocol tx block buffer  */
  #ifdef PMHT_MIN
    TMhpTgtDev MhpTxDev[MAX_MHP_TX];                                    /* administration of target devices    */
  #endif
    pTMhpTx MhpTxBPtr;                                                  /* Pointer at tx block buffer          */
    bool    MhpTxPending;
    bool    MhpTxRetrigger;
#endif


#ifdef MHP_RX_MIN

    TMhpRx   MhpRxBuf[MAX_MHP_RX];                                      /* MOST High Protocol rx block buffer */

    pTMhpRx  MhpRxBPtr;                                                 /* Pointer at rx block buffer */
    pTMhpRx  MhpRxBPtrAct;                                              /* Pointer at active rx block buffer entry  */
                                                                        /* (this entry receives current frames) */
    pTMhpRx  MhpPtrBufIntf;                                             /* Pointing at connection list entry, which is */
                                                                        /* is waiting for description of buffer interfac */

    pTMhpRx  MhpRxListFirstPtr;                                         /* Pointer to the first connection the rx connection list */
    pTMhpRx  MhpRxListLastPtr;                                          /* Pointer to the last connection the rx connection list */

    byte MhpRxFrameCnt;                                                 /* counting all received frames */
    byte MhpDelAckFrId;
    byte MhpDelAckFrIdLast;


    TMostTimerHL MhpRxTimerG1;

    word    MhpRxFrAck[16];                                             /* RxAcknowledge Bitfield */
    byte    MhpRxNumFrameFailed;                                        /* Number of data frames, that have been received without 0-Frame */
    byte    MhpRxErrCnt;
    bool    MhpRxFirstRequest;
    byte    MhpRxHighestFrame;
#endif


#if (defined MHP_TX_MIN) || (defined PMHT_MIN) || (defined MHP_RX_MIN)
    TMhpTxTel  MhpTxTel;                                                /* Tx Telegram Buffer */
    TMhpRxTel  MhpRxTel;                                                /* Rx Telegram Buffer */


    TMostTimerHL MhpTimerTxTelBuf;                                      /* timer controlling retries after TX FIFO full (MhpSendTxTel()) */

    word    MhpAvrgIntRate;                                             /* average interrupt rate (Tx) in microseconds */
    word    MhpAvrgIntRateRx;                                           /* average interrupt rate (Rx) in microseconds */


  #ifdef MNSH_OPT_1
    word MnsPendingHigh;
  #endif

    word MhpNumDataPacketRaw;                                           /* Number of Raw Data Bytes, which is supported by Hardware Layer */
#endif

#ifdef MHP_ENABLE_CFG_MSG_FILTER
    bool MhpConfigStateFilterOn;
#endif

#if (defined MHP_DBG_TX) || (defined MHP_DBG_RX)
    byte MhpDebugInfo[MHP_SIZE_DEBUG_INFO];
#endif


/*
==============================================================================
    Includes Part II
==============================================================================
*/

#ifdef NS_INC_MHP_BEHIND
#include NS_INC_MHP_BEHIND                      /* include the file which is defined in adjustmh.h  */
#endif


/*
==============================================================================
==============================================================================
    Module Implementation
==============================================================================
==============================================================================
*/
/*
==============================================================================
    Init - Section
==============================================================================
*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpInit()                                                  */
/* Description : Init MOST High Protocol Service.                           */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_0
void  MhpInit(void) 
{
    static bool mhp_first_init = MNS_TRUE;
    word i;

    RES_MHP_BEGIN

    #ifdef MHP_INIT_EXTEND
    MHP_INIT_EXTEND;
    #endif

    #ifdef MHP_TX_MIN
    MhpTxBPtrAct  = NULL;                                                       /* Init pointer at active buffer entry */
    MhpTxBPtr     = MhpTxBuf;                                                   /* load pointer for init procedure */

    for(i=(word)0; i<(word)MAX_MHP_TX; i++)                                     /* Mark all tx buffer lines free */
    {
        if( (BUF_FREE != MhpTxBPtr->Status) && (MNS_FALSE == mhp_first_init) )  /* List entry is in use and not first initialization ? */
        {
            (void)MhpTxStatusWrap(MHP_TX_CLOSE, MhpTxBPtr);                     /* Notify application about tx connection clean-up */
        }
        #ifdef MHP_DBG_TX
        MhpTxBPtr->State = TX_NORM_OP;                                          /* pre-initialisation, since state transition is reported  */
        #endif
        MostRegisterTimer(&(MhpTxBPtr->TimerTx1), (TMostEventCB *)MnsRequestHighTimerTx, (word)0);
        MhpSetTxBufferFree(MhpTxBPtr++);
    }

    MhpTxBPtr       = MhpTxBuf;                                                 /* Init pointer at tx block buffer */
    MhpTxPending    = MNS_FALSE;
    MhpTxRetrigger  = MNS_FALSE;

    MostRegisterTimer(&MhpTxTimerG1, (TMostEventCB *)MnsRequestHighTimerTx, (word)0);
    MostRegisterTimer(&MhpTxTimerG2, (TMostEventCB *)MnsRequestHighTimerTx, (word)0);

    #ifdef MHP_TX_BURST
    MhpTxBurstCntMax = MHP_TX_BURST_CNT;
    #endif
    #endif /* MHP_TX_MIN */

    #ifdef PMHT_MIN
    PmhtInit();
    #endif

    #ifdef MHP_RX_MIN
    MhpPtrBufIntf = NULL;
    MhpRxBPtrAct  = NULL;                                                       /* Init pointer at active buffer entry */
    MhpRxListFirstPtr = NULL;
    MhpRxListLastPtr  = NULL;
    MhpRxBPtr     = MhpRxBuf;                                                   /* load pointer for init procedure */

    for(i=(word)0; i<(word)MAX_MHP_RX; i++)                                     /* Mark all rx buffer lines free */
    {
        if( (BUF_FREE != MhpRxBPtr->Status) && (MNS_FALSE == mhp_first_init) )  /* List entry is in use and not first initialization ? */
        {
            MhpRxCompleteWrap(MHP_RX_CLOSE, MhpRxBPtr);                         /* Notify application about rx connection clean-up */
        }
        #ifdef MHP_DBG_RX
        MhpRxBPtr->State = RX_NORM_OP;                                          /* pre-initialisation, since state transition is reported  */
        #endif
        MhpRxBPtr->PreviousPtr = NULL;
        MhpRxBPtr->NextPtr     = NULL;
        MostRegisterTimer(&(MhpRxBPtr->TimerRx1), (TMostEventCB *)MnsRequestHighTimerRx, (word)0);
        MhpSetRxBufferFree(MhpRxBPtr++);
    }

    MhpRxBPtr     = MhpRxBuf;                                                   /* Init pointer at rx block buffer */
    MhpRxFrAck[0] = (word)1;

    MostRegisterTimer(&MhpRxTimerG1, (TMostEventCB *)MnsRequestHighTimerRx, (word)0);
    #endif /* MHP_RX_MIN */


    MhpTxTel.Status   = BUF_FREE;                                               /* unlock Tx telegramm buffer */
    MhpRxTel.Status   = BUF_FREE;                                               /* unlock Rx telegramm buffer */
    MhpTxTel.RetryCnt = MHP_TX_TEL_RETRY_MAX;
    MostRegisterTimer(&MhpTimerTxTelBuf, (TMostEventCB *)MnsRequestHighTimerTx, (word)0);

    MhpNumDataPacketRaw = MHP_PAYLOAD_ADS;
    MhpAvrgIntRate      = MHP_AVRG_INT_RATE_DEFAULT_ADS;                        /* Load average interrupt rate */
    MhpAvrgIntRateRx    = MHP_AVRG_INT_RATE_RX_DEFAULT_ADS;


    #ifdef MHP_ENABLE_CFG_MSG_FILTER
    MhpConfigStateFilterOn = MNS_TRUE;
    #endif

    #ifdef MNSH_OPT_1
    RES_MNSPH_BEGIN
    MnsPendingHigh = (word)0x0000;                                              /* Clear all request flags */
    RES_MNSPH_END
    #endif

    mhp_first_init = MNS_FALSE;                                                 /* First initialization completed ! */

    RES_MHP_END
}
#endif





/*
==============================================================================
    Kernel - Section
==============================================================================
*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MostServiceHigh()                                          */
/* Description : Trigger function of MOST High Protocol Service Module      */
/* Parameter(s): none                                                       */
/* Returns     : - Request Flags, if MNSH_OPT_1 is defined in adjustmh.h    */
/*               - 0xFFFF, if MNSH_OPT_1 is not defined in adjustmh.h       */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_01
word  MostServiceHigh(word events) 
{
    word i;
    #ifdef MNSH_OPT_1
    word ret;
    #endif


    RES_MHP_BEGIN

    #if (defined MHP_TX_MIN) || (defined PMHT_MIN)
    if(events & MNSH_E_TX_EVENT)                /* Tx Event by alternative packet service ?   */
    {
        MhpTxPending = MNS_FALSE;               /* --> functionality of MhpTxFrameComplete()    */
                                                /*     (and flag MNSH_P_TX_STATE is copied from event parameter) */
    }
    #endif

    #ifdef MNSH_OPT_1
    RES_MNSPH_BEGIN
    MnsPendingHigh |= (events & (MNSH_E_MASK)); /* capture relevant events */
    RES_MNSPH_END
    #endif


    #ifndef PACKET_COMMUNICATION_ONLY
    if(MNS_NET_ON == MostGetState())            /* Check state of MOST Supervisor */
    #endif
    {
        #ifdef MNSH_OPT_1
        RES_MNSPH_BEGIN
        MnsPendingHigh &= MNSH_P_MASK;          /* mask used flags */
        RES_MNSPH_END
        #endif

        /*-------------------------- */
        /*    Service TxTel Buffer   */
        /*-------------------------- */
        if(MhpTxTel.RetryCnt < MHP_TX_TEL_RETRY_MAX)
        {
            MhpSendTxTel();                     /* call only if retry */
        }

        #if (defined MHP_T6) || (defined PMHT_6)
        /*-------------------------- */
        /*    Service Tx Section     */
        /*-------------------------- */
        #ifdef MNSH_OPT_1
        RES_MNSPH_BEGIN
        if(MnsPendingHigh & (MNSH_P_TX_STATE | MNSH_P_TX_TIMEOUT | MNSH_P_TX_NOBUF))                /* check pending tx flags */
        RES_MNSPH_END
        #endif
        {
            #ifdef MNSH_OPT_1
            RES_MNSPH_BEGIN
            MnsPendingHigh &= (word)(~(MNSH_P_TX_STATE | MNSH_P_TX_TIMEOUT | MNSH_P_TX_NOBUF));     /* clear pending tx flags */
            RES_MNSPH_END
            #endif

            #ifdef MHP_TX_BURST
            MhpTxBurstCnt = MhpTxBurstCntMax;
            #endif

            for(i=(word)0; i<(word)MAX_MHP_TX; i++)
            {
                do
                {
                    MhpTxTrigger();             /* MOST High Protocol Service Tx Section */
                } while(MNS_FALSE != MhpTxRetrigger);
            }
        }
        #endif


        #ifdef MHP_R1
        /*-------------------------- */
        /*    Service Rx Section     */
        /*-------------------------- */
        #ifdef MNSH_OPT_1
        RES_MNSPH_BEGIN
        if(MnsPendingHigh & (MNSH_P_RX_STATE | MNSH_P_RX_TIMEOUT | MNSH_P_RX_NOBUF))                /* check pending rx flags */
        RES_MNSPH_END
        #endif
        {
            #ifdef MNSH_OPT_1
            RES_MNSPH_BEGIN
            MnsPendingHigh &= (word)(~(MNSH_P_RX_STATE | MNSH_P_RX_TIMEOUT | MNSH_P_RX_NOBUF));     /* clear pending rx flags */
            RES_MNSPH_END
            #endif

            for(i=(word)0; i<(word)MAX_MHP_RX; i++)
            {
                MhpRxTrigger();                 /* MOST High Protocol Service Rx Section */
            }
        }
        #endif

    }   /* end of (if state == MNS_NET_ON) */
    #ifndef PACKET_COMMUNICATION_ONLY
    #ifdef MNSH_OPT_1
    else
    {
        RES_MNSPH_BEGIN
        MnsPendingHigh = (word)0;               /* Clear request flags, since not in state MNS_NET_ON */
        RES_MNSPH_END
    }
    #endif
    #endif

    #ifdef MNSH_OPT_1
    RES_MNSPH_BEGIN
    ret = MnsPendingHigh;                       /* Return Pending Flags */
    RES_MNSPH_END
    RES_MHP_END
    return(ret);
    #else
    RES_MHP_END
    return((word)0xFFFF);
    #endif
}
#endif




/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MnsPendingHighSet()                                        */
/* Description : Set one or several request flags in variable               */
/*               MnsPendingHigh.                                            */
/*                                                                          */
/* Parameter(s): flags                                                      */
/* Returns     : Nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_02
void  MnsPendingHighSet(word flags) 
{
    RES_MNSPH_BEGIN
    MnsPendingHigh |= flags;
    RES_MNSPH_END
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MnsPendingHighSetAndCall()                                 */
/* Description : Set one or several request flags in variable               */
/*               MnsPendingHigh and call application via callback function. */
/*                                                                          */
/* Parameter(s): flags                                                      */
/* Returns     : Nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_03
void  MnsPendingHighSetAndCall(word flags) 
{
    RES_MNSPH_BEGIN
    MnsPendingHigh |= flags;
    flags = MnsPendingHigh;
    RES_MNSPH_END
    MnsRequestHigh(flags);
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                   API    */
/* Function    : MhpSetTxBurstCnt()                                         */
/* Description : Set number of frames that should be generated in one       */
/*               one single MostServiceHigh() cycle and the software        */
/*               architecture.                                              */
/*                                                                          */
/* Parameter(s): number of frames within one cycle                          */
/* Returns     : Nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_06
void  MhpSetTxBurstCnt(word cnt) 
{
    #ifdef MHP_TX_BURST
    MhpTxBurstCntMax = (byte)cnt;
    #else
    (void)cnt;      /* To avoid PC-Lint warning */
    #endif
}
#endif





/*
==============================================================================
    Transceiver - Section
==============================================================================
*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpGetTxPtrCon()                                       API */
/* Description : Get ptr for next tx block buffer entry.                    */
/* Parameter(s): Ptr at structure containing connection description.        */
/*               This description contains:                                 */
/*               Target.FBlockID.InstID.Func.Operation                      */
/*                                                                          */
/* Returns     : Pointer at corresponding tx buffer, if connection already  */
/*               established, or free buffer, whenever connection not       */
/*               already existing.                                          */
/*                                                                          */
/*               NULL: No free buffer available                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T2
pTMhpTx  MhpGetTxPtrCon(pTMhpCon con_ptr)      /*lint -esym( 818, con_ptr ) no use of pcTMhpCon in API functions*/
{
    word    i;
    pTMhpTx buf_ptr;

    RES_MHP_BEGIN

    buf_ptr = MhpSearchTxBufCon(con_ptr);                   /* Search for already existing buffer entry */

    if(NULL == buf_ptr)
    {
        /*------------------------------*/
        /* No existing connection found */
        /*------------------------------*/

        buf_ptr = MhpTxBuf;                                 /* -> search for a free bufferline */
        for(i=(word)0; i<(word)MAX_MHP_TX; i++)
        {
            if((byte)0 == buf_ptr->Status)                  /* free buffer line found ? */
            {
                buf_ptr->Status = BUF_F_LOCK;               /* lock buffer line */

                buf_ptr->Tgt_Adr    = con_ptr->Addr;        /* copy description of connection to new buffer entry  */
                buf_ptr->FBlock_ID  = con_ptr->FBlock_ID;
                buf_ptr->Inst_ID    = con_ptr->Inst_ID;
                buf_ptr->Func_ID    = con_ptr->Func_ID;
                buf_ptr->Operation  = con_ptr->Operation;
                buf_ptr->HoldConOpt = MHP_HOLD_TO;

                #ifdef MHP_DBG_TX
                MhpDebugInfo[0] = MHP_DBG_GETTXPTR_TX;
                MhpDebugInfo[1] = (byte)0x01;               /* new connection will be established */
                MhpDebugInfoTx(buf_ptr, &MhpDebugInfo[0], (byte)2);
                #endif

                RES_MHP_END
                return(buf_ptr);                            /* and return pointer at buffer */
            }

            buf_ptr++;
        }

        #ifdef MHP_DBG_TX
        MhpDebugInfo[0] = MHP_DBG_GETTXPTR_TX;
        MhpDebugInfo[1] = (byte)0x02;                       /* Connection not already existing, but no free entry in TX list */
        MhpDebugInfoTx(buf_ptr, &MhpDebugInfo[0], (byte)2);
        #endif

        RES_MHP_END
        return(NULL);                                       /* return NULL since no buffer free */
    }
    else
    {
        /*------------------------------*/
        /* Existing connection found    */
        /*------------------------------*/
        if(buf_ptr->Status & BUF_F_RELOAD)                  /* time slot, in which re-load is possible ? */
        {
            if(buf_ptr->Status & BUF_F_READY)
            {
                /*------------------------------*/
                /*     Connection still         */
                /*        established           */
                /*------------------------------*/

                #ifdef MHP_TX_BUFFER_LOCK
                buf_ptr->Status = BUF_F_LOCK;               /* -> reset status of existing connection until application  */
                                                            /*    filled out the structure fields */
                #endif

                if(MhpTxBPtrAct == buf_ptr)                 /* Check Token */
                {
                    MhpTxBPtrAct = NULL;
                }

                #ifdef MHP_DBG_TX
                MhpDebugInfo[0] = MHP_DBG_GETTXPTR_TX;
                MhpDebugInfo[1] = (byte)0x04;               /* Connection still established. Transmission not pending. Not yet terminated. */
                MhpDebugInfoTx(buf_ptr, &MhpDebugInfo[0], (byte)2);
                #endif
            }
            else
            {
                /*------------------------------*/
                /*     "End Connection TX"      */
                /*        already sent          */
                /*------------------------------*/
                #ifdef MHP_DBG_TX
                MhpDebugInfo[0] = MHP_DBG_GETTXPTR_TX;
                MhpDebugInfo[1] = (byte)0x05;                   /* Termination in process. */
                MhpDebugInfoTx(buf_ptr, &MhpDebugInfo[0], (byte)2);
                #endif

                buf_ptr->Status = BUF_F_LOCK;                   /* -> reset status of existing connection until application  */
                                                                /*    filled out the structure fields */

                MHP_DBG_TX_STATE(buf_ptr, TX_NORM_OP);          /* DebugPrint state transition */
                buf_ptr->State  = TX_NORM_OP;                   /* -> Build connection again, since receiver has lost the connection */
            }

            RES_MHP_END
            return(buf_ptr);
        }

        #ifdef MHP_DBG_TX
        MhpDebugInfo[0] = MHP_DBG_GETTXPTR_TX;
        MhpDebugInfo[1] = (byte)0x03;                           /* Already existing connection. Transmission pending. */
        MhpDebugInfoTx(buf_ptr, &MhpDebugInfo[0], (byte)2);
        #endif

        RES_MHP_END
        return(NULL);                                           /* Prevent a new connection, since current transmission is finished not yet */
    }
}                                                               /*lint +esym( 818, con_ptr ) no use of pcTMhpCon in API functions*/
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpOpenCon()                                           API */
/* Description : Open an MHP Connection and set the connection into         */
/*               hold state.                                                */
/* Parameter(s): ptr at buffer line                                         */
/* Returns     : Status                                                     */
/*                 MHP_OC_SUCCESS: Open conn. trig. successfully            */
/*                 MHP_OC_ALREADY_OPEN: Connection already open             */
/*                 MHP_OC_AH_BUSY: Connection not opend,                    */
/*                                 Address Handler is busy                  */
/*                 MHP_OC_NCS_NOT_OK: Connection is not opened,             */
/*                                    because ConfigState is in State NotOk */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T102
byte  MhpOpenCon(pTMhpTx tx_ptr) 
{
    byte ret;
    byte res_addr_res;

    RES_MHP_BEGIN

    if(TX_NORM_OP == tx_ptr->State)                         /* Connection still closed? */
    {
        ret = MHP_OC_SUCCESS;                               /* Default return value => Everything is alright */

        MhpOpenCheckPriority(tx_ptr);                       /* Check preconditions. If not valid, */
                                                            /* the consitions are set to default values */

        #ifdef MHP_ENABLE_CFG_MSG_FILTER
        if(MNS_FALSE != MhpConfigStateFilterOn)             /* Connection configuration not ok? */
        {
            res_addr_res = MHP_AR_NONE;                     /* Address resolution not checked */

            MhpSetTxBufferFree(tx_ptr);                     /* Error occurred => Set Tx buffer free */
            ret = MHP_OC_NCS_NOT_OK;
        }
        else
        #endif
        {
            res_addr_res = MhpAddressResolution(tx_ptr);    /* Check address resolution */

            if(MHP_AR_BUSY == res_addr_res)                 /* Address handler busy ? */
            {
                MhpSetTxBufferFree(tx_ptr);                 /* Error occurred => Set Tx buffer free */
                ret = MHP_OC_AH_BUSY;                       /* Error occurred => Return Error Code */
            }
            else
            {
                tx_ptr->Flags |= MHP_OPEN_CON_ONLY;
            }
        }

        /* Check whether everything is alright and not waiting for address handler */
        if( (MHP_OC_SUCCESS == ret) && (MHP_AR_OK == res_addr_res) )
        {
            tx_ptr->Flags |= MHP_OPEN_CON_ONLY;             /* Set flag "MHP Open Connection Only" */
            MhpStartSend(tx_ptr);                           /* Start to open connection */
        }
    }
    else
    {
        ret = MHP_OC_ALREADY_OPEN;                          /* Connection already opened => Return status code */
    }

    RES_MHP_END

    return(ret);                                            /* Return error/status code */
}
#endif



/*---------------------------------------------------------------------------------*/
/*                                                                                 */
/* Function    : MhpSend()                                                     API */
/* Description : Open an MHP connection and send data.                             */
/* Parameter(s): ptr at buffer line                                                */
/* Returns     : Status                                                            */
/*         MHP_SEND_SUCCESS: Open conn. and sending data  trig. successfully       */
/*         MHP_SEND_TX_LEN_ZERO: Data is not send, Length is zero                  */
/*         MHP_SEND_INV_TOTAL_PL: Data is not send, Total Packet Length is too big */
/*         MHP_SEND_AH_BUSY:  Data not send, Address Handler is busy               */
/*         MHP_SEND_NCS_NOT_OK : Data is not send,                                 */
/*                               NWM.ConfigurationState is NotOK                   */
/*         MHP_SEND_TX_NO_DATA: No data pointer given                              */
/*                                                                                 */
/*---------------------------------------------------------------------------------*/
#ifdef MHP_T3
byte  MhpSend(pTMhpTx tx_ptr) 
{
    byte ret;
    byte res_addr_res;

    RES_MHP_BEGIN

    ret = MHP_SEND_SUCCESS;                             /* Everything is alright */

    MhpOpenCheckPriority(tx_ptr);                       /* Check preconditions to open connection */

    ret = MhpCheckPreCondSend(tx_ptr);

    if(MHP_SEND_SUCCESS == ret)
    {
        res_addr_res = MhpAddressResolution(tx_ptr);    /* Check address resolution */

        if(MHP_AR_BUSY == res_addr_res)                 /* Address handler busy ? */
        {
            MhpSetTxBufferFree(tx_ptr);                 /* Error occurred => Set Tx buffer free */
            ret = MHP_SEND_AH_BUSY;                     /* Error occurred => Return Error Code */
        }
    }
    else
    {
        res_addr_res = MHP_AR_NONE;                     /* Address resolution not checked */
        MhpSetTxBufferFree(tx_ptr);                     /* Error occurred => Set Tx buffer free */
    }

    /* Check whether everything is alright and not waiting for address handler */
    if( (MHP_SEND_SUCCESS == ret) && (MHP_AR_OK == res_addr_res) )
    {
        tx_ptr->Flags &= (word)(~MHP_OPEN_CON_ONLY);    /* Reset flag "MHP Open Connection Only" */
        if(TX_NORM_OP != tx_ptr->State)                 /* Connection not in "Normal State"? */
        {
            MHP_DBG_TX_STATE(tx_ptr, TX_NEXT_SEG);      /* DebugPrint state transition */
            tx_ptr->State = TX_NEXT_SEG;                /* -> next state, but wait there until segment is ready to send (BUF_F_READY) */
        }
        MhpStartSend(tx_ptr);                           /* Start to open connection and to send data respectively */
    }

    RES_MHP_END
    return(ret);
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpOpenCheckPriority()                                     */
/* Description : Check priority field and force default value, if value out */
/*               of range.                                                  */
/* Parameter(s): ptr at buffer line                                         */
/* Returns     : none                                                       */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T103
static _INLINE void  MhpOpenCheckPriority(pTMhpTx tx_ptr) 
{
    /* check priority field and force default value, if value out of range */
    if(MHP_PRIO_MAX < tx_ptr->Priority)
    {
        tx_ptr->Priority = MHP_PRIO_DEFAULT;
    }
    if(MHP_PRIO_MIN > tx_ptr->Priority)
    {
        tx_ptr->Priority = MHP_PRIO_DEFAULT;
    }

    return;
}
#endif




/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpCheckPreCondSend()                                      */
/* Description : Check preconditions to send MHP data.                      */
/* Parameter(s): ptr at buffer line                                         */
/* Returns     : status/error code:                                         */
/*                 MHP_SEND_SUCCESS       successful                        */
/*                 MHP_SEND_TX_LEN_ZERO   Length is zero                    */
/*                 MHP_SEND_INV_TOTAL_PL  Total Packet Length is too big    */
/*                 MHP_SEND_NCS_NOT_OK    NWM.ConfigurationState is NotOK   */
/*                 MHP_SEND_TX_NO_DATA    missing data pointer given        */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T104
static byte  MhpCheckPreCondSend(pTMhpTx tx_ptr) 
{
    byte ret;

    ret = MHP_SEND_SUCCESS;                                         /* Default return value MHP_SEND_SUCCESS => Everything is alright */

    if((dword)0 == tx_ptr->TotalPacketLength)                       /* Zero TotalPacketLength? */
    {
        tx_ptr->TotalPacketLengthInt = (dword)tx_ptr->Length;       /* Set Internal-TotalPacketLength to data length */
    }
    else
    {
        tx_ptr->TotalPacketLengthInt = tx_ptr->TotalPacketLength;   /* Set Internal-TotalPacketLength to User-TotalPacketLength */
    }

    if((word)0 == tx_ptr->Length)                                   /* Zero data length? */
    {
        ret = MHP_SEND_TX_LEN_ZERO;                                 /* Error occurred => Set return value to MHP_SEND_TX_LEN_ZERO */
    }
    else if (NULL == tx_ptr->DataPtr)                                  /* Data pointer is NULL */
    {
        ret = MHP_SEND_TX_NO_DATA;                                  /* Error occurred => Set return value to MHP_SEND_TX_NO_DATA */
    }
    else if( (MHP_MAX_TOTAL_PACKET_LENGTH < tx_ptr->TotalPacketLengthInt) ||
             ( (tx_ptr->TotalPacketLengthInt < (dword)tx_ptr->Length) ) )  /* Invalid total packet length? */
    {
        ret = MHP_SEND_INV_TOTAL_PL;                                /* Error occurred => Set return value to MHP_SEND_INV_TOTAL_PL */
    }
    else
    #ifdef MHP_ENABLE_CFG_MSG_FILTER
    if(MNS_FALSE != MhpConfigStateFilterOn)                         /* Connection configuration not ok? */
    {
        ret = MHP_SEND_NCS_NOT_OK;
    }
    else
    #endif
    {
        if(MHP_FRAME_ACK != tx_ptr->Options)                        /* Check option field and force default value, if out of range */
        {
            tx_ptr->Options = MHP_BLOCK_ACK;
        }

        tx_ptr->LengthRem = tx_ptr->Length;                         /* Set remaining block length to data length */
    }

    return(ret);                                                    /* Return error/status code */
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpAddressResolution()                                     */
/* Description : Make address resolution for target address. Call address   */
/*               handler if required.                                       */
/* Parameter(s): ptr at buffer line                                         */
/* Returns     : status/error code:                                         */
/*                 MHP_AR_OK      address resolution successful             */
/*                 MHP_AR_BUSY    address handler is busy                   */
/*                 MHP_AR_WAIT    waiting for address resolution            */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T105
static byte  MhpAddressResolution(pTMhpTx tx_ptr) 
{
    byte ret;

    #if (defined NS_AH_MHP) && (defined MHP_ADDRESS_RESOLUTION)                     /* interface to Address Handler available ? */
    pTMsgTx ptr_ams_msg;

    #ifdef AH_8                                                                     /* decentral device registry available ? */
    word device_id;
    #endif


    ret = MHP_AR_OK;

    if( ((word)0xFFFF == tx_ptr->Tgt_Adr) && (OP_REPORTS > tx_ptr->Operation) )     /* unknown Device ID ? --> use Address Handler for Address */
    {                                                                               /* resolution, but only when not directed to a FBlock Shadow */

        if( (FBLOCK_ALL != tx_ptr->FBlock_ID) && (INST_ALL != tx_ptr->Inst_ID) )    /* no broadcast allowed on MHP */
        {
            #ifdef AH_8                                                             /* decentral device registry available ? */
            device_id = AddrHDevTabGet(tx_ptr->FBlock_ID, tx_ptr->Inst_ID);         /* search in local device registry */
            #endif

            #ifdef AH_8
            if((word)0xFFFF == device_id)                                           /* device id not already known ? */
            {
            #endif
                tx_ptr->Status |= BUF_F_MHP_AH;

                ptr_ams_msg = MsgGetTxPtrExt((word)0);
                if(ptr_ams_msg)
                {
                    ptr_ams_msg->Tgt_Adr    = (word)0xFFFF;
                    ptr_ams_msg->FBlock_ID  = tx_ptr->FBlock_ID;
                    ptr_ams_msg->Inst_ID    = tx_ptr->Inst_ID;
                    ptr_ams_msg->Func_ID    = (word)0x0000;                         /* FktIDs */
                    ptr_ams_msg->Operation  = (byte)0x01;                           /* Get */
                    ptr_ams_msg->MidLevelRetries = (byte)0;                         /* set number of mid level retries */
                    AmsMsgSend(ptr_ams_msg);
                    ret = MHP_AR_WAIT;
                }
                else
                {
                    ret = MHP_AR_BUSY;
                }
            #ifdef AH_8
            }
            else
            {
                tx_ptr->Tgt_Adr = device_id;
                ret = MHP_AR_OK;
            }
            #endif
        }
        else
        {
                ret = MHP_AR_OK;
        }
    }
    else
    #else
    (void)tx_ptr;                           /* To avoid PC-Lint warning */
    #endif /* #if (defined NS_AH_MHP) && (defined MHP_ADDRESS_RESOLUTION) */
    {
        ret = MHP_AR_OK;
    }

    return(ret);
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpStartSend()                                             */
/* Description : Mark buffer line as ready to send.                         */
/*               This function is called by MHP service as soon as the      */
/*               DeviceID is has been solved.                               */
/* Parameter(s): ptr at buffer line                                         */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T4
static void  MhpStartSend(pTMhpTx tx_ptr) 
{
    RES_MHP_BEGIN

    tx_ptr->Status = (BUF_F_LOCK | BUF_F_READY);                /* mark buffer line as ready to send */
    MNS_REQUEST_HIGH_CALL(MNSH_P_TX_STATE);                     /* Request for calling MostServiceHigh()  */

    RES_MHP_END
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpAddrHComplete()                                         */
/* Description : This function is called by the Address Handler             */
/*               after the search process has been finished.                */
/*               When the address could not be found, the device id is      */
/*               is still 0xFFFF and the connection fails.                  */
/* Parameter(s): Device ID, FBlock ID, Inst ID                              */
/*               Device ID == 0x0000 and Device ID == 0xFFFF                */
/*               will result in a dummy transmission to target 0x0000 !     */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T8
void  MhpAddrHComplete(word device_id, byte fblock_id, byte inst_id) 
{
    #if (defined NS_AH_MHP) && (defined MHP_ADDRESS_RESOLUTION)     /* interface to Address Handler available ? */
    word    i;
    pTMhpTx buf_ptr;

    buf_ptr = MhpTxBuf;

    for(i=(word)0; i<(word)MAX_MHP_TX; i++)         /* search whole buffer for corresponding tx buffer lines */
    {
        if(buf_ptr->Status & BUF_F_MHP_AH)          /* uncomplete entry, waiting for DeviceID */
        {
            if( (buf_ptr->FBlock_ID == fblock_id) &&
                (buf_ptr->Inst_ID   == inst_id) )
            {
                RES_MHP_BEGIN
                buf_ptr->Tgt_Adr = device_id;
                RES_MHP_END
                MhpStartSend(buf_ptr);              /* flag BUF_F_MHP_AH will be cleared in MhpStartSend() */
            }
        }

        buf_ptr++;
    }
    #else
    /* Just a dummy function. Buffer entry has already marked as ready by MhpStartSend(). */
    /* void-casts to avoid PC-Lint warnings */
    (void)device_id;
    (void)fblock_id;
    (void)inst_id;
    #endif
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpSetTxBufferFree()                                       */
/* Description : Set certain tx block buffer line free after block was      */
/*               send or error occurred.                                    */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T5
static void  MhpSetTxBufferFree(pTMhpTx buf_ptr) 
{
    #if (MAX_MHP_TX_HANDLE >0)
    word i;
    #endif

    buf_ptr->Status     = BUF_FREE;                     /* Set buffer line free */
    MHP_DBG_TX_STATE(buf_ptr, TX_NORM_OP);              /* DebugPrint state transition */
    buf_ptr->State      = TX_NORM_OP;                   /* Reset State Machine */
    buf_ptr->Priority   = MHP_PRIO_DEFAULT;             /* Default Priority */

    #ifdef MHP_DELAY_RANGE_EN
    buf_ptr->MinDelay   = (word)0;                      /* init range of delay value */
    buf_ptr->MaxDelay   = MAX_DELAY_DEFAULT;
    #endif

    buf_ptr->Length               = (word)0;            /* reset length field */
    buf_ptr->LengthRem            = (word)0;
    buf_ptr->TotalPacketLength    = (dword)0;
    buf_ptr->TotalPacketLengthInt = (dword)0;
    buf_ptr->TotalPacketSent      = (dword)0;
    buf_ptr->CurrentBlockSize     = (word)0;
    buf_ptr->Options              = MHP_BLOCK_ACK;      /* Default: Block Ack */
    buf_ptr->HoldEvent            = (byte)0x00;
    buf_ptr->EndEvent             = (byte)0x00;
    buf_ptr->FBlock_ID            = (byte)0x00;
    buf_ptr->BufRetry             = MHP_BUF_RETRY;      /* Preload retry counter  */
                                                        /* (is decremented on eeach try to get a telegram buffer) */
    buf_ptr->Flags                = (word)0;

    MostClearTimer(&buf_ptr->TimerTx1);                 /* unload timer */

    #if (MAX_MHP_TX_HANDLE > 0)
    for (i=(word)0; i<MAX_MHP_TX_HANDLE; ++i)
    {
        buf_ptr->TxHandle[i] = (byte)0;
    }
    #endif

    if(buf_ptr == MhpTxBPtrAct)
    {
        MhpTxBPtrAct = NULL;
    }
}
#endif


/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpIncTxBPtr()                                             */
/* Description : Increment outgoing pointer MhpTxBPtr.                      */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T7
void  MhpIncTxBPtr(void) 
{
    if(&MhpTxBuf[MAX_MHP_TX - 1] == MhpTxBPtr)                  /* inc pointer modulo table size */
    {
        MhpTxBPtr = MhpTxBuf;
    }
    else
    {
        MhpTxBPtr++;
    }
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                   API    */
/* Function    : MhpTxFrameComplete()                                       */
/* Description : Trigger when transmission of last frame has been completed.*/
/*               This function must be called by the application within     */
/*               the callback function CtrlTxStatus() and DataTxStatus()    */
/*               respectively.                                              */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T10
void  MhpTxFrameComplete(void) 
{
    RES_MHP_BEGIN

    MhpTxPending = MNS_FALSE;
    MNS_REQUEST_HIGH_CALL(MNSH_P_TX_STATE);                     /* Request for calling MostServiceHigh() */

    RES_MHP_END
}
#else
void  MhpTxFrameComplete(void) 
{
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpTxTrigger()                                             */
/* Description : Trigger function for tx state machine.                     */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T6
static void  MhpTxTrigger(void) 
{
    word num;

    if(MNS_FALSE != MhpTxRetrigger)                              /* retrigger same connection ? */
    {
        MhpTxRetrigger = MNS_FALSE;
    }
    else
    {
        MhpIncTxBPtr();                                         /* service next connection */
    }

    switch(MhpTxBPtr->State)
    {
        /*----------------------------------------------------- */
        case TX_NORM_OP:
        /*----------------------------------------------------- */
            if(MhpTxBPtr->Status & BUF_F_READY)                 /* Is there a message to send ? */
            {
                MHP_DBG_TX_STATE(MhpTxBPtr, TX_GETBUF_REQ);     /* DebugPrint state transition */
                MhpTxBPtr->State = TX_GETBUF_REQ;
                MhpTxBPtr->Retry = TX_RETRY_REQUEST;

                MNS_REQUEST_HIGH_SET(MNSH_P_TX_STATE);          /* Set Request Flag for calling MostServiceHigh() */
            }
            break;


        /*----------------------------------------------------- */
        case TX_GETBUF_REQ:
        /*----------------------------------------------------- */
            if(NULL != MhpTxPrepTx(MhpTxBPtr))
            {
                num = (word)(MhpNumDataPacketRaw - MHP_ASYNC_DATA_OFFSET);          /* AsyncChannel: NumDataPacketRaw - 8bytes */

                MhpTxTel.Data[0]    = MHP_CMD_REQ_CON;                              /* HighCmd= CA */
                MhpTxTel.Data[1]    = MhpTxBPtr->Priority;                          /* Priority */
                MhpTxTel.Data[2]    = HB(num);                                      /* NumData/Frame (HB) */
                MhpTxTel.Data[3]    = LB(num);                                      /* NumData/Frame (LB) */
                MhpTxTel.Data[4]    = MHP_VERSION_ID;                               /* set own version id (sender) */
                #ifdef _OS81110_PCK_LLR
                MhpTxTel.LowLevelRetries = MDP_DEFAULT_RETRY;
                #endif
                MhpSetLengthCtrl((word)5);

                MhpSendTxTel();
                MhpTxBPtr->BufRetry = MHP_BUF_RETRY;

                MostSetTimer(&MhpTxBPtr->TimerTx1, MHP_CFG_TO_SEND, MNS_FALSE);         /* Load Timer Tx1 */

                MHP_DBG_TX_STATE(MhpTxBPtr, TX_WAIT_START);                         /* DebugPrint state transition */
                MhpTxBPtr->State = TX_WAIT_START;

                MhpTxBPtr->BlockCnt = (word)0xFFFF;                                 /* Init block counter, first block = 0x00 */
                                                                                    /* -> received "StartConnection" is ignored, */
                                                                                    /* whenever "BlockCnt" is unequal 0x00 and 0xFF. */

                MNS_REQUEST_HIGH_SET(MNSH_P_TX_STATE);                              /* Set Request Flag for calling MostServiceHigh()  */
            }
            else                                                                    /* no tx buffer available */
            {
                MhpTxCheckBufRetry(MhpTxBPtr);
            }

            break;


        /*----------------------------------------------------- */
        case TX_WAIT_START:
        /*----------------------------------------------------- */
            if((word)0 == MostGetTimer(&MhpTxBPtr->TimerTx1))               /* Timeout (t_send) ? */
            {
                if(MhpTxBPtr->Retry)                                                /* Check Retry: TX_RETRY_REQUEST */
                {
                    MHP_DBG_TX_STATE(MhpTxBPtr, TX_GETBUF_REQ);                     /* DebugPrint state transition */
                    MhpTxBPtr->State = TX_GETBUF_REQ;
                    MhpTxBPtr->Retry--;
                    MNS_REQUEST_HIGH_SET(MNSH_P_TX_STATE);                          /* Set Request Flag for calling MostServiceHigh()  */
                }
                else
                {
                    (void)MhpTxStatusWrap(MHP_TX_ERROR_BUILDCON, MhpTxBPtr);        /* -> Transmission failed */
                    MhpSetTxBufferFree(MhpTxBPtr);
                }
            }
            break;


        /*----------------------------------------------------- */
        case TX_GETBUF_READY:
        /*----------------------------------------------------- */
            if(NULL != MhpTxPrepTx(MhpTxBPtr))
            {
                #ifdef MHP_DEBUG_AVRG_INT
                MhpTxTel.Data[0]    = MHP_CMD_READY;                                /* HighCmd= FD */
                MhpTxTel.Data[1]    = HB(MhpAvrgIntRate);
                MhpTxTel.Data[2]    = LB(MhpAvrgIntRate);
                MhpTxTel.Data[3]    = HB(MhpTxBPtr->TxDelayMax);
                MhpTxTel.Data[4]    = LB(MhpTxBPtr->TxDelayMax);
                MhpSetLengthCtrl((word)5);
                #else
                MhpTxTel.Data[0]    = MHP_CMD_READY;                                /* HighCmd= FD */
                MhpSetLengthCtrl((word)1);
                #endif

                #ifdef _OS81110_PCK_LLR
                MhpTxTel.LowLevelRetries = MHP_RETRY_SHORT_FRAMES;
                #endif
                MhpSendTxTel();
                MhpTxBPtr->BufRetry = MHP_BUF_RETRY;

                MhpTxBPtr->BlockCnt = (word)0xFFFF;                     /* Init block counter, first block = 0x00 */
                MHP_DBG_TX_STATE(MhpTxBPtr, TX_NEXT_SEG);               /* DebugPrint state transition */

                if(MhpTxBPtr->Flags & MHP_OPEN_CON_ONLY)                /* Only open connection. No data to send. */
                {
                    /* ------------------------------- */
                    /*  Delay Connection:              */
                    /* ------------------------------- */
                    MhpTxBPtr->Status &= (byte)(~BUF_F_ACTIVE);         /* clear flag BUF_F_ACTIVE, since current segment acknowledged */
                    MhpTxBPtr->Status |= BUF_F_RELOAD;                  /* set flag indicating time-slot in which re-load is possible */

                    MhpTxBPtr->HoldLoopTime = MHP_LOOP_DELAY_TIME;      /* Timeout Hold-Loop */
                    MhpTxBPtr->HoldEvent    = MHP_HOLD_EVENT_TX;        /* Event for Hold-Telegram */
                    MHP_DBG_TX_STATE(MhpTxBPtr, TX_GETBUF_HOLD);        /* DebugPrint state transition */
                    MhpTxBPtr->State        = TX_GETBUF_HOLD;           /* Next State */
                    MhpTxBPtr->StateBack    = TX_GETBUF_END;            /* State when returning from Hold Loop */
                    MhpTxBPtr->Retry        = TX_RETRY_END;             /* Preload retry for state when returning from Hold Loop */
                                                                        /* Timer needs not to be stored */
                    //MhpTxBPtr->Flags &= (word)(~MHP_OPEN_CON_ONLY);	// Bug : Errata sheet 3.0.X-6 - 0300XX-017
                }
                else
                {
                    MhpTxBPtr->State = TX_NEXT_SEG;
                }

                MNS_REQUEST_HIGH_SET(MNSH_P_TX_STATE);                  /* Set Request Flag for calling MostServiceHigh()  */
            }
            else                                                        /* no tx buffer available */
            {
                MhpTxCheckBufRetry(MhpTxBPtr);
            }
            break;


        /*----------------------------------------------------- */
        case TX_CHECK_FRAME:
        /*----------------------------------------------------- */
            if((word)0 == MostGetTimer(&MhpTxBPtr->TimerTx1))             /* Timeout (t_trans) ? */
            {
                MhpGoTxWaitAck();
                break;
            }

            #ifndef MHP_TX_BURST
            if(MNS_FALSE != MhpTxPending)
            {
                break;
            }
            #endif


            if(MNS_FALSE != MhpCheckTxAckCurrent())                     /* Search for next data frame to send */
            {
                MHP_DBG_TX_STATE(MhpTxBPtr, TX_SEND_FRAME);             /* DebugPrint state transition */
                MhpTxBPtr->State = TX_SEND_FRAME;                       /* Send current data frame */
                MhpTxRetrigger   = MNS_TRUE;                            /* retrigger same connection immediately again */
            }
            else                                                        /* no data frame available */
            {
                MhpGoTxWaitAck();
            }
            break;


        /*----------------------------------------------------- */
        case TX_SEND_FRAME:
        /*----------------------------------------------------- */
            if((word)0 == MostGetTimer(&MhpTxBPtr->TimerTx1))             /* Timeout (t_trans) ? */
            {
                MhpGoTxWaitAck();
                break;
            }

            if((word)0 == MostGetTimer(&MhpTxTimerG2))                    /* timer to recover higher speed */
            {
                MostSetTimer(&MhpTxTimerG2, MHP_CFG_TO_TX_SPEED_RECOVERY, MNS_FALSE);               /* load timer to recover next higher speed */
                MhpTxSpeedUp(MhpTxBPtr);
            }

            if(NULL != MhpTxPrepTx(MhpTxBPtr))
            {
                 if( ((word)0 == MhpTxDelayCounter) || (MhpTxBPtr->Options & MHP_FRAME_ACK) )   /* All delay loops done or Single-FrAck-Mode ? */
                 {
                    MhpPrepFrame(MhpTxBPtr, MhpTxBPtr->FrameCurrent);
                    MhpTxDelayCounter = MhpTxBPtr->TxDelayMax;                                  /* load current retry counter (delay) */

                    MhpSendTxTel();
                    MhpTxFrIdState(MhpTxBPtr->FrameCurrent, MNS_FALSE);                         /* Current frame sent */
                    MhpTxBPtr->BufRetry = MHP_BUF_RETRY;
                 }
                 else
                 {
                    if(MNS_FALSE != MhpTxDelay(MhpTxDelayCounter, MhpTxBPtr))                   /* Start transmission delay and check if delay already done */
                    {
                        MhpTxRetrigger = MNS_TRUE;
                    }
                    else
                    {
                        MhpTxPending = MNS_TRUE;
                    }
                    MhpTxDelayCounter = (word)0;
                    MhpTxTel.Status = BUF_FREE;                         /* set telegram buffer free */

                    MHP_DBG_TX_STATE(MhpTxBPtr, TX_CHECK_FRAME);        /* DebugPrint state transition */
                    MhpTxBPtr->State = TX_CHECK_FRAME;                  /* try same frame the next time (delay loop) */

                    #ifdef MHP_TX_BURST
                    if(MhpTxBurstCnt)
                    {
                        MhpTxBurstCnt--;
                        MhpTxRetrigger = MNS_TRUE;
                    }
                    #endif
                    break;
                 }

                 if(MNS_FALSE != MhpTxFrAllSent())                      /* Were all frames of current block sent ? */
                 {
                    MhpGoTxWaitAck();
                    break;
                 }
                                                                        /* further frames in current block, which were sent not yet: */
                 if(MHP_FRAME_ACK == MhpTxBPtr->Options)                /* Single Frame Ack ? */
                 {
                    MhpGoTxWaitAck();
                    break;
                 }

                 /* Block Ack: */
                 MhpTxBPtr->FrameCurrent = MhpTxFrGetNext();            /* Next Frame ! */

                 MHP_DBG_TX_STATE(MhpTxBPtr, TX_CHECK_FRAME);           /* DebugPrint state transition */
                 MhpTxBPtr->State = TX_CHECK_FRAME;

                 #ifdef MHP_TX_BURST
                 if(MhpTxBurstCnt)
                 {
                     MhpTxBurstCnt--;
                     MhpTxRetrigger = MNS_TRUE;
                 }
                 #endif
            }
            else                                                        /* No tx buffer available */
            {
                MhpTxCheckBufRetry(MhpTxBPtr);
            }
            break;


        /*----------------------------------------------------- */
        case TX_WAIT_ACK:
        /*----------------------------------------------------- */
            if(MNS_FALSE != MhpCheckTxAckAll())                             /* Are all frames of current block acknowledged ? */
            {
                MhpTxBPtrAct = NULL;                                        /* release tx token, since current block completed */
                MostClearTimer(&MhpTxBPtr->TimerTx1);                       /* --> send "Hold Tx" immediately, if token can not be established */

                MhpTxBPtr->TotalPacketSent += (dword)MhpTxBPtr->CurrentBlockSize;   /* total bytes of current packet */

                if(MhpTxBPtr->TotalPacketSent >= MhpTxBPtr->TotalPacketLengthInt)
                {
                    MhpTxBPtr->TotalPacketSent   = (dword)0;
                    MhpTxBPtr->TotalPacketLengthInt = (dword)0;
                }

                if(MhpTxBPtr->FrameRemain)                                  /* further block necessary (current segment) ? */
                {
                    MHP_DBG_TX_STATE(MhpTxBPtr, TX_NEXT_BLOCK);             /* DebugPrint state transition */
                    MhpTxBPtr->State      = TX_NEXT_BLOCK;
                    MhpTxBPtr->LengthRem -= MhpTxBPtr->CurrentBlockSize;    /* remaining bytes in current block */
                }
                else                                                        /* current segment completed (no further block) */
                {
                    byte status;

                    status = MhpTxStatusWrap(MHP_TX_SUCCESS, MhpTxBPtr);

                    if(MHP_STATUS_SEND_NEXT == status)                      /* -> Callback Application for success */
                    {                                                               /*    and ask: "further segments to transmit ?" */
                        MHP_DBG_TX_STATE(MhpTxBPtr, TX_NEXT_SEG);           /* DebugPrint state transition */
                        MhpTxBPtr->State     = TX_NEXT_SEG;

                        MhpTxBPtr->LengthRem = MhpTxBPtr->Length;           /* reset remaining block length */
                    }
                    else if(MHP_STATUS_CLOSE_CON == status)                 /* shut down connection immediately */
                    {
                        MhpTxBPtr->EndEvent = (byte)0x00;                         /* connection is closed in an ordinary way */

                        MhpTxBPtr->Status = (byte)(BUF_F_IGN_HOLD | BUF_F_RELOAD | BUF_F_READY | BUF_F_LOCK);
                                                                            /* Flag: Ignore incoming HoldRx Telegrams */
                        MHP_DBG_TX_STATE(MhpTxBPtr, TX_GETBUF_END);           /* DebugPrint state transition                                                                           */
                        MhpTxBPtr->State = TX_GETBUF_END;
                        MhpTxBPtr->Retry = TX_RETRY_END;
                    }
                    else
                    {
                        if(TX_WAIT_ACK == MhpTxBPtr->State)                     /* no further segment -> delay end of connection */
                        {                                                       /* but only, if application did not terminate connection */
                                                                                /* within MhpTxStatus() by calling MhpTxEndCon() */
                            /* ------------------------------- */
                            /*  Delay Connection:              */
                            /* ------------------------------- */
                            MhpTxBPtr->Status &= (byte)(~BUF_F_ACTIVE);         /* clear flag BUF_F_ACTIVE, since current segment acknowledged */
                            MhpTxBPtr->Status |= BUF_F_RELOAD;                  /* set flag indicating time-slot in which re-load is possible  */

                            MhpTxBPtr->HoldLoopTime = MHP_LOOP_DELAY_TIME;      /* Timeout Hold-Loop */
                            MhpTxBPtr->HoldEvent    = MHP_HOLD_EVENT_ENDDELAY;  /* Event for Hold-Telegram */
                            MHP_DBG_TX_STATE(MhpTxBPtr, TX_GETBUF_HOLD);        /* DebugPrint state transition */
                            MhpTxBPtr->State        = TX_GETBUF_HOLD;           /* Next State */
                            MhpTxBPtr->StateBack    = TX_GETBUF_END;            /* State when returning from Hold Loop */
                            MhpTxBPtr->Retry        = TX_RETRY_END;             /* Preload retry for state when returning from Hold Loop */
                                                                                /* Timer needs not to be stored */
                        }
                    }
                }

                MNS_REQUEST_HIGH_SET(MNSH_P_TX_STATE);                      /* Set Request Flag for calling MostServiceHigh()  */

            }
            else                                                            /* At least one frame of current block has not been acknowledged ! */
            {
                if(MHP_FRAME_ACK == MhpTxBPtr->Options)                     /* Single Frame Ack: */
                {
                    if(MNS_FALSE != MhpCheckTxAck(MhpTxBPtr->FrameCurrent))
                    {
                        MhpTxBPtr->FrameCurrent = MhpTxFrGetNext();         /* Next Frame ! */

                        MHP_DBG_TX_STATE(MhpTxBPtr, TX_CHECK_FRAME);        /* DebugPrint state transition */
                        MhpTxBPtr->State = TX_CHECK_FRAME;

                        MNS_REQUEST_HIGH_SET(MNSH_P_TX_STATE);              /* Set Request Flag for calling MostServiceHigh() */
                        break;
                    }
                }
                else    /* Block Ack Mode: */
                {
                    #ifdef MHP_DBG_TX
                    MhpDebugInfo[0] = MHP_DBG_NOTACK_TX;                    /* Debug Event: Not all frames were acknowledged */
                    MhpDebugInfoTx(MhpTxBPtr, &MhpDebugInfo[0], (byte)1);
                    #endif
                }

                if((word)0 == MostGetTimer(&MhpTxBPtr->TimerTx1))           /* Timeout (t_trans) ? */
                {
                    if(MhpTxBPtr->Retry)                                    /* Not all retries done ?  Retry: TX_RETRY_TRANS */
                    {
                        if(MHP_FRAME_ACK == MhpTxBPtr->Options)             /* Single Frame Ack: */
                        {
                            MhpStartSameBlock((byte)1);                     /* retry only remaining frames (not acknowledged frames) */
                        }
                        else                                                /* Block Ack: */
                        {
                            MhpStartSameBlock((byte)0);                     /* retry all frames of block */
                        }

                        MostSetTimer(&MhpTxBPtr->TimerTx1, MHP_CFG_TO_TRANS, MNS_FALSE);    /* Load Timer (t_trans) */

                        MHP_DBG_TX_STATE(MhpTxBPtr, TX_CHECK_FRAME);        /* DebugPrint state transition */
                        MhpTxBPtr->State = TX_CHECK_FRAME;
                        MhpTxBPtr->Retry--;                                 /* Retry: TX_RETRY_TRANS */

                        MNS_REQUEST_HIGH_SET(MNSH_P_TX_STATE);              /* Set Request Flag for calling MostServiceHigh()  */
                    }
                    else                                                    /* All retries done ! */
                    {
                        (void)MhpTxStatusWrap(MHP_TX_ERROR_NAC, MhpTxBPtr); /* -> Transmission failed, since  */
                                                                            /*    not all frames acknowledged, */
                                                                            /*    after all retries done */
                        MhpSetTxBufferFree(MhpTxBPtr);
                    }
                }
                else                                                        /* Retries within time interval t_trans */
                {
                    if((word)0 == MostGetTimer(&MhpTxTimerG1))                /* Timeout (t_trans_delay) ? */
                    {
                        if(MHP_FRAME_ACK == MhpTxBPtr->Options)                 /* Single Frame Ack: */
                        {
                            MhpStartSameBlock((byte)1);                         /* retry only remaining frames (not acknowledged frames) */
                        }
                        else                                                    /* Block Ack: */
                        {
                            MhpStartSameBlock((byte)0);                         /* retry all frames of block */
                        }

                        MHP_DBG_TX_STATE(MhpTxBPtr, TX_CHECK_FRAME);        /* DebugPrint state transition */
                        MhpTxBPtr->State = TX_CHECK_FRAME;

                        MNS_REQUEST_HIGH_SET(MNSH_P_TX_STATE);              /* Set Request Flag for calling MostServiceHigh() */
                    }
                }
            }
            break;


        /*----------------------------------------------------- */
        case TX_GETBUF_END:
        /*----------------------------------------------------- */
            if(NULL != MhpTxPrepTx(MhpTxBPtr))
            {
                MhpTxTel.Data[0] = MHP_CMD_END_TX;                          /* HighCmd= F3 */
                MhpTxTel.Data[1] = MHP_DUMMY_PID;                           /* Dummy */
                MhpTxTel.Data[2] = MhpTxBPtr->EndEvent;                     /* Event */

                #ifdef _OS81110_PCK_LLR
                MhpTxTel.LowLevelRetries = MHP_RETRY_SHORT_FRAMES;
                #endif
                MhpSetLengthCtrl((word)3);

                MhpSendTxTel();
                MhpTxBPtr->BufRetry = MHP_BUF_RETRY;

                MhpTxBPtr->Status &= (byte)(~BUF_F_READY);                  /* clear flag "READY": End Tx is transmitted -> receiver lost connection */

                MostSetTimer(&MhpTxBPtr->TimerTx1, MHP_CFG_TO_END, MNS_FALSE);  /* load Timer Tx1 */

                MHP_DBG_TX_STATE(MhpTxBPtr, TX_LOOP_END);                   /* DebugPrint state transition */
                MhpTxBPtr->State = TX_LOOP_END;

                MNS_REQUEST_HIGH_SET(MNSH_P_TX_STATE);                      /* Set Request Flag for calling MostServiceHigh() */
            }
            else    /* no tx buffer available */
            {
                MhpTxCheckBufRetry(MhpTxBPtr);
            }
            break;


        /*----------------------------------------------------- */
        case TX_LOOP_END:
        /*----------------------------------------------------- */
            if((word)0 == MostGetTimer(&MhpTxBPtr->TimerTx1))                 /* Timeout (t_end) ? */
            {
                if(MhpTxBPtr->Retry)                                        /* not all retries done ?  Retry: TX_RETRY_END */
                {
                    MHP_DBG_TX_STATE(MhpTxBPtr, TX_GETBUF_END);             /* DebugPrint state transition */
                    MhpTxBPtr->State = TX_GETBUF_END;
                    MhpTxBPtr->Retry--;

                    MNS_REQUEST_HIGH_SET(MNSH_P_TX_STATE);                  /* Set Request Flag for calling MostServiceHigh()  */
                }
                else                                                        /* all retries done ! */
                {
                    (void)MhpTxStatusWrap(MHP_TX_END_READY, MhpTxBPtr);

                    MhpSetTxBufferFree(MhpTxBPtr);
                }
            }
            break;


        /*----------------------------------------------------- */
        case TX_GETBUF_HOLD:
        /*----------------------------------------------------- */
            if(NULL != MhpTxPrepTx(MhpTxBPtr))
            {
                MhpTxTel.Data[0] = MHP_CMD_HOLD_TX;                                 /* HighCmd= F1 (Hold Tx) */
                MhpTxTel.Data[1] = MHP_DUMMY_PID;                                   /* Dummy */
                MhpTxTel.Data[2] = MhpTxBPtr->HoldEvent;                            /* Event */

                #ifdef _OS81110_PCK_LLR
                MhpTxTel.LowLevelRetries = MHP_RETRY_SHORT_FRAMES;
                #endif
                MhpSetLengthCtrl((word)3);

                MhpSendTxTel();
                MhpTxBPtr->BufRetry = MHP_BUF_RETRY;

                MostSetTimer(&MhpTxBPtr->TimerTx1, MHP_CFG_TO_HOLD_RESEND, MNS_FALSE);  /* Load Timeout (t_hold_resend)  */

                MHP_DBG_TX_STATE(MhpTxBPtr, TX_HOLD);                               /* DebugPrint state transition */
                MhpTxBPtr->State = TX_HOLD;

                MNS_REQUEST_HIGH_SET(MNSH_P_TX_STATE);                              /* Set Request Flag for calling MostServiceHigh() */
            }
            else    /* no tx buffer available */
            {
                MhpTxCheckBufRetry(MhpTxBPtr);
            }
            break;


        /*----------------------------------------------------- */
        case TX_HOLD:
        /*----------------------------------------------------- */
            if((word)0 == MostGetTimer(&MhpTxBPtr->TimerTx1))                         /* Timeout (t_hold_resent) */
            {
                if(MHP_HOLD_INF == MhpTxBPtr->HoldConOpt)                           /* Don't close connection automatically */
                {
                    MHP_DBG_TX_STATE(MhpTxBPtr, TX_GETBUF_HOLD);                    /* DebugPrint state transition */
                    MhpTxBPtr->State = TX_GETBUF_HOLD;
                }
                else if(MhpTxBPtr->HoldLoopTime > (word)0)                          /* not all loops done ? */
                {
                    MHP_DBG_TX_STATE(MhpTxBPtr, TX_GETBUF_HOLD);                    /* DebugPrint state transition */
                    MhpTxBPtr->State = TX_GETBUF_HOLD;

                    if(MhpTxBPtr->HoldLoopTime > MHP_CFG_TO_HOLD_RESEND)          /* decrement loop time to 0 */
                    {
                        MhpTxBPtr->HoldLoopTime -= MHP_CFG_TO_HOLD_RESEND;
                    }
                    else
                    {
                        MhpTxBPtr->HoldLoopTime = (word)0;
                    }
                }
                else                                                                    /* all loops done ! */
                {                                                                       /*  -> ready to go into backup state */
                    MostSetTimer(&MhpTxBPtr->TimerTx1, MhpTxBPtr->TimerTx1Back, MNS_FALSE);   /* Restore Timer  */

                    MHP_DBG_TX_STATE(MhpTxBPtr, MhpTxBPtr->StateBack);              /* DebugPrint state transition */
                    MhpTxBPtr->State = MhpTxBPtr->StateBack;                        /* Restore State */
                }

                MNS_REQUEST_HIGH_SET(MNSH_P_TX_STATE);                              /* Set Request Flag for calling MostServiceHigh() */
            }
            break;


        /*----------------------------------------------------- */
        case TX_WAIT:
        /*----------------------------------------------------- */
            if((word)0 == MostGetTimer(&MhpTxBPtr->TimerTx1))
            {
                MHP_DBG_TX_STATE(MhpTxBPtr, MhpTxBPtr->StateBack);                  /* DebugPrint state transition */
                MhpTxBPtr->State = MhpTxBPtr->StateBack;                            /* Restore State */
                MostSetTimer(&MhpTxBPtr->TimerTx1, MhpTxBPtr->TimerTx1Back, MNS_FALSE);   /* Restore Timer */

                MNS_REQUEST_HIGH_SET(MNSH_P_TX_STATE);                              /* Set Request Flag for calling MostServiceHigh() */
            }
            break;


        /*----------------------------------------------------- */
        case TX_NEG_ACK_REQ:
        case TX_BLOCK_REQ:
        case TX_NEXT_BLOCK:
        case TX_NEXT_SEG:
        /*----------------------------------------------------- */
            if(BUF_F_LOCK == MhpTxBPtr->Status)                                 /* Is segment ready to transmit? */
            {                                                                   /* If not, wait until flag is set */
                break;                                                          /* (actually only needed in case: TX_NEXT_SEG) */
            }


            if(MNS_FALSE != MhpTxApplyForToken(MhpTxBPtr))                      /* Apply for Tx Token */
            {
                switch(MhpTxBPtr->State)
                {
                    case TX_NEG_ACK_REQ:
                        if((word)0xFFFF != MhpTxBPtr->BlockCnt)                 /* No data frame sent after StartConnection */
                        {
                            MhpStartSameBlock((byte)0);                                         /* Same block again */
                            MostSetTimer(&MhpTxBPtr->TimerTx1, MHP_CFG_TO_TRANS, MNS_FALSE);    /* Load Timer (t_trans) */
                        }
                        else
                        {
                            MhpStartNewBlock((byte)0);                          /* Load parameters for transmission */
                        }
                        break;

                    case TX_BLOCK_REQ:
                        MhpStartSameBlock((byte)0);                                         /* Same block again */
                        MostSetTimer(&MhpTxBPtr->TimerTx1, MHP_CFG_TO_TRANS, MNS_FALSE);    /* Load Timer (t_trans) */
                        MhpSetTxAck((byte)0x00, MNS_FALSE);                                 /* Auto-Acknowledge of Null-Frame */
                        break;

                    case TX_NEXT_BLOCK:
                        MhpStartNewBlock((byte)1);                              /* Load parameters for transmission */
                        break;                                                  /* of further block of current segment */

                    case TX_NEXT_SEG:
                        MhpStartNewBlock((byte)0);                              /* Load parameters for transmission */
                        break;                                                  /* of first block of a new segment */

                    default:
                        break;
                }

                MHP_DBG_TX_STATE(MhpTxBPtr, TX_CHECK_FRAME);                    /* DebugPrint state transition */
                MhpTxBPtr->State = TX_CHECK_FRAME;                              /* Go to next state after TxToken has been grabbed */
                                                                                /* and init respective variables: */

                MhpTxDelayCounter = (word)(MhpTxBPtr->TxDelayMax + MHP_DELAY_NFRAME);   /* Init TX Delay Counter */

                if(MhpTxBPtr->Flags & MHP_FIRST_BLOCK)                              /* Initialisation only if first block */
                {
                    MostSetTimer(&MhpTxTimerG2, MHP_CFG_TO_TX_SPEED_RECOVERY, MNS_FALSE);   /* load timer to recover next higher speed */
                }

                MhpTxSpeedDir = MHP_SPEED_NEUTRAL;                                  /* set indicator to neutral position */
                MhpTxBPtr->Flags &= (word)(~MHP_FIRST_BLOCK);                       /* clear flag "MHP_FIRST_BLOCK" */

                MNS_REQUEST_HIGH_SET(MNSH_P_TX_STATE);                              /* Set Request Flag for calling MostServiceHigh()  */
            }
            else
            {
                if((word)0 == MostGetTimer(&MhpTxBPtr->TimerTx1))                     /* if Tx Token denied ! */
                {                                                                   /* -> Check for Timeout */
                    /*----------------------------------------- */
                    /* If Timeout supervision is not provided   */
                    /*----------------------------------------- */
                    if(NULL != MhpTxPrepTx(MhpTxBPtr))
                    {
                        MhpTxTel.Data[0] = MHP_CMD_HOLD_TX;                             /* HighCmd= F1 (Hold Tx) */
                        MhpTxTel.Data[1] = MHP_DUMMY_PID;                               /* Dummy */
                        MhpTxTel.Data[2] = MHP_HOLD_EVENT_BUSY_TX;                      /* Event */

                        #ifdef _OS81110_PCK_LLR
                        MhpTxTel.LowLevelRetries = MHP_RETRY_SHORT_FRAMES;
                        #endif
                        MhpSetLengthCtrl((word)3);

                        MhpSendTxTel();
                        MhpTxBPtr->BufRetry = MHP_BUF_RETRY;

                        MostSetTimer(&MhpTxBPtr->TimerTx1, MHP_CFG_TO_HOLD_RESEND, MNS_FALSE);  /* Load Timeout */

                        #ifdef MHP_DBG_TX
                        MhpDebugInfo[0] = MHP_DBG_CON_HOLD;                             /* Debug Event: Connection was set to Hold */
                        MhpDebugInfoTx(MhpTxBPtr, &MhpDebugInfo[0], (byte)1);
                        #endif
                    }
                    else                                                                /* no tx buffer available */
                    {
                        MhpTxCheckBufRetry(MhpTxBPtr);
                    }
                }
            }
            break;


        /*----------------------------------------------------- */
        default:
        /*----------------------------------------------------- */
            #ifdef MHP_24
            MhpSendMdmMhpUnknownTriggerState(MHP_MDM_TX_TRIGGER, MhpTxBPtr->State);     /* Unknown state ==> send debug message */
            #endif

            /* Error Handling */
            (void)MhpTxStatusWrap(MHP_TX_CLOSE, MhpTxBPtr);         /* -> Transmission failed */
            MhpSetTxBufferFree(MhpTxBPtr);                          /*    Set tx buffer free */

            break;

    }

}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpGoTxWaitAck()                                           */
/* Description : Called on state transition to state TX_WAIT_ACK.           */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T109
static void  MhpGoTxWaitAck(void) 
{
    if(MHP_BLOCK_ACK == MhpTxBPtr->Options)     /* Block-Acknowledge-Mode ? */
    {
        MostSetTimer(&MhpTxTimerG1, MHP_CFG_TO_TRANS_RETRY_DELAY_BA, MNS_FALSE);
    }
    else                                        /* Single-Frame-Acknowledge-Mode */
    {
        MostSetTimer(&MhpTxTimerG1, MHP_CFG_TO_TRANS_RETRY_DELAY_SFA, MNS_FALSE);
    }

    MHP_DBG_TX_STATE(MhpTxBPtr, TX_WAIT_ACK);   /* DebugPrint state transition */
    MhpTxBPtr->State = TX_WAIT_ACK;

    MNS_REQUEST_HIGH_SET(MNSH_P_TX_STATE);      /* Set Request Flag for calling MostServiceHigh() */
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpTxStatusWrap()                                          */
/* Description : Wrapper function for MhpTxStatus().                        */
/*               If status == MHP_TX_SUCCESS ==> Validation of parameters   */
/*               If silent connection termination ==> Send debug message    */
/* Parameter(s): status, const pointer at buffer entry                      */
/* Returns     : MNS_TRUE:  Next data chunk is ready to transmit            */
/*               MNS_FALSE: No further data chunk to transmit               */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T106
byte  MhpTxStatusWrap(byte status, const pTMhpTx tx_ptr)
{
    byte ret;

    switch(status)
    {
        case MHP_TX_SUCCESS:
            ret = MhpTxStatusAndValidate(status, tx_ptr);
            break;
        case MHP_TX_ERROR_NAC:
        case MHP_TX_ERROR_KILLED:
        case MHP_TX_ERROR_BUILDCON:
        case MHP_TX_ERROR_PRIO:
        case MHP_TX_ERROR_SCALE:
        case MHP_TX_ERROR_NOBUF:
        case MHP_TX_KILLED_TX:
        case MHP_TX_ERROR_NDF:
        case MHP_TX_LEN_ZERO:
        case MHP_TX_LEN_TOO_BIG:
        case MHP_TX_CLOSE:
            #ifdef MHP_26
            MhpSendMdmMhpSilentTermination(status, MHP_MDM_TX_STATUS);      /* Send debug message for states above */
            #endif
            /*lint -fallthrough */
        default:
            ret = MhpTxStatus(status, tx_ptr);
            break;
    }

    return(ret);
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpTxStatusAndValidate()                                   */
/* Description : In case of MHP_TX_SUCCESS this function is called. Before  */
/*               transmitting the next block, parameters (like e.g. Length) */
/*               are checked again. If parameters are invalid, connection   */
/*               will be closed.                                            */
/* Parameter(s): status, const pointer at buffer entry                      */
/* Returns     : MNS_TRUE:  Next data chunk is ready to transmit            */
/*               MNS_FALSE: No further data chunk to transmit               */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T107
static byte  MhpTxStatusAndValidate(byte status, const pTMhpTx tx_ptr)
{
    byte ret;
    byte res;
    byte err_status;

    ret = MhpTxStatus(status, tx_ptr);                  /* Callback application for success */

    if(MHP_STATUS_SEND_NEXT == ret)                                /* Further data chunk to transmit? */
    {
        MhpOpenCheckPriority(tx_ptr);                   /* Check priority field */
        res = MhpCheckPreCondSend(tx_ptr);              /* Check preconditions for sending */

        if(MHP_SEND_SUCCESS != res)                     /* Preconditions not ok ? */
        {
            ret        = MHP_STATUS_NO_DATA;            /* Return status "No further data chunk to send" */
            err_status = MHP_TX_KILLED_TX;              /* Default error status */

            if(MHP_SEND_TX_LEN_ZERO == res)             /* Data length is zero ? */
            {
                err_status = MHP_TX_LEN_ZERO;
            }
            else if(MHP_SEND_INV_TOTAL_PL == res)       /* Total Packet Length is too big ? */
            {
                err_status = MHP_TX_LEN_TOO_BIG;
            }
            else if(MHP_SEND_NCS_NOT_OK == res)         /* NWM.ConfigurationState is NotOK ? */
            {
                err_status = MHP_TX_CONFIG_NOTOK;
            }

            (void)MhpTxStatusWrap(err_status, tx_ptr);  /* Transmission failed, due to wrong parameter settings! */
            MhpSetTxBufferFree(tx_ptr);                 /* Error occurred => Set Tx buffer free */
        }
    }

    return(ret);
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpStartNewBlock()                                         */
/* Description : Prepares parameters for a new block transmission.          */
/*               (Transmitter only)                                         */
/* Parameter(s): ID: 0x00: first block of next segment                      */
/*                   0x01: further block of current segment                 */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T20
static void  MhpStartNewBlock(byte id) 
{
    word frame_remain;
    word num_data_frame;                        /* number of user data bytes per frame */
    byte frame_last;
    byte scale;
    byte frames_per_block;
    word num_blocks;

    MhpTxBPtrAct = MhpTxBPtr;                   /* Set Tx Token */
    MhpTxBPtr->Status |= BUF_F_ACTIVE;          /* Set Flag "Active" */

    MhpTxBPtr->BlockCnt++;                      /* increment block counter */
    MhpTxBPtr->BlockCnt &= (word)0x00FF;        /* BlockCnt 8bit */

    /*-------------------------- */
    /* Prepare Temp vars         */
    /*-------------------------- */
    num_data_frame = MhpTxBPtr->NumDataFrame;
    scale          = MhpTxBPtr->Scale;


    /*--------------------------- */
    /* Start first block:         */
    /*   - calculate FrameRemain  */
    /*   - set data ptr           */
    /*--------------------------- */
    if((byte)0 == id)                                                                                     /* first block */
    {
        if( ((word)0 != MhpTxBPtr->MaxBlkSize) && ((byte)0xFF != scale) )                       /* use complete receive buffer */
        {
            frames_per_block = (byte)( ((MhpTxBPtr->MaxBlkSize - (word)1) / num_data_frame       ) + (word)1 );
            num_blocks       = (word)( ((MhpTxBPtr->Length     - (word)1) / MhpTxBPtr->MaxBlkSize) + (word)1 );
            frame_remain     = ((word)((num_blocks - (word)1) * (word)frames_per_block) +      /* complete blocks */
                               ((MhpTxBPtr->Length - ((word)((num_blocks - (word)1) * MhpTxBPtr->MaxBlkSize) + (word)1)) / num_data_frame) + (word)1);
        }
        else
        {
            frames_per_block = (byte)0;
            frame_remain     = (word)( ((MhpTxBPtr->Length - (word)1) / num_data_frame) + (word)1 );    /* whole number of frames needed to transport all data */
                                                                                                        /* of this segment */
            MhpTxBPtr->MaxBlkSize = (word)0;                                        /* indicates that usage of complete receive buffer is not possible */
        }

        MhpTxBPtr->DataPtrBlock = MhpTxBPtr->DataPtr;                               /* Set data pointer at first byte of all  */
                                                                                    /* (first Byte of first block of this segment) */
    }

    /*--------------------------- */
    /* Start further block:       */
    /*   - remember FrameRemain   */
    /*   - set data ptr           */
    /*--------------------------- */
    else                                                            /* further block */
    {
        frame_remain            =  MhpTxBPtr->FrameRemain;          /* number of remaining frames in all further blocks */
                                                                    /* of this segment */
        frames_per_block        =  MhpTxBPtr->FramesPerBlock;       /* actual number of frames in current block */
        MhpTxBPtr->DataPtrBlock += MhpTxBPtr->CurrentBlockSize;     /* set data pointer to correct offset (first byte of next block) */
    }


    /*----------------------------- */
    /* Set FrameLast,  FrameRemain, */
    /* FrameCurrent and SegID       */
    /*----------------------------- */
    if((word)0 == MhpTxBPtr->MaxBlkSize)
    {
        if(frame_remain <= (word)scale)                 /* Last block ? -> no further block required */
        {
            frame_last                  = (byte)frame_remain;
            MhpTxBPtr->FrameRemain      = (word)0x0000;
            MhpTxBPtr->CurrentBlockSize = MhpTxBPtr->LengthRem;

            if((byte)0 == id)                                     /* first block (= last block) */
            {
                if( ((dword)0 != MhpTxBPtr->TotalPacketLengthInt) &&
                    ((dword)0 != MhpTxBPtr->TotalPacketSent) &&
                    ((MhpTxBPtr->TotalPacketSent + (dword)MhpTxBPtr->CurrentBlockSize) >= MhpTxBPtr->TotalPacketLengthInt) )
                {
                    MhpTxBPtr->SegIDTx = (byte)0x03;    /* --> last block of segmented transfer */
                }
                else
                {
                    MhpTxBPtr->SegIDTx = (byte)0x00;    /* --> single block transmission */
                }
            }
            else                                        /* not first block */
            {
                if((MhpTxBPtr->TotalPacketSent + MhpTxBPtr->CurrentBlockSize) >= MhpTxBPtr->TotalPacketLengthInt)
                {
                    MhpTxBPtr->SegIDTx = (byte)0x03;    /* --> last block of segmented transfer */
                }
                else
                {
                    MhpTxBPtr->SegIDTx = (byte)0x02;    /* --> another segment will follow */
                }
            }
        }
        else                                            /* not last block -> further block required */
        {
            frame_last                  = scale;
            MhpTxBPtr->FrameRemain      = (word)(frame_remain - (word)scale);
            MhpTxBPtr->CurrentBlockSize = (word)(frame_last * num_data_frame);

            if((byte)0 == id)                                     /* first block (but not last) */
            {
                if( ((dword)0 != MhpTxBPtr->TotalPacketLengthInt) && ((dword)0 != MhpTxBPtr->TotalPacketSent) )
                {
                    MhpTxBPtr->SegIDTx = (byte)0x02;    /* --> middle block of a segmented transmission */
                }
                else
                {
                    MhpTxBPtr->SegIDTx = (byte)0x01;    /* --> first block of a segmented transmission */
                }
            }
            else                                        /* not first block */
            {
                MhpTxBPtr->SegIDTx = (byte)0x02;        /* --> middle block of a segmented transmission */
            }
        }
    }
    else
    {
        if( ( ((MhpTxBPtr->TotalPacketLengthInt - MhpTxBPtr->TotalPacketSent) <= (dword)MhpTxBPtr->MaxBlkSize) &&
              ((MhpTxBPtr->TotalPacketLengthInt - MhpTxBPtr->TotalPacketSent) <= (dword)MhpTxBPtr->Length)
            ) ||
              ((MhpTxBPtr->TotalPacketLengthInt <= (dword)MhpTxBPtr->MaxBlkSize) && ((dword)MhpTxBPtr->Length == MhpTxBPtr->TotalPacketLengthInt)) )
        {
            frame_last                  = (byte)frame_remain;
            MhpTxBPtr->FrameRemain      = (word)0x0000;
            MhpTxBPtr->CurrentBlockSize = MhpTxBPtr->LengthRem;

            if((byte)0 == id)                                     /* first block (= last block) */
            {
                if( ((dword)0 != MhpTxBPtr->TotalPacketLengthInt) &&
                    ((dword)0 != MhpTxBPtr->TotalPacketSent) &&
                    ((MhpTxBPtr->TotalPacketSent + (dword)MhpTxBPtr->CurrentBlockSize) >= MhpTxBPtr->TotalPacketLengthInt) )
                {
                    MhpTxBPtr->SegIDTx = (byte)0x03;    /* --> last block of segmented transfer */
                }
                else
                {
                    MhpTxBPtr->SegIDTx = (byte)0x00;    /* --> single block transmission */
                }
            }
            else                                        /* not first block */
            {
                if((MhpTxBPtr->TotalPacketSent + (dword)MhpTxBPtr->CurrentBlockSize) >= MhpTxBPtr->TotalPacketLengthInt)
                {
                    MhpTxBPtr->SegIDTx = (byte)0x03;    /* --> last block of segmented transfer */
                }
                else
                {
                    MhpTxBPtr->SegIDTx = (byte)0x02;    /* --> another segment will follow */
                }
            }
        }
        else
        {
            if(MhpTxBPtr->LengthRem > MhpTxBPtr->MaxBlkSize)
            {
                frame_last                  = frames_per_block;
                MhpTxBPtr->FrameRemain      = (word)(frame_remain - (word)frames_per_block);
                MhpTxBPtr->CurrentBlockSize = MhpTxBPtr->MaxBlkSize;
            }
            else
            {
                frame_last                  = (byte)frame_remain;
                MhpTxBPtr->FrameRemain      = (word)0x0000;
                MhpTxBPtr->CurrentBlockSize = MhpTxBPtr->LengthRem;
            }

            if((byte)0 == id)                                     /* first block (but not last) */
            {
                if( ((dword)0 != MhpTxBPtr->TotalPacketLengthInt) && ((dword)0 != MhpTxBPtr->TotalPacketSent) )
                {
                    MhpTxBPtr->SegIDTx = (byte)0x02;    /* --> middle block of a segmented transmission */
                }
                else
                {
                    MhpTxBPtr->SegIDTx = (byte)0x01;    /* --> first block of a segmented transmission */
                }
            }
            else                                        /* not first block */
            {
                MhpTxBPtr->SegIDTx = (byte)0x02;        /* --> middle block of a segmented transmission */
            }
        }
    }

    MhpTxBPtr->FrameLast      = frame_last;
    MhpTxBPtr->FrameCurrent   = (byte)0x00;
    MhpTxBPtr->FramesPerBlock = frames_per_block;

    /*--------------------------- */
    /* Reset Retry + Timeout      */
    /*--------------------------- */
    MhpTxBPtr->Retry = TX_RETRY_TRANS;
    MostSetTimer(&MhpTxBPtr->TimerTx1, MHP_CFG_TO_TRANS, MNS_FALSE);
    MhpTxPending = MNS_FALSE;

    /*----------------------------------- */
    /* Prepare TxAck and TxFrId BitFields */
    /*----------------------------------- */
    MhpTxFrIdInit(MhpTxBPtr->FrameLast);
    MhpPrepareTxAckField();
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpStartSameBlock()                                        */
/* Description : Prepares parameters for a transmission of same block.      */
/*               (Transmitter only)                                         */
/* Parameter(s): ID: 0x00: all frames of same block again                   */
/*                   0x01: remaining frames of same block                   */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T21
static void  MhpStartSameBlock(byte id) 
{
    MhpTxBPtrAct = MhpTxBPtr;                                               /* Set Tx Token */
    MhpTxBPtr->FrameCurrent = (byte)0x00;

    if((byte)0 == id)                                                       /* all frames of same block again, */
    {                                                                       /* since block request */
        MhpTxFrIdInit(MhpTxBPtr->FrameLast);
        MhpPrepareTxAckField();
    }

}
#endif





/*
==============================================================================
    Tx-Ack-Bitfield - Section
==============================================================================
*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpPrepareTxAckField()                                     */
/* Description : Set all necessary bits for next block.                     */
/*                                                                          */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T22
static void  MhpPrepareTxAckField(void) 
{
    byte i;
    byte x;
    byte y;
    word bitmask;
    byte frame_last;

    frame_last = MhpTxBPtr->FrameLast;

    /*--------------------------- */
    /* Prepare TxAck BitField     */
    /*--------------------------- */
    x = (byte)(frame_last >> 4) & (byte)0x0F;       /* index of array of 16bit values */
    y = (byte)frame_last & (byte)0x0F;              /* bit position */

    for(i=(byte)0; i<x; i++)
    {
        MhpTxFrAck[i] = (word)0xFFFF;               /* set all bits */
    }

    for(i=x; i<(byte)16; i++)
    {
        MhpTxFrAck[i] = (word)0x0000;               /* clear all remaining lines */
    }

    bitmask = (word)0x0001;

    for(i=(byte)0; i<=y; i++)
    {
        MhpTxFrAck[x] |= bitmask;                   /* set bit = 1 within current index */
        bitmask = (word)(bitmask << 1);             /* next bit */
    }
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpCheckTxAckCurrent()                                     */
/* Description : Check if current frame was already acknowledged            */
/*               or if frame has to be transmitted.                         */
/*               Increment Frame Index until Frame was found or last        */
/*               frame has been detected.                                   */
/*                                                                          */
/* Parameter(s): none                                                       */
/* Returns     : status: MNS_TRUE:  Current Frame has to be transmitted,    */
/*                                  since not acknowledged                  */
/*                       MNS_FALSE: Current Frame has not to be transmitted,*/
/*                                  since already acknowledged              */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T23
static bool  MhpCheckTxAckCurrent(void) 
{
    byte x;
    byte y;
    byte i;
    word bitmask;
    byte frame_id;
    byte frame_last;
    bool ret;

    frame_last = MhpTxBPtr->FrameLast;

    do
    {
        frame_id = MhpTxBPtr->FrameCurrent;

        x = (byte)(frame_id >> 4) & (byte)0x0F;     /* index of array of 16bit values */
        y = (byte)frame_id & (byte)0x0F;            /* bit position */

        bitmask = (word)0x0001;
        for(i=(byte)0; i<y; i++)
        {
            bitmask = (word)(bitmask << 1);         /* prepare bitmask */
        }

        if(MhpTxFrAck[x] & bitmask)
        {
            ret = MNS_TRUE;
        }
        else
        {
            ret = MNS_FALSE;
            MhpTxBPtr->FrameCurrent = MhpTxFrGetNext();
        }

    } while((MNS_FALSE == ret) && (frame_id < frame_last));

    return(ret);
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpSetTxAck()                                              */
/* Description : Change flag of a certain FrameID, whenever frame has been  */
/*               acknowledged or requested.                                 */
/*                                                                          */
/* Parameter(s): FrameID, Command                                           */
/*                        (MNS_FALSE: Clear Flag, since acknowledged        */
/*                         MNS_TRUE: Set Flag, since frame requested)       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T24
static void  MhpSetTxAck(byte frame_id, bool cmd) 
{
    byte x;
    byte y;
    byte i;
    word bitmask;

    x = (byte)(frame_id >> 4) & (byte)0x0F;         /* index of array of 16bit values */
    y = (byte)frame_id & (byte)0x0F;                /* bit position  */

    bitmask = (word)0x0001;
    for(i=(byte)0; i<y; i++)
    {
        bitmask = (word)(bitmask << 1);             /* prepare bitmask */
    }

    if(MNS_FALSE != cmd)                            /* set respective bit in TxAck Bitfield, since frame requested */
    {
        MhpTxFrAck[x] |= bitmask;
    }
    else
    {
        MhpTxFrAck[x] &= (word)(~bitmask);          /* clear respective bit in TxAck Bitfield, since frame acknowledged */
    }
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpCheckTxAckAll()                                         */
/* Description : Check if all frames have been acknowledged.                */
/*                                                                          */
/* Parameter(s): none                                                       */
/*                                                                          */
/* Returns     : MNS_FALSE: At least one frame, which has not been          */
/*                          acknowledged                                    */
/*               MNS_TRUE:  All frames have been acknowledged               */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T25
static bool  MhpCheckTxAckAll(void) 
{
    byte i;
    bool ret;

    ret = MNS_TRUE;             /* Default: All frames acknowledged ! */

    for(i=(byte)0; i<(byte)16; i++)
    {
        if(MhpTxFrAck[i])
        {
            ret = MNS_FALSE;    /* Frame has not been acknowledged */
            break;
        }
    }

    return(ret);
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpCheckTxAck()                                            */
/* Description : Check if a certain frame has already been acknowledged.    */
/* Parameter(s): respective frame id                                        */
/* Returns     : MNS_TRUE:  frame has been acknowledged                     */
/*               MNS_FALSE: frame acknowledged not yet                      */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T28
static bool  MhpCheckTxAck(byte frame_id) 
{
    byte x;
    byte y;
    byte i;
    word bitmask;
    bool ret;

    x = (byte)(frame_id >> 4) & (byte)0x0F;     /* index of array of 16bit values */
    y = (byte)frame_id & (byte)0x0F;            /* bit position */

    bitmask = (word)0x0001;
    for(i=(byte)0; i<y; i++)
    {
        bitmask = (word)(bitmask << 1);         /* prepare bitmask */
    }

    if(MhpTxFrAck[x] & bitmask)
    {
        ret = MNS_FALSE;                        /* frame not yet acknowledged */
    }
    else
    {
        ret = MNS_TRUE;                         /* frame already acknowledged */
    }

    return(ret);
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpBlockAck()                                              */
/* Description : Clear all bits, since whole block acknowledged.            */
/*                                                                          */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T26
static void  MhpBlockAck(void) 
{
    byte i;

    for(i=(byte)0; i<(byte)16; i++)
    {
        MhpTxFrAck[i] = (word)0x0000;           /* clear all flags */
    }
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpPrepFrame()                                             */
/* Description : Prep current or requested frame.                           */
/*                                                                          */
/* Parameter(s): Pointer at const buffer entry, frame id                    */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T27
static void  MhpPrepFrame(pcTMhpTx buf_ptr, byte frame_id) 
{
    byte *DataPtrByte;
    byte *tgt_ptr;
    word data_size;
    word temp_size;


    MhpTxTel.TelType = MHP_TELTYPE_DATA;                                /* TelType (overwrite preloaded TelType=MHP_TELTYPE_CTRL) */
    MhpTxPending     = MNS_TRUE;

    /*-------------------------------------- */
    /*  Prepare Null-Frame                   */
    /*-------------------------------------- */
    if((byte)0 == frame_id)
    {
        MhpTxTel.Data[0]    = (byte)0x00;                               /* Current Frame */
        MhpTxTel.Data[1]    = buf_ptr->FrameLast;                       /* Last Frame */
        MhpTxTel.Data[2]    = buf_ptr->SegIDTx;                         /* segmentation id */
        MhpTxTel.Data[3]    = buf_ptr->Options;                         /* Options */
        MhpTxTel.Data[4]    = (byte)(buf_ptr->BlockCnt & (word)0x00FF); /* Current Block Counter */

        #ifdef _OS81110_PCK_LLR
        MhpTxTel.LowLevelRetries = MHP_RETRY_SHORT_FRAMES;
        #endif
        MhpSetLengthCtrl((word)5);

        MhpTxTel.DataPtr = MhpTxTel.Data + MHP_DF_OH_PCKT;
    }
    /*-------------------------------------- */
    /*  Prepare Data Frame                   */
    /*-------------------------------------- */
    else
    {
        tgt_ptr = &MhpTxTel.Data[0];                                /* Load target pointer for mem-copy */

        *tgt_ptr++ = frame_id;                                      /* Current Frame */
        *tgt_ptr++ = buf_ptr->FrameLast;                            /* Last Frame */

        data_size = buf_ptr->NumDataFrame;

        /* Load Source Ptr at first Byte in current frame */
        DataPtrByte = &buf_ptr->DataPtrBlock[(((word)frame_id - (word)1) * data_size)];

        if((word)0 == MhpTxBPtr->MaxBlkSize)
        {
            if(((word)0 == buf_ptr->FrameRemain) && (frame_id == buf_ptr->FrameLast))   /* Last frame in last block ? */
            {
                temp_size = (word)(buf_ptr->Length % data_size);            /* Re-calculate length of last frame in last block ! */

                /* Re-calculated length not zero? */
                if((word)0 != temp_size)
                {
                    data_size = temp_size;              /* Use re-calculated length */
                }
            }
        }
        else
        {
            if(frame_id == buf_ptr->FrameLast)          /* Last frame in current block ? */
            {
                /* Last frame in last block ? */
                if((word)0 == buf_ptr->FrameRemain)
                {
                    temp_size = (word)(buf_ptr->CurrentBlockSize % data_size);      /* Re-calculate length of last frame in last block ! */
                }
                else                                    /* Not the last block */
                {
                    temp_size = (word)(buf_ptr->MaxBlkSize % data_size);            /* Re-calculate length of last frame in current block ! */
                }

                /* Re-calculated length not zero? */
                if((word)0 != temp_size)
                {
                    data_size = temp_size;              /* Use re-calculated length */
                }
            }
        }

        #ifdef _OS81110_PCK_LLR
        if (data_size != buf_ptr->NumDataFrame)         /* Frame is not filled completely */
        {
            MhpTxTel.LowLevelRetries = MHP_RETRY_SHORT_FRAMES;
        }
        else
        {
            MhpTxTel.LowLevelRetries = MDP_DEFAULT_RETRY;
        }
        #endif

        MhpTxTel.DataPtr = DataPtrByte;

        MhpSetLengthData(MHP_DF_OH_PCKT, data_size);
    }
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpTxSpeedUp()                                             */
/* Description : Increment speed of transmission on sender side.            */
/*               Decrement property TxDelayMax.                             */
/* Parameter(s): pointer at entry in connection list                        */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T101
static void  MhpTxSpeedUp(pTMhpTx buf_ptr) 
{
    if(buf_ptr->TxDelayMax <= (word)1)
    {
        buf_ptr->TxDelayMax = (word)0;
    }
    else
    {
        buf_ptr->TxDelayMax = (word)((word)(buf_ptr->TxDelayMax + (word)1) >> 1);
    }

    #ifdef MHP_DELAY_RANGE_EN       /* Range of delay controlled by application ?    */
    if(buf_ptr->TxDelayMax < buf_ptr->MinDelay)
    {
        buf_ptr->TxDelayMax = buf_ptr->MinDelay;
    }
    #endif

    MhpTxSpeedDir = MHP_SPEED_INC;  /* speed is incremented */
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpTxSpeedDown()                                           */
/* Description : Decrement speed of transmission on sender side.            */
/*               Increment property TxDelayMax.                             */
/* Parameter(s): pointer at entry in connection list                        */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T119
static void  MhpTxSpeedDown(pTMhpTx buf_ptr) 
{
    if(buf_ptr->TxDelayMax)
    {
        buf_ptr->TxDelayMax = (word)(buf_ptr->TxDelayMax << 1);     /* x --> 2x */
    }
    else
    {
        buf_ptr->TxDelayMax = (word)2;                              /* 0 --> 2 */
    }

    /* Ckeck for maximum delay value: */
    #ifdef MHP_DELAY_RANGE_EN               /* Range of delay controlled by application ? */
    if(buf_ptr->TxDelayMax > buf_ptr->MaxDelay)
    {
        buf_ptr->TxDelayMax = buf_ptr->MaxDelay;
    }
    #else
    if(buf_ptr->TxDelayMax > MAX_DELAY_DEFAULT)
    {
        buf_ptr->TxDelayMax = MAX_DELAY_DEFAULT;
    }
    #endif

    MhpTxSpeedDir = MHP_SPEED_DEC;          /* speed is decremented */
}
#endif





/*
==============================================================================
    TX Connection Section (needed by Transmitter)
==============================================================================
*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpTxEndCon()                                          API */
/* Description : Close or kill existing Connection.                         */
/*                                                                          */
/* Parameter(s): Ptr at structure containing description of respective      */
/*               connection (TgtAddr.FBlockID.InstID.FuncID.OpType).        */
/*               Event, which indicates if the connection is canceled in an */
/*               ordinary way, or if the connection is killed, since data   */
/*               is not valid or any error occurred:                        */
/*                  0x00:  connection is closed in an ordinary way          */
/*                  0xFF:  connection is killed                             */
/*                                                                          */
/* Returns     : 0x00:  Ok, connection closed/killed                        */
/*               0x01:  respective connection not found                     */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T61
byte  MhpTxEndCon(pTMhpCon con_ptr, byte event) 
{
    pTMhpTx buf_ptr;
    byte    ret;

    RES_MHP_BEGIN

    ret = (byte)0x01;                                       /* respective connection not found */

    if(NULL != (buf_ptr = MhpSearchTxBufCon(con_ptr)))
    {
        if(!(buf_ptr->Status & BUF_F_IGN_HOLD))             /* Make sure that function is called only one time */
        {
            if(buf_ptr == MhpTxBPtrAct)
            {
                MhpTxBPtrAct = NULL;                        /* Release TX token */
            }

            if( (buf_ptr->Status & BUF_F_READY) &&          /* Call application only on pending transmission */
               !(buf_ptr->Status & BUF_F_RELOAD) )
            {
                (void)MhpTxStatusWrap(MHP_TX_KILLED_TX, buf_ptr);
            }

            if((byte)0x00 != event)
            {
                event = (byte)0xFF;
            }

            buf_ptr->EndEvent = event;

            buf_ptr->Status = (byte)(BUF_F_IGN_HOLD | BUF_F_RELOAD | BUF_F_READY | BUF_F_LOCK);
                                                            /* Flag: Ignore incoming HoldRx Telegrams */
            buf_ptr->Retry = TX_RETRY_END;

            MHP_DBG_TX_STATE(buf_ptr, TX_GETBUF_END);       /* DebugPrint state transition */
            buf_ptr->State = TX_GETBUF_END;

            MNS_REQUEST_HIGH_CALL(MNSH_P_TX_STATE);         /* Request for calling MostServiceHigh() */

            ret = (byte)0x00;                               /* Ok */
        }
    }

    RES_MHP_END
    return(ret);                                            /* respective connection not found */
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpTxHoldCon()                                         API */
/* Description : Push existing Connection in Wait-State.                    */
/*                                                                          */
/* Parameter(s): Ptr at structure containing description of respective      */
/*               connection (TgtAddr.FBlockID.InstID.FuncID.OpType)         */
/*                                                                          */
/* Returns     : 0x00:  Ok, connection is now in wait state                 */
/*               0x01:  respective connection not found                     */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T62
byte  MhpTxHoldCon(pTMhpCon con_ptr, byte event) 
{
    pTMhpTx buf_ptr;
    byte    state;
    byte    ret;

    RES_MHP_BEGIN

    ret = (byte)0x01;                                               /* respective connection not found */

    if(NULL != (buf_ptr = MhpSearchTxBufCon(con_ptr)))
    {
        state = buf_ptr->State;
        buf_ptr->HoldLoopTime = MHP_LOOP_HOLD_MAX_APPL_TIME;        /* Counter for Hold-Loop  */
        buf_ptr->HoldEvent    = event;

        if( (TX_GETBUF_HOLD != state) && (TX_HOLD != state) && (TX_WAIT != state) ) /* In Hold state not yet ? */
        {
            buf_ptr->TimerTx1Back = MostGetTimer(&buf_ptr->TimerTx1);       /* Store Timer */

            if(buf_ptr == MhpTxBPtrAct)
            {
                MhpTxBPtrAct = NULL;                                /* release tx token */
                buf_ptr->StateBack = TX_NEG_ACK_REQ;                /* backup state */
            }
            else
            {
                buf_ptr->StateBack = state;                         /* backup state */
            }
        }

        MHP_DBG_TX_STATE(buf_ptr, TX_GETBUF_HOLD);                  /* DebugPrint state transition */
        buf_ptr->State = TX_GETBUF_HOLD;
        MNS_REQUEST_HIGH_CALL(MNSH_P_TX_STATE);                     /* Request for calling MostServiceHigh()  */

        ret = (byte)0x00;                                           /* Ok */
    }

    RES_MHP_END
    return(ret);
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpTxContinueCon()                                     API */
/* Description : Continue connection being in Wait-State.                   */
/*                                                                          */
/* Parameter(s): Ptr at structure containing description of respective      */
/*               connection (TgtAddr.FBlockID.InstID.FuncID.OpType)         */
/*                                                                          */
/* Returns     : 0x00:  Ok, connection continued                            */
/*               0x01:  respective connection not found                     */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T63
byte  MhpTxContinueCon(pTMhpCon con_ptr) 
{
    pTMhpTx buf_ptr;
    byte    ret;

    RES_MHP_BEGIN

    ret = (byte)0x01;                                       /* respective connection not found */

    if(NULL != (buf_ptr = MhpSearchTxBufCon(con_ptr)))
    {
        buf_ptr->HoldLoopTime = (word)0;
        MostClearTimer(&buf_ptr->TimerTx1);                 /* Clear Hold Loop */
        MNS_REQUEST_HIGH_CALL(MNSH_P_TX_TIMEOUT);           /* Request for calling MostServiceHigh()  */

        ret = (byte)0x00;                                   /* Ok */
    }

    RES_MHP_END
    return(ret);
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpTxEndAllCon()                                       API */
/* Description : Kill all existing connections.                             */
/*                                                                          */
/* Parameter(s): none                                                       */
/*                                                                          */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T64
void  MhpTxEndAllCon(void) 
{
    pTMhpTx buf_ptr;

    buf_ptr = MhpTxBuf;

    do
    {
        RES_MHP_BEGIN
        if(BUF_FREE != buf_ptr->Status)
        {
            if(!(buf_ptr->Status & BUF_F_IGN_HOLD))         /* Make sure that function is called only one time */
            {
                if(buf_ptr == MhpTxBPtrAct)
                {
                    MhpTxBPtrAct = NULL;                    /* Release TX token */
                }

                if(  (buf_ptr->Status & BUF_F_READY) &&     /* Call application only on pending transmission */
                    !(buf_ptr->Status & BUF_F_RELOAD) )
                {
                    (void)MhpTxStatusWrap(MHP_TX_CONFIG_NOTOK, buf_ptr);
                }

                buf_ptr->EndEvent = (byte)0xFF;

                buf_ptr->Status = (byte)(BUF_F_IGN_HOLD | BUF_F_RELOAD | BUF_F_READY | BUF_F_LOCK);
                                                            /* Flag: Ignore incoming HoldRx Telegrams */
                buf_ptr->Retry = TX_RETRY_END;

                MHP_DBG_TX_STATE(buf_ptr, TX_GETBUF_END);   /* DebugPrint state transition */
                buf_ptr->State = TX_GETBUF_END;

                MNS_REQUEST_HIGH_CALL(MNSH_P_TX_STATE);     /* Request for calling MostServiceHigh() */
            }
        }
        RES_MHP_END
    } while(++buf_ptr < &MhpTxBuf[MAX_MHP_TX]);

    return;                                                 /* respective connection not found */
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpSetConfigStateFilterStatus()                            */
/* Description : sets the status of the ConfigState Based Message Filter.   */
/*                                                                          */
/* Parameter(s): on  new status                                             */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T65
void  MhpSetConfigStateFilterStatus(byte on) 
{
    if(MNS_ON == on)
    {
        MhpConfigStateFilterOn = MNS_TRUE;
        MhpTxEndAllCon();               /* terminate all active connections */
    }
    else
    {
        MhpConfigStateFilterOn = MNS_FALSE;
    }
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpSearchTxBufCon()                                        */
/* Description : Search Tx bufferline for a certain connection.             */
/*               The connection is described by                             */
/*               "TgtAddr.FBlockID.InstID.FuncID.OpType".                   */
/*                                                                          */
/* Parameter(s): Ptr at const structure containing description of           */
/*               respective connection                                      */
/*               (TgtAddr.FBlockID.InstID.FuncID.OpType)                    */
/*                                                                          */
/* Returns     : ptr at Tx bufferline                                       */
/*               NULL: error, corresponding bufferline not found            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T60
pTMhpTx  MhpSearchTxBufCon(pcTMhpCon con_ptr) 
{
    pTMhpTx buf_ptr;
    pTMhpTx ret;

    buf_ptr = MhpTxBuf;
    ret     = NULL;

    do
    {
        if( (buf_ptr->Tgt_Adr   == con_ptr->Addr) &&
            (buf_ptr->FBlock_ID == con_ptr->FBlock_ID) &&
            (buf_ptr->Inst_ID   == con_ptr->Inst_ID) &&
            (buf_ptr->Func_ID   == con_ptr->Func_ID) &&
            (buf_ptr->Operation == con_ptr->Operation) )
        {
            ret = buf_ptr;
            break;
        }

    } while(++buf_ptr < &MhpTxBuf[MAX_MHP_TX]);

    return(ret);
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpTxApplyForToken()                                       */
/* Description : Look for a free Tx Token.                                  */
/*                                                                          */
/* Parameter(s): Ptr at const tx structure                                  */
/*                                                                          */
/* Returns     : MNS_FALSE: Apply rejected                                  */
/*               MNS_TRUE:  Token successfully allocated                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T70
static bool  MhpTxApplyForToken(pcTMhpTx buf_ptr_waiting) 
{
    pTMhpTx buf_ptr;
    byte    prio_waiting;
    byte    state;
    bool    ret;

    if(MhpTxBPtrAct == buf_ptr_waiting)
    {
        ret = MNS_TRUE;                             /* Ok, since Token already grabbed */
    }
    else if(NULL != MhpTxBPtrAct)
    {
        ret = MNS_FALSE;                            /* Token used by another connection */
    }
    else
    {
        ret = MNS_TRUE;                             /* Ok, no higher priority waiting for Tx Token */
                                                    /* order of serviced connections depends on priority: */
        prio_waiting = buf_ptr_waiting->Priority;   /* priority of applying connection */

        buf_ptr = MhpTxBuf;
        do
        {
            if(buf_ptr->Priority > prio_waiting)
            {
                state = buf_ptr->State;
                if( (TX_NEXT_BLOCK  == state) ||
                    (TX_NEXT_SEG    == state) ||
                    (TX_BLOCK_REQ   == state) ||
                    (TX_NEG_ACK_REQ == state) )
                {
                    ret = MNS_FALSE;                /* Buffer entry with higher priority is also waiting for Tx Token */
                    break;
                }
            }
        } while(++buf_ptr < &MhpTxBuf[MAX_MHP_TX]);
    }

    return(ret);
}
#endif





/*
==============================================================================
    Section for Request Buffer (needed by Transmitter)
==============================================================================
*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpTxFrIdInit()                                            */
/* Description : Initialize the tx frame id list.                           */
/*                                                                          */
/* Parameter(s): last frame id of block                                     */
/*                                                                          */
/* Returns     : none                                                       */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T90
static void MhpTxFrIdInit(byte frame_last)
{
    byte i;
    byte x;
    byte y;
    word bitmask;

    /*--------------------------- */
    /* Prepare TxFrId BitField    */
    /*--------------------------- */
    x = (byte)(frame_last >> 4) & (byte)0x0F;       /* index of array of 16bit values */
    y = (byte)frame_last & (byte)0x0F;              /* bit position */

    for(i=(byte)0; i<x; i++)
    {
        MhpTxFrId[i] = (word)0xFFFF;                /* set all bits */
    }

    for(i=x; i<(byte)16; i++)
    {
        MhpTxFrId[i] = (word)0x0000;                /* clear all remaining lines */
    }

    bitmask = (word)0x0001;

    for(i=(byte)0; i<=y; i++)
    {
        MhpTxFrId[x] |= bitmask;                    /* set bit = 1 within current index */
        bitmask = (word)(bitmask << 1);             /* next bit */
    }
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpTxFrIdState()                                           */
/* Description : Set state of passed frame id.                              */
/*                                                                          */
/* Parameter(s): frame id                                                   */
/*               state    MNS_TRUE:  Set bit in bitmask                     */
/*                        MNS_FALSE: Reset bit in bitmask                   */
/*                                                                          */
/* Returns     : none                                                       */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T116
static void MhpTxFrIdState(byte frame_id, bool state)
{
    byte x;
    byte y;
    byte i;
    word bitmask;

    x = (byte)(frame_id >> 4) & (byte)0x0F;         /* index of array of 16bit values */
    y = (byte)frame_id & (byte)0x0F;                /* bit position  */

    bitmask = (word)0x0001;
    for(i=(byte)0; i<y; i++)
    {
        bitmask = (word)(bitmask << 1);             /* prepare bitmask */
    }

    if(MNS_FALSE != state)                         /* set respective bit in TxFrId Bitfield */
    {
        MhpTxFrId[x] |= bitmask;
    }
    else
    {
        MhpTxFrId[x] &= (word)(~bitmask);           /* clear respective bit in TxFrId Bitfield */
    }
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpTxFrGetNext()                                           */
/* Description : Get next frame id.                                         */
/*                                                                          */
/* Parameter(s): none                                                       */
/*                                                                          */
/* Returns     : Next frame id                                              */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T117
static byte MhpTxFrGetNext(void)
{
    byte i;
    byte j;
    bool check;
    byte ret;

    ret   = (byte)0x00;
    check = MNS_TRUE;

    for(i=(byte)0; ((i<(byte)16) && (MNS_FALSE != check)); i++)
    {
        if((word)0x0000 != MhpTxFrId[i])                                    /* Is bit set within current index? */
        {
            for(j=(byte)0; ((j<(byte)16) && (MNS_FALSE != check)); j++)     /* Find bit position */
            {
                if((word)((word)1 << j) == (MhpTxFrId[i] & (word)((word)1 << j)))
                {
                    ret = (byte)(i << 4) + j;
                    check = MNS_FALSE;
                }
            }
        }
    }

    return(ret);
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpTxFrAllSent()                                           */
/* Description : Have all frames been sent?                                 */
/*                                                                          */
/* Parameter(s): none                                                       */
/*                                                                          */
/* Returns     : MNS_TRUE  - All frames have been sent                      */
/*               MNS_FALSE - Not all frames have been sent                  */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T118
static bool MhpTxFrAllSent(void)
{
    byte i;
    bool ret;

    ret = MNS_TRUE;

    for(i=(byte)0; i<(byte)16; i++)
    {
        if((word)0x0000 != MhpTxFrId[i])        /* Is bit set within current index? */
        {
            ret = MNS_FALSE;
            break;
        }
    }

    return(ret);
}
#endif





/*
==============================================================================
    Telegram Section (needed by Transmitter and Receiver)  - Outgoing Section
==============================================================================
*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpTxPrepTx()                                              */
/* Description : Prepares header in the Mhp Tx Telegram Buffer.             */
/*               The source fields are placed in the                        */
/*               Mhp Tx Connection Buffer.                                  */
/*               (Transmitter only)                                         */
/*                                                                          */
/* Parameter(s): Ptr at Mhp Tx Connection Buffer                            */
/*                                                                          */
/* Returns     : NULL:      No Tx message possible, since                   */
/*                          Mhp Tx Telegram Buffer is locked                */
/*               0xFFFF:    Header was prepared                             */
/*                          Data can now be filled.                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T40
void *  MhpTxPrepTx(pTMhpTx buf_ptr) 
{
    word tgt_adr;
    word func_id_op;
    void *ret;

    if(BUF_FREE != MhpTxTel.Status)
    {
        ret = NULL;                             /* return NULL since Mhp Tx Telegram buffer is busy */
    }
    else
    {
        /*lint -save -e923 cast from int to pointer [MISRA 2004 Rule 11.3]*/
        ret = (void *)0xFFFF;
        /*lint -restore -e923*/

        MhpTxTel.Status = BUF_F_LOCK | BUF_F_MHPTX;
        MhpTxTel.BufPtr = (void *)buf_ptr;
        tgt_adr         = buf_ptr->Tgt_Adr;

        if((word)0xFFFF == tgt_adr)             /* still unresolved target address ? */
        {
            tgt_adr = MSG_TGT_DEBUG;            /* send dummy frames to debug address (0x0FF0), since 0xFFFF could */
        }                                       /* be used in the system as broadcast address by other protocols on the asynchronous channel */

        func_id_op = (word)(buf_ptr->Func_ID << 4) | (word)buf_ptr->Operation;

        MhpTxTel.Addr_H         = HB(tgt_adr);                  /* Target Address */
        MhpTxTel.Addr_L         = LB(tgt_adr);
        MhpTxTel.UserHandle     = (byte)0x00;                   /* Dummy */
        MhpTxTel.TelType        = MHP_TELTYPE_CTRL;             /* Please note: TelType is preloaded and must be overwritten, */
                                                                /*              whenever sending data frames */
        MhpTxTel.FBlock_ID      = buf_ptr -> FBlock_ID;         /* FBlockID */
        MhpTxTel.Inst_ID        = buf_ptr -> Inst_ID;           /* InstID */
        MhpTxTel.Func_ID_H      = HB(func_id_op);               /* HB(FktID) */
        MhpTxTel.Func_ID_L_Op   = LB(func_id_op);               /* LB(FktID) | OPType */

        MhpTxTel.Priority       = MHP_TX_PRIO_ASYNC;
    }

    return(ret);
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpRxPrepTx()                                              */
/* Description : Prepares header in the Mhp Tx Telegram Buffer.             */
/*               The source fields are placed in the                        */
/*               Mhp Rx Connection Buffer.                                  */
/*               (Receiver only)                                            */
/*                                                                          */
/* Parameter(s): Ptr at Mhp Rx Connection Buffer                            */
/*                                                                          */
/* Returns     : NULL:      No Tx message possible, since                   */
/*                          Mhp Tx Telegram Buffer is locked                */
/*               0xFFFF:    Header was prepared                             */
/*                          Data can now be filled.                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R40
static void *  MhpRxPrepTx(pTMhpRx buf_ptr) 
{
    word src_adr;
    word func_id_op;
    void *ret;

    if(BUF_FREE != MhpTxTel.Status)
    {
        ret = NULL;                             /* return NULL since Mhp Tx Telegram buffer is busy */
    }
    else
    {
        /*lint -save -e923 cast from int to pointer [MISRA 2004 Rule 11.3]*/
        ret = (void *)0xFFFF;
        /*lint -restore -e923*/

        MhpTxTel.Status = BUF_F_LOCK;           /* BUF_F_MHPRX is not used, since zero ! */
        MhpTxTel.BufPtr = (void *)buf_ptr;

        src_adr    = buf_ptr -> Src_Adr;
        func_id_op = (word)(buf_ptr->Func_ID << 4) | (word)buf_ptr->Operation;

        MhpTxTel.Addr_H         = HB(src_adr);                  /* Source Address -> Target Address of Tx Message */
        MhpTxTel.Addr_L         = LB(src_adr);
        MhpTxTel.UserHandle     = (byte)0x00;                   /* Dummy */
        MhpTxTel.TelType        = MHP_TELTYPE_CTRL;
        MhpTxTel.FBlock_ID      = buf_ptr -> FBlock_ID;         /* FBlockID */
        MhpTxTel.Inst_ID        = buf_ptr -> Inst_ID;           /* InstID */
        MhpTxTel.Func_ID_H      = HB(func_id_op);               /* HB(FktID) */
        MhpTxTel.Func_ID_L_Op   = LB(func_id_op);               /* LB(FktID) | OPType */

        MhpTxTel.Priority = MHP_RX_PRIO_ASYNC;
    }

    return(ret);
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpSendTxTel()                                             */
/* Description : Send Tx Telegram Buffer to respective Service (CMS/ADS).   */
/*                                                                          */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_11
void  MhpSendTxTel(void) 
{
    if(BUF_FREE == MhpTxTel.Status)
    {
        return;
    }

    if((word)0 != MostGetTimer(&MhpTimerTxTelBuf))         /* timeout not yet */
    {
        return;
    }

    if((byte)0 == MhpTxTel.RetryCnt)                              /* try to terminate, if last retry failed */
    {
        #if (defined MHP_TX_MIN) || (defined PMHT_MIN)
        if(BUF_F_MHPTX == (MhpTxTel.Status & POS_BUF_F_MHPTXRX))    /* Connection of TX section: */
        {
            ((pTMhpTx)(MhpTxTel.BufPtr))->BufRetry = (word)0;
            MhpTxCheckBufRetry((pTMhpTx)(MhpTxTel.BufPtr));         /* terminate retry session !!! */
        }
        else                                                        /* Connection of RX section: */
        #endif
        {
            #ifdef MHP_RX_MIN
            ((pTMhpRx)(MhpTxTel.BufPtr))->BufRetry = (word)0;
            MhpRxCheckBufRetry((pTMhpRx)(MhpTxTel.BufPtr));         /* terminate retry session !!! */
            #endif
        }

        MhpTxTel.Status = BUF_FREE;                                 /* set telegram buffer free */
        MhpTxTel.RetryCnt = MHP_TX_TEL_RETRY_MAX;
        return;
    }

    #ifdef MHP_14
    MhpSendTxTelAds();                                  /* Interface: ADS */
    #endif

    #ifdef MHP_DBG_TXTEL
    MhpDebugInfoTxTel(MhpTxTel.Status, &MhpTxTel);      /* Status  = 0 ? -> Xmit done */
    #endif                                              /* Status != 0 ? -> Xmit denied, retry required */

    if(BUF_FREE != MhpTxTel.Status)
    {
        MostSetTimer(&MhpTimerTxTelBuf, MHP_CFG_TO_BUF_RETRY, MNS_FALSE);
        MhpTxTel.RetryCnt--;
        /* No request flag is set, since re-trigger is forced by timer 'MhpTimerTxTelBuf' */
    }
    else
    {
        MhpTxTel.RetryCnt = MHP_TX_TEL_RETRY_MAX;
    }
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpSendTxTelAds()                                          */
/* Description : Send Tx Telegram Buffer via ADS of Layer I.                */
/*                                                                          */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_14
void  MhpSendTxTelAds(void) 
{
    byte     *tgt_ptr;
    byte     *src_ptr;
    word     length;
    pTDataTx tx_ptr;

	// Bug : Errata sheet 3.0.X-6 - 0300XX-015
    /*length = MhpTxTel.DataLength; REMOVED LINE */
    length = MhpTxTel.TelLen; /* NEW LINE */	

    tx_ptr = DataGetTxPtrExt((word)(length + MHP_OVERHEAD_PCKT));

    if(NULL != tx_ptr)
    {
        MhpTxTel.Tel_ID_Len_H = (MhpTxTel.TelType | (byte)(HB(MhpTxTel.TelLen) & (byte)0x0F));
        MhpTxTel.Tel_Len_L    = (byte) LB(MhpTxTel.TelLen);

        /* Copy header */
        tx_ptr->Tgt_Adr_H     = MhpTxTel.Addr_H;
        tx_ptr->Tgt_Adr_L     = MhpTxTel.Addr_L;
        tx_ptr->Data[0]       = MhpTxTel.FBlock_ID;
        tx_ptr->Data[1]       = MhpTxTel.Inst_ID;
        tx_ptr->Data[2]       = MhpTxTel.Func_ID_H;
        tx_ptr->Data[3]       = MhpTxTel.Func_ID_L_Op;
        tx_ptr->Data[4]       = MhpTxTel.Tel_ID_Len_H;
        tx_ptr->Data[5]       = MhpTxTel.Tel_Len_L;
        #ifdef _OS81110_PCK_LLR
        tx_ptr->Retry         = MhpTxTel.LowLevelRetries;
        #endif
        tgt_ptr = &tx_ptr->Data[6];

		// Bug : Errata sheet 3.0.X-6 - 0300XX-015
		/* START OF REMOVED LINES
        src_ptr = MhpTxTel.Data;
        if((word)0 != MhpTxTel.DataPtrLength)
        {
            MBM_SET_ADD_PAYLOAD(tx_ptr, MhpTxTel.DataPtr, MhpTxTel.DataPtrLength);
        }
		END OF REMOVED LINES */
		/* START OF NEW LINES */
        if (MHP_TELTYPE_DATA == MhpTxTel.TelType)
        {
            if((word)0 != MhpTxTel.TelLen)
            {
                tx_ptr->Data[6] = MhpTxTel.Data[0];
                tx_ptr->Data[7] = MhpTxTel.Data[1];
                tgt_ptr = &tx_ptr->Data[8];
                length -= MHP_DF_OH_PCKT;
            }
            src_ptr = MhpTxTel.DataPtr;
        }
        else
        {
            src_ptr = MhpTxTel.Data;
        }
		/* END OF NEW LINES */
        MHP_MEM_CPY(tgt_ptr, src_ptr, length);  /* Copy user payload */

        DataSend(tx_ptr);                       /* Send data packet */

        MhpTxTel.Status = BUF_FREE;             /* Unlock Buffer */
    }
}
#endif





/*
==============================================================================
    Telegram Section (needed by Transmitter and Receiver)  - Incoming Section
==============================================================================
*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpRxTriggerAds()                                          */
/* Description : Receives incoming message from Asyncronous Data.           */
/*               Transmission Service                                       */
/*                                                                          */
/* Parameter(s): Ptr at const ADS Rx Buf                                    */
/* Returns     : reaction:  0x00: retrigger forced                          */
/*                          0x01: clear ads message                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_21
byte  MhpRxTriggerAds(pcTDataRx ptr_ads) 
{
    byte *src_ptr;
    byte *tgt_ptr;

    if(BUF_FREE != MhpRxTel.Status)
    {
        return((byte)0);
    }

    MhpRxTel.Status  = BUF_F_LOCK;                          /* Lock Rx Buffer */

    /* Copy header */
    MhpRxTel.Src_Addr_H   = ptr_ads->Src_Adr_H;
    MhpRxTel.Src_Addr_L   = ptr_ads->Src_Adr_L;
    MhpRxTel.FBlock_ID    = ptr_ads->Data[0];
    MhpRxTel.Inst_ID      = ptr_ads->Data[1];
    MhpRxTel.Func_ID_H    = ptr_ads->Data[2];
    MhpRxTel.Func_ID_L_Op = ptr_ads->Data[3];
    MhpRxTel.Tel_ID_Len_H = ptr_ads->Data[4];
    MhpRxTel.Tel_Len_L    = ptr_ads->Data[5];

    src_ptr = &ptr_ads->Data[6];
    tgt_ptr = MhpRxTel.Data;

    MhpRxTel.TelType = (byte)(MhpRxTel.Tel_ID_Len_H & (byte)0xF0);                                              /* TelID */
    MhpRxTel.TelLen  = ((word)((word)MhpRxTel.Tel_ID_Len_H << 8) & (word)0x0F00) + (word)MhpRxTel.Tel_Len_L;    /* Tel Length */

    #ifdef MHP_DBG_RXTEL
    MhpDebugInfoRxTel(&MhpRxTel);                           /* Call debug function to monitor received telegram */
    #endif

    /* check if packet size on MOST bus corresponds to protocol payload size */
    if ( ((ptr_ads->Length - MHP_HDR_SIZE_ADS) <   MhpRxTel.TelLen) ||
         ((ptr_ads->Length - MHP_HDR_SIZE_ADS) >= (MhpRxTel.TelLen + (word)4)) )
    {
        MhpRxTel.Status  = BUF_FREE;                        /* Unlock Rx Buffer */
        return((byte)1);
    }

    if(MhpRxTel.TelLen > MHP_NUM_DATA_RX)                   /* Telegram length bigger than buffer size ? */
    {
        MhpRxTel.Status = BUF_FREE;                         /* Unlock Rx Buffer */
        return((byte)1);
    }

    /* MHP control frame? */
    if(MHP_TELTYPE_CTRL == MhpRxTel.TelType)
    {
        MHP_MEM_CPY(tgt_ptr, src_ptr, MhpRxTel.TelLen);     /* Copy receive data fields Data[0]..Data[n-1] */
    }

    MhpRxInterpreter(ptr_ads);

    MhpRxTel.Status = BUF_FREE;                             /* Unlock Rx Buffer */
    return((byte)1);
}
#endif





/*
==============================================================================
    Interpreter Section (needed by Transmitter and Receiver)
==============================================================================
*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpRxInterpreter()                                         */
/* Description : Checks incoming messages from CMS and ADS                  */
/*               (flow control protocol only)                               */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_22
static void  MhpRxInterpreter(pcTDataRx ptr_ads) 
{
    RES_MHP_BEGIN

    #ifndef MHP_RX_MIN
    (void)ptr_ads;                          /* To avoid PC-Lint warning */
    #endif

    switch(MhpRxTel.TelType)
    {
        /*----------------------------------- */
        /*      Data Protocol                 */
        /*     (Receiver only)                */
        /*----------------------------------- */
        #ifdef MHP_RX_MIN
        case MHP_TELTYPE_DATA:
            MhpDataFrameReceive(ptr_ads);
            break;
        #endif

        /*----------------------------------- */
        /*      Flow Control Protocol         */
        /*----------------------------------- */
        case MHP_TELTYPE_CTRL:
            switch(MhpRxTel.Data[0])        /* MostHigh Cmd */
            {
                /*-------------------------------------------- */
                /* Transmitter only                            */
                /*-------------------------------------------- */
                #if (defined MHP_TX_MIN) || (defined PMHT_MIN)
                case MHP_CMD_STA_CON:
                    MhpStartConnection();
                    break;

                case MHP_CMD_ACK:
                    MhpAcknowledge();
                    break;

                case MHP_CMD_REQUEST:
                    MhpRequest();
                    break;

                case MHP_CMD_REQUEST_MULTI:
                    MhpRequestMulti();
                    break;

                case MHP_CMD_HOLD_RX:
                    MhpHoldRx();
                    break;

                case MHP_CMD_END_RX:
                    MhpEndRx();
                    break;
                #endif


                /*-------------------------------------------- */
                /* Receiver only                               */
                /*-------------------------------------------- */
                #ifdef MHP_RX_MIN
                case MHP_CMD_REQ_CON:
                    MhpRequestConnection();
                    break;

                case MHP_CMD_READY:
                    MhpReadyForData();
                    break;

                case MHP_CMD_HOLD_TX:
                    MhpHoldTx();
                    break;

                case MHP_CMD_END_TX:
                    MhpEndTx();
                    break;
                #endif
                default:                                        /* unknown MOST High Cmd */
                    break;
            }
            break;

        default:                                                /* unknown TelID */
            break;
    }

    RES_MHP_END
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpGetTxBufPtr()                                           */
/* Description : Search corresponding Tx buffer, which refers to received   */
/*               MOST High Protocol Telegram (flow control only).           */
/*                                                                          */
/* Parameter(s): none                                                       */
/* Returns     : Ptr at corresponding Tx buffer                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T30
pTMhpTx  MhpGetTxBufPtr(void) 
{
    pTMhpTx buf_ptr;
    pTMhpTx ret;
    word    src_addr;
    word    func_rx;

    buf_ptr = MhpTxBuf;
    ret     = NULL;

    do
    {
        if(BUF_FREE != buf_ptr->Status)
        {
            src_addr = (word)((word)MhpRxTel.Src_Addr_H << 8) + (word)MhpRxTel.Src_Addr_L;

            if( (buf_ptr->Tgt_Adr   == src_addr) &&
                (buf_ptr->FBlock_ID == MhpRxTel.FBlock_ID) &&
                (buf_ptr->Inst_ID   == MhpRxTel.Inst_ID) )
            {
                func_rx = ((word)((word)MhpRxTel.Func_ID_H << 4) + ((word)((word)MhpRxTel.Func_ID_L_Op >> 4) & (word)0x000F)) & (word)0x0FFF;
                if((buf_ptr->Func_ID == func_rx) && ((MhpRxTel.Func_ID_L_Op & (byte)0x0F) == buf_ptr->Operation))
                {
                    ret = buf_ptr;
                    break;
                }
            }
        }

    } while(++buf_ptr < &MhpTxBuf[MAX_MHP_TX]);

    return(ret);
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpGetRxBufPtr()                                           */
/* Description : Search corresponding Rx buffer, which refers to received   */
/*               MOST High Protocol Telegram.                               */
/*                                                                          */
/* Parameter(s): none                                                       */
/* Returns     : Ptr at corresponding Rx buffer                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R30
static pTMhpRx  MhpGetRxBufPtr(void) 
{
    pTMhpRx buf_ptr;
    pTMhpRx ret;
    word    src_addr;
    word    func_rx;

    buf_ptr = MhpRxBuf;
    ret = NULL;

    do
    {
        if(BUF_FREE != buf_ptr->Status)
        {
            src_addr = (word)((word)MhpRxTel.Src_Addr_H << 8) + (word)MhpRxTel.Src_Addr_L;

            if( (buf_ptr->Src_Adr   == src_addr) &&
                (buf_ptr->FBlock_ID == MhpRxTel.FBlock_ID)  &&
                (buf_ptr->Inst_ID   == MhpRxTel.Inst_ID) )
            {
                func_rx = ((word)((word)MhpRxTel.Func_ID_H << 4) + ((word)((word)MhpRxTel.Func_ID_L_Op >> 4) & (word)0x000F)) & (word)0x0FFF;
                if((buf_ptr->Func_ID == func_rx) && ((MhpRxTel.Func_ID_L_Op & (byte)0x0F) == buf_ptr->Operation))
                {
                    ret = buf_ptr;
                    break;
                }
            }
        }

    } while(++buf_ptr < &MhpRxBuf[MAX_MHP_RX]);

    return(ret);
}
#endif





/*
==============================================================================
    Functions, which are called by Mhp Command Interpreter.
    The following functions are needed by Transmitter only.
==============================================================================
*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpStartConnection()                                       */
/* Description : Service Function for received command: F2                  */
/*               (Transmitter only)                                         */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T31
static void  MhpStartConnection(void) 
{
    pTMhpTx buf_ptr;
    byte    err_status;
    word    temp;
    #ifdef MHP_DELAY_RANGE_EN
    word    temp2;
    #endif

    if(NULL == (buf_ptr = MhpGetTxBufPtr()))                                /* Look for corresponding buffer entry  */
    {
        #ifdef MHP_DBG_TX
        MhpDebugInfo[0] = MHP_DBG_STCON_TX;
        MhpDebugInfo[1] = (byte)0x01;                                       /* "Start Connection" ignored, due to missing connection entry */
        MhpDebugInfoTx(buf_ptr, &MhpDebugInfo[0], (byte)2);
        #endif
        return;
    }

    if( ((word)0xFFFF != buf_ptr->BlockCnt) && ((word)0x0000 != buf_ptr->BlockCnt) )    /* Ignore the "StartConnection" command as soon as */
    {                                                                                   /* block 0x00 has been acknowledged. */
                                                                                        /* Just testing state "TX_WAIT_START" is not sufficient, */
                                                                                        /* since the "ReadyForData" command could be lost. */
        #ifdef MHP_DBG_TX
        MhpDebugInfo[0] = MHP_DBG_STCON_TX;
        MhpDebugInfo[1] = (byte)0x02;                               /* "Start Connection" ignored, due to wrong state (BlockCnt) */
        MhpDebugInfoTx(buf_ptr, &MhpDebugInfo[0], (byte)2);
        #endif
        return;                                                     /* When the connection has already been */
                                                                    /* established (that means as soon as block #0 has been acknowledged. */
    }

    if( (TX_GETBUF_END == buf_ptr->State) || (TX_LOOP_END == buf_ptr->State) )  /* Ignore the "StartConnection" command if connection */
    {                                                                           /* is currently shut down */
        #ifdef MHP_DBG_TX
        MhpDebugInfo[0] = MHP_DBG_STCON_TX;
        MhpDebugInfo[1] = (byte)0x03;
        MhpDebugInfoTx(buf_ptr, &MhpDebugInfo[0], (byte)2);
        #endif
        return;
    }

    err_status = (byte)0;                                   /* init error indication */

    buf_ptr->VersionIDRx = MhpRxTel.Data[2];                /* get version id of receiver */

    if(MhpRxTel.Data[3] > buf_ptr->Priority)                /* Request rejected since higher priority */
    {
        err_status  = MHP_TX_ERROR_PRIO;                    /* --> Transmission failed, since request rejected */
                                                            /*     by higher priority */
    }
    else                                                    /* Request accepted */
    {
        if(MhpRxTel.Data[1])                                /* Scale > 0 ? */
        {
            buf_ptr->NumDataFrame = (word)((word)MhpRxTel.Data[4] << 8) + (word)MhpRxTel.Data[5];
            if(buf_ptr->NumDataFrame > (word)(MhpNumDataPacketRaw - MHP_ASYNC_DATA_OFFSET))
            {
                buf_ptr->NumDataFrame = (word)(MhpNumDataPacketRaw - MHP_ASYNC_DATA_OFFSET);
            }

            if((word)0 != buf_ptr->NumDataFrame)
            {
                buf_ptr->Scale = MhpRxTel.Data[1];                              /* Read Scale (Frames/Block) */

                if(MHP_STA_CON_LEN_2V3 == MhpRxTel.TelLen)                      /* receiver uses MHP spec 2V3 ? */
                {
                    buf_ptr->MaxBlkSize = (word)((word)MhpRxTel.Data[8] << 8) + (word)MhpRxTel.Data[9];
                }
                else
                {
                    buf_ptr->MaxBlkSize = (word)0;
                }

                MHP_DBG_TX_STATE(buf_ptr, TX_GETBUF_READY);                     /* DebugPrint state transition */
                buf_ptr->State = TX_GETBUF_READY;

                temp = (word)((word)MhpRxTel.Data[6] << 8) + (word)MhpRxTel.Data[7];    /* received average interrupt rate of receiver */
                #ifdef MHP_DELAY_RANGE_EN
                temp2 = temp;
                #endif                                                          /* store AIR by receiver for presentation in MhpTxEstablishDelay() */
                temp = (word)(temp / MhpAvrgIntRate);                           /* ratio of receiver to sender */

                if(temp)                                                        /* delay = (air_rx / air_tx - 1) if ratio >= 1 */
                {
                    temp--;                                                     /* delay = 0 if ratio < 1 */
                }

                #ifdef MHP_DELAY_RANGE_EN                                               /* Range of delay controlled by application ? */
                temp2 = MhpTxEstablishDelay(MhpAvrgIntRate, temp2, temp, buf_ptr);      /* call application to establish the delay window */
                if((word)0xFFFF == temp2)
                {
                    if(temp < buf_ptr->MinDelay)
                    {
                        temp = buf_ptr->MinDelay;
                    }
                    else if(temp > buf_ptr->MaxDelay)
                    {
                        temp = buf_ptr->MaxDelay;
                    }
                }
                else
                {
                    temp = temp2;                           /* start delay has been given by application */
                }
                #else
                if(temp > MAX_DELAY_DEFAULT)                /* Check only max limit due range is not defined by application */
                {
                    temp = MAX_DELAY_DEFAULT;
                }
                #endif

                buf_ptr->TxDelayMax = temp;                 /* Set default delay since new connection */

                buf_ptr->Flags |= MHP_FIRST_BLOCK;

                MNS_REQUEST_HIGH_CALL(MNSH_P_TX_STATE);     /* Request for calling MostServiceHigh() */
            }
            else                                            /* NumDataFrames == 0: */
            {
                err_status = MHP_TX_ERROR_NDF;              /* Numer of data bytes per frame == 0 ! */
            }
        }
        else                                                /* Scale == 0: */
        {
            err_status = MHP_TX_ERROR_SCALE;                /* Scale == 0 ! */
        }
    }

    if((byte)0 != err_status)                               /* any error on receiption of "StartConnection" */
    {
        #ifdef MHP_DBG_TX
        MhpDebugInfo[0] = MHP_DBG_STCON_TX;
        MhpDebugInfo[1] = err_status;                       /* connection not established, due to any wrong parameter (id: 0x04, 0x05 or 0x09) */
        MhpDebugInfoTx(buf_ptr, &MhpDebugInfo[0], (byte)2);
        #endif

        (void)MhpTxStatusWrap(err_status, buf_ptr);
        MhpSetTxBufferFree(buf_ptr);                        /* Cancel job and reset state machine */
    }
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpAcknowledge()                                           */
/* Description : Service Function for received command: FA                  */
/*               (Transmitter only)                                         */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T110
static void  MhpAcknowledge(void) 
{
    byte    frame_id;
    byte    frame_id_last;
    pTMhpTx buf_ptr;
    byte    block_cnt_rx;
    byte    block_cnt_current;
    word    remaining_time;

    if(NULL == (buf_ptr = MhpGetTxBufPtr()))                        /* Look for corresponding buffer entry  */
    {
        return;
    }

    if(buf_ptr != MhpTxBPtrAct)
    {
        if(buf_ptr->Status & BUF_F_READY)                           /* waiting buffer entry ? */
        {
            #ifdef MHP_DBG_TX
            MhpDebugInfo[0] = MHP_DBG_ACKNOTACTIVE_TX;              /* Debug Event: Acknowledge received, but not from expected function  */
            MhpDebugInfoTx(buf_ptr, &MhpDebugInfo[0], (byte)1);
            #endif

            remaining_time = MostGetTimer(&buf_ptr->TimerTx1);  /* save remaining time of HOLD timer */
            MostClearTimer(&buf_ptr->TimerTx1);                         /* Clear Hold Loop */
            buf_ptr->Retry++;
            buf_ptr->HoldLoopTime += remaining_time;                    /* compensate the the flushed timeout value */
            MNS_REQUEST_HIGH_CALL(MNSH_P_TX_TIMEOUT);                   /* Request for calling MostServiceHigh() */
        }

        return;
    }

    frame_id_last = MhpRxTel.Data[1];                               /* High Byte */
    frame_id      = MhpRxTel.Data[2];                               /* Low Byte */
    block_cnt_rx  = MhpRxTel.Data[3];

    /*---------------- */
    /* Negative Ack    */
    /*---------------- */
    if( ((byte)0 == frame_id) && ((byte)0 == frame_id_last) )           /* Negative Acknowledge, since frame_id == 0 and frame_id_last == 0 */
    {
        block_cnt_current = (byte)(buf_ptr->BlockCnt & (word)0x00FF);   /* current block counter */

        #ifdef MHP_DBG_TX
            MhpDebugInfo[0] = MHP_DBG_NEGACK_TX;                    /* Debug Event: Negative Acknowledge received */
            MhpDebugInfo[1] = block_cnt_current;                    /* current block counter */
            MhpDebugInfo[2] = block_cnt_rx;                         /* received block counter */
            MhpDebugInfoTx(buf_ptr, &MhpDebugInfo[0], (byte)3);
        #endif

        if( (block_cnt_current < block_cnt_rx) || (((byte)0xFF == block_cnt_current) && ((byte)0 == block_cnt_rx)) )    /* Next block expected by receiver ? */
        {
            if(buf_ptr != MhpTxBPtrAct)                     /* Check Tx Token  <Bugfix, 99-07-27> */
            {
                return;
            }

            MhpBlockAck();                                  /* Forget about current block, since next block is expected by receiver  */

            MHP_DBG_TX_STATE(buf_ptr, TX_WAIT_ACK);         /* DebugPrint state transition */
            buf_ptr->State = TX_WAIT_ACK;                   /* Branch in that state and try to transmit next block. */
        }
        else                                                /* repetition of current block required */
        {
            MHP_DBG_TX_STATE(buf_ptr, TX_NEG_ACK_REQ);      /* DebugPrint state transition */
            buf_ptr->State = TX_NEG_ACK_REQ;
            MhpTxSpeedDown(buf_ptr);
        }
    }
    /*---------------- */
    /* Positive Ack    */
    /*---------------- */
    else
    {
        if(buf_ptr != MhpTxBPtrAct)                             /* Tx Token Protection */
        {
            return;
        }

        if(buf_ptr->Options & MHP_BLOCK_ACK)                    /* If block acknowledge required: */
        {
            if(frame_id == buf_ptr->FrameLast)                  /* make sure that last frame is acknowledged */
            {
                MhpBlockAck();
            }
        }

        MhpSetTxAck(frame_id, MNS_FALSE);                       /* Reset flag in bitfield, since frame acknowledged */

        #ifdef MHP_DBG_TX
        if(frame_id == buf_ptr->FrameLast)
        {
            MhpDebugInfo[0] = MHP_DBG_FINACK_TX;                        /* Debug Event: Last Acknowledge received */
            MhpDebugInfoTx(buf_ptr, &MhpDebugInfo[0], (byte)1);
        }
        #endif

        if(MNS_FALSE != MhpCheckTxAckAll())                             /* All frames of current block acknowledged ? */
        {
            buf_ptr->State = TX_WAIT_ACK;                               /* -> try to transmit next block */
        }

    }

    MNS_REQUEST_HIGH_CALL(MNSH_P_TX_STATE);                 /* Request for calling MostServiceHigh()  */
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpRequest()                                               */
/* Description : Service Function for received command: FB                  */
/*               (Transmitter only)                                         */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T111
static void  MhpRequest(void) 
{
    pTMhpTx buf_ptr;
    byte    frame_id;

    if(NULL == (buf_ptr = MhpGetTxBufPtr()))                    /* Look for corresponding buffer entry */
    {
        return;
    }

    if(buf_ptr != MhpTxBPtrAct)                                 /* received message does not refer to current block */
    {
        return;
    }

    frame_id = MhpRxTel.Data[2];

    if(frame_id > buf_ptr->FrameLast)                           /* make sure that frame is available in current block */
    {
        return;
    }

    /*--------------------- */
    /* Block Request        */
    /*--------------------- */
    if((byte)0 == frame_id)                                     /* frame_id == 0 ? */
    {
        MHP_DBG_TX_STATE(buf_ptr, TX_BLOCK_REQ);                /* DebugPrint state transition */
        buf_ptr->State = TX_BLOCK_REQ;                          /* -> same block again */
    }
    /*--------------------- */
    /* Single Frame Request */
    /*--------------------- */
    else
    {
        MhpSetTxAck(frame_id, MNS_TRUE);                        /* Set bit in TxAck bitfield */
        MhpTxFrIdState(frame_id, MNS_TRUE);                     /* Set bit in TxFrId bitfield */

        if(buf_ptr->FrameCurrent > frame_id)                    /* Current frame id greater than requested frame id? */
        {
            buf_ptr->FrameCurrent = frame_id;                   /* Set Current frame id to requested frame id */
        }
    }

    MNS_REQUEST_HIGH_CALL(MNSH_P_TX_STATE);                     /* Request for calling MostServiceHigh() */
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpRequestMulti()                                          */
/* Description : Service Function for received command: FF                  */
/*               (Transmitter only)                                         */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T112
static void  MhpRequestMulti(void) 
{
    pTMhpTx buf_ptr;
    byte    *src_ptr;
    byte    frame_id;
    byte    num;
    bool    buf_full;

    if(NULL == (buf_ptr = MhpGetTxBufPtr()))                    /* Look for corresponding buffer entry */
    {
        return;
    }

    if(buf_ptr != MhpTxBPtrAct)                                 /* received message does not refer to current block */
    {
        return;
    }

    if(buf_ptr->Options & MHP_FRAME_ACK)                        /* ignore multi frame request in Single-FrAck-Mode */
    {
        return;
    }

    src_ptr = &MhpRxTel.Data[1];                                /* ptr at received frame request telegram */

    if(MhpRxTel.Tel_Len_L > (byte)1)
    {
        num = (byte)(MhpRxTel.Tel_Len_L - (byte)1);
    }
    else
    {
        num = (byte)0;
    }

    buf_full = MNS_FALSE;

    while(num-- && (MNS_FALSE == buf_full))
    {
        frame_id = *src_ptr++;
        if(frame_id <= buf_ptr->FrameLast)                      /* Make sure that frame is available in current block */
        {
            MhpSetTxAck(frame_id, MNS_TRUE);                    /* Set bit in TxAck bitfield */
            MhpTxFrIdState(frame_id, MNS_TRUE);                 /* Set bit in TxFrId bitfield */

            if(buf_ptr->FrameCurrent > frame_id)                /* Current frame id greater than requested frame id? */
            {
                buf_ptr->FrameCurrent = frame_id;               /* Set Current frame id to requested frame id */
            }
        }
    }

    buf_ptr->State = TX_SEND_FRAME;

    if((word)0 == MostGetTimer(&MhpTxTimerG2) || (MHP_SPEED_DEC != MhpTxSpeedDir))    /* decement only, if after timeout or last change wasn't decrement */
    {
          MhpTxSpeedDown(buf_ptr);
    }

    MostSetTimer(&MhpTxTimerG2, MHP_CFG_TO_TX_SPEED_RECOVERY, MNS_FALSE);       /* load timer to recover higher speed */

    MNS_REQUEST_HIGH_CALL(MNSH_P_TX_STATE);                                 /* Request for calling MostServiceHigh() */
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpHoldRx()                                                */
/* Description : Service Function for received command: FE                  */
/*               (Transmitter only)                                         */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T113
static void  MhpHoldRx(void) 
{
    pTMhpTx buf_ptr;
    byte    state;

    if(NULL == (buf_ptr = MhpGetTxBufPtr()))                    /* Look for corresponding buffer entry */
    {
        return;
    }

    if( (byte)0 == (buf_ptr->Status & BUF_F_IGN_HOLD) )         /* Check Flag: "Ignore HoldRx" */
    {
        state = buf_ptr->State;
        if( (TX_WAIT != state) && (TX_HOLD != state) && (TX_GETBUF_HOLD != state) )
        {
            buf_ptr->TimerTx1Back   = MostGetTimer(&buf_ptr->TimerTx1); /* backup timer */

            MHP_DBG_TX_STATE(buf_ptr, TX_WAIT);                                 /* DebugPrint state transition */
            buf_ptr->State          = TX_WAIT;                                  /* new state */

            if(buf_ptr == MhpTxBPtrAct)
            {
                MhpTxBPtrAct       = NULL;                      /* release tx token */
                buf_ptr->StateBack = TX_NEG_ACK_REQ;            /* backup state */

                MNS_REQUEST_HIGH_CALL(MNSH_P_TX_STATE);         /* Request for calling MostServiceHigh() */
                                                                /* --> look for another connection, which is waiting for Tx Token */
            }
            else
            {
                buf_ptr->StateBack = state;                     /* backup state */
            }
        }

        MostSetTimer(&buf_ptr->TimerTx1, MHP_CFG_TO_HOLD, MNS_FALSE);   /* Refresh Timer */
    }
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpEndRx()                                                 */
/* Description : Service Function for received command: FC                  */
/*               (Transmitter only)                                         */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T114
static void  MhpEndRx(void) 
{
    pTMhpTx buf_ptr;

    if(NULL != (buf_ptr = MhpGetTxBufPtr()))                    /* Look for corresponding buffer entry */
    {
        if( (buf_ptr->Status & BUF_F_READY) && !(buf_ptr->Status & BUF_F_RELOAD) )      /* Call application only on pending transmission */
        {
            (void)MhpTxStatusWrap(MHP_TX_ERROR_KILLED, buf_ptr);
        }
        else
        {
            (void)MhpTxStatusWrap(MHP_TX_KILLED_RX, buf_ptr);
        }

        MhpSetTxBufferFree(buf_ptr);                            /* -> release buffer entry */
    }
}
#endif





/*
==============================================================================
    Functions, which are called by Mhp Command Interpreter.
    The following functions are needed by Receiver only.
==============================================================================
*/
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpRxCompleteWrap()                                        */
/* Description : Wrapper function for MhpRxComplete().                      */
/*               If silent connection termination ==> Send debug message    */
/* Parameter(s): status, const pointer at buffer entry                      */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R84
static void MhpRxCompleteWrap(byte status, pTMhpRx rx_ptr)
{
    switch(status)
    {
        case MHP_RX_CLOSE:
        case MHP_RX_ERROR_NFRAME:
        case MHP_RX_ERROR_FRAME:
        case MHP_RX_ERROR_NOREADY:
        case MHP_RX_ERROR_KILLED:
        case MHP_RX_ERROR_NOBUF:
        case MHP_RX_KILLED_RX:
            #ifdef MHP_26
            MhpSendMdmMhpSilentTermination(status, MHP_MDM_RX_STATUS);      /* Send debug message for states above */
            #endif
            /*lint -fallthrough */
        case MHP_RX_SUCCESS:
        default:
            MhpRxComplete(status, rx_ptr);
            break;
    }
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpRequestConnection()                                     */
/* Description : Service Function for received command: CA                  */
/*               (Receiver only)                                            */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R31
static void  MhpRequestConnection(void) 
{
    pTMhpRx buf_ptr;
    byte    *data_ptr;
    void    *void_ptr;                                  /* temporary universal pointer, read from ROM table */
    byte    prio_highest;

    if(NULL != (buf_ptr = MhpGetRxBufPtr()))            /* Same Request already done ? */
    {
        if(buf_ptr->State < RX_WAIT_NFRAME)
        {
            #ifdef MHP_DBG_RX
            MhpDebugInfo[0] = MHP_DBG_REQCON_RX;
            MhpDebugInfo[1] = (byte)0x01;
            MhpDebugInfoRx(buf_ptr, &MhpDebugInfo[0], (byte)2);
            #endif
            return;
        }
        else /* implicite End-By-Tx: */
        {
            MhpRxCompleteWrap(MHP_RX_CLOSE, buf_ptr);
            MhpSetRxBufferFree(buf_ptr);
            buf_ptr->Status = BUF_F_LOCK;
        }
    }
    else
    {
        if(NULL == (buf_ptr = MhpSearchFreeRxLine()))  /* Free Rx Buffer available ? */
        {
            #ifdef MHP_DBG_RX
            MhpDebugInfo[0] = MHP_DBG_REQCON_RX;
            MhpDebugInfo[1] = (byte)0x02;                   /* "Request Connection" ignored, due to full RX list */
            MhpDebugInfoRx(buf_ptr, &MhpDebugInfo[0], (byte)2);
            #endif
            return;
        }
    }

    MhpRxListInsertCon(buf_ptr);                        /* Insert the new connection to the rx connection list */

    buf_ptr->Src_Adr      = (word)((word)MhpRxTel.Src_Addr_H << 8) + (word)MhpRxTel.Src_Addr_L;
    buf_ptr->NumDataFrame = (word)((word)MhpRxTel.Data[2] << 8) + (word)MhpRxTel.Data[3];

    if(MhpRxTel.TelLen < (word)5)
    {
        buf_ptr->VersionIDTx = (byte)0x00;              /* version id of sender: Rev.2.1 */
    }
    else
    {
        buf_ptr->VersionIDTx = MhpRxTel.Data[4];        /* get version id of sender */
    }

    buf_ptr->FBlock_ID  = MhpRxTel.FBlock_ID;
    buf_ptr->Inst_ID    = MhpRxTel.Inst_ID;
    buf_ptr->Func_ID    = ((word)((word)MhpRxTel.Func_ID_H << 4) + ((word)((word)MhpRxTel.Func_ID_L_Op >> 4) & (word)0x000F)) & (word)0x0FFF;
    buf_ptr->Operation  = (byte)MhpRxTel.Func_ID_L_Op & (byte)0x0F;

    if(MhpRxTel.Data[1] > MHP_PRIO_MAX)                 /* check for valid priority level */
    {
        MhpRxTel.Data[1] = MHP_PRIO_MAX;
    }

    buf_ptr->Priority = MhpRxTel.Data[1];

    prio_highest = MhpRxGetHighestPrio(buf_ptr);        /* Get highest Rx Priority */

    if(prio_highest >= MhpRxTel.Data[1])                /* Received priority to low ? */
    {
        if(prio_highest == MhpRxTel.Data[1])
        {
            prio_highest++;
        }

        buf_ptr->Priority = prio_highest;

        MHP_DBG_RX_STATE(buf_ptr, RX_GETBUF_REJECT);    /* DebugPrint state transition */
        buf_ptr->State = RX_GETBUF_REJECT;              /* -> Reject Priority, else accept priority */

        #ifdef MHP_DBG_RX
        MhpDebugInfo[0] = MHP_DBG_REQCON_RX;
        MhpDebugInfo[1] = (byte)0x03;                   /* "Request Connection" rejected, since priority too low */
        MhpDebugInfoRx(buf_ptr, &MhpDebugInfo[0], (byte)2);
        #endif

        MNS_REQUEST_HIGH_CALL(MNSH_P_RX_STATE);         /* Request for calling MostServiceHigh()  */
        return;
    }

    MhpPtrBufIntf = buf_ptr;                            /* store pointer for access within MhpSetBufIntf() */
    MhpPtrBufIntf->BufPtr = NULL;                       /* must be set by MhpSetBufIntf() or directly within MhpGetBufIntf() */

    if( ((byte)0xFF != MhpPtrBufIntf->FBlock_ID) && ((byte)0xFF != MhpPtrBufIntf->Inst_ID) )
    {
        if(MNS_FALSE != MhpGetBufIntf(MhpPtrBufIntf))
        {
            #ifdef MHP_DBG_RX
            MhpDebugInfo[0] = MHP_DBG_REQCON_RX;
            MhpDebugInfo[1] = (byte)0x09;               /* "Request Connection" ignored, since MhpGetBufIntf() failed (no free AMS buffers) */
            MhpDebugInfoRx(buf_ptr, &MhpDebugInfo[0], (byte)2);
            #endif
        }
    }
    else
    {
        #ifdef MHP_DBG_RX
        MhpDebugInfo[0] = MHP_DBG_REQCON_RX;
        MhpDebugInfo[1] = (byte)0x08;                   /* "Request Connection" ignored, since FBlockID==0xFF or InstID==0xFF (broadcast) */
        MhpDebugInfoRx(buf_ptr, &MhpDebugInfo[0], (byte)2);
        #endif
    }

    MhpPtrBufIntf = NULL;
    if(NULL != (void_ptr = buf_ptr->BufPtr))            /* Buffer interface available ? */
    {
        switch(buf_ptr->BufFlags)
        {
            case MHP_FLAGS_DATA:                        /* get buffer pointer directly */
                data_ptr = (byte *)void_ptr;
                break;

            case MHP_FLAGS_FUNC:                                /* get pointer at Data Buffer by service function */
                data_ptr = (*(byte *(*)(byte val, pTMhpRx p_val)) void_ptr)(MHP_GETRXPTR_REQUEST, buf_ptr);     /*lint !e611 Casts are valid */
                break;                                          /* parameter: id = Request; pointer at mhp rx buffer entry */
                                                                /* (first call for allocating a free buffer) */
            default:
                data_ptr = NULL;
                break;
        }

        if(NULL == data_ptr)
        {
            #ifdef MHP_DBG_RX
            MhpDebugInfo[0] = MHP_DBG_REQCON_RX;
            switch(buf_ptr->BufFlags)
            {                                                   /*  "Request Connection" ignored, since no data buffer available ... */
                case MHP_FLAGS_DATA:
                    MhpDebugInfo[1] = (byte)0x05;               /*  ... static NULL Ptr  */
                    break;

                case MHP_FLAGS_FUNC:
                    MhpDebugInfo[1] = (byte)0x06;               /*  ... callback function returned NULL  */
                    break;

                default:
                    MhpDebugInfo[1] = (byte)0x07;               /*  ... invalid description of buffer interface (wrong flagfield) */
                    break;

            }
            MhpDebugInfoRx(buf_ptr, &MhpDebugInfo[0], (byte)2);
            #endif

            MhpSetRxBufferFree(buf_ptr);                        /* Release Mhp Rx Buffer, since no data buffer available */
            return;
        }

        buf_ptr->DataPtrBlock = data_ptr;                       /* store pointer at data buffer in structure field */
    }
    else                                                        /* buffer interface not available */
    {
        #ifdef MHP_DBG_RX
        MhpDebugInfo[0] = MHP_DBG_REQCON_RX;
        MhpDebugInfo[1] = (byte)0x04;                           /* "Request Connection" ignored, since description of buffer interface not available */
        MhpDebugInfoRx(buf_ptr, &MhpDebugInfo[0], (byte)2);     /* (e.g. table entry not available or MhpSetBufIntf() has not been called) */
        #endif

        MhpSetRxBufferFree(buf_ptr);                            /* Release Mhp Rx Buffer, since Table entry not available */
        return;
    }

    buf_ptr->Retry1 = RX_RETRY_START;
    MHP_DBG_RX_STATE(buf_ptr, RX_GETBUF_START);         /* DebugPrint state transition */
    buf_ptr->State  = RX_GETBUF_START;

    MNS_REQUEST_HIGH_CALL(MNSH_P_RX_STATE);             /* Request for calling MostServiceHigh()  */

}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpReadyForData()                                          */
/* Description : Service Function for received command: FD                  */
/*               (Receiver only)                                            */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R81
static void  MhpReadyForData(void) 
{
    pTMhpRx buf_ptr;

    if(NULL != (buf_ptr = MhpGetRxBufPtr()))                    /* Look for corresponding buffer entry */
    {
        if(RX_WAIT_READY == buf_ptr->State)
        {
            MostSetTimer(&buf_ptr->TimerRx1, MHP_CFG_TO_FRAME, MNS_FALSE);  /* Load Timeout  */
            buf_ptr->Retry1 = RX_RETRY_NEGACK;                          /* <Bugfix, 04.05.2000> */

            MHP_DBG_RX_STATE(buf_ptr, RX_WAIT_NFRAME);          /* DebugPrint state transition */
            buf_ptr->State = RX_WAIT_NFRAME;

            MhpRxNumFrameFailed  = (byte)0;
            buf_ptr->BlockCnt    = (byte)0x00;                  /* Init Block Counter (incremented on each new block, even on first block of new packet) */
            buf_ptr->PacketCnt   = (word)0xFFFF;                /* Init Packet Counter (incremented on each N-Frame of first block) */
            buf_ptr->BlockCntRel = (word)0x0000;                /* Relative block counter (incremented on each N-Frame, reset on first or single block) */
            buf_ptr->SegIDRx     = (byte)0xFF;                  /* Init segmentation ID (0xFF: N-Frame not yet received) */

            MNS_REQUEST_HIGH_CALL(MNSH_P_RX_STATE);             /* Request for calling MostServiceHigh()  */
        }
    }
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpHoldTx()                                                */
/* Description : Service Function for received command: F1                  */
/*               (Receiver only)                                            */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R82
static void  MhpHoldTx(void) 
{
    pTMhpRx buf_ptr;

    if(NULL != (buf_ptr = MhpGetRxBufPtr()))                    /* Look for corresponding buffer entry */
    {
        buf_ptr->Status |= BUF_F_HOLD_BY_TX;
        MostSetTimer(&buf_ptr->TimerRx1, MHP_CFG_TO_HOLD, MNS_FALSE);   /* Refresh Timeout */

        MhpRxCheckPrio();                                       /* Look for another connection */

        if(RX_WAIT_NFRAME == buf_ptr->State)                    /* waiting for N-Frame ? */
        {
            buf_ptr->Retry1 = RX_RETRY_NEGACK;                  /* Refresh retry counter for NEGACK, */
        }                                                       /* due to the problem when the MhpHoldTx */
                                                                /* telegram comes too late */
    }
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpEndTx()                                                 */
/* Description : Service Function for received command: F3                  */
/*               (Receiver only)                                            */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R83
static void  MhpEndTx(void) 
{
    pTMhpRx buf_ptr;
    byte    status;


    if(NULL != (buf_ptr = MhpGetRxBufPtr()))            /* Look for corresponding buffer entry */
    {
        if((byte)0xFF == MhpRxTel.Data[2])              /* read event */
        {
            status = MHP_RX_ERROR_KILLED;               /* Transmitter killed connection */
        }
        else
        {
            status = MHP_RX_CLOSE;                      /* normal shut down */
        }

        MhpRxCompleteWrap(status, buf_ptr);

        MhpSetRxBufferFree(buf_ptr);
    }
}
#endif



#ifdef MHP_R32
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpDataFrameReceive()                                      */
/* Description : Receive one data frame.                                    */
/*               (Receiver only)                                            */
/* Parameter(s): Const pointer to data Rx struct                            */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
static void  MhpDataFrameReceive(pcTDataRx ptr_ads) 
{
    byte    frame_id;
    byte    frame_id_last_tmp;
    word    num_data_frame;
    word    length;
    pTMhpRx buf_ptr;
    byte    *src_ptr;                               /* source pointer for mem-copy */
    byte    *dest_ptr;                              /* destination pointer for mem-copy */
    byte    state;
    byte    block_cnt_rx;                           /* received block counter (0-Frame only) */
    dword   current_block_length;

    if(NULL == (buf_ptr = MhpGetRxBufPtr()))        /* Look for corresponding buffer entry */
    {
        return;
    }

    buf_ptr->Status &= (byte)(~BUF_F_HOLD_BY_TX);   /* Clear flag -> buffer entry is not ignored when checking priority */

    length = MhpRxTel.TelLen - MHP_DF_OH_PCKT;      /* Length without FrAck */

    num_data_frame = buf_ptr->NumDataFrame;

    src_ptr           = &ptr_ads->Data[8];          /* prepare source pointer for mem-copy */
    frame_id          = ptr_ads->Data[6];
    frame_id_last_tmp = ptr_ads->Data[7];

    /*--------------------------------------- */
    /* Receiving Null-Frame (Part 1)          */
    /*--------------------------------------- */
    if((byte)0 == frame_id)
    {
        buf_ptr->FrameIdLast = ptr_ads->Data[7];

        if(RX_WAIT_READY == buf_ptr->State)                     /* discard NULL-frame in state RX_WAIT_READY */
        {
            return;
        }

        if(MhpRxBPtrAct)                                        /* If Rx Token engaged: */
        {
            if(buf_ptr != MhpRxBPtrAct)                         /* Token used by another task ?  */
            {
                buf_ptr->StateBack  = RX_GETBUF_NEGACK;         /* Backup State when returning from Hold-Loop */
                buf_ptr->HoldLoop   = MHP_LOOP_HOLD_MAX_BUSY;   /* Counter for Hold-Loop */
                buf_ptr->HoldEvent  = MHP_HOLD_EVENT_BUSY_RX;   /* -> Hold the connection, since receiver is processing  */
                                                                /*    another connection */

                MHP_DBG_RX_STATE(buf_ptr, RX_GETBUF_HOLD);      /* DebugPrint state transition */
                buf_ptr->State      = RX_GETBUF_HOLD;

                MNS_REQUEST_HIGH_CALL(MNSH_P_RX_STATE);         /* Request for calling MostServiceHigh()  */
                return;
            }
            else
            {
                if(RX_RECEIVE_DATA == buf_ptr->State)           /* Resynchronization in 'Single Frame Ack Mode' */
                {                                               /* due to lost acknowledge of N-Frame necessary ! */
                    if(buf_ptr->Options & MHP_FRAME_ACK)                /* Single Frame Ack mode ? */
                    {
                        MHP_DBG_RX_STATE(buf_ptr, RX_WAIT_NFRAME);      /* DebugPrint state transition */
                        buf_ptr->State = RX_WAIT_NFRAME;
                    }
                }
            }
        }
        else                                                    /* If Rx Token unused: */
        {
            if(NULL == buf_ptr->DataPtrBlock)                   /* Missing Data Buffer ? -> Hold Rx */
            {
                buf_ptr->StateBack  = RX_GETBUF_NEGACK;         /* Backup State when returning from Hold-Loop */
                buf_ptr->HoldLoop   = MHP_LOOP_HOLD_MAX_BUF;    /* Counter for Hold-Loop */
                buf_ptr->HoldEvent  = MHP_HOLD_EVENT_BUF;       /* -> Hold the connection, since buffer of application */
                MHP_DBG_RX_STATE(buf_ptr, RX_GETBUF_HOLD);      /* DebugPrint state transition */
                buf_ptr->State      = RX_GETBUF_HOLD;           /* has not been established */

                MNS_REQUEST_HIGH_CALL(MNSH_P_RX_STATE);         /* Request for calling MostServiceHigh()  */
                return;

            }
            else
            {
                if(MhpRxSearchHighestPrio(MHP_PRIO_INIT) == buf_ptr)    /* No higher priority waiting ? */
                {
                    MhpRxBPtrAct = buf_ptr;                             /* Set Token ! */
                }
                else
                {
                    buf_ptr->StateBack  = RX_GETBUF_NEGACK;         /* Backup State when returning from Hold-Loop */
                    buf_ptr->HoldLoop   = MHP_LOOP_HOLD_MAX_BUSY;   /* Counter for Hold-Loop */
                    buf_ptr->HoldEvent  = MHP_HOLD_EVENT_BUSY_RX;   /* -> Hold the connection, since receiver is processing */
                    MHP_DBG_RX_STATE(buf_ptr, RX_GETBUF_HOLD);      /* DebugPrint state transition */
                    buf_ptr->State      = RX_GETBUF_HOLD;           /* another connection */

                    MNS_REQUEST_HIGH_CALL(MNSH_P_RX_STATE);         /* Request for calling MostServiceHigh() */
                    return;
                }
            }
        }
    }
    /*--------------------------------------- */
    /* Receiving Data Frame (Part 1)          */
    /*--------------------------------------- */
    else
    {
        if(RX_WAIT_NFRAME == buf_ptr->State)
        {
            if((word)0 != MostGetTimer(&MhpRxTimerG1))                 /* Timeout not yet ? */
            {
                return;
            }

            MhpRxNumFrameFailed++;
            if(MhpRxNumFrameFailed > MHP_RX_MAX_FRAME_FAILED)
            {
                MhpRxNumFrameFailed = (byte)0;

                buf_ptr->Retry1++;
                MostClearTimer(&buf_ptr->TimerRx1);                 /* Clear timer to force negative acknowledge */
                MNS_REQUEST_HIGH_CALL(MNSH_P_RX_TIMEOUT);           /* Request for calling MostServiceHigh() */

                #ifdef MHP_DBG_RX
                MhpDebugInfo[0] = MHP_DBG_DATAFR_RX;                /* Debug Event: Data Frame received without Null-Frame */
                MhpDebugInfo[1] = buf_ptr->FrameIdLast;
                MhpDebugInfo[2] = frame_id;
                MhpDebugInfoRx(buf_ptr, &MhpDebugInfo[0], (byte)3);
                #endif

                MostSetTimer(&MhpRxTimerG1, MHP_CFG_TO_DWN_NEGACK, MNS_FALSE);  /* Timer to avoid too many NegAcks  */
            }
            return;
        }

        if(buf_ptr != MhpRxBPtrAct)                                 /* forget about, since data frame belongs not  */
        {
            return;                                                 /* to active receive buffer */
        }
    }

    state = buf_ptr->State;

    /*--------------------------------------- */
    /* Receiving Null-Frame (Part 2)          */
    /*--------------------------------------- */
    if((byte)0 == frame_id)
    {
        if(RX_WAIT_NFRAME != state)
        {
            return;
        }

        buf_ptr->SegIDRx = *src_ptr++;                          /* received SegID */

        buf_ptr->Options = *src_ptr++;                          /* copy "Options" */
        block_cnt_rx = *src_ptr;                                /* get block counter */

        #ifdef MHP_DBG_RX
        MhpDebugInfo[0] = MHP_DBG_NFRAME_RX;                    /* Debug Event: Null Frame Received */
        MhpDebugInfo[1] = buf_ptr->BlockCnt;                    /* expected block counter */
        MhpDebugInfo[2] = block_cnt_rx;                         /* received block counter */
        MhpDebugInfoRx(buf_ptr, &MhpDebugInfo[0], (byte)3);
        #endif

        if(buf_ptr->BlockCnt != block_cnt_rx)                   /* Check block counter */
        {
            if(MhpRxBPtrAct == buf_ptr)
            {
                MhpRxBPtrAct = NULL;                           /* release the Rx token */
            }
            return;                                             /* to prevent repetitions */
        }
                                                                /* Init variables, since RxToken grabbed and N-Frame received: */
        MhpPrepareRxAckField(buf_ptr->FrameIdLast);             /* Prepare RxAck Bitfield for next block */
        MhpRxFrameCnt = buf_ptr->FrameIdLast;                   /* load counter counting received frames (decrementing) */
        MhpRxErrCnt = MHP_RX_ERROR_CNT_MAX;                     /* reset error counter */
        MhpRxFirstRequest = MNS_TRUE;                           /* reset flag */
        MostSetTimer(&MhpRxTimerG1, MHP_CFG_TO_RX_CHECK, MNS_FALSE);
        MhpRxHighestFrame = (byte)0x00;                         /* reset highest frame id, that have been received */

        if(buf_ptr->VersionIDTx >= (byte)0x01)                  /* segmentation id supported by sender device */
        {
            if(buf_ptr->SegIDRx < (byte)0x02)                   /* single block or first block of segmented transmission */
            {
                buf_ptr->BlockCntRel = (word)0;                 /* init relative block counter, since a new packet is transmitted */
                if((word)0xFFFF == buf_ptr->PacketCnt)          /* avoid overflow of unsigned integer */
                {
                    buf_ptr->PacketCnt = (word)0;               /* new packet is started */
                }
                else
                {
                    buf_ptr->PacketCnt++;                       /* new packet is started */
                }
            }
            else
            {
                buf_ptr->BlockCntRel++;                         /* incr. relative block counter since middle or last block received */
            }
        }
        else
        {
            /* segmentation id is not yet supported by sender device */
        }

        MHP_DBG_RX_STATE(buf_ptr, RX_RECEIVE_DATA);             /* DebugPrint state transition */
        buf_ptr->State = RX_RECEIVE_DATA;                       /* next state */
    }
    /*--------------------------------------- */
    /* Receiving Data Frame (Part 2)          */
    /*--------------------------------------- */
    else
    {
        if(RX_RECEIVE_DATA != state)
        {
            return;
        }

        current_block_length = (dword)(((word)frame_id - (word)1) * num_data_frame) + length;   /* calculate length of current block */

        if ((dword)0xFFFF < current_block_length)                                                      /* Ignore frame to avoid buffer overflow */
        {
            return;
        }

        if(frame_id == buf_ptr->FrameIdLast)
        {
            buf_ptr->Length = (word)current_block_length;
            MostClearTimer(&MhpRxTimerG1);
            MNS_REQUEST_HIGH_CALL(MNSH_P_RX_STATE);
        }

        if( (frame_id > buf_ptr->FrameIdLast) ||                                                /* Frame Id greater than Max. Frame Id ? */
            (frame_id_last_tmp != buf_ptr->FrameIdLast) ||                                      /* Max. Frame Id "Data Frame" unequal Max. Frame Id "0-Frame" */
            (length > num_data_frame) ||                                                        /* Length bigger than NDFAck value */
            (current_block_length > (dword)buf_ptr->BufSize) )                                         /* Block length exceeds buffer size */
        {
            return;                                                                             /* Ignore invalid frame */
        }

        dest_ptr = &buf_ptr->DataPtrBlock[(((word)frame_id - (word)1) * num_data_frame)];       /* prepare destination pointer for mem-copy */
        MHP_MEM_CPY(dest_ptr, src_ptr, length);                                                 /* mem-copy to destination buffer */

        if(MHP_BLOCK_ACK == buf_ptr->Options)                                   /* Check and request of previous frame only in case of Block-Acknowledge-Mode ! */
        {
            if(MNS_FALSE == MhpCheckRxAck(frame_id - (byte)1))                  /* Previous Frame not yet received ? */
            {
                if(MhpRxErrCnt)
                {
                    MhpRxErrCnt--;
                }
                else
                {
                    if(MNS_FALSE != MhpRxFirstRequest)                          /* force request message immediately only at first time after N-Frame */
                    {
                        MostClearTimer(&MhpRxTimerG1);
                        MNS_REQUEST_HIGH_CALL(MNSH_P_RX_STATE);
                    }
                    else
                    {
                        /* wait until timeout but do not refresh timer MhpRxTimerG1 */
                    }
                }
            }
        }

        if(MNS_FALSE == MhpCheckRxAck(frame_id))                        /* check if data frame is received the first time (N-frame is ignored) */
        {
            if(MhpRxFrameCnt)
            {
                MhpRxFrameCnt--;                                        /* decrement counter, since frame was received the first time */
            }

            if((byte)0 == MhpRxFrameCnt)
            {
                MNS_REQUEST_HIGH_CALL(MNSH_P_RX_STATE);                 /* Request for calling MostServiceHigh() */
            }

            if(MhpRxErrCnt && (frame_id != buf_ptr->FrameIdLast))           /* Refresh timer only in case of a new received frame (not already received), */
            {                                                               /* and only if we are not waiting for the N-Frame, */
                MostSetTimer(&MhpRxTimerG1, MHP_CFG_TO_RX_CHECK, MNS_FALSE);    /* and only if limit of too much errors is reached not yet, */
            }                                                               /* and only if timeout not already forced since last frame received !! */
        }

        if(frame_id > MhpRxHighestFrame)                                /* store highest received frame id */
        {
            MhpRxHighestFrame = frame_id;
        }
    }

    MostSetTimer(&buf_ptr->TimerRx1, MHP_CFG_TO_RECEIVE, MNS_FALSE);        /* Refresh Timeout  */

    /*--------------------------------------- */
    /* Acknowledge */
    /*--------------------------------------- */
    MhpClearRxAck(frame_id);                                            /* Clear bit in RxAck bitfield */

    if(MhpGetRxAckLowest())                                             /* Are further frames of current block expected ? */
    {
        if(buf_ptr->Options & MHP_FRAME_ACK)                            /* Single Frame Acknowledge required ? */
        {                                                               /* But make sure that last received frame will not be acknowledged */
            /*------------------------------- */                        /* until application will be informed */
            /* Send "Frame Acknowledge"       */
            /*------------------------------- */
            if(NULL != MhpRxPrepTx(buf_ptr))
            {
                MhpPrepFrAck(buf_ptr, frame_id, buf_ptr->FrameIdLast, MNS_FALSE);

                MhpSendTxTel();
            }
            /* if no telegram buffer available: forget about acknowledge */
            #ifdef MHP_DBG_RX
            else
            {
                MhpDebugInfo[0] = MHP_DBG_ACKNOBUF_RX;                  /* Debug Event: No buffer available for frame acknowledge */
                MhpDebugInfo[1] = buf_ptr->FrameIdLast;                 /* last frame id */
                MhpDebugInfo[2] = frame_id;                             /* received frame id */
                MhpDebugInfoRx(buf_ptr, &MhpDebugInfo[0], (byte)3);
            }
            #endif
        }
    }
    else                                                                /* all frames of current block were received -> Ready for final Ack */
    {
        MhpDelAckFrIdLast = buf_ptr->FrameIdLast;                       /* store frame_id and number of frames for */
                                                                        /* final acknowledge. This final acknowledge */
                                                                        /* will be send after application is informed about data receiption */
        if(buf_ptr->Options & MHP_FRAME_ACK)
        {
            MhpDelAckFrId = frame_id;                                   /* if Single-Frame-Ack */
        }
        else
        {
            MhpDelAckFrId = buf_ptr->FrameIdLast;                       /* if Block-Ack */
        }
    }
}
#endif





/*
==============================================================================
    Receiver - Section 
==============================================================================
*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpRxTrigger()                                             */
/* Description : Trigger function for rx state machine.                     */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R1
static void  MhpRxTrigger(void) 
{
    byte num_rx_requests;
    void *fkt_ptr;
    byte *data_ptr;
    word scale;
    word scale_max;

    MhpIncRxBPtr();

    switch(MhpRxBPtr->State)
    {
        /*--------------------------------------------------------- */
        case RX_NORM_OP:
        /*--------------------------------------------------------- */
            break;

        /*--------------------------------------------------------- */
        case RX_GETBUF_REJECT:
        /*--------------------------------------------------------- */
            if(NULL != MhpRxPrepTx(MhpRxBPtr))
            {
                MhpTxTel.Data[0] = MHP_CMD_STA_CON;                     /* HighCmd= F2 (Start Connection) */
                MhpTxTel.Data[1] = (byte)0x00;                          /* Scale */
                MhpTxTel.Data[2] = MHP_VERSION_ID;                      /* set version id of receiver */
                MhpTxTel.Data[3] = MhpRxBPtr->Priority;                 /* Priority */


                switch(MhpTxTel.Data[3])
                {
                    case (byte)0xFF:
                        MhpTxTel.Data[4] = (byte)0x03;
                        MhpTxTel.Data[5] = (byte)0x00;
                        MhpTxTel.Data[6] = (byte)0x05;
                        MhpTxTel.Data[7] = (byte)0xFF;
                        break;

                    case (byte)0xFE:
                        MhpTxTel.Data[4] = (byte)0x34;
                        MhpTxTel.Data[5] = (byte)0xC4;
                        MhpTxTel.Data[6] = (byte)0x0C;
                        MhpTxTel.Data[7] = (byte)0x06;
                        break;

                    default:
                        MhpTxTel.Data[4] = (byte)0x00;                  /* NumData/Frame (HighByte) */
                        MhpTxTel.Data[5] = (byte)0x00;                  /* NumData/Frame (LowByte) */

                        MhpTxTel.Data[6] = HB(MhpAvrgIntRateRx);
                        MhpTxTel.Data[7] = LB(MhpAvrgIntRateRx);
                        break;
                }

                MhpSetLengthCtrl((word)8);
                MhpSendTxTel();
                MhpSetRxBufferFree(MhpRxBPtr);
            }
            else                                                        /* if no tx buffer available */
            {
                MhpRxCheckBufRetry(MhpRxBPtr);
            }
            break;

        /*--------------------------------------------------------- */
        case RX_GETBUF_START:
        /*--------------------------------------------------------- */
            if(NULL != MhpRxPrepTx(MhpRxBPtr))
            {
                if((MhpRxBPtr->NumDataFrame > (word)(MhpNumDataPacketRaw - MHP_ASYNC_DATA_OFFSET))) /* num_data_tx > num_data_rx ? */
                {
                    MhpRxBPtr->NumDataFrame = (word)(MhpNumDataPacketRaw - MHP_ASYNC_DATA_OFFSET);  /* --> num_data_tx = num_data_rx ! */
                }

                if(MhpRxBPtr->NumDataFrame > MhpRxBPtr->BufSize)            /* limit NumDataFrame to BufSize */
                {
                    MhpRxBPtr->NumDataFrame = MhpRxBPtr->BufSize;           /* to avoid scale=0 */
                }

                if((word)0 != MhpRxBPtr->NumDataFrame)                      /* avoid division by 0 */
                {
                    scale_max = (word)((word)65535 / MhpRxBPtr->NumDataFrame);  /* !!! Maximum size of Data / Block = 64KBytes - 1 */
                    if(scale_max > (word)0xFF)
                    {
                        scale_max = (word)0xFF;                             /* Maximum number of Frames / Block */
                    }

                    scale = (word)(MhpRxBPtr->BufSize / MhpRxBPtr->NumDataFrame);   /* Scale = Bytes per Block / Bytes per Frame */

                    if(scale > scale_max)
                    {
                        scale = scale_max;
                    }

                    MhpRxBPtr->Scale = (byte)scale;
                }
                else
                {
                    scale = (word)0;
                }

                MhpTxTel.Data[0] = MHP_CMD_STA_CON;                         /* HighCmd= F2 (Start Connection) */
                MhpTxTel.Data[1] = (byte)scale;                             /* Scale */
                MhpTxTel.Data[2] = MHP_VERSION_ID;                          /* set version id of receiver */
                MhpTxTel.Data[3] = MhpRxBPtr->Priority;                     /* Priority */
                MhpTxTel.Data[4] = HB(MhpRxBPtr->NumDataFrame);             /* NumData/Frame (HighByte) */
                MhpTxTel.Data[5] = LB(MhpRxBPtr->NumDataFrame);             /* NumData/Frame (LowByte) */
                MhpTxTel.Data[6] = HB(MhpAvrgIntRateRx);                    /* performance class (HighByte) */
                MhpTxTel.Data[7] = LB(MhpAvrgIntRateRx);                    /* performance class (LowByte) */

                /* transmit MaxBlkSize */
                MhpTxTel.Data[8] = HB(MhpRxBPtr->BufSize);
                MhpTxTel.Data[9] = LB(MhpRxBPtr->BufSize);

                MhpSetLengthCtrl(MHP_STA_CON_LEN_2V3);

                MhpSendTxTel();
                MhpRxBPtr->BufRetry = MHP_BUF_RETRY;

                MostSetTimer(&MhpRxBPtr->TimerRx1, MHP_CFG_TO_READY, MNS_FALSE);    /* Load Timer Rx1 */

                MHP_DBG_RX_STATE(MhpRxBPtr, RX_WAIT_READY);                 /* DebugPrint state transition */
                MhpRxBPtr->State = RX_WAIT_READY;

                MNS_REQUEST_HIGH_SET(MNSH_P_RX_STATE);                      /* Set Request Flag for calling MostServiceHigh()  */
            }
            else                                                            /* if no tx buffer available */
            {
                MhpRxCheckBufRetry(MhpRxBPtr);
            }
            break;

        /*--------------------------------------------------------- */
        case RX_WAIT_READY:
        /*--------------------------------------------------------- */
            if((word)0 == MostGetTimer(&MhpRxBPtr->TimerRx1))                 /* Timeout (t_ready) ? */
            {
                if(MhpRxBPtr->Retry1)                                       /* Retry: RX_RETRY_START */
                {
                    MhpRxBPtr->Retry1--;
                    MHP_DBG_RX_STATE(MhpRxBPtr, RX_GETBUF_START);           /* DebugPrint state transition */
                    MhpRxBPtr->State = RX_GETBUF_START;

                    MNS_REQUEST_HIGH_SET(MNSH_P_RX_STATE);                  /* Set Request Flag for calling MostServiceHigh()  */
                }
                else
                {
                    MhpRxCompleteWrap(MHP_RX_ERROR_NOREADY, MhpRxBPtr);

                    MhpSetRxBufferFree(MhpRxBPtr);                          /* no "Ready for Data" received */
                }                                                           /* --> Cancel connection */
            }
            break;

        /*--------------------------------------------------------- */
        case RX_WAIT_NFRAME:
        /*--------------------------------------------------------- */
            if((word)0 == MostGetTimer(&MhpRxBPtr->TimerRx1))                 /* Timeout (t_frame) ? */
            {
                if(MhpRxBPtr->Retry1)                                       /* Timeout, but not all retries done: */
                {                                                           /* Check Retry: RX_RETRY_NEGACK */
                    if(NULL != MhpRxBPtr->DataPtrBlock)                     /* Buffer available ? */
                    {                                                       /* If no Buffer available -> Send no negative FrameAck */
                        /*------------------------------- */
                        /* Prepare "Negative Acknowledge" */
                        /*------------------------------- */
                        if(NULL != MhpRxPrepTx(MhpRxBPtr))
                        {
                            MhpPrepFrAck(MhpRxBPtr, (byte)0x00, (byte)0x00, MNS_FALSE);

                            MhpSendTxTel();
                            MhpRxBPtr->BufRetry = MHP_BUF_RETRY;
                            MhpRxNumFrameFailed = (byte)0;
                        }
                        else                                                /* if no tx buffer available */
                        {
                            MhpRxCheckBufRetry(MhpRxBPtr);
                            break;
                        }

                    }

                    MhpRxBPtr->Retry1--;

                    MostSetTimer(&MhpRxBPtr->TimerRx1, MHP_CFG_TO_FRAME, MNS_FALSE);    /* Send Negative Acknowledge and wait for the N-Frame */
                }
                else                                                            /* Timeout and all retries done: */
                {
                    MhpRxCompleteWrap(MHP_RX_ERROR_NFRAME, MhpRxBPtr);

                    MhpSetRxBufferFree(MhpRxBPtr);                              /* no "Null-Frame" received */
                }                                                               /* --> Cancel connection */
            }
            break;


        /*--------------------------------------------------------- */
        case RX_RECEIVE_DATA:
        /*--------------------------------------------------------- */
            if((word)0 == MostGetTimer(&MhpRxBPtr->TimerRx1))                         /* Timeout (t_receive) ? */
            {
                /*------------------------------- */
                /* Prepare "Negative Acknowledge" */
                /*------------------------------- */
                if(NULL != MhpRxPrepTx(MhpRxBPtr))
                {
                    MhpPrepFrAck(MhpRxBPtr, (byte)0x00, (byte)0x00, MNS_FALSE);

                    MhpSendTxTel();
                    MhpRxBPtr->BufRetry = MHP_BUF_RETRY;
                    MhpRxNumFrameFailed = (byte)0;

                    if(MhpRxBPtr->Retry1)                                           /* Timeout, but not all retries done: */
                    {                                                               /* Check Retry: RX_RETRY_NEGACK */
                        MhpRxBPtr->Retry1--;
                        MostSetTimer(&MhpRxBPtr->TimerRx1, MHP_CFG_TO_FRAME, MNS_FALSE);
                        MHP_DBG_RX_STATE(MhpRxBPtr, RX_WAIT_NFRAME);                /* DebugPrint state transition */
                        MhpRxBPtr->State = RX_WAIT_NFRAME;
                        MNS_REQUEST_HIGH_SET(MNSH_P_RX_STATE);                      /* Set Request Flag for calling MostServiceHigh()  */
                    }
                    else                                                            /* Timeout and all retries done: */
                    {
                        MhpRxCompleteWrap(MHP_RX_ERROR_FRAME, MhpRxBPtr);
                        MhpSetRxBufferFree(MhpRxBPtr);                              /* no further frame received */
                    }                                                               /* --> Cancel connection */
                }
                else                                                                /* no tx buffer available */
                {
                    MhpRxCheckBufRetry(MhpRxBPtr);
                }
                break;
            }

            /*------------------------------- */
            /* Prepare command MULTI REQUEST  */
            /*------------------------------- */
            if((word)0 == MostGetTimer(&MhpRxTimerG1))                /* Timeout (t_receive_2) ? */
            {
                num_rx_requests = MHP_MAX_NUM_MULTI_REQ_PCKT;       /* maximum number frames to request */

                if(NULL != MhpRxPrepTx(MhpRxBPtr))
                {
                    if(MhpRxBPtr->Options & MHP_FRAME_ACK)          /* SingleFrAck Mode ? */
                    {
                        num_rx_requests = (byte)0;                  /* avoid Multi-Frame-Request in this mode */
                    }
                    else
                    {
                        num_rx_requests = MhpCollectRxAck(&MhpTxTel.Data[1], num_rx_requests);  /* prepare Data[1]..Data[n] */
                    }

                    if((byte)0 != num_rx_requests)
                    {
                        MhpSetLengthCtrl((word)num_rx_requests + (word)1);
                        MhpTxTel.Data[0] = MHP_CMD_REQUEST_MULTI;       /* HighCmd= FF (Request n frames) */

                        MhpSendTxTel();
                    }
                    else
                    {
                        MhpTxTel.Status   = BUF_FREE;                   /* set telegram buffer free */
                        MhpTxTel.RetryCnt = MHP_TX_TEL_RETRY_MAX;
                    }

                    MhpRxBPtr->BufRetry = MHP_BUF_RETRY;

                    MostSetTimer(&MhpRxTimerG1, MHP_CFG_TO_RX_WAIT_AFTER_REQUEST, MNS_FALSE); /* load timer (t_receive_2) */
                    MhpRxErrCnt = MHP_RX_ERROR_CNT_MAX;                                 /* load error counter */
                    MhpRxFirstRequest = MNS_FALSE;                                      /* reset flag, since first request done */
                }
                else                                                    /* no tx buffer available */
                {
                    MhpRxCheckBufRetry(MhpRxBPtr);
                }
            }

            if((byte)0 == MhpRxFrameCnt)                                          /* Were all frames of current block received ? */
            {
                /*------------------------------- */
                /* All frames were received !     */
                /*------------------------------- */
                if((byte)0 == MhpGetRxAckLowest())
                {
                    MhpRxCompleteWrap(MHP_RX_SUCCESS, MhpRxBPtr);       /* --> Callback Application since all frames received */

                    MHP_DBG_RX_STATE(MhpRxBPtr, RX_GETBUF_LAST_ACK);    /* DebugPrint state transition */
                    MhpRxBPtr->State = RX_GETBUF_LAST_ACK;
                    MNS_REQUEST_HIGH_SET(MNSH_P_RX_STATE);              /* Set Request Flag for calling MostServiceHigh()  */
                }
            }
            break;

        /*--------------------------------------------------------- */
        case RX_GETBUF_LAST_ACK:
        /*--------------------------------------------------------- */
            /*--------------------------------- */
            /* Send last "Frame Acknowledge"    */
            /*--------------------------------- */
            if(NULL != MhpRxPrepTx(MhpRxBPtr))
            {
                switch(MhpRxBPtr->BufFlags)
                {
                    /*---------------------------- */
                    /* Ptr at data buffer is fix   */
                    /*---------------------------- */
                    case MHP_FLAGS_DATA:
                        break;

                    /*---------------------------- */
                    /* Get Ptr by service function */
                    /*---------------------------- */
                    case MHP_FLAGS_FUNC:
                        fkt_ptr  = MhpRxBPtr->BufPtr;
                        data_ptr = (*(byte *(*)(byte val, pTMhpRx p_val))fkt_ptr)(MHP_GETRXPTR_NEXT, MhpRxBPtr);    /*lint !e611 Casts are valid */
                        MhpRxBPtr->DataPtrBlock = data_ptr;
                        break;

                    default:
                        break;
                }

                #ifdef MHP_REV_2_3
                if(NULL == MhpRxBPtr->DataPtrBlock)
                {
                    MhpPrepFrAck(MhpRxBPtr, MhpDelAckFrId, MhpDelAckFrIdLast, MNS_TRUE);
                }
                else
                #endif
                {
                    MhpPrepFrAck(MhpRxBPtr, MhpDelAckFrId, MhpDelAckFrIdLast, MNS_FALSE);
                }

                MhpRxBPtr->BlockCnt++;                                                  /* Increment block counter */
                MhpRxBPtr->BlockCnt &= (byte)0xFF;                                      /* BlockCnt 8bit */
                MhpSendTxTel();
                MhpRxBPtr->BufRetry = MHP_BUF_RETRY;

                #ifdef MHP_REV_2_3
                if(NULL == MhpRxBPtr->DataPtrBlock)
                {
                    MhpRxBPtr->StateBack = RX_GETBUF_NEGACK;            /* Backup State when returning from Hold-Loop */
                    MhpRxBPtr->HoldLoop  = MHP_LOOP_HOLD_MAX_BUF;       /* Counter for Hold-Loop */
                    MhpRxBPtr->HoldEvent = MHP_HOLD_EVENT_BUF;          /* -> Hold the connection, since buffer of application  */

                    MostSetTimer(&MhpRxBPtr->TimerRx1, MHP_CFG_TO_HOLD_RESEND, MNS_FALSE);  /* Load Timeout  */

                    MHP_DBG_RX_STATE(MhpRxBPtr, RX_HOLD);                               /* DebugPrint state transition */
                    MhpRxBPtr->State = RX_HOLD;
                }
                else
                #endif
                {
                    MostSetTimer(&MhpRxBPtr->TimerRx1, MHP_CFG_TO_FRAME, MNS_FALSE);        /* Load Timeout  */
                    MhpRxBPtr->Retry1 = RX_RETRY_NEGACK;
                    MHP_DBG_RX_STATE(MhpRxBPtr, RX_WAIT_NFRAME);                        /* DebugPrint state transition */
                    MhpRxBPtr->State  = RX_WAIT_NFRAME;                                 /* Wait for a new block (with 0-Frame) */
                    MhpRxNumFrameFailed = (byte)0;
                }

                #ifdef MHP_REV_2_3
                /* Update Mhp Connection List */
                MhpRxListDeleteCon(MhpRxBPtrAct);
                MhpRxListInsertCon(MhpRxBPtrAct);
                /* Check for higher priority and force sending  a Null Frame if necessary */
                MhpRxCheckPrio();
                MhpRxBPtrAct = NULL;                                    /* Release Rx Token */
                #else
                MhpRxBPtrAct = NULL;                                    /* Release Rx Token */
                MhpRxCheckPrio();                                       /* Look for higher rx priority */
                #endif

                MNS_REQUEST_HIGH_SET(MNSH_P_RX_STATE);                  /* Set Request Flag for calling MostServiceHigh()  */
            }
            else                                                        /* no tx buffer available */
            {
                MhpRxCheckBufRetry(MhpRxBPtr);
            }
            break;

        /*--------------------------------------------------------- */
        case RX_GETBUF_KILL:
        /*--------------------------------------------------------- */
            /*--------------------------------- */
            /* Send "Kill Connection"           */
            /*--------------------------------- */
            if(NULL != MhpRxPrepTx(MhpRxBPtr))
            {
                MhpTxTel.Data[0] = MHP_CMD_END_RX;                      /* HighCmd= FC (End Connection Rx) */
                MhpTxTel.Data[1] = (byte)0x00;                          /* Dummy */
                MhpTxTel.Data[2] = (byte)0xFF;                          /* Event: Kill */
                MhpSetLengthCtrl((word)3);

                MhpSendTxTel();
                MhpRxBPtr->BufRetry = MHP_BUF_RETRY;

                MhpRxCompleteWrap(MHP_RX_KILLED_RX, MhpRxBPtr);

                MhpSetRxBufferFree(MhpRxBPtr);
            }
            else                                                        /* no tx buffer available */
            {
                MhpRxCheckBufRetry(MhpRxBPtr);
            }
            break;

        /*----------------------------------------------------- */
        case RX_GETBUF_HOLD:
        /*----------------------------------------------------- */
            /*--------------------------------- */
            /* Send "Hold Rx"                   */
            /*--------------------------------- */
            if(NULL != MhpRxPrepTx(MhpRxBPtr))
            {
                MhpTxTel.Data[0]    = MHP_CMD_HOLD_RX;                  /* HighCmd= FE (Hold Rx) */
                MhpTxTel.Data[1]    = MHP_DUMMY_PID;                    /* Dummy */
                MhpTxTel.Data[2]    = MhpRxBPtr->HoldEvent;             /* Event */
                MhpSetLengthCtrl((word)3);

                MhpSendTxTel();
                MhpRxBPtr->BufRetry = MHP_BUF_RETRY;

                MostSetTimer(&MhpRxBPtr->TimerRx1, MHP_CFG_TO_HOLD_RESEND, MNS_FALSE);  /* Load Timeout  */

                MHP_DBG_RX_STATE(MhpRxBPtr, RX_HOLD);                   /* DebugPrint state transition */
                MhpRxBPtr->State = RX_HOLD;

                MNS_REQUEST_HIGH_SET(MNSH_P_RX_STATE);                  /* Set Request Flag for calling MostServiceHigh()  */
            }
            else                                                        /* no tx buffer available */
            {
                MhpRxCheckBufRetry(MhpRxBPtr);
            }
            break;

        /*--------------------------------------------------------- */
        case RX_HOLD:
        /*--------------------------------------------------------- */
            if((word)0 == MostGetTimer(&MhpRxBPtr->TimerRx1))
            {
                if(MhpRxBPtr->HoldLoop)                                 /* not all loops done ? */
                {
                    MhpRxBPtr->HoldLoop--;
                    MHP_DBG_RX_STATE(MhpRxBPtr, RX_GETBUF_HOLD);        /* DebugPrint state transition */
                    MhpRxBPtr->State = RX_GETBUF_HOLD;
                }
                else                                                    /* all loops done ! */
                {                                                       /* -> ready to go into backup state, */
                    if(NULL != MhpRxBPtr->DataPtrBlock)                 /*    but only, if Data Buffer established */
                    {
                        MostSetTimer(&MhpRxBPtr->TimerRx1, MhpRxBPtr->TimerRx1Back, MNS_FALSE);   /* Restore Timer  */
                        MHP_DBG_RX_STATE(MhpRxBPtr, MhpRxBPtr->StateBack);                  /* DebugPrint state transition */
                        MhpRxBPtr->State        = MhpRxBPtr->StateBack;                     /* Restore State */
                        MhpRxBPtr->Status      &= (byte)(~BUF_F_APPL_HOLD);                 /* Clear flag, since connection not longer in hold state */
                    }
                    else                                                /* if no Data Buffer established -> Kill Connection */
                    {
                        MHP_DBG_RX_STATE(MhpRxBPtr, RX_GETBUF_KILL);    /* DebugPrint state transition */
                        MhpRxBPtr->State        = RX_GETBUF_KILL;       /* Kill Connection,  */
                                                                        /* since Application has not established */
                    }                                                   /* any buffer before Timeout  */
                }


                MNS_REQUEST_HIGH_SET(MNSH_P_RX_STATE);                  /* Set Request Flag for calling MostServiceHigh()  */
            }
            break;

        /*--------------------------------------------------------- */
        case RX_GETBUF_NEGACK:
        /*--------------------------------------------------------- */
            /*--------------------------------- */
            /* Send "Negative Acknowledge"      */
            /* after being in Wait-Loop         */
            /*--------------------------------- */
            if(NULL != MhpRxPrepTx(MhpRxBPtr))
            {
                MhpPrepFrAck(MhpRxBPtr, (byte)0x00, (byte)0x00, MNS_FALSE);

                MhpSendTxTel();
                MhpRxBPtr->BufRetry = MHP_BUF_RETRY;
                MhpRxNumFrameFailed = (byte)0;

                MhpRxBPtr->Retry1 = RX_RETRY_NEGACK;
                MostSetTimer(&MhpRxBPtr->TimerRx1, MHP_CFG_TO_FRAME, MNS_FALSE);
                MHP_DBG_RX_STATE(MhpRxBPtr, RX_WAIT_NFRAME);                /* DebugPrint state transition */
                MhpRxBPtr->State = RX_WAIT_NFRAME;
                MNS_REQUEST_HIGH_SET(MNSH_P_RX_STATE);                      /* Set Request Flag for calling MostServiceHigh()  */
            }
            else                                                            /* no tx buffer available */
            {
                MhpRxCheckBufRetry(MhpRxBPtr);
            }
            break;

        /*--------------------------------------------------------- */
        default:
        /*--------------------------------------------------------- */
            #ifdef MHP_24
            MhpSendMdmMhpUnknownTriggerState(MHP_MDM_RX_TRIGGER, MhpRxBPtr->State);     /* Unknown state ==> send debug message */
            #endif

            /* Error Handling */
            MhpRxCompleteWrap(MHP_RX_CLOSE, MhpRxBPtr);             /* -> Reception failed */
            MhpSetRxBufferFree(MhpRxBPtr);                          /*    Set rx buffer free */

            break;

    }

}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                      API */
/* Function    : MhpOfferRxBufCon()                                         */
/* Description : Offer new pointer whenever application release last        */
/*               buffer.                                                    */
/*               The connection is described by "FBlock.Inst.Func.Op".      */
/*                                                                          */
/* Parameter(s): ptr at structure containing Connection Description         */
/*               ptr at data buffer                                         */
/*                                                                          */
/* Returns     : 0x00: ok, buffer grabbed                                   */
/*               0x01: error, floating connection not found                 */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R6
byte  MhpOfferRxBufCon(pTMhpCon con_ptr, byte *data_ptr)   /*lint -esym( 818, con_ptr ) no use of pcTMhpCon in API functions*/
{
    pTMhpRx buf_ptr;
    byte    ret;

    RES_MHP_BEGIN

    ret = (byte)0x01;                                               /* Floating connection not found */

    if(NULL != (buf_ptr = MhpSearchRxBufCon(con_ptr)))              /* Look for corresponding buffer line */
    {
        if(NULL == buf_ptr->DataPtrBlock)
        {
            buf_ptr->DataPtrBlock = data_ptr;

                                                                                        /* A negative Acknowledge must only */
            if( (RX_GETBUF_HOLD == buf_ptr->State) || (RX_HOLD == buf_ptr->State) )     /* be sent, if the Receiver is in active */
            {                                                                           /* hold state */

                buf_ptr->HoldLoop = (byte)0;
                MostClearTimer(&buf_ptr->TimerRx1);                 /* Clear Hold Loop */
                MNS_REQUEST_HIGH_CALL(MNSH_P_RX_TIMEOUT);           /* Request for calling MostServiceHigh()  */
            }

            ret = (byte)0x00;                                       /* Ok */
        }
    }

    RES_MHP_END
    return(ret);
}                                                                   /*lint +esym( 818, con_ptr ) no use of pcTMhpCon in API functions*/
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpSearchFreeRxLine()                                      */
/* Description : Search for a free rx block buffer line.                    */
/* Parameter(s): none                                                       */
/* Returns     : pointer at free Rx buffer                                  */
/*               NULL: -> no buffer line free                               */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R2
static pTMhpRx  MhpSearchFreeRxLine(void) 
{
    pTMhpRx buf_ptr;
    pTMhpRx ret;
    byte    i;

    buf_ptr = MhpRxBuf;
    ret     = NULL;

    for(i=(byte)0; i<(byte)MAX_MHP_RX; i++)
    {
        if(BUF_FREE == buf_ptr->Status)
        {
            buf_ptr->Status = BUF_F_LOCK;       /* lock buffer line */
            ret = buf_ptr;
            break;
        }

        buf_ptr++;                              /* increment buffer pointer */
    }

    return(ret);
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpSetRxBufferFree()                                       */
/* Description : Set certain rx block buffer line free                      */
/*               after connection was finished or error occurred.           */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R3
static void  MhpSetRxBufferFree(pTMhpRx buf_ptr) 
{
    MhpRxListDeleteCon(buf_ptr);                    /* Delete bufferline from the rx con. List */
    buf_ptr->Status         = BUF_FREE;             /* Set buffer line free */
    MHP_DBG_RX_STATE(buf_ptr, RX_NORM_OP);          /* DebugPrint state transition */
    buf_ptr->State          = RX_NORM_OP;           /* Reset State Machine */
    buf_ptr->Priority       = MHP_PRIO_INIT ;       /* Set value to a not used Prio value, smaller as MHP_PRIO_MIN */
    buf_ptr->DataPtrBlock   = NULL;
    buf_ptr->FBlock_ID      = (byte)0x00;
    buf_ptr->HoldEvent      = (byte)0x00;
    buf_ptr->BufRetry       = MHP_BUF_RETRY;        /* Preload retry counter */
                                                    /* (is decremented on each try to get a telegram buffer) */

    if(MhpRxBPtrAct == buf_ptr)                     /* Release token, if connection is closed */
    {
        MhpRxBPtrAct = NULL;
    }

    MostClearTimer(&buf_ptr->TimerRx1);             /* unload timer */

    MhpRxCheckPrio();                               /* Look for another connection */
}
#endif

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpSetBufIntf()                                            */
/* Description :                                                            */
/*                                                                          */
/* Parameter(s): Flags, Ptr at buffer or service function, buffer size      */
/*                  Flags:  0x01 = Ptr points at respecive data buffer      */
/*                          0x02 = Ptr points at service function           */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R4
void  MhpSetBufIntf(byte flags, void *ptr, word buf_size) 
{
    if(MhpPtrBufIntf)
    {
        MhpPtrBufIntf->BufFlags = flags;
        MhpPtrBufIntf->BufPtr   = ptr;
        MhpPtrBufIntf->BufSize  = buf_size;
    }
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpIncRxBPtr()                                             */
/* Description : Increment pointer MhpRxBPtr                                */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R5
static void  MhpIncRxBPtr(void) 
{
    if(&MhpRxBuf[MAX_MHP_RX - 1] == MhpRxBPtr)          /* inc pointer modulo table size */
    {
        MhpRxBPtr = MhpRxBuf;
    }
    else
    {
        MhpRxBPtr++;
    }
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpPrepFrAck()                                             */
/* Description : Prepare a Frame-Acknowledge Protocol                       */
/* Parameter(s): Const pointer to data Rx struct                            */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R10
static void  MhpPrepFrAck(pcTMhpRx buf_ptr, byte frame_id, byte frame_id_last, bool hold) 
{
    byte block_cnt;

    block_cnt = buf_ptr->BlockCnt;

    MhpTxTel.Data[0] = MHP_CMD_ACK;                     /* HighCmd= FA (Frame Acknowledge) */
    MhpTxTel.Data[1] = frame_id_last;                   /* FrAck (High Byte) */
    MhpTxTel.Data[2] = frame_id;                        /* FrAck (Low Byte) */
    MhpTxTel.Data[3] = block_cnt;

#ifdef MHP_REV_2_3
    if(MNS_FALSE != hold)
    {
        MhpTxTel.Data[4] = MHP_IMPLICIT_HOLD_FLAG;      /* signal implicit HOLD command */
        MhpSetLengthCtrl((word)5);
    }
    else
#endif
    {
        MhpSetLengthCtrl((word)4);
    }

    if(((byte)0 == frame_id) && ((byte)0 == frame_id_last))     /* Negative Acknowledge ? */
    {
        if(buf_ptr == MhpRxBPtrAct)
        {
            MhpRxBPtrAct = NULL;                        /* Release Rx Token */
        }

        #ifdef MHP_DBG_RX
        MhpDebugInfo[0] = MHP_DBG_NEGACK_RX;            /* Debug Event: Negative Acknowledge transmitted */
        MhpDebugInfo[1] = block_cnt;                    /* expected block counter */
        MhpDebugInfoRx((pTMhpRx)buf_ptr, &MhpDebugInfo[0], (byte)2);
        #endif /* #ifdef MHP_DBG_RX */
    }
}
#endif





/*
==============================================================================
    Rx-Ack-Bitfield - Section
==============================================================================
*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpPrepareRxAckField()                                     */
/* Description : Set all necessary bits for next block.                     */
/*                                                                          */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R20
static void  MhpPrepareRxAckField(byte frame_last) 
{
    byte i;
    byte x;
    byte y;
    word bitmask;


    /*--------------------------- */
    /* Prepare RxAck BitField     */
    /*--------------------------- */
    x = (byte)(frame_last >> 4) & (byte)0x0F;       /* index of array of 16bit values */
    y = (byte)frame_last & (byte)0x0F;              /* bit position */

    for(i=(byte)0; i<x; i++)
    {
        MhpRxFrAck[i] = (word)0xFFFF;       /* set all bits */
    }

    for(i=x; i<(byte)16; i++)
    {
        MhpRxFrAck[i] = (word)0x0000;       /* clear all remaining lines */
    }

    bitmask = (word)0x0001;

    for(i=(byte)0; i<=y; i++)
    {
        MhpRxFrAck[x] |= bitmask;                   /* set bit = 1 within current index */
        bitmask       =  (word)(bitmask << 1);      /* next bit */
    }
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpClearRxAck()                                            */
/* Description : Clear certain bit in RxAck Bitfield.                       */
/* Parameter(s): FrameID                                                    */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R21
static void  MhpClearRxAck(byte frame_id) 
{
    byte x;
    byte y;
    byte i;
    word bitmask;

    x = (byte)(frame_id >> 4) & (byte)0x0F;     /* index of array of 16bit values */
    y = (byte)frame_id & (byte)0x0F;            /* bit position  */

    bitmask = (word)0x0001;
    for(i=(byte)0; i<y; i++)
    {
        bitmask = (word)(bitmask << 1);         /* prepare bitmask */
    }

    MhpRxFrAck[x] &= (word)(~bitmask);          /* clear respective bit in RxAck Bitfield, since frame received */
}
#endif


/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpGetRxAckLowest()                                        */
/* Description : Get lowest bit, which is not cleared in RxAck Bitfield.    */
/* Parameter(s): none                                                       */
/* Returns     : respective frame_id                                        */
/*               0x00: all bits cleared                                     */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R22
static byte  MhpGetRxAckLowest(void) 
{
    byte x;
    byte y;
    byte ret;
    byte found;
    word bitmask;

    found = MNS_FALSE;
    ret = (byte)0x00;                                   /* All bits cleared */

    for(x=(byte)0; ((x<(byte)16) && (MNS_FALSE == found)); x++)
    {
        if(MhpRxFrAck[x])
        {
            bitmask = (word)0x0001;
            for(y=(byte)0; ((y<(byte)16) && (MNS_FALSE == found)); y++)
            {
                if(MhpRxFrAck[x] & bitmask)
                {
                    ret = (byte)(x << 4) + y;           /* return lowest frame_id, which has not been received */
                    found = MNS_TRUE;
                }

                bitmask = (word)(bitmask << 1);
            }
        }
    }

    return(ret);
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpCheckRxAck()                                            */
/* Description : Check if a certain frame has already been received         */
/* Parameter(s): respective frame id                                        */
/* Returns     : MNS_TRUE:  frame has been received                         */
/*               MNS_FALSE: frame received not yet                          */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R23
static bool  MhpCheckRxAck(byte frame_id) 
{
    byte x;
    byte y;
    byte i;
    word bitmask;
    bool ret;

    x = (byte)(frame_id >> 4) & (byte)0x0F;     /* index of array of 16bit values */
    y = (byte)frame_id & (byte)0x0F;            /* bit position */

    bitmask = (word)0x0001;
    for(i=(byte)0; i<y; i++)
    {
        bitmask = (word)(bitmask << 1);         /* prepare bitmask */
    }

    if(MhpRxFrAck[x] & bitmask)
    {
        ret = MNS_FALSE;                        /* frame not yet received */
    }
    else
    {
        ret = MNS_TRUE;                         /* frame already received */
    }

    return(ret);
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpCollectRxAck()                                          */
/* Description : Collect all frame ids that have been received not yet.     */
/* Parameter(s): pointer at target buffer, maximum number of frame ids      */
/* Returns     : number of prepared frame ids                               */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R24
static byte  MhpCollectRxAck(byte *buf_ptr, byte max_num) 
{
    byte x;
    byte y;
    word bitmask;
    byte num_requests;
    byte frame_id;
    bool found;

    found = MNS_FALSE;
    num_requests = (byte)0;

    for(x=(byte)0; ((x<(byte)16) && (MNS_FALSE == found)); x++)
    {
        if(MhpRxFrAck[x])
        {
            bitmask = (word)0x0001;
            for(y=(byte)0; ((y<(byte)16) && (MNS_FALSE == found)); y++)
            {
                if(MhpRxFrAck[x] & bitmask)
                {
                    if(num_requests < max_num)
                    {
                        frame_id = (byte)(x << 4) + y;

                        if((MNS_FALSE != MhpRxFirstRequest) && (frame_id >= MhpRxHighestFrame))
                        {
                            found = MNS_TRUE;
                        }
                        else
                        {
                            *buf_ptr++ = frame_id;      /* store frame_id in target array */
                            num_requests++;

                            if(num_requests == max_num)
                            {
                                found = MNS_TRUE;
                            }
                        }
                    }
                }
                bitmask = (word)(bitmask << 1);
            }
        }
    }

    return(num_requests);                               /* return number of requested frames */
}
#endif





/*
==============================================================================
    Rx-Prio - Section
==============================================================================
*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpRxGetHighestPrio()                                      */
/* Description : Search for the highest priority in the rx buffer           */
/* Parameter(s): Pointer to const data Rx struct                            */
/* Returns     : highest priority                                           */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R50
static byte  MhpRxGetHighestPrio(pcTMhpRx buf_ptr_req) 
{
    byte    prio_highest;
    byte    prio_buf;
    pTMhpRx buf_ptr;
    byte    ret;

    if( (MHP_PRIO_MAX == buf_ptr_req->Priority) &&
        (FBLOCK_ALL   == buf_ptr_req->FBlock_ID) &&
        (INST_ALL     == buf_ptr_req->Inst_ID) )
    {
        ret = (byte)((byte)0xF0 | buf_ptr_req->Operation);
    }
    else
    {
        buf_ptr      = MhpRxBuf;
        prio_highest = MHP_PRIO_INIT;

        do
        {
            if( (buf_ptr != buf_ptr_req) &&
                (buf_ptr->FBlock_ID == buf_ptr_req->FBlock_ID) &&
                (buf_ptr->Inst_ID   == buf_ptr_req->Inst_ID  ) &&
                (buf_ptr->Func_ID   == buf_ptr_req->Func_ID  ) &&
                (buf_ptr->Operation == buf_ptr_req->Operation) )
            {
                prio_buf = buf_ptr->Priority;

                if(prio_buf > prio_highest)
                {
                    prio_highest = prio_buf;
                }

                if(prio_buf < buf_ptr_req->Priority)
                {
                    buf_ptr->State = RX_GETBUF_KILL;        /* kill connection with lower priority */
                }
            }

        } while(++buf_ptr < &MhpRxBuf[MAX_MHP_RX]);

        ret = prio_highest;
    }

    return(ret);
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpRxCheckPrio()                                           */
/* Description : Search the connection, which has the highest priority      */
/*               and decide if it is necessary to force a Null-Frame        */
/*               by sending a "Negative Acknowledge".                       */
/*                                                                          */
/* Parameter(s): none                                                       */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R51
static void  MhpRxCheckPrio(void) 
{
    pTMhpRx buf_ptr;

    /*--------------------------------------------------------------*/
    /* MHP Spec Rev2.2 or older:                                    */
    /*--------------------------------------------------------------*/
    #ifndef MHP_REV_2_3
    byte prio;

    if(MhpRxBPtrAct)
    {
        prio = MhpRxBPtrAct->Priority;          /* Priority of last token */
    }
    else
    {
        prio = MHP_PRIO_INIT;
    }

    buf_ptr = MhpRxSearchHighestPrio(prio);
    MhpRxForceNegAck(buf_ptr);                  /* Force sending a Negative Ack for this connection */

    /*--------------------------------------------------------------*/
    /* MHP Spec Rev2.3:                                             */
    /*--------------------------------------------------------------*/
    #else
    buf_ptr = MhpRxSearchHighestPrio(MHP_PRIO_INIT);

    /* Just in the case another than the current connection
    has the highest prio force a Negative  Acknowlege */
    if(buf_ptr != MhpRxBPtrAct)
    {
        MhpRxForceNegAck(buf_ptr);
    }
    #endif
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpRxSearchHighestPrio()                                   */
/* Description : Search buffer entry with highest priority                  */
/*               and the furtherst position in the MhpConnectionList.       */
/*                                                                          */
/* Parameter(s): Priority value used to start search for higher             */
/*               priority                                                   */
/* Returns :     Return the pointer to the first entry                      */
/*               in the Mhp Rx Connection list with the highest priority.   */
/*               In the case that no connection is found, null will be      */
/*               returned                                                   */
/*--------------------------------------------------------------------------*/

#ifdef MHP_R52
static pTMhpRx  MhpRxSearchHighestPrio(byte prio_highest) 
{
    byte    prio_buf;
    byte    last_list_entry;
    pTMhpRx buf_ptr;
    pTMhpRx buf_ptr_highest;

    last_list_entry = (byte)0;
    buf_ptr_highest = NULL;
    buf_ptr         = MhpRxListFirstPtr;

    while((byte)0 == last_list_entry)
    {
        if(NULL != buf_ptr)
        {
            if(buf_ptr == MhpRxListLastPtr)             /* Check if this is the last connection */
            {
                last_list_entry = (byte)1;
            }
            prio_buf =  buf_ptr->Priority;              /* Store Priority of the  selected connection */

            /* Check for a  higher Prio, in the case multiple connections have the same value the first in the Mhp Rx Con. List is chosed */
            if(prio_buf > prio_highest)
            {
                if( (NULL != buf_ptr->DataPtrBlock) &&  /* only if Data Buffer established and connection is not in Hold state */
                    ((byte)0x00 == (buf_ptr->Status & (BUF_F_APPL_HOLD | BUF_F_HOLD_BY_TX))) )
                {
                    prio_highest    = prio_buf;         /* store higher priority */
                    buf_ptr_highest = buf_ptr;          /* store pointer at buffer */
                }
            }
            buf_ptr = buf_ptr->NextPtr;                 /* Go to the next entry in the list */
        }
        else
        {
            break;
        }
    }   /* END  while(!last_list_entry) */

    return buf_ptr_highest;
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpRxForceNegAck()                                         */
/* Description : Force a Negative Acknowledge by clearing the timeout       */
/*               and the loop value in the case the connection is in        */
/*               the hold state (RX_GETBUF_HOLD or RX_HOLD).                */
/*                                                                          */
/* Parameter(s): pointer to the according connection                        */
/* Returns :     none                                                       */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#ifdef MHP_R53

static  void MhpRxForceNegAck(pTMhpRx buf_ptr) 
{
    byte state_highest;

    /* Force a Null Frame on the case a other than the actual connection has the highest prio or is firster in the MHP Connection List */
    if(NULL != buf_ptr)                                     /* Pointer to connection valid? */
    {
        if(NULL != buf_ptr->DataPtrBlock)                   /* only if Data Buffer established */
        {
            state_highest = buf_ptr->State;
            if( (RX_GETBUF_HOLD == state_highest) || (RX_HOLD == state_highest) )
            {
                buf_ptr->HoldLoop = (byte)0;
                MostClearTimer(&buf_ptr->TimerRx1);         /* Clear Hold Loop */
                MNS_REQUEST_HIGH_CALL(MNSH_P_RX_TIMEOUT);   /* Request for calling MostServiceHigh() */
            }
        }
    }
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpRxListDeleteCon()                                       */
/* Description : Deletes the corresponing connection from the rx connection */
/*               list.                                                      */
/* Parameter(s): ptr at entry in RX connection list                         */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R54
static  void MhpRxListDeleteCon(pTMhpRx buf_ptr) 
{
    pTMhpRx buf_ptr_temp_next;
    pTMhpRx buf_ptr_temp_previous;


    if( (NULL != buf_ptr) &&
        (NULL != buf_ptr->PreviousPtr) &&
        (NULL != buf_ptr->NextPtr) )
    {
        /* Check if this is not the only connection */
        if( !((MhpRxListFirstPtr == buf_ptr) && (MhpRxListLastPtr == buf_ptr)) )
        {
            if(MhpRxListFirstPtr == buf_ptr)        /* Is this the first entry in the list? */
            {
                MhpRxListFirstPtr = buf_ptr->NextPtr;
            }
            if(MhpRxListLastPtr == buf_ptr)         /* Is this the last entry in the list? */
            {
                MhpRxListLastPtr = buf_ptr->PreviousPtr;
            }

            /* Exchange the pointers */
            buf_ptr_temp_next     = buf_ptr->NextPtr;
            buf_ptr_temp_previous = buf_ptr->PreviousPtr;

            buf_ptr_temp_next->PreviousPtr = buf_ptr->PreviousPtr ;
            buf_ptr_temp_previous->NextPtr = buf_ptr->NextPtr;
        }
        else
        {
            MhpRxListFirstPtr = NULL;
            MhpRxListLastPtr  = NULL;
        }

        /* Clear the Next and Previous pointer as a sign that the bufferline is cleared from the rx connection list */
        buf_ptr->NextPtr     = NULL;
        buf_ptr->PreviousPtr = NULL;
    }

    return;
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpRxListInsertCon()                                       */
/* Description : Inserts a connection to the rx connection list.            */
/* Parameter(s): ptr at entry in RX connection list                         */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R55
static  void MhpRxListInsertCon(pTMhpRx buf_ptr) 
{
    if(NULL != buf_ptr)
    {
        if( (NULL != MhpRxListFirstPtr) && (NULL != MhpRxListLastPtr) )
        {
            buf_ptr->NextPtr     = MhpRxListFirstPtr;
            buf_ptr->PreviousPtr = MhpRxListLastPtr;

            MhpRxListFirstPtr->PreviousPtr = buf_ptr;
            MhpRxListLastPtr->NextPtr      = buf_ptr;

            MhpRxListLastPtr               = buf_ptr;
        }
        else
        {
            MhpRxListFirstPtr    = buf_ptr;
            MhpRxListLastPtr     = buf_ptr;
            buf_ptr->NextPtr     = buf_ptr;
            buf_ptr->PreviousPtr = buf_ptr;
        }
    }
}
#endif





/*
==============================================================================
    RX Connection Section (needed by Receiver)
==============================================================================
*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpRxKillCon()                                         API */
/* Description : Kill existing Connection                                   */
/* Parameter(s): Ptr at structure containing description of respective      */
/*               connection (FBlock.Inst.Func.Op).                          */
/* Returns     : 0x00:  Ok, connection killed                               */
/*               0x01:  respective connection not found                     */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R61
byte  MhpRxKillCon(pTMhpCon con_ptr)       /*lint -esym( 818, con_ptr ) no use of pcTMhpCon in API functions*/
{
    pTMhpRx buf_ptr;
    byte    ret;

    RES_MHP_BEGIN

    ret = (byte)0x01;                                   /* respective connection not found */

    if(NULL != (buf_ptr = MhpSearchRxBufCon(con_ptr)))
    {
        MHP_DBG_RX_STATE(buf_ptr, RX_GETBUF_KILL);      /* DebugPrint state transition */
        buf_ptr->State = RX_GETBUF_KILL;
        MNS_REQUEST_HIGH_CALL(MNSH_P_RX_STATE);         /* Request for calling MostServiceHigh()  */

        ret = (byte)0x00;                               /* Ok */
    }

    RES_MHP_END
    return(ret);
}                                                       /*lint +esym( 818, con_ptr ) no use of pcTMhpCon in API functions*/
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpRxHoldCon()                                         API */
/* Description : Push existing Connection in Wait-State.                    */
/* Parameter(s): Ptr at structure containing description of respective      */
/*               connection (FBlock.Inst.Func.Op), Event                    */
/* Returns     : 0x00:  Ok, connection is now in wait state                 */
/*               0x01:  respective connection not found                     */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R62
byte  MhpRxHoldCon(pTMhpCon con_ptr, byte event)       /*lint -esym( 818, con_ptr ) no use of pcTMhpCon in API functions*/
{
    pTMhpRx buf_ptr;
    byte    state;
    byte    ret;

    RES_MHP_BEGIN

    ret = (byte)0x01;                                           /* respective connection not found */

    if(NULL != (buf_ptr = MhpSearchRxBufCon(con_ptr)))
    {
        state = buf_ptr->State;
        buf_ptr->HoldLoop  = MHP_LOOP_HOLD_MAX_APPL;            /* Counter for Hold-Loop  */
        buf_ptr->HoldEvent = event;

        if( (RX_GETBUF_HOLD != state) && (RX_HOLD != state) )   /* in hold state not yet ? */
        {
            buf_ptr->TimerRx1Back = MostGetTimer(&buf_ptr->TimerRx1);   /* Store Timer */
            buf_ptr->StateBack    = state;                                      /* Store State */
        }

        MHP_DBG_RX_STATE(buf_ptr, RX_GETBUF_HOLD);              /* DebugPrint state transition */
        buf_ptr->State   = RX_GETBUF_HOLD;
        buf_ptr->Status |= BUF_F_APPL_HOLD;                     /* Set flag, since connection must be ignored by prio check */

        MNS_REQUEST_HIGH_CALL(MNSH_P_RX_STATE);                 /* Request for calling MostServiceHigh()  */

        ret = (byte)0x00;                                       /* Ok */
    }

    RES_MHP_END
    return(ret);
}                                                               /*lint +esym( 818, con_ptr ) no use of pcTMhpCon in API functions*/
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpRxContinueCon()                                     API */
/* Description : Continue connection being in Wait-State.                   */
/* Parameter(s): Ptr at structure containing description of respective      */
/*               connection (FBlock.Inst.Func.Op)                           */
/* Returns     : 0x00:  Ok, connection continued                            */
/*               0x01:  respective connection not found                     */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R63
byte  MhpRxContinueCon(pTMhpCon con_ptr)       /*lint -esym( 818, con_ptr ) no use of pcTMhpCon in API functions*/
{
    pTMhpRx buf_ptr;
    byte    ret;

    RES_MHP_BEGIN

    ret = (byte)0x01;                               /* respective connection not found */

    if(NULL != (buf_ptr = MhpSearchRxBufCon(con_ptr)))
    {
        buf_ptr->HoldLoop = (byte)0;
        MostClearTimer(&buf_ptr->TimerRx1);         /* Clear Hold Loop */
        MNS_REQUEST_HIGH_CALL(MNSH_P_RX_TIMEOUT);   /* Request for calling MostServiceHigh()  */

        ret = (byte)0x00;                           /* Ok */
    }

    RES_MHP_END
    return(ret);
}                                                   /*lint +esym( 818, con_ptr ) no use of pcTMhpCon in API functions*/
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpSearchRxBufCon()                                        */
/* Description : Search Rx bufferline for a certain connection              */
/*               The connection is described by "FBlock.Inst.Func.Op"       */
/*               The field "Addr" is optional and is only used, if there    */
/*               are several entries for same function.                     */
/* Parameter(s): Ptr at const structure containing Connection Descr.        */
/* Returns     : ptr at Rx bufferline                                       */
/*               NULL: error, corresponding bufferline not found            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R60
static pTMhpRx  MhpSearchRxBufCon(pcTMhpCon con_ptr) 
{
    pTMhpRx buf_ptr;
    pTMhpRx buf_ptr_single;
    byte    counter;

    counter        = (byte)0x00;
    buf_ptr        = MhpRxBuf;
    buf_ptr_single = NULL;                              /* to avoid compiler warnings */

    do
    {
        if( (buf_ptr->FBlock_ID == con_ptr->FBlock_ID) &&
            (buf_ptr->Inst_ID   == con_ptr->Inst_ID) &&
            (buf_ptr->Func_ID   == con_ptr->Func_ID) &&
            (buf_ptr->Operation == con_ptr->Operation) )
        {
            counter++;
            buf_ptr_single = buf_ptr;

            if(buf_ptr->Src_Adr == con_ptr->Addr)
            {
                return(buf_ptr);                        /* SrcAddr.FBlockID.InstID.FuncID.OpType was found */
            }
        }

    } while(++buf_ptr < &MhpRxBuf[MAX_MHP_RX]);


    if((byte)0x01 == counter)
    {
        return(buf_ptr_single);                         /* FBlockID.InstID.FuncID.OpType was found (Single Entry) */
    }
    else
    {
        return(NULL);                                   /* No entry or several identical entries (Addr = don't care) */
    }                                                   /* have been found */
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpTxCheckBufRetry()                                       */
/* Description : Checks retry counter on each attempt whenever trying       */
/*               to get a telegram buffer on sender side.                   */
/*                                                                          */
/* Parameter(s): ptr at entry in TX connection list                         */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_T80
void  MhpTxCheckBufRetry(pTMhpTx buf_ptr) 
{
    if(buf_ptr->BufRetry)
    {
        buf_ptr->BufRetry--;
    }
    else
    {
        (void)MhpTxStatusWrap(MHP_TX_ERROR_NOBUF, buf_ptr);     /* -> Transmission failed */
        MhpSetTxBufferFree(buf_ptr);                            /*    internal error, no buffer */
    }
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpRxCheckBufRetry()                                       */
/* Description : Checks retry counter on each attempt whenever trying       */
/*               to get a telegram buffer on receiver side.                 */
/*                                                                          */
/* Parameter(s): ptr at entry in RX connection list                         */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_R80
static void  MhpRxCheckBufRetry(pTMhpRx buf_ptr) 
{
    if(buf_ptr->BufRetry)
    {
        buf_ptr->BufRetry--;
    }
    else
    {
        MhpRxCompleteWrap(MHP_RX_ERROR_NOBUF, buf_ptr);     /* -> Receiption failed */
        MhpSetRxBufferFree(MhpRxBPtr);                      /*    internal error, no buffer */
    }
}
#endif





/*
==============================================================================
    Helper Functions Section (needed by Receiver & Transmitter)
==============================================================================
*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpSetLengthCtrl()                                         */
/* Description : Set Data Length for MHP Control Frame.                     */
/* Parameter(s): Data Length (number of bytes)                              */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_23
_INLINE void  MhpSetLengthCtrl(word length) 
{
    MhpTxTel.TelLen = length;
	
	// Bug : Errata sheet 3.0.X-6 - 0300XX-015
	/*MhpTxTel.DataLength = length; REMOVED LINE */
	/*MhpTxTel.DataPtrLength = (word)0; REMOVED LINE */
}
#endif

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpSetLengthData()                                         */
/* Description : Set Data Length for MHP User Data Frame.                   */
/* Parameter(s): Header Data Length (number of bytes)                       */
/*               Payload Data Length (numnber of bytes)                     */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_25
_INLINE void  MhpSetLengthData(word headerLength, word payloadLength) 
{
    MhpTxTel.TelLen = headerLength + payloadLength;

	// Bug : Errata sheet 3.0.X-6 - 0300XX-015
	/*MhpTxTel.DataLength = headerLength; REMOVED LINE */
	/*MhpTxTel.DataPtrLength = payloadLength; REMOVED LINE */
}
#endif





/*
==============================================================================
    Debug Message Functions
==============================================================================
*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpSendMdmMhpSilentTermination()                           */
/* Description : Send debug message "Silent termination of MHP connection". */
/* Parameter(s): status  Error status code                                  */
/*               dir     Direction MHP_MDM_TX_STATUS or MHP_MDM_RX_STATUS   */
/* Returns     : none                                                       */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_26
_INLINE void  MhpSendMdmMhpSilentTermination(byte status, byte dir)
{
    byte tmp[2];

    tmp[0] = status;
    tmp[1] = dir;
    MdmMsgSendMns(MDM_MHP_SILENT_TERMINATION, tmp, (byte)2);
}
#endif



/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : MhpSendMdmMhpUnknownTriggerState()                         */
/* Description : Send debug message "Unknown state in MhpTxTrigger() /      */
/*               MhpRxTrigger()".                                           */
/* Parameter(s): dir  Direction Rx/Tx                                       */
/* Returns     : none                                                       */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#ifdef MHP_24
_INLINE void  MhpSendMdmMhpUnknownTriggerState(byte dir, byte state)
{
    byte tmp[2];

    tmp[0] = dir;
    tmp[1] = state;
    MdmMsgSendMns(MDM_MHP_UNKNOWN_TRIGGER_STATE, tmp, (byte)2);
}
#endif





/*
==============================================================================
    Debug Functions (only active on MHP_DBG_TX and/or MHP_DBG_RX)
==============================================================================
*/

#ifdef MHP_DBG_TX
void  MhpTxDbgPrintState(pTMhpTx buf_ptr, byte new_state) 
{
    if(buf_ptr->State != new_state)
    {
        MhpDebugInfo[0] = MHP_DBG_STATE_TX;
        MhpDebugInfo[1] = buf_ptr->State;
        MhpDebugInfo[2] = new_state;
        MhpDebugInfoTx(buf_ptr, &MhpDebugInfo[0], (byte)3);
    }
}
#endif

#ifdef MHP_DBG_RX
static void  MhpRxDbgPrintState(pTMhpRx buf_ptr, byte new_state) 
{
    if(buf_ptr->State != new_state)
    {
        MhpDebugInfo[0] = MHP_DBG_STATE_RX;
        MhpDebugInfo[1] = buf_ptr->State;
        MhpDebugInfo[2] = new_state;
        MhpDebugInfoRx(buf_ptr, &MhpDebugInfo[0], (byte)3);
    }
}
#endif


