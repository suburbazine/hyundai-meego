#include "DHAVN_MOSTManager_DMBTunerFBlockControl.h"

CDMBTunerFblockControl::CDMBTunerFblockControl(CMarshaledACSApplication *context)
    : CControlThreadBase(context, LOG_TAG_DMB_CNT)
{
    LOG_INIT_V2(LOG_PREFIX_MOST)    

    m_DMBTunerFblock = new CDMBTuner(0x01);
    m_pMOSTElement = m_DMBTunerFblock;

    m_SignalQuality = new CSignalQuality(m_DMBTunerFblock);
    m_TPEGStatus = new CTPEGStatus(m_DMBTunerFblock);
    m_TPEGInfo = new CTPEGInfo(m_DMBTunerFblock);
    m_DMBStatus = new CDMBStatus(m_DMBTunerFblock);
    m_TPEGDataRequest = new CTPEGDataRequest(m_DMBTunerFblock);

    m_TPEGDataRequestHandler = new CTPEGDataRequestHandler(this);

    m_IsTPEGDataAvailable = false;
    memset(&m_xTPEGData, 0, sizeof(TPEGData_T));
}

CDMBTunerFblockControl::~CDMBTunerFblockControl()
{
    delete m_DMBTunerFblock;
    delete m_SignalQuality;
    delete m_TPEGStatus;
    delete m_TPEGInfo;
    delete m_DMBStatus;
    delete m_TPEGDataRequest;
    delete m_TPEGDataRequestHandler;

    if(m_xTPEGDataSharedMem.isAttached())
    {
        m_xTPEGDataSharedMem.detach();
    }
}

bool CDMBTunerFblockControl::Initialize()
{
    //uchar eventChannels[] = {EVENT_CHANNEL_MOST_HIGH};

    if(m_DMBTunerFblock->InitializeMOSTElement())
    {
        SetHandler();

        /*
        if( !m_AppContext->Events()->JoinChannels(sizeof(eventChannels), eventChannels) )
        {            
            MOST_LOG_CRITICAL << LOG_TAG_DMB_CNT << " Cannot joined ACS event channel " << MOST_LOG_ENDL;
        }
        */

        m_Initialized = true;
    }

    return m_Initialized;
}

void CDMBTunerFblockControl::SetHandler()
{
    m_TPEGDataRequest->SetHandler(m_TPEGDataRequestHandler);
}

bool CDMBTunerFblockControl::FBlockVisible()
{
    return GetIsRegistered();
}

/*
void CDMBTunerFblockControl::SetACSEventListener(void)
{
    m_AppContext->Events()->SetListener(m_TPEGDataRequestHandler);
}
*/

bool CDMBTunerFblockControl::SendSignalQuality(const uchar optype, const uchar dmbSignalQuality)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_DMB_CNT << " SendSignalQuality API got called to send to IBOX" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DMB_CNT << " SendSignalQuality:OPTYPE = " << optype << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DMB_CNT << " SendSignalQuality:DMBSignalQuality = " << dmbSignalQuality << MOST_LOG_ENDL;
*/    
// 2014.07.22, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_DMB_CNT << " SignalQuality_Status --> : DMBSignalQuality = " << dmbSignalQuality << MOST_LOG_ENDL;

    bool result = false;	
	
    switch(optype)
    {
        case PROPERTY_STATUS:
            if(m_SignalQuality->DMBSignalQualityValue(dmbSignalQuality))
            {
                result = m_SignalQuality->UpdateShadows();
            }
            break;
        case PROPERTY_ERROR:
            result = m_SignalQuality->Error(IBOX_DEVICE_ADDRESS, 0x41, 0, NULL);
            break;
        default:
            MOST_LOG_INFO << LOG_TAG_DMB_CNT << "Undefined Operation of SendSignalQuality" << MOST_LOG_ENDL;
            break;
    }

    return result;
}

bool CDMBTunerFblockControl::SendTPEGStatus(const uchar optype, const uchar tpegStatus)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_DMB_CNT << " SendTPEGStatus API got called to send to IBOX" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DMB_CNT << " SendTPEGStatus:OPTYPE = " << optype << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DMB_CNT << " SendTPEGStatus:TPEGStatus = " << tpegStatus << MOST_LOG_ENDL;
*/
// 2014.07.22, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_DMB_CNT << " TPEGStatus_Status --> : TPEGStatus = " << tpegStatus << MOST_LOG_ENDL;

    bool result = false;	
    switch(optype)
    {
        case PROPERTY_STATUS:
            if(m_TPEGStatus->TPEGStatusValue((CTPEGStatus::TTPEGStatus)tpegStatus))
            {
                SetTPEGDataState(true);
                result = m_TPEGStatus->UpdateShadows();
            }
            break;
        case PROPERTY_ERROR:
            result = m_TPEGStatus->Error(IBOX_DEVICE_ADDRESS, 0x41, 0, NULL);
            break;
        default:                        
            MOST_LOG_INFO << LOG_TAG_DMB_CNT << "Undefined Operation of SendTPEGStatus" << MOST_LOG_ENDL;
            break;
    }

    return result;
}

