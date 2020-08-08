/**
 * FileName: DABUIListener.cpp
 * Author: David.Bae
 * Time: 2011-08-23 15:18
 * Description:
 *          This file is used for communication with GUI QML file with Qt file.
 */

#include <QDebug>
#include <QFile>
#include <qtextcodec.h>
#include <QTime>
#include <QDateTime>
#include <QList>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QDir>
#include <QStringList>
#include <QApplication>

#include "DABUIListener.h"
#include "DABController.h"
#include "DABDataStorage.h"
#include "DABEPGItem.h"
#include "DABApplication_Def.h"

DABUIListener::DABUIListener(DABController &aDABController, QObject *parent) :
    QObject(parent),
#ifndef __UNIX_DESKTOP__
    m_VRAdaptor(parent),
#endif
    m_DABController(aDABController)
{
    this->m_pChannelManager = new DABChannelManager(*this);
    this->InitializeData(parent);
    loadData();
    removeTempFiles();
    connect(&m_EPGManager,      SIGNAL(cmdAddEPGItem(quint32,quint32,int,int,int,int,QString,QString)),     this,           SLOT(onAddEPGItem(quint32,quint32,int,int,int,int,QString,QString)));
    connect(&m_EPGManager,      SIGNAL(cmdSortingEPGData(quint32)),                                         this,           SLOT(onSortingCurrentChannelEPGData(quint32)));
    connect(m_pChannelManager,  SIGNAL(cmdWritePresetList()),                                               &m_VRAdaptor,   SLOT(writePresetList()));
    m_VRAdaptor.writePresetList();
}

DABChannelManager*
DABUIListener::getChannelManager(void)
{
    return this->m_pChannelManager;
}

void
DABUIListener::initEPG(void)
{
    DABLogger::instance()->Log((QString("  ======DABUIListener(QObject *parent) : initEPG called")));
    m_EPGManager.init();
}

void
DABUIListener::InitializeData(QObject *parent)
{
    this->m_bSlideShowOn = true;
    this->m_iAnnouncementFlag = ANN_TRAFFIC_ALARM_ON;
    this->m_iServiceFollowing = SERVICE_FOLLOWING_ON;
    this->m_iBandSelection = BAND_SELECTION_BANDIII;
    this->m_sDLS = "";
    this->m_sSLS = "";
    this->m_sTitle = "";
    this->m_sArtist = "";
    this->m_bEPG = false;
    this->m_bViewMode = false;
    this->m_bListScanningOn = false;
    this->m_bPresetScanningOn = false;
    this->m_iCurrentPresetIndex = -1;
    this->m_iPresetIndex = -1;
    this->m_iPS = 0x01;     // 0x00 : secondary service, 0x01 :primary service
    this->m_sControllerStatus ="";
    this->m_sModuleStatus ="";
    this->m_sServiceStatus ="";
    this->m_iCER = 0;
    this->m_iSNR = 0;
    this->m_iRSSI = 0;
    this->m_iCER_sub = 0;
    this->m_iSNR_sub = 0;
    this->m_iRSSI_sub = 0;
    this->m_serviceID = 0;
    this->m_iBootCount = 0;
    this->m_iFirstEnsembleCount = 0;
    this->m_iSecondEnsembleCount = 0;
    this->m_iThirdEnsembleCount = 0;
    this->m_iPreserveCount = 0;
    this->m_sPtyName = 0;
    this->m_iDABGoodCount = 0;
    this->m_iReqCountDABtoMicom = 0;
    this->m_iFMSensitivity = 0x00;
    this->m_iFMFequency = 0x00;
    this->m_sServiceEvent = "";
    this->m_sChangeFrequencyID = 0;
    this->m_sFrequencyID = 0;
    this->m_iClearCount = 0;
    this->m_iLanguageID = 0;
    this->m_bEnsembleSeek = false;
    this->m_iAnnounceDeleayTime = ANNOUNCEMENT_DELAY_TIME;
    this->m_iAnnounceTimeout = ANNOUNCEMENT_TIMEOUT;
    this->m_iServLinkMuteTimeout = SERVCE_LINK_MUTE_TIMEOUT;
    this->m_iFMtoDABInterval = DAB_TO_FM_FIRST_INTERVAL;
    this->m_ViewChannel = new DABChannelItem(parent);
    this->m_StationList = new ListModel(new DABChannelItem(parent), parent);
    this->m_StationListPty = new ListModel(new DABChannelItem(parent), parent);
    this->m_StationListAlphabet = new ListModel(new DABChannelItem(parent), parent);
}

DABUIListener::~DABUIListener()
{
    delete this->m_ViewChannel;
    this->m_EPGManager.clear();
}

void
DABUIListener::PrepareCurrentChannelInfo(void)
{
    qDebug() << Q_FUNC_INFO;
    m_pChannelManager->setChannelCount(m_pChannelManager->getCurrentChannelCount());
    setChannelInfo(m_pChannelManager->getCurrentChannelNumber());
    setEnsembleName(m_pChannelManager->getCurrentEnsembleName());
    setFrequencyID(m_pChannelManager->getCurrentFrequencyID());
    setServiceName(m_pChannelManager->getCurrentServiceName());
    setPtyName (m_pChannelManager->getCurrentPtyName());
    setCurrentPresetIndex(m_pChannelManager->getCurrentChannelItem());
    setPresetIndex(this->m_iCurrentPresetIndex);
    setPS(m_pChannelManager->getSerivceType());
    this->initDLS(true);
}

void
DABUIListener::setCurrentChannelInfo(DABChannelItem* pItem, bool viewMode)
{
    qDebug() << __FUNCTION__ << " ================= View Mode : " << viewMode;

    if(viewMode)
    {
        setChannelInfo(pItem->frequencyLabel());
        setServiceName(pItem->label());
        setEnsembleName(pItem->ensembleLabel());
        setPtyName (pItem->programeType());
        setPresetIndex(pItem->presetIndex());        
        QString serviceID = QString::number(pItem->serviceID(), 16);
        setChannelLogo(serviceID);
//        setEPG(false);
    }
    else
    {
        setBitrate(pItem->bitrate());
        setChannelInfo(pItem->frequencyLabel());
        setServiceName(pItem->label());
        setEnsembleName(pItem->ensembleLabel());
        setFrequencyID(pItem->frequency());
        setPS(pItem->ps());
        setPtyName (pItem->programeType());
        setCurrentPresetIndex(pItem);
        setPresetIndex(this->m_iCurrentPresetIndex);
        QString serviceID = QString::number(pItem->serviceID(), 16);
        setChannelLogo(serviceID);
        emit playIconUpdateInPresetList((m_iPresetIndex-1));
        ListModel* currentList = m_pChannelManager->getCurrentChannelList();
        int index = currentList->findIndex(pItem->key(), pItem->subKey(), pItem->id());
        if(index == -1)
            setEPG(false);
        else
            setEPG(m_EPGManager.getCurrentDateEPGExists(pItem->ensembleLabel(), pItem->label()));
    }
}

