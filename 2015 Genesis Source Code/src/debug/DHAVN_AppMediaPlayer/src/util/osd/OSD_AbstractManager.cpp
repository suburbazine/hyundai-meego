#include "OSD_AbstractManager.h"

OSDAbstractManager::OSDAbstractManager(AppMediaPlayer_EngineMain * pEngine)
: QObject(pEngine){ 
	m_engine = pEngine;
	m_currentEntry = OSDEntry::instance();
	m_currentMode = OSDMode::instance();
    m_frontEntry = eAPP_NONE; // added by kihyung 2013.08.01 for static test
    m_rearEntry = eAPP_NONE; // added by kihyung 2013.08.01 for static test
}

APP_ENTRY_POINT_T OSDAbstractManager::currentFrontEntry(){
	return m_currentEntry->frontEntry();
}

APP_ENTRY_POINT_T OSDAbstractManager::currentRearEntry(){
	return m_currentEntry->rearEntry();
}

void OSDAbstractManager::setEntry(bool isFront, APP_ENTRY_POINT_T entry){
	if ( isFront ) {
		MP_LOG << "OnScreenDisplay FRONT : " << OSDEntry::EntryToString(entry) << LOG_ENDL;
		m_currentEntry->setFrontEntry(entry);
	}
	else {
		MP_LOG << "OnScreenDisplay REAR : " << OSDEntry::EntryToString(entry) << LOG_ENDL;
		m_currentEntry->setRearEntry(entry);
	}
}

bool OSDAbstractManager::setMode(MODE_STATE_T mode){
	MODE_STATE_T prev = m_currentMode->mode();
	m_currentMode->setMode(mode);

	MP_LOG << "OnScreenDisplay : Change Current AV from " << OSDMode::ModeToString(prev) << " -> to : " << OSDMode::ModeToString(mode) << LOG_ENDL;
	if ( mode != prev)
        {
            if(m_engine->getPowerOffBTCall() == false)//added by taihyun.ahn 2013.10.28 for its 198808,  0198848 
            {
                m_engine->setPlayOnBTCall();//added by edo.lee 2013.09.15 ITS 0190407
            }else
                m_engine->setPowerOffBTCall(false);
            return true;
        }


	return false;
}

bool OSDAbstractManager::setCurrentMode(APP_ENTRY_POINT_T entry){
	MODE_STATE_T prev = m_currentMode->mode();
	MODE_STATE_T current = mapEntryToMode(entry);
	if ( current == MODE_STATE_MAX ) return false;

	m_currentMode->setMode(current);

	if ( prev != current)
		return true;

	return false;
}

bool OSDAbstractManager::isVideoModeCurrent(){
	return OSDMode::IsVideoMode(m_currentMode->mode());
}

bool OSDAbstractManager::isAudioModeCurrent(){
	return OSDMode::IsAudioMode(m_currentMode->mode());
}

bool OSDAbstractManager::compareDeviceToMode(OSD_DEVICE device){
	MODE_STATE_T mode = mapDeviceToMode(device);

	if ( mode == currentMode()) return true;
	else {
		if ( mode == MODE_STATE_USB1_AUDIO && currentMode() == MODE_STATE_USB2_AUDIO)
			return true;
		else if ( mode == MODE_STATE_USB1_VIDEO && currentMode() == MODE_STATE_USB2_VIDEO)
			return true;
		else if ( mode == MODE_STATE_IPOD1_AUDIO && currentMode() == MODE_STATE_IPOD2_AUDIO)
			return true;
	}
	return false;
}

