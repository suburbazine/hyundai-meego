#include <K2LDebug.h>
#include <K2LSystem.h>
#include <LLDStub.h>
#include <LLD.h>
#include <MOSTDebugZones.h>
#include <K2LMOSTHigh.h>
#include <memory.h>
#include <stddef.h>

//Cause string are always so platform dependant
#ifndef WIN32
#include <stdio.h>
#define LPCTSTR const char*
#define TCHAR char
#define _stprintf sprintf
#define _tcscat strcat
#else
#include <windows.h>
#endif


#define SEND_BUFFER_SIZE 1500

//TODO: Following definitions should be removed
#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif 

static void LLDStubOnStartInterface(void);
static void LLDStubOnStopInterface(void);
static void LLDStubOnBufferAvailable(void);
static void LLDStubResetINIC(void);
static uint8_t LLDStubTransmitEx(HMBMBUF handle, uint8_t *add_ptr, uint16_t add_length, eLLDTransport transportType);
static uint8_t LLDStubTransmit(HMBMBUF handle, uint8_t *add_ptr, uint16_t add_length);
static uint8_t LLDStubControlTransmit(HMBMBUF handle, uint8_t *add_ptr, uint16_t add_length);
static uint8_t LLDStubAsyncTransmit(HMBMBUF handle, uint8_t *add_ptr, uint16_t add_length);
static void LLDStubDumpMessage(LPCTSTR tszPrefix, const uint8_t *cpucMessage, uint32_t dwLength );
static void LLDStubDumpRxMessage( HMBMBUF rx_handle );

// This Flag implementation is caused by oasis code review. See TestTrack Item 10 of the ComBox Project.
static bool bLLDBuffersAvailable = false;


typedef struct LLDStubDiagnostics_tag
{	uint32_t dwSendBufferUsageCount;
	uint32_t dwTransmitFailCount;
	uint32_t dwTransmitCount;
	uint32_t dwReceiveCount;
	uint32_t dwNoPmsBufferCount;
	uint32_t dwInterruptCount;
} LLDStubDiagnostics_t;

static struct TLLDStubContext_tag
{	IMAC *					pMAC;
	eLLDInterface   		interfaceType;
	uint8_t					SendBuffer[SEND_BUFFER_SIZE];
	LLDStubDiagnostics_t	counters;
    HMBMBUF                 lastCTLPendingTxHandle;
    HMBMBUF                 lastATXPendingTxHandle;
} s_ctxLLDStub;

static __inline void DiagInitCounters(void)
{
	s_ctxLLDStub.counters.dwSendBufferUsageCount = 0UL;
	s_ctxLLDStub.counters.dwTransmitFailCount = 0UL;
	s_ctxLLDStub.counters.dwTransmitCount = 0UL;
	s_ctxLLDStub.counters.dwReceiveCount = 0UL;
	s_ctxLLDStub.counters.dwNoPmsBufferCount = 0UL;
	s_ctxLLDStub.counters.dwInterruptCount = 0UL;
}

static __inline void DiagTraceCounters(void)
{
	K2L_RETAILMSG(ZONE_LLD,
		(K2LDEBUG_TEXT("SBUsage: %d TxFail: %d Tx: %d Rx: %d NoPmsBuffer: %d"),
		  s_ctxLLDStub.counters.dwSendBufferUsageCount
		, s_ctxLLDStub.counters.dwTransmitFailCount
		, s_ctxLLDStub.counters.dwTransmitCount
		, s_ctxLLDStub.counters.dwReceiveCount
		, s_ctxLLDStub.counters.dwNoPmsBufferCount
		));
}

static __inline void DiagUseSendBuffer(void)
{
	++s_ctxLLDStub.counters.dwSendBufferUsageCount;
}

static __inline void DiagTransmitFailed(void)
{
	++s_ctxLLDStub.counters.dwTransmitFailCount;
	DiagTraceCounters();
}

static __inline void DiagTransmit(void)
{
	++s_ctxLLDStub.counters.dwTransmitCount;
}

static __inline void DiagReceive(void)
{
	++s_ctxLLDStub.counters.dwReceiveCount;
}

static __inline void DiagNoPmsBuffer(void)
{
	++s_ctxLLDStub.counters.dwNoPmsBufferCount;
	DiagTraceCounters();
}

