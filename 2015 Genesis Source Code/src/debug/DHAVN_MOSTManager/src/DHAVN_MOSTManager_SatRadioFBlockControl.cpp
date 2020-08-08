#include "DHAVN_MOSTManager_SatRadioFBlockControl.h"

CSatRadioFblockControl::CSatRadioFblockControl(CMarshaledACSApplication *context)
    : CControlThreadBase(context, LOG_TAG_RADIO_CNT)
{
    LOG_INIT_V2(LOG_PREFIX_MOST)

    m_SatRadioFblock = new CSatRadio(0x01);
    m_pMOSTElement = m_SatRadioFblock;

    m_StationSelect = new CStationSelect(m_SatRadioFblock);
    m_StationSelectListener = new CStationSelectListener(this);

    m_ServiceProvider = new CServiceProvider(m_SatRadioFblock);
    m_ServiceProviderListener = new CServiceProviderListener(this);

    m_ServiceStatus = new CServiceStatus(m_SatRadioFblock);
    m_TMCStatus = new CTMCStatus(m_SatRadioFblock);

    m_TMCDataRequest = new CTMCDataRequest(m_SatRadioFblock);
    m_TMCDataRequestHandler = new CTMCDataRequestHandler(this);
}

CSatRadioFblockControl::~CSatRadioFblockControl()
{
    delete m_SatRadioFblock;
    delete m_StationSelect;
    delete m_ServiceProvider;
    delete m_ServiceStatus;
    delete m_TMCStatus;

    delete m_StationSelectListener;
    delete m_ServiceProviderListener;

    delete m_TMCDataRequestHandler;
}

bool CSatRadioFblockControl::Initialize()
{    
    //uchar eventChannels[] = {EVENT_CHANNEL_MOST_HIGH};

    if(m_SatRadioFblock->InitializeMOSTElement())
    {
        SetListener();
        SetHandler();

        /*
        if( !m_AppContext->Events()->JoinChannels(sizeof(eventChannels), eventChannels) )
        {
            MOST_LOG_CRITICAL << LOG_TAG_RADIO_CNT << " Cannot joined ACS event channel " << MOST_LOG_ENDL;
        }
        */

        m_Initialized = true;
    }

    return m_Initialized;
}

bool CSatRadioFblockControl::FBlockVisible()
{
    return GetIsRegistered();
}

void CSatRadioFblockControl::SetListener()
{
    m_StationSelect->SetListener(m_StationSelectListener);
    m_ServiceProvider->SetListener(m_ServiceProviderListener);
}

void CSatRadioFblockControl::SetHandler()
{
    m_TMCDataRequest->SetHandler(m_TMCDataRequestHandler);
}

bool CSatRadioFblockControl::RemoveListener()
{
    bool result = false;

    result = m_StationSelect->RemoveListener(m_StationSelectListener);
    result &= m_ServiceProvider->RemoveListener(m_ServiceProviderListener);

    return result;
}

/*
void CSatRadioFblockControl::SetACSEventListener(void)
{
    m_AppContext->Events()->SetListener(m_TMCDataRequestHandler);
}
*/

void CSatRadioFblockControl::StationSelectListener(void)
{
    uchar type = m_StationSelect->StationValue();
    uchar station = m_StationSelect->StationValue();
/*
    MOST_LOG_SIGNAL << LOG_TAG_RADIO_CNT << " OnStationSelect API got called" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_RADIO_CNT << " OnStationSelect:Type = " << type << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_RADIO_CNT << " OnStationSelect:Station = " << station << MOST_LOG_ENDL;
*/    
// 2014.08.11, for log, hyunyong.sung
    MOST_LOG_SIGNAL << LOG_TAG_RADIO_CNT << " StationSelect_Set <-- : Type = " << type 
    << " Station = " << station  << MOST_LOG_ENDL;		

    emit StationSelect(type, station);
}

