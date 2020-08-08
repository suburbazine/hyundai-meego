/**
 * FileName: DABController.cpp
 * Author: David.Bae
 * Time: 2011-07-22 11:49
 */

#include <QtDebug>
#include <QFile>
#include <QDir>
#include <QDataStream>
#include <QStandardItemModel>
#include <QGraphicsObject>
#include <QDeclarativeView>
#include <QDeclarativeProperty>
#include "DABController.h"
#include "DABProtocol.h"
#include "DABApplication_Def.h"
#include "DABEPGItem.h"
#include "DABEPGReservationItem.h"

DABController::DABController(QObject *parent) :
    QObject(parent)
{
    this->pUIListener = new DABUIListener(*this);
    this->pChannelManager = this->pUIListener->getChannelManager();
    this->pCommManager = new DABCommManager(parent);
    this->pEPGReservationList = new DABEPGReservationList(*this);

    this->init();
    this->loadModuleVersion();
    this->pEPGReservationItem = new DABEPGReservationItem(parent);
    this->createConnection();
    this->pUIListener->initEPG();
/**
  2013.07.24 Mobilus DaehyungE
  방송 time 수신 안될 경우에는, 시스템 시간 보다가 방송에서 time내려오면 방송 time 보도록 수정
  */
    this->pEPGReservationList->loadReservationItem();
    DABLogger::instance()->initBroadcastingTime();    
}

DABController::~DABController()
{
    if(this->pUIListener != NULL)
    {
        delete this->pUIListener;
        this->pUIListener = NULL;
    }
    if(this->pCommManager != NULL)
    {
        delete this->pCommManager;
    }
}

void
DABController::init()
{
    setStatus(DAB_STATUS_READY);
    setServiceStatus(DAB_SERVICE_READY);
    this->m_bGetVersionRequested = false;
    this->m_bListScanning = false;
    this->m_bPresetScanning = false;
    this->m_iPlayIndexBackup = 0;
    this->m_bExistCurrentChannel = false;
    this->m_bExistCurrentChannelInPreset = false;
    this->m_eServiceStatus = DAB_SERVICE_IDLE;
    this->m_eServiceFollowingState = DAB_SERVICE_FOLLOWING_STATE_AUDIO_DAB;
    this->m_iRadioPlayBand = 0x01;
    this->m_qRadioPlayFrequency = 0;
    this->m_sImageRootDir = "";
    this->m_bIsAutoScan = false;
    this->m_iLinkingState = 0;
    this->m_bRequestDABtoFM = false;
#ifdef __DTC__
    this->m_AliveMsgOnOff = false;
    this->m_AliveUartMsg = false;
#endif
    this->m_iServiceEvent = EVT_SEND_CHANGE_SERVICE_EVENT_NORMAL;
    this->m_bPICodeStatus = 0x00;
    this->m_bPinkStatus = false;
    this->m_iPreviousFreq = 0;
    this->m_eModuleStatus = EVT_SEND_MODULE_STATUS_BOOT;
    this->m_bPICodeSensitive = 0x00;
    this->m_FMFrequency = 0x00;
    memset(this->m_PICodeValue, 0x00, sizeof(this->m_PICodeValue));
    this->m_PICodeOffset = -1;
    this->m_PICodeValueCount = 0;
    this->pEPGReservationItem = NULL;
    this->m_iReqGetVersionCount = 0;
    this->m_iSLSTimeCount = 0;
    this->m_iDLSTimeCount = 0;
    this->m_iDLPlustTimeCount = 0;
    this->m_iDLSTimeout = DLS_TIME_INTERVAL;
    this->m_iSLSTimeout = SLS_TIME_INTERVAL;
    this->m_iDLPlustTimeout = DL_PLUS_TIME_INTERVAL;
    this->m_iReservationCount = 0;
    this->m_bLoadReservationItem = false;
    this->m_bDeactiveTA = false;
    this->m_bTAInterrupt = false;
    this->m_bDeactiveDABtoFM = false;
    this->m_bDeactiveFMLinking = false;    
    this->m_bIgnoreBADSignal = 0;    
    this->m_eFMtoDABState = FM_DAB_STATE_FIRST;
    this->m_iTASubChId = 0;
    this->m_bMuteCheck = false;	//# Mute Cancel
    this->m_sBackupEPGTitle = "";
    this->m_sBackupEPGServiceName = "";
    this->m_bSoundSettingFlag = false;
    this->m_bVrStatus = false;
}

void
DABController::createConnection(void)
{
    // ==================================================  DABController.cpp -> DABChannelManager.cpp
    connect(this, SIGNAL(cmdUpdateChannelItem(quint32,quint16,quint8,QString,quint8,quint32,quint8,quint8,quint16,quint8,quint8,quint8,quint8,QString,stASU_INFO,stPICODE_INFO,quint8,quint16)),
             pChannelManager, SLOT(onUpdateChannelItem(quint32,quint16,quint8,QString,quint8,quint32,quint8,quint8,quint16,quint8,quint8,quint8,quint8,QString,stASU_INFO,stPICODE_INFO,quint8,quint16)));
    connect(this, SIGNAL(cmdAddTempChannelItem(quint32,quint16,quint8,QString,quint8,quint32,quint8,quint8,quint16,quint8,quint8,quint8,quint8,QString,stASU_INFO,stPICODE_INFO,quint8,quint16)),
            pChannelManager, SLOT(onAddTempChannelItem(quint32,quint16,quint8,QString,quint8,quint32,quint8,quint8,quint16,quint8,quint8,quint8,quint8,QString,stASU_INFO,stPICODE_INFO,quint8,quint16)));
    connect(this, SIGNAL(cmdAddNewChannelItem(quint32,quint16,quint8,QString,quint8,quint32,quint8,quint8,quint16,quint8,quint8,quint8,quint8,QString,stASU_INFO,stPICODE_INFO,quint8,quint16)),
            pChannelManager, SLOT(onAddNewChannelItem(quint32,quint16,quint8,QString,quint8,quint32,quint8,quint8,quint16,quint8,quint8,quint8,quint8,QString,stASU_INFO,stPICODE_INFO,quint8,quint16)));
    connect(this, SIGNAL(cmdRemoveUnlockFrequency(quint32)),                                                pChannelManager, SLOT(onRemoveUnlockFrequency(quint32)));
    connect(this, SIGNAL(cmdUpdateChannelListFromAutoScan()),                                               pChannelManager, SLOT(onUpdateChannelListFromAutoScan()));
    connect(this, SIGNAL(cmdUpdateChannelListFromReconfiguration()),                                        pChannelManager, SLOT(onUpdateChannelListFromReconfiguration()));
    connect(this, SIGNAL(cmdUpdateChannelListFromReconfigurationNotChange()),                               pChannelManager, SLOT(onUpdateChannelListFromReconfigurationNotChanged()));



    // ==================================================  DABChannelManager.cpp -> DABController.cpp
    connect(pChannelManager, SIGNAL(cmdReqSetSelectService()),                                              this, SLOT(ReqSetSelectService()));
    connect(pChannelManager, SIGNAL(cmdInitPICodeValue()),                                                  this, SLOT(initPICodeValue()));

    // ==================================================  DABCommManager.cpp -> DABUIListener.cpp
    connect(pCommManager, SIGNAL(cmdAddEPGItem(quint32,quint32,int,int,int,int,QString,QString)),           pUIListener, SLOT(onAddEPGItem(quint32,quint32,int,int,int,int,QString,QString)));


    // ==================================================  DABUIListener.cpp -> DABCommManager.cpp
    connect(pUIListener, SIGNAL(cmdReqSetAnnounceDelay(quint32)),                                           pCommManager, SLOT(onReqSetAnnounceDelay(quint32)));
    connect(pUIListener, SIGNAL(cmdReqSetAnnounceTimeout(quint32)),                                         pCommManager, SLOT(onReqSetAnnounceTimeout(quint32)));
    connect(pUIListener, SIGNAL(cmdReqSetAnnounceWeakTimeout(quint32)),                                     pCommManager, SLOT(onReqSetAnnounceWeakTimeout(quint32)));
    connect(pUIListener, SIGNAL(cmdReqSetServLinkTimeout(quint32)),                                         pCommManager, SLOT(onReqSetServLinkTimeout(quint32)));


    // ==================================================  DABCommManager.cpp -> DABController.cpp
    connect(pCommManager, SIGNAL(sendChannelItem(quint32,quint16,quint8,QString,quint8,quint32,quint8,quint8,quint16,quint8,quint8,quint8,quint8,QString,stASU_INFO,stPICODE_INFO,quint8,quint16)),
            this, SLOT(sendChannelItem(quint32,quint16,quint8,QString,quint8,quint32,quint8,quint8,quint16,quint8,quint8,quint8,quint8,QString,stASU_INFO,stPICODE_INFO,quint8,quint16)));
    connect(pCommManager, SIGNAL(cmdAddStationLogo(QString)),                                               this, SLOT(CmdAddStationLogo(QString)));
#ifdef __DAB_TPEG__
    connect(pCommManager, SIGNAL(cmdRspTPEGData(const QByteArray &)),                                       this, SLOT(onRspTPEGData(const QByteArray &)));
#endif
    connect(pCommManager, SIGNAL(cmdAudioInfo(quint8,quint8,quint8)),                                       this, SLOT(onAudioInfo(quint8,quint8,quint8)));



    // ==================================================  EPGReservationList.cpp -> DABController.cpp
    connect(pEPGReservationList, SIGNAL(cmdEvtPreservedPopup(QString, QString)),                            pUIListener, SLOT(onEvtApperPreservedPopup(QString, QString)));


    // ==================================================  DABController.cpp -> EPGReservationList.cpp
    connect(this,                SIGNAL(cmdCheckReservationItem()),                                         pEPGReservationList, SLOT(onCheckReservationItem()));

    //=================================================  Connection with DABCommManager for Response
    connect(pCommManager, SIGNAL(rspGetVersion(quint8,quint8,quint8,quint8,quint8,quint8,quint8,QString,quint8)),           this, SLOT(RspGetVersion(quint8,quint8,quint8,quint8,quint8,quint8,quint8,QString,quint8)));
    connect(pCommManager, SIGNAL(rspSetSeekService(eREQ_SET_FULL_SCAN_BAND,quint8,quint8,quint8,quint32,quint32,quint8)),   this, SLOT(RspSetSeekService(eREQ_SET_FULL_SCAN_BAND,quint8,quint8,quint8,quint32,quint32,quint8)));
    connect(pCommManager, SIGNAL(rspSetSelectService(quint32,quint8,quint32,quint8,quint16,QString)),                       this, SLOT(RspSetSelectService(quint32,quint8,quint32,quint8,quint16,QString)));
    connect(pCommManager, SIGNAL(rspSetAnnouncementFlag(quint16)),                                                          this, SLOT(RspSetAnnouncementFlag(quint16)));
    connect(pCommManager, SIGNAL(rspGetServiceList()),                                                                      this, SLOT(RspGetServiceList()));
    connect(pCommManager, SIGNAL(rspGetDLS(QString,QString,QString,QString)),                                               this, SLOT(onRspGetDLS(QString,QString,QString,QString)));
    connect(pCommManager, SIGNAL(rspGetSLS(QString)),                                                                       this, SLOT(RspGetSLS(QString)));
    connect(pCommManager, SIGNAL(removeDuplicatEPGData(quint32,quint32)),                                                   this, SLOT(removeDuplicatEPGData(quint32,quint32)));
    connect(pCommManager, SIGNAL(rspGetEPG(quint32, quint32)),                                                              this, SLOT(onRspGetEPG(quint32, quint32)));


    //=================================================  Connection with DABCommManager for EVENT
    connect(pCommManager, SIGNAL(evtSendModuleStatus(eEVT_SEND_MODULE_STATUS,eEVT_SEND_SERVICE_STATUS)),                    this, SLOT(EvtSendModuleStatus(eEVT_SEND_MODULE_STATUS,eEVT_SEND_SERVICE_STATUS)));
    connect(pCommManager, SIGNAL(evtSendChangeService(int,int,int,int,int,int,QString,QString,eEVT_SEND_CHANGE_SERVICE_EVENT)), this, SLOT(EvtSendChangeService(int,int,int,int,int,int,QString,QString,eEVT_SEND_CHANGE_SERVICE_EVENT)));

    connect(pCommManager, SIGNAL(evtSendAnnouncement(QString,int,eEVT_SEND_ANNOUNCEMENT_STATUS,quint8)),                    this, SLOT(EvtSendAnnouncement(QString,int,eEVT_SEND_ANNOUNCEMENT_STATUS,quint8)));
    connect(pCommManager, SIGNAL(evtSendTime(int,int,int,int,int,int)),                                                     this, SLOT(EvtSendTime(int,int,int,int,int,int)));
    connect(pCommManager, SIGNAL(evtSendQOS(int,int,int,int,int,int)),                                                      this, SLOT(EvtSendQOS(int,int,int,int,int,int)));
    connect(pCommManager, SIGNAL(evtUpdateDLS(quint8)),                                                                     this, SLOT(EvtUpdateDLS(quint8)));
    connect(pCommManager, SIGNAL(evtUpdateSLS()),                                                                           this, SLOT(EvtUpdateSLS()));
    connect(pCommManager, SIGNAL(evtUpdateEPG()),                                                                           this, SLOT(EvtUpdateEPG()));
#ifdef __DTC__
    connect(pCommManager, SIGNAL(evtSendDtcUart()),                                                                         this, SLOT(EvtSendDtcUart()));
#endif
    connect(pCommManager, SIGNAL(evtServiceFollowingState(quint8)),                                                         this, SLOT(EvtServiceFollowingState(quint8)));
    connect(pCommManager, SIGNAL(evtSendPinkStatus(quint8)),                                                                this, SLOT(EvtSendPinkStatus(quint8)));
    connect(pCommManager, SIGNAL(evtSendSignalInfo(quint8)),                                                                this, SLOT(EvtSendSignalInfo(quint8)));
    connect(pCommManager, SIGNAL(evtAutoScanInfo(quint8, quint32, quint8)),                                                 this, SLOT(EvtAutoScanInfo(quint8, quint32, quint8)));
    connect(pCommManager, SIGNAL(evtSendAnnouncementTAInvalid(quint16)),                                                    this, SLOT(EvtSendAnnouncementTAInvalid(quint16)));

     //=================================================  Connection with TIMER EVENT
    connect(&m_ScanTimer,               SIGNAL(timeout()),      this,       SLOT(ScanTimeOut()));
    connect(&m_WeakSignalTimer,         SIGNAL(timeout()),      this,       SLOT(WeakTimeOut()));
    connect(&m_TuneTimer,               SIGNAL(timeout()),      this,       SLOT(onTuneTimeOut()));
    connect(&m_TASearchTimer,           SIGNAL(timeout()),      this,       SLOT(onTASearchTimeOut()));
    connect(&m_BroadcastingTimer,       SIGNAL(timeout()),      this,       SLOT(onBroadcastingTimeTick()));
    connect(&m_DeactiveFMLinkingTimer,  SIGNAL(timeout()),      this,       SLOT(onDeactivateFMLinking()));
    connect(&m_DeactiveTATimer,         SIGNAL(timeout()),      this,       SLOT(onDeactivateTA()));
}

DABUIListener*
DABController::getUIListener(void)
{
    if(this->pUIListener == NULL)
        qDebug() << Q_FUNC_INFO << " pUIListener is NULL.";

    return this->pUIListener;
}

void
DABController::start(void)
{
#ifndef __UNIX_DESKTOP__
    if(this->pCommManager != NULL)
    {
        DABLogger::instance()->Log((QString(" DABController.cpp :: start() : this->pCommManager->startComm() called !!")));
        this->pCommManager->startComm();
    }
    else
    {
        qDebug() << " pCommManager is NULL !!";
    }
#endif
}

void
DABController::onReqSeekDown(void)
{
    qDebug() << Q_FUNC_INFO << " Called Parm: ";

    if( this->pUIListener->setPrevChannelInfoForShortSeek() != true )
    {
        qDebug() <<  __FUNCTION__ << " Seek Down failed.. ";
        return;
    }
    this->allScanStop();
    this->ReqSetSelectService();
}

void
DABController::onReqSeekUp(void)
{
    qDebug() << Q_FUNC_INFO << " Called Parm: ";

    if( this->pUIListener->setNextChannelInfoForShortSeek() != true )
    {
        qDebug() <<  __FUNCTION__ << " Seek Up failed.. ";
        return;
    }
    this->allScanStop();
    this->ReqSetSelectService();
}

void
DABController::onReqLongSeekDown(void)
{
    this->ReqSetSeekService((eREQ_SET_FULL_SCAN_BAND)pUIListener->getBandSelection(), eREQ_SET_SEEK_SERVICE_DIRECTION_DOWNWARD, true);
}

void
DABController::onReqLongSeekUp(void)
{
    this->ReqSetSeekService((eREQ_SET_FULL_SCAN_BAND)pUIListener->getBandSelection(), eREQ_SET_SEEK_SERVICE_DIRECTION_UPWARD, true);
}

void
DABController::onReqTuneDown(void)
{
    qDebug() << Q_FUNC_INFO;

    if( this->pUIListener->setPrevChannelInfoForTuneUpDown() != true )
    {
        qDebug() <<  " Tune Down failed.. ";
        return;
    }
    this->onEventExceptionHandler(DAB_EVENT_TUNE_DOWN);
    if(this->pUIListener->checkCurrentChannelItem())
    {
        this->pUIListener->modeChange(false);
        setServiceStatus(DAB_SERVICE_IDLE);
        DABChannelItem* item = pChannelManager->getCurrentChannelItem();
        emit cmdReqdisplayOSD(item->label(), item->ensembleLabel(), DAB_OP_TYPE_SELECT);
    }
    else
    {
        this->pUIListener->modeChange(true);
        setServiceStatus(DAB_SERVICE_TUNE_SEEK);
        onSetCurrentInfoToHomeMedia();
    }
}

void
DABController::onReqTuneUp(void)
{
    qDebug() << Q_FUNC_INFO;
    if( this->pUIListener->setNextChannelInfoForTuneUpDown() != true )
    {
        qDebug() <<  __FUNCTION__ << " Tune Up failed.. ";
        return;
    }
    this->onEventExceptionHandler(DAB_EVENT_TUNE_UP);
    if(this->pUIListener->checkCurrentChannelItem())
    {
        this->pUIListener->modeChange(false);
        setServiceStatus(DAB_SERVICE_IDLE);
        DABChannelItem* item = pChannelManager->getCurrentChannelItem();
        emit cmdReqdisplayOSD(item->label(), item->ensembleLabel(), DAB_OP_TYPE_SELECT);
    }
    else
    {
        this->pUIListener->modeChange(true);
        setServiceStatus(DAB_SERVICE_TUNE_SEEK);
        onSetCurrentInfoToHomeMedia();
    }
}

void
DABController::onReqTunePressed(void)
{
    qDebug() << Q_FUNC_INFO;
    if(this->pUIListener->setViewChanneltoCurrentChannel())
    {
        this->ReqSetSelectService();
    }
    else
    {
        qDebug() << __FUNCTION__ << " Tune Pressed Fail !!.";
    }
}