void
DABUIListener::setPresetIndex(int index)
{
    qDebug() << Q_FUNC_INFO << "DABUIListener::setPresetIndex called: index = " << index;
    m_iPresetIndex = index+1;
    emit presetIndexChanged();
}

void
DABUIListener::setChannelLogo(QString eName)
{
    QString filename = DAB_STATION_LOGO_ROOT + eName + ".png";

    if(QFile::exists(filename))
    {
        qDebug() << __FUNCTION__ << " Channel Logo is exisits !!";

        QImage image(filename);
        int width = image.width();
        int height = image.height();

        if(width <= 100 && height <= 100)
            m_sChannelLogo = "";
        else
            m_sChannelLogo = filename;

        emit channelLogoChanged(width, height);
    }
    else
    {
        qDebug() << __FUNCTION__ << " Channel Logo is not exisits !!";
        m_sChannelLogo = "";
        emit channelLogoChanged(0,0);
    }
}

void
DABUIListener::setChangeFrequencyID(int frequency)
{
    qDebug() << Q_FUNC_INFO << " frequency = " << frequency;
    m_sChangeFrequencyID = frequency;
    emit changeFrequencyIDChanged();
}

void
DABUIListener::setPtyName(quint8 pty)
{
    m_sPtyName = pty;
    emit ptyNameChanged();
}

void
DABUIListener::initDLS(bool init)
{
    qDebug() << Q_FUNC_INFO << " ::  init = " << init;
    QString str = "";
    this->m_sDLS = "";
    emit dlsChanged();
    if(init)
    {
        str = "...";
    }
//    else
//    {
//        str = QApplication::translate("StringInfo", "STR_DAB_PLAYER_NO_INFO");
//    }
    this->m_sTitle = str;
    emit titleChanged();
    this->m_sArtist =str;
    emit artistChanged();
}

void
DABUIListener::initDLS(void)
{
    qDebug() << Q_FUNC_INFO;
    this->m_sDLS = "";
    emit dlsChanged();
}

void
DABUIListener::initDLPlus(void)
{
    this->m_sTitle = "";//QApplication::translate("StringInfo", "STR_DAB_PLAYER_NO_INFO");
    emit titleChanged();
    this->m_sArtist ="";//QApplication::translate("StringInfo", "STR_DAB_PLAYER_NO_INFO");
    emit artistChanged();
}

void DABUIListener::setSLS(QString sls)
{
    if(this->m_sSLS == sls)
    {
        return;
    }

    QString tempSLS = this->m_sSLS;
    this->m_sSLS = sls;
    emit slsChanged();
   this->removeSLSFile(tempSLS);
}

void
DABUIListener::removeDLSSLSData(void)
{
    this->initDLS(false);
    this->setSLS("");
}

void
DABUIListener::onAddEPGItem(quint32 mjd, quint32 serviceID, int Hour, int Minute, int Second, int Duration, QString ProgramLabel, QString Description)
{
    ListModel* currentList = m_pChannelManager->getCurrentChannelList();
    quint32 freq = (quint32)this->getChangeFrequencyID();
    int index = currentList->findIndex(freq, (QString("%1").arg(serviceID)));

    DABChannelItem* pChannelItem = (DABChannelItem*)m_pChannelManager->getChannelItemFromChannelList(index);
    if(pChannelItem == NULL)
    {
        DABLogger::instance()->Log((QString("onAddEPGItem() :: ERROR Current Channel Item is NULL. Skip EPG Item. serviceID = %1").arg(serviceID)));
        return;
    }

    DABEPGItem* pEPGItem = new DABEPGItem(this);
    if(pEPGItem == NULL)
    {
        DABLogger::instance()->Log((QString("onAddEPGItem() :: ERROR DABEPGItem allocation fail")));
        return;
    }

    pEPGItem->setTime(Hour, Minute, Second, Duration);
    if(ProgramLabel == EPG_NO_LABEL)
        ProgramLabel = EPG_NO_LABEL;
    pEPGItem->setProgramTitle(ProgramLabel);
    if(Description == EPG_NO_DESCRIPTOR)
        Description = EPG_NO_DESCRIPTOR;
    pEPGItem->setDescription(Description);
    pEPGItem->setServiceID(serviceID);
    pEPGItem->setFrequencyID(pChannelItem->frequency());
    m_EPGManager.addEPGItem(mjd, pChannelItem->ensembleLabel(), pChannelItem->label(),pEPGItem);
}

void
DABUIListener::showEPG(void) //For Debugging..
{
    this->m_EPGManager.showEPG();
}

bool
DABUIListener::onSortingEPGData(quint32 mjd, quint32 sID)
{   
    QList<DABEPGItem*>::iterator i;
    DABEPGItem* pEpgItem = NULL;
    ListModel* currentList = m_pChannelManager->getCurrentChannelList();

    quint32 freq = (quint32)this->getChangeFrequencyID();
    int index = currentList->findIndex(freq, (QString("%1").arg(sID)));

    if(index < 0)
    {
        qDebug() << __FUNCTION__ << " ==> ERROR index is -1. Skip this function, serivceID = " << sID;
        return false;
    }

    DABChannelItem* pItem = (DABChannelItem*)m_pChannelManager->getChannelItemFromChannelList(index);
    if(pItem == NULL)
    {
        qDebug() << __FUNCTION__ << " ==> ERROR Channel Item is NULL. Skip this function";
        return false;
    }

    QList<DABEPGItem*>* pEPGListForService = NULL;
    pEPGListForService = this->m_EPGManager.getEPGServiceList(mjd, pItem->ensembleLabel(), pItem->label());

    if((pEPGListForService == NULL) || (pEPGListForService->count() == 0))
    {
        qDebug() << __FUNCTION__ << " ==> ERROR EPG List is Null or EPG list count is 0";
        return false;
    }

    this->m_EPGManager.saveEPGData(pEPGListForService, mjd, pItem->frequencyLabel(), pItem->ensembleID(), pItem->serviceID());

    DABChannelItem* pChannelItem = m_pChannelManager->getCurrentChannelItem();
    if(pChannelItem == NULL)
    {
        qDebug() << __FUNCTION__ << " ==> ERROR Current Channel Item is NULL. Skip this function";
        return false;
    }

    qDebug() << __FUNCTION__ << " EPG ServiceID = " << sID << ", Current Channel Service ID = " << pChannelItem->serviceID();

    if(freq == pChannelItem->frequency() && sID == pChannelItem->serviceID())
    {
        QDate receiveDate = DABLogger::instance()->convertMJDToDate(mjd);
        QDate currentDate = QDate::currentDate();
        quint32 cMjd = DABLogger::instance()->convertDateToMJD(currentDate);
        QTime cTime = QTime::currentTime();
        qDebug() << __FUNCTION__ << " : Current Date : " << currentDate.toString() << ", receiveDate : " << receiveDate.toString();
        if(receiveDate == currentDate)
        {
#if 1
            bool bExist = false;
            for(i = pEPGListForService->begin(); i != pEPGListForService->end(); i++)
            {
                pEpgItem = i[0];
                QTime sTime(pEpgItem->hour(), pEpgItem->minute(), pEpgItem->second());
                QTime eTime = sTime.addSecs(pEpgItem->duration());
                QTime tTime(0,0,0);
                QTime endTime = pEpgItem->getEndTime();
                if(tTime == endTime)
                {
                    endTime.setHMS(23,59,59);
                }

                qDebug() << "currentTime:" << cTime.toString() << " StartTime:" << pEpgItem->getStartTime().toString() << ", EndTime:" << eTime.toString();
                if((!(endTime >= cTime || pEpgItem->getStartTime() >= cTime)) && (mjd == cMjd))
                {
                    continue;
                }
                bExist = true;
            }
            setEPG(bExist);
#else
            setEPG(true);
#endif
        }
    }
    return true;
}

