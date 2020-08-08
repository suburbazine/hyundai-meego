/*=== K2L GmbH ===============================================================*/
/**
\file
            This file contains the implementation of the ETFBlock
            functionality.

\ingroup    NSWCore

\par        COPYRIGHT (c) 2000-2011 by K2L GmbH
            All rights reserved.

\par        File History
\code
Date        Author Scope
--------------------------------------------------------------------------------
2011-10-13  bko Initial version.
\endcode */

/*=============================================================================
Copyright © 2011 K2L GmbH (“K2L”). All rights reserved.
Please make sure that all information within a document marked as ‘Confidential’
or ‘Restricted Access’ is handled solely in accordance with the agreement
pursuant to which it is provided, and is not reproduced or disclosed to others
without the prior written consent of K2L. The confidential ranking of a document
can be found in the footer of every page. This document supersedes and replaces
all information previously supplied. The technical information in this document
loses its validity with the next edition. Although the information is believed
to be accurate, no responsibility is assumed for inaccuracies. Specifications
and other documents mentioned in this document are subject to change without
notice. K2L reserves the right to make changes to this document and to the
products at any time without notice. Neither the provision of this information
nor the sale of the described products conveys any licenses under any patent
rights or other intellectual property rights of K2L or others. The products
may contain design defects or errors known as anomalies, including but not
necessarily limited to any which may be identified in this document, which may
cause the product to deviate from published descriptions. Anomalies are
described in errata sheets available upon request. K2L products are not
designed, intended, authorized or warranted for use in any life support or
other application where product failure could cause or contribute to personal
injury or severe property damage. Any and all such uses without prior written
approval of an officer of K2L will be fully at your own risk. The K2L logo is
a trademark of K2L. Other names mentioned may be trademarks of their respective
holders.

K2L disclaims and excludes any and all warranties, including without limitation
any and all implied warranties of merchantability, fitness for a particular
purpose, title, and against infringement and the like, and any and all
warranties arising from any course of dealing or usage of trade. In no event
shall K2L be liable for any direct, incidental, indirect, special, punitive,
or consequential damages; or for lost data, profits, savings or revenues of
any kind; regardless of the form of action, whether based on contract; tort;
negligence of K2L or others; strict liability; breach of warranty; or
otherwise; whether or not any remedy of buyer is held to have failed of its
essential purpose, and whether or not K2L has been advised of the possibility
of such damages.
===HEADER END================================================================*/

#include <stddef.h>
#include <ETFBlock.h>
#include <ACSStreams.h>
#include <MOSTProtocolDefinitions.h>
#include <ComServerEvents.h>
#include <crc32calc.h>
#include <adjustmh.h>

//-------------------------------------------------------------------------------------------

void CETFBlock::ErrorOpTypeNotAvailable(const CMOSTMessage& rMsg)
{
	CMOSTMessage msg(3);
	msg.DeviceID(rMsg.DeviceID());
	msg.FBlockID(rMsg.FBlockID());
	msg.InstanceID(rMsg.InstanceID());
	msg.FunctionID(rMsg.FunctionID());
	msg.OpType(k2l::acs::base::most::OPTYPE_ERROR);
	msg.Transport(rMsg.Transport());
	msg.AddressingType(rMsg.AddressingType());
	msg.SenderIsFBlock();

    // check for acknowledge Operation type within received message
    switch( rMsg.OpType() )
    {
    case k2l::acs::base::most::OPTYPE_START_RESULT_ACK    :   // no break
    case k2l::acs::base::most::OPTYPE_ABORT_ACK           :   // no break
    case k2l::acs::base::most::OPTYPE_START_ACK           :
    case k2l::acs::base::most::OPTYPE_PROCESSING_ACK      :
    case k2l::acs::base::most::OPTYPE_RESULT_ACK          :
    case k2l::acs::base::most::OPTYPE_ERROR_ACK           :
    	//Special handling for ACKs
    	if (rMsg.PayloadLength() >= 2)
    	{
    		msg.Payload()[0] = rMsg.Payload()[0];
    		msg.Payload()[1] = rMsg.Payload()[1];
    		msg.Payload()[2] = k2l::acs::base::most::ERROR_OPTYPE_NOT_AVAILABLE;
    		msg.OpType(k2l::acs::base::most::OPTYPE_ERROR_ACK);
    	}
    	else
    	{
    		msg.Payload()[0] = k2l::acs::base::most::ERROR_INVALID_LENGTH;
    		msg.TrimPayloadLength(1);
    	}
        break;
    default:
		msg.Payload()[0] = k2l::acs::base::most::ERROR_OPTYPE_NOT_AVAILABLE;
		msg.TrimPayloadLength(1);
    }

    m_pMAC->SendMOSTMessage(msg);
}

