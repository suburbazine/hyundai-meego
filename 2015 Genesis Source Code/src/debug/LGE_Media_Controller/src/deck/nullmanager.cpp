#include "nullmanager.h"
#include "deckmanager.h"
#include "mediamanager.h"
#include "util.h"

NullManager* NullManager::m_pInstance = 0;

NullManager* NullManager::GetInstance()
{
    if(m_pInstance == 0) {
        m_pInstance = new NullManager();
    }
    
    return m_pInstance;
}

NullManager::~NullManager()
{
    MC_LOG << "~NullManager()" << LOG_ENDL;
}

void NullManager::Init()
{
    MC_LOG << "NullManager::Init" << LOG_ENDL;
}

void NullManager::StartMedia()
{   
}

void NullManager::EndMedia()
{
}

void NullManager::ProcessPacket(StatusData *pPacket)
{
    Q_UNUSED(pPacket);
}

int NullManager::ChangeMediaMode(MEDIA_PLAYER_MODE eMode)
{
    MediaManager::GetInstance()->SetMediaMode(MP_MODE_DISC, eMode);
    return true;
}

QString NullManager::GetCurrentFileName()
{
    return QString();
}

int NullManager::GetCurrentIndex()
{
    return 0;
}

void NullManager::SetCurrentIndex(int nIndex)
{
    Q_UNUSED(nIndex);
}

int NullManager::GetCurrentMediaState()
{
    return PLAYER_STOP;
}

int NullManager::GetCurrentMediaDuration()
{
    return 0;
}

int NullManager::GetCurrentMediaPosition()
{
    return 0;
}

void NullManager::SetCurrentMediaPosition(int position)
{
    Q_UNUSED(position);
}

int NullManager::GetDeviceState()
{
    DeckManager *p = DeckManager::GetInstance();
    
    return p->GetDeviceState();
}

void NullManager::GetCurrentMediaInfo(MediaInfo *pMediaInfo)
{
    if(pMediaInfo) {
    }
}

void NullManager::GetDeviceInfo(DeviceInfo *pDeviceInfo)
{
    Q_UNUSED(pDeviceInfo);
}

void NullManager::GetPlaylist(PlayList *pPlaylist)
{
    Q_UNUSED(pPlaylist);
}

void NullManager::SetPlayList(PlayList *pPlaylist)
{
    Q_UNUSED(pPlaylist);
}

bool NullManager::SeekableMedia()
{
    return true;
}

void NullManager::SetBrightness(double fBrightness)
{
    Q_UNUSED(fBrightness);
}

void NullManager::SetContrast(double fContrast)
{
    Q_UNUSED(fContrast);
}

void NullManager::SetTint(double fTint)
{
    Q_UNUSED(fTint);
}

void NullManager::SetBrightness(MEDIA_DISPLAY displayPlane, double fBrightness)
{
    Q_UNUSED(displayPlane);
    Q_UNUSED(fBrightness);
}

void NullManager::SetContrast(MEDIA_DISPLAY displayPlane, double fContrast)
{
    Q_UNUSED(displayPlane);
    Q_UNUSED(fContrast);
}

void NullManager::SetTint(MEDIA_DISPLAY displayPlane, double fTint)
{
    Q_UNUSED(displayPlane);
    Q_UNUSED(fTint);
}

double NullManager::GetBrightness()
{
    return 0.0;
}

double NullManager::GetContrast()
{
    return 0.0;
}

double NullManager::GetTint()
{
    return 0.0;
}

void NullManager::SetSystemState(int nSystemState)
{
    Q_UNUSED(nSystemState);
    m_nSystemState = nSystemState;
}

QWidget* NullManager::GetVideoWidget()
{
    return DeckManager::GetInstance()->GetVideoWidget();
}

void NullManager::EnableSubTitle(bool bEnable)
{
    Q_UNUSED(bEnable);
}

void NullManager::SetPlayerEngine(PLAYER_ENGINE_TYPE eType)
{
    Q_UNUSED(eType);
}

void NullManager::SetFontSize(const int fontSize)
{
    Q_UNUSED(fontSize);
}

bool NullManager::ClearDisplay(bool isBackground)
{
    Q_UNUSED(isBackground);
    return false;
}

void NullManager::SetHue(double value)
{
    Q_UNUSED(value);
}

void NullManager::SetHue(MEDIA_DISPLAY displayPlane, double value)
{
    Q_UNUSED(displayPlane);
    Q_UNUSED(value);
}

double NullManager::GetHue()
{
    return 0.0;
}

