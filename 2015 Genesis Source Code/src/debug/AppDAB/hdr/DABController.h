#ifndef DABCONTROLLER_H
#define DABCONTROLLER_H
/**
 * FileName: DABController.h
 * Author: David.Bae
 * Time: 2011-07-22 11:49
 */

#include <QObject>
#include <QTimer>
#include "DABUIListener.h"
#include "DABDataStorage.h"
#include "DABCommManager.h"
#include "DABChannelManager.h"
#include "DABEPGReservationList.h"
#include "DABRdsAdaptor.h"
#include "DABLogger.h"
#ifndef __DAB_DEBUG_MSG_OUTPUT_FILE__
#include <DHAVN_LogUtil.h>
#endif

#ifndef __DAB_DEBUG_MSG_OUTPUT_FILE__
#define qDebug()          qCritical()/*LOG_HIGH*/
#endif

class DABController : public QObject
{
#ifndef __DAB_DEBUG_MSG_OUTPUT_FILE__
    USE_LOG_UTIL
#endif
    Q_OBJECT

public:
    enum CONTROLLER_STATUS
    {
        DAB_STATUS_STARTUP = 0,
        DAB_STATUS_IDLE,
        DAB_STATUS_READY,
        DAB_STATUS_SCANNING,
        DAB_STATUS_WAITING,
        DAB_STATUS_SERVICE,
        DAB_STATUS_END
    };

    enum DAB_SEEK_TYPE
    {
        DAB_SEEK_NONE   = 0,
        DAB_SEEK_SHORT  = 1,
        DAB_SEEK_LONG   = 2
    };