void CETFBlock::ErrorDSO(const CMOSTMessage& rMsg)
{
	CMOSTMessage msg(3);
	msg.DeviceID(rMsg.DeviceID());
	msg.FBlockID(rMsg.FBlockID());
	msg.InstanceID(rMsg.InstanceID());
	msg.FunctionID(rMsg.FunctionID());
	msg.OpType(k2l::acs::base::most::OPTYPE_ERROR_ACK);
	msg.Transport(rMsg.Transport());
	msg.AddressingType(rMsg.AddressingType());
	msg.SenderIsFBlock();
	msg.Payload()[0] = rMsg.Payload()[0];
	msg.Payload()[1] = rMsg.Payload()[1];
	msg.Payload()[2] = k2l::acs::base::most::ERROR_PROCESSING_ERROR;
}


bool CETFBlock::HandleDSIDSOCount(const CMOSTMessage& rMsg, k2l::acs::base::CACSStreamReader& reader)
{
	bool consumed = false;

	if (0x3fd == rMsg.FunctionID())
	{
		if (k2l::acs::base::most::OPTYPE_GET == rMsg.OpType())
		{
			//Control
			CMOSTMessage msg(2);
			msg.DeviceID(rMsg.DeviceID());
			msg.FBlockID(rMsg.FBlockID());
			msg.InstanceID(rMsg.InstanceID());
			msg.FunctionID(rMsg.FunctionID());
			msg.OpType(k2l::acs::base::most::OPTYPE_STATUS);
			msg.Transport(MOSTMESSAGE_TRANSPORT_CONTROL_CHANNEL);
			msg.AddressingType(MOSTMESSAGE_ADDRESSING_LOGICAL);
			msg.SenderIsFBlock();
			msg.Payload()[0] = MAX_MHP_RX;
			msg.Payload()[1] = MAX_MHP_TX;
			m_pMAC->SendMOSTMessage(msg);
		}
		else
		{
			ErrorOpTypeNotAvailable(rMsg);
		}
		consumed = true;
	}
	return consumed;
}

bool CETFBlock::HandleDSO(const CMOSTMessage& rMsg, k2l::acs::base::CACSStreamReader& reader)
{
	bool consumed = false;
	//DSO
	if (0x3fe == rMsg.FunctionID())
	{
		if (k2l::acs::base::most::OPTYPE_START_ACK == rMsg.OpType())
		{
			const uint16_t DSO_SIZE = 4*1024;
			uint16_t senderHandle = 0;
			uint16_t numPackets = 0;
			uint8_t nextPacketMethod = 0;
			uint8_t ackMode = 0;
			uint8_t targetFBlockID = 0;
			uint8_t targetInstID = 0;
			uint16_t targetFunctionID = 0;
			uint8_t targetOpType = 0;

			reader.Read16(&senderHandle);
			reader.Read16(&numPackets);
			reader.Read8(&nextPacketMethod);
			reader.Read8(&ackMode);
			reader.Read8(&targetFBlockID);
			reader.Read8(&targetInstID);
			reader.Read16(&targetFunctionID);
			reader.Read8(&targetOpType);

			if (0x02 != nextPacketMethod)
			{
                DSO_Settings dso;

                if (dso.parsePayload(rMsg))
                {
                    m_dsoSettings[dso.getKey()] = dso;
                }

				CMOSTMessage msg(DSO_SIZE); //4 KBytes
				msg.DeviceID(rMsg.DeviceID());
				msg.FBlockID(targetFBlockID);
				msg.InstanceID(targetInstID);
				msg.FunctionID(targetFunctionID);
				msg.OpType(targetOpType);
				msg.Transport(MOSTMESSAGE_TRANSPORT_MOST_HIGH);
				msg.AddressingType(MOSTMESSAGE_ADDRESSING_LOGICAL);
				msg.SenderIsFBlock();
				uint8_t* pPayload = &(msg.Payload()[0]);
				uint32_t i = 0;
				
                for (i = 0; i < DSO_SIZE; i++)
				{
					pPayload[i] = (uint8_t) i;
				}
				
                for (i = 0; i < numPackets; i++)
				{
					m_pMAC->SendMOSTMessage(msg);
				}
			}
			else
			{
				ErrorOpTypeNotAvailable(rMsg);
			}
		}
		else
		{
			ErrorOpTypeNotAvailable(rMsg);
		}
		consumed = true;
	}
	return consumed;
}

