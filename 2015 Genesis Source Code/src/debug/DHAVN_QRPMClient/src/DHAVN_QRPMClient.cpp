/*
 ******************************************************************
 * COPYRIGHT © 2011, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE       : rpmclient
 *
 * PROGRAMMER   : Hariprasad.Boga@teleca.com                        
 *                Kuladeep.Rayalla@teleca.com                       
 *                Jayan.John@teleca.com                             
 * DATE         : 01/08/2011                                        
 * VERSION      : Draft 0.6                                               
 *------------------------------------------------------------------*
 *
 * MODULE SUMMARY :                                                 
 * Resource Policy Manager is used by QT applications in the DH AVN 
 * project for audio management                                     
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS *
 * Created         Hariprasad.Boga@teleca.com       01/08/2011
 * Modified        Hariprasad.Boga@teleca.com       05/10/2011
 * Modified        siddaiah.konduru@teleca.com      13/10/2011
 ******************************************************************/

#include <DHAVN_QRPMClient.h>
#include <DHAVN_QRPMClient_Internal.h>
#include <DHAVN_hda_verbs.h>

static bool MixingOnOff=false;

/*** Public members of QRPMClient class ****/

/*
 *******************************************************************
 * Function: QRPMClient
 *
 * Description : QRPMClient class constructor
 *
 * Parameters :
 *    String *appName   Unique application name
 *
 * Returns : None
 *
 *******************************************************************
 */
/*** QRPMClient Constructor ****/


QRPMClient::QRPMClient(QString *appName) :
    front_display_pipeline( NULL ),
    front_video_src( NULL ),
    front_csp( NULL ),
    front_mqueue( NULL ),
    front_deinterlace( NULL ),
    front_capsfilter( NULL ),
    front_mainvideosink( NULL ),
    front_bus( NULL ),
    rear_display_pipeline( NULL ),
    rear_video_src( NULL ),
    rear_csp( NULL ),
    rear_mqueue( NULL ),
    rear_deinterlace( NULL ),
    rear_capsfilter( NULL ),
    rear_mainvideosink( NULL ),
    rear_bus( NULL ),
	m_pAudio_Chennel_Interface( NULL )
{
    ///Service name of the QRPMClient
    static QString ServiceName = "com.lge.car.QtDBus.QRPMClient";
    static QString ASMInterface = "com.lgcar.QtDBus.AudioStateIF";
    qint64 appPid = QApplication::applicationPid();

    ServiceName.append(".");
    ServiceName.append(appName);
    ServiceName.append("_");
    ServiceName.append(QString("%1").arg(appPid));

    ASMInterface.append(".");
    ASMInterface.append(appName);
    ASMInterface.append("_");
    ASMInterface.append(QString("%1").arg(appPid));

    ////DBus connetion to the Session Bus
    QDBusConnection connection = QDBusConnection::sessionBus();
    if (!connection.isConnected()) {
        WriteLog( QString( "QRPMClient::Cannot connect to the D-BUS session bus." ) );
    }

    if (!connection.registerService(ServiceName)) {
        WriteLog ( "QRPMClient::Cannot register service to the D-BUS session bus." );
    }

    if (!connection.registerObject("/", this, QDBusConnection::ExportAllSlots)) {
        WriteLog( "QRPMClient::Cannot register object to the D-BUS session bus." );
    }

    /***
        This slot is used to notify the application of MICOM initiated volume change i.e. by the Volume Nob or SW Remote Key ****/

    if (!QDBusConnection::sessionBus().connect("com.lge.car.micom", "/", "com.lge.car.micom.audioif", "EventMessage", this, SLOT(DBusMicomSignalHandler(QDBusMessage)))){
        WriteLog( "QRPMClient::Cannot Connect to audioif EventMessage" );
    }

    if(!connection.connect("", "/", ASMInterface, "AudioManagerSignal", this, SLOT(DBusASMSignalHandler(QDBusMessage))))
    {
	    WriteLog( "QRPMClient::Cannot receive the AudioManagerSignal from D-BUS session bus" );
    }
}

/*
 *******************************************************************
 * Function: QRPMClient
 *
 * Description : QRPMClient class Destructor
 *
 * Parameters :
 *    None
 *
 * Returns : None
 *
 *******************************************************************
 */
QRPMClient::~QRPMClient()
{
}

/*
 *******************************************************************
 * Function: RPMClientAcquire
 *
 * Description : Request to acquire the audio resource i.e. before
 * the application does audio playback or rendering sound 
 *
 * Parameters :
 *	TAudioMode eAudioMode	audio mode e.g. Jukebox, FM1, DVD deck etc
 *	TaudioSink eAudioSink	audio sink i.e. either Main or Secondary.
 *
 * Returns : TRPMClientRequestStatus    return status
 *
 *******************************************************************
 */