void
DABController::onRspTrafficRDStoDAB(bool onOff)
{    
    quint16 flag = this->pUIListener->getAnnouncementFlag();

    qDebug() << Q_FUNC_INFO << " :: onOff = " << onOff << ", Announcement Flag = " << hex << flag;
    DABLogger::instance()->Log((QString(" onRspTrafficRDStoDAB() :: onOff = %1, AnnouncementFlag = %2").arg(onOff).arg(flag)));

    if(onOff)       // Traffic Announcement On
    {
        flag |= ANN_TRAFFIC_ALARM_ON;
        StartTASearchTimer();
    }
    else            // Traffic Announcement Off
    {
        flag = (flag | ANN_TRAFFIC_ALARM_ON)^ANN_TRAFFIC_ALARM_ON;
        flag |= ANN_ALARM_ON;
        StopTASearchTimer();
    }

    this->pUIListener->setAnnouncementFlag(flag);
    this->pCommManager->ReqSetAnnouncementFlag(flag);
}

void
DABController::onReqSetAnnounement(bool onOff)
{
    quint16 flag = 0;

    if(onOff == true)
    {
        flag |= ANN_TRAFFIC_ALARM_ON;
        StartTASearchTimer();
    }
    else if(onOff == false)
    {
        flag = ANN_ALARM_ON;
        StopTASearchTimer();
    }

    this->pUIListener->setAnnouncementFlag(flag);
    this->pCommManager->ReqSetAnnouncementFlag(flag);

    qDebug() << Q_FUNC_INFO << " :: onOff = " << onOff << ", flag = " << hex << flag;
}

void
DABController::onReqTA(bool onOff)
{
    bool ret = false;
    quint16 flag = this->pUIListener->getAnnouncementFlag();
    qDebug() << __FUNCTION__ << " Announcement Flag = 0x" << hex << flag << ", onOff = " << onOff;

    if(flag == ANN_TRAFFIC_ALARM_ON)        // Traffic  Announcement Off
    {
        if(onOff)
        {
            qDebug() << __FUNCTION__ << " :: already taffic on !!. so return";
            return;
        }
        flag = (flag | ANN_TRAFFIC_ALARM_ON) ^ ANN_TRAFFIC_ALARM_ON;
        flag |= ANN_ALARM_ON;
        ret = false;
        StopTASearchTimer();
    }
    else
    {
        if(!onOff)
        {
            qDebug() << __FUNCTION__ << " :: already taffic off !!. so return";
            return;
        }
        flag |= ANN_TRAFFIC_ALARM_ON;
        ret = true;
        StartTASearchTimer();
    }

    qDebug() << __FUNCTION__ << " Announcement Flag = 0x" << hex << flag << " ret = " << ret << ", onOff = " << onOff;
    this->pUIListener->setAnnouncementFlag(flag);
    this->pCommManager->ReqSetAnnouncementFlag(flag);
}

void
DABController::onReqTASetting(bool onOff)
{
    quint16 flag = 0;

    if(!onOff)
    {
        flag = ANN_ALARM_ON;
        StopTASearchTimer();
        this->pUIListener->setAnnouncementFlag(flag);
        this->pCommManager->ReqSetAnnouncementFlag(flag);
        emit cmdReqTrafficDABtoRDS(onOff);
    }

    qDebug() << __FUNCTION__ << " Announcement Flag = 0x" << hex << flag << ", onOff = " << onOff;
}

void
DABController::onReqScan(void)
{
    qDebug() << Q_FUNC_INFO << "DABController: onReqScan Called Parm: ";

    if(isListStarted())
    {
        this->listScanStop();
        return;
    }

    int index = 0;
    DABChannelItem* pItem = pChannelManager->getCurrentChannelItem();
    index = pChannelManager->findNextChannelIndexInChannelList(pItem);
    qDebug() << Q_FUNC_INFO << " Next Index : " << index;

    if(pChannelManager->isAlreadyExistCurrentChannel(pItem))
    {
        this->m_bExistCurrentChannel = false;
        this->m_iPlayIndexBackup = pChannelManager->getCurrentChannelIndexFromChannelList();
        index = pChannelManager->findNextChannelIndexInChannelList(pItem);
    }
    else
    {
        this->m_bExistCurrentChannel = true;
        this->m_iPlayIndexBackup = pChannelManager->getCurrentChannelIndexFromChannelList();
    }

    if( this->pUIListener->setCurrentChannelInfoFromChannelList(index) )
    {
        this->listScanStart();
        this->ReqSetSelectService();
        DABChannelItem* pCurrentChannelItem = pChannelManager->getCurrentChannelItem();
        StartScanTimer(pCurrentChannelItem->serviceType());
    }
    else
    {
        qDebug() << " Can not start Scan..";
    }
}

void
DABController::onReqPresetScan(void)
{
    if(isPresetScanStarted())
    {
        this->PresetScanStop();
        return;
    }

    int index = 0;
    if(pChannelManager->isAlreadyExistCurrentChannelInPresetList())
    {
        this->m_bExistCurrentChannelInPreset = false;
        this->m_iPlayIndexBackup = pChannelManager->getCurrentChannelIndexFromPresetList();
        DABChannelItem* pItem = pChannelManager->getCurrentChannelItem();
        index = pChannelManager->findNextChannelIndexInPresetList(pItem);
    }
    else
    {
        this->m_bExistCurrentChannelInPreset = true;
        this->m_iPlayIndexBackup = pChannelManager->getCurrentChannelIndexFromChannelList();
    }

    if( this->pUIListener->setCurrentChannelInfoFromPresetList(index) )
    {
        if(pChannelManager->getCurrentChannelIndexFromChannelList() == -1)
        {
            DABChannelItem* pItem = pChannelManager->getChannelItemFromPresetList(index);
            pChannelManager->addCurrentItemInChannelList(pItem);
        }
        this->PresetScanStart();
        this->ReqSetSelectService();
        DABChannelItem* pCurrentChannelItem = pChannelManager->getCurrentChannelItem();
        StartScanTimer(pCurrentChannelItem->serviceType());
    }
    else
    {
        qDebug() << " Can not start PresetScan..";
    }
}

void
DABController::onReqSaveData(void)
{
    qDebug() << Q_FUNC_INFO;
    this->pUIListener->saveData();
}

void
DABController::onReqTASearch(void)
{
    DABChannelItem* pItem = pChannelManager->findTAChannelInChannelList();
    if(pItem == NULL)
    {
        qDebug() << __FUNCTION__ << " =================== pItem is NULL";
    }
    else
    {
        this->pUIListener->setCurrentChannelInfoFromItem(pItem);
        this->ReqSetSelectService();
    }
}

void
DABController::onReqSeekNextPresetList(void)
{
    int index = 0;
    if(pChannelManager->isAlreadyExistCurrentChannelInPresetList())
    {
        DABChannelItem* pItem = pChannelManager->getCurrentChannelItem();
        index = pChannelManager->findNextChannelIndexInPresetList(pItem);
    }

    this->pUIListener->setCurrentChannelInfoFromPresetList(index);
    allScanStop();

    if(pChannelManager->getCurrentChannelIndexFromChannelList() == -1)
    {
        DABChannelItem* pItem = pChannelManager->getChannelItemFromPresetList(index);
        pChannelManager->addCurrentItemInChannelList(pItem);
    }
    this->ReqSetSelectService();    
}

void
DABController::onReqSeekPrevPresetList(void)
{
    int index = 0;
    if(pChannelManager->isAlreadyExistCurrentChannelInPresetList())
    {
        DABChannelItem* pItem = pChannelManager->getCurrentChannelItem();
        index = pChannelManager->findPrevChannelIndexInPresetList(pItem);
    }

    this->pUIListener->setCurrentChannelInfoFromPresetList(index);
    allScanStop();

    if(pChannelManager->getCurrentChannelIndexFromChannelList() == -1)
    {
        DABChannelItem* pItem = pChannelManager->getChannelItemFromPresetList(index);
        pChannelManager->addCurrentItemInChannelList(pItem);
    }
    this->ReqSetSelectService();
}

void
DABController::onReqPresetSelected(int count)
{
    allScanStop();
    DABChannelItem* pPresetItem = pChannelManager->getChannelItemFromPresetList(count);
    if(pPresetItem == NULL)
    {
        qDebug() << Q_FUNC_INFO << " Error pItem is NULL!!!!!";
        return;
    }
    DABChannelItem* pCurrentItem = pChannelManager->getCurrentChannelItem();
    if(pCurrentItem == NULL)
    {
        qDebug() << Q_FUNC_INFO << " Error pItem is NULL!!!!!";
        return;
    }

    if((pCurrentItem->frequency() == pPresetItem->frequency()) && (pCurrentItem->serviceID() == pPresetItem->serviceID()) && (pCurrentItem->subChId() == pPresetItem->subChId()))
    {
        qDebug() << __FUNCTION__ << " Current Item and Preset Item is same.!! preset index = " << pPresetItem->presetIndex() << ", m_bMuteCheck = " << m_bMuteCheck << ", NoSignal = " << pPresetItem->isNoSignal();
#if 1 //ITS 208496
        this->pUIListener->setPresetIndex(pPresetItem->presetIndex());
        if(!(pCurrentItem->isNoSignal() || pCurrentItem->isDABtoDAB() || pCurrentItem->isDABtoFM()))
        {
            this->pUIListener->setCurrentChannelInfoFromItem(pPresetItem);
            pChannelManager->updateChannelItemStatus(DAB_CHANNEL_ICON_CLEAR, true);
            emit cmdReqClusterControl(pPresetItem->label(), this->pUIListener->getPresetIndex(), DAB_CLUSTER_NOMAR_STATE);
        }
        if(m_bMuteCheck == true)
        {
            emit cmdSendMuteCancel();
        }
#else
        this->pUIListener->setCurrentChannelInfoFromItem(pPresetItem);
        this->pUIListener->setPresetIndex(pPresetItem->presetIndex());
        pChannelManager->updateChannelItemStatus(DAB_CHANNEL_ICON_CLEAR, true);
        emit cmdReqClusterControl(pPresetItem->label(), this->pUIListener->getPresetIndex(), DAB_CLUSTER_NOMAR_STATE);
        if(m_bMuteCheck == true)
        {
            emit cmdSendMuteCancel();
        }
#endif
    }
    else
    {
        qDebug() << __FUNCTION__ << " :: isNoSignal = " << pPresetItem->isNoSignal() << ", isDABtoDAB = " << pPresetItem->isDABtoDAB() << ", isDABtoFM = " << pPresetItem->isDABtoFM();
        this->pUIListener->setCurrentChannelInfoFromItem(pPresetItem);       
        pChannelManager->updateChannelItemStatus(DAB_CHANNEL_ICON_CLEAR, true); //2013.11.19 DaehyungE, staionlist 진입시 linking 정보값이 남아있는 현상이 있어서 초기화 시킨다.

        if(pChannelManager->searchPresetChannelFromChannelList() == -1)
        {
            pChannelManager->addCurrentItemInChannelList(pPresetItem);
        }

        this->ReqSetSelectService();
    }
}

void
DABController::onSetCurrentChannelInfo(int serviceID)
{
    if(this->pUIListener->setCurrentChannelInfoFromServiceID(serviceID) != true)
    {
        qDebug() <<  __FUNCTION__ << " ::  Current Channel is NULL";
        return;
    }

    this->ReqSetSelectService();
}

void
DABController::onReqSetReservationChannel(void)
{
    DABEPGReservationItem* item = pEPGReservationList->getReservationItem();
    DABChannelItem* pCurrentItem = pChannelManager->getCurrentChannelItem();
    if(item->getFrequency() == 0 || item->getServiceID() == 0)
    {
        qDebug() << Q_FUNC_INFO << " Frequency:"<<item->getFrequency()<<", ServiceID:"<<item->getServiceID()<<". Can't select service..";
        return;
    }
    allScanStop();
    this->onEventExceptionHandler(DAB_EVENT_SELECT_SERVICE);

    QByteArray byteLabel;
    byteLabel.append(item->getLabel());

    quint16 picode[12] ={0,};
    int picodeCount = 0;
    for(int j=0; j<item->getPicodeInfo().PICodeCount; j++)
    {
        quint8 activate = (item->getPicodeInfo().PICodeValidation >> (15-j)) & 0x01;

        if(item->getPicodeInfo().PICodeType[j] == 0x00 && (activate == 0x01))
        {
            DABLogger::instance()->Log(QString("  onReqSetReservationChannel()====================== PICodeValue[%1] serviceID[%2]").arg(item->getPicodeInfo().PICodeValue[j]).arg(item->getServiceID()));
            picode[picodeCount] = item->getPicodeInfo().PICodeValue[j];
            picodeCount++;
        }
    }

    if((pCurrentItem->frequency() == item->getFrequency()) && (pCurrentItem->serviceID() == item->getServiceID()) && (pCurrentItem->subChId() == item->getSubChId()))
    {
        qDebug() << __FUNCTION__ << " ::  Select channel and current channel is same !! so not select service ";
    }
    else
    {
        if(picodeCount == 0)
        {
            this->pCommManager->ReqSetSelectService(item->getFrequency(), item->getServiceType(), item->getServiceID(), item->getSubChId(), item->getBitrate(), item->getSubCHSize(), item->getAddress(), 0x01,(const char*)byteLabel.constData(), picodeCount, NULL);
        }
        else
        {
            this->pCommManager->ReqSetSelectService(item->getFrequency(), item->getServiceType(), item->getServiceID(), item->getSubChId(), item->getBitrate(), item->getSubCHSize(), item->getAddress(), 0x01, (const char*)byteLabel.constData(), picodeCount, picode);
        }
    }

    DABChannelItem *pItem = new DABChannelItem(this);
    pItem->setFrequency(item->getFrequency());
    pItem->setEnsembleLabel(item->getEnsembleLabel());
    pItem->setServiceID(item->getServiceID());
    pItem->setServiceType(item->getServiceType());
    pItem->setSubChId(item->getSubChId());
    pItem->setBitrate(item->getBitrate());
    pItem->setProgrameType(item->getPTY());
    pItem->setLabel(item->getLabel());
    pItem->setPresetIndex(item->getPresetIndex());
    pItem->setSubCHSize(item->getSubCHSize());
    pItem->setAddress(item->getAddress());
    pItem->setLinkingInfo(item);

    this->pUIListener->setCurrentChannelInfoFromItem(pItem);
    setStatus(DAB_STATUS_WAITING);
    this->pUIListener->changeReservationChannel();
    pChannelManager->updateChannelItemStatus(DAB_CHANNEL_ICON_CLEAR, true);

    if(!(this->m_bListScanning || this->m_bPresetScanning))
    {
        setServiceStatus(DAB_SERVICE_SELECT);
    }
    onSetCurrentInfoToHomeMedia();
    emit cmdReqClusterControl(pItem->label(), 0, DAB_CLUSTER_NOMAR_STATE);
    pEPGReservationList->removeItem();

    if(pChannelManager->getCurrentChannelIndexFromChannelList() == -1)
    {
        pChannelManager->addCurrentItemInChannelList(pItem);
    }

    if(m_bMuteCheck == true)
    {
        emit cmdSendMuteCancel();
    }
}

void
DABController::onReqListSelected(int count, QString type)
{
    qDebug() << Q_FUNC_INFO << ":: count =  " << count << ", type = " << type;

    allScanStop();
    if( this->pUIListener->setCurrentChannelInfoFromStationlList(count, type) != true )
    {
        qDebug() <<  __FUNCTION__ << " ========= Current Channel is NULL or Same channel !!! , m_bMuteCheck = " << m_bMuteCheck;
        if(m_bMuteCheck == true)
        {
            emit cmdSendMuteCancel();
        }
        return;
    }
    this->ReqSetSelectService();
    if(pChannelManager->getCurrentChannelIndexFromChannelList() == -1)
    {
        DABChannelItem* pItem = pChannelManager->getCurrentChannelItem();
        pChannelManager->addCurrentItemInChannelList(pItem);
    }
}

void
DABController::onReqCancelTrafficAnnouncement(bool onOff)
{
    DABChannelItem* item = pChannelManager->getCurrentChannelItem();
    quint16 announceFlags = this->pUIListener->getAnnouncementFlag();
    qDebug() << __FUNCTION__ << " announceFlags = 0x" << hex << announceFlags;
    QByteArray byteLabel;
    byteLabel.append(item->label());

    if(onOff)
    {
        announceFlags = ANN_ALARM_ON;
        this->pUIListener->setAnnouncementFlag(announceFlags);
        this->pCommManager->ReqSetAnnouncementFlag(announceFlags);
        emit cmdReqTrafficDABtoRDS(false);
    }

    quint16 picode[12] ={0,};
    int picodeCount = 0;
    for(int j=0; j<item->picodeInfo().PICodeCount; j++)
    {
        quint8 activate = (item->picodeInfo().PICodeValidation >> (15-j)) & 0x01;
        if(item->picodeInfo().PICodeType[j] == 0x00 && (activate == 0x01))
        {
            picode[picodeCount] = item->picodeInfo().PICodeValue[j];
            picodeCount++;
        }
    }

    if(picodeCount == 0)
    {
        this->pCommManager->ReqSetSelectAnnouncementService(item->frequency(), item->serviceType(), item->serviceID(), item->subChId(), item->bitrate(), item->subCHSize(), item->address(), (const char*)byteLabel.constData(), picodeCount, NULL, announceFlags, 0x00);
    }
    else
    {
        QByteArray picodeValue ((const char*)picode, sizeof(picode));
        this->pCommManager->ReqSetSelectAnnouncementService(item->frequency(), item->serviceType(), item->serviceID(), item->subChId(), item->bitrate(), item->subCHSize(), item->address(), (const char*)byteLabel.constData(), picodeCount, (const char*)picodeValue.constData(), announceFlags, 0x00);
    }
    setServiceStatus(DAB_SERVICE_IDLE);
}

void
DABController::onReqCancelAlarmAnnouncement(void)
{
    DABChannelItem* item = pChannelManager->getCurrentChannelItem();
    QByteArray byteLabel;
    byteLabel.append(item->label());
    this->pCommManager->ReqSetSelectAnnouncementService(item->frequency(), item->serviceType(), item->serviceID(), item->subChId(), item->bitrate(), item->subCHSize(), item->address(), (const char*)byteLabel.constData(), 0, NULL, ANN_ALARM_ON, 0x00);
    setServiceStatus(DAB_SERVICE_IDLE);
}

void
DABController::onSettingSlideShow(bool isOn)
{
    qDebug() << Q_FUNC_INFO << "isOn: " << isOn;   
    this->pUIListener->setSlideShowOn(isOn);
}

void
DABController::onSettingServiceFollowing(int mode)
{
    qDebug() << Q_FUNC_INFO << "Mode: " << mode;
    this->pUIListener->setServiceFollowing(mode);
    if(mode == 3)
    {
        emit cmdStartDABtoFMTimer();
    }
    else
    {
        this->onEventExceptionHandler(DAB_EVENT_DAB_FM_OFF);
    }
}

void
DABController::onSettingBandSelection(int mode)
{
    qDebug() << Q_FUNC_INFO << "Mode: " << mode << " band : " << this->pUIListener->getBandSelection();
    this->pUIListener->setBandSelection(mode);
    this->pCommManager->ReqSetBandSelection(mode);
}

void
DABController::onSettingSaveData(void)
{
    this->pUIListener->saveSettingInfo();
}

