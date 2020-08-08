#include <DABAppEngine.h>
#include <DHAVN_AppFramework_Event_Def.h>
#include <DHAVN_AppFramework_Logger_Def.h>
#include <DHAVN_AppFramework_Popup.h>
#include <uistate_manager_def.h>
#include <QApplication>
#include <QGraphicsObject>
#include <QtDebug>
#include <QDebug>
#include <QKeyEvent>
#include <QDeclarativeContext>
#include <QHostInfo>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QTime>
#include <QDeclarativeProperty>

#ifdef __RPMCLIENT__
#include <DHAVN_QRPMClient.h>
#endif

//#include "DHAVN_PopUp_Import.h"
#include "DABApplication_Def.h"

#ifdef __AUTO_TEST__
#include <athena.h>
#endif
#ifdef __ENG_MODE__
#include <DHAVN_gpioCtl.h>
#endif
#ifdef __DAB_TPEG__
#include "DABTpegIf.h"
#endif

#ifdef __WDT__
static DABAppEngine* SigM = NULL;
#endif

DABAppEngine::DABAppEngine( QDeclarativeView &aQMLView, LANGUAGE_T nLanguage, QObject *pParent )
: AppEngineQML( &aQMLView, "AppDAB", pParent )
, m_pTranslator( NULL )
, m_nLanguage(nLanguage)
, dabController(pParent)
, m_eDabCurrentStatus(DAB_BG_STATUS)
{
//    QTime startTime;
//    QTime endTime;
//    startTime = QTime::currentTime();
//    startTime.start();
    // This function uses virtual GetThisAppId() = 0,
    // so ConnectToUIStateManager() could be used only in derived class
    //GetLogger().SetLogType(Logger::LOG_FILE, "/home/meego/AppDAB1.log");
    //Log("== DABAppEngine::DABAppEngine ConnectToUIStateManager");
    Log("[DABAppEngine] start ");
#ifndef __DAB_DEBUG_MSG_OUTPUT_FILE__
    LOG_INIT_V2("AppDAB");
#endif
    this->CreateQRPMClient();
    this->Init();
    this->SetPropertyToQML();
    this->SetQMLSource();
    this->ConnectionWithQML();
    this->Start();
    //qDebug("============ TimeCheck Start: %2.3F", (double)startTime.elapsed()/1000);
#ifdef __DAB_DABUG_INFO__
    QList<QHostAddress> hosts = QNetworkInterface::allAddresses ();
    QHostAddress addr;
    for (int i = 0; i < hosts.count(); ++i)
    {
        addr = hosts.at(i);
        if(addr.toString().startsWith("10.177.")){
            qDebug() << "IP :" << addr.toString();
            dabController.getUIListener()->setIPAddress(addr.toString());
        }
    }
#endif

    // Check Session Bus...
    if (!QDBusConnection::sessionBus().isConnected()) {
        qDebug() << qPrintable(QDBusConnection::sessionBus().lastError().message());
    }
    QDBusConnection bus = QDBusConnection::sessionBus();

#ifdef __ENG_MODE__
    m_engClient = new EngModeIf("com.lge.car.eng.service", "/", bus, pParent);
    QObject::connect(m_engClient, SIGNAL(reqVersion(int)), &dabController, SLOT(onReqVersion(int)) );
    QObject::connect(&dabController, SIGNAL(cmdReqVersion(QString)), this, SLOT(onReqVersion(QString)) );

    QObject::connect(m_engClient, SIGNAL(reqConnectivity(int)), this, SLOT(onReqConnectivity(int)) );
    QObject::connect(m_engClient, SIGNAL(reqDiagnosis()), this, SLOT(onReqDiagnosis()) );
#endif

#ifdef __DTC__
    m_MicomClient = new MICOMIf("com.lge.car.micom", "/Dtc", bus, pParent);

    // DAB Antenna DTC
    m_interAntennaTimer = new QTimer(this);
    QObject::connect(m_interAntennaTimer, SIGNAL(timeout()), this, SLOT(onInterAntenaCheck()));
    m_interAntennaTimer->start(200);

    // UART Module DTC
    m_UARTCheckTimer = new QTimer(this);
    QObject::connect(m_UARTCheckTimer, SIGNAL(timeout()), this, SLOT(onUartModuleCheck()));
    m_UARTCheckTimer->start(2000);

    m_exterAntennaTimer = new QTimer(this);
    QObject::connect(m_exterAntennaTimer, SIGNAL(timeout()), this, SLOT(onExterAntenaCheck()));
#endif

    m_DABtoFMClient = new DABtoFMIf("com.lge.car.micom", "/Dab", bus, pParent);
    QObject::connect(m_DABtoFMClient, SIGNAL(LgeDBus_EventDABPISensitivity(const QByteArray &)), &dabController, SLOT(onReceiveFMSensitivity(const QByteArray &)) );
    QObject::connect(&dabController, SIGNAL(cmdLgeDBus_DABPICodeRequest(quint16)), this, SLOT(onLgeDBus_DABPICodeRequest(quint16)) );

    m_DABtoFMTimer = new QTimer(this);
    QObject::connect(m_DABtoFMTimer, SIGNAL(timeout()), &dabController, SLOT(sendPICodeToMicom()));

#ifdef __DAB_TPEG__
    m_tpegDataService = new DABTpegService(pParent);
    new DABTpegIf(m_tpegDataService);

    bool ret = bus.registerService("com.lge.car.dhavn.Dmb");
    ret = bus.registerObject("/DmbTpegService", m_tpegDataService);
    if(!ret)
        Log("Cannot register Object to the D-Bus session bus.\n Please check your system settings and try again.\n");
    else
        Log("TPEG D-Bus registerService Success !!");

    QObject::connect(&dabController, SIGNAL(cmdSendTPEGData(const QByteArray &, int)), m_tpegDataService, SLOT(onSendTPEGData(const QByteArray &, int)));
#endif

    m_pRdsAdaptor = new DABRdsAdaptor(pParent);
    new DABRdsIf(m_pRdsAdaptor);

    ret = bus.registerService("com.lge.car.dab.rds");
    ret = bus.registerObject("/DABtoRDS", m_pRdsAdaptor);
    if(!ret)
        Log("Cannot register Object to the D-Bus session bus.\n Please check your system settings and try again.\n");
    else
        Log("RDS Sync D-Bus registerService Success !!");

    QObject::connect(m_pRdsAdaptor,  SIGNAL(cmdReqSetAnnounement(bool)),                &dabController,     SLOT(onReqSetAnnounement(bool)));
    QObject::connect(m_pRdsAdaptor,  SIGNAL(cmdTrafficRDStoDAB(bool)),                  this,               SLOT(onRspTrafficRDStoDAB(bool)));
    QObject::connect(m_pRdsAdaptor,  SIGNAL(cmdReqDisplayStatusBar(quint8,bool)),       this,               SLOT(onReqDisplayStatusBar(quint8,bool)));
    QObject::connect(&dabController, SIGNAL(cmdReqTrafficDABtoRDS(bool)),               m_pRdsAdaptor,      SLOT(onReqTrafficDABtoRDS(bool)));

    m_pRdsAdaptor->init();
    m_pQCanController = new CQCANController_ApplnTxRx(this);
    if (m_pQCanController)
    {
        m_pQCanController->Init();
    }

    Log("[DABAppEngine] Successfully Started... ");

    m_xSystemPopupSharedMem.setKey("UISHStatusInformation");
    m_xSystemPopupSharedMem.attach();
    m_xSystemPopupSharedMem.create(SYSTEM_OFFSET_TOTAL_SIZE + SYSTEM_POPUP_STATUS_SIZE);

    m_xCurrentAVModeSharedMem.setKey("UISHStatusInformation");
    m_xCurrentAVModeSharedMem.attach();
    m_xCurrentAVModeSharedMem.create(SYSTEM_OFFSET_CURRENT_AV_MODE + SYSTEM_POPUP_STATUS_SIZE);

    SetAppHandlesResponseFg(false);

#ifdef __AUTO_TEST__
    int result = athenaOpen(1); // 1: auto test on, 0: auto test off
    Log(QString("athenaOpen result : %1").arg(result));
#endif

#ifdef __WDT__
    signalConnection();
    setSignalHandlerObject(this);
#endif

    QTimer::singleShot(1000, m_pRdsAdaptor, SLOT(onCheckRDSLanuch()));
//    endTime = QTime::currentTime();
//    qDebug() << " Start Time:" << startTime.toString();
//    qDebug() << "   End Time:" << endTime.toString();
//    qDebug(" Start Up Duration: %2.3F", (double)startTime.elapsed()/1000);
}

DABAppEngine::~DABAppEngine()
{
#ifdef __RPMCLIENT__
    if ( m_pRpmClient != NULL )
    {
       delete m_pRpmClient;
    }
#endif

#ifdef __AUTO_TEST__
    athenaOpen(0); // 1: auto test on, 0: auto test off
#endif
}

void
DABAppEngine::CreateQRPMClient(void)
{
#ifdef __RPMCLIENT__
    Log("[DABAppEngine] CreateQRPMClient ");
    m_pRpmClient = NULL;
    Log(QString("QPRM Client new qApp->applicationName(): %1").arg(qApp->applicationName()));
    m_pRpmClient = new QRPMClient(&(qApp->applicationName()));
    Log(QString("    ===> m_pRpmClient: ").arg(m_pRpmClient->objectName()));
    connect(m_pRpmClient,SIGNAL(RPMClientAcquireResponse(TAudioMode, TAudioSink)),this,SLOT(audioCHsetResponse(TAudioMode, TAudioSink)));
    Log(" Connect..");
#endif
}

void
DABAppEngine::Init(void)
{
    this->setImageRootDir(DAB_IMAGE_ROOT);

//    m_bIsLongSeekPressed = false;
    m_bDABAudioOpen = false;
    m_APP_VERSION = (QString)APP_VERSION;
    m_BUILDDATE = (QString)BUILDDATE;
    m_sTAType = "";
    m_sTAServiceName = "";
    m_iDisplayMode = 0;
    m_bCallFlag = false;
    m_bVRFlag = false;
    m_bMixFlag = false;
    m_bAgreeShowOSD = false;
    m_sBackupOSD = "";
    m_AnnouncementFlags = ANN_ALARM_ON;
    m_sAnnouncementServiceName = "";
    m_bBlockStation = false;
    m_bStationListFromVR = false;
#ifdef __DTC__
    m_iAntennaShortCount = DTC_CHECK_COUNT;
    m_iAntennaOpenCount = DTC_CHECK_COUNT;
    m_iAntennaNormalCount = DTC_CHECK_COUNT;
    m_iAntennaOutBatteryCount = DTC_CHECK_COUNT;
    m_iUartGoodCount = DTC_CHECK_COUNT;
    m_iUartErrorCount = DTC_CHECK_COUNT;
    m_DtcSetState = 0x00;
    m_bDABForeground = false;
    m_bFirstShort = true;
    m_bFirstOpen = true;
    m_bUartCheck = true;
    m_SystemShowingPopup = false;
    m_bPowerLogigOff = false;
    m_bFactoryReset = false;
    m_bPopupNone = false;
    m_bCCPLongPressed = false;
    m_bSeekLongPressed = false;
#endif
    m_eDabSystemPopupType = DAB_POPUP_NONE;
    emit appVersionChanged();
    emit buildDateChanged();
}
void
DABAppEngine::SetPropertyToQML(void)
{
    QDeclarativeContext *ctxt = this->GetCurrentView()->rootContext();

    ctxt->setContextProperty("UIListener",          this );   
    ctxt->setContextProperty("LocTrigger",          &locTrigger );
    ctxt->setContextProperty("DABController",       &dabController );
    ctxt->setContextProperty("DABListener",         dabController.getUIListener());
    ctxt->setContextProperty("DABChannelManager",   dabController.getUIListener()->getChannelManager());
}

void
DABAppEngine::SetQMLSource(void)
{
    QStringList fontListB;
    fontListB << "NewHDB_CN" << "Amiri" << "DFHeiW5-A";
    QFont::insertSubstitutions(QString("DH_HDB"),fontListB);

    QStringList fontListR;
    fontListR << "NewHDR_CN" << "Amiri" << "DFHeiW5-A";
    QFont::insertSubstitutions(QString("DH_HDR"),fontListR);

    QDeclarativeView* pView = this->GetCurrentView();

#ifdef __UNIX_DESKTOP__
    setImageRootDir(DAB_IMAGE_ROOT);
    pView->setSource( QUrl( DAB_DEFAULT_SOURCE ) );
#else
    Log(QString("   loading: ") + DAB_DEFAULT_SOURCE);
    pView->setSource( QUrl( DAB_DEFAULT_SOURCE ) );
    Log(QString("   setSource: %1").arg(pView->status()));
    if(pView->status() == QDeclarativeView::Ready){
        Log("   pView->status(): Ready");
    }else if(pView->status() == QDeclarativeView::Error){
        Log("   pView->status(): Error");
        setImageRootDir(DAB_IMAGE_ROOT);
        Log("   loading: ../AppDAB/qml/DHDABMain.qml");
        pView->setSource( QUrl( "../PlanB/subapp/DHDABMain.qml" ) );
        Log(QString("     status: %1").arg(pView->status()));
    }
#endif
}

void
DABAppEngine::ConnectionWithQML(void)
{
    Log("[DABAppEngine] ConnectionWithQML - ");
    QObject *rootObject = (QObject*)(this->GetCurrentView()->rootObject());

    // ==================================================  AppEngine.cpp -> DABController.cpp
    QObject::connect(this,       SIGNAL(cmdReqSeekNextFromUISH()),                  &dabController, SLOT (onReqSeekUp()));
    QObject::connect(this,       SIGNAL(cmdReqLongSeekNextFromUISH()),              &dabController, SLOT (onReqLongSeekUp()));
    QObject::connect(this,       SIGNAL(cmdReqSeekPrevFromUISH()),                  &dabController, SLOT (onReqSeekDown()));
    QObject::connect(this,       SIGNAL(cmdReqLongSeekPrevFromUISH()),              &dabController, SLOT (onReqLongSeekDown()));
    QObject::connect(this,       SIGNAL(cmdReqTuneLeft()),                          &dabController, SLOT (onReqTuneDown()));
    QObject::connect(this,       SIGNAL(cmdReqTuneRight()),                         &dabController, SLOT (onReqTuneUp()));
    QObject::connect(this,       SIGNAL(cmdReqTunePressed()),                       &dabController, SLOT (onReqTunePressed()));
    QObject::connect(this,       SIGNAL(cmdReqTA(bool)),                            &dabController, SLOT (onReqTA(bool)));
    QObject::connect(this,       SIGNAL(cmdReqTASetting(bool)),                     &dabController, SLOT (onReqTASetting(bool)));
    QObject::connect(this,       SIGNAL(cmdReqCancelTrafficAnnouncement(bool)),     &dabController, SLOT (onReqCancelTrafficAnnouncement(bool)));
    QObject::connect(this,       SIGNAL(cmdReqCancelAlarmAnnouncement()),           &dabController, SLOT (onReqCancelAlarmAnnouncement()));
    QObject::connect(this,       SIGNAL(cmdReqSetReservationChannel()),             &dabController, SLOT (onReqSetReservationChannel()));
    QObject::connect(this,       SIGNAL(cmdReqSaveData()),                          &dabController, SLOT (onReqSaveData()));
    QObject::connect(this,       SIGNAL(cmdSetCurrentInfoToHomeMedia()),            &dabController, SLOT (onSetCurrentInfoToHomeMedia()));
    QObject::connect(this,       SIGNAL(cmdSendClusterUpdate()),                    &dabController, SLOT (onSendClusterUpdate()));
    QObject::connect(this,       SIGNAL(cmdSendOSDUpdate()),                        &dabController, SLOT (onSendOSDUpdate()));
    QObject::connect(this,       SIGNAL(changeDABtoFM()),                           &dabController, SLOT (onChangeDABtoFM()));
    QObject::connect(this,       SIGNAL(cmdEventExceptionHandler(eDAB_EVENT_TYPE)), &dabController, SLOT (onEventExceptionHandler(eDAB_EVENT_TYPE)));
    QObject::connect(this,       SIGNAL(cmdFactoryResetRemoveData(QString)),        &dabController, SLOT (onFactoryResetRemoveData(QString)));
#ifdef __VR_COMMAND__
    QObject::connect(this,       SIGNAL(cmdReqPresetSelected(int)),                 &dabController, SLOT (onReqPresetSelected(int)));
#endif
    QObject::connect(this,       SIGNAL(cmdReqSeekNextPresetList()),                &dabController, SLOT (onReqSeekNextPresetList()));
    QObject::connect(this,       SIGNAL(cmdReqSeekPrevPresetList()),                &dabController, SLOT (onReqSeekPrevPresetList()));
    QObject::connect(this,       SIGNAL(cmdReqTASearch()),                          &dabController, SLOT (onReqTASearch()));    
    QObject::connect(this,       SIGNAL(cmdCancelEPGReservation(bool)),             &dabController, SLOT (onCancelEPGReservation(bool)));
    QObject::connect(this,       SIGNAL(cmdReqSetAntennaStatus(quint8)),            &dabController, SLOT (onReqSetAntennaStatus(quint8)));

    // ==================================================  AppEngine.cpp -> UIListener.cpp
    QObject::connect(this,       SIGNAL(cmdLanguageChange(int)),                    dabController.getUIListener(), SLOT (onLanguageChange(int)));

    // ==================================================  DABController -> AppEngine.cpp
   QObject::connect(&dabController, SIGNAL(cmdReqDABSoundOn()),                                             this,           SLOT (onReqDABSoundOn()));
   QObject::connect(&dabController, SIGNAL(cmdReqRadioEnd(int,quint16)),                                    this,           SLOT (onReqRadioEnd(int,quint16)));
   QObject::connect(&dabController, SIGNAL(cmdReqDABtoFM(quint16)),                                         this,           SLOT (onReqDABtoFM(quint16)));
   QObject::connect(&dabController, SIGNAL(cmdReqClusterControl(QString,quint8,quint8)),                    this,           SLOT (onReqClusterControl(QString,quint8,quint8)));
   QObject::connect(&dabController, SIGNAL(cmdReqdisplayOSD(QString,QString,quint8)),                       this,           SLOT (onReqDisplayOSD(QString,QString,quint8)));
   QObject::connect(&dabController, SIGNAL(cmdStartDABtoFMTimer()),                                         this,           SLOT (startDABtoFMTimer()));
   QObject::connect(&dabController, SIGNAL(cmdStopDABtoFMTimer()),                                          this,           SLOT (stopDABtoFMTimer()));
   QObject::connect(&dabController, SIGNAL(cmdReqTASearchPopup(bool)),                                      this,           SLOT (onReqTASearchPopup(bool)));
   QObject::connect(&dabController, SIGNAL(cmdCloseSystemPopup(eDAB_SYSTEM_POPUP_TYPE)),                    this,           SLOT (closeSystemPopup(eDAB_SYSTEM_POPUP_TYPE)));
   QObject::connect(&dabController, SIGNAL(cmdSendMuteCancel()),                                            this,           SLOT (onReqSendMuteCancel()));  //# Mute Cancel




   // ==================================================  UIListener.cpp -> AppEngine.cpp
   QObject::connect(dabController.getUIListener(), SIGNAL(cmdReqTAStart(quint16,QString)),                  this,           SLOT (onReqTAStart(quint16,QString)));
   QObject::connect(dabController.getUIListener(), SIGNAL(cmdReqAlarmStart(quint16,QString)),               this,           SLOT (onReqAlarmStart(quint16,QString)));
   QObject::connect(dabController.getUIListener(), SIGNAL(cmdReqTAStop()),                                  this,           SLOT (onReqTAStop()));
   QObject::connect(dabController.getUIListener(), SIGNAL(cmdReqAlarmStop()),                               this,           SLOT (onReqAlarmStop()));
   QObject::connect(dabController.getUIListener(), SIGNAL(cmdReqEPGPreservePopup(QString,QString)),         this,           SLOT (onReqEPGPreservePopup(QString,QString)));
   QObject::connect(dabController.getUIListener(), SIGNAL(cmdReqdisplayOSD(QString,QString,quint8)),        this,           SLOT (onReqDisplayOSD(QString,QString,quint8)));

    // ==================================================  QML -> DABController.cpp
    QObject::connect(rootObject, SIGNAL(cmdReqScan()),                                                                &dabController, SLOT (onReqScan()));
    QObject::connect(rootObject, SIGNAL(cmdReqPresetScan()),                                                          &dabController, SLOT (onReqPresetScan()));
    QObject::connect(rootObject, SIGNAL(cmdReqPresetSelected(int)),                                                   &dabController, SLOT (onReqPresetSelected(int)));
    QObject::connect(rootObject, SIGNAL(cmdReqSetCurrentChannelInfo(int)),                                            &dabController, SLOT (onSetCurrentChannelInfo(int)));
    QObject::connect(rootObject, SIGNAL(cmdReqListSelected(int, QString)),                                            &dabController, SLOT (onReqListSelected(int,QString)));
    QObject::connect(rootObject, SIGNAL(cmdSettingSlideShow(bool)),                                                   &dabController, SLOT (onSettingSlideShow(bool)));
    QObject::connect(rootObject, SIGNAL(cmdSettingServiceFollowing(int)),                                             &dabController, SLOT (onSettingServiceFollowing(int)));
    QObject::connect(rootObject, SIGNAL(cmdSettingBandSelection(int)),                                                &dabController, SLOT (onSettingBandSelection(int)));
    QObject::connect(rootObject, SIGNAL(cmdSettingSaveData()),                                                        &dabController, SLOT (onSettingSaveData()));
    QObject::connect(rootObject, SIGNAL(cmdCheckExistServiceID(QDateTime,int,int,int,int)),                           &dabController, SLOT (onCheckExistServiceID(QDateTime,int,int,int,int)));
    QObject::connect(rootObject, SIGNAL(cmdCancelEPGReservation(bool)),                                               &dabController, SLOT (onCancelEPGReservation(bool)));
    QObject::connect(rootObject, SIGNAL(cmdAddEPGReservation(QDateTime,QString,int,QString,QString,int,int,int,int)), &dabController, SLOT (onAddEPGReservation(QDateTime,QString,int,QString,QString,int,int,int,int)));
    QObject::connect(rootObject, SIGNAL(cmdAddRemoveEPGReservation(QDateTime,QString,int,QString,QString,int,int,int,int)),   &dabController, SLOT (onAddRemoveEPGReservation(QDateTime,QString,int,QString,QString,int,int,int,int)));
    QObject::connect(rootObject, SIGNAL(cmdReqSeekCancel()),                                                          &dabController, SLOT (onReqSeekCancel()));        
    QObject::connect(rootObject, SIGNAL(cmdReqCancelTrafficAnnouncement(bool)),                                       &dabController, SLOT (onReqCancelTrafficAnnouncement(bool)));
    QObject::connect(rootObject, SIGNAL(cmdReqCancelAlarmAnnouncement()),                                             &dabController, SLOT (onReqCancelAlarmAnnouncement()));


    // ==================================================  QML -> AppEngine.cpp
    QObject::connect(rootObject, SIGNAL(cmdSoundSetting(bool)),                                this,            SLOT (onSoundSetting(bool)));
    QObject::connect(rootObject, SIGNAL(cmdReqTAStop()),                                       this,            SLOT (onReqTAStop()));
    QObject::connect(rootObject, SIGNAL(cmdReqAlarmStop()),                                    this,            SLOT (onReqAlarmStop()));


    // ==================================================  QML -> UIListener.cpp
    QObject::connect(rootObject, SIGNAL(cmdReqUpdatePresetList(int,int)),                      dabController.getUIListener(), SLOT (onReqUpdatePresetList(int,int)));
    QObject::connect(rootObject, SIGNAL(cmdReqModeChange(bool)),                      dabController.getUIListener(), SLOT (modeChange(bool)));
}

