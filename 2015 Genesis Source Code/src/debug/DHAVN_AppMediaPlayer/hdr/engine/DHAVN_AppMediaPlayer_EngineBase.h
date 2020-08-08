#if !defined( APP_MEDIA_PLAYER_ENGINE_BASE_H )
#define APP_MEDIA_PLAYER_ENGINE_BASE_H

#include <QDeclarativeItem>
#include <DHAVN_AppFramework_AppVextEngineQML_Def.h>
#include "controller/DHAVN_AppMediaPlayer_ControllerBase.h"

typedef enum
{
   APP_MEDIA_MODE_AUDIO,
   APP_MEDIA_MODE_VIDEO,
   APP_MEDIA_MODE_PHOTO_FRONT,
   APP_MEDIA_MODE_PHOTO_REAR,
   APP_MEDIA_MODE_MAX
} APP_MEDIA_MODE_T;

typedef enum
{
    JUKEBOX_MUSIC,
    JUKEBOX_VIDEO,
    USB_MUSIC,
    USB_VIDEO,
    // { modified by cychoi 2013.11.12 for OSD Title Translation
    DISC_MP3_MUSIC,
    DISC_CDDA_MUSIC,
    DISC_DVD_VIDEO,
    DISC_VCD_VIDEO,
    // } modified by cychoi 2013.11.12
    IPOD_MUSIC,	
    BLUETOOTH_MUSIC,
    AUX_MUSIC,
    AUX_VIDEO,
    DEVICE_MAX
} OSD_DEVICE;


typedef enum
{
   APP_MPLAYER_STATE_NONE,
   APP_MPLAYER_AUDIO_STATE_INIT,
   /* Adding above AUDIO related states */

   APP_MPLAYER_VIDEO_STATE_INIT,
   APP_MPLAYER_VIDEO_STATE_JUKEBOX,
   APP_MPLAYER_VIDEO_STATE_USB,
   APP_MPLAYER_VIDEO_STATE_VCD,
   APP_MPLAYER_VIDEO_STATE_DVD,
   APP_MPLAYER_VIDEO_STATE_AUX,  //! reserved for future use
   APP_MPLAYER_VIDEO_STATE_MAX,
   /* Adding above AUDIO related states */

   APP_MPLAYER_PHOTO_STATE_INIT,
   /* Adding above AUDIO related states */

} APP_MPLAYER_STATE_T;

#ifdef DUAL_KEY // added by sangmin.seol 2014.02.26 DUAL_KEY Flag moved from appfreamework
enum KEY_CCP_STATUS_T{
    CCP_NONE = 0,
    CCP_CENTER = 1,
    CCP_UP = 2,
    CCP_DOWN = 4,
    CCP_LEFT = 8,
    CCP_RIGHT = 16,
    CCP_MENU = 32,
    CCP_BACK = 64
};
enum KEY_RRC_STATUS_T{
    RRC_NONE = 0,
    RRC_CENTER = 1,
    RRC_UP = 2,
    RRC_DOWN = 4,
    RRC_LEFT = 8,
    RRC_RIGHT = 16,
    RRC_MENU = 32,
    RRC_BACK = 64
};
enum KEY_ETC_STATUS_T{
    ETC_NONE = 0,
    ETC_FACE_REW = 1,
    ETC_FACE_FF = 2,
    ETC_FACE_TUNE_CENTER = 4,
    ETC_SWRC_REW = 8,
    ETC_SWRC_FF = 16,
    ETC_CCP_SEEK_REW = 32,
    ETC_CCP_SEEK_FF = 64,
    ETC_RRC_SEEK_REW = 128,
    ETC_RRC_SEEK_FF = 256,
    ETC_RRC_CH_BK = 512,
    ETC_RRC_CH_FW = 1024
};
#endif      // added by sangmin.seol 2014.02.26 DUAL_KEY Flag moved from appfreamework


class Event;
class QDeclarativeContext;

class AppMediaPlayer_EngineBase : public QDeclarativeItem
{
   Q_OBJECT
//   Q_PROPERTY(bool StateIsActive READ GetStateIsActive WRITE SetStateIsActive NOTIFY notifyStateIsActive)//Commented by Alexey Edelev CR14638
public:

    AppMediaPlayer_EngineBase() : QDeclarativeItem(NULL),
                                  m_EntryPoint(eAPP_NONE),
                                  m_EngineState( APP_MPLAYER_STATE_NONE ),
                                 // m_pEngineContext(NULL){}
//                                  m_StateIsActive(false),//Commented by Alexey Edelev CR14638
                                  m_pEngineFrontContext(NULL),
                                  m_pEngineRearContext(NULL) {}

    virtual ~AppMediaPlayer_EngineBase() {}

    virtual bool EventHandler( Event&, DISPLAY_T ) = 0;

    APP_MPLAYER_STATE_T GetState() const { return m_EngineState; }

