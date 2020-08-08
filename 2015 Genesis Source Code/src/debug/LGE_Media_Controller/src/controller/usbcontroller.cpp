/*
*******************************************************************
* COPYRIGHT  LGE AB *
 *----------------------------------------------------------------*
 * MODULE     :  USBController                                    *
 *  
 * PROGRAMMERS : Jungae, Kim                                      *
 * DATE       :  8 August 2011                                    *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                    USB Controller                              *
 *                                                                *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 ******************************************************************
 */

#include "usbcontroller.h"
#include <QVariant>
#include <QDBusObjectPath>
#include <QtDBus/QDBusReply>
#include "gplayerengine.h"
#include "mplayerengine.h"
#include "common.h"
#include "filemanager.h"
#include "mediamanager.h"
#include "performance.h"
#include "util.h"

#define USB_MAX_SHUFFLE_LIST_SIZE           1024

#define USE_PE_MUTE_FUNCTION                0

#define USER_REQ_TRACK_NEXT_INTERVAL        700    //Modified for ITS_0179426 black screen shown for long time > modified by sungha.choi 2013.08.16 (from 300 to 700)
#define USER_REQ_TRACK_PLAY_INTERVAL        1000

#if USE_USB_SINGLE_INSTANCE
USBController* USBController::m_pInstance = 0;
#else
USBController* USBController::m_pFirstInstance = 0;
USBController* USBController::m_pSecondInstance = 0;
USBController* USBController::m_pThirdInstance = 0;
#endif

#if USE_USB_SINGLE_INSTANCE
USBController* USBController::GetInstance()
{
    if(m_pInstance == 0)
    {
        PT_START_TIME(PT_OBJ_NAME_0, 0);
        m_pInstance = new USBController(MP_MODE_USB);
        m_pInstance->SetMyMediaMode(MP_MODE_USB);
        PT_END_TIME(PT_OBJ_NAME_0, "USBController::GetInstance()");
    }

    return m_pInstance;
}
#else
USBController* USBController::GetFirstInstance()
{
    if(m_pFirstInstance == 0)
    {
        PT_START_TIME(PT_OBJ_NAME_0, 0);
        m_pFirstInstance = new USBController(MP_MODE_USB);
        m_pFirstInstance->SetMyMediaMode(MP_MODE_USB);
        PT_END_TIME(PT_OBJ_NAME_0, "USBController::GetFirstInstance()");
    }

    return m_pFirstInstance;
}

USBController* USBController::GetSecondInstance()
{
    if(m_pSecondInstance == 0)
    {
        PT_START_TIME(PT_OBJ_NAME_0, 0);
        m_pSecondInstance = new USBController(MP_MODE_USB);
        m_pSecondInstance->SetMyMediaMode(MP_MODE_USB);
        PT_END_TIME(PT_OBJ_NAME_0, "USBController::GetSecondInstance");
    }

    return m_pSecondInstance;
}

USBController* USBController::GetThirdInstance()
{
    if(m_pThirdInstance == 0)
    {
        PT_START_TIME(PT_OBJ_NAME_0, 0);
        m_pThirdInstance = new USBController(MP_MODE_USB);
        m_pThirdInstance->SetMyMediaMode(MP_MODE_USB);
        PT_END_TIME(PT_OBJ_NAME_0, "USBController::GetThirdInstance");
    }

    return m_pThirdInstance;
}
#endif

/*
*******************************************************************
* Function: ~USBController()
*
* Description : destructors of USB Controller
*
* Parameters : None
*
* Returns : None
*
*******************************************************************
*/
USBController::~USBController()
{
    MC_HIGH << LOG_ENDL;
    
    if(m_pQDBus)
        delete m_pQDBus;
    
    if(m_pShuffleList)
        free(m_pShuffleList);

    #if (USE_USB_SINGLE_INSTANCE == 0)
    if(m_pGPlayer)
        delete m_pGPlayer;
    if(m_pMPlayer)
        delete m_pMPlayer;
    #endif
}

/*
*******************************************************************
* Function: GetMediaInstance()
*
* Description : get instance for media_interface
*
* Parameters : None
*
* Returns : pointer type of IMediaInterface(current instance)
*
*******************************************************************
*/
IMediaInterface* USBController::GetMediaInstance()
{
    return this;
}

/*
*******************************************************************
* Function: GetViewerInstance()
*
* Description : get instance for viewer_interface
*
* Parameters : None
*
* Returns : pointer type of IViewerInterface(current instance)
*
*******************************************************************
*/
IViewerInterface* USBController::GetViewerInstance()
{
    return this;
}

/*
*******************************************************************
* Function: GetFileInstance()
*
* Description : get instance for file_interface
*
* Parameters : None
*
* Returns : pointer type of IFileInterface(current instance)
*
*******************************************************************
*/
IFileInterface* USBController::GetFileInstance()
{
    return this;
}

int USBController::ChangeMediaMode(MEDIA_PLAYER_MODE eMode)
{
    m_bControllerChanged = true;
    MediaManager::GetInstance()->SetMediaMode(m_eMyMediaMode, eMode);

    if(m_pPlayer)
    {
        if(m_pPlayer->getPipeline() == false)
        {
            m_currentMediaState = PLAYER_STOP;
        }
    }
    return true;
}

/*
*******************************************************************
* Function: GetCurrentFileName()
*
* Description : It will give the name of the current playing media file
*                   media interface related functions
*
* Parameters : None
*
* Returns : current file name of QString type
*
*******************************************************************
*/
QString USBController::GetCurrentFileName()
{
    if(m_tPlayList.isEmpty())
    {
        return QString();
    }
    else
    {
        return (QString)(m_tPlayList.at(GetTrackIdx()));
    }
}

/*
*******************************************************************
* Function: GetCurrentIndex()
*
* Description : It will give the index of current playing media file in the 
*                   Playlist.
*                   media interface related functions
*
* Parameters : None
*
* Returns : current media index of integer type
*
*******************************************************************
*/
int USBController::GetCurrentIndex()
{
    return GetTrackIdx();
}

/*
*******************************************************************
* Function: SetCurrentIndex()
*
* Description : It will set the index of the Playlist for which media file
*                   to play.
*                   media interface related functions
*
* Parameters : int nIndex
*
* Returns : void
*
*******************************************************************
*/
void USBController::SetCurrentIndex(int nIndex)
{
    if(nIndex >= 0 && nIndex < m_tPlayList.count())
    {
        m_nCurrTrackIndex = nIndex;
    }
    else
    {
        MC_HIGH << "Error nIndex:" << nIndex << m_tPlayList.count() << LOG_ENDL;
    }
}

/*
*******************************************************************
* Function: GetCurrentMediaState()
*
* Description : It will give the current state of the player(play,pause
*                   ,stop etc).
*                   media interface related functions
*
* Parameters : None
*
* Returns : current media status of integer type
*
*******************************************************************
*/
int USBController::GetCurrentMediaState()
{
    if(m_pPlayer && m_pPlayer->getPipeline() == false)
    {
        return PLAYER_STOP;       
    }
    else
    {
        return m_currentMediaState;
    }
}

/*
*******************************************************************
* Function: GetCurrentMediaDuration()
*
* Description : It will give the total time duration of the current playing
*                   media track.
*                   media interface related functions
*
* Parameters : None
*
* Returns : current media duration of integer type
*
*******************************************************************
*/
int USBController::GetCurrentMediaDuration()
{
    m_currentMediaDuration = m_pPlayer->GetDuration();
    
    return m_currentMediaDuration;
}

/*
*******************************************************************
* Function: GetCurrentMediaPosition()
*
* Description : It will give the current position of the current media track
*                   media interface related functions
*
* Parameters : None
*
* Returns : current media position of integer type
*
*******************************************************************
*/
int USBController::GetCurrentMediaPosition()
{
    // m_currentMediaPosition = m_pPlayer->GetPosition(); // removed by kihyung 2013.12.10 for ITS 0214009 
 
    return (int)m_currentMediaPosition;
}

/*
*******************************************************************
* Function: SetCurrentMediaPosition()
*
* Description : It will set the position of the current media file to specific
*                   position. It will be used in case of seeking the media file
*                   media interface related functions
*
* Parameters : int position
*
* Returns : void
*
*******************************************************************
*/
void USBController::SetCurrentMediaPosition(int position)
{
    MC_HIGH << "cursorMA" << position << GetCurrentMediaState() << m_bChangedDuration << LOG_ENDL;
    
    if(position < 0)
    {
        return;
    }

    // added by kihyung 2013.08.29 for ITS 0186278
    if(m_bChangedDuration == false)
    {
        return;
    }

    // added by sangmin.seol 2014.02.26 for ITS 0222963
    if(GetCurrentMediaState() == PLAYER_CHANGE_PLAY_ICON)
    {
        return;
    }
    
    if(GetCurrentMediaState() == PLAYER_STOP)
    {
        m_nUserPlayPosition = position;
    }
    else
    {
        m_pPlayer->SetPosition(position);
    }
}

/*
*******************************************************************
* Function: GetDeviceState()
*
* Description : It will give the current device Error code or it's state
*                   media interface related functions
*
* Parameters : None
*
* Returns : current device status of integer type
*
*******************************************************************
*/
int USBController::GetDeviceState()
{
#if 0    
    if(m_nDeviceMountAtBoot == 1) {
        MC_LOG << "GetDeviceState():USB was not mount in boot time" << LOG_ENDL;
        int nMountCount = FindAttachedDevices();
        if(nMountCount > 0) {
            m_currentDeviceState = USB_CONNECTED;
        }
        MC_LOG << "GetDeviceState(): FindAttachedDevices() = " <<  nMountCount << LOG_ENDL;
    }
#endif    

    return m_currentDeviceState;
}

/*
*******************************************************************
* Function: GetCurrentMediaInfo()
*
* Description : It will set the MediaInfo structure to Media Player App.
*                   to values like file name, different tags, It can be used 
*                   by the CUSBController untill index is not completed by 
*                   the Tracker.
*                   media interface related functions
*
* Parameters : MediaInfo *pMediaInfo
*
* Returns : void
*
*******************************************************************
*/
void USBController::GetCurrentMediaInfo(MediaInfo *pMediaInfo)
{
    m_pPlayer->GetCurrentMediaInfo(pMediaInfo);
}

/*
*******************************************************************
* Function: GetDeviceInfo()
*
* Description : It will set the structure of the DeviceInfo.
*                   media interface related functions
*
* Parameters : DeviceInfo *pDeviceInfo
*
* Returns : void
*
*******************************************************************
*/
void USBController::GetDeviceInfo(DeviceInfo *pDeviceInfo)
{
    if(pDeviceInfo)
    {
        pDeviceInfo->ObjectPathList.clear();
        pDeviceInfo->MountNameList.clear();
        pDeviceInfo->SerialNumberList.clear();

        for(int i = 0; i < m_tUSBSerialNumber.count(); i++)
        {
            pDeviceInfo->ObjectPathList     += m_tUSBObjectPath.at(i);
            pDeviceInfo->MountNameList      += m_tUSBMountPath.at(i);
            pDeviceInfo->SerialNumberList   += m_tUSBSerialNumber.at(i);
        }
    }
}

/*
*******************************************************************
* Function: GetPlaylist()
*
* Description : It will get the PlayList structure.
*                   media interface related functions
*
* Parameters : PlayList *pPlaylist
*
* Returns : void
*
*******************************************************************
*/
void USBController::GetPlaylist(PlayList *pPlaylist)
{
    if(pPlaylist)
    {
        pPlaylist->index        = GetTrackIdx();
        pPlaylist->position     = m_currentMediaPosition;
        pPlaylist->PlaylistName = m_playlistName;
        //pPlaylist->isMLT        = m_bisMLT; //added by jeeeun for MLT play in FF/REW 2012.12.05 // removed by kihyung 2013.07.18 for ITS 0180268
        for(int i = 0; i < m_tPlayList.count(); i++)
        {
            pPlaylist->list->append(m_tPlayList.at(i));
        }
    }
    else
    {        
        MC_HIGH << "pPlaylist NULL pointer error" << LOG_ENDL;
    }
}

/*
*******************************************************************
* Function: SetPlayList()
*
* Description : It will provide the PlayList structure to CUSBController to
*                   set Playlist, index number, mode, etc.
*                   media interface related functions
*
* Parameters : PlayList *pPlaylist
*
* Returns : void
*
*******************************************************************
*/
void USBController::SetPlayList(PlayList *pPlayList)
{
    if(pPlayList == 0)
    {
        MC_HIGH << "pPlayList is 0" << LOG_ENDL;
        return;
    }

    if(pPlayList->list == 0) {
        MC_HIGH << "pPlayList->list is 0" << LOG_ENDL;
        return;
    }

    QString strPrevFileName = GetCurrentFileName(); // added by kihyung 2014.01.06 for ITS 0217528
    
    // SCAN ---> SCAN OFF
    if(m_nScanMode != SCANOFF) {
        MC_HIGH << "m_nScanMode:" << m_nScanMode << LOG_ENDL;
        Scan_Impl(SCANOFF);
    }

    // { removed by kihyung 2013.3.28
    /*
    // Default Repeat Mode
    if(m_nRepeatMode != REPEATALL) {
        m_nRepeatMode = REPEATALL;
    }
    emit OnModeChanged(m_nRepeatMode);

    // RANDOM ---> RANDOM OFF
    if(m_nRandomMode != RANDOMOFF) {
        m_nRandomMode = RANDOMOFF;
    }
    emit OnModeChanged(m_nRandomMode);
    */
    // } removed by kihyung 2013.3.28    
    
    m_tPlayList.clear();

    m_playlistName          = pPlayList->PlaylistName;
    m_nUserPlayPosition     = pPlayList->position;
    m_nPLCategory           = pPlayList->category;
    m_nSetURITrackIndex     = -1;
    // m_bisMLT                = pPlayList->isMLT; //added by jeeeun for MLT play in FF/REW 2012.12.05	// removed by kihyung 2013.07.18 for ITS 0180268
    
    for(int i = 0; i < pPlayList->list->count(); i++)
    {
        m_tPlayList.append(pPlayList->list->at(i));
    }

    SetPlayableList(pPlayList->category);
    
    if(SetTrackIdx(pPlayList->index) == false)
    {
        MC_HIGH << "pPlayList->index:" << pPlayList->index << LOG_ENDL;
    }

    // { added by kihyung 2014.01.06 for ITS 0217528
    //if(GetCurrentMediaState() != PLAYER_STOP && strPrevFileName == GetCurrentFileName())
    if(strPrevFileName == GetCurrentFileName()) //modified by jaehwan.lee 2014.3.21 to fix ITS 229885
    {
        m_bSameSong = true;
    }
    else
    {
        m_bSameSong = false;
    }
    // } added by kihyung 2014.01.06 for ITS 0217528

    if(m_nRandomMode != RANDOMOFF)
    {
        SetShuffleList(false);
    }

    if(m_tPlayList.count() > 0)
    {
        MC_HIGH << "ALL:" << m_tPlayList.count()<< "T:" << GetTrackIdx() << "P:" << m_nUserPlayPosition << LOG_ENDL;
        m_bControllerChanged = true; // for OnTrackChanged();
        // SetURI(GetTrackIdx(), true); // removed by kihyung 2013.3.20 for ISV 76585
    }
    else
    {
        MC_HIGH << "List is empty" << LOG_ENDL;
    }
}

/*
*******************************************************************
* Function: SeekableMedia()
*
* Description : It will return whether media file is seekable or not.
*                   media interface related functions
*
* Parameters : None
*
* Returns : seekable status of boolean type
*
*******************************************************************
*/
bool USBController::SeekableMedia()
{
    return m_pPlayer->IsSeekable();
}

/*
*******************************************************************
* Function: SetBrightness()
*
* Description : Sets the brightness value for the current image. value 
*                   ranges between -1.0 and 1.0. Default value is 0.0.
*                   media interface related functions
*
* Parameters : double fBrightness
*
* Returns : void
*
*******************************************************************
*/
void USBController::SetBrightness(double fBrightness)
{
    MC_HIGH << fBrightness << LOG_ENDL;
    m_pPlayer->SetBrightness(fBrightness);
}

/*
*******************************************************************
* Function: SetContrast()
*
* Description : Sets the contrast value for the current image. value
*                   ranges between 0.0 and 2.0. Default value is 1.0.
*                   media interface related functions
*
* Parameters : double fContrast
*
* Returns : void
*
*******************************************************************
*/
void USBController::SetContrast(double fContrast)
{
    MC_HIGH << fContrast << LOG_ENDL;
    m_pPlayer->SetContrast(fContrast);
}

/*
*******************************************************************
* Function: SetTint()
*
* Description : It will set the value of the Tint of the screen.
*                   media interface related functions
*
* Parameters : double fTint
*
* Returns : void
*
*******************************************************************
*/
void USBController::SetTint(double fTint)
{
    MC_HIGH << fTint << LOG_ENDL;
    m_pPlayer->SetTint(fTint);
}

void USBController::SetBrightness(MEDIA_DISPLAY displayPlane, double fBrightness)
{
    m_pPlayer->SetBrightness(displayPlane, fBrightness);
}

void USBController::SetContrast(MEDIA_DISPLAY displayPlane, double fContrast)
{
    m_pPlayer->SetContrast(displayPlane, fContrast);
}

void USBController::SetTint(MEDIA_DISPLAY displayPlane, double fTint)
{
    m_pPlayer->SetTint(displayPlane, fTint);
}

/*
*******************************************************************
* Function: GetBrightness()
*
* Description :  It will return the value of the brightness of the screen.
*                   media interface related functions
*
* Parameters : None
*
* Returns : brightness value of double type
*
*******************************************************************
*/
double USBController::GetBrightness()
{
    return m_pPlayer->GetBrightness();
}

/*
*******************************************************************
* Function: GetContrast()
*
* Description :  It will return the value of the contrast of the screen.
*                   media interface related functions
*
* Parameters : None
*
* Returns : contrast value of double type
*
*******************************************************************
*/
double USBController::GetContrast()
{
    return m_pPlayer->GetContrast();
}

/*
*******************************************************************
* Function: GetTint()
*
* Description :  It will return the value of the Tint of the screen.
*                   media interface related functions
*
* Parameters : None
*
* Returns : tint value of double type
*
*******************************************************************
*/
double USBController::GetTint()
{
    return m_pPlayer->GetTint();
}

/*
*******************************************************************
* Function: SetSystemState()
*
* Description :  It will set the value of the State of the system like 
*                   AVOFF/ON, PowerOff/ON, ACC OFF/ON. 
*                   media interface related functions
*
* Parameters : int sysState
*
* Returns : void
*
*******************************************************************
*/
void USBController::SetSystemState(int sysState)
{
    switch(sysState)
    {
        case SYSTEM_AV_OFF:
        case SYSTEM_AV_ON:
        case SYSTEM_ACC_OFF:
        case SYSTEM_ACC_ON:
            if(sysState == SYSTEM_ACC_OFF) {
                Stop();
            }
            RandomPlay_Impl(RANDOMOFF);
            RepeatPlay_Impl(REPEATALL);
            break;
        case SYSTEM_POWER_OFF:
            Stop();
            RandomPlay_Impl(RANDOMOFF);
            break;
        case SYSTEM_POWER_ON:
            RandomPlay_Impl(RANDOMOFF);
            break;
        case SYSTEM_SLOW_COPY_SPEED:
            GetFileManager()->SetCopySpeed(FM_SLOW_COPY_SPEED);
            break;
        case SYSTEM_NORMAL_COPY_SPEED:
            // { modified by kihyung 2013.11.11 for ITS 0193747
            //MC_HIGH << "SYSTEM_NORMAL_COPY_SPEED not supported..." << LOG_ENDL;
            // GetFileManager()->SetCopySpeed(FM_NORMAL_COPY_SPEED);
            // } modified by kihyung 2013.11.11 for ITS 0193747            
            break;    
        case SYSTEM_CLEAR_DISPLAY_FG:
            if(m_nVideoType != -1) {
                emit OnVideoInfo(m_nVideoType, m_nVideoStatus, m_nVideoWidth, m_nVideoHeight, m_nVideoPar, m_nVideoDar, GetCurrentFileName());// modified for ITS 224766 2014.02.12
                m_nVideoType = -1;
            }
            break;
        case SYSTEM_USB1_MOUNTED:
            m_bUSB1Connect = true;
            MC_HIGH << "SYSTEM_USB1_MOUNTED" << LOG_ENDL;
            break;
        case SYSTEM_USB2_MOUNTED:
            m_bUSB2Connect = true;
            MC_HIGH << "SYSTEM_USB2_MOUNTED" << LOG_ENDL;
            break;
        case SYSTEM_USB1_REMOVED:
            m_bUSB1Connect = false;
            MC_HIGH << "SYSTEM_USB1_REMOVED" << LOG_ENDL;
            break;
        case SYSTEM_USB2_REMOVED:
            m_bUSB2Connect = false;
            MC_HIGH << "SYSTEM_USB2_REMOVED" << LOG_ENDL;
            break;
        case SYSTEM_DO_NOT_SEND_METAINFO:
            m_bSendMetaInfo = false;
            MC_HIGH << "SYSTEM_DO_NOT_SEND_METAINFO" << LOG_ENDL;
            break;
        case SYSTEM_REQUEST_MUTE:
            m_bSystemRequestMute = true;
            MC_HIGH << "SYSTEM_REQUEST_MUTE" << LOG_ENDL;
            break;
        case SYSTEM_BT_CALL_START:
            m_bBTCallStart = true;
            MC_HIGH << "SYSTEM_BT_CALL_START" << LOG_ENDL;
            break;
        case SYSTEM_BT_CALL_END:
            m_bBTCallStart = false;
            MC_HIGH << "SYSTEM_BT_CALL_END" << LOG_ENDL;
            break;
        default:
            break;
    }
    
    m_systemState = sysState;
}

/*
*******************************************************************
* Function: GetVideoWidget()
*
* Description :  It will provide the QWidget to the Media Application layer. 
*                   media interface related functions
*
* Parameters : None
*
* Returns : video widget of QWidget pointer type
*
*******************************************************************
*/
QWidget* USBController::GetVideoWidget()
{
    return m_pGPlayer->m_pVideoWidget;
}

/*
*******************************************************************
* Function: EnableSubTitle()
*
* Description : It will tell whether subtitle have to be enabled or disabled. 
*                   media interface related functions
*
* Parameters : bool bEnable
*
* Returns : void
*
*******************************************************************
*/
void USBController::EnableSubTitle(bool bEnable)
{
    if(bEnable) 
        MC_LOG << "Enable ShowSubTitle Mode" << LOG_ENDL;
    else
        MC_LOG << "Disable ShowSubTitile Mode" << LOG_ENDL;

    m_pPlayer->ShowSubTitle(bEnable);
}