void
DABController::CmdReqSetSeekStop(void)
{
    this->pCommManager->ReqSetSeekStop();
}

void
DABController::CmdReqSetSelectStop(void)
{
    this->pCommManager->ReqSetSelectStop();
}

void
DABController::onChangeDABtoFM(void)
{
    this->m_iLinkingState = 2;
    this->pUIListener->setServiceEvent("DAB to FM");
    emit displayLinkingIcon(false, true, false);
    setServiceStatusForDebugging("DAB-FM Weak");
    pChannelManager->updateChannelItemStatus(DAB_CHANNEL_DAB_TO_FM, true);
    setServiveFollowingState(DAB_SERVICE_FOLLOWING_STATE_AUDIO_FM);
//    DABChannelItem* item = pChannelManager->getCurrentChannelItem();
//    emit cmdReqdisplayOSD(item->label(), item->ensembleLabel(), DAB_OP_TYPE_SELECT);
}

void
DABController::loadModuleVersion(void)
{
    QSettings settings(DAB_MODULE_VERSION_FILE, QSettings::IniFormat);
    if(settings.childGroups().contains("AppDAB_ModuleVersion",Qt::CaseInsensitive))
    {
        settings.beginGroup("AppDAB_ModuleVersion");
        this->pUIListener->setBootloaderVer(settings.value("Bootloader").toString());
        this->pUIListener->setKernelVer(settings.value("Kernel").toString());
        this->pUIListener->setApplicationVer(settings.value("Application").toString());
        settings.endGroup();
    }
}

void
DABController::writeModuleVersion(void)
{
    QSettings settings(DAB_MODULE_VERSION_FILE, QSettings::IniFormat);
    settings.clear();
    settings.beginGroup("AppDAB_ModuleVersion");
    settings.setValue("initDABModule", true);
    settings.setValue("Bootloader", this->pUIListener->getBootloaderVer());
    settings.setValue("Kernel", this->pUIListener->getKernelVer());
    settings.setValue("Application", this->pUIListener->getApplicationVer());
    settings.endGroup();
    settings.sync();
}

void
DABController::ReqGetVersion(void)
{
    this->m_iReqGetVersionCount++;
    if(this->m_bGetVersionRequested)
    {
        if(m_iReqGetVersionCount == 30)
        {
            this->pCommManager->restartUART();
            this->m_iReqGetVersionCount = 0;
            this->pCommManager->ReqGetVersion();
            this->m_bGetVersionRequested = true;
        }
        return;
    }
    this->pCommManager->ReqGetVersion();
    this->m_bGetVersionRequested = true;
}

void
DABController::RspGetVersion(quint8 bootloaderMajorVersion, quint8 bootloaderMinorVersion, quint8 kernelMajor, quint8 kernelMinor, quint8 appMajor, quint8 appMinor, quint8 appMicro, QString buildTime, quint8 bootCount)
{
    qDebug() << Q_FUNC_INFO;
    DABLogger::instance()->Log((QString("  void DABController::RspGetVersion()")));

#ifdef __DAB_DABUG_INFO__
    qDebug() << "Version Bootloader : v" <<  bootloaderMajorVersion << "." << bootloaderMinorVersion;
    qDebug() << "        Kernel     : v" <<  kernelMajor << "." << kernelMinor;
    qDebug() << "        App        : v" <<  appMajor << "." << appMinor << "." << appMicro;
    qDebug() << "        Build Time : " <<  buildTime;
    qDebug() << "        Boot Count : " <<  bootCount;

    QString imsi1 = QString::number(bootloaderMajorVersion);
    QString imsi2 = QString::number(bootloaderMinorVersion);
    QString strBootloaderVer = imsi1 + "." + imsi2;
    this->pUIListener->setBootloaderVer(strBootloaderVer);

    imsi1 = QString::number(kernelMajor);
    imsi2 = QString::number(kernelMinor);
    QString strKernelVer = imsi1 + "." + imsi2;
    this->pUIListener->setKernelVer(strKernelVer);

    imsi1 = QString::number(appMajor, 16);
    imsi2 = QString::number(appMinor, 16);
    QString imsi3 = QString::number(appMicro, 16);
    QString strApplicatonVer = imsi1 + "." + imsi2 + "." + imsi3;
    this->pUIListener->setApplicationVer(strApplicatonVer);
    quint8 count = this->pUIListener->getBootCount();
    count = count+bootCount;
    this->pUIListener->setBootCount(count);
#endif

    // Module version save
    this->writeModuleVersion();

    this->m_iReqGetVersionCount = 0;
    this->m_bGetVersionRequested = false;

    // REQ_SET_EVT_SEND_TIME
    this->pCommManager->ReqSetEvtSendTime(EVT_SEND_TIME_START);

    // REQ_SET_SERVICE_FOLLOWING
    this->pCommManager->RegSetServiceFollowing(DAB_SERVICE_FOLLOWING_ON);

    // REQ_SET_BROADCASTING
    this->pCommManager->ReqSetBroadcasting(eREQ_SET_BROADCASTING_DAB_DMB);

#if 1 // 2013.08.05 sendServiceInfo 호출 시점을 first select service 이후로 변경한다.
    // REQ_SET_SERVICE_INFO  
    sendServiceInfo();
#endif
    // REQ_SET_AUTOSCAN
    this->pCommManager->ReqSetAutoScan(pUIListener->getBandSelection(), eREQ_SET_AUTO_SCAN_ACTION_START, eREQ_SET_AUTO_SCAN_DEBUG_ON);
    this->pCommManager->sendBackupCommandFromRebooting();
}

void
DABController::sendServiceInfo(void)
{
    qDebug() << Q_FUNC_INFO;
    ListModel* m_tempListChannel = new ListModel(new DABChannelItem(this), this);
    ListModel* pList = this->pChannelManager->getCurrentChannelList();
    ListModel* pPresetList = this->pChannelManager->getCurrentPresetList();
    DABChannelItem* pFindItem = NULL;
    DABChannelItem* pListItem = NULL;
    DABChannelItem* pItem = NULL;
    int tempCount = 0;
    int listCount = pList->rowCount();
    int presetCount = pPresetList->rowCount();

    for(int i=0; i<listCount; i++)
    {
        pItem = (DABChannelItem*)pList->dataItem(i);
        m_tempListChannel->appendRow(new DABChannelItem(pItem));
    }

    for(int i=0; i<presetCount; i++)
    {
        pItem = (DABChannelItem*)pPresetList->dataItem(i);
        pFindItem = (DABChannelItem*)m_tempListChannel->findItem(pItem->key(), pItem->subKey(), pItem->id());
        if(pFindItem == NULL)
        {
            m_tempListChannel->appendRow(new DABChannelItem(pItem));
        }
    }

    QStandardItemModel *model = new QStandardItemModel(0, 2, this);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("index"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("frequency"));

    tempCount = m_tempListChannel->rowCount();
    for(int i = 0; i < tempCount; i++)
    {
        pItem = (DABChannelItem*)m_tempListChannel->dataItem(i);        
        model->insertRow(0);
        model->setData(model->index(0, 0), i);
        model->setData(model->index(0, 1), pItem->frequency());
    }

    model->sort(1, Qt::AscendingOrder);
    int index = 0;
    for(int i = 0; i < tempCount; i++)
    {
        index = model->data(model->index(i, 0)).toInt();
        pListItem = (DABChannelItem*)m_tempListChannel->dataItem(index);
        quint16 picode[12] ={0,};
        bool isActivate = false;
        int picodeCount = 0;
        for(int j=0; j<pListItem->picodeInfo().PICodeCount; j++)
        {
            quint8 activate = (pListItem->picodeInfo().PICodeValidation >> (15-j)) & 0x01;
            if(pListItem->picodeInfo().PICodeType[j] == 0x00 && (activate == 0x01))
            {
                picode[picodeCount] = pListItem->picodeInfo().PICodeValue[j];
                isActivate = true;
                picodeCount++;
            }
        }

        if(pListItem->ps() == 0x00)
            continue;

        QByteArray byteLabel;
        byteLabel.append(pListItem->label());

        if(picodeCount == 0)
        {
            this->pCommManager->ReqSetServiceInfo(pListItem->frequency(), pListItem->serviceType(), pListItem->serviceID(), pListItem->subChId(), pListItem->bitrate(), 0x00, (const char*)byteLabel.constData(), picodeCount, NULL);
        }
        else
        {
            QByteArray picodeValue ((const char*)picode, sizeof(picode));
            this->pCommManager->ReqSetServiceInfo(pListItem->frequency(), pListItem->serviceType(), pListItem->serviceID(), pListItem->subChId(), pListItem->bitrate(), 0x00, (const char*)byteLabel.constData(), picodeCount, (const char*)picodeValue.constData());
        }

        if((i > 10)  && (i % 20) == 0)
        {
            usleep(100000);
        }
    }
}

void
DABController::ReqSetSeekService(eREQ_SET_FULL_SCAN_BAND band, eREQ_SET_SEEK_SERVICE_DIRECTION direction, bool isEnsembleSeek)
{
    DABChannelItem* pItem = pChannelManager->getCurrentChannelItem();

    if(pItem->frequency() == 0 || pItem->serviceID() == 0)
    {
        qDebug() << Q_FUNC_INFO << " ::  Frequency:"<< pItem->frequency()<<", ServiceID:"<<pItem->serviceID()<<". Can't Seek service..";
        return;
    }

    quint8 filterType = eREQ_SET_SEEK_SERVICE_FILTERTYPE_NONE;
    quint8 filterValueLength = 0;
    QByteArray filterValue;
    quint32 frequency = pItem->frequency();
    quint32 serviceID = 0;
    quint8 subChId = 0;

    if(!isEnsembleSeek)
    {
        this->onEventExceptionHandler(DAB_EVENT_SHORT_SEEK);
        serviceID = pItem->serviceID();
        subChId = pItem->subChId();
        this->setServiceStatus(DAB_SERVICE_SHORT_SEEK);
    }
    else
    {
        this->onEventExceptionHandler(DAB_EVENT_LONG_SEEK);
        this->setServiceStatus(DAB_SERVICE_LONG_SEEK);
    }

    setStatus(DAB_STATUS_WAITING);
    emit cmdReqClusterControl(pItem->label(), pItem->presetIndex(), DAB_CLUSTER_RECEVING_STATE);
    this->pCommManager->ReqSetSeekService(band, direction, filterType, filterValueLength, filterValue, frequency, serviceID, subChId);
    StopTASearchTimer();
    emit cmdStopDABtoFMTimer();
    pUIListener->setEPG(false);

    if(m_bMuteCheck == true)
    {
        emit cmdSendMuteCancel();
    }
}

void
DABController::RspSetSeekService(eREQ_SET_FULL_SCAN_BAND band, quint8 direction, quint8 filterType, quint8 filterValue, quint32 frequency, quint32 serviceID, quint8 subChId)
{
    qWarning() << Q_FUNC_INFO;
    qWarning() << "= Band     : " << band;
    qWarning() << "= Direction: " << direction;
    qWarning() << "= Filter Type : " << filterType;
    qWarning() << "= Filter Value: " << filterValue;
    qWarning() << "= Frequency: " << frequency;
    qWarning() << "= ServiceID: " << serviceID;
    qWarning() << "= subChId  : " << subChId;
}

void
DABController::ReqFirstSelectService(void)
{
    qDebug() << Q_FUNC_INFO;
    DABChannelItem* item = pChannelManager->getCurrentChannelItem();

    if(item == NULL)
    {
        qDebug() << __FUNCTION__ << " channel item is null !!. so return";
        return;
    }

    if(item->frequency() == 0 || item->serviceID() == 0)
    {
        // Empty Item..
        qDebug() << Q_FUNC_INFO;
        qDebug() << " Frequency:"<<item->frequency()<<", ServiceID:"<<item->serviceID()<<". Can't select service..";
        return;
    }

    this->onEventExceptionHandler(DAB_EVENT_SELECT_SERVICE);
    QByteArray byteLabel;
    byteLabel.append(item->label());

    quint16 picode[12] ={0,};
    int picodeCount = 0;
    for(int j=0; j<item->picodeInfo().PICodeCount; j++)
    {
        quint8 activate = (item->picodeInfo().PICodeValidation >> (15-j)) & 0x01;
        if(item->picodeInfo().PICodeType[j] == 0x00 && (activate == 0x01))
        {
            picode[picodeCount] = item->picodeInfo().PICodeValue[j];
            picodeCount++;
        }
    }

    if(picodeCount == 0)
    {
        this->pCommManager->ReqSetSelectService(item->frequency(), item->serviceType(), item->serviceID(), item->subChId(), item->bitrate(), item->subCHSize(), item->address(), item->ps(), (const char*)byteLabel.constData(), picodeCount, NULL);
    }
    else
    {
        this->pCommManager->ReqSetSelectService(item->frequency(), item->serviceType(), item->serviceID(), item->subChId(), item->bitrate(), item->subCHSize(), item->address(), item->ps(), (const char*)byteLabel.constData(), picodeCount, picode);
    }

    setStatus(DAB_STATUS_WAITING);
    this->pUIListener->modeChange(false);
    this->pUIListener->initDLS(false);
    pChannelManager->updateChannelItemStatus(DAB_CHANNEL_ICON_CLEAR, true);
    if(!(this->m_bListScanning || this->m_bPresetScanning))
    {
        setServiceStatus(DAB_SERVICE_SELECT);
    }        
    if(pChannelManager->getCurrentChannelIndexFromChannelList() == -1)
    {
        pChannelManager->addCurrentItemInChannelList(item);
    }
}

void
DABController::ReqSetSelectService(void)
{
    qDebug() << Q_FUNC_INFO;
    DABChannelItem* item = pChannelManager->getCurrentChannelItem();

    if(item == NULL)
    {
        qDebug() << __FUNCTION__ << " channel item is null !!. so return";
        return;
    }

    if(item->frequency() == 0 || item->serviceID() == 0)
    {
        qDebug() << __FUNCTION__ << " Frequency:"<<item->frequency()<<", ServiceID:"<<item->serviceID()<<". Can't select service..";
        CmdReqSetSelectStop();
        return;
    }

    this->pUIListener->setChangeFrequencyID(item->frequency());
    this->onEventExceptionHandler(DAB_EVENT_SELECT_SERVICE);

    QByteArray byteLabel;
    byteLabel.append(item->label());

    quint16 picode[12] ={0,};
    int picodeCount = 0;
    for(int j=0; j<item->picodeInfo().PICodeCount; j++)
    {
        quint8 activate = (item->picodeInfo().PICodeValidation >> (15-j)) & 0x01;

        if(item->picodeInfo().PICodeType[j] == 0x00 && (activate == 0x01))
        {
            picode[picodeCount] = item->picodeInfo().PICodeValue[j];
            picodeCount++;
        }
    }

    if(picodeCount == 0)
    {
        this->pCommManager->ReqSetSelectService(item->frequency(), item->serviceType(), item->serviceID(), item->subChId(), item->bitrate(), item->subCHSize(), item->address(), item->ps(), (const char*)byteLabel.constData(), picodeCount, NULL);
    }
    else
    {
        this->pCommManager->ReqSetSelectService(item->frequency(), item->serviceType(), item->serviceID(), item->subChId(), item->bitrate(), item->subCHSize(), item->address(), item->ps(), (const char*)byteLabel.constData(), picodeCount, picode);
    }

    setStatus(DAB_STATUS_WAITING);
    /**
    2013.06.22 Mobilus DaehyungE
    NoSignal 아이콘 바로 없애기 위해서 Servie Status 값을 "None"으로 셋팅한다.
    None을 셋팅하는 이유는 Module쪽으로 Select Event를 보내면 Module Status값으로 "None"을 내려주기 때문이다.
    */
    setServiceStatusForDebugging("None");
    this->pUIListener->modeChange(false);
    this->pUIListener->initDLS(false);
    pChannelManager->updateChannelItemStatus(DAB_CHANNEL_ICON_CLEAR, true);


    if(this->m_bListScanning || this->m_bPresetScanning)
    {
        //emit closeOptionMenu();
    }
    else
    {
        setServiceStatus(DAB_SERVICE_SELECT);
    }

    onSetCurrentInfoToHomeMedia();
    emit cmdReqClusterControl(item->label(), this->pUIListener->getPresetIndex(), DAB_CLUSTER_NOMAR_STATE);
    emit cmdStopDABtoFMTimer();
    StopTASearchTimer();

    emit loadEPGListBySelect();

    if(m_bMuteCheck == true)
    {
        emit cmdSendMuteCancel();
    }
}

void
DABController::RspSetSelectService(quint32 frequency, quint8 serviceType, quint32 serviceID, quint8 subChId, quint16 bitrate, QString label)
{
    qWarning() << Q_FUNC_INFO << "; freq = " << frequency << ", sType = " << serviceType << ", sID = " << serviceID << ", subID = " << subChId << ", bitrate = " << bitrate << ", label = " << label;

    if(this->m_bListScanning || this->m_bPresetScanning)
    {
        qDebug() << "Scanning.....";
    }
    else
    {
        setServiceStatus(DAB_SERVICE_IDLE);
    }
}

void
DABController::RspSetAnnouncementFlag(quint16 flag)
{
    qWarning() << Q_FUNC_INFO << " ; flag = 0x" << hex << flag;
}

void
DABController::RspGetServiceList()
{
//    qDebug() << Q_FUNC_INFO << ": event = " << m_iServiceEvent;

    switch(m_iServiceEvent)
    {
        case EVT_SEND_CHANGE_SERVICE_EVENT_NORMAL:
            emit cmdUpdateChannelListFromAutoScan();
            if(this->m_eServiceStatus != DAB_SERVICE_ANNOUNCEMENT)
                this->setServiceStatus(DAB_SERVICE_IDLE);            
            break;

        case EVT_SEND_CHANGE_SERVICE_EVENT_SEEK:
            m_iServiceEvent = EVT_SEND_CHANGE_SERVICE_EVENT_NORMAL;
            break;

        case EVT_SEND_CHANGE_SERVICE_EVENT_RECONFIGURATION_CHANGE_SERVICE:
            emit cmdUpdateChannelListFromReconfiguration();
            break;

        case EVT_SEND_CHANGE_SERVICE_EVENT_RECONFIGURATION_NOT_CHANGE_SERVICE:
            emit cmdUpdateChannelListFromReconfigurationNotChange();
            break;

        default:
            break;
    }

    m_iServiceEvent = EVT_SEND_CHANGE_SERVICE_EVENT_NORMAL;
}

void
DABController::onRspGetDLS(QString sDLS, QString sTitle, QString sAlbum, QString sArtist)
{
    Q_UNUSED(sAlbum);

    DABChannelItem* pCurrentItem = this->pChannelManager->getCurrentChannelItem();
    if(this->m_eServiceStatus == DAB_SERVICE_ANNOUNCEMENT && m_iTASubChId != pCurrentItem->subChId())
    {
        qDebug() << __FUNCTION__ << " Now TA service status !! so DLS / DL+  ignored!!" << ", m_iTASubChId = " << m_iTASubChId << ", current SubChID = " << pCurrentItem->subChId() << ", m_eServiceStatus = " << this->m_eServiceStatus;
        return;
    }

    if(this->pUIListener->checkCurrentChannelItem())
    {
        m_iDLSTimeCount = 0;
        this->pUIListener->setDLS(sDLS);

        if(sTitle != "" || sArtist != "")
        {
            m_iDLPlustTimeCount = 0;
        }

        if(sTitle != "")
        {
            QString str = sTitle.simplified();
            this->pUIListener->setTitle(str);
        }

        if(sArtist != "")
        {
            QString str = sArtist.simplified();
            this->pUIListener->setArtist(str);
        }
    }
    else
    {
        qDebug() << __FUNCTION__ << " Now View Channel is not Current Channel Info. so DLS ignored!!";
    }
}

