#ifndef IVIDEOINTERFACE_H
#define IVIDEOINTERFACE_H

#include "mediaplayer_global.h"
#include "mediaplayer_def.h"

class MEDIAPLAYERSHARED_EXPORT IVideoInterface
{
public:    
    // Parameters: none
    // Description: Advance playback by 1 frame and pause. 
    virtual void FrameAdvance() = 0;

    // Parameters: none
    // Description: Rewind playback by 1 frame and pause. 
    virtual void FrameBack() = 0;

    // Parameters: (uint) mode : 0 - Off, 1- On
    // Description: This API enabled or disables subtitle depending on the mode value.
    virtual void SubTitle(uint mode) = 0;

    // Parameters: None.
    // Description:Activate disc LOADING and Disc detection
    virtual void Load() = 0;

    // Parameters: (uint) mode. 0: Normal eject. 1: Forced eject.
    // Description: This API does ejection of media.
    virtual void Eject(uint mode) = 0;

    // Parameters: (unsigned int) mode
    // Description: This API does slow play to forward direction."Mode" selects the speed of slow forward, defined by an enumeration.     
    virtual void SlowForward(uint nMode) = 0;

    // Parameters: (unsigned int) mode
    // Description: This API does slow play to forward direction."Mode" selects the speed of slow rewind, defined by an enumeration.     
    virtual void SlowRewind(uint nMode) = 0;

    // Parameters: none
    // Description: Page up (+ direction) browser-able picture of DVD-Audio.
    virtual void PageUp() = 0;

    // Parameters: none
    // Description: Page down(- direction) browser-able picture of DVD-Audio.
    virtual void PageDown() = 0;

    // Parameters: (uint) setting
    // Description: Display Title menu.
    virtual void Titlemenu(uint nSetting) = 0;

    // Parameters: none
    // Description: Receive ENTER key holding down notice.
    virtual void EnterKey() = 0;
    
    // Parameters: none
    // Description: Receive RETURN key holding down notice.
    virtual void ReturnKey() = 0;

    // Parameters: (uint) direction
    // Description: Receive cursor key holding down notice. "Direction" tells between up, down, left and right.
    virtual void CursorKey(uint direction) = 0;

    // Parameters: (uint) setting
    // Description: Display Top menu. 
    virtual void TopMenu(uint setting)  = 0;

    // Parameters: none
    // Description: Switch subtitle cyclically, including no caption.
    virtual void SwitchSubTitle() = 0;  

    // Parameters: none
    // Description: Switch DVD angle cyclically.
    virtual void SwitchAngle() = 0;  

    // Parameters: none
    // Description: Switch audio mode (audio number) cyclically. Switch audio channel at video CD too
    virtual void SwitchAudio() = 0;  

    // Parameters: none
    // Description: Calls the search designated screen(OSD) and switches cyclically.
    virtual void Search() = 0;

    // Parameters: (DeckPrioritySettings*) priority settings for the deck.
    // Description: Initialize the deck with the given priority settings.
    virtual void InitializeDeckPrioritySettings(DeckPrioritySettings *pSettings) = 0;

    // Parameters: (DVDSettings*) settings for the DVD.
    // Description: Initialize the DVD with the given settings.
    virtual void InitializeDVDSettings(DVDSettings *pSettings) = 0;

    // Return: -1: Failure, 
    //          0: HMI have to wait a OnArrivedResponse signal. And if received a signal, then HMI must call a GetTOC() again.
    //          1: Success;
    // Description:Fetch the ToC data, and return in the given structure.
    // For eg: HMI can call this API to display track information in the disc such as  start track number , end track number, total play duration.    
    // virtual int GetTOC(DvdcTOC *pToc) = 0;
    virtual int RequestTOC() = 0;

    // Return: -1: Failure, 
    //          0: HMI have to wait a OnArrivedResponse signal. And if received a signal, then HMI must call a GetTOC2() again.
    //          1: Success;
    // Parameters: int block_no.
    // Description: Fetch ToC2 data by block number. "Block_no" specifies the block. Applicable for CDDA,VCD & CA.
    // virtual int GetTOC2(DvdcTOC2 *pToc2, int block_no) = 0;
    virtual int RequestTOC2(int block_no) = 0;

    // Return: None.
    // Parameters: (int)keyId: 0-9 ,(int)pressStatus: pressed/released
    // Description: Send the key ID pressed by user to deck unit.
    virtual void TenKey(int keyId, int pressStatus) = 0;

    // Parameters: None.
    // Description: Change AUDIO mode for 2ch source to cyclic from current mode.
    virtual void DualMonoChange() = 0;

    // Return: -1: Failure. 1: Success.
    // Description: Retrieve the "CD-Video Status" from the deck, including the elapsed time information.
    virtual int GetVCDStatus(VCDStatusResponse *pResponse) = 0;

    // Return: -1: Failure. 1: Success.
    // Description: Retrieve the "DVD A/V/VR Status" from the deck, including the elapsed time information.
    virtual int GetDVDStatus(DVDStatusResponse *pResponse) = 0;

    // Parameters: None.
    // Description: Retrieve the priority settings for the Deck for DVD.
    virtual int GetDVDPrioritySettings(DeckPrioritySettings *pSettings) = 0;

    // Description: Request spoken language list to Deck Controller
    virtual int RequestAudioLanguages() = 0;

    // Parameters: int nAudioCode: spoken language code to select
    // Description: Change spoken language related code    
    virtual void SetAudioLanguage(int nAudioCode, int nIndex) = 0;

    // Description: Request subtitle language list to Deck Controller    
    virtual int RequestSubtitleLanguages() = 0;

    // Parameters: int nSubtitleCode: menu language code to select
    // Description: Change subtitle language related code
    virtual	void SetSubtitleLanguage(int nSubtitleCode) = 0;

    virtual void SetAngle(int nAngleNumber) = 0;

    // Parameters: (uint) mode. 0: PBC off. 1: PBC on
    virtual void SetPBC(uint mode) = 0;

    // Parameters: (uint) mode. 0: OSD off. 1: OSD on
    virtual void SetOSD(uint mode) = 0;

    // 
    virtual void SearchTitleChapter(uint nTitle, uint nChapter) = 0;

    // 
    virtual void CancelAudioLangRequest() = 0;

    // 
    virtual void CancelSubtitleLangRequest() = 0;

    // 
    virtual int  GetCurrentScreenState() = 0;
};

#endif // IVIDEOINTERFACE_H
