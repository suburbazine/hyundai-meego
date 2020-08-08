/*============================================================================
 * COPYRIGHT (c) 1999-2006 by K2L GmbH
 * All rights reserved
 *
 * ABSTRACT: Implementation of SMSC Netservices (NS) callback functions.
 *			 Implements callbacks of NS layer I, layer II and MH (MOST high).
 *			 Only the callbacks, required by the K2L MOST driver are implemented.
 *			 Customers may implement additional callbacks, either directly in
 *			 in this file or in an adjacent file.
 *			 The implementation interacts with functionality, defined in IMAC.h
 *			 and generic helper objects, defined in NSWCore component.
 *
 * GROUP PREFIX: NSW (Netservices Wrapper)
 *=============================================================================
 *
 * CONTACT:		K2L Software GmbH, Bluecherstr.32, 75177 Pforzheim; info@k2l.de
 *				Tel: 07231/95688-60		Fax: -65
 *
 *=============================================================================
 *
 * HISTORY:
 *
 * Date:		Ver:	Author:	Scope:
 * ----------------------------------------------------------------------------
 * 09.10.2006	1.0		K2L		Initial version
 *
 *============================================================================*/
#include <K2LDebug.h>
#include <K2LSystem.h>
// Include SMSC NetServices main header file as 'extern "C"'.
// C-Programmers often forget to think of us C++ folks.
extern "C"
{
#include <adjust1.h>
#include <adjust2.h>
#include <adjustmh.h>
#include <mostns.h>
}
#include <stdlib.h>
#include <IMAC.h>
#include <NSWCore.h>


#ifdef PACKET_COMMUNICATION_ONLY
extern "C" void LLDSetMostNetEUI48(const uint8_t eui48[]);
extern "C" uint8_t LLDSendTriggerMessage(void);
#define SET_MAC_ADDRESS(x) LLDSetMostNetEUI48(x)
#define SEND_TRIGGER_MESSAGE() LLDSendTriggerMessage()
#else
#define SET_MAC_ADDRESS(x)
#define SEND_TRIGGER_MESSAGE()
#endif


// Layer I callback functions prototypes.
extern "C"
{
static void NSCBL1MostServiceRequest(void);
static void NSCBL1OnInitComplete(void);
static void NSCBL1OnError(TMnsResult code);
static void NSCBL1OnBufferFreed(void);
static word NSCBL1GetTickCount(void);
#ifndef PACKET_COMMUNICATION_ONLY
static byte NSCBL1AMSTxStatus(byte status, TMsgTx *msg_ptr);
static byte NSCBL1AMSTxFilter(TMsgTx *msg_ptr);
static byte NSCBL1AMSRxComplete(TMsgRx *msg_ptr);
static void NSCBL1AMSRxError(byte error, TMsgRx *msg_ptr);
static void NSCBL1VMSVStateChanged(byte state);
static void NSCBL1VMSVOnError(byte error, byte *info_ptr);
static void NSCBL1VMSVOnEvent(byte event, byte *info_ptr);
static void NSCBL1VMSVDiagResult(byte event, byte *info_ptr);
static void NSCBL1VMSVStoreDiagInfo(byte diag_info);
static void NSCBL1VMSVPMIStateChanged(byte state, byte events);
static void NSCBL1SCMBandwidthChanged(byte preset, byte current, byte total, word free, byte packet,  byte used);
#else
static byte NSCBL1WADSRxFilter(TDataRx *msg_ptr);
#endif
}


/** Function to prepare a TNetServicesConfig structure.
  *
  * @param cfg_ptr is a pointer to the config structure to use.
  * @see   InitNetServices
  */