void
DABController::RspGetSLS(QString fileName)
{
    DABChannelItem* pCurrentItem = this->pChannelManager->getCurrentChannelItem();
    if(this->m_eServiceStatus == DAB_SERVICE_ANNOUNCEMENT && m_iTASubChId != pCurrentItem->subChId())
    {
        qDebug() << __FUNCTION__ << " Now TA service status !! so SLS ignored!!" << ", m_iTASubChId = " << m_iTASubChId << ", current SubChID = " << pCurrentItem->subChId() << ", m_eServiceStatus = " << this->m_eServiceStatus;
        return;
    }

    if(this->pUIListener->checkCurrentChannelItem())
    {
        m_iSLSTimeCount = 0;
        if(this->m_eStatus == DAB_STATUS_SERVICE) // reqselect after update SLS, rspselect before timming issue
            this->pUIListener->setSLS(fileName);
    }
    else
    {
        qDebug() << __FUNCTION__ << " Now View Channel is not Current Channel Info. so SLS ignored!!";
    }
}

void
DABController::removeDuplicatEPGData(quint32 sID, quint32 mjd)
{
    qDebug() << __FUNCTION__ << " : Current EPG Data removed !!";
    pUIListener->removeCurrentEPG(sID, mjd);
}

void
DABController::onRspGetEPG(quint32 mjd, quint32 sID)
{
    qDebug() << Q_FUNC_INFO << "EPG Received." << ", sID = " << sID;
    pUIListener->onSortingEPGData(mjd, sID);
}

void
DABController::EvtSendModuleStatus(eEVT_SEND_MODULE_STATUS moduleState, eEVT_SEND_SERVICE_STATUS serviceState)
{
//    qDebug() << Q_FUNC_INFO << " ==ModuleStatus== module : " << moduleState << ", service: "<<serviceState;;

    setModuleStatus(moduleState);

    switch(moduleState)
    {
        case EVT_SEND_MODULE_STATUS_BOOT:
            setModulesStatusForDebugging("Booting");
            break;

        case EVT_SEND_MODULE_STATUS_IDLE:
            if(serviceState == EVT_SEND_SERVICE_STATUS_NONE)
            {
                this->ReqGetVersion();
            }

            setModulesStatusForDebugging("Idle");
            break;

        case EVT_SEND_MODULE_STATUS_READY:
            if(serviceState == EVT_SEND_SERVICE_STATUS_NONE)
            {
                setStatus(DAB_STATUS_READY);
                setServiceStatus(DAB_SERVICE_READY);
                if(this->pChannelManager->getCurrentChannelCount() != 0)
                {
                    this->ReqFirstSelectService();
                }
            }
            setModulesStatusForDebugging("Ready");
            break;

    case EVT_SEND_MODULE_STATUS_WAITING:
        switch(serviceState)
        {
            case EVT_SEND_SERVICE_STATUS_NONE:
                setServiceStatusForDebugging("None");
                break;

            case EVT_SEND_SERVICE_STATUS_GOOD:
                break;

            case EVT_SEND_SERVICE_STATUS_WEAK:
                break;

            default:
                qDebug() << " Module Status: SERVICE, but Unknown Service Status: " << serviceState;
                break;
        }

#ifdef __DAB_DABUG_INFO__
        setModulesStatusForDebugging("Waiting");
#endif
        break;

    case EVT_SEND_MODULE_STATUS_SERVICE:
        setStatus(DAB_STATUS_SERVICE);
        switch(serviceState)
        {
            case EVT_SEND_SERVICE_STATUS_NONE:
                setServiceStatusForDebugging("None");
                break;

            case EVT_SEND_SERVICE_STATUS_GOOD:
                this->pCommManager->ReqGetAudioInfo();
                break;

            case EVT_SEND_SERVICE_STATUS_WEAK:
                break;

            default:
                qDebug() << " Module Status: SERVICE, but Unknown Service Status: " << serviceState;
                break;
        }

#ifdef __DAB_DABUG_INFO__
        setModulesStatusForDebugging("Service");
#endif
        break;

    case EVT_SEND_MODULE_STATUS_SCANNING:
        setStatus(DAB_STATUS_SCANNING);
        setModulesStatusForDebugging("Scanning");
        break;

    case EVT_SEND_MODULE_STATUS_SCANSTOP:
        setModulesStatusForDebugging("Scan Stop");
        break;

    case EVT_SEND_MODULE_STATUS_SCANEND:
        setModulesStatusForDebugging("Scan End");
        break;

    case  EVT_SEND_MODULE_STATUS_SEEKEND:
        qDebug() << " ------ Seek Ended...";
        setStatus(DAB_STATUS_READY);
        onReqSeekCancel();
        break;

    case EVT_SEND_MODULE_STATUS_SEEKSTOP:
        setStatus(DAB_STATUS_READY);
        break;

    default:
        qDebug() << " Error! EVT_SEND_MODULE_STATUS Unknown ModuleState:" << moduleState;
        setModulesStatusForDebugging("Unknown");
        break;
    }
}

void
DABController::EvtSendChangeService(int freq, int serviceType, int serviceID, int SubChId, int Bitrate, int Pty, QString label, QString ensembelLabel,eEVT_SEND_CHANGE_SERVICE_EVENT event)

{
//    qDebug() << __FUNCTION__ << "Freq:"<<freq<<", serviceType:"<<serviceType<<", serviceID:"<<serviceID<<", subChId:"<<SubChId<<", Bitrate:"<<Bitrate<<", Label:"<<label<<", event:" << event << ", ensembelLabel:" << ensembelLabel << ", Pty:" << Pty;
    QString serviceEvent = " ";

    switch(event)
    {
        case EVT_SEND_CHANGE_SERVICE_EVENT_SELECT_SERVICE:
            serviceEvent = "SELECT";
            this->pUIListener->setEnsembleSeek(false);
    #ifdef __DAB_DABUG_INFO__
            {
            QString imsi = QString::number(serviceID, 16);
            pUIListener->setCurrentServiceID(imsi);
            pUIListener->setFrequencyID(freq);
            }
    #endif
            this->pUIListener->saveChannelInfo();
            setServiceStatus(DAB_SERVICE_IDLE);
            pUIListener->removeDLSSLSData();
            this->initPICodeValue();
            emit cmdStartDABtoFMTimer();
            StartTASearchTimer();
            this->sendPICodeToMicom();
            m_bIgnoreBADSignal = 0;
            break;

        case EVT_SEND_CHANGE_SERVICE_EVENT_SEEK:
            qDebug() << "EVT_SEND_CHANGE_SERVICE_EVENT_SEEK : ensemblelabel : " << ensembelLabel;
            serviceEvent = "SEEK";
            this->pUIListener->setEnsembleSeek(false);
            setCurrentChannelItemForSeek(freq, serviceType, serviceID, SubChId, Bitrate, Pty, label, ensembelLabel);
            this->pUIListener->modeChange(false);
            this->pUIListener->initDLS(false);
            this->pUIListener->setChangeFrequencyID(freq);
            pChannelManager->updateChannelItemStatus(DAB_CHANNEL_ICON_CLEAR, true);
            m_iServiceEvent = EVT_SEND_CHANGE_SERVICE_EVENT_SEEK;
            this->pCommManager->ReqGetServiceList(freq);
            setServiceStatus(DAB_SERVICE_IDLE);
            pUIListener->removeDLSSLSData();
            onSetCurrentInfoToHomeMedia();
            this->pUIListener->saveChannelInfo();
            m_bIgnoreBADSignal = 0;

    #ifdef __DAB_DABUG_INFO__
            {
            QString imsi = QString::number(serviceID, 16);
            pUIListener->setCurrentServiceID(imsi);
            pUIListener->setFrequencyID(freq);
            }
    #endif
            setServiceStatus(DAB_SERVICE_IDLE);
            emit cmdStartDABtoFMTimer();
            StartTASearchTimer();
            this->initPICodeValue();
            this->sendPICodeToMicom();
            break;

        case EVT_SEND_CHANGE_SERVICE_EVENT_ANNOUNCEMENT:
            serviceEvent = "ANNOUNCEMENT";
            this->pUIListener->setEnsembleSeek(false);
            break;

        case EVT_SEND_CHANGE_SERVICE_EVENT_RECONFIGURATION_CHANGE_SERVICE:
            serviceEvent = "RECONFIGURATION CHANGE";
            /**
              2013.11.05 DaehyungE  requested MJY
              1. Reconfiguration change event는 Audio 변경되기 때문에 OSD, Cluster 둘다 업데이트 한다.
              2. main 화면 정보도 업데이트 한다.
            */
            if(isListStarted() || isPresetScanStarted())
                return;

            if(this->m_iLinkingState == 0)
                setCurrentChannelItem(freq, serviceType, serviceID, SubChId, Bitrate, label, ensembelLabel);

            this->pUIListener->changeNormalMode();
            setServiceStatus(DAB_SERVICE_IDLE);
            onSetCurrentInfoToHomeMedia();
            this->pUIListener->removeDLSSLSData();
            m_iServiceEvent = EVT_SEND_CHANGE_SERVICE_EVENT_RECONFIGURATION_CHANGE_SERVICE;
            this->pCommManager->ReqGetServiceList(freq);
            this->pUIListener->setChangeFrequencyID(freq);
    #ifdef __DAB_DABUG_INFO__
            {
            QString imsi = QString::number(serviceID, 16);
            pUIListener->setCurrentServiceID(imsi);
            pUIListener->setFrequencyID(freq);
            }
    #endif
            break;

        case EVT_SEND_CHANGE_SERVICE_EVENT_RECONFIGURATION_NOT_CHANGE_SERVICE:
        {
            /**
              2013.11.05 DaehyungE  requested MJY
              1. serivce label이 바뀔경우에만 cluster 업데이트 한다.
              2. OSD는 업데이트 하지 않는다.
              3. main 화면은 Ensemble/Label이 변경될 경우에만 업데이트 한다.
            */
            if(isListStarted() || isPresetScanStarted())
                return;

            serviceEvent = "RECONFIGURATION NOT CHANGE";
            setServiceStatus(DAB_SERVICE_IDLE);

            DABChannelItem *pItem = this->pChannelManager->getCurrentChannelItem();
            if(pItem->label() != label && this->m_iLinkingState == 0)
            {
                qDebug() << __FUNCTION__ << " EVT_SEND_CHANGE_SERVICE_EVENT_RECONFIGURATION_NOT_CHANGE_SERVICE :: cluster label update";
                emit cmdReqClusterControl(label, this->pUIListener->getPresetIndex(), DAB_CLUSTER_NOMAR_STATE);
            }
            qDebug() << " current Ensemble = " << pItem->ensembleLabel() << ", Label = " << pItem->label();
            if((pItem->ensembleLabel() != ensembelLabel || pItem->label() != label) && (this->m_iLinkingState == 0))
            {
                qDebug() << __FUNCTION__ << " EVT_SEND_CHANGE_SERVICE_EVENT_RECONFIGURATION_NOT_CHANGE_SERVICE :: main player info update : ensembelLabel = " << ensembelLabel;
                setCurrentChannelItem(freq, serviceType, serviceID, SubChId, Bitrate, label, ensembelLabel);
                this->pUIListener->changeNormalMode();
            }

            m_iServiceEvent = EVT_SEND_CHANGE_SERVICE_EVENT_RECONFIGURATION_NOT_CHANGE_SERVICE;
            this->pCommManager->ReqGetServiceList(freq);

    #ifdef __DAB_DABUG_INFO__
            QString imsi = QString::number(serviceID, 16);
            pUIListener->setCurrentServiceID(imsi);
            pUIListener->setFrequencyID(freq);
    #endif
        }
            break;

        case EVT_SEND_CHANGE_SERVICE_EVENT_DAB_TO_DAB:
        {
//            if(isListStarted() || isPresetScanStarted())
//            {
//                qDebug() << "============================ EVT_SEND_CHANGE_SERVICE_EVENT_DAB_TO_DAB :: Now Scanning Status. !!. so not linking update";
//                return;
//            }

            DABChannelItem* item = pChannelManager->getCurrentChannelItem();
            QString imsi = QString::number(serviceID, 16);
            pUIListener->setFrequencyID(item->frequency());
            pUIListener->setCurrentServiceID(imsi);
            pUIListener->setChangeFrequencyID(freq);

            qDebug() << " ============================ EVT_SEND_CHANGE_SERVICE_EVENT_DAB_TO_DAB : current freq = " << item->frequency() << ". freq = " << freq << ", ServiceFollowingState = " << this->getServiceFollowingState();

            if(item->frequency() == (quint32)freq)
                this->m_iLinkingState = 0;
            else
                this->m_iLinkingState = 1;

            if(this->getServiceFollowingState() == DAB_SERVICE_FOLLOWING_STATE_AUDIO_FM)
            {
                this->pUIListener->setDABGoodCount(0);
                /**
                 2013.06.22, Mobilus DaehyungE
                 DAB-DAB Linking이 발생하더라도 DAB-FM 상태에서는 바로 DAB-DAB 전환하지 않고 10초동안 Signal Status Good이 들어 올때 전환한다.
                 그래서 여기서 linkingState를 DAB-DAB로 셋팅하지 않고, EvtSendSignalInfo()에서 DAB-DAB 값을 셋팅한다.
                 이렇게 처리하는 이유는 HMETC 요청사항으로 DAB-FM. DAB-DAB 빈번하게 일어나는 현상을 막기 위함이다.
                 this->m_iLinkingState = 2;
                 */
            }
            else
            {
                if(this->m_iLinkingState == 0)
                {
                    serviceEvent = "DAB TO DAB";
                    pChannelManager->updateChannelItemStatus(DAB_CHANNEL_DAB_TO_DAB, false);
                    emit displayLinkingIcon(false, false, false);
                }
                else
                {
                    serviceEvent = "DAB TO DAB";
                    pChannelManager->updateChannelItemStatus(DAB_CHANNEL_DAB_TO_DAB, true);
                    emit displayLinkingIcon(true, false, false);
                }
            }
        }
            break;

    case EVT_SEND_CHANGE_SERVICE_EVENT_PLAY_PREVIOUS_SERVICE:
        serviceEvent = "PLAY PREVIOUS";
        break;

    default:
        break;
    }

#ifdef __DAB_DABUG_INFO__
    this->pUIListener->setServiceEvent(serviceEvent);
#endif
}

void
DABController::EvtSendAnnouncement(QString label, int flag, eEVT_SEND_ANNOUNCEMENT_STATUS status, quint8 subChID)
{
    qDebug() << Q_FUNC_INFO << " :  Flag = " << flag << " label = " << label << " status = " << status << ", subChID = " << subChID;

    if(m_bDeactiveTA && status == EVT_SEND_ANNOUNCEMENT_START)
    {
        qDebug() << __FUNCTION__ << " If switched from DAB-FM to DAB or AV Mode Changed, TA/Alarm Service ignored for 5 seconds";
        if(flag & 0x0001)
        {
            qDebug() << __FUNCTION__ << " Alarm Service ignored for 5 seconds";
        }
        else if(flag & 0x0002)
        {
            qDebug() << __FUNCTION__ << " TA Service ignored for 5 seconds";
        }
        return;
    }

    quint16 announcmentFlag = this->pUIListener->getAnnouncementFlag();
    if((announcmentFlag == ANN_ALARM_ON) && (flag & 0x0002))
    {
        qDebug() << __FUNCTION__ << " TA Setting value is off. so ignored !!";
        onReqCancelAlarmAnnouncement();
        return;
    }

    if(status == EVT_SEND_ANNOUNCEMENT_START)
    {
        if(isListStarted() || isPresetScanStarted())
        {
            qDebug() << __FUNCTION__ << " Now Scanning!!. so Announcement ignored !!";
        }        
        else if(this->getServiceFollowingState() == DAB_SERVICE_FOLLOWING_STATE_AUDIO_FM)
        {
            qDebug() << __FUNCTION__ << " Now DAB-FM Linking state !!, so Announcement event ignored !!";
        }
        else
        {            
            this->pUIListener->announcementScreenOn(true, flag, label);
            setServiceStatus(DAB_SERVICE_ANNOUNCEMENT);
            m_iTASubChId = subChID;
        }
    }
    else if(status == EVT_SEND_ANNOUNCEMENT_FINISH)
    {
        this->pUIListener->announcementScreenOn(false, flag, label);
        setServiceStatus(DAB_SERVICE_IDLE);
        m_iTASubChId = 0;
        this->pCommManager->ReqGetPinkStatus();
    }
}

void
DABController::EvtSendTime(int year, int month, int day, int hour, int minute, int second)
{
    QDateTime dateTime = DABLogger::instance()->getBroadcastingTime();
#if 1
    if(dateTime.isValid() && !m_bLoadReservationItem)
    {
        m_bLoadReservationItem = true;
    }
#else
    if(dateTime.isValid() && !m_bLoadReservationItem)
    {
        this->pEPGReservationList->loadReservationItem();
        m_bLoadReservationItem = true;
    }
#endif

    setSystemTimeFromModule(year, month, day, hour, minute, second);
}

void
DABController::EvtSendQOS(int CER, int SNR, int RSSI, int CER_sub, int SNR_sub, int RSSI_sub)
{
    pUIListener->setCER(CER);
    pUIListener->setSNR(SNR);
    pUIListener->setRSSI(RSSI);

    pUIListener->setCER_sub(CER_sub);
    pUIListener->setSNR_sub(SNR_sub);
    pUIListener->setRSSI_sub(RSSI_sub);
}

void
DABController::EvtUpdateDLS(quint8 update)
{
    if(update == 0x00)
    {
        this->pUIListener->initDLPlus();
    }
    else if(update == 0x01)
    {
        this->pCommManager->ReqGetDLS();
    }
}

void
DABController::EvtUpdateSLS()
{
    this->pCommManager->ReqGetSLS();
}

void
DABController::EvtUpdateEPG()
{
    qDebug() << Q_FUNC_INFO;
    this->pCommManager->ReqGetEPG();
}