/*
*******************************************************************
* Function: SetPlayerEngine()
*
* Description : It will set the gplayer engine 
*                   media interface related functions
*
* Parameters : PLAYER_ENGINE_TYPE eType
*
* Returns : void
*
*******************************************************************
*/
void USBController::SetPlayerEngine(PLAYER_ENGINE_TYPE eType)
{
    m_eEngineMode = eType;
}

void USBController::SetFontSize(const int fontSize)
{
    m_pPlayer->SetFontSize(fontSize);
}

bool USBController::ClearDisplay(bool isBackground)
{
    //MC_LOG << "isBG " << isBackground << LOG_ENDL;
    if(isBackground == false)
    {
        m_nClearDisplay = -1;
    }
    return m_pPlayer->ClearDisplay(isBackground);
}

void USBController::SetHue(double value)
{
    m_pPlayer->SetHue(value);
}

void USBController::SetHue(MEDIA_DISPLAY displayPlane, double value)
{
    m_pPlayer->SetHue(displayPlane, value);
}

double USBController::GetHue()
{
    return m_pPlayer->GetHue();
}

void USBController::GetCurrentFileFolderIndex(int *pFileIdx, int *pTotalFiles)
{
    int nTrackIdx;
    int nFileIdx;
    int nFileSize;

#if USE_FILE_INDEX_IN_FOLDER
    if(m_nRandomMode != RANDOMOFF && m_nShuffleIdx >= 0 && m_nShuffleIdx < m_tPlayList.count()) 
    {
        nTrackIdx = m_nShuffleIdx;
    }
    else 
    {
        nTrackIdx = GetTrackIdx();
    }
    
    nFileIdx   = GetFileIdxInFolder(nTrackIdx);
    nFileSize  = GetFileSizeOfFolder(nTrackIdx);
#else
    nTrackIdx  = GetTrackIdx();
    nFileIdx   = nTrackIdx;
    nFileSize  = m_tPlayList.count();
#endif

    if(pFileIdx)
    {
        *pFileIdx = nFileIdx;
    }

    if(pTotalFiles)
    {
        *pTotalFiles = nFileSize;
    }

    //MC_HIGH << nTrackIdx << m_nShuffleIdx << nFileIdx << nFileSize << LOG_ENDL;
}

void USBController::AudioDragStart()
{
    //MC_HIGH << LOG_ENDL;
    m_bAudioDragStart = true;
    m_pPlayer->AudioDragStart();
}

void USBController::AudioDragEnd()
{
    if(m_bAudioDragStart == true)
    {
        //MC_HIGH << LOG_ENDL;
        m_bAudioDragStart = false;
        m_pPlayer->AudioDragEnd();    
    }
    else
    {
        MC_HIGH << "cancel" << LOG_ENDL;
    }
}

void USBController::RequestAudioLanguages()
{
    m_pPlayer->RequestAudioLanguages();
}

void USBController::SetAudioLanguage(int nStreamId)
{
    m_pPlayer->SetCurrentAudioLanguage(nStreamId);
}
// { added by jeeeun 2013-01-07 for multi-audio
int USBController::GetAudioLanguage()
{
    return m_pPlayer->GetCurrentAudioLanguage();
}
// } added by jeeeun 2013-01-07 for multi-audio
bool USBController::IsFileSupported(const QString &strFileName)
{
    return m_pGPlayer->IsFileSupported(strFileName);
}

int USBController::IsFileSupportedAdvanced(const QString &strFileName)
{
    return m_pGPlayer->IsFileSupportedAdvanced(strFileName);
}
// { added by jeeeun 2012-12-20 for is file supported check
bool USBController::getIsFileSupportedCheck() 
{
    return m_pGPlayer->getIsFileSupportedCheck();
}

void USBController::setIsFileSupportedCheck(bool val)
{
	m_pGPlayer->setIsFileSupportedCheck(val);
}
// } added by jeeeun 2012-12-20 for is file supported check
// { added by jeeeun 2012-01-24 for checking subtitle enable or disable
bool USBController::getIsSubtitleEnable()
{
     return m_pGPlayer->getIsSubtitleEnable(); 
}
// } added by jeeeun 2012-01-24 for checking subtitle enable or disable

/*
*******************************************************************
* Function: CreateFolder()
*
* Description : It will create the new folder name with specified name.
*                   file interface related functions
*
* Parameters : const QString &createFolderName
*
* Returns : integer type specify whether the creation of the Folder is 
*              successful or not.
*
*******************************************************************
*/
int USBController::CreateFolder(const QString &createFolderName)
{
    int ret;
    ret = GetFileManager()->CreateFolder(createFolderName);
    return ret;
}

/*
*******************************************************************
* Function: EditedFolder()
*
* Description : It will update the folder name.
*                   file interface related functions
*
* Parameters : const QString &strOldName, const QString &strNewName
*
* Returns : integer type specify whether the editing of the Folder is 
*              successful or not.
*
*******************************************************************
*/
int USBController::EditedFolder(const QString &strOldName, const QString &strNewName)
{
    int ret;
    ret = GetFileManager()->RenameFolder(strOldName, strNewName);
    return ret;
}

int USBController::EditedFile(const QString &strOldName, const QString &strNewName)
{
    int ret;
    ret = GetFileManager()->RenameFile(strOldName, strNewName);
    return ret;
}

/*
*******************************************************************
* Function: DeleteFileFolder()
*
* Description : It will delete the Specific folder/file
*                   file interface related functions
*
* Parameters : const QString &absoluteFolderName
*
* Returns : integer type specify whether the delete of the Folder/File is 
*              successful or not.
*
*******************************************************************
*/
int USBController::DeleteFileFolder(const QString &absoluteFolderName, int typemode, ETAVolumeType volume)
{
    int ret;
    ret = GetFileManager()->DeleteFileFolder(absoluteFolderName, typemode, volume);
    return ret;
}

/*
*******************************************************************
* Function: DeleteFileFolder()
*
* Description : It will Move the File or folder from one space of SSD
*                   to another space of SSD
*                   file interface related functions
*
* Parameters : const QString &absoluteFolderName, const QString &destPath
*
* Returns : integer type specify whether the move of the folder is 
*              successful or not.
*
*******************************************************************
*/
int USBController::MoveFolder(const QString &absoluteFolderName, const QString &destPath)
{
    int ret;
    ret = GetFileManager()->MoveFolder(absoluteFolderName, destPath);
    return ret;
}

/*
*******************************************************************
* Function: CopyFolder()
*
* Description : It will copy the File or folder from one USB  to specified
*                   space or folder of SSD
*                   file interface related functions
*
* Parameters : const QString &absoluteFolderName, const QString &destPat
*
* Returns : integer type specify whether the copy of the folder is 
*              successful or not.
*
*******************************************************************
*/
int USBController::CopyFolder(const QString &absoluteFolderName, const QString &destPath)
{
    int ret;
    ret = GetFileManager()->CopyFolder(absoluteFolderName, destPath, -1, eUSB);
    return ret;
}

/*
*******************************************************************
* Function: CopyFolder()
*
* Description : It will copy the File or folder from one USB  to specified
*                   space or folder of SSD, while parameter typemode define
*                   type of file to be copied.
*                   file interface related functions
*
* Parameters : const QString &absoluteFolderName, const QString &destPat,
*                   int typemode
*
* Returns : integer type specify whether the copy of the folder is 
*              successful or not.
*
*******************************************************************
*/
int USBController::CopyFolder(const QString &absoluteFolderName, const QString &destPath, int typemode, ETAVolumeType volume)
{
    int ret;
    ret = GetFileManager()->CopyFolder(absoluteFolderName, destPath, typemode, volume);
    return ret;
}

int USBController::DeleteFileList(const QStringList &strList, int typemode, ETAVolumeType volume)
{
    int ret;
    ret = GetFileManager()->DeleteFileList(strList, typemode, volume);
    return ret;
}

int USBController::MoveFileList(const QStringList &strList, const QString &destPath)
{
    int ret;
    ret = GetFileManager()->MoveFileList(strList, destPath, -1);
    return ret;
}

int USBController::CopyFileList(const QStringList &strList, const QString &destPath)
{
    int ret;
    ret = GetFileManager()->CopyFileList(strList, destPath, -1, eUSB);
    return ret;
}

int USBController::CopyFileList(const QStringList &strList, const QString &destPath, int typemode, ETAVolumeType volume)
{
    int ret;
    ret = GetFileManager()->CopyFileList(strList, destPath, typemode, volume);
    return ret;
}

/*
*******************************************************************
* Function: TotalFileForCopy()
*
* Description : It will give the total number of file to be copied.
*                   file interface related functions
*
* Parameters : None
*
* Returns : integer type specify whether the copy of the total file is 
*              successful or not.
*
*******************************************************************
*/
int USBController::TotalFileForCopy()
{
    return GetFileManager()->GetTotalFileNum();
}

int USBController::TotalFileForMove()
{
    return GetFileManager()->GetTotalFileNum();
}

int USBController::TotalFileForDeletion()
{
    return GetFileManager()->GetTotalFileNum();
}

qint64 USBController::GetTotalCopySize()
{
    return GetFileManager()->GetTotalFileSpace();
}

qint64 USBController::GetTotalMoveSize()
{
    return GetFileManager()->GetTotalFileSpace();
}

qint64 USBController::GetTotalDeleteSize()
{
    return GetFileManager()->GetTotalFileSpace();
}

qint64 USBController::GetTotalDiscSpace(const QString &strVolumeName)
{
    return GetFileManager()->GetTotalDiscSpace(strVolumeName);
}

qint64 USBController::GetTotalUsedDiscSpace(const QString &strVolumeName)
{
    return GetFileManager()->GetTotalUsedDiscSpace(strVolumeName);
}

int USBController::StartDiscFormat(const QString &strVolumeName)
{
    return GetFileManager()->StartDiscFormat(strVolumeName);
}

int USBController::CancelCopyOperation()
{
    return GetFileManager()->CancelCopyOperation();
}

int USBController::CancelDeleteOperation()
{
    return GetFileManager()->CancelDeleteOperation();
}

int USBController::CancelFormatOperation()
{
    return GetFileManager()->CancelFormatOperation();
}

int USBController::SetOverWriteCurrentExistFileMode(int event)
{
    return GetFileManager()->SetOverWriteCurrentExistFileMode(event);
}

int USBController::ContinueCopying()
{
    return GetFileManager()->ContinueCopying();
}

int USBController::PauseCopying()
{
    return GetFileManager()->PauseCopying();
}

int USBController::CancelMoveOperation()
{
    return CancelCopyOperation();
}

int USBController::ContinueMoving()
{
    return ContinueCopying();
}

int USBController::PauseMoving()
{
    return PauseCopying();
}

/*
*******************************************************************
* Function: GetMediaPlayerInstance()
*
* Description : get instance for media player
*
* Parameters : None
*
* Returns : pointer of QMediaPlayer type
*
*******************************************************************
*/
QMediaPlayer* USBController::GetMediaPlayerInstance()
{
    // return GetMPlayerEngine()->GetMediaPlayer();
    return NULL;
}

void USBController::ConnectSignal()
{
    //Modified by jaehwan.lee to reduce log (2014.07.09).
    /*if(m_eMyMediaMode == MP_MODE_USB)
    {
        MC_HIGH << "USB MODE connect" << LOG_ENDL;
    }
    else
    {
        MC_HIGH << "Jukebox MODE connect" << LOG_ENDL;
    }*/

    if(m_bConnectGSignal == false && m_pGPlayer != 0) 
    {
        //Modified by jaehwan.lee to reduce log (2014.07.09).
        //MC_HIGH << "GPlayerEngine" << LOG_ENDL;
        
        connect(m_pGPlayer, SIGNAL(PositionChanged(int)),                           this, SLOT(HandleUSBPositionChanged(int)));
        connect(m_pGPlayer, SIGNAL(SubtitleChanged(QString)),                       this, SIGNAL(OnSubtitleChanged(QString)));
        connect(m_pGPlayer, SIGNAL(DurationChanged()),                              this, SLOT(HandleUSBDurationChanged()));
        connect(m_pGPlayer, SIGNAL(StateChanged(int)),                              this, SLOT(HandleUSBStateChanged(int)));
        connect(m_pGPlayer, SIGNAL(Error(int)),                                     this, SLOT(HandleUSBError(int)));    
        connect(m_pGPlayer, SIGNAL(ChangeTrack()),                                  this, SLOT(HandleTrackChange())); 
        connect(m_pGPlayer, SIGNAL(CurrentMediaInfo(MediaInfo*)),                   this, SLOT(HandleCurrentMediaInfo(MediaInfo*))); 
        connect(m_pGPlayer, SIGNAL(ImageInfo(int, int)),                            this, SIGNAL(OnPositionImageInfo(int, int)));
        connect(m_pGPlayer, SIGNAL(AudioLanguageList(QStringList, int*, int)),      this, SIGNAL(OnTotalAudioLangReceived(QStringList, int*, int)));
        connect(m_pGPlayer, SIGNAL(TextLanguageList(QStringList, int*, int)),       this, SIGNAL(OnTotalTextLangReceived(QStringList, int*, int)));
        connect(m_pGPlayer, SIGNAL(CurrentTrackDRMStatus(int, int, int)),           this, SIGNAL(OnDRMStatusUpdated(int, int, int))); //modified by jeeeun 2012.11.14
        connect(m_pGPlayer, SIGNAL(OnPrivateSignal(int)),                           this, SIGNAL(OnPrivateSignal(int)));
        connect(m_pGPlayer, SIGNAL(VideoInfo(int, int, int, int, float, float)),    this, SLOT(HandleVideoInfo(int, int, int, int, float, float)));
        connect(m_pGPlayer, SIGNAL(XSubtitleChanged(uchar*, int)),                  this, SIGNAL(HandleXSubtitleChanged(uchar*, int)));
        connect(m_pGPlayer, SIGNAL(RealSoundChanged()),                             this, SLOT(HandleRealSoundChanged()));
        
        m_bConnectGSignal = true;
    }
    
    if(m_bConnectMSignal == false && m_pMPlayer != 0) 
    {
        MC_HIGH << "MPlayerEngine" << LOG_ENDL;
        
        connect(m_pMPlayer, SIGNAL(PositionChanged(int)),                     this, SLOT(HandleUSBPositionChanged(int)));
        connect(m_pMPlayer, SIGNAL(DurationChanged()),                        this, SLOT(HandleUSBDurationChanged()));
        connect(m_pMPlayer, SIGNAL(StateChanged(int)),                        this, SLOT(HandleUSBStateChanged(int)));
        connect(m_pMPlayer, SIGNAL(Error(int)),                               this, SLOT(HandleUSBError(int)));
        connect(m_pMPlayer, SIGNAL(ChangeTrack()),                            this, SLOT(HandleTrackChange()));
        connect(m_pMPlayer, SIGNAL(CurrentMediaInfo(MediaInfo*)),             this, SLOT(HandleCurrentMediaInfo(MediaInfo*))); 
        connect(m_pMPlayer, SIGNAL(CurrentTrackDRMStatus(int, int, int)),     this, SIGNAL(OnDRMStatusUpdated(int, int, int))); // modified by jeeeun 2012.11.14
        
        m_bConnectMSignal = true;
    }
}

bool USBController::isDivXDRM(int *type, int *limitcnt, int *usecount)
{
    bool ret = false;

    if(m_pPlayer)
    {
        ret = m_pPlayer->IsDivXDRMSupported(type , limitcnt , usecount);
    }

    return ret;
}

void USBController::DisconnectSignal()
{
    /*
    if(m_eMyMediaMode == MP_MODE_USB)
    {
        MC_HIGH << "USB MODE disconnect" << LOG_ENDL;
    }
    else
    {
        MC_HIGH << "Jukebox MODE disconnect" << LOG_ENDL;
    }
    */

    if(m_bConnectGSignal == true && m_pGPlayer != 0) 
    {
        MC_HIGH << "GPlayerEngine" << LOG_ENDL;
        
        disconnect(m_pGPlayer, SIGNAL(PositionChanged(int)),                       this, SLOT(HandleUSBPositionChanged(int)));
        disconnect(m_pGPlayer, SIGNAL(SubtitleChanged(QString)),                   this, SIGNAL(OnSubtitleChanged(QString)));
        disconnect(m_pGPlayer, SIGNAL(DurationChanged()),                          this, SLOT(HandleUSBDurationChanged()));
        disconnect(m_pGPlayer, SIGNAL(StateChanged(int)),                          this, SLOT(HandleUSBStateChanged(int)));
        disconnect(m_pGPlayer, SIGNAL(Error(int)),                                 this, SLOT(HandleUSBError(int)));
        disconnect(m_pGPlayer, SIGNAL(ChangeTrack()),                              this, SLOT(HandleTrackChange()));    
        disconnect(m_pGPlayer, SIGNAL(CurrentMediaInfo(MediaInfo*)),               this, SLOT(HandleCurrentMediaInfo(MediaInfo*))); 
        disconnect(m_pGPlayer, SIGNAL(ImageInfo(int, int)),                        this, SIGNAL(OnPositionImageInfo(int, int)));
        disconnect(m_pGPlayer, SIGNAL(AudioLanguageList(QStringList, int*, int)),  this, SIGNAL(OnTotalAudioLangReceived(QStringList, int*, int)));
        disconnect(m_pGPlayer, SIGNAL(TextLanguageList(QStringList, int*, int)),   this, SIGNAL(OnTotalTextLangReceived(QStringList, int*, int)));
        disconnect(m_pGPlayer, SIGNAL(CurrentTrackDRMStatus(int, int, int)),       this, SIGNAL(OnDRMStatusUpdated(int, int, int))); //modified by jeeeun 2012.11.14
        disconnect(m_pGPlayer, SIGNAL(OnPrivateSignal(int)),                       this, SIGNAL(OnPrivateSignal(int)));
        disconnect(m_pGPlayer, SIGNAL(VideoInfo(int, int, int, int, float, float)),this, SLOT(HandleVideoInfo(int, int, int, int, float, float)));
        disconnect(m_pGPlayer, SIGNAL(XSubtitleChanged(uchar*, int)),              this, SIGNAL(HandleXSubtitleChanged(uchar*, int)));
        disconnect(m_pGPlayer, SIGNAL(RealSoundChanged()),                         this, SLOT(HandleRealSoundChanged()));
        
        m_bConnectGSignal = false;
    }

    if(m_bConnectMSignal == true && m_pMPlayer != 0) 
    {
        MC_HIGH << "MPlayerEngine" << LOG_ENDL;
        
        disconnect(m_pMPlayer, SIGNAL(PositionChanged(int)),                      this, SLOT(HandleUSBPositionChanged(int)));
        disconnect(m_pMPlayer, SIGNAL(DurationChanged()),                         this, SLOT(HandleUSBDurationChanged()));
        disconnect(m_pMPlayer, SIGNAL(StateChanged(int)),                         this, SLOT(HandleUSBStateChanged(int)));
        disconnect(m_pMPlayer, SIGNAL(Error(int)),                                this, SLOT(HandleUSBError(int)));

        disconnect(m_pMPlayer, SIGNAL(ChangeTrack()),                             this, SLOT(HandleTrackChange()));
        disconnect(m_pMPlayer, SIGNAL(CurrentMediaInfo(MediaInfo*)),              this, SLOT(HandleCurrentMediaInfo(MediaInfo*))); 
        disconnect(m_pMPlayer, SIGNAL(CurrentTrackDRMStatus(int, int, int)),      this, SIGNAL(OnDRMStatusUpdated(int, int, int))); //modified by jeeeun 2012.11.14
        
        m_bConnectMSignal = false;
    }   
}

int USBController::IsBootWithUSB()
{
    return m_bBootWithUSB;
}

void USBController::DrmConfirmPlay()
{
    if(m_pPlayer)
    {
        if(m_pPlayer->IsDivXDRMSupported(&divxDrmType , &divxDrmLimitCnt,&divxDrmUseCnt))
        {
            MC_HIGH << "USBController::DrmConfirmPlay Called so calling Play" << LOG_ENDL;
            m_pDrmConfirmPlay = true;
            m_pPlayer->Play();
        }
    }

}

// { added by kihyung for ITS 0181967 
void USBController::SetFSFolderInfo(QList<FSFolderInfo> &tFolderList)
{
    
    if(tFolderList.count() < 0)
    {
        MC_HIGH << "ERROR: tFolderList count:" << tFolderList.count() << LOG_ENDL;
        return;
    }

    if(m_tFolderInfo.nMaxFolderSize < (unsigned int)tFolderList.count())
    {
        if(m_tFolderInfo.pFolderIdx)
        {
            free(m_tFolderInfo.pFolderIdx);
        }
    
        m_tFolderInfo.pFolderIdx     = (MP_FOLDER_INDEX_T*)calloc(tFolderList.count(), sizeof(MP_FOLDER_INDEX_T));
        m_tFolderInfo.nMaxFolderSize = tFolderList.count();
    }

    m_tFolderInfo.nFolderCount    = tFolderList.count();
    m_tFolderInfo.nCurrFolderSize = tFolderList.count();

    for(int i = 0; i < tFolderList.count(); i++)
    {
        FSFolderInfo &tFolderInfo = tFolderList[i];
        m_tFolderInfo.pFolderIdx[i].nFileSize  = tFolderInfo.nFileSize;
        m_tFolderInfo.pFolderIdx[i].nStartIdx  = tFolderInfo.nStartIdx;
        m_tFolderInfo.pFolderIdx[i].nErrorSize = 0;

        // MC_HIGH << i << tFolderInfo.nStartIdx << tFolderInfo.nFileSize << LOG_ENDL;
    }

    m_tFolderInfo.tTotalFolderIdx = m_tFolderInfo.pFolderIdx[0];
}
// } added by kihyung for ITS 0181967 

