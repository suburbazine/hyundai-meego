#include "OSD_ScreenManager.h"

OSDScreenManager::OSDScreenManager(AppMediaPlayer_EngineMain * pEngineMain)
: OSDAbstractManager(pEngineMain){
	m_frontEntry = m_rearEntry = eAPP_NONE;
	m_listScreen = DISPLAY_NONE;	
	m_frontVideoFullScreen = m_rearVideoFullScreen = false;
	m_frontVideoSubScreen = m_rearVideoSubScreen = false; // added by cychoi 2013.09.22 for ITS 190767 Tune OSD on DirectAccess

	m_cloneStatus = CLONE_OFF;

	m_agreeShow = false;

	m_waitCloneRelease = false;

	m_cloneModeFG = false;

	m_drsForeground = false;

    m_AuxDisconnect = false;    //added by honggi.shin 2013.10.25, fix static test problem

	connect( engine(), SIGNAL(showSubScreen()), SLOT(enterSubScreen()));
	connect( engine(), SIGNAL(hideSubScreen()), SLOT(exitSubScreen()));
    // { added by cychoi 2013.09.22 for ITS 190767 Tune OSD on DirectAccess
	connect( engine(), SIGNAL(showVideoSubScreen(bool)), SLOT(enterVideoSubScreen(bool)));
	connect( engine(), SIGNAL(hideVideoSubScreen(bool)), SLOT(exitVideoSubScreen(bool)));
    // } added by cychoi 2013.09.22
	connect( engine(), SIGNAL(showFullScreen(bool)), SLOT(enterFullScreen(bool)));
	connect( engine(), SIGNAL(hideFullScreen(bool)), SLOT(exitFullScreen(bool)));
}

void OSDScreenManager::enterSubScreen(){
	MP_LOG << "OnScreenDisplay" << LOG_ENDL;
}

void OSDScreenManager::exitSubScreen(){
	MP_LOG << "OnScreenDisplay" << LOG_ENDL;
}

// { added by cychoi 2013.09.22 for ITS 190767 Tune OSD on DirectAccess
void OSDScreenManager::enterVideoSubScreen(bool isFront){
	MP_LOG << "OnScreenDisplay : " << isFront << LOG_ENDL;
	if ( isFront ) m_frontVideoSubScreen = true;
	else m_rearVideoSubScreen = true;
}

void OSDScreenManager::exitVideoSubScreen(bool isFront){
	MP_LOG << "OnScreenDisplay : " << isFront << LOG_ENDL;
	if ( isFront ){
		if ( m_frontVideoSubScreen && OSDEntry::IsVideoAppEntry(m_frontEntry))
			clearAvOSD( (unsigned int) DISPLAY_FRONT);

		m_frontVideoSubScreen = false;
	}
	else{
		if ( m_rearVideoSubScreen && OSDEntry::IsVideoAppEntry(m_rearEntry))
			clearAvOSD( (unsigned int) DISPLAY_REAR);

		m_rearVideoSubScreen = false;
	}
}
// } added by cychoi 2013.09.22

void OSDScreenManager::enterFullScreen(bool isFront){
	MP_LOG << "OnScreenDisplay : " << isFront << LOG_ENDL;
	if ( isFront ) m_frontVideoFullScreen = true;
	else m_rearVideoFullScreen = true;
}
void OSDScreenManager::setFileManagerScreen(DISPLAY_T display){
	MP_LOG << "OnScreenDisplay" << LOG_ENDL;
	m_listScreen = display;
}

void OSDScreenManager::setFileManagerBG(){
	MP_LOG << "OnScreenDisplay" << LOG_ENDL;

	if ( m_listScreen != DISPLAY_NONE &&
			isAudioModeCurrent()){
		if ( OSDEntry::IsMusicAppEntry(m_frontEntry))
			clearAvOSD( (unsigned int) DISPLAY_FRONT);

		if ( OSDEntry::IsMusicAppEntry(m_rearEntry))
			clearAvOSD( (unsigned int) DISPLAY_REAR);
	}

	m_listScreen = DISPLAY_NONE;
}

void OSDScreenManager::exitFullScreen(bool isFront){
	MP_LOG << "OnScreenDisplay : " << isFront << LOG_ENDL;
	if ( isFront ){
		if ( m_frontVideoFullScreen && OSDEntry::IsVideoAppEntry(m_frontEntry))
			clearAvOSD( (unsigned int) DISPLAY_FRONT);

		m_frontVideoFullScreen = false;
	}
	else{
		if ( m_rearVideoFullScreen && OSDEntry::IsVideoAppEntry(m_rearEntry))
			clearAvOSD( (unsigned int) DISPLAY_REAR);

		m_rearVideoFullScreen = false;
	}
}

