#include "OSD_Entry.h"

OSDEntry * OSDEntry::pOSDEntry = NULL;

OSDEntry * OSDEntry::instance(){
	if ( pOSDEntry == NULL)
		pOSDEntry = new OSDEntry;

	return pOSDEntry;
}

OSDEntry::OSDEntry(){
	m_frontEntry = eAPP_NONE;
	m_rearEntry = eAPP_NONE;
}

void OSDEntry::setFrontEntry(APP_ENTRY_POINT_T entry){
	m_frontEntry = entry;
}

void OSDEntry::setRearEntry(APP_ENTRY_POINT_T entry){
	m_rearEntry = entry;
}

bool OSDEntry::IsVideoAppEntry(APP_ENTRY_POINT_T entry){
	switch(entry){
	case eAPP_AVP_VIDEO_MODE_JUKEBOX_DRS: 
	case eAPP_AVP_VIDEO_MODE_JUKEBOX: 
	case eAPP_AVP_VIDEO_MODE_AUX_DRS: 
	case eAPP_AVP_VIDEO_MODE_AUX: 
	case eAPP_AVP_VIDEO_MODE_DISC_DRS: 
	case eAPP_AVP_VIDEO_MODE_DISC: 
	case eAPP_AVP_VIDEO_MODE_USB1_DRS: 
	case eAPP_AVP_VIDEO_MODE_USB2_DRS:
	case eAPP_AVP_VIDEO_MODE_USB1: 
	case eAPP_AVP_VIDEO_MODE_USB2: return true;
	default: break;
	}
	return false;
}

// { added by cychoi 2014.02.24 for ITS 226456 always display front OSD on DRS mode
bool OSDEntry::IsVideoAppDRSEntry(APP_ENTRY_POINT_T entry){
    switch(entry){
    case eAPP_AVP_VIDEO_MODE_JUKEBOX_DRS: 
    case eAPP_AVP_VIDEO_MODE_AUX_DRS: 
    case eAPP_AVP_VIDEO_MODE_DISC_DRS: 
    case eAPP_AVP_VIDEO_MODE_USB1_DRS: 
    case eAPP_AVP_VIDEO_MODE_USB2_DRS: return true;
    default: break;
    }
    return false;
}
// } added by cychoi 2014.02.24

bool OSDEntry::IsMusicAppEntry(APP_ENTRY_POINT_T entry)
{
    switch(entry)
    {
    case eAPP_AVP_AUDIO_MODE_JUKEBOX:
    case eAPP_AVP_AUDIO_MODE_AUX:
    case eAPP_AVP_AUDIO_MODE_DISC:
    case eAPP_AVP_AUDIO_MODE_IPOD1:
    case eAPP_AVP_AUDIO_MODE_IPOD2:
    case eAPP_AVP_AUDIO_MODE_USB1:
    case eAPP_AVP_AUDIO_MODE_USB2:
        //{added by junam 2014.01.17 for LIST_ENTRY_POINT
    case eAPP_FILEMANAGER_JUKEBOX_AUDIO:
    case eAPP_FILEMANAGER_USB1_AUDIO:
    case eAPP_FILEMANAGER_USB2_AUDIO:
    case eAPP_FILEMANAGER_IPOD1:
    case eAPP_FILEMANAGER_IPOD2:
        //}added by junam
    case eAPP_FILEMANAGER_DISC_AUDIO: // added by oseong.kwon 2014.02.10 for ITS 223602
    case eAPP_AVP_AUDIO_MODE_BT:
    case eAPP_AVP_MODE_BLUETOOTH: return true;// added for OSD 2014.06.05
    default: break;
    }
    return false;
}

bool OSDEntry::IsAvAppEntry(APP_ENTRY_POINT_T entry){
	return OSDEntry::IsMusicAppEntry(entry) || OSDEntry::IsVideoAppEntry(entry);
}

bool OSDEntry::IsAuxEntry(APP_ENTRY_POINT_T entry){
	switch(entry){
	case eAPP_AVP_AUDIO_MODE_AUX: 
	case eAPP_AVP_VIDEO_MODE_AUX: 
	case eAPP_AVP_VIDEO_MODE_AUX_DRS: 
		return true;
	default: break;
	}
	return false;
}

bool OSDEntry::IsIpodEntry(APP_ENTRY_POINT_T entry){
	switch(entry){
	case eAPP_AVP_AUDIO_MODE_IPOD1: 
	case eAPP_AVP_AUDIO_MODE_IPOD2: 
		return true;
	default: break;
	}
	return false;

}

bool OSDEntry::IsDiscEntry(APP_ENTRY_POINT_T entry){
	switch(entry){
	case eAPP_AVP_AUDIO_MODE_DISC: 
	case eAPP_AVP_VIDEO_MODE_DISC: 
	case eAPP_AVP_VIDEO_MODE_DISC_DRS: 
		return true;
	default: break;
	}
	return false;
}

