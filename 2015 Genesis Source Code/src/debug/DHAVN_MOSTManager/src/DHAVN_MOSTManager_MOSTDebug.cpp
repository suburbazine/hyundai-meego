#include "DHAVN_MOSTManager_MOSTDebug.h"

CTraceListener::CTraceListener()
    : m_pTraceConnector(NULL)
{
    LOG_INIT_V2(LOG_PREFIX_MOST)
}

void CTraceListener::TraceConnector( CACSTraceConnector *pTraceConnector )
{
    m_pTraceConnector = pTraceConnector;
}

void CTraceListener::OnConnect()
{
    LOG_TRACE << LOG_TAG_DEBUG << " The application is now connected to a bus/node/network. " << LOG_ENDL;
}

void CTraceListener::OnDisconnect()
{
    LOG_TRACE << LOG_TAG_DEBUG << " The application is NOT connected to a bus/node/network. " << LOG_ENDL;
}

void CTraceListener::OpenSuccess()
{
    LOG_TRACE << LOG_TAG_DEBUG << " Trace connection sucessfully established. " << LOG_ENDL;

    if( NULL != m_pTraceConnector )
    {
        m_pTraceConnector->Start();
    }
}

void CTraceListener::OpenFailed()
{
    LOG_TRACE << LOG_TAG_DEBUG << " Trace connection failed. " << LOG_ENDL;
}

void CTraceListener::StartSuccess()
{
    LOG_TRACE << LOG_TAG_DEBUG << " Tracing started. " << LOG_ENDL;
}

void CTraceListener::StartSelectionSuccess( uint8_t count, const uint32_t * protocolList )
{
    LOG_TRACE << LOG_TAG_DEBUG << " Tracing for selected protocols started. " << LOG_ENDL;
}

void CTraceListener::TraceRawMessage
    ( uint32_t flags
    , uint32_t protocol
    , uint32_t senderNodeAddress
    , uint16_t recepientInfoCount
    , const uint8_t * recepientInfo
    , uint16_t serializedMessageLength
    , const uint8_t * serializedMessage
    )
{
    LOG_TRACE << LOG_TAG_DEBUG << " Trace message received. " << LOG_ENDL;
}

void CTraceListener::ProtocolTraceDescriptor
    ( uint32_t targetProtocol
    , uint32_t descriptorLength
    , const uint8_t *descriptor
    )
{
    LOG_TRACE << LOG_TAG_DEBUG << " Protocol descriptor received. " << LOG_ENDL;
}


CMOSTProtocolTracer::CMOSTProtocolTracer()
{
    LOG_INIT_V2(LOG_PREFIX_MOST)
}

void CMOSTProtocolTracer::ProtocolAvailableAtTargetNode()
{
    LOG_TRACE << LOG_TAG_DEBUG << " MOST protocol is installed at target node. " << LOG_ENDL;
}

void CMOSTProtocolTracer::ProtocolNotAvailableAtTargetNode()
{
    LOG_TRACE << LOG_TAG_DEBUG << " No MOST protocol available at target node. " << LOG_ENDL;
}

void CMOSTProtocolTracer::TraceDescriptor( uint32_t length, const uint8_t *pDescriptor )
{
    LOG_TRACE << LOG_TAG_DEBUG << " MOST Trace descriptor received. " << LOG_ENDL;
}

void CMOSTProtocolTracer::DataMessage
    ( const CMessageDetails& details
    , uint32_t flags
    , uint32_t messageID
    , uint16_t deviceID
    , uint8_t functionBlockID
    , uint8_t instanceID
    , uint16_t functionID
    , uint8_t operation
    , uint32_t payloadLength
    , const uint8_t *payload
    )
{
    TraceMessageDetails(details);

    //if( details.IsIncomingMessage() )
    {
        QString log;
        //log.append(QString(" InComingMsg = DEV:0x%1 FB:0x%2 IID:%3 FUN:0x%4 OP:0x%5 (%6 bytes) ").arg(deviceID,0,16)
        log.append(QString(" Msg = DEV:0x%1 FB:0x%2 IID:%3 FUN:0x%4 OP:0x%5 (%6 bytes) ").arg(deviceID,0,16)
                .arg(functionBlockID,0,16).arg(instanceID).arg(functionID,0,16).arg(operation,0,16).arg(payloadLength));               

        uint32_t index;
        for( index = 0; index < payloadLength; index++ )
        {
            log.append(QString("0x%1 ").arg(payload[index],0,16));
            //printf("%02X ", payload[index] );
        }
        //printf("\n")
        LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
    }
}