    explicit DABController(QObject *parent = 0);
    ~DABController();
    DABUIListener* getUIListener(void);
    void start(void);
    void setEPGReservationItem(DABEPGReservationItem* pItem);
    DABEPGReservationItem* getEPGReservationItem(void);
    inline CONTROLLER_STATUS getStatus(void) {return this->m_eStatus;}
    inline DAB_SERVICE_STATUS getServiceStatus(void) {return this->m_eServiceStatus;}
    inline eEVT_SEND_MODULE_STATUS getModuleStatus(void) {return this->m_eModuleStatus;}
    void setModuleStatus(eEVT_SEND_MODULE_STATUS moduleStatus) {this->m_eModuleStatus = moduleStatus;}
    void sendServiceInfo(void);
    void listScanStart(void);
    void listScanStop(void);
    bool isListStarted(void) { return m_bListScanning; }
    void PresetScanStart(void);
    void PresetScanStop(void);
    bool isPresetScanStarted(void) { return m_bPresetScanning; }
    void allScanStop(void);
    void StartScanTimer(quint8 serviceType);
    void StopScanTimer(void);
    void StartWeakSignalTimer(void);
    void StopWeakSignalTimer(void);
    void StartTuneTimer(void);
    void StopTuneTimer(void);
    void StartTASearchTimer(void);
    void StopTASearchTimer(void);
    void StartBroadcastingTimer(void);
    void StopBroadcastingTimer(void);
    void StartDeactiveFMLinkingTimer(void);
    void StopDeactiveFMLinkingTimer(void);
    void StartDeactiveTATimer(void);
    void StopDeactiveTATimer(void);
    void setServiceStatus(DAB_SERVICE_STATUS status);
    DAB_SERVICE_STATUS  m_eServiceStatus;
    QString getImageRootDir(void){ return m_sImageRootDir; }
    void setImageRootDir(QString dir) { this->m_sImageRootDir = dir; }
    void setRadioPlayBand(int band) {this->m_iRadioPlayBand = band;}
    void setServiveFollowingState(eDAB_SERVICE_FOLLOWING_STATE_INFO state);
    void setRadioPlayFreq(quint16 freq) { this->m_qRadioPlayFrequency = freq; }
    int getRadioPlayBand() { return this->m_iRadioPlayBand; }
    eDAB_SERVICE_FOLLOWING_STATE_INFO getServiceFollowingState() { return this->m_eServiceFollowingState; }
    void setRequestDABtoFM(bool value) { this->m_bRequestDABtoFM = value; }
    void setDeactiveDABtoFM(bool onOff) { this->m_bDeactiveDABtoFM = onOff; }
    bool getDeactiveDABtoFM(void) { return this->m_bDeactiveDABtoFM; }
    quint16 getRadioPlayFreq() { return this->m_qRadioPlayFrequency; }
    Q_INVOKABLE int getDLPlusTimeout(void){ return this->m_iDLPlustTimeout; }
    Q_INVOKABLE int getDLSTimeout(void){ return this->m_iDLSTimeout; }
    Q_INVOKABLE int getSLSTimeout(void){ return this->m_iSLSTimeout; }
    Q_INVOKABLE void seekDownFromStationList(void);
    Q_INVOKABLE void seekUpFromStationList(void);
    Q_INVOKABLE void debugRequestDABtoDAB(QString onOff);
    Q_INVOKABLE void sendServLinkCERValue(quint32 dab_worst, quint32 dab_bad, quint32 dab_nogood, quint32 dab_good, quint32 dabPlus_worst, quint32 dabPlus_bad, quint32 dabPlus_nogood, quint32 dabPlus_good);
    Q_INVOKABLE void sendtSigStatusCERValue(quint8 count, quint32 dab_bad, quint32 dab_nogood, quint32 dab_good, quint32 dabPlus_bad, quint32 dabPlus_nogood, quint32 dabPlus_good);
    Q_INVOKABLE void sendPinknoiseCERValue(quint8 unmute_count, quint8 mute_count, quint32 dab_bad, quint32 dab_good, quint32 dabPlus_bad, quint32 dabPlus_good);
    Q_INVOKABLE void debugSetDLSTimeout(int time);
    Q_INVOKABLE void debugSetSLSTimeout(int time);
    void initAnnouncementValue(void);
    void loadModuleVersion(void);
    void writeModuleVersion(void);
    void setReservationCount(int count) { this->m_iReservationCount = count; }
    void requestDABtoFM(void);
    void changeFMtoDAB(void);
    void requestFMtoDAB(void);
    bool checkSwitchingTime(void);
    bool getBroadcastingTimeActive(void) { return m_bLoadReservationItem; }
    void setTAInterrupt(bool value) { m_bTAInterrupt = value; }
    void checkEPGReservationList(void);
#ifdef __DTC__
    bool m_AliveUartMsg;
    bool m_AliveMsgOnOff;
#endif

signals:
    void cmdUpdateChannelListFromAutoScan(void);
    void cmdUpdateChannelListFromReconfiguration(void);
    void cmdUpdateChannelListFromReconfigurationNotChange(void);
    void cmdUpdateChannelItem(quint32 freq, quint16 eId, quint8 iId, QString eLabel, quint8 sCount, quint32 sID, quint8 sType, quint8 subChId, quint16 bitrate, quint8 ps, quint8 charset, quint8 pty, quint8 language, QString sLabel, stASU_INFO asuInfo, stPICODE_INFO picodeInfo, quint8 subCHSize, quint16 address);
    void cmdAddTempChannelItem(quint32 freq, quint16 eId, quint8 iId, QString eLabel, quint8 sCount, quint32 sID, quint8 sType, quint8 subChId, quint16 bitrate, quint8 ps, quint8 charset, quint8 pty, quint8 language, QString sLabel, stASU_INFO asuInfo, stPICODE_INFO picodeInfo, quint8 subCHSize, quint16 address);
    void cmdAddNewChannelItem(quint32 freq, quint16 eId, quint8 iId, QString eLabel, quint8 sCount, quint32 sID, quint8 sType, quint8 subChId, quint16 bitrate, quint8 ps, quint8 charset, quint8 pty, quint8 language, QString sLabel, stASU_INFO asuInfo, stPICODE_INFO picodeInfo, quint8 subCHSize, quint16 address);
    void cmdReqClusterControl(QString serviceName, quint8 presetNumber, quint8 opState);
    void cmdReqdisplayOSD(QString serviceName, QString freqLabel, quint8 op_type);
    void cmdReqDABSoundOn(void);
    void cmdReqRadioEnd(int band, quint16 frequency);
    void cmdReqDABtoFM(quint16 playPICode);
    void cmdResponseAddReservation(bool isReserve, int hour, int minute, int second);
    void cancelEPGReservation(bool isSystemPopup);
    void cmdReservationCountFull();
    void displayLinkingIcon(bool isDABtoDAB, bool isDABtoFM, bool isSearching);
    void alreadyPreserved();
    void updateChannelItemStatus(quint8 type, bool onOff);
    void cmdRemoveUnlockFrequency(quint32 freq);
    void cmdReqVersion(QString ver);
#ifdef __DAB_TPEG__
    void cmdSendTPEGData(const QByteArray &data, int length);
#endif
    void cmdLgeDBus_DABPICodeRequest(quint16 playPICode);
    void cmdReqTrafficDABtoRDS(bool onOff);
    void cmdReqAllAnnounceDABtoRDS(bool bTraffic);
    void newsFlagChanged();
    void closeOptionMenu();
    void cmdStartDABtoFMTimer();
    void cmdStopDABtoFMTimer();
    void cmdReqTASearchPopup(bool onOff);
    void cmdCloseSystemPopup(eDAB_SYSTEM_POPUP_TYPE m_ePopupType);
    void cmdCheckReservationItem(void);
    void loadEPGListBySelect(void);
    void sendAudioInfo(quint8 protectType, quint8 protectLevel, quint8 protectOption);
    void cmdSendMuteCancel();
    void playIndexUpdateForScan(quint32 frequency, quint32 serviceID, QString serviceName);
    void menuEnableInitialized();

public slots:
    void onReqSeekDown(void);
    void onReqSeekUp(void);
    void onReqLongSeekDown(void);
    void onReqLongSeekUp(void);
    void onReqTuneDown(void);
    void onReqTuneUp(void);
    void onReqTunePressed(void);
    void onRspTrafficRDStoDAB(bool onOff);
    void onReqSetAnnounement(bool onOff);
    void onReqTA(bool onOff);
    void onReqTASetting(bool onOff);
    void onReqCancelTrafficAnnouncement(bool onOff);
    void onReqCancelAlarmAnnouncement(void);
    void onReqScan(void);
    void onReqPresetScan(void);
    void onReqSaveData(void);
    void onReqSeekNextPresetList(void);
    void onReqSeekPrevPresetList(void);
    void onReqTASearch(void);
    void onReqPresetSelected(int count);
    void onReqListSelected(int count, QString type);
    void onSetCurrentChannelInfo(int serviceID);
    void onReqSetReservationChannel(void);
    void onSettingSlideShow(bool isON);
    void onSettingServiceFollowing(int mode);
    void onSettingBandSelection(int mode);
    void onSettingSaveData(void);
    void onFactoryResetRemoveData(QString path);

