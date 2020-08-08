#ifndef DABUILISTENER_H
#define DABUILISTENER_H

/**
 * FileName: DABUIListener.h
 * Author: David.Bae
 * Time: 2011-08-23 15:18
 * Description:
 *          This file is used for communication with GUI QML file with Qt file.
 */

#include <QObject>
#include <QDebug>
#include <QSettings>
#include "ListModel.h"
#include "DABChannelItem.h"
#include "DABEPGItem.h"
#include "DABEPGManager.h"
#include "DABLogger.h"
#include "DABChannelManager.h"
#ifdef __VR_COMMAND__
#include "DABVRAdaptor.h"
#endif
#ifdef __ENG_MODE__
#include "DABEngineerModeItem.h"
#endif
#ifndef __DAB_DEBUG_MSG_OUTPUT_FILE__
#include <DHAVN_LogUtil.h>
#endif

#ifndef __DAB_DEBUG_MSG_OUTPUT_FILE__
#define qDebug()          qCritical()/*LOG_HIGH*/
#endif

class DABController;
class DABChannelManager;

class DABUIListener : public QObject
{
#ifndef __DAB_DEBUG_MSG_OUTPUT_FILE__
    USE_LOG_UTIL
#endif
    Q_OBJECT

    //Center Info
    Q_PROPERTY(QString m_sChannelInfo       READ getChannelInfo         WRITE setChannelInfo        NOTIFY channelInfoChanged)
    Q_PROPERTY(QString m_sServiceName       READ getServiceName         WRITE setServiceName        NOTIFY serviceNameChanged)
    Q_PROPERTY(QString m_sEnsembleName      READ getEnsembleName        WRITE setEnsembleName       NOTIFY ensembleNameChanged)
    Q_PROPERTY(int     m_iPresetIndex       READ getPresetIndex         WRITE setPresetIndex        NOTIFY presetIndexChanged)
    Q_PROPERTY(QString m_sChannelLogo       READ getChannelLogo         WRITE setChannelLogo        NOTIFY channelLogoChanged)
    Q_PROPERTY(int     m_sFrequencyID       READ getFrequencyID         WRITE setFrequencyID        NOTIFY frequencyIDChanged)
    Q_PROPERTY(quint8  m_sPtyName           READ getPtyName             WRITE setPtyName            NOTIFY ptyNameChanged)
    Q_PROPERTY(QString m_sDLS               READ getDLS                 WRITE setDLS                NOTIFY dlsChanged)
    Q_PROPERTY(QString m_sSLS               READ getSLS                 WRITE setSLS                NOTIFY slsChanged)
    Q_PROPERTY(bool    m_bEPG               READ getEPG                 WRITE setEPG                NOTIFY epgChanged)
    Q_PROPERTY(bool    m_bViewMode          READ getViewMode            WRITE setViewMode           NOTIFY viewModeChanged)
    Q_PROPERTY(QString m_sTitle             READ getTitle               WRITE setTitle              NOTIFY titleChanged)
    Q_PROPERTY(QString m_sArtist            READ getArtist              WRITE setArtist             NOTIFY artistChanged)
    Q_PROPERTY(quint8  m_iPS                READ getPS                  WRITE setPS                 NOTIFY psChanged)
    Q_PROPERTY(bool    m_bListScanningOn    READ getListScanningOn      WRITE setListScanningOn     NOTIFY listScanningOnChanged)
    Q_PROPERTY(bool    m_bPresetScanningOn  READ getPresetScanningOn    WRITE setPresetScanningOn   NOTIFY presetScanningOnChanged)
    Q_PROPERTY(bool    m_bEnsembleSeek      READ getEnsembleSeek        WRITE setEnsembleSeek       NOTIFY ensembleSeekChanged)

