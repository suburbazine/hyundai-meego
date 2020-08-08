#include <QTime>
#include <QTimer>


#include <uistate_manager_def.h>
#include "DHAVN_AppFramework_Def.h"
#include "DHAVN_AppIBox_MOSTMessageHandler.h"
#include "DHAVN_AppIBox_LogUtils.h"
#include "DHAVN_AppIBox_Def.h"
#include "DHAVN_AppIBox_MOST_Def.h"

MOSTMessageHandler::MOSTMessageHandler(Logger &pLogger, IStatusBarController *pStatuBarController,
                                       IEventSender *pEventSender, CANMessageSender *pCanMessageSender, QObject *parent) :
    m_pLogger(pLogger),
    m_pCanMessageSender(pCanMessageSender),
    QObject(parent),
    m_uiCurDeckStatus(0xFF),
    m_pStatusBarController(pStatuBarController),
    m_pEventSender(pEventSender),
    m_pTelNum( "" ),
	m_modemStatus(-1), m_modemLevel(-1)
{
    m_pMostDlnaQdbusInterface = new QDBusInterface( MOST_MANAGER_SERVICE_NAME,
                                                    MOST_MANAGER_DLNA_PATH,
                                                    MOST_MANAGER_DLNA_INTERFACE,
                                                    QDBusConnection::sessionBus() );

    if( !m_pMostDlnaQdbusInterface->isValid() )
    {
        CR_LOG("DLNA Adaptor: Invalid D-Bus Interface - IBox might not work!" );
    }

    m_pMostHMIQdbusInterface = new QDBusInterface( MOST_MANAGER_SERVICE_NAME,
                                                  MOST_MANAGER_HMI_PATH,
                                                  MOST_MANAGER_HMI_INTERFACE,
                                                  QDBusConnection::sessionBus() );

  if( !m_pMostHMIQdbusInterface->isValid() )
  {
      CR_LOG("HMI Adaptor: Invalid D-Bus Interface - IBox might not work!" );
  }

    connectDBUS_MOST();

    /*
    QTimer* testTimer = new QTimer;

    testTimer->setInterval(20000);
    testTimer->setSingleShot(false);

    connect (testTimer, SIGNAL(timeout()), this, SLOT(OnDisplayOSDTest()));
    testTimer->start();
    */
}

MOSTMessageHandler::~MOSTMessageHandler()
{

    disconnectDBUS_MOST();

    if (m_pMostDlnaQdbusInterface)
    {
        delete m_pMostDlnaQdbusInterface;
    }

    if (m_pMostHMIQdbusInterface)
    {
        delete m_pMostHMIQdbusInterface;
    }

    if (m_pCanMessageSender)
    {
        delete m_pCanMessageSender;
    }
}

void MOSTMessageHandler::requestStatusBarIndicatorInfo()
{
    LW_LOG("");

    getModemStatus();

    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    QDBusMessage requestModemLevel = QDBusMessage::createMethodCall( MOST_MANAGER_SERVICE_NAME,
                                                                     MOST_MANAGER_PHONE_PATH,
                                                                     MOST_MANAGER_PHONE_INTERFACE,
                                                                     "GetModemLevel");

    sessionBus.call(requestModemLevel, QDBus::NoBlock);

    QDBusMessage requestWifiMode = QDBusMessage::createMethodCall( MOST_MANAGER_SERVICE_NAME,
                                                                   MOST_MANAGER_IBOX_PATH,
                                                                   MOST_MANAGER_IBOX_INTERFACE,
                                                                   "GetWiFiMode");

    sessionBus.call(requestWifiMode, QDBus::NoBlock);

}


void MOSTMessageHandler::onTimePosition(const int diskTime, const int trackTimeMS, const int titleTime)
{
    QTime trackTime;
    trackTime = trackTime.addMSecs(trackTimeMS);
//    LOG(QString("diskTime=[%1], trackTime=[%2], titleTime=[%3]").
//        arg(diskTime).arg(trackTimeMS).arg(titleTime));
//    LOG(QString("trackTime(hour,min,sec)=[%1],[%2],[%3]").
//        arg(trackTime.hour()).arg(trackTime.minute()).arg(trackTime.second()));

    CANMessage_HU_CLU_PE_09 message(GetLogger());
    message.setField(CANMessage_HU_CLU_PE_09::FIELD_OP_STS, CANMessage_HU_CLU_PE_09::OP_STS_DLNA);
    //    message.setField(CANMessage_HU_CLU_PE_09::FIELD_PRESET_NO, 0);
    //    message.setField(CANMessage_HU_CLU_PE_09::FIELD_FREQ_CH_NO, 0);
    //    message.setData(CANMessage_HU_CLU_PE_09::FIELD_HD_NO, 0);
    //    message.setData(CANMessage_HU_CLU_PE_09::FIELD_DISC_NO, 0);
    message.setField(CANMessage_HU_CLU_PE_09::FIELD_PLAY_TIME_HOUR, (uchar)trackTime.hour());
    message.setField(CANMessage_HU_CLU_PE_09::FIELD_PLAY_TIME_MIN, (uchar)trackTime.minute());
    message.setField(CANMessage_HU_CLU_PE_09::FIELD_PLAY_TIME_SEC, (uchar)trackTime.second());
    //    message.setData(CANMessage_HU_CLU_PE_09::FIELD_TRACK_NO, 0);

    m_pCanMessageSender->send(dynamic_cast<CANMessage*>(&message));
}

