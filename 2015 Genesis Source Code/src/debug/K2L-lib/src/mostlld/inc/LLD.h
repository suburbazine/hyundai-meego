/*============================================================================
 * COPYRIGHT (c) 2005-2006 by K2L GmbH
 * All rights reserved
 *
 * ABSTRACT: Defines the API contract between higher layers (NSW or MOST driver
 *			 in general) and the hardware platform specific communications
 *			 to SMSC INIC, either via I2C or an MLB hardware interface.
 *			 The API is designed in a way, that it needs not interact directly
 *			 with OASIS/SMSC NetServices code.
 *			 In NSW layer, there is a dedicated integration module (LLDStub)
 *			 which - in combination with the NetServices callbacks module -
 *			 interacts with the LLD API.
 *
 * GROUP PREFIX: LLD (Low level MOST driver).
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
 * 11.05.2006	1.0		K2L		Initial version
 *
 *============================================================================*/
#ifndef __LLD_H
#define __LLD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum eLLDInterfaceType_tag
{	LLD_INTERFACE_ERROR
,	LLD_INTERFACE_I2C
,	LLD_INTERFACE_MLB
} eLLDInterface;

typedef enum eLLDTransportType_tag
{	LLD_TRANSPORT_ANY
,	LLD_TRANSPORT_CONTROL_CHANNEL
,	LLD_TRANSPORT_ASYNC_CHANNEL
} eLLDTransport;

typedef enum LLDStubBuffer_tag
{
    LLDSTUB_BUFFER_CTL = 0,
    LLDSTUB_BUFFER_ATX = 1
} LLDStubBuffer_t;

typedef enum eLLDMLBClkRates_tag
{	LLD_MLB_CLK_256fs  = 256
,	LLD_MLB_CLK_512fs  = 512
,   LLD_MLB_CLK_1024fs = 1024
} eLLDMLBClkRate;

typedef void * LLDPacketBufferHandle_t;

// ----------------------------------------------------------
// API, to be implemented by the LLD library.
// ----------------------------------------------------------
uint8_t LLDInitialize(eLLDInterface *pInterfaceType);
uint8_t LLDTerminate(void);
uint8_t LLDEnable(void);
uint8_t LLDDisable(void);
uint8_t LLDServiceInterrupt(void);
uint8_t LLDInterruptDone(void);
uint8_t LLDPollInterrupt(uint32_t timeoutMilliseconds /* -1 for infinite */);

uint8_t LLDSendTriggerMessage(void);

int LLDGetIPCRxHandle();
int LLDGetIPCTxHandle();

//TMO:
void LLDSetMostNetStatusOn(void);
void LLDSetMostNetStatusOff(void);
void LLDSetMostNetEUI48(const uint8_t eui48[]);

// This function is used by applications running in other threads to wait for
// LLD to be available. For example needed in case MLB is used for IPC communication.
//
void LLDWaitForInitDone(void);

// The packet data musst be processed during this call. It is not allowed to store a
// reference to the packet. The pointer to the packet is only guaranteed to be valid
// for the duration of this call to LLDTransmitPacket().
uint8_t
LLDTransmitPacket
	( const uint8_t * cpucPacket		// Complete packet - can be written directly towards INIC.
	, uint32_t dwPacketLength			// Length of packet - number of bytes.
	, eLLDTransport	transportType	// ANY in case of I2C and one of the others in case of MLB.
	);

// This function is called, whenever it is necessary to reset the INIC chip.
uint8_t
LLDResetINIC
	( void
	);

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
	);

extern
uint8_t
LLDStubOnPacketReceived
	( LLDPacketBufferHandle_t hPacketBuffer
	);

// Called from the IST of INIC interrupt.
// Do not try to service the interrupt (e.g. calling LLDStubGetPacketBuffer()) directly from
// the IST context. Instead, call LLDStubOnINICInterrupt() from within the IST and
// Service and re-enable the interrupt again (InterruptDone()) as soon as LLDServiceInterrupt() is
// called.
extern uint8_t LLDStubOnInterrupt(void);

extern void LLDStubReleaseTxBuffer(const LLDStubBuffer_t buffer);

#ifdef __cplusplus
}
#endif

#endif