    //Setting
    Q_PROPERTY(bool m_bSlideShowOn          READ getSlideShowOn         WRITE setSlideShowOn        NOTIFY slideShowChanged)
    Q_PROPERTY(int m_iAnnouncementFlag      READ getAnnouncementFlag                                NOTIFY announcementFlagChanged)
    Q_PROPERTY(int m_iServiceFollowing      READ getServiceFollowing                                NOTIFY serviceFollowingChanged)
    Q_PROPERTY(int m_iBandSelection         READ getBandSelection                                   NOTIFY bandSelectionChanged)
    Q_PROPERTY(QString m_sServiceStatus     READ getServiceStatus       WRITE setServiceStatus      NOTIFY serviceStatusChanged)

    //Debug Info
    Q_PROPERTY(int m_iDABGoodCount          READ getDABGoodCount        WRITE setDABGoodCount        NOTIFY goodCountChanged)
    Q_PROPERTY(int m_iFMtoDABInterval       READ getFMtoDABInterval     WRITE setFMtoDABInterval     NOTIFY fmToDABIntervalChanged)
    Q_PROPERTY(int m_iReqCountDABtoMicom    READ getReqCountDABtoMicom  WRITE setReqCountDABtoMicom  NOTIFY reqCountDABtoMicomChanged)
    Q_PROPERTY(int m_iFMSensitivity         READ getFMSensitivity       WRITE setFMSensitivity       NOTIFY fmSensitivityChanged)
    Q_PROPERTY(quint16 m_iFMFequency        READ getFMFrequency         WRITE setFMFrequency         NOTIFY fmFrequencyChanged)
    Q_PROPERTY(QString m_sSystemTime        READ getSystemTime          WRITE setSystemTime          NOTIFY systemTimeChanged)
    Q_PROPERTY(QString m_sServiceEvent      READ getServiceEvent        WRITE setServiceEvent        NOTIFY serviceEventChanged)
    Q_PROPERTY(QString m_sControllerStatus  READ getControllerStatus    WRITE setControllerStatus    NOTIFY controllerStatusChanged)
    Q_PROPERTY(QString m_sModuleStatus      READ getModuleStatus        WRITE setModuleStatus        NOTIFY moduleStatusChanged)
    Q_PROPERTY(int m_iCER                   READ getCER                 WRITE setCER                 NOTIFY cerChanged)
    Q_PROPERTY(int m_iSNR                   READ getSNR                 WRITE setSNR                 NOTIFY snrChanged)
    Q_PROPERTY(int m_iRSSI                  READ getRSSI                WRITE setRSSI                NOTIFY srriChanged)
    Q_PROPERTY(int m_iCER_sub               READ getCER_sub             WRITE setCER_sub             NOTIFY cerSubChanged)
    Q_PROPERTY(int m_iSNR_sub               READ getSNR_sub             WRITE setSNR_sub             NOTIFY snrSubChanged)
    Q_PROPERTY(int m_iRSSI_sub              READ getRSSI_sub            WRITE setRSSI_sub            NOTIFY srriSubChanged)
    Q_PROPERTY(QString m_sServiceID         READ getCurrentServiceID    WRITE setCurrentServiceID    NOTIFY currentServiceIDChanged)
    Q_PROPERTY(QString m_sPicode            READ getPicode              WRITE setPicode              NOTIFY picodeChanged)
    Q_PROPERTY(QString m_sIPAddress         READ getIPAddress           WRITE setIPAddress           NOTIFY ipAddressChanged)
    Q_PROPERTY(QString m_sBootloaderVer     READ getBootloaderVer       WRITE setBootloaderVer       NOTIFY bootloaderVerChanged)
    Q_PROPERTY(QString m_sKernelVer         READ getKernelVer           WRITE setKernelVer           NOTIFY kernerlVerChanged)
    Q_PROPERTY(QString m_sApplicationVer    READ getApplicationVer      WRITE setApplicationVer      NOTIFY applicationVerChanged)
    Q_PROPERTY(int m_iBootCount             READ getBootCount           WRITE setBootCount           NOTIFY bootCountChanged)
    Q_PROPERTY(int m_iFirstEnsembleCount    READ getFirstEnsembleCount  WRITE setFirstEnsembleCount  NOTIFY firstEnsembleCountChanged)
    Q_PROPERTY(int m_iSecondEnsembleCount   READ getSecondEnsembleCount WRITE setSecondEnsembleCount NOTIFY secondEnsembleCountChanged)
    Q_PROPERTY(int m_iThirdEnsembleCount    READ getThirdEnsembleCount  WRITE setThirdEnsembleCount  NOTIFY thirdEnsembleCountChanged)
    Q_PROPERTY(int m_iPreserveCount         READ getPreserveCount       WRITE setPreserveCount       NOTIFY preserveCountChanged)
    Q_PROPERTY(int m_sChangeFrequencyID     READ getChangeFrequencyID   WRITE setChangeFrequencyID   NOTIFY changeFrequencyIDChanged)
    Q_PROPERTY(int m_iSignalStatus          READ getSignalStatus        WRITE setSignalStatus        NOTIFY signalStatusChanged)
    Q_PROPERTY(int m_iBitrate               READ getBitrate             WRITE setBitrate             NOTIFY bitrateChanged)
    Q_PROPERTY(int m_iAnnounceDeleayTime    READ getAnnounceDelayTime   WRITE setAnnounceDelayTime   NOTIFY announceDelayTimeChanged)
    Q_PROPERTY(int m_iServLinkMuteTimeout   READ getServLinkMuteTimeout WRITE setServLinkMuteTimeout NOTIFY servLinkMuteTimeoutChanged)
    Q_PROPERTY(int m_iAnnounceTimeout       READ getAnnounceTimeout     WRITE setAnnounceTimeout     NOTIFY announceTimeoutChanged)    


public:
    DABChannelManager *m_pChannelManager;
    explicit DABUIListener(DABController &aDABController, QObject *parent = 0);
    ~DABUIListener();
    DABChannelManager* getChannelManager(void);