/*
*******************************************************************
* Function: Play()
*
* Description : To play the media file when the user clicks on "Play".
*                   It will call the CUSBController Play Api, which internally 
*                   will call PlayerEngine Play Api.
*                   media interface related slots
*
* Parameters : None
*
* Returns : void
*
*******************************************************************
*/
void USBController::Play()
{
    if(m_tPlayList.count() <= 0) return;

//added by edo.lee 2013.10.15 ITS 195538
    static int nUsbCount = 0;
	static int nJBCount = 0;

    //added by edo.lee 2013.10.15 ITS 195538
    if(nUsbCount != 0 && m_eMyMediaMode == MP_MODE_USB)
    {
        MC_HIGH << "nUsbCount:" << nUsbCount << LOG_ENDL;
        return;
    }
    else if(nJBCount != 0 && m_eMyMediaMode == MP_MODE_JUKEBOX)
    {
        MC_HIGH << "nJBCount:" << nJBCount << LOG_ENDL;
        return;
    }


   //added by edo.lee 2013.10.15 ITS 195538
    if(m_eMyMediaMode == MP_MODE_USB)
        nUsbCount++;
    else if(m_eMyMediaMode == MP_MODE_JUKEBOX)
        nJBCount++;

    if(m_currentMediaState != PLAYER_PAUSE)
    {
        if(m_nScanMode != SCANOFF)
        {
            MC_HIGH << "m_nScanMode:" << m_nScanMode << LOG_ENDL;
            Scan_Impl(SCANOFF);
        }
    }
    else if(m_nScanMode != SCANOFF)
    {
        MC_HIGH << "m_nScanMode:" << m_nScanMode << LOG_ENDL;
    }

    // for ISV 60630
    if(m_nFFREWRate != FAST_SPEED_X1)
    {
        m_bPlayAfterFFREW = true;
    }

    m_bUserStop  = false;
    m_bUserPause = false; // added by kihyung 2013.07.22 for ITS 0179477 
    m_nFFREWRate = FAST_SPEED_X1;
    m_bSystemRequestMute = false;
    //{added by edo.lee 2013.10.04 
	if(m_currentMediaState == PLAYER_FASTFORWARD || m_currentMediaState == PLAYER_REWIND || m_currentMediaState >= PLAYER_FASTFORWARD_X4)
	{
		m_bAutoTrackChanged = false;
	}
    //}added by edo.lee 2013.10.04 
    
    int nCurrTrackIdx = GetTrackIdx();
    int nUserTrackIdx = m_nCurrTrackIndex;

    MC_HIGH << "m_currentMediaState:" << m_currentMediaState << " nUserTrackIdx:" << nUserTrackIdx << LOG_ENDL;
    
    if(nUserTrackIdx >= 0 && nUserTrackIdx < m_tPlayList.count()) 
    {
        // REPEAT ONE ---> REPEAT OFF
        if(nUserTrackIdx != GetTrackIdx()) { 
            if(m_nRepeatMode == REPEATFILE) {
                m_nRepeatMode = REPEATALL;
                emit OnModeChanged(m_nRepeatMode);
            }
            #if (USE_FILE_INDEX_IN_FOLDER == 0)
            else if(m_nRepeatMode == REPEATFOLDER) 
            {
                int nTrackIdx;
                if(m_nRandomMode != RANDOMOFF && m_nShuffleIdx >= 0 && m_nShuffleIdx < m_tPlayList.count()) 
                {
                    nTrackIdx = m_nShuffleIdx;
                }
                else 
                {
                    nTrackIdx = nCurrTrackIdx;
                }
    
                int nUserFolderNum = GetFolderIdx(nUserTrackIdx);
                int nCurrFolderNum = GetFolderIdx(nTrackIdx);
                MC_HIGH << nTrackIdx << nUserFolderNum << nCurrFolderNum << LOG_ENDL;
                if(nUserFolderNum != nCurrFolderNum) {
                    RepeatPlay(REPEATALL, false);
                }
            }
            #endif
        }

        // modified by kihyung 2013.09.09 for ITS 0188773 
        // if(nUserTrackIdx != nCurrTrackIdx || m_currentMediaState == PLAYER_STOP || m_bFirstPlay == false || m_nSetURITrackIndex == -1) {
        if(nUserTrackIdx != nCurrTrackIdx || m_bFirstPlay == false || m_nSetURITrackIndex == -1)
        {
            SetURI(nUserTrackIdx, true);
            m_bAutoTrackChanged = false; // added by kihyung 2013.12.03 for 0210751 
        }

        if(m_pPlayer->IsDivXDRMSupported(&divxDrmType , &divxDrmLimitCnt,&divxDrmUseCnt) && !m_pDrmConfirmPlay)
        {
            MC_HIGH << "DivxDRM is Supported , use DRM Play for Actual Playing File on User Input" << LOG_ENDL;
            if(m_currentMediaState == PLAYER_FASTFORWARD || m_currentMediaState == PLAYER_REWIND || m_currentMediaState >= PLAYER_FASTFORWARD_X4)
			{
                 if(divxDrmLimitCnt == 0)//modified by taihyun.ahn 2013.11.22
                     m_pPlayer->Play();   // jinkyoung@20131116
            }
        }
        else
        {
            if(m_bBTCallStart == false) 
                m_pPlayer->Play();
            else if(m_pPlayer->getPipeline() == true) // modified by kihyung 2013.08.28
                m_pPlayer->Pause();
        }
        SetTrackIdx(nUserTrackIdx);
        if(m_nRandomMode != RANDOMOFF && m_nCurrTrackIndex != -1)
        {
            for(int i = 0; i < m_tPlayList.count(); i++)
            {
                if(m_pShuffleList[i].nID == nUserTrackIdx)
                {
                    m_nShuffleIdx = i;
                    break;
                }
            }
        }
        m_nCurrTrackIndex    = -1;
        m_bFirstPlay         = true;
        m_bControllerChanged = true; // for OnTrackChanged()
    }
    else 
    {
        if(nCurrTrackIdx >= 0 && nCurrTrackIdx < m_tPlayList.count()) 
        {
            // modified by kihyung 2013.09.09 for ITS 0188773 
            // if(m_currentMediaState == PLAYER_STOP || m_nSetURITrackIndex == -1) {
            if(m_nSetURITrackIndex == -1)
            {
                SetURI(nCurrTrackIdx, true);
            }
            
            if(m_pPlayer->IsDivXDRMSupported(&divxDrmType , &divxDrmLimitCnt,&divxDrmUseCnt) && !m_pDrmConfirmPlay)
            {
                MC_HIGH << "DivxDRM is Supported , use DRM Play for Actual Playing File on User Input" <<LOG_ENDL;
                if(m_currentMediaState == PLAYER_FASTFORWARD || m_currentMediaState == PLAYER_REWIND || m_currentMediaState >= PLAYER_FASTFORWARD_X4)
                {
                    if(divxDrmLimitCnt == 0)//modified by taihyun.ahn 2013.11.22
                        m_pPlayer->Play();   // jinkyoung@20131116
                }
            }
            else
            {
                if(m_bBTCallStart == false) 
                    m_pPlayer->Play();
                else if(m_pPlayer->getPipeline() == true) // modified by kihyung 2013.08.28
                    m_pPlayer->Pause();
            }
        }
        else
        {
            MC_HIGH << "nCurrTrackIdx:" << nCurrTrackIdx << LOG_ENDL;
        }
    }

    if(m_bPlayAfterFFREW == true) 
    {
        EmitOnStateChanged(PLAYER_PLAY);
    }

    //added by edo.lee 2013.10.15 ITS 195538
	//nCount = 0;    
    if(m_eMyMediaMode == MP_MODE_USB)
        nUsbCount = 0;
    else if(m_eMyMediaMode == MP_MODE_JUKEBOX)
        nJBCount = 0;
}

/*
*******************************************************************
* Function: Pause()
*
* Description : To play the media file when the user clicks on "Pause".
*                   It will call the CUSBController Pause Api, which internally 
*                   will call PlayerEngine Pause Api.
*                   media interface related slots
*
* Parameters : None
*
* Returns : void
*
*******************************************************************
*/
void USBController::Pause()
{
    if(m_tPlayList.count() <= 0) return;

    MC_HIGH << LOG_ENDL;

    m_bUserPause = true; // added by kihyung 2013.07.22 for ITS 0179477 
    
    if(m_pPlayer->getPipeline() == true)
    {
        ClearUserReqTrack();
        m_bSkipPauseSignal = false;
        m_pPlayer->Pause();
    }
    else
    {
        MC_HIGH << "m_pPlayer->getPipeline() return false" << LOG_ENDL;
        usleep(300000);
        if(m_pPlayer->getPipeline() == true)
        {
            MC_HIGH << "m_pPlayer->getPipeline() return true" << LOG_ENDL;
            ClearUserReqTrack();
            m_bSkipPauseSignal = false;
            m_pPlayer->Pause();
        }
        else
        {
            MC_HIGH << "m_pPlayer->getPipeline() return false again" << LOG_ENDL;
        }
    }

    // { added by kihyung 2013.12.05 for ITS 0212166 
    int nErrIdx;
            
    if(m_nRandomMode != RANDOMOFF)
    {
        nErrIdx = m_nShuffleIdx;
    }
    else
    {
        nErrIdx = m_nSetURITrackIndex;
    }

    if(nErrIdx >= 0 && nErrIdx < m_tPlayList.count()) 
    {
        if(m_pShuffleList[nErrIdx].bError == 1)
        {
            m_nSetURITrackIndex = -1;
            EmitOnStateChanged(PLAYER_CHANGE_PAUSE_ICON);
        }
    }
    // } added by kihyung 2013.12.05 for ITS 0212166 
}

void USBController::FunctionStop()
{
    MC_HIGH << LOG_ENDL;
    
    Stop();
}

/*
*******************************************************************
* Function: Stop()
*
* Description : To play the media file when the user clicks on "Stop".
*                   It will call the CUSBController Stop Api, which internally 
*                   will call PlayerEngine Stop Api.
*                   media interface related slots
*
* Parameters : None
*
* Returns : void
*
*******************************************************************
*/
void USBController::Stop()
{
    MC_HIGH << LOG_ENDL;

    if(m_tPlayTimer.isActive() == true) // { added by sungha.choi 2013.08.14 for ITS 0184305
    {
        m_tPlayTimer.stop();
    }    // } added by sungha.choi 2013.08.14 for ITS 0184305

    if(m_bAudioDragStart == true)
    {
        AudioDragEnd();
    }
    
    if(m_nScanMode != SCANOFF)
    {
        MC_HIGH << "m_nScanMode:" << m_nScanMode << LOG_ENDL;
        Scan_Impl(SCANOFF);
    }
    
    m_bUserStop  = true;
    m_bUserPause = false; // added by kihyung 2013.07.22 for ITS 0179477 
    m_nFFREWRate = FAST_SPEED_X1;
    m_bSystemRequestMute = false;
    
	ClearUserReqTrack();

    m_bSkipPauseSignal = false;
    
    if(GetCurrentMediaState() != PLAYER_STOP || m_pPlayer->getPipeline() == true)
    {
        m_pPlayer->Stop();
    }
    // { added by kihyung 2013.10.04 for ITS 0191215 
    else if(GetCurrentMediaState() == PLAYER_STOP || m_pPlayer->getPipeline() == false)
    {
        EmitOnStateChanged(PLAYER_STOP);
    }
    // } added by kihyung 2013.10.04 for ITS 0191215 

    m_nSetURITrackIndex = -1; // added by kihyung 2013.11.04
}

void USBController::Next(int skiplevel)
{
    Next(skiplevel, false);
}

void USBController::Next(int skiplevel, int nAutoSkip)
{
    if(m_tPlayList.count() <= 0) return;

    MC_HIGH << skiplevel<<nAutoSkip << LOG_ENDL;

    MEDIA_TYPE eMediaType = m_pPlayer->GetMediaType();

    m_bUserStop      = false;
    m_bUserPause     = false; // added by kihyung 2013.07.22 for ITS 0179477 
    m_nFFREWRate     = FAST_SPEED_X1;
    m_bPrevDirection = false;
    m_bSystemRequestMute = false;
    m_nUserPlayPosition  = 0; // added by kihyung 2013.09.09 for ITS 0188880 

    // { modified by kihyung 2013.08.21 for ITS 0185415
    int bEngineStop = false;
    if(GetCurrentMediaState() != PLAYER_STOP || m_pPlayer->getPipeline() == true)
    {
        if(m_tPlayList.size() == 1) bEngineStop = true;
        if(m_nRepeatMode == REPEATFOLDER) {
            // { modified by kihyung 2013.11.04            
            /*
            int nFileIdx    = 0;
            int nTotalFiles = 1;
            GetCurrentFileFolderIndex(&nFileIdx, &nTotalFiles);
            */
            int nTotalFiles = 1;
            if(m_nRandomMode != RANDOMOFF && m_nShuffleIdx >= 0 && m_nShuffleIdx < m_tPlayList.count()) 
            {
                nTotalFiles = GetFileSizeOfFolder(m_nShuffleIdx);
            }
            else 
            {
                nTotalFiles = GetFileSizeOfFolder(GetTrackIdx());
            }
            // } modified by kihyung 2013.11.04
            if(nTotalFiles == 1) bEngineStop = true;
        }
        if(bEngineStop == true)
        {
            // for ITS 0179426  no need to Stop for Next , as it shows Black screen for long time. Already done in PlayerEngine
            MC_HIGH << "bEngineStop =" << bEngineStop << LOG_ENDL;
            m_pPlayer->Stop();
        }
    }
    // } modified by kihyung 2013.08.21 for ITS 0185415    

    if(eMediaType != DVD_DEVICE && eMediaType != AUX_DEVICE) 
    {
        // { added by sungha.choi 2013.07.01
        if(m_tPlayTimer.isActive() == true)
        {
            m_tPlayTimer.stop();
        }
 
       if(skiplevel > 0) {
            if(m_pPlayer->GetMediaType() == AUDIO_FILE)
            {
                if(GetCurrentMediaState() != PLAYER_STOP || m_pPlayer->getPipeline() == true)
                { // { added by sungha.choi 2013.08.29
                   m_pPlayer->Stop();
                   m_currentMediaPosition = 0;
                   emit OnPositionChanged(m_currentMediaPosition);
                } // } added by sungha.choi 2013.08.29
            }
            else {
                if(GetCurrentMediaState() != PLAYER_PAUSE && m_pPlayer->getPipeline() == true) { // modified by kihyung 2013.08.28
                    m_bSkipPauseSignal = true;
                    if(m_pPlayer->IsDivXDRMSupported(&divxDrmType , &divxDrmLimitCnt,&divxDrmUseCnt) && !m_pDrmConfirmPlay)
                    {
                        MC_HIGH << "DivxDRM is Supported , use DRM Play for Actual Playing File on User Input" << LOG_ENDL;
                    }
                    else
                    {
                        m_pPlayer->Pause();
                    }
                    m_currentMediaPosition = 0;
                    emit OnPositionChanged(m_currentMediaPosition);
                }
            }
            m_tPlayTimer.start();
            m_nPrevForced = 0;  // Next Play don't use this variable. So, this condition will be omit in timer handler.
            int nNewTrackIdx = -1;
            m_tUserReqMutex.lock();

            // Add new track number into list for delay play.
            if(m_nRandomMode == RANDOMOFF)
            {
                nNewTrackIdx = GetNextTrackIdx(skiplevel, GetTrackIdx());
            }
            else
            {
                nNewTrackIdx = GetRandomTrackIdx(true);
            }
            SetTrackIdx(nNewTrackIdx);
            m_tUserReqMutex.unlock();

            // { modified by kihyung 2013.11.13 for ITS 0208734 
            if(nAutoSkip == false) 
            {
                //if(bEngineStop == false)  //remove by edo.lee 2013.10.22
				    EmitTrackBuffered(false, nNewTrackIdx);
            }
            // } modified by kihyung 2013.11.13 for ITS 0208734

            //{added by taihyun.ahn 2014.01.03
            // { modified by kihyung 2014.01.07 for ITS 0210751 
            // if(m_bAutoTrackChanged == true && m_currentMediaState == PLAYER_SEARCH)
            if(m_bAutoTrackChanged == true && m_currentMediaState != PLAYER_CHANGE_PLAY_ICON) // modified by sangmin.seol 2014.02.12 for ITS 0224945 Add (m_currentMediaState != PLAYER_CHANGE_PLAY_ICON) condition
            // } modified by kihyung 2014.01.07 for ITS 0210751                 
            {
                MC_HIGH << "m_bAutoTrackChanged 1 -> 0. m_currentMediaState:" << m_currentMediaState << LOG_ENDL;
                m_bAutoTrackChanged = false;
            }
            //} added by taihyun.ahn 2014.01.03
            return;
        }
        // } added by sungha.choi 2013.07.01
        
        // remvoed by kihyung 2013.08.21
    }
    
    // remvoed by kihyung 2013.08.21
}

void USBController::Prev(int skiplevel, int nForced)
{
    Prev(skiplevel, false, nForced);
}

void USBController::Prev(int skiplevel, int nAutoSkip, int nForced)
{
    if(m_tPlayList.count() <= 0) return;

    //MC_HIGH << skiplevel<<nAutoSkip << nForced << LOG_ENDL;
    
    int        nPosition  = m_pPlayer->GetPosition();
    MEDIA_TYPE eMediaType = m_pPlayer->GetMediaType();

    m_bUserStop      = false;
    m_bUserPause     = false; // added by kihyung 2013.07.22 for ITS 0179477 
    m_nFFREWRate     = FAST_SPEED_X1;
    m_bPrevDirection = true;
    m_bSystemRequestMute = false;
    m_nUserPlayPosition  = 0; // added by kihyung 2013.09.09 for ITS 0188880

    // { modified by kihyung 2013.08.21 for ITS 0185415
    int bEngineStop = false;
    if(GetCurrentMediaState() != PLAYER_STOP || m_pPlayer->getPipeline() == true)
    {
        if(skiplevel == 0)
        {
            m_pPlayer->Stop();            
        }
        else if(skiplevel > 0)
        {
            // GO TO PREVIOUS FILE.
            if(nPosition <= 4000 || nForced != 0)
            {
                if(m_tPlayList.size() == 1) bEngineStop = true;
                if(m_nRepeatMode == REPEATFOLDER)
                {
                    // { modified by kihyung 2013.11.04            
                    /*
                    int nFileIdx    = 0;
                    int nTotalFiles = 1;
                    GetCurrentFileFolderIndex(&nFileIdx, &nTotalFiles);
                    */
                    int nTotalFiles = 1;
                    if(m_nRandomMode != RANDOMOFF && m_nShuffleIdx >= 0 && m_nShuffleIdx < m_tPlayList.count()) 
                    {
                        nTotalFiles = GetFileSizeOfFolder(m_nShuffleIdx);
                    }
                    else 
                    {
                        nTotalFiles = GetFileSizeOfFolder(GetTrackIdx());
                    }
                    // } modified by kihyung 2013.11.04
                    if(nTotalFiles == 1) bEngineStop = true;
                }
                if(bEngineStop == true)
                {
                    // for ITS 0179426  no need to Stop for Next , as it shows Black screen for long time. Already done in PlayerEngine
                    MC_HIGH << "bEngineStop =" << bEngineStop << LOG_ENDL;
                    m_pPlayer->Stop();
                }
            }
        }
    }
    // } modified by kihyung 2013.08.21 for ITS 0185415


    MC_HIGH << "eMediaType = "<< eMediaType << " nPosition = " << nPosition << \
               " nAutoSkip = " << nAutoSkip << "nForced = " << nForced << \
               " m_tPlayTimer.isActive() = " << m_tPlayTimer.isActive() << LOG_ENDL;

    if((eMediaType != DVD_DEVICE && eMediaType != AUX_DEVICE) && ((nPosition <= 4000 || nForced != 0) || m_tPlayTimer.isActive()))  // modified by sungha.choi 2013.08.29
    {
        // { added by sungha.choi 2013.07.01
        if(m_tPlayTimer.isActive() == true)
        {
            m_tPlayTimer.stop();
        }

        if(skiplevel > 0) {
            if(m_pPlayer->GetMediaType() == AUDIO_FILE)
            {
                if(GetCurrentMediaState() != PLAYER_STOP || m_pPlayer->getPipeline() == true)
                { // { added by sungha.choi 2013.08.29
                    m_pPlayer->Stop();
                    m_currentMediaPosition = 0;
                    emit OnPositionChanged(m_currentMediaPosition);
                } // } added by sungha.choi 2013.08.29
            }
            else
            {
                if(GetCurrentMediaState() != PLAYER_PAUSE && m_pPlayer->getPipeline() == true)
                { // modified by kihyung 2013.08.28
                    m_bSkipPauseSignal = true;
                    if(m_pPlayer->IsDivXDRMSupported(&divxDrmType , &divxDrmLimitCnt,&divxDrmUseCnt) && !m_pDrmConfirmPlay)
                    {
                        MC_HIGH << "DivxDRM is Supported , use DRM Play for Actual Playing File on User Input" << LOG_ENDL;
                    }
                    else
                    {
                        m_pPlayer->Pause();
                    }
                    m_currentMediaPosition = 0;
                    emit OnPositionChanged(m_currentMediaPosition);
                }
            }
            m_tPlayTimer.start();
            m_nPrevForced = nForced;
            int nNewTrackIdx = -1;
            m_tUserReqMutex.lock();

            // Add new track number into list for delay play.
            if(m_nRandomMode == RANDOMOFF)
            {
                nNewTrackIdx = GetPrevTrackIdx(skiplevel, GetTrackIdx());
            }
            else
            {
                nNewTrackIdx = GetRandomTrackIdx(false);
            }
            SetTrackIdx(nNewTrackIdx);
            m_tUserReqMutex.unlock();

            // { modified by kihyung 2013.11.13 for ITS 0208734
            if(nAutoSkip == false) 
            {
                //if(bEngineStop == false) //remove by edo.lee 2013.10.22
                    EmitTrackBuffered(true, nNewTrackIdx);
            }
            // } modified by kihyung 2013.11.13 for ITS 0208734
			//{added by taihyun.ahn 2014.01.03
            if(m_bAutoTrackChanged == true && m_currentMediaState != PLAYER_CHANGE_PLAY_ICON)   // modified by sangmin.seol 2014.02.12 for ITS 0224945 Add (m_currentMediaState != PLAYER_CHANGE_PLAY_ICON) condition
            {
                m_bAutoTrackChanged = false;
            }
            //}added by taihyun.ahn 2014.01.03
            return;
        }
        // } added by sungha.choi 2013.07.01
        
        // remvoed by kihyung 2013.08.21
    }

    // remvoed by kihyung 2013.08.21
    
    if(skiplevel > 0)
    {
        // GO TO BEGINNING OF FILE
//added by edo.lee 2013.10.12 pop noise
        /*if(nPosition > 4000)
        {
            MC_HIGH << "GO TO BEGINNING OF FILE" << nPosition << LOG_ENDL;
            
            m_pPlayer->SetPosition(0);
            if(m_pPlayer->GetState() == eMP_STATE_PAUSED)
            {
                MC_HIGH << "nState == eMP_STATE_PAUSED -> Play()" << LOG_ENDL;
                if(m_pPlayer->IsDivXDRMSupported(&divxDrmType , &divxDrmLimitCnt,&divxDrmUseCnt))
                {
                    MC_HIGH << "DivxDRM is Supported , use DRM Play for Actual Playing File on User Input" << LOG_ENDL;
                }
                else
                {
                    m_pPlayer->Play();
                }
            }
        }*/
        if(m_pPlayer->GetMediaType() == AUDIO_FILE)
        {
            if(GetCurrentMediaState() != PLAYER_STOP || m_pPlayer->getPipeline() == true) // { added by sungha.choi 2013.08.29
            {
                m_pPlayer->Stop();
                m_currentMediaPosition = 0;
                emit OnPositionChanged(m_currentMediaPosition);
            } 
        }
        else
        {
            if(GetCurrentMediaState() != PLAYER_PAUSE && m_pPlayer->getPipeline() == true)  // modified by kihyung 2013.08.28
            {
               // m_bSkipPauseSignal = true;
               // if(m_pPlayer->IsDivXDRMSupported(&divxDrmType , &divxDrmLimitCnt,&divxDrmUseCnt) && !m_pDrmConfirmPlay)
               // {
               //     MC_HIGH << "DivxDRM is Supported , use DRM Play for Actual Playing File on User Input" << LOG_ENDL;
               // }
               // else
               // {
                m_pPlayer->Pause();
               // }
                m_currentMediaPosition = 0;
                emit OnPositionChanged(m_currentMediaPosition);
            }
        }
		m_tPlayTimer.start();//added by edo.lee 2013.10.12 pop noise
    }
}