void
DABUIListener::onSortingCurrentChannelEPGData(quint32 mjd)
{
    DABChannelItem* pChannelItem = m_pChannelManager->getCurrentChannelItem();
    if(pChannelItem == NULL)
    {
        DABLogger::instance()->Log((QString("  onSortingCurrentChannelEPGData() :: ERROR Current Channel Item is NULL. Skip this function")));
        return;
    }

    QList<DABEPGItem*>* pEPGListForService = NULL;

    DABLogger::instance()->Log((QString("onSortingCurrentChannelEPGData() :: mjd = %1 ensembleLabel = %2, serviceLabel = %3").arg(mjd).arg(pChannelItem->ensembleLabel()).arg(pChannelItem->label())));
    pEPGListForService = this->m_EPGManager.getEPGServiceList(mjd, pChannelItem->ensembleLabel(), pChannelItem->label());

    if((pEPGListForService == NULL) || (pEPGListForService->count() == 0))
    {
        DABLogger::instance()->Log((QString("  onSortingCurrentChannelEPGData() :: pEPGListForService == NULL, setEPG(false) called")));
        setEPG(false);
    }
    else
    {
        QDate epgDate = DABLogger::instance()->convertMJDToDate(mjd);
        QDate currentDate = QDate::currentDate();
        DABLogger::instance()->Log((QString(" onSortingCurrentChannelEPGData() :: current date = %1, receiveDate = %2").arg(currentDate.toString()).arg(epgDate.toString())));
        if(epgDate == currentDate)
        {
            DABLogger::instance()->Log((QString(" onSortingCurrentChannelEPGData() :: setEPG(true) called!!")));
            setEPG(true);
        }
        else
        {
            DABLogger::instance()->Log((QString(" onSortingCurrentChannelEPGData() :: setEPG(false) called!!")));
            setEPG(false);
        }
    }
}

void
DABUIListener::removeCurrentEPG(quint32 sID, quint32 mjd)
{
    ListModel* currentList = m_pChannelManager->getCurrentChannelList();
    quint32 freq = (quint32)this->getChangeFrequencyID();
    int index = currentList->findIndex(freq, (QString("%1").arg(sID)));
    if(index < 0)
    {
        qDebug() << __FUNCTION__ << " ==> ERROR index is -1. Skip this function";
        return;
    }

    DABChannelItem* pChannelItem = (DABChannelItem*)m_pChannelManager->getChannelItemFromChannelList(index);
    if(pChannelItem == NULL)
    {
        qDebug() << __FUNCTION__ << " ==> ERROR Current Channel Item is NULL. Skip this function";
        return;
    }

    this->m_EPGManager.clear(mjd, pChannelItem->ensembleLabel(), pChannelItem->label());
}

void
DABUIListener::loadData(void)
{
    m_pChannelManager->loadChannelData(DAB_MODE1_DATA_FILE);
    loadSettingFromFile();
    PrepareCurrentChannelInfo();
    this->setViewChannelInfoFromItem(m_pChannelManager->getCurrentChannelItem());
    initVRAdaptor();
}

void
DABUIListener::saveData(void)
{
    m_pChannelManager->saveChannelData(DAB_MODE1_DATA_FILE);
    saveSettingToFile();
}

void
DABUIListener::initVRAdaptor(void)
{
#ifdef __VR_COMMAND__
    m_VRAdaptor.init(m_pChannelManager->getCurrentPresetList());
#endif
}

void
DABUIListener::saveSettingInfo(void)
{
    qDebug() << Q_FUNC_INFO;
    saveSettingToFile();
}

void
DABUIListener::saveChannelInfo(void)
{
    m_pChannelManager->saveChannelData(DAB_MODE1_DATA_FILE);
}

void
DABUIListener::loadSettingFromFile(void)
{
    DABLogger::instance()->Log((QString(" =====================SETTING FILE LOAD START=========================")));

    QSettings settings(DAB_SETTING_FILE, QSettings::IniFormat);

    if (settings.childGroups().contains("AppDAB_SettingInformation",Qt::CaseInsensitive))
    {
        DABLogger::instance()->Log((QString(" loadSettingFromFile() :: Key Exists!!")));

        settings.beginGroup("AppDAB_SettingInformation");
        this->m_bSlideShowOn = settings.value("SlideShow").toBool();
        this->m_iServiceFollowing = settings.value("ServiceFollowing").toInt();
        this->m_iBandSelection = settings.value("Band").toInt();
        settings.endGroup();
    }
    else
    {
        DABLogger::instance()->Log((QString(" loadSettingFromFile() :: Key doesn't exist!!")));

        settings.clear();
        settings.beginGroup("AppDAB_SettingInformation");
        settings.setValue("SlideShow", this->m_bSlideShowOn);
        settings.setValue("ServiceFollowing", this->m_iServiceFollowing);
        settings.setValue("Band", this->m_iBandSelection);
        settings.endGroup();
        settings.sync();
        DABLogger::instance()->sendProcessCommand("sync");
    }
}

void
DABUIListener::saveSettingToFile(void)
{
    QSettings settings(DAB_SETTING_FILE, QSettings::IniFormat);
    settings.clear();
    settings.beginGroup("AppDAB_SettingInformation");
    settings.setValue("SlideShow", this->m_bSlideShowOn);
    settings.setValue("ServiceFollowing", this->m_iServiceFollowing);
    settings.setValue("Band", this->m_iBandSelection);
    settings.endGroup();
    settings.sync();
    DABLogger::instance()->sendProcessCommand("sync");
}