    /*
    ===========================================================================================
    +++++++++++++++++++++++++++ Data Management +++++++++++++++++++++++++++++++++++++++++++++++
    ===========================================================================================
    */
    void InitializeData(QObject *parent);
    void saveData(void);
    void loadSettingFromFile(void);
    void removeTempFiles(void);
    void removeDLSSLSData(void);
    void PrepareCurrentChannelInfo(void);
    void washOffCurrentChannelData(void);

    /*
    ===========================================================================================
    +++++++++++++++++++++++++++ Player Main +++++++++++++++++++++++++++++++++++++++++++++++++++
    ===========================================================================================
    */
    DABChannelItem* getViewCurrentItem(void) { return m_ViewChannel; }
    bool checkCurrentChannelItem(void);
    bool setCurrentChannelInfoFromChannelList(int index);
    bool setCurrentChannelInfoFromStationlList(int index, QString type);
    void setCurrentChannelInfoFromItem(DABChannelItem* pItem);
    void setViewChannelInfoFromItem(DABChannelItem* pItem);
    bool setCurrentChannelInfoFromServiceID(quint32 serviceID);
    bool setViewChanneltoCurrentChannel(void);
    bool setNextChannelInfoForTuneUpDown(void);
    bool setPrevChannelInfoForTuneUpDown(void);
    bool setNextChannelInfoForShortSeek(void);
    bool setPrevChannelInfoForShortSeek(void);