void CMOSTProtocolTracer::MACRegistry
    ( const CMessageDetails& details
    , uint32_t mostStateFlags
    , uint16_t logicalDeviceID
    , uint16_t count
    , const uint8_t * functionBlockList
    )
{
    TraceMessageDetails(details);

    QString log;
    //LOG_TRACE << LOG_TAG_DEBUG << " MOST MAC Registry received. " << LOG_ENDL;
    LOG_TRACE << LOG_TAG_DEBUG << " Prints out the MAC registry. " << LOG_ENDL;

    CACSTraceMOSTProtocol::CMACRegistryFunctionBlockListParser parser( count, functionBlockList );
    CACSTraceMOSTProtocol::CMACRegistryFunctionBlockListParser::Element_t element;

    if( parser.begin( element ) )
    {
        log.append(QString(" FB(0x%1,%2,0x%3) ").arg(element.functionBlockID, 0, 16).arg(element.instanceID).arg(element.deviceID, 0, 16));

        while( parser.next( element ) )
        {
            log.append(QString("FB(0x%1,%2,0x%3) ").arg(element.functionBlockID, 0, 16).arg(element.instanceID).arg(element.deviceID, 0, 16));
        }
    }

    LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
}


void CMOSTProtocolTracer::MOSTState
    ( const CMessageDetails& details
    , uint32_t mostStateFlags
    , uint16_t ownDeviceID
    , uint16_t count
    , const uint8_t * functionBlockList
    )
{
    QString log;

    TraceMessageDetails(details);

    //LOG_TRACE << LOG_TAG_DEBUG << " MOST MAC Registry received. " << LOG_ENDL;

    //log.append(QString(" MOST State received. (Own deviceID:0x%1 flags:%2) ").arg(ownDeviceID, 0, 16).arg(mostStateFlags));
    log.append(QString(" Show MOST state. (Own deviceID:0x%1 flags:%2) ").arg(ownDeviceID, 0, 16).arg(mostStateFlags));
    LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
    log.clear();

    CACSTraceMOSTProtocol::CMOSTStateFunctionBlockListParser parser( count, functionBlockList );
    CACSTraceMOSTProtocol::CMOSTStateFunctionBlockListParser::Element_t element;

    if( parser.begin( element ) )
    {        
        log.append(QString(" FB(0x%1,%2,0x%3) ").arg(element.functionBlockID, 0, 16).arg(element.instanceID).arg(element.deviceID, 0, 16));

        while( parser.next( element ) )
        {
            //printf("FB(%02X,%02X,%04X) ", element.functionBlockID, element.instanceID, element.deviceID);
            log.append(QString("FB(0x%1,%2,0x%3) ").arg(element.functionBlockID, 0, 16).arg(element.instanceID).arg(element.deviceID, 0, 16));
        }
        //printf("\n");
    }

    LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
}