static __inline void DiagInterrupt(void)
{
	++s_ctxLLDStub.counters.dwInterruptCount;
}

eLLDInterface LLDStubGetInterfaceType(void)
{
   return (s_ctxLLDStub.interfaceType);
}

LLDStubResult_t
LLDStubInitialize
	( IMAC * pMAC
	, struct TNetServicesConfig::TLldConfig *lld_cfg_ptr
	)
{
	LLDStubResult_t result = LLDSTUB_RESULT_FAIL;
	DiagInitCounters();

	s_ctxLLDStub.pMAC = pMAC;
	s_ctxLLDStub.interfaceType = LLD_INTERFACE_ERROR;
	bLLDBuffersAvailable = false;

	if( LLDInitialize(&s_ctxLLDStub.interfaceType) )
	{
		result = LLDSTUB_RESULT_OK;

		switch( s_ctxLLDStub.interfaceType )
		{
		case LLD_INTERFACE_I2C:
			{
#ifdef MNSV3
				lld_cfg_ptr->start_interfaces_fptr = LLDStubOnStartInterface;
#ifdef MNSV3_3
				lld_cfg_ptr->stop_interfaces_fptr = LLDStubOnStopInterface;
#endif
				lld_cfg_ptr->reset_fptr = LLDStubResetINIC;
				lld_cfg_ptr->on_buf_freed_fptr = LLDStubOnBufferAvailable;
				lld_cfg_ptr->i2c_tx_fptr = LLDStubTransmit;
				lld_cfg_ptr->ctrl_tx_fptr = 0;
				lld_cfg_ptr->data_tx_fptr = 0;

				lld_cfg_ptr->ctrl_interface_id = PMS_IFACE_I2C;
				lld_cfg_ptr->data_interface_id = PMS_IFACE_I2C;
#else
				lld_cfg_ptr->start_interfaces_fptr = LLDStubOnStartInterface;
				lld_cfg_ptr->reset_fptr = LLDStubResetINIC;
				lld_cfg_ptr->on_buf_freed_fptr = LLDStubOnBufferAvailable;
				lld_cfg_ptr->i2c_tx_fptr = LLDStubTransmit;
				lld_cfg_ptr->mlb_ctrl_tx_fptr = NULL;
				lld_cfg_ptr->mlb_data_tx_fptr = NULL;

				lld_cfg_ptr->icm_interface_id = PMS_IFACE_I2C;
				lld_cfg_ptr->mcm_interface_id = PMS_IFACE_I2C;
				lld_cfg_ptr->mdp_interface_id = PMS_IFACE_I2C;
#endif
			}
			break;
		case LLD_INTERFACE_MLB:
			{
#ifdef MNSV3
				lld_cfg_ptr->start_interfaces_fptr = LLDStubOnStartInterface;
#ifdef MNSV3_3
				lld_cfg_ptr->stop_interfaces_fptr = LLDStubOnStopInterface;
#endif
				lld_cfg_ptr->reset_fptr = LLDStubResetINIC;
				lld_cfg_ptr->on_buf_freed_fptr = LLDStubOnBufferAvailable;
				lld_cfg_ptr->i2c_tx_fptr = 0;
				lld_cfg_ptr->ctrl_tx_fptr = LLDStubControlTransmit;
				lld_cfg_ptr->data_tx_fptr = LLDStubAsyncTransmit;

				lld_cfg_ptr->ctrl_interface_id = PMS_IFACE_MLB;
				lld_cfg_ptr->data_interface_id = PMS_IFACE_MLB;
#else
				lld_cfg_ptr->start_interfaces_fptr = LLDStubOnStartInterface;
				lld_cfg_ptr->reset_fptr = LLDStubResetINIC;
				lld_cfg_ptr->on_buf_freed_fptr = LLDStubOnBufferAvailable;
				lld_cfg_ptr->i2c_tx_fptr = NULL;
				lld_cfg_ptr->mlb_ctrl_tx_fptr = LLDStubControlTransmit;
				lld_cfg_ptr->mlb_data_tx_fptr = LLDStubAsyncTransmit;

				lld_cfg_ptr->icm_interface_id = PMS_IFACE_MLB;
				lld_cfg_ptr->mcm_interface_id = PMS_IFACE_MLB;
				lld_cfg_ptr->mdp_interface_id = PMS_IFACE_MLB;
#endif
			}
			break;
		case LLD_INTERFACE_ERROR:
			{
				K2L_RETAILMSG(K2L_ZONE_ERROR,(K2LDEBUG_TEXT("ERROR: LLD did not specify the interface type!")));
			}
			break;
		default:
			{
				K2L_RETAILMSG(K2L_ZONE_ERROR,(K2LDEBUG_TEXT("ERROR: unknown interface type!")));
			}
			break;
		}
	}

	return result;
}

