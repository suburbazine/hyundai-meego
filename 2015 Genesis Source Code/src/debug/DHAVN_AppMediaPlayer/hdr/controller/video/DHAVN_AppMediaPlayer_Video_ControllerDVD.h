#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_CONTROLLERDVD_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_CONTROLLERDVD_H

#include <QObject>
#include <QHash>
#include "DHAVN_AppMediaPlayer_Video_ControllerBase.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_DVD.h"
#include "deckengineermode.h"
#include "deckcontroller.h"
#include "imediainterface.h"
#include "mediaplayer_def.h"

class AppMediaPlayer_Video_CModel_Base;
class AppMediaPlayer_Video_CModel_DVD;
class AppMediaPlayer_Controllers;
class AppMediaPlayer_EngineVideo; // added by lssanh 2012.08.24 for audio path
class DeckController;
class IVideoInterface;


class AppMediaPlayer_Video_ControllerDVD : public QObject,
                                           public AppMediaPlayer_Video_ControllerBase
{
    Q_OBJECT

public:
    AppMediaPlayer_Video_ControllerDVD( AppMediaPlayer_Controllers *controllers,
                                        AppMediaPlayer_EngineVideo *engine, // added by lssanh 2012.08.24 for audio path
                                        QObject *parent );
    ~AppMediaPlayer_Video_ControllerDVD();

    virtual void dropControllers();
    virtual void attachControllers();
    virtual AppMediaPlayer_Video_CModel_Base *getModel();
    virtual void setAudioPath();
    virtual void rewind();
    virtual void rewindCritical();
    virtual void fastForward();
    virtual void fastForwardCritical();
    virtual bool tuneWheel(bool bTuneForward); // modified by kihyung 2012.07.17. add return value.
    virtual bool selectTuned(); // modified by junggil 2012.07.05 for CR11261 : Not able to search video files using CCP/RRC jogdial
    virtual void cancelTune();
    virtual void flickPrev();
    virtual void flickNext();
    // { added by cychoi 2013.07.22 for for DVD Seek Up/Down mute
    virtual void prev(bool bForce = 0);
    virtual void next();
    // } added by cychoi 2013.07.22
    virtual void ShowCurrentModelToOSD();

    void onDeviceRemoved(); // added by cychoi 2013.08.07 for ITS 183155 Repeat mode init error on Disc Eject

    void titleMenu();
    void topMenu();
    void selectTitleChapter( int title, int chapter );
    void selectChapter(int chapter);
    void restoreDiscState();
    void storeDiscState();
    // { modified by cychoi 2013.07.07 for ISV 85460 DVD OSD mismatch
    void getDVDInfo(int trackIndex = -1);
    //void getDVDInfo();
    // } modified by cychoi 2013.07.07
    int getChapterCount(); // added by minho 20120828 for able list menu on DVD
    void cursorKey(int key);
    void select();
    void requestDefaultLanguagesSet(); // modified by ravikanth CR 15994 12-11-22
	// { modified by ravikanth CR 15994 12-11-22
//    void setAudioLanguage( int code, int index );
    void setAudioLanguage( int code);
	// } modified by ravikanth CR 15994 12-11-22
    void setSubtitleLanguage( int code );
    void setMenuLanguage(int code); // modified by ravikanth CR 15994 12-11-22
    void setDVDSettingsLanguages(); // added by cychoi 2013.06.24 for new DVD Settings UX
    void setAngle( int code );
    void cleanDVDSettingsInfo();

    // { added by lssanh 2013.04.19 ITS158176
    int getAudioLanguage();
    int getSubtitleLanguage();
    int getMenuLanguage();
    int getAngle();    
    // } added by lssanh 2013.04.19 ITS158176

    // { commented by cychoi 2014.06.27 for removed too many logs... // { added by kihyung 2012.07.13 for CR 9867
    //void setSelectByTitleMenu(bool bShow);
    //bool getSelectByTitleMenu();
    // } commented by cychoi 2014.06.27 for removed too many logs... // } added by kihyung

    // { added by kihyung 2012.07.29 for CR 11057
    void requestScreenTitleChapter();
    // } added by kihyung
	// { modified by ravikanth CR 15994 12-11-22
    void switchCaption();
    void switchVoice();
    void switchAngle();
    void InitDeckLang(); // added by cychoi 2013.09.08 for ITS 181382 EVT_INITIALIZATION_REQUEST
	QString getOSDTitle(QString title, QString chapter); // modified by cychoi 2014.02.21 for ITS 226393
	void showOSD(bool isStart){m_isStart = isStart;}
    bool isScreenDVDMenuTitleState(); // added by cychoi 2013.09.24 for ITS 191333
    void retranslateUI(); // added by cychoi 2013.10.22 for ITS 197384

private:
    void SendVideoControlData( int captionLangCode, int audioLangCode, int menuLangCode);
	// } modified by ravikanth CR 15994 12-11-22
    
signals:
    void screenStateChanged( int nScreenState );
    // removed by Sergey for CR#15775
    // removed by Sergey 23.09.2013 for ITS#188498
    // removed by sjhyun 2013.11.05 for ITS 206357
    void cancelFfRew(); // added by yongkyun 2013.01.19 for ITS 150703   
private:
    AppMediaPlayer_Controllers      *m_pControllers;
    AppMediaPlayer_Video_CModel_DVD *m_pModel;
    AppMediaPlayer_EngineVideo   *m_pEngineVideo; // added by lssanh 2012.08.24 for audio path
    DeckController                  *m_pDeckController;
    IVideoInterface                 *m_pIVideo;

    DVDStatusResponse              m_storedDVDInfo;
    int                            m_saveChapter;//added by yongkyun.lee  2012.10.17  for Current file is white color
    int                            m_nPendingChapter;
    int                            m_nPendingTitle;
    bool                           m_onRestoreDiscState;
    //bool                           m_bSelectByTitleMenu; // commented by cychoi 2014.06.27 for removed too many logs... // added by kihyung 2012.07.13 for CR 9867
    // { added by cychoi 2013.06.24 for new DVD Settings UX
    int                            m_nUserSubTitleLangCode;
    int                            m_nUserAudioLangCode;
    int                            m_nUserMenuLangCode;
    // } added by cychoi 2013.06.24
    bool                           m_bDVDMuteState; // modified by cychoi 2013.09.01 for ITS 183910 & 183912 Sound path if DVD or VCD //modified by yongkyun.lee 2013-07-17 for : ITS 175107
    QString                        m_searchTitle;
	bool                           m_isStart;
    bool                           m_bPlayTimeOSDAllowed; // added by cychoi 20103.10.16 for ITS 196052 OSD on DVD Audio Play
    bool                           m_bDelayedMuteForSeek; // added by cychoi 20103.12.05 for ITS 212889 should not mute on forbidden command
    bool                           m_bDelayedLoadingMode; // added by cychoi 2014.01.29 for ITS 223305 delayed loading screen
    // removed by cychoi 2014.03.18 for ITS 229882 No option menu on Title/Disc Menu Screen // added by yongkyun.lee 2013-11-05 for ITS 206356

private slots:
    void onTrackChanged(int);
    void onSearchCompleted( int nTitleNo, int nChapterNo, int nChapterSize );
    void onActiveAudioLangChanged(int nAudioCode);
    void onActiveCaptionChanged(int nSubtitleCode);
    void onActiveAngleChanged(int nAngle);
    void OnDeckLangChanged(); // modified by ravikanth 27-12-12
    void onScreenStateChanged( int nScreenState );
    void onStateChanged( int nState );
    void onHandleError(int nError); // added by kihyung 2012.07.13 for CR 9867
    void onRealSoundChanged(); // added by cychoi 20103.06.11
    void OnTrackBufferred(int bPrev, int nTotalIdx, int nIdxInFolder, int nFolderSize); // added by cychoi 2013.06.18 for ISV 85460 DVD OSD mismatch
    void onUserTrackChanged(int trackindex);
    //void onAutoTrackChanged(int trackIndex); // added by cychoi 2014.04.06 for ITS 233500
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_CONTROLLERDVD_H
