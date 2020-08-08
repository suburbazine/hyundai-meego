#ifndef DHAVN_MOSTMANAGER_MOSTDEBUG_H
#define DHAVN_MOSTMANAGER_MOSTDEBUG_H

#include <QThread>

#include <DHAVN_LogUtil.h>

#include "DHAVN_MOSTManager_ACSApplication.h"
#include "K2LACSTrace.h"
#include "DHAVN_MOSTManager_Config.h"
#include "DHAVN_MOSTManager_LogWrapper.h"

#define LOG_TAG_DEBUG   "K2LACS_DEBUG"

using namespace k2l::acs::protocols::trace;

static k2l::acs::protocols::most::CMOSTUnsignedLong s_unixTimeMOST;
static k2l::acs::protocols::most::CMOSTFunctionClassNumber s_unixTimeProperty;

const uint8_t ACS_SHUTDOWN_CHANNEL = 0;
const uint8_t ACS_SHUTDOWN_EVENT_ID = 0;

class CTraceListener : public IACSTraceListener
{
    USE_LOG_UTIL

    CACSTraceConnector *m_pTraceConnector;

public:
    CTraceListener();
    virtual ~CTraceListener(){}

    void TraceConnector( CACSTraceConnector *pTraceConnector );

    virtual void OnConnect();
    virtual void OnDisconnect();
    virtual void OpenSuccess();
    virtual void OpenFailed();
    virtual void StartSuccess();
    virtual void StartSelectionSuccess(uint8_t count, const uint32_t *protocolList);
    virtual void ProtocolTraceDescriptor(uint32_t targetProtocol, uint32_t descriptorLength, const uint8_t *descriptor);

    virtual void TraceRawMessage
            ( uint32_t flags
            , uint32_t protocol
            , uint32_t senderNodeAddress
            , uint16_t recepientInfoCount
            , const uint8_t * recepientInfo
            , uint16_t serializedMessageLength
            , const uint8_t * serializedMessage
            );
};


class CMOSTProtocolTracer : public CACSTraceMOSTProtocol
{
    USE_LOG_UTIL

private:
    void TraceFunctionBlockElement(CACSTraceMOSTProtocol::CUpdateMOSTStatusMOSTElementInfoParser::FunctionBlockElement_t& element);
    void TraceShadowElement(CACSTraceMOSTProtocol::CUpdateMOSTStatusMOSTElementInfoParser::ShadowElement_t& element);

public:
    CMOSTProtocolTracer();
    virtual ~CMOSTProtocolTracer(){}

    virtual void ProtocolAvailableAtTargetNode();
    virtual void ProtocolNotAvailableAtTargetNode();
    virtual void TraceDescriptor( uint32_t length, const uint8_t *pDescriptor );
    virtual void DataMessage(const CMessageDetails& details, uint32_t flags, uint32_t messageID, uint16_t deviceID, uint8_t functionBlockID,
                             uint8_t instanceID, uint16_t functionID, uint8_t operation, uint32_t payloadLength, const uint8_t *payload);
    virtual void MACRegistry(const CMessageDetails& details, uint32_t mostStateFlags, uint16_t logicalDeviceID, uint16_t count, const uint8_t * functionBlockList);
    virtual void MOSTState(const CMessageDetails& details, uint32_t mostStateFlags, uint16_t ownDeviceID, uint16_t count, const uint8_t * functionBlockList);
    virtual void RemoveFBlocks(const CMessageDetails& details, uint16_t count, const uint8_t * functionBlockList);
    virtual void RemoveShadows (const CMessageDetails &details, uint16_t count, const uint8_t *shadowList);
    virtual void SetCustomerDeviceID (const CMessageDetails &details, uint16_t defaultDeviceID);
    virtual void TxFailed (const CMessageDetails &details, uint16_t deviceId, uint8_t fblockID, uint8_t instanceID);
    virtual void UpdateMOSTStatus(const CMessageDetails &details, uint32_t physicalMOSTNodeAddress, uint16_t logicalDeviceID,
                                  uint16_t ownDeviceID, uint32_t mostStateFlags, uint16_t count, const uint8_t *mostElementInfo);
    virtual void UpdateShadowFilterInfo(const CMessageDetails &details, uint8_t fblockID, uint8_t instanceID, uint16_t count, const uint8_t *filterInfo);
    virtual void AddFBlocks(const CMessageDetails &details, uint16_t count, const uint8_t *fblockList);
    virtual void AddShadows(const CMessageDetails &details, uint16_t numberOfShadows, uint16_t count, const uint8_t *shadowList);
    virtual void FBlockInstanceRenamed(const CMessageDetails &details, uint8_t fblockID, uint8_t oldInstanceID, uint8_t newInstanceID);
    virtual void FBlockListForMAC(const CMessageDetails &details, uint16_t count, const uint8_t *functionBlockList);
    virtual void ShadowListForMAC(const CMessageDetails &details, uint16_t count, const uint8_t *shadowList);
    virtual void GetUpdateMOSTStatus(const CMessageDetails &details);

    void TraceMessageDetails( const CMessageDetails & details );
};


class CEventProtocolTracer : public CACSTraceEventProtocol
{
    USE_LOG_UTIL

public:
    CEventProtocolTracer();
    virtual ~CEventProtocolTracer();

    virtual void ProtocolAvailableAtTargetNode();
    virtual void ProtocolNotAvailableAtTargetNode();
    virtual void TraceDescriptor(uint32_t length, const uint8_t *pDescriptor);
    virtual void CBELinkStatusUpdate(const CMessageDetails &details, uint8_t count, const uint32_t *linkStatus);
    virtual void DataMessage(const CMessageDetails &details, uint8_t channel, uint8_t eventID, uint16_t payloadLength, const uint8_t *payload);
    virtual void JoinChannels(const CMessageDetails &details, uint8_t count, const uint8_t *setOfChannels);
    virtual void LeaveChannels(const CMessageDetails &details, uint8_t count, const uint8_t *setOfChannels);

    void TraceMessageDetails( const CMessageDetails & details );
};

class CTraceThread
    : public CIACSApplicationEvents
    , public k2l::acs::protocols::events::IACSEventListener
    , public QThread
{
    USE_LOG_UTIL

    CMarshaledACSApplication m_ACSApplication;
    k2l::platform::system::CManualResetEvent m_messagesEvent;
    k2l::platform::system::CManualResetEvent m_secondsEvent;
    volatile bool m_running;

    CTraceListener m_traceListener;
    CACSTraceConnector m_traceConnector;
    CEventProtocolTracer m_eventTracer;
    CMOSTProtocolTracer m_MOSTTracer;

public:
    CTraceThread();
    ~CTraceThread();

    bool Configure();

    virtual void run();

    virtual void MessagesAvailable();
    virtual void NoMessagesAvailable();
    virtual void OnEvent( uint8_t channel, uint8_t eventID, uint32_t payloadLength, const uint8_t *pPayload);
};



#endif // DHAVN_MOSTMANAGER_MOSTDEBUG_H
