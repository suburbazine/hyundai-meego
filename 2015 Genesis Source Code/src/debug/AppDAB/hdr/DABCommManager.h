#ifndef DABCOMMMANAGER_H
#define DABCOMMMANAGER_H

#include <QObject>
#include <QProcess>
#include "DABCommDataHandler.h"
#include "DABCommSPIPacketInfo.h"
#include "DABCommReceiverThread.h"
#include "DABProtocol.h"

typedef struct
{
    quint8 m_ascll;
    quint16 m_unicode;
} EBU_CHAR_TABLE;

class DABCommManager : public QObject
{
    Q_OBJECT
public:
    explicit DABCommManager(QObject *parent = 0);
    ~DABCommManager();
    void startComm(void);
    void stopComm(void);
    void restartUART(void);
    DABCommUART m_ComUart;
    DABCommSPI m_ComSpi;
    DABCommSPIPacketInfo* getSPIPacketInfo(void);

    struct DABCommandDataBackup{
        quint8 data[100];
        quint8 cmd;
        int size;
    };

public:
    //Request Command from DAB Controller.. (Naming from DAB Module Specification Document)
    //Receive Response from DABCommReceiverThread (Naming from DAB Module Specification Document)
    void ReqGetVersion(void);
    void RspGetVersion(unsigned char* buff, int size);
    void ReqSetSeekService(quint8 band, quint8 direction, quint8 filterType, quint8 filterValueLen, QByteArray filterValue, quint32 frequency, quint32 serviceID, quint8 subChId);
    void RspSetSeekService(unsigned char* buff, int size);
    void ReqSetSeekStop(void);
    void RspSetSeekStop(unsigned char* buff, int size);
    void ReqSetSelectService(quint32 frequency, quint8 serviceType, quint32 serviceID, quint8 subChId, quint16 bitrate, quint8 subCHSize, quint16 address, quint8 ps,const char* label, quint8 picodeCount, quint16* picode);
    void RspSetSelectService(unsigned char* buff, int size);
    void ReqSetServiceInfo(quint32 frequency, quint8 serviceType, quint32 serviceID, quint8 subChId, quint16 bitrate, quint8 labelCharset, const char* label, quint8 picodeCount, const char* picodeValue);
    void ReqSetSelectStop(void);
    void RspSetSelectStop(unsigned char* buff, int size);
    void RegSetServiceFollowing(int mode);
    void RspSetServiceFollowing(unsigned char* buff, int size);
    void ReqSetAnnouncementFlag(quint16 flag);
    void RspSetAnnouncementFlag(unsigned char* buff, int size);
    void ReqGetServiceList(unsigned int frequency = 0);
    void RspGetServiceList(unsigned char* buff, int size);
    void ReqGetDLS(void);
    void RspGetDLS(unsigned char* buff, int size);
    void ReqGetSLS(void);
    void RspGetSLS(unsigned char* buff, int size);
    void ReqGetEPG(void);
    void ReqSetEvtSendTime(quint8 status);
    void RspSetEvtSendTime(unsigned char* buff, int size);
    void ReqSetDrcFlag(quint8 flag);
    void RspSetDrcFlag(unsigned char* buff, int size);
    void ReqSetSelectAnnouncementService(quint32 frequency, quint8 serviceType, quint32 serviceID, quint8 subChId, quint16 bitrate, quint8 subCHSize, quint16 address, const char* label, quint8 picodeCount, const char* picodeValue, quint16 flags, quint8 status);
    void RspSetSelectAnnouncementService(unsigned char* buff, int size);
    void ReqGetAnnouncementStatus(void);
    void RspGetAnnouncementStatus(unsigned char* buff, int size);
    void ReqGetPinkStatus(void);
    void RspGetPinkStatus(unsigned char* buff, int size);
    void ReqSetAutoScan(quint8 band, quint8 action, quint8 onOff);
    void RspSetAutoScan(unsigned char* buff, int size);
    void ReqSetFMService(quint8 band, quint32 piCode);
    void RspSetFMService(unsigned char* buff, int size);    
    void ReqSetBroadcasting(quint8 band);
    void RspSetBroadcasting(unsigned char* buff, int size);
    void ReqSetBandSelection(quint8 band);
    void ReqGetAudioInfo(void);
    void RspGetAudioInfo(unsigned char* buff, int size);
    void RspSetBandSelection(unsigned char* buff, int size);
    void RspSetAnnounceDelay(unsigned char* buff, int size);
    void RspSetAnnounceTimeout(unsigned char* buff, int size);
    void RspSetServLinkTimeout(unsigned char* buff, int size);
    void ReqSetServLinkCERValue(quint32 dab_worst, quint32 dab_bad, quint32 dab_nogood, quint32 dab_good, quint32 dabPlus_worst, quint32 dabPlus_bad, quint32 dabPlus_nogood, quint32 dabPlus_good);
    void RspSetServLinkCERValue(unsigned char* buff, int size);
    void ReqSetSigStatusCERValue(quint8 count, quint32 dab_bad, quint32 dab_nogood, quint32 dab_good, quint32 dabPlus_bad, quint32 dabPlus_nogood, quint32 dabPlus_good);
    void RspSetSigStatusCERValue(unsigned char* buff, int size);
    void ReqSetPinknoiseCERValue(quint8 unmute_count, quint8 mute_count, quint32 dab_bad, quint32 dab_good, quint32 dabPlus_bad, quint32 dabPlus_good);
    void RspSetPinknoiseCERValue(unsigned char *buff, int size);
    void RspSetAnnouncementWeakTimeout(unsigned char* buff, int size);
    void ReqSetAntennaStatus(quint8 flag);
    void RspSetAntennaStatus(unsigned char *buff, int size);