void
DABAppEngine::Start(void)
{
    Log("[DABAppEngine] Start - ");
    dabController.start();
}

void
DABAppEngine::NotifyFromUIStateManager( Event aEvent )
{
    NotifyFromUIStateManagerGeneric( aEvent, DISPLAY_FRONT );
}

void
DABAppEngine::NotifyFromUIStateManagerRear( Event aEvent )
{
    NotifyFromUIStateManagerGeneric( aEvent, DISPLAY_REAR );
}

void
DABAppEngine::doNotifyUISHFront( Event aEvent )
{
   Log("MobileTvAppEngine: doNotifyUISHFront()");
   NotifyUISH( aEvent, DISPLAY_FRONT );
}

void
DABAppEngine::doNotifyUISHRear( Event aEvent )
{
   Log("MobileTvAppEngine: doNotifyUISHRear()");
   NotifyUISH( aEvent, DISPLAY_REAR );
}

void
DABAppEngine::NotifyFromUIStateManagerCommon(Event aEvent)
{
    Log(QString("NotifyFromUIStateManagerCommon : aEvent = %1").arg(aEvent.GetEventId()));

    switch ( aEvent.GetEventId() )
    {
    case EVT_LANGUAGE_CHANGED:
    {
        EVENT_LANGUAGE_CHANGED_DATA_T *pLanguageData = (EVENT_LANGUAGE_CHANGED_DATA_T *)aEvent.Data().data();

        if ( pLanguageData != NULL )
        {
            Log(QString("=============================== EVT_LANGUAGE_CHANGED : languageId=%1").arg(pLanguageData->languageId));
            switch ( pLanguageData->languageId )
            {                
                case LANGUAGE_EN:
                case LANGUAGE_EN_US:{if( !m_pTranslator->load("AppDAB_en_us.qm",DAB_LOCALIZATION_PATH) ){Log(" English Load Fail");}break;}
                case LANGUAGE_EN_GB:{if( !m_pTranslator->load("AppDAB_en_gb.qm",DAB_LOCALIZATION_PATH) ){Log(" English Load Fail");}break;}
//                case LANGUAGE_EN:{if( !m_pTranslator->load("AppDAB_en.qm",DAB_LOCALIZATION_PATH) ){Log(" English Load Fail");}break;}
                case LANGUAGE_KO:{if( !m_pTranslator->load("AppDAB_ko.qm",DAB_LOCALIZATION_PATH) ){Log(" Korean Load Fail");}break;}
                case LANGUAGE_PT:{if( !m_pTranslator->load("AppDAB_pt.qm",DAB_LOCALIZATION_PATH) ){Log(" Portugese Load Fail");}break;}//< Europeen Portuguese
                case LANGUAGE_FR:{if( !m_pTranslator->load("AppDAB_fr.qm",DAB_LOCALIZATION_PATH) ){Log(" European French Load Fail");}break;}//< European French
                case LANGUAGE_IT:{if( !m_pTranslator->load("AppDAB_it.qm",DAB_LOCALIZATION_PATH) ){Log(" Italian Load Fail");}break;}//< Italian
                case LANGUAGE_DE:{if( !m_pTranslator->load("AppDAB_de.qm",DAB_LOCALIZATION_PATH) ){Log(" German Load Fail");}break;}//< German
                case LANGUAGE_ES:{if( !m_pTranslator->load("AppDAB_es.qm",DAB_LOCALIZATION_PATH) ){Log(" European Spanish Load Fail");}break;}//< European Spanish
                case LANGUAGE_RU:{if( !m_pTranslator->load("AppDAB_ru.qm",DAB_LOCALIZATION_PATH) ){Log(" Russian Load Fail");}break;}//< Russian
                case LANGUAGE_NL:{if( !m_pTranslator->load("AppDAB_nl.qm",DAB_LOCALIZATION_PATH) ){Log(" Dutch Load Fail");}break;}//< Dutch
                case LANGUAGE_SV:{if( !m_pTranslator->load("AppDAB_sv.qm",DAB_LOCALIZATION_PATH) ){Log(" Swedish Load Fail");}break;}//< Swedish
                case LANGUAGE_PL:{if( !m_pTranslator->load("AppDAB_pl.qm",DAB_LOCALIZATION_PATH) ){Log(" Polish Load Fail");}break;}//< Polish
                case LANGUAGE_TR:{if( !m_pTranslator->load("AppDAB_tr.qm",DAB_LOCALIZATION_PATH) ){Log(" Turkish Load Fail");}break;}//< Turkish
                case LANGUAGE_CS:{if( !m_pTranslator->load("AppDAB_cs.qm",DAB_LOCALIZATION_PATH) ){Log(" Czech Load Fail");}break;}//< Czech
                case LANGUAGE_DA:{if( !m_pTranslator->load("AppDAB_da.qm",DAB_LOCALIZATION_PATH) ){Log(" Danish Load Fail");}break;}//< Danish
                case LANGUAGE_SK:{if( !m_pTranslator->load("AppDAB_sk.qm",DAB_LOCALIZATION_PATH) ){Log(" Slovakia Load Fail");}break;}//< Slovakia
                case LANGUAGE_AR://< Arabic
                case LANGUAGE_ZH_CN://< Peoples Republic Of China (Mandarin)
                //case LANGUAGE_ZH_CMN://< Peoples Republic Of China (Mandarin) to be backward compatible
                case LANGUAGE_ZH_YUE://< Peoples Republic Of China (Cantonese)
                default:{if( !m_pTranslator->load("AppDAB_en.qm",DAB_LOCALIZATION_PATH) ){Log( " Default English Load Fail");}break;}
            }

            //qApp->removeTranslator(m_pTranslator);
            qApp->installTranslator( m_pTranslator );
           // qDebug() << "DABAppEngine.cpp : installTranslator ";
            emit retranslateUi(pLanguageData->languageId);
            emit cmdLanguageChange(pLanguageData->languageId);

//            QObject *rootObject = dynamic_cast<QObject *>( GetCurrentView()->rootObject() );
//            if ( rootObject != NULL )
//            {
//                qDebug() << "DABAppEngine.cpp : QObject *rootObject = dynamic_cast<QObject *>( GetCurrentView()->rootObject() ); ";
//                QMetaObject::invokeMethod( rootObject, "retranslateUi", Q_ARG( QVariant, pLanguageData->languageId ));
//            }
        }
        break;
    }

    case EVT_AV_MODE_CHANGE:
    {
        EVENT_MODE_CHANGED_DATA_T* payload = (EVENT_MODE_CHANGED_DATA_T*)aEvent.Data().data();
        Log(QString("=============================== EVT_AV_MODE_CHANGE : %1, ServiceFollowingState : %2 ").arg(payload->sMode).arg(dabController.getServiceFollowingState()));
        qDebug() << "=============================== EVT_AV_MODE_CHANGE : payload->sMode = " << payload->sMode << ", m_eDabCurrentStatus = " << m_eDabCurrentStatus << ", ServiceFollowingState = " << dabController.getServiceFollowingState();
        m_bPopupNone = false;
        m_bVRFlag = false;
        m_bMixFlag = false;
        m_bCCPLongPressed = false;
        if(payload->sMode == MODE_STATE_DAB1 || payload->sMode == MODE_STATE_DAB2 || payload->sMode == MODE_STATE_DAB3)
        {
            m_bDABAudioOpen = true;
            if(m_eDabCurrentStatus == DAB_BG_STATUS) emit cmdSetCurrentInfoToHomeMedia();

            if(m_eDabCurrentStatus == DAB_BG_FM_STATUS) m_eDabCurrentStatus = DAB_BG_STATUS;

            emit cmdEventExceptionHandler(DAB_EVENT_AV_MODE_FG);

            m_pRdsAdaptor->writeAnnouncePopupStatus(true);
            startDABtoFMTimer();
        }
        else if(payload->sMode == MODE_STATE_AVOFF)
        {
            m_bDABAudioOpen = false;
            emit cmdEventExceptionHandler(DAB_EVENT_AV_OFF);
        }
        else if(payload->sMode == MODE_STATE_DAB_FM)
        {
            m_bDABAudioOpen = false;
            stopDABtoFMTimer();
            if(dabController.getServiceFollowingState() == DAB_SERVICE_FOLLOWING_STATE_AUDIO_FM_CHANGEING)
            {
                Log(" EVT_AV_MODE_CHANGE : MODE_STATE_DAB_FM ");
                qDebug() << "====================================== EVT_AV_MODE_CHANGE : MODE_STATE_DAB_FM ===";
                if(m_eDabCurrentStatus == DAB_FG_STATUS) m_eDabCurrentStatus = DAB_FG_FM_STATUS;
                else if(m_eDabCurrentStatus == DAB_BG_STATUS) m_eDabCurrentStatus = DAB_BG_FM_STATUS;
                emit changeDABtoFM();
            }
            m_pRdsAdaptor->writeAnnouncePopupStatus(false);
        }
        else if(payload->sMode == MODE_STATE_NONE)
        {
            m_bDABAudioOpen = false;
            stopDABtoFMTimer();
            emit cmdEventExceptionHandler(DAB_EVENT_AV_MODE_NONE);
            m_pRdsAdaptor->writeAnnouncePopupStatus(false);
        }
        break;
    }

    case EVT_NOTIFY_AV_MODE:
    {
        EVENT_MODE_CHANGED_DATA_T* payload = (EVENT_MODE_CHANGED_DATA_T*)aEvent.Data().data();
        Log(QString("=============================== EVT_NOTIFY_AV_MODE : %1, ServiceFollowingState : %2 ").arg(payload->sMode).arg(dabController.getServiceFollowingState()));
        qDebug() << "=============================== EVT_NOTIFY_AV_MODE : payload->sMode = " << payload->sMode << ", m_eDabCurrentStatus = " << m_eDabCurrentStatus << ", ServiceFollowingState = " << dabController.getServiceFollowingState();

        if(payload->sMode == MODE_STATE_DAB1 || payload->sMode == MODE_STATE_DAB2 || payload->sMode == MODE_STATE_DAB3)
        {
            emit cmdEventExceptionHandler(DAB_EVENT_NOTIFY_AV_MODE);
        }

        break;
    }

    case EVT_DRS_SHOW :
    {
        DABLogger::instance()->Log((QString(" ======================================== EVT_DRS_SHOW ========================================")));
        qDebug() << "======================================== EVT_DRS_SHOW ========================================";
        emit drivingRegulation(true);
        break;
    }

    case EVT_DRS_HIDE:
    {
        DABLogger::instance()->Log((QString(" ======================================== EVT_DRS_HIDE ========================================")));
        qDebug() << "======================================== EVT_DRS_HIDE ========================================";
        emit drivingRegulation(false);
        break;
    }

    case EVT_UPDATE_CLUSTER:
    {
        int currentAVMode = checkAudioPath();
        DABLogger::instance()->Log((QString(" ======================================== EVT_UPDATE_CLUSTER :: currentAVMode = %1").arg(currentAVMode)));
        qDebug() << "======================================== EVT_UPDATE_CLUSTER :: currentAVMode = " << currentAVMode;
        if(currentAVMode == MODE_STATE_DAB1)
        {
            emit cmdSendClusterUpdate();
        }
        break;
    }

    case EVT_TA_DAB_START:
        DABLogger::instance()->Log((QString(" ======================================== EVT_TA_DAB_START ========================================")));
        qDebug() << "======================================== EVT_TA_DAB_START ========================================";
        break;

    case EVT_TA_DAB_END:
    {
        DABLogger::instance()->Log((QString(" ======================================== EVT_TA_DAB_END ========m_eDabSystemPopupType = %1, aEvent.GetCommand() = %2").arg(m_eDabSystemPopupType).arg(aEvent.GetCommand())));
        qDebug() << "======================================== EVT_TA_DAB_END  : m_eDabSystemPopupType = " << m_eDabSystemPopupType << ", aEvent.GetCommand() = " << aEvent.GetCommand();

        if(aEvent.GetCommand() == ePOPUP_DAB_TA_ALARM)  // Alarm announcement
        {
            if(m_eDabSystemPopupType != DAB_POPUP_ALARM_TYPE)
            {
                return;
            }
            emit cmdReqCancelAlarmAnnouncement();
            emit cmdEventExceptionHandler(DAB_EVENT_ANNOUNCEMENT_STOP);
            Event event(EVT_REQUEST_SYSTEM_POPUP_CLOSE, GetThisAppId(), APP_UI_MANAGER);
            NotifyUISHCommon(event);
        }
        else if(aEvent.GetCommand() == TA_POPUP_FROM_DAB) // traffic announcement
        {
            if(m_eDabSystemPopupType != DAB_POPUP_TA_TYPE)
            {
                return;
            }
            emit cmdReqCancelTrafficAnnouncement(false);
            emit cmdEventExceptionHandler(DAB_EVENT_ANNOUNCEMENT_STOP);
            Event event(EVT_REQUEST_SYSTEM_POPUP_CLOSE, GetThisAppId(), APP_UI_MANAGER);
            NotifyUISHCommon(event);
        }

        emit displayTAPopup(m_sAnnouncementServiceName, m_AnnouncementFlags, false);

        m_eDabSystemPopupType = DAB_POPUP_NONE;
        m_AnnouncementFlags = ANN_ALARM_ON;
        m_sAnnouncementServiceName = "";
    }
        break;

    case EVT_DISPLAY_ON:
        DABLogger::instance()->Log((QString(" ======================================== EVT_DISPLAY_ON ========================================")));
        qDebug() << "======================================== EVT_DISPLAY_ON ========================================";
        break;

    case EVT_DISPLAY_OFF:
        DABLogger::instance()->Log((QString(" ======================================== EVT_DISPLAY_OFF ========================================")));
        qDebug() << "======================================== EVT_DISPLAY_OFF ========================================";
        emit cmdEventExceptionHandler(DAB_EVENT_DISPLAY_OFF);
        break;

    case EVT_AGREE_SHOW_OSD:
        DABLogger::instance()->Log((QString(" ======================================== EVT_AGREE_SHOW_OSD ========================================")));
        qDebug() << "======================================== EVT_AGREE_SHOW_OSD ========================================";
        m_bAgreeShowOSD = true;
        break;

    case EVT_KEY_HU_SEEK_NEXT:
    case EVT_KEY_RRC_SEEK_NEXT:
    case EVT_KEY_SWRC_SEEK_NEXT:
    case EVT_KEY_RRC_CH_FW:
    {
        if(m_eDabSystemPopupType == DAB_POPUP_TA_TYPE  || m_eDabSystemPopupType == DAB_POPUP_ALARM_TYPE)
        {
            qDebug() << " ============= EVT_KEY_HU_SEEK_NEXT : currnet activate TA popup, so ignored seek next";
            return;
        }

        if(m_bCCPLongPressed)
        {
            qDebug() << " ============= EVT_KEY_HU_SEEK_NEXT : CCP Long Pressed status. so ignored";
            return;
        }

        KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();       
        this->closeSystemPopup(m_eDabSystemPopupType);
        checkEPGPopupVisible();

        QObject *objectStationList = this->GetCurrentView()->rootObject()->findChild<QObject*>("DABStationList");
        bool isStationList = QDeclarativeProperty::read(objectStationList, "visible").toBool();

        QObject *objectPresetList = this->GetCurrentView()->rootObject()->findChild<QObject*>("DABPresetList");
        bool isPresetList = QDeclarativeProperty::read(objectPresetList, "visible").toBool();

        qDebug() << " ============= EVT_KEY_HU_SEEK_NEXT : keyStatus = " << message->keyStatus << ", StationList visible = " << isStationList << ", PresetList visible = " << isPresetList << ", m_eDabSystemPopupType = " << m_eDabSystemPopupType;

        if((isStationList || isPresetList) && (m_eDabCurrentStatus == DAB_FG_STATUS || m_eDabCurrentStatus == DAB_FG_FM_STATUS))
        {
            if(message->keyStatus == KEY_STATUS_PRESSED_LONG)
            {
                m_bSeekLongPressed = true;
                playAudioBeep();
            }
            else if(message->keyStatus == KEY_STATUS_RELEASED)
            {
                if(m_bSeekLongPressed)
                {
                    m_bSeekLongPressed = false;
                }
            }
            PostKeyEvent((KEY_EVENT_DATA_T *)aEvent.Data().data(), Qt::Key_Q);
        }
        else
        {
            if(message->keyStatus == KEY_STATUS_PRESSED_LONG)
            {
                m_bSeekLongPressed = true;
                playAudioBeep();
                emit cmdReqLongSeekNextFromUISH();
            }
            else if(message->keyStatus == KEY_STATUS_RELEASED)
            {
                if(m_bSeekLongPressed)
                {
                    m_bSeekLongPressed = false;
                }
                else
                {
                    if(aEvent.GetEventId() == EVT_KEY_SWRC_SEEK_NEXT || aEvent.GetEventId() == EVT_KEY_RRC_CH_FW)
                        emit cmdReqSeekNextPresetList();
                    else
                        emit cmdReqSeekNextFromUISH();
                }
            }
            if((m_eDabCurrentStatus == DAB_FG_STATUS || m_eDabCurrentStatus == DAB_FG_FM_STATUS))
                PostKeyEvent((KEY_EVENT_DATA_T *)aEvent.Data().data(), Qt::Key_Q);
        }
        break;
    }

    case EVT_KEY_HU_SEEK_PREV:
    case EVT_KEY_RRC_SEEK_PREV:
    case EVT_KEY_SWRC_SEEK_PREV:
    case EVT_KEY_RRC_CH_BK: //2012-08-17 jyjeon
    {
        if(m_eDabSystemPopupType == DAB_POPUP_TA_TYPE  || m_eDabSystemPopupType == DAB_POPUP_ALARM_TYPE)
        {
            qDebug() << " ============= EVT_KEY_HU_SEEK_PREV : currnet activate TA popup, so ignored seek next";
            return;
        }

        if(m_bCCPLongPressed)
        {
            qDebug() << " ============= EVT_KEY_HU_SEEK_PREV : CCP Long Pressed status. so ignored";
            return;
        }

        KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
        this->closeSystemPopup(m_eDabSystemPopupType);
        checkEPGPopupVisible();

        QObject *objectStationList = this->GetCurrentView()->rootObject()->findChild<QObject*>("DABStationList");
        bool isStationList = QDeclarativeProperty::read(objectStationList, "visible").toBool();

        QObject *objectPresetList = this->GetCurrentView()->rootObject()->findChild<QObject*>("DABPresetList");
        bool isPresetList = QDeclarativeProperty::read(objectPresetList, "visible").toBool();

        qDebug() << " ============= EVT_KEY_HU_SEEK_PREV : keyStatus = " << message->keyStatus << ", StationList visible = " << isStationList << ", PresetList visible = " << isPresetList << ", m_eDabCurrentStatus = " << m_eDabCurrentStatus;

        if((isStationList || isPresetList) && (m_eDabCurrentStatus == DAB_FG_STATUS || m_eDabCurrentStatus == DAB_FG_FM_STATUS))
        {
            if(message->keyStatus == KEY_STATUS_PRESSED_LONG)
            {
                Log( " EVT_KEY_HU_SEEK_PREV - KEY_STATUS_PRESSED_LONG" );
                m_bSeekLongPressed = true;
                playAudioBeep();
            }
            else if(message->keyStatus == KEY_STATUS_RELEASED)
            {
                if(m_bSeekLongPressed)
                {
                    m_bSeekLongPressed = false;
                }
            }
            PostKeyEvent((KEY_EVENT_DATA_T *)aEvent.Data().data(), Qt::Key_W);
        }
        else
        {
            if(message->keyStatus == KEY_STATUS_PRESSED_LONG)
            {
                m_bSeekLongPressed = true;
                playAudioBeep();
                emit cmdReqLongSeekPrevFromUISH();
            }
            else if(message->keyStatus == KEY_STATUS_RELEASED)
            {
                if(m_bSeekLongPressed)
                {
                    m_bSeekLongPressed = false;
                }
                else
                {
                    if(aEvent.GetEventId() == EVT_KEY_SWRC_SEEK_PREV || aEvent.GetEventId() == EVT_KEY_RRC_CH_BK)
                        emit cmdReqSeekPrevPresetList();
                    else
                        emit cmdReqSeekPrevFromUISH();
                }
            }
            if((m_eDabCurrentStatus == DAB_FG_STATUS || m_eDabCurrentStatus == DAB_FG_FM_STATUS))
                PostKeyEvent((KEY_EVENT_DATA_T *)aEvent.Data().data(), Qt::Key_W);
        }
        break;
    }

    case EVT_KEY_CCP_JOG_DIAL_ENCODER: //103
    {
        KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
        Log( QString(" EVT_KEY_CCP_JOG_DIAL_ENCODER - message->keyStatus:%1, Command:%2").arg(message->keyStatus).arg(aEvent.GetCommand()));
        PostJogEvent((KEY_EVENT_DATA_T *)aEvent.Data().data(), aEvent.GetCommand());
        break;
    }

    case EVT_KEY_RRC_JOG_DIAL_ENCODER: //104
    {
        KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
        Log( QString(" EVT_KEY_RRC_JOG_DIAL_ENCODER - message->keyStatus:%1, Command:%2").arg(message->keyStatus).arg(aEvent.GetCommand()) );
        PostJogEvent((KEY_EVENT_DATA_T *)aEvent.Data().data(), aEvent.GetCommand());
    }
        break;

    case EVT_KEY_HU_BACK:
    case EVT_KEY_CCP_BACK:
    case EVT_KEY_RRC_BACK:
    {
        Log(" EVT_KEY_.._BACK received.. Do Nothing.." );
        if(m_eDabSystemPopupType == DAB_POPUP_TA_TYPE || m_eDabSystemPopupType == DAB_POPUP_PRESERVE_TYPE  || m_eDabSystemPopupType == DAB_POPUP_ALARM_TYPE)
        {
            qDebug() << " =======case EVT_KEY_HU_BACK : If TA / Preserve Popup Popup, Menu Key ignore !!";
            return;
        }

        KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();

        if(message->keyStatus == KEY_STATUS_PRESSED_LONG)
        {
            qDebug() << " =======case EVT_KEY_HU_BACK: Back long key ignore !!";
            return;
        }

        PostKeyEvent((KEY_EVENT_DATA_T *)aEvent.Data().data(), Qt::Key_Backspace);
    }
        break;

    case EVT_KEY_HU_HOME:
    case EVT_KEY_CCP_HOME:
    case EVT_KEY_RRC_HOME:
        Log(" EVT_KEY_.._HOME received.. Do Nothing.." );
        break;

    case EVT_CALL_START :
        Log( QString(" EVT_CALL_START - currentDABStatus:%1").arg(m_eDabCurrentStatus) );
        m_bCallFlag = true;
        emit cmdEventExceptionHandler(DAB_EVENT_BT_CALL_START);
        break;

    case EVT_CALL_END :
        Log( QString(" EVT_CALL_END - bcurrentDABStatus:%1").arg(m_eDabCurrentStatus) );
        m_bCallFlag = false;
        emit cmdSendClusterUpdate();
        break;

    case EVT_MIX_PLAY_START:
        Log( QString(" EVT_MIX_PLAY_START - bcurrentDABStatus:%1").arg(m_eDabCurrentStatus) );
        m_bMixFlag = true;
        break;

    case EVT_MIX_PLAY_END:
        Log( QString(" EVT_MIX_PLAY_END - bcurrentDABStatus:%1").arg(m_eDabCurrentStatus) );
        m_bMixFlag = false;
        break;

    case EVT_KEY_SOFT_MENU:
    {
        Log( "EVT_KEY_SOFT_MENU" );
        if(m_eDabSystemPopupType == DAB_POPUP_TA_TYPE || m_eDabSystemPopupType == DAB_POPUP_PRESERVE_TYPE  || m_eDabSystemPopupType == DAB_POPUP_ALARM_TYPE)
        {
            qDebug() << "when TA / Preserve Popup Popup, Menu Key ignore !!";
            return;
        }

        QKeyEvent* event = new QKeyEvent(QEvent::KeyPress, Qt::Key_I, Qt::NoModifier);
        qApp->postEvent(GetCurrentView(), event);
        break;
    }

    case EVT_KEY_HU_MENU:
    case EVT_KEY_CCP_MENU:
    case EVT_KEY_RRC_MENU:
    {
        Log( " EVT_KEY_.._MENU received.. Do Nothing.." );
        if(m_eDabSystemPopupType == DAB_POPUP_TA_TYPE || m_eDabSystemPopupType == DAB_POPUP_PRESERVE_TYPE  || m_eDabSystemPopupType == DAB_POPUP_ALARM_TYPE)
        {
            qDebug() << " =======case EVT_KEY_RRC_MENU: If TA / Preserve Popup Popup, Menu Key ignore !!";
            return;
        }
        KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();

        if(message->keyStatus == KEY_STATUS_PRESSED_LONG)
        {
            qDebug() << " =======case EVT_KEY_HU_MENU: Menu long key ignore !!";
            return;
        }

        PostKeyEvent((KEY_EVENT_DATA_T *)aEvent.Data().data(), Qt::Key_I);
        break;
    }

    case EVT_KEY_CCP_JOG_DIAL_UP:
    case EVT_KEY_RRC_JOG_DIAL_UP:
    {
        KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
        DABLogger::instance()->Log(QString(" EVT_KEY_..EVT_KEY_CCP_JOG_DIAL_UP received.. KeyStatus=%1").arg(message->keyStatus));
        if(message->keyStatus == KEY_STATUS_PRESSED_LONG)
        {
            m_bCCPLongPressed = true;
        }
        else if(message->keyStatus == KEY_STATUS_RELEASED)
        {
            m_bCCPLongPressed = false;
        }
        PostKeyEvent((KEY_EVENT_DATA_T *)aEvent.Data().data(), Qt::Key_Up);
        break;
    }

    case EVT_KEY_CCP_JOG_DIAL_DOWN:
    case EVT_KEY_RRC_JOG_DIAL_DOWN:
    {
        KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
        DABLogger::instance()->Log(QString(" EVT_KEY_..EVT_KEY_CCP_JOG_DIAL_DOWN received.. KeyStatus=%1").arg(message->keyStatus));
        if(message->keyStatus == KEY_STATUS_PRESSED_LONG)
        {
            m_bCCPLongPressed = true;
        }
        else if(message->keyStatus == KEY_STATUS_RELEASED)
        {
            m_bCCPLongPressed = false;
        }
        PostKeyEvent((KEY_EVENT_DATA_T *)aEvent.Data().data(), Qt::Key_Down);
        break;
    }

    case EVT_KEY_CCP_JOG_DIAL_LEFT:
    case EVT_KEY_RRC_JOG_DIAL_LEFT:
    {
        Log(" EVT_KEY_.._JOG_DIAL_LEFT received..");
        m_bCCPLongPressed = false;
        PostKeyEvent((KEY_EVENT_DATA_T *)aEvent.Data().data(), Qt::Key_Left);
        break;
    }

    case EVT_KEY_CCP_JOG_DIAL_RIGHT:
    case EVT_KEY_RRC_JOG_DIAL_RIGHT:
    {
        Log(" EVT_KEY_.._JOG_DIAL_RIGHT received..");
        m_bCCPLongPressed = false;
        PostKeyEvent((KEY_EVENT_DATA_T *)aEvent.Data().data(), Qt::Key_Right);
        break;
    }

    case EVT_KEY_CCP_JOG_DIAL_CENTER:
    case EVT_KEY_RRC_JOG_DIAL_CENTER:
    {
        KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
        DABLogger::instance()->Log(QString(" EVT_KEY_..EVT_KEY_CCP_JOG_DIAL_CENTER received.. KeyStatus=%1").arg(message->keyStatus));

        if(m_bSeekLongPressed)
        {
            qDebug() << " Seek up/down Long press!!. so jog enter event ignored. !!";
            return;
        }

        if(message->keyStatus == KEY_STATUS_PRESSED_LONG || message->keyStatus == KEY_STATUS_PRESSED)
        {
            m_bCCPLongPressed = true;
        }
        else if(message->keyStatus == KEY_STATUS_RELEASED)
        {
            m_bCCPLongPressed = false;
        }
        PostKeyEvent((KEY_EVENT_DATA_T *)aEvent.Data().data(), Qt::Key_Return);
        break;
    }

    case EVT_KEY_CCP_JOG_DIAL_TOP_LEFT:
    case EVT_KEY_RRC_JOG_DIAL_TOP_LEFT:
    case EVT_KEY_CCP_JOG_DIAL_DOWN_LEFT:
    case EVT_KEY_RRC_JOG_DIAL_DOWN_LEFT:
    {
        Log(" EVT_KEY_.._JOG_DIAL_TOP, DOWN_LEFT received.. Do Nothing..");
        //        PostKeyEvent((KEY_EVENT_DATA_T *)aEvent.Data().data(), Qt::Key_Minus);
        //        KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
        //        if(message->keyStatus == KEY_STATUS_PRESSED){
        //            //DAB_LOG << " Send KeyPress, Key_TopLeft -> Key_Minus" << endl;
        //            QKeyEvent* event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Minus, Qt::NoModifier);
        //            qApp->postEvent(GetCurrentView(), event);
        //        }else if(message->keyStatus == KEY_STATUS_RELEASED){
        //            //DAB_LOG << " Send KeyRelease, Key_TopLeft -> Key_Minus" << endl;
        //            QKeyEvent* event = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Minus, Qt::NoModifier);
        //            qApp->postEvent(GetCurrentView(), event);
        //        }
        break;
    }

    case EVT_KEY_CCP_JOG_DIAL_TOP_RIGHT:
    case EVT_KEY_RRC_JOG_DIAL_TOP_RIGHT:
    case EVT_KEY_CCP_JOG_DIAL_DOWN_RIGHT:
    case EVT_KEY_RRC_JOG_DIAL_DOWN_RIGHT:
    {
        Log(" EVT_KEY_.._JOG_DIAL_TOP, DOWN_RIGHT received.. Do Nothing..");
        //        PostKeyEvent((KEY_EVENT_DATA_T *)aEvent.Data().data(), Qt::Key_Equal);
        //        KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();
        //        if(message->keyStatus == KEY_STATUS_PRESSED){
        //            //DAB_LOG << " Send KeyPress, Key_TopLeft -> Key_Equal" << endl;
        //            QKeyEvent* event = new QKeyEvent(QEvent::KeyPress, Qt::Key_Equal, Qt::NoModifier);
        //            qApp->postEvent(GetCurrentView(), event);
        //        }else if(message->keyStatus == KEY_STATUS_RELEASED){
        //            //DAB_LOG << " Send KeyRelease, Key_TopLeft -> Key_Equal" << endl;
        //            QKeyEvent* event = new QKeyEvent(QEvent::KeyRelease, Qt::Key_Equal, Qt::NoModifier);
        //            qApp->postEvent(GetCurrentView(), event);
        //        }
        break;
    }
    ////////////////////////////////////////////////
    //Power Management
    //This is sent from UISH to applications to save their states/ context after receiving signal from DSME
    case EVT_REQUEST_SAVE_UNSAVED_DATA:
    {
        Log(" EVT_REQUEST_SAVE_UNSAVED_DATA - emit cmdReqSaveData()");
        EVENT_RESTORE_LAST_MODE_DATA_T BandMode;
        BandMode.sMode = MODE_STATE_DAB1;
        if(!m_bFactoryReset)
            emit cmdReqSaveData();
        Event event(EVT_RESPONSE_SAVE_UNSAVED_DATA, GetThisAppId(),APP_UI_MANAGER, QByteArray( (char *)&BandMode, sizeof(BandMode) ));
        NotifyUISHCommon( event );
        m_bFactoryReset = false;
        break;
    }

    case EVT_RESTORE_LAST_AV_MODE:
    {
        emit cmdSetCurrentInfoToHomeMedia();
        break;
    }

    case EVT_ALL_SLEEP_CANCEL_ACTIVATE:
    {
        emit cmdSetCurrentInfoToHomeMedia();
        break;
    }

    case EVT_KEY_HU_JOG_DIAL_ENCODER:
    {
        if(m_eDabSystemPopupType == DAB_POPUP_TA_TYPE  || m_eDabSystemPopupType == DAB_POPUP_ALARM_TYPE)
        {
            qDebug() << " ============= EVT_KEY_HU_JOG_DIAL_ENCODER : currnet activate TA popup, so ignored tune dial event";
            return;
        }

        DABLogger::instance()->Log(QString(" EVT_KEY_..EVT_KEY_HU_JOG_DIAL_ENCODER received.. m_bCCPLongPressed=%1").arg(m_bCCPLongPressed));

        if(m_bCCPLongPressed)
        {
            qDebug() << " ============= EVT_KEY_HU_JOG_DIAL_ENCODER : CCP Long Pressed status. so ignored";
            return;
        }

        this->closeSystemPopup(m_eDabSystemPopupType);
        //checkEPGPopupVisible(); // ITS 0219033
        QObject *objectPlayerOptionMenu = this->GetCurrentView()->rootObject()->findChild<QObject*>("DABPlayerOptionMenu");
        bool isPlayerOptionMenu = QDeclarativeProperty::read(objectPlayerOptionMenu, "visible").toBool();

        QObject *objectStationList = this->GetCurrentView()->rootObject()->findChild<QObject*>("DABStationList");
        bool isStationList = QDeclarativeProperty::read(objectStationList, "visible").toBool();

        QObject *objectPresetList = this->GetCurrentView()->rootObject()->findChild<QObject*>("DABPresetList");
        bool isPresetList = QDeclarativeProperty::read(objectPresetList, "visible").toBool();

        qDebug() << " ============= EVT_KEY_HU_JOG_DIAL_ENCODER : StationList visible = " << isStationList << ", PresetList visible = " << isPresetList << ", m_eDabSystemPopupType = " << m_eDabSystemPopupType << ", isPlayerOptionMenu = " << isPlayerOptionMenu;

        if((isStationList || isPresetList) && (m_eDabCurrentStatus == DAB_FG_STATUS || m_eDabCurrentStatus == DAB_FG_FM_STATUS))
        {
            if( aEvent.GetCommand() > 0 )
            {
                qApp->postEvent(GetCurrentView(), new QKeyEvent(QEvent::KeyPress, Qt::Key_O, Qt::AltModifier ));
                qApp->processEvents(QEventLoop::AllEvents);
                qApp->postEvent(GetCurrentView(), new QKeyEvent(QEvent::KeyRelease, Qt::Key_O, Qt::AltModifier ));
                qApp->processEvents(QEventLoop::AllEvents);
            }
            else
            {
                qApp->postEvent(GetCurrentView(), new QKeyEvent(QEvent::KeyPress, Qt::Key_O, Qt::ControlModifier ));
                qApp->processEvents(QEventLoop::AllEvents);
                qApp->postEvent(GetCurrentView(), new QKeyEvent(QEvent::KeyRelease, Qt::Key_O, Qt::ControlModifier ));
                qApp->processEvents(QEventLoop::AllEvents);
            }            
            return;
        }

        if(isPlayerOptionMenu)
        {
            if( aEvent.GetCommand() > 0 )
            {
                qApp->postEvent(GetCurrentView(), new QKeyEvent(QEvent::KeyPress, Qt::Key_O, Qt::AltModifier ));
                qApp->processEvents(QEventLoop::AllEvents);
                qApp->postEvent(GetCurrentView(), new QKeyEvent(QEvent::KeyRelease, Qt::Key_O, Qt::AltModifier ));
                qApp->processEvents(QEventLoop::AllEvents);
                emit cmdReqTuneRight();
            }
            else
            {
                qApp->postEvent(GetCurrentView(), new QKeyEvent(QEvent::KeyPress, Qt::Key_O, Qt::ControlModifier ));
                qApp->processEvents(QEventLoop::AllEvents);
                qApp->postEvent(GetCurrentView(), new QKeyEvent(QEvent::KeyRelease, Qt::Key_O, Qt::ControlModifier ));
                qApp->processEvents(QEventLoop::AllEvents);
                emit cmdReqTuneLeft();
            }
        }
        else
        {
            if( aEvent.GetCommand() > 0 )
            {
                emit cmdReqTuneRight();
            }
            else
            {
                emit cmdReqTuneLeft();
            }
        }
        break;
    }

#ifdef __DTC__
    case EVT_DIAGNOSTIC_TEST_START:
    {
        Log("EVT_DIAGNOSTIC_TEST_START");

        APP_EVENT_DIAGNOSTIC_TEST_T* payload = (APP_EVENT_DIAGNOSTIC_TEST_T*)aEvent.Data().data();
        if(payload == NULL) return;

        m_interAntennaTimer->stop();
        m_iAntennaNormalCount = DTC_EXT_CHECK_COUNT;
        m_iAntennaOutBatteryCount = DTC_EXT_CHECK_COUNT;
        m_iAntennaShortCount = DTC_EXT_CHECK_COUNT;
        m_iAntennaOpenCount = DTC_EXT_CHECK_COUNT;
        m_exterAntennaTimer->start(200);
        break;
    }
#endif

    case EVT_RADIO_STATUS_RESPONSE:
        Log(" EVT_RADIO_STATUS_RESPONSE_DAB ");
//        EVENT_RADIO_STATUS_RESPONSE_DATA_T* resRadioData = (EVENT_RADIO_STATUS_RESPONSE_DATA_T*)aEvent.Data().data();

//        quint8 playStatus = resRadioData->sPlayStatus;
//        quint16 playPICode = resRadioData->sPlayPICode;
        break;

    case EVT_RADIO_RESPONSE_DAB:
        Log(" EVT_RADIO_RESPONSE_DAB ");
        break;

#ifdef __VR_COMMAND__
    case EVT_VR_COMMAND:
    {
        Log( " >> START VR COMMAND Event::EVT_VR_COMMAND" );
        if ( !aEvent.Data().isEmpty() )
        {
            EVENT_EVT_VR_COMMAND_DATA_T *eventData = (EVENT_EVT_VR_COMMAND_DATA_T*)aEvent.Data().data();
            Log( QString(" >> VR COMMAND sub Command %1").arg(eventData->commandId));

            switch ( eventData->commandId )
            {
                case AV_MODE_CONTROL_RADIO_SEEK:
                {
                    bool seekUp = static_cast<bool>( eventData->sVRPayload.data[0] );
                    Log(QString(" =================>> AV_MODE_CONTROL_RADIO_SEEK direction = %1").arg(seekUp));

                    if (seekUp)
                        emit cmdReqSeekNextFromUISH();
                    else
                        emit cmdReqSeekPrevFromUISH();
                    break;
                }

                case AV_MODE_CONTROL_CALL_PRESET_MEMORY:
                {
                    quint8 presetIndex = static_cast<quint8>( eventData->sVRPayload.data[0] );
                    Log(QString(" =================>> AV_MODE_CONTROL_CALL_PRESET_MEMORY presetId = %1 ").arg(presetIndex));
                    emit cmdReqPresetSelected(presetIndex-1);
                    break;
                }

                case AV_MODE_CONTROL_STATION_LIST:
                {
                    m_bStationListFromVR = true;
                    emit reqStationListFromVR();                    
                    break;
                }

                default:
                    break;
                }
        }
        break;
    }
#endif
    case EVT_KEY_HU_TUNE_PRESSED:
    {
        if(m_eDabSystemPopupType == DAB_POPUP_TA_TYPE  || m_eDabSystemPopupType == DAB_POPUP_ALARM_TYPE)
        {
            qDebug() << " ============= EVT_KEY_HU_TUNE_PRESSED : currnet activate TA popup, so ignored seek next";
            return;
        }

        KEY_EVENT_DATA_T *message = (KEY_EVENT_DATA_T *)aEvent.Data().data();

        QObject *objectStationList = this->GetCurrentView()->rootObject()->findChild<QObject*>("DABStationList");
        bool isStationList = QDeclarativeProperty::read(objectStationList, "visible").toBool();

        QObject *objectPresetList = this->GetCurrentView()->rootObject()->findChild<QObject*>("DABPresetList");
        bool isPresetList = QDeclarativeProperty::read(objectPresetList, "visible").toBool();

        QObject *objectPlayerOptionMenu = this->GetCurrentView()->rootObject()->findChild<QObject*>("DABPlayerOptionMenu");
        bool isPlayerOptionMenu = QDeclarativeProperty::read(objectPlayerOptionMenu, "visible").toBool();

        Log( QString(" EVT_KEY_HU_TUNE_PRESSED - message->keyStatus:%1, Command:%2, getBlockedStation=%3").arg(message->keyStatus).arg(aEvent.GetCommand()).arg(this->getBlockedStation()));
        qDebug() << " ============= EVT_KEY_HU_TUNE_PRESSED : keyStatus = " << message->keyStatus << ", command = " << aEvent.GetCommand() << ", getBlockedStation = " << this->getBlockedStation();
        qDebug() << " ============= EVT_KEY_HU_TUNE_PRESSED : StationList visible = " << isStationList << ", PresetList visible = " << isPresetList << ", m_eDabSystemPopupType = " << m_eDabSystemPopupType << ", isPlayerOptionMenu = " << isPlayerOptionMenu;
        if(message->keyStatus == KEY_STATUS_PRESSED)
        {
            if(isStationList || isPresetList || isPlayerOptionMenu)
            {
                Log( " EVT_KEY_HU_TUNE_PRESSED - KEY_STATUS_PRESSED" );
                QKeyEvent* event = new QKeyEvent(QEvent::KeyPress, Qt::Key_O, Qt::NoModifier);
                qApp->postEvent(GetCurrentView(), event);
            }
            else
            {
                emit cmdReqTunePressed();
            }
        }
        break;
    }

    case EVT_FACTORY_RESET_REQUEST: //# Factory Reset
    {
        Log( " EVT_FACTORY_RESET_REQUEST " );
        Event event(EVT_FACTORY_RESET_RESPONSE, GetThisAppId(), APP_UI_MANAGER);

        emit cmdFactoryResetRemoveData(DAB_DATA_ROOT);

        if(!NotifyUISHCommon(event))
        {
            qDebug() << "Factory Reset Response Failed";
        }
        m_bFactoryReset = true;
        break;
    }

    case EVT_POST_EVENT:
    {
        Log( " EVT_POST_EVENT" );
        const QByteArray& eventData = aEvent.Data();
        EVT_COMMON_MPAUTOTEST_EVENT_DATA_T* mpAutoTestData = ( EVT_COMMON_MPAUTOTEST_EVENT_DATA_T* )eventData.constData();
        Log( QString(" EVT_POST_EVENT - reasonEvent : %1").arg( mpAutoTestData->tHeader.reasonEvent) );
        switch(mpAutoTestData->tHeader.reasonEvent)
        {
            case EVT_MP_DAB_BER_RESULT_VALUE_REQ:
            {
               Log( " EVT_POST_EVENT : EVT_MP_DAB_BER_RESULT_VALUE_RES" );
               EVT_COMMON_MPAUTOTEST_EVENT_DATA_T param;
               memset(&param, 0, sizeof(EVT_COMMON_MPAUTOTEST_EVENT_DATA_T));
               param.tHeader.appIdReciever = APP_MPAUTOTEST;
               param.tHeader.reasonEvent = EVT_MP_DAB_BER_RESULT_VALUE_RES;
               param.tHeader.dataSize = sizeof(param);
               int value = dabController.getUIListener()->getCER();
               QString message = QString("%1").arg(value);
               int  iLength = message.length();
               param.tPayload.iLength = iLength;
               memcpy( param.tPayload.data, message.toLatin1().constData(), iLength );
               Event postEvent( EVT_POST_EVENT, GetThisAppId(), APP_MPAUTOTEST, EVT_MP_DAB_BER_RESULT_VALUE_RES, QByteArray( (char*)&param, sizeof( param ) ) );
               NotifyUISHCommon( postEvent );
               break;
            }
        }
        break;
    }

    case EVT_REQEUST_MUTE:
         Log( " EVT_REQEUST_MUTE " );
        emit cmdEventExceptionHandler(DAB_EVENT_MUTE);
        break;

    case EVT_REQEUST_UNMUTE:
        Log( " EVT_REQEUST_UNMUTE " );
        emit cmdEventExceptionHandler(DAB_EVENT_UNMUTE);
        break;

    case EVT_POWER_STATE_LOGIC_OFF:
        Log( " EVT_POWER_STATE_LOGIC_OFF " );
        qDebug() << " ============= EVT_POWER_STATE_LOGIC_OFF ";
        m_bPowerLogigOff = true;
        emit cmdEventExceptionHandler(DAB_EVENT_POWER_LOGIC_OFF);
        m_eDabCurrentStatus = DAB_BG_STATUS;
        break;

    case EVT_POWER_STATE_NORMAL_ON:
        Log( " EVT_POWER_STATE_NORMAL_ON " );
        qDebug() << " ============= EVT_POWER_STATE_NORMAL_ON ";
        m_bPowerLogigOff = false;
        emit cmdEventExceptionHandler(DAB_EVENT_POWER_NORMAL_ON);
        break;

    case EVT_NOTIFY_VR_STATUS:
        Log( QString(" EVT_NOTIFY_VR_STATUS - aEvent.GetCommand() : %1").arg(aEvent.GetCommand()) );
        qDebug() << " ============= EVT_NOTIFY_VR_STATUS ";
        if(aEvent.GetCommand())
        {
            m_bVRFlag = true;
            emit cmdEventExceptionHandler(DAB_EVENT_VR_START);
        }
        else
        {
            m_bVRFlag = false;
            int currentAVMode = checkAudioPath();
            if(currentAVMode == MODE_STATE_DAB1)
                emit cmdEventExceptionHandler(DAB_EVENT_VR_END);
        }
        break;

    case EVT_AV_MODE_NOTIFY_FROM_VR:
    {
        EVT_COMMON_BASE_EVENT_DATA_T* payload = ( EVT_COMMON_BASE_EVENT_DATA_T* )aEvent.Data().data();
        Log( QString(" EVT_AV_MODE_NOTIFY_FROM_VR - m_bVRFlag : %1, payload->iEventData01 : %2").arg(m_bVRFlag).arg(payload->iEventData01) );
        qDebug() << " ============= EVT_AV_MODE_NOTIFY_FROM_VR :: payload->iEventData01 = " << payload->iEventData01 << ", m_bVRFlag = " << m_bVRFlag;
        m_bVRFlag = false;
        if(payload->iEventData01 == MODE_STATE_DAB1 || payload->iEventData01 == MODE_STATE_DAB2 || payload->iEventData01 == MODE_STATE_DAB3)
        {
            emit cmdEventExceptionHandler(DAB_EVENT_NOTIFY_FROM_VR);
        }
        break;
    }
    default:
        Log (QString("== NotifyFromUIStateManagerCommon - Unknown EventID: %1").arg(aEvent.GetEventId() ));
        break;
    }
}

