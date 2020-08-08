#include "util/video/DHAVN_AppMediaPlayer_MainUtils.h"

AppMediaPlayer_MainUtils* AppMediaPlayer_MainUtils::m_pInstance = 0;
int AppMediaPlayer_MainUtils::m_count = 0;

AppMediaPlayer_MainUtils::AppMediaPlayer_MainUtils()
{

}

AppMediaPlayer_MainUtils::~AppMediaPlayer_MainUtils()
{

}

AppMediaPlayer_MainUtils*
AppMediaPlayer_MainUtils::GetInstance()
{
    if ( m_pInstance == 0 )
    {
        m_pInstance = new AppMediaPlayer_MainUtils();
    }
    ++m_count;
    return m_pInstance;
}

void
AppMediaPlayer_MainUtils::FreeInstance()
{
    --m_count;
    if (m_count == 0)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }

}

void AppMediaPlayer_MainUtils::copyUChars( QChar *buffer,
                                           int capacity,
                                           QString source )
{
    int charsToCopy = qMin( capacity - 1, source.length() );
    for ( int i=0; i < charsToCopy; i++ )
    {
        buffer[i] = source.at(i);
    }
    buffer[charsToCopy] = QChar::Null;
}

QString AppMediaPlayer_MainUtils::convertDuration( int duration )
{
// { modified by lssanh 2012.10.12 for 3PDMS 142919
/*
   int hour = floor( duration / ( 60 * 60 ) );
   int minute = floor( duration / 60 - hour * 60 );
   int second = floor( duration - minute * 60 - hour * 3600 );
   return QString::number( hour ) + ":" + ( ( minute < 10 ) ? "0" : "" ) + QString::number( minute ) + ":" + ( ( second < 10 ) ? "0" : "" ) + QString::number( second );
*/
    QTime time;

    time = time.addMSecs(duration);
    return time.toString("hh:mm:ss");
// { modified by lssanh 2012.10.12 for 3PDMS 142919   
}
// {added by yungi 2012.11.14 for ISV 62667,ISV 62596
QString AppMediaPlayer_MainUtils::convertDurationAudio( int duration )
{
    QTime time;

    time = time.addMSecs(duration);
    return time.toString("mm:ss");
}
// }added by yungi 2012.11.14 for ISV 62667,ISV 62596


