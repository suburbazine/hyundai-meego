#ifndef DAB_APP_ENGINE_H
#define DAB_APP_ENGINE_H

#include "DHAVN_AppFramework_AppEngineQML_Def.h"
#include <QDeclarativeView>
#include <QTranslator>
#include <uistate_manager_def.h>

#ifdef __RPMCLIENT__
#include "DHAVN_QRPMClient.h"
#endif

#ifdef __ENG_MODE__
#include "EngModeIf.h"
#endif
#include "DABtoFMIf.h"
#ifdef __DTC__
#include "MICOMIf.h"
#include <QTimer>
#endif
#include <QFile>
#include <QTextStream>
#include <QStack>
#include <DHAVN_QCANController_ApplnTxRx.h>
#include <DHAVN_QCANController_ApplnTxRxStruct.h>
#include "DABController.h"
#include "DABUIListener.h"
#include "LocTrigger.h"
#include "DABLogger.h"
#include "DABProtocol.h"
#ifndef __DAB_DEBUG_MSG_OUTPUT_FILE__
#include <DHAVN_LogUtil.h>
#endif
#ifdef __DAB_TPEG__
#include "DABTpegService.h"
#include "DABTpegIf.h"
#endif

#ifdef __WDT__
#include <signal.h>
#include <bits/siginfo.h>
#endif
#include "DABRdsIf.h"
#include "osdwriter.h"

// constant which defines language command line parameter
const QString KLanguageId = QString("-l:");

enum eRpmClientType
{
    NO_DAB = 0,
    DAB1,
    DAB2,
    DAB3,
    DAB_AV_OFF
};

enum eDAB_CURRENT_STATUS
{
    DAB_FG_STATUS = 0,
    DAB_BG_STATUS,
    DAB_FG_FM_STATUS,
    DAB_BG_FM_STATUS,
    DAB_EXIT_STATUS
};

#ifdef __DTC__
enum eDTC_CMD_TYPE
{
    DTC_CMD_ANTENNA_OUT_BATTERY = 0x00,
    DTC_CMD_ANTENNA_OPEN        = 0x01,
    DTC_CMD_ANTENNA_SHORT       = 0x02,
    DTC_CMD_ANTENNA_NORMAL      = 0x03,
    DTC_CMD_MODULE_UART_ERROR   = 0x04,
    DTC_CMD_EXT_ANTENNA_OPEN    = 0x05
};

#define DTC_EXT_CHECK_COUNT         25
#define DTC_CHECK_COUNT             5
#define DTC_ERROR                 0x01
#define DTC_CLEAR                 0x00
#endif


#define SYSTEM_POPUP_STATUS_SIZE                        (sizeof(char)*4)
#define SYSTEM_OFFSET_CURRENT_AV_MODE                   (28)
#define SYSTEM_OFFSET_POPUP_STATUS                      (32)
#define SYSTEM_OFFSET_CALL_STATUS                       (40)
#define SYSTEM_OFFSET_TOTAL_SIZE                        (40)

#ifndef __DAB_DEBUG_MSG_OUTPUT_FILE__
#define qDebug()          qCritical()/*LOG_HIGH*/
#endif
// User Application must be based from AppEngineBase class (App Framework Lib), it allows to hide d-bus interactions with
// UI State Handler and process common behavior inside of App Framework Lib
class DABAppEngine: public AppEngineQML
{
#ifndef __DAB_DEBUG_MSG_OUTPUT_FILE__
    USE_LOG_UTIL
#endif
   Q_OBJECT

    //Image Source directory
    Q_PROPERTY(QString m_sImageRoot     READ getImageRootDir                            NOTIFY imageRootDirChanged)

public:

    DABAppEngine( QDeclarativeView &aQMLView, LANGUAGE_T nLanguage, QObject *pParent = NULL );
    ~DABAppEngine();

    Q_INVOKABLE void pushScreen(const QString &screenName) { mFlowStack.push(screenName); }
    Q_INVOKABLE QString popScreen();
    Q_INVOKABLE int countScreen() {  return mFlowStack.count(); }
    Q_INVOKABLE void HandleBackKey(QString sInputMode);
    Q_INVOKABLE void HandleHomeKey();
    Q_INVOKABLE void HandleFMKey();
    Q_INVOKABLE void HandleAMKey();
    Q_INVOKABLE void playAudioBeep(void);
    Q_INVOKABLE void setBlockedStation(bool onOff);
#ifdef __AUTO_TEST__
    Q_INVOKABLE void autoTest_athenaSendObject(void);
#endif

