#ifndef DHAVN_APPUSERMANUAL_ENGINE_H
#define DHAVN_APPUSERMANUAL_ENGINE_H

#include <QDeclarativeView>
#include <QGraphicsObject>
#include <QTranslator>
#include <QDeclarativeContext>
#include <uistate_manager_def.h>
#include <DHAVN_AppFramework_AppVextEngineQML_Def.h>
#include <DHAVN_AppUserManual_Log_Def.h>
#include <DHAVN_OptionMenuModel.h>
#include <signal.h>
#include <bits/siginfo.h>
#include <QtDBus/QtDBus>
#include <athena.h>

class APP_ENTRY_POINT_DATA_T;

class AppUserManual_Engine : public AppVextEngineQML
{
    Q_OBJECT
    Q_PROPERTY(OptionMenuModel* optionMenuModel READ getOptionMenuModel NOTIFY changeOptionMenuModel)

public:
    AppUserManual_Engine( int &argc, char **argv, LANGUAGE_T aLanguage, QTranslator& aTranslator, int evv , QObject* aParent = NULL );
    virtual ~AppUserManual_Engine();

    /* Beep */
    QDBusMessage msg;

    Q_INVOKABLE int CheckVehicleStatus();
    Q_INVOKABLE void HandleSoftKeyHome();
    Q_INVOKABLE void HandleBackKey( int screenId, bool touch , bool front );
    Q_INVOKABLE void onSetOptionMenu(int id_State);
    Q_INVOKABLE void setFullScreen( bool fullScreen, int screenId );
    Q_INVOKABLE void foregroundEventReceived( int screenId );
    Q_INVOKABLE void launchKeypadSettings( int screenId );
    Q_INVOKABLE void sendAthenaObject( );
    Q_INVOKABLE int getCountyVariant( );
    Q_INVOKABLE int getLanguage( );
    Q_INVOKABLE void playAudioBeep();
    Q_INVOKABLE void setViewport(int screenId , bool full );
    Q_INVOKABLE bool getDRSStatus();
    Q_INVOKABLE void saveState( QString state ,  int curPage , int totalPages , int screen );
    Q_INVOKABLE void saveTempState( QString state );
    Q_INVOKABLE void savePage( int curPage );
    Q_INVOKABLE int getPage();
    Q_INVOKABLE int getTotalPage();
    Q_INVOKABLE QString getState();
    Q_INVOKABLE QString getTempState();
    Q_INVOKABLE void ChangeEntry();
    Q_INVOKABLE void sendChineseKeypad(int);
    Q_INVOKABLE void setChineseKeypadType(int);
    Q_INVOKABLE int getChineseKeypadType();
    Q_INVOKABLE int getHWType();
    Q_INVOKABLE void logForQML( QString );

    void RetranslateUI();
    void showDRS(DISPLAY_T display);
    void hideDRS(DISPLAY_T display);
    OptionMenuModel* getOptionMenuModel() { return m_OptionMenuModel; }

    static void signalHandler( int sigNum );
    void signalConnection();
    static void setSignalHandlerObject( AppUserManual_Engine* m_appUserManualEngine );
    static AppUserManual_Engine* SigM;
    void restart( int sigNum );

public slots:
    virtual void NotifyFromUIStateManager( Event aEvent );
    virtual void NotifyFromUIStateManagerRear( Event aEvent );
    virtual void NotifyFromUIStateManagerCommon( Event aEvent );

signals:
    void setDrs( bool status );
    void jog8Dir( int dir );
    void retranslateUi( int lang);
    void changeOptionMenuModel();
    void foregroundEvent( int screenId );
    void showDisablePopup( bool visible );
    void setDefaultState(int screenId );
    void setSearchView(int screenId );
    void backgroundEvent( int screenId );
    void btManual( int screenId );
    void vrManual( int screenId );
    void btDRSManual( int screenId );
    void vrDRSManual( int screenId );
    void hideDRSBtVrManual( int screenId );
    void setVrStatePage();
    void temporalMode( int screenId , QString saveState, QString saveTempState );
    void temporalSettingMode( int screenId , QString saveState, QString saveTempState );
    void backkeyPressed( int screenId , bool front );
    void menukeyPressed(int screenId );
    void prevkeyPressed( int screenId );
    void nextkeyPressed( int screenId );
    void showFullScreen();
    void updateKeypad(int screenId );
    void setTouchLock( bool lock );
    void pressedBackHK(); // hyeonil.shin [ITS DH_CHN 229944]

private:
    virtual APP_ID_T GetThisAppId() const { return APP_E_MANUAL; }
    void setOptionsMenuModel(OptionMenuModel* optionMenuModel);
    void NotifyFromUIStateManagerGeneric( Event aEvent, int screenId );
    void SetLocalization( LANGUAGE_T languageId );

private:
    int m_nVehicle;
    QTranslator& m_pTranslator;
    LANGUAGE_T m_nLanguage;
    OptionMenuModel* m_OptionMenuModel;
    DISPLAY_T m_nDisplayId;
    ECountryVariant m_nCountryVariant;
    bool m_nDRS;
    bool m_bEmergency;
    bool m_isCloneMode;
    bool m_isSwapDC;
    QString m_strState;
    QString m_strTempState;
    QString m_strOldState;
    QString m_strRearState;
    QString m_strRearTempState;
    QString m_strRearOldState;
    int m_nChineseKeypadType;
    int m_nPage;
    int m_nTotalPage;
    int m_nEntryPoint;
    int m_nOldEntryPoint;
    int m_nRearEntryPoint;
    int m_nRearOldEntryPoint;
    bool m_bSendChangeEntry;
    bool m_bTouchLock;
    int m_iEvv;
};

class LocalizeTrigger : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString empty READ empty NOTIFY retrigger)

public:
    QString empty() { return QString(); }

signals:
    void retrigger();
};

#endif // DHAVN_APPUSERMANUAL_ENGINE_H