void CMOSTProtocolTracer::RemoveFBlocks
    ( const CMessageDetails& details
    , uint16_t count
    , const uint8_t * functionBlockList
    )
{
    TraceMessageDetails(details);

    LOG_TRACE << LOG_TAG_DEBUG << " MOST function blocks removed. " << LOG_ENDL;
    //printf("MOST function blocks removed.\n");

    CACSTraceMOSTProtocol::CRemoveFBlocksFunctionBlockListParser parser( count, functionBlockList );
    CACSTraceMOSTProtocol::CRemoveFBlocksFunctionBlockListParser::Element_t element;

    if( parser.begin( element ) )
    {
        QString log;
        //printf(" FB(%02X,%02X) ", element.functionBlockID, element.instanceID);
        log.append(QString(" FB(0x%1,%2) ").arg(element.functionBlockID, 0, 16).arg(element.instanceID));

        while( parser.next( element ) )
        {
            //printf("FB(%02X,%02X) ", element.functionBlockID, element.instanceID);
            log.append(QString("FB(0x%1,%2) ").arg(element.functionBlockID, 0, 16).arg(element.instanceID));
        }
        LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
        //printf("\n");
    }    
}

void CMOSTProtocolTracer::RemoveShadows
    ( const CMessageDetails& details
    , uint16_t count
    , const uint8_t * shadowList
    )
{
    TraceMessageDetails(details);

    //printf("MOST shadows removed.\n");
    LOG_TRACE << LOG_TAG_DEBUG << " MOST shadows removed. " << LOG_ENDL;

    CACSTraceMOSTProtocol::CRemoveShadowsFunctionBlockListParser parser( count, shadowList );
    CACSTraceMOSTProtocol::CRemoveShadowsFunctionBlockListParser::Element_t element;

    if( parser.begin( element ) )
    {
        QString log;
        //printf("sh(%02X,%02X) ", element.functionBlockID, element.instanceID);
        log.append(QString(" SH(0x%1,%2) ").arg(element.functionBlockID, 0, 16).arg(element.instanceID));

        while( parser.next( element ) )
        {
            //printf("sh(%02X,%02X) ", element.functionBlockID, element.instanceID);
            log.append(QString("SH(0x%1,%2) ").arg(element.functionBlockID, 0, 16).arg(element.instanceID));
        }
        LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
        //printf("\n");
    }    
}

void CMOSTProtocolTracer::SetCustomerDeviceID
    ( const CMessageDetails& details
    , uint16_t defaultDeviceID
    )
{    
    TraceMessageDetails(details);
    QString log;
    log.append(QString(" MOST customer device ID set to 0x%1. ").arg(defaultDeviceID,0,16));
    LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
    //printf("MOST customer device ID set to %04X.\n", defaultDeviceID);
}

void CMOSTProtocolTracer::TxFailed
    ( const CMessageDetails& details
    , uint16_t deviceID
    , uint8_t fblockID
    , uint8_t instanceID
    )
{
    QString log;

    TraceMessageDetails(details);
    //printf("MOST Tx failed (DEV:%04X FBID:%02X IID:%02X.\n", deviceID, fblockID, instanceID);
    log.append(QString(" MOST Tx failed (DEV:0x%1 FBID:0x%2 IID:%3. ").arg(deviceID,0,16).arg(fblockID,0,16).arg(instanceID));
    LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
}


void CMOSTProtocolTracer::TraceFunctionBlockElement( CACSTraceMOSTProtocol::CUpdateMOSTStatusMOSTElementInfoParser::FunctionBlockElement_t& element )
{
    QString log;
    //printf("FB(%02X,%02X,%04X) ", element.functionBlockID, element.instanceID, element.deviceID );
    log.append(QString(" FB(0x%1,%2,0x%3) ").arg(element.functionBlockID,0,16).arg(element.instanceID).arg(element.deviceID,0,16));
    LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
}

void CMOSTProtocolTracer::TraceShadowElement( CACSTraceMOSTProtocol::CUpdateMOSTStatusMOSTElementInfoParser::ShadowElement_t& element )
{
    QString log;

    //printf("sh(%02X,%02X) ", element.functionBlockID, element.instanceID );
    log.append(QString(" SH(0x%1,%2) ").arg(element.functionBlockID,0,16).arg(element.instanceID));

    if( element.allFunctionsEnabled )
    {
        //printf("No function level filter active. ");
        log.append("No function level filter active. ");
    }
    else
    {
        //printf("Enabled functions: ");
        log.append("Enabled functions: ");

        uint16_t index;
        for( index = 0; index < element.enabledFunctionsCount; index++ )
        {
            //printf("%04X ", element.enabledFunctions[index] );
            log.append(QString("0x%1 ").arg(element.enabledFunctions[index],0,16));
        }
    }
    LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
    //printf("\n");
}

