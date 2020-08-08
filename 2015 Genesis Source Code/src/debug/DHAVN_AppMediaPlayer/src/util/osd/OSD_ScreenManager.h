#ifndef	OSD_SCREEN_MANAGER_H_
#define	OSD_SCREEN_MANAGER_H_

#include "OSD_AbstractManager.h"
#include "../hdr/engine/DHAVN_AppMediaPlayer_EngineVideo.h" // added by yungi 2013.10.23 for ITS 198148
#include "../hdr/engine/DHAVN_AppMediaPlayer_EngineAudio.h" // added by sangmin.seol 2014.05.21 ITS 0236977 fix clearAVOSD problem

class OSDScreenManager : public OSDAbstractManager{
Q_OBJECT
public:
	OSDScreenManager(AppMediaPlayer_EngineMain * pEngineMain);

	void preHandleUishEvent(Event aEvent, DISPLAY_T display);
	void postHandleUishEvent(Event aEvent, DISPLAY_T display);
	DISPLAY_T displayOfOSD(bool isLong);
	void clearAvOSD(DISPLAY_T display);

	bool blockOSD();

	bool agreeShow() { return m_agreeShow; }

	void setFileManagerScreen(DISPLAY_T display);
	void setFileManagerBG();
	void setAuxDisconnect(bool val ){ m_AuxDisconnect = val;   }//{ modified by yongkyun.lee 2013-09-09 for : AUX video->audio OSD
	bool getAuxDisconnect( ){ return m_AuxDisconnect;   }// modified by yongkyun.lee 2013-10-07 for :  ITS 193970
	void checkAuxMode(); //moved by jaehwan.lee to fix buid error
	
private:
	APP_ENTRY_POINT_T m_frontEntry;
	APP_ENTRY_POINT_T m_rearEntry;

	bool setScreenEntry(DISPLAY_T display, APP_ENTRY_POINT_T entry);
	void setCloneStatus(CLONE_STATUS_T status);

	CLONE_STATUS_T m_cloneStatus;

	bool m_frontVideoFullScreen;
	bool m_rearVideoFullScreen;

    // { added by cychoi 2013.09.22 for ITS 190767 Tune OSD on DirectAccess
	bool m_frontVideoSubScreen;
	bool m_rearVideoSubScreen;
    // } added by cychoi 2013.09.22

	bool m_agreeShow;

	bool m_waitCloneRelease;
	bool m_cloneModeFG;
	bool m_drsForeground;

	bool isSubScreen(APP_ENTRY_POINT_T);

	DISPLAY_T m_listScreen;
        bool m_AuxDisconnect; //{ modified by yongkyun.lee 2013-09-09 for : AUX video->audio OSD

signals:
	void clearAvOSD(unsigned int);
	void agreeScreenShow();
	void lastIncomedOSD();
	void clearConnectInfo();

private slots:
	void enterSubScreen();
	void exitSubScreen();
    // { added by cychoi 2013.09.22 for ITS 190767 Tune OSD on DirectAccess
	void enterVideoSubScreen(bool);
	void exitVideoSubScreen(bool);
    // } added by cychoi 2013.09.22
	void enterFullScreen(bool);
	void exitFullScreen(bool);
};

#endif