void NullManager::GetCurrentFileFolderIndex(int *pFileIdx, int *pTotalFiles)
{
    Q_UNUSED(pFileIdx);
    Q_UNUSED(pTotalFiles);
}

void NullManager::AudioDragStart()
{
    
}

void NullManager::AudioDragEnd()
{
    
}

void NullManager::Load()
{
    if(DeckManager::GetInstance()->IsExistCDInDeck() == true)
    {
        MC_LOG << "NullManager::Load()" << LOG_ENDL;
        Packet::GetInstace()->Loading();
    }
}

void NullManager::Eject(uint mode)
{
    if(DeckManager::GetInstance()->IsExistCDInDeck() == true || mode == 1)
    {
        MC_HIGH << "NullManager::Eject(" << mode << ")" << LOG_ENDL;
        DeckManager::GetInstance()->SetEject(true);
        DeckManager::GetInstance()->Eject(mode);
        Init();
        Packet::GetInstace()->RequestCommonStatus();
    }
}

void NullManager::EnterKey()
{
    
}

void NullManager::CursorKey(uint direction)
{
    Q_UNUSED(direction);
}

void NullManager::ReturnKey()
{
    
}

void NullManager::SwitchPlayMode(bool mode)
{
    Q_UNUSED(mode);
}

int NullManager::RequestTOC()
{
    return -1;
}

int NullManager::RequestTOC2(int block_no)
{
    Q_UNUSED(block_no);

    return -1;
}

int NullManager::RequestGraceNoteTOC()
{
    return -1;
}

void NullManager::TenKey(int nKeyID, int pressStatus)
{
    Q_UNUSED(nKeyID);
    Q_UNUSED(pressStatus);
}


int NullManager::RequestCDText(int track_no, int data_type, int block_no, int frame_no)
{
    Q_UNUSED(track_no);
    Q_UNUSED(data_type);
    Q_UNUSED(block_no);
    Q_UNUSED(frame_no);

    return -1;
}

int NullManager::RequestCDTextInfo()
{   
    return -1;
}

int NullManager::RequestDirTree(unsigned int mode, unsigned int folder_no, unsigned int block_no)
{
    Q_UNUSED(mode);
    Q_UNUSED(folder_no);
    Q_UNUSED(block_no);

    return -1;
}

int NullManager::RequestTag()
{
    return -1;
}

int NullManager::RequestTag2(int info_type)
{
    Q_UNUSED(info_type);

    return -1;
}

int NullManager::RequestAllTagInfo()
{
    return -1;
}

int NullManager::RequestFileName(unsigned int nFolderNo, unsigned int nFileNo)
{
    Q_UNUSED(nFolderNo);
    Q_UNUSED(nFileNo);
    
    return -1;
}

int NullManager::RequestFolderName(unsigned int nFolderNo)
{
    Q_UNUSED(nFolderNo);
        
    return -1;
}

int NullManager::GetTotalFileFolder(TotalFileFolderCount *pCount)
{
    Q_UNUSED(pCount);

    return -1;
}

int NullManager::GetCDStatus(CDDAStatusResponse *pCDStatus)
{
    Q_UNUSED(pCDStatus);

    return -1;
}

int NullManager::GetCAStatus(CAStatusResponse *pCAStatus)
{
    Q_UNUSED(pCAStatus);

    return -1;
}

void NullManager::Play()
{

}

void NullManager::Pause()
{

}

void NullManager::Stop()
{

}

void NullManager::FunctionStop()
{
    
}

void NullManager::Next(int nSkipLevel)
{
    Q_UNUSED(nSkipLevel);
}

void NullManager::Prev(int nSkipLevel, int nForced)
{
    Q_UNUSED(nSkipLevel);
    Q_UNUSED(nForced);
}

void NullManager::Scan(int nMode)
{
    Q_UNUSED(nMode);
}

void NullManager::RandomPlay(int nMode, int bPlay)
{
    Q_UNUSED(nMode);
    Q_UNUSED(bPlay);
}

void NullManager::RepeatPlay(int nMode, int bPlay)
{
    Q_UNUSED(nMode);  
    Q_UNUSED(bPlay);
}

void NullManager::FastForward(int nRate)
{
    Q_UNUSED(nRate);
}

void NullManager::Rewind(int nRate)
{
    Q_UNUSED(nRate);
}

void NullManager::NextFolder(PlayList *pPlayList)
{
    Q_UNUSED(pPlayList);
}

void NullManager::PrevFolder(PlayList *pPlayList)
{
    Q_UNUSED(pPlayList);
}

NullManager::NullManager(QObject *parent) : QObject(parent),
m_nSystemState(0)    
{
}