bool CDMBTunerFblockControl::SendTPEGInfo(const uchar optype, const STPEGInfo &tpegInfo)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_DMB_CNT << " SendTPEGInfo API got called to send to IBOX" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DMB_CNT << " SendTPEGInfo:OPTYPE = " << optype << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DMB_CNT << " SendTPEGInfo:TPEGInfo.Date = " << tpegInfo.date.toAscii().data() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DMB_CNT << " SendTPEGInfo:TPEGInfo.Time = " << tpegInfo.time.toAscii().data() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DMB_CNT << " SendTPEGInfo:TPEGInfo.Text = " << tpegInfo.text.toAscii().data() << MOST_LOG_ENDL;
*/
// 2014.07.22, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_DMB_CNT << " TPEGInfo_Status --> : ReceiveDate = " << tpegInfo.date.toAscii().data()
	<< " ReceiveTime = " << tpegInfo.time.toAscii().data() << " ReceiveText = " << tpegInfo.text.toAscii().data() << MOST_LOG_ENDL;
	
    bool result = false;
    switch(optype)
    {
        case PROPERTY_STATUS:
            result = m_TPEGInfo->RecevieDateString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                                   reinterpret_cast<const uint8_t*>(tpegInfo.date.toUtf8().data()),
                                                   tpegInfo.date.length()+1);
            result &= m_TPEGInfo->RecevieTimeString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                                    reinterpret_cast<const uint8_t*>(tpegInfo.time.toUtf8().data()),
                                                    tpegInfo.time.length()+1);
            result &= m_TPEGInfo->RecevieTextString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                                    reinterpret_cast<const uint8_t*>(tpegInfo.text.toUtf8().data()),
                                                    tpegInfo.text.length()+1);
            result &= m_TPEGInfo->UpdateShadows();
            break;
        case PROPERTY_ERROR:
            result = m_TPEGInfo->Error(IBOX_DEVICE_ADDRESS, 0x41, 0, NULL);
            break;
        default:                        
            MOST_LOG_INFO << LOG_TAG_DMB_CNT << "Undefined Operation of SendTPEGInfo" << MOST_LOG_ENDL;
            break;
    }

    return result;
}

bool CDMBTunerFblockControl::SendDMBStauts(const uchar optype, const uchar dmbStauts, const uchar currentBrowserListLength)
{
/*
    MOST_LOG_INFO << LOG_TAG_DMB_CNT << " SendDMBStauts API got called to send to IBOX" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DMB_CNT << " SendDMBStauts:OPTYPE = " << optype << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DMB_CNT << " SendDMBStauts:DMBStauts = " << dmbStauts << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DMB_CNT << " SendDMBStauts:CurrentBrowserListLength = " << currentBrowserListLength << MOST_LOG_ENDL;
*/
// 2014.07.22, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_DMB_CNT << " DMBStatus_Status --> : DMBStauts = " << dmbStauts
	<< " CurrentBrowserListLength = " << currentBrowserListLength << MOST_LOG_ENDL;

    bool result = false;
    switch(optype)
    {
        case PROPERTY_STATUS:
            result = m_DMBStatus->DMBStatusValue((CDMBStatus::TDMBStatus)dmbStauts);
            result &= m_DMBStatus->CurrentBrowserListLengthValue(currentBrowserListLength);
            result = m_DMBStatus->UpdateShadows();
            break;
        case PROPERTY_ERROR:
            result = m_TPEGStatus->Error(IBOX_DEVICE_ADDRESS, 0x41, 0, NULL);
            break;
        default:        
            MOST_LOG_CRITICAL << LOG_TAG_DMB_CNT << "Undefined Operation of SendDMBStauts" << MOST_LOG_ENDL;
            break;
    }

    return result;
}


CTPEGDataRequestHandler::CTPEGDataRequestHandler(CDMBTunerFblockControl *context)
{
    LOG_INIT_V2(LOG_PREFIX_MOST)

    m_DMBTunerFblockControlContext = context;
}

void CTPEGDataRequestHandler::StartResultAck(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t * payload)
{
    m_DMBTunerFblockControlContext->TPEGDataRequestHandler(sourceDeviceID, payloadLength, payload, METHOD_STARTRESULTACK);
}

