/*============================================================================
 * COPYRIGHT (c) 2006-2006 by K2L GmbH
 * All rights reserved
 *
 * ABSTRACT: MOST High via MOST asynchronous channel implementation helpers.
 *			 1. Receive buffer configuration management: CMOSTHighRxBufferConfiguration
 *			 2. MLB specific packet data socket management.
 *
 * GROUP PREFIX: Not applicable.
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

#ifndef __K2LMOSTHIGH_H
#define __K2LMOSTHIGH_H

#include <IMAC.h>
#include <K2LDebug.h>

typedef uint16_t TMnsResult;           /* may only change into > 16 bits ! */
/*
 * K2L has defined a set of K2L Events in an event channel, dedicated to Application MOST high
 * control and status interaction. Those events and their content is well known to customers
 * of K2L technology and the events are compabtible to various K2L technologies.
 * Even though users could redefine the events and their content, this is strongly disadvised.
 */
#define EVENT_CHANNEL_MOST_HIGH 17
// Commands App -> MAC
#define EVENT_ID_MOST_HIGH_ADD_CONFIGURATION			1
#define EVENT_ID_MOST_HIGH_REMOVE_CONFIGURATION			2
#define EVENT_ID_MOST_HIGH_REMOVE_ALL_CONFIGURATIONS	3
// Results MAC -> App
#define EVENT_ID_MOST_HIGH_RECEIVE_REFUSED				4
#define EVENT_ID_MOST_HIGH_RECEIVE_FAILED				5
#define EVENT_ID_MOST_HIGH_TRANSMIT_COMPLETE			6
#define EVENT_ID_MOST_HIGH_TRANSMIT_FAILED				7
#define EVENT_ID_MOST_HIGH_TX_CONNECTION_END            8

#define ADDRESSING_WILDCARD 0xFFFF

/*
 * Applications configure the receive buffer sizes on a detailed FBlock/Instance/Function/OpType
 * specific level. This class stores the configurations and provides this information to the
 * MOST High Rx buffer allocator.
 */
class CMOSTHighRxBufferConfiguration
	: public IMACEvents
{
	static const uint32_t LIMIT_CONFIG_STORAGE_CAPACITY = 50;
	//Important note: If you integrate ACS into system which allocates
	//memory in 16bit space, you will need to reduce CONFIGURATION_DEFAULT_BUFFER_SIZE
	//for 18 bytes so that header and message fit together in one 16bit space segment.
	static const uint16_t CONFIGURATION_DEFAULT_BUFFER_SIZE = (65535);
	struct Config_t
	{
		uint64_t key;
		uint16_t value;
	};

	IMAC *		m_pMAC;
	Config_t	m_Configurations[LIMIT_CONFIG_STORAGE_CAPACITY];
	Config_t *  m_pBegin;
	Config_t *  m_pEndOfBuffer;
	Config_t *  m_pEnd;

#ifdef DEBUG
	inline
	void
	CheckDistance( Config_t *pFirst, Config_t *pLast, uint_fast16_t distance ) const
	{
		for( ; distance > 0; --distance )
		{
			pFirst++;
		}
		K2L_DEBUGCHK( pFirst == pLast );
	}
	inline
	void
	CheckAdvance( Config_t *pStart, Config_t *pResult, uint_fast16_t distance ) const
	{
		uint_fast16_t counter;
		for( counter = 0; counter < distance; ++counter )
		{
			*pStart++;
		}
		K2L_DEBUGCHK( pStart == pResult );
	}
#else
#	define CheckDistance(a,b,c) ((void)0)
#	define CheckAdvance(a,b,c) ((void)0)
#endif


	// Not available.
	CMOSTHighRxBufferConfiguration(const CMOSTHighRxBufferConfiguration & crOther);
	// Not available.
	const CMOSTHighRxBufferConfiguration & operator=( const CMOSTHighRxBufferConfiguration & crOther );
public:
	CMOSTHighRxBufferConfiguration();
	virtual ~CMOSTHighRxBufferConfiguration();

	void Initialize( IMAC *pMAC );
	void Terminate();

	//
	// IMACEvents implementation.
	//
	void
	virtual
	OnK2LEvent
		( EVENTCHANNEL channel			// [0..0xFE]
		, EVENTID id					// [0..0xFF]
		, uint8_t tagLength				// [0..3]
		, const uint8_t* cpTagData		// ((cpTagData == NULL) && (tagLength == 0)) || (cpTagData != NULL && tagLength >= 0)
		, uint16_t payloadLength
		, const uint8_t* cpPayload
		);

	//
	// Query functions for MOST high implementation.
	//
	uint16_t FindBufferSize( FBLOCKID fblockID, INSTANCEID instanceID, FUNCTIONID functionID, OPTYPE opType );

	//
	// Additional info functions (used by UnitTest).
	//
	inline bool empty() const
	{
		return m_pEnd == m_pBegin;
	}

	inline uint16_t size() const
	{
		uint16_t distance = static_cast<uint16_t>(m_pEnd - m_pBegin);
		CheckDistance( m_pBegin, m_pEnd, distance );
		return distance;
	}

	inline uint32_t capacity() const
	{
		return LIMIT_CONFIG_STORAGE_CAPACITY;
	}

private:
	void AddConfiguration( const Config_t &config );
	void RemoveConfiguration( const Config_t & config );
	void Clear();
	Config_t *LowerBound( Config_t* pFirst, Config_t *pLast, uint64_t key );
	bool Find( uint64_t key, Config_t *&rpResult );

	inline
	uint64_t
	InternalKeyFromConfigurationKeys
		( uint16_t fblockKey
		, uint16_t instanceIDKey
		, uint16_t functionIDKey
		, uint16_t opTypeKey
		)
	{
		uint64_t key = (((uint64_t)fblockKey) << 48)
				 + (((uint64_t)instanceIDKey) << 32)
				 + (((uint64_t)functionIDKey) << 16)
				 + (((uint64_t)opTypeKey ) /*<< 0*/);
		return key;
	}

	inline
	uint16_t
	ReadMOST_WORD
		( const uint8_t *cpBuffer
		, uint16_t & rwReadPosition
		, uint16_t wMessageLength
		)
	{
		uint16_t wResult = 0;
		if( (rwReadPosition + 1) < wMessageLength )
		{
			wResult = cpBuffer[rwReadPosition++];
			wResult <<= 8;
			wResult += cpBuffer[rwReadPosition++];
		}
		else
		{
			K2L_DEBUGCHK(false);
		}
		return wResult;
	}
};

#endif