void MOSTMessageHandler::sendIncrementTrackPosition()
{
    LW_LOG ("");

    if(m_pMostDlnaQdbusInterface->isValid())
    {
        uint uiValue = 1;
        m_DBusreply = m_pMostDlnaQdbusInterface->call("SetIncrementTrackPosition", uiValue );
        LW_LOG("DBus Reply - m_DBusreply:" << m_DBusreply);
    }
    else
    {
        CR_LOG( "m_pMostDlnaQdbusInterface is not valid" );
    }
}

void MOSTMessageHandler::sendBackKeyOn(bool isSoftKey)
{
    //MOST Manager : bool SetKeyInputStatus(const uint hardKey, const uint softKey)
    LW_LOG ("");

    if(m_pMostHMIQdbusInterface->isValid())
    {
        if (isSoftKey) {
            m_DBusreply = m_pMostHMIQdbusInterface->call("SetKeyInputStatus", (uint)0, (uint)ONLY_BACK_KEY_IS_ON);
            LW_LOG("Sent Soft Back key input to IBox, DBus Reply :" << m_DBusreply) ;
        }
        else { //Hard key
            m_DBusreply = m_pMostHMIQdbusInterface->call("SetKeyInputStatus", (uint)ONLY_BACK_KEY_IS_ON, (uint)0);
            LW_LOG("Sent Hard Back key input to IBox, DBus Reply :" << m_DBusreply) ;
        }
    }
    else
    {
        CR_LOG( "m_pMostHMIQdbusInterface is not valid" );
    }
}

void MOSTMessageHandler::sendDecrementTrackPosition()
{
    LW_LOG ("");

    if(m_pMostDlnaQdbusInterface->isValid())
    {
        uint uiValue = 1;
        m_DBusreply = m_pMostDlnaQdbusInterface->call("SetDecrementTrackPosition", uiValue );
        LW_LOG("DBus Reply - m_DBusreply:" << m_DBusreply);
    }
    else
    {
        CR_LOG( "m_pMostDlnaQdbusInterface is not valid" );
    }
}

void MOSTMessageHandler::sendPlayPause( bool isPlay )
{
    //LOG( QString( "isPlay - %1" ).arg( isPlay ) );

    if(m_pMostDlnaQdbusInterface->isValid())
    {
        uint uiValue;
        //! Play
        if ( isPlay )
        {
            uiValue = 0x0;
            m_DBusreply = m_pMostDlnaQdbusInterface->call("SetDeckStatus", uiValue );
        }
        //! Pause
        else
        {
            uiValue = 0x2;
            m_DBusreply = m_pMostDlnaQdbusInterface->call("SetDeckStatus", uiValue );
        }
        //LOG(QString("DBus Reply - %1").arg(m_DBusreply));
    }
    else
    {
        CR_LOG( "m_pMostDlnaQdbusInterface is not valid" );
    }
}

void MOSTMessageHandler::onCurrentSongInfo(const uint playtime, const ushort trackno, const QStringList songinfo)
{
    LW_LOG("playtime:" << playtime << ", trackno:" << trackno);

    for (int i = 0; i < songinfo.count(); i++)
    {
        LW_LOG("songinfo[" << i << "]=[" << songinfo[i]);
    }

    //! CR 14592: Don't display AV info
    /*
    HOME_MEDIA_T stData;

    memset( &stData, 0, sizeof( HOME_MEDIA_T ) );
    stData.nType = HomeScreenDef::MEDIA_TYPE_AUDIO;
    stData.nSource = HomeScreenDef::MEDIA_SOURCE_DLNA;

    if ( songinfo.size() > SILI_ARTIST )
    {
       //songinfo.at( SILI_ARTIST ).left( MEDIA_STRING_LENGHT ).toWCharArray( stData.sMedia1 );
       //songinfo.at( SILI_TITLE ).left( MEDIA_STRING_LENGHT ).toWCharArray( stData.sMedia2 );
    }

    emit SendDLNAMediaInfoToHome( &stData );
    */
}