    ////////////////////////////////////////////////////////////////////////////////////////
    //Receive Event from DABCommReceiverThread (Naming from DAB Module Specification Document)
    ////////////////////////////////////////////////////////////////////////////////////////
    void EvtSendModuleStatus(quint8 ms, quint8 ss);
    void EvtSendChangeService(unsigned char* buff, int buffLen);
    void EvtSendAnnouncement(unsigned char* buff, int buffLen);
    void EvtSendTime(unsigned char* buff, int buffLen);
    void EvtSendQOS(unsigned char* buff, int buffLen);
    void EvtUpdateDLS(unsigned char* buff, int buffLen);
    void EvtUpdateSLS(unsigned char* buff, int buffLen);
    void EvtUpdateEPG(unsigned char* buff, int buffLen);
    void EvtDownloadStatus(unsigned char* buff, int buffLen);
    void EvtSendAutoScanInfo(unsigned char* buff, int buffLen);
    void EvtSendSignalInfo(unsigned char* buff, int buffLen);
    void EvtServiceFollowingState(unsigned char* buff, int buffLen);
    void EvtSendCountryInfo(unsigned char* buff, int buffLen);
    void EvtSendPinkStatus(unsigned char* buff, int buffLen);
    void EvtSendUserAppType(unsigned char* buff, int buffLen);
    void EvtSendAnnouncementTAInvalid(unsigned char* buff, int buffLen);
    void EvtSPIUpdateSLS(unsigned char* buff, int buffLen, int value, unsigned char framepostion, int totalLen);
    void EvtSPIUpdateEPG(unsigned char* buff, int buffLen);
    void EvtSPIUpdateTPEG(unsigned char* buff, int buffLen);
    void EvtDataInfo(unsigned char* buff, int buffLen);
    void EvtSPIUpdateStationLogo(unsigned char* buff, int buffLen, unsigned char framepostion);
    QString ebuLatinToUnicode(unsigned char* latinChar, int strlLength);

signals:
    void sendChannelItem(quint32 freq,
                         quint16 eId,
                         quint8 iId,
                         QString eLabel,
                         quint8 sCount,
                         quint32 sID,
                         quint8 sType,
                         quint8 subChId,
                         quint16 bitrate,
                         quint8 ps,
                         quint8 charset,
                         quint8 pty,
                         quint8 language,
                         QString sLabel,
                         stASU_INFO asuInfo,
                         stPICODE_INFO picodeInfo,
                         quint8 subCHSize,
                         quint16 address
                         );
    void cmdAddEPGItem(quint32 mjd, quint32 serviceID, int Hour, int Minute, int Second, int Duration, QString ProgramLabel, QString Description);
    void rspGetVersion(quint8 booloaderMajorVersion, quint8 bootloaderMinorVersion, quint8 kernelMajor, quint8 kernelMinor, quint8 appMajor, quint8 appMinor, quint8 appMicro, QString buildTime, quint8 bootCount);
    void rspSetSeekService(eREQ_SET_FULL_SCAN_BAND band, quint8 direction, quint8 FilterType, quint8 FilterValue, quint32 Frequency, quint32 ServiceID, quint8 SubChId);
    void rspSetSelectService(quint32 frequency, quint8 serviceType, quint32 serviceID, quint8 subChId, quint16 bitrate, QString label);
    void rspSetAnnouncementFlag(quint16 flag);
    void rspGetServiceList();
    void rspGetDLS(QString sDLS, QString sTitle, QString sAlbum, QString sArtist);
    void rspGetSLS(QString fileName);
    void evtSendAnnouncementTAInvalid(quint16 flags);
    void removeDuplicatEPGData(quint32 sID, quint32 mjd);
    void rspGetEPG(quint32 mjd, quint32 sID);
    void evtSendModuleStatus(eEVT_SEND_MODULE_STATUS m, eEVT_SEND_SERVICE_STATUS s);
    void evtSendChangeService(int freq, int serviceType, int serviceID, int SubChId, int Bitrate, int Pty, QString label, QString ensembleLabel,eEVT_SEND_CHANGE_SERVICE_EVENT event);