bool
DABUIListener::setCurrentChannelInfoFromChannelList(int index)
{
    DABChannelItem* pItem = m_pChannelManager->getChannelItemFromChannelList(index);
    if(pItem == NULL)
    {
        qDebug() << __FUNCTION__ << " Channel Item is NULL !!!. so return ";
        return false;
    }

//    this->m_iCurrentPresetIndex = index;
//    qDebug() << "=======setCurrentChannelInfoFromChannelList==============================================================================================";
//    qDebug() << "=========setCurrentChannelInfoFromChannelList==================== pItem->picodeInfo().PICodeCount = " << pItem->picodeInfo().PICodeCount;
//    for(int i=0; i<pItem->picodeInfo().PICodeCount; i++)
//    {
//        qDebug() << "========setCurrentChannelInfoFromChannelList================= picodeType = " << pItem->picodeInfo().PICodeType[i];
//        qDebug() << "========setCurrentChannelInfoFromChannelList================= PICodeValue = " << pItem->picodeInfo().PICodeValue[i];
//    }
//    qDebug() << "=============setCurrentChannelInfoFromChannelList========================================================================================";
    this->setCurrentChannelInfoFromItem(pItem);
    return true;
}

bool
DABUIListener::setViewChanneltoCurrentChannel(void)
{
    DABChannelItem* pItem = this->getViewCurrentItem();
    if(pItem == NULL)
    {
        qDebug() << __FUNCTION__ << " view current item is NULL. so return !!";
        return false;
    }

    DABChannelItem* pCurrentItem = m_pChannelManager->getCurrentChannelItem();
    if(pCurrentItem == NULL)
    {
        qDebug() << __FUNCTION__ << " Error pItem is NULL!!!!!";
        return false;
    }

    if((pCurrentItem->frequency() == pItem->frequency()) && (pCurrentItem->serviceID() == pItem->serviceID()) && (pCurrentItem->subChId() == pItem->subChId()))
    {
        qDebug() << __FUNCTION__ << " ::  Select channel and current channel is same !! so not select service ";
#if 1
        if(m_DABController.isPresetScanStarted() || m_DABController.isListStarted())
        {
            emit cmdReqdisplayOSD(pCurrentItem->label(), pCurrentItem->ensembleLabel(), DAB_OP_TYPE_SCAN);
        }
        else
        {
            m_DABController.onSetCurrentInfoToHomeMedia();
        }
#else
        m_DABController.onSetCurrentInfoToHomeMedia();
#endif
        return false;
    }
    else
    {
        this->m_iCurrentPresetIndex = pItem->presetIndex();
        this->setCurrentChannelInfoFromItem(pItem);
        return true;
    }
}

bool
DABUIListener::setCurrentChannelInfoFromStationlList(int index, QString type)
{
    qDebug() << Q_FUNC_INFO;

    ListModel* pStationList = NULL;

    if(type == "ensemble")
    {
        pStationList = this->getStationList();
    }
    else if(type == "alphabet")
    {
        pStationList = this->getStationListAlphabet();
    }
    else if(type == "pty")
    {
        pStationList = this->getStationListPTY();
    }
    else
    {
        pStationList = this->getStationList();
    }

    int count = pStationList->rowCount();

    if(count == 0)
    {
        qDebug() << __FUNCTION__ << " ::  Channel List is 0. Empty..";
        return false;
    }

    if(index < 0 || index >= count)
    {
        qDebug() << __FUNCTION__ << " ::  index is out of Bound. index:"<<index<<", list Count:"<< count;
        return false;
    }

    DABChannelItem* pItem = (DABChannelItem*)pStationList->dataItem(index);
    if(pItem == NULL)
    {
        qDebug() << __FUNCTION__ << " ::  channel item is NULL. index = " << index;
        return false;
    }

    DABChannelItem* pCurrentItem = m_pChannelManager->getCurrentChannelItem();
    if(pCurrentItem == NULL)
    {
        qDebug() << __FUNCTION__ << " Error pItem is NULL!!!!!";
        return false;
    }

    if((pCurrentItem->frequency() == pItem->frequency()) && (pCurrentItem->serviceID() == pItem->serviceID()) && (pCurrentItem->subChId() == pItem->subChId()))
    {
        qDebug() << __FUNCTION__ << " ::  Select channel and current channel is same !! so not select service ";
        return false;
    }
    else
    {
        this->m_iCurrentPresetIndex = index;
        this->setCurrentChannelInfoFromItem(pItem);
        return true;
    }
}

bool
DABUIListener::setCurrentChannelInfoFromServiceID(quint32 serviceID)
{
    ListModel* pList = m_pChannelManager->getCurrentChannelList();

    if(pList == NULL)
    {
        qDebug() << __FUNCTION__ << ": CurrentChannel List is NULL";
        return false;
    }

    QString id = QString("%1").arg(serviceID);
    quint32 freq = (quint32)this->getChangeFrequencyID();
    int index = pList->findIndex(freq, id);

    if( index == -1)
    {
        qDebug() <<"current item is not exist in channel list";
        return false;
    }

    DABChannelItem* pItem = m_pChannelManager->getChannelItemFromChannelList(index);
    if(pItem == NULL)
    {
        qDebug() << __FUNCTION__ << ": DABChannelItem is NULL";
        return false;
    }
    this->m_iCurrentPresetIndex = index;
    DABChannelItem* pCurrentItem = m_pChannelManager->getCurrentChannelItem();
    if((pCurrentItem->frequency() == pItem->frequency()) && (pCurrentItem->serviceID() == pItem->serviceID()) && (pCurrentItem->subChId() == pItem->subChId()))
    {
        qDebug() << __FUNCTION__ << " ::  Select channel and current channel is same !! so not select service ";
        return false;
    }
    this->setCurrentChannelInfoFromItem(pItem);   
    return true;
}

bool
DABUIListener::setCurrentChannelInfoFromPresetList(int index)
{
    qDebug() << Q_FUNC_INFO << ", index = " << index;
    DABChannelItem* pItem = m_pChannelManager->getChannelItemFromPresetList(index);
    if(pItem == NULL)
    {
        qDebug() << Q_FUNC_INFO << " Error pItem is NULL!!!!!";
        return false;
    }

    this->setCurrentChannelInfoFromItem(pItem);
    return true;
}

void
DABUIListener::setCurrentChannelInfoFromItem(DABChannelItem* pItem)
{
    if(pItem == NULL){
        qDebug() << Q_FUNC_INFO << " Error pItem is NULL!!!!!";
        return;
    }
    *(m_pChannelManager->getCurrentChannelItem()) = *pItem;
}

