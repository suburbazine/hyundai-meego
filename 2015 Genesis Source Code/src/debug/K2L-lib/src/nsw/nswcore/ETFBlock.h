/*=== K2L GmbH ===============================================================*/
/**
\file
            This file contains the interfaces of the ETFBlock.

\ingroup    NSWCore

\par        COPYRIGHT (c) 2000-2011 by K2L GmbH
            All rights reserved.

 */
/*============================================================================*/

#ifndef _ETFBLOCK_H
#define _ETFBLOCK_H
 
#include <IMAC.h>
#include <ACSStreams.h>
#include <map>

class DSO_Settings
{
public:
    typedef enum ACK_MODE_tag { SINGLE_FRAME_ACK = 0, BLOCK_ACK = 1 } ACK_MODE;
	uint16_t deviceID;
	uint8_t fblockID;
	uint8_t instanceID;
	uint16_t functionID;
	uint8_t opType;
	uint8_t ackMode;


	DSO_Settings()
		: deviceID(0)
		, fblockID(0)
		, instanceID(0)
		, functionID(0)
		, opType(0)
		, ackMode(BLOCK_ACK)
	{

	}

	virtual ~DSO_Settings()
	{
	}

	uint64_t getKey()
	{

		uint64_t key = functionID << 24 |
				       fblockID << 16 |
				       instanceID << 8 |
				       opType;

		return key;
	}

	bool parsePayload(const CMOSTMessage& crMessage)
	{
		bool result = false;
		const uint8_t* payload = crMessage.Payload();
		uint16_t length = crMessage.PayloadLength();

		if (NULL != payload)
		{
			if (length == 11)
			{
                if (payload[5] == 0)
                {
				    ackMode = SINGLE_FRAME_ACK;
                }
                else
                {
                    ackMode = BLOCK_ACK;
                }

				fblockID = payload[6];
				instanceID = payload[7];
				functionID = payload[8] << 8 | payload[9];
				opType = payload[10];
				result = true;
			}
		}

		return result;
	}
};


class CETFBlock
	: public IMACEvents
	, public INetServicesEvents
{
    IMAC                       *m_pMAC;
    INetServices               *m_pNetServices;
    bool					    m_dsiHold[256];
    std::map<uint64_t, DSO_Settings> m_dsoSettings;

    void ErrorOpTypeNotAvailable(const CMOSTMessage& rMsg);
    void ErrorDSO(const CMOSTMessage& rMsg);

    //Handler functions
    bool HandleDSIDSOCount(const CMOSTMessage& rMsg, k2l::acs::base::CACSStreamReader& reader);
    bool HandleDSO(const CMOSTMessage& rMsg, k2l::acs::base::CACSStreamReader& reader);
    bool HandleDSIHold(const CMOSTMessage& rMsg, k2l::acs::base::CACSStreamReader& reader);
    bool HandleDSI(const CMOSTMessage& rMsg, k2l::acs::base::CACSStreamReader& reader);

public:
    CETFBlock() ;
    virtual ~CETFBlock();

    void Initialize( IMAC *pMAC, INetServices * pNetServices );
    void Terminate (void);
    void Process (MILLISECONDS nominalInterval);
    bool HoldState(uint8_t dsiIndex);
    bool UseSingleFrameAck(const CMOSTMessage& crMessage);

	//IMACEvents
public:
    virtual 
    void 
    OnK2LEvent
        ( EVENTCHANNEL channel            // [0..0xFE]
        , EVENTID id                    // [0..0xFF]
        , uint8_t tagLength                // [0..3]
        , const uint8_t* cpTagData        // ((cpTagData == NULL) && (tagLength == 0)) || (cpTagData != NULL && tagLength >= 0) 
        , uint16_t payloadLength
        , const uint8_t* cpPayload
        );
	
	//INetServicesEvents
public:
    virtual void OnNetservicesAPIAvailable();
    virtual void OnNetservicesAPIUnavailable();
    virtual void OnNetOn();
    virtual void OnNetOff();
    virtual void OnShortUnlock();
    virtual void OnLongUnlock();
    virtual void OnMPR( MPR mpr );
    virtual void OnMessageReceived( const CMOSTMessage& crMessage, uint32_t &rdwConsumerCount );
};


#endif // _ETFBLOCK_H

/*============================================================================*/
/* END OF FILE                                                                */
/*============================================================================*/
