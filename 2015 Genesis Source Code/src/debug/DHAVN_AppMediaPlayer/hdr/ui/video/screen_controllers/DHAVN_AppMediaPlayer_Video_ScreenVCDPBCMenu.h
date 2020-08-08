#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SCREENVCDPBCMENU_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SCREENVCDPBCMENU_H


#include "ui/video/screen_controllers/DHAVN_AppMediaPlayer_Video_Screen.h"
#include "ui/video/DHAVN_AppMediaPlayer_Video_Screens.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_Base.h"
#include "engine/DHAVN_AppMediaPlayer_EngineVideo.h"

class AppMediaPlayer_Video_Controller;
class AppMediaPlayer_Video_ControllerVCD;
class AppMediaPlayer_Video_CModel_VCD;
class AppMediaPlayer_Video_SModel_PbcMenu;

enum PBC_NUMPAD_BTN
{
   KEY_1 = 0,
   KEY_2,
   KEY_3,
   KEY_4,
   KEY_5,
   KEY_6,
   KEY_7,
   KEY_8,
   KEY_9,
// { modified by cychoi 2013.07.07 for New UX
   KEY_OK,
   KEY_0,
   KEY_DEL,
   KEY_PREV,
   KEY_RETURN,
   KEY_NEXT,
// } modified by cychoi 2013.07.07
};

class AppMediaPlayer_Video_ScreenVCDPBCMenu : public AppMediaPlayer_Video_Screen
{
    Q_OBJECT
public:
    AppMediaPlayer_Video_ScreenVCDPBCMenu( AppMediaPlayer_Video_Controller *controller,
                                           AppMediaPlayer_EngineVideo *pEngineVideo,
                                           int disp,
                                           QObject *parent = NULL );
    virtual ~AppMediaPlayer_Video_ScreenVCDPBCMenu();

    virtual QString viewName() { return AppMediaPlayer_Video_Screens::VIEW_VCD_PBCMENU; }

signals:
    void updateModeArea(int tab, bool isShow);
//added by edo.lee 2013.03.21
Q_SIGNALS:
	void showLockout(bool onShow);
    void setPBCMenuUI(bool onTempMode); // added by cychoi 2013.12.19 for ITS 215825 Default focus

public slots:
    virtual void onHide();
    virtual void onShow( QVariant arg );
    virtual void onDeactivate(); // added by cychoi 2013.07.03 for ITS 177711 

    Q_INVOKABLE void onKeyEntered( int index );
    Q_INVOKABLE void onEnterKey();
    Q_INVOKABLE void onReturnKey(); // modified by cychoi 2013.07.07 for New UX
    Q_INVOKABLE void onSoftkeyBack();
    Q_INVOKABLE void onPbcOff();
    Q_INVOKABLE void setDefaultInputText(); // added by cychoi 2013.12.16 ITS 215825
    //Q_INVOKABLE int getCurrentInputTextCount(); // added by cychoi 2014.01.23 for ITS 222055
    //Q_INVOKABLE bool isInputAvailable(); // added by cychoi 2014.01.23 for ITS 222055

    void onIsPbcMenuDisplayedChanged( bool value );

	void onLockoutMode(bool on); //added by edo.lee 2013.03.21

    void onBackKeyPressed(); // added by wspark 2013.04.18 for ISV 70606
    
    void setupModeArea(); // modified by cychoi 2013.07.07 for New UX
    void onRetranslateUI(); // added by yungi 2013.11.22 for ITS 210298

private:
    AppMediaPlayer_Video_Controller *m_pController;
    AppMediaPlayer_Video_ControllerVCD *m_pControllerVCD;
    AppMediaPlayer_Video_CModel_VCD *m_pControllerModel;
    AppMediaPlayer_Video_SModel_PbcMenu *m_pModel;
    AppMediaPlayer_EngineVideo *m_pEngineVideo;
	bool m_isLockout; //added by edo.lee 2013.03.21
    bool m_bInitText; // added by cychoi 2013.07.07 for New UX

	int m_disp;
};


#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SCREENVCDPBCMENU_H