TRPMClientRequestStatus QRPMClient::RPMClientAcquire(TAudioMode eAudioMode, TAudioSink eAudioSink, bool bMuteMode)
{
    CntlAcquireArg info;
    QString path = "/app/bin/UIStateManagerDaemon";
    QString path1 = QApplication::applicationFilePath();

    info.sourceId = MapToAudioChannelFromAudioMode( eAudioMode );
	info.muteMode = bMuteMode;

    if( QString::compare(path,path1,Qt::CaseInsensitive) == 0 )
    {
        if(eAudioSink == eMain)
       	{
			if ( eAudioMode == eAVMute )
			{
				//return ucResult;	// Mute State
			}
			else if ( eAudioMode != eDeckAudioSPDIF && eAudioMode != eDeckVideoSPDIF )
			{
				int fd = open_hw_device( (char*)"/dev/snd/hwC0D0" );
				if( fd < 0)
				{
					WriteLog( QString( "hwC0D0 open failed %1" ).arg(fd) );
				}
				else
				{
					int res = clear_SPDIF_RXTX_icontrol( fd, RXTXIC_1_LOOPTHRU_B );

//					if ( res >= 0 )
//						qDebug()<<"QRPMClient::SPDIF loop through disabled" << res;
//					else
					if ( res < 0 )
						WriteLog( QString( "SPDIF loop disabling failed" ).arg(res) );

					close_hw_device( fd );
				}
			}
			else
			{
				int fd = open_hw_device( (char*)"/dev/snd/hwC0D0" );
				if( fd < 0)
				{
				    WriteLog( QString( "hwC0D0 open fail : %1" ).arg(fd) );
				}
				else
				{
					int res = set_SPDIF_RXTX_icontrol( fd, RXTXIC_1_LOOPTHRU_B );

//					if ( res >= 0 )
//						qDebug()<<"QRPMClient::SPDIF loop through enabled" << res;
//					else
					if ( res < 0 ) 
					    WriteLog ( QString( "SPDIF loop enabling failed" ).arg(res) );

					close_hw_device( fd );
				}
			}
       		TRPMClientRequestStatus ucResult;
            ucResult = AudioControl( eCntlAcquireMain, &info );
			return ucResult;
       	}
        else
       	{
            return AudioControl(eCntlAcquireSub, &info);
       	}
    }
//    else
//    {
//        qDebug()<<"QRPMClient::Application requested for audio path = "<< QApplication::applicationFilePath() ;
//    }

    return eNoError;

}


/*
 *******************************************************************
 * Function: RPMClientMix
 *
 * Request to mix audio sources i.e. channels can be enabled or 
 * disabled for mixing. Behaviors are defined as to how different 
 * audio channels will be rendered over front and rear speakers 
 *
 * Parameters :
 * TAudioMixChannel eAudioChannel	audio channel e.g. Navi, VR, BT Phone etc.
 * TOnOff eOnOff					channel on or channel off
 *
 * Returns : TRPMClientRequestStatus    return status
 *
 *******************************************************************
 */
TRPMClientRequestStatus QRPMClient::RPMClientMix(TAudioMixChannel eAudioChannel, TOnOff eOnOff)
{
    CntlMixArg info;

    info.audioChannel = eAudioChannel;
    info.onOff = eOnOff;
    return AudioControl(eCntlMix, &info);
}

/*
 *******************************************************************
 * Function: RPMClientSetAudioVolume
 *
 * Command used to set the volume for an audio mode/ source e.g. 
 * Main, AUX, Navi, VR etc.
 *
 * Parameters :
 * TVolumeMode eVolumeMode	audio mode/ source e.g. Main, AUX, Navi, VR etc
 * quint8 nVolumeLevel 			volume level (Range 0-45)
 *
 * Returns : TRPMClientRequestStatus    return status
 *
 *******************************************************************
 */
TRPMClientRequestStatus QRPMClient::RPMClientSetAudioVolume(TVolumeMode eVolumeMode, qint8 nVolumeLevel)
{
    CntlSetVolumeArg info;

    info.sourceId = eVolumeMode; // use SourceId
    info.volume = nVolumeLevel;// use Volume

    return AudioControl(eCntlVolumeSet, &info);
}

/*
 *******************************************************************
 * Function: RPMClientGetAudioVolumeInfo
 *
 * Request to get volume info for various audio modes/ sources 
 * i.e. NAVI, VR, Main, Aux etc.
 *
 * Parameters :
 * None
 *
 * Returns : TRPMClientRequestStatus    return status
 *
 *******************************************************************
 */
TRPMClientRequestStatus QRPMClient::RPMClientGetAudioVolumeInfo()
{
    return AudioControl(eCntlVolumeInfoGet, NULL);
}

/*
 *******************************************************************
 * Function: RPMClientSetAudioSetupValue
 *
 * Command used to set a specific audio setup value e.g. equalizer ,
 * fader, balance, surround, AVC, beep settings.
 *
 * Parameters :
 * TAudioSetup eAudioSetupValue		specific audio setup setting 
 *                                  e.g. equalizer, fader etc. 
 *
 * Returns : TRPMClientRequestStatus    return status
 *
 *******************************************************************
 */
TRPMClientRequestStatus QRPMClient::RPMClientSetAudioSetupValue(TAudioSetup eAudioSetupFunction, qint8 value)
{
    CntlAudioSetupArg info;

    info.type = eAudioSetupFunction;
    info.value = value;
    return AudioControl(eCntlAudioSetupValueSet, &info);
}

/*
 *******************************************************************
 * Function: RPMClientGetAudioSetupInfo
 *
 * Request to get all audio info that has been set i.e. equalizer,
 * fader, balance, surround, AVC, beep settings.
 *
 * Parameters :
 * None
 *
 * Returns : TRPMClientRequestStatus    return status
 *
 *******************************************************************
 */
TRPMClientRequestStatus QRPMClient::RPMClientGetAudioSetupInfo()
{
    return AudioControl(eCntlAudioSetupInfoGet, NULL);
}

/*
 *******************************************************************
 * Function: RPMClientAudioBeepPlay
 *
 * Used to render audio beep.
 *
 * Parameters :
 * TAudioBeepType eAudioBeepType	audio beep type 
 *
 * Returns : TRPMClientRequestStatus    return status
 *
 *******************************************************************
 */
TRPMClientRequestStatus QRPMClient::RPMClientAudioBeepPlay(TAudioBeepType eAudioBeepType)
{
    return AudioControl(eCntlAudioBeep, &eAudioBeepType);
}

/*
 *******************************************************************
 * Function: RPMClientAudioMuteControl
 *
 * Used to mute/ unmute the specified audio sink.
 *
 * Parameters :
 * TAudioSink eAudioSink		Audio sink to be muted/ unmuted
 *                              i.e. Main, Secondary, AMP, Mixer
 * TMuteControl eMuteUnmute	To specify Mute or Unmute
 *
 * Returns : TRPMClientRequestStatus    return status
 *
 *******************************************************************
 */