#ifdef __DTC__
void
DABController::EvtSendDtcUart(void)
{
    int count = pUIListener->getReqCountDABtoMicom();
    pUIListener->setReqCountDABtoMicom(count+1);

    if(pUIListener->getReqCountDABtoMicom() > 3 )
    {
        pUIListener->setReqCountDABtoMicom(0);
        if(m_bRequestDABtoFM == true)
        {
            this->m_bPICodeStatus = MICOM_STATUS_FM_NO_AVAILABLE;
            this->m_bPICodeSensitive = 0x00;
            m_bRequestDABtoFM = false;
        }
    }

    m_iSLSTimeCount++;
    if(m_iSLSTimeCount > m_iSLSTimeout)
    {
        qDebug() << " ====================== SLS initialize !! =======================";
        m_iSLSTimeCount = 0;
        if(this->getServiceFollowingState() != DAB_SERVICE_FOLLOWING_STATE_AUDIO_FM)
        {
            this->pUIListener->setSLS("");
        }
    }

    m_iDLSTimeCount++;
    if(m_iDLSTimeCount > m_iDLSTimeout)
    {
        qDebug() << " ====================== DLS initialize !! =======================";
        m_iDLSTimeCount = 0;
        if(this->getServiceFollowingState() != DAB_SERVICE_FOLLOWING_STATE_AUDIO_FM)
        {
            this->pUIListener->initDLS();
        }
    }

    m_iDLPlustTimeCount++;
    if(m_iDLPlustTimeCount > m_iDLPlustTimeout)
    {
        qDebug() << " ====================== DL Plus initialize !! =======================";
        m_iDLPlustTimeCount = 0;
        if(this->getServiceFollowingState() != DAB_SERVICE_FOLLOWING_STATE_AUDIO_FM)
        {
            this->pUIListener->initDLPlus();
        }
    }

    if(this->m_AliveMsgOnOff)
    {
        qDebug() << __FUNCTION__ << " : DTC UART CHECK OFF !!";
        this->m_AliveUartMsg = false;
        return;
    }
    this->m_AliveUartMsg = true;
}
#endif

void
DABController::setStatus(CONTROLLER_STATUS status)
{
    this->m_eStatus = status;

    switch(m_eStatus)
    {
        case DAB_STATUS_STARTUP:
            pUIListener->setControllerStatus("StartUp");
            break;
        case DAB_STATUS_IDLE:
            pUIListener->setControllerStatus("Idle");
            break;
        case DAB_STATUS_READY:
            pUIListener->setControllerStatus("Ready");
            pUIListener->removeDLSSLSData();
            break;
        case DAB_STATUS_SCANNING:
            pUIListener->setControllerStatus("Scanning");
            pUIListener->removeDLSSLSData();
            break;
        case DAB_STATUS_WAITING:
            /**
            2013.06.22 Mobilus DaehyungE
            Weak Timer가 돌고있는 상태에서 Select Service시 Weak Timer가 초기화 되지 않는 현상 발생. Select시 Weak Timer를 Stop 시킨다.
            */
            StopWeakSignalTimer();
            pUIListener->setControllerStatus("Waiting");
            pUIListener->removeDLSSLSData();
            break;
        case DAB_STATUS_SERVICE:
            pUIListener->setControllerStatus("Service");
            break;
        case DAB_STATUS_END:
            pUIListener->setControllerStatus("End");
            pUIListener->removeDLSSLSData();
            break;
        default:
            pUIListener->setControllerStatus("Unknown");
            break;
    }
}

void
DABController::setSystemTimeFromModule(int year, int month, int day, int hour, int minute, int second)
{
    QString str;
    QDate date(year, month, day);
    QTime time(hour, minute, second);

    QString str1 = date.toString();
    QString str2 = time.toString();
    str = str1 + " :: " + str2;

    pUIListener->setSystemTime(str);
    StopBroadcastingTimer();
    DABLogger::instance()->setBroadcastingTime(date, time);
    StartBroadcastingTimer();
}

void
DABController::listScanStart(void)
{
    if(isPresetScanStarted())
    {
        PresetScanStop();
    }

    this->m_bListScanning = true;
    setServiceStatus(DAB_SERVICE_LIST_SCAN);
    this->pUIListener->setListScanningOn(true);
}

void
DABController::listScanStop(void)
{
    if(isPresetScanStarted())
    {
        PresetScanStop();
    }

    StopScanTimer();
    this->m_bListScanning = false;
    this->pUIListener->setListScanningOn(false);
    setServiceStatus(DAB_SERVICE_IDLE);
}

void
DABController::allScanStop(void)
{
    qDebug() << __FUNCTION__ << ":: All Channel Scan : " << isListStarted() << ", Preset Scan : " << isPresetScanStarted();

    if(isListStarted())
    {
        listScanStop();
    }

    if(isPresetScanStarted())
    {
        PresetScanStop();
    }
}

void
DABController::PresetScanStart(void)
{
    if(isListStarted())
    {
        listScanStop();
    }
    this->m_bPresetScanning = true;    
    this->pUIListener->setPresetScanningOn(true);
    setServiceStatus(DAB_SERVICE_LIST_SCAN);
}

void
DABController::PresetScanStop(void)
{
    StopScanTimer();
    this->m_bPresetScanning = false;
    this->pUIListener->setPresetScanningOn(false);
    setServiceStatus(DAB_SERVICE_IDLE);
}

void
DABController::StartScanTimer(quint8 serviceType)
{
    qDebug() << __FUNCTION__ << " :: channel serviceType = " << serviceType;

    int interval = 0;

    switch(serviceType)
    {
        case SERVICE_TYPE_DAB:
        interval = DAB_SCAN_INTERVAL;
        break;

        case SERVICE_TYPE_DAB_PLUS:
        interval = DAB_PLUS_SCAN_INTERVAL;
        break;

        case SERVICE_TYPE_DMB:
        interval = DMB_SCAN_INTERVAL;
        break;
    }

    m_ScanTimer.start(interval);
}

void
DABController::StopScanTimer(void)
{
    qDebug() << Q_FUNC_INFO;
    if(m_ScanTimer.isActive())
    {
        qDebug() << " Scan Timer Stop..";
        m_ScanTimer.stop();
    }
}

void
DABController::ScanTimeOut(void)
{
    qDebug() << Q_FUNC_INFO << " TimeOut Called....";

    if(isListStarted())
    {
        DABChannelItem* pItem = pChannelManager->getCurrentChannelItem();
        int index = pChannelManager->findNextChannelIndexForChannelScan(pItem);

        if(index == -1)
        {
            listScanStop();
            this->pUIListener->setCurrentChannelInfoFromChannelList(this->m_iPlayIndexBackup);
            this->m_bExistCurrentChannel = false;
        }
        else if(this->m_bExistCurrentChannel && index == 0)
        {
            listScanStop();
            this->pUIListener->setCurrentChannelInfoFromChannelList(this->m_iPlayIndexBackup);
            this->m_bExistCurrentChannel = false;
        }
        else if(this->m_iPlayIndexBackup == index)
        {
            listScanStop();
            this->pUIListener->setCurrentChannelInfoFromChannelList(this->m_iPlayIndexBackup);
        }
        else
        {
            this->pUIListener->setCurrentChannelInfoFromChannelList(index);
            DABChannelItem* pCurrentChannelItem = pChannelManager->getCurrentChannelItem();
            StartScanTimer(pCurrentChannelItem->serviceType());
        }

        qDebug() << " Next Scan Index : " << index;
        this->ReqSetSelectService();
        emit menuEnableInitialized();		
        setServiceStatus(DAB_SERVICE_LIST_SCAN);
        onSetCurrentInfoToHomeMedia();
        DABChannelItem* cItem = pChannelManager->getCurrentChannelItem();
        emit playIndexUpdateForScan(cItem->frequency(), cItem->serviceID(), cItem->label());
    }

    if(isPresetScanStarted())
    {
        DABChannelItem* pItem = pChannelManager->getCurrentChannelItem();
        int index = pChannelManager->findNextChannelIndexInPresetList(pItem);

        qDebug() << "m_iPlayIndexBackup = " << this->m_iPlayIndexBackup << "  index = " << index << " m_bExistCurrentChannelInPreset = " << this->m_bExistCurrentChannelInPreset;

        if(this->m_bExistCurrentChannelInPreset && index == 0)
        {
            PresetScanStop();
            this->pUIListener->setCurrentChannelInfoFromPresetList(this->m_iPlayIndexBackup);
            this->m_bExistCurrentChannelInPreset = false;
            if(pChannelManager->getCurrentChannelIndexFromChannelList() == -1)
            {
                DABChannelItem* pPresetItem = pChannelManager->getCurrentChannelItem();
                pChannelManager->addCurrentItemInChannelList(pPresetItem);
            }
        }
        else if(this->m_iPlayIndexBackup == index)
        {
            PresetScanStop();
            this->pUIListener->setCurrentChannelInfoFromPresetList(this->m_iPlayIndexBackup);

            if(pChannelManager->getCurrentChannelIndexFromChannelList() == -1)
            {
                DABChannelItem* pPresetItem = pChannelManager->getCurrentChannelItem();
                pChannelManager->addCurrentItemInChannelList(pPresetItem);
            }
        }
        else
        {
            this->pUIListener->setCurrentChannelInfoFromPresetList(index);
            DABChannelItem* pCurrentChannelItem = pChannelManager->getCurrentChannelItem();
            StartScanTimer(pCurrentChannelItem->serviceType());
            if(pChannelManager->getCurrentChannelIndexFromChannelList() == -1)
            {
                DABChannelItem* pItem = pChannelManager->getChannelItemFromPresetList(index);
                pChannelManager->addCurrentItemInChannelList(pItem);
            }
        }
        this->ReqSetSelectService();
        emit menuEnableInitialized();
        setServiceStatus(DAB_SERVICE_LIST_SCAN);
        onSetCurrentInfoToHomeMedia();
    }
}

void
DABController::StartWeakSignalTimer(void)
{
    qDebug() << Q_FUNC_INFO << " m_WeakSignalTimer.isActive = " << m_WeakSignalTimer.isActive();
//    if(!m_WeakSignalTimer.isActive())
//    {
//        qDebug() << " WeakSignal Timer Start..";
//        m_WeakSignalTimer.start(WEAK_TIME_INTERVAL);
//    }

    if(m_WeakSignalTimer.isActive())
    {
        m_WeakSignalTimer.setInterval(WEAK_TIME_INTERVAL);

    }
    else
    {
        m_WeakSignalTimer.start(WEAK_TIME_INTERVAL);
    }
}

void
DABController::StopWeakSignalTimer(void)
{
    qDebug() << Q_FUNC_INFO << " m_WeakSignalTimer.isActive = " << m_WeakSignalTimer.isActive();
    if(m_WeakSignalTimer.isActive())
    {
        qDebug() << " weak signal Timer Stop..";
        m_WeakSignalTimer.stop();
    }
}

void
DABController::WeakTimeOut(void)
{
    qDebug() << Q_FUNC_INFO << " TimeOut Called....";
    StopWeakSignalTimer();

    if(m_TuneTimer.isActive())
    {
        qDebug() << __FUNCTION__ << " Tune Timer Stop..";
        m_TuneTimer.stop();
        this->pUIListener->modeChange(false);
    }

    if(this->m_eServiceStatus == DAB_SERVICE_ANNOUNCEMENT)
    {
        qDebug() << __FUNCTION__ << " Now TA service status !! so weak signal ignored!!" << ", m_iTASubChId = " << m_iTASubChId << ", m_eServiceStatus = " << this->m_eServiceStatus;
        return;
    }

    if(this->getServiceFollowingState() == DAB_SERVICE_FOLLOWING_STATE_AUDIO_FM)
    {
        qDebug() << __FUNCTION__ << " Current status is DAB to FM !!. so not appear No Signal icon";
        setServiceStatusForDebugging("DAB-FM Weak");
        return;
    }

    DABChannelItem* item = pChannelManager->getCurrentChannelItem();
    if(item->isNoSignal() == false)
    {
        emit cmdReqClusterControl(item->label(), this->pUIListener->getPresetIndex(), DAB_CLUSTER_SIGNAL_WEAK_STATE);
        emit cmdReqdisplayOSD(item->label(), item->ensembleLabel(), DAB_OP_TYPE_NOSIGNAL);
        pChannelManager->updateChannelItemStatus(DAB_CHANNEL_NO_SIGNAL, true);
    }
    else
    {
        qDebug() << __FUNCTION__ << " :: alredady no signal!!! so return";
    }

/**
  2013.07.24 Mobilus DaehyungE
  Weak Signal이 발생하면 동일한 주파수에서 현재 Play 채널외에 다른 채널들을 다 삭제한다.(HMETC 요구사항)
  예외사항으로 Weak signal이 발생했는데 현재 Play 채널이 Station List에서 없을경우에는 삭제하지 않는다.(지역 이동간의 삭제되는 현상이 나올수 있기때문) <== require by LG MJY
  여기서 삭제하지 않더라도 Auto Scan에 의해서 unlock이 내려오면 해달 채널들은 전부 삭제되기 때문에 문제되지 않을것으로 판단된다.
  */
    ListModel* currentList = pChannelManager->getCurrentChannelList();
    DABChannelItem* pNextItem = NULL;
    int channelCount = currentList->rowCount();
    bool existCurrentChannel = false;
    for(int i = 0; i<channelCount; i++)
    {
        pNextItem = (DABChannelItem*)currentList->dataItem(i);
        if(pNextItem->frequency() == item->frequency())
        {
            qWarning() << __FUNCTION__ << "Current Channel Item :: freq = " << item->frequency() << ", subChid =  " << item->subChId() << ", serviceID = " << item->serviceID();
            qWarning() << __FUNCTION__ << "   Next Channel Item :: freq = " << pNextItem->frequency() << ", subChid =  " << pNextItem->subChId() << ", serviceID = " << pNextItem->serviceID();
            if((pNextItem->serviceID() == item->serviceID()) && (pNextItem->subChId() == item->subChId()) && (pNextItem->frequency() == item->frequency()))
            {
                qDebug() << __FUNCTION__ << " current channel is exist !!";
                existCurrentChannel = true;
            }
        }
    }

    if(existCurrentChannel)
        emit cmdRemoveUnlockFrequency(item->frequency());

    setServiceStatusForDebugging("Weak");
}

void
DABController::StartTuneTimer(void)
{
    qDebug() << " Tune Timer Start.. Active = " << m_TuneTimer.isActive();
    if(m_TuneTimer.isActive())
    {
        m_TuneTimer.setInterval(5000);
    }
    else
    {
        m_TuneTimer.start(5000);
    }
}

void
DABController::StopTuneTimer(void)
{
    qDebug() << Q_FUNC_INFO;
    if(m_TuneTimer.isActive())
    {
        qDebug() << " Tune Timer Stop..";
        m_TuneTimer.stop();
    }
}

void
DABController::onTuneTimeOut(void)
{
    qDebug() << Q_FUNC_INFO;
    StopTuneTimer();
    this->pUIListener->modeChange(false);
}

void
DABController::StartTASearchTimer(void)
{
    qDebug() << Q_FUNC_INFO << " Tune Timer Start.. Active = " << m_TASearchTimer.isActive();
    if(m_TASearchTimer.isActive())
    {
        m_TASearchTimer.setInterval(TA_SEARCH_INTERVAL);
    }
    else
    {
        m_TASearchTimer.start(TA_SEARCH_INTERVAL);
    }
}

void
DABController::StopTASearchTimer(void)
{
    if(m_TASearchTimer.isActive())
    {
        qDebug() << Q_FUNC_INFO << " TA search timer stop Stop..";
        m_TASearchTimer.stop();
    }
}

void
DABController::onTASearchTimeOut(void)
{
    quint16 flag = this->pUIListener->getAnnouncementFlag();
    qDebug() << Q_FUNC_INFO << " announcement flag = 0x" << hex << flag;

    StopTASearchTimer();

    if(isListStarted() || isPresetScanStarted())
    {
        qDebug() << __FUNCTION__ << " : if Channel Scan or Preset Scan, not TA Search!!";
    }
    else
    {
        if(flag == ANN_TRAFFIC_ALARM_ON)
        {
            this->checkTAService();
        }
    }
}

void
DABController::StartBroadcastingTimer(void)
{
    m_BroadcastingTimer.start(BROADCASTING_TIME_INTERVAL);
}

void
DABController::StopBroadcastingTimer(void)
{
    if(m_BroadcastingTimer.isActive())
    {
        m_BroadcastingTimer.stop();
    }
}

void
DABController::onBroadcastingTimeTick(void)
{
    QDateTime dateTime = DABLogger::instance()->getBroadcastingTime();
    QTime time = dateTime.time().addSecs(1);
    dateTime.setTime(time);
    DABLogger::instance()->setBroadcastingTime(dateTime.date(), dateTime.time());
    qWarning() << "======================== broadcasting date time ========================= " << dateTime.toString() << ", ReservationCount = " << m_iReservationCount;

    if(m_iReservationCount)
    {
        emit cmdCheckReservationItem();
    }

    if(!pEPGReservationList->isEmpty())
    {
        DABEPGReservationItem* item = pEPGReservationList->getReservationItem();
        if(item == NULL)
        {
            qDebug() << __FUNCTION__ << " : EPG Reservation item is null !!. so return;";
            return;
        }
        QDate currentDate = QDate::currentDate();
        quint32 mjd = DABLogger::instance()->convertDateToMJD(currentDate);
        if(mjd == item->getMJD())
        {
            QTime reserveTime(item->getHour(), item->getMinute(), item->getSecond());
            QTime eTime = reserveTime.addSecs(item->getDuration());
            qDebug() << __FUNCTION__ << "     reserve end time = " << eTime.toString() << ", current time :" << time.toString() << ", item->getDuration() = " << item->getDuration() << "reserveTime = " << reserveTime.toString();

            QTime rTime(0,0,0);
            if(rTime == eTime)
            {
                eTime.setHMS(23,59,59);
            }

            if(time > eTime)
            {
                qDebug() << __FUNCTION__ << " : Reservation broadcasting time ended!!. so close popup!!";
                emit cmdCloseSystemPopup(DAB_POPUP_PRESERVE_TYPE);
            }
        }
    }
}

void
DABController::checkEPGReservationList(void)
{
    qDebug() << Q_FUNC_INFO;

    if(!pEPGReservationList->isEmpty())
    {
        DABEPGReservationItem* item = pEPGReservationList->getReservationItem();
        if(item == NULL)
        {
            qDebug() << __FUNCTION__ << " : EPG Reservation item is null !!. so return;";
            return;
        }
        QDate currentDate = QDate::currentDate();
        quint32 mjd = DABLogger::instance()->convertDateToMJD(currentDate);
        if(mjd == item->getMJD())
        {
            QTime reserveTime(item->getHour(), item->getMinute(), item->getSecond());
            QDateTime dateTime = DABLogger::instance()->getBroadcastingTime();
            QTime currentTime = dateTime.time();
            qWarning() << __FUNCTION__ << "     reserve time = " << reserveTime.toString() << ", current time :" << currentTime.toString();
            if(currentTime > reserveTime)
            {
                qWarning() << __FUNCTION__ << " : EPG Reservation broadcasting is already started !! , Title = " << item->getProgramtitle() << ", sevice name = " << item->getLabel();
                m_sBackupEPGTitle = item->getProgramtitle();
                m_sBackupEPGServiceName = item->getLabel();
                onCancelEPGReservation(true);
                QTimer::singleShot(3000, this, SLOT(onOSDEPGStarted()));
            }
        }
    }
}

void
DABController::StartDeactiveFMLinkingTimer(void)
{
    qDebug() << Q_FUNC_INFO;
    if(m_DeactiveFMLinkingTimer.isActive())
    {
        qDebug() << __FUNCTION__ << " Deactive FM Linking Timer is running !!. so Timer Stop and Timer restart !!";
        m_DeactiveFMLinkingTimer.stop();
        m_DeactiveFMLinkingTimer.start(FM_WEAK_TIME_INTERVAL);
    }
    else
    {
        m_DeactiveFMLinkingTimer.start(FM_WEAK_TIME_INTERVAL);
    }
    m_bDeactiveFMLinking = true;
}