void
DABUIListener::setViewChannelInfoFromItem(DABChannelItem* pItem)
{
    if(pItem == NULL){
        qDebug() << Q_FUNC_INFO << " Error pItem is NULL!!!!!";
        return;
    }
    *(this->m_ViewChannel) = *pItem;
}

bool
DABUIListener::setNextChannelInfoForTuneUpDown(void)
{
    qDebug() << Q_FUNC_INFO;

    DABChannelItem* pItem = this->getViewCurrentItem();

    if(pItem == NULL)
    {
        qDebug() << __FUNCTION__ << ": pItem == NULL. return;";
        return false;
    }

    ListModel* pChannelList = m_pChannelManager->getCurrentChannelList();
    if(pChannelList == NULL)
    {
        qDebug() << __FUNCTION__ << ": CurrentChannel List is NULL";
        return false;
    }
    if(m_pChannelManager->getCurrentChannelCount() == 0)
    {
        qDebug() << __FUNCTION__ << ": Current Channel Count is 0. return;";
        return false;
    }

    int index = m_pChannelManager->findNextChannelIndexInChannelList(pItem);

    // Next item is same Ensemble id. Move to Next Channel;
    qDebug() << "  ==> Same.. Move to next index = " << index;
    DABChannelItem* channelItem = m_pChannelManager->getChannelItemFromChannelList(index);
    if(channelItem == NULL)
    {
        qDebug() << __FUNCTION__ << ": Channel Item is NULL";
        return false;
    }

    this->setViewChannelInfoFromItem(channelItem);
    return true;
}

bool
DABUIListener::setNextChannelInfoForShortSeek(void)
{
    qDebug() << Q_FUNC_INFO;

    DABChannelItem* pItem = m_pChannelManager->getCurrentChannelItem();

    if(pItem == NULL)
    {
        qDebug() << __FUNCTION__ << ": pItem == NULL. return;";
        return false;
    }

    ListModel* pChannelList = m_pChannelManager->getCurrentChannelList();
    if(pChannelList == NULL)
    {
        qDebug() << __FUNCTION__ << ": CurrentChannel List is NULL";
        return false;
    }

    if(m_pChannelManager->getCurrentChannelCount() == 0)
    {
        qDebug() << __FUNCTION__ << ": Current Channel Count is 0. return;";
        return false;
    }

    int index = m_pChannelManager->findNextChannelIndexInChannelList(pItem);
    if(index == -1)
    {
        qDebug() << __FUNCTION__ << " find prev channel index is -1 !!!";
        return false;
    }
    qDebug() << "  ==> Same.. Move to next index = " << index;
    this->setCurrentChannelInfoFromChannelList(index);
    return true;
}

bool
DABUIListener::setPrevChannelInfoForShortSeek(void)
{
    qDebug() << Q_FUNC_INFO;

    DABChannelItem* pItem = m_pChannelManager->getCurrentChannelItem();
    if(pItem == NULL)
    {
        qDebug() << __FUNCTION__ << ": pItem == NULL. return;";
        return false;
    }

    ListModel* pChannelList = m_pChannelManager->getCurrentChannelList();
    if(pChannelList == NULL)
    {
        qDebug() << __FUNCTION__ << ": CurrentChannel List is NULL";
        return false;
    }

    if(m_pChannelManager->getCurrentChannelCount() == 0)
    {
        qDebug() << __FUNCTION__ << " current channel count is zero !!. so return false";
        return false;
    }

    int index = m_pChannelManager->findPrevChannelIndexInChannelList(pItem);
    if(index == -1)
    {
        qDebug() << __FUNCTION__ << " find prev channel index is -1 !!!";
        return false;
    }
    this->setCurrentChannelInfoFromChannelList(index);
    return true;
}

bool
DABUIListener::setPrevChannelInfoForTuneUpDown(void)
{
    DABChannelItem* pItem = this->getViewCurrentItem();
    if(pItem == NULL)
    {
        qDebug() << __FUNCTION__ << ": View CurrntItem is NULL";
        return false;
    }

    ListModel* pChannelList = m_pChannelManager->getCurrentChannelList();
    if(pChannelList == NULL)
    {
        qDebug() << __FUNCTION__ << ": CurrentChannel List is NULL";
        return false;
    }

    if(m_pChannelManager->getCurrentChannelCount() == 0)
    {
        qDebug() << __FUNCTION__ << " current channel count is zero !!. so return false";
        return false;
    }

    int index = m_pChannelManager->findPrevChannelIndexInChannelList(pItem);
    if(index == -1)
    {
        qDebug() << __FUNCTION__ << " find prev channel index is -1 !!!";
        return false;
    }

    DABChannelItem* channelItem = m_pChannelManager->getChannelItemFromChannelList(index);
    if(channelItem == NULL)
        return false;

    this->setViewChannelInfoFromItem(channelItem);
    return true;
}

void
DABUIListener::setCurrentPresetIndex(DABChannelItem* pItem)
{
    qDebug() << Q_FUNC_INFO << " :: index = " << pItem->presetIndex();
    int index = 0;
    if(pItem->presetIndex() == -1)
         index = m_pChannelManager->isExistPresetList(pItem);
    else
         index = pItem->presetIndex();
    pItem->setPresetIndex(index);
    this->m_iCurrentPresetIndex = index;
}

void
DABUIListener::setAnnouncementFlag(quint16 flag)
{
    this->m_iAnnouncementFlag = flag;
}

void
DABUIListener::announcementScreenOn(bool on, int flag, QString label)
{
    qDebug() << __FUNCTION__ << "Receive ANNOUNCEMENT EVENT : on = " << on << ", serviceName = " << label << ", flag = 0x"  << hex << flag;

    if(on)
    {
        if(flag & 0x0001)
        {
            emit cmdReqAlarmStart(flag, label);
        }
        else if(flag & 0x0002)
        {
            emit cmdReqTAStart(flag, label);
        }
    }
    else
    {
        if(flag & 0x0001)
        {
            emit cmdReqAlarmStop();
        }
        else if(flag & 0x0002)
        {
            emit cmdReqTAStop();
        }
    }
}

QString
DABUIListener::getTAType(quint16 flags)
{
    QString flagLabel = NULL;

#if 1
    if(flags & 0x0001)
        flagLabel = "Alam";
    else if(flags & 0x0002)
        flagLabel = "Traffic";
#else
    if(flags & 0x0001)
        flagLabel = "Alam";
    else if(flags & 0x0002)
        flagLabel = "Traffic";
    else if(flags & 0x0004)
        flagLabel = "Travel";
    else if(flags & 0x0008)
        flagLabel = "Warning";
    else if(flags & 0x0010)
        flagLabel = "News";
    else if(flags & 0x0020)
        flagLabel = "Weather";
    else if(flags & 0x0040)
        flagLabel = "Event";
    else if(flags & 0x0080)
        flagLabel = "Special";
    else if(flags & 0x0100)
        flagLabel = "Rad_info";
    else if(flags & 0x0200)
        flagLabel = "Sports";
    else if(flags & 0x0400)
        flagLabel = "Finance";
#endif

    return flagLabel;
}