void MOSTMessageHandler::onCurrentVideoInfo(const QString vtitle, const uint time, const ushort track, const QString vfilename)
{
    LW_LOG("vtitle:" << vtitle << ", time:" << time << ", track:" << track << ", vfilename:" << vfilename);

    //! CR 14592: Don't display AV info
    /*
    HOME_MEDIA_T stData;

    memset( &stData, 0, sizeof( HOME_MEDIA_T ) );
    stData.nType = HomeScreenDef::MEDIA_TYPE_VIDEO;
    stData.nSource = HomeScreenDef::MEDIA_SOURCE_DLNA;

    emit SendDLNAMediaInfoToHome( &stData );
    */
}

void MOSTMessageHandler::onDeckStatus( const uint deckStatus )
{
    LW_LOG("deckStatus:" << deckStatus);
}

void MOSTMessageHandler::onShowStatusBar(const uint statusbar)
{
    LW_LOG("statusbar:" << statusbar);

    switch (statusbar)
    {
    case 0: // Hide
        m_pStatusBarController->hideStatusBar();
        break;

    case 1: // Show
        m_pStatusBarController->showStatusBar();
        break;
    }
}


void MOSTMessageHandler::OnDisplayOSD( const QDBusVariant &displayOSD )
{
    LW_LOG("------->> ") ;

    struct SDisplayOSD sDisplayOSD;

    displayOSD.variant().value<QDBusArgument>() >> sDisplayOSD;

    QString osdCommand ("@OSD");

    bool swapped = m_pStatusBarController->swapped();
    CLONE_STATUS_T clone = m_pStatusBarController->cloneStatus();

    if (sDisplayOSD.target == 0x00)
    {
        osdCommand.append(":FR");
    }
    else if (sDisplayOSD.target == 0x01)
    {
        if (clone == CLONE_ON_FRONT)
            osdCommand.append(":F");

        else if (clone == CLONE_ON_REAR )
            osdCommand.append(":R");

        else if (swapped)
            osdCommand.append(":R");

        else
            osdCommand.append(":F");
    }
    else if (sDisplayOSD.target == 0x02)
    {
        if (clone == CLONE_ON_FRONT)
            osdCommand.append(":F");

        else if (clone == CLONE_ON_REAR )
            osdCommand.append(":R");

        else if (swapped)
            osdCommand.append(":F");

        else
            osdCommand.append(":R");
    }
    else if (sDisplayOSD.target == 0x03)
    {
        if (clone == CLONE_ON_FRONT)
            osdCommand.append(":F");

        else if (clone == CLONE_ON_REAR )
            osdCommand.append(":R");

        else if (swapped)
            osdCommand.append(":F");

        else
            osdCommand.append(":R");
    }

    //osdCommand.append("^COMMON^INFO^");
    //osdCommand.append(sDisplayOSD.OSDtext);

    //OSD^IBOX^OSDMode^Main Icon Index^Sub Icon Index^Text Color1^Text Color2^Text Color3^OSDText
    osdCommand.append("^IBOX^");
    osdCommand.append(QString("%1^").arg(sDisplayOSD.osdMode));
    osdCommand.append(QString("%1^").arg(sDisplayOSD.mainIcon));
    osdCommand.append(QString("%1^").arg(sDisplayOSD.subIcon));
    osdCommand.append(QString("%1^").arg(sDisplayOSD.textColour1));
    osdCommand.append(QString("%1^").arg(sDisplayOSD.textColour2));
    osdCommand.append(QString("%1^").arg(sDisplayOSD.textColour3));
    osdCommand.append(sDisplayOSD.OSDtext);


    m_pStatusBarController->showOSD(osdCommand);

    LOG("<<------- ");
}