void CMOSTProtocolTracer::UpdateMOSTStatus
    ( const CMessageDetails& details
    , uint32_t physicalMOSTNodeAddress
    , uint16_t logicalDeviceID
    , uint16_t ownDeviceID
    , uint32_t mostStateFlags
    , uint16_t count
    , const uint8_t * mostElementInfo
    )
{
    QString log;

    TraceMessageDetails(details);

    log.append(QString(" MOST status update received.(physical MOST node:0x%1 logical device ID:0x%2 own device ID:0x%3 flags:%4)")
            .arg(physicalMOSTNodeAddress,0,16).arg(logicalDeviceID,0,16).arg(ownDeviceID,0,16).arg(mostStateFlags));
    //printf("MOST status update received.(physical MOST node: %X. logical device id: %04X own device id: %04X flags: %08X\n",physicalMOSTNodeAddress,logicalDeviceID,ownDeviceID,mostStateFlags );
    LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;

    CACSTraceMOSTProtocol::CUpdateMOSTStatusMOSTElementInfoParser parser( count, mostElementInfo );
    CACSTraceMOSTProtocol::CUpdateMOSTStatusMOSTElementInfoParser::FunctionBlockElement_t functionBlockElement;
    if( parser.BeginFunctionBlock( functionBlockElement ) )
    {
        //TraceFunctionBlockElement( functionBlockElement );
        log.clear();
        log.append(QString(" FB(0x%1,%2,0x%3) ").arg(functionBlockElement.functionBlockID,0,16)
                   .arg(functionBlockElement.instanceID).arg(functionBlockElement.deviceID,0,16));

        while( parser.NextFunctionBlock( functionBlockElement ) )
        {
            log.append(QString("FB(0x%1,%2,0x%3) ").arg(functionBlockElement.functionBlockID,0,16)
                       .arg(functionBlockElement.instanceID).arg(functionBlockElement.deviceID,0,16));
            //TraceFunctionBlockElement( functionBlockElement );
        }
        LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
    }

    CACSTraceMOSTProtocol::CUpdateMOSTStatusMOSTElementInfoParser::ShadowElement_t shadowElement;
    if( parser.BeginShadow( shadowElement ) )
    {
        TraceShadowElement( shadowElement );
        while( parser.NextShadow( shadowElement ) )
        {
            TraceShadowElement( shadowElement );
        }
    }
}


void CMOSTProtocolTracer::UpdateShadowFilterInfo
    ( const CMessageDetails& details
    , uint8_t fblockID
    , uint8_t instanceID
    , uint16_t count
    , const uint8_t * filterInfo
    )
{
    TraceMessageDetails(details);

    QString log;

    log.append(QString(" MOST shadow filter info received (0x%1,%2). ").arg(fblockID,0,16).arg(instanceID));
    //printf("MOST shadow filter info received (%02X,%02X).\n", fblockID, instanceID);
    LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
    log.clear();

    CACSTraceMOSTProtocol::CUpdateShadowFilterInfoParser parser( count, filterInfo );
    if( parser.Parse() )
    {
        if( parser.AllFunctionsEnabled() )
        {
            LOG_TRACE << LOG_TAG_DEBUG << " No function level filters enabled. " << LOG_ENDL;
            //printf("No function level filters enabled.\n");
        }
        else
        {
            uint16_t functionCount = 0;
            const uint16_t *functions = NULL;
            if( parser.GetEnabledFunctions( functionCount, functions ) )
            {                
                log.append(" Enabled functions: ");
                //printf("Enabled functions: ");
                uint16_t index;
                for( index = 0; index < functionCount; index++ )
                {
                    log.append(QString("0x%1 ").arg(functions[index],0,16));
                    //printf("%04X ", functions[index]);
                }
                LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
                //printf("\n");
            }
            else
            {
                LOG_TRACE << LOG_TAG_DEBUG << " ERROR: GetEnabledFunctions() failed! " << LOG_ENDL;
                //printf("ERROR: GetEnabledFunctions() failed!\n");
            }
        }
    }
    else
    {
        LOG_TRACE << LOG_TAG_DEBUG << " ERROR: filterInfo could not be parsed! " << LOG_ENDL;
        //printf("ERROR: filterInfo could not be parsed!\n");
    }
}