void
DABAppEngine::NotifyFromUIStateManagerGeneric( Event aEvent, DISPLAY_T display )
{
    Log(QString("NotifyFromUIStateManagerGeneric : aEvent = %1").arg(aEvent.GetEventId()));

    m_iDisplayMode = display;

    switch ( aEvent.GetEventId() )
    {

    case EVT_REQUEST_PRE_FG :
    {
        qDebug() << " ================= EVT_REQUEST_PRE_FG =================================== m_bDABForeground = " << m_bDABForeground;
        DABLogger::instance()->Log(QString("=============== EVT_REQUEST_PRE_FG =============================== m_bDABForeground=%1").arg(m_bDABForeground));
        QDeclarativeView* pView = this->GetCurrentView();
        if (pView)
        {
            qDebug() << " ================= EVT_REQUEST_PRE_FG =================================== pView->repaint();";
            DABLogger::instance()->Log(QString("=============== EVT_REQUEST_PRE_FG =============================== pView->repaint();"));
            pView->repaint();
        }
        break;
    }

    case EVT_REQUEST_FG:
    {
        qDebug() << " ================= EVT_REQUEST_FG =================================== m_bDABForeground = " << m_bDABForeground;
        DABLogger::instance()->Log(QString("=============== EVT_REQUEST_FG =============================== m_bDABForeground=%1").arg(m_bDABForeground));
        m_bPopupNone = false;
        m_bVRFlag = false;
        m_bMixFlag = false;
        if(m_bDABForeground)
        {
            return;
        }

        APP_ENTRY_POINT_DATA_T* payload = (APP_ENTRY_POINT_DATA_T *)aEvent.Data().data();
        if ( payload != NULL )
        {
            Log(QString("EVT_REQUEST_FG : payload->sAppEntryPoint = %1 m_eDabSystemPopupType = %2").arg(payload->sAppEntryPoint).arg(m_eDabSystemPopupType));
            qDebug() << "EVT_REQUEST_FG : payload->sAppEntryPoint = " << payload->sAppEntryPoint << ", m_eDabSystemPopupType = " << m_eDabSystemPopupType << ", m_eDabCurrentStatus = " << m_eDabCurrentStatus;

            QTimer::singleShot(500, this, SLOT(onDABAudioOpen())); 
            if(m_bStationListFromVR)
            {
                m_bStationListFromVR = false;
            }
            else
            {
                emit reqForegroundFromUISH(payload->sHeader.bTemporalMode);
            }

            switch( payload->sAppEntryPoint )
            {
                case eAPP_DAB_MODE_DAB1:
                    break;

                case eAPP_DAB_MODE_DAB2:
                    break;

    #ifdef __ENG_MODE__
                case eAPP_MODE_DYNAMICS_DAB:
                    emit dabEngineerMode();
                    break;
    #endif
                case eAPP_MODE_DYNAMICS_DAB_DEALER_MODE:
                    emit dabDealerMode();
                    break;

                default:
                    break;
            }
        }

        if ( display == DISPLAY_FRONT )
        {
            connect( this, SIGNAL(NotifyUISH(Event)), this, SLOT(doNotifyUISHFront(Event)));
        }
        else
        {
            connect( this, SIGNAL(NotifyUISH(Event)), this, SLOT(doNotifyUISHRear(Event)));
        }


        if(m_eDabCurrentStatus == DAB_BG_FM_STATUS)
            m_eDabCurrentStatus = DAB_FG_FM_STATUS;
        else
            m_eDabCurrentStatus = DAB_FG_STATUS;

        emit cmdSendOSDUpdate();
        m_pRdsAdaptor->writeAnnouncePopupStatus(true);

        if(m_eDabSystemPopupType == DAB_POPUP_TA_TYPE  || m_eDabSystemPopupType == DAB_POPUP_ALARM_TYPE)
        {
            emit displayTAPopup(m_sAnnouncementServiceName, m_AnnouncementFlags, true);
        }
        m_bDABForeground = true;
        m_bCCPLongPressed = false;
        m_bSeekLongPressed = false;
        break;
    }          

    case EVT_REQUEST_BG:
    {
        DABLogger::instance()->Log((QString(" ======================================== EVT_REQUEST_BG ========================================")));
        qDebug() << "== EVT_REQUEST_BG === m_bDABAudioOpen = " << m_bDABAudioOpen << ", m_eDabCurrentStatus = " << m_eDabCurrentStatus <<  ", m_bPowerLogigOff = " << m_bPowerLogigOff << ", m_eDabSystemPopupType = " << m_eDabSystemPopupType;
        emit cmdEventExceptionHandler(DAB_EVENT_BG);
        emit cmdReqSaveData();
        if(m_eDabCurrentStatus == DAB_FG_FM_STATUS)
            m_eDabCurrentStatus = DAB_BG_FM_STATUS;
        else
            m_eDabCurrentStatus = DAB_BG_STATUS;

        if(m_eDabSystemPopupType == DAB_POPUP_PRESERVE_TYPE && !m_bPowerLogigOff)
        {
            emit cmdCancelEPGReservation(true);
            m_eDabSystemPopupType = DAB_POPUP_NONE;
        }
        else if(m_eDabSystemPopupType == DAB_POPUP_TA_SEARCH_TYPE || m_eDabSystemPopupType == DAB_POPUP_TA_NO_SEARCH_TYPE)
        {
            m_eDabSystemPopupType = DAB_POPUP_NONE;
            dabController.StartTASearchTimer();
        }
        m_bDABForeground = false;
        m_bPopupNone = false;
        m_bCCPLongPressed = false;
        m_bSeekLongPressed = false;
        break;
    }

    case EVT_FRONT_LCD_ON:
        DABLogger::instance()->Log((QString(" ======================================== EVT_FRONT_LCD_ON ========================================")));
        qDebug() << "=============== EVT_FRONT_LCD_ON === m_bDABAudioOpen = " << m_bDABAudioOpen << ", m_eDabCurrentStatus = " << m_eDabCurrentStatus;
        break;

    case EVT_FRONT_LCD_OFF:
        DABLogger::instance()->Log((QString(" ======================================== EVT_FRONT_LCD_OFF ========================================")));
        qDebug() << "=============== EVT_FRONT_LCD_OFF === m_bDABAudioOpen = " << m_bDABAudioOpen << ", m_eDabCurrentStatus = " << m_eDabCurrentStatus;
        break;

    case EVT_SHOW_POPUP:
    {
        m_SystemShowingPopup = true;
        if ( aEvent.Data().size() > 0 )
        {
            POPUP_BASE_T *pEventData = (POPUP_BASE_T *)aEvent.Data().data();
            DABLogger::instance()->Log((QString(" ====== EVT_SHOW_POPUP :: pEventData->type = %1, m_eDabSystemPopupType = %2").arg(pEventData->type).arg(m_eDabSystemPopupType)));
            switch( pEventData->type )
            {
                case POPUP_WARNING:
                case POPUP_TEXT:
                {
                    emit cmdEventExceptionHandler(DAB_EVENT_CLOSE_OPTION_MENU);                    
                    if(m_bPopupNone)
                    {
                        if(m_eDabSystemPopupType == DAB_POPUP_TA_TYPE || m_eDabSystemPopupType == DAB_POPUP_ALARM_TYPE)
                        {
                            emit displayTAPopup(m_sAnnouncementServiceName, m_AnnouncementFlags, true);
                        }
                        m_bPopupNone = false;
                    }
                    break;
                }

                default:
                    break;
            }
        }
        checkEPGPopupVisible(); // common rule local popup close
        break;
    }

    case EVT_HIDE_POPUP:
        m_SystemShowingPopup = false;
        break;

    case EVT_REQUEST_APP_EXIT:
    {
        Log(" EVT_REQUEST_APP_EXIT - emit cmdReqSaveData()");
        m_eDabCurrentStatus = DAB_EXIT_STATUS;
        emit cmdReqSaveData();
        qApp->quit();
        break;
    }

    case EVT_POPUP_RESPONSE :
    {
        Log(" EVT_POPUP_RESPONSE ");
        qDebug() << "EVT_POPUP_RESPONSE : popup type = " << m_eDabSystemPopupType;
        POPUP_RESPONSE_T* pEventData = (POPUP_RESPONSE_T*)aEvent.Data().data();

        switch(pEventData->btn)
        {
        case POPUP_BUTTON_NONE:
            Log(" POPUP_BUTTON_NONE ");
            qDebug() << "=========== POPUP_BUTTON_NONE ====== m_eDabSystemPopupType = " << m_eDabSystemPopupType << ", m_bPowerLogigOff = " << m_bPowerLogigOff;
            if(m_eDabSystemPopupType == DAB_POPUP_PRESERVE_TYPE && !m_bPowerLogigOff)
            {
                emit cmdCancelEPGReservation(true);
                m_eDabSystemPopupType = DAB_POPUP_NONE;
            }

            if(m_eDabSystemPopupType == DAB_POPUP_TA_SEARCH_TYPE || m_eDabSystemPopupType == DAB_POPUP_TA_NO_SEARCH_TYPE)
            {
                m_eDabSystemPopupType = DAB_POPUP_NONE;
                dabController.StartTASearchTimer();
            }

            if(m_eDabSystemPopupType == DAB_POPUP_TA_TYPE || m_eDabSystemPopupType == DAB_POPUP_ALARM_TYPE)
            {
                m_bPopupNone = true;
            }
            break;

        case POPUP_BUTTON_TIMEOUT:
            Log(" POPUP_BUTTON_TIMEOUT ");
            break;

        case POPUP_BUTTON_0:
            Log(" POPUP_BUTTON_0 ");
            if(m_eDabSystemPopupType == DAB_POPUP_TA_TYPE)
            {
                qDebug() << " POPUP_BUTTON_0 : This Popup Type is Taffic Announcement  !!";
                emit cmdReqCancelTrafficAnnouncement(false);
                this->onReqTAStop();
            }
            else if(m_eDabSystemPopupType == DAB_POPUP_ALARM_TYPE)
            {
                qDebug() << " POPUP_BUTTON_0 : This Popup Type is Alarm Announcement  !!";
                emit cmdReqCancelAlarmAnnouncement();
                this->onReqAlarmStop();
            }
            else if(m_eDabSystemPopupType == DAB_POPUP_PRESERVE_TYPE)
            {
                qDebug() << " POPUP_BUTTON_0 : This Popup Type is PRESERVE POPUP !!";
                emit cmdReqSetReservationChannel();

                qDebug() << " ==============POPUP_BUTTON_0 : m_eDabCurrentStatus = " << m_eDabCurrentStatus;
                if(m_eDabCurrentStatus == DAB_BG_STATUS || m_eDabCurrentStatus == DAB_BG_FM_STATUS)
                {
                    qDebug() << "================= Sound Path Change is Requested!!!";
                    APP_ENTRY_POINT_DATA_T pEventData;
                    pEventData.sAppEntryPoint = eAPP_DAB_MODE_DAB1;

                    Event softEvent( EVT_EPG_REQUEST_FG, GetThisAppId(), APP_UI_MANAGER, QByteArray((char *)&pEventData, sizeof(pEventData)));
                    NotifyUISHCommon(softEvent);
                }
                else
                {
                    emit reqForegroundFromUISH(false);
                }
            }
            else if(m_eDabSystemPopupType == DAB_POPUP_TA_SEARCH_TYPE)
            {
                qDebug() << " POPUP_BUTTON_0 : This Popup Type is TA SEARCH POPUP !!";
                QObject *objectStationList = this->GetCurrentView()->rootObject()->findChild<QObject*>("DABPlayerMain");
                bool bDABPlayerMain = QDeclarativeProperty::read(objectStationList, "visible").toBool();
                if(!bDABPlayerMain)
                {
                    emit gotoMainScreen();
                }
                emit cmdReqTASearch();
            }
            else if(m_eDabSystemPopupType == DAB_POPUP_TA_NO_SEARCH_TYPE)
            {
                emit cmdReqTASetting(false); //2013.07.19 Mobilus DaehyungE ISV #87710 HMETC 요청으로 No TA Search 팝업에서 ok 선택할경우 TA OFF 되도록 변경
            }
            else
            {
                qDebug() << " POPUP_BUTTON_0 : This Popup Type is NONE !!";
            }
            m_eDabSystemPopupType = DAB_POPUP_NONE;
            break;

        case POPUP_BUTTON_1:
            Log(" POPUP_BUTTON_1 ");
            if(m_eDabSystemPopupType == DAB_POPUP_TA_TYPE)
            {
                qDebug() << " POPUP_BUTTON_1 : This Popup Type is Announcement  !!";
                emit cmdReqCancelTrafficAnnouncement(true);
                this->onReqTAStop();
            }
            else if(m_eDabSystemPopupType == DAB_POPUP_PRESERVE_TYPE)
            {
                qDebug() << " POPUP_BUTTON_1 : This Popup Type is PRESERVE POPUP !!";
                emit cmdCancelEPGReservation(true);
            }
            else if(m_eDabSystemPopupType == DAB_POPUP_TA_SEARCH_TYPE)
            {
                qDebug() << " POPUP_BUTTON_1 : This Popup Type is TA SEARCH POPUP !!";                
                emit cmdReqTASetting(false); //2013.07.19 Mobilus DaehyungE ISV #87710 HMETC 요청으로 TA Search 팝업에서 no 선택할경우 TA OFF 되도록 변경
            }
            else
            {
                qDebug() << " POPUP_BUTTON_1 : This Popup Type함 is NONE !!";
            }
            m_eDabSystemPopupType = DAB_POPUP_NONE;
            break;

        case POPUP_BUTTON_2:
            Log(" POPUP_BUTTON_2 ");
            break;

        case POPUP_BUTTON_3:
            Log(" POPUP_BUTTON_3 ");
            break;
        }
        break;
    }

    case EVT_DRS_SHOW :
    {
        Log(" EVT_DRS_SHOW ");
        emit drivingRegulation(true);
        break;
    }

    case EVT_DRS_HIDE:
    {
        Log(" EVT_DRS_HIDE ");
        emit drivingRegulation(false);
        break;
    }

    case EVT_KEY_HU_TA :
        break;

    case EVT_AV_ON:
        Log(" EVT_AV_ON ");
        emit cmdEventExceptionHandler(DAB_EVENT_AV_ON);
        break;

    default:
        Log (QString("== NotifyFromUIStateManager - Unknown EventID: %1").arg(aEvent.GetEventId() ));
        break;
    }
}