bool OSDScreenManager::isSubScreen(APP_ENTRY_POINT_T entry){
	switch(entry){
	case eAPP_AVP_VIDEO_CAPTION_SETTING:
	case eAPP_AVP_VIDEO_DIVX_SETTING:
	case eAPP_AVP_DVD_SETTING:
	case eAPP_AVP_VCD_PBC_MENU_SETTING:
	case eAPP_AVP_VCD_SHORTCUT_SETTING:
		return true;
	default: break;
	}
	return false;
}

DISPLAY_T OSDScreenManager::displayOfOSD(bool isLong){
	APP_ENTRY_POINT_T frontEntry, rearEntry;
	bool frontList, rearList;
	
	bool isFrontOut;
	bool isRearOut;
    // bool isDRS = false; // added by yungi 2013.10.23 for ITS 198148

	isFrontOut = isRearOut = false;
	frontList = rearList = false;
	frontEntry = m_frontEntry;
    rearEntry = m_rearEntry;

    // { modified by yungi 2013.11.05 for ITS 206355 // added by yungi 2013.10.23 for ITS 198148
    // remove by edo.lee 2013.11.06
    //VIDEO_DISPLAY_T lockout_disp = VIDEO_DISPLAY_MAX;
    //AppMediaPlayer_EngineVideo * engineVideo = (AppMediaPlayer_EngineVideo *)engine()->GetEngine(APP_MEDIA_MODE_VIDEO);

    //if (engineVideo != NULL)
    //{
    //    lockout_disp = engineVideo->getIsShowLockoutView();
    //    isDRS = ( VIDEO_DISPLAY_MAX != lockout_disp );  // not VIDEO_DISPLAY_MAX
    //}
    // } modifed by yungi

	if ( m_listScreen == DISPLAY_FRONT ) frontList = true;
	else if ( m_listScreen == DISPLAY_REAR) rearList = true;
	else if ( m_listScreen == DISPLAY_FRONT_REAR) frontList = rearList = true;

	if ( m_cloneStatus == CLONE_ON_FRONT){
		rearEntry = frontEntry;
		rearList = frontList; 
	}
	else if ( m_cloneStatus == CLONE_ON_REAR){
		frontEntry = rearEntry;
		frontList = rearList;
	}

	if ( frontEntry == eAPP_NONE ){
		if ( isVideoModeCurrent() && !isLong && frontList)
			isFrontOut = false;		// VIDEO LIST
		else
			isFrontOut = true;
	}
	else if ( OSDEntry::IsImageAppEntry(frontEntry)){
		isFrontOut = true;
	}
	else if ( OSDEntry::IsVideoAppEntry(frontEntry)){
        if ( m_frontVideoFullScreen || m_frontVideoSubScreen )//|| isDRS) // deleted by yungi 2013.10.23 for ITS 198148 // modified by cychoi 2013.09.22 for ITS 190767 Tune OSD on DirectAccess
            isFrontOut = true;
        // { added by cychoi 2014.02.24 for ITS 226456 always display front OSD on DRS mode
        else if(OSDEntry::IsVideoAppDRSEntry(frontEntry))
            isFrontOut = true;
        // } added by cychoi 2014.02.24
	}
	else if ( OSDEntry::IsMusicAppEntry(frontEntry)){
		if ( frontList && isLong ) isFrontOut = true;
	}
	else if ( isSubScreen(frontEntry)){
		isFrontOut = true;
	}

	if ( rearEntry == eAPP_NONE ){
		if ( isVideoModeCurrent() && !isLong && rearList)
			isRearOut = false;
		else
			isRearOut = true;
	}
	else if ( OSDEntry::IsImageAppEntry(rearEntry)){
		isRearOut = true;
	}
	else if ( OSDEntry::IsVideoAppEntry(rearEntry)){
        if ( m_rearVideoFullScreen || m_rearVideoSubScreen)// || m_drsForeground ) // modified by yungi 2013.11.05 for ITS 206355 // deleted by yungi 2013.10.23 for ITS 198148 //modified by cychoi 2013.09.22 for ITS 190767 Tune OSD on DirectAccess
            isRearOut = true;
	}
	else if ( OSDEntry::IsMusicAppEntry(rearEntry)){
		if ( rearList && isLong ) isRearOut = true;
	}
	else if ( isSubScreen(rearEntry)){
		isRearOut = true;
	}

    // { added by yungi 2013.11.06 for ITS 206355
    // remove by edo.lee 2013.11.06
    //if (!lockout_disp == VIDEO_DISPLAY_FRONT) // Swap Check
    //{
    //    bool temp = isFrontOut;
    //    isFrontOut = isRearOut;
    //    isRearOut = temp;
    //}
    // } added by yungi

	if ( isFrontOut && isRearOut)
		return DISPLAY_FRONT_REAR;
	else if ( isFrontOut && !isRearOut)
		return DISPLAY_FRONT;
	else if ( !isFrontOut && isRearOut)
		return DISPLAY_REAR;

	return DISPLAY_NONE;
}