void CSatRadioFblockControl::ServiceProviderListener(void)
{
    uchar serviceProvider = m_ServiceProvider->Value();
/*
    MOST_LOG_SIGNAL << LOG_TAG_RADIO_CNT << " OnServiceProvider API got called" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_RADIO_CNT << " OnServiceProvider:ServiceProvider = " << serviceProvider << MOST_LOG_ENDL;
*/
// 2014.08.11, for log, hyunyong.sung
    MOST_LOG_SIGNAL << LOG_TAG_RADIO_CNT << " ServiceProvider_Set <-- : ServiceProvider = " << serviceProvider << MOST_LOG_ENDL;	

    emit ServiceProvider(serviceProvider);
}

bool CSatRadioFblockControl::SendStationSelect(const uchar optype, const uchar station, const uchar presetNo)
{
    bool result = false;
/*
    MOST_LOG_SIGNAL << LOG_TAG_RADIO_CNT << " SendStationSelect API got called" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_RADIO_CNT << " SendStationSelect:Optype = " << optype << MOST_LOG_ENDL;
*/
    switch(optype)
    {
        case PROPERTY_STATUS:
// 2014.08.11, for log, hyunyong.sung
		MOST_LOG_HIGH << LOG_TAG_RADIO_CNT << " StationSelect_Status --> : Station = " << station 
		<< " PresetNo = " << presetNo  << MOST_LOG_ENDL;	
/*
            MOST_LOG_INFO << LOG_TAG_RADIO_CNT << " SendStationSelect:Station = " << station << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_RADIO_CNT << " SendStationSelect:PresetNo = " << presetNo << MOST_LOG_ENDL;
*/            
            m_StationSelect->StationValue(station);
            m_StationSelect->PresetNoValue(presetNo);
            result = m_StationSelect->UpdateShadows();
            break;
        case PROPERTY_ERROR:
            MOST_LOG_INFO << LOG_TAG_RADIO_CNT << "Undefined Operation of SendStationSelect" << MOST_LOG_ENDL;
            result = m_StationSelect->Error(IBOX_DEVICE_ADDRESS, 0x41, 0, NULL);
            break;
    }

    return result;
}

bool CSatRadioFblockControl::SendServiceProvider(const uchar optype, const uchar serviceProvider)
{
    bool result = false;
/*
    MOST_LOG_SIGNAL << LOG_TAG_RADIO_CNT << " SendServiceProvider API got called" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_RADIO_CNT << " SendServiceProvider:Optype = " << optype << MOST_LOG_ENDL;
*/
    switch(optype)
    {
        case PROPERTY_STATUS:
// 2014.08.11, for log, hyunyong.sung
	    MOST_LOG_HIGH << LOG_TAG_RADIO_CNT << " ServiceProvider_Status --> : ServiceProvider = " << serviceProvider << MOST_LOG_ENDL;	
//            MOST_LOG_INFO << LOG_TAG_RADIO_CNT << " SendServiceProvider:ServiceProvider = " << serviceProvider << MOST_LOG_ENDL;
            m_ServiceProvider->Value((CServiceProvider::TServiceProvider)serviceProvider);
            result = m_ServiceProvider->UpdateShadows();
            break;
        case PROPERTY_ERROR:
            MOST_LOG_INFO << LOG_TAG_RADIO_CNT << "Undefined Operation of SendServiceProvider" << MOST_LOG_ENDL;
            result = m_ServiceProvider->Error(IBOX_DEVICE_ADDRESS, 0x41, 0, NULL);
            break;
    }

    return result;
}