void
LLDStubTerminate
	(
	)
{
	LLDDisable();
	LLDTerminate();
	s_ctxLLDStub.pMAC = NULL;
}

void
LLDStubServiceInterrupt
	(
	)
{
	if( !LLDServiceInterrupt() )
	{
		K2L_RETAILMSG( K2L_ZONE_WARNING, (K2LDEBUG_TEXT("LLDServiceInterrupt() failed!\r\n")));
	}
}

void
LLDStubInterruptDone
	(
	)
{
	if( !LLDInterruptDone() )
	{
		K2L_RETAILMSG( K2L_ZONE_WARNING, (K2LDEBUG_TEXT("LLDInterruptDone() failed!\r\n")));
	}
}

static void LLDStubOnStartInterface(void)
{
	bLLDBuffersAvailable = true;
	if( !LLDEnable() )
	{
		K2L_RETAILMSG(K2L_ZONE_WARNING,(K2LDEBUG_TEXT("LLDEnable() failed.")));
	}
}

static void LLDStubOnStopInterface(void)
{
	// may be incompatible with LLD for some platform,
	// has to be virified for all platforms

	// the main idea of this callback is to tell the application
	// not to use the NetServices API,
	// so the driver shouldn't shutdown by the call of LLDDisable()

	if( !LLDDisable() )
	{
		K2L_RETAILMSG(K2L_ZONE_WARNING,(K2LDEBUG_TEXT("LLDDisable() failed.")));
	}
	bLLDBuffersAvailable = false;
}

static void LLDStubOnBufferAvailable(void)
{
    // first set variable then call function:
    // the bLLDBuffersAvailable is used in the callback LLDStubGetPacketBuffer()
    bLLDBuffersAvailable = true;
    LLDServiceInterrupt();
}

static void LLDStubResetINIC(void)
{
	if( !LLDResetINIC() )
	{
		K2L_RETAILMSG( K2L_ZONE_WARNING, (K2LDEBUG_TEXT("INIC Reset NOT IMPLEMENTED!\r\n")));
	}
}

static
uint8_t
LLDStubTransmitEx
( HMBMBUF handle
 , uint8_t *add_ptr
 , uint16_t add_length
 , eLLDTransport	transportType
 )
{
    uint8_t ucResult = 0;
    uint32_t totalDataLength = 0;
    uint8_t* pSendBuffer = NULL;

    {
        uint8_t * pData = MbmGetBufData( handle );
        uint32_t dataLen = MbmGetBufLength( handle );
        K2L_DEBUGCHK(NULL != pData);
        if ((NULL != pData) &&
            (dataLen > 0))
        {
            //Data length without extended data
            totalDataLength = dataLen;

            //Add add data if existing
            if (add_ptr != NULL &&
                add_length > 0)
            {
                //Copy data and ext_data into s_ctxLLDStub.SendBuffer.
                memcpy(s_ctxLLDStub.SendBuffer, pData, dataLen);
                K2L_DEBUGCHK(add_length > 0);
                memcpy(s_ctxLLDStub.SendBuffer + dataLen, add_ptr, add_length);
                totalDataLength += add_length;
                pSendBuffer = s_ctxLLDStub.SendBuffer;
            }
            else
            {
                //Do not copy data into s_ctxLLDStub.SendBuffer, but take pointer from NS
                pSendBuffer = pData;
            }

            DiagTransmit();

            if (transportType == LLD_TRANSPORT_ASYNC_CHANNEL)
            {
            	LLDStubDumpMessage( K2LDEBUG_TEXT("t:"), pSendBuffer, totalDataLength );
                s_ctxLLDStub.lastATXPendingTxHandle = handle;
            }
            else
            {
            	LLDStubDumpMessage( K2LDEBUG_TEXT("T:"), pSendBuffer, totalDataLength );
                s_ctxLLDStub.lastCTLPendingTxHandle = handle;
            }

            if( LLDTransmitPacket(
                pSendBuffer,
                totalDataLength,
                transportType) )
            {
                //VERY IMPORTANT NOTE:
                //Please note that buffer is not immediatelly release. This is intentionally.
                //When buffer is released this is indication that it is already sent on HW.
                //That means NS think we have enough space in our HW buffer is we release NS
                //bufer.

                ucResult = 1;
            }
            else
            {
                DiagTransmitFailed();
                if (transportType == LLD_TRANSPORT_ASYNC_CHANNEL)
                {
                    s_ctxLLDStub.lastATXPendingTxHandle = NULL;
                }
                else
                {
                    s_ctxLLDStub.lastCTLPendingTxHandle = NULL;
                }
            }
        }
    }

    return ucResult;
}