    void setCurrentChannelInfo(DABChannelItem* pItem, bool viewMode);
    QString getChannelInfo(void) const { return m_sChannelInfo; }
    void setChannelInfo(QString channel) { this->m_sChannelInfo = channel; emit channelInfoChanged(); }
    QString getServiceName(void) const { return m_sServiceName; }
    void setServiceName(QString sName) { this->m_sServiceName = sName; emit serviceNameChanged(); }
    QString getEnsembleName(void) const { return m_sEnsembleName; }
    void setEnsembleName(QString eName) { this->m_sEnsembleName = eName; emit ensembleNameChanged(); }
    int  getFrequencyID(void) const { return m_sFrequencyID; }
    void setFrequencyID(int frequency) { this->m_sFrequencyID = frequency; emit frequencyIDChanged(); }
    int  getChangeFrequencyID(void) const { return m_sChangeFrequencyID; }
    void setChangeFrequencyID(int frequency);
    QString getChannelLogo(void) const { return m_sChannelLogo; }
    void setPtyName(quint8 pty);
    quint8 getPtyName(void) const { return m_sPtyName; }
    bool getEnsembleSeek(void) { return this->m_bEnsembleSeek; }
    void setEnsembleSeek(bool onOff){ this->m_bEnsembleSeek = onOff; emit ensembleSeekChanged(); }

    /*
    ===========================================================================================
    +++++++++++++++++++++++++++ Station List / Preset List Scan +++++++++++++++++++++++++++++++
    ===========================================================================================
    */
    bool getListScanningOn(void) { return this->m_bListScanningOn; }
    void setListScanningOn(bool on) { this->m_bListScanningOn = on; emit listScanningOnChanged(); }
    bool getPresetScanningOn(void) { return this->m_bPresetScanningOn; }
    void setPresetScanningOn(bool on) { this->m_bPresetScanningOn = on; emit presetScanningOnChanged(); }
    void setPresetIndex(int index);
    int getPresetIndex(void) const { return m_iPresetIndex; }


    /*
    ===========================================================================================
    +++++++++++++++++++++++++++ Station List+++++++++++++++++++++++++++++++++++++++++++++++++++
    ===========================================================================================
    */
    ListModel* getStationList(void) const { return this->m_StationList; }
    ListModel* getStationListPTY(void) const { return this->m_StationListPty; }
    ListModel* getStationListAlphabet(void) const { return this->m_StationListAlphabet; }
    QString getSectionStatus(quint32 freq);
    void setClearCount(int count) { this->m_iClearCount = count; }
    int getClearCount(void) { return this->m_iClearCount; }
    int getLanguageID(void) { return this->m_iLanguageID; }

    /*
    ===========================================================================================
    +++++++++++++++++++++++++++ Preset List++++++++++++++++++++++++++++++++++++++++++++++++++++
    ===========================================================================================
    */
    void setCurrentPresetIndex(DABChannelItem* pItem);
    bool setCurrentChannelInfoFromPresetList(int index);



    /*
    ===========================================================================================
    +++++++++++++++++++++++++++ SLS Info ++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ===========================================================================================
    */
    QString getSLS(void) { return m_sSLS; }
    void setSLS(QString sls);


    /*
    ===========================================================================================
    +++++++++++++++++++++++++++ DLS, DL+ Info +++++++++++++++++++++++++++++++++++++++++++++++++
    ===========================================================================================
    */
    void initDLS(bool init);
    void initDLS(void);
    void initDLPlus(void);
    bool getViewMode(void) { return m_bViewMode; }
    void setViewMode(bool onOff) { this->m_bViewMode = onOff; emit viewModeChanged(); }
    QString getDLS(void) { return m_sDLS; }
    void setDLS(QString dls){ this->m_sDLS = dls; emit dlsChanged(); }
    QString getTitle(void) { return m_sTitle; }
    void setTitle(QString title){ this->m_sTitle = title; emit titleChanged(); }
    QString getArtist(void) { return m_sArtist; }
    void setArtist(QString artist){ this->m_sArtist = artist; emit artistChanged(); }