TNetServicesConfig *NetServicesCallbacksInitialize(void)
{
    TNetServicesConfig *cfg_ptr = GetNetServicesConfig();

    if( cfg_ptr == 0 )
        return 0;

	// TODO: goes somewhere else...
    // LLD_Init( &cfg_ptr->lld );

    // general system callbacks
    cfg_ptr->general.most_service_request_fptr = NSCBL1MostServiceRequest;
    cfg_ptr->general.on_init_complete_fptr = NSCBL1OnInitComplete;
    cfg_ptr->general.on_error_fptr = NSCBL1OnError;
    cfg_ptr->general.on_buf_freed_fptr = NSCBL1OnBufferFreed;
    cfg_ptr->general.get_tick_count_fptr = NSCBL1GetTickCount;
#ifdef MNS_TM_ALTERNATIVE
    cfg_ptr->general.next_min_timeout_fptr = mns_next_min_timeout;	// Not required, not defined.
#endif

#ifndef PACKET_COMMUNICATION_ONLY
    cfg_ptr->general.watchdogmode.overwrite = MNS_FALSE;
    cfg_ptr->general.watchdogmode.reset = MNS_FALSE;
    cfg_ptr->general.watchdogmode.timeout = MNS_TM_WD_DISABLE;
    cfg_ptr->general.watchdogmode.auto_shutdown_delay = 0xFFFF;
#endif

    // application (control) message service wrapper callbacks
#ifdef AMS_MIN
    cfg_ptr->msg.tx_status_fptr = NSCBL1AMSTxStatus;
    cfg_ptr->msg.tx_filter_fptr = NSCBL1AMSTxFilter;
    cfg_ptr->msg.rx_complete_fptr = NSCBL1AMSRxComplete;
    cfg_ptr->msg.rx_error_fptr = NSCBL1AMSRxError;
#endif

    // asynchronous (packet) data service wrapper callbacks
#ifdef ADS_MIN
#ifdef MNSV3_3
    cfg_ptr->wads.tx_status_fptr = NULL;
    cfg_ptr->wads.rx_complete_fptr = NULL;
#ifndef PACKET_COMMUNICATION_ONLY
    cfg_ptr->wads.rx_filter_fptr = NULL;
#else
    cfg_ptr->wads.rx_filter_fptr = NSCBL1WADSRxFilter;
#endif
#else
    cfg_ptr->data.tx_status_fptr = NULL;
    cfg_ptr->data.rx_complete_fptr = NULL;
    cfg_ptr->data.rx_filter_fptr = NULL;

#endif
#endif

    // virtual MOST supervisor service callbacks
#ifdef VMSV_MIN
    cfg_ptr->vmsv.msval_state_changed_fptr = NSCBL1VMSVStateChanged;
    cfg_ptr->vmsv.msval_error_fptr = NSCBL1VMSVOnError;
    cfg_ptr->vmsv.msval_event_fptr = NSCBL1VMSVOnEvent;
    cfg_ptr->vmsv.msval_diag_result_fptr = NSCBL1VMSVDiagResult;
#ifndef MNSV3
    cfg_ptr->vmsv.store_diag_info_fptr = NSCBL1VMSVStoreDiagInfo;
#endif
    cfg_ptr->vmsv.pmistate_changed_fptr = NSCBL1VMSVPMIStateChanged;
#endif

#ifdef SCM_MIN
    cfg_ptr->scm.bandwidth_changed_fptr           = NSCBL1SCMBandwidthChanged;
    
    cfg_ptr->scm.packet.mode          = SCM_PM_AUTO;
    cfg_ptr->scm.packet.clock_config  = SCM_PORT_CFG_MLB_1024_FS;

    /* Channels are configured on MICOM (see file ns_cb_l1.c).
     * Changes here have no effect because Linux runs in packet only mode.
     */
    /*
    cfg_ptr->scm.packet.in.address    = 8;
    cfg_ptr->scm.packet.in.bandwidth  = 4;
    cfg_ptr->scm.packet.out.address   = 6;
    cfg_ptr->scm.packet.out.bandwidth = 4;
    */

#endif
    // control message service wrapper
#ifdef CMS_MIN
    cfg_ptr->ctrl.retry_config_adjusted_fptr = ctrl_retry_config_adjusted;
#endif

    return cfg_ptr;
}