static uint8_t	LLDStubTransmit(HMBMBUF handle, uint8_t *add_ptr, uint16_t add_length)
{
	return LLDStubTransmitEx( handle, add_ptr, add_length, LLD_TRANSPORT_ANY );
}

// MLB control channel message case (uses LLD_TRANSPORT_CONTROL_CHANNEL as message type)
static
uint8_t
LLDStubControlTransmit
	( HMBMBUF handle
	, uint8_t *add_ptr
	, uint16_t add_length
	)
{
	return LLDStubTransmitEx( handle, add_ptr, add_length, LLD_TRANSPORT_CONTROL_CHANNEL );
}

// MLB async channel message case (uses LLD_TRANSPORT_ASYNC_CHANNEL as message type)
static
uint8_t
LLDStubAsyncTransmit
	( HMBMBUF handle
	, uint8_t *add_ptr
	, uint16_t add_length
	)
{
	return LLDStubTransmitEx( handle, add_ptr, add_length, LLD_TRANSPORT_ASYNC_CHANNEL );
}

// ----------------------------------------------------------
// API provided by MOST driver LLD_STUB, to be used by LLD
// implementations.
// All functions, defined in this section except for LLDStubOnInterrupt() MUST be called
// from the MOST drivers thread context.
// ----------------------------------------------------------
extern
uint8_t
LLDStubGetPacketBuffer
	( uint32_t dwPacketLength
	, LLDPacketBufferHandle_t *phPacketBuffer
	, uint8_t **ppucPacketBuffer
	)
{
    struct profiler {
        profiler() {
            _start_time = k2l::platform::system::SystemGetTickCount();
        }
        ~profiler() {
            uint32_t const time_diff = k2l::platform::system::SystemGetTickCount() - _start_time;
#if defined K2LDEBUG
	    if (time_diff > 5)
	    {
                K2L_DEBUGMSG( K2L_ZONE_INFO, (K2LDEBUG_TEXT( "LLDStubGetPacketBuffer() took %d ms" ), time_diff) );
            }
#endif
        }
        uint32_t _start_time;
    };

    struct profiler _;

	uint8_t bSuccess = FALSE;

	K2L_DEBUGCHK(NULL != phPacketBuffer );
	K2L_DEBUGCHK(NULL != ppucPacketBuffer );

	if( !bLLDBuffersAvailable )
	{
		// don't even try to call PmsGetRxBuf()
		// it can cause the NetServices deadlock
		return FALSE;
	}

	if( (NULL != ppucPacketBuffer) && (NULL != phPacketBuffer) )
	{
		HMBMBUF rx_handle;

		(*ppucPacketBuffer) = NULL;

		rx_handle = PmsGetRxBuf( (uint16_t)dwPacketLength );
		if( rx_handle )
		{
			(*ppucPacketBuffer) = MbmGetBufData( rx_handle );
			K2L_DEBUGCHK( NULL != (*ppucPacketBuffer) );
			(*phPacketBuffer) = (LLDPacketBufferHandle_t)rx_handle;

			bSuccess = TRUE;
		}
		else
		{
			bLLDBuffersAvailable = false;
			DiagNoPmsBuffer();
		}
	}

	return bSuccess;
}

