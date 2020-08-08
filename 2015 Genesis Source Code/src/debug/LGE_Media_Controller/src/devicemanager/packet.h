#ifndef PACKET_H
#define PACKET_H

#include <QObject>
#include <QThread>
#include "type.h"
#include "deck_def.h"
#include "messageserialport.h"
#include "tab2protocol.h"

#define DM_USE_DEBUG_PRINT              0

#define MAX_SERAIL_BUFFER               512
#define MAX_COMMAND_DATA_SIZE           32

class Packet : public QObject
{
    Q_OBJECT
public:
    static Packet*  GetInstace();
    static void     DeleteInstace();

    virtual     ~Packet();

    int         Open();
    int         Close();

    int         Play();
    int         Stop();
    int         Pause();
    int         Eject(bool bForced);
    int         Loading();
    int         FunctionStop();
    int         TrackUp(int nTrackUpMode);
    int         TrackDown(int nTrackDownMode);
    int         FastUp(int nFastUpMode);
    int         FastDown(int nFastDownMode);
    int         BusSwitching();
    int         Release();
    int         DirectCont(DirectContCommand *pCommand);
    int         DirectStop(DirectStopCommand *pCommand);
    int         DiscDetection();
    int         RandomOn(int nRandomMode);
    int         RandomOff();
    int         RepeatOn(int nRepeatMode);
    int         RepeatOff();
    int         ScanOn(int nScanMode);
    int         ScanOff();
    int         PlayModeSwitching(DECK_MIXED_CD_MODE nSwitchMode);
    int         ResumeInitialization();
    int         SeekDataForCDROMPlay(int nMode, int nStartMin, int nStartSec, int nStartBlock, int nEndMin, int nEndSec, int nEndBlock);
    int         SlowForward(int nMode);
    int         SlowBackward(int nMode);
    int         Subtitle();
    int         Audio();
    int         Angle();
    int         FrameAdvance();
    int         FrameBack();
    int         TimeSearch(int nMediaMode, int nHour, int nMinute, int nSecond);
    int         SearchCalling();
    int         StillOff();
    int         PageUp();
    int         PageDown();
    int         SubtitleOn();
    int         SubTitleOff();
    int         TopMenu(DECK_TOP_MNEU_SETTING nSetting);
    int         TitleMenu(DECK_TITLE_MENU_SETTING nSetting);
    int         Enter();
    int         Return();
    int         Cursor(int nMode);
    int         Key10(int nOper1, int nOper2);
    int         PictureSetting(int nContrast, int nColor, int nTint, int nBright, int nSharp, int nGamma, int nCoring, int nBlack, int nWhite);
    int         InitializationSetting1(InitialSetting1Command *pCommand);
    int         InitializationSetting2a(InitialSetting2aCommand *pCommand);
    int         InitializationSetting3(InitialSetting3Command *pCommand);
    int         ParentalChangePermission(DECK_PERMISSION_FLAG nPermission);
    int         BonusGroupPINNumber(BonusGroupPinNumberCommand *pCommand);
    int         TestSP(int nMode);
    int         DirectTouch(int nSelect, int nX, int nY);
    int         DualMonoChange();
    int         DirectStopSpeedSetting(DirectStopSpeedSettingCommand *pCommand);
    int         LowConsumptionModeOn();
    int         LowConsumptionModeOff();
    int         ResetStart();
    int         AccOn();
    
    int         RequestTOCStatus(int bSync = false);
    int         RequestTOCStatus2(int nBlockNum, int bSync = false);
    int         RequestDirTreeStatus(DirTreeCommand *pCommand, int bSync = false);
    int         RequestTagStatus(int bSync = false);
    int         RequestFileNameStatus(FileNameCommand *pCommand, int bSync = false);
    int         RequestFolderNameStatus(FolderNameCommand *pCommand, int bSync = false);
    int         RequestCDText(CDTextCommand *pCommand, int bSync = false);
    int         RequestCDTextInfo(int bSync = false);
    int         RequestFolderFile4Status(FolderFile4Command *pCommand, int bSync = false);
    int         RequestDVDVRTitle(DVDVRTitleCommand *pCommand, int bSync = false);
    int         RequestDVDVRInfor(int bSync = false);
    int         RequestPictureSettingStatus(int bSync = false);
    int         RequestCommonStatus(int bSync = false);
    int         RequestDVDAudioVideoVRStatus(int bSync = false);
    int         RequestCDStatus(int bSync = false);
    int         RequestVideoCDStatus(int bSync = false);
    int         RequestCompAudioStatus(int bSync = false);
    int         RequestInitialSetting1Status(int bSync = false);
    int         RequestInitialSetting2aStatus(int bSync = false);
    int         RequestInitialSetting3Status(int bSync = false);
    int         RequestFileFolderTotalNum(int bSync = false);
    int         RequestTagStatus2(int nOutputType, int bSync = false);

private slots:
    void        HandleAboutToClose();
    void        HandleBytesWritten(qint64 bytes);
    void        HandleReadChannelFinished();
    void        HandleDataReceived();
    
private:
    Packet(QObject *parent = 0);

    int         ReadPacket(unsigned char *pBuffer, int nMaxSize);
    int         WritePacket(unsigned char *pBuffer, int nMaxSize);
    int         WriteNReadPacket(unsigned char *pBuffer, int nMaxSize);

    void        PrintPacket(int bWrite, unsigned char *pBuffer, int nMaxSize);

    static Packet       *m_pInstance;
    
    MessageSerialPort   *m_pProtocol;
    unsigned char       *m_pPacketBuff;
    int                  m_nPacketSize;
    StatusData          *m_pStatusData;
    int                  m_nCommandCode; // added by cychoi 2013.06.26 for ITS 176335

public:
    int                  m_nCount;
};


#endif