void CMOSTProtocolTracer::AddFBlocks
    ( const CMessageDetails& details
    , uint16_t count
    , const uint8_t * fblockList
    )
{
    TraceMessageDetails(details);
    LOG_TRACE << LOG_TAG_DEBUG << " MOST fblock added. " << LOG_ENDL;
    //printf("MOST function blocks add.\n");
    CACSTraceMOSTProtocol::CAddFBlocksParser parser( count, fblockList );
    CACSTraceMOSTProtocol::CAddFBlocksParser::Element_t element;
    if( parser.begin( element ) )
    {
        QString log;
        log.append(QString(" FB(0x%1,%2) ").arg(element.functionBlockID,0,16).arg(element.instanceID));
        //printf("FB(%02X,%02X) ", element.functionBlockID, element.instanceID);
        while( parser.next(element) )
        {
            log.append(QString("FB(0x%1,%2) ").arg(element.functionBlockID,0,16).arg(element.instanceID));
            //printf("FB(%02X,%02X) ", element.functionBlockID, element.instanceID);
        }
        LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
    }
}

void CMOSTProtocolTracer::AddShadows
    ( const CMessageDetails& details
    , uint16_t numberOfShadows
    , uint16_t count
    , const uint8_t * shadowList
    )
{
    TraceMessageDetails(details);
    LOG_TRACE << LOG_TAG_DEBUG << " MOST shadow added. " << LOG_ENDL;
    //printf("MOST shadow added.\n");
    CACSTraceMOSTProtocol::CAddShadowsParser parser( count, shadowList );
    CACSTraceMOSTProtocol::CAddShadowsParser::Element_t element;
    if( parser.begin( element ) )
    {
        QString log;
        log.append(QString(" SH(0x%1,%2) ").arg(element.functionBlockID,0,16).arg(element.instanceID));
        //printf("FB(%02X,%02X) ", element.functionBlockID, element.instanceID);

        if( element.allFunctionsEnabled )
        {
            log.append("No function level filtering enabled.");
            LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
            //printf("No function level filtering enabled.\n");
        }
        else
        {
            log.append("Enabled functions: ");
            //printf("Enabled functions: ");
            uint16_t index;
            for( index = 0; index < element.enabledFunctionsCount; index++ )
            {
                log.append(QString("0x%1 ").arg(element.enabledFunctions[index],0,16));
                //printf("%04X ", element.enabledFunctions[index]);
            }
            LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
            //printf("\n");
        }

        while( parser.next(element) )
        {
            log.clear();
            log.append(QString(" SH(0x%1,%2) ").arg(element.functionBlockID,0,16).arg(element.instanceID));
            //printf("FB(%02X,%02X) ", element.functionBlockID, element.instanceID);

            if( element.allFunctionsEnabled )
            {
                log.append("No function level filtering enabled.");
                //printf("No function level filtering enabled.\n");
                LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
            }
            else
            {
                log.append("Enabled functions: ");
                //printf("Enabled functions: ");
                uint16_t index;
                for( index = 0; index < element.enabledFunctionsCount; index++ )
                {
                    log.append(QString("0x%1 ").arg(element.enabledFunctions[index],0,16));
                    //printf("%04X ", element.enabledFunctions[index]);
                }
                LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
                //printf("\n");
            }
        }                
    }
}