TRPMClientRequestStatus QRPMClient::RPMClientAudioMuteControl(TAudioSink eAudioSink, TMuteControl eMuteUnmute)
{
    CntlMuteArg info;

    info.audioSink = MapToMuteChannelFromMuteMode( eAudioSink );
    info.muteUnmute = eMuteUnmute;

    return AudioControl(eCntlMute, &info);
}


/*
 *******************************************************************
 * Function: RPMClientGetLastAudioList
 *
 * Requests for a list of the last eight audio modes recently 
 * used on the main channel.
 *
 * Parameters :
 * None
 *
 * Returns : TRPMClientRequestStatus    return status
 *
 *******************************************************************
 */
TRPMClientRequestStatus QRPMClient::RPMClientGetLastAudioList()
{
    return AudioControl(eCntlLastAudio, NULL);
}

// ----------------------------------------------------------------------------
// save the last audio list to micom
// ----------------------------------------------------------------------------
void QRPMClient::SendLastAudioList(SLastAudioList lastAudioList, int count)
{
    QDBusConnection conn = QDBusConnection::sessionBus();
	QDBusMessage message;
	quint8 audioChannel = 0;
	
	message = QDBusMessage::createMethodCall( "com.lge.car.micom", "/","com.lge.car.micom.audioif", "ProcessAudioControlMessage" );

	QList<QVariant>  args;

	args.append(QVariant(c_CntlLastAudioListCommand));

	for (int i=0; i<count && i<NUM_OF_LAST_AUDIO; i++)
	{
	    audioChannel = MapToAudioChannelFromAudioMode(lastAudioList.eAudioModeList[i] );
	    args.append( (quint8)audioChannel );
	}
	message.setArguments(args);

	if( !conn.send( message ) )
        WriteLog( QString( "QRPMClient::SendLastAudioList Failed to send msg to Micom. " ) );
}


/*
 *******************************************************************
 * Function: RPMClientAcquireDisplay
 *
 * Description : Request to acquire the display resource i.e. when
 * switching between Head Unit mode and IBox mode and vice versa
 *
 * Parameters :
 *	TDisplayResourceType eDisplayResource	display resource mode i.e. Front Display, Rear Display, Both
 *	TDisplayResourceUser eDisplayUser	display resource user i.e. either Head Unit or IBox.
 *
 * Returns : TRPMClientRequestStatus    return status
 *
 *******************************************************************
 */
TRPMClientRequestStatus QRPMClient::RPMClientAcquireDisplay(TDisplayResourceType eDisplayResource, TDisplayResourceUser eDisplayUser)
{
    gst_init (NULL,NULL);
    GValue display = { 0, { { 0 } } };
    g_value_init (&display, G_TYPE_STRING);

    if (eFrontDisplay == eDisplayResource) {
        if (front_display_pipeline != NULL) {
            gst_element_set_state(GST_ELEMENT(front_display_pipeline), GST_STATE_NULL);
	    front_video_src = NULL;
	    front_csp = NULL;
	    front_mqueue = NULL;
            front_capsfilter = NULL;
	    front_mainvideosink = NULL;
            front_bus = NULL;
	    front_display_pipeline = NULL;
        }
        if (eIBox == eDisplayUser)
        {
            front_display_pipeline = gst_pipeline_new (NULL);
            front_csp = gst_element_factory_make ("ffmpegcolorspace", NULL);
            front_capsfilter = gst_element_factory_make("capsfilter", NULL);


            GstCaps *front_caps = gst_caps_new_simple("video/x-raw-rgb",
                                      /*"width", G_TYPE_INT, 800,
                                      "height", G_TYPE_INT, 480,*/ "format", GST_TYPE_FOURCC,
                                      GST_MAKE_FOURCC ('R', 'G', 'B', 'A'), NULL);
    
            front_video_src = gst_element_factory_make("v4l2src", NULL);
    
            g_object_set (G_OBJECT(front_video_src), "device", "/dev/video/ibox", (void *)NULL);

            g_object_set (front_capsfilter, "caps", front_caps, (void *)NULL);

            front_mqueue = gst_element_factory_make ("queue", "queue");
            front_mainvideosink = gst_element_factory_make ("ximagesink","imagesink");
 
            g_value_set_string(&display, ":0.0");
            g_object_set_property(G_OBJECT(front_mainvideosink), "display", &display);
  
            front_bus = gst_pipeline_get_bus(GST_PIPELINE(front_display_pipeline));
            gst_bus_add_signal_watch(front_bus);
            
            gst_bin_add(GST_BIN(front_display_pipeline), front_video_src);
            gst_bin_add(GST_BIN(front_display_pipeline), front_capsfilter);
            gst_bin_add(GST_BIN(front_display_pipeline), front_csp);
            gst_bin_add(GST_BIN(front_display_pipeline), front_mqueue);
            gst_bin_add(GST_BIN(front_display_pipeline), front_mainvideosink);

            gst_element_link_many(front_video_src, front_capsfilter, front_csp, front_mqueue, front_mainvideosink, (void *)NULL);
            gst_element_set_state(GST_ELEMENT(front_display_pipeline), GST_STATE_PLAYING);
        }
    } else {
        if (rear_display_pipeline != NULL) {
            gst_element_set_state(GST_ELEMENT(rear_display_pipeline), GST_STATE_NULL);
	    rear_video_src = NULL;
	    rear_csp = NULL;
	    rear_mqueue = NULL;
            rear_capsfilter = NULL;
	    rear_mainvideosink = NULL;
            rear_bus = NULL;
	    rear_display_pipeline = NULL;
        }
        if (eIBox == eDisplayUser)
        {
            rear_display_pipeline = gst_pipeline_new (NULL);
            rear_csp = gst_element_factory_make ("ffmpegcolorspace", NULL);
            rear_capsfilter = gst_element_factory_make("capsfilter", NULL);


            GstCaps *rear_caps = gst_caps_new_simple("video/x-raw-rgb",
                                      /*"width", G_TYPE_INT, 800,
                                      "height", G_TYPE_INT, 480,*/ "format", GST_TYPE_FOURCC,
                                      GST_MAKE_FOURCC ('R', 'G', 'B', 'A'), NULL);
    
            rear_video_src = gst_element_factory_make("v4l2src", NULL);
    
            g_object_set (G_OBJECT(rear_video_src), "device", "/dev/video/ibox", (void *)NULL);

            g_object_set (rear_capsfilter, "caps", rear_caps, (void *)NULL);

            rear_mqueue = gst_element_factory_make ("queue", "queue");
            rear_mainvideosink = gst_element_factory_make ("ximagesink","imagesink");

            g_value_set_string(&display, ":0.1");
            g_object_set_property(G_OBJECT(rear_mainvideosink), "display", &display);

            rear_bus = gst_pipeline_get_bus(GST_PIPELINE(rear_display_pipeline));
            gst_bus_add_signal_watch(rear_bus);
 
            gst_bin_add(GST_BIN(rear_display_pipeline), rear_video_src);
            gst_bin_add(GST_BIN(rear_display_pipeline), rear_capsfilter);
            gst_bin_add(GST_BIN(rear_display_pipeline), rear_csp);
            gst_bin_add(GST_BIN(rear_display_pipeline), rear_mqueue);
            gst_bin_add(GST_BIN(rear_display_pipeline), rear_mainvideosink);

            gst_element_link_many(rear_video_src, rear_capsfilter, rear_csp, rear_mqueue, rear_mainvideosink, (void *)NULL);
            gst_element_set_state(GST_ELEMENT(rear_display_pipeline), GST_STATE_PLAYING);
        }
    }
    return eNoError;
}

