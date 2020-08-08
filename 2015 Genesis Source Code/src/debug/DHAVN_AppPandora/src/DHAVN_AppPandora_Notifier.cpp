#include "DHAVN_AppPandora_Notifier.h"
#include "DHAVN_AppPandoraBase.h"

 /**
 *******************************************************************
 * Constructor
 *
 * @param[in] inEngine , Base class pointer
 *
 *******************************************************************
 */
CExternalNotifier::CExternalNotifier(CAppPandoraBase* inEngineMain )
: m_pEngineMain(inEngineMain)
{
    LOG_INIT_V2("Pandora");

    // Cluster:
    m_pQCanController = new CQCANController_ApplnTxRx(this);
    m_pQCanController->Init();

    if (QDBusConnection::sessionBus().interface()->isServiceRegistered("com.lge.car.micom"))
    {
        m_pCanTPInterface = new QDBusInterface("com.lge.car.micom", "/Dtc", "com.lge.car.micom.dtc", QDBusConnection::sessionBus()); //{ added by esjang 2012.11.20 CAN TP Message
    }
    else
    {
        m_pCanTPInterface = NULL;
    }

    connect(&(m_pEngineMain->GetStationListRef()),SIGNAL(tunedStation(quint32 , QString , bool)),this,SLOT(OnTunedStation(quint32 , QString , bool)));

    m_pOSDDisplay = new QDeclarativeView();  

    isSkiped = false; 
}

/**
 *******************************************************************
 * Destructor
 *
 *******************************************************************
 */
CExternalNotifier::~CExternalNotifier()
{
    delete m_pQCanController;
    delete m_pCanTPInterface;
    delete m_pOSDDisplay; 
}

/**
 *******************************************************************
 * SendClusterData
 *
 *******************************************************************
 */
void CExternalNotifier::SendClusterData(quint64 data)
{
    SQCanFrame frame = {0x114, 0x08, {0,0,0,0,0,0,0,0}};
    data = qToBigEndian(data);
    memcpy(&frame.uqcandata, &data, sizeof(quint64));

    if(m_pEngineMain && (m_pEngineMain->IsClusterUpdate())&& (!m_pEngineMain->IsCalling())) //modified by cheolhwan 2014-04-09. added the condition "m_pEngineMain" to prevent crash.
    {
        emit Can_data_send(frame);
    }
}

/**
 *******************************************************************
 * SendClusterData_Progress
 *
 *******************************************************************
 */
void CExternalNotifier::SendClusterData_Progress(int position)
{
    quint64 data = 0;
    data = 0x0D; 

    //HU Tuner Area(3bits)
    data <<= 3;
    data += 0x7;

    //HU Preset Number(5bits)
    data <<= 5;
    data += 0x1f;

    //HU Track Number (16bits)
    data <<= 16;
    data += 0xffff;

    //HU_Play_time_Sec (6bits)
    data <<= 6;
    data += position % 60;

    //HU_Play_time_Min (7bits)
    data <<= 7;
    data += (position % 3600) / 60;

    //HU_Play_time_Hour (6bits)
    data <<= 6;
    data += position / 3600;

    //HU Frequency(9bits)
    data <<= 9;
    data += 0x1ff;

    //HU Disc select No(4bits)
    data <<= 4;
    data += 0xf;

    SendClusterData(data);

}

void CExternalNotifier::SendClusterData_OpStateOnly()
{
    
    quint64 data = 0;

    // HU NAVI ONOFF (first 1bit)
    // HU_OpState (7 bits)
    data = GetClusterState();

    // HU_Tuner_Area (3bits)
    data <<= 3;
    data += 0x7;

    // HU_Preset_Number (5bits)
    data <<= 5;
    data += 0x1f;

    // HU_Track_Number (16bits)
    data <<= 16;
    data += 0xffff;

    // HU_Play_time_Sec (6bits)
    data <<= 6;
    data += 0x3f;

    // HU_Play_time_Min (7bits)
    data <<= 7;
    data += 0x7f;

    // HU_Play_time_Hour (6bits)
    data <<= 6;
    data += 0x3f;

    // HU_Frequency (9bits)
    data <<= 9;

    // HU_Disc_select_No (4bits)
    data <<= 4;
    data += 0xf;

    SendClusterData(data);
    
}