void CMOSTProtocolTracer::FBlockInstanceRenamed
    ( const CMessageDetails& details
    , uint8_t fblockID
    , uint8_t oldInstanceID
    , uint8_t newInstanceID
    )
{    
    TraceMessageDetails(details);
    QString log;
    log.append(QString(" MOST function block 0x%1 instance %2 renamed to instance %3. ").arg(fblockID,0,16).arg(oldInstanceID).arg(newInstanceID));
    LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
    //printf("MOST function block %02X instance %02X renamed to instance %02X.\n", fblockID, oldInstanceID, newInstanceID);
}

void CMOSTProtocolTracer::FBlockListForMAC
    ( const CMessageDetails& details
    , uint16_t count
    , const uint8_t * functionBlockList
    )
{
    TraceMessageDetails(details);
    LOG_TRACE << LOG_TAG_DEBUG << " MOST function block list for MAC. " << LOG_ENDL;
    //printf("MOST function block list for MAC.\n");
    CACSTraceMOSTProtocol::CFBlockListForMACParser parser( count, functionBlockList );
    CACSTraceMOSTProtocol::CFBlockListForMACParser::Element_t element;
    if( parser.begin( element ) )
    {
        QString log;
        log.append(QString(" FB(0x%1,%2) ").arg(element.functionBlockID,0,16).arg(element.instanceID));
        //printf("FB(%02X,%02X) ", element.functionBlockID, element.instanceID);

        while( parser.next( element ) )
        {
            log.append(QString("FB(0x%1,%2) ").arg(element.functionBlockID,0,16).arg(element.instanceID));
            //printf("FB(%02X,%02X) ", element.functionBlockID, element.instanceID);
        }
        //printf("\n");
        LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
    }

}

void CMOSTProtocolTracer::ShadowListForMAC
    ( const CMessageDetails& details
    , uint16_t count
    , const uint8_t * shadowList
    )
{
    TraceMessageDetails(details);
    LOG_TRACE << LOG_TAG_DEBUG << " MOST shadow list for MAC. " << LOG_ENDL;
    //printf("MOST shadow list for MAC.\n");
    CACSTraceMOSTProtocol::CFBlockListForMACParser parser( count, shadowList );
    CACSTraceMOSTProtocol::CFBlockListForMACParser::Element_t element;
    if( parser.begin( element ) )
    {
        QString log;
        log.append(QString(" SH(0x%1,%2) ").arg(element.functionBlockID,0,16).arg(element.instanceID));
        //printf("FB(%02X,%02X) ", element.functionBlockID, element.instanceID);

        while( parser.next( element ) )
        {
            log.append(QString("SH(0x%1,%2) ").arg(element.functionBlockID,0,16).arg(element.instanceID));
            //printf("FB(%02X,%02X) ", element.functionBlockID, element.instanceID);
        }

        //printf("\n");
        LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
    }
}

void CMOSTProtocolTracer::GetUpdateMOSTStatus(const CMessageDetails& details)
{
    TraceMessageDetails(details);
    LOG_TRACE << LOG_TAG_DEBUG << " MOST get status update. " << LOG_ENDL;
    //printf("MOST get status update.\n");
}

void CMOSTProtocolTracer::TraceMessageDetails( const CMessageDetails & details )
{
    QString log;

    log.append(QString(" SendingNode:0x%1 ").arg(details.SenderNodeAddress(),0,16));
    //printf("SendingNode: %04X ", details.SenderNodeAddress() );

    if( details.IsIncomingMessage() )
    {
        log.append("Incoming ");
        //printf("Incoming ");
    }
    if( details.IsOutgoingMessage() )
    {
        log.append("Outgoing ");
        //printf("Outgoing ");
    }
    if( details.IsMisdirectedMessage() )
    {
        log.append("Misdirected ");
        //printf("Misdirected ");
    }
    for(  uint32_t index = 0; index < details.RecepientCount(); index++ )
    {
        CMessageDetails::RecepientInfo_t info = details.Recepient( index );
        //printf("(%08X,%04X) ", info.node, info.connection);
        log.append(QString("(Node:%1, Connection:%2) ").arg(info.node).arg(info.connection));
    }
    //printf("\n");
    LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
}