/*
 *******************************************************************
 * Function: DBusMicomResponseHandler
 *
 * The RPMClient Qt slot to receive DBUS Responses from MICOM Manager
 *
 * Parameters :
 * QDBusMessage reply
 *
 * Returns : None
 *
 *******************************************************************
 */
void QRPMClient::DBusMicomResponseHandler(QDBusMessage reply)
{
    QList<QVariant> list;

    list = reply.arguments();

    if(list.isEmpty())
    {
	    WriteLog( QString( "No Arguments in the Message" ) );
    }
    quint16 responseCommand = list.first().toInt();

    switch(responseCommand)
    {
        case c_ResAcquireMainCommand:
        {
            //SendAvModeResponseToASM( list, eMain );
            break;
        }
        case c_ResAcquireSubCommand:
        {
			SendAvModeResponseToASM( list, eSecondary );
            break;
        }
        case c_ResMixCommand:
        {
            //SendMixResponseToASM(list);
            break;
        }
		case c_ResMuteCommand:
		{
			SendMuteResponseToASM(list);
			break;
		}
        case c_ResVolumeInfoGetCommand:
        {
            SendVolumeGetResponsetoApp(list);
            break;
        }
        case c_ResAudioSetupInfoGetCommand:
        {
            SendGetAudioSetupResponsetoApp(list);
            break;
        }
        case c_ResLastAudioCommand:
        {
            SendLastAudioResponsetoApp(list);
            break;
        }
        default:
			WriteLog( QString( "Invalid Response Command Received From MMus ") );
			break;
    }
}

/*
 *******************************************************************
 * Function: SendAcquireResponsetoApp
 *
 * Response to the application request made to acquire the audio 
 * resource i.e. before the application does audio playback or 
 * rendering sound
 *
 * Parameters :
 * QList<QVariant> list
 *
 * Returns : None
 *
 *******************************************************************
 */
void QRPMClient::SendAcquireResponsetoApp(QList<QVariant> list, TAudioSink eAudioSink)
{
    TAudioMode eAudioMode;

    quint8 modeValue = list.at(1).toInt();
    eAudioMode = MapToAudioModeFromAudioChannel( modeValue );

    emit RPMClientAcquireResponse(eAudioMode, eAudioSink);
}

/*
 *******************************************************************
 * Function: SendVolumeGetResponsetoApp
 *
 * Response to the request to get the volume info for various 
 * audio modes/ sources i.e. NAVI, VR, Main, Aux etc
 *
 * Parameters :
 * QList<QVariant> list
 *
 * Returns : None
 *
 *******************************************************************
 */
void QRPMClient::SendVolumeGetResponsetoApp(QList<QVariant> list)
{
    SAudioVolumeInfo sAudioVolumeInfo;

    sAudioVolumeInfo.nMain = list.at(1).toUInt();
    sAudioVolumeInfo.nAux = list.at(2).toUInt();
    sAudioVolumeInfo.nNavi = list.at(3).toUInt();
    sAudioVolumeInfo.nVR = list.at(4).toUInt();
    sAudioVolumeInfo.nBTPhone = list.at(5).toUInt();
    sAudioVolumeInfo.nBTBell = list.at(6).toUInt();
    sAudioVolumeInfo.nRDSTA = list.at(7).toUInt();
    sAudioVolumeInfo.nRDSNews = list.at(8).toUInt();
    sAudioVolumeInfo.nRDSAlarm = list.at(9).toUInt();
    sAudioVolumeInfo.nTTS = list.at(10).toUInt();
    sAudioVolumeInfo.nWelcoming = list.at(11).toUInt();
    sAudioVolumeInfo.nTMUCall = list.at(12).toUInt();
    sAudioVolumeInfo.nTMUTBT = list.at(13).toUInt();
    sAudioVolumeInfo.nIBoxPhone = list.at(14).toUInt();
    sAudioVolumeInfo.nIBoxBell = list.at(15).toUInt();

    emit RPMClientGetVolumeResponse(sAudioVolumeInfo);
}