    /*
    ===========================================================================================
    +++++++++++++++++++++++++++ EPG List ++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ===========================================================================================
    */
    void initEPG(void);
    Q_INVOKABLE int getEPGDataList(QDateTime date, QString ensembleName, QString serviceName);
    Q_INVOKABLE void getCurrentServiceEPGList(QDateTime date);
    Q_INVOKABLE bool isCurrentDate(QDateTime epgDate);
    void removeCurrentEPG(quint32 sID, quint32 mjd);
    bool getEPG(void) { return m_bEPG; }
    void setEPG(bool onOff) { this->m_bEPG = onOff; emit epgChanged(); }
    void showEPG(void);


    /*
    ===========================================================================================
    +++++++++++++++++++++++++++ Setting +++++++++++++++++++++++++++++++++++++++++++++++++++++++
    ===========================================================================================
    */
    bool getSlideShowOn(void) {return this->m_bSlideShowOn;}
    void setSlideShowOn(bool isOn) {this->m_bSlideShowOn = isOn; emit slideShowChanged(isOn);}
    quint16 getAnnouncementFlag(void) { return this->m_iAnnouncementFlag;}
    void setAnnouncementFlag(quint16 flag);    
    void announcementScreenOn(bool on, int flag, QString label);
    QString getTAType(quint16 flags);
    int getServiceFollowing(void) {return this->m_iServiceFollowing;}
    void setServiceFollowing(int mode) {this->m_iServiceFollowing = mode; emit serviceFollowingChanged();}
    int getBandSelection(void) {return this->m_iBandSelection; }
    void setBandSelection(int mode) { this->m_iBandSelection = mode; emit bandSelectionChanged();}


    /*
    ===========================================================================================
    +++++++++++++++++++++++++++ External Communication Function +++++++++++++++++++++++++++++++
    ===========================================================================================
    */
    void initVRAdaptor(void);


    /*
    ===========================================================================================
    +++++++++++++++++++++++++++ Debugging Info ++++++++++++++++++++++++++++++++++++++++++++++++
    ===========================================================================================
    */