void
DABController::StopDeactiveFMLinkingTimer(void)
{
    qDebug() << Q_FUNC_INFO;
    if(m_DeactiveFMLinkingTimer.isActive())
    {
        qDebug() << " Deactive FM Linking Timer Stop..";
        m_DeactiveFMLinkingTimer.stop();
        m_bDeactiveFMLinking = false;
    }
}

void
DABController::StartDeactiveTATimer(void)
{
    qDebug() << Q_FUNC_INFO;
    m_bDeactiveTA = true;
    if(m_DeactiveTATimer.isActive())
    {
        qDebug() << __FUNCTION__ << " Deactive TA Timer is running !!. so Timer Stop and Timer restart !!";
        m_DeactiveTATimer.stop();
        m_DeactiveTATimer.start(DEACTIVE_TA_INTERVAL);
    }
    else
    {
        m_DeactiveTATimer.start(DEACTIVE_TA_INTERVAL);
    }
}

void
DABController::StopDeactiveTATimer(void)
{
    qDebug() << Q_FUNC_INFO;
    if(m_DeactiveTATimer.isActive())
    {
        qDebug() << " Deactive TA Timer Stop..";
        m_DeactiveTATimer.stop();
        m_bDeactiveTA = false;
    }
}

void
DABController::setCurrentChannelItem(int freq, int serviceType, int serviceID, int subChId, int bitrate, QString label, QString ensembelLabel)
{
    DABChannelItem* pItem = pChannelManager->getCurrentChannelItem();
    pItem->setFrequency(freq);
    pItem->setEnsembleLabel(ensembelLabel);
    pItem->setServiceID(serviceID);
    pItem->setServiceType(serviceType);
    pItem->setSubChId(subChId);
    pItem->setBitrate(bitrate);
    pItem->setLabel(label);
    pItem->setPresetIndex(pChannelManager->isExistPresetList(pItem));
}

void
DABController::setCurrentChannelItemForSeek(int freq, int serviceType, int serviceID, int subChId, int bitrate, int pty, QString label, QString ensembelLabel)
{
    DABChannelItem* pItem = pChannelManager->getCurrentChannelItem();
    pItem->setFrequency(freq);
    pItem->setEnsembleLabel(ensembelLabel);
    pItem->setServiceID(serviceID);
    pItem->setServiceType(serviceType);
    pItem->setSubChId(subChId);
    pItem->setBitrate(bitrate);
    pItem->setProgrameType(pty);
    pItem->setLabel(label);
    pItem->setPresetIndex(pChannelManager->isExistPresetList(pItem));
    pItem->clearLiningInfo();
}

void
DABController::sendChannelItem(quint32 freq, quint16 eId, quint8 iId, QString eLabel, quint8 sCount, quint32 sID, quint8 sType, quint8 subChId, quint16 bitrate, quint8 ps, quint8 charset, quint8 pty, quint8 language, QString sLabel, stASU_INFO asuInfo, stPICODE_INFO picodeInfo, quint8 subCHSize, quint16 address)
{
//    qDebug()  << Q_FUNC_INFO << "Current Service Status : " << this->getServiceStatus() << " IsAutoScan : " << this->m_bIsAutoScan;

    if(freq == 0)
    {
        qDebug()  << Q_FUNC_INFO << "Current Service Status : " << this->getServiceStatus() << " IsAutoScan : " << this->m_bIsAutoScan;
        return;
    }

    if(m_iServiceEvent == EVT_SEND_CHANGE_SERVICE_EVENT_SEEK)
    {
        qDebug() << " DAB_SERVICE_SEEK : cmdUpdateChannelItem called !!";
        emit cmdUpdateChannelItem(freq, eId, iId, eLabel, sCount, sID, sType, subChId, bitrate, ps, charset, pty, language, sLabel, asuInfo, picodeInfo, subCHSize, address);
    }
    else if(m_iServiceEvent == EVT_SEND_CHANGE_SERVICE_EVENT_RECONFIGURATION_CHANGE_SERVICE || m_iServiceEvent == EVT_SEND_CHANGE_SERVICE_EVENT_RECONFIGURATION_NOT_CHANGE_SERVICE)
    {
        qDebug() << " DAB_SERVICE_RECONFIGURATION or DAB_SERVICE_RECONFIGURATION : cmdAddTempChannelItem called!!";
        emit cmdAddNewChannelItem(freq, eId, iId, eLabel, sCount, sID, sType, subChId, bitrate,ps, charset, pty, language, sLabel, asuInfo, picodeInfo, subCHSize, address);
    }
    else if(m_iServiceEvent == EVT_SEND_CHANGE_SERVICE_EVENT_NORMAL)
    {
        emit cmdAddTempChannelItem(freq, eId, iId, eLabel, sCount, sID, sType, subChId, bitrate,ps, charset, pty, language, sLabel, asuInfo, picodeInfo, subCHSize, address);
    }
}

void
DABController::onSendClusterUpdate(void)
{
    qDebug() << Q_FUNC_INFO;
    DABChannelItem* item = this->pChannelManager->getCurrentChannelItem();
    emit cmdReqClusterControl(item->label(), this->pUIListener->getPresetIndex(), DAB_CLUSTER_NOMAR_STATE);
}

void
DABController::onSendOSDUpdate(void)
{
    qDebug() << Q_FUNC_INFO;
    DABChannelItem* item = this->pChannelManager->getCurrentChannelItem();
    emit cmdReqdisplayOSD(item->label(), item->ensembleLabel(), DAB_OP_TYPE_SELECT);
}

void
DABController::onSetCurrentInfoToHomeMedia(void)
{
    qDebug() << __FUNCTION__ << " : service status = " << this->getServiceStatus();

    switch(this->getServiceStatus())
    {
        case DAB_SERVICE_IDLE:
        {
            DABChannelItem* item = pChannelManager->getCurrentChannelItem();
            qDebug() << Q_FUNC_INFO << " : Current staus is idle :: isNoSignal = " << item->isNoSignal();
            if(item->isNoSignal())
            {
                emit cmdReqdisplayOSD(item->label(), item->ensembleLabel(), DAB_OP_TYPE_NOSIGNAL);
                emit cmdReqClusterControl(item->label(), this->pUIListener->getPresetIndex(), DAB_CLUSTER_SIGNAL_WEAK_STATE);
            }
            else
            {
                emit cmdReqdisplayOSD(item->label(), item->ensembleLabel(), DAB_OP_TYPE_SELECT);
                emit cmdReqClusterControl(item->label(), this->pUIListener->getPresetIndex(), DAB_CLUSTER_NOMAR_STATE);
            }
            break;
        }

        case DAB_SERVICE_FULL_SCAN:
        case DAB_SERVICE_SINGLE_SCAN:
            this->allScanStop();
            break;

        case DAB_SERVICE_LIST_SCAN:
        {
            DABChannelItem* item = pChannelManager->getCurrentChannelItem();
            emit cmdReqdisplayOSD(item->label(), item->ensembleLabel(), DAB_OP_TYPE_SCAN);
            break;
        }

        case DAB_SERVICE_SELECT:
        {
            DABChannelItem* item = pChannelManager->getCurrentChannelItem();
            this->allScanStop();
            emit cmdReqdisplayOSD(item->label(), item->ensembleLabel(), DAB_OP_TYPE_SELECT);
            break;
        }

        case DAB_SERVICE_TUNE_SEEK:
        {
            DABChannelItem* item = this->pUIListener->getViewCurrentItem();
            emit cmdReqdisplayOSD(item->label(), item->ensembleLabel(), DAB_OP_TYPE_TUNE_SEEK);
            break;
        }

        case DAB_SERVICE_LONG_SEEK:
        case DAB_SERVICE_SHORT_SEEK:
        {
            this->allScanStop();
            DABChannelItem* item = pChannelManager->getCurrentChannelItem();
            emit cmdReqdisplayOSD(item->label(), item->ensembleLabel(), DAB_OP_TYPE_SEARCHING);
            break;
        }

        case DAB_SERVICE_ANNOUNCEMENT:
            this->allScanStop();
            break;

        case DAB_SERVICE_READY:
        {
            DABChannelItem* item = pChannelManager->getCurrentChannelItem();
            qDebug() << Q_FUNC_INFO << " : Current staus is DAB_SERVICE_READY ";
            emit cmdReqdisplayOSD(item->label(), item->ensembleLabel(), DAB_OP_TYPE_SELECT);
            emit cmdReqClusterControl(item->label(), this->pUIListener->getPresetIndex(), DAB_CLUSTER_NOMAR_STATE);
            break;
        }
        case DAB_SERVICE_HANDOVER:
        case DAB_SERVICE_RECONFIGURATION:
            break;

        default :
            break;
    }
}

void
DABController::changeFMtoDAB(void)
{
    DABChannelItem* item = pChannelManager->getCurrentChannelItem();
    quint32 freq = pUIListener->getChangeFrequencyID();
    qDebug() << __FUNCTION__ << " new frequency = " << item->frequency() << ", old frequency = " << freq;

    if(item->frequency() == freq)
    {
        this->m_iLinkingState = 0;
        pChannelManager->updateChannelItemStatus(DAB_CHANNEL_DAB_TO_FM, false);
    }
    else if(freq !=0)
    {
        this->m_iLinkingState = 1;
        pChannelManager->updateChannelItemStatus(DAB_CHANNEL_DAB_TO_FM, false);
        pChannelManager->updateChannelItemStatus(DAB_CHANNEL_DAB_TO_DAB, true);
    }
    setStatus(DAB_STATUS_SERVICE);
    setServiceStatus(DAB_SERVICE_IDLE);
    qDebug() << __FUNCTION__ << " this->m_iLinkingState = " << this->m_iLinkingState;
    if(this->m_iLinkingState == 1)
        emit displayLinkingIcon(true, false, false);
    else
        emit displayLinkingIcon(false, false, false);

    this->pUIListener->setDABGoodCount(0);

    this->m_bPICodeStatus = MICOM_STATUS_FM_NO_AVAILABLE;
    this->m_bPICodeSensitive = 0x00;
    setServiceStatusForDebugging("None");
    this->pUIListener->setServiceEvent("FM to DAB");
    setServiveFollowingState(DAB_SERVICE_FOLLOWING_STATE_AUDIO_DAB);
}

void
DABController::requestFMtoDAB(void)
{
    emit cmdReqRadioEnd(this->getRadioPlayBand(), this->getRadioPlayFreq());
}

bool
DABController::checkSwitchingTime(void)
{
    bool ret = false;
    qDebug() << __FUNCTION__ << "DAB Cood Count = " << this->pUIListener->getDABGoodCount() << ", FMtoDABState = " << m_eFMtoDABState;

    switch(m_eFMtoDABState)
    {
        case FM_DAB_STATE_FIRST:
            if(this->pUIListener->getDABGoodCount() > DAB_TO_FM_FIRST_INTERVAL)
            {
                m_eFMtoDABState = FM_DAB_STATE_SECOND;
                this->pUIListener->setFMtoDABInterval(DAB_TO_FM_SECOND_INTERVAL);
                this->pUIListener->setDABGoodCount(0);
                ret = true;
            }
            break;

        case FM_DAB_STATE_SECOND:
            if(this->pUIListener->getDABGoodCount() > DAB_TO_FM_SECOND_INTERVAL)
            {
                m_eFMtoDABState = FM_DAB_STATE_THIRD;
                this->pUIListener->setFMtoDABInterval(DAB_TO_FM_THIRD_INTERVAL);
                this->pUIListener->setDABGoodCount(0);
                ret = true;
            }
            break;

        case FM_DAB_STATE_THIRD:
            if(this->pUIListener->getDABGoodCount() > DAB_TO_FM_THIRD_INTERVAL)
            {
                this->pUIListener->setDABGoodCount(0);
                ret = true;
            }
            break;

        default:
            if(this->pUIListener->getDABGoodCount() > DAB_TO_FM_FIRST_INTERVAL)
            {
                m_eFMtoDABState = FM_DAB_STATE_FIRST;
                this->pUIListener->setFMtoDABInterval(DAB_TO_FM_FIRST_INTERVAL);
                this->pUIListener->setDABGoodCount(0);
                ret = true;
            }
            break;
    }

    return ret;
}

void
DABController::EvtServiceFollowingState(quint8 state)
{
#if 1  // 2013.06.22 Mobilus DaehyungE. HMETC 요청사항으로 linking 전환시 회전 아이콘을 삭제한다.
    Q_UNUSED(state);
    return;
#else

    qDebug() << Q_FUNC_INFO << " : state = " << state;

    switch(state)
    {
        case DAB_SERVICE_FOLLOWING_STATE_READY:
            if(this->getServiceFollowingState())
            {
                qDebug() << "==================== case DAB_SERVICE_FOLLOWING_STATE_READY : Current FM Playing. so searching icon is not update !!" ;
            }
            else
            {
                qDebug() << "==================== case DAB_SERVICE_FOLLOWING_STATE_READY : searching icon is update !!" ;
                emit displayLinkingIcon(false, false, true);
            }
            break;

        case DAB_SERVICE_FOLLOWING_STATE_SWAP:
            qDebug() << "  ==================== DAB_SERVICE_FOLLOWING_STATE_SWAP ";
            break;

        case DAB_SERVICE_FOLLOWING_STATE_STOP:
            qDebug() << "==================== case DAB_SERVICE_FOLLOWING_STATE_STOP : m_iLinkingState = " << m_iLinkingState;
            if(m_iLinkingState == 1) // DAB to DAB
                emit displayLinkingIcon(true, false, false);
            else if(m_iLinkingState == 2) // DAB to FM
                emit displayLinkingIcon(false, true, false);
            else
                emit displayLinkingIcon(false, false, false);
            break;

        default:
            break;
    }
#endif
}

void
DABController::EvtSendPinkStatus(quint8 status)
{    
    qDebug() << Q_FUNC_INFO << " :: =========== status : " << status << ", service following state : " << this->getServiceFollowingState() << ",  m_eServiceStatus = " << this->m_eServiceStatus;

    StopWeakSignalTimer();
    if(status == 0x01)
    {
        if(this->getServiceFollowingState() == DAB_SERVICE_FOLLOWING_STATE_AUDIO_FM)
        {
            setServiceStatusForDebugging("DAB-FM Weak");
        }
        else
        {
            StartWeakSignalTimer();
        }
    }
    else
    {
        DABChannelItem* item = this->pChannelManager->getCurrentChannelItem();
        qDebug() << __FUNCTION__ << " : isDABtoDAB = " << item->isDABtoDAB() << ", isDABtoFM = " << item->isDABtoFM() << ", isNoSignal = " << item->isNoSignal();
        if(item->isNoSignal())
        {
            pChannelManager->updateChannelItemStatus(DAB_CHANNEL_NO_SIGNAL, false);
            emit cmdReqClusterControl(item->label(), this->pUIListener->getPresetIndex(), DAB_CLUSTER_NOMAR_STATE);
            if(this->m_iLinkingState == 1)
                pChannelManager->updateChannelItemStatus(DAB_CHANNEL_DAB_TO_DAB, true);
        }
        setServiceStatusForDebugging("Good");
    }
}

void
DABController::EvtSendSignalInfo(quint8 status)
{
    qWarning() << __FUNCTION__ << "==== DAB SERVICE STATUS : DABtoFM status = " << this->getServiceFollowingState() << ", Module Signal status = " << status << ", service status = " << this->pUIListener->getServiceStatus() << ", m_bIgnoreBADSignal = " << m_bIgnoreBADSignal;

    int goodCount = 0;
    if(m_bIgnoreBADSignal < SIGNAL_IGNORE_COUNT)
    {
        qWarning() << " bIgnoreBADSignal count = " << m_bIgnoreBADSignal << ", so EvtSendSignalInfo ignored";
        m_bIgnoreBADSignal++;
        return;
    }

    if(this->getServiceFollowingState() == DAB_SERVICE_FOLLOWING_STATE_AUDIO_FM)
    {
        switch(status)
        {
            case DAB_SIGNAL_GOOD:
                goodCount = this->pUIListener->getDABGoodCount();
                goodCount++;
                this->pUIListener->setDABGoodCount(goodCount);

                if(checkSwitchingTime())
                {
                    requestFMtoDAB();
                    StopWeakSignalTimer();
                }
                break;

            case DAB_SIGNAL_NO_GOOD:
            case DAB_SIGNAL_BAD:
                this->pUIListener->setDABGoodCount(0);
                break;

        default:
            break;
        }
    }
    else if(this->getServiceFollowingState() == DAB_SERVICE_FOLLOWING_STATE_AUDIO_DAB)
    {
        switch(status)
        {
            case DAB_SIGNAL_GOOD:     // Good
                goodCount = this->pUIListener->getDABGoodCount();
                goodCount++;
                this->pUIListener->setDABGoodCount(goodCount);
                 if(this->pUIListener->getDABGoodCount() > DAB_TO_FM_INIT_INTERVAL && m_eFMtoDABState != FM_DAB_STATE_FIRST)
                 {
                     m_eFMtoDABState = FM_DAB_STATE_FIRST;
                     this->pUIListener->setFMtoDABInterval(DAB_TO_FM_FIRST_INTERVAL);
                     this->pUIListener->setDABGoodCount(0);
                 }
                 break;

            case DAB_SIGNAL_NO_GOOD:
            case DAB_SIGNAL_BAD:
                this->pUIListener->setDABGoodCount(0);
                requestDABtoFM();                
                break;

            default:
                break;
        }
    }
    else
    {
        qWarning() << __FUNCTION__ << "==== DAB SERVICE STATUS : ServiceFollowingState = " << this->getServiceFollowingState() << ", Module Signal status = " << status << ", service status = " << this->pUIListener->getServiceStatus();
        return;
    }

    this->pUIListener->setSignalStatus(status);

    if(goodCount > DAB_TO_FM_INIT_INTERVAL)
        this->pUIListener->setDABGoodCount(0);
}

void
DABController::requestDABtoFM(void)
{
    if(!checkDABtoFM())
    {
        qDebug() << __FUNCTION__ << " :: DAB NORMAL STATUS : Status is BAD : service status is not IDLE!!. so DABtoFM not Request!!";
        return;
    }

    if(m_bDeactiveFMLinking)
    {
        qDebug() << __FUNCTION__ << " :: received FM Weak event. so ignored 5 second ";
        return;
    }

    if(m_bDeactiveDABtoFM)
    {
        qDebug() << __FUNCTION__ << " :: Now TA Service status !! so ignored DAB to FM ";
        return;
    }

    if(this->pUIListener->getServiceFollowing() == DAB_SERVICE_FOLLOWING_ON)
    {
        qDebug() << __FUNCTION__ << ": PI Code Status = 0x" << hex << this->m_bPICodeStatus << ", Sensitive = " << this->m_bPICodeSensitive;
        if(this->m_bRequestDABtoFM)
        {
            qDebug() << __FUNCTION__ << "=================== : DABtoFM not response from RDS. so PI Code is not send !!";
            return;
        }

        if(this->pUIListener->getEnsembleSeek())
        {
            qDebug() << __FUNCTION__ << " :: Now Seek Searching status !! so ignored DAB to FM ";
            return;
        }

        if(isListStarted() || isPresetScanStarted())
        {
            qDebug() << __FUNCTION__ << " :: Now Scanning status!! so ignored DAB to FM ";
            return;
        }

        if(this->m_bPICodeStatus == 0x01 && this->m_bPICodeSensitive == 0x01)
        {
            this->m_bPICodeStatus = MICOM_STATUS_FM_NO_AVAILABLE;
            this->m_bPICodeSensitive = 0x00;
            this->pUIListener->setServiceEvent("RequestDABtoFM");
            DABChannelItem* item = pChannelManager->getCurrentChannelItem();
            emit cmdReqDABtoFM(item->serviceID());
            if(item->isNoSignal())
            {
                pChannelManager->updateChannelItemStatus(DAB_CHANNEL_NO_SIGNAL, false);
                emit cmdReqClusterControl(item->label(), this->pUIListener->getPresetIndex(), DAB_CLUSTER_NOMAR_STATE);
            }
        }
    }
}

