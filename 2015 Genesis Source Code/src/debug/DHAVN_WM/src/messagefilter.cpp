#include "messagefilter.h"
#include "dhavnlog.h"

namespace UISH_MODE {
	enum {
		NONE = 0,
		AVOFF,	
		FM1,
		FM2,
		AM,
		DISC_AUDIO,
		DISC_VIDEO,
		USB1_AUDIO,
		USB1_VIDEO,
		USB2_AUDIO,
		USB2_VIDEO,	// idx : 10
		
		IPOD1_AUDIO,
		IPOD1_PANDORA,
		AHA_RADIO_IPOD1,
		IPOD2_AUDIO,
		IPOD2_PANDORA,
		AHA_RADIO_IPOD2,
		VCDC,
		JUKEBOX_AUDIO,
		JUKEBOX_VIDEO,
		BT_AUDIO,

		BT_PANDORA,	
		AHA_RADIO_BT,
		AUX1_AUDIO,
		AUX1_VIDEO,
		AUX2_AUDIO,		// deprecated
		AUX2_VIDEO,		// deprecated
		DMB1,
		DMB2,
		CMMB1,			// deprecated
		CMMB2,			// deprecated

		XM1,
		XM2,
		DAB1,
		DAB2,			// deprecated
		DAB3,			// deprecated
		HD_RADIO,
		IBOX_DLNA,		// deprecated
		TTS,
		RDS_TA,
		RDS_NEWS,
		
		RDS_ALARM,
		DAB_FM,			// deprecated
		DAB_AM,			// deprecated
		IBOX_WEBPLAYER,	// deprecated
		IBOX_VR,		// deprecated
		HDR_FM1,
		HDR_FM2,
		HDR_AM,
		MIR_LINK_USB1,	// deprecated
		MIR_LINK_USB2,	// deprecated

		MIR_LINK_BT,	// deprecated
		VIDEO_GUIDE,
		XM3,
		DAB_TA,
                VIDEO_GUIDE_LIST,	// deprecated
                EMERGENCY
	};
};

MessageFilter::MessageFilter(){
	m_currentMode = MODE_UNKNOWN;
	m_radioGroup << "RADIO" << "RDS" << "HDRADIO";
	m_mediaCommand << "MEDIA" << "MEDIA_INFO_MODE";
}

bool MessageFilter::canPass(const QStringList & list){
	MessageFilter::Mode mode = getMode(list);
        //lprintf("MessageFilter Mode : %s", qPrintable(getModeString(mode)));
	if ( mode == m_currentMode || mode == MODE_UNKNOWN || m_currentMode == MODE_UNKNOWN)
		return true;

	lprintf("Cannot pass filter current(%s)", qPrintable(getModeString(m_currentMode)));
	return false;
}

MessageFilter::Mode MessageFilter::getMode(const QStringList & list){
	if ( list.count()){
		if ( isRadioGroup(list.at(0)))
			return MODE_RADIO;
		else if ( isMediaCommand(list.at(0))){
			if ( list.count() > 3){
				return getMediaMode(list.at(2));
			}	
		}
		else if ( list.at(0) == "AHA")
			return MODE_AHA;
		else if ( list.at(0) == "DMB")
			return MODE_DMB;
		else if ( list.at(0) == "SXM_INFO"){
			if ( list.count() > 1 && list.at(1) == "DATA")
				return MODE_UNKNOWN;
			else
				return MODE_SXM;
		}
		else if ( list.at(0) == "SXM")
			return MODE_SXM;
                else if ( list.at(0) == "DAB" || list.at(0) == "DAB_INFO") {
                    if ( list.count() > 1 && list.at(1) == "EPG")
                            return MODE_UNKNOWN;
                    else
                            return MODE_DAB;
                } else if ( list.at(0) == "EMERGENCY") { //dmchoi
                    return MODE_EMERGENCY;
                } else{
			if ( list.count() > 2){
				if ( list.at(0) == "BLUETOOTH" && list.at(1) == "AUDIO")
					return MODE_BT_AUDIO;
			}
		}	
	}

	return MODE_UNKNOWN;
}

MessageFilter::Mode MessageFilter::getMediaMode(const QString & mode){
	if ( mode == "USB")
		return MODE_USB;
	else if ( mode == "JUKEBOX")
		return MODE_JUKEBOX;
	else if ( mode == "AUX")
		return MODE_AUX;
	else if ( mode == "DVD" || mode == "VCD" || mode == "DISC")
		return MODE_DISC;
	else if ( mode == "PANDORA")
		return MODE_PANDORA;
	else if ( mode == "IPOD")
		return MODE_IPOD;
	else if ( mode == "AHA")
		return MODE_AHA;
	else if ( mode == "BLUETOOTH")
		return MODE_BT_AUDIO;

	return MODE_UNKNOWN;
}

bool MessageFilter::isMediaCommand(const QString & command){
	for ( int i = 0; i < m_mediaCommand.count(); ++i)
		if ( command == m_mediaCommand.at(i))
			return true;
	return false;
}