//
// MOST NetServices Layer I callbacks
//
extern "C"
{
/** Whenever this callback is called the application should call MostService().
  *
  * @see   MostService
  *
  * Implementation hints: In contrast to older NetServices versions, it is now possible
  * to integrate netservices without any cyclic activation. MostService is only called,
  * after it has been requested by NSCBL1MostServiceRequest().
  * NetServices are also activated whenever:
  * 1. LLD has received a packet and passed it to NetServices.
  * 2. Application messages are being sent to MOST (and thus passed to NetServices).
  * 3. Other NetServices API calls take place (e.g. ScmCreateSocket() etc.)
  * NetServices are free to request MostService() at any time.
  * Since this thread is also responsible for other tasks, it is important to invoke
  * IMAC::RequestService() and not to implement this mechanism locally in NSW.
  *
  */
static void NSCBL1MostServiceRequest(void)
{
	CNetServicesImpl::NSWCoreNetServicesRequestMostService();
}

/** Whenever this callback is called the NetServices have completed their
  * initialization (triggered by InitNetServices() or a re-attach scenario).
  * The application is now allowed to call NetServices API functions (again).
  *
  * @see   InitNetServices, NSCBL1OnError
  */
static void NSCBL1OnInitComplete(void)
{
	CNetServicesImpl::NSWCoreNetServicesAvailable();
    SEND_TRIGGER_MESSAGE();
}

/** Whenever this callback is called the NetServices have detected an error.
  *
  * @param code contains one of the above error codes.
  * @see   InitNetServices, on_init_complete
  */
static void NSCBL1OnError(TMnsResult code)
{
    switch( code )
    {
    case NSR_E_REATTACH:
		/*	Re-attach scenario: The NetServices need to re-attach
		 *  to the INIC. Do not use any NetServices API functions,
		 *  but MostService() and MostTimerIntDiff(). The LLD is
		 *  not affected by this. The application shall wait for
		 *  on_init_complete().
		 */
		K2L_RETAILMSG(K2L_ZONE_ERROR,(K2LDEBUG_TEXT("@@@ INIC: REATTACH\r\n")));
		CNetServicesImpl::NSWCoreNetServicesNotAvailable();
        break;
    case NSR_E_NO_COMM:

		K2L_RETAILMSG(K2L_ZONE_ERROR,(K2LDEBUG_TEXT("@@@ INIC: NO_COMM\r\n")));
		CNetServicesImpl::NSWCoreNetServicesNotAvailable();
        break;
    case NSR_E_INIC_VERSION:
		// An outdated firmware version inside the INIC.
		K2L_RETAILMSG(K2L_ZONE_ERROR,(K2LDEBUG_TEXT("@@@ INIC: VERSION\r\n")));
        K2L_DEBUGCHK( false ); // check your hardware and lld implementation
		CNetServicesImpl::NSWCoreNetServicesNotAvailable();
        break;
	//case NSR_E_INVALID_PRIORITY:
 //       K2L_RETAILMSG(ZONE_ERROR,(K2LDEBUG_TEXT("NSCBL1OnError(NSR_E_INVALID_PRIORITY)\r\n")));
 //       break;
 //   case NSR_E_INVALID_RETRY_CONFIG:
	//	K2L_RETAILMSG(ZONE_ERROR,(K2LDEBUG_TEXT("NSCBL1OnError(NSR_E_INVALID_RETRY_CONFIG)\r\n")));
 //       break;
 //   case NSR_E_INVALID_REMOTE_ACCESS_CONFIG:
	//	K2L_RETAILMSG(ZONE_ERROR,(K2LDEBUG_TEXT("NSCBL1OnError(NSR_E_INVALID_REMOTE_ACCESS_CONFIG)\r\n")));
 //       break;
 //   case NSR_E_BIST_MEMORY:
	//	K2L_RETAILMSG(ZONE_ERROR,(K2LDEBUG_TEXT("NSCBL1OnError(NSR_E_BIST_MEMORY)\r\n")));
 //       break;
 //   case NSR_E_BIST_PCODE:
	//	K2L_RETAILMSG(ZONE_ERROR,(K2LDEBUG_TEXT("NSCBL1OnError(NSR_E_BIST_PCODE)\r\n")));
 //       break;
	case NSR_EI_RESET:
		/* Either the communication has a hardware problem or
		 * the INIC has a serious firmware problem. In the
         * second case a hardware reset of the INIC might help.
	     */
		K2L_RETAILMSG(K2L_ZONE_ERROR,(K2LDEBUG_TEXT("NSCBL1OnError(NSR_EI_RESET)\r\n")));
		// TODO: Reset INIC here? -> Check NetServices manual.
        break;
	case NSR_EI_SYNC_FAILED:
		K2L_RETAILMSG(K2L_ZONE_ERROR,(K2LDEBUG_TEXT("NSCBL1OnError(NSR_EI_SYNC_FAILED)\r\n")));
	    break;
	case NSR_EI_SYNC:
		K2L_RETAILMSG(K2L_ZONE_ERROR,(K2LDEBUG_TEXT("NSCBL1OnError(NSR_EI_SYNC)\r\n")));
        break;
 //   case NSR_E_INVALID_SEG_MODE_CONFIG:
	//	K2L_RETAILMSG(ZONE_ERROR,(K2LDEBUG_TEXT("NSCBL1OnError(NSR_E_INVALID_SEG_MODE_CONFIG)\r\n")));
 //       break;
    default:
        /* unknown error code */
		K2L_RETAILMSG(K2L_ZONE_ERROR,(K2LDEBUG_TEXT("NSCBL1OnError(unknown error code: 0x%08X)\r\n"), code));
		break;
    }
}

static void NSCBL1OnBufferFreed(void)
{
	CNetServicesImpl::NSWCoreOnTxBufferFreed();
}

static word NSCBL1GetTickCount(void)
{
	return static_cast<word>(k2l::platform::system::SystemGetTickCount());
}

/** Whenever this callback is called the NetServices have compeleted the
  * transmission of a control message. Check the status and choose one of the
  * following return values ...
  *
  * MSG_TX_FREE                 The message will automatically be freed.
  *                             (In case of XMIT_SUCCESS/XMIT_NO_ICM or un-
  *                             recoverable errors the only sense-making
  *                             choice).
  *
  * MSG_TX_RETRY                The AMS will retry to send this segment.
  * MSG_TX_POSTPONE             The AMS will not retry. You are considered to
  *                             pass the message pointer to MsgSend() again.
  *                             (In case of segmented messages this means that
  *                             the entire message will be sent again).
  *
  * The application is not required to implement the callback, it is for your
  * information only.
  *
  * @param  status is one of the XMIT_xxx constants explained in the manual.
  *             XMIT_SUCCESS means success, every other status reports an error.
  * @param  msg_ptr is a pointer to the message transmitted.
  * @return MSG_TX_FREE, MSG_TX_RETRY or MSG_TX_POSTPONE
  * @see    MsgGetTxPtrExt, MsgSend, MsgTxRetry, MsgTxPostpone
  */
#ifndef PACKET_COMMUNICATION_ONLY
static byte NSCBL1AMSTxStatus(byte status, TMsgTx *msg_ptr)
{
    return CNetServicesImpl::NSWCoreAMSTxStatus(status, msg_ptr);
}
#endif

/** Whenever this callback is called the NetServices are going to transmit
  * a control message. It can be used for debugging or other filtering purposes.
  *
  * You are not required to implement this callback, but if you choose to do so
  * you have to return one of the following values ...
  *
  * MSG_TX_FILTER_DEFAULT   -> (Default) The message will be sent normaly.
  * MSG_TX_FILTER_CANCEL    -> The message will not be sent but freed.
  * MSG_TX_FILTER_BUSY      -> Since the application is busy, the AMS will not
  *                            yet sent the message but call this callback
  *                            again.
  *
  * @param  msg_ptr is a pointer to the message transmitted.
  * @see    MsgGetTxPtrExt, MsgSend
  */
#ifndef PACKET_COMMUNICATION_ONLY
static byte NSCBL1AMSTxFilter(TMsgTx *msg_ptr)
{
    return MSG_TX_FILTER_DEFAULT;
}
#endif

/** Whenever this callback is called the NetServices have received a control
  * message. The application is meant to handle it inside this callback.
  *
  * You are not required to implement this callback, but if you choose to do so
  * you have to return one of the following values ...
  *
  * MSG_RX_TAKE            -> The application takes the responsibility for
  *                           freeing the message via MsgFreeRxMsg().
  * MSG_RX_FREE            -> (Default) The message will be freed by the AMS.
  * MSG_RX_BUSY            -> The callback will be called again with this
  *                           message. The UsageCnt is untouched.
  * MSG_RX_INC_USAGE_CNT   -> Combination of MSG_RX_TAKE and MSG_RX_BUSY, but
  *                           the UsageCnt is increased. That means that
  *                           MsgFreeRxMsg() has to be called as often as this
  *                           value was returned.
  *
  * @param  msg_ptr is a pointer to the message received.
  * @see    NSCBL1AMSRxError
  */
#ifndef PACKET_COMMUNICATION_ONLY
static byte NSCBL1AMSRxComplete(TMsgRx *msg_ptr)
{
    return MSG_RX_FREE;
}
#endif

/** Whenever this callback is called the NetServices have failed to receive a
  * message.
  *
  * You are not required to implement this callback, it is for your information
  * only.
  *
  * @param  error is one of the MSG_ERR_xxx constants explained in the manual.
  * @param  msg_ptr is a pointer to the segment correspondig to the error if
  *             any, else it is NULL.
  * @see    msg_rx_complete
  */
#ifndef PACKET_COMMUNICATION_ONLY
static void NSCBL1AMSRxError(byte error, TMsgRx *msg_ptr)
{
	K2L_RETAILMSG(ZONE_ERROR,(K2LDEBUG_TEXT("NSCBL1AMSRxError(error code: 0x%08X)\r\n"), error));
}
#endif

#ifndef PACKET_COMMUNICATION_ONLY
static void NSCBL1VMSVStateChanged(byte state)
{
}
#endif

#ifndef PACKET_COMMUNICATION_ONLY
static void NSCBL1VMSVOnError(byte error, byte *info_ptr)
{
    switch( error )
    {
    case MSVAL_ERR_UNLOCK_CRITICAL:
		CNetServicesImpl::NSWCoreOnLongUnlock();
        break;
    case MSVAL_ERR_UNLOCK_SHORT:
		CNetServicesImpl::NSWCoreOnShortUnlock();
        break;
    case MSVAL_ERR_INIT_ERROR:
        CNetServicesImpl::NSWCoreNetServicesNotAvailable();
        break;
    default:
        break;
    }
}
#endif

#ifndef PACKET_COMMUNICATION_ONLY
static void NSCBL1VMSVOnEvent(byte event, byte *info_ptr)
{
    // docu: page 42

    switch( event )
    {
    case MSVAL_E_NETON:
		CNetServicesImpl::NSWCoreNetServicesAvailable();
		CNetServicesImpl::NSWCoreOnNetOn();
        break;

    case MSVAL_E_SHUTDOWN: // net off
		CNetServicesImpl::NSWCoreOnMPR( 1 );
        CNetServicesImpl::NSWCoreOnNetOff();
        break;

    case MSVAL_E_MPRDEL_INC:
    case MSVAL_E_MPRDEL_DEC:
    case MSVAL_E_MPRDEL_EQUAL:
		CNetServicesImpl::NSWCoreOnMPR( info_ptr[1] );
        break;

	// case MSVAL_E_MPR:
    default:
        break;
    }
}
#endif

#ifndef PACKET_COMMUNICATION_ONLY
static void NSCBL1VMSVDiagResult(byte event, byte *info_ptr)
{
	// TODO: Implement or remove
}
#endif

#ifndef PACKET_COMMUNICATION_ONLY
static void NSCBL1VMSVStoreDiagInfo(byte diag_info)
{
	// TODO: Implement or remove
}
#endif

#ifndef PACKET_COMMUNICATION_ONLY
static void NSCBL1VMSVPMIStateChanged(byte state, byte events)
{
	// TODO: Implement or remove
}
#endif

#ifndef PACKET_COMMUNICATION_ONLY
static void NSCBL1SCMBandwidthChanged(byte preset, byte current, byte total, word free, byte packet,  byte used)
{
	CNetServicesImpl::NSWCoreOnBandWidthChanged( packet );
}
#endif

} // extern "C"