DISPLAY_T OSDAbstractManager::outputEvent(unsigned int event, APP_ENTRY_POINT_T entry){
    switch(event){
    case OSD_EVENT_NO_PLAYABLE_FILE:
    case OSD_EVENT_UNSUPPORTED_ACTION:
    case OSD_EVENT_IPOD_SENDINGTAGS:
    case OSD_EVENT_IPOD_MEMORYFULL:
    case OSD_EVENT_IPOD_READING:
    case OSD_EVENT_IPOD_SUCCESS:
    case OSD_EVENT_IPOD_FAILED:
    case OSD_EVENT_IPOD_UNSUPPORTED:
    case OSD_EVENT_IPOD_CONNECTION_ERROR:
    case OSD_EVENT_NO_INSERTED_DISC:
    case OSD_EVENT_JUKEBOX_READING:
    case OSD_EVENT_BT_UNSUPPORTED:
    case OSD_EVENT_IPOD_UNSUPPORTED_ACTION:
        return DISPLAY_FRONT_REAR;
    case OSD_EVENT_AUX_EXTERNAL_DEVICE_CONNECTED: // moved by cychoi 2013.12.18 for ITS 216289
    case OSD_EVENT_AUX_CONNECTED:
    {
        if ( OSDEntry::IsAuxEntry(currentFrontEntry()) && OSDEntry::IsAuxEntry(currentRearEntry()))
            return DISPLAY_NONE;
        else if ( OSDEntry::IsAuxEntry(currentFrontEntry()))
            return DISPLAY_REAR;
        else if ( OSDEntry::IsAuxEntry(currentRearEntry()))
            return DISPLAY_FRONT;

        return DISPLAY_FRONT_REAR;
    }
    case OSD_EVENT_COPY_COMPLETED:
    case OSD_EVENT_DELETE_COMPLETED:
    case OSD_EVENT_MOVE_COMPLETED:
    {
        if ( OSDEntry::IsImageAppEntry(entry)){
            if ( OSDEntry::IsImageAppEntry(currentFrontEntry()) && OSDEntry::IsImageAppEntry(currentRearEntry())) return DISPLAY_NONE;
            else if ( OSDEntry::IsImageAppEntry(currentFrontEntry()))
                return DISPLAY_REAR;
            else if ( OSDEntry::IsImageAppEntry(currentRearEntry()))
                return DISPLAY_FRONT;
        }
        return DISPLAY_FRONT_REAR;
    }
    case OSD_EVENT_ADISC_EJECTED:
    {
        return DISPLAY_FRONT_REAR;
    }
    case OSD_EVENT_AUX_DISCONNECTED:
    {
        if ( OSDMode::IsAuxMode(currentMode())){
            if ( OSDEntry::IsAuxEntry(currentFrontEntry()) && OSDEntry::IsAuxEntry(currentRearEntry()))
                return DISPLAY_FRONT_REAR;
            else if ( OSDEntry::IsAuxEntry(currentFrontEntry()))
                return DISPLAY_FRONT;
            else if ( OSDEntry::IsAuxEntry(currentRearEntry()))
                return DISPLAY_REAR;

            return DISPLAY_NONE;
        }
        else{
            return DISPLAY_FRONT_REAR;
        }
    }
    case OSD_EVENT_IPOD_EMPTY_TITLE:
    case OSD_EVENT_IPOD_UNSUPPORTED_ACTION_FG: // added for ITS 219160
    {
        if( OSDMode::IsIpodMode(currentMode()))
        {
            if( OSDEntry::IsIpodEntry(currentFrontEntry()) && OSDEntry::IsIpodEntry(currentRearEntry())) return DISPLAY_NONE;
            else if( OSDEntry::IsIpodEntry(currentFrontEntry()))
                return DISPLAY_REAR;
            else if( OSDEntry::IsIpodEntry(currentRearEntry()))
                return DISPLAY_FRONT;
        }
        else
        {
            return DISPLAY_FRONT_REAR;
        }
    }
    default: break;
    }
    return DISPLAY_FRONT_REAR;
}

MODE_STATE_T OSDAbstractManager::mapDeviceToMode(OSD_DEVICE device){
	switch(device){
	case JUKEBOX_MUSIC:   return MODE_STATE_JUKEBOX_AUDIO;
	case JUKEBOX_VIDEO:   return MODE_STATE_JUKEBOX_VIDEO;
	case USB_MUSIC:       return MODE_STATE_USB1_AUDIO;
	case USB_VIDEO:       return MODE_STATE_USB1_VIDEO;
    // { modified by cychoi 2013.11.12 for OSD Title Translation
	case DISC_CDDA_MUSIC: return MODE_STATE_DISC_AUDIO;
	case DISC_MP3_MUSIC:  return MODE_STATE_DISC_AUDIO;
	case DISC_DVD_VIDEO:  return MODE_STATE_DISC_VIDEO;
	case DISC_VCD_VIDEO:  return MODE_STATE_DISC_VIDEO;
    // } modified by cychoi 2013.11.12
	case IPOD_MUSIC:      return MODE_STATE_IPOD1_AUDIO;
	case BLUETOOTH_MUSIC: return MODE_STATE_BT_AUDIO;
	case AUX_MUSIC:       return MODE_STATE_AUX1_AUDIO;
	case AUX_VIDEO:       return MODE_STATE_AUX1_VIDEO;
	default: break;
	}
	return MODE_STATE_MAX;
}