    QStack<QString> mFlowStack;
    Q_PROPERTY(QString m_APP_VERSION    READ getAPP_VERSION     NOTIFY appVersionChanged);
    Q_PROPERTY(QString m_BUILDDATE      READ getBUILDDATE       NOTIFY buildDateChanged);

    void setTranslator(QTranslator *translator) { m_pTranslator = translator; }
    void setStatusBarTranslator(QTranslator *translator) { m_pStatusBarTranslator = translator; }
    void RetranslateUi(void);
    QString getAPP_VERSION(void) {return m_APP_VERSION; };
    QString getBUILDDATE(void) {return m_BUILDDATE; };

    // Image Root
    QString getImageRootDir(void){ return this->dabController.getImageRootDir(); }
    void setImageRootDir(QString dir) { this->dabController.setImageRootDir(dir); }

    void Log( const QString &aMessage);
#ifdef __WDT__
    static void signalHandler(int signum);
    void signalConnection();
    static void setSignalHandlerObject(DABAppEngine* m);
    void restart(int sig);
#endif

    bool isEnableSystemPopup(void);
    int checkAudioPath(void);
    void checkEPGPopupVisible(void);

    bool eventFilter(QObject *pFromObj, QEvent *pEvent);
    void setConnectUISH(void);
    void setInitLanguage(int languageID);

signals:
    void retranslateUi(int languageId);
    void cmdReqSeekPrevFromUISH(void);
    void cmdReqSeekNextFromUISH(void);
    void cmdReqLongSeekPrevFromUISH(void);
    void cmdReqLongSeekNextFromUISH(void);
    void cmdReqSaveData(void);
    void cmdSetCurrentInfoToHomeMedia(void);
    void cmdSendClusterUpdate(void);
    void cmdSendOSDUpdate(void);
    void cmdReqTuneLeft(void);
    void cmdReqTuneRight(void);
    void cmdReqTunePressed(void);
    void cmdFactoryResetRemoveData(QString path);

    // Image Root Dir
    void imageRootDirChanged();
    void cmdReqTrafficDABtoRDS(bool onOff);
    void changeDABtoFM(void);
    void reqForegroundFromUISH(bool bTemporalMode);
    void reqStationListFromVR();
    void cmdLanguageChange(int languageID);
#ifdef __VR_COMMAND__
    void cmdReqPresetSelected(int index);
#endif
    void cmdReqSeekNextPresetList(void);
    void cmdReqSeekPrevPresetList(void);
    void appVersionChanged(void);
    void buildDateChanged(void);
    void drivingRegulation(bool onOff);
    void cmdEventExceptionHandler(eDAB_EVENT_TYPE event);

    //Cluster
    void Can_filter_msg_id(const SQCanIdList &c_data);
    void Can_filter_bcm_msg(const SQBCMHead &c_data);
    void Can_data_send(const SQCanFrame &c_data);
    void Can_data_send_bcm(const SQBCMHead &c_bcm_data,const SQCanFrameList &c_can_data);

    void cmdReqTA(bool bDABAudioOpen);
    void cmdReqTASetting(bool onOff);
    void cmdReqCancelTrafficAnnouncement(bool onOff);
    void cmdReqCancelAlarmAnnouncement(void);
    void sendSeekToEPGList(void);
    void cmdRspTARDStoDAB(bool onOff);
    void cmdReqSetReservationChannel(void);
    void cmdReqTASearch(void);
    void cmdCancelEPGReservation(bool isSystemPopup);
    void gotoMainScreen(void);
    void displayTAPopup(QString serviceName, quint16 flags, bool onOff);
    void cmdReqSetAntennaStatus(quint8 status);
    void closeEPGDescPopup(void);
    void closeEPGFullPopup(void);
    void closeEPGDateListPopup(void);
    void dabEngineerMode(void);
    void dabDealerMode(void);

public slots:
    virtual void NotifyFromUIStateManager( Event aEvent );
    virtual void NotifyFromUIStateManagerRear( Event aEvent );
    virtual void NotifyFromUIStateManagerCommon ( Event aEvent );
    void onReqClusterControl(QString serviceName, quint8 presetNumber, quint8 opState);
    void onReqDisplayOSD(QString str1, QString str2, quint8 op_type);
    void onReqDisplayStatusBar(quint8 type, bool onOff);

#ifdef __RPMCLIENT__
    void audioCHsetResponse(TAudioMode eBAND, TAudioSink tSINK);
#endif
    void onReqDABSoundOn();
    void onLgeDBus_DABPICodeRequest(quint16 playPICode);
#ifdef __ENG_MODE__
    void onReqVersion(QString ver);
    void onReqConnectivity(int in0);
    void onReqDiagnosis();
#endif
#ifdef __DTC__
    void onInterAntenaCheck(void);
    void onExterAntenaCheck(void);
    void sendDTCMessage(quint8 command, quint8 error);
    void onEndExternalDTC(void);
    void onUartModuleCheck(void);
    void initAntennaCount(void);
    void onRspTrafficRDStoDAB(bool onOff);
#endif    
    void onReqTAStart(quint16 flag, QString serviceName);
    void onReqTAStop(void);
    void onReqAlarmStart(quint16 flag, QString serviceName);
    void onReqAlarmStop(void);
    void onReqRadioEnd(int band, quint16 playPICode);
    void onReqDABtoFM(quint16 playPICode);
    void onReqEPGPreservePopup(QString title, QString serviceName);
    void onSoundSetting(bool isjogMode);
    void closeSystemPopup(eDAB_SYSTEM_POPUP_TYPE m_ePopupType);
    bool getBlockedStation(void) { return this->m_bBlockStation; }