QString
DABUIListener::getSectionStatus(quint32 freq)
{
    qDebug() << Q_FUNC_INFO;
    QString str = NULL;

    if(m_pChannelManager->getCurrentFrequencyID() == (int)freq)
        str = "open";
    else
        str = "close";

    return str;
}

void
DABUIListener::getCurrentServiceEPGList(QDateTime date)
{   
    m_DABController.allScanStop();
    ListModel* pListModel = m_pChannelManager->getCurrentChannelList();
    DABChannelItem* pCurrItem = m_pChannelManager->getCurrentChannelItem();
    int listCount = pListModel->rowCount();
    quint32 freq = pCurrItem->frequency();
    QString currServiceName = pCurrItem->label();
    QString ensemble = pCurrItem->ensembleLabel();
    bool isExist = false;
    QString firstServiceName = "";
    quint32 mjd = DABLogger::instance()->convertDateToMJD(date.date());
    QHash<QString, QVariant*>* pEnsembleHash = this->m_EPGManager.getEnsembleHash(mjd, ensemble);

    if(pEnsembleHash == NULL)
    {
        qDebug() << __FUNCTION__ << ": EnsembleHash Table is not exist!!";
        emit existenceEPGData(false);
        this->m_EPGManager.setDateListIndex(-1);
        return;
    }

    for(int i=0; i < listCount; i++)
    {
        DABChannelItem* pItem = (DABChannelItem*) pListModel->dataItem(i);
        QString serviceName = pItem->label();
        if(freq == pItem->frequency())
        {
#if 1
            if(this->m_EPGManager.checkExistEPGHashTable(mjd, pEnsembleHash, serviceName))
#else
            if(this->m_EPGManager.checkExistEPGHashTable(pEnsembleHash, serviceName))
#endif			
            {
                emit receiveServiceNameForEPG(serviceName, ensemble);
                if(currServiceName == serviceName && !isExist)
                {
                    isExist = true;
                }

                if(firstServiceName == "")
                {
                    firstServiceName = serviceName;
                }
            }
        }
    }

    if(firstServiceName == "")
    {
        qDebug() << __FUNCTION__ << ": EPG Service is not exist!!";
        emit existenceEPGData(false);
        this->m_EPGManager.setDateListIndex(-1);
        return;
    }

    if(isExist)
    {
        this->getEPGDataList(date, ensemble, currServiceName);
    }
    else
    {
        this->getEPGDataList(date, ensemble, firstServiceName);
    }
    m_pChannelManager->saveTempEpgChannelList();
}

bool
DABUIListener::isCurrentDate(QDateTime epgDate){
    QDate currentDate = QDate::currentDate();
    QDate epgSelectDate = epgDate.date();

    qDebug() << " currentDate date : " << currentDate.toString() << ", epgSelectDate date : " << epgSelectDate.toString();

    if(epgSelectDate == currentDate)
        return true;
    else
        return false;
}

int
DABUIListener::getEPGDataList(QDateTime date, QString ensembleName, QString serviceName)
{
    qDebug() << __FUNCTION__ << " :  ensemble:" << ensembleName << ", service:" << serviceName;

    if(ensembleName == "..." || serviceName == "...")
    {
        qDebug() << __FUNCTION__ << " : ensemble name or service name is ...,  EPG List is not available";
        emit existenceEPGData(false);
        this->m_EPGManager.setDateListIndex(0);
        return 0;
    }

    QList<DABEPGItem*>* pListModel = NULL;
    DABEPGItem* pEpgItem = NULL;
    QList<DABEPGItem*>::iterator i;    
    DABEPGReservationItem* pEPGReservationItem = m_DABController.getEPGReservationItem();
    QTime tempTime;

   if(pEPGReservationItem == NULL)
   {
       qDebug() << __FUNCTION__ << "=========== EPGReservationItem is NULL";
   }
   else
   {
       tempTime = pEPGReservationItem->getStartTime();
   }

   qDebug() << "=========================tempTime==================" << tempTime.toString();

    quint32 mjd = DABLogger::instance()->convertDateToMJD(date.date());

    qDebug() << __FUNCTION__ << ": mjd = " << mjd << " date = " << date.toString();

    int hashIndex = this->m_EPGManager.getEPGHashIndex(mjd, false);

    if(hashIndex == -1)
    {
        qDebug() << __FUNCTION__ << " : hashIndex is -1 , EPG List is not available";
        emit existenceEPGData(false);
        this->m_EPGManager.setDateListIndex(0);
        return 0;
    }

    pListModel = this->m_EPGManager.getCurrentEPGData(hashIndex, ensembleName, serviceName);

    if(pListModel == NULL || pListModel->count() == 0)
    {
        qDebug() << __FUNCTION__ << " : pListModel is NULL, EPG List is not available";
        emit existenceEPGData(false);
        this->m_EPGManager.setDateListIndex(0);
        return 0;
    }
#if 0
    int index = 0;
    qDebug() << "Before Sorting =================================================";
    for(i = pListModel->begin(); i != pListModel->end(); i++, index++)
    {
        pEpgItem = i[0];
        QTime sTime(pEpgItem->hour(), pEpgItem->minute(), pEpgItem->second());
        QTime eTime = sTime.addSecs(pEpgItem->duration());
        qDebug() << "["<<index<<"] StartTime:" << (sTime.toString()) << ", EndTime:" << eTime.toString();
    }
#endif

#if 1
    QTime currentTime = QTime::currentTime();
    QDate currentDate = QDate::currentDate();
    quint32 currentMJD = DABLogger::instance()->convertDateToMJD(currentDate);
#else
    QDate currentDate = QDate::currentDate();
    quint32 currentMJD = DABLogger::instance()->convertDateToMJD(currentDate);
//    if(mjd == currentMJD)
//        this->m_EPGManager.sortEPGInOrderToCurrentTime(pListModel);
#endif
#if 0
    index = 0;
    qDebug() << "After Sorting =================================================";
    for(i = pListModel->begin(); i != pListModel->end(); i++)
    {
        pEpgItem = i[0];
        QTime sTime(pEpgItem->hour(), pEpgItem->minute(), pEpgItem->second());
        QTime eTime = sTime.addSecs(pEpgItem->duration());
        qDebug() << "["<<index<<"] StartTime:" << (sTime.toString()) << ", EndTime:" << eTime.toString();
    }
    qDebug() << "===============================================================";
#endif

    for(i = pListModel->begin(); i != pListModel->end(); i++)
    {
        pEpgItem = i[0];
        QTime sTime(pEpgItem->hour(), pEpgItem->minute(), pEpgItem->second());
        QTime eTime = sTime.addSecs(pEpgItem->duration());

        qDebug() << "ServiceName:" << serviceName << " StartTime:" << sTime.toString() << ", EndTime:" << eTime.toString();

        QTime time(0,0,0);
        QTime endTime = pEpgItem->getEndTime();
        if(time == endTime)
        {
            endTime.setHMS(23,59,59);
        }

        if((!(endTime >= currentTime || pEpgItem->getStartTime() >= currentTime)) && (mjd == currentMJD))
        {
            continue;
        }

        if(pEPGReservationItem == NULL)
        {
            emit receiveEPGData(pEpgItem->getServiceID(), ensembleName, serviceName, sTime.toString("hh:mm"), eTime.toString("hh:mm"), pEpgItem->hour(), pEpgItem->minute(), pEpgItem->second(), pEpgItem->duration(), pEpgItem->programtitle(), pEpgItem->description(), false);
        }
        else if((pEPGReservationItem->getServiceID() == pEpgItem->getServiceID()) && (pEPGReservationItem->getFrequency() == pEpgItem->getFrequencyID()) && (pEPGReservationItem->getStartTime() == sTime) && (pEPGReservationItem->getMJD() == mjd))
        {
            emit receiveEPGData(pEpgItem->getServiceID(), ensembleName, serviceName, sTime.toString("hh:mm"), eTime.toString("hh:mm"), pEpgItem->hour(), pEpgItem->minute(), pEpgItem->second(), pEpgItem->duration(), pEpgItem->programtitle(), pEpgItem->description(), true);
        }
        else
        {
            emit receiveEPGData(pEpgItem->getServiceID(), ensembleName, serviceName, sTime.toString("hh:mm"), eTime.toString("hh:mm"), pEpgItem->hour(), pEpgItem->minute(), pEpgItem->second(), pEpgItem->duration(), pEpgItem->programtitle(), pEpgItem->description(), false);
        }
    }

    int listCount = pListModel->count();

    if(listCount == 0)
    {
        qDebug() << __FUNCTION__ << " : EPG List is not available";
        emit existenceEPGData(false);
        this->m_EPGManager.setDateListIndex(-1);
    }
    else
    {
        emit existenceEPGData(true);
        this->m_EPGManager.setDateListIndex(hashIndex);
    }

    return listCount;
}