/*
void MOSTMessageHandler::OnDisplayOSDTest( )
{
    struct SDisplayOSD sDisplayOSD;

    sDisplayOSD.target = 0x00;
    sDisplayOSD.osdMode = 0xB0;
    sDisplayOSD.mainIcon = 0x01;
    sDisplayOSD.subIcon = 0x02;
    sDisplayOSD.textColour1 = 0x01;
    sDisplayOSD.textColour2 = 0x02;
    sDisplayOSD.textColour3 = 0x00;
    sDisplayOSD.OSDtext = "iBox OSD Test...";

    QString osdCommand ("@OSD");

    bool swapped = m_pStatusBarController->swapped();
    CLONE_STATUS_T clone = m_pStatusBarController->cloneStatus();


    if (sDisplayOSD.target == 0x00)
    {
        osdCommand.append(":FR");
    }
    else if (sDisplayOSD.target == 0x01)
    {
        if (clone == CLONE_ON_FRONT)
            osdCommand.append(":F");

        else if (clone == CLONE_ON_REAR )
            osdCommand.append(":R");

        else if (swapped)
            osdCommand.append(":R");

        else
            osdCommand.append(":F");
    }
    else if (sDisplayOSD.target == 0x02)
    {
        if (clone == CLONE_ON_FRONT)
            osdCommand.append(":F");

        else if (clone == CLONE_ON_REAR )
            osdCommand.append(":R");

        else if (swapped)
            osdCommand.append(":F");

        else
            osdCommand.append(":R");
    }
    else if (sDisplayOSD.target == 0x03)
    {
        if (clone == CLONE_ON_FRONT)
            osdCommand.append(":F");

        else if (clone == CLONE_ON_REAR )
            osdCommand.append(":R");

        else if (swapped)
            osdCommand.append(":F");

        else
            osdCommand.append(":R");
    }

    //osdCommand.append("^COMMON^INFO^");
    //osdCommand.append(sDisplayOSD.OSDtext);

    //OSD^IBOX^OSDMode^Main Icon Index^Sub Icon Index^Text Color1^Text Color2^Text Color3^OSDText
    osdCommand.append("^IBOX^");
    osdCommand.append(QString("%1^").arg(sDisplayOSD.osdMode));
    osdCommand.append(QString("%1^").arg(sDisplayOSD.mainIcon));
    osdCommand.append(QString("%1^").arg(sDisplayOSD.subIcon));
    osdCommand.append(QString("%1^").arg(sDisplayOSD.textColour1));
    osdCommand.append(QString("%1^").arg(sDisplayOSD.textColour2));
    osdCommand.append(QString("%1^").arg(sDisplayOSD.textColour3));
    osdCommand.append(sDisplayOSD.OSDtext);


    m_pStatusBarController->showOSD(osdCommand);

}
*/


/*
void MOSTMessageHandler::OnDisplayOSD( const uint target, const uint  osdMode, const uint mainIcon, const uint subIcon,
                  const uint textColour1, const uint textColour2, const uint textColour3, const QString OSDtext)
{
    LOG(QString("OnDisplayOSD  ------->> "));

    LOG( QString ("target [%1], text [%2]")
        .arg(target).arg(OSDtext));

    QString osdCommand ("OSD");

    if (target == 0x00)
    {
        osdCommand.append(":FG");
    }
    else if (target == 0x01)
    {
        osdCommand.append(":F");
    }
    else if (target == 0x02)
    {
        osdCommand.append(":R");
    }
    else if (target == 0x03)
    {
        osdCommand.append(":R");
    }

    osdCommand.append("^COMMON^INFO^");
    osdCommand.append(OSDtext);

    m_pStatusBarController->showOSD(osdCommand);

    LOG(QString("OnDisplayOSD  <<------- "));
}
*/

void MOSTMessageHandler::onAdjustScreenFromIBOX( const int brightness, const int saturation, const int contrast, const int hue )
{
    LW_LOG("brightness:" << brightness << ", saturation:" << saturation << ", contrast:" << contrast << ", hue:" << hue);

    EVENT_SETTINGS_CHANGED_DATA_T message;

    message.sHeader.appIdReciever = APP_SETTINGS;
    message.sHeader.reasonEvent = EVT_POST_EVENT;
    message.sSourceDisplay = SPRITEC_PLANE;

    // Make parsed string and send event. It is used for sending only one event.
    message.sKey = DB_KEY_VIDEO_BRIGHTNESS;
    QString sParseStr = QString().sprintf("%d|%d|%d|%d|", brightness, saturation, contrast, hue);
    memcpy( message.sValue2, sParseStr.toUtf8(),
            (sParseStr.length() >= (PATH_MAX_LENGTH+1)) ? PATH_MAX_LENGTH+1 : sParseStr.length() );

    QByteArray payload( (char *)&message, sizeof(EVENT_SETTINGS_CHANGED_DATA_T) );

    Event event( EVT_POST_EVENT, APP_IBOX, APP_SETTINGS, payload );

    if ( !m_pEventSender->sendEventToUISH( event ))
    {
        CR_LOG("Event notification failed");
    }
}