bool OSDScreenManager::blockOSD()
{
    if ( m_waitCloneRelease )
	{
		 return true;
	}
	else if (m_drsForeground)
	{ 
		m_drsForeground = false; //added by edo.lee 2013.08.29
                return true;
	}
	else if (m_cloneModeFG)
	{
		if ( isVideoModeCurrent())
		{
			if ( OSDEntry::IsVideoAppEntry(m_frontEntry) && (m_frontVideoFullScreen || m_frontVideoSubScreen)) // modified by cychoi 2013.09.22 for ITS 190767 Tune OSD on DirectAccess
				return false;
			else if ( OSDEntry::IsVideoAppEntry(m_rearEntry) && (m_rearVideoFullScreen || m_rearVideoSubScreen)) // modified by cychoi 2013.09.22 for ITS 190767 Tune OSD on DirectAccess
				return false;
		}
		return true;
	}
    else//this is to block ipod osd during phone call. OSD_EVENT_IPOD_EMPTY_TITLE will be displayed instead.
    {
        AppMediaPlayer_EngineMain* pEngineMain = engine();
        AppMediaPlayer_EngineAudio* pEngineAudio = (AppMediaPlayer_EngineAudio*)pEngineMain->GetEngine(APP_MEDIA_MODE_AUDIO);
        AppMediaPlayer_Controller_Audio *pControllerAudio = (AppMediaPlayer_Controller_Audio*)pEngineAudio->GetController();
        int osd_device = pControllerAudio->getOSDevice(pControllerAudio->getDevice());
        bool isIPodDuringPhoneCall =  ( osd_device == IPOD_MUSIC) && pEngineMain->getisBTCall();
        if(isIPodDuringPhoneCall)
        {
            return true;
        }
    }
	return false;
}

bool OSDScreenManager::setScreenEntry(DISPLAY_T display, APP_ENTRY_POINT_T entry){
	MP_LOG << "OnScreenDisplay : setScreenEntry : " << display << ", entry : " << OSDEntry::EntryToString(entry) << LOG_ENDL;
	if ( display == DISPLAY_FRONT ){
		m_frontEntry = entry;
		setEntry(true, m_frontEntry);
	}
	else if ( display == DISPLAY_REAR){
		 m_rearEntry = entry;
		setEntry(false, m_rearEntry);
	}
	else{
		MP_HIGH << "OnScreenDisplay : setScreenEntry UNKNOWN : " << display << LOG_ENDL; 
		return false;
	}

	if ( entry == eAPP_NONE || isSubScreen(entry) || OSDEntry::IsImageAppEntry(entry) )
		return false;
	else{
		if ( OSDEntry::IsVideoAppEntry(entry) && m_listScreen != DISPLAY_NONE)	// for Back key from Video FileManager
			setFileManagerBG();

		if ( m_cloneStatus == CLONE_ON_FRONT || m_cloneStatus == CLONE_ON_REAR)
		    emit clearAvOSD( (unsigned int) DISPLAY_FRONT_REAR);
		else{
            // { modified by cychoi 2013.09.22 for ITS 190767 Tune OSD on DirectAccess
		    if((m_frontVideoFullScreen == false && m_frontVideoSubScreen == false && display == DISPLAY_FRONT) // when enter video with full screen
		       ||(m_rearVideoFullScreen == false && m_rearVideoSubScreen == false && display == DISPLAY_REAR))//added by edo.lee 2013.08.23
		        emit clearAvOSD( (unsigned int) display);
            // } modified by cychoi 2013.09.22
		}
	}
	return true;
}

void OSDScreenManager::setCloneStatus(CLONE_STATUS_T status){
	// TODO: find todo when clone status is changed
	MP_LOG << "OnScreenDisplay(" << status << ") : " << (status == CLONE_OFF ? "CLONE_OFF" : ( status == CLONE_ON_FRONT ? "CLONE_ON_FRONT" : "CLONE_ON_REAR") ) << LOG_ENDL;
	m_cloneStatus = status;
	if ( m_cloneStatus == CLONE_OFF){	// CLONE_OFF
		setEntry(true, m_frontEntry);
		setEntry(false, m_rearEntry);

		if ( m_waitCloneRelease ){
			m_waitCloneRelease = false;
			emit lastIncomedOSD();
		}
	}
	else if ( m_cloneStatus == CLONE_ON_FRONT){
		setEntry(false, m_frontEntry);

		if ( isAudioModeCurrent()) clearAvOSD( (unsigned int) DISPLAY_REAR);
		else if ( isVideoModeCurrent() && !m_frontVideoFullScreen && !m_frontVideoSubScreen) // modified by cychoi 2013.09.22 for ITS 190767 Tune OSD on DirectAccess
			clearAvOSD( (unsigned int) DISPLAY_REAR);
	}
	else if ( m_cloneStatus == CLONE_ON_REAR){
		setEntry(true, m_rearEntry);
		if ( isAudioModeCurrent()) clearAvOSD( (unsigned int) DISPLAY_FRONT);
		else if ( isVideoModeCurrent() && !m_rearVideoFullScreen && !m_rearVideoSubScreen) // modified by cychoi 2013.09.22 for ITS 190767 Tune OSD on DirectAccess
			clearAvOSD( (unsigned int) DISPLAY_FRONT);
	}
	m_cloneModeFG = false;
}