bool OSDEntry::IsUsbEntry(APP_ENTRY_POINT_T entry){
	switch(entry){
	case eAPP_AVP_AUDIO_MODE_USB1: 
	case eAPP_AVP_AUDIO_MODE_USB2: 
	case eAPP_AVP_VIDEO_MODE_USB1: 
	case eAPP_AVP_VIDEO_MODE_USB2: 
	case eAPP_AVP_VIDEO_MODE_USB1_DRS: 
	case eAPP_AVP_VIDEO_MODE_USB2_DRS: 
		return true;
	default: break;
	}
	return false;

}

bool OSDEntry::IsImageAppEntry(APP_ENTRY_POINT_T entry){
	switch(entry){
	case eAPP_AVP_PHOTO_MODE_JUKEBOX:
	case eAPP_AVP_PHOTO_MODE_USB1:
	case eAPP_AVP_PHOTO_MODE_USB2:
	case eAPP_AVP_PHOTO_MODE_JUKEBOX_DRS:
	case eAPP_AVP_PHOTO_MODE_USB1_DRS:
	case eAPP_APP_PHOTO_MODE_USB2_DRS:
		return true;
	default: break;
	}
	return false;
}

QString OSDEntry::EntryToString(APP_ENTRY_POINT_T entryPoint)
{
    switch(entryPoint)
    {
    case eAPP_NONE: return "NONE";
    case eAPP_AVP_AUDIO_MODE_JUKEBOX: return "JUKEBOX AUDIO";
    case eAPP_AVP_VIDEO_MODE_JUKEBOX: return "JUKEBOX VIDEO";
    case eAPP_AVP_PHOTO_MODE_JUKEBOX: return "JUKEBOX PHOTO";
    case eAPP_AVP_AUDIO_MODE_AUX: return "AUX AUDIO";
    case eAPP_AVP_VIDEO_MODE_AUX: return "AUX VIDEO";
    case eAPP_AVP_AUDIO_MODE_DISC: return "DISC AUDIO";
    case eAPP_AVP_VIDEO_MODE_DISC: return "DISC VIDEO";
    case eAPP_AVP_AUDIO_MODE_IPOD1: return "IPOD1 AUDIO";
    case eAPP_AVP_AUDIO_MODE_IPOD2: return "IPOD2 AUDIO";
    case eAPP_AVP_AUDIO_MODE_USB1: return "USB1 AUDIO";
    case eAPP_AVP_AUDIO_MODE_USB2: return "USB2 ADUIO";
    case eAPP_AVP_VIDEO_MODE_USB1: return "USB1 VIDEO";
    case eAPP_AVP_VIDEO_MODE_USB2: return "USB2 VIDEO";
    case eAPP_AVP_PHOTO_MODE_USB1: return "USB1 PHOTO";
    case eAPP_AVP_PHOTO_MODE_USB2: return "USB2 PHOTO";
    case eAPP_AVP_MODE_BLUETOOTH: return "BT AUDIO";
    case eAPP_AVP_AUDIO_MODE_BT: return "BT AUDIO";
    case eAPP_AVP_VIDEO_MODE_DISC_DRS: return "DRS DISC VIDEO";
    case eAPP_AVP_VIDEO_MODE_JUKEBOX_DRS: return "DRS JUKEBOX VIDEO";
    case eAPP_AVP_VIDEO_MODE_USB1_DRS: return "DRS USB1 VIDEO";
    case eAPP_AVP_VIDEO_MODE_USB2_DRS: return "DRS USB2 VIDEO";
    case eAPP_AVP_VIDEO_MODE_AUX_DRS: return "DRS AUX VIDEO";
    case eAPP_AVP_PHOTO_MODE_JUKEBOX_DRS: return "DRS JUKEBOX PHOTO";
    case eAPP_AVP_PHOTO_MODE_USB1_DRS: return "DRS USB1 PHOTO";
    case eAPP_APP_PHOTO_MODE_USB2_DRS: return "DRS USB2 PHOTO";
        //{added by junam 2014.01.17 for LIST_ENTRY_POINT
    case eAPP_FILEMANAGER_JUKEBOX_AUDIO: return "JUKEBOX AUDIO LIST";
    case eAPP_FILEMANAGER_USB1_AUDIO: return "USB1 AUDIO LIST";
    case eAPP_FILEMANAGER_USB2_AUDIO: return "USB2 AUDIO LIST";
    case eAPP_FILEMANAGER_IPOD1: return "IPOD1 AUDIO LIST";
    case eAPP_FILEMANAGER_IPOD2: return "IPOD2 AUDIO LIST";
        //}added by junam
    case eAPP_FILEMANAGER_DISC_AUDIO: return "DISC AUDIO LIST"; // added by oseong.kwon 2014.02.10 for ITS 223602
    default: break;
    }
    return QString::number( (int)entryPoint);
}