//////////////////////////////////////////////
// Data Management
void
DABUIListener::washOffCurrentChannelData(void)
{
    DABChannelItem* pItem = m_pChannelManager->getCurrentChannelItem();
    pItem->washOffData();
    ListModel* channelList = m_pChannelManager->getCurrentChannelList();
    channelList->removeAllRows();
    m_pChannelManager->setChannelCount(0);
    ListModel* presetList = m_pChannelManager->getCurrentPresetList();
    presetList->removeAllRows();
    setPtyName(255);
}

void
DABUIListener::removeSLSFile(QString sls)
{
    if(QFile::exists(sls))
    {
        qDebug() << "exists !!! removeSLSFile : filename = " << sls;
        QFile::remove(sls);
    }
    else
    {
        qDebug() << "NOT exists !!! removeSLSFile : filename = " << sls;
    }
}

void
DABUIListener::setServiceStatus (QString str) {
    qDebug() << Q_FUNC_INFO << ": str = " << str;
    m_sServiceStatus = str;
    emit serviceStatusChanged();
}

void
DABUIListener::onEvtApperPreservedPopup(QString title, QString serviceName)
{
    emit cmdReqEPGPreservePopup(title, serviceName);
}

void
DABUIListener::onReqUpdatePresetList(int currIndex, int realIndex)
{
    qDebug() << Q_FUNC_INFO << " Called Parm : currIndex = " << currIndex << "  realIndex = " << realIndex;

    m_DABController.allScanStop();

    DABChannelItem* pItem = m_pChannelManager->getCurrentChannelItem();
    ListModel* pPresetList = m_pChannelManager->getCurrentPresetList();
    int count = pPresetList->rowCount();
    int insertIndex = 0;

    if(realIndex == -1) // Empty ...
    {
        insertIndex = count;
    }
    else
    {
        if( !pPresetList->removeRow(realIndex) )
        {
            qDebug() << __FUNCTION__ << " removeRow false.";
        }
        insertIndex = realIndex;
    }

    pItem->setPresetIndex(currIndex);
    pPresetList->insertRow(insertIndex,new DABChannelItem(pItem));

    this->m_iCurrentPresetIndex = currIndex;
    setPresetIndex(this->m_iCurrentPresetIndex);
    emit indexUpdateInPresetList(currIndex, insertIndex);
    m_DABController.setServiceStatus(DAB_SERVICE_IDLE);    
    m_DABController.onSetCurrentInfoToHomeMedia();
    saveChannelInfo();

    emit m_VRAdaptor.writePresetList();
}

void
DABUIListener::removeTempFiles(void)
{
    qDebug() << Q_FUNC_INFO;

    QStringList fileNames;
    QDir dir(DAB_DATA_ROOT);
    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList();

    qDebug() << " list size = " << list.size();

    if(list.size() == 0)
    {
        qDebug() << " list size is 0 !!. return !! ";
        return;
    }

    for(int i=0; i<list.size(); i++)
    {
        QFileInfo fileInfo = list.at(i);
        fileNames.append(fileInfo.fileName());
    }

    qDebug() << " filename List size = " << fileNames.size();

    if(fileNames.size() == 0)
    {
        qDebug() << " String list size is 0 !!. return !! ";
        return;
    }

    for(int j=0; j<fileNames.size(); j++)
    {
        qDebug() << "[" << j << "] FileName : " << fileNames.at(j);

        DABLogger::instance()->Log((QString(" [%1] File Name : %2").arg(j).arg(fileNames.at(j))));

        QString str = fileNames.at(j);

        if(str.contains(".JPEG", Qt::CaseInsensitive) || str.contains(".png", Qt::CaseInsensitive))
        {
            qDebug() << " JPG File Exists !!";
            DABLogger::instance()->Log(QString("JPEG or PNG File Exists !!. name = %1").arg(str));
            if(!dir.remove(str))
            {
                qDebug() << str << " file remove fail !!";
                DABLogger::instance()->Log(QString("file remove fail !!"));
            }
            else
            {
                DABLogger::instance()->Log(QString("file remove success !!"));
            }
        }
        else
        {
            qDebug() << " JPG File Not Exists !!";
        }
    }
}

