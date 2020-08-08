#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SCREENFULL_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SCREENFULL_H

#include <QDeclarativeItem>
#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
//#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerFS.h" //removed by Sergey for CR#16015
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_Base.h" // added by wspark 2012.11.14 for ISV 62589


class AppMediaPlayer_Video_Controller;
class AppMediaPlayer_EngineVideo;
// { added by wspark 2012.11.14 for ISV 62589
class AppMediaPlayer_Video_ControllerVCD;
class AppMediaPlayer_Video_CModel_VCD;
// } added by wspark

class AppMediaPlayer_Video_ScreenFull : public AppMediaPlayer_Video_Screen
{
   Q_OBJECT
public:
   AppMediaPlayer_Video_ScreenFull ( AppMediaPlayer_Video_Controller *controller,
                                       AppMediaPlayer_EngineVideo *pEngineVideo,
                                       int disp,
                                       QObject *parent = NULL );

   virtual ~AppMediaPlayer_Video_ScreenFull();

   virtual QString viewName() { return AppMediaPlayer_Video_Screens::VIEW_PLAYBACK; }

   Q_INVOKABLE bool isFullScreenMode() const {return  m_fullScreenState;}  // added by cychoi 2013.06.10 for ITS 172874 popup dimming

// { modified by edo.lee 2013.08.10 ISV 88688
Q_SIGNALS:  
   void showLockout(bool onShow);
// } modified by edo.lee 2013.08.10 ISV 88688

public slots:
   virtual void onHide();
   virtual void onShow( QVariant arg );
   virtual void onBackKeyPressed();
   virtual void onMenuKeyPressed();
   void onSetTuneMode(bool on);
   virtual void onDeactivate();
   virtual void onEventReceived(const QString &srcScreenName, const QString &resScreenName,
                                const int eventId, const QVariant arg); // added by Sergey 04.08.2013 for ITS#179312
   void onLockoutMode(bool on);// added by edo.lee 2013.08.10 ISV 88688

   Q_INVOKABLE void onMouseReleased();//added by edo.lee 2013.10.10
   Q_INVOKABLE void onMouseClicked();
   Q_INVOKABLE void onNextBtn();
   Q_INVOKABLE void onPrevBtn();
   Q_INVOKABLE void onFlick_next();
   Q_INVOKABLE void onFlick_prev();
   // { added by Sergey 20.04.2013
   Q_INVOKABLE void onPrevBtnHold();
   Q_INVOKABLE void onPrevBtnHoldCritical();
   Q_INVOKABLE void onNextBtnHold();
   Q_INVOKABLE void onNextBtnHoldCritical();
   Q_INVOKABLE void onCancel_ff_rew();
   Q_INVOKABLE void onLongPressPlayBtn(); // added by Dmitry 26.05.13
   Q_INVOKABLE void onTogglePlayBtn(); // added by Dmitry 26.05.13
   Q_INVOKABLE void onSelectTuned();
   // } added by Sergey 20.04.2013

//   void onFSPlaybackError(AppMediaPlayer_Video_ControllerFS::FSERROR_TYPE_T); //removed by Sergey for CR#16015
   void onIsPbcMenuDisplayedChanged( bool value ); // added by wspark 2012.11.14 for ISV 62589
   void onFMCopyCompleted(); // modified by ravikanth 12-07-13 for ITS 0174301

private:
   AppMediaPlayer_EngineVideo *m_pEngineVideo;
   bool                        m_bTuneMode;
   AppMediaPlayer_Video_Controller *m_pControllerVideo;
   // { removed by Sergey for CR#16015
//   AppMediaPlayer_Video_ControllerFS *m_pControllerFS_USB1;
//   AppMediaPlayer_Video_ControllerFS *m_pControllerFS_USB2;
//   AppMediaPlayer_Video_ControllerFS *m_pControllerFS_JUKEBOX;
   // } removed by Sergey for CR#16015.

   // { added by wspark 2012.11.14 for ISV 62589
   AppMediaPlayer_Video_ControllerVCD *m_pControllerVCD;
   AppMediaPlayer_Video_CModel_VCD *m_pControllerModel;
   // } added by wspark
   int m_disp;
   int m_fullScreenState; // added by cychoi 2013.06.10 for ITS 172874 popup dimming
   bool m_isLockout;// added by edo.lee 2013.08.10 ISV 88688
};


#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SCREENFULL_H
