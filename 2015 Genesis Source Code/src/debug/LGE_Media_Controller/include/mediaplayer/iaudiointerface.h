#ifndef IAUDIOINTERFACE_H
#define IAUDIOINTERFACE_H

#include "mediaplayer_global.h"
#include "mediaplayer_def.h"

class MEDIAPLAYERSHARED_EXPORT IAudioInterface
{
public: 
    // Parameters: None.
    // Description:Activate disc LOADING and Disc detection
    virtual void Load() = 0;

    // Parameters: (uint) mode. 0: Normal eject. 1: Forced eject.
    // Description: This API does ejection of media.
    virtual void Eject(uint mode) = 0;

    // Parameters: none
    // Description: Receive ENTER key holding down notice.
    virtual void EnterKey() = 0;

    // Parameters: (uint) direction
    // Description: Receive cursor key holding down notice. "Direction" tells between up, down, left and right.
    virtual void CursorKey(uint direction) = 0;

    // Parameters: none
    // Description: Receive RETURN key holding down notice.
    virtual void ReturnKey() = 0;

    // Parameters: (Boolean) mode
    // Description: For CD-DA and CD-Audio mixed CD, switch between the two media types based on mode value. 
    virtual void SwitchPlayMode(bool mode) = 0;

    // Return: -1: Failure, 
    //          0: HMI have to wait a OnArrivedResponse signal. And if received a signal, then HMI must call a GetTOC() again.
    //          1: Success;
    // Description:Fetch the ToC data, and return in the given structure.
    // virtual int GetTOC(DvdcTOC *pToc) = 0;
    virtual int RequestTOC() = 0;

    // Return: -1: Failure, 
    //          0: HMI have to wait a OnArrivedResponse signal. And if received a signal, then HMI must call a GetTOC()2 again..
    //          1: Success;
    // Parameters: int block_no.
    // Description: Fetch ToC2 data by block number. "Block_no" specifies the block. Applicable for CDDA,VCD & CA.
    // virtual int GetTOC2(DvdcTOC2 *pToc2, int block_no) = 0;
    virtual int RequestTOC2(int block_no) = 0;

    virtual int RequestGraceNoteTOC() = 0;

    // Return: None.
    // Parameters: (int)keyId: 0-9 ,(int)pressStatus: pressed/released
    // Description: Send the key ID pressed by user to deck unit.
    virtual void TenKey(int keyId, int pressStatus) = 0;

    // Return: -1: Failure, 
    //          0: HMI have to wait a OnArrivedResponse signal. And if received a signal, then HMI must call a GetCDText() again.
    //          1: Success;
    // Parameters: int track_no, int data_type, int block_no, int frame_no, (struct DvdcCDText*) stat;
    // Description: Fetch the CD-Text data. "Block_no" specifies the block.
    // virtual int GetCDText(int track_no, int data_type, int block_no, int frame_no, DvdcCDText* stat) = 0;
    virtual int RequestCDText(int track_no, int data_type, int block_no, int frame_no) = 0;

    // Return: -1: Failure, 
    //          0: HMI have to wait a OnArrivedResponse signal. And if received a signal, then HMI must call a GetCDTextInfo() again.
    //          1: Success;
    // Parameters: (struct DvdcCdTextInfo*) stat;
    // Description: Fetch the CD-Text Information data. 
    // virtual int GetCDTextInfo(DvdcCdTextInfo* stat) = 0;
    virtual int RequestCDTextInfo() = 0;

    // Return: -1: Failure, 
    //          0: HMI have to wait a OnArrivedResponse signal. And if received a signal, then HMI must call a GetDirTree() again.
    //          1: Success;
    // Parameters: (unsigned int) mode, (unsigned int) folder_no, (unsigned int) block_no.
    // Description: Get the directory tree of block_no block within folder_no folder. (Meaning of "mode": TBD).
    // virtual int GetDirTree(DvdcDirTree *pDirTree, unsigned int mode, unsigned int folder_no, unsigned int block_no) = 0;
    virtual int RequestDirTree(unsigned int mode, unsigned int folder_no, unsigned int block_no) = 0;

    // Return: -1: Failure, 
    //          0: HMI have to wait a OnArrivedResponse signal. And if received a signal, then HMI must call a GetTag() again.
    //          1: Success;
    // Parameters: None
    // Description: Get the TAG information from currently playing comp-audio file.
    // virtual int GetTag(DvdcTagInfo *pTagInfo) = 0;
    virtual int RequestTag() = 0;
    

    // Return: -1: Failure, 
    //          0: HMI have to wait a OnArrivedResponse signal. And if received a signal, then HMI must call a GetTag2() again.
    //          1: Success;
    // Parameters: (int) info_type.
    // Description: Get the specific TAG information from currently playing comp-audio file.
    // virtual int GetTag2(DvdcTag2Info *pTag2Info, int info_type) = 0;
    virtual int RequestTag2(int info_type) = 0;

    virtual int RequestAllTagInfo() = 0;

    virtual int RequestFileName(unsigned int nFolderNo, unsigned int nFileNo) = 0;

    virtual int RequestFolderName(unsigned int nFolderNo) = 0;
    

    // Return: -1: Failure, 
    //          0: HMI have to wait a OnArrivedResponse signal. And if received a signal, then HMI must call a GetTotalFileFolder() again.
    //          1: Success;
    // Parameters: (int*) folder_total, (int*) file_total
    // Description: Get the count of files and folders in Comp-Audio disc.
    virtual int GetTotalFileFolder(TotalFileFolderCount *pCount) = 0;

    // Parameters: None.
    // Description: Retrieve the "CD-DA Status" from the deck, including the elapsed time information.
    virtual int GetCDStatus(CDDAStatusResponse *pCDStatus) = 0;
    
    // Parameters: None.
    // Description: Retrieve the "Comp-Audio Status" from the deck, including the elapsed time information.
    virtual int GetCAStatus(CAStatusResponse *pCAStatus) = 0;

};

#endif // IAUDIOINTERFACE_H