    //Request / Response
    void ReqGetVersion(void);
    void RspGetVersion(quint8 booloaderMajorVersion, quint8 bootloaderMinorVersion, quint8 kernelMajor, quint8 kernelMinor, quint8 appMajor, quint8 appMinor, quint8 appMicro, QString buildTime, quint8 bootCount);
    void ReqSetSeekService(eREQ_SET_FULL_SCAN_BAND band, eREQ_SET_SEEK_SERVICE_DIRECTION direction, bool isEnsembleSeek);
    void RspSetSeekService(eREQ_SET_FULL_SCAN_BAND band, quint8 direction, quint8 FilterType, quint8 FilterValue, quint32 Frequency, quint32 ServiceID, quint8 SubChId);
    void RspSetSelectService(quint32 frequency, quint8 serviceType, quint32 serviceID, quint8 subChId, quint16 bitrate, QString label);
    void RspSetAnnouncementFlag(quint16 flag);
    void ReqSetSelectService(void);
    void ReqFirstSelectService(void);
    void onReqSetAntennaStatus(quint8 status);
    void RspGetServiceList();
    void onRspGetDLS(QString sDLS, QString sTitle, QString sAlbum, QString sArtist);
    void RspGetSLS(QString fileName);
    void removeDuplicatEPGData(quint32 sID, quint32 mjd);
    void onRspGetEPG(quint32 mjd, quint32 sID);
    void CmdReqSetSeekStop(void);
    void CmdReqSetSelectStop(void);    
    void onChangeDABtoFM(void);
    void EvtSendModuleStatus(eEVT_SEND_MODULE_STATUS moduleState, eEVT_SEND_SERVICE_STATUS serviceState);
    void EvtSendChangeService(int freq, int serviceType, int serviceID, int SubChId, int Bitrate, int Pty, QString label, QString ensembelLabel,eEVT_SEND_CHANGE_SERVICE_EVENT event);
    void EvtSendAnnouncement(QString label, int flag, eEVT_SEND_ANNOUNCEMENT_STATUS status, quint8 subChID);
    void EvtSendTime(int year, int month, int day, int hour, int minute, int second);
    void EvtSendQOS(int CER, int SNR, int RSSI, int CER_sub, int SNR_sub, int RSSI_sub);
    void EvtUpdateDLS(quint8 update);
    void EvtUpdateSLS(void);
    void EvtUpdateEPG(void);
    void ScanTimeOut(void);
    void WeakTimeOut(void);
    void onTuneTimeOut(void);
    void onTASearchTimeOut(void);
    void onBroadcastingTimeTick(void);
    void EvtAutoScanInfo(quint8 status, quint32 frequency, quint8 lock);
    void EvtSendAnnouncementTAInvalid(quint16 flags);
    void sendChannelItem( quint32 freq, quint16 eId, quint8 iId, QString eLabel, quint8 sCount, quint32 sID, quint8 sType, quint8 subChId, quint16 bitrate, quint8 ps, quint8 charset, quint8 pty, quint8 language, QString sLabel, stASU_INFO asuInfo, stPICODE_INFO picodeInfo, quint8 subCHSize, quint16 address);
    void onSendClusterUpdate(void);
    void onSendOSDUpdate(void);
    void EvtSendPinkStatus(quint8 status);
    void setSystemTimeFromModule(int year, int month, int day, int hour, int minute, int second);
    void onSetCurrentInfoToHomeMedia(void);
    void EvtSendSignalInfo(quint8 status);
    void EvtServiceFollowingState(quint8 state);
    void onAddEPGReservation(QDateTime dateTime, QString serviceName, int serviceID, QString title, QString description, int hour, int minute, int second, int duration);
    void onAddRemoveEPGReservation(QDateTime dateTime, QString serviceName, int serviceID, QString title, QString description, int hour, int minute, int second, int duration);
    void onCheckExistServiceID(QDateTime dateTime, int serviceID, int hour, int minute, int second);
    void onCancelEPGReservation(bool isSystemPopup);
    void CmdAddStationLogo(QString filename);
    void onAudioInfo(quint8 ProtectType, quint8 ProtectLevel, quint8 ProtectOption);
#ifdef __ENG_MODE__
    void onReqVersion(int in0);
#endif
    void onEventExceptionHandler(eDAB_EVENT_TYPE event);
#ifdef __DAB_TPEG__
    void onRspTPEGData(const QByteArray &data);
#endif
#ifdef __DTC__
    void EvtSendDtcUart(void);
#endif
    void onReqSeekCancel(void);
    bool checkDABtoFM(void);
    void onReceiveFMSensitivity(const QByteArray &data);
    void sendPICodeToMicom(void);
    void initPICodeValue(void);
    void checkTAService(void);
    void onDeactivateTA(void);
    void onDeactivateFMLinking(void);
    void onOSDEPGStarted(void);

private:
    CONTROLLER_STATUS   m_eStatus;
    eEVT_SEND_MODULE_STATUS m_eModuleStatus;
    DABUIListener* pUIListener;
    DABCommManager* pCommManager;
    DABChannelManager* pChannelManager;
    DABEPGReservationList* pEPGReservationList;
    DABEPGReservationItem* pEPGReservationItem;
    bool    m_bGetVersionRequested;
    bool    m_bListScanning;
    bool    m_bPresetScanning;
    int     m_iPlayIndexBackup;
    bool    m_bExistCurrentChannel;
    bool    m_bExistCurrentChannelInPreset;
    int     m_iLinkingState;   //0: orginal, 1:DABtoDAB, 2:DABtoFM
    eDAB_SERVICE_FOLLOWING_STATE_INFO  m_eServiceFollowingState;
    bool    m_bIsAutoScan;
    int     m_iRadioPlayBand;
    bool    m_bRequestDABtoFM;    
    quint16 m_qRadioPlayFrequency;
    QTimer  m_TuneTimer;
    QTimer  m_TASearchTimer;
    QTimer  m_BroadcastingTimer;
    QTimer  m_DeactiveFMLinkingTimer;
    QTimer  m_DLPlusTimer;
    QTimer  m_DeactiveTATimer;
    QTimer  m_ScanTimer;
    QTimer  m_WeakSignalTimer;
    QString m_sImageRootDir;
    quint32 m_iPreviousFreq;
    int     m_iServiceEvent;
    quint8  m_bPICodeStatus;
    quint8  m_bPICodeSensitive;
    quint16 m_FMFrequency;
    bool    m_bPinkStatus;
    quint16 m_PICodeValue[12];
    int     m_PICodeOffset;
    int     m_PICodeValueCount;
    int     m_iReqGetVersionCount;
    int     m_iSLSTimeCount;
    int     m_iDLSTimeCount;
    int     m_iDLPlustTimeCount;
    int     m_iReservationCount;
    bool    m_bLoadReservationItem;
    int     m_iDLSTimeout;
    int     m_iSLSTimeout;
    int     m_iDLPlustTimeout;
    bool    m_bMuteCheck;
    bool    m_bDeactiveTA;
    bool    m_bTAInterrupt;
    bool    m_bSoundSettingFlag;
    bool    m_bVrStatus;