    Q_INVOKABLE void setFMtoDABIntervalCount(int count);
    Q_INVOKABLE void debugSetAnnounceDelayTime(int time);
    Q_INVOKABLE void debugSetAnnounceTimeout(int time);
    Q_INVOKABLE void debugSetAnnounceWeakTimeout(int time);
    Q_INVOKABLE void debugSetServiceLinkMuteTimeout(int time);
    Q_INVOKABLE void debugSetSeekTimeout(int time);
    QString getServiceStatus (void) { return m_sServiceStatus; }
    void setServiceStatus (QString str);
    int getDABGoodCount(void) { return m_iDABGoodCount; }
    void setDABGoodCount(int count) { this->m_iDABGoodCount = count; emit goodCountChanged(); }
    void setFMtoDABInterval(int count) { this->m_iFMtoDABInterval = count; emit fmToDABIntervalChanged(); }
    int getFMtoDABInterval(void) { return m_iFMtoDABInterval; }
    void setReqCountDABtoMicom(int count) { this->m_iReqCountDABtoMicom = count; emit reqCountDABtoMicomChanged(); }
    int getReqCountDABtoMicom(void) { return m_iReqCountDABtoMicom; }
    void setFMSensitivity(int value) { this->m_iFMSensitivity = value; emit fmSensitivityChanged(); }
    int getFMSensitivity(void) { return m_iFMSensitivity; }          
    void setFMFrequency(int value) { this->m_iFMFequency = value; emit fmFrequencyChanged(); }
    quint16 getFMFrequency(void) { return m_iFMFequency; }
    QString getSystemTime(void) { return m_sSystemTime; }
    void setSystemTime(QString time) { this->m_sSystemTime = time; emit systemTimeChanged(); }
    QString getServiceEvent(void) {return this->m_sServiceEvent;}
    void setServiceEvent(QString event) {this->m_sServiceEvent = event; emit serviceEventChanged(); }
    QString getControllerStatus(void) { return m_sControllerStatus; }
    void setControllerStatus (QString str) { m_sControllerStatus = str; emit controllerStatusChanged(); }
    QString getModuleStatus(void) { return m_sModuleStatus; }
    void setModuleStatus (QString str) { m_sModuleStatus = str; emit moduleStatusChanged(); }
    int getCER(void) {return m_iCER;}
    void setCER(int value){m_iCER = value; emit cerChanged();}
    int getSNR(void) {return m_iSNR;}
    void setSNR(int value) { m_iSNR = value; emit snrChanged();}
    int getRSSI(void) { return m_iRSSI;}
    void setRSSI(int value){ m_iRSSI = value; emit srriChanged();}
    int getCER_sub(void) {return m_iCER_sub;}
    void setCER_sub(int value){m_iCER_sub = value; emit cerSubChanged();}
    int getSNR_sub(void) {return m_iSNR_sub;}
    void setSNR_sub(int value) { m_iSNR_sub = value; emit snrSubChanged();}
    int getRSSI_sub(void) { return m_iRSSI_sub;}
    void setRSSI_sub(int value){ m_iRSSI_sub = value; emit srriSubChanged();}
    QString getCurrentServiceID(void) { return m_sServiceID;}
    void setCurrentServiceID(QString value){ m_sServiceID = value; emit currentServiceIDChanged();}
    QString getPicode(void) { return m_sPicode;}
    void setPicode(QString value) { m_sPicode = value; emit picodeChanged();}
    QString getIPAddress(void) { return m_sIPAddress; }
    QString getBootloaderVer(void) { return m_sBootloaderVer; }
    QString getKernelVer(void) { return m_sKernelVer; }
    QString getApplicationVer(void) { return m_sApplicationVer; }
    int  getBootCount(void) { return m_iBootCount; }
    void setIPAddress(QString addr) { this->m_sIPAddress = addr; emit ipAddressChanged();}
    void setBootloaderVer(QString addr) { this->m_sBootloaderVer = addr; emit bootloaderVerChanged();}
    void setKernelVer(QString addr) { this->m_sKernelVer = addr; emit kernerlVerChanged();}
    void setApplicationVer(QString addr) { this->m_sApplicationVer = addr; emit applicationVerChanged();}
    void setBootCount(int count) { this->m_iBootCount = count; emit bootCountChanged();}
    int getFirstEnsembleCount(void) {return m_iFirstEnsembleCount;}
    int getSecondEnsembleCount(void) {return m_iSecondEnsembleCount;}
    int getThirdEnsembleCount(void) {return m_iPreserveCount;}
    int getPreserveCount(void) {return m_iThirdEnsembleCount;}
    void setFirstEnsembleCount(int count) {this->m_iFirstEnsembleCount = count; emit firstEnsembleCountChanged();}
    void setSecondEnsembleCount(int count) {this->m_iSecondEnsembleCount = count; emit secondEnsembleCountChanged();}
    void setThirdEnsembleCount(int count) {this->m_iThirdEnsembleCount = count; emit thirdEnsembleCountChanged();}
    int getSignalStatus(void) {return m_iSignalStatus;}
    void setSignalStatus(int status){m_iSignalStatus = status; emit signalStatusChanged();}
    void setBitrate(int bitrate) { this->m_iBitrate = bitrate; emit bitrateChanged(); }
    int getBitrate(void){ return m_iBitrate; }
    void setPS(quint8 ps) { this->m_iPS = ps; emit psChanged(); }
    quint8  getPS(void) {return m_iPS; }
    void setAnnounceDelayTime(int time) { m_iAnnounceDeleayTime = time; emit announceDelayTimeChanged(); }
    int  getAnnounceDelayTime(void) { return m_iAnnounceDeleayTime; }
    void setServLinkMuteTimeout(int time) { m_iServLinkMuteTimeout = time; emit servLinkMuteTimeoutChanged(); }
    int  getServLinkMuteTimeout(void) { return m_iServLinkMuteTimeout; }
    void setAnnounceTimeout(int time) { m_iAnnounceTimeout = time; emit announceTimeoutChanged(); }
    int  getAnnounceTimeout(void) { return m_iAnnounceTimeout; }
    void setSeekTimeout(int time) { this->m_iSeekTimeout = time; }
    Q_INVOKABLE int getSeekTimeout(void) { return this->m_iSeekTimeout; }
    void initLanguageID(int languageID) {this->m_iLanguageID = languageID;}

signals:    
    void channelCountChanged(void);
    void channelInfoChanged(void);
    void serviceNameChanged(void);
    void ensembleNameChanged(void);
    void presetIndexChanged(void);
    void channelLogoChanged(int width, int height);
    void frequencyIDChanged(void);
    void changeFrequencyIDChanged(void);
    void ptyNameChanged(void);
    void slsChanged(void);
    void dlsChanged(void);
    void epgChanged(void);
    void viewModeChanged(void);
    void titleChanged(void);
    void artistChanged(void);
    void psChanged(void);
    void serviceEventChanged(void);
    void channelListChanged1(void);
    void presetListChanged1(void);
    void existenceEPGData(bool onOff);
    void selectPresetIndexForSave(void);
    void popupCurrentChannelAlreadyInPreset(void);
    void popupAlreadySaveAsPreset(void);
    void ensembleSeekChanged(void);
    void listScanningOnChanged(void);
    void presetScanningOnChanged(void);
    void announcementStart();
    void announcementFinish();    
    void announcementFlagChanged(bool all, bool alarm, bool traffic, bool travel, bool warning, bool news, bool weather, bool event, bool special, bool rad_info, bool sports, bool finance);
    void systemTimeChanged();
    void controllerStatusChanged(void);
    void moduleStatusChanged(void);
    void serviceStatusChanged(void);
    void goodCountChanged(void);
    void fmToDABIntervalChanged(void);
    void reqCountDABtoMicomChanged(void);
    void fmSensitivityChanged(void);
    void fmFrequencyChanged(void);
    void cerChanged(void);
    void snrChanged(void);
    void srriChanged(void);
    void cerSubChanged(void);
    void snrSubChanged(void);
    void srriSubChanged(void);
    void picodeChanged(void);
    void currentServiceIDChanged(void);
    void ipAddressChanged(void);
    void bootloaderVerChanged(void);
    void kernerlVerChanged(void);
    void applicationVerChanged(void);
    void bootCountChanged(void);
    void signalStatusChanged(void);
    void firstEnsembleCountChanged(void);
    void secondEnsembleCountChanged(void);
    void thirdEnsembleCountChanged(void);
    void preserveCountChanged(void);
    void bitrateChanged(void);
    void announceDelayTimeChanged(void);
    void servLinkMuteTimeoutChanged(void);
    void announceTimeoutChanged(void);
    void slideShowChanged(bool isOn);
    void clockChanged(void);
    void serviceFollowingChanged(void);
    void bandSelectionChanged(void);
    void sendEnsembletoEPG(QString ensembleName, quint32 frequency);
    void serviceNameForEPG(int listIndex, QString ensembleName, QString serviceName, bool isSkipped, int pty, int lang, QString programTitle, QString description);
    void receiveEPGData(quint32 serviceID, QString ensembleName, QString serviceName, QString sTime, QString eTime, int hour, int minute, int second, int duration, QString programTitle, QString description, bool reserveIcon);
    void receiveServiceNameForEPG(QString serviceName, QString ensembleName);
    void cmdReqEPGPreservePopup(QString title, QString serviceName);
    void updateChannelList(DABChannelItem* pItem);
    void cmdReqTAStart(quint16 flag, QString serviceName);
    void cmdReqTAStop(void);
    void cmdReqAlarmStart(quint16 flag, QString serviceName);
    void cmdReqAlarmStop(void);
    void playIconUpdateInPresetList(int currPresetindex);
    void indexUpdateInPresetList(int currPresetindex, int realPresetindex);
    void checkExistChannelPresetList(bool isOn);
    void cmdReqSetAnnounceDelay(quint32 time);
    void cmdReqSetAnnounceTimeout(quint32 time);
    void cmdReqSetAnnounceWeakTimeout(quint32 time);
    void cmdReqSetServLinkTimeout(quint32 time);
    void cmdReqdisplayOSD(QString serviceName, QString freqLabel, quint8 op_type);

public slots:
    void onAddEPGItem(quint32 mjd, quint32 serviceID, int Hour, int Minute, int Second, int Duration, QString ProgramLabel, QString Description);
    void onEvtApperPreservedPopup(QString title, QString serviceName);
    void onReqUpdatePresetList(int currIndex, int realIndex);   
    void removeSLSFile(QString sls);
    void saveSettingToFile(void);
    void setPreserveCount(int count) {this->m_iPreserveCount = count; emit preserveCountChanged();}
    void setChannelLogo(QString eName);
    bool onSortingEPGData(quint32 mjd, quint32 sID);
    void onSortingCurrentChannelEPGData(quint32 mjd);
    void saveSettingInfo(void);
    void saveChannelInfo(void);
    void loadData(void);
    void modeChange(bool viewMode);
    void changeReservationChannel(void);
    void changeNormalMode(void);
    void onLanguageChange(int languageID);

private:
    int m_iCurrentPresetIndex;
    DABChannelItem* m_ViewChannel;
    ListModel* m_StationList;
    ListModel* m_StationListPty;
    ListModel* m_StationListAlphabet;
#ifdef __VR_COMMAND__
    DABVRAdaptor m_VRAdaptor;
#endif
    int m_iFollowingValue;      // 0 : normal, 1 : DABtoDAB, 2 : DAB to FM
    quint16 m_iAnnouncementFlag;
    bool m_bListScanningOn;
    bool m_bPresetScanningOn;
    bool m_bEnsembleSeek;
    bool m_bSlideShowOn;
    int m_iServiceFollowing;
    int m_iBandSelection;
    DABEPGManager m_EPGManager;
    QString m_sChannelInfo;
    QString m_sServiceName;
    QString m_sEnsembleName;
    QString m_sChannelLogo;
    bool    m_bViewMode;
    QString m_sServiceEvent;
    int     m_sChangeFrequencyID;
    int     m_sFrequencyID;
    int     m_iPresetIndex;
    quint8  m_sPtyName;
    QString m_sDLS;
    QString m_sSLS;
    bool    m_bEPG;
    QString m_sTitle;
    QString m_sArtist;
    int     m_iPS;
    QString m_sSystemTime;
    QString m_sControllerStatus;
    QString m_sModuleStatus;
    QString m_sServiceStatus;
    int m_iCER;
    int m_iSNR;
    int m_iRSSI;
    int m_iCER_sub;
    int m_iSNR_sub;
    int m_iRSSI_sub;
    int m_serviceID;
    int m_iSignalStatus;
    int m_iFirstEnsembleCount;
    int m_iSecondEnsembleCount;
    int m_iThirdEnsembleCount;
    int m_iPreserveCount;
    QString m_sServiceID;
    QString m_sIPAddress;
    QString m_sBootloaderVer;
    QString m_sKernelVer;
    QString m_sApplicationVer;
    int     m_iBootCount;
    int     m_iBitrate;
    int     m_iDABGoodCount;
    int     m_iFMtoDABInterval;
    int     m_iReqCountDABtoMicom;
    int     m_iFMSensitivity;
    quint16 m_iFMFequency;
    QString m_sPicode;
    int     m_iAnnounceDeleayTime;
    int     m_iAnnounceTimeout;
    int     m_iServLinkMuteTimeout;
    int     m_iSeekTimeout;
    int     m_iClearCount;
    int     m_iLanguageID;
    DABController &m_DABController;
};

#endif // DABUILISTENER_H