bool CETFBlock::HandleDSIHold(const CMOSTMessage& rMsg, k2l::acs::base::CACSStreamReader& reader)
{
	bool consumed = false;
	//DSIHold
	if (0x3ff == rMsg.FunctionID())
	{
		if (k2l::acs::base::most::OPTYPE_START_ACK == rMsg.OpType())
		{
			uint16_t senderHandle = 0;
			uint8_t hold = 0;
			uint8_t dsiIndex = 0;

			reader.Read16(&senderHandle);
			reader.Read8(&hold);
			reader.Read8(&dsiIndex);

			bool holdState = false;
			if (0 == hold)
			{
				holdState = false;
			}
			else
			{
				holdState = true;
			}
			m_dsiHold[dsiIndex] = holdState;
		}
		else
		{
			ErrorOpTypeNotAvailable(rMsg);
		}
		consumed = true;
	}
	return consumed;
}

bool CETFBlock::HandleDSI(const CMOSTMessage& rMsg, k2l::acs::base::CACSStreamReader& reader)
{
	bool consumed = false;
	//DSI 0-0xFF
	if ((rMsg.FunctionID() >= 0x400) && (rMsg.FunctionID() <= 0x4FF))
	{
		if (k2l::acs::base::most::OPTYPE_START_RESULT_ACK == rMsg.OpType())
		{
			uint16_t senderHandle = 0;
			reader.Read16(&senderHandle);

			const uint8_t* pData = reader.PointerAtCurrentPosition();
			uint32_t dataLength = reader.Capacity() - reader.Position();
			//TODO: Not allowed to init CRC here. Copy CRC impl here inside.
			//crc32Init();
			uint32_t checkSum = crc32Calc(pData, dataLength);

			const uint16_t DSI_MESSAGE_LENGTH = 10;
			CMOSTMessage msg(DSI_MESSAGE_LENGTH);
			msg.DeviceID(rMsg.DeviceID());
			msg.FBlockID(rMsg.FBlockID());
			msg.InstanceID(rMsg.InstanceID());
			msg.FunctionID(rMsg.FunctionID());
			msg.OpType(k2l::acs::base::most::OPTYPE_RESULT_ACK);
			msg.Transport(MOSTMESSAGE_TRANSPORT_CONTROL_CHANNEL);
			msg.AddressingType(MOSTMESSAGE_ADDRESSING_LOGICAL);
			msg.SenderIsFBlock();
			k2l::acs::base::CACSStreamWriter writer(msg.Payload(), DSI_MESSAGE_LENGTH);
			writer.Write16(senderHandle);
			writer.Write32(checkSum);
			writer.Write32(dataLength);

			m_pMAC->SendMOSTMessage(msg);
		}
		else
		{
			ErrorOpTypeNotAvailable(rMsg);
		}
		consumed = true;
	}
	return consumed;
}

bool CETFBlock::UseSingleFrameAck(const CMOSTMessage& crMessage)
{
    bool singleFrameAck = false;

    uint64_t dsoKey = (crMessage.FunctionID() << 24) | (crMessage.FBlockID() << 16) |  crMessage.InstanceID() << 8 | crMessage.OpType();
    if (m_dsoSettings.find(dsoKey) != m_dsoSettings.end())
    {
        DSO_Settings dso = m_dsoSettings[dsoKey];

        if (dso.ackMode == DSO_Settings::SINGLE_FRAME_ACK) // single frame acknowledge
        {
            singleFrameAck = true;
        }
     }

     return singleFrameAck;
} 

//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------

CETFBlock::CETFBlock()
    : m_pMAC(NULL)
    , m_pNetServices(NULL)
    , m_dsiHold()
    , m_dsoSettings()
{
	for (uint32_t i = 0; i < (sizeof(m_dsiHold)/sizeof(m_dsiHold[0])); i++)
	{
		m_dsiHold[i] = false;
	}
}