void
DABAppEngine::onRspTrafficRDStoDAB(bool onOff)
{
    qDebug() << " ============== onRspTrafficRDStoDAB :: m_eDabSystemPopupType = " << m_eDabSystemPopupType << ", onOff = " << onOff;
    Log (QString("== onRspTrafficRDStoDAB - m_eDabSystemPopupType : %1 TA onOff = %2").arg(m_eDabSystemPopupType).arg(onOff));

    if(m_eDabSystemPopupType == DAB_POPUP_TA_TYPE)
    {
        emit cmdReqCancelTrafficAnnouncement(true);
        this->onReqTAStop();
    }
    else if(m_eDabSystemPopupType == DAB_POPUP_ALARM_TYPE)
    {
        qDebug() << " ============== onRspTrafficRDStoDAB :: IF Alarm popup is display, TA key ignored !! ";
        emit cmdReqTA(onOff);
    }
    else if(m_eDabSystemPopupType == DAB_POPUP_TA_SEARCH_TYPE || m_eDabSystemPopupType == DAB_POPUP_TA_NO_SEARCH_TYPE)
    {
        Event event(EVT_REQUEST_SYSTEM_POPUP_CLOSE, GetThisAppId(), APP_UI_MANAGER);
        NotifyUISHCommon(event);
        m_eDabSystemPopupType = DAB_POPUP_NONE;
        emit cmdReqTA(onOff);
    }
    else
    {
        emit cmdReqTA(onOff);
    }
}