CEventProtocolTracer::CEventProtocolTracer()
    : CACSTraceEventProtocol()
{
    LOG_INIT_V2(LOG_PREFIX_MOST)
}

CEventProtocolTracer::~CEventProtocolTracer()
{
}

void CEventProtocolTracer::ProtocolAvailableAtTargetNode()
{
    LOG_TRACE << LOG_TAG_DEBUG << " Event protocol is installed at target node. " << LOG_ENDL;
    //printf("Event protocol is installed at target node.\n");
}

void CEventProtocolTracer::ProtocolNotAvailableAtTargetNode()
{
    LOG_TRACE << LOG_TAG_DEBUG << " No event protocol available at target node. " << LOG_ENDL;
    //printf("No event protocol available at target node.\n");
}


void CEventProtocolTracer::TraceDescriptor( uint32_t length, const uint8_t *pDescriptor )
{
    LOG_TRACE << LOG_TAG_DEBUG << " Event Trace descriptor received. " << LOG_ENDL;
    //printf("Event Trace descriptor received.\n");
}


void CEventProtocolTracer::CBELinkStatusUpdate( const CMessageDetails & details, uint8_t count, const uint32_t * linkStatus)
{
    TraceMessageDetails(details);

    LOG_TRACE << LOG_TAG_DEBUG << " Event Link Status update message received. " << LOG_ENDL;
    //printf("Event Link Status update message received.\n");

    CACSTraceEventProtocol::CCBELinkStatusUpdateLinkStatusParser parser( count, linkStatus );
    const uint8_t *channels = NULL;
    uint16_t channelCount = 0U;

    if( parser.ActiveChannels( channelCount, channels ) )
    {
        QString log;
        log.append(" Channels in use: ");
        //printf("Channels in use: ");

        for( uint16_t index = 0; index < channelCount; index++ )
        {
            log.append(QString("0x%1 ").arg(channels[index],0,16));
            //printf("%02X ", channels[index]);
        }
        //printf("\n");
        LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
    }
}

void CEventProtocolTracer::DataMessage( const CMessageDetails & details, uint8_t group, uint8_t channel, uint16_t payloadLength, const uint8_t *payload )
{
    TraceMessageDetails(details);

    QString log;
    log.append(QString(" Event(%1,%2) %3 bytes : ").arg(group).arg(channel).arg(payloadLength));
    for( uint16_t index=0; index < payloadLength; index++ )
    {
        log.append(QString("%1 ").arg(payload[index],0,16));
    }
    LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;

    //printf("Event(%d,%d) %d bytes.\n", group, channel, payloadLength);
}

void CEventProtocolTracer::JoinChannels( const CMessageDetails & details, uint8_t count, const uint8_t * setOfChannels)
{
    TraceMessageDetails(details);
    //printf("An application has joined event channels: ");

    QString log;
    log.append(" An application has joined event channels: ");

    for( uint16_t index=0; index < count; index++ )
    {
        log.append(QString("%1 ").arg(setOfChannels[index]));
        //printf("%02X ", setOfChannels[index]);
    }
    //printf("\n");
    LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
}

void CEventProtocolTracer::LeaveChannels( const CMessageDetails & details, uint8_t count, const uint8_t * setOfChannels)
{
    TraceMessageDetails(details);
    //printf("An application has left event channels: ");
    QString log;
    log.append(" An application has left event channels: ");

    for( uint16_t index=0; index < count; index++ )
    {
        log.append(QString("%1 ").arg(setOfChannels[index]));
        //printf("%02X ", setOfChannels[index]);
    }
    //printf("\n");
    LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
};