/**
 *******************************************************************
 * GetClusterState
 *
 *******************************************************************
 */
int CExternalNotifier::GetClusterState()
{
    return 0x0D;
}

/**
 *******************************************************************
 * UpdateTrackTitle
 *
 *******************************************************************
 */
void CExternalNotifier::UpdateTrackTitle(QString title)
{
    sendCanTpMessage(0x48F , title);
    if(isSkiped)
    {
        SetSkiped(false);
    }
}

/**
 *******************************************************************
 * sendCanTpMessage
 *
 *******************************************************************
 */
bool CExternalNotifier::sendCanTpMessage(quint16 id, QString data)
{
    LOG_INFO << "CExternalNotifier::sendCanTpMessage() <-- TP id : " << id << " data: " << data << LOG_ENDL;
    LOG_INFO << "CExternalNotifier::sendCanTpMessage: "<<m_pEngineMain->IsClusterUpdate()<< " Calling: "<<m_pEngineMain->IsCalling()<<LOG_ENDL;

    if((m_pEngineMain->IsClusterUpdate())&&(!m_pEngineMain->IsCalling()))
    {
        QDBusReply<bool> reply;

        QByteArray buffer;
        buffer.resize(100);

        buffer.clear();

        quint8 nLength2 = data.size();

        buffer[0] = (id >> 8);
        buffer[1] = (id & 0xff);

        if(id == 0x48F) // 0x48F : IPOD, Pandora
        {

            if(data == "")
            {
                LOG_INFO << "CExternalNotifier::sendCanTpMessage() Null title" << LOG_ENDL;
                buffer[2] = 3; // opcode(1byte) + clear TP(2byte)
                buffer[4] = 0x0; // clear TP
                buffer[5] = 0x0; // clear TP
            }
            else
            {
                buffer[2] = nLength2 * 2 + 1; // for opcode
            }


            buffer[3] = 0x2; // opcode : 0x2
        }
        else
        {
            LOG_INFO << "CExternalNotifier::sendCanTpMessage() Null title not iPod and pandora " << LOG_ENDL;

            if(data == "")
            {
                buffer[2] = 2;      // length
                buffer[3] = 0x0;    // clear TP
                buffer[4] = 0x0;    // clear TP
            }
            else
            {
                buffer[2] = nLength2 * 2;
            }
        }
        
        if(data != "")
        {
            QByteArray tpbody((const char*) (data.utf16()), data.size() * 2);
            buffer.append(tpbody);
        }

        if(m_pCanTPInterface == NULL)
        {
            if(QDBusConnection::sessionBus().interface()->isServiceRegistered("com.lge.car.micom"))
            {
                m_pCanTPInterface = new QDBusInterface("com.lge.car.micom", "/Dtc", "com.lge.car.micom.dtc", QDBusConnection::sessionBus());
            }
            else
            {
                LOG_INFO << "CExternalNotifier::sendCanTpMessage() Service com.lge.car.micom is not registered" << LOG_ENDL;
            }
        }

        if(m_pCanTPInterface && m_pCanTPInterface->isValid())
        {
            reply = m_pCanTPInterface->call("LgeDBus_CANTPSendMsg", buffer);
        }

        LOG_INFO << "CExternalNotifier::sendCanTpMessage() -->" << LOG_ENDL;

        return reply;
    }
    else
    {
        LOG_INFO <<"CExternalNotifier::sendCanTpMessage() -->" << LOG_ENDL; 
        return false;
    }
}

/**
 *******************************************************************
 * Clear OSD
 *
 * @param[in] None
 *
 * @return  None
 *
 *******************************************************************
 */
void CExternalNotifier::ClearOSD()
{
    LOG_INFO << " CExternalNotifier::ClearOSD <--" << LOG_ENDL;
    QString str = "";

    m_pOSDDisplay = m_pEngineMain->GetCurrentView();
    str = "@OP^CLEAR_AV";

    OSDWriter::SendOSD(str); 
}

/**
 *******************************************************************
 * Clear OSD
 *
 * @param[in] None
 *
 * @return  None
 *
 *******************************************************************
 */