void
DABAppEngine::RetranslateUi()
{
    emit retranslateUi( (int)m_nLanguage);
}

void
DABAppEngine::setInitLanguage(int languageID)
{
    qDebug() << __FUNCTION__ << " ============= languageID = " << languageID;
    dabController.getUIListener()->initLanguageID(languageID);
}

QString
DABAppEngine::popScreen()
{
    if(mFlowStack.count() > 0)
    {
        return mFlowStack.pop();
    }
    else
    {
        return "DABPlayerMain";
    }
}

void
DABAppEngine::HandleBackKey(QString sInputMode)
{
    qDebug() << __FUNCTION__ << " ============= bTouchMode = " << sInputMode;
    if(sInputMode == "jog")
    {
        Event event( EVT_KEY_SOFT_BACK, GetThisAppId(), APP_UI_MANAGER );
        NotifyUISH(event);
    }
    else
    {
        Event event( EVT_TOUCH_BACK_KEY , GetThisAppId(), APP_UI_MANAGER );
        NotifyUISH(event);
    }
}

void
DABAppEngine::HandleHomeKey()
{
    Log( "DABAppEngine::HandleHomeKey: Start" );
    Event event( EVT_KEY_SOFT_HOME, GetThisAppId(), APP_UI_MANAGER );
    NotifyUISH(event);
}

void
DABAppEngine::HandleFMKey(void)
{
    Log( "DABAppEngine::HandleFMKey: Start" );
    Event event( EVT_KEY_SOFT_RADIO, GetThisAppId(), APP_UI_MANAGER);
    event.SetCommand(eAPP_RADIO_MODE_FM1);
    NotifyUISH(event);
    stopDABtoFMTimer();
}

void
DABAppEngine::HandleAMKey(void)
{
    Log( "DABAppEngine::HandleAMKey: Start" );
    Event event( EVT_KEY_SOFT_RADIO, GetThisAppId(), APP_UI_MANAGER);
    event.SetCommand(eAPP_RADIO_MODE_AM);
    NotifyUISH(event);
    stopDABtoFMTimer();
}

void
DABAppEngine::onReqDisplayOSD(QString str1, QString str2, quint8 op_type)
{
    Log(QString("DABAppEngine::onReqDisplayOSD : m_bDABAudioOpen ? = %1, op_type = %2, m_eDabCurrentStatus = %3").arg(m_bDABAudioOpen).arg(op_type).arg(m_eDabCurrentStatus));
    qDebug() << __FUNCTION__ << "m_eDabCurrentStatus = " << m_eDabCurrentStatus << ", serviceName = " << str1 << ", freqLabel = " << str2 << ", op_type = " << op_type;

    if((m_eDabCurrentStatus == DAB_BG_STATUS) || (m_eDabCurrentStatus == DAB_BG_FM_STATUS))
    {
        QString strOSD;
        strOSD.clear();

        switch(op_type)
        {
        case DAB_OP_TYPE_AVOFF:
            strOSD = "@OSD:F^COMMON^WARN^";
            strOSD = strOSD + str1;
            break;

        case DAB_OP_TYPE_SEEK:
            strOSD = "@OSD:F^COMMON^INFO^";
            strOSD = strOSD + str1;
            break;

        case DAB_OP_TYPE_INFO:
            strOSD = "@OSD:F^COMMON^INFO^";
            strOSD = strOSD + str1;
            break;

        case DAB_OP_TYPE_TA:
            strOSD = "@OSD:F^TA^INFO^DAB^";
            strOSD = strOSD + str1 +"^"+ str2;
            break;

        case DAB_OP_TYPE_EPG:
            strOSD = "@OSD:F^DAB_INFO^EPG^DAB^";
            strOSD = strOSD + str1 + "^" + QApplication::translate("StringInfo", "STR_DAB_EPG_NOW_PLAYING");
            strOSD = strOSD + "^" + str2;
            break;

        case DAB_OP_TYPE_EPG_STARTED:
            strOSD = "@OSD:F^DAB_INFO^EPG^DAB^";
            strOSD = strOSD + str1 + "^" + QApplication::translate("StringInfo", "STR_DAB_OSD_STARTED");
            strOSD = strOSD + "^" + str2;
            break;

        case DAB_OP_TYPE_SCANING:
            strOSD = "@OSD:F^DAB^INFO^DAB^";
            strOSD = strOSD + str1;
            strOSD = strOSD + "^(SE)";
            break;

        case DAB_OP_TYPE_SCAN:
            strOSD = "@OSD:F^DAB^INFO^DAB^";
            strOSD = strOSD + str2 +"^"+ str1;
            strOSD = strOSD + "^(SE)";
            break;

        case DAB_OP_TYPE_NOSIGNAL:
            strOSD = "@OSD:F^DAB^INFO^DAB^";
            strOSD = strOSD + str2 +"^"+ str1;
            strOSD = strOSD + "^" + QApplication::translate("StringInfo", "STR_DAB_PLAYER_NO_SIGNAL");
            break;

        case DAB_OP_TYPE_TUNE_SEEK:
            strOSD = "@OSD:F^DAB^INFO_DIAL^DAB^";
            strOSD = strOSD + str2 +"^"+ str1;
            break;

        case DAB_OP_TYPE_SELECT:
            strOSD = "@OSD:F^DAB^INFO^DAB^";
            strOSD = strOSD + str2 +"^"+ str1;
            break;
        case DAB_OP_TYPE_SEARCHING:
            strOSD = "@OSD:F^DAB^INFO_SEARCH^DAB^";
            strOSD = strOSD + QApplication::translate("StringInfo", "STR_DAB_PLAYER_SEARCHING");
            break;

        default:
            Log(QString("DABAppEngine::onReqDisplayOSD : Invalid OP TYPE = %1").arg(op_type));
            break;

        }

        Log(QString("DABAppEngine::onReqDisplayOSD : strOSD = %1").arg(strOSD));

        if(!m_bAgreeShowOSD)
        {
            Log("DABAppEngine::onReqDisplayOSD :: AGREE_SHOW_OSD Event not receive already. so recall after 1 second");
            m_sBackupOSD = strOSD;
            QTimer::singleShot(1000, this, SLOT(onOSDUpdate()));
        }
        else
        {
            OSDWriter::SendOSD(strOSD);
        }
    }
}

void
DABAppEngine::onOSDUpdate(void)
{
    if(!m_bAgreeShowOSD)
    {
        Log("DABAppEngine::onReqDisplayOSD :: AGREE_SHOW_OSD Event not receive already. so recall after 1 second");
        QTimer::singleShot(1000, this, SLOT(onOSDUpdate()));
    }
    else
    {
        OSDWriter::SendOSD(m_sBackupOSD);
    }
}

void
DABAppEngine::onDABAudioOpen(void){
    m_bDABAudioOpen = true;
    emit cmdEventExceptionHandler(DAB_EVENT_FG);
}

void
DABAppEngine::onReqClusterControl(QString serviceName, quint8 presetNumber, quint8 opState)
{
    Log( QString("onReqClusterControl ::  m_eDabCurrentStatus:%1").arg(m_eDabCurrentStatus) );
    Log(QString("Cluster OpState = %1, presetNumber = %2, serviceName = %3").arg(opState).arg(presetNumber).arg(serviceName));

    if(m_bCallFlag)
    {
        Log("onReqClusterControl :: Current Status is CALL_STATUS!!. so return !!");
        return;
    }

    int currentAVMode = checkAudioPath();

    if( (currentAVMode != MODE_STATE_DAB1) && (currentAVMode != MODE_STATE_DAB_FM))
    {
        qDebug() << __FUNCTION__ << " :: current audio path is other apps!!. so request cluster ignored";
        return;
    }

    quint64 frame = 0;
    frame = (opState & 0x7F);
    frame += (presetNumber & 0x1F) << 8;
    frame = frame | 0xFFFFFFFFFFFFE080;

    SQCanFrame qcanData;
    memset(&qcanData, 0x00, sizeof(qcanData));
    qcanData.uqcanid = 0x114;
    qcanData.uqcandlc = 0x08;
    memcpy( &qcanData.uqcandata, &frame, sizeof(quint64));
    emit Can_data_send(qcanData);

    unsigned char clu_buffer[60] = {0x00,};
    int nSIZE = 0;
    clu_buffer[nSIZE++] = 0x04;
    clu_buffer[nSIZE++] = 0xB7;

    QByteArray arrayStationName((const char*)(serviceName.utf16()), serviceName.size()*2);
    quint8 nLength = arrayStationName.length();
    clu_buffer[nSIZE++] = nLength;
    memcpy(clu_buffer+nSIZE, arrayStationName.data(), nLength);
    nSIZE += nLength;
    if(m_MicomClient->isValid())
    {
        QByteArray canTPdata((const char*)clu_buffer, nSIZE);
        m_MicomClient->LgeDBus_CANTPSendMsg(canTPdata);
    }

    unsigned char hu_buffer[60] = {0x00,};
    nSIZE = 0;
    //TP_CLU_DAB_HU
    hu_buffer[nSIZE++] = 0x04;
    hu_buffer[nSIZE++] = 0xA7;
    hu_buffer[nSIZE++] = nLength;

    memcpy(hu_buffer+nSIZE, arrayStationName.data(), nLength);
    nSIZE += nLength;
    if(m_MicomClient->isValid())
    {
        QByteArray canTPdata((const char*)hu_buffer, nSIZE);
        m_MicomClient->LgeDBus_CANTPSendMsg(canTPdata);
    }
}