void MOSTMessageHandler::onLCDBrightnessFromIBOX( const int target, const int lcdFront, const int lcdRear1, const int lcdRear2 )
{
    LW_LOG("target:" << target << ", front:" << lcdFront << ", lcdRear1:" << lcdRear1 << ", lcdRear2:" << lcdRear2);

    // Front Screen
    if ( target == 0 || target == 0x1 )
    {
        EVENT_SETTINGS_CHANGED_DATA_T message;

        message.sHeader.appIdReciever = APP_SETTINGS;
        message.sHeader.reasonEvent = EVT_POST_EVENT;
        message.sSourceDisplay = SPRITEC_PLANE;

        message.sKey = DB_KEY_FRONT_SCREENBRIGHTNESS;
        message.sValue = lcdFront;

        QByteArray payload( (char *)&message, sizeof(EVENT_SETTINGS_CHANGED_DATA_T) );

        Event event( EVT_POST_EVENT, APP_IBOX, APP_SETTINGS, payload );

        if ( !m_pEventSender->sendEventToUISH( event ) )
        {
            CR_LOG("Event notification failed");
        }
    }

    // Rear Screen
    if ( target == 0 || target == 0x2 )
    {
        EVENT_SETTINGS_CHANGED_DATA_T message;

        message.sHeader.appIdReciever = APP_SETTINGS;
        message.sHeader.reasonEvent = EVT_POST_EVENT;
        message.sSourceDisplay = SPRITEC_PLANE;

        message.sKey = DB_KEY_REAR_SCREENBRIGHTNESS;
        message.sValue = lcdRear1;

        QByteArray payload( (char *)&message, sizeof(EVENT_SETTINGS_CHANGED_DATA_T) );

        Event event( EVT_POST_EVENT, APP_IBOX, APP_SETTINGS, payload );

        if ( !m_pEventSender->sendEventToUISH( event ) )
        {
            CR_LOG("Event notification failed");
        }
    }
}

void MOSTMessageHandler::onIlluminationFromIBOX( const int illumination )
{
    LW_LOG("illumination =" << illumination);

    EVENT_SETTINGS_CHANGED_DATA_T message;

    message.sHeader.appIdReciever = APP_SETTINGS;
    message.sHeader.reasonEvent = EVT_POST_EVENT;
    message.sSourceDisplay = SPRITEC_PLANE;

    message.sKey = DB_KEY_EXPOSURE;
    message.sValue = illumination;

    QByteArray payload( (char *)&message, sizeof(EVENT_SETTINGS_CHANGED_DATA_T) );

    Event event( EVT_POST_EVENT, APP_IBOX, APP_SETTINGS, payload );

    if ( !m_pEventSender->sendEventToUISH( event ) )
    {
        CR_LOG("Event notification failed");
    }
}

void MOSTMessageHandler::onVideoRatioFromIBOX( const int videoRatio )
{
    LW_LOG("videoRatio: " << videoRatio) ;

    EVENT_SETTINGS_CHANGED_DATA_T message;

    message.sHeader.appIdReciever = APP_SETTINGS;
    message.sHeader.reasonEvent = EVT_POST_EVENT;
    message.sSourceDisplay = SPRITEC_PLANE;

    message.sKey = DB_KEY_ASPECT_RADIO;
    message.sValue = videoRatio;

    QByteArray payload( (char *)&message, sizeof(EVENT_SETTINGS_CHANGED_DATA_T) );

    Event event( EVT_POST_EVENT, APP_IBOX, APP_SETTINGS, payload );

    if ( !m_pEventSender->sendEventToUISH( event ) )
    {
        CR_LOG("Event notification failed");
    }
}

void MOSTMessageHandler::onDisplayControlFromIBOX( const uint displayOnOff )
{
    LW_LOG("displayOnOff = " << displayOnOff);

    EVENT_SETTINGS_CHANGED_DATA_T message;

    message.sHeader.appIdReciever = APP_SETTINGS;
    message.sHeader.reasonEvent = EVT_POST_EVENT;
    message.sSourceDisplay = SPRITEC_PLANE;

    message.sKey = DB_KEY_REAR_ON;
    message.sValue = displayOnOff;

    QByteArray payload( (char *)&message, sizeof(EVENT_SETTINGS_CHANGED_DATA_T) );

    Event event( EVT_POST_EVENT, APP_IBOX, APP_SETTINGS, payload );

    if ( !m_pEventSender->sendEventToUISH( event ) )
    {
        CR_LOG("Event notification failed");
    }
}

void MOSTMessageHandler::onVideoDefaultRequestIBOX( const uint deviceID, const ushort senderHandle, const uint sTarget )
{
    LW_LOG("deviceID:" << deviceID << ", senderHandle:" << senderHandle << ", sTarget:" << sTarget);

    EVENT_SETTINGS_CHANGED_DATA_T message;

    message.sHeader.appIdReciever = APP_SETTINGS;
    message.sHeader.reasonEvent = EVT_POST_EVENT;
    message.sSourceDisplay = SPRITEC_PLANE;

    //! Make parsed string and send event. It is used for sending only one event.
    //! TODO: It needs to modify DB key type when new DB key is added
    message.sKey = DB_KEY_PHOTO_FRAME;
    char * pMsgValue = &message.sValue2[0];
    memcpy( pMsgValue, &deviceID, sizeof(uint) );
    pMsgValue += sizeof(uint);
    memcpy( pMsgValue, &senderHandle, sizeof(ushort) );
    pMsgValue += sizeof(ushort);
    memcpy( pMsgValue, &sTarget, sizeof( uint) );

    QByteArray payload( (char *)&message, sizeof(EVENT_SETTINGS_CHANGED_DATA_T) );

    Event event( EVT_POST_EVENT, APP_IBOX, APP_SETTINGS, payload );

    if ( !m_pEventSender->sendEventToUISH( event ) )
    {
        CR_LOG("VideoDefaultRequest Event notification failed");
    }
}