void
DABController::EvtAutoScanInfo(quint8 status, quint32 frequency, quint8 lock)
{
    Q_UNUSED(status);
//    qDebug( " Status : 0x%02X, Frequency : 0x%08X, Lock : 0x%02X", status, frequency, lock);

    DABChannelItem* pCurrentItem = pChannelManager->getCurrentChannelItem();

    if(lock == 0x01)
    {
        qDebug() << " EvtAutoScanInfo :: lock :: frequency = " << frequency;
        if((frequency == pCurrentItem->frequency()) && (this->pUIListener->getServiceStatus() == "Weak"))
        {
            qDebug() << __FUNCTION__ << ":: current frequency is weak status. so ignored. !!!";
        }
        else
        {
            this->pCommManager->ReqGetServiceList(frequency);
        }
    }
    else if(lock == 0x00)
    {
        if((frequency == pCurrentItem->frequency()) && (this->pUIListener->getServiceStatus() != "Weak"))
        {
            qDebug() << __FUNCTION__ << " frequency = " << frequency << " current frequency = " << pCurrentItem->frequency() <<" ==================Current Channel Services is not remove !!!!" ;
            return;
        }
        else
        {
            emit cmdRemoveUnlockFrequency(frequency);
        }
    }
}

void
DABController::EvtSendAnnouncementTAInvalid(quint16 flags)
{
    qDebug() << Q_FUNC_INFO << ": flags = " << hex << flags;
#if 0
    quint16 announcementFlags = this->pUIListener->getAnnouncementFlag();
    if(announcementFlags == ANN_TRAFFIC_ALARM_ON)
    {
        ListModel* pList = pChannelManager->getCurrentChannelList();
        int index = pList->findIndex(0x0002);
        if(index == -1)
        {
            qDebug() << __FUNCTION__ << " TA Station is not exist !!";
            emit cmdReqTASearchPopup(false);
        }
        else
        {
            qDebug() << __FUNCTION__ << " TA Station is exist !!";
            emit cmdReqTASearchPopup(true);
        }
    }
#endif
}

void
DABController::setModulesStatusForDebugging(QString str)
{
    this->pUIListener->setModuleStatus(str);
}

void
DABController::setServiceStatusForDebugging(QString str)
{
    this->pUIListener->setServiceStatus(str);
}

void
DABController::onAddEPGReservation(QDateTime dateTime, QString serviceName, int serviceID, QString title, QString description, int hour, int minute, int second, int duration)
{
    qDebug() << "ServiceID    : " << serviceID;
    qDebug() << "ProgramTitle : " << title;
    qDebug() << "Description  : " << description;
    qDebug() << "Hour         : " << hour;
    qDebug() << "Minute       : " << minute;
    qDebug() << "Second       : " << second;

    DABEPGReservationItem* pItem = new DABEPGReservationItem(this);
    pItem->setTime(hour, minute, second, duration);
    pItem->setProgramTitle(title);
    pItem->setDescription(description);
    pItem->setServiceID(serviceID);
    pItem->setLabel(serviceName);

    QDate date = dateTime.date();
    quint32 mjd = DABLogger::instance()->convertDateToMJD(date);
    pItem->setMJD(mjd);
#if 1
    QDateTime broadcastingTime = DABLogger::instance()->getBroadcastingTime();
    QTime currentTime = broadcastingTime.time();
#else
    QTime currentTime =  QTime::currentTime();
#endif
    qDebug() << "Current Time : " << currentTime.toString() << "Reserve Time : " << pItem->getStartTime().toString() << ", Reservation Count : " << pEPGReservationList->reservationCount;
    if( pEPGReservationList->reservationCount == 1 )
    {
        emit cmdReservationCountFull();
        return;
    }

    QDate currentDate = QDate::currentDate();
    quint32 currentMJD = DABLogger::instance()->convertDateToMJD(currentDate);

    if(currentMJD == mjd)
    {
        if(currentTime < pItem->getStartTime())
        {
            qDebug() << "Reseve Time is big than Current Time";

            DABChannelItem* pCurrentItem = pChannelManager->getCurrentChannelItem();
            ListModel* pList = pChannelManager->getCurrentChannelList();
            if(pCurrentItem == NULL) { qDebug() << __FUNCTION__ << " : Current Item is NULL!!.";  return; }

            DABChannelItem* pFindItem = (DABChannelItem*)pList->findItem(pCurrentItem->key(), QString("%1").arg(pItem->getServiceID()));
            if(pFindItem == NULL) {
                qDebug() << __FUNCTION__ << " : Find Item is NULL!!.";
                ListModel* pTempList = pChannelManager->getTempEpgChannelList();
                pFindItem = (DABChannelItem*)pTempList->findItem(pCurrentItem->key(), QString("%1").arg(pItem->getServiceID()));
                if(pFindItem == NULL){
                    qDebug() << __FUNCTION__ << " : Find pTempList Item is NULL!!. ";
                    return;
                }
            }

            pItem->setSupplementData(pFindItem);
            pEPGReservationList->addEPGReservation(pItem, true);
            emit cmdResponseAddReservation(true, hour, minute, second);
            setEPGReservationItem(pItem);
        }
        else
        {
            qDebug() << "Current Time is big than Reserve Time";
            emit cmdResponseAddReservation(false, hour, minute, second);
        }
    }
    else if(mjd > currentMJD)
    {
        DABChannelItem* pCurrentItem = pChannelManager->getCurrentChannelItem();
        ListModel* pList = pChannelManager->getCurrentChannelList();

        if(pCurrentItem == NULL) { qDebug() << __FUNCTION__ << " : Current Item is NULL!!.";  return; }
        DABChannelItem* pFindItem = (DABChannelItem*)pList->findItem(pCurrentItem->key(), QString("%1").arg(pItem->getServiceID()));
        if(pFindItem == NULL) {
            qDebug() << __FUNCTION__ << " : Find Item is NULL!!.";
            ListModel* pTempList = pChannelManager->getTempEpgChannelList();
            pFindItem = (DABChannelItem*)pTempList->findItem(pCurrentItem->key(), QString("%1").arg(pItem->getServiceID()));
            if(pFindItem == NULL){
                qDebug() << __FUNCTION__ << " : Find pTempList Item is NULL!!. ";
                return;
            }
        }

        pItem->setSupplementData(pFindItem);
        pEPGReservationList->addEPGReservation(pItem, true);
        emit cmdResponseAddReservation(true, hour, minute, second);
        setEPGReservationItem(pItem);
    }
}

void
DABController::onAddRemoveEPGReservation(QDateTime dateTime, QString serviceName, int serviceID, QString title, QString description, int hour, int minute, int second, int duration)
{
    qDebug() << Q_FUNC_INFO;

    DABEPGReservationItem* pItem = new DABEPGReservationItem(this);
    pItem->setTime(hour, minute, second, duration);
    pItem->setProgramTitle(title);
    pItem->setDescription(description);
    pItem->setServiceID(serviceID);
    pItem->setLabel(serviceName);

    QDate date = dateTime.date();
    quint32 mjd = DABLogger::instance()->convertDateToMJD(date);
    pItem->setMJD(mjd);

    QDate currentDate = QDate::currentDate();
    quint32 currentMJD = DABLogger::instance()->convertDateToMJD(currentDate);

    qDebug() << "ServiceID    : " << serviceID;
    qDebug() << "ProgramTitle : " << title;
    qDebug() << "Description  : " << description;
    qDebug() << "Hour         : " << hour;
    qDebug() << "Minute       : " << minute;
    qDebug() << "Second       : " << second;
    qDebug() << "Duration     : " << duration;
    qDebug() << "MJD          : " << mjd;

#if 1
    QDateTime broadcastingTime = DABLogger::instance()->getBroadcastingTime();
    QTime currentTime = broadcastingTime.time();
#else
    QTime currentTime =  QTime::currentTime();
#endif
    qDebug() << "Current Time : " << currentTime.toString();
    qDebug() << "Reserve Time : " << pItem->getStartTime().toString();
    qDebug() << "Reservation Count : " << pEPGReservationList->reservationCount;
    qDebug() << __FUNCTION__ << " EPG MJD = " << mjd << ", Current MJD = " << currentMJD;
    if(mjd == currentMJD)
    {
        if(currentTime < pItem->getStartTime())
        {
            qDebug() << "Reseve Time is big than Current Time";
            DABChannelItem* pCurrentItem = pChannelManager->getCurrentChannelItem();
            ListModel* pList = pChannelManager->getCurrentChannelList();
            if(pCurrentItem == NULL) { qDebug() << __FUNCTION__ << " : Current Item is NULL!!.";  return; }

            DABChannelItem* pFindItem = (DABChannelItem*)pList->findItem(pCurrentItem->key(), QString("%1").arg(pItem->getServiceID()));
            if(pFindItem == NULL) {
                qDebug() << __FUNCTION__ << " : Find Item is NULL!!.";
                ListModel* pTempList = pChannelManager->getTempEpgChannelList();
                pFindItem = (DABChannelItem*)pTempList->findItem(pCurrentItem->key(), QString("%1").arg(pItem->getServiceID()));
                if(pFindItem == NULL){
                    qDebug() << __FUNCTION__ << " : Find pTempList Item is NULL!!. ";
                    return;
                }
            }

            pItem->setSupplementData(pFindItem);
            pEPGReservationList->addRemoveEPGReservation(pItem);
            emit cmdResponseAddReservation(true, hour, minute, second);
            setEPGReservationItem(pItem);
        }
        else
        {
            qDebug() << "Current Time is big than Reserve Time";
            emit cmdResponseAddReservation(false, hour, minute, second);
        }
    }
    else if(mjd > currentMJD)
    {
        qDebug() << "Reseve Time is big than Current Time";
        DABChannelItem* pCurrentItem = pChannelManager->getCurrentChannelItem();

        ListModel* pList = pChannelManager->getCurrentChannelList();
        if(pCurrentItem == NULL) { qDebug() << __FUNCTION__ << " : Current Item is NULL!!.";  return; }

        DABChannelItem* pFindItem = (DABChannelItem*)pList->findItem(pCurrentItem->key(), QString("%1").arg(pItem->getServiceID()));
        if(pFindItem == NULL) {
            qDebug() << __FUNCTION__ << " : Find Item is NULL!!.";
            ListModel* pTempList = pChannelManager->getTempEpgChannelList();
            pFindItem = (DABChannelItem*)pTempList->findItem(pCurrentItem->key(), QString("%1").arg(pItem->getServiceID()));
            if(pFindItem == NULL){
                qDebug() << __FUNCTION__ << " : Find pTempList Item is NULL!!. ";
                return;
            }
        }

        pItem->setSupplementData(pFindItem);
        pEPGReservationList->addRemoveEPGReservation(pItem);
        emit cmdResponseAddReservation(true, hour, minute, second);
        setEPGReservationItem(pItem);
    }
}

DABEPGReservationItem*
DABController::getEPGReservationItem(void)
{
    qDebug() << Q_FUNC_INFO;

    return pEPGReservationItem;
}

void
DABController::setEPGReservationItem(DABEPGReservationItem* pItem)
{
    qDebug() << Q_FUNC_INFO;

    if(pItem == NULL){
        qDebug() << Q_FUNC_INFO << " Error pItem is NULL!!!!!";
        return;
    }
    *pEPGReservationItem = *pItem;
}

void
DABController::onCheckExistServiceID(QDateTime dateTime, int serviceID, int hour, int minute, int second)
{
    QDate date = dateTime.date();
    int ret = pEPGReservationList->isAlreadyExistReservationList(date, serviceID, hour, minute, second);
    if(ret == DAB_EPG_RESERVE_DUP_STARTTIME_SERVICE_ID)
    {
        qDebug() << "onCheckExistServiceID : alreadyPreserved called(serviceID same, StartTime same) !!";
        emit alreadyPreserved();
    }
}

void
DABController::onCancelEPGReservation(bool isSystemPopup)
{
    qDebug() << Q_FUNC_INFO << ", isSystemPopup = " << isSystemPopup;
    if(pEPGReservationList->removeItem())
    {
        emit cancelEPGReservation(isSystemPopup);
    }
    else
    {
        qDebug() << __FUNCTION__ << " EPG Reservation remove Fail !!";
    }
}

void
DABController::CmdAddStationLogo(QString filename)
{
    qDebug() << Q_FUNC_INFO << "filename : " << filename;
    QString logoFilename = filename;
    QString stationLogoRoot = DAB_STATION_LOGO_ROOT;
    logoFilename.remove(0,stationLogoRoot.length());
    int size = logoFilename.size();
    logoFilename.remove(size-4, size);

    DABChannelItem* pItem = pChannelManager->getCurrentChannelItem();
    QString serviceID = QString::number(pItem->serviceID(), 16);

    int result = QString::compare(serviceID, logoFilename);
    if(result == 0)
    {
        qDebug() << " Service ID is same !!";
        this->pUIListener->setChannelLogo(logoFilename);
    }
    else
    {
        qDebug() << " Service ID is different !!";
    }
}

void
DABController::onAudioInfo(quint8 ProtectType, quint8 ProtectLevel, quint8 ProtectOption)
{
    emit sendAudioInfo(ProtectType, ProtectLevel, ProtectOption);
}

#ifdef __ENG_MODE__
void
DABController::onReqVersion(int in0){
    Q_UNUSED(in0);
    qDebug() << Q_FUNC_INFO << APP_VERSION;
    QString moduleVer = this->pUIListener->getApplicationVer();
    emit cmdReqVersion(moduleVer);
}
#endif

void
DABController::onEventExceptionHandler(eDAB_EVENT_TYPE event)
{
    qDebug() << __FUNCTION__ << " :  event = " << event << " ServiceFollowingState = " << getServiceFollowingState() << ", m_iLinkingState = " << m_iLinkingState << ", m_bEnsembleSeek = " << this->pUIListener->getEnsembleSeek();
    switch(event)
    {
        case DAB_EVENT_NONE :
            break;

        case DAB_EVENT_FG :
            if(this->getServiceFollowingState() == DAB_SERVICE_FOLLOWING_STATE_AUDIO_DAB)
            {
                emit cmdStartDABtoFMTimer();
            }
            if(this->pUIListener->getSlideShowOn() == false)
            {
                this->pUIListener->setSLS("");
            }

            if(!m_bMuteCheck)
                StartDeactiveTATimer();

            this->pCommManager->ReqGetPinkStatus();
            break;

        case DAB_EVENT_BG :
            if(this->m_bSoundSettingFlag == true){
                this->m_bSoundSettingFlag = false;
                emit closeOptionMenu();
            }

            if(this->m_bVrStatus == true){
                this->m_bVrStatus = false;
                this->allScanStop();
                onReqSeekCancel();
            }

            if(this->pUIListener->getViewMode())
            {
                this->pUIListener->modeChange(false);
            }
            break;

        case DAB_EVENT_AV_MODE_NONE:
            if(getServiceFollowingState() == DAB_SERVICE_FOLLOWING_STATE_AUDIO_FM)
            {
                qDebug() << "::============= DAB_EVENT_AV_MODE_NONE :: Current Service is Service Following(DAB-FM). so Stop Service Folllowing !!";
                changeFMtoDAB();
            }
            this->allScanStop();
            onReqSeekCancel();
            break;

        case DAB_EVENT_AV_MODE_FG:
            StartDeactiveTATimer();
            if(getServiceFollowingState() != DAB_SERVICE_FOLLOWING_STATE_AUDIO_DAB)
            {
                changeFMtoDAB();
            }            
            this->pCommManager->ReqGetPinkStatus();
            break;

        case DAB_EVENT_UNMUTE:
            m_bMuteCheck = false;
            qDebug() << __FUNCTION__ << " :: announcementFlag() = " << this->pUIListener->getAnnouncementFlag() << ", this->m_eServiceStatus = " << this->m_eServiceStatus;
            if((this->pUIListener->getAnnouncementFlag() == ANN_TRAFFIC_ALARM_ON) && (this->m_eServiceStatus != DAB_SERVICE_ANNOUNCEMENT))
            {
                this->pCommManager->ReqGetAnnouncementStatus();
            }
            break;

        case DAB_EVENT_MUTE:
            if(isListStarted() || isPresetScanStarted() || this->pUIListener->getEnsembleSeek())
            {
                qDebug() << __FUNCTION__ << " :: DAB_EVENT_MUTE :: MuteCheck not setting while All Channel Scan or Preset Scan or Long Seek";
            }
            else
            {
                m_bMuteCheck = true;
            }
            break;

        case DAB_EVENT_AV_OFF :
            this->allScanStop();
            break;

        case DAB_EVENT_AV_ON :
            if(this->getServiceFollowingState() == DAB_SERVICE_FOLLOWING_STATE_AUDIO_FM)
            {
                onSetCurrentInfoToHomeMedia();
            }
            break;

        case DAB_EVENT_LONG_SEEK :
        case DAB_EVENT_SHORT_SEEK :
            this->allScanStop();
            StopWeakSignalTimer();
            StopDeactiveFMLinkingTimer();
            this->m_iLinkingState = 0;
            this->initPICodeValue();
            emit displayLinkingIcon(false, false, false);
            if(this->getServiceFollowingState() != DAB_SERVICE_FOLLOWING_STATE_AUDIO_DAB)
            {
                qDebug() << "current status is Linking Service. so stop the service following(DAB-FM)";
                DABChannelItem* pItem = pChannelManager->getCurrentChannelItem();
                this->pUIListener->setChangeFrequencyID(pItem->frequency());
                emit cmdReqRadioEnd(this->getRadioPlayBand(), this->getRadioPlayFreq());
            }

            this->pUIListener->setEnsembleSeek(true);
            emit cmdReqdisplayOSD("", "", DAB_OP_TYPE_SEARCHING);
            this->pUIListener->setSignalStatus(DAB_SIGNAL_NONE);
            this->pUIListener->setDABGoodCount(0);
            pUIListener->setFMSensitivity(0x00);
            this->m_eFMtoDABState = FM_DAB_STATE_FIRST;
            this->pUIListener->setFMtoDABInterval(DAB_TO_FM_FIRST_INTERVAL);
            this->m_bPICodeStatus = MICOM_STATUS_FM_NO_AVAILABLE;
            this->m_bPICodeSensitive = 0x00;
            this->m_bDeactiveDABtoFM = false;
            this->pUIListener->setFMFrequency(0x00);
            this->pUIListener->setPicode("0x00");
            emit cmdStopDABtoFMTimer();
            break;

        case DAB_EVENT_PRESET_SCAN :
            break;

        case DAB_EVENT_LIST_SCAN :
            break;

        case DAB_EVENT_SOUND_SETTING:
            this->m_bSoundSettingFlag = true;
            break;

        case DAB_EVENT_TUNE_UP :
        case DAB_EVENT_TUNE_DOWN :
            this->allScanStop();
            break;

        case DAB_EVENT_SELECT_SERVICE :
            this->m_iLinkingState = 0;
            this->pUIListener->setEnsembleSeek(false);
            StopWeakSignalTimer();
            StopDeactiveFMLinkingTimer();
            this->initPICodeValue();
            emit displayLinkingIcon(false, false, false);
            if(this->getServiceFollowingState() != DAB_SERVICE_FOLLOWING_STATE_AUDIO_DAB)
            {
                qDebug() << "current status is Linking Service. so stop the service following(DAB-FM)";
                emit cmdReqRadioEnd(this->getRadioPlayBand(), this->getRadioPlayFreq());
            }

            if(this->pUIListener->getListScanningOn() && isListStarted() == false)
            {
                this->pUIListener->setListScanningOn(false);
            }

            if(this->pUIListener->getPresetScanningOn() && isPresetScanStarted() == false)
            {
                this->pUIListener->setPresetScanningOn(false);
            }

            this->pUIListener->setSignalStatus(DAB_SIGNAL_NONE);
            this->pUIListener->setDABGoodCount(0);
            pUIListener->setFMSensitivity(0x00);
            this->m_eFMtoDABState = FM_DAB_STATE_FIRST;
            this->pUIListener->setFMtoDABInterval(DAB_TO_FM_FIRST_INTERVAL);
            this->m_bPICodeStatus = MICOM_STATUS_FM_NO_AVAILABLE;
            this->m_bPICodeSensitive = 0x00;
            this->m_bDeactiveDABtoFM = false;
            this->pUIListener->setFMFrequency(0x00);
            this->pUIListener->setPicode("0x00");
            emit cmdStopDABtoFMTimer();
            break;

        case DAB_EVENT_FM_CO_CHANNEL:
            emit cmdStopDABtoFMTimer();
            if(this->getServiceFollowingState() != DAB_SERVICE_FOLLOWING_STATE_AUDIO_DAB)
            {
                requestFMtoDAB();
                StartWeakSignalTimer();
            }
            pUIListener->setFMSensitivity(0x05);
            StartDeactiveFMLinkingTimer();
            break;

        case DAB_EVENT_FM_WEAK:
            emit cmdStopDABtoFMTimer();
            if(this->getServiceFollowingState() != DAB_SERVICE_FOLLOWING_STATE_AUDIO_DAB)
            {
                requestFMtoDAB();
                StartWeakSignalTimer();
            }
            pUIListener->setFMSensitivity(0x04);
            StartDeactiveFMLinkingTimer();
            break;                        

        case DAB_EVENT_DAB_FM_OFF:
            emit cmdStopDABtoFMTimer();
            if(this->getServiceFollowingState() != DAB_SERVICE_FOLLOWING_STATE_AUDIO_DAB)
            {
                requestFMtoDAB();
                StartWeakSignalTimer();
            }
            break;

        case DAB_EVENT_NOTIFY_AV_MODE:
            break;

        case DAB_EVENT_DISPLAY_OFF:        
            //onReqSeekCancel(); // ITS 219251
            break;

        case DAB_EVENT_ANNOUNCEMENT_STOP:
            this->m_bTAInterrupt = true;
            this->m_bDeactiveDABtoFM = false;
            break;

        case DAB_EVENT_BT_CALL_START:
            this->allScanStop();
            break;

        case DAB_EVENT_POWER_LOGIC_OFF:
            setServiceStatus(DAB_SERVICE_IDLE);
            onTuneTimeOut();            
            break;

        case DAB_EVENT_POWER_NORMAL_ON:
            checkEPGReservationList();
            if(this->getServiceFollowingState() == DAB_SERVICE_FOLLOWING_STATE_AUDIO_FM)
            {
                onSetCurrentInfoToHomeMedia();
            }
            break;

        case DAB_EVENT_NOTIFY_FROM_VR:
            if(getServiceFollowingState() != DAB_SERVICE_FOLLOWING_STATE_AUDIO_DAB)
            {
                changeFMtoDAB();
            }
            break;

        case DAB_EVENT_CLOSE_OPTION_MENU:
            emit closeOptionMenu();
            break;

        case DAB_EVENT_VR_START:
            this->m_bVrStatus = true;
            break;

        case DAB_EVENT_VR_END:
            this->m_bVrStatus = false;
            StartDeactiveTATimer();
            break;

        default :
            qDebug() << " onEventExceptionHandler : event =  " << event;
            break;
    }
}