MODE_STATE_T OSDAbstractManager::mapEntryToMode(APP_ENTRY_POINT_T entry){
	switch(entry){
	case eAPP_NONE: return MODE_STATE_NONE;
	case eAPP_AVP_AUDIO_MODE_JUKEBOX: return MODE_STATE_JUKEBOX_AUDIO;
	case eAPP_AVP_VIDEO_MODE_JUKEBOX: return MODE_STATE_JUKEBOX_VIDEO;
	case eAPP_AVP_AUDIO_MODE_AUX: return MODE_STATE_AUX1_AUDIO;
	case eAPP_AVP_VIDEO_MODE_AUX: return MODE_STATE_AUX1_VIDEO;
	case eAPP_AVP_AUDIO_MODE_DISC: return MODE_STATE_DISC_AUDIO;
	case eAPP_AVP_VIDEO_MODE_DISC: return MODE_STATE_DISC_VIDEO;
	case eAPP_AVP_AUDIO_MODE_IPOD1: return MODE_STATE_IPOD1_AUDIO;
	case eAPP_AVP_AUDIO_MODE_IPOD2: return MODE_STATE_IPOD2_AUDIO;
	case eAPP_AVP_AUDIO_MODE_USB1: return MODE_STATE_USB1_AUDIO;
	case eAPP_AVP_AUDIO_MODE_USB2: return MODE_STATE_USB2_AUDIO;
	case eAPP_AVP_VIDEO_MODE_USB1: return MODE_STATE_USB1_VIDEO;
	case eAPP_AVP_VIDEO_MODE_USB2: return MODE_STATE_USB2_VIDEO;
	case eAPP_AVP_MODE_BLUETOOTH:
	case eAPP_AVP_AUDIO_MODE_BT: return MODE_STATE_BT_AUDIO;
	case eAPP_AVP_VIDEO_MODE_DISC_DRS: return MODE_STATE_DISC_VIDEO;
	case eAPP_AVP_VIDEO_MODE_JUKEBOX_DRS: return MODE_STATE_JUKEBOX_VIDEO;
	case eAPP_AVP_VIDEO_MODE_USB1_DRS: return MODE_STATE_USB1_VIDEO;
	case eAPP_AVP_VIDEO_MODE_USB2_DRS: return MODE_STATE_USB2_VIDEO;	
	case eAPP_AVP_VIDEO_MODE_AUX_DRS: return MODE_STATE_AUX1_VIDEO;
	case eAPP_AVP_PHOTO_MODE_JUKEBOX:
	case eAPP_AVP_PHOTO_MODE_JUKEBOX_DRS: 
	case eAPP_AVP_PHOTO_MODE_USB1: 
	case eAPP_AVP_PHOTO_MODE_USB2: 
	case eAPP_AVP_PHOTO_MODE_USB1_DRS: 
	case eAPP_APP_PHOTO_MODE_USB2_DRS: 
		return MODE_STATE_MAX;
	default: break;
	}
	return MODE_STATE_MAX;
} 

APP_ENTRY_POINT_T OSDAbstractManager::mapToEntryState(MODE_STATE_T mode){
    APP_ENTRY_POINT_T result = eAPP_NONE;

    switch(mode)
    {
     	case MODE_STATE_JUKEBOX_AUDIO:
            return eAPP_AVP_AUDIO_MODE_JUKEBOX;
        case MODE_STATE_USB1_AUDIO:
            return eAPP_AVP_AUDIO_MODE_USB1;
        case MODE_STATE_USB2_AUDIO:
            return eAPP_AVP_AUDIO_MODE_USB2;
        case MODE_STATE_DISC_AUDIO:
            return eAPP_AVP_AUDIO_MODE_DISC;
        case MODE_STATE_IPOD1_AUDIO:
            return eAPP_AVP_AUDIO_MODE_IPOD1;
		case MODE_STATE_IPOD2_AUDIO:
            return eAPP_AVP_AUDIO_MODE_IPOD2;
		case MODE_STATE_AUX1_AUDIO:
            return eAPP_AVP_AUDIO_MODE_AUX;
		case MODE_STATE_AUX2_AUDIO:
            return eAPP_AVP_AUDIO_MODE_AUX;
		case MODE_STATE_BT_AUDIO:
            return eAPP_AVP_MODE_BLUETOOTH;
        case MODE_STATE_JUKEBOX_VIDEO:
            return eAPP_AVP_VIDEO_MODE_JUKEBOX;
        case MODE_STATE_USB1_VIDEO:
            return eAPP_AVP_VIDEO_MODE_USB1;
        case MODE_STATE_USB2_VIDEO:
            return eAPP_AVP_VIDEO_MODE_USB2;
        case MODE_STATE_DISC_VIDEO:
            return eAPP_AVP_VIDEO_MODE_DISC;
        case MODE_STATE_AUX1_VIDEO:
            return eAPP_AVP_VIDEO_MODE_AUX;
		case MODE_STATE_AUX2_VIDEO:
            return eAPP_AVP_VIDEO_MODE_AUX;
        default: break;
    }

    return result;
}

QString OSDAbstractManager::displayToString(DISPLAY_T display){
	if ( display == DISPLAY_FRONT) return "DISPLAY_FRONT";
	else if ( display == DISPLAY_REAR) return "DISPLAY_REAR";
	else if ( display == DISPLAY_FRONT_REAR) return "DISPLAY_FRONT_REAR";
	
	return QString("DISPLAY(") + QString::number((int)display) + ")";
}