bool
DABUIListener::checkCurrentChannelItem(void)
{
    DABChannelItem* pCurrentItem = m_pChannelManager->getCurrentChannelItem();
    DABChannelItem* pViewItem = this->getViewCurrentItem();

//    qDebug() << __FUNCTION__ << "============ current item frequency = " << pCurrentItem->frequency() << ", serviceID = " << pCurrentItem->serviceID() << ", subChId = " << pCurrentItem->subChId();
//    qDebug() << __FUNCTION__ << "============ view    item frequency = " << pViewItem->frequency() << ", serviceID = " << pViewItem->serviceID() << ", subChId = " << pViewItem->subChId();
    if((pCurrentItem->frequency() == pViewItem->frequency()) && (pCurrentItem->serviceID() == pViewItem->serviceID()) && (pCurrentItem->subChId() == pViewItem->subChId()))
        return true;
    else
        return false;
}

void
DABUIListener::modeChange(bool viewMode)
{
    qDebug() << Q_FUNC_INFO << " :: view mode = " << viewMode << ", current view mode = " << getViewMode();

    if(viewMode)
    {
        DABChannelItem* pItem = this->getViewCurrentItem();
        if(pItem->frequency() == 0 || pItem->serviceID() == 0)
        {
            qDebug() << __FUNCTION__ << " Frequency:"<< pItem->frequency() <<", ServiceID:"<<pItem->serviceID()<<". Can't view mode service channel info..";
            return;
        }

        setViewMode(viewMode);
        this->setCurrentChannelInfo(pItem, viewMode);
        m_DABController.StartTuneTimer();
        DABChannelItem* pCurrentItem = m_pChannelManager->getCurrentChannelItem();
        setEPG(m_EPGManager.getCurrentDateEPGExists(pCurrentItem->ensembleLabel(), pCurrentItem->label()));
    }
    else
    {
        m_DABController.StopTuneTimer();
        if(m_DABController.m_eServiceStatus == DAB_SERVICE_TUNE_SEEK){
            m_DABController.setServiceStatus(DAB_SERVICE_IDLE);
        }
        DABChannelItem* pItem = m_pChannelManager->getCurrentChannelItem();
        if(pItem->frequency() == 0 || pItem->serviceID() == 0)
        {
            qDebug() << __FUNCTION__ << " Frequency:"<< pItem->frequency() <<", ServiceID:"<<pItem->serviceID()<<". Can't nomarl mode service channel info..";
            return;
        }
        this->setViewChannelInfoFromItem(pItem);
        setViewMode(viewMode);
        this->setCurrentChannelInfo(pItem, viewMode);
    }
}

void
DABUIListener::changeReservationChannel(void)
{
    qDebug() << Q_FUNC_INFO;

    DABChannelItem* pItem = m_pChannelManager->getCurrentChannelItem();
    if(pItem == NULL)
    {
        qDebug() << __FUNCTION__ << " Channel Item is NULL !!. so return";
        return;
    }
    if(pItem->frequency() == 0 || pItem->serviceID() == 0)
    {
        qDebug() << __FUNCTION__ << " Frequency:"<< pItem->frequency() <<", ServiceID:"<<pItem->serviceID()<<". Can't nomarl mode service channel info..";
        return;
    }

    this->setViewChannelInfoFromItem(pItem);
    setViewMode(false);
    setBitrate(pItem->bitrate());
    setChannelInfo(pItem->frequencyLabel());
    setServiceName(pItem->label());
    setEnsembleName(pItem->ensembleLabel());
    setFrequencyID(pItem->frequency());
    setPS(pItem->ps());
    setCurrentPresetIndex(pItem);
    setPtyName (pItem->programeType());
    setPresetIndex(this->m_iCurrentPresetIndex);
    QString serviceID = QString::number(pItem->serviceID(), 16);
    setChannelLogo(serviceID);
    emit playIconUpdateInPresetList((m_iPresetIndex-1));
    ListModel* currentList = m_pChannelManager->getCurrentChannelList();
    int index = currentList->findIndex(pItem->key(), pItem->subKey(), pItem->id());
    if(index == -1)
        setEPG(false);
    else
        setEPG(m_EPGManager.getCurrentDateEPGExists(pItem->ensembleLabel(), pItem->label()));

    this->initDLS(false);
    this->setDABGoodCount(0);
    m_DABController.StopTuneTimer();
}

void
DABUIListener::changeNormalMode(void)
{
    DABChannelItem* pItem = this->m_pChannelManager->getCurrentChannelItem();
    if(pItem == NULL)
    {
        qDebug() << __FUNCTION__ << " Channel Item is NULL !!. so return";
        return;
    }
    if(pItem->frequency() == 0 || pItem->serviceID() == 0)
    {
        qDebug() << __FUNCTION__ << " Frequency:"<< pItem->frequency() <<", ServiceID:"<<pItem->serviceID()<<". Can't nomarl mode service channel info..";
        return;
    }
    this->setViewChannelInfoFromItem(pItem);
    setViewMode(false);
    this->setCurrentChannelInfo(pItem, false);    
}

void
DABUIListener::onLanguageChange(int languageID)
{
    qDebug() << Q_FUNC_INFO;
    this->m_pChannelManager->initPtyTable();
    this->m_iLanguageID = languageID;
}

void
DABUIListener::setFMtoDABIntervalCount(int count)
{
    this->setDABGoodCount(0);
    this->setFMtoDABInterval(count);
}

void
DABUIListener::debugSetAnnounceDelayTime(int time)
{
    qDebug() << Q_FUNC_INFO;
    quint32 value = (quint32)(time * 1000);
    emit cmdReqSetAnnounceDelay(value);
    this->setAnnounceDelayTime(time);
}

void
DABUIListener::debugSetAnnounceTimeout(int time)
{
    qDebug() << Q_FUNC_INFO;
    quint32 value = (quint32)(time * 60);
    emit cmdReqSetAnnounceTimeout(value);
    this->setAnnounceTimeout(time);
}

void
DABUIListener::debugSetAnnounceWeakTimeout(int time)
{
    qDebug() << Q_FUNC_INFO;
    quint32 value = (quint32)(time * 60);
    emit cmdReqSetAnnounceWeakTimeout(value);
}

void
DABUIListener::debugSetServiceLinkMuteTimeout(int time)
{
    qDebug() << Q_FUNC_INFO;
    quint32 value = (quint32)(time * 1000);
    emit cmdReqSetServLinkTimeout(value);
}

void
DABUIListener::debugSetSeekTimeout(int time)
{
    qDebug() << Q_FUNC_INFO;
    int value = time * 1000;
    this->setSeekTimeout(value);
}