//
// MOST NetServices Layer II callbacks
//
extern "C"
{
#ifdef CMD_CB1
byte CmdUnknownFBlockShadow(struct Msg_Tx_Type *tx_ptr, struct Msg_Rx_Type *rx_ptr)
{
    return OP_NO_REPORT;
}
#endif

#ifdef CMD_CB2
// -------------------------------------------------------------------------
// Description : possibility to filter or modify incoming messages
// CB Type     : B
// Parameter(s): rx_ptr - pointer to received nessage
// Parameter(s): tx_ptr - pointer to reserved tx message
// Returns     : information how to handle the rx and tx messages
// -------------------------------------------------------------------------
byte CmdRxFilter(struct Msg_Tx_Type *tx_ptr, struct Msg_Rx_Type *rx_ptr)
{
	return CNetServicesImpl::NSWCoreCmdRxFilter( rx_ptr );

#ifdef ET_MIN
#	error TODO: ET fblock is hooked here.
#endif
}
#endif

#ifdef PACKET_COMMUNICATION_ONLY
static byte NSCBL1WADSRxFilter(TDataRx *msg_ptr)
{
#if 0
    K2L_RETAILMSG(K2L_ZONE_INFO,(K2LDEBUG_TEXT("NSCBL1WADSRxFilter()\n")));
    
    if ( (FBLOCK_NETBLOCK == msg_ptr->Data[0])
         && ((byte)0x00 == msg_ptr->Data[2])
         && ((byte)0x3C == msg_ptr->Data[3])
         && ((byte)0x0A == msg_ptr->Data[5]) )
    {
        byte ni_state = msg_ptr->Data[8];
        byte sys_state = msg_ptr->Data[15];
        
        K2L_RETAILMSG(K2L_ZONE_INFO,(K2LDEBUG_TEXT("NSCBL1WADSRxFilter - State information received (ni_state=0x%02X, sys_state=0x%02X)\n"), ni_state, sys_state));
        
        if (0x00 == ni_state) // NetOff
        {
            CNetServicesImpl::NSWCoreOnMPR( 1 );
            CNetServicesImpl::NSWCoreOnNetOff();
        }
        else                  // NetOn 
        {
            CNetServicesImpl::NSWCoreNetServicesAvailable();
            CNetServicesImpl::NSWCoreOnNetOn();
        }
        
        byte data[1] = { sys_state };
        
        Msg_Rx_Type rx_ptr;
        rx_ptr.FBlock_ID = 0x02; // NWM
        rx_ptr.Inst_ID   = 0x01;
        rx_ptr.Func_ID   = 0xA00; // Configuration
        rx_ptr.Operation = 0x0C; // Status
        rx_ptr.Src_Adr   = 0xFFFF;
        rx_ptr.Length    = 1;
        rx_ptr.Data      = data;
        rx_ptr.Rcv_Type  = MSG_RCV_TYPE_LOGICAL;
        
        CNetServicesImpl::NSWCoreCmdRxFilter(&rx_ptr);
        
        SET_MAC_ADDRESS(&msg_ptr->Data[9]);
    }
#endif
    return DATA_RX_FILTER_DEFAULT;
}
#endif

#ifdef CMD_CB3
byte CmdTxFilter(struct Msg_Tx_Type *tx_ptr, struct Msg_Rx_Type *rx_ptr)
{
    return 0x01;
}
#endif

#ifdef CMD_CB3
void CmdTxNoResult(pTMsgRx rx_ptr)
{
    // nop
}
#endif

#ifdef CMD_CB5
// -------------------------------------------------------------------------
// Description : possibility to filter or modify incoming messages (alternative)
// CB Type     : B
// Parameter(s): rx_ptr - pointer to received nessage
// Parameter(s): tx_ptr - pointer to reserved tx message
// Returns     : information how to handle the rx and tx messages
// -------------------------------------------------------------------------
byte CmdRxFilter2(struct Msg_Tx_Type *tx_ptr, struct Msg_Rx_Type *rx_ptr)
{
	return CNetServicesImpl::NSWCoreCmdRxFilter( rx_ptr );

#ifdef ET_MIN
#	error TODO: ET fblock is hooked here.
#endif
}
#endif

// =========================================================================
// Callback Functions of NetBlock Module

#ifdef NB_CB6
// -------------------------------------------------------------------------
// Description : a request could not be answered directly
// CB Type     : B
// Parameter(s): id     - request ID
// Parameter(s): tx_ptr - reserved Tx message
// Returns     : OP_STATUS or OP_ERROR
// -------------------------------------------------------------------------
byte NbGetDeviceInfo(byte id, struct Msg_Tx_Type *tx_ptr)
{
    byte returnvalue;

    returnvalue = OP_STATUS;

    switch( id )
    {
    case 0xAA:
        // just an example!
        tx_ptr->Data[1] = 0x12; // Fill out just data and length
        tx_ptr->Data[2] = 0x34; // Please note: Data[0] contains ID and is inserted by NetBlock
        tx_ptr->Length  = 3;
        break;

        // insert the ids, which should be handled by your device

    default: // id not available?
        // -> error message will be prepared by the NetServices !
        // you have to return OP_ERROR
        returnvalue = OP_ERROR;
        break;
    }

    return returnvalue;
}
#endif

#ifdef NB_CB1
// -------------------------------------------------------------------------
// Description : method ShutDown is started with parameter "Query"
// CB Type     : B
// Parameter(s): none
// Returns     : suspend result required or not
// -------------------------------------------------------------------------
bool NbShutDown(void)
{
    return 0; // no suspend result required
    // return 1; // suspending
}
#endif

#ifdef NB_CB8
// -------------------------------------------------------------------------
// Description : method ShutDown is started with parameter "Execute"
// CB Type     : A
// Parameter(s): none
// -------------------------------------------------------------------------
void NbShutDownExecute(void)
{
    // save your application signals and wait for light off
}
#endif
} // extern "C"