void USBController::Scan(int mode)
{
    int bEmit = false;
    
    MC_HIGH << mode << LOG_ENDL;

    if(m_nScanMode != mode)
    {
        m_nScanMode = mode;
        bEmit = true;
    }
    
    if(mode == SCANFILE || mode == SCANFOLDER || mode == SCANALL)
    {
        ClearUserReqTrack();

        // removed by kihyung 2013.09.17 for HMC new sepc.
        // m_nRandomModeBeforeScan = m_nRandomMode; // added by kihyung 2013.07.17 for ITS 0180420
        // MC_HIGH << "m_nRandomModeBeforeScan:" << m_nRandomModeBeforeScan << LOG_ENDL;
        
        // RANDOM OFF
        if(m_nRandomMode != RANDOMOFF)
        {
            RandomPlay_Impl(RANDOMOFF);
        }

        // SET REPEAT MODE LIKE SCAN MODE
        if(mode == SCANFILE && m_nRepeatMode != REPEATFOLDER)
        {
            m_nRepeatMode = REPEATFOLDER;
            emit OnModeChanged(m_nRepeatMode);
        }
        else if((mode == SCANALL || mode == SCANFOLDER) && m_nRepeatMode != REPEATALL)
        {
            m_nRepeatMode = REPEATALL;
            emit OnModeChanged(m_nRepeatMode);
        }
        // REPEAT FILE --> REPEAT FOLDER
        else if(m_nRepeatMode == REPEATFILE)
        {
            m_nRepeatMode = REPEATALL;
            emit OnModeChanged(m_nRepeatMode);
        }
        
        m_nEndScanIndex = GetTrackIdx();
        SetTrackIdx(GetNextTrackIdx());
        MC_LOG << "m_nEndScanIndex:" << m_nEndScanIndex << LOG_ENDL;

        if(m_tPlayList.count() > 0)
        {
            m_pPlayer->Stop();
            m_nUserPlayPosition = 0; // added by kihyung 2013.09.09 for ITS 0188880 
            SetURI(GetTrackIdx());
            m_pPlayer->Play();
            //EmitOnTrackChanged(GetTrackIdx());
            m_bEmitTrackChanged = true;
        }
    }
    else
    {
        m_nEndScanIndex = -1;

        // { removed by kihyung 2013.09.17 for HMC new sepc.
        /*
        // { added by kihyung 2013.07.17 for ITS 0180420
        MC_HIGH << "m_nRandomModeBeforeScan:" << m_nRandomModeBeforeScan << LOG_ENDL;
        if(m_nRandomModeBeforeScan != RANDOMOFF)
        {
            RandomPlay_Impl(m_nRandomModeBeforeScan);
            m_nRandomModeBeforeScan = RANDOMOFF;
        }
        // } added by kihyung 2013.07.17 for ITS 0180420
        */
        // } removed by kihyung 2013.09.17 for HMC new sepc.
    }

    if(bEmit == true)
    {
        emit OnModeChanged(m_nScanMode);
    }

    // Spec 1. cancel pause mode when selecting repeat on, random on and scan on. 
    // Spec 2. keep pause mode when selecting repeat off, random off, scan off.
    // { added by kihyung 2012.07.11 for CR 5502
    if(m_currentMediaState == PLAYER_PAUSE && m_nScanMode != SCANOFF)
    {
        MC_HIGH << "Pause--->Play()" << LOG_ENDL;
        Play();
    }
    // } added by kihyung
}

void USBController::RandomPlay(int mode, int bPlay)
{   
    if(mode != RANDOMFILE && mode != RANDOMFOLDER && mode != RANDOMOFF)
    {
        MC_HIGH << "Invalid option" << mode << LOG_ENDL;
        return;
    }

    MC_HIGH << mode << LOG_ENDL;
    
    if(mode == RANDOMFILE || mode == RANDOMFOLDER)
    {
        if(mode != m_nRandomMode) // added by kihyung 2013.08.25 for ITS 0186423 
        {
            if(m_nScanMode != SCANOFF)
            {
                Scan_Impl(SCANOFF);
            }

            SetShuffleList(false); // moved by sangmin.seol 2014.01.22 for ITS 0221836 Do SetShuffleList only once after randommode changed
        }

        if(m_nRepeatMode == REPEATFILE)
        {
            m_nRepeatMode = REPEATALL;
            emit OnModeChanged(m_nRepeatMode);
        }

        //SetShuffleList(false); // moved by sangmin.seol 2014.01.22 for ITS 0221836 Do SetShuffleList only once after randommode changed
    }
    else if(m_nRandomMode != RANDOMOFF) 
    {
        int nFileSize = m_tPlayList.count();
        int nStartIdx = 0;

        if(m_nRepeatMode == REPEATFOLDER)
        {
            int nTrackIdx = GetTrackIdx();
            nFileSize     = GetFileSizeOfFolder(nTrackIdx);
            nStartIdx     = GetStartFileIdxOfFolder(nTrackIdx);
        }
        
        if(nFileSize > 1)
        {
            RestoreFromShuffle(nStartIdx, nFileSize);
        }
    }
        
    if(m_nRandomMode != mode)
    {
        m_nRandomMode = mode;
        emit OnModeChanged(m_nRandomMode);
    }

    // Spec 1. cancel pause mode when selecting repeat on, random on and scan on. 
    // Spec 2. keep pause mode when selecting repeat off, random off, scan off.
    // { added by kihyung 2012.07.11 for CR 5502
    if(m_currentMediaState == PLAYER_PAUSE && m_nRandomMode != RANDOMOFF && bPlay == true) {
        MC_HIGH << "Pause--->Play()" << LOG_ENDL;
        Play();
    }
    // } added by kihyung
}

void USBController::RepeatPlay(int mode, int bPlay)
{
    int bShuffleList = false;
    int nPrevRepeatMode = m_nRepeatMode;
    
    if(mode == REPEATFILE || mode == REPEATFOLDER || mode == REPEATALL)
    {
        MC_HIGH << mode << LOG_ENDL;
        
        if(m_nRandomMode != RANDOMOFF) 
        {
            if(m_nRepeatMode == REPEATALL && mode == REPEATFOLDER)
            {
                RestoreFromShuffle(0, m_tPlayList.count());
                bShuffleList = true;
            }
            else if(m_nRepeatMode == REPEATFOLDER && mode == REPEATALL)
            {
                int nTrackIdx = GetTrackIdx();
                RestoreFromShuffle(GetStartFileIdxOfFolder(nTrackIdx), GetFileSizeOfFolder(nTrackIdx));
                bShuffleList = true;
            }
        }

        if(nPrevRepeatMode != mode) // added by kihyung 2013.08.25 for ITS 0186423 
        { 
            if(m_nScanMode != SCANOFF)
            {
                Scan_Impl(SCANOFF);
            }
        }

        m_nRepeatMode = mode;

        if(mode == REPEATFILE && m_nRandomMode != RANDOMOFF)
        {
            RandomPlay_Impl(RANDOMOFF);
        }

        // { added by jeeeun for eMP_ERR_UNSUPPORTED_ALL_FILE on REPEAT FOLDER
        if(mode == REPEATFOLDER)
        {
            m_nPlayableSize = GetFileSizeOfFolder(GetTrackIdx()) - GetErrorSizeOfFolder(GetTrackIdx()); 
            if(m_nPlayableSize < 0)
            {
                MC_HIGH << "ERROR : m_nPlayableSize:" << m_nPlayableSize << LOG_ENDL;
                m_nPlayableSize = 0;
            }
        }
        else if(mode == REPEATALL)
        {
            m_nPlayableSize = m_tPlayList.count() - m_nTotalErrSize;     
            if(m_nPlayableSize < 0)
            {
                MC_HIGH << "ERROR : m_nPlayableSize:" << m_nPlayableSize << LOG_ENDL;
                m_nPlayableSize = 0;
            }
        }		
        MC_LOG << "m_nPlayableSize:" << m_nPlayableSize << LOG_ENDL;
		// } added by jeeeun 
    }
    else
    {
        MC_HIGH << "Invalid option" << mode << LOG_ENDL;
        return;
    }

    if(nPrevRepeatMode != mode)
    {
        emit OnModeChanged(m_nRepeatMode);
    }

    if(bShuffleList == true)
    {
        SetShuffleList(false);
    }

    // Spec 1. cancel pause mode when selecting repeat on, random on and scan on. 
    // Spec 2. keep pause mode when selecting repeat off, random off, scan off.
    // { added by kihyung 2012.07.11 for CR 5502
    if(m_currentMediaState == PLAYER_PAUSE && bPlay == true)
    {
        // { modified by kihyung 2013.10.02 for ITS 0191919 
        // { modified by kihyung 2013.07.04 for ITS NA 0171742 
        if(m_nRepeatMode == REPEATFILE || m_nRepeatMode == REPEATFOLDER)
        // if(m_nRepeatMode == REPEATFILE || m_nRepeatMode == REPEATFOLDER || m_nRepeatMode == REPEATALL)
        {
            MC_HIGH << "Pause--->Play()" << LOG_ENDL;
            Play();
        }
        // } modified by kihyung 2013.07.04 for ITS NA 0171742         
        // } modified by kihyung 2013.10.02 for ITS 0191919         
    }
    // } added by kihyung
}

/*
*******************************************************************
* Function: FastForward()
*
* Description : IMedia Player Application will call this Api with the rate of 
*                   fastforward to CUSBController which internally call 
*                   PlayerEngine FastForward(int) Api.
*                   media interface related slots
*
* Parameters : int rate
*
* Returns : void
*
*******************************************************************
*/
void USBController::FastForward(int rate)
{    
    if(m_tPlayList.count() <= 0) return;

    MC_HIGH << rate << LOG_ENDL;

    if(m_tPlayTimer.isActive() == true)
    {   // added by sungha.choi 2013.09.09 for ITS 0188777
        m_tPlayTimer.stop();
        HandlePlayTimer();
    }


    if(m_nScanMode != SCANOFF && rate != FAST_SPEED_X1)
    {
        MC_HIGH << "SCAN->SCAN OFF" << LOG_ENDL;
        Scan_Impl(SCANOFF);
    }

    int bChanged = false;

    if(m_nFFREWRate != rate)
    {
        bChanged     = true;
        m_nFFREWRate = rate;
    }

    m_bUserStop  = false;
    m_bUserPause = false; // added by kihyung 2013.07.22 for ITS 0179477 
    m_bREWMode   = false;
    m_bSystemRequestMute = false;

    if(m_pPlayer->GetState() == eMP_STATE_PAUSED && rate == FAST_SPEED_X1)  // modified by edo.lee 2013.08.17 hotfix pop noise on FF20
    {
        MC_LOG << "FastForward(): nState == eMP_STATE_PAUSED -> Play()" << LOG_ENDL;
        #if USE_PE_MUTE_FUNCTION
        m_pPlayer->SetMute(true);
        #endif
        m_pPlayer->Play();
    }
    
    m_pPlayer->FastForward(rate);

    if(bChanged)
    {
        if(rate == FAST_SPEED_X4)
        {
            EmitOnStateChanged(PLAYER_FASTFORWARD_X4);
        }
        else if(rate == FAST_SPEED_X16)
        {
            EmitOnStateChanged(PLAYER_FASTFORWARD_X16);
        }
        else if(rate == FAST_SPEED_X20)
        {
            EmitOnStateChanged(PLAYER_FASTFORWARD_X20);
        }
    }
    emit OnAutoTrackChanged(-1); //added by edo.lee 2013.10.19
}

/*
*******************************************************************
* Function: Rewind()
*
* Description : IMedia Player Application will call this Api with the rate of 
*                   fastforward to CUSBController which internally call 
*                   PlayerEngine Rewind(int) Api.
*                   media interface related slots
*
* Parameters : int rate
*
* Returns : void
*
*******************************************************************
*/
void USBController::Rewind(int rate)
{
    if(m_tPlayList.count() <= 0) return;

    MC_HIGH << rate << LOG_ENDL;

    if(m_tPlayTimer.isActive() == true)    // added by sungha.choi 2013.09.09 for ITS 0188777
    {
        m_tPlayTimer.stop();
        HandlePlayTimer();
    }

    if(m_nScanMode != SCANOFF && rate != FAST_SPEED_X1)
    {
        MC_HIGH << "SCAN->SCAN OFF" << LOG_ENDL;
        Scan_Impl(SCANOFF);
    }

    int bChanged = false;
    if(m_nFFREWRate != rate)
    {
        bChanged     = true;
        m_nFFREWRate = rate;
    }

    m_bUserStop  = false;
    m_bUserPause = false; // added by kihyung 2013.07.22 for ITS 0179477 
    m_bREWMode   = true;
    m_bSystemRequestMute = false;
    
    if(m_pPlayer->GetState() == eMP_STATE_PAUSED && rate == FAST_SPEED_X1) // modified by edo.lee 2013.08.17 hotfix pop noise on FF20
    {
        MC_HIGH << "nState == eMP_STATE_PAUSED -> Play()" << LOG_ENDL;
        #if USE_PE_MUTE_FUNCTION
        m_pPlayer->SetMute(true);
        #endif
        m_pPlayer->Play();       
    }
    
    m_pPlayer->Rewind(rate);

    if(bChanged)
    {
        if(rate == FAST_SPEED_X4)
        {
            EmitOnStateChanged(PLAYER_REWIND_X4);
        }
        else if(rate == FAST_SPEED_X16)
        {
            EmitOnStateChanged(PLAYER_REWIND_X16);
        }
        else if(rate == FAST_SPEED_X20)
        {
            EmitOnStateChanged(PLAYER_REWIND_X20);
        }
    }
    emit OnAutoTrackChanged(-1);//added by edo.lee 2013.10.19
}


/*
*******************************************************************
* Function: NextFolder()
*
* Description : when user clicks the "NextFolder", Media Player App.
*                   will call the NextFolder Api of CUSBController and pass 
*                   the Playlist structure as the parameter
*                   media interface related slots
*
* Parameters : PlayList *pPlayList
*
* Returns : void
*
*******************************************************************
*/
void USBController::NextFolder(PlayList *pPlayList)
{
    if(pPlayList)
    {
        if(pPlayList->list->count() <= 0) return;
        
        m_tPlayList.clear();
        
        for(int i = 0; i < pPlayList->list->count(); i++)
        {
            m_tPlayList.append(pPlayList->list->at(i));
        }

        if(SetTrackIdx(pPlayList->index) == false)
        {
            SetTrackIdx(0);
        }

        m_playlistName = pPlayList->PlaylistName;
        m_nUserPlayPosition = 0; // added by kihyung 2013.09.09 for ITS 0188880 
        SetURI(GetTrackIdx());
        if(pPlayList->position > 0) SetCurrentMediaPosition(pPlayList->position);
    }
    else
    { 
        int nFolderIdx = GetFolderIdx(GetTrackIdx()) + 1;
        if(GetFolderSize() <= nFolderIdx) nFolderIdx = 0;

        int nNewTrackIdx = GetStartFileIdx(nFolderIdx);
        SetCurrentIndex(nNewTrackIdx);
        Stop();
        Play();
    }
}


/*
*******************************************************************
* Function: PrevFolder()
*
* Description : when user clicks the "PrevFolder", Media Player App.
*                   will call the PrevFolder Api of CUSBController and pass 
*                   the Playlist structure as the parameter
*                   media interface related slots
*
* Parameters : PlayList *pPlayList
*
* Returns : void
*
*******************************************************************
*/
void USBController::PrevFolder(PlayList *pPlayList)
{
    if(pPlayList)
    {
        if(pPlayList->list->count() <= 0) return;
        
        m_tPlayList.clear();
        
        for(int i = 0; i < pPlayList->list->count(); i++)
        {
            m_tPlayList.append(pPlayList->list->at(i));
        }

        if(SetTrackIdx(pPlayList->index) == false)
        {
            SetTrackIdx(0);
        }
        
        m_playlistName = pPlayList->PlaylistName;

        m_nUserPlayPosition = 0; // added by kihyung 2013.09.09 for ITS 0188880 
        SetURI(GetTrackIdx());
        if(pPlayList->position > 0) SetCurrentMediaPosition(pPlayList->position);
    }
    else
    {
        int nFolderIdx = GetFolderIdx(GetTrackIdx()) - 1;
        if(nFolderIdx < 0) nFolderIdx = GetFolderSize() - 1;
        if(nFolderIdx < 0)
        {
            MC_HIGH << "nFolderIdx:" << nFolderIdx << LOG_ENDL;
            return;
        }

        int nNewTrackIdx = GetStartFileIdx(nFolderIdx);
        SetCurrentIndex(nNewTrackIdx);
        Stop();
        Play();
    }
}


/*
*******************************************************************
* Function: StartSlideShow()
*
* Description : It will be able to show Images in slide show way one after
*                   other after a specified time duration(which is also configurable).  
*                   viewer interface related slots
*
* Parameters : None
*
* Returns : void
*
*******************************************************************
*/
void USBController::StartSlideShow()
{
    if(m_tPlayList.count() <= 0) return;

    QStringList tPlaylist;
    for(int i = 0; i < m_tPlayList.count(); i++)
    {
        tPlaylist.append(m_tPlayList.at(i));
    }
    
    m_pPlayer->SetSlideShowData(tPlaylist, GetTrackIdx());
    m_pPlayer->StartSlideShow();

    m_bSldShowMode = true;
}

/*
*******************************************************************
* Function: StopSlideShow()
*
* Description : It will stop the image slide show. Implemented as slots. 
*                   viewer interface related slots
*
* Parameters : None
*
* Returns : void
*
*******************************************************************
*/
void USBController::StopSlideShow()
{
    if(m_tPlayList.count() <= 0) return;
    
    m_bSldShowMode = false;
    m_pPlayer->EndSlideShow();
}

/*
*******************************************************************
* Function: Zoom()
*
* Description : Supports Zoom In/out functionality on image with 
*                   1x, 2x, 4x, -2x, -4x levels. Implemented as slots. 
*                   viewer interface related slots
*
* Parameters : int level
*
* Returns : void
*
*******************************************************************
*/
void USBController::Zoom(int level)
{
    if(m_tPlayList.count() <= 0) return;

    int nScale = 1;

    switch(level)
    {
    case ZOOM_LEVEL_MINUS_4X:
        nScale = -4;
        break;
    case ZOOM_LEVEL_MINUS_2X:
        nScale = -2;
        break;
    case ZOOM_LEVEL_PLUS_2X:
        nScale = 2;
        break;
    case ZOOM_LEVEL_PLUS_4X:
        nScale = 4;
        break;
    default:
        nScale = 1;
    }

    MC_HIGH << "level:" << nScale << LOG_ENDL;
    
    m_pPlayer->Zoom(nScale);
}

void USBController::ZoomWithPinch(double level)
{
    if(m_tPlayList.count() <= 0) return;
    
    m_pPlayer->ZoomWithPinch(level);
}

/*
*******************************************************************
* Function: Rotate()
*
* Description : Supports rotation functionality on image with 90, 180 
*                   & 270 degress to the right. Implemented as slots.
*                   viewer interface related slots
*
* Parameters : int angle
*
* Returns : void
*
*******************************************************************
*/
void USBController::Rotate(int angle)
{
    if(m_tPlayList.count() <= 0) return;
    
    m_pPlayer->Rotate(angle);
}

/*
*******************************************************************
* Function: Scroll()
*
* Description : This API supports scrolling of an image.
*                   viewer interface related slots
*
* Parameters : int mode, int motion
*                   (mode support UP 0, DOWN 1, LEFT 2, RIGHT 3
*                    motion supports movement in pixel units)
*
* Returns : void
*
*******************************************************************
*/
void USBController::Scroll(int mode, int motion)
{
    if(m_tPlayList.count() <= 0) return;   
    
    m_pPlayer->Scroll(mode, motion);
}

/*
*******************************************************************
* Function: SaveImage()
*
* Description : It will save the specific Image after rotation of the Image.
*                   viewer interface related slots
*
* Parameters : const QString &strOldName, const QString &strNewName,
*                   const QString &destPath
*
* Returns :void
*
*******************************************************************
*/
void USBController::SaveImage(const QString &strOldName, const QString &strNewName, const QString & destPath)
{
    m_pPlayer->SaveImage(strOldName, strNewName, destPath);
}

/*
*******************************************************************
* Function: SetSlideShowDuration()
*
* Description : It will set the time duration of the slideshow for images.
*                   viewer interface related slots
*
* Parameters : int duration
*
* Returns : void
*
*******************************************************************
*/
void USBController::SetSlideShowDuration(int duration)
{
    m_nSldDuration = duration;
    m_pPlayer->SetSlideShowDuration(duration);
}