void MOSTMessageHandler::onModemDataStatus( const uint status )
{
    QString pStr = "@STA^3G_UD^";

    m_modemStatus = (int) status;

    if ( modemServiceAvailable()) {
        if ( m_modemLevel != -1)
            onModemLevel( (const uint) m_modemLevel);
    }

    if ( status == IMOSTDef::I_MDS_TRANSFERING_DATA_3G )
    {
        pStr += "1";
    }
    else
    {
        pStr += "0";
    }

    m_pEventSender->sendEventToStatusBar( pStr );
}

void MOSTMessageHandler::onWifiMode( const uint mode, const uint clientStatus )
{
    LW_LOG( "WiFi:" << mode << ", Client:" << clientStatus ) ;

    switch( mode )
    {
    case IMOSTDef::I_WMW_AP_MODE:
    {
        setAccessPointIcon( true );
        setWiFiAntennaIcon( IMOSTDef::I_WMC_INVALID );
        break;
    }

    case IMOSTDef::I_WMW_CLIENT_MODE:
    {
        setAccessPointIcon( false );
        setWiFiAntennaIcon( clientStatus );
        break;
    }

    case IMOSTDef::I_WMW_DIRECT_MODE:
    case IMOSTDef::I_WMW_NOT_ACTIVATED:
    case IMOSTDef::I_WMW_INVALID:
    default:
    {
        setAccessPointIcon( false );
        setWiFiAntennaIcon( IMOSTDef::I_WMC_INVALID );
        break;
    }
    }
}

bool MOSTMessageHandler::modemServiceAvailable(){
//	if ( m_modemStatus < 0)
//		getModemStatus();

	if ( m_modemStatus > 0 && m_modemStatus <= 0x06)
		return true;

	return false;
}

void MOSTMessageHandler::getModemStatus(){
    LW_LOG("");

    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    QDBusMessage requestModemStatus = QDBusMessage::createMethodCall( MOST_MANAGER_SERVICE_NAME,
                                                                     MOST_MANAGER_PHONE_PATH,
                                                                     MOST_MANAGER_PHONE_INTERFACE,
                                                                     "GetModemDataStatus");

    sessionBus.call(requestModemStatus, QDBus::NoBlock);
}

void MOSTMessageHandler::onModemLevel( const uint modemLevel )
{
    m_modemLevel = (int) modemLevel;

	if ( !modemServiceAvailable()){
		return;
	}

    QString pStr = "@STA^HSDPA_RSSI^";

    if ( modemLevel >= IMOSTDef::I_ML_MAX )
    {
        CR_LOG( "Modem Level is invalid value or hide." );
        pStr += "0^0";
    }

    else
    {
        pStr += QString( "1^%1" ).arg( modemLevel );
    }

    m_pEventSender->setStatusBarModemState(pStr);
    //m_pEventSender->sendEventToStatusBar( pStr );
}

void MOSTMessageHandler::onUserMode( const uint userMode, const bool micOff )
{
    LW_LOG( "userMode:" << userMode << ", micOff:" << micOff ) ;

    switch( userMode )
    {
    case IMOSTDef::I_UMU_HANDSFREE_MODE_FRONT:
    case IMOSTDef::I_UMU_HANDSFREE_MODE_REAR:
        if ( micOff == true )
        {
            setBluelinkCallIcon( true );
            setBluelinkMuteIcon( true );
        }
        else
        {
            setBluelinkCallIcon( true );
            setBluelinkMuteIcon( false );
        }
        break;

    case IMOSTDef::I_UMU_NOT_USED:
    default:
        setBluelinkCallIcon( false );
        setBluelinkMuteIcon( false );
        break;
    }
}

void MOSTMessageHandler::setAccessPointIcon( bool isEnable )
{
    QString pStr;

    if ( isEnable == true )
    {
        pStr = "@STA^AP^1";
    }
    else
    {
        pStr = "@STA^AP^0";
    }

    m_pEventSender->sendEventToStatusBar( pStr );
}