    /**
      2013.07.22 Mobilus DaehyungE
      TA 서비스 중에는 DAB-FM을 전환이 안되도록 처리한다.
      이유는 TA 발생시 Audio Path가 DAB에서 TA로 전환되는데, 이때 DAB-FM이 발생하면 Audio Path가 꼬이는 현상이 나타나서 사운드 무감이 현상이 발생할수도 있기 때문에 아예 원천 봉쇄한다.
    */
    bool    m_bDeactiveDABtoFM;
    bool    m_bDeactiveFMLinking;
    /**
    2013.06.30 Mobilus DaehyungE
    채널 선택시 3번째 signal status값은 무시한다.
    이유는 DAB-DAB, DAB-FM 지역에서 채널 선택시 DAB-FM으로 먼저 linking되는 현상을 방지하기 위해서임
    하지만 연속으로 NoGood or BAD Signal 들어오는 case에 대해서는 어쩔수 없이 DAB-FM으로 먼저 넘어간다.
    */
    int     m_bIgnoreBADSignal;

    /**
    2013.08.29 Mobilus DaehyungE
    Announcement SubChID를 따로 저장한다.
    이유는 Announcement 수신중에 SLS가 오면 원래 듣고 있던 방송이랑 SubChID를 비교해서 다르면 SLS 무시하기 위해서임
    */
    quint8 m_iTASubChId;
    FM_DAB_STATE_INFO m_eFMtoDABState;
    QString m_sBackupEPGTitle;
    QString m_sBackupEPGServiceName;

    void init(void);
    void createConnection(void);
    void setStatus(CONTROLLER_STATUS status);
    void setModulesStatusForDebugging(QString str);
    void setServiceStatusForDebugging(QString str);
    void setCurrentChannelItem(int freq, int serviceType, int serviceID, int subChId, int bitrate, QString label, QString ensembelLabel);
    void setCurrentChannelItemForSeek(int freq, int serviceType, int serviceID, int subChId, int bitrate, int pty, QString label, QString ensembelLabel);
};

#endif // DABCONTROLLER_H