/*
*******************************************************************
* Function: HandleUSBPositionChanged()
*
* Description : slot about PositionChanged() signal
*                   controller related slots
*
* Parameters : int nPosition
*
* Returns : void
*
*******************************************************************
*/
void USBController::HandleUSBPositionChanged(int nPosition)
{
    #if USE_USB_SINGLE_INSTANCE
    if(MediaManager::GetInstance()->GetMediaMode() != m_eMyMediaMode)
    {
        if(m_eMyMediaMode == MP_MODE_USB)
        {
            MC_HIGH << "Current Media Mode is not USB" << LOG_ENDL; 
        }
        else
        {
            MC_HIGH << "Current Media Mode is not Jukebox" << LOG_ENDL;
        }        
        return;
    }
    #endif

    // { added by kihyung 2013.08.22 for ITS 0185780 
    if(m_tPlayTimer.isActive() == true)
    {
        MC_HIGH << "m_tPlayTimer.isActive() == true" << LOG_ENDL;
        return;
    }
    // { added by kihyung 2013.08.22 for ITS 0185780 

    if((m_nScanMode == SCANFILE || m_nScanMode == SCANFOLDER || m_nScanMode == SCANALL) && nPosition >= 11000)
    {
        CheckedScanMode();
    }
    else if(nPosition <= m_currentMediaDuration)
    {
        // MC_HIGH << "state:" << m_currentMediaState << "position:" << nPosition << LOG_ENDL;
        // { modified by kihyung 2013.07.18 for ITS 0179389
        if(m_currentMediaState != PLAYER_STOP)
        {
            m_currentMediaPosition = (double)nPosition;
            emit OnPositionChanged(nPosition);
        }
        // } modified by kihyung 2013.07.18 for ITS 0179389
    }
    else
    {
        MC_HIGH << "Error nPosition" << nPosition << " > m_currentMediaDuration" << m_currentMediaDuration << LOG_ENDL;
    }
}

/*
*******************************************************************
* Function: HandleUSBDurationChanged()
*
* Description : slot about DurationChanged() signal
*                   controller related slots
*
* Parameters : None
*
* Returns : void
*
*******************************************************************
*/
void USBController::HandleUSBDurationChanged()
{
    #if USE_USB_SINGLE_INSTANCE
    if(MediaManager::GetInstance()->GetMediaMode() != m_eMyMediaMode)
    {
        if(m_eMyMediaMode == MP_MODE_USB)
        {
            MC_HIGH << "Current Media Mode is not USB" << LOG_ENDL; 
        }
        else
        {
            MC_HIGH << "Current Media Mode is not Jukebox" << LOG_ENDL;
        }        
        return;
    }
    #endif

    /*
    // { added by kihyung 2012.12.06 for performance
    m_tUserReqMutex.lock();
    if(m_tUserReqTrackList.count() > 0)
    {
        int nReqTracIdx = m_tUserReqTrackList.last();
        if(nReqTracIdx == GetTrackIdx())
        {
            m_tUserReqTrackList.clear();
            if(m_tUserReqTimer.isActive() == true)
            {
                m_tUserReqTimer.stop();
            }
            MC_LOG << "USBController::HandleUSBDurationChanged(): Requested track clear" << LOG_ENDL;
        }
        else
        {
            if(m_tUserReqTimer.isActive() == true)
            {
                m_tUserReqTimer.stop();
            }
            m_tUserReqTimer.start(USER_REQ_TRACK_PLAY_INTERVAL);
            MC_LOG << "USBController::HandleUSBDurationChanged(): Reset Timer. " << nReqTracIdx << ", " << GetTrackIdx() << LOG_ENDL;
        }
    }
    m_tUserReqMutex.unlock();
    // } added by kihyung 2012.12.06 for performance
    */
    
    int nDuration = m_pPlayer->GetDuration();

    //MC_HIGH << "cursorMA" << nDuration << LOG_ENDL;

    m_currentMediaDuration = nDuration;
    
    emit OnDurationChanged(nDuration);
    
    m_bChangedDuration = true;

    // FF/REW
    MC_HIGH << "m_nFFREWRate:" << m_nFFREWRate << " Duration = " << nDuration << LOG_ENDL;
    if(m_nFFREWRate != FAST_SPEED_X1)
    {
        // FF
        if(m_bREWMode == false)
        {
            m_pPlayer->FastForward(m_nFFREWRate);
            MC_HIGH << "FF MODE." << m_nFFREWRate << m_bREWMode << LOG_ENDL;
        }
        // REW
        else
        {
            emit OnPositionChanged(nDuration - 1000);
            
            m_pPlayer->SetPosition(nDuration - 1000);
            m_pPlayer->Play();
            m_pPlayer->Rewind(m_nFFREWRate);
            MC_HIGH << "REW MODE." << nDuration << nDuration - 1000 << LOG_ENDL;
        }
    }
}

/*
*******************************************************************
* Function: HandleUSBError()
*
* Description : slot about Error() signal
*                   controller related slots
*
* Parameters : int nError
*
* Returns : void
*
*******************************************************************
*/
void USBController::HandleUSBError(int nError)
{    
    #if USE_USB_SINGLE_INSTANCE
    if(MediaManager::GetInstance()->GetMediaMode() != m_eMyMediaMode)
    {
        if(m_eMyMediaMode == MP_MODE_USB)
        {
            MC_HIGH << "Current Media Mode is not USB" << LOG_ENDL; 
        }
        else
        {
            MC_HIGH << "Current Media Mode is not Jukebox" << LOG_ENDL;
        }        
        return;
    }
    #endif
    if(m_tPlayList.count() <= 0) return;    // added by sungha.choi 2013.08.28 for ITS 0187115

    // { added by kihyung 2012.12.27 for performance
    if(nError != eMP_ERR_SUBTITLE_FILE_NOTFOUND) {
        m_tUserReqMutex.lock();
        if(m_tUserReqTrackList.count() > 0) {
            MC_HIGH << "m_tUserReqTrackList.clear()" << LOG_ENDL;
            m_tUserReqTrackList.clear();
        }
        m_tUserReqMutex.unlock();
    }
    // } added by kihyung 2012.12.27 for performance

	MC_HIGH << "nError:" << nError << LOG_ENDL; // added by kihyung 2013.12.10 for ITS 0214009

    int nNewError = nError;
    int bSendEmit = true; // added by kihyung 2013.07.22 for ITS 0179477 

    // Player Engine ERRORs
    if(nError >= eMP_ERR_UNSUPPORTED_FILE && nError < eMP_ERR_NONE) 
    {
        // { modified by kihyung 2013.12.10 for ITS 0214009 
        // if(nError == eMP_ERR_UNSUPPORTED_FILE) 

        if(nError == eMP_ERR_UNSUPPORTED_FILE)
        {
            if(m_bAutoTrackChanged)
                emit OnAutoTrackChanged(1);//added by taihyun.ahn 2014.01.04
        }
        if(nError == eMP_ERR_UNSUPPORTED_FILE || 
           nError == eMP_ERR_STREAM ||
           nError == eMP_ERR_RESOURCE || 
           nError == eMP_ERR_OTHERDATAERROR ||
           nError == eMP_ERR_AUTOAUDIOSINK_FAIL)         
        // } modified by kihyung 2013.12.10 for ITS 0214009 
        {
            if(!CheckUSBMounted()) // modified by jaehwan lee 2014.08.21 to fix ITS 245899, 245883, 245872
            {
                MC_HIGH << "USB Disconnected !!!"<< LOG_ENDL;
                return;
            }
        }
        
        if(m_nPlayableSize <= 0 && (nError == eMP_ERR_UNSUPPORTED_FILE ||
                                    nError == eMP_ERR_STREAM || 
                                    nError == eMP_ERR_FORMAT_ERROR ||
                                    nError == eMP_ERR_RESOURCE ||
                                    nError == eMP_ERR_OTHERDATAERROR ||
                                    nError == eMP_ERR_DRM_UNAUTHORIZED_FILE_DRM ||
                                    nError == eMP_ERR_DIVX_AUDIO_NOT_SUPPORTED ||
                                    nError == eMP_ERR_DIVX_VIDEO_NOT_SUPPORTED ) ) // added by kihyung 2013.08.31 for ITS 0187672
        {

            int filecount = GetFileSizeOfFolder(GetTrackIdx()) ;
            int error_count = GetErrorSizeOfFolder(GetTrackIdx());
            MC_HIGH << "m_nRepeatMode = " << m_nRepeatMode  << " m_nTotalErrSize = "
                       <<   m_nTotalErrSize << " m_tPlayList.count() = " <<  m_tPlayList.count()
                       << " filecount in folder = " << filecount << " error_count in folder = " << error_count << LOG_ENDL;
            if(m_nRepeatMode == REPEATFOLDER && filecount == error_count && m_nTotalErrSize < m_tPlayList.count())
            {
                MC_HIGH << "eMP_ERR_UNSUPPORTED_ALL_FILES_IN_FOLDER" << LOG_ENDL;
                nNewError = eMP_ERR_UNSUPPORTED_ALL_FILES_IN_FOLDER;
            }
            else 
            {
                MC_HIGH << "eMP_ERR_UNSUPPORTED_ALL_FILE" << LOG_ENDL;
                nNewError = eMP_ERR_UNSUPPORTED_ALL_FILE;
            }
        }
        else 
        {
            int nErrIdx;
            
            if(m_nRandomMode != RANDOMOFF)
            {
                nErrIdx = m_nShuffleIdx;
            }
            else
            {
                nErrIdx = m_nSetURITrackIndex;
            }

            MC_HIGH << "nErrIdx: " << nErrIdx << LOG_ENDL;

            if(nErrIdx >= 0 && nErrIdx < m_tPlayList.count()) 
            {
                int bRemoveFile = false;
                if(nError == eMP_ERR_UNSUPPORTED_FILE ||
                   nError == eMP_ERR_STREAM || 
                   nError == eMP_ERR_FORMAT_ERROR ||
                   nError == eMP_ERR_RESOURCE ||
                   nError == eMP_ERR_OTHERDATAERROR ||
                   nError == eMP_ERR_DRM_UNAUTHORIZED_FILE_DRM ||
                   nError == eMP_ERR_DIVX_AUDIO_NOT_SUPPORTED ||
                   nError == eMP_ERR_DIVX_VIDEO_NOT_SUPPORTED )  // added by kihyung 2013.08.31 for ITS 0187672
                {
                    bRemoveFile = true;
                    
                    // { modified by kihyung 2013.09.12 for ISV 90634
                    if(m_bUserPause == true)
                    {
                        if(GetCurrentMediaState() != PLAYER_PAUSE)
                        {
                            m_nSetURITrackIndex = -1;
                            EmitOnStateChanged(PLAYER_CHANGE_PAUSE_ICON);
                        }
                        bSendEmit = false;
                    }
                    else if(GetCurrentMediaState() != PLAYER_PLAY)
                    {
                        EmitOnStateChanged(PLAYER_CHANGE_PLAY_ICON);
                    }
                    // } modified by kihyung 2013.09.12 for ISV 90634
                }

                if(bRemoveFile == true) 
                {
                    if(m_pShuffleList[nErrIdx].bError == 0) 
                    {
                        m_nPlayableSize--;
                        m_pShuffleList[nErrIdx].bError = 1;
                        MC_HIGH << "nErrIdx:" << nErrIdx << ", m_nPlayableSize:" << m_nPlayableSize << LOG_ENDL;

                        if(m_nTotalErrSize < m_tPlayList.count())
                        {
                            m_nTotalErrSize++;
                            SetErrorSizeofFolder(GetTrackIdx(), GetErrorSizeOfFolder(GetTrackIdx()) + 1);
                        }
                    }
                    //else
                    //{
                    //    MC_HIGH << "nErrIdx: " << nErrIdx << LOG_ENDL;
                    //}
                }
            }            
        }

        if(nError == eMP_ERR_AUTOAUDIOSINK_FAIL)
        {
            m_nSetURITrackIndex = -1;
            //usleep(600000); // removed by kihyung 2013.07.04 for ISV 85455
        }
    }

    // { modified by kihyung 2013.12.05 for ITS 0212166  
    /*
    MC_HIGH << "nError:" << nError << ", bSendEmit:" << bSendEmit << LOG_ENDL;
    // modified by kihyung 2013.07.22 for ITS 0179477     
    if(bSendEmit == true) {
        emit OnError(nNewError);
    }
    */
    // removed by kihyung 2013.12.10
    emit OnError(nNewError);
    // } modified by kihyung 2013.12.05 for ITS 0212166  
}

/*
*******************************************************************
* Function: HandleUSBStateChanged()
*
* Description : slot about StateChanged() signal
*                   controller related slots
*
* Parameters : int nState
*
* Returns : void
*
*******************************************************************
*/
void USBController::HandleUSBStateChanged(int nState)
{
    #if USE_USB_SINGLE_INSTANCE
    if(MediaManager::GetInstance()->GetMediaMode() != m_eMyMediaMode) {
        if(m_eMyMediaMode == MP_MODE_USB) {
            MC_HIGH << "Current Media Mode is not USB") << LOG_ENDL; 
        }else{
            MC_HIGH << "Current Media Mode is not Jukebox") << LOG_ENDL;
        }        
        return;
    }
    #endif

    int nPrevState = m_currentMediaState;
    int nCurrState = m_currentMediaState;
    m_nEngineMediaState = nState;

    switch(nState)
    {
        case eMP_STATE_PLAYING:
            if(m_bControllerChanged == true || m_bEmitTrackChanged == true)
            {
                m_bControllerChanged = false;
                m_bEmitTrackChanged  = false;
                // { added by kihyung 2013.06.19 for ITS 0174795, 0174794 
                m_tUserReqMutex.lock();
                if(m_tUserReqTrackList.count() > 0)
                {
                    int nTrackIndex = m_tUserReqTrackList.at(m_tUserReqTrackList.count() - 1);
                    if(nTrackIndex == GetTrackIdx())
                    {
                        MC_HIGH << "m_tUserReqTrackList.clear()" << LOG_ENDL;
                        m_tUserReqTrackList.clear();
                        m_tUserReqTimer.stop();
                    }
                    else
                    {
                        MC_HIGH << "nTrackIndex" << nTrackIndex << "!= GetTrackIdx()" << GetTrackIdx() << LOG_ENDL;
                    }
                }
                m_tUserReqMutex.unlock();
                // } added by kihyung 2013.06.19    

                //if(m_nSetURITrackIndex == GetTrackIdx())
                //modified by jaehwan.lee 2014.3.21 to fix ITS 229885
                if(m_nSetURITrackIndex == GetTrackIdx() || m_bSameSong)
                {    // added by sungha.choi 2013.07.29
                    EmitOnTrackChanged(GetTrackIdx());
                    CheckMediaInfoSignal(); // added by kihyung 2013.12.12 for ITS 0214770 
                }
            }
            
            if(m_nFFREWRate != FAST_SPEED_X1)
            {
                if(m_bREWMode == false)
                    nCurrState = PLAYER_FASTFORWARD;
                else
                    nCurrState = PLAYER_REWIND;
            }
            else
            {
                nCurrState = PLAYER_PLAY;
            }
            break;
        case eMP_STATE_PAUSED:
            nCurrState = PLAYER_PAUSE;
            // { modified by kihyung 2012.12.10
            /*
            if(m_nUserPlayPosition > 0)
            {
                int nNewStartPos = m_nUserPlayPosition;
                m_nUserPlayPosition = 0;
                m_pPlayer->SetPosition(nNewStartPos);
                MC_LOG << "USBController::HandleUSBStateChanged(): m_nUserPlayPosition: " << nNewStartPos << LOG_ENDL;
            }
            */
            // } modified by kihyung 2012.12.10            
            break;
        case eMP_STATE_STOPPED:
            nCurrState = PLAYER_STOP;
            // removed by kihyung 2013.11.04
            // m_nSetURITrackIndex = -1; // added by kihyung 2013.09.09 for ITS 0188773 
            break;
        case eMP_STATE_SEARCHING:
            nCurrState = PLAYER_SEARCH;
            break;
        default:
            // MC_LOG << "[Error]USBController::HandleUSBStateChanged state = " << nState << LOG_ENDL;
            break;
    }

    MC_HIGH << "PE state:" << nState <<"-> ( Prev:" << nPrevState << "-> Curr:" << nCurrState << ")" << LOG_ENDL; 

    if(nPrevState != nCurrState) 
    {
        // IF SCAN AND PLAYER_STOP-->OMIT
        if(nCurrState == PLAYER_STOP && m_nScanMode != SCANOFF)
        {
            MC_HIGH << "omitted PLAYER_STOP in scan mode" << m_nScanMode << LOG_ENDL;
        }
        // IF FF/REW AND PLAYER_STOP-->OMIT
        else if(nCurrState == PLAYER_STOP && m_nFFREWRate != FAST_SPEED_X1)
        {
            MC_HIGH << "omitted PLAYER_STOP in FF/REW mode" << m_nFFREWRate << LOG_ENDL;
        }
        // IN THE PLAYING, OMIT PLAYER_STOP
        else if(m_tPlayList.count() > 0 && nState == eMP_STATE_STOPPED && m_bUserStop == false)
        {
            MC_HIGH << "omitted PLAYER_STOP in Normal Playback" << LOG_ENDL;
            m_currentMediaState = nCurrState;
        }
        else if(m_bSkipPauseSignal == true && nState == eMP_STATE_PAUSED)
        {
            m_currentMediaState   = nCurrState; // modified by kihyung 2013.07.22 for ITS 0181149
            //m_bSkipPauseSignal  = false;
            MC_HIGH << "omitted PLAYER_PAUSE in Normal Playback" << LOG_ENDL;
        }
        // { added by kihyung 2014.01.07 for ITS 0217537 
        else if(m_bUserPause == true && nCurrState == PLAYER_PLAY && m_pPlayer->getPipeline() == true)
        {
            MC_HIGH << "omitted PLAYER_PLAY after PAUSE on no pipeline" << LOG_ENDL;
            m_currentMediaState   = nCurrState; // Added by sangmin.seol 2014.01.21 ITS 0221546, 0221544
            ClearUserReqTrack();
            m_bSkipPauseSignal = false;
            m_pPlayer->Pause();
            MC_HIGH << "m_pPlayer->Pause()" << LOG_ENDL;
        }
        // } added by kihyung 2014.01.07 for ITS 0217537         
        else
        {
            EmitOnStateChanged(nCurrState);
            m_bPlayAfterFFREW  = false;
            m_bSkipPauseSignal = false;
        }

        // { removed by kihyung 2014.01.07
        /*
        // { added by kihyung 2013.10.02 for ITS 0188100 
        if(m_bUserPause == true && nCurrState == PLAYER_PLAY)
        {
            if(m_pPlayer->getPipeline() == true) 
            {
                ClearUserReqTrack();
                m_bSkipPauseSignal = false;
                m_pPlayer->Pause();
                MC_HIGH << "m_pPlayer->Pause()" << LOG_ENDL;
            }
        }
        // } added by kihyung 2013.10.02 for ITS 0188100 
        */
        // } removed by kihyung 2014.01.07        
    }
    // { modified by kihyung 2013.12.03 for 0210751
    else 
    {
        /*
        // { added by edo.lee 2013.10.13 send state when auto next track
         << LOG_ENDL; << "omitted PLAYER_PLAY in Auto next m_nScanMode "<<m_nScanMode << LOG_ENDL;
        if((m_bAutoTrackChanged &&  nCurrState == PLAYER_PLAY) || m_nScanMode != SCANOFF)
        {
            m_bAutoTrackChanged = false;
            EmitOnStateChanged(PLAYER_PLAY);
        }
        */
        if(nCurrState == PLAYER_PLAY && (m_nScanMode != SCANOFF || m_bAutoTrackChanged == true))
        {
            EmitOnStateChanged(PLAYER_PLAY);
        }
    }
    // } modified by kihyung 2013.12.03 for 0210751    
}