/*
 *******************************************************************
 * Function: SendGetAudioSetupResponsetoApp
 *
 * Response to the request to get all audio info that has been 
 * set i.e. equalizer, fader, balance, surround, AVC, beep settings
 *
 * Parameters :
 * QList<QVariant> list
 *
 * Returns : None
 *
 *******************************************************************
 */
void QRPMClient::SendGetAudioSetupResponsetoApp(QList<QVariant> list)
{
    SAudioSetupInfo aAudioSetupInfo ;

    aAudioSetupInfo.nBass = list.at(1).toUInt();
    aAudioSetupInfo.nMid = list.at(2).toUInt();
    aAudioSetupInfo.nTreble = list.at(3).toUInt();
    aAudioSetupInfo.nBalance = list.at(4).toUInt();
    aAudioSetupInfo.nFader = list.at(5).toUInt();
    aAudioSetupInfo.nVariableEQ = list.at(6).toUInt();
    aAudioSetupInfo.nAVCSDVC = list.at(7).toUInt();
    aAudioSetupInfo.nSurround = list.at(8).toUInt();
    aAudioSetupInfo.nBeep = list.at(9).toUInt();
    aAudioSetupInfo.nVolRatio = list.at(10).toUInt();
    aAudioSetupInfo.nQuantum = list.at(11).toUInt();
    aAudioSetupInfo.nPowerBass = list.at(12).toUInt();
    aAudioSetupInfo.nWelcoming = list.at(13).toUInt();

    emit RPMClientGetAudioSetupResponse(aAudioSetupInfo);
}

/*
 *******************************************************************
 * Function: SendLastAudioResponsetoApp
 *
 * Response to the request (refer Sec 4.1.2.9) for a list of the 
 * last eight audio modes recently used on the main channel
 *
 * Parameters :
 * QList<QVariant> list
 *
 * Returns : None
 *
 *******************************************************************
 */
void QRPMClient::SendLastAudioResponsetoApp(QList<QVariant> list)
{
    SLastAudioList lastAudioList;
    quint8 modeValue[8];

    modeValue[0] = (quint8)list.at(1).toUInt();
    modeValue[1] = (quint8)list.at(2).toUInt();
    modeValue[2] = (quint8)list.at(3).toUInt();
    modeValue[3] = (quint8)list.at(4).toUInt();
    modeValue[4] = (quint8)list.at(5).toUInt();
    modeValue[5] = (quint8)list.at(6).toUInt();
    modeValue[6] = (quint8)list.at(7).toUInt();
    modeValue[7] = (quint8)list.at(8).toUInt();

	lastAudioList.eAudioModeList[0] = MapToAudioModeFromAudioChannel(  modeValue[0] );
	lastAudioList.eAudioModeList[1] = MapToAudioModeFromAudioChannel(  modeValue[1] );
	lastAudioList.eAudioModeList[2] = MapToAudioModeFromAudioChannel(  modeValue[2] );
	lastAudioList.eAudioModeList[3] = MapToAudioModeFromAudioChannel(  modeValue[3] );
	lastAudioList.eAudioModeList[4] = MapToAudioModeFromAudioChannel(  modeValue[4] );
	lastAudioList.eAudioModeList[5] = MapToAudioModeFromAudioChannel(  modeValue[5] );
	lastAudioList.eAudioModeList[6] = MapToAudioModeFromAudioChannel(  modeValue[6] );
	lastAudioList.eAudioModeList[7] = MapToAudioModeFromAudioChannel(  modeValue[7] );

    emit RPMClientGetLastAudioListResponse(lastAudioList);

}

/*
 *******************************************************************
 * Function: SendAvModeResponseToASM
 *
 * Response to the application request made to acquire the audio 
 * resource i.e. before the application does audio playback or 
 * rendering sound
 *
 * Parameters :
 * QList<QVariant> list
 *
 * Returns : None
 *
 *******************************************************************
 */
void QRPMClient::SendAvModeResponseToASM(QList<QVariant> list, TAudioSink eAudioSink)
{
    TAudioMode eAudioMode;

    quint8 modeValue = list.at(1).toInt();
    eAudioMode = MapToAudioModeFromAudioChannel( modeValue );

	emit RPMClientAvModeResponse(eAudioMode, eAudioSink);
    emit RPMClientAcquireResponse(eAudioMode, eAudioSink);
}

/*
 *******************************************************************
 * Function: SendMixResponseToASM
 *
 * Sends response to the application request to mix audio sources 
 * i.e. channels can be enabled or disabled for mixing
 *
 * Parameters :
 * QList<QVariant> list
 *
 * Returns : None
 *
 *******************************************************************
 */
void QRPMClient::SendMixResponseToASM(QList<QVariant> list)
{
	TAudioMixChannel eAudioChannel;
	bool bResult;

	eAudioChannel = (TAudioMixChannel)list.at(1).toInt();
	bResult = (bool)list.at(2).toInt();

	emit RPMClientMixResponse(eAudioChannel, bResult);
}

void QRPMClient::SendMuteResponseToASM(QList<QVariant> list)
{
	EAudioSink eAudioSink;
	bool bResult;

	eAudioSink = (EAudioSink)list.at(1).toInt();
	bResult = (bool)list.at(2).toInt();

	emit RPMClientMuteResponse(eAudioSink, bResult);
}

/*
 *******************************************************************
 * Function: SendVolumeInfoToASM
 *
 * Response to the request to get the volume info for various 
 * audio modes/ sources i.e. NAVI, VR, Main, Aux etc
 *
 * Parameters :
 * QList<QVariant> list
 *
 * Returns : None
 *
 *******************************************************************
 */