bool CSatRadioFblockControl::SendServiceStatus(const uchar optype, const bool audio, const bool stationList, const uchar subscription)
{
    bool result = false;
/*
    MOST_LOG_SIGNAL << LOG_TAG_RADIO_CNT << " SendServiceStatus API got called" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_RADIO_CNT << " SendServiceStatus:Optype = " << optype << MOST_LOG_ENDL;
*/
    switch(optype)
    {
        case PROPERTY_STATUS:
// 2014.08.11, for log, hyunyong.sung
	    MOST_LOG_HIGH << LOG_TAG_RADIO_CNT << " ServiceStatus_Status --> : AudioStatus = " << audio
	    << " StationList = " << stationList << " subscription = " << subscription << MOST_LOG_ENDL;	
/*
            MOST_LOG_INFO << LOG_TAG_RADIO_CNT << " SendServiceStatus:AudioStatus" << audio << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_RADIO_CNT << " SendServiceStatus:StationListStatus = " << stationList << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_RADIO_CNT << " SendServiceStatus:Subscription = " << subscription << MOST_LOG_ENDL;
*/          
            m_ServiceStatus->DataAudioStatusValue(audio);
            m_ServiceStatus->DataStationListStatusValue(stationList);
            m_ServiceStatus->DataSubscriptionStatusValue((CServiceStatus::TDataSubscriptionStatus)subscription);
            result = m_ServiceStatus->UpdateShadows();
            break;
        case PROPERTY_ERROR:
            MOST_LOG_INFO << LOG_TAG_RADIO_CNT << "Undefined Operation of SendServiceStatus" << MOST_LOG_ENDL;
            result = m_ServiceStatus->Error(IBOX_DEVICE_ADDRESS, 0x41, 0, NULL);
            break;
    }

    return result;
}

bool CSatRadioFblockControl::SendTMCStatus(const uchar optype, const uchar tmcStaus)
{
    bool result = false;
/*
    MOST_LOG_SIGNAL << LOG_TAG_RADIO_CNT << " SendTMCStatus API got called" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_RADIO_CNT << " SendTMCStatus:Optype = " << optype << MOST_LOG_ENDL;
*/
    switch(optype)
    {
        case PROPERTY_STATUS:
// 2014.08.11, for log, hyunyong.sung
	    MOST_LOG_HIGH << LOG_TAG_RADIO_CNT << " TMCStatus_Status --> : TMCStatus = " << tmcStaus << MOST_LOG_ENDL;	
//            MOST_LOG_INFO << LOG_TAG_RADIO_CNT << " SendTMCStatus:TMCStatus = " << tmcStaus << MOST_LOG_ENDL;
            m_TMCStatus->TMCStatusValue((CTMCStatus::TTMCStatus)tmcStaus);
            result = m_TMCStatus->UpdateShadows();
            break;
        case PROPERTY_ERROR:
            MOST_LOG_INFO << LOG_TAG_RADIO_CNT << "Undefined Operation of SendTMCStatus" << MOST_LOG_ENDL;
            result = m_TMCStatus->Error(IBOX_DEVICE_ADDRESS, 0x41, 0, NULL);
            break;
    }

    return result;
}

CStationSelectListener::CStationSelectListener(CSatRadioFblockControl *context)
{
    m_SatRadioFblockControlContext = context;
}

void CStationSelectListener::OnChange()
{
    m_SatRadioFblockControlContext->StationSelectListener();
}

CServiceProviderListener::CServiceProviderListener(CSatRadioFblockControl *context)
{
    m_SatRadioFblockControlContext = context;
}

void CServiceProviderListener::OnChange()
{
    m_SatRadioFblockControlContext->ServiceProviderListener();
}

CTMCDataRequestHandler::CTMCDataRequestHandler(CSatRadioFblockControl *context)
{
    LOG_INIT_V2(LOG_PREFIX_MOST)

    m_SatRadioFblockControlContext = context;    
}

void CTMCDataRequestHandler::StartResultAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload)
{
    m_SatRadioFblockControlContext->TMCDataRequestHandler(sourceDeviceID, payloadLength, payload, METHOD_STARTRESULTACK);    
}