void OSDScreenManager::preHandleUishEvent(Event aEvent, DISPLAY_T display){
    switch(aEvent.GetEventId()){
    case EVT_REQUEST_PRE_FG: // modified by Sergey 07.09.2013 for PRE_FG supoort
    {
        MP_MEDIUM << "OnScreenDisplay : EVT_REQUEST_PRE_FG : " << display << LOG_ENDL;
        FETCH_EVENT_DATA(APP_ENTRY_POINT_DATA_T);

        m_cloneModeFG = false;

        if ( !setScreenEntry(display, pEventData->sAppEntryPoint)) return;

        bool modeChanged = setCurrentMode(pEventData->sAppEntryPoint);

        emit clearConnectInfo();

        if ( pEventData->sHeader.bReserved07){
            MP_LOG << "OnScreenDisplay : reserved 7" << LOG_ENDL;
            if ( !m_drsForeground)
                m_drsForeground = true;
            else
                m_drsForeground = false;
        }
        else if ( pEventData->sHeader.bReserved02){
            MP_LOG << "OnScreenDisplay : reserved 2" << LOG_ENDL;
            m_cloneModeFG = true;
        }
        else if ( modeChanged && (m_cloneStatus == CLONE_ON_FRONT || m_cloneStatus == CLONE_ON_REAR) && (pEventData->sHeader.bReserved05 || pEventData->sHeader.bReserved06) ){
            m_waitCloneRelease = true;	
            MP_LOG << "OnScreenDisplay : reserved" << LOG_ENDL;
        }

        //if ( modeChanged ) checkAuxMode();	//deleted by hyochang.ryu 20131024 for ITS 192354
    }	
    break;

    case EVT_REQUEST_BG:
    {
        MP_MEDIUM << "OnScreenDisplay : EVT_REQUEST_BG : " << display << LOG_ENDL;
        m_cloneModeFG = false;
        m_drsForeground = false;//added by edo.lee 2013.08.24

        setScreenEntry(display, eAPP_NONE);
    }
    break;

    case EVT_CLONE_NOTIFICATION:	// all clone status is delivered
    {
        // TODO: when occurs AV clone, then clear OSD
        FETCH_EVENT_DATA(EVENT_CLONE_NOTIFICATION_DATA_T);
        setCloneStatus(pEventData->sCloneStatus);
    }	
    break;

    case EVT_AV_MODE_CHANGE:
    {
        MP_MEDIUM << "OnScreenDisplay : EVT_AV_MODE_CHANGE" << LOG_ENDL;
        FETCH_EVENT_DATA(EVENT_MODE_CHANGED_DATA_T);
        if(setMode( pEventData->sMode)) checkAuxMode();
    }
    break;

    case EVT_AGREE_SHOW_OSD:
    {
        MP_MEDIUM << "OnScreenDisplay : EVT_AGREE_SHOW_OSD" << LOG_ENDL;
        if(engine()->getOnBoot()) // added by edo.lee 2013.11.05 ITS 0206260
            m_agreeShow = true;
        if ( currentMode() != MODE_STATE_NONE)
            emit agreeScreenShow();		
    }

    default: break;
    }
}

void OSDScreenManager::checkAuxMode(){
    //{ modified by yongkyun.lee 2013-09-09 for : AUX video->audio OSD
    if(m_AuxDisconnect)
        return;
    //} modified by yongkyun.lee 2013-09-09 
	switch ( currentMode()){
	case MODE_STATE_AUX1_AUDIO:
	case MODE_STATE_AUX1_VIDEO:
	case MODE_STATE_AUX2_AUDIO:
	case MODE_STATE_AUX2_VIDEO:
		MP_LOG << "OnScreenDisplay : AUX MODE ENTERED" << LOG_ENDL;
		emit auxModeEntered();
		break;
	default: break;
	}
}

void OSDScreenManager::postHandleUishEvent(Event aEvent, DISPLAY_T display){

}