void CExternalNotifier::ClearAVOSD()
{
    QString str = "";

    m_pOSDDisplay = m_pEngineMain->GetCurrentView();
    str = "@OSD:FR:CLEAR_AV";
    OSDWriter::SendOSD(str);  
}

/**
 *******************************************************************
 * Clear Cluster
 *
 * @param[in] None
 *
 * @return  None
 *
 *******************************************************************
 */
void CExternalNotifier::ClearCluster()
{
    LOG_INFO << "CExternalNotifier::ClearCluster <--" << LOG_ENDL;
    SendClusterData_OpStateOnly();
    //sendCanTpMessage(0x48F , ""); add by wonseok.heo for delete clearTP 2015.02.04
}

/**
 *******************************************************************
 * SendNullToCluster Cluster
 *
 * @param[in] None
 *
 * @return  None
 *
 *******************************************************************
 */
void CExternalNotifier::SendNullToCluster()
{
    LOG_INFO << "CExternalNotifier::SendNullToCluster <--" << LOG_ENDL;
    sendCanTpMessage(0x48F , "");
}


/**
 *******************************************************************
 * Device disconnected , update OSD.
 *
 * @param[in] inOSDEvent , type of osd event
 *
 * @return  None
 *
 *******************************************************************
 */
void CExternalNotifier::DisplayOSD(CExternalNotifier::HandleOSDEvent inOSDEvent , QString info)
{

   LOG_INFO << " CExternalNotifier::DisplayOSD <--" << LOG_ENDL;
   LOG_INFO << " inOSDEvent: " << inOSDEvent << LOG_ENDL;

   m_pOSDDisplay = m_pEngineMain->GetCurrentView();

   if(m_pEngineMain == NULL)
   {
       LOG_CRITICAL << " engine instance are null ?" << LOG_ENDL ;
       return ;
   }

     if( !m_pEngineMain->IsRunningInBG() && !m_pEngineMain->IsException())
     {
         LOG_CRITICAL << "DisplayOSD::IsRunningInBG : inOSDEvent =" << inOSDEvent << LOG_ENDL ;
         return;
     }

    QString str = "";
    TrackInfo* track_info   = NULL;
    QString trackTitle      = "";
    QString stationName     = "";

    track_info = m_pEngineMain->GetTrackRef().GetCurrentTrackInfo();
    if(track_info != NULL)
    {
        trackTitle = track_info->getTitle();
        stationName = track_info->getStationName();
        LOG_INFO << "TrackTitle : " << trackTitle << " StationName : " << stationName << LOG_ENDL;
    }
		
    switch(inOSDEvent)
    {
        case ETrackInfo:
        {
            
            if(track_info != NULL)
            {
                LOG_INFO << "TrackTitle : " << trackTitle << " StationName : " << stationName << LOG_ENDL;

                str = "@OSD:FR^MEDIA^INFO^PANDORA^";
        	    str += "Pandora^";
	            str += "MUSIC^";

                LOG_INFO << "Encoding any charactors(ex.'^', '\') in TrackTitle"<< LOG_ENDL;
                str += PNDR_ENCODE_OSD_FORMAT_RULE1( PNDR_ENCODE_OSD_FORMAT_RULE2( trackTitle ) );
                str += "^";

                LOG_INFO << "Encodded TrackTitle : " << PNDR_ENCODE_OSD_FORMAT_RULE1( PNDR_ENCODE_OSD_FORMAT_RULE2( trackTitle ) )<< LOG_ENDL;

                LOG_INFO << "OSD message ->  " <<trackTitle<< LOG_ENDL;

                if(stationName == "Shuffle" || stationName == "QuickMix")
                {
                    LOG_INFO << "set shuffle indicator icon " << LOG_ENDL;
                    str += "(SH)";
                }
            }
            else
            {
                LOG_INFO << " No Track info available " << LOG_ENDL;
                str = "@OSD:FR^MEDIA^INFO^PANDORA^";
                str += "Pandora^";
                str += "^";
            }
	        break;
        }
        case ESeekUp:
        {
            LOG_INFO << "OSD message -> Skip this song " << LOG_ENDL;
            
            str = "@OSD:FR^MEDIA^INFO^PANDORA^"; 
    	    str += "Pandora^";
            str += "MUSIC^";
            str += qApp->translate("main","STR_PANDORA_SKIP_THIS_SONG_OSD");
            break;
        }
        //{ modified by yongkyun.lee 2014-07-17 for : ITS 243240
        case EThumbsUp:
        {
            LOG_INFO << "OSD message -> Skip this song " << LOG_ENDL;
            
            str = "@OSD:FR^MEDIA^INFO^PANDORA^"; 
            str += "Pandora^";
            str += "MUSIC^";
            str += qApp->translate("main","STR_PANDORA_THUMBSUP");
            break;
        }
        case EThumbsDown:
        {
            LOG_INFO << "OSD message -> Skip this song " << LOG_ENDL;
            
            str = "@OSD:FR^MEDIA^INFO^PANDORA^"; 
    	    str += "Pandora^";
            str += "MUSIC^";
            str += qApp->translate("main","STR_PANDORA_THUMBSDOWN");
            break;
        }
        //} modified by yongkyun.lee 2014-07-17 
        case ESkipLimitReached:
        {
            LOG_INFO << "OSD message -> Skip limit reached " << LOG_ENDL;
            
            str = "@OSD:FR^MEDIA_INFO^WARN^PANDORA^";
    	    str += "Pandora^";
            str += qApp->translate("main","STR_PANDORA_MAX_SKIP_LIMIT_OSD");
            break;
        }
        case ESeekDown:
        {
            LOG_INFO << "OSD message -> Rewind not available in pandora " << LOG_ENDL;
            
    	    str = "@OSD:FR^MEDIA_INFO^WARN^PANDORA^"; 
    	    str += "Pandora^";
            str += qApp->translate("main","STR_PANDORA_REWIND_NOT_AVAILABLE_OSD");
            break;
        }

        case ETuneKeyDial:
        {
            str = "@OSD:FR^MEDIA^INFO_DIAL^PANDORA^";
            str += "Pandora^";
            str += "MUSIC^";
            str += PNDR_ENCODE_OSD_FORMAT_RULE1( PNDR_ENCODE_OSD_FORMAT_RULE2( info ) );
            str += "^";

        }
        break;        

        case ETuneCurrStation:
        {

            if(info != NULL)
            {
                str = "@OSD:FR^MEDIA^INFO^PANDORA^";
                    str += "Pandora^";
                    str += "MUSIC^";
                str += PNDR_ENCODE_OSD_FORMAT_RULE1( PNDR_ENCODE_OSD_FORMAT_RULE2( info ) );
                str += "^";
                if(info == "Shuffle" || info == "QuickMix")
                {
                    LOG_INFO << "set shuffle indicator icon " << LOG_ENDL;
                    str += "(SH)";
                }
            }
            else
            {
                LOG_INFO << "get stationName from track info : " << stationName << LOG_ENDL;

                str = "@OSD:FR^MEDIA^INFO^PANDORA^";
                    str += "Pandora^";
                    str += "MUSIC^";
                str += PNDR_ENCODE_OSD_FORMAT_RULE1( PNDR_ENCODE_OSD_FORMAT_RULE2( stationName ) );
                str += "^";
            }
        }
        break;

        case ECallingState:
            str = "@OSD:FR^MEDIA_INFO^WARN^PANDORA^";
    	    str += "Pandora^";
            str += qApp->translate("main","STR_PANDORA_CALLING_STATE_OSD");
            
        break;
        //{ modified by yongkyun.lee 2014-09-18 for : ITS 248606
        case Econnecting: 
            str = "@OSD:FR^CONNECT_INFO^LOADING^PANDORA^"; //str = "@OSD:FR^MEDIA^INFO^PANDORA^";
            str += "Pandora^";
            str += qApp->translate("main","STR_PANDORA_CONNECTING_VIEW_TEXT1");
        break;
        //} modified by yongkyun.lee 2014-09-18 
        case EDisconnect: 
            str = "@OSD:FR^MEDIA_INFO^WARN^PANDORA^";
    	    str += "Pandora^";
            str += qApp->translate("main","STR_PANDORA_DISCONNECTED");

        break;
        case ENetworkErrorOSD: 
            str = "@OSD:FR^MEDIA_INFO^WARN^PANDORA^";
    	    str += "Pandora^";
            str += qApp->translate("main","STR_PANDORA_UNABLE_TO_PLAY_OSD");            
        break;

        case ECannotBeManipulated:
            str = "@OSD:FR^MEDIA_INFO^WARN^PANDORA^";
            str += "Pandora^";
            str += qApp->translate("main","STR_PANDORA_CANNOT_BE_MANIPULATED");
        break;

        case ENoNetwork:
            str = "@OSD:FR^MEDIA_INFO^WARN^PANDORA^";
            str += "Pandora^";
            str += qApp->translate("main","STR_PANDORA_TRACK_VIEW_NO_NETWORK_TEXT");
        break;

        //{ modified by yongkyun.lee 2014-07-17 for : ITS 243240
        case EStationDoesNotExist:
        {
            str = "@OSD:FR^MEDIA_INFO^WARN^PANDORA^";
            str += "Pandora^";
            str += qApp->translate("main","STR_PANDORA_STATION_DOES_NOT_EXIST");
            break;
        }
        case EFailedToConnect:
        {
            str = "@OSD:FR^MEDIA_INFO^WARN^PANDORA^";
            str += "Pandora^";
            str += qApp->translate("main","STR_PANDORA_FAILED_TO_CONNECT_TO_PANDORA_RADIO");
            break;
        }
        //} modified by yongkyun.lee 2014-07-17 

        default:
        break;        

    }

     OSDWriter::SendOSD(str);
     str.clear();

}