extern
uint8_t
LLDStubOnPacketReceived
	( LLDPacketBufferHandle_t hPacketBuffer
	)
{
	//uint32_t dwStart = k2l::platform::system::SystemGetTickCount();

	uint8_t bSuccess = FALSE;
	HMBMBUF rx_handle = (HMBMBUF)hPacketBuffer;

	K2L_DEBUGCHK(0 != rx_handle);
	if( rx_handle )
	{
		LLDStubDumpRxMessage(rx_handle);
		PmsRx( rx_handle );
		DiagReceive();
		bSuccess = TRUE;
	}
	else
	{
		K2L_RETAILMSG(K2L_ZONE_WARNING,(K2LDEBUG_TEXT("LLDStubOnPacketReceived() rx_handle is 0")));
	}

	//uint32_t dwEnd = k2l::platform::system::SystemGetTickCount();
	//uint32_t dwDiff = dwEnd - dwStart;
	//K2L_RETAILMSG(5 < dwDiff,(K2LDEBUG_TEXT("LLDStubOnPacketReceived() took %d ms"), dwDiff));

	return bSuccess;
}

// Called from the IST of INIC interrupt.
extern
uint8_t
LLDStubOnInterrupt
	( void
	)
{
	DiagInterrupt();
	K2L_RETAILMSG(ZONE_IST,(K2LDEBUG_TEXT("INIC_IST")));
	K2L_DEBUGCHK(NULL != s_ctxLLDStub.pMAC);
	if( NULL != s_ctxLLDStub.pMAC )
	{
		s_ctxLLDStub.pMAC->OnInterrupt();
	}
	return TRUE;
}

extern
void
LLDStubReleaseTxBuffer(const LLDStubBuffer_t buffer)
{
    switch(buffer)
    {
    case LLDSTUB_BUFFER_CTL:
	    if (NULL != s_ctxLLDStub.lastCTLPendingTxHandle)
	    {
		    PmsTxRelease(s_ctxLLDStub.lastCTLPendingTxHandle);
		    s_ctxLLDStub.lastCTLPendingTxHandle = NULL;
	    }
        break;
    case LLDSTUB_BUFFER_ATX:
        if (NULL != s_ctxLLDStub.lastATXPendingTxHandle)
	    {
		    PmsTxRelease(s_ctxLLDStub.lastATXPendingTxHandle);
		    s_ctxLLDStub.lastATXPendingTxHandle = NULL;
	    }
        break;
    default:
        break;
    }

}

// ----------------------------------------------------------
// LLD message trace support.
// ----------------------------------------------------------

#ifdef DEBUG
#define TRAFFIC_TRACE
#define ZONES_TRAFFIC_TRACE ZONE_RECEIVE || ZONE_TRANSMIT
#else
// TODO: remove me after testing
//#define TRAFFIC_TRACE
//#define ZONES_TRAFFIC_TRACE TRUE
#endif


static void LLDStubDumpMessage(LPCTSTR tszPrefix, const uint8_t *cpucMessage, uint32_t dwLength )
{
#ifdef TRAFFIC_TRACE
	static TCHAR buffer[1024];
	if( ZONES_TRAFFIC_TRACE )
	{
		TCHAR hexvalue[15];
		uint32_t dwIndex;

		buffer[0] = 0;
		_stprintf(buffer, K2LDEBUG_TEXT("%d "), k2l::platform::system::SystemGetTickCount() );
		_tcscat( buffer, tszPrefix );
		_tcscat( buffer, K2LDEBUG_TEXT(" ") );

		for( dwIndex = 0; dwIndex < dwLength; ++dwIndex )
		{
			_stprintf( hexvalue, K2LDEBUG_TEXT("%02X "), cpucMessage[dwIndex] );
			_tcscat( buffer, hexvalue );
		}
		_tcscat( buffer, K2LDEBUG_TEXT("\r\n") );

		K2L_RETAILMSG( ZONES_TRAFFIC_TRACE, (buffer) );
	}
#endif
}

static void LLDStubDumpRxMessage( HMBMBUF rx_handle )
{
#ifdef TRAFFIC_TRACE
	uint8_t *pucMessage = (uint8_t*)MbmGetBufData( rx_handle );
	uint32_t dwLength = ((uint32_t)pucMessage[0] << 8) + (uint32_t)pucMessage[1];
	LLDStubDumpMessage( K2LDEBUG_TEXT("R:"), pucMessage, dwLength + 2 );
#endif
}