bool MessageFilter::isRadioGroup(const QString & command){
	for (int i = 0; i < m_radioGroup.count(); ++i)
		if ( command == m_radioGroup.at(i))
			return true;

	return false;
}

void MessageFilter::setCurrentMode(int mode){
	switch( mode){
        case UISH_MODE::NONE:
	case UISH_MODE::AUX2_AUDIO:
	case UISH_MODE::AUX2_VIDEO:
	case UISH_MODE::CMMB1:
	case UISH_MODE::CMMB2:
	case UISH_MODE::DAB2:
	case UISH_MODE::DAB3:
	case UISH_MODE::IBOX_DLNA:
//	case UISH_MODE::DAB_FM:
    case UISH_MODE::DAB_AM:
	case UISH_MODE::IBOX_WEBPLAYER:
	case UISH_MODE::IBOX_VR:
	case UISH_MODE::MIR_LINK_USB1:
	case UISH_MODE::MIR_LINK_USB2:
	case UISH_MODE::MIR_LINK_BT:
	case UISH_MODE::VIDEO_GUIDE:
	case UISH_MODE::VIDEO_GUIDE_LIST:
		m_currentMode = MODE_NONE;
		break;

        case UISH_MODE::AVOFF:
                m_currentMode = MODE_AVOFF;
                break;

	case UISH_MODE::FM1:
	case UISH_MODE::FM2:
	case UISH_MODE::AM:
	case UISH_MODE::HD_RADIO:
	case UISH_MODE::HDR_FM1:
	case UISH_MODE::HDR_FM2:
	case UISH_MODE::HDR_AM:
	case UISH_MODE::RDS_TA:
	case UISH_MODE::RDS_NEWS:
	case UISH_MODE::RDS_ALARM:	
		m_currentMode = MODE_RADIO;
		break;

	case UISH_MODE::VCDC:
	case UISH_MODE::DISC_AUDIO:
	case UISH_MODE::DISC_VIDEO:
		m_currentMode = MODE_DISC;
		break;

	case UISH_MODE::USB1_AUDIO:
	case UISH_MODE::USB1_VIDEO:
	case UISH_MODE::USB2_AUDIO:
	case UISH_MODE::USB2_VIDEO:
		m_currentMode = MODE_USB;
		break;
	
	case UISH_MODE::IPOD1_AUDIO:
	case UISH_MODE::IPOD2_AUDIO:
		m_currentMode = MODE_IPOD;
		break;

	case UISH_MODE::IPOD1_PANDORA:
	case UISH_MODE::IPOD2_PANDORA:
	case UISH_MODE::BT_PANDORA:
		m_currentMode = MODE_PANDORA;
		break;

	case UISH_MODE::AHA_RADIO_IPOD1:
	case UISH_MODE::AHA_RADIO_IPOD2:
	case UISH_MODE::AHA_RADIO_BT:
		m_currentMode = MODE_AHA;
		break;

	case UISH_MODE::JUKEBOX_AUDIO:
	case UISH_MODE::JUKEBOX_VIDEO:
		m_currentMode = MODE_JUKEBOX;
		break;

	case UISH_MODE::BT_AUDIO:
		m_currentMode = MODE_BT_AUDIO;
		break;

	case UISH_MODE::AUX1_AUDIO:
	case UISH_MODE::AUX1_VIDEO:
		m_currentMode = MODE_AUX;
		break;

	case UISH_MODE::DMB1:
	case UISH_MODE::DMB2:
		m_currentMode = MODE_DMB;
		break;

	case UISH_MODE::XM1:
	case UISH_MODE::XM2:
	case UISH_MODE::XM3:
		m_currentMode = MODE_SXM;
		break;

	case UISH_MODE::DAB1:
	case UISH_MODE::DAB_TA:
    case UISH_MODE::DAB_FM:
		m_currentMode = MODE_DAB;
		break;
        case UISH_MODE::EMERGENCY: //dmchoi
            m_currentMode = MODE_EMERGENCY;
            break;
	default: m_currentMode = MODE_NONE; break;
	}

	lprintf("Change Current Mode : %s", qPrintable(getModeString(m_currentMode)));
}

QString MessageFilter::getModeString(MessageFilter::Mode mode){
	switch(mode){
	case MODE_UNKNOWN: return "MODE_UNKNOWN";
	case MODE_NONE:	return "MODE_NONE";
        case MODE_AVOFF: return "MODE_AVOFF";
	case MODE_RADIO: return "MODE_RADIO";
	case MODE_USB: return "MODE_USB";
	case MODE_IPOD: return "MODE_IPOD";
	case MODE_AHA: return "MODE_AHA";
	case MODE_PANDORA: return "MODE_PANDORA";
	case MODE_DISC: return "MODE_DISC";
	case MODE_JUKEBOX: return "MODE_JUKEBOX";
	case MODE_BT_AUDIO: return "MODE_BT_AUDIO";
	case MODE_AUX: return "MODE_AUX";
	case MODE_DMB: return "MODE_DMB";
	case MODE_SXM: return "MODE_SXM";
	case MODE_DAB: return "MODE_DAB";
        case MODE_EMERGENCY: return "MODE_EMERGENCY";
	}
	return "MODE_XXX";
}