void MOSTMessageHandler::setWiFiAntennaIcon( int antenna )
{
    QString pStr = "";

    switch ( antenna )
    {
    case IMOSTDef::I_WMC_0:
    {
        pStr = "@STA^WIFI^0^0";
        break;
    }

    case IMOSTDef::I_WMC_1:
    {
        pStr = "@STA^WIFI^1^1";
        break;
    }

    case IMOSTDef::I_WMC_2:
    {
        pStr = "@STA^WIFI^1^2";
        break;
    }

    case IMOSTDef::I_WMC_3:
    {
        pStr = "@STA^WIFI^1^3";
        break;
    }

    case IMOSTDef::I_WMC_4:
    {
        pStr = "@STA^WIFI^1^4";
        break;
    }

    case IMOSTDef::I_WMC_INVALID:
    default:
    {
        pStr = "@STA^WIFI^0^0";
        break;
    }
    }

    m_pEventSender->sendEventToStatusBar( pStr );
}

void MOSTMessageHandler::setBluelinkCallIcon( bool isEnable )
{
    QString pStr = "";

    if ( isEnable == true )
    {
        pStr = "@STA^CALL^1";
    }
    else
    {
        pStr = "@STA^CALL^0";
    }

    m_pEventSender->sendEventToStatusBar( pStr );
}

void MOSTMessageHandler::setBluelinkMuteIcon( bool isEnable )
{
    QString pStr = "";

    if ( isEnable == true )
    {
        pStr = "@STA^MUTE^1";
    }
    else
    {
        pStr = "@STA^MUTE^0";
    }

    m_pEventSender->sendEventToStatusBar( pStr );
}

void MOSTMessageHandler::connectDBUS_MOST()
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    if( !sessionBus.isConnected() )
    {
        CR_LOG("Cannot connect to the D-Bus session bus for MOST Manager!");
        return;
    }

    sessionBus.connect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_DLNA_PATH, MOST_MANAGER_DLNA_INTERFACE, "TimePosition",
                        this, SLOT( onTimePosition(int,int,int)) );

    sessionBus.connect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_DLNA_PATH, MOST_MANAGER_DLNA_INTERFACE, "CurrentSongInfo",
                        this, SLOT( onCurrentSongInfo(uint,ushort,QStringList)) );

    sessionBus.connect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_DLNA_PATH, MOST_MANAGER_DLNA_INTERFACE, "CurrentVideoInfo",
                        this, SLOT( onCurrentVideoInfo(QString,uint,ushort,QString)) );

    sessionBus.connect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_DLNA_PATH, MOST_MANAGER_DLNA_INTERFACE, "Deckstatus",
                        this, SLOT( onDeckStatus( const uint )) );

    //for StatusBar
    sessionBus.connect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_HMI_PATH, MOST_MANAGER_HMI_INTERFACE, "ShowStatusBar",
                        this, SLOT( onShowStatusBar(uint)) );

    /** 3G/HSDPA(Slot 3) */
    sessionBus.connect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_PHONE_PATH, MOST_MANAGER_PHONE_INTERFACE, "ModemDataStatus",
                        this, SLOT( onModemDataStatus( const uint ) ) );

    /** Wifi(Slot 4) */
    sessionBus.connect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_IBOX_PATH, MOST_MANAGER_IBOX_INTERFACE, "WifiMode",
                        this, SLOT( onWifiMode( const uint, const uint ) ) );

    /** MTS Group - RSSI Antenna */
    sessionBus.connect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_PHONE_PATH, MOST_MANAGER_PHONE_INTERFACE, "ModemLevel",
                        this, SLOT( onModemLevel( const uint ) ) );

    /** MTS Group -  Front Active Calling/ Rear Active Calling/ Private calling/ Mic off */
    sessionBus.connect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_PHONE_PATH, MOST_MANAGER_PHONE_INTERFACE, "UserMode",
                        this, SLOT( onUserMode( const uint, const bool ) ) );

    // for OSD
