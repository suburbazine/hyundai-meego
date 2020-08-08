#ifndef NULLMANAGER_H
#define NULLMANAGER_H

#include <QObject>
#include "imediainterface.h"
#include "iaudiointerface.h"
#include "icdinterface.h"
#include "packet.h"

class NullManager : public QObject, public ICDInterface, public IMediaInterface, public IAudioInterface
{
    Q_OBJECT
public:
    static NullManager* GetInstance();
    
    virtual        ~NullManager();

    void            Init();
    void            StartMedia();
    void            EndMedia();
    void            ProcessPacket(StatusData *pPacket);
    
    // FOR IMediaInterface
    int             ChangeMediaMode(MEDIA_PLAYER_MODE eMode);
    QString         GetCurrentFileName();
    int             GetCurrentIndex();
    void            SetCurrentIndex(int nIndex);
    int             GetCurrentMediaState();
    int             GetCurrentMediaDuration();
    int             GetCurrentMediaPosition();
    void            SetCurrentMediaPosition(int position);
    int             GetDeviceState();

    void            GetCurrentMediaInfo(MediaInfo *pMediaInfo);
    void            GetDeviceInfo(DeviceInfo *pDeviceInfo);
    void            GetPlaylist(PlayList *pPlaylist);

    void            SetPlayList(PlayList *pPlayList);
    bool            SeekableMedia();
    void            SetBrightness(double fBrightness);
    void            SetContrast(double fContrast);
    void            SetTint(double fTint);
    void            SetBrightness(MEDIA_DISPLAY displayPlane, double fBrightness);
    void            SetContrast(MEDIA_DISPLAY displayPlane, double fContrast);
    void            SetTint(MEDIA_DISPLAY displayPlane, double fTint);
    double          GetBrightness();
    double          GetContrast();
    double          GetTint();
    void            SetSystemState(int nSystemState);
    QWidget*        GetVideoWidget();
    void            EnableSubTitle(bool bEnable);
    void            SetPlayerEngine(PLAYER_ENGINE_TYPE eType);
    void            SetFontSize(const int fontSize);
    bool            ClearDisplay(bool isBackground);
    void            SetHue(double value);
    void            SetHue(MEDIA_DISPLAY displayPlane, double value);
    double          GetHue();
    void            GetCurrentFileFolderIndex(int *pFileIdx, int *pTotalFiles);
    void            AudioDragStart();
    void            AudioDragEnd();    

    // FOR IAudioInteface
    void            Load();
    void            Eject(uint mode);
    void            EnterKey();
    void            CursorKey(uint direction);
    void            ReturnKey();
    void            SwitchPlayMode(bool mode);
    int             RequestTOC();
    int             RequestTOC2(int block_no);
    int             RequestGraceNoteTOC();
    void            TenKey(int nKeyID, int pressStatus);
    int             RequestCDText(int track_no, int data_type, int block_no, int frame_no);
    int             RequestCDTextInfo();
    int             RequestDirTree(unsigned int mode, unsigned int folder_no, unsigned int block_no);
    int             RequestTag();
    int             RequestTag2(int info_type);
    int             RequestAllTagInfo();
    int             RequestFileName(unsigned int nFolderNo, unsigned int nFileNo);
    int             RequestFolderName(unsigned int nFolderNo);
    int             GetTotalFileFolder(TotalFileFolderCount *pCount);   
    int             GetCDStatus(CDDAStatusResponse *pCDStatus);
    int             GetCAStatus(CAStatusResponse *pCAStatus);

public slots:    
    void            Play();
    void            Pause();
    void            FunctionStop();
    void            Stop();
    void            Next(int nSkipLevel);
    void            Prev(int nSkipLevel, int nForced = 0);
    void            Scan(int nMode);
    void            RandomPlay(int nMode, int bPlay = true);
    void            RepeatPlay(int nMode, int bPlay = true);
    void            FastForward(int nRate);
    void            Rewind(int nRate);
    void            NextFolder(PlayList *pPlayList);
    void            PrevFolder(PlayList *pPlayList);    
    
private:
    NullManager(QObject *parent = 0);

    static NullManager      *m_pInstance;
    int                      m_nSystemState;
};

#endif // NULLMANAGER_H
