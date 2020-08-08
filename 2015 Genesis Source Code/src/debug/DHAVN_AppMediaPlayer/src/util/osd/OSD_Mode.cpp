#include "OSD_Mode.h"

OSDMode * OSDMode::pOSDMode = NULL;

OSDMode * OSDMode::instance(){
	if ( pOSDMode == NULL)
		pOSDMode = new OSDMode;

	return pOSDMode;
}

OSDMode::OSDMode(){
	m_mode = MODE_STATE_NONE;
}

void OSDMode::setMode(MODE_STATE_T mode){
	m_mode = mode;
}

bool OSDMode::IsVideoMode(MODE_STATE_T mode){
	switch(mode){
    case MODE_STATE_JUKEBOX_VIDEO:
	case MODE_STATE_USB1_VIDEO:
	case MODE_STATE_USB2_VIDEO:
	case MODE_STATE_DISC_VIDEO:
	case MODE_STATE_AUX1_VIDEO:
	case MODE_STATE_AUX2_VIDEO:
		return true;
	default: break;
	}
	return false;
}

bool OSDMode::IsAudioMode(MODE_STATE_T mode){
	switch(mode){
    case MODE_STATE_DISC_AUDIO: 
    case MODE_STATE_USB1_AUDIO: 
    case MODE_STATE_USB2_AUDIO: 
    case MODE_STATE_IPOD1_AUDIO: 
	case MODE_STATE_IPOD2_AUDIO:
    case MODE_STATE_JUKEBOX_AUDIO: 
    case MODE_STATE_BT_AUDIO : 
    case MODE_STATE_AUX1_AUDIO: 
    case MODE_STATE_AUX2_AUDIO: 
		return true;
 	default: break;
	}
	return false; 
}

bool OSDMode::IsAuxMode(MODE_STATE_T mode){
	switch(mode){
	case MODE_STATE_AUX1_AUDIO:
	case MODE_STATE_AUX2_AUDIO:
	case MODE_STATE_AUX1_VIDEO:
	case MODE_STATE_AUX2_VIDEO:
		return true;
	default: break;
	}
	return false;
}

bool OSDMode::IsIpodMode(MODE_STATE_T mode){
	switch(mode){
	case MODE_STATE_IPOD1_AUDIO:
	case MODE_STATE_IPOD2_AUDIO:
		return true;
	default: break;
	}
	return false;
}

bool OSDMode::IsDiscMode(MODE_STATE_T mode){
	switch(mode){
	case MODE_STATE_DISC_AUDIO:
	case MODE_STATE_DISC_VIDEO:
		return true;
	default: break;
	}
	return false;
}

bool OSDMode::IsUsbMode(MODE_STATE_T mode){
	switch(mode){
	case MODE_STATE_USB1_AUDIO:
	case MODE_STATE_USB2_AUDIO:
	case MODE_STATE_USB1_VIDEO:
	case MODE_STATE_USB2_VIDEO:
		return true;
	default: break;
	}
	return false;
}