    void evtSendAnnouncement(QString label, int flag, eEVT_SEND_ANNOUNCEMENT_STATUS status, quint8 subChID);
    void evtSendTime(int year, int month, int day, int hour, int minute, int second);
    void evtSendQOS(int CER, int SNR, int RSSI, int CER_sub, int SNR_sub, int RSSI_sub);
    void evtUpdateDLS(quint8 update);
    void evtUpdateSLS();
    void evtUpdateEPG();
    void evtSendSignalInfo(quint8 status);
    void evtServiceFollowingState(quint8 state);
    void evtSendPinkStatus(quint8 status);
    void evtAutoScanInfo(quint8 status, quint32 frequency, quint8 lock);
    void cmdAddStationLogo(QString);
    void cmdAudioInfo(quint8 ProtectType, quint8 ProtectLevel, quint8 ProtectOption);
#ifdef __DTC__
    void evtSendDtcUart();
#endif

#ifdef __DAB_TPEG__
    void cmdRspTPEGData(const QByteArray &data);
#endif

public slots:
    void onProcessCommand(unsigned char type, unsigned char command, QByteArray bArray, int dataLen);
    void onProcessCommand(unsigned char type, unsigned char command, unsigned char *pData, int dataLen);
    void onSPIProcessCommand(unsigned char type, unsigned char command, unsigned char framepostion, QByteArray bArray, int dataLen, int totalLen);
    void onSPIProcessCommand(unsigned char type, unsigned char command, unsigned char framepostion, unsigned char *pData, int dataLen, int totalLen);
    void onUpdateOutputText();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onProcessError(QProcess::ProcessError error);
    void sendBackupCommandFromRebooting(void);
    void onReqSetAnnounceDelay(quint32 time);
    void onReqSetAnnounceTimeout(quint32 time);
    void onReqSetAnnounceWeakTimeout(quint32 time);
    void onReqSetServLinkTimeout(quint32 time);

private:
    QProcess process;

    enum PlaySvcState {
        PlaySvcNone = 0,
        PlaySvcReq,
        PlaySvcRsp,
        PlaySvcReceiving,
        PlaySvcPlaying
    };

    enum ModuleStatus {
        ModBooting = 0,
        ModIdle,
        ModReady,
        ModWaiting,
        ModServiceGood,
        ModServiceWeak,
        ModScanning,
        ModScanStop,
        ModScanEnd
    };

    enum ScanMode {
        StartScan = 0,
        StopScan
    };

    DABCommReceiverThread* pCommReceiverThread;
    DABCommDataHandler* pCommDataHandler;
    DABCommSPIPacketInfo* pSPIPacketInfo;
    ModuleStatus ModState;    
    PlaySvcState BState;
    ScanMode ScanState;

    DABCommandDataBackup m_xDataBackup;

    int writeCmd(quint8* cmdBuf, int cmdLen);
    void copyLabelBufferAndRemoveSpace(char* pDest, int destSize, const char* pSrc, int srcSize);
};

#endif // DABCOMMMANAGER_H