#ifdef __RPMCLIENT__
void
DABAppEngine::audioCHsetResponse(TAudioMode tBAN, TAudioSink tSINK)
{
    Q_UNUSED(tBAN);
    Q_UNUSED(tSINK);
    Log("AudioChsetResponse called");
}
#endif

void
DABAppEngine::onReqDABSoundOn(void)
{
    Log(QString("DABAppEndgine::onReqDABSoundOn : m_eDabCurrentStatus = %1").arg(m_eDabCurrentStatus));
    qDebug() << __FUNCTION__ <<" : m_eDabCurrentStatus = " << m_eDabCurrentStatus;

#if 1
    int currentAVMode = checkAudioPath();
    if(currentAVMode == MODE_STATE_DAB_FM)
    {
        EVENT_AUDIO_PATH_REQUEST_DATA payload;
        payload.avState = MODE_STATE_DAB1;

        Event event(EVT_REQUEST_AUDIO_PATH, GetThisAppId(), APP_UI_MANAGER, QByteArray( (char *)&payload, sizeof(payload) ));
        NotifyUISHCommon(event);
    }
#else
    if(m_eDabCurrentStatus == DAB_BG_STATUS || m_eDabCurrentStatus == DAB_FG_FM_STATUS || m_eDabCurrentStatus == DAB_BG_FM_STATUS)
    {
        EVENT_AUDIO_PATH_REQUEST_DATA payload;
        payload.avState = MODE_STATE_DAB1;

        Event event(EVT_REQUEST_AUDIO_PATH, GetThisAppId(), APP_UI_MANAGER, QByteArray( (char *)&payload, sizeof(payload) ));
        NotifyUISHCommon(event);
    }
#endif
}

void
DABAppEngine::playAudioBeep(void)
{
#ifdef __RPMCLIENT__
    TRPMClientRequestStatus status = m_pRpmClient->RPMClientAudioBeepPlay(eBeepType0);
    switch(status)
    {
    case eNoError:
        Log(" RPMClientAcquire : eNoError");
        break;
    case eOutOfRange:
        Log(" RPMClientAcquire : eOutOfRange");
        break;
    case eBadArguement:
        Log(" RPMClientAcquire : eBadArguement");
        break;
    case eNotSupported:
        Log(" RPMClientAcquire : eNotSupported");
        break;
    case eFatalError:
        Log(" RPMClientAcquire : eFatalError");
        break;
    case eTimeout:
        Log(" RPMClientAcquire : eTimeout");
        break;
    case eNoResource:
        Log(" RPMClientAcquire : eNoResource");
        break;
    case eActiveCallInProgress:
        Log(" RPMClientAcquire : eActiveCallInProgress");
        break;
    case eGeneralError:
        Log(" RPMClientAcquire : eGeneralError");
        break;
    default:
        Log(" RPMClientAcquire : Unknown");
        break;
    }
#endif
}

void
DABAppEngine::setBlockedStation(bool onOff)
{
    qDebug() << Q_FUNC_INFO << " :: onOff = " << onOff;
    this->m_bBlockStation = onOff;
}

void
DABAppEngine::onSoundSetting(bool isjogMode)
{
    qDebug() << Q_FUNC_INFO << " :: isJogMode = " << isjogMode;
    APP_EVENT_DATA_BASE_T pEventData;
    pEventData.sIsJogEnabled = isjogMode;
    Event softEvent(EVT_KEY_SOFT_SETTINGS, GetThisAppId(), APP_UI_MANAGER, eAPP_SETTINGS_SOUND, QByteArray((char *)&pEventData, sizeof(pEventData)));
    emit NotifyUISH(softEvent, m_iDisplayMode);
    emit cmdEventExceptionHandler(DAB_EVENT_SOUND_SETTING);
}

void
DABAppEngine::onReqDisplayStatusBar(quint8 type, bool onOff)
{
    DABLogger::instance()->Log((QString(" onReqDisplayStatusBar() : type : %1, onOff : %2").arg(type).arg(onOff)));
    QString strOSD = NULL;
    strOSD.clear();

    switch(type)
    {
        case DAB_STATUS_BAR_TRAFFIC:
            if(onOff)
                strOSD = "@STA^TA^1";
            else
                strOSD = "@STA^TA^0";
            break;

        case DAB_STATUS_BAR_NEWS:
            if(onOff)
                strOSD = "@STA^NEWS^1";
            else
                strOSD = "@STA^NEWS^0";
            break;

        case DAB_STATUS_BAR_ALL:
            if(onOff)
                strOSD = "@STA^NEWS^1@STA^TA^1";
            else
                strOSD = "@STA^NEWS^0@STA^TA^0";
            break;
    }

    qDebug() << __FUNCTION__ << strOSD;
    DABLogger::instance()->Log((QString(" onReqDisplayStatusBar() : SendOSD :: strOSD : %1").arg(strOSD)));

    //GetCurrentView()->setWindowTitle(strOSD);
    OSDWriter::SendOSD(strOSD);
    QCoreApplication::flush();
}

void
DABAppEngine::PostKeyEvent(KEY_EVENT_DATA_T *message, Qt::Key key)
{
    if(message->keyStatus == KEY_STATUS_PRESSED){
        Log("    KEY_STATUS_PRESSED ===> ");
        QKeyEvent* event = new QKeyEvent(QEvent::KeyPress, key, Qt::NoModifier);
        qApp->postEvent(GetCurrentView(), event);
    }else if(message->keyStatus == KEY_STATUS_RELEASED){
        Log("                       <=== KEY_STATUS_RELEASED");
        QKeyEvent* event = new QKeyEvent(QEvent::KeyRelease, key, Qt::NoModifier);
        qApp->postEvent(GetCurrentView(), event);
    }else if(message->keyStatus == KEY_STATUS_PRESSED_LONG){
        Log("    KEY_STATUS_PRESSED_LONG ====> Do Nothing for Pressed Long!");
        QKeyEvent* event = new QKeyEvent(QEvent::KeyPress, key, Qt::ShiftModifier);
        qApp->postEvent(GetCurrentView(), event);
    }else if(message->keyStatus == KEY_STATUS_CANCEL){
        Log("                       <=== KEY_STATUS_CANCEL");
        QKeyEvent* event = new QKeyEvent(QEvent::KeyRelease, key, Qt::ShiftModifier);
        qApp->postEvent(GetCurrentView(), event);
    }
}

void
DABAppEngine::PostJogEvent(KEY_EVENT_DATA_T *message, EVT_COMMAND_T keyCount)
{
    Q_UNUSED(message);
    if(keyCount > 0xff)
    {
        qDebug() << Q_FUNC_INFO << "warring :: it may be lost some count";
    }

    unsigned short keyCountFlag = (unsigned short)keyCount;
    if(keyCount < 0)
    {
        keyCountFlag = (keyCount*-1) << 8 ;
    }

    qApp->postEvent(GetCurrentView(), new QKeyEvent(QEvent::KeyPress, keyCountFlag & 0x00ff ? Qt:: Key_Apostrophe: Qt:: Key_Semicolon, Qt::NoModifier, QString(""), false, keyCountFlag));
    qApp->processEvents(QEventLoop::AllEvents);
    qApp->postEvent(GetCurrentView(), new QKeyEvent(QEvent::KeyRelease, keyCountFlag & 0x00ff ? Qt:: Key_Apostrophe: Qt:: Key_Semicolon, Qt::NoModifier, QString(""), false, keyCountFlag));
    qApp->processEvents(QEventLoop::AllEvents);
}

void
DABAppEngine::onLgeDBus_DABPICodeRequest(quint16 playPICode)
{
    QByteArray in0;
    in0.resize(2);
    in0[0] = (playPICode >> 8) & 0x00FF;
    in0[1] = (playPICode & 0x00FF);

    if(m_DABtoFMClient->isValid())
    {
        qDebug() << "onLgeDBus_DABPICodeRequest : 0x" << hex << playPICode << " ========== Send PICode Request Success";
        m_DABtoFMClient->LgeDBus_DABPICodeRequest(in0);
    }
    else
    {
        qDebug() << "onLgeDBus_DABPICodeRequest : 0x" << hex << playPICode << " ========== Send PICode Request Fail";
    }
}


#ifdef __ENG_MODE__
void
DABAppEngine::onReqVersion(QString ver){
    qDebug() << Q_FUNC_INFO << " module version = " << ver;
    m_engClient->VersionInfo(EngModeIf::App_DAB, ver);
}

void
DABAppEngine::onReqDiagnosis(void)
{
    qDebug() << Q_FUNC_INFO;

    m_bFirstShort = true;
    m_bFirstOpen = true;
    m_bUartCheck = true;
    m_iAntennaShortCount = DTC_CHECK_COUNT;
    m_iAntennaOpenCount = DTC_CHECK_COUNT;
    m_iAntennaNormalCount = DTC_CHECK_COUNT;
    m_iAntennaOutBatteryCount = DTC_CHECK_COUNT;
    m_iUartGoodCount = DTC_CHECK_COUNT;
    m_iUartErrorCount = DTC_CHECK_COUNT;
    m_DtcSetState = 0x00;
    m_interAntennaTimer->start(200);
    m_UARTCheckTimer->start(2000);
}

void
DABAppEngine::onReqConnectivity(int in0){
    qDebug() << Q_FUNC_INFO;

    Q_UNUSED(in0);
    int gpio_value = -1;
    unsigned int ant_short_dab = 0;
    unsigned int ant_open_dab = 0;

    gpio_value = get_gpio(DH_GPIO_DMB_ANT_SHORT, &ant_short_dab);
    if(gpio_value == -1){
        qDebug() << __FUNCTION__ << " : [GPIO ERROR] DAB ANT SHORT";
        m_engClient->ConnectivityInfo(EngModeIf::DAB_ConnectInfo, EngModeIf::Unconnected);
        return;
    }
    gpio_value = get_gpio(DH_GPIO_DMB_ANT_OPEN, &ant_open_dab);
    if(gpio_value == -1){
        qDebug() << __FUNCTION__ << " : [GPIO ERROR] DAB ANT OPEN";
        m_engClient->ConnectivityInfo(EngModeIf::DAB_ConnectInfo, EngModeIf::Unconnected);
        return;
    }

    qDebug() << __FUNCTION__ << "ant_short_dab : " << ant_short_dab << endl;
    qDebug() << __FUNCTION__ << "ant_open_dab : " << ant_open_dab << endl;

    if((ant_short_dab == PIN_LOW) && (ant_open_dab == PIN_HIGH))
        m_engClient->ConnectivityInfo(EngModeIf::DAB_ConnectInfo, EngModeIf::Unconnected);
    else
        m_engClient->ConnectivityInfo(EngModeIf::DAB_ConnectInfo, EngModeIf::Connected);
}
#endif

void
DABAppEngine::onReqRadioEnd(int band, quint16 playPICode)
{
    Log(QString("onReqRadioEnd : band = %1, playPICode = %2").arg(band).arg(playPICode));
    qDebug() << " =============================== onReqRadioEnd : playPICode = " << playPICode;

    if(m_bCallFlag)
    {
        qDebug() << __FUNCTION__ << " : Current BT Call status !!. so DAB-FM ignore !!";
        return;
    }

    if(m_bVRFlag)
    {
        qDebug() << __FUNCTION__ << " : Current VR status !!. so DAB-FM ignore !!";
        return;
    }

    if(m_bMixFlag)
    {
        qDebug() << __FUNCTION__ << " : Current MIX Mode status !!. so DAB-FM ignore !!";
        return;
    }

    EVENT_AUDIO_PATH_REQUEST_DATA payload;
    payload.avState = MODE_STATE_DAB1;
    payload.sPlayPICode = playPICode;
    Event event(EVT_REQUEST_AUDIO_PATH, GetThisAppId(), APP_UI_MANAGER, QByteArray( (char *)&payload, sizeof(payload) ));
    NotifyUISHCommon(event);
    dabController.setServiveFollowingState(DAB_SERVICE_FOLLOWING_STATE_AUDIO_DAB_CHANGEING);
}

void
DABAppEngine::onReqDABtoFM(quint16 playPICode)
{
    Log(QString("onReqDABtoFM : playPICode = %1,  m_bDABAudioOpen = %2").arg(playPICode).arg(m_bDABAudioOpen));
    qDebug() << "=============================== onReqDABtoFM : playPICode = " << playPICode << " m_bDABAudioOpen = " << m_bDABAudioOpen;

    if(!m_bDABAudioOpen)
    {
        qDebug() << __FUNCTION__ << " : Current DAB Audio Path is not open. so DAB-FM ignore !!";
        return;
    }

    if(m_bCallFlag)
    {
        qDebug() << __FUNCTION__ << " : Current BT Call status !!. so DAB-FM ignore !!";
        return;
    }

    if(m_bVRFlag)
    {
        qDebug() << __FUNCTION__ << " : Current VR status !!. so DAB-FM ignore !!";
        return;
    }

    if(m_bMixFlag)
    {
        qDebug() << __FUNCTION__ << " : Current MIX Mode status !!. so DAB-FM ignore !!";
        return;
    }

    if(checkAudioPath() != MODE_STATE_DAB1)
    {
        qDebug() << __FUNCTION__ << " :: current audio path is not dab!!. so DAB-FM ignore";
        return;
    }

    stopDABtoFMTimer();

#if 1
    EVENT_AUDIO_PATH_REQUEST_DATA payload;
    payload.avState = MODE_STATE_DAB_FM;
    payload.sPlayPICode = playPICode;
    Event event(EVT_REQUEST_AUDIO_PATH, GetThisAppId(), APP_UI_MANAGER, QByteArray( (char *)&payload, sizeof(payload) ));
    NotifyUISHCommon(event);
#else
    EVENT_DAB_REQUEST_RADIO_DATA_T reqRadioPlayData;

    reqRadioPlayData.sBand = 0x01;  // FM : 0x01, AM : 0x02
    reqRadioPlayData.sPlayPICode = playPICode;
    dabController.setRadioPlayBand(0x01);   
    QByteArray payload;
    payload.setRawData((const char*)&reqRadioPlayData, sizeof(EVENT_DAB_REQUEST_RADIO_DATA_T));
    Event event( EVT_DAB_REQUEST_RADIO, GetThisAppId(), /*APP_UI_MANAGER*/APP_RADIO, payload );
    NotifyUISHCommon(event);
#endif
    dabController.setServiveFollowingState(DAB_SERVICE_FOLLOWING_STATE_AUDIO_FM_CHANGEING);
}

void
DABAppEngine::onReqEPGPreservePopup(QString title, QString serviceName)
{
    Log(QString("onReqEPGPreservePopup : serviceName = %1 title = %2").arg(serviceName).arg(title));
    qDebug() << __FUNCTION__ << " : service name = " << serviceName << ", title  = " << title;

    QString strMessage;
    QString strHTML1, strHTML2,strHTML3;
    QString strPopup = QApplication::translate("StringInfo", "STR_DAB_EPG_NOW_PLAYING");

    if(!isEnableSystemPopup())
    {
        Log(QString("onReqEPGPreservePopup : OSD Display"));
        this->onReqDisplayOSD(title, serviceName, DAB_OP_TYPE_EPG);
        emit cmdCancelEPGReservation(true);
        return;
    }

    EVENT_SYSTEM_POPUP_T InfoData;
    POPUP_SHOW_T PopUpData;

    memset(&InfoData, 0x00, sizeof(EVENT_SYSTEM_POPUP_T));
    memset(&PopUpData, 0x00, sizeof(POPUP_SHOW_T));

    QString strHide = QApplication::translate("StringInfo", "STR_DAB_EPG_POPUP_LISTEN");
    QString strCancel = QApplication::translate("StringInfo", "STR_DAB_EPG_POPUP_CANCEL");

    strHTML1 = "<center><span style='font-size:50pt'; style='color:#FAFAFA'>";
    strHTML2 = "<</span><span style='font-size:36pt';style='color:#D4D4D4'><br>";
    strHTML3 = strPopup + " </span></center>";
    strMessage = strHTML1 + "\""+ serviceName + "\"" + strHTML2 + strHTML3;

    strMessage.toWCharArray(PopUpData.popup.text.msg);
    PopUpData.popup.text.btn_count = 2;

    PopUpData.popup.text.btn[0] = POPUP_BUTTON_TYPE_TEXT;
    PopUpData.popup.text.btn[1] = POPUP_BUTTON_TYPE_TEXT;

    strHide.toWCharArray(PopUpData.popup.text.btn_txt[0]);
    strCancel.toWCharArray(PopUpData.popup.text.btn_txt[1]);

    PopUpData.type = POPUP_TEXT;
    PopUpData.source = GetThisAppId();
    PopUpData.target = APP_UI_MANAGER;

    InfoData.Popuptype = EPG_RADIO_POPUP;
    InfoData.Popupshow = PopUpData;
    InfoData.Display = DISPLAY_FRONT;

    Event event(EVT_REQUEST_SYSTEM_POPUP, GetThisAppId(),APP_UI_MANAGER, QByteArray( (char *)& InfoData, sizeof(InfoData) ));
    NotifyUISHCommon(event);

    m_eDabSystemPopupType = DAB_POPUP_PRESERVE_TYPE;
}

bool
DABAppEngine::isEnableSystemPopup(void)
{
//    QSharedMemory systemPopupSharedMem;
//    m_xSystemPopupSharedMem.setKey("UISHStatusInformation");
//    m_xSystemPopupSharedMem.attach();
//    m_xSystemPopupSharedMem.create(SYSTEM_OFFSET_TOTAL_SIZE + SYSTEM_POPUP_STATUS_SIZE);

    quint8 value[SYSTEM_OFFSET_TOTAL_SIZE + SYSTEM_POPUP_STATUS_SIZE + 1];

    m_xSystemPopupSharedMem.lock();
    memcpy(value, m_xSystemPopupSharedMem.data(), SYSTEM_OFFSET_TOTAL_SIZE + SYSTEM_POPUP_STATUS_SIZE + 1 );
    m_xSystemPopupSharedMem.unlock();

    int popupStatus = (*(value + SYSTEM_OFFSET_POPUP_STATUS));
    int btCallStatus = (*(value + SYSTEM_OFFSET_CALL_STATUS));

    Log(QString("isEnableSystemPopup : popupStatus ? = %1, btCallStatus = %2 ").arg(popupStatus).arg(btCallStatus));

    if(popupStatus || btCallStatus)
        return false;
    else
        return true;
}

int
DABAppEngine::checkAudioPath(void)
{
//    QSharedMemory systemPopupSharedMem;
//    m_xCurrentAVModeSharedMem.setKey("UISHStatusInformation");
//    m_xCurrentAVModeSharedMem.attach();
//    m_xCurrentAVModeSharedMem.create(SYSTEM_OFFSET_CURRENT_AV_MODE + SYSTEM_POPUP_STATUS_SIZE);

    quint8 value[SYSTEM_OFFSET_CURRENT_AV_MODE + SYSTEM_POPUP_STATUS_SIZE + 1];

    m_xCurrentAVModeSharedMem.lock();
    memcpy(value, m_xCurrentAVModeSharedMem.data(), SYSTEM_OFFSET_CURRENT_AV_MODE + SYSTEM_POPUP_STATUS_SIZE + 1 );
    m_xCurrentAVModeSharedMem.unlock();

    int currentAVMode = (*(value + SYSTEM_OFFSET_CURRENT_AV_MODE));

    Log(QString("checkAudioPath : currentAVMode ? = %1").arg(currentAVMode));
    qDebug() << __FUNCTION__ << " ================ current av mode = " << currentAVMode;
    return currentAVMode;
}