//    sessionBus.connect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_HMI_PATH, MOST_MANAGER_HMI_INTERFACE, "DisplayOSD",
//                        this, SLOT( OnDisplayOSD( const QDBusVariant& ) ) );
    //sessionBus.connect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_HMI_PATH, MOST_MANAGER_HMI_INTERFACE,
    //                   "DisplayOSD", this, SLOT( OnDisplayOSD( const uint , const uint  , const uint , const uint ,
    //                                                          const uint , const uint , const uint , const QString  ) ) );

    /** Video AdjustScreen */
    sessionBus.connect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_HMI_PATH, MOST_MANAGER_HMI_INTERFACE, "AdjustScreenFromIBOX",
                        this, SLOT( onAdjustScreenFromIBOX( const int, const int, const int, const int ) ) );
    //sessionBus.connect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_IBOX_PATH, MOST_MANAGER_IBOX_INTERFACE, "SlotAdjustScreen", this, SLOT( SlotAdjustScreen( int, int, int, int ) ) );

    /** Video LCDBrightness */
    sessionBus.connect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_HMI_PATH, MOST_MANAGER_HMI_INTERFACE, "LCDBrightnessFromIBOX",
                        this, SLOT( onLCDBrightnessFromIBOX( const int, const int, const int, const int ) ) );

    /** Video IlluminationSetting */
    sessionBus.connect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_HMI_PATH, MOST_MANAGER_HMI_INTERFACE, "IlluminationFromIBOX",
                        this, SLOT( onIlluminationFromIBOX( const int ) ) );

    /** Video VideoRatio */
    sessionBus.connect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_HMI_PATH, MOST_MANAGER_HMI_INTERFACE, "VideoRatioFromIBOX",
                        this, SLOT( onVideoRatioFromIBOX( const int ) ) );

    /** Video DisplayOnOff */
    sessionBus.connect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_HMI_PATH, MOST_MANAGER_HMI_INTERFACE, "DisplayControlFromIBOX",
                        this, SLOT( onDisplayControlFromIBOX( const uint ) ) );

    /** Video Default Set */
    sessionBus.connect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_HMI_PATH, MOST_MANAGER_HMI_INTERFACE, "VideoDefaultRequestIBOX",
                        this, SLOT( onVideoDefaultRequestIBOX( const uint, const ushort, const uint ) ) );
}

void MOSTMessageHandler::disconnectDBUS_MOST()
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    if( !sessionBus.isConnected() )
    {
        CR_LOG("Cannot connect to the D-Bus session bus for MOST Manager!");
        return;
    }

    sessionBus.disconnect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_DLNA_PATH, MOST_MANAGER_DLNA_INTERFACE, "TimePosition",
                           this, SLOT( onTimePosition(int,int,int)) );

    sessionBus.disconnect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_DLNA_PATH, MOST_MANAGER_DLNA_INTERFACE, "CurrentSongInfo",
                           this, SLOT( onCurrentSongInfo(uint,ushort,QStringList)) );

    sessionBus.disconnect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_DLNA_PATH, MOST_MANAGER_DLNA_INTERFACE, "CurrentVideoInfo",
                           this, SLOT( onCurrentVideoInfo(QString,uint,ushort,QString)) );

    sessionBus.disconnect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_DLNA_PATH, MOST_MANAGER_DLNA_INTERFACE, "Deckstatus",
                           this, SLOT( onDeckStatus( const uint )) );

    sessionBus.disconnect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_HMI_PATH, MOST_MANAGER_HMI_INTERFACE, "ShowStatusBar",
                           this, SLOT( onShowStatusBar(uint)) );

    /** Video AdjustScreen */
    sessionBus.disconnect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_HMI_PATH, MOST_MANAGER_HMI_INTERFACE, "AdjustScreenFromIBOX",
                           this, SLOT( SlotAdjustScreen( const int, const int, const int, const int ) ) );

    //sessionBus.connect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_IBOX_PATH, MOST_MANAGER_IBOX_INTERFACE, "SlotAdjustScreen", this, SLOT( SlotAdjustScreen( int, int, int, int ) ) );

    /** Video LCDBrightness */
    sessionBus.disconnect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_HMI_PATH, MOST_MANAGER_HMI_INTERFACE, "LCDBrightnessFromIBOX",
                           this, SLOT( SlotLCDBrightness( const int, const int, const int, const int ) ) );

    /** Video IlluminationSetting */
    sessionBus.disconnect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_HMI_PATH, MOST_MANAGER_HMI_INTERFACE, "IlluminationFromIBOX",
                           this, SLOT( SlotIlluminationSetting( const int ) ) );

    /** Video VideoRatio */
    sessionBus.disconnect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_HMI_PATH, MOST_MANAGER_HMI_INTERFACE, "VideoRatioFromIBOX",
                           this, SLOT( SlotVideoRatio( const int ) ) );

    /** Video DisplayOnOff */
    sessionBus.disconnect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_HMI_PATH, MOST_MANAGER_HMI_INTERFACE, "DisplayControlFromIBOX",
                           this, SLOT( SlotDisplayOnOff( const uint ) ) );

    /** Video Default Set */
    sessionBus.disconnect( MOST_MANAGER_SERVICE_NAME, MOST_MANAGER_HMI_PATH, MOST_MANAGER_HMI_INTERFACE, "VideoDefaultRequestIBOX",
                           this, SLOT( SlotVideoDefaultRequestIBOX( const uint, const ushort, const uint ) ) );
}