QString OSDMode::ModeToString(MODE_STATE_T mode){
	switch(mode){
    case MODE_STATE_AVOFF: return "MODE_STATE_AVOFF";
    case MODE_STATE_FM1: return "MODE_STATE_FM1";
    case MODE_STATE_FM2: return "MODE_STATE_FM2";
    case MODE_STATE_AM: return "MODE_STATE_AM";
    case MODE_STATE_DISC_AUDIO: return "MODE_STATE_DISC_AUDIO";
    case MODE_STATE_DISC_VIDEO: return "MODE_STATE_DISC_VIDEO";
    case MODE_STATE_USB1_AUDIO: return "MODE_STATE_USB1_AUDIO";
    case MODE_STATE_USB1_VIDEO: return "MODE_STATE_USB1_VIDEO";
    case MODE_STATE_USB2_AUDIO: return "MODE_STATE_USB2_AUDIO";
    case MODE_STATE_USB2_VIDEO: return "MODE_STATE_USB2_VIDEO";
    case MODE_STATE_IPOD1_AUDIO: return "MODE_STATE_IPOD1_AUDIO";
    case MODE_STATE_IPOD1_PANDORA: return "MODE_STATE_IPOD1_PANDORA";
    case MODE_STATE_AHA_RADIO_IPOD1: return "MODE_STATE_AHA_RADIO_IPOD1";
    case MODE_STATE_IPOD2_AUDIO: return "MODE_STATE_IPOD2_AUDIO";
    case MODE_STATE_IPOD2_PANDORA: return "MODE_STATE_IPOD2_PANDORA";
    case MODE_STATE_AHA_RADIO_IPOD2: return "MODE_STATE_AHA_RADIO_IPOD2";
    case MODE_STATE_VCDC: return "MODE_STATE_VCDC";
    case MODE_STATE_JUKEBOX_AUDIO: return "MODE_STATE_JUKEBOX_AUDIO";
    case MODE_STATE_JUKEBOX_VIDEO: return "MODE_STATE_JUKEBOX_VIDEO";
    case MODE_STATE_BT_AUDIO : return "MODE_STATE_BT_AUDIO";
    case MODE_STATE_BT_PANDORA: return "MODE_STATE_BT_PANDORA";
    case MODE_STATE_AHA_RADIO_BT: return "MODE_STATE_AHA_RADIO_BT";
    case MODE_STATE_AUX1_AUDIO: return "MODE_STATE_AUX1_AUDIO";
    case MODE_STATE_AUX1_VIDEO: return "MODE_STATE_AUX1_VIDEO";
    case MODE_STATE_AUX2_AUDIO: return "MODE_STATE_AUX2_AUDIO";
    case MODE_STATE_AUX2_VIDEO: return "MODE_STATE_AUX2_VIDEO";
    case MODE_STATE_DMB1: return "MODE_STATE_DMB1";
    case MODE_STATE_DMB2: return "MODE_STATE_DMB2";
    case MODE_STATE_CMMB1: return "MODE_STATE_CMMB1";
    case MODE_STATE_CMMB2: return "MODE_STATE_CMMB2,";
    case MODE_STATE_XM1: return "MODE_STATE_XM1";
    case MODE_STATE_XM2: return "MODE_STATE_XM2";
    case MODE_STATE_DAB1: return "MODE_STATE_DAB1";
    case MODE_STATE_DAB2: return "MODE_STATE_DAB2";
    case MODE_STATE_DAB3: return "MODE_STATE_DAB3";
    case MODE_STATE_HD_RADIO: return "MODE_STATE_HD_RADIO";
    case MODE_STATE_IBOX_DLNA: return "MODE_STATE_IBOX_DLNA";
    case MODE_STATE_TTS: return "MODE_STATE_TTS";
    case MODE_STATE_RDS_TA: return "MODE_STATE_RDS_TA";
    case MODE_STATE_RDS_NEWS : return "MODE_STATE_RDS_NEWS";
    case MODE_STATE_RDS_ALARM: return "MODE_STATE_RDS_ALARM";
    case MODE_STATE_DAB_FM: return "MODE_STATE_DAB_FM";
    case MODE_STATE_DAB_AM: return "MODE_STATE_DAB_AM";
    case MODE_STATE_IBOX_WEBPLAYER: return "MODE_STATE_IBOX_WEBPLAYER";
    case MODE_STATE_IBOX_VR: return "MODE_STATE_IBOX_VR";
    case MODE_STATE_HDR_FM1: return "MODE_STATE_HDR_FM1";
    case MODE_STATE_HDR_FM2: return "MODE_STATE_HDR_FM2";
    case MODE_STATE_HDR_AM: return "MODE_STATE_HDR_AM";
	case MODE_STATE_MIR_LINK_USB1: return "MODE_STATE_MIR_LINK_USB1";
    case MODE_STATE_MIR_LINK_USB2: return "MODE_STATE_MIR_LINK_USB2";
	case MODE_STATE_MIR_LINK_BT: return "MODE_STATE_MIR_LINK_BT";
	case MODE_STATE_VIDEO_GUIDE: return "MODE_STATE_VIDEO_GUIDE";
    case MODE_STATE_XM3: return "MODE_STATE_XM3";
    case MODE_STATE_DAB_TA: return "MODE_STATE_DAB_TA";
    case MODE_STATE_VIDEO_GUIDE_LIST: return "MODE_STATE_VIDEO_GUIDE_LIST";
    case MODE_STATE_POWEROFF: return "MODE_STATE_POWEROFF";
	default: break;
	}
	return "MODE_STATE_UNKNOWN";
}