//
// MOST NetServices MOST High callbacks
//
extern "C"
{
#ifdef MHP_CB1
byte MhpTxStatus(byte status, pTMhpTx tx_ptr)
{
	return CNetServicesImpl::NSWCoreMhpTxStatus( status, tx_ptr );
}
#endif

#ifdef MHP_CB30
bool MhpTxDelay(word delay, pTMhpTx tx_ptr)
{
    uint16_t waitTime = (delay * (uint16_t)1000)/(1000000/MHP_AVRG_INT_RATE_DEFAULT);
    bool status = false;

    /* waiting more than 10 ms synchronously would cause timing issues */
    if (waitTime < 10)
    {
        if(waitTime >= 1)
        {
            k2l::platform::system::ThreadSleep(waitTime);
        }
        status = true;
    }
    else
    {
        status = true;
    }

    return status;
}
#endif

#ifdef MHP_CB31
word MhpTxEstablishDelay(word air_tx, word air_rx, word ratio, pTMhpTx tx_ptr)
{

    // Here you have the possibility to establish the delay window if desired:
    // tx_ptr->MinDelay = ... ;
    // tx_ptr->MaxDelay = ... ;

    // Here you have the possibility to set the start delay value:
    return 0xFFFF; // 0xFFFF: "automatic", start value is calculated by NetServices
}
#endif

#ifdef MHP_CB5
bool MhpGetBufIntf(pTMhpRx buf_ptr)
{
    //void MhpSetBufIntf(byte flags, void* ptr, word buf_size);
    //TODO: Use MhpSetBufIntf() instead of using private fields.
    //TODO: BufSize must be set to valid value here!!!!!

	buf_ptr->BufSize = 0;			// TODO: why?
	buf_ptr->RxHandle[0] = 0xFF;	// TODO: why?
	buf_ptr->BufFlags = MHP_FLAGS_FUNC;
	buf_ptr->BufPtr = (void*)CNetServicesImpl::NSWCoreMhpRxBufferAllocator;
	return 0;
}
#endif

#ifdef MHP_CB2
void MhpRxComplete(byte status, pTMhpRx rx_ptr)
{
	CNetServicesImpl::NSWCoreMhpRxComplete( status, rx_ptr );
}
#endif

#ifdef NB_CB20
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbFBlockInfoGet()                                          */
/* Description : get information on FBlock NetBlock                         */
/* CB Type     : B                                                          */
/* Parameter(s): id              - request ID                               */
/*               answer_prepared - TRUE if NB has prepared the answer       */
/*               tx_ptr          - reserved Tx message                      */
/* Returns     : TRUE   if the request could be serviced                    */
/*               FALSE  if the request could not be serviced                */
/*                                                                          */
/*--------------------------------------------------------------------------*/
bool NbFBlockInfoGet(word id, bool answer_prepared, pTMsgTx tx_ptr)
{

    if (answer_prepared)
    {
        return(MNS_TRUE);
    }
    else
    {
        return(MNS_FALSE);
    }

}
#endif

#ifdef NB_CBS2
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbFBlockIDsStatus()                                        */
/* Description : NetBlock.FBlockIDs.Status message was received             */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received status message                 */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbFBlockIDsStatus(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBE2
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbFBlockIDsError()                                         */
/* Description : NetBlock.FBlockIDs.Error message was received              */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received error message                  */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbFBlockIDsError(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBS3
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbDeviceInfoStatus()                                       */
/* Description : NetBlock.DeviceInfo.Status message was received            */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received status message                 */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbDeviceInfoStatus(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBE3
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbDeviceInfoError()                                        */
/* Description : NetBlock.DeviceInfo.Error message was received             */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received error message                  */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbDeviceInfoError(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBS4
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbGroupAddrStatus()                                        */
/* Description : NetBlock.GroupAddress.Status message was received          */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received status message                 */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbGroupAddrStatus(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBE4
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbGroupAddrError()                                         */
/* Description : NetBlock.GroupAddress.Error message was received           */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received error message                  */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbGroupAddrError(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBS7
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbShutDownResult()                                         */
/* Description : NetBlock.ShutDown.Result message was received              */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received status message                 */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbShutDownResult(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
/*----------------------------------------------------------------------------- */
#ifdef NB_CBE7
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbShutDownError()                                          */
/* Description : NetBlock.ShutDown.Error message was received               */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received error message                  */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbShutDownError(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBS9
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbNodePositionStatus()                                     */
/* Description : NetBlock.NodePosition.Status message was received          */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received status message                 */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbNodePositionStatus(pTMsgRx prbuf)
{
}
#endif


/*----------------------------------------------------------------------------- */
#ifdef NB_CBE9
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbNodePositionError()                                      */
/* Description : NetBlock.NodePosition.Error message was received           */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received error message                  */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbNodePositionError(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBS10
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbRetryParametersStatus()                                  */
/* Description : NetBlock.RetryParameters.Status message was received       */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received status message                 */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbRetryParametersStatus(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBE10
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbRetryParametersError()                                   */
/* Description : NetBlock.RetryParameters.Error message was received        */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received status message                 */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbRetryParametersError(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBS11
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbSamplingFrequencyStatus()                                */
/* Description : NetBlock.SamplingFrequency.Status message was received     */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received status message                 */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbSamplingFrequencyStatus(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBE11
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbSamplingFrequencyError()                                 */
/* Description : NetBlock.SamplingFrequency.Error message was received      */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received status message                 */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbSamplingFrequencyError(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBS14
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbBoundaryStatus()                                         */
/* Description : NetBlock.Boundary.Status message was received              */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received status message                 */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbBoundaryStatus(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBE14
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbBoundaryError()                                          */
/* Description : NetBlock.Boundary.Error message was received               */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received error message                  */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbBoundaryError(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBS15
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbVersionStatus()                                          */
/* Description : NetBlock.Version.Status message was received               */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received status message                 */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbVersionStatus(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBE15
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbVersionError()                                           */
/* Description : NetBlock.Version.Error message was received                */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received error message                  */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbVersionError(pTMsgRx prbuf)
{
}
#endif

/*--------------------------------------------------------------------------*/
#ifdef NB_CBS16
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbShutDownReasonStatus()                                   */
/* Description : NetBlock.ShutDownReason.Status message was received        */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received status message                 */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbShutDownReasonStatus(pTMsgRx prbuf)
{
}
#endif

/*--------------------------------------------------------------------------*/
#ifdef NB_CBE16
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbShutDownReasonError()                                    */
/* Description : NetBlock.ShutDownReason.Error message was received         */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received error message                 */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbShutDownReasonError(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBS17
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbFBlockInfoStatus()                                       */
/* Description : NetBlock.FBlockInfo.Status message was received            */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received status message                 */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbFBlockInfoStatus(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBE17
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbFBlockInfoError()                                        */
/* Description : NetBlock.FBlockInfo.Error message was received             */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received error message                  */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbFBlockInfoError(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBS18
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbImplFBlocksStatus()                                       */
/* Description : NetBlock.ImplFBlocks.Status message was received            */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received status message                 */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbImplFBlocksStatus(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBE18
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbImplFBlocksError()                                        */
/* Description : NetBlock.ImplFBlocks.Error message was received             */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received error message                  */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbImplFBlocksError(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBS19
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbEUI48Status()                                            */
/* Description : NetBlock.EUI48.Status message was received                 */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received status message                 */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbEUI48Status(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBE19
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbEUI48Error()                                             */
/* Description : NetBlock.EUI48.Error message was received                  */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received error message                  */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbEUI48Error(pTMsgRx prbuf)
{
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CB17
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbShutDownDevice()                                         */
/* Description : method ShutDown is started with parameter "DeviceShutdown" */
/* CB Type     : A                                                          */
/* Parameter(s): none                                                       */
/* Returns     : MNS_TRUE  - DeviceShutdown will be done                    */
/*               MNS_FALSE - DeviceShutdown is denied                       */
/*                                                                          */
/*--------------------------------------------------------------------------*/
bool NbShutDownDevice(void)
{
    /* perform a device shutdown */
    return (MNS_FALSE);
}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBS1
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbNodeAddrStatus()                                         */
/* Description : NetBlock.NodeAddress.Status message was received           */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received status message                 */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbNodeAddrStatus(pTMsgRx prbuf)
{
  byte data = MostGetNodePos();

}
#endif

/*----------------------------------------------------------------------------- */
#ifdef NB_CBE1
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* Function    : NbNodeAddrError()                                          */
/* Description : NetBlock.NodeAddress.Error message was received            */
/* CB Type     : C                                                          */
/* Parameter(s): prbuf - pointer to received error message                  */
/* Returns     : nothing                                                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
void NbNodeAddrError(pTMsgRx prbuf)
{
}
#endif

} // extern "C"

//
// MOST NetServices miscellaneous callbacks
//
extern "C"
{
#ifdef MNS_DEBUG
void
sys_mns_trace
	( int service
	, int event
	, int pcnt
	, ...
	)
{
	//va_list ap;
 //   int cnt = 0;
	//static TCHAR tszBuffer[20000];
	//static TCHAR tszParam[50];

	//if( ZONE_ACTIVE(ZONE_HELPER) )
	//{
	//	_stprintf( tszBuffer, K2LDEBUG_TEXT("MNS(%d,%d): "), service, event );

	//	va_start(ap, pcnt);
	//	for( cnt = 0; cnt < pcnt; cnt++ )
	//	{
	//		_stprintf(tszParam,K2LDEBUG_TEXT("%d "), va_arg( ap, int ) );
	//		_tcscat( tszBuffer, tszParam );
	//	}
	//	// K2L_RETAILMSG(ZONE_HELPER,(tszBuffer));
	//	va_end(ap);
	//}
}
#endif
}