//-------------------------------------------------------------------------------------------

CETFBlock::~CETFBlock()
{
    m_pMAC = NULL;
    m_pNetServices = NULL;
}

//-------------------------------------------------------------------------------------------

void CETFBlock::Initialize( IMAC *pMAC, INetServices * pNetServices )
{
	m_pMAC = pMAC;
	m_pNetServices = pNetServices;
	m_pNetServices->AddEventListener(this);
	m_pMAC->AddEventListener(this);
	m_pMAC->JoinChannel(ETVIAEVENT);
}

//-------------------------------------------------------------------------------------------

void CETFBlock::Terminate (void)
{
	m_pNetServices->RemoveEventListener(this);
	m_pMAC->RemoveEventListener(this);
	m_pNetServices = NULL;
	m_pMAC = NULL;
}

//-------------------------------------------------------------------------------------------

void CETFBlock::Process (MILLISECONDS nominalInterval)
{

}

bool CETFBlock::HoldState(uint8_t dsiIndex)
{
	return m_dsiHold[dsiIndex];
}

//IMACEvents
//virtual 
void CETFBlock::OnK2LEvent
	( EVENTCHANNEL channel            // [0..0xFE]
	, EVENTID id                    // [0..0xFF]
	, uint8_t tagLength                // [0..3]
	, const uint8_t* cpTagData        // ((cpTagData == NULL) && (tagLength == 0)) || (cpTagData != NULL && tagLength >= 0) 
	, uint16_t payloadLength
	, const uint8_t* cpPayload
	)
{
	if (NULL == m_pMAC)
	{
		return;
	}
	
	if (ETVIAEVENT == channel)
	{
		k2l::acs::base::CACSStreamReader reader(cpPayload, payloadLength);
		
		//Used to ignore params from stream
		uint8_t dummy8 = 0;
		
		//Following params are used
		uint16_t deviceID = 0;
		uint8_t fblockID = 0;
		uint8_t instanceID = 0;
		uint16_t functionID = 0;
		uint8_t opType = 0;
		uint32_t messageLength = 0;
		
		reader.Read16(&deviceID);
		reader.Read8(&dummy8);
		reader.Read8(&fblockID);
		reader.Read8(&instanceID);
		reader.Read16(&functionID);
		reader.Read8(&opType);
		reader.Read32(&messageLength);
        		

		CMOSTMessage inputMsg(messageLength);
		inputMsg.DeviceID(deviceID);
		inputMsg.FBlockID(fblockID);
		inputMsg.InstanceID(instanceID);
		inputMsg.FunctionID(functionID);
		inputMsg.OpType(opType);

        uint32_t streamPos = reader.Position();
        reader.ReadBlock(inputMsg.Payload(), inputMsg.PayloadLength());
        reader.SeekFromStart(streamPos);

		(void)HandleDSIDSOCount(inputMsg, reader);
		(void)HandleDSO(inputMsg, reader);
		(void)HandleDSIHold(inputMsg, reader);
		(void)HandleDSI(inputMsg, reader);
	}
}

//
//virtual 
void CETFBlock::OnNetservicesAPIAvailable()
{
}

//virtual 
void CETFBlock::OnNetservicesAPIUnavailable()
{
}    

//virtual  
void CETFBlock::OnNetOn()
{
}

//virtual 
void CETFBlock::OnNetOff()
{
}    

//virtual  
void CETFBlock::OnShortUnlock()
{
}

//virtual 
void CETFBlock::OnLongUnlock()
{
}

//virtual 
void CETFBlock::OnMPR( MPR mpr )
{
}

//virtual 
void CETFBlock::OnMessageReceived( const CMOSTMessage& crMessage, uint32_t &rdwConsumerCount )
{
	k2l::acs::base::CACSStreamReader reader(crMessage.Payload(), crMessage.PayloadLength());
	bool consumed = false;
	consumed |= HandleDSIDSOCount(crMessage, reader);
	consumed |= HandleDSO(crMessage, reader);
	consumed |= HandleDSIHold(crMessage, reader);
	consumed |= HandleDSI(crMessage, reader);
	if (consumed)
	{
		rdwConsumerCount++;
	}
}

//-------------------------------------------------------------------------------------------

/*=============================================================================
 * END OF FILE
 *=============================================================================*/