/*
*******************************************************************
* Function: HandleTrackChange()
*
* Description : slot about ChangeTrack() signal
*                   controller related slots
*
* Parameters : None
*
* Returns : void
*
*******************************************************************
*/
void USBController::HandleTrackChange()
{
    #if USE_USB_SINGLE_INSTANCE
    if(MediaManager::GetInstance()->GetMediaMode() != m_eMyMediaMode)
    {
        if(m_eMyMediaMode == MP_MODE_USB)
        {
            MC_HIGH << "Current Media Mode is not USB" << LOG_ENDL; 
        }
        else
        {
            MC_HIGH << "Current Media Mode is not Jukebox" << LOG_ENDL;
        }
        return;
    }
    #endif

    m_bAutoTrackChanged = true;
    
    int bAutoTrachChanged = m_bAutoTrackChanged;

    // { added by kihyung 2013.06.07 for ITS 0172711
    m_tUserReqMutex.lock();
    if(m_tUserReqTrackList.count() > 0)
    {
        int nTrackIndex = m_tUserReqTrackList.at(m_tUserReqTrackList.count() - 1);
        if(nTrackIndex == GetTrackIdx())
        {
            MC_HIGH << "m_tUserReqTrackList.clear()" << LOG_ENDL;
            m_tUserReqTrackList.clear();
            m_tUserReqTimer.stop();
        }
        else
        {
            MC_HIGH << "nTrackIndex" << nTrackIndex << "!= GetTrackIdx" << GetTrackIdx() << LOG_ENDL;
        }
    }
    m_tUserReqMutex.unlock();
    // } added by kihyung 2013.06.07    

    MEDIA_TYPE eMediaType = m_pPlayer->GetMediaType();
    
    MC_HIGH << "m_currentMediaState = " << m_currentMediaState << " m_nFFREWRate = " << m_nFFREWRate << LOG_ENDL;

    if(m_bSldShowMode == false && eMediaType != IMAGE_FILE) // when slide show mode stop
    {
        // { added by jaehwan lee 2014.08.21 to fix ITS 245899, 245883, 245872
        if (!CheckUSBMounted())
        {
            MC_HIGH << "USB Disconnected !!!" << LOG_ENDL;
            return;
        }
        // } added by jaehwan.lee

        if(m_currentMediaDuration <= 11000)
        {
            if(m_nScanMode != SCANOFF)
            {
                if(CheckedScanMode() == true)
                {
                    return;
                }
            }
        }
        
        if(m_currentMediaState != PLAYER_STOP)
        {
            MC_HIGH << "PLAYER_EndOfMedia" << LOG_ENDL;
            emit OnCurrentMediaStatusChanged(PLAYER_EndOfMedia);
            //remove by edo.lee 2013.10.04
        }

        if(m_nRepeatMode == REPEATFILE && m_nFFREWRate != FAST_SPEED_X1)
        {
            MC_HIGH << "FF or REW on REPEATFILE--->REPEATALL" << LOG_ENDL;
            m_nRepeatMode = REPEATALL;
            emit OnModeChanged(m_nRepeatMode);
        }

        m_nSetURITrackIndex = -1; // added by kihyung 2013.07.26
        m_nUserPlayPosition = 0; // added by kihyung 2013.09.09 for ITS 0188880 

        // FF/REW
        if(m_nFFREWRate != FAST_SPEED_X1) 
        {
            // { removed by kihyung 2013.06.05 for ISV 79686
            /*
            // { added by jeeeun for MLT play in FF/REW 2012.12.05
            if(m_bisMLT == true)
            { 
                Play();
                EmitOnTrackChanged(GetTrackIdx());                
            }
            // } added by jeeeun for MLT play in FF/REW 2012.12.05
            */
            // } removed by kihyung 2013.06.05
            // FF
            if(m_bREWMode == false)
            {
                MC_HIGH << "FF MODE" << LOG_ENDL;
                if(m_nRandomMode != RANDOMOFF)
                {
                    SetTrackIdx(GetRandomTrackIdx(true));
                }
                else
                {
                    SetTrackIdx(GetNextTrackIdx());
                }
                SetURI(GetTrackIdx());
                if(m_pPlayer->IsDivXDRMSupported(&divxDrmType , &divxDrmLimitCnt,&divxDrmUseCnt))
                {
                    MC_HIGH << "USBController::Play() : DivxDRM is Supported , use DRM Play for Actual Playing File on User Input" << LOG_ENDL;
                }
                else
                {
                    m_pPlayer->Play();
                }

                EmitOnTrackChanged(GetTrackIdx(), bAutoTrachChanged);
                EmitOnStateChanged(PLAYER_FASTFORWARD);
            }
            // REW
            else
            {
                MC_HIGH << "REW MODE" << LOG_ENDL;
                // REW -> NORMAL PLAY IN FIRST FILE
                //remove by edo.lee 2013.10.04
                if(m_nRepeatMode == REPEATFOLDER && GetFileIdxInFolder(GetTrackIdx()) == 0)
                {
                    Play();
                    EmitOnTrackChanged(GetTrackIdx(), bAutoTrachChanged);  
                    m_bAutoTrackChanged = false;   // added for ITS 0178515                    
                    emit OnNormalPlay();
                }
                // REW -> NORMAL PLAY IN FIRST FILE  
                else if(m_nRepeatMode == REPEATALL && GetTrackIdx() == 0)
                {
                    Play();
                    EmitOnTrackChanged(GetTrackIdx(), bAutoTrachChanged);
                    m_bAutoTrackChanged = false;  // added for ITS 0178515
                    emit OnNormalPlay();
                }
                // REW -> REW
                else
				{
                    if(m_nRandomMode != RANDOMOFF)
                    {
                        SetTrackIdx(GetRandomTrackIdx(false));
                    }
                    else
                    {
                        SetTrackIdx(GetPrevTrackIdx());
                    }
                    
                    SetURI(GetTrackIdx());
                    
                    EmitOnTrackChanged(GetTrackIdx(), bAutoTrachChanged);
                    EmitOnStateChanged(PLAYER_REWIND);
                }
            }

            m_bChangedDuration = false;
        }
        // REPEAT FILE
        else if(m_nRepeatMode == REPEATFILE)
        {
            Play();
            EmitOnTrackChanged(GetTrackIdx(), bAutoTrachChanged);
            //remove by edo.lee 2013.11.18
        }
        // REPEAT FOLDER/ALL
        else 
        {
            if(m_nRandomMode == RANDOMOFF)
            {
                SetTrackIdx(GetNextTrackIdx());
            }
            else
            {
                SetTrackIdx(GetRandomTrackIdx(true));
            }

            m_bSkipPauseSignal = true;
            Play();
            EmitOnTrackChanged(GetTrackIdx(), bAutoTrachChanged);  
            //remove by edo.lee 2013.11.18
            m_bChangedDuration = false;
        }

        CheckMediaInfoSignal(); // added by kihyung 2013.12.12 for ITS 0214770 
    }
    else if(m_bSldShowMode == true) // slide show mode start
    {
        SetTrackIdx(GetNextTrackIdx());
        MC_HIGH << "Slide show track changed" << GetTrackIdx() << LOG_ENDL;
        EmitOnTrackChanged(GetTrackIdx(), bAutoTrachChanged);
    }
}

/*
*******************************************************************
* Function: HandleUSBMediaStatusChanged()
*
* Description : slot about MediaStatusChanged() signal
*                   controller related slots
*
* Parameters : int nMediaStatus
*
* Returns : void
*
*******************************************************************
*/
void USBController::HandleUSBMediaStatusChanged(int nMediaStatus) // not used
{
    Q_UNUSED(nMediaStatus);
    #if USE_USB_SINGLE_INSTANCE
    if(MediaManager::GetInstance()->GetMediaMode() != m_eMyMediaMode)
    {
        MC_HIGH << "Current Media Mode is not USB" << LOG_ENDL;        
        return;
    }
    #endif
}

// added by sungha.choi 2013.07.01
void USBController::HandlePlayTimer()
{
    if(GetCurrentMediaState() != PLAYER_STOP || m_pPlayer->getPipeline() == true) // { added by sungha.choi 2013.08.29
    {
        m_pPlayer->Stop();
    } // } added by sungha.choi 2013.08.29

    // MEDIA_TYPE eMediaType       = m_pPlayer->GetMediaType(); // removed by kihyung 2013.12.13 for smoke test

    if(m_nRepeatMode == REPEATFILE )
    {// && !m_nPrevForced) {//modified by edo.lee 2013.11.20
        m_nRepeatMode = REPEATALL;
        emit OnModeChanged(m_nRepeatMode);
    }

    // SCAN -> OFF
    if(m_nScanMode != SCANOFF && !m_nPrevForced)
    {//added by edo.lee 2013.10.12 pop noise
        Scan(SCANOFF);
    } 

    SetURI(GetTrackIdx(), true);

    if(m_pPlayer->IsDivXDRMSupported(&divxDrmType , &divxDrmLimitCnt,&divxDrmUseCnt))
    {
        MC_HIGH << "USBController::Play() : DivxDRM is Supported , use DRM Play for Actual Playing File on User Input" << LOG_ENDL;
    }
    else
    {
        m_pPlayer->Play();
    }

    // { modified by kihyung 2013.12.13 for smoke test
    /*
    if(m_nPrevForced != 1 && (eMediaType == AUDIO_FILE || eMediaType == VIDEO_FILE))  // When Next Play, m_nPrevForced is always 0, so this condition will be omit.
    {
        m_bEmitTrackChanged = true;
    }
    else
    {
        if(m_nSetURITrackIndex == GetTrackIdx() && !m_nPrevForced)  //added by edo.lee 2013.10.12 pop noise
        {
            EmitOnTrackChanged(GetTrackIdx());
            CheckMediaInfoSignal();
        }
    }
    */
    m_bEmitTrackChanged = true;
    // } modified by kihyung 2013.12.13 for smoke test
}

void USBController::HandleUserReqTrack()
{
    MC_HIGH << LOG_ENDL;

    int nNewTrackIdx = -1;
    m_tUserReqMutex.lock();
    if(m_tUserReqTrackList.count() > 0) 
    {
        nNewTrackIdx = m_tUserReqTrackList.takeLast();
        m_tUserReqTrackList.clear();
        if(nNewTrackIdx < 0 || nNewTrackIdx >= m_tPlayList.count())
        {
            nNewTrackIdx = -1;
        }
    }
    m_tUserReqTimer.stop();
    m_tUserReqMutex.unlock();

    if(nNewTrackIdx >= 0)
    {
        MC_HIGH << "Last Track Index:" << nNewTrackIdx << LOG_ENDL;
        SetCurrentIndex(nNewTrackIdx);
        m_pPlayer->Stop();
        m_nSetURITrackIndex = -1;
        Play();
    }
}

void USBController::HandleVideoInfo(int type, int status, int width, int height, float par, float dar)
{
    m_nVideoType    = type;
    m_nVideoStatus  = status;
    m_nVideoWidth   = width;
    m_nVideoHeight  = height;
    m_nVideoPar     = par;
    m_nVideoDar     = dar;
    
    emit OnVideoInfo(m_nVideoType, m_nVideoStatus, m_nVideoWidth, m_nVideoHeight, m_nVideoPar, m_nVideoDar, GetCurrentFileName());// modified for ITS 224766 2014.02.12
}

void USBController::HandleCurrentMediaInfo(MediaInfo *pMediaInfo)
{
    // added by kihyung 2013.12.12 for ITS 0214770 
    if(pMediaInfo) 
    {
        m_tMediaInfo.tInfo  = *pMediaInfo; 
        m_tMediaInfo.bValid = true;
    }
    
    int bSend = false;
    m_tUserReqMutex.lock();
	//{remove by edo.lee 2013.10.22 for sending correct media info 
    //if(m_nSetURITrackIndex <=0)//m_tUserReqTrackList.size() <= 0) 
    //{
    //    bSend = true;
    //}
    //else/
    //{
        //int nTrackIndex = m_tUserReqTrackList.at(m_tUserReqTrackList.size() - 1);
        //{modified by kihyung 2014.01.06 for ITS 0217528 
        //if(m_nSetURITrackIndex == GetCurrentIndex())
        //{
        if(m_nSetURITrackIndex == GetCurrentIndex() || (m_bSameSong == true && m_nSetURITrackIndex == -1))
        {
            bSend = true;
        }
        //}modified by kihyung 2014.01.06 for ITS 0217528        
    //}
    m_tUserReqMutex.unlock();

    if(bSend == true || m_nScanMode != SCANOFF || m_nFFREWRate != FAST_SPEED_X1)
    {
        // { modified by kihyung 2013.12.12 for ITS 0214770 
        // emit OnCurrentMediaInfo(pMediaInfo);
        if(m_tMediaInfo.bValid == true && m_tMediaInfo.bTrackChanged == true) 
        {
            MC_HIGH << "emit OnCurrentMediaInfo..." << LOG_ENDL;
            m_tMediaInfo.bValid = false;
            emit OnCurrentMediaInfo(&m_tMediaInfo.tInfo);
        }
        // } modified by kihyung 2013.12.12 for ITS 0214770 
    }
}

void USBController::HandleRealSoundChanged()
{
    MC_HIGH << "m_bAutoTrackChanged:" << m_bAutoTrackChanged << ", m_bSystemRequestMute" << m_bSystemRequestMute << LOG_ENDL;
    
    if(m_bAutoTrackChanged == true)
    {
        m_bAutoTrackChanged = false;
        m_bSystemRequestMute = false;
    }
    else
    {
        if(m_bSystemRequestMute == true)
        {
            m_bSystemRequestMute = false;
        }
        else
        {
            emit OnRealSoundChanged();          
//remove by edo.lee 2013.10.04
        }
    }
}

/*
*******************************************************************
* Function: HandleFileDeleted()
*
* Description : slot about OnFileDeleted() signal
*                   control file interface signal
*
* Parameters : int nIndex, qint64 nDiscSize
*
* Returns : void
*
*******************************************************************
*/
void USBController::HandleFileDeleted(int nIndex, qint64 nDeletedSize)
{
    Q_UNUSED(nIndex);
    qint64 nTotalFileSize = GetFileManager()->GetTotalFileSpace();
    int    nPercent       = 0;
    double fRatio;
    
    if(nTotalFileSize > 0)
    {
        fRatio   = (double)nDeletedSize / (double)nTotalFileSize;
        nPercent = (int)(fRatio * 100.0);
    }

    emit OnPercentageAndSizeDeleted(nPercent, nDeletedSize);
}

void USBController::HandleFileDeleted2(int nIndex, qint64 nDeletedSize, QString strName)
{
    Q_UNUSED(nIndex);
    qint64 nTotalFileSize = GetFileManager()->GetTotalFileSpace();
    int    nPercent       = 0;
    double fRatio;
    
    if(nTotalFileSize > 0)
    {
        fRatio   = (double)nDeletedSize / (double)nTotalFileSize;
        nPercent = (int)(fRatio * 100.0);
    }

    emit OnPercentageAndSizeAndNameDeleted(nPercent, nDeletedSize, strName);
}

/*
*******************************************************************
* Function: HandleFileCopied()
*
* Description : slot about OnFileCopied() signal
*                   control file interface signal
*
* Parameters : int nIndex, qint64 nDiscSize
*
* Returns : void
*
*******************************************************************
*/
void USBController::HandleFileCopied(int nIndex, qint64 nCopiedSize)
{
    //Q_UNUSED(nIndex);
    qint64 nTotalFileSize = GetFileManager()->GetTotalFileSpace();
    int    nPercent       = 0;
    double fRatio;
    
    if(nTotalFileSize > 0)
    {
        fRatio   = (double)nCopiedSize / (double)nTotalFileSize;
        nPercent = (int)(fRatio * 100.0);
    }
    
    emit OnPercentageAndSizeCopied(nPercent, nCopiedSize, nIndex + 1); // modified by kihyung 2013.09.13 for ISV 91171
}

/*
*******************************************************************
* Function: HandleFileMoved()
*
* Description : slot about OnFileMoved() signal
*                   control file interface signal
*
* Parameters : int nIndex, qint64 nDiscSize
*
* Returns : void
*
*******************************************************************
*/
void USBController::HandleFileMoved(int nIndex, qint64 nMovedSize)
{
    Q_UNUSED(nIndex);
    qint64 nTotalFileSize = GetFileManager()->GetTotalFileSpace();
    int    nPercent       = 0;
    double fRatio;
    
    if(nTotalFileSize > 0)
    {
        fRatio   = (double)nMovedSize / (double)nTotalFileSize;
        nPercent = (int)(fRatio * 100.0);
    }
    
    emit OnPercentageAndSizeMoved(nPercent, nMovedSize);
}

void USBController::HandleFileCopiedPercentage(int nIndex, int nPercentage)
{
    emit OnFileCopiedPercentage(nIndex + 1, nPercentage); // modified by kihyung 2013.09.13 for ISV 91171
}

void USBController::HandleFileMovedPercentage(int nIndex, int nPercentage)
{
    emit OnFileMovedPercentage(nIndex + 1, nPercentage); // modified by kihyung 2013.09.13 for ISV 91171
}

/*
*******************************************************************
* Function: HandleCreateFileFinished()
*
* Description : slot about OnCreateFinished() signal
*                   control file interface signal
*
* Parameters : int bSuccess
*
* Returns : void
*
*******************************************************************
*/
void USBController::HandleCreateFileFinished(int bSuccess)
{
    emit OnCreateFolder(bSuccess);
}

/*
*******************************************************************
* Function: HandleRenameFinished()
*
* Description : slot about OnRenameFinished() signal
*                   control file interface signal
*
* Parameters : int bSuccess
*
* Returns : void
*
*******************************************************************
*/
void USBController::HandleRenameFinished(int bSuccess, int bFolder)
{
    if(bFolder == true)
    {
        emit OnEditFolder(bSuccess);
    }
    else
    {
        emit OnEditFile(bSuccess);
    }
}

/*
*******************************************************************
* Function: HandleDeleteFinished()
*
* Description : slot about OnDeleteFinished() signal
*                   control file interface signal
*
* Parameters : int bSuccess
*
* Returns : void
*
*******************************************************************
*/
void USBController::HandleDeleteFinished(int bSuccess, int nMode, int nVolume)
{
    MEDIA_TYPE eMediaType;

    if(nMode == FileManagerThread::FC_COPY_VIDEO)       eMediaType = VIDEO_FILE;
    else if(nMode == FileManagerThread::FC_COPY_MUSIC)  eMediaType = AUDIO_FILE;
    else if(nMode == FileManagerThread::FC_COPY_IMAGE)  eMediaType = IMAGE_FILE;
    else                                                eMediaType = UNSUPPORTED_FILE;

    emit OnTotalFileDeleted(bSuccess, eMediaType, nVolume);
}

/*
*******************************************************************
* Function: HandleMoveFinished()
*
* Description : slot about OnMoveFinished() signal
*                   control file interface signal
*
* Parameters : int bSuccess
*
* Returns : void
*
*******************************************************************
*/
void USBController::HandleMoveFinished(int bSuccess, int nMode)
{
    MEDIA_TYPE eMediaType;

    if(nMode == FileManagerThread::FC_COPY_VIDEO)       eMediaType = VIDEO_FILE;
    else if(nMode == FileManagerThread::FC_COPY_MUSIC)  eMediaType = AUDIO_FILE;
    else if(nMode == FileManagerThread::FC_COPY_IMAGE)  eMediaType = IMAGE_FILE;
    else                                                eMediaType = UNSUPPORTED_FILE;
    
    emit OnTotalFileMoved(bSuccess, eMediaType);
}

/*
*******************************************************************
* Function: HandleCopyFinished()
*
* Description : slot about OnCopyFinished() signal
*                   control file interface signal
*
* Parameters : int bSuccess
*
* Returns : void
*
*******************************************************************
*/
// { modified by ravikanth 22-04-13
void USBController::HandleCopyFinished(int bSuccess, int nSkipCount, int nFilesCount, int nMode, int nVolume,  int nCurrentReqMode, int nCurrentReqVolume)
{
    MEDIA_TYPE eMediaType;

    if(nMode == FileManagerThread::FC_COPY_VIDEO)       eMediaType = VIDEO_FILE;
    else if(nMode == FileManagerThread::FC_COPY_MUSIC)  eMediaType = AUDIO_FILE;
    else if(nMode == FileManagerThread::FC_COPY_IMAGE)  eMediaType = IMAGE_FILE;
    else                                                eMediaType = UNSUPPORTED_FILE;

    MC_HIGH << bSuccess << nSkipCount << nFilesCount << eMediaType << nVolume << LOG_ENDL;
    
    MEDIA_TYPE eCurrentRequestMediaType;
    if(nCurrentReqMode == FileManagerThread::FC_COPY_VIDEO)      eCurrentRequestMediaType = VIDEO_FILE;
    else if(nCurrentReqMode == FileManagerThread::FC_COPY_MUSIC) eCurrentRequestMediaType = AUDIO_FILE;
    else if(nCurrentReqMode == FileManagerThread::FC_COPY_IMAGE) eCurrentRequestMediaType = IMAGE_FILE;
    else                                                         eCurrentRequestMediaType = UNSUPPORTED_FILE;

    emit OnTotalFileCopied(bSuccess, nSkipCount, nFilesCount, eMediaType, nVolume, eCurrentRequestMediaType, nCurrentReqVolume);
}
// } modified by ravikanth 22-04-13

void USBController::HandleFormatFinished(int bSuccess)
{
    emit OnFormatFinished(bSuccess);
}

/*
*******************************************************************
* Function: HandleUsbAttached()
*
* Description : attached USB device slot
*                   control file interface signal
*
* Parameters : QDBusObjectPath aDBUSObjectPath
*
* Returns : void
*
*******************************************************************
*/
void USBController::HandleUsbAttached(QDBusObjectPath aDBUSObjectPath)
{
    Q_UNUSED(aDBUSObjectPath);
#if 0    
    PT_START_TIME(PT_OBJ_NAME_USB_CONNECT, 0);

    QString objectpath = aDBUSObjectPath.path();

    QDBusInterface tDeviceInterface("org.freedesktop.UDisks",
                                    objectpath,
                                    "org.freedesktop.UDisks.Device",
                                    QDBusConnection::systemBus());

    bool deviceIsPartition = tDeviceInterface.property("DeviceIsPartition").toBool();
    if(deviceIsPartition)
    {
        // MC_LOG << " the device has partition" << LOG_ENDL;
    }
    else
    {
        // MC_LOG << " the device don't have partition" << LOG_ENDL;
        return;
    }
    
    bool deviceIsMounted = tDeviceInterface.property("DeviceIsMounted").toBool();
    #if 0
    if(deviceIsMounted)
        MC_LOG << " the device is mounted " << LOG_ENDL;
    else
    {
        MC_LOG << " the device is not mounted " << LOG_ENDL;
    }
    #endif

    QString mountPath;
    QString serialNum = tDeviceInterface.property("DriveSerial").toString();

    if(deviceIsMounted == true)
    {
        QStringList mountPaths = tDeviceInterface.property("DeviceMountPaths").toStringList();
        
        if(mountPaths.size() >= 1) 
        {
            mountPath = mountPaths.at(0);
        }
    }

    if(m_tUSBObjectPath.contains(objectpath) == true && m_tUSBSerialNumber.contains(serialNum) == true)
    {
        // MC_LOG << " already exist " << qPrintable(objectpath) << " " <<  qPrintable(mountPath) << " " << qPrintable(serialNum) << LOG_ENDL;
    }
    else
    {
        m_tUSBObjectPath   += objectpath;
        m_tUSBMountPath    += mountPath;
        m_tUSBSerialNumber += serialNum;
        MC_LOG << " append " << qPrintable(objectpath) << " " <<  qPrintable(mountPath) << " " << qPrintable(serialNum) << LOG_ENDL;
    }
    
    m_currentDeviceState = USB_CONNECTED;

    PT_END_TIME(PT_OBJ_NAME_USB_CONNECT, 0);
    
    emit OnDeviceConnected();

    // Update FileManager
    if(m_bConnectFMSignal == true)
    {
        GetFileManager()->SetUpdatedStorage(false);
    }
#endif    
}

/*
*******************************************************************
* Function: HandleUsbDetached()
*
* Description : detached USB device slot
*                   control file interface signal
*
* Parameters : QDBusObjectPath aDBUSObjectPath
*
* Returns : void
*
*******************************************************************
*/
void USBController::HandleUsbDetached(QDBusObjectPath aDBUSObjectPath)
{
    Q_UNUSED(aDBUSObjectPath);
#if 0        
    MC_LOG << "USBController::HandleUsbDetached()" << LOG_ENDL;

    QString objectpath = aDBUSObjectPath.path();
    
    int nIndex = m_tUSBObjectPath.lastIndexOf(objectpath);
    if(nIndex >= 0) 
    {
        m_tUSBObjectPath.removeAt(nIndex);
        m_tUSBMountPath.removeAt(nIndex);
        m_tUSBSerialNumber.removeAt(nIndex);
        MC_LOG << " removed " <<  qPrintable(objectpath) << LOG_ENDL;

        m_currentDeviceState = USB_DISCONNECTED;
        emit OnDeviceDisConnected();

        // Update FileManager
        if(m_bConnectFMSignal == true)
        {
            GetFileManager()->SetUpdatedStorage(false);
        }
    }
    else 
    {
        MC_LOG << " can not remove " << qPrintable(objectpath) << LOG_ENDL;
    }
#endif    
}

void USBController::HandleFMError(int nError, QString strFileName)
{
    MC_HIGH << nError << LOG_ENDL;
    
    bool bSend = true;
    if(nError == eMP_CORRUPT_FILE_COPY_ERROR)
    {
        QFileInfo tFile(strFileName);
        if(tFile.exists() == false)
        {
            MC_HIGH << strFileName << "does not exist" << LOG_ENDL;
            bSend = false;
        }
    }

    if(bSend)
    {
        emit OnError(nError, strFileName);
    }
}