void
DABController::onRspTPEGData(const QByteArray &data)
{
    emit cmdSendTPEGData(data, data.size());
}

void
DABController::setServiceStatus(DAB_SERVICE_STATUS status)
{
    qDebug() << __FUNCTION__ << " ========================== old status = " << this->m_eServiceStatus << ", new status = " << status;
    this->m_eServiceStatus = status;
}

void
DABController::setServiveFollowingState(eDAB_SERVICE_FOLLOWING_STATE_INFO state)
{
    qDebug() << __FUNCTION__ << " old serviceFollowing state = " << this->m_eServiceFollowingState << ", old serviceFollowing state = = " << state;
    this->m_eServiceFollowingState = state;
}

void
DABController::onReqSeekCancel(void)
{
    qDebug() << Q_FUNC_INFO << " :: bEnsembleSeek = " << this->pUIListener->getEnsembleSeek();
    if(this->pUIListener->getEnsembleSeek())
    {
        this->pUIListener->setEnsembleSeek(false);
        this->ReqSetSelectService();
    }
}

bool
DABController::checkDABtoFM(void)
{
    bool retValue;
    switch(this->getServiceStatus())
    {
        case DAB_SERVICE_IDLE:
        case DAB_SERVICE_TUNE_SEEK:
            retValue = true;
            break;

        default:
            retValue = false;
            break;
    }

    return retValue;
}

void
DABController::onReceiveFMSensitivity(const QByteArray &data)
{
    quint16 hpicode = quint16((data.at(0) & 0xff) << 8);
    quint16 picode = quint16((hpicode) | ((data.at(1))& 0xff));
    this->m_bPICodeStatus = data[2];
    this->m_bPICodeSensitive = data[3];
    quint16 hFreq = quint16((data.at(4) & 0xff) << 8);
    this->m_FMFrequency = quint16((hFreq) | ((data.at(5))& 0xff)) ;
    pUIListener->setFMFrequency(m_FMFrequency);

    this->m_bRequestDABtoFM = false;

    if(this->m_bPICodeStatus == MICOM_STATUS_FM_AVAILABLE)
    {
        if(this->m_bPICodeSensitive == 0x01)
            pUIListener->setFMSensitivity(0x02); // Good
        else
            pUIListener->setFMSensitivity(0x01);  // Bad
    }
    else if(this->m_bPICodeStatus == MICOM_STATUS_FM_AUDIO_ACTIVATE)
    {
        pUIListener->setFMSensitivity(0x03); // Audio Path is not DAB
        emit cmdStopDABtoFMTimer();
    }
    else if(this->m_bPICodeStatus == MICOM_STATUS_FM_WEAK)
    {
        qDebug() << __FUNCTION__ << "FM Sensivivity is Weak !!. so stop the service following(DAB-FM)";
        this->onEventExceptionHandler(DAB_EVENT_FM_WEAK);
    }
    else if(this->m_bPICodeStatus == MICOM_STATUS_FM_CO_CHANNEL)
    {
        qDebug() << __FUNCTION__ << "FM Co-Channel !!. so stop the service following(DAB-FM)";
        this->onEventExceptionHandler(DAB_EVENT_FM_CO_CHANNEL);
    }    
    else
    {
        pUIListener->setFMSensitivity(0x00); // FM not avaiable
    }

    qDebug() << "=======================================================================";
    qDebug() << "====== onReceiveFMSensitivity : PI Code            : 0x" << hex << picode;
    qDebug() << "====== onReceiveFMSensitivity : PI Code Status     : 0x" << this->m_bPICodeStatus;
    qDebug() << "====== onReceiveFMSensitivity : PI Code Sensitive  : 0x" << this->m_bPICodeSensitive;
    qDebug() << "====== onReceiveFMSensitivity : FM Frequency       : 0x" << this->m_FMFrequency;
    qDebug() << "=======================================================================";
}

void
DABController::sendPICodeToMicom(void)
{
    if(this->pUIListener->getServiceFollowing() == SERVICE_FOLLOWING_OFF)
    {
        qDebug() << __FUNCTION__ << "=================== : Service Following Setting value is Off. so PI Code is not send !!";
        emit cmdStopDABtoFMTimer();
        return;
    }

    if(this->m_bRequestDABtoFM)
    {
        qDebug() << __FUNCTION__ << "=================== : DABtoFM not response from RDS. so PI Code is not send !!";
        return;
    }

    if(isListStarted() || isPresetScanStarted())
    {
        qDebug() << __FUNCTION__ << "=================== :  PI Code is not send while prset scan or list scan";
        return;
    }


    DABChannelItem* pItem = pChannelManager->getCurrentChannelItem();

    quint16 picode = 0;

    if(m_PICodeOffset == -1)
    {
        m_PICodeValue[++m_PICodeOffset] = (quint16)pItem->serviceID();
        m_PICodeValueCount++;
        for(int i=0; i<pItem->picodeInfo().PICodeCount; i++)
        {
            if((pItem->picodeInfo().PICodeType[i] == 0x01) && (pItem->picodeInfo().PICodeValue[i] != m_PICodeValue[0])) // 0x00 : DAB,  0x01 : FM
            {
                m_PICodeValue[m_PICodeValueCount] = pItem->picodeInfo().PICodeValue[i];
                m_PICodeValueCount++;
            }
        }
        picode = m_PICodeValue[m_PICodeOffset];
    }
    else
    {
        if(this->m_bPICodeStatus == 0x01 && this->m_bPICodeSensitive == 0x01)
        {
            picode = m_PICodeValue[m_PICodeOffset];
        }
        else
        {
            m_PICodeOffset++;
            if(m_PICodeOffset >= m_PICodeValueCount)
                m_PICodeOffset = 0;

            picode = m_PICodeValue[m_PICodeOffset];
        }
    }

//    qDebug() << "=======================================================================";
//    qDebug() << "====== sendPICodeToMicom : PICode Count            : " << m_PICodeValueCount;
//    for(int k=0; k < m_PICodeValueCount; k++)
//    {
//        qDebug() << "====== sendPICodeToMicom : PICode Value[" << k << "]          : 0x" << hex <<  m_PICodeValue[k];
//    }
//    qDebug() << "====== sendPICodeToMicom : SEND PI Code            : 0x" << hex <<  picode;
//    qDebug() << "====== sendPICodeToMicom : Label          : " << pItem->label();
//    qDebug() << "=======================================================================";

    if(picode == 0x00)
    {
        qDebug() << "================= sendPICodeToMicom : Current Channel is not PICode. so ignored !!========================";
        return;
    }

    emit cmdLgeDBus_DABPICodeRequest(picode);
    QString imsi = QString::number(picode, 16);
    pUIListener->setPicode(imsi);
    this->m_bRequestDABtoFM = true;
    pUIListener->setReqCountDABtoMicom(0);
}

void
DABController::initPICodeValue(void)
{
    qDebug() << Q_FUNC_INFO;
    memset(this->m_PICodeValue, 0x0000, 12);
    this->m_PICodeOffset = -1;
    this->m_PICodeValueCount = 0;
}

void
DABController::onFactoryResetRemoveData(QString path)
{
    QDir directory;
    QString fileFullPath, dirFullPath;
    directory.setPath(path);

    QStringList list = directory.entryList();  
    foreach (const QString &str, list)
    {
        if(str.contains(".dat") || str.contains(".gif") ||  str.contains(".jpg") ||  str.contains(".bmp") || str.contains(".png") || str.contains(".db") || str.contains(".txt") || str.contains(".ini"))
        {
            fileFullPath = path + str;
            QFile file(fileFullPath);
            if(!(file.remove()))
            {
                qDebug() << Q_FUNC_INFO << "File remove Failed";
            }         
            file.close();
        }
        else if(!(str.contains('.') || str.contains("..")))
        {          
            dirFullPath = path + str +"/";
            onFactoryResetRemoveData(dirFullPath);
            QDir dir;
            if(!(dir.rmpath(dirFullPath)))
            {
                qDebug() << Q_FUNC_INFO << "Dir remove Failed";
            }         
        }
    }
}

void
DABController::checkTAService(void)
{
    qDebug() << Q_FUNC_INFO;
    DABChannelItem* pItem = pChannelManager->getCurrentChannelItem();
    quint16 ausFlags = pItem->asuInfo().ASUFlags;
    qDebug() << " ausFlags = 0x" << hex << ausFlags << ", flag = 0x" << hex << (ausFlags & 0x0002);
    if((ausFlags & 0x0002) > 0)
    {
        qDebug() << __FUNCTION__ << " TA Service exists in current channel!!";
    }
    else
    {
        ListModel* pList = pChannelManager->getCurrentChannelList();
        int index = pList->findIndex(0x0002);
        if(index == -1)
        {
            qDebug() << __FUNCTION__ << " TA Station is not exist !!";
            emit cmdReqTASearchPopup(false);
        }
        else
        {
            qDebug() << __FUNCTION__ << " TA Station is exist !!";
            emit cmdReqTASearchPopup(true);
        }
    }
}

void
DABController::onDeactivateTA(void)
{
    StopDeactiveTATimer();
    this->m_bDeactiveTA = false;
    quint16 flag = this->pUIListener->getAnnouncementFlag();
    qDebug() << __FUNCTION__ << " m_bTAInterrupt = " << m_bTAInterrupt << ", m_bDeactiveTA = " << m_bDeactiveTA << " Announcement Flag = 0x" << hex << flag << ", m_eServiceStatus = " << this->m_eServiceStatus;

    if(m_bTAInterrupt && (flag == ANN_TRAFFIC_ALARM_ON) && (this->m_eServiceStatus != DAB_SERVICE_ANNOUNCEMENT))
    {
        this->pCommManager->ReqGetAnnouncementStatus();
    }
}

void
DABController::onDeactivateFMLinking(void)
{
    qDebug() << Q_FUNC_INFO;
    StopDeactiveFMLinkingTimer();
    this->m_bDeactiveTA = false;
    this->m_eFMtoDABState = FM_DAB_STATE_FIRST;
    this->pUIListener->setDABGoodCount(0);
    this->pUIListener->setFMtoDABInterval(DAB_TO_FM_FIRST_INTERVAL);
    emit cmdStartDABtoFMTimer();
}

void
DABController::onOSDEPGStarted(void)
{
    qDebug() << __FUNCTION__ << " : EPG Reservation broadcasting is already started !! , m_sBackupEPGTitle = " << m_sBackupEPGTitle << ", m_sBackupEPGServiceName = " << m_sBackupEPGServiceName;
    emit cmdReqdisplayOSD(m_sBackupEPGTitle, m_sBackupEPGServiceName, DAB_OP_TYPE_EPG_STARTED);
}

void
DABController::sendServLinkCERValue(quint32 dab_worst, quint32 dab_bad, quint32 dab_nogood, quint32 dab_good, quint32 dabPlus_worst, quint32 dabPlus_bad, quint32 dabPlus_nogood, quint32 dabPlus_good)
{
    qDebug() << Q_FUNC_INFO << " : " << dab_worst << ", " << dab_bad << "," << dab_nogood << ", " << dab_good << ", " << dabPlus_worst << ", " << dabPlus_bad << ", " << dabPlus_nogood << ", " << dabPlus_good;
    this->pCommManager->ReqSetServLinkCERValue(dab_worst, dab_bad, dab_nogood, dab_good, dabPlus_worst, dabPlus_bad, dabPlus_nogood, dabPlus_good);
}

void
DABController::sendtSigStatusCERValue(quint8 count, quint32 dab_bad, quint32 dab_nogood, quint32 dab_good, quint32 dabPlus_bad, quint32 dabPlus_nogood, quint32 dabPlus_good)
{
    qDebug() << Q_FUNC_INFO << " : " << count << ", " << dab_bad << "," << dab_nogood << ", " << dab_good << ", " << dabPlus_bad << ", " << dabPlus_nogood << ", " << dabPlus_good;
    this->pCommManager->ReqSetSigStatusCERValue(count, dab_bad, dab_nogood, dab_good, dabPlus_bad, dabPlus_nogood, dabPlus_good);
}

void
DABController::sendPinknoiseCERValue(quint8 unmute_count, quint8 mute_count, quint32 dab_bad, quint32 dab_good, quint32 dabPlus_bad, quint32 dabPlus_good)
{
    qDebug() << Q_FUNC_INFO << " : " << unmute_count << ", " << mute_count << ", " << dab_bad << "," << dab_good << ", " << dabPlus_bad << ", " << dabPlus_good;
    this->pCommManager->ReqSetPinknoiseCERValue(unmute_count, mute_count, dab_bad, dab_good, dabPlus_bad, dabPlus_good);
}

void
DABController::onReqSetAntennaStatus(quint8 status)
{
    qDebug() << Q_FUNC_INFO << " : Antenna status = " << status;

    this->pCommManager->ReqSetAntennaStatus(status);
}

void
DABController::debugSetDLSTimeout(int time)
{
    qDebug() << Q_FUNC_INFO << ": time = " << time;
    int value = time * 60;
    this->m_iDLSTimeCount = 0;
    this->m_iDLSTimeCount = 0;
    this->m_iDLSTimeout = value;
    this->m_iDLPlustTimeout = value;
}

void
DABController::debugSetSLSTimeout(int time)
{
    qDebug() << Q_FUNC_INFO << ": time = " << time;
    int value = time * 60;
    this->m_iSLSTimeCount = 0;
    this->m_iSLSTimeout = value;
}

void
DABController::seekDownFromStationList(void)
{
    qDebug() << Q_FUNC_INFO;

    if( this->pUIListener->setNextChannelInfoForShortSeek() != true )
    {
        qDebug() <<  __FUNCTION__ << " Seek Down failed.. ";
        return;
    }

    this->ReqSetSelectService();
    this->pChannelManager->redrawStationList();
}

void
DABController::seekUpFromStationList(void)
{
    qDebug() << Q_FUNC_INFO;
    if( this->pUIListener->setPrevChannelInfoForShortSeek() != true )
    {
        qDebug() <<  __FUNCTION__ << " Seek Down failed.. ";
        return;
    }

    this->ReqSetSelectService();
    this->pChannelManager->redrawStationList();
}

#ifdef __DAB_DABUG_INFO__
void
DABController::debugRequestDABtoDAB(QString onOff)
{
    qDebug() << Q_FUNC_INFO << ": onOff = " << onOff;

    if(onOff == "on")
    {
        this->pCommManager->RegSetServiceFollowing(DAB_SERVICE_FOLLOWING_ON);
    }
    else
    {
        this->pCommManager->RegSetServiceFollowing(DAB_SERVICE_FOLLOWING_OFF);
    }
}

#endif