void QRPMClient::SendVolumeInfoToASM(QList<QVariant> list)
{
	SAudioVolumeInfo sAudioVolumeInfo;

	sAudioVolumeInfo.nMain = list.at(1).toUInt();
	sAudioVolumeInfo.nAux = list.at(2).toUInt();
	sAudioVolumeInfo.nNavi = list.at(3).toUInt();
	sAudioVolumeInfo.nVR = list.at(4).toUInt();
	sAudioVolumeInfo.nBTPhone = list.at(5).toUInt();
	sAudioVolumeInfo.nBTBell = list.at(6).toUInt();
	sAudioVolumeInfo.nRDSTA = list.at(7).toUInt();
	sAudioVolumeInfo.nRDSNews = list.at(8).toUInt();
	sAudioVolumeInfo.nRDSAlarm = list.at(9).toUInt();
	sAudioVolumeInfo.nTTS = list.at(10).toUInt();
	sAudioVolumeInfo.nWelcoming = list.at(11).toUInt();
	sAudioVolumeInfo.nTMUCall = list.at(12).toUInt();
	sAudioVolumeInfo.nTMUTBT = list.at(13).toUInt();
	sAudioVolumeInfo.nIBoxPhone = list.at(14).toUInt();
	sAudioVolumeInfo.nIBoxBell = list.at(15).toUInt();

	emit RPMClientGetVolumeInfo(sAudioVolumeInfo);
}

/*
 *******************************************************************
 * Function: DBusMicomSignalHandler
 *
 * DBusMicomSignalHandler slot for Micom signals
 *
 * Parameters :
 * QDBusMessage signal
 *
 * Returns : None
 *
 *******************************************************************
 */
void QRPMClient::DBusMicomSignalHandler(QDBusMessage signal)
{
    QList<QVariant> list;
    TVolumeMode volumeMode = eMainVolume;
    quint8 volumeValue = 0;

    list = signal.arguments();

    if(list.isEmpty())
    {
	    WriteLog( QString( "No Arguments in the Message" ) );
    }
    for(int k = 0; k < list.count(); k++)
        qDebug()<<"list["<<k<<"]"<<list[k];
    quint16 responseCommand = list.first().toInt();
    switch(responseCommand)
    {
        case c_ResVolumeStepUpSignal:

            volumeMode = (TVolumeMode) list.at(1).toInt();
            volumeValue = list.at(2).toInt();

            emit MICOMVolumeStepSignal(volumeMode, volumeValue);
            break;
        default:
            WriteLog( QString( "QRPMClient::Invalid Signal from Micom Manager" ) );
			break;
     }
}

/*
 *******************************************************************
 * Function: ErrorResponseHandler
 *
 * Slot for Micom error handling
 *
 * Parameters :
 * None
 *
 * Returns : None
 *
 *******************************************************************
 */
void QRPMClient::ErrorResponseHandlerMainAudio( void )
{
	WriteLog( "QRPMClient::ErrorResponseHandlerMainAudio Fail" );
}

void QRPMClient::ErrorResponseHandlerSubAudio( void )
{
	WriteLog( "QRPMClient::ErrorResponseHandlerSubAudio Fail" );
}

void QRPMClient::ErrorResponseHandlerMixAudio( void )
{
	WriteLog( "QRPMClient::ErrorResponseHandlerMixAudio Fail" );
}

void QRPMClient::ErrorResponseHandlerVolumeInfo( void )
{
	WriteLog( "QRPMClient::ErrorResponseHandlerVolumeInfo Fail" );
}

void QRPMClient::ErrorResponseHandlerSetupInfo( void )
{
	WriteLog( "QRPMClient::ErrorResponseHandlerSetupInfo Fail" );
}

void QRPMClient::ErrorResponseHandlerLastAudio( void )
{
	WriteLog( "QRPMClient::ErrorResponseHandlerLastAudio Fail" );
}

/*
 *******************************************************************
 * Function: DBusASMSignalHandler
 *
 * Slot for Audio System Manager response handling
 *
 * Parameters :
 * None
 *
 * Returns : None
 *
 *******************************************************************
 */
void QRPMClient::DBusASMSignalHandler(QDBusMessage signal)
{
    QList<QVariant> args = signal.arguments();
    int argument = args.at(0).toInt();
    qint8 MixingOnOff = 0;
    
    if(!argument) {
        emit ASMModeChangeSignal();
    }
    else {
        MixingOnOff = (qint8) args.at(1).toInt();
        emit ASMMixingInfoSignal(MixingOnOff);
    }
}