/*
*******************************************************************
* Function: USBController
*
* Description : USBController Constructor
*
* Parameters : QObject *parent
*
* Returns : void
*
*******************************************************************
*/
USBController::USBController(MEDIA_PLAYER_MODE eMode, QObject *parent) : QObject(parent)
    ,m_pGPlayer(0)
    ,m_pMPlayer(0)
    ,m_bConnectFMSignal(false)
    ,m_playlistName("")
    ,m_currentMediaFileName("")
    ,m_currentMediaState(PLAYER_STOP)
    ,m_nEngineMediaState(eMP_STATE_NONE)
    ,m_currentMediaPosition(0)
    ,m_currentMediaIndex(0)
    ,m_currentMediaDuration(0)
    ,m_currentDeviceState(USB_DISCONNECTED)
    ,m_slideShowDurationTime(0)
    ,m_systemState(0)
    ,m_brightness(0)
    ,m_contrast(0)
    ,m_tint(0)
    ,m_nSetURITrackIndex(-1)
    ,m_nCurrTrackIndex(-1)
    ,m_bFirstPlay(false)
    ,m_nDeviceMountAtBoot(0)
    ,m_nRepeatMode(REPEATALL)
    ,m_nRandomMode(RANDOMOFF)
    ,m_nScanMode(SCANOFF)
    ,m_bSldShowMode(false)
    ,m_nSldDuration(5)
    ,m_bUserStop(false)
    ,m_bUserPause(false)  // added by kihyung 2013.07.22 for ITS 0179477 
    ,m_bPrevDirection(false)
    ,m_nFFREWRate(FAST_SPEED_X1)
    ,m_bREWMode(false)
    ,m_eMyMediaMode(MP_MODE_NONE)
    ,m_bConnectGSignal(false)
    ,m_bConnectMSignal(false)
    ,m_pQDBus(0)
    ,m_bBootWithUSB(false)
    ,m_nUserPlayPosition(0)
    ,m_nPLCategory(PL_CATEGORY_NONE)
    ,m_bControllerChanged(false)
    ,m_nClearDisplay(-1)
    ,m_bEmitTrackChanged(false)
    ,m_bPlayAfterFFREW(false)
    // ,m_bisMLT(false)// removed by kihyung 2013.07.18 for ITS 0180268
    ,m_tUserReqMutex(QMutex::Recursive)
    ,m_nPrevForced(0) // added by sungha.choi 2013.07.01
    ,m_nVideoType(-1)
    ,m_bUSB1Connect(false)
    ,m_bUSB2Connect(false)
    ,m_bSendMetaInfo(true)
    ,m_bSkipPauseSignal(true)
    ,m_bAutoTrackChanged(false)
    ,m_bSystemRequestMute(false)
    ,m_bBTCallStart(false)
    ,m_bAudioDragStart(false)
//remove by edo.lee 2013.10.04
    // ,m_nRandomModeBeforeScan(RANDOMOFF) // added by kihyung 2013.07.17 for ITS 0180420 // removed by kihyung 2013.09.17 for HMC new spec
    ,divxDrmType(0)
    ,divxDrmLimitCnt(0)
    ,divxDrmUseCnt(0)
    ,m_pDrmConfirmPlay(false)
    ,m_bChangedDuration(false)
{
    m_pShuffleList  = (MP_TRACK_INDEX_T*)calloc(USB_MAX_SHUFFLE_LIST_SIZE, sizeof(MP_TRACK_INDEX_T));
    m_nShuffleStart = 0;
    m_nShuffleIdx   = 0;
    m_nPlayableSize = 0;
    m_nTotalErrSize = 0;
    m_nShuffleSize  = USB_MAX_SHUFFLE_LIST_SIZE;
    
    for(int nIdx = 0; nIdx < USB_MAX_SHUFFLE_LIST_SIZE; nIdx++)
    {
        m_pShuffleList[nIdx].nID = nIdx;
    }

    m_tFolderInfo.pFolderIdx     = (MP_FOLDER_INDEX_T*)calloc(USB_MAX_SHUFFLE_LIST_SIZE, sizeof(MP_FOLDER_INDEX_T));
    m_tFolderInfo.nFolderCount   = 0;
    m_tFolderInfo.nMaxFolderSize = USB_MAX_SHUFFLE_LIST_SIZE;

    #if USE_USB_SINGLE_INSTANCE
    m_pGPlayer = GPlayerEngine::GetInstance();
    #else
    m_pGPlayer = new GPlayerEngine();
    ConnectSignal();
    #endif

    m_pPlayer = m_pGPlayer;
    
    SetPlayerEngine(GPLAYER_ENGINE);

    m_tUserReqTrackList.clear();
    connect(&m_tUserReqTimer, SIGNAL(timeout()), this, SLOT(HandleUserReqTrack()));

    // { added by sungha.choi 2013.07.01
    m_tPlayTimer.setSingleShot(true);
    m_tPlayTimer.setInterval(USER_REQ_TRACK_NEXT_INTERVAL);
    connect(&m_tPlayTimer, SIGNAL(timeout()), this, SLOT(HandlePlayTimer()));
    // } added by sungha.choi 2013.07.01

    // { added by kihyung 2013.12.12 for ITS 0214770 
    m_tMediaInfo.nTrackIdx      = -1;
    m_tMediaInfo.bTrackChanged  = false;
    m_tMediaInfo.bValid         = false;
    // } added by kihyung 2013.12.12 for ITS 0214770 
    
    // check the status of USB device
    if(eMode == MP_MODE_USB) 
    {
        #if 0
        m_pQDBus = new QDBusInterface( "org.freedesktop.UDisks", 
                                       "/org/freedesktop/UDisks",
                                       "org.freedesktop.UDisks",
                                       QDBusConnection::systemBus());

        m_pQDBus->connection().connect( "org.freedesktop.UDisks", 
                                        "/org/freedesktop/UDisks",
                                        "org.freedesktop.UDisks",
                                        "DeviceAdded",
                                        this, SLOT(HandleUsbAttached(QDBusObjectPath)));

        m_pQDBus->connection().connect( "org.freedesktop.UDisks",
                                        "/org/freedesktop/UDisks",
                                        "org.freedesktop.UDisks",
                                        "DeviceRemoved",
                                        this, SLOT(HandleUsbDetached(QDBusObjectPath)));

        int nMountCount = FindAttachedDevices();
        if(nMountCount > 0) {
            MC_LOG << "USBController: FindAttachedDevices() " <<  nMountCount << LOG_ENDL;
            m_currentDeviceState = USB_CONNECTED;
            m_bBootWithUSB = true;
            emit OnDeviceConnected();
        }
        else {
            MC_LOG << "USBController: FindAttachedDevices() 0" << LOG_ENDL;
            m_currentDeviceState = USB_DISCONNECTED;
            emit OnDeviceDisConnected();
        }
        #endif
    }

    PrintVersion();
}

FileManager* USBController::GetFileManager()
{
    FileManager *pFileMngr = FileManager::GetInstance();

    if(m_bConnectFMSignal == false)
    {
        m_bConnectFMSignal = true;
        connect(pFileMngr, SIGNAL(OnPercentageAndSizeCopied(int, qint64)), this, SLOT(HandleFileCopied(int, qint64)));
        connect(pFileMngr, SIGNAL(OnPercentageAndSizeMoved(int, qint64)), this, SLOT(HandleFileMoved(int, qint64)));
        connect(pFileMngr, SIGNAL(OnFileCopiedPercentage(int, int)), this, SLOT(HandleFileCopiedPercentage(int, int)));
        connect(pFileMngr, SIGNAL(OnFileMovedPercentage(int, int)), this, SLOT(HandleFileMovedPercentage(int, int)));
        connect(pFileMngr, SIGNAL(OnPercentageAndSizeDeleted(int, qint64)), this, SLOT(HandleFileDeleted(int, qint64)));
        connect(pFileMngr, SIGNAL(OnPercentageAndSizeAndNameDeleted(int, qint64, QString)), this, SLOT(HandleFileDeleted2(int, qint64, QString)));
        connect(pFileMngr, SIGNAL(OnCreateFinished(int)), this, SLOT(HandleCreateFileFinished(int)));
        connect(pFileMngr, SIGNAL(OnRenameFinished(int,int)), this, SLOT(HandleRenameFinished(int,int)));
        connect(pFileMngr, SIGNAL(OnDeleteFinished(int,int,int)), this, SLOT(HandleDeleteFinished(int,int,int)));
        connect(pFileMngr, SIGNAL(OnMoveFinished(int,int)), this, SLOT(HandleMoveFinished(int,int)));
        connect(pFileMngr, SIGNAL(OnCopyFinished(int,int,int,int,int,int,int)), this, SLOT(HandleCopyFinished(int,int,int,int,int,int,int))); // modified by ravikanth 22-04-13
        connect(pFileMngr, SIGNAL(OnFormatFinished(int)), this, SLOT(HandleFormatFinished(int)));
        connect(pFileMngr, SIGNAL(OnError(int, QString)), this, SLOT(HandleFMError(int, QString)));
        connect(pFileMngr, SIGNAL(OnFileMoved(QString)), this, SIGNAL(OnFileMoved(QString)));

        pFileMngr->SetPlayerEngine(new GPlayerEngine()); // Modified By Puneet 2013.05.24 for video overlay on top while copying.
    }

    return pFileMngr;
}

MPlayerEngine* USBController::GetMPlayerEngine()
{
    /*
    if(m_pMPlayer == 0) {
        MC_LOG << "USBController::GetMPlayerEngine(): create MPlayerEngine" << LOG_ENDL;
        #if USE_USB_SINGLE_INSTANCE
        m_pMPlayer = MPlayerEngine::GetInstance();
        #else
        m_pMPlayer = new MPlayerEngine();
        ConnectSignal();
        #endif
    }
    */

    return m_pMPlayer;
}


/*
*******************************************************************
* Function: FindAttachedDevices()
*
 * Description : find attached device
*
* Parameters : None
*
* Returns : mounted device of QStringList type
*
*******************************************************************
*/
int USBController::FindAttachedDevices()
{
#if 0    
    m_tUSBObjectPath.clear();
    m_tUSBMountPath.clear();
    m_tUSBSerialNumber.clear();
    
    // MC_LOG << "USBController::FindAttachedDevices() " << LOG_ENDL;

    QDBusInterface USBInterface("org.freedesktop.UDisks",
                                "/org/freedesktop/UDisks",
                                "org.freedesktop.UDisks",
                                QDBusConnection::systemBus() );

    QDBusReply< QList<QDBusObjectPath> > busreply = USBInterface.call("EnumerateDevices");

    QList<QDBusObjectPath> args = busreply.value();

    if(args.size())
    {
        int size = args.size();

        for(int index = 0; index < size; index++)
        {
            QDBusObjectPath obPath = args[index];

            QString path = obPath.path();

            QDBusInterface DeviceInterface("org.freedesktop.UDisks",
                                           path ,
                                           "org.freedesktop.UDisks.Device",
                                           QDBusConnection::systemBus() );

            bool deviceIsRemovable = DeviceInterface.property("DeviceIsRemovable").toBool();
            bool deviceIsDrive = DeviceInterface.property("DeviceIsDrive").toBool();

            #if 0
			if(deviceIsRemovable)
                MC_LOG << " the device is removable" << LOG_ENDL;
			else
                MC_LOG << " the device is not removable" << LOG_ENDL;
			
			if(deviceIsDrive)
                MC_LOG << " the device is a drive" << LOG_ENDL;
			else
                MC_LOG << " the device is not a drive" << LOG_ENDL;
            #endif

            if(deviceIsDrive == true && deviceIsRemovable == true)
            {
                for(int index2 = 0 ; index2 < size ; index2++)
                {
                    QDBusObjectPath obPath2 = args[index2];

                    QString objectpath = obPath2.path();

                    if(objectpath.contains(path) == true && index != index2)
                    {
                        QDBusInterface DeviceInterface2("org.freedesktop.UDisks",
                                                        objectpath,
                                                        "org.freedesktop.UDisks.Device",
                                                        QDBusConnection::systemBus());

                        bool deviceIsMounted = DeviceInterface2.property("DeviceIsMounted").toBool();
                        bool deviceIsPartition = DeviceInterface2.property("DeviceIsPartition").toBool();

                        #if 0
                        if(deviceIsMounted)
                            MC_LOG << " the device is mounted " << LOG_ENDL;
                        else
                            MC_LOG << " the device is not mounted " << LOG_ENDL;

                        if(deviceIsPartition)
                            MC_LOG << " the device has partition";
                        else
                            MC_LOG << " the device don't have partition" << LOG_ENDL;
                        #endif

                        if(deviceIsMounted == true && deviceIsPartition == true)
                        {
                            QStringList mountPaths = DeviceInterface2.property("DeviceMountPaths").toStringList();

                            if(mountPaths.size() >= 1)
                            {
                                QString serialNum = DeviceInterface2.property("DriveSerial").toString(); 
                                QString mountPath = mountPaths.at(0);

                                m_tUSBObjectPath    += objectpath;
                                m_tUSBMountPath     += mountPath;
                                m_tUSBSerialNumber  += serialNum;
                                m_nDeviceMountAtBoot = 2;
                                MC_LOG << " objectpath: " <<  qPrintable(objectpath) << " mountPath: " <<  qPrintable(mountPath) << " serial: " << qPrintable(serialNum) << LOG_ENDL;
                            }
                        }
                        else if(deviceIsMounted == false && deviceIsPartition == true)
                        {
                            if(m_nDeviceMountAtBoot == 0)
                            {
                                MC_LOG << "USB is connected, but is not mounted" << LOG_ENDL;
                                m_nDeviceMountAtBoot = 1;    
                            }
                        }
                    }
                }
            }
        }
    }
    
#endif    
    return m_tUSBMountPath.count();
}

/*
*******************************************************************
* Function: CheckedScanMode()
*
* Description : slot about scan model
*                   controller related slots
*
* Parameters : None
*
* Returns : void
*
*******************************************************************
*/
int USBController::CheckedScanMode()
{
    if(m_nScanMode == SCANFILE || m_nScanMode == SCANFOLDER || m_nScanMode == SCANALL)
    {
        MC_HIGH << LOG_ENDL;
        SetTrackIdx(GetNextTrackIdx());
        if(GetTrackIdx() == m_nEndScanIndex)
        {
            emit OnScanCompleted();
            Scan_Impl(SCANOFF);

            // { removed by kihyung 2013.09.17 for HMC new sepc.
            /*
            // { added by kihyung 2013.07.17 for ITS 0180420 
            if(m_nRandomModeBeforeScan != RANDOMOFF) {
                RandomPlay_Impl(m_nRandomModeBeforeScan);
                m_nRandomModeBeforeScan = RANDOMOFF;
            }
            // } added by kihyung 2013.07.17 for ITS 0180420             
            */
            // } removed by kihyung 2013.09.17 for HMC new sepc.
        }
        
        if(m_tPlayList.count() > 0)
        {
            m_pPlayer->Stop();
            SetURI(GetTrackIdx());
            m_pPlayer->Play();
        }

        m_bAutoTrackChanged = true; // added by kihyung 2013.07.12 for ITS 0175486
        m_bEmitTrackChanged = true;
        
        return true;
    }
    else
    {
        return false;
    }
}


void USBController::SetMyMediaMode(MEDIA_PLAYER_MODE eMode)
{
    m_eMyMediaMode = eMode;
}

int USBController::GetMediaState(int EngineState)
{
    int nCurrState = m_currentMediaState;
    
    switch(EngineState)
    {
        case eMP_STATE_PLAYING:
            if(m_nFFREWRate != FAST_SPEED_X1)
            {
                if(m_bREWMode == false) nCurrState = PLAYER_FASTFORWARD;
                else                    nCurrState = PLAYER_REWIND;
            }
            else
            {
                nCurrState = PLAYER_PLAY;
            }
            break;
        case eMP_STATE_PAUSED:
            nCurrState = PLAYER_PAUSE;
            break;
        case eMP_STATE_STOPPED:
            nCurrState = PLAYER_STOP;
            break;
        case eMP_STATE_SEARCHING:
            nCurrState = PLAYER_SEARCH;
            break;
        default:
            // MC_LOG << "[Error]USBController::HandleUSBStateChanged state = " <<  nState << LOG_ENDL;
            nCurrState = PLAYER_STOP;
            break;
    }

    return nCurrState;
}

int USBController::IsLastTrack()
{
    if(m_currentMediaIndex == m_tPlayList.count() - 1) 
        return true;
    else

        return false;
}

int USBController::IsFirstTrack()
{
    if(m_currentMediaIndex == 0) 
        return true;
    else
        return false;
}

int USBController::SetURI(int nTrackIdx, int bCalledPrevNext)
{
    if(m_pPlayer == NULL)
	{
        MC_HIGH << "m_pPlayer is null" << LOG_ENDL;
        return nTrackIdx;
	}

    MC_HIGH << nTrackIdx << " " << bCalledPrevNext << " " << m_nUserPlayPosition << LOG_ENDL;
    
    if(nTrackIdx >= 0 && nTrackIdx < m_tPlayList.count()) 
    {
        QString strFilePath = m_tPlayList.at(nTrackIdx);
        if(strFilePath.isNull() || strFilePath.isEmpty()) return nTrackIdx;

        m_bSameSong = false; // added by kihyung 2014.01.06 for ITS 0217528

		if(m_bBTCallStart == false) // added by edo.lee 2013.11.22 ITS 0210504 
	        EmitOnStateChanged(PLAYER_SEARCH); // added by kihyung 2013.10.08 to display loading popup.
	    else
			EmitOnStateChanged(PLAYER_PAUSE);

        m_bSkipPauseSignal          = true;
        m_nSetURITrackIndex         = nTrackIdx;
        // { added by kihyung 2013.12.12 for ITS 0214770 
        m_tMediaInfo.nTrackIdx      = nTrackIdx; 
        m_tMediaInfo.bTrackChanged  = false;
        m_tMediaInfo.bValid         = false;
        // } added by kihyung 2013.12.12 for ITS 0214770
        
        if(m_nUserPlayPosition > 0) {
            m_pPlayer->SetURI(strFilePath, m_nUserPlayPosition);
            m_currentMediaPosition  = m_nUserPlayPosition; // added by kihyung 2013.12.10 for ITS 0214009
            m_pDrmConfirmPlay       = false;
            m_bAudioDragStart       = false;
            m_nUserPlayPosition     = 0;
            usleep(2000);
        }
        else
        {
            if(m_pPlayer != m_pGPlayer)
            {
                MC_HIGH << "m_pPlayer != m_pGPlayer" << LOG_ENDL;
                m_pPlayer = m_pGPlayer;
            }
            m_pDrmConfirmPlay       = false; //moved by taihyun.ahn for ITS 219250
            m_pPlayer->SetURI(strFilePath);
            //m_pDrmConfirmPlay       = false;
            m_bAudioDragStart       = false;
            m_currentMediaPosition  = 0; // added by kihyung 2013.12.10 for ITS 0214009
            usleep(2000);
        }

        if(m_bUserStop && m_pPlayer->GetMediaType() == VIDEO_FILE)
        {
            // As Player_SEARCH is Handled by only video , because of this Stop() from HMI is called loop in Loop
            // so need to check the last value of stop set.
            m_pPlayer->Stop();
        }
    }
    else 
    {
        MC_HIGH << "Error : nTrackIdx:" << nTrackIdx << LOG_ENDL;
    }

    m_bSendMetaInfo = true;
    
    return nTrackIdx;
}

int USBController::GetTrackIdx()
{
    int nTrackIdx = m_currentMediaIndex;
    
    if(nTrackIdx >= 0 && nTrackIdx < m_tPlayList.count())
    {
        return nTrackIdx;
    }
    else
    {
        return 0;
    }
}

int USBController::SetTrackIdx(int nTrackIdx)
{
    if(nTrackIdx >= 0 && nTrackIdx < m_tPlayList.count())
    {
        m_currentMediaIndex = nTrackIdx;
        return true;
    }
    else
    {
        return false;
    }
}

int USBController::GetNextTrackIdx()
{
    int nTrackIdx = m_currentMediaIndex;
    int nFileSize = m_tPlayList.count();
    int nStartIdx = 0;

    if(m_nRepeatMode == REPEATFOLDER || m_nScanMode == SCANFILE)
    {
        nFileSize = GetFileSizeOfFolder(nTrackIdx);
        nStartIdx = GetStartFileIdxOfFolder(nTrackIdx);
    }

    if(nTrackIdx < nStartIdx)
    {
        nTrackIdx = nStartIdx;
    }
    else if(nTrackIdx < nStartIdx + nFileSize - 1)
    {
        nTrackIdx++;
    }
    else
    {
        nTrackIdx = nStartIdx;
    }

    m_bPrevDirection = false;

    return nTrackIdx;
}

int USBController::GetPrevTrackIdx()
{
    int nTrackIdx = m_currentMediaIndex;
    int nFileSize = m_tPlayList.count();
    int nStartIdx = 0;

    if(m_nRepeatMode == REPEATFOLDER)
    {
        nFileSize = GetFileSizeOfFolder(nTrackIdx);
        nStartIdx = GetStartFileIdxOfFolder(nTrackIdx);
    }

    if(nTrackIdx > nStartIdx)
    {
        nTrackIdx--;
    }
    else if(nFileSize > 0)
    {
        nTrackIdx = nStartIdx + nFileSize - 1;
    }
    else
    {
        nTrackIdx = nStartIdx;
    }

    m_bPrevDirection = true;

    return nTrackIdx;
}

int USBController::GetNextTrackIdx(int nSkiplevel, int nTrackIdx)
{
    if(m_nRepeatMode == REPEATFOLDER) 
    {
        int nFileSize    = GetFileSizeOfFolder(nTrackIdx);
        int nNewTrackIdx = GetFileIdxInFolder(nTrackIdx) + nSkiplevel % nFileSize;
        if(nNewTrackIdx >= nFileSize)
        {
            nNewTrackIdx = nNewTrackIdx - nFileSize;
        }
        return GetStartFileIdxOfFolder(nTrackIdx) + nNewTrackIdx;
    }
    else 
    {
        int nNewTrackIdx = nTrackIdx + nSkiplevel % m_tPlayList.count();
        if(nNewTrackIdx >= m_tPlayList.count())
        {
            nNewTrackIdx = nNewTrackIdx - m_tPlayList.count();
        }
        return nNewTrackIdx;
    }
}