/**
 *******************************************************************
 * Display AV Off on OSD
 *
 * @param[in] None
 *
 * @return  None
 *
 *******************************************************************
 */
void CExternalNotifier::DisplayOSDAVOff()
{
    LOG_INFO << " CExternalNotifier::DisplayOSDAVOff <--" << LOG_ENDL;

    m_pOSDDisplay = m_pEngineMain->GetCurrentView(); 

    if(m_pEngineMain == NULL)
    {
        LOG_CRITICAL << " engine instance are null ?" << LOG_ENDL ;
        return ;
    }
}

/**
 *******************************************************************
 * Device disconnected , update OSD.
 *
 * @param[in] None
 *
 * @return  None
 *
 *******************************************************************
 */
void CExternalNotifier::DeviceDisconnected()
{
    LOG_INFO << "CExternalNotifier::DeviceDisconnected() <--" << LOG_ENDL;

    m_pOSDDisplay = m_pEngineMain->GetCurrentView();

    if(m_pEngineMain == NULL)
    {
        LOG_CRITICAL << " engine instance are null ?" << LOG_ENDL ;
        return ;
    }
    if(!m_pEngineMain->IsRunningInBG() || m_pEngineMain->IsAVModeChanged() )
    {
        return ;
    }

        QString str = "";
        
        str = "@OSD:FR^MEDIA_INFO^WARN^PANDORA^";
        str += "Pandora^";
        str += qApp->translate("main","STR_PANDORA_NO_CONNECTED_DEVICE");

}

 void CExternalNotifier::OnTunedStation(quint32 /*token*/, QString inStation , bool isCurr)
 {
    //{ modified by yongkyun.lee 2014-11-03 for : //leeyk1 OSD TEST
    if(m_pEngineMain->isActiveView() == CAppPandoraBase::ETrackView && !m_pEngineMain->IsRunningInBG() )
        return;
    //} modified by yongkyun.lee 2014-11-03 
    
    if(isCurr){
    DisplayOSD(CExternalNotifier::ETuneCurrStation , inStation);
    }
    else{
        DisplayOSD(CExternalNotifier::ETuneKeyDial , inStation);
    }
 }


 /**
  *******************************************************************
  * Forcee update , irrespective of any mode and state.
  *
  * @param[in] None
  *
  * @return  None
  *
  *******************************************************************
  */
 void CExternalNotifier::UpdateOSDOnCallingState()
 {
     QString str("");
     str = "@OSD:FR^MEDIA_INFO^WARN^PANDORA^";
     str += "Pandora^";
     str += qApp->translate("main","STR_PANDORA_CALLING_STATE_OSD");
     OSDWriter::SendOSD(str);
 }
