#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_CONTROLLERVCD_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_CONTROLLERVCD_H

#include <QObject>
#include "DHAVN_AppMediaPlayer_Video_ControllerBase.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_VCD.h"
#include "deckengineermode.h"
#include "deckcontroller.h"
#include "imediainterface.h"
#include "mediaplayer_def.h"

class DeckController;
class AppMediaPlayer_Video_CModel_VCD;
class AppMediaPlayer_Video_CModel_Base;
class AppMediaPlayer_Controllers;
class AppMediaPlayer_EngineVideo; // added by kihyung 2012.07.12
class IVideoInterface;
class DvdcTOC;

class AppMediaPlayer_Video_ControllerVCD : public QObject,
                                           public AppMediaPlayer_Video_ControllerBase
{
    Q_OBJECT
public:
    AppMediaPlayer_Video_ControllerVCD( AppMediaPlayer_Controllers *controllers,
                                        AppMediaPlayer_EngineVideo *engine, // aded by kihyung 2012.07.12
                                        QObject *parent );
    ~AppMediaPlayer_Video_ControllerVCD();

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
    virtual void flickNext();
    virtual void flickPrev();
    virtual void toggleRepeat(); // added by ravikanth - 12-09-19
    virtual void prev(bool bForce = 0); //modified by edo.lee 2013.06.11
    virtual void next(); // added by cychoi 2013.07.07 for blocking next fucntion if PBC on 
    virtual void ShowCurrentModelToOSD();

    void onDeviceRemoved(); // added by cychoi 2013.08.07 for ITS 183155 Repeat mode init error on Disc Eject

    void switchTrack( int trackNo );
    void setPBC( bool enabled );

    void restoreDiscState();
    void storeDiscState();
    // { modified by cychoi 2013.07.07 for ISV 85460 DVD OSD mismatch
    void getVCDInfo(int trackIndex = -1);
    //void getVCDInfo();
    // } modified by cychoi 2013.07.07

    void pbcTenKey( int key );
    void pbcEnterKey();
    void pbcReturnKey();
    void pbcClearKey(); // added by cychoi 2013.07.06 for ITS 163782

    // { commented by Sergey for CR#14692
    //void setDeckPBC(bool enable);
    //bool isDeckPBC();
    // } commented by Sergey for CR#14692.
    
    // { added by kihyung 2012.08.27 for Video(LGE) #12
    void setPlaybackControl(bool bShow);
    // } added by kihyung
    // { modified by wspark 2014.02.19 for ITS 225827
    void setDirectAccessDisplayed (int disp, bool bShow) { m_DirectMenuState[disp] = bShow;}; //added by yungi 2013.09.05 for ITS 187311
    bool isDirectAccessDisplayed (int disp) { return m_DirectMenuState[disp]; } //added by yungi 2013.09.05 for ITS 187311
    // } modified by wspark
    void setPbcMenuDisplayed(bool value); // added by wspark 2013.03.17 for ISV 76329
    bool isPbcMenuShown(); // added by cychoi 2013.12.19 for ITS 215825 Default focus
    bool isMediaSearchState(); // added by shkim 2013.08.27 for ITS 186402
    QString getOSDTitle(QString title);
	void showOSD(bool isStart){m_isStart = isStart;}
    void retranslateUI(); // added by cychoi 2013.10.27 for ITS 197433

private:
    DeckController               *m_pDeckController;
    IVideoInterface              *m_pIVideo;
    AppMediaPlayer_EngineVideo   *m_pEngineVideo; // added by kihyung 2012.07.12
    AppMediaPlayer_Controllers   *m_pControllers;
    AppMediaPlayer_Video_CModel_VCD *m_pModel;
    VCDStatusResponse             m_storedVCDInfo;
    int                           m_tempTuneIndex;
    int                           m_lastPlayingIndex;
    bool                          m_onRestoreDiscState;
	QString                       m_searchTitle;
    bool                          m_bVCDMuteState; // modified by cychoi 2013.09.01 for ITS 183910 & 183912 Sound path if DVD or VCD  // modified by yongkyun.lee 2013-07-16 for : ITS 175107
    bool                          m_isStart;
    bool                          m_DirectMenuState[2]; // modified by wspark 2014.02.19 //added by yungi 2013.09.05 for ITS 187311
//    bool                          m_bDeckPBCMode; // commented by Sergey for CR#14692
// removed by cychoi 2013.06.04 for ISV 78671

signals:
    // removed by Sergey 23.09.2013 for ITS#188498
    void showPlaybackControls(bool bShow); // added by kihyung 2012.08.27 for Video(LGE) #12
    // removed by sjhyun 2013.11.05 for ITS 206357
    void cancelFfRew(); // added by yongkyun 2013.01.19 for ITS 150703
private slots:
    void onTrackChanged(int);
    void onTOCReceived( DvdcTOC *pTOC );
    void onScreenStateChanged( int nScreenState );
    void onStateChanged( int nState );
    void onPositionChanged( int position ); // added by Sergey Vetugov for CR#13620
    void onHandleError(int nError); // added by kihyung 2012.11.29
    void onRealSoundChanged(); // added by cychoi 20103.06.11
    void OnTrackBufferred(int bPrev, int nTotalIdx, int nIdxInFolder, int nFolderSize); // added by cychoi 2013.06.18 for ISV 85460 DVD OSD mismatch
    void onUserTrackChanged(int trackindex);
    //void onAutoTrackChanged(int trackIndex); // added by cychoi 2014.04.06 for ITS 233500
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_CONTROLLERVCD_H