void QRPMClient::InitAudioResponseConnection()
{
    m_pAudio_Chennel_Interface = new QDBusInterface( "com.lge.car.micom", "/","com.lge.car.micom.audioif",QDBusConnection::sessionBus());

    if ( !m_pAudio_Chennel_Interface )
    {
        return;
    }

    if( !QDBusConnection::sessionBus().isConnected() )
    {
        WriteLog( QString( "QRPMClient: Cannot connect to the D-Bus session bus for MICOM Manager!" ) );
        return;
    }
    //m_pAudio_Chennel_Interface = new QDBusInterface( "com.lge.car.micom", "/","com.lge.car.micom.audioif",QDBusConnection::sessionBus());

    if(!(m_pAudio_Chennel_Interface->connection().connect( "com.lge.car.micom", "/SysInfo","com.lge.car.micom.sysinfo", "LgeDBus_AudioMixCHCtrl",
                this, SLOT( MixAudioResponseSlot(QByteArray) ) )))
    {
        WriteLog( QString( "QRPMClient: MixAudioResponseSlot INIT FAIL!" ) );
    }

    if(!(m_pAudio_Chennel_Interface->connection().connect( "com.lge.car.micom", "/SysInfo","com.lge.car.micom.sysinfo", "LgeDBus_AudioMainCHCtrl",
                this, SLOT( MainAudioResponseSlot(QByteArray) ) )))
    {
        WriteLog( QString( "QRPMClient: MainAudioResponseSlot INIT FAIL!" ) );
    }

    if(!(m_pAudio_Chennel_Interface->connection().connect( "com.lge.car.micom", "/SysInfo","com.lge.car.micom.sysinfo", "LgeDBus_AudioMuteCtrl",
                this, SLOT( MuteAudioResponseSlot(QByteArray) ) )))
    {
        WriteLog( QString( "QRPMClient: MuteAudioResponseSlot INIT FAIL!" ) );
    }

//    if(!(m_pAudio_Chennel_Interface->connection().connect( "com.lge.car.micom", "/SysInfo","com.lge.car.micom.sysinfo", "LgeDBus_AudioLastMemory",
//                this, SLOT( LastAudioResponseSlot(QByteArray) ) )))
//    {
//            qDebug() << "QRPMClient: LastAudioResponseSlot INIT FAIL!" ;
//    }
}
void QRPMClient::LastAudioResponseSlot(QByteArray reply)
{
    SLastAudioList lastAudioList;

	lastAudioList.eAudioModeList[0] = MapToAudioModeFromAudioChannel(  (quint8)reply.at(0) );
	lastAudioList.eAudioModeList[1] = MapToAudioModeFromAudioChannel(  (quint8)reply.at(1) );
	lastAudioList.eAudioModeList[2] = MapToAudioModeFromAudioChannel(  (quint8)reply.at(2) );
	lastAudioList.eAudioModeList[3] = MapToAudioModeFromAudioChannel(  (quint8)reply.at(3) );
	lastAudioList.eAudioModeList[4] = MapToAudioModeFromAudioChannel(  (quint8)reply.at(4) );
	lastAudioList.eAudioModeList[5] = MapToAudioModeFromAudioChannel(  (quint8)reply.at(5) );
	lastAudioList.eAudioModeList[6] = MapToAudioModeFromAudioChannel(  (quint8)reply.at(6) );
	lastAudioList.eAudioModeList[7] = MapToAudioModeFromAudioChannel(  (quint8)reply.at(7) );

	emit RPMClientGetLastAudioListResponse(lastAudioList);
}


void QRPMClient::MuteAudioResponseSlot(QByteArray reply)
{
	EAudioSink eAudioSink;
	bool bResult;

	eAudioSink = (EAudioSink)reply.at(1);
	bResult = (bool)reply.at(2);

	emit RPMClientMuteResponse(eAudioSink, bResult);
}

void QRPMClient::MainAudioResponseSlot(QByteArray reply)
{
    TAudioMode eAudioMode;

    eAudioMode = MapToAudioModeFromAudioChannel( (TAudioMixChannel)reply.at(0) );

    emit RPMClientAvModeResponse(eAudioMode, eMain);
    emit RPMClientAcquireResponse(eAudioMode, eMain);
}


void QRPMClient::MixAudioResponseSlot(QByteArray reply)
{
    TAudioMixChannel eAudioChannel;
    bool bResult;

    eAudioChannel = (TAudioMixChannel)reply.at(0);
    bResult = (bool)reply.at(1);

    emit RPMClientMixResponse(eAudioChannel, bResult);
}
/*
 *******************************************************************
 * Function: AudioControl
 *
 * Used by RPM Client APIs to send a DBUS message to MICOM Manager 
 * with the required information
 *
 * Parameters :
 * TControlType nCommand
 * void *Data
 *
 * Returns : TRPMClientRequestStatus    return status
 *
 *******************************************************************
 */