void
DABAppEngine::onReqTAStart(quint16 flag, QString serviceName)
{
    Log(QString("============ TA START =========== : flag = %1, serviceName = %2").arg(flag).arg(serviceName));
    qDebug() << " ============ TA START ========== : flag = " << hex << flag << ", service name = " << serviceName;

    QString strAnnouncement = QApplication::translate("StringInfo", "STR_DAB_TA_POPUP_TRAFFIC_ANNOUNCEMENT");
    QString strHTML1 = "<center><span style='font-size:50pt'; style='color:#FAFAFA'>";
    QString strHTML2 = "</span><span style='font-size:36pt';style='color:#D4D4D4'><br>";
    QString strHTML3 = strAnnouncement + "</span></center>";
    QString strMessage = strHTML1 + serviceName + strHTML2 + strHTML3;

    QString strCancel = QApplication::translate("StringInfo", "STR_DAB_EPG_POPUP_CANCEL");
    QString strOff    = QApplication::translate("StringInfo", "STR_DAB_TA_POPUP_TA_OFF");    

    if(!m_pRdsAdaptor->readAnnouncePopupStatus())
    {
        qDebug() << __FUNCTION__ << " :: RDS POPUP is starting!!. so DAB Popup ignored";
        emit cmdReqCancelTrafficAnnouncement(false);
        emit cmdEventExceptionHandler(DAB_EVENT_ANNOUNCEMENT_STOP);
        return;
    }

    int currentAVMode = checkAudioPath();
    if(!(currentAVMode == MODE_STATE_DAB1 || currentAVMode == MODE_STATE_AVOFF))
    {
        qDebug() << __FUNCTION__ << " :: current audio path is other apps!!. so TA Popup ignored";
        emit cmdReqCancelTrafficAnnouncement(false);
        emit cmdEventExceptionHandler(DAB_EVENT_ANNOUNCEMENT_STOP);
        return;
    }

    if(m_bCallFlag)
    {
        qDebug() << __FUNCTION__ << " :: current status is BT CALL!!. so TA Popup ignored";
        emit cmdReqCancelTrafficAnnouncement(false);
        emit cmdEventExceptionHandler(DAB_EVENT_ANNOUNCEMENT_STOP);
        return;
    }

    if(m_bVRFlag)
    {
        qDebug() << __FUNCTION__ << " :: current status is VR Mode!!. so TA Popup ignored";
        emit cmdReqCancelTrafficAnnouncement(false);
        emit cmdEventExceptionHandler(DAB_EVENT_ANNOUNCEMENT_STOP);
        return;
    }

    if(m_bMixFlag)
    {
        qDebug() << __FUNCTION__ << " :: current status is MIX Mode!!. so TA Popup ignored";
        emit cmdReqCancelTrafficAnnouncement(false);
        emit cmdEventExceptionHandler(DAB_EVENT_ANNOUNCEMENT_STOP);
        return;
    }

    if(!m_bDABAudioOpen)
    {
        emit cmdReqCancelTrafficAnnouncement(false);
        emit cmdEventExceptionHandler(DAB_EVENT_ANNOUNCEMENT_STOP);
    }

    if(!isEnableSystemPopup())
    {
        this->onReqDisplayOSD(strAnnouncement, serviceName, DAB_OP_TYPE_TA);
//        return;
    }

    EVENT_SYSTEM_POPUP_T InfoData ;
    POPUP_SHOW_T PopUpData;
    memset(&InfoData, 0x00, sizeof(EVENT_SYSTEM_POPUP_T));
    memset(&PopUpData, 0x00, sizeof(POPUP_SHOW_T));

    strMessage.toWCharArray(PopUpData.popup.text.msg);

    PopUpData.popup.text.btn_count = 2;

    PopUpData.popup.text.btn[0] = POPUP_BUTTON_TYPE_TEXT;
    PopUpData.popup.text.btn[1] = POPUP_BUTTON_TYPE_TEXT;

    strCancel.toWCharArray(PopUpData.popup.text.btn_txt[0]);
    strOff.toWCharArray(PopUpData.popup.text.btn_txt[1]);

    PopUpData.type = POPUP_TEXT;
    PopUpData.source = GetThisAppId();
    PopUpData.target = APP_UI_MANAGER;

    InfoData.Popuptype = TA_POPUP_FROM_DAB;
    InfoData.Popupshow = PopUpData;
    InfoData.Display = DISPLAY_FRONT;

    Event event(EVT_REQUEST_SYSTEM_POPUP, GetThisAppId(),APP_UI_MANAGER, QByteArray( (char *)& InfoData, sizeof(InfoData) ));

    NotifyUISHCommon(event);
    m_eDabSystemPopupType = DAB_POPUP_TA_TYPE;
    m_AnnouncementFlags = flag;
    m_sAnnouncementServiceName = serviceName;
    dabController.setDeactiveDABtoFM(true);
}

void
DABAppEngine::onReqTAStop(void)
{
    qDebug() << " ==================TA STOP====START===== m_eDabSystemPopupType = " << m_eDabSystemPopupType;
    Log(QString("============ TA STOP =========== : m_eDabSystemPopupType = %1").arg(m_eDabSystemPopupType));

    if(m_eDabSystemPopupType != DAB_POPUP_TA_TYPE)
    {
        dabController.setDeactiveDABtoFM(false);
        m_eDabSystemPopupType = DAB_POPUP_NONE;
        qDebug() << __FUNCTION__ << " :: System Popup is not TA Popup. so TA Stop command ignored";
        return;
    }

    EVENT_SYSTEM_POPUP_T InfoData ;
    POPUP_SHOW_T PopUpData;

     memset(&InfoData, 0x00, sizeof(EVENT_SYSTEM_POPUP_T));
     memset(&PopUpData, 0x00, sizeof(POPUP_SHOW_T));

     PopUpData.type = POPUP_TEXT;
     PopUpData.source = GetThisAppId();
     PopUpData.target = APP_UI_MANAGER;

     InfoData.Popuptype = TA_POPUP_FROM_DAB;
     InfoData.Popupshow = PopUpData;
     InfoData.Display = DISPLAY_FRONT;

     Event event(EVT_TA_DAB_END, GetThisAppId(), APP_UI_MANAGER, QByteArray( (char *)& InfoData, sizeof(InfoData) ));
     NotifyUISHCommon(event);

     m_AnnouncementFlags = ANN_ALARM_ON;
     m_eDabSystemPopupType = DAB_POPUP_NONE;
     m_sAnnouncementServiceName = "";
     emit displayTAPopup(m_sAnnouncementServiceName, m_AnnouncementFlags, false);
     dabController.setDeactiveDABtoFM(false);
     dabController.setTAInterrupt(false);
}

void
DABAppEngine::onReqAlarmStart(quint16 flag, QString serviceName)
{
    Log(QString("============ ALARM START =========== : flag = #1, serviceName = %2").arg(flag).arg(serviceName));
    qDebug() << " ============ ALARM START ========== : flag = " << hex << flag << ", service name = " << serviceName;

    QString strAnnouncement = QApplication::translate("StringInfo", "STR_DAB_PTY_ALARM");
    QString strHTML1 = "<center><span style='font-size:50pt'; style='color:#FAFAFA'>";
    QString strHTML2 = "</span><span style='font-size:36pt';style='color:#D4D4D4'><br>";
    QString strHTML3 = strAnnouncement + "</span></center>";
    QString strMessage = strHTML1 + serviceName + strHTML2 + strHTML3;
    QString strCancel = QApplication::translate("StringInfo", "STR_DAB_EPG_POPUP_CANCEL");

    if(!m_pRdsAdaptor->readAnnouncePopupStatus())
    {
        qDebug() << __FUNCTION__ << " :: RDS POPUP is starting!!. so DAB Popup ignored";
        emit cmdReqCancelAlarmAnnouncement();
        emit cmdEventExceptionHandler(DAB_EVENT_ANNOUNCEMENT_STOP);
        return;
    }

    int currentAVMode = checkAudioPath();
    if(!(currentAVMode == MODE_STATE_DAB1 || currentAVMode == MODE_STATE_AVOFF))
    {
        qDebug() << __FUNCTION__ << " :: current audio path is other apps!!. so TA Popup ignored";
        emit cmdReqCancelAlarmAnnouncement();
        emit cmdEventExceptionHandler(DAB_EVENT_ANNOUNCEMENT_STOP);
        return;
    }

    if(m_bCallFlag)
    {
        qDebug() << __FUNCTION__ << " :: current status is BT CALL!!. so TA Popup ignored";
        emit cmdReqCancelAlarmAnnouncement();
        emit cmdEventExceptionHandler(DAB_EVENT_ANNOUNCEMENT_STOP);
        return;
    }

    if(m_bVRFlag)
    {
        qDebug() << __FUNCTION__ << " :: current status is VR Mode!!. so TA Popup ignored";
        emit cmdReqCancelAlarmAnnouncement();
        emit cmdEventExceptionHandler(DAB_EVENT_ANNOUNCEMENT_STOP);
        return;
    }

    if(m_bMixFlag)
    {
        qDebug() << __FUNCTION__ << " :: current status is MIX Mode!!. so TA Popup ignored";
        emit cmdReqCancelAlarmAnnouncement();
        emit cmdEventExceptionHandler(DAB_EVENT_ANNOUNCEMENT_STOP);
        return;
    }


    if(!isEnableSystemPopup())
    {
        this->onReqDisplayOSD(strAnnouncement, serviceName, DAB_OP_TYPE_TA);
    }

    EVENT_SYSTEM_POPUP_T InfoData ;
    POPUP_SHOW_T PopUpData;
    memset(&InfoData, 0x00, sizeof(EVENT_SYSTEM_POPUP_T));
    memset(&PopUpData, 0x00, sizeof(POPUP_SHOW_T));

    strMessage.toWCharArray(PopUpData.popup.text.msg);
    PopUpData.popup.text.btn_count = 1;
    PopUpData.popup.text.btn[0] = POPUP_BUTTON_TYPE_TEXT;

    strCancel.toWCharArray(PopUpData.popup.text.btn_txt[0]);

    PopUpData.type = POPUP_TEXT;
    PopUpData.source = GetThisAppId();
    PopUpData.target = APP_UI_MANAGER;

    InfoData.Popuptype = ePOPUP_DAB_TA_ALARM;
    InfoData.Popupshow = PopUpData;
    InfoData.Display = DISPLAY_FRONT;

    Event event(EVT_REQUEST_SYSTEM_POPUP, GetThisAppId(),APP_UI_MANAGER, QByteArray( (char *)& InfoData, sizeof(InfoData) ));
    NotifyUISHCommon(event);

    m_eDabSystemPopupType = DAB_POPUP_ALARM_TYPE;
    m_AnnouncementFlags = flag;
    m_sAnnouncementServiceName = serviceName;
}

void
DABAppEngine::onReqAlarmStop(void)
{
    qDebug() << " ==================ALARM STOP===== m_eDabSystemPopupType = " << m_eDabSystemPopupType;
    Log(QString(" ==================ALARM STOP===== : m_eDabSystemPopupType = %1").arg(m_eDabSystemPopupType));

    if(m_eDabSystemPopupType != DAB_POPUP_ALARM_TYPE)
    {
        m_eDabSystemPopupType = DAB_POPUP_NONE;
        qDebug() << __FUNCTION__ << " :: System Popup is not Alarm Popup. so Alarm Stop command ignored";
        return;
    }

    EVENT_SYSTEM_POPUP_T InfoData ;
    POPUP_SHOW_T PopUpData;

     memset(&InfoData, 0x00, sizeof(EVENT_SYSTEM_POPUP_T));
     memset(&PopUpData, 0x00, sizeof(POPUP_SHOW_T));

     PopUpData.type = POPUP_TEXT;
     PopUpData.source = GetThisAppId();
     PopUpData.target = APP_UI_MANAGER;

     InfoData.Popuptype = ePOPUP_DAB_TA_ALARM;
     InfoData.Popupshow = PopUpData;
     InfoData.Display = DISPLAY_FRONT;

     Event event(EVT_TA_DAB_END, GetThisAppId(), APP_UI_MANAGER, QByteArray( (char *)& InfoData, sizeof(InfoData) ));
     NotifyUISHCommon(event);

     m_AnnouncementFlags = ANN_ALARM_ON;
     m_eDabSystemPopupType = DAB_POPUP_NONE;
     m_sAnnouncementServiceName = "";
     emit displayTAPopup(m_sAnnouncementServiceName, m_AnnouncementFlags, false);
     dabController.setDeactiveDABtoFM(false);
     dabController.setTAInterrupt(false);
}

#ifdef __AUTO_TEST__
void
DABAppEngine::autoTest_athenaSendObject(void)
{
    athenaSendObject();
}
#endif

void
DABAppEngine::Log( const QString &aMessage)
{
#ifdef __UNIX_DESKTOP__
    return;
#else
#ifdef __DAB_APPENGINE_LOG__
    //m_tsLog << aMessage << endl;
    DABLogger::instance()->Log(aMessage);
#endif
    GetLogger().Log(aMessage);
#endif
//    qDebug() << aMessage;
}

void
DABAppEngine::closeSystemPopup(eDAB_SYSTEM_POPUP_TYPE m_ePopupType)
{
    Log(QString("closeSystemPopup : m_eDabSystemPopupType = %1").arg(m_ePopupType));
    qDebug() << __FUNCTION__ << " m_ePopupType = " << m_ePopupType;

    switch(m_ePopupType)
    {
        case DAB_POPUP_TA_TYPE:
        case DAB_POPUP_ALARM_TYPE:
            break;

        case DAB_POPUP_PRESERVE_TYPE:
        {
            emit cmdCancelEPGReservation(true);
            Event event(EVT_REQUEST_SYSTEM_POPUP_CLOSE, GetThisAppId(), APP_UI_MANAGER);
            NotifyUISHCommon(event);
            m_eDabSystemPopupType = DAB_POPUP_NONE;
        }
            break;

        case DAB_POPUP_TA_SEARCH_TYPE:
        case DAB_POPUP_TA_NO_SEARCH_TYPE:
        {
            Event event(EVT_REQUEST_SYSTEM_POPUP_CLOSE, GetThisAppId(), APP_UI_MANAGER);
            NotifyUISHCommon(event);
            dabController.StartTASearchTimer();
            m_eDabSystemPopupType = DAB_POPUP_NONE;
        }
            break;

        default:
            qDebug() << __FUNCTION__ << " This Popup Type is NONE !!";
            break;
    }
}

void
DABAppEngine::startDABtoFMTimer(void)
{
    qDebug() << Q_FUNC_INFO;
    if(m_DABtoFMTimer->isActive())
    {
        qDebug() << __FUNCTION__ << " DABtoFM Timer is running !!";
    }
    else
    {
        dabController.setRequestDABtoFM(false);
        dabController.initPICodeValue();
        m_DABtoFMTimer->start(1000);
    }
}

void
DABAppEngine::stopDABtoFMTimer(void)
{
    qDebug() << Q_FUNC_INFO;

    if(m_DABtoFMTimer->isActive())
    {
        qDebug() << " DABtoFM Timer Stop..";
        dabController.setRequestDABtoFM(false);
        m_DABtoFMTimer->stop();
    }
}

void
DABAppEngine::onReqTASearchPopup(bool onOff)
{
    qDebug() << Q_FUNC_INFO << ", onOff = " << onOff;

    if(m_eDabSystemPopupType == DAB_POPUP_TA_SEARCH_TYPE)
    {
        qDebug() << __FUNCTION__ << " :  TA Search Popup is already running!! so. return !!";
        return;
    }

    if(!isEnableSystemPopup())
    {
        qDebug() << __FUNCTION__ << " :  Current App is Standby or Camera!! so. return !!";
        return;
    }
#if 1
    int currentAVMode = checkAudioPath();
    if(!(currentAVMode == MODE_STATE_DAB1 || currentAVMode == MODE_STATE_DAB2 || currentAVMode == MODE_STATE_DAB3))
    {
        qDebug() << __FUNCTION__ << " :: current audio path is other apps!!. so TA Popup ignored";
        Log(QString("onReqTASearchPopup :: current audio path is other apps!!. so TA Popup ignored"));
        return;
    }
#else
    if(!checkAudioPath())
    {
        qDebug() << __FUNCTION__ << " :: current audio path is other apps!!. so TA Popup ignored";
        return;
    }
#endif

    QString strMessage, strHTML1;
    EVENT_SYSTEM_POPUP_T InfoData;
    POPUP_SHOW_T PopUpData;

    memset(&InfoData, 0x00, sizeof(EVENT_SYSTEM_POPUP_T));
    memset(&PopUpData, 0x00, sizeof(POPUP_SHOW_T));

    strHTML1 = "<span style='font-size:32pt'; style='color:#FAFAFA'>";

    if(onOff)
    {
        QString strYes = QApplication::translate("StringInfo", "STR_DAB_EPG_POPUP_YES");
        QString strNo = QApplication::translate("StringInfo", "STR_DAB_EPG_POPUP_NO");
        QString strBody = QApplication::translate("StringInfo", "STR_DAB_TA_POPUP_TUNE_TRAFFIC_STATION");
        strMessage = strHTML1 + strBody;
        strMessage.toWCharArray(PopUpData.popup.text.msg);
        PopUpData.popup.text.btn_count = 2;
        PopUpData.popup.text.btn[0] = POPUP_BUTTON_TYPE_TEXT;
        PopUpData.popup.text.btn[1] = POPUP_BUTTON_TYPE_TEXT;

        strYes.toWCharArray(PopUpData.popup.text.btn_txt[0]);
        strNo.toWCharArray(PopUpData.popup.text.btn_txt[1]);
        m_eDabSystemPopupType = DAB_POPUP_TA_SEARCH_TYPE;
    }
    else
    {
        QString strOk = QApplication::translate("StringInfo", "STR_DAB_EPG_POPUP_OK");
        QString strBody = QApplication::translate("StringInfo", "STR_DAB_TA_POPUP_NO_TRAFFIC_STATION");
        strMessage = strHTML1 + strBody;
        strMessage.toWCharArray(PopUpData.popup.text.msg);
        PopUpData.popup.text.btn_count = 1;
        PopUpData.popup.text.btn[0] = POPUP_BUTTON_TYPE_TEXT;
        PopUpData.popup.text.btn[1] = POPUP_BUTTON_TYPE_TEXT;

        strOk.toWCharArray(PopUpData.popup.text.btn_txt[0]);
        m_eDabSystemPopupType = DAB_POPUP_TA_NO_SEARCH_TYPE;
    }

    PopUpData.type = POPUP_TEXT;
    PopUpData.source = GetThisAppId();
    PopUpData.target = APP_UI_MANAGER;

    InfoData.Popuptype = /*CURRENT_POPUP_NONE*/COMMON_POPUP_HANDLE;
    InfoData.Popupshow = PopUpData;
    InfoData.Display = DISPLAY_FRONT;

    Event event(EVT_REQUEST_SYSTEM_POPUP, GetThisAppId(),APP_UI_MANAGER, QByteArray( (char *)& InfoData, sizeof(InfoData) ));
    NotifyUISHCommon(event);    
}