    void startDABtoFMTimer(void);
    void stopDABtoFMTimer(void);

    void onReqTASearchPopup(bool onOff);
    void onReqSendMuteCancel(void);  //# Mute Cancel

private slots:
    void doNotifyUISHFront( Event aEvent );
    void doNotifyUISHRear( Event aEvent );
    void onOSDUpdate(void);
    void onDABAudioOpen(void);

private:
    virtual APP_ID_T GetThisAppId() const { return APP_DAB; }
    void NotifyFromUIStateManagerCommonGeneric ( Event aEvent );
    void NotifyFromUIStateManagerGeneric( Event aEvent, DISPLAY_T display );
    LANGUAGE_T m_nLanguage;
    QTranslator *m_pTranslator;
    QTranslator *m_pStatusBarTranslator;
    void CreateQRPMClient(void);
    void Init(void);
    void SetPropertyToQML(void);
    void SetQMLSource(void);
    void ConnectionWithQML(void);
    void Start(void);

private:
#ifdef __RPMCLIENT__
    QRPMClient* m_pRpmClient;
#endif
#ifdef __ENG_MODE__
    EngModeIf* m_engClient ;
#endif
#ifdef __DTC__
    MICOMIf* m_MicomClient;
    DABtoFMIf* m_DABtoFMClient;
    QTimer* m_DABtoFMTimer;
    QTimer* m_interAntennaTimer;
    QTimer* m_exterAntennaTimer;
    QTimer* m_UARTCheckTimer;
    int m_iAntennaShortCount;
    int m_iAntennaOpenCount;
    int m_iAntennaNormalCount;
    int m_iAntennaOutBatteryCount;
    int m_iUartGoodCount;
    int m_iUartErrorCount;
    bool m_bFirstShort;
    bool m_bFirstOpen;
    bool m_bUartCheck;
    quint8 m_DtcSetState;
#endif
    CQCANController_ApplnTxRx* m_pQCanController;
    QString m_APP_VERSION;
    QString m_BUILDDATE;
    QString m_sTAType;
    QString m_sTAServiceName;

    void PostKeyEvent(KEY_EVENT_DATA_T *message, Qt::Key key);
    void PostJogEvent(KEY_EVENT_DATA_T *message, EVT_COMMAND_T keyCount);
    void PrepareLogFile(QObject *pParent);
    bool m_bDABAudioOpen;
//    bool m_bIsLongSeekPressed;
    DABController dabController;
    LocTrigger locTrigger;
    eDAB_CURRENT_STATUS m_eDabCurrentStatus;
    eDAB_SYSTEM_POPUP_TYPE m_eDabSystemPopupType;
    int m_iDisplayMode;
    bool m_bCallFlag;
    bool m_bVRFlag;
    bool m_bMixFlag;
    bool m_bBlockStation;
    quint16 m_AnnouncementFlags;
    QString m_sAnnouncementServiceName;
    bool m_bAgreeShowOSD;
    bool m_bDABForeground;
    bool m_bStationListFromVR;
    QString m_sBackupOSD;
    bool m_SystemShowingPopup;
    bool m_bPowerLogigOff;
    bool m_bFactoryReset;
    bool m_bPopupNone;
    bool m_bCCPLongPressed;
    bool m_bSeekLongPressed;
#ifdef __DAB_TPEG__
    DABTpegService *m_tpegDataService;
#endif
    DABRdsAdaptor *m_pRdsAdaptor;

    QSharedMemory m_xSystemPopupSharedMem;
    QSharedMemory m_xCurrentAVModeSharedMem;
};

#endif //DAB_APP_ENGINE_H