// { added by Sergey for SwapDC
bool AppMediaPlayer_MainUtils::isCommonEvent(Event &aEvent)
{
    bool result = false;

    switch(aEvent.GetEventId())
    {
        case	EVT_USB_REMOVED	:
        case	EVT_APP_LAUNCH	:
        case	EVT_LANGUAGE_CHANGED	:
        case	EVT_DVD_MEDIA_INSERT	:
        case	EVT_RESOURCE_GRANTED	:
        case	EVT_RESOURCE_DENIED	:
        case	EVT_RESOURCE_RELEASED	:
        case	EVT_RESOURCE_RELEASED_BY_MANAGER	:
        case	EVT_RESOURCE_LOST	:
        case	EVT_RESOURCE_ERROR	:
        case	EVT_RESOURCE_AVAILABLE	:
        case	EVT_KEY_HU_BACK	:
        case	EVT_KEY_HU_MENU	:
        case	EVT_KEY_HU_TUNE	:
        case	EVT_KEY_HU_SEEK_NEXT	:
        case	EVT_KEY_HU_SEEK_PREV	:
        case	EVT_KEY_CCP_JOG_DIAL	:
        case	EVT_KEY_RRC_BACK	:
        case	EVT_KEY_RRC_MENU	:
        case	EVT_KEY_RRC_VOL_UP	:
        case	EVT_KEY_RRC_VOL_DOWN	:
        case	EVT_KEY_CCP_SEEK_NEXT	: // added by AVP 2014.02.18 Add CCP_SEEK
        case	EVT_KEY_CCP_SEEK_PREV	: // added by AVP 2014.02.18 Add CCP_SEEK
        case	EVT_KEY_RRC_SEEK_NEXT	:
        case	EVT_KEY_RRC_SEEK_PREV	:
        case	EVT_KEY_RRC_MUTE	:
        case	EVT_KEY_RRC_JOG_DIAL	:
        case	EVT_KEY_SWRC_PTT	:
        case	EVT_KEY_SWRC_CALL_SEND	:
        case	EVT_KEY_SWRC_CALL_END	:
        case	EVT_KEY_SWRC_MUTE	:
        case	EVT_KEY_SWRC_VOL_UP	:
        case	EVT_KEY_SWRC_VOL_DOWN	:
        case	EVT_KEY_SWRC_SEEK_NEXT	:
        case	EVT_KEY_SWRC_SEEK_PREV	:
        case	EVT_KEY_SOFT_MENU	:
        case	EVT_KEY_HU_EJECT	:
        case	EVT_KEY_HU_TUNE_LEFT	:
        case	EVT_KEY_HU_TUNE_RIGHT	:
        case	EVT_KEY_HU_TUNE_PRESSED	:
        case	EVT_KEY_CCP_BACK	:
        case	EVT_KEY_CCP_MENU	:
        case	EVT_KEY_CCP_JOG_DIAL_TOP_LEFT	:
        case	EVT_KEY_CCP_JOG_DIAL_LEFT	:
        case	EVT_KEY_CCP_JOG_DIAL_DOWN_LEFT	:
        case	EVT_KEY_CCP_JOG_DIAL_UP	:
        case	EVT_KEY_CCP_JOG_DIAL_CENTER	:
        case	EVT_KEY_CCP_JOG_DIAL_DOWN	:
        case	EVT_KEY_CCP_JOG_DIAL_TOP_RIGHT	:
        case	EVT_KEY_CCP_JOG_DIAL_RIGHT	:
        case	EVT_KEY_CCP_JOG_DIAL_DOWN_RIGHT	:
        case	EVT_KEY_RRC_JOG_DIAL_TOP_LEFT	:
        case	EVT_KEY_RRC_JOG_DIAL_LEFT	:
        case	EVT_KEY_RRC_JOG_DIAL_DOWN_LEFT	:
        case	EVT_KEY_RRC_JOG_DIAL_UP	:
        case	EVT_KEY_RRC_JOG_DIAL_CENTER	:
        case	EVT_KEY_RRC_JOG_DIAL_DOWN	:
        case	EVT_KEY_RRC_JOG_DIAL_TOP_RIGHT	:
        case	EVT_KEY_RRC_JOG_DIAL_RIGHT	:
        case	EVT_KEY_RRC_JOG_DIAL_DOWN_RIGHT	:
        case	EVT_KEY_HU_JOG_DIAL_ENCODER	:
        case	EVT_KEY_CCP_JOG_DIAL_ENCODER	:
        case	EVT_KEY_RRC_JOG_DIAL_ENCODER	:
        case	EVT_DISC_STATUS	:
        case	EVT_NAVI_GPS_TIME_SET	:
        case	EVT_CLOCK_TIME_SET	:
        case	EVT_BT_INCOMING_CALL	:
        case	EVT_BT_CALL_ACCEPTED	:
        case	EVT_BT_CALL_REJECTED	:
        case	EVT_CAMERA_CLOSE	:
        case	EVT_KEY_SWRC_LONG_CALL_SEND	:
        case	EVT_START_AUDIO_PLAY_IN_BG	:
        case	EVT_BT_CALL_STATUS	:
        case	EVT_BT_CALL_ENDED	:
        case	EVT_BT_MUSIC_PLAY	:
        case	EVT_BT_MUSIC_STOP	:
        case	EVT_BT_MUSIC_PAUSE	:
        case	EVT_BT_MUSIC_PREV_TRACK	:
        case	EVT_BT_MUSIC_NEXT_TRACK	:
        case	EVT_BT_MUSIC_REPEAT_ALL_ON	:
        case	EVT_BT_MUSIC_REPEAT_ALL_OFF	:
        case	EVT_BT_MUSIC_REPEAT_ONE_ON	:
        case	EVT_BT_MUSIC_REPEAT_ONE_OFF	:
        case	EVT_BT_MUSIC_SHUFFLE_ON	:
        case	EVT_BT_MUSIC_SHUFFLE_OFF	:
        case	EVT_BT_MUSIC_SET_INFO	:
        case	EVT_BT_MUSIC_GET_MODE	:
        case	EVT_BT_MUSIC_CONN_CONFIG	:
        case	EVT_BT_MUSIC_ENABLED	:
        case	EVT_BT_MUSIC_DISABLED	:
        case	EVT_BT_MUSIC_PLAYBACK_STARTED	:
        case	EVT_BT_MUSIC_PLAYBACK_PAUSED	:
        case	EVT_TA_RDS_START	:
        case	EVT_TA_RDS_STOP	:
        case	EVT_DAB_REQUEST_RADIO	:
        case	EVT_RADIO_RESPONSE_DAB	:
        case	EVT_RADIO_END_REQUEST	:
        case	EVT_VIDEOPLAY_STATE	:
        case	EVT_ASPECTRATIO_CHANGE	:
        case	EVT_INFO_DRIVING_MODE	:
        case	EVT_INFO_VEHICLE_HEIGHT	:
        case	EVT_TA_DAB_START	:
        case	EVT_TA_DAB_END	:
        case	EVT_UPDATE_OSD_ON_TA	:
        case	EVT_DAB_CURRENT_PLAY	:
        case	EVT_DAB_STATUS_REQUEST	:
        case	EVT_RADIO_STATUS_RESPONSE	:
        case	EVT_KEY_AGREE_PRESSED	:
        case	EVT_KEY_HOME_MENU_LOCK	:
        case	EVT_KEY_HOME_MENU_UNLOCK	:
        case	EVT_BLUELINK_CALL_ONGOING	:
        case	EVT_BLUELINK_CALL_IDLE	:
        case	EVT_ENGINEERING_SETTING_DRIVINGRESTRICTION	:
        case	EVT_ENGINEERING_SETTING_REARCAMERASIGNAL	:
        case	EVT_KEY_RRC_CH_FW	:
        case	EVT_KEY_RRC_CH_BK	:
        case	EVT_EPG_DAB_START	:
        case	EVT_AV_MODE_CHANGE	:
        case	EVT_REQEUST_MUTE	:
        case	EVT_REQEUST_UNMUTE	:
        case	EVT_STATUSBAR_INFO_CHANGED	:
        case	EVT_SETTINGS_CHANGED	:
        case	EVT_ACC_OFF	:
        case	EVT_POPUP_HISTORY_RESPONSE	:
        case	EVT_REQUEST_VRTTS_VERSION	:
        case	EVT_RESPONSE_VRTTS_VERSION	:
        case	EVT_BT_PAIRING_STATUS	:
        case	EVT_GLOBAL_SEARCH_EVENT	:
        case	EVT_ALT_ON	:
        case	EVT_ALT_OFF	:
        case	EVT_REQ_ENG_MODE_ENTER	:
        case	EVT_TERMINATE_ENG_MODE	:
        case	EVT_DIAGNOSTIC_TEST_START	:
        case	EVT_DIAGNOSTIC_TEST_END	:
        case	EVT_FACTORY_RESET_REQUEST	:
        case	EVT_FACTORY_RESET_RESPONSE	:
        case	EVT_INITIAL_OK	:
        case	EVT_POWER_STATE_LOGIC_ON	:
        case	EVT_POWER_STATE_NORMAL_OFF	:
        case	EVT_MEDIA_ENABLED	:
        case	EVT_JUKEBOX_ENABLED	:
        case	EVT_USB1_ENABLED	:
        case	EVT_USB2_ENABLED	:
        case	EVT_IPOD1_ENABLED	:
        case	EVT_IPOD2_ENABLED	:
        case	EVT_MIRROR_LINK_ENABLED	:
        case	EVT_NAVI_INIT_GPS	:
        case	EVT_NAVI_INITIALIZE_REQUEST	:
        case	EVT_NAVI_INITIALIZE_RESPONSE	:
        case	EVT_NAVI_NOTIFY_CHANGE_ADMIN_DISTRICT	:
        case	EVT_NAVI_USER_FUEL_INFO	:
        case	EVT_NAVI_CPU_RESTRICTED	:
        case	EVT_NAVI_CPU_NORMAL	:
        case	EVT_REQUEST_NAVI_DISPLAY_STATUS	:
        case	EVT_RESPONSE_NAVI_DISPLAY_STATUS	:
        case	EVT_NAVI_SHARED_IMAGE_INITIALIZE	:
        case	EVT_NAVI_SHARED_IMAGE_UPDATE	:
        case	EVT_NAVI_GLOBAL_SEARCH_START	:
        case	EVT_NAVI_GLOBAL_SEARCH_RESPONSE	:
        case	EVT_NAVI_POI	:
        case	EVT_NAVI_SETTTING_UNIT	:
        case	EVT_NAVI_IBOX_POI	:
        case	EVT_NAVI_COMMAND	:
        case	EVT_NAVI_FULL_PATH	:
        case	EVT_NAVI_FULL_PATH_TPEG	:
        case	EVT_NAVI_OPEN_BROWSER_URL	:
        case	EVT_NAVI_SETTTING_PHONE_PRIORITY	:
        case	EVT_XM_REQUEST_NAVI	:
        case	EVT_NAVI_MANUAL_INPUT	:
        case	EVT_BT_HOST_BD_ADDRESS	:
        case	EVT_BT_DEV_BD_ADDRESS	:
        case	EVT_BT_CONNECTION_STATUS	:
        case	EVT_ML_REQUEST_BT_CONNECTION	:
        case	EVT_ML_AUDIO_REQUEST	:
        case	EVT_ML_AUDIO_START	:
        case	EVT_ML_AUDIO_STOP	:
        case	EVT_ML_STATUS_NOTIFY	:
        case	EVT_SUBMODE_CHANGED	:
        case	EVT_SWAP_DC_ENABLED	:
        case	EVT_SWAP_DC_DISABLED	:
        case    EVT_CHANGE_OVERLAYPLANE:
        case	EVT_VR_INITIALIZE	:
        case	EVT_VR_INITIAL_OK	:
        case	EVT_VR_LIST_CREATE	:
        case	EVT_VR_END	:
        case	EVT_VR_COMMAND	:
        case	EVT_VR_MODE_CHANGED	:
        case	EVT_LIST_CREATE_START	:
        case	EVT_LIST_CREATE_END	:
        case	EVT_MIX_PLAY_START	:
        case	EVT_MIX_PLAY_STOP	:
        case	EVT_MIX_PLAY_END	:
        case	EVT_IPOD_MOUNTED	:
        case	EVT_IPOD_REMOVED	:
        case	EVT_AUX_PLUGEDIN	:
        case	EVT_AUX_REMOVED	:
        case	EVT_CDDVD_MOUNTED	:
        case	EVT_CDDVD_REMOVED	:
        case	EVT_IPOD_UDEV_DETECTED	:
        case	EVT_IPOD_UDEV_REMOVED	:
        case	EVT_IPHONE_UDEV_DETECTED	:
        case	EVT_IPHONE_UDEV_REMOVED	:
        case	EVT_CALL_START	:
        case	EVT_CALL_END	:
        case	EVT_REQUEST_UPGRADE	:
        case	EVT_RESPONSE_UPGRADE_CANCEL	:
        case	EVT_SYSTEM_OPTIONS	:
        case	EVT_SYSTEM_OPTIONS_SET	:
        case	EVT_NAVI_ENTRY_POINT_SETTINGS	:
        case	EVT_ENG_REQUEST_CONNECTIVITY	:
        case	EVT_ENG_RESPONSE_CONNECTIVITY	:
        case	EVT_REQUEST_SAVE_UNSAVED_DATA	:
        case	EVT_RESPONSE_SAVE_UNSAVED_DATA	:
        case	EVT_ALL_SLEEP_CANCEL_ACTIVATE	:
        case	EVT_RADIO_FREQUENCY_CHANGE	:
        case	EVT_DRS_ON	:
        case	EVT_DRS_OFF	:
        case	EVT_DWS_ON	:
        case	EVT_DWS_OFF	:
        case	EVT_REAR_GEAR_ENGAGED	:
        case	EVT_REAR_GEAR_REMOVED	:
        case	EVT_RESTORE_LAST_AV_MODE	:
        case	EVT_IGN_CHANGED	:
        case	EVT_ACC_CHANGED	:
        case	EVT_POWER_STATE_LOGIC_OFF	:
        case	EVT_POWER_STATE_NORMAL_ON	:
        case	EVT_REQUEST_ACC_STATUS	:
        case	EVT_REQUEST_IGN_STATUS	:
        case	EVT_RESPONSE_ACC_STATUS	:
        case	EVT_RESPONSE_IGN_STATUS	:
        case	EVT_NAVI_GPS_TIME_GET	:
        case	EVT_XSAMPA_AVAILABLE	:
        case	EVT_FORMAT_JUKEBOX_START	:
        case	EVT_FORMAT_JUKEBOX_END	:
        case	EVT_RSE_LOCK_STATUS	:
        case	EVT_BEEP_PLAY	:
        case	EVT_FM_LAUNCH_STATE	:
        case	EVT_PLAY_VIDEO	:
        case	EVT_SHOW_PHOTO	:
        case	EVT_AUTO_PLAY_IN_BG	:
        case	EVT_INITIALIZATION_REQUEST	:
        case	EVT_REQUEST_AUDIO_PATH	:
        case	EVT_ENABLE_DVDSETTINGS	:
        case	EVT_DISABLE_DVDSETTINGS	:
        case	EVT_REQUEST_DVD_INFO	:
        case	EVT_SHOW_DESKTOP_COMPLETE	:
        case	EVT_USB_PHOTO_STATUS	:
        case	EVT_VIDEO_MOVE_START	:
        case	EVT_VIDEO_MOVE_END	:
        case	EVT_SOUND_STATE_IBOX	:
        case	EVT_CAMERA_OFF_EVENT	:
        case	EVT_PANDORA_STATUS	:
        case	EVT_REQUEST_DIVX_REG_INFO	:
        case	EVT_RESPONSE_DIVX_REG_INFO	:
        case    EVT_NOTIFY_AV_MODE :
        {
            result = true;
            break;
        }
        default:
        {
            result = false;
            break;
        }
    }

    return result;
}
// } added by Sergey for SwapDC