/*
void CTMCDataRequestHandler::OnEvent(uint8_t channel, uint8_t eventID, uint32_t payloadLength, const uint8_t *pPayload)
{
    if(channel == EVENT_CHANNEL_MOST_HIGH)
    {
        switch(eventID)
        {
            case EVENT_ID_MOST_HIGH_TRANSMIT_COMPLETE:
                LOG_TRACE << LOG_TAG_RADIO_CNT << " MOST_HIGH_TRANSMIT_COMPLETE " << LOG_ENDL;
                break;
            case EVENT_ID_MOST_HIGH_TRANSMIT_FAILED:
                LOG_CRITICAL << LOG_TAG_RADIO_CNT << " MOST_HIGH_TRANSMIT_FAILED " << LOG_ENDL;
                break;
            case EVENT_ID_MOST_HIGH_TX_CONNECTION_END:
                LOG_TRACE << LOG_TAG_RADIO_CNT << " MOST_HIGH_TX_CONNECTION_END " << LOG_ENDL;
                break;
            default:
                break;
        }
    }
}
*/

void CSatRadioFblockControl::TMCDataRequestHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t * payload, const uchar optype)
{
    ushort senderHandle;
    uint blockCounter;
/*
    MOST_LOG_SIGNAL << LOG_TAG_RADIO_CNT << "TMCDataRequestHandler API was called from IBOX" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_RADIO_CNT << "TMCDataRequestHandler::Optype : " << optype << MOST_LOG_ENDL;
*/
    switch(optype)
    {
        case METHOD_STARTRESULTACK:
            {
                bool result = false;
                m_TMCDataRequest->ParseStartResultAck(payloadLength, payload);
                senderHandle = m_TMCDataRequest->SenderHandleValue();
                blockCounter = m_TMCDataRequest->BlockCounterValue();
// 2014.08.11, for log, hyunyong.sung
		MOST_LOG_SIGNAL << LOG_TAG_RADIO_CNT << " TMCDataRequest_StartResultAck <-- : senderHandle = " 
		<< senderHandle << " BlockCounter = " << blockCounter << MOST_LOG_ENDL;	
/*
                MOST_LOG_INFO << LOG_TAG_RADIO_CNT << "TMCDataRequestHandler::SenderHandle : " << senderHandle << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_RADIO_CNT << "TMCDataRequestHandler::BlockCounter : " << blockCounter << MOST_LOG_ENDL;
*/
                QByteArray buf(1006, (char)blockCounter);

                m_TMCDataRequest->SenderHandleValue(senderHandle);

                if(blockCounter == 0)
                {
                    m_TMCDataRequest->BlockTypeValue(CTMCDataRequest::BlockTypeStartOfFrame);
                }
                else if(blockCounter < 10)
                {
                    m_TMCDataRequest->BlockTypeValue(CTMCDataRequest::BlockTypeInlineFrame);
                }
                else
                {
                    m_TMCDataRequest->BlockTypeValue(CTMCDataRequest::BlockTypeEndOfFrame);
                }
                m_TMCDataRequest->BlockCounterValue(blockCounter);
                m_TMCDataRequest->BlockSizeValue(1006);
                m_TMCDataRequest->TMCDataValue(reinterpret_cast<uint8_t *>(buf.data()), buf.length());

                //SetACSEventListener();
                result = m_TMCDataRequest->ResultAck(IBOX_DEVICE_ADDRESS);
// 2014.08.11, for log, hyunyong.sung
			MOST_LOG_HIGH << LOG_TAG_RADIO_CNT << " TMCDataRequest_ResultAck --> : senderHandle = " 
			<< m_TMCDataRequest->SenderHandleValue() << " BlockType = " << m_TMCDataRequest->BlockTypeValue()
			<< " BlockCounter = " << m_TMCDataRequest->BlockCounterValue() << " BlockSize = " << m_TMCDataRequest->BlockSizeValue()
			<< " TMCData = " << m_TMCDataRequest->TMCDataValue() << MOST_LOG_ENDL;	
		
//                MOST_LOG_INFO << LOG_TAG_RADIO_CNT << "TMCDataRequestHandler::ResultAck : " << result << MOST_LOG_ENDL;

            }
            break;

        default:
            MOST_LOG_SIGNAL << LOG_TAG_RADIO_CNT << "Undefined Operation of TMCDataRequestHandler" << MOST_LOG_ENDL;
            return;
    }
}