void CEventProtocolTracer::TraceMessageDetails( const CMessageDetails & details )
{
    QString log;

    log.append(QString(" SendingNode:0x%1 ").arg(details.SenderNodeAddress(),0,16));
    //printf("SendingNode: %04X ", details.SenderNodeAddress() );

    if( details.IsIncomingMessage() )
    {
        log.append("Incoming ");
        //printf("Incoming ");
    }
    if( details.IsOutgoingMessage() )
    {
        log.append("Outgoing ");
        //printf("Outgoing ");
    }
    if( details.IsMisdirectedMessage() )
    {
        log.append("Misdirected ");
        //printf("Misdirected ");
    }
    for(  uint32_t index = 0; index < details.RecepientCount(); index++ )
    {
        CMessageDetails::RecepientInfo_t info = details.Recepient( index );
        //printf("(%08X,%04X) ", info.node, info.connection);
        log.append(QString("(Node:%1, Connection:%2) ").arg(info.node).arg(info.connection));
    }
    //printf("\n");
    LOG_TRACE << LOG_TAG_DEBUG << log << LOG_ENDL;
}


CTraceThread::CTraceThread()
    : CIACSApplicationEvents()
    , m_ACSApplication(this)    // The warning you see at compile time tells you that you must never do this
                                // if you do more to the pointer than store it in a member variable.
    , m_messagesEvent(true)
    , m_running(false)
    , m_traceListener()
    , m_traceConnector(&m_traceListener)
    , m_eventTracer()
{
    LOG_INIT_V2(LOG_PREFIX_MOST)

    m_traceListener.TraceConnector( &m_traceConnector );
    m_traceConnector.SetProtocolListener( CACSTraceEventProtocol::ProtocolID(), &m_eventTracer );
    m_traceConnector.SetProtocolListener( CACSTraceMOSTProtocol::ProtocolID(), &m_MOSTTracer );
    m_ACSApplication.AddProtocol( &m_traceConnector );
}

CTraceThread::~CTraceThread()
{
}

bool CTraceThread::Configure()
{
    // TODO: Add your application configuration code here. All which needs to be initialized before we connect
    // to the communications middleware.

    return true;
}

void CTraceThread::run()
{
    m_running = true;
    m_ACSApplication.Events()->SetListener( this );

    if( m_ACSApplication.StartApplication(THREAD_PRIORITY, SERVER_ADDRESS, SERVER_PORT, BUS_ADDRESS) )
    {
        uint8_t channels[] = {
            ACS_SHUTDOWN_CHANNEL // We join event channel 0. There comes our sample shutdown event.
        };

        m_ACSApplication.Events()->JoinChannels( sizeof(channels), channels );
        m_traceConnector.Open(42);

        LOG_TRACE << LOG_TAG_DEBUG << " Start ACS Trace. " << LOG_ENDL;

        while( m_running )
        {
            //TODO: Find solution to the standard blocking problem on *nix
            if (m_messagesEvent.Wait(500))
            {
                //Message arrived
                while( m_ACSApplication.Dispatch() )
                {
                    // EMPTY
                }
            }
            else
            {
                if (m_secondsEvent.Wait(0))
                {
                    m_secondsEvent.Reset();
                    s_unixTimeMOST.Value( k2l::platform::system::SystemGetTickCount() );
                    s_unixTimeProperty.UpdateShadows();
                }
            }
        }

        LOG_TRACE << LOG_TAG_DEBUG << " Stop ACS Trace. " << LOG_ENDL;

        m_ACSApplication.StopApplication();
    }
}

//
// IACSApplicationEvents implementation
//
void CTraceThread::MessagesAvailable()
{
    m_messagesEvent.Set();
}

void CTraceThread::NoMessagesAvailable()
{
    m_messagesEvent.Reset();
}

//
// k2l::acs::protocols::events::IACSEventListener implementation
//
void CTraceThread::OnEvent
    ( uint8_t channel
    , uint8_t eventID
    , uint32_t payloadLength
    , const uint8_t *pPayload
    )
{
    switch( channel )
    {
        case ACS_SHUTDOWN_CHANNEL:
            if( ACS_SHUTDOWN_EVENT_ID == eventID )
            { // Event channel 0, eventID 0 is our sample shutdown event. It has no additional data.
                m_running = false;
            }
            break;
        default: // Other events we do not process here.
            break;
    }
}