TRPMClientRequestStatus QRPMClient::AudioControl(TControlType nCommand, void *Data)
{
    TRPMClientRequestStatus status = eNoError;
    QDBusMessage msg;
    QDBusMessage reply;
    CntlAcquireArg *acqrptr = NULL;
    CntlMixArg *mixptr = NULL;
    CntlSetVolumeArg *setvolptr = NULL;
    CntlMuteArg *muteptr = NULL;
    CntlAudioSetupArg *audiosetupptr = NULL;
    TAudioSetup audiosetuptype = eAllInitialize;
    quint8 audiosetupvalue = 0;
    TAudioBeepType *beepptr = NULL;
    TAudioBeepType beepvalue = eBeepType0;
    QList<QVariant> list;

    QDBusConnection conn = QDBusConnection::sessionBus();
    if (!conn.isConnected()) {
        WriteLog( QString( "QRPMClient::Failed to connect to the bus\n" ) );
        //exit(-1);
    }

	msg = QDBusMessage::createMethodCall( "com.lge.car.micom", "/","com.lge.car.micom.audioif", "ProcessAudioControlMessage" );

    switch (nCommand)
    {
    case eCntlAcquireMain:
        acqrptr = (CntlAcquireArg *)Data;

        list.append(QVariant(c_CntlAcquireMainCommand));
        list.append(QVariant(acqrptr->sourceId));
		list.append(QVariant(acqrptr->muteMode));

        msg.setArguments(list);
        QDBusConnection::sessionBus().call(msg,QDBus::NoBlock, -1);
        break;

    case eCntlAcquireSub:
        acqrptr = (CntlAcquireArg *)Data;

        list.append(QVariant(c_CntlAcquireSubCommand));
        list.append(QVariant(acqrptr->sourceId));
		list.append(QVariant(acqrptr->muteMode));

        msg.setArguments(list);

        QDBusConnection::sessionBus().callWithCallback(msg, this, SLOT(DBusMicomResponseHandler(QDBusMessage)), SLOT(ErrorResponseHandlerSubAudio()), -1 );
        break;

    case eCntlMix:
        mixptr = (CntlMixArg *)Data;
        qDebug()<< mixptr->audioChannel<<" "<< mixptr->onOff;
        list.append(QVariant(c_CntlMixCommand));
        list.append(QVariant(mixptr->audioChannel));
        list.append(QVariant(mixptr->onOff));

        MixingOnOff = mixptr->onOff;

        msg.setArguments(list);
        QDBusConnection::sessionBus().call(msg,QDBus::NoBlock, -1);
        break;

    case eCntlVolumeSet:
        setvolptr = (CntlSetVolumeArg *)Data;
        qDebug()<< setvolptr->sourceId << setvolptr->volume;

        list.append(QVariant(c_CntlVolumeSetCommand));
        list.append(QVariant(setvolptr->sourceId));
        list.append(QVariant(setvolptr->volume));

        msg.setArguments(list);

        reply =	QDBusConnection::sessionBus().call( msg, QDBus::NoBlock, -1 );
        break;

    case eCntlVolumeInfoGet:
        list.append(QVariant(c_CntlVolumeInfoGetCommand));
        msg.setArguments(list);

		QDBusConnection::sessionBus().callWithCallback( msg,this,SLOT(DBusMicomResponseHandler(QDBusMessage)),SLOT(ErrorResponseHandlerVolumeInfo()),-1 );
        break;

    case eCntlAudioSetupValueSet:
        audiosetupptr = (CntlAudioSetupArg *)Data;
        audiosetuptype = audiosetupptr->type;
        audiosetupvalue = audiosetupptr->value;

        list.append(QVariant(c_CntlAudioSetupValueSetCommand));
        list.append(QVariant(audiosetuptype));
        list.append(QVariant(audiosetupvalue));

        msg.setArguments(list);

        reply = QDBusConnection::sessionBus().call( msg, QDBus::NoBlock, -1);
        break;

    case eCntlAudioSetupInfoGet:
        list.append(QVariant(c_CntlAudioSetupInfoGetCommand));
                msg.setArguments(list);

        QDBusConnection::sessionBus().callWithCallback( msg,this,SLOT(DBusMicomResponseHandler(QDBusMessage)),SLOT(ErrorResponseHandlerSetupInfo()),-1 );
        break;

    case eCntlMute:
        muteptr = (CntlMuteArg *)Data;
        qDebug()<< muteptr->audioSink <<" " << muteptr->muteUnmute;

        list.append(QVariant(c_CntlMuteCommand));
        list.append(QVariant(muteptr->audioSink));
        list.append(QVariant(muteptr->muteUnmute));
        msg.setArguments(list);

		QDBusConnection::sessionBus().call(msg,QDBus::NoBlock, -1);
        break;

    case eCntlAudioBeep:

        beepptr = (TAudioBeepType *)Data;
        beepvalue = *beepptr;

        list.append(QVariant(c_CntlAudioBeepCommand));
        list.append(QVariant(beepvalue));
        msg.setArguments(list);

        reply =	QDBusConnection::sessionBus().call( msg, QDBus::NoBlock, -1 );
        break;

    case eCntlLastAudio:
        list.append(QVariant(c_CntlLastAudioCommand));
		msg.setArguments(list);

        QDBusConnection::sessionBus().callWithCallback( msg,this,SLOT(DBusMicomResponseHandler(QDBusMessage)),SLOT(ErrorResponseHandlerLastAudio()),-1 );
        break;

    default:
        WriteLog( QString( "QRPMClient::Invalid Control Command From Application" ) );
        //exit(-1);
		break;
	}

	return status;
}

quint8
QRPMClient::MapToAudioChannelFromAudioMode( EAudioMode ucMicomMode )
{
	int index  = 0;

	while ( index < (int) (sizeof(AUDIO_MODE_TO_AUDIO_CHANNEL_MAP)/sizeof(AUDIO_MODE_TO_AUDIO_CHANNEL)))
	{
		if ( AUDIO_MODE_TO_AUDIO_CHANNEL_MAP[index].ucMicomMode == ucMicomMode )
		{
			return AUDIO_MODE_TO_AUDIO_CHANNEL_MAP[index].i8AudioChannel;
		}
		++index;
	}
	return 0xFF;
}

EAudioMode
QRPMClient::MapToAudioModeFromAudioChannel( quint8 i8AudioChannel )
{
	int index  = 0;

	while ( index < (int) (sizeof(AUDIO_MODE_TO_AUDIO_CHANNEL_MAP)/sizeof(AUDIO_MODE_TO_AUDIO_CHANNEL)))
	{
		if ( AUDIO_MODE_TO_AUDIO_CHANNEL_MAP[index].i8AudioChannel == i8AudioChannel )
		{
			return AUDIO_MODE_TO_AUDIO_CHANNEL_MAP[index].ucMicomMode;
		}
		++index;
	}
	return eAudioNone;
}

quint8
QRPMClient::MapToMuteChannelFromMuteMode( EAudioSink ucMuteMode )
{
	int index  = 0;

	while ( index < (int) (sizeof(MUTE_MODE_TO_MUTE_CHANNEL_MAP)/sizeof(MUTE_MODE_TO_MUTE_CHANNEL)))
	{
		if ( MUTE_MODE_TO_MUTE_CHANNEL_MAP[index].ucMuteMode == ucMuteMode )
		{
			return MUTE_MODE_TO_MUTE_CHANNEL_MAP[index].i8MuteChannel;
		}
		++index;
	}
	return 0xFF;
}

void 
QRPMClient::WriteLog( QString strLog )
{
    QDateTime dateTime = QDateTime::currentDateTime();
    dateTime.toString("[hh:mm:ss.zzz]");
    
	QFile fdLogFile( "/app/data/uish/abnormalTermination.log" );
	
	fdLogFile.open( QIODevice::Text | QIODevice::Append | QIODevice::WriteOnly );

	if( fdLogFile.size()>=1024*1024 )
		fdLogFile.remove();
  
    QTextStream fdStream( &fdLogFile );
	fdStream << QString( "[%1] " ).arg(dateTime.toString("[hh:mm:ss.zzz]") ) << strLog;

	fdLogFile.close();
}