void
DABAppEngine::checkEPGPopupVisible(void)
{
    QObject *objectEPGDescPopup = this->GetCurrentView()->rootObject()->findChild<QObject*>("EPGDescPopup");
    bool isEPGDescPopup = QDeclarativeProperty::read(objectEPGDescPopup, "visible").toBool();

    if(isEPGDescPopup)
    {
        emit closeEPGDescPopup();
        return;
    }

    QObject *objectEPGFullPopup = this->GetCurrentView()->rootObject()->findChild<QObject*>("EPGFullPopup");
    bool isEPGFullPopup = QDeclarativeProperty::read(objectEPGFullPopup, "visible").toBool();

    if(isEPGFullPopup)
    {
        emit closeEPGFullPopup();
        return;
    }

    QObject *objectEPGDateListPopup = this->GetCurrentView()->rootObject()->findChild<QObject*>("EPGDateListPopup");
    bool isEPGDateListPopup = QDeclarativeProperty::read(objectEPGDateListPopup, "visible").toBool();

    if(isEPGDateListPopup)
    {
        emit closeEPGDateListPopup();
        return;
    }
}

bool
DABAppEngine::eventFilter(QObject *pFromObj, QEvent *pEvent)
{
    if ( pEvent->type() == ( QEvent::User - 15 ) )
    {
        if(m_SystemShowingPopup)
        {
            pEvent->accept();
            PlayBeep();
        }else{
            return QObject::eventFilter( pFromObj, pEvent );
       }
    }else{ // other case, pass event to AFW
        return AppEngineBase::eventFilter( pFromObj, pEvent );
    }
    return true;
}

void
DABAppEngine::onReqSendMuteCancel(void)
{
    EVENT_AUDIO_PATH_REQUEST_DATA payload;
    payload.avState = MODE_STATE_DAB1;

    Event event(EVT_MUTE_CANCEL, GetThisAppId(), APP_UI_MANAGER, QByteArray( (char *)&payload, sizeof(payload) ));
    NotifyUISHCommon(event);
}

void
DABAppEngine::setConnectUISH(void){
    Log(QString("setConnectUISH "));
     Event event( EVT_APP_IS_READY_TO_PLAY, GetThisAppId(), APP_UI_MANAGER );
     NotifyUISHCommon( event );
}

#ifdef __DTC__
void
DABAppEngine::onUartModuleCheck(void)
{
    if(dabController.m_AliveUartMsg)
    {
        m_iUartGoodCount--;
        m_iUartErrorCount = 5;
        if(m_iUartGoodCount == 0)
        {
            if(this->m_DtcSetState & DTC_CMD_MODULE_UART_ERROR || m_bUartCheck)
            {
                sendDTCMessage(DTC_CMD_MODULE_UART_ERROR, DTC_CLEAR);
                this->m_DtcSetState = (this->m_DtcSetState | DTC_CMD_MODULE_UART_ERROR) ^ DTC_CMD_MODULE_UART_ERROR;
                m_bUartCheck = false;
            }
            m_iUartGoodCount = 5;
        }
    }
    else
    {
        m_iUartErrorCount--;
        m_iUartGoodCount = 5;

        if(m_iUartErrorCount == 0)
        {
            if((!(this->m_DtcSetState & DTC_CMD_MODULE_UART_ERROR)) || m_bUartCheck)
            {
                sendDTCMessage(DTC_CMD_MODULE_UART_ERROR, DTC_ERROR);
                m_bUartCheck = false;
            }
            m_iUartErrorCount = 5;
        }
    }

    dabController.m_AliveUartMsg = false;
}

void
DABAppEngine::onExterAntenaCheck(void)
{
    int gpio_open_value = -1;
    int gpio_short_value = -1;
    unsigned int ant_short_dab = 0;
    unsigned int ant_open_dab = 0;

    gpio_short_value = get_gpio(GET_DMB_ANT_SHORT_VALUE, &ant_short_dab);
    gpio_open_value = get_gpio(GET_DMB_ANT_OPEN_VALUE, &ant_open_dab);

    if(gpio_short_value == -1 || gpio_open_value == -1)
    {
        qDebug() << __FUNCTION__ << ": GIPO ERROR !!!";
        return;
    }

    if(ant_short_dab == PIN_HIGH && ant_open_dab == PIN_HIGH)     //Normal State
    {
        m_iAntennaNormalCount--;
        m_iAntennaOutBatteryCount = DTC_EXT_CHECK_COUNT;
        m_iAntennaShortCount = DTC_EXT_CHECK_COUNT;
        m_iAntennaOpenCount = DTC_EXT_CHECK_COUNT;

        if(m_iAntennaNormalCount == 0)
        {
            sendDTCMessage(DTC_CMD_EXT_ANTENNA_OPEN, DTC_CLEAR);
            sendDTCMessage(DTC_CMD_ANTENNA_OPEN, DTC_CLEAR);
            sendDTCMessage(DTC_CMD_ANTENNA_SHORT, DTC_CLEAR);
            this->m_DtcSetState = (this->m_DtcSetState | 0x03) ^ 0x03; // DTC Clear
            this->onEndExternalDTC();
        }
    }
    else if(ant_short_dab == PIN_LOW && ant_open_dab == PIN_HIGH)     //Short State
    {
        m_iAntennaShortCount--;
        m_iAntennaOutBatteryCount = DTC_EXT_CHECK_COUNT;
        m_iAntennaOpenCount = DTC_EXT_CHECK_COUNT;
        m_iAntennaNormalCount = DTC_EXT_CHECK_COUNT;

        if(m_iAntennaShortCount == 0)
        {
            sendDTCMessage(DTC_CMD_EXT_ANTENNA_OPEN, DTC_CLEAR);
            sendDTCMessage(DTC_CMD_ANTENNA_SHORT, DTC_ERROR);
            this->onEndExternalDTC();
        }
    }
    else if(ant_short_dab == PIN_HIGH && gpio_open_value == PIN_LOW)     //Open State
    {
        m_iAntennaOpenCount--;
        m_iAntennaOutBatteryCount = DTC_EXT_CHECK_COUNT;
        m_iAntennaShortCount = DTC_EXT_CHECK_COUNT;
        m_iAntennaNormalCount = DTC_EXT_CHECK_COUNT;

        if(m_iAntennaOpenCount == 0)
        {
            sendDTCMessage(DTC_CMD_ANTENNA_OPEN, DTC_ERROR);
            sendDTCMessage(DTC_CMD_EXT_ANTENNA_OPEN, DTC_ERROR);
            this->onEndExternalDTC();
        }
    }
    else if(ant_short_dab == PIN_LOW && gpio_open_value == PIN_LOW)     //Out of Battery State
    {
        m_iAntennaOutBatteryCount--;
        m_iAntennaShortCount = DTC_EXT_CHECK_COUNT;
        m_iAntennaOpenCount = DTC_EXT_CHECK_COUNT;
        m_iAntennaNormalCount = DTC_EXT_CHECK_COUNT;
        if(m_iAntennaOutBatteryCount == 0)
        {
            sendDTCMessage(DTC_CMD_ANTENNA_OPEN, DTC_ERROR);
            sendDTCMessage(DTC_CMD_ANTENNA_SHORT, DTC_ERROR);
            sendDTCMessage(DTC_CMD_EXT_ANTENNA_OPEN, DTC_ERROR);
            this->onEndExternalDTC();
        }
    }
}

void
DABAppEngine::onInterAntenaCheck(void) //boot only
{
    int gpio_open_value = -1;
    int gpio_short_value = -1;
    unsigned int ant_short_dab = 0;
    unsigned int ant_open_dab = 0;

    gpio_short_value = get_gpio(GET_DMB_ANT_SHORT_VALUE, &ant_short_dab);
    gpio_open_value = get_gpio(GET_DMB_ANT_OPEN_VALUE, &ant_open_dab);

    if(gpio_short_value == -1 || gpio_open_value == -1)
    {
        qDebug() << __FUNCTION__ << ": GIPO ERROR !!!";
        return;
    }

    if(ant_short_dab == PIN_HIGH && ant_open_dab == PIN_HIGH)     //Normal State
    {
        m_iAntennaNormalCount--;
        m_iAntennaOutBatteryCount = DTC_CHECK_COUNT;
        m_iAntennaShortCount = DTC_CHECK_COUNT;
        m_iAntennaOpenCount = DTC_CHECK_COUNT;

        if(m_iAntennaNormalCount == 0)
        {          
            quint8 previousCmd =  (quint8)(this->m_DtcSetState & DTC_CMD_ANTENNA_NORMAL);

            if(previousCmd == DTC_CMD_ANTENNA_NORMAL || (m_bFirstShort && m_bFirstOpen))
            {
                sendDTCMessage(DTC_CMD_ANTENNA_OPEN, DTC_CLEAR);
                sendDTCMessage(DTC_CMD_ANTENNA_SHORT, DTC_CLEAR);
                this->m_DtcSetState = (this->m_DtcSetState | 0x03) ^ 0x03; // DTC Clear
                m_bFirstShort = false;
                m_bFirstOpen = false;
            }
            else if(previousCmd == DTC_CMD_ANTENNA_OPEN || m_bFirstOpen)
            {
                sendDTCMessage(previousCmd, DTC_CLEAR);
                this->m_DtcSetState = (this->m_DtcSetState | previousCmd) ^ previousCmd; // DTC Clear
                m_bFirstOpen = false;
            }
            else if(previousCmd == DTC_CMD_ANTENNA_SHORT || m_bFirstShort)
            {
                sendDTCMessage(previousCmd, DTC_CLEAR);
                this->m_DtcSetState = (this->m_DtcSetState | previousCmd) ^ previousCmd; // DTC Clear
                m_bFirstShort = false;
            }

             // DTC Antenna Count initialize
            initAntennaCount();
        }
    }
    else if(ant_short_dab == PIN_LOW && ant_open_dab == PIN_HIGH)     //Short State
    {
        m_iAntennaShortCount--;
        m_iAntennaOutBatteryCount = DTC_CHECK_COUNT;
        m_iAntennaOpenCount = DTC_CHECK_COUNT;
        m_iAntennaNormalCount = DTC_CHECK_COUNT;

        if(m_iAntennaShortCount == 0)
        {
            quint8 previousCmd = (quint8)(this->m_DtcSetState & DTC_CMD_ANTENNA_SHORT)/* >> 0x01*/;

//            qDebug() << __FUNCTION__ << " previousCmd : " << previousCmd << ", this->m_DtcSetState = " << this->m_DtcSetState;

            if(previousCmd != DTC_CMD_ANTENNA_SHORT || m_bFirstShort) // is previous SET B2953 ?
            {
                sendDTCMessage(DTC_CMD_ANTENNA_SHORT, DTC_ERROR);
                m_bFirstShort = false;
            }

            previousCmd = (this->m_DtcSetState & DTC_CMD_ANTENNA_OPEN)/* >> 0x01*/;

            if(previousCmd == DTC_CMD_ANTENNA_OPEN || m_bFirstOpen)
            {
                sendDTCMessage(DTC_CMD_ANTENNA_OPEN, DTC_CLEAR);
                this->m_DtcSetState = (this->m_DtcSetState | DTC_CMD_ANTENNA_OPEN) ^ DTC_CMD_ANTENNA_OPEN; // DTC Clear
                m_bFirstOpen = false;
            }

            // DTC Antenna Count initialize
            initAntennaCount();
        }
    }
    else if(ant_short_dab == PIN_HIGH && ant_open_dab == PIN_LOW)     //Open State
    {
        m_iAntennaOpenCount--;
        m_iAntennaOutBatteryCount = DTC_CHECK_COUNT;
        m_iAntennaShortCount = DTC_CHECK_COUNT;
        m_iAntennaNormalCount = DTC_CHECK_COUNT;

        if(m_iAntennaOpenCount == 0)
        {
            quint8 previousCmd = (quint8)(this->m_DtcSetState & DTC_CMD_ANTENNA_OPEN);
            if(previousCmd != DTC_CMD_ANTENNA_OPEN || m_bFirstOpen) //               is previous SET B2953 ?
            {
                sendDTCMessage(DTC_CMD_ANTENNA_OPEN, DTC_ERROR);
                m_bFirstOpen = false;
            }

            previousCmd = (this->m_DtcSetState & DTC_CMD_ANTENNA_SHORT)/* >> 0x01*/;
            if(previousCmd == DTC_CMD_ANTENNA_SHORT || m_bFirstShort)
            {
                sendDTCMessage(DTC_CMD_ANTENNA_SHORT, DTC_CLEAR);
                this->m_DtcSetState = (this->m_DtcSetState | DTC_CMD_ANTENNA_SHORT) ^ DTC_CMD_ANTENNA_SHORT; // DTC Clear
                m_bFirstShort = false;
            }

             // DTC Antenna Count initialize
            initAntennaCount();
        }
    }
    else if(ant_short_dab == PIN_LOW && ant_open_dab == PIN_LOW)     //Out of Battery State
    {
        m_iAntennaOutBatteryCount--;
        m_iAntennaShortCount = DTC_CHECK_COUNT;
        m_iAntennaOpenCount = DTC_CHECK_COUNT;
        m_iAntennaNormalCount = DTC_CHECK_COUNT;

        if(m_iAntennaOutBatteryCount == 0)
        {
            quint8 previousCmd =  (quint8)(this->m_DtcSetState & DTC_CMD_ANTENNA_NORMAL);

            if(previousCmd == DTC_CMD_ANTENNA_OUT_BATTERY || (m_bFirstShort && m_bFirstOpen))
            {
                sendDTCMessage(DTC_CMD_ANTENNA_OPEN, DTC_ERROR);
                sendDTCMessage(DTC_CMD_ANTENNA_SHORT, DTC_ERROR);
                m_bFirstShort = false;
                m_bFirstOpen = false;
            }

             // DTC Antenna Count initialize
            initAntennaCount();
        }
    }
}

void
DABAppEngine::initAntennaCount()
{
    m_iAntennaOutBatteryCount = DTC_CHECK_COUNT;
    m_iAntennaShortCount = DTC_CHECK_COUNT;
    m_iAntennaOpenCount = DTC_CHECK_COUNT;
    m_iAntennaNormalCount = DTC_CHECK_COUNT;
}

void
DABAppEngine::sendDTCMessage(quint8 command, quint8 error)
{
    QByteArray in0;
    in0.resize(4);

    qDebug() << __FUNCTION__ << " : Command = " << command;

    switch(command)
    {
        case DTC_CMD_ANTENNA_OPEN:    //B2953(0xA95300) DAB Antenna Open State
            qDebug() << __FUNCTION__ << " DTC_CMD_ANTENNA_OPEN : value = " << error;
            in0[0] = 0xA9;
            in0[1] = 0x53;
            in0[2] = 0x00;
            in0[3] = error;
#if 0
            if(error == 0x00)
                emit cmdReqSetAntennaStatus(0x01);
            else
                emit cmdReqSetAntennaStatus(0x00);
#endif
            break;

        case DTC_CMD_EXT_ANTENNA_OPEN:  //B2951(0xA95100) External DAB Antenna Open State
            qDebug() << __FUNCTION__ << " DTC_CMD_EXT_ANTENNA_OPEN : value = " << error;
            in0[0] = 0xA9;
            in0[1] = 0x51;
            in0[2] = 0x00;
            in0[3] = error;
            break;

        case DTC_CMD_ANTENNA_SHORT:   //B2952(0xA95200) DAB Antenna Short State
            qDebug() << __FUNCTION__ << " DTC_CMD_ANTENNA_SHORT : value = " << error;
            in0[0] = 0xA9;
            in0[1] = 0x52;
            in0[2] = 0x00;
            in0[3] = error;
            break;

        case DTC_CMD_MODULE_UART_ERROR:
            qDebug() << __FUNCTION__ << " DTC_CMD_MODULE_UART_ERROR : value = " << error;
            in0[0] = 0xB8;
            in0[1] = 0x53;
            in0[2] = 0x00;
            in0[3] = error;
            break;

        default :
            break;
    }

    if(m_MicomClient->isValid())
    {
        qDebug() << __FUNCTION__ << "LgeDBus_DTCSendMsg!! ";
        m_MicomClient->LgeDBus_DTCSendMsg(in0);
    }
    else
    {
        qDebug() << __FUNCTION__ << "m_MicomClient not valid!! ";
    }

     m_DtcSetState |= command;
}

void
DABAppEngine::onEndExternalDTC(void)
{
    Log("============== [onEndExternalDTC] ");
    m_exterAntennaTimer->stop();

    APP_EVENT_DIAGNOSTIC_TEST_T pEventData;
    memset(&pEventData, 0 ,sizeof(APP_EVENT_DIAGNOSTIC_TEST_T));
    Event event( EVT_DIAGNOSTIC_TEST_END, GetThisAppId(), APP_UI_MANAGER );
    NotifyUISHCommon( event );

    m_exterAntennaTimer->stop();
    initAntennaCount();
    m_interAntennaTimer->start(200);
}
#endif //__DTC__

#ifdef __WDT__
void
DABAppEngine::signalConnection()
{
    qDebug() << __FUNCTION__;
    struct sigaction act;
    memset(&act, 0x00, sizeof(act));    //# 20130327 prevent

    act.sa_flags = 0;

//    act.sa_flags |= SA_RESTART;
    act.sa_flags |= SA_RESETHAND;
    act.sa_handler = DABAppEngine::signalHandler;

    for (int signumber = SIGHUP; signumber<SIGUNUSED; signumber++)
    {
        if(signumber == SIGKILL || signumber == SIGSTOP || signumber == SIGCHLD) continue;
        if(sigaction(signumber, &act, (struct sigaction*)NULL) < 0)
        {
            DABLogger::instance()->Log((QString(" ====================================exit(1);")));
//            exit(1);
        }
    }
}

void
DABAppEngine::setSignalHandlerObject(DABAppEngine* m)
{
//    qDebug() << __FUNCTION__;
    SigM = m;
}

void
DABAppEngine::signalHandler(int signum)
{
//    qDebug() << " ================================DAB App Restart===========================signum = " << signum << LOG_ENDL;
//    qDebug() << " ================================DAB App Restart==========================="<< LOG_ENDL;
//    qDebug() << " ================================DAB App Restart===========================" << LOG_ENDL;
    DABLogger::instance()->Log((QString(" ==============DAB App Restart======================signum = %1").arg(signum)));
    SigM->restart(signum);
    raise(signum);
}

void
DABAppEngine::restart(int sig)
{
    qDebug() << __FUNCTION__;
    Event event(EVT_REQUEST_ABNORMAL_TERMINATION, GetThisAppId(), APP_UI_MANAGER, (EVT_COMMAND_T)sig);
    NotifyUISH(event, DISPLAY_FRONT);
    exit(0);
}

#endif
