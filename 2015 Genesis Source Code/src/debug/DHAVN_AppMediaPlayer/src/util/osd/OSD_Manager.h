#ifndef	OSD_MANAGER_H_
#define	OSD_MANAGER_H_


#include "OSD_AbstractManager.h"

class OSDScreenManager;
class AppMediaPlayer_EngineMain;
class QTime;

typedef struct{
	QString m_prefix;
	QString m_title;
	OSD_DEVICE m_device;

	bool m_isTune;
	int m_shuffle;
	int m_repeat;
	int m_scan;
	int m_pos;
} OSDLastInfo;

class OSDManager : public OSDAbstractManager{
Q_OBJECT
public:
	static OSDManager * instance(AppMediaPlayer_EngineMain * pEngineMain = 0){
		if ( pOSDManager == NULL){
			Q_ASSERT(pEngineMain != NULL);
			pOSDManager = new OSDManager(pEngineMain);
		}

		return pOSDManager;
	}

	void displayNoMediaFileOSD(MEDIA_SOURCE_T source);
	void displayDVDMenuOSD(bool bothScreen);
    void displayDiscErrorOSD(int error, DISPLAY_T display = DISPLAY_FRONT_REAR); // modified by cychoi 2014.04.09 for HW Event DV2-1 1403-00067 HIGH TEMPERATURE handling
	void setFileManagerScreen(DISPLAY_T display);
	void setFileManagerToBG();

	void displayDVDMenuMode();

	void displayOSD(unsigned int nEventType, APP_ENTRY_POINT_T entry = eAPP_NONE);

	void preHandleUishEvent(Event aEvent, DISPLAY_T display = DISPLAY_FRONT);
	void postHandleUishEvent(Event aEvent, DISPLAY_T display = DISPLAY_FRONT);

	void clearOSDPub(unsigned int aDisplay) { clearAvOSD(aDisplay); }	//added by hyochang.ryu 20130821
	void setAUXDisconnect(bool val);//{ modified by yongkyun.lee 2013-09-09 for : AUX video->audio OSD
	bool getAUXDisconnect( );// modified by yongkyun.lee 2013-10-07 for :  ITS 193970
	void checkAuxMode();	//added by hyochang.ryu 20131024 for ITS 192354
    void connectOSDsignalToSlot();//added by suilyou 20131213 ITS 0214646
private:
	OSDManager(AppMediaPlayer_EngineMain * pEngineMain);
	~OSDManager();

	static OSDManager * pOSDManager;

	OSDScreenManager * m_screenManager;
	OSDLastInfo m_lastInfo;

	QString getLastModeString();
	QString getLastTimeString();
	
	QString	translateTitle(const QString &);
	
	inline QString createPrefix(bool isLong = false);
	inline QString createContent(const QString & title = QString());
	inline QString getDisplayString(bool isLong);
	inline QString getDeviceString(OSD_DEVICE device);
	inline QString convertToSecond(int duration, bool isAudio = true);

	void displayOSDWithTime();

	void resetLastAdditionalInfo();
	void resetLastInfo();
	void saveToLastInfo(OSD_DEVICE device, const QString & title, int shuffle, int repeat, int scan);
	void showCurrentOSD(OSD_DEVICE device, int pos, bool isLong, bool isCurrentShow);


private slots:
	void displayOSD(OSD_DEVICE device, QString title, int shuffle, int repeat, int scan, bool isTune);
	void updatePosition(OSD_DEVICE device, int pos, bool isLong);
	void updatePlayingMode(OSD_DEVICE device, int shuffle, int repeat, int scan);
	void updateMetaData(OSD_DEVICE device, QString);
    void updateCDDAOSDMetaData(OSD_DEVICE device, QString); // added by wonseok.heo for ITS 197433 2013.10.28

	void showCurrentOSD();

	void clearAvOSD(unsigned int aDisplay);
	void clearConnectInfo();

	// DEVICE EVENT
	void onDiscLoading();
	void onDeviceRemoved(MEDIA_SOURCE_T, MEDIA_TYPE_T);
	void displayAuxMode();
};


#endif