    void SetState( APP_MPLAYER_STATE_T aState ) { m_EngineState = aState; }

    virtual void ShowUI(DISPLAY_T display) = 0;

    virtual void HideUI(DISPLAY_T display) = 0; // called when AVP gets EVT_REQUEST_BG for Engine

    /*  Called when other Engine overlaps this Engine (e.g. by EVT_AV_MODE_CHANGE).
     *  By EVT_REQUEST_BG also called.
	 */
    virtual void DeactivateUI(DISPLAY_T display) { Q_UNUSED(display); } // added by Sergey 10.05.2013

    virtual void SaveSettings(bool bSync = false) = 0; // modified by kihyung 2013.08.09 for ITS 0183077

    virtual void LoadSettings() = 0;

    virtual void Restore(MODE_STATE_T mode) = 0;

    virtual void PlayTrackFromFileManager( QString ){}

    virtual AppMediaPlayer_Controller_Base* GetController() = 0;

    virtual void setRescanProgressLoading() = 0; // modified by ravikanth 20-04-13

    virtual void factoryReset() {}; // added by Sergey 13.09.2013 for ITS#188762

    virtual void resetAll() {};	 // added by Sergey 13.09.2013 for ITS#188762

//{Commented by Alexey Edelev CR14638
//    bool GetStateIsActive() { return m_StateIsActive; }
//    void SetStateIsActive( bool value )
//    {
//       m_StateIsActive = value;
//       emit notifyStateIsActive();
//    }
//}Commented by Alexey Edelev CR14638

    void SetEntryPoint( APP_ENTRY_POINT_T value ) { m_EntryPoint = value; }
    APP_ENTRY_POINT_T getEntryPoint() { return m_EntryPoint; } // added by Dmitry 16.04.13

    /*
	QDeclarativeContext *GetContext() { return m_pEngineContext; }
    void SetContext(QDeclarativeContext *ctxt)
    {
       m_pEngineContext = ctxt;
       emit contextChanged();
    }
    */
    //added by edo.lee 2013.02.06
    
    QDeclarativeContext *GetFrontContext() { return m_pEngineFrontContext; }
	QDeclarativeContext *GetRearContext() { return m_pEngineRearContext; }
     void SetFrontContext(QDeclarativeContext *ctxt)
    {
       m_pEngineFrontContext = ctxt;
       emit contextChanged(true);
    }
    void SetRearContext(QDeclarativeContext *ctxt)
    {
       m_pEngineRearContext = ctxt;
       emit contextChanged(false);
    }

    Q_INVOKABLE void StatusBarPostEventSend( int nCMD, int nDispType )
    {
       emit statusBarPostEventSend( nCMD, nDispType );
    }
// { deleted by lssanh 2012.12.04 changed clear osd seq
// { added by lssanh 2012.10.25 status bar clear
//    Q_INVOKABLE void StatusBarClearOSD( int nDispType )
//    {
//       emit statusBarClearOSD( nDispType );
//    }
// } added by lssanh 2012.10.25 status bar clear
// } deleted by lssanh 2012.12.04 changed clear osd seq


signals:
//    void notifyStateIsActive();//Commented by Alexey Edelev CR14638
    void contextChanged(bool isFront);
    void statusBarPostEventSend( int nCMD, int nDispType );
// { deleted by lssanh 2012.12.04 changed clear osd seq
// { added by lssanh 2012.10.25 status bar clear	
//	void statusBarClearOSD( int nDispType );
// } added by lssanh 2012.10.25 status bar clear
// } deleted by lssanh 2012.12.04 changed clear osd seq

    void updateOSD( unsigned int nType );
    // { removed by junam 2012.11.28 for CR14466
    //void cancelUpdateOSD(bool cancelMode); //added by changjin 2012.10.22 for CR 14445
    // } removed by junam

	void setOSDMetaDataChanged(OSD_DEVICE , QString , int, int, int , bool ) ;
	void updatePositionChanged(OSD_DEVICE  , int, bool) ;
	void updatePlayingModeChanged(OSD_DEVICE  , int, int, int ); 
	void showCurrentOSDChanged();
	void updateOSDMetaDataChanged(OSD_DEVICE , QString );
    void updateCDDAOSDMetaDataChanged(OSD_DEVICE, QString); // added by wonseok.heo for ITS 197433 2013.10.28

protected:
    APP_ENTRY_POINT_T m_EntryPoint;

private:
    APP_MPLAYER_STATE_T m_EngineState;
//    bool m_StateIsActive;//Commented by Alexey Edelev CR14638
	//modified by edo.lee 2013.02.06
    //QDeclarativeContext *m_pEngineContext;
	QDeclarativeContext *m_pEngineFrontContext;
    QDeclarativeContext *m_pEngineRearContext;


};

Q_DECLARE_METATYPE(APP_MEDIA_MODE_T)
Q_DECLARE_METATYPE(OSD_DEVICE)

#endif