/*
void CTPEGDataRequestHandler::OnEvent(uint8_t channel, uint8_t eventID, uint32_t payloadLength, const uint8_t *pPayload)
{
    if(channel == EVENT_CHANNEL_MOST_HIGH)
    {
        switch(eventID)
        {
            case EVENT_ID_MOST_HIGH_TRANSMIT_COMPLETE:                                
                LOG_INFO << LOG_TAG_DMB_CNT << " MOST_HIGH_TRANSMIT_COMPLETE " << LOG_ENDL;
                m_DMBTunerFblockControlContext->SetTPEGDataState(false);
                break;
            case EVENT_ID_MOST_HIGH_TRANSMIT_FAILED:                                
                LOG_CRITICAL << LOG_TAG_DMB_CNT << " MOST_HIGH_TRANSMIT_FAILED " << LOG_ENDL;
                break;
            case EVENT_ID_MOST_HIGH_TX_CONNECTION_END:                                
                LOG_INFO << LOG_TAG_DMB_CNT << " MOST_HIGH_TX_CONNECTION_END " << LOG_ENDL;
                break;
            default:
                break;
        }
    }
}
*/

void CDMBTunerFblockControl::TPEGDataRequestHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t * payload, const uchar optype)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_DMB_CNT << "TPEGDataRequestHandler API was called from IBOX" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_DMB_CNT << "TPEGDataRequestHandler::Optype = " << optype << MOST_LOG_ENDL;
*/
    ushort senderHandle = 0;
    switch(optype)
    {
        case METHOD_STARTRESULTACK:
            m_TPEGDataRequest->ParseStartResultAck(payloadLength, payload);
            senderHandle = m_TPEGDataRequest->SenderHandleValue();
            if( GetTPEGDataState() )
            {
                uint blockCounter = m_TPEGDataRequest->BlockCounterValue();

                if( !m_xTPEGDataSharedMem.isAttached() )
                {
                    m_xTPEGDataSharedMem.setKey(TPEG_SHARED_MEM_KEY);
                    if( !m_xTPEGDataSharedMem.attach(QSharedMemory::ReadOnly) )
                    {                        
                        MOST_LOG_CRITICAL << LOG_TAG_DMB_CNT << "Cannot access to the shared memory for TPEG Data " << MOST_LOG_ENDL;
                        return;
                    }
                }

                m_xTPEGDataSharedMem.lock();
                const char *to = (const char *)m_xTPEGDataSharedMem.constData();
                (void)memcpy(&m_xTPEGData, to, sizeof(TPEGData_T));
                m_xTPEGDataSharedMem.unlock();

                if( m_xTPEGData.bWrite && m_xTPEGData.nDataLength > 0 )
                {
                    m_TPEGDataRequest->BlockTypeValue(CTPEGDataRequest::BlockTypeEndOfFrame);
                    m_TPEGDataRequest->BlockCounterValue(0);
                    m_TPEGDataRequest->BlockSizeValue(m_xTPEGData.nDataLength);
                    m_TPEGDataRequest->TPEGDataValue((uint8_t *)m_xTPEGData.szTPEGData, m_xTPEGData.nDataLength);

                    //SetACSEventListener();
                    m_TPEGDataRequest->ResultAck(sourceDeviceID);
/*
                    MOST_LOG_INFO << LOG_TAG_DMB_CNT << "TPEGDataRequestHandler::SenderHandle = " << senderHandle << MOST_LOG_ENDL;
                    MOST_LOG_INFO << LOG_TAG_DMB_CNT << "TPEGDataRequestHandler::BlockCounter = " << blockCounter << MOST_LOG_ENDL;
                    MOST_LOG_INFO << LOG_TAG_DMB_CNT << "TPEGDataRequestHandler::BlockSize = " << m_xTPEGData.nDataLength << MOST_LOG_ENDL;
*/
// 2014.07.22, for log, hyunyong.sung
			MOST_LOG_SIGNAL << LOG_TAG_DMB_CNT << "TPEGDataRequest_StartResultAck <-- : SenderHandle = " << senderHandle
			<<" BlockCounter = " << blockCounter << " BlockSize = " << m_xTPEGData.nDataLength << MOST_LOG_ENDL;					
			
                }
            }
            else
            {
                m_TPEGDataRequest->ErrorAck(sourceDeviceID, senderHandle, 0x41, 0, NULL);
            }
            break;
        default:            
            MOST_LOG_SIGNAL << LOG_TAG_DMB_CNT << "Undefined Operation of TPEGDataRequestHandler" << MOST_LOG_ENDL;
            return;
    }
}