int USBController::GetPrevTrackIdx(int nSkiplevel, int nTrackIdx)
{
    if(m_nRepeatMode == REPEATFOLDER) 
    {
        int nFileSize    = GetFileSizeOfFolder(nTrackIdx);    
        int nNewTrackIdx = GetFileIdxInFolder(nTrackIdx) - nSkiplevel % nFileSize;
        if(nNewTrackIdx < 0)
        {
            nNewTrackIdx = nFileSize + nNewTrackIdx;
        }
        return GetStartFileIdxOfFolder(nTrackIdx) + nNewTrackIdx;
    }
    else 
    {
        int nNewTrackIdx = nTrackIdx - nSkiplevel % m_tPlayList.count();
        if(nNewTrackIdx < 0)
        {
            nNewTrackIdx = m_tPlayList.count() + nNewTrackIdx;
        }
        return nNewTrackIdx;
    }
}

int USBController::GetRandomTrackIdx(int bNext)
{
    int nIndex    = 0;
    int nFileSize = m_tPlayList.count();
    int nStartIdx = 0;

    if(m_nRepeatMode == REPEATFOLDER)
    {
        int nTrackIdx = m_pShuffleList[m_nShuffleIdx].nID;
        nFileSize = GetFileSizeOfFolder(nTrackIdx);
        nStartIdx = GetStartFileIdxOfFolder(nTrackIdx);
    }
    
    if(bNext == true) 
    {
        m_nShuffleIdx++;
        if(m_nShuffleIdx >= nStartIdx + nFileSize)
        {
            m_nShuffleIdx = nStartIdx;
        }
        else if(m_nShuffleIdx < nStartIdx)
        {
            m_nShuffleIdx = nStartIdx + nFileSize - 1;
            if(m_nShuffleIdx < nStartIdx) m_nShuffleIdx = nStartIdx;
        }

        // { removed by kihyung 2013.10.22 for P-P2-4-15
        /*
        if(m_nShuffleIdx == m_nShuffleStart)
        {
            //MC_LOG << "====================================" << LOG_ENDL;
            //MC_LOG << "      Remake Shuffle List" << LOG_ENDL;
            //MC_LOG << "====================================" << LOG_ENDL;
            SetShuffleList(true);
        }
        */
        // } removed by kihyung 2013.10.22 for P-P2-4-15

        m_bPrevDirection = false;

        nIndex = m_pShuffleList[m_nShuffleIdx].nID;
    }
    else 
    {
        m_nShuffleIdx--; 
        if(m_nShuffleIdx >= nStartIdx + nFileSize)
        {
            m_nShuffleIdx = nStartIdx;
        }
        else if(m_nShuffleIdx < nStartIdx)
        {
            m_nShuffleIdx = nStartIdx + nFileSize - 1;
            if(m_nShuffleIdx < nStartIdx) m_nShuffleIdx = nStartIdx;
        }   

        m_bPrevDirection = true;
    
        nIndex = m_pShuffleList[m_nShuffleIdx].nID;
    }
    
    return nIndex;
}

int USBController::GetFolderIdx(int nTrackIdx)
{
    return m_pShuffleList[nTrackIdx].nFolderIdx;
}

int USBController::GetStartFileIdx(int nFolderIdx)
{
    return m_tFolderInfo.pFolderIdx[nFolderIdx].nStartIdx;
}

int USBController::GetFolderSize()
{
    return m_tFolderInfo.nFolderCount;
}

int USBController::GetFileSizeOfFolder(int nTrackIdx)
{
    return m_tFolderInfo.pFolderIdx[m_pShuffleList[nTrackIdx].nFolderIdx].nFileSize;
}

int USBController::GetErrorSizeOfFolder(int nTrackIdx)
{
    return m_tFolderInfo.pFolderIdx[m_pShuffleList[nTrackIdx].nFolderIdx].nErrorSize;
}

void USBController::SetErrorSizeofFolder(int nTrackIdx, int nSize)
{
    m_tFolderInfo.pFolderIdx[m_pShuffleList[nTrackIdx].nFolderIdx].nErrorSize = nSize;
}

int USBController::GetStartFileIdxOfFolder(int nTrackIdx)
{
    return m_tFolderInfo.pFolderIdx[m_pShuffleList[nTrackIdx].nFolderIdx].nStartIdx;
}

int USBController::GetFileIdxInFolder(int nTrackIdx)
{
    return m_pShuffleList[nTrackIdx].nFileIdxInFolder;
}

void USBController::SetShuffleList(int bAgain)
{
    int nCount = m_tPlayList.count();
    if(nCount <= 1)
    {
        m_pShuffleList[0].bError = 0;
        return;
    }

    int nStartIdx   = 0;
    int nEndIdx     = nCount - 1;
    int nFileSize   = nCount;

    MC_HIGH << bAgain << nStartIdx << nEndIdx << nFileSize << LOG_ENDL;
    
    if(m_nRepeatMode == REPEATFOLDER)
    {
        int nTrackIdx = GetTrackIdx();
        nFileSize     = GetFileSizeOfFolder(nTrackIdx);
        nStartIdx     = GetStartFileIdxOfFolder(nTrackIdx);
        nEndIdx       = nStartIdx + nFileSize - 1;
        MC_HIGH << nTrackIdx << nStartIdx << nFileSize << LOG_ENDL;
    }

    int nOriTrackIdx;
    int nPrevLast    = -1;
    if(bAgain == true && nFileSize > 0 && m_nShuffleSize >= nFileSize)
    {
        nPrevLast    = m_pShuffleList[nEndIdx].nID;
        nOriTrackIdx = nPrevLast;
    }
    else
    {
        nOriTrackIdx = GetTrackIdx();
    }

    m_nShuffleIdx   = 0;
    m_nShuffleStart = 0;

    if(nFileSize > 1) 
    {
        int              nRandom;
        int              nIdx;
        MP_TRACK_INDEX_T tTemp; 

        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    
        for(nIdx = nEndIdx; nIdx > nStartIdx; --nIdx)
        {
            nRandom = nStartIdx + qrand() % nFileSize;
            tTemp                   = m_pShuffleList[nIdx];
            m_pShuffleList[nIdx]    = m_pShuffleList[nRandom];
            m_pShuffleList[nRandom] = tTemp;
        }

        if(bAgain == true && nFileSize >= 3)
        {
            MC_HIGH << nPrevLast << m_pShuffleList[0].nID << LOG_ENDL;
            if(nPrevLast == m_pShuffleList[0].nID)
            {
                MC_HIGH << nPrevLast << LOG_ENDL;
                int nMid = nStartIdx + nFileSize/2;
                tTemp                     = m_pShuffleList[nMid];
                m_pShuffleList[nMid]      = m_pShuffleList[nStartIdx];
                m_pShuffleList[nStartIdx] = tTemp;
            }
        }
    }

    for(int nIdx = nEndIdx; nIdx >= nStartIdx; --nIdx) // modified by kihyung 2013.07.10
    {
        if(m_pShuffleList[nIdx].nID == nOriTrackIdx)
        {
            m_nShuffleIdx = nIdx;
            break;
        }
    }
    MC_HIGH << "m_nShuffleIdx = " << m_nShuffleIdx << " nOriTrackIdx = " << nOriTrackIdx << LOG_ENDL;
    m_nShuffleStart = m_nShuffleIdx;
}

void USBController::SetPlayableList(int bFolder)
{
	// { modified by kihyung for ITS 0181967 
    int nCount = m_tPlayList.count();
    //MC_HIGH << "bFolder:" << bFolder << LOG_ENDL;

    m_nPlayableSize = nCount;
    m_nTotalErrSize = 0;
    
    if(nCount < 1) return;

    if(m_nShuffleSize < nCount)
    {
        if(m_pShuffleList)
        {
            free(m_pShuffleList);
        }
        // if(m_tFolderInfo.pFolderIdx)
        // {
        //     free(m_tFolderInfo.pFolderIdx);
        // }

        int nDivd       = nCount / USB_MAX_SHUFFLE_LIST_SIZE;
        int nRest       = nCount % USB_MAX_SHUFFLE_LIST_SIZE;
        int nNewCount   = nCount;
        if(nRest > 0)
        {
            nDivd++;
        }
        nNewCount = nDivd * USB_MAX_SHUFFLE_LIST_SIZE;
        m_nShuffleSize = nNewCount;

        m_pShuffleList = (MP_TRACK_INDEX_T*)calloc(m_nShuffleSize, sizeof(MP_TRACK_INDEX_T));
        MC_HIGH << "new Size:" << m_nShuffleSize << LOG_ENDL;

        // m_tFolderInfo.pFolderIdx     = (MP_FOLDER_INDEX_T*)calloc(m_nShuffleSize, sizeof(MP_FOLDER_INDEX_T));
        // m_tFolderInfo.nMaxFolderSize = m_nShuffleSize;
    }

    // m_tFolderInfo.nFolderCount = 0;
    /*
    int         nIdx;
    int         nStartIdx = -1;
    QFileInfo   tStartFile;
    QFileInfo   tEndFile;
    QString     strStartFolderName;
    QString     strEndFolderName;
    for(nIdx = 0; nIdx < nCount; nIdx++) 
    {
        if(nStartIdx < 0) {
            nStartIdx = nIdx;
            tStartFile.setFile(m_tPlayList.at(nStartIdx));
            strStartFolderName = tStartFile.absolutePath();

            m_pShuffleList[nIdx].nFileIdxInFolder = 0;
            m_pShuffleList[nIdx].nFolderIdx       = m_tFolderInfo.nFolderCount;
        }
        else {
            m_pShuffleList[nIdx].nFileIdxInFolder = nIdx - nStartIdx;
            m_pShuffleList[nIdx].nFolderIdx       = m_tFolderInfo.nFolderCount;

            if(bFolder == 1) {
                tEndFile.setFile(m_tPlayList.at(nIdx));
                strEndFolderName = tEndFile.absolutePath();
                if(strStartFolderName.compare(strEndFolderName) != 0) 
                {
                    m_tFolderInfo.pFolderIdx[m_tFolderInfo.nFolderCount].nFileSize  = nIdx - nStartIdx;
                    m_tFolderInfo.pFolderIdx[m_tFolderInfo.nFolderCount].nStartIdx  = nStartIdx;
                    m_tFolderInfo.pFolderIdx[m_tFolderInfo.nFolderCount].nErrorSize = 0;
                    m_tFolderInfo.nFolderCount++;
                    nStartIdx = -1;
                    nIdx--;
                }
            }
        }
    }
    
    if(nStartIdx >= 0) {
        m_tFolderInfo.pFolderIdx[m_tFolderInfo.nFolderCount].nFileSize  = nIdx - nStartIdx;
        m_tFolderInfo.pFolderIdx[m_tFolderInfo.nFolderCount].nStartIdx  = nStartIdx;
        m_tFolderInfo.pFolderIdx[m_tFolderInfo.nFolderCount].nErrorSize = 0;
        m_tFolderInfo.nFolderCount++;
    }
    */
    
    if(bFolder == 1) 
    {
        m_tFolderInfo.nFolderCount  = m_tFolderInfo.nCurrFolderSize;
        m_tFolderInfo.pFolderIdx[0] = m_tFolderInfo.tTotalFolderIdx;
        
        int nLastIdx = 0;
        for(unsigned int nFolderIdx = 0; nFolderIdx < m_tFolderInfo.nCurrFolderSize; nFolderIdx++)
        {
            m_tFolderInfo.pFolderIdx[nFolderIdx].nErrorSize = 0;
            
            int nStartIdx = m_tFolderInfo.pFolderIdx[nFolderIdx].nStartIdx;
            int nFileSize = m_tFolderInfo.pFolderIdx[nFolderIdx].nFileSize;
            if((nStartIdx < 0) || (nStartIdx + nFileSize > nCount))
            {
                MC_HIGH << "ERROR!!! nStartIdx, nFileSize, nCount" << nStartIdx << nFileSize << nCount << LOG_ENDL;
                bFolder = 0;
                break;
            }
            if(nStartIdx != nLastIdx)
            {
                MC_HIGH << "ERROR!!! nStartIdx, nLastIdx" << nStartIdx << nLastIdx << LOG_ENDL;
                bFolder = 0;
                break;
            }

            for(int nFileIdx = 0; nFileIdx < nFileSize; nFileIdx++, nStartIdx++) 
            {    
                // MC_LOG << "USBController::SetPlayableList() " << nStartIdx << " " <<  nFileIdx << " " <<  nFileSize << " " <<  nFolderIdx << LOG_ENDL;
                m_pShuffleList[nStartIdx].nID              = nStartIdx;
                m_pShuffleList[nStartIdx].bError           = 0;
                m_pShuffleList[nStartIdx].nFileIdxInFolder = nFileIdx;
                m_pShuffleList[nStartIdx].nFolderIdx       = nFolderIdx;
            }

            nLastIdx = nStartIdx;
        }
    }
    
    if(bFolder != 1)
    {
        m_tFolderInfo.pFolderIdx[0].nErrorSize = 0;
        
        for(int nIdx = 0; nIdx < nCount; nIdx++) 
        {
            m_pShuffleList[nIdx].nID              = nIdx;
            m_pShuffleList[nIdx].bError           = 0;
            m_pShuffleList[nIdx].nFileIdxInFolder = nIdx;
            m_pShuffleList[nIdx].nFolderIdx       = 0;
        }

        m_tFolderInfo.nFolderCount = 1;
        m_tFolderInfo.pFolderIdx[0].nFileSize  = nCount;
        m_tFolderInfo.pFolderIdx[0].nStartIdx  = 0;
        m_tFolderInfo.pFolderIdx[0].nErrorSize = 0;
    }
    // } modified by kihyung for ITS 0181967 
}

bool USBController::RestoreFromShuffle(int nStartIdx, int nSize)
{
    int              nIdx;
    int              nErrCheck;
    MP_TRACK_INDEX_T tTemp;

    if(nSize <= 1) return false;

    MC_HIGH << nStartIdx << nSize << "Restore from shuffle list..." << LOG_ENDL;
    for(nIdx = nStartIdx; nIdx <= nStartIdx + nSize; nIdx++) 
    {
        if(m_pShuffleList[nIdx].nID == nIdx) continue;
        
        nErrCheck = 0;
        do {
            tTemp                     = m_pShuffleList[nIdx];
            m_pShuffleList[nIdx]      = m_pShuffleList[tTemp.nID];
            m_pShuffleList[tTemp.nID] = tTemp;
            nErrCheck++;
        } while(m_pShuffleList[nIdx].nID != nIdx && nErrCheck <= nSize);
    }
    MC_HIGH << "Restore from shuffle list...OK!!!" << LOG_ENDL;

    return true;
}

void USBController::Scan_Impl(int mode)
{
    int bEmit = false;
    
    MC_HIGH << mode << LOG_ENDL;

    if(m_nScanMode != mode)
    {
        m_nScanMode = mode;
        bEmit = true;
    }
    
    if(mode == SCANFILE || mode == SCANFOLDER || mode == SCANALL)
    {
        if(m_nRepeatMode == REPEATFILE)
        {
            m_nRepeatMode = REPEATALL;
            emit OnModeChanged(m_nRepeatMode);
        }

        if(m_nRandomMode != RANDOMOFF)
        {
            RandomPlay_Impl(RANDOMOFF);
        }
        
        m_nEndScanIndex = GetTrackIdx();
        SetTrackIdx(GetNextTrackIdx());
        MC_HIGH << "m_nEndScanIndex:" << m_nEndScanIndex << LOG_ENDL;

        if(m_tPlayList.count() > 0)
        {
            m_pPlayer->Stop();
            SetURI(GetTrackIdx());
            m_pPlayer->Play();
            // EmitOnTrackChanged(GetTrackIdx());
            m_bEmitTrackChanged = true;
        }
    }
    else
    {
        m_nEndScanIndex = -1;
    }

    if(bEmit == true) {
        emit OnModeChanged(m_nScanMode);
    }
}

void USBController::RandomPlay_Impl(int mode)
{   
    if(mode == RANDOMFILE || mode == RANDOMFOLDER)
    {
        if(m_nScanMode != SCANOFF)
        {
            Scan_Impl(SCANOFF);
        }

        if(m_nRepeatMode == REPEATFILE)
        {
            m_nRepeatMode = REPEATALL;
            emit OnModeChanged(m_nRepeatMode);
        }
        
        SetShuffleList(false);
    }
    else if(mode == RANDOMOFF && m_nRandomMode != RANDOMOFF) 
    {
        int nFileSize = m_tPlayList.count();
        int nStartIdx = 0;

        if(m_nRepeatMode == REPEATFOLDER)
        {
            int nTrackIdx = GetTrackIdx();
            nFileSize     = GetFileSizeOfFolder(nTrackIdx);
            nStartIdx     = GetStartFileIdxOfFolder(nTrackIdx);
        }
        
        if(nFileSize > 1)
        {
            RestoreFromShuffle(nStartIdx, nFileSize);
        }
    }
        
    if(m_nRandomMode != mode)
    {
        m_nRandomMode = mode;
        emit OnModeChanged(m_nRandomMode);
    }
}

void USBController::RepeatPlay_Impl(int mode)
{

    if(mode == REPEATFILE || mode == REPEATFOLDER || mode == REPEATALL)
    {
        if(m_nScanMode != SCANOFF)
        {
            Scan_Impl(SCANOFF);
        }

        if(m_nRandomMode != RANDOMOFF)
        {
            RandomPlay_Impl(RANDOMOFF);
        }
    }

    if(m_nRepeatMode != mode)
    {
        m_nRepeatMode = mode;
        emit OnModeChanged(m_nRepeatMode);
    }
}

void USBController::PrintVersion()
{
    MC_HIGH << "===============================================" << LOG_ENDL;
    MC_HIGH << LGE_CONTROLLER_VER << LOG_ENDL;
    MC_HIGH << "===============================================" << LOG_ENDL;
}

void USBController::EmitOnTrackChanged(int nTrackIdx, int bAuto)
{
    MC_HIGH << "nTrackIdx:" << nTrackIdx << "bAuto:" << bAuto << LOG_ENDL;
    if(bAuto == true)
    {
        emit OnAutoTrackChanged(nTrackIdx);
    }
    else
    {
        emit OnAutoTrackChanged(-1);
    }
    emit OnTrackChanged(nTrackIdx);
}

void USBController::EmitOnStateChanged(int nState)
{
    MC_HIGH << nState << LOG_ENDL;

    // modified by kihyung 2013.10.08 for loading popup.
    if(nState != PLAYER_SEARCH) 
    {
        m_currentMediaState = nState;
    }

    // removed by kihyung 2013.12.03 for ITS 0210751
    // if(nState == PLAYER_PLAY)
    //     m_bAutoTrackChanged = false;
    
    emit OnStateChanged(nState);
}

void USBController::EmitTrackBuffered(int bPrev, int nTotalIdx)
{
    int nTrackIdx = 0;

    if(m_nRandomMode != RANDOMOFF && m_nShuffleIdx >= 0 && m_nShuffleIdx < m_tPlayList.count()) 
    {
        for(int i = 0; i < m_tPlayList.count(); i++)
        {
            if(m_pShuffleList[i].nID == nTotalIdx)
            {
                nTrackIdx = i;
                break;
            }
        }
    }
    else 
    {
        nTrackIdx = nTotalIdx;
    }

#if USE_FILE_INDEX_IN_FOLDER
    int nFileIdx   = GetFileIdxInFolder(nTrackIdx);
    int nFileSize  = GetFileSizeOfFolder(nTrackIdx);
#else
    int nFileIdx   = nTotalIdx;
    int nFileSize  = m_tPlayList.count();
#endif

    MC_HIGH << nTotalIdx << nTrackIdx << nFileIdx << nFileSize << LOG_ENDL;
    emit OnAutoTrackChanged(-1);//added by edo.lee 2013.10.19
    emit OnTrackBufferred(bPrev, nTotalIdx, nFileIdx, nFileSize); 
}

void USBController::SetMute(int bMute)
{
    if(m_pPlayer->setMute(bMute) == true)
    {
        MC_HIGH << bMute << LOG_ENDL;
        m_bMute = bMute;
    }
}

int USBController::GetMute()
{
    return m_bMute;
}

void USBController::ClearUserReqTrack()
{
    m_tUserReqMutex.lock();
    if(m_tUserReqTrackList.count() > 0)
    {
    	m_tUserReqTrackList.clear();
    }
    if(m_tUserReqTimer.isActive() == true)
    {
        m_tUserReqTimer.stop();
    }
    m_tUserReqMutex.unlock();
}

// { added by kihyung 2013.12.12 for ITS 0214770 
void USBController::CheckMediaInfoSignal()
{
    m_tMediaInfo.bTrackChanged = true;

    if(m_tMediaInfo.nTrackIdx == GetTrackIdx() && m_tMediaInfo.bValid == true)
    {
        MC_HIGH << "emit OnCurrentMediaInfo..." << LOG_ENDL;
        m_tMediaInfo.bValid = false;
        emit OnCurrentMediaInfo(&m_tMediaInfo.tInfo);
    }
}
// } added by kihyung 2013.12.12 for ITS 0214770

bool USBController::setSubtitleLanguage(int subId)
{
    if(m_pPlayer)
        m_pPlayer->SetSubtitleLanguage(subId);
    
    return true;
}

int USBController::getCurrentSubtitleLanguage()
{
    if(m_pPlayer)
        return m_pPlayer->GetSubtitleLanguage();
    else
        return -1;
}

void USBController::RequestSubtitles()
{
    if(m_pPlayer)
        m_pPlayer->RequestTextLanguages();
}
//{ added by jaehwan lee 2014.08.21 to fix ITS 245899, 245883, 245872
bool USBController::CheckUSBMounted()
{
    QString strUSB1Path = MediaManager::GetInstance()->GetFrontUSBPath();
    QString strUSB2Path = MediaManager::GetInstance()->GetRearUSBPath();

    int nErrTrackIndex = GetTrackIdx();
    QString strFilePath = m_tPlayList.at(nErrTrackIndex);


    if( (strFilePath.indexOf(strUSB1Path) == 0 && m_bUSB1Connect == false)
        || (strFilePath.indexOf(strUSB2Path) == 0 && m_bUSB2Connect == false) )
    {
        m_nPlayableSize = 0;
        MC_HIGH << "USB disconnected ..." << LOG_ENDL;
        return false;
    }

    QString strUSBMount;

    if(strFilePath.contains("/rw_data/media"))
        strUSBMount = "/" + strFilePath.section('/', 1, 4);
    else
        strUSBMount = "/" + strFilePath.section('/', 1, 3);

    QFileInfo tFileInfo(strUSBMount);
    if(tFileInfo.exists() == false)
    {
        MC_HIGH << strUSBMount << "USB mount point removed" << LOG_ENDL;
        return false;
    }
    return true;
}
//} added by jaehwan lee 2014.08.21 to fix ITS 245899, 245883, 245872

