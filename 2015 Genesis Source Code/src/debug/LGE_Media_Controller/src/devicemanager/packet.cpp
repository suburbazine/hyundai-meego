#include "packet.h"
#include "packetbuffer.h"
#include "deck_def.h"
#include "util.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>

#define DM_USE_COMMAND_FRAME_SIZE   32
#define DM_FRAME_FDL_OFFSET         1
#define DM_FRAME_FDL_DIFF           0
#define DM_PACKET_HEADER            0x05
#define DM_PACKET_DISC_NO           0x01

Packet* Packet::m_pInstance = 0;

Packet* Packet::GetInstace()
{
    if(m_pInstance != 0) {
        return m_pInstance;
    }
    else {
        m_pInstance = new Packet();
        return m_pInstance;
    }
}

void Packet::DeleteInstace()
{
    if(m_pInstance) {
        delete m_pInstance;
        m_pInstance = 0;
    }
}

Packet::~Packet()
{
    MC_LOG << "~Packet()" << LOG_ENDL;
    
    if(m_pProtocol) {
        m_pProtocol->Close();
        delete m_pProtocol;
    }

    if(m_pPacketBuff != 0) {
        delete m_pPacketBuff;
    }
}

int Packet::Open()
{
    if(m_pProtocol) {
        m_pProtocol->Open();
        return 1;
    }
    else {
        MC_LOG << "m_pProtocol is null" << LOG_ENDL;
        return -1;
    }
}

int Packet::Close()
{
    if(m_pProtocol) {
        m_pProtocol->Close();
        return 1;
    }
    else {
        MC_LOG << "m_pProtocol is null" << LOG_ENDL;
        return -1;
    }
}

int Packet::Play()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x10;

    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;
    
    return WritePacket(pBuffer, nIdx);    
}

int Packet::Stop()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x11;

    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;
    
    return WritePacket(pBuffer, nIdx);
}

int Packet::Pause()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x12;

    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;
    
    return WritePacket(pBuffer, nIdx);  
}

int Packet::Eject(bool bForced)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x13;
    
    if(bForced == true) {
        pBuffer[nIdx++] = 0x81;
    }
    else {
        pBuffer[nIdx++] = 0x01;        
    }

    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::Loading()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x14;
    pBuffer[nIdx++] = DM_PACKET_DISC_NO;

    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;
        
    return WritePacket(pBuffer, nIdx);
}

int Packet::FunctionStop()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x04;
    pBuffer[nIdx++] = 0x15;

    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;
   
    return WritePacket(pBuffer, nIdx);
}

int Packet::TrackUp(int nTrackUpMode)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x16;
    pBuffer[nIdx++] = nTrackUpMode;
       
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;
    
    return WritePacket(pBuffer, nIdx);
}

int Packet::TrackDown(int nTrackDownMode)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x17;
    pBuffer[nIdx++] = nTrackDownMode;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;
    
    return WritePacket(pBuffer, nIdx);
}

int Packet::FastUp(int nFastUpMode)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x18;
    pBuffer[nIdx++] = nFastUpMode;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;
    
    return WritePacket(pBuffer, nIdx);
}

int Packet::FastDown(int nFastDownMode)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x19;
    pBuffer[nIdx++] = nFastDownMode;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;
    
    return WritePacket(pBuffer, nIdx);
}

int Packet::BusSwitching()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x1A;
    
    return WritePacket(pBuffer, nIdx);
}

int Packet::Release()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x1B;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;
    
    return WritePacket(pBuffer, nIdx);
}

int Packet::DirectCont(DirectContCommand *pCommand)
{
    int             nIdx = 0;
    unsigned char   pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pCommand->nFH     = DM_PACKET_HEADER;
    pCommand->nFDL    = 0x00;
    pCommand->nCode   = 0x1C;
    pCommand->nDiscNo = DM_PACKET_DISC_NO;

    memcpy((void*)pBuffer, (void*)pCommand, sizeof(DirectContCommand));
    nIdx = sizeof(DirectContCommand);
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;
    
    return WritePacket(pBuffer, nIdx);
}

int Packet::DirectStop(DirectStopCommand *pCommand)
{
    int             nIdx = 0;
    unsigned char   pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pCommand->nFH     = DM_PACKET_HEADER;
    pCommand->nFDL    = 0x00;
    pCommand->nCode   = 0xEF;
    pCommand->nDiscNo = DM_PACKET_DISC_NO;

    memcpy((void*)pBuffer, (void*)pCommand, sizeof(DirectStopCommand));
    nIdx = sizeof(DirectStopCommand);
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;
    
    return WritePacket(pBuffer, nIdx);
}

int Packet::DiscDetection()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x1E;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::RandomOn(int nRandomMode)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x20;
    pBuffer[nIdx++] = nRandomMode;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::RandomOff()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x21;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}


int Packet::RepeatOn(int nRepeatMode)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x22;
    pBuffer[nIdx++] = nRepeatMode;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::RepeatOff()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x23;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::ScanOn(int nScanMode)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x24;
    pBuffer[nIdx++] = nScanMode;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::ScanOff()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x25;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::PlayModeSwitching(DECK_MIXED_CD_MODE nSwitchMode)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x29;
    pBuffer[nIdx++] = nSwitchMode;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::ResumeInitialization()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x2F;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::SeekDataForCDROMPlay(int nMode, int nStartMin, int nStartSec, int nStartBlock, int nEndMin, int nEndSec, int nEndBlock)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x38;
    pBuffer[nIdx++] = nMode | DM_PACKET_DISC_NO;
    pBuffer[nIdx++] = DEC2BCD2((UINT1)nStartMin);
    pBuffer[nIdx++] = DEC2BCD2((UINT1)nStartSec);
    pBuffer[nIdx++] = DEC2BCD2((UINT1)nStartBlock);
    pBuffer[nIdx++] = DEC2BCD2((UINT1)nEndMin);
    pBuffer[nIdx++] = DEC2BCD2((UINT1)nEndSec);
    pBuffer[nIdx++] = DEC2BCD2((UINT1)nEndBlock);
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::SlowForward(int nMode)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x40;
    pBuffer[nIdx++] = nMode;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::SlowBackward(int nMode)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x41;
    pBuffer[nIdx++] = nMode;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::Subtitle()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x42;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::Audio()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x43;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::Angle()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x44;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::FrameAdvance()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x45;
        
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::FrameBack()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x46;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::TimeSearch(int nMediaMode, int nHourOrTrack, int nMinute, int nSecond)
{
    int             nIdx = 0;
    int             nInterval = 0;
    unsigned char   pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x47;

    switch(nMediaMode)
    {
    case eDM_MODE_DVD_VIDEO:
    case eDM_MODE_DVD_AUDIO:       
        pBuffer[nIdx++] = DEC2BCD2((UINT1)nHourOrTrack);
        pBuffer[nIdx++] = DEC2BCD2((UINT1)nMinute);
        pBuffer[nIdx++] = DEC2BCD2((UINT1)nSecond);
        break;
    case eDM_MODE_CD_DA:
    case eDM_MODE_VIDEO_CD_1_1:
    case eDM_MODE_VIDEO_CD_2_0:
    case eDM_MODE_VIDEO_CD_SVCD:
        pBuffer[nIdx++] = DEC2BCD2((UINT1)nHourOrTrack);
        pBuffer[nIdx++] = DEC2BCD2((UINT1)nMinute);
        pBuffer[nIdx++] = nInterval | DEC2BCD2((UINT1)nSecond);
        break;
    default:
        return 0;
    }
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::SearchCalling()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x48;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::StillOff()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x4A;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::PageUp()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x4B;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::PageDown()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x4C;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::SubtitleOn()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x4D;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::SubTitleOff()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x4E;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::TopMenu(DECK_TOP_MNEU_SETTING nSetting)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x50;
    pBuffer[nIdx++] = nSetting;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::TitleMenu(DECK_TITLE_MENU_SETTING nSetting)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x51;
    pBuffer[nIdx++] = nSetting;    
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::Enter()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x52;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::Return()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x53;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::Cursor(int nMode)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x54;
    pBuffer[nIdx++] = nMode;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::Key10(int nOper1, int nOper2)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x55;
    pBuffer[nIdx++] = nOper1;
    pBuffer[nIdx++] = nOper2;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::PictureSetting(int nContrast, int nColor, int nTint, int nBright, int nSharp, int nGamma, int nCoring, int nBlack, int nWhite)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x5E;
    pBuffer[nIdx++] = nContrast;
    pBuffer[nIdx++] = nColor;
    pBuffer[nIdx++] = nTint;
    pBuffer[nIdx++] = nBright;
    pBuffer[nIdx++] = nSharp;
    pBuffer[nIdx++] = nGamma;
    pBuffer[nIdx++] = nCoring;
    pBuffer[nIdx++] = nBlack;
    pBuffer[nIdx++] = nWhite;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::InitializationSetting1(InitialSetting1Command *pCommand)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pCommand->nFH     = DM_PACKET_HEADER;
    pCommand->nFDL    = 0x00;
    pCommand->nCode   = 0x60;

    memcpy((void*)pBuffer, (void*)pCommand, sizeof(InitialSetting1Command));
    nIdx = sizeof(InitialSetting1Command);
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::InitializationSetting2a(InitialSetting2aCommand *pCommand)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pCommand->nFH     = DM_PACKET_HEADER;
    pCommand->nFDL    = 0x00;
    pCommand->nCode   = 0xB1;

    memcpy((void*)pBuffer, (void*)pCommand, sizeof(InitialSetting2aCommand));
    nIdx = sizeof(InitialSetting2aCommand);
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::InitializationSetting3(InitialSetting3Command *pCommand)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pCommand->nFH     = DM_PACKET_HEADER;
    pCommand->nFDL    = 0x00;
    pCommand->nCode   = 0x62;

    memcpy((void*)pBuffer, (void*)pCommand, sizeof(InitialSetting3Command));
    nIdx = sizeof(InitialSetting3Command);
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::ParentalChangePermission(DECK_PERMISSION_FLAG nPermission)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x69;
    pBuffer[nIdx++] = (UINT1)nPermission;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::BonusGroupPINNumber(BonusGroupPinNumberCommand *pCommand)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pCommand->nFH     = DM_PACKET_HEADER;
    pCommand->nFDL    = 0x00;
    pCommand->nCode   = 0x6A;

    memcpy((void*)pBuffer, (void*)pCommand, sizeof(BonusGroupPinNumberCommand));
    nIdx = sizeof(BonusGroupPinNumberCommand);
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::TestSP(int nMode)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x6B;
    pBuffer[nIdx++] = nMode;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::DirectTouch(int nSelect, int nX, int nY)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x6C;
    pBuffer[nIdx++] = nSelect;
    pBuffer[nIdx++] = nX;
    pBuffer[nIdx++] = nY;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::DualMonoChange()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0xB3;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::DirectStopSpeedSetting(DirectStopSpeedSettingCommand *pCommand)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pCommand->nFH     = DM_PACKET_HEADER;
    pCommand->nFDL    = 0x00;
    pCommand->nCode   = 0xE9;
    pCommand->nDiscNo = DM_PACKET_DISC_NO;
    
    memcpy((void*)pBuffer, (void*)pCommand, sizeof(DirectStopSpeedSettingCommand));
    nIdx = sizeof(DirectStopSpeedSettingCommand);
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::LowConsumptionModeOn()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0xF1;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::LowConsumptionModeOff()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0xF2;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::ResetStart()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0xFD;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::AccOn()
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0xFE;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    return WritePacket(pBuffer, nIdx);
}

int Packet::RequestTOCStatus(int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x31;
    pBuffer[nIdx++] = DM_PACKET_DISC_NO;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}

int Packet::RequestTOCStatus2(int nBlockNum, int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x30;
    pBuffer[nIdx++] = DM_PACKET_DISC_NO;
    pBuffer[nIdx++] = DEC2BCD2(nBlockNum);
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}

int Packet::RequestDirTreeStatus(DirTreeCommand *pCommand, int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pCommand->nFH     = DM_PACKET_HEADER;
    pCommand->nFDL    = 0x00;
    pCommand->nCode   = 0x34;

    memcpy((void*)pBuffer, (void*)pCommand, sizeof(DirTreeCommand));
    nIdx = sizeof(DirTreeCommand);
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}

int Packet::RequestTagStatus(int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x36;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}

int Packet::RequestFileNameStatus(FileNameCommand *pCommand, int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pCommand->nFH     = DM_PACKET_HEADER;
    pCommand->nFDL    = 0x00;
    pCommand->nCode   = 0x35;

    memcpy((void*)pBuffer, (void*)pCommand, sizeof(FileNameCommand));
    nIdx = sizeof(FileNameCommand);
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}

int Packet::RequestFolderNameStatus(FolderNameCommand *pCommand, int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pCommand->nFH     = DM_PACKET_HEADER;
    pCommand->nFDL    = 0x00;
    pCommand->nCode   = 0x37;

    memcpy((void*)pBuffer, (void*)pCommand, sizeof(FolderNameCommand));
    nIdx = sizeof(FolderNameCommand);
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}

int Packet::RequestCDText(CDTextCommand *pCommand, int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pCommand->nFH     = DM_PACKET_HEADER;
    pCommand->nFDL    = 0x00;
    pCommand->nCode   = 0x3A;
    pCommand->nDiscNo = DM_PACKET_DISC_NO;
    
    memcpy((void*)pBuffer, (void*)pCommand, sizeof(CDTextCommand));
    nIdx = sizeof(CDTextCommand);
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}

int Packet::RequestCDTextInfo(int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x3B;
    pBuffer[nIdx++] = DM_PACKET_DISC_NO;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}

int Packet::RequestFolderFile4Status(FolderFile4Command *pCommand, int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pCommand->nFH     = DM_PACKET_HEADER;
    pCommand->nFDL    = 0x00;
    pCommand->nCode   = 0x3E;

    memcpy((void*)pBuffer, (void*)pCommand, sizeof(FolderFile4Command));
    nIdx = sizeof(FolderFile4Command);
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}

int Packet::RequestDVDVRTitle(DVDVRTitleCommand *pCommand, int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pCommand->nFH     = DM_PACKET_HEADER;
    pCommand->nFDL    = 0x00;
    pCommand->nCode   = 0x5C;
    pCommand->nDiscNo = DM_PACKET_DISC_NO;

    memcpy((void*)pBuffer, (void*)pCommand, sizeof(DVDVRTitleCommand));
    nIdx = sizeof(DVDVRTitleCommand);
        
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}

int Packet::RequestDVDVRInfor(int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x5D;
    pBuffer[nIdx++] = DM_PACKET_DISC_NO;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}

int Packet::RequestPictureSettingStatus(int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x5F;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}

int Packet::RequestCommonStatus(int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x63;

    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1 - DM_FRAME_FDL_DIFF;
    
    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}

int Packet::RequestDVDAudioVideoVRStatus(int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x64;
    
    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}

int Packet::RequestCDStatus(int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x66;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}

int Packet::RequestVideoCDStatus(int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x67;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}

int Packet::RequestCompAudioStatus(int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x68;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}

int Packet::RequestInitialSetting1Status(int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x6E;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}

int Packet::RequestInitialSetting2aStatus(int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0xBF;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}
    
int Packet::RequestInitialSetting3Status(int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x6D;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}

int Packet::RequestFileFolderTotalNum(int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0x97;
    pBuffer[nIdx++] = DM_PACKET_DISC_NO;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}

int Packet::RequestTagStatus2(int nOutputType, int bSync)
{
    int           nIdx = 0;
    unsigned char pBuffer[DM_USE_COMMAND_FRAME_SIZE];

    pBuffer[nIdx++] = DM_PACKET_HEADER;
    pBuffer[nIdx++] = 0x00;
    pBuffer[nIdx++] = 0xB6;
    pBuffer[nIdx++] = nOutputType;
    
    pBuffer[DM_FRAME_FDL_OFFSET] = nIdx + 1;

    if(bSync) 
        return WriteNReadPacket(pBuffer, nIdx);
    else
        return WritePacket(pBuffer, nIdx);
}


void Packet::HandleAboutToClose()
{
    MC_LOG << "HandleAboutToClose" << LOG_ENDL;
}

void Packet::HandleBytesWritten(qint64 bytes)
{
    MC_LOG << "HandleBytesWritten: " << bytes << LOG_ENDL;
}

void Packet::HandleReadChannelFinished()
{
    MC_LOG << "HandleReadChannelFinished" << LOG_ENDL;
}

void Packet::HandleDataReceived()
{
    int nReads;

    nReads = ReadPacket(m_pPacketBuff, MAX_SERAIL_BUFFER);
    if(nReads <= 0) {
        return;
    }

    // decode packet.
    m_nPacketSize          = nReads;
    m_pStatusData->m_nFH   = 0;
    m_pStatusData->m_nFDL  = 0;
    m_pStatusData->m_nCode = m_pPacketBuff[0];
    memcpy(&m_pStatusData->m_tData, &m_pPacketBuff[1], sizeof(unsigned char) * (m_nPacketSize-1));
    // { added by cychoi 2013.10.13 for ITS 195211
    // Should reset command code to 0xFF if received the below response from Deck
    if(m_nCommandCode == 0x10)
    {
        switch(m_pStatusData->m_nCode)
        {
        case 0x84: // DS_CODE_DVD_AUDIOVIDEOVR_STATUS
        case 0x86: // DS_CODE_CD_STATUS
        case 0x87: // DS_CODE_VIDEO_CD_STATUS
        case 0x88: // DS_CODE_COMP_AUDIO_STATUS
        case 0x7F: // DS_CODE_REQ_ERR
        case 0xFD: // DS_CODE_RESET_START
            m_nCommandCode = 0xFF;
            MC_HIGH << "[DECK_RXPACKET] Clear Last Command Code on Response." << LOG_ENDL;
            break;
        default: // make compiler happy
            // Do nothing
            break;
        }
    }
    // } added by cychoi 2013.10.13

    // print packet for test.
    PrintPacket(0, m_pPacketBuff, m_nPacketSize);

    // send signal new packet arrived.
    PacketBuffer *pBuffer = PacketBuffer::GetInstance();
    pBuffer->AppendPacket(*m_pStatusData);
}

Packet::Packet(QObject *parent) : QObject(parent),
m_pProtocol(0),
m_pPacketBuff(0),
m_nPacketSize(0),
m_pStatusData(0),
m_nCommandCode(0xFF), // added by cychoi 2013.06.26 for ITS 176335
m_nCount(0)
{

    Tab2Protocol *pProtocol = new Tab2Protocol();
    
    m_pProtocol   = pProtocol;
    m_pPacketBuff = new unsigned char[MAX_SERAIL_BUFFER];
    m_pStatusData = new StatusData();

    connect(pProtocol, SIGNAL(ReadReady()), this, SLOT(HandleDataReceived()));
}


int Packet::ReadPacket(unsigned char *pBuffer, int nMaxSize)
{
    int nReads = 0;

    nReads = m_pProtocol->RecvMessage((void*)pBuffer, nMaxSize);

    return nReads;
} 

int Packet::WritePacket(unsigned char *pBuffer, int nMaxSize)
{
    int nWrite = 0;
        
    if(nMaxSize <= 2) return 0;

    // { added by cychoi 2013.06.26 for ITS 176335
    // VCD is resumed at wrong position on fast double call.
    // It is temporary code to ignore second play call by AVP.
    int nCommandCode = pBuffer[2];
    if(nCommandCode == 0x10 && nCommandCode == m_nCommandCode)
    {
        m_nCommandCode = 0xFF;
        MC_HIGH << "[DECK_TXPACKET] Ignore Play Request on fast double call." << LOG_ENDL;
        return 0;
    }
    m_nCommandCode = nCommandCode;
    // } added by cychoi 2013.06.26

    nWrite = m_pProtocol->SendMessage((void*)&pBuffer[2], nMaxSize-2);

    #if DM_USE_DEBUG_PRINT
    PrintPacket(1, pBuffer, nMaxSize);
    #endif

    // MC_LOG << "Packet::WritePacket: nWrite: " << nWrite << LOG_ENDL;

    return nWrite;
}

int Packet::WriteNReadPacket(unsigned char *pBuffer, int nMaxSize)
{
#if 0    
    int nReads;
    
    if(nMaxSize <= 2) return 0;

    nReads = m_pProtocol->SendNRecvMessage((void*)&pBuffer[2], nMaxSize-2, (void*)m_pPacketBuff, MAX_SERAIL_BUFFER);
    if(nReads <= 0) {
        return 0;
    }

    // decode packet.
    m_nPacketSize          = nReads;
    m_pStatusData->m_nFH   = 0;
    m_pStatusData->m_nFDL  = 0;
    m_pStatusData->m_nCode = m_pPacketBuff[0];
    memcpy(&m_pStatusData->m_tData, &m_pPacketBuff[1], sizeof(unsigned char) * (m_nPacketSize-1));

    // print packet for test.
    PrintPacket(m_pPacketBuff, m_nPacketSize);

    // send signal new packet arrived.
    PacketBuffer::GetInstance()->PrependPacket(*m_pStatusData);

    return nReads;
#else
    return WritePacket(pBuffer, nMaxSize);
#endif
}

void Packet::PrintPacket(int bWrite, unsigned char *pPacket, int nMaxSize)
{
#if DM_USE_DEBUG_PRINT
    int  count = 0;
    char strBuff[1024];
    char strTemp[8];

    memset(strBuff, 0x00, sizeof(char)*1024);
    for(int i = 0; i < nMaxSize; i++)
    {
        if(pPacket[i] <= 0x0E) {
            sprintf(strTemp, "0%X ", pPacket[i]);
        }
        else {
            sprintf(strTemp, "%X ", pPacket[i]);
        }

        int nLen = strlen(strTemp);
        memcpy(&strBuff[count], strTemp, nLen);
        count += nLen;
        if(count > 1000) break;
    }

    m_nCount++;
    if(bWrite)
    {
        MC_HIGH << "[DECK_TXPACKET] " << m_nCount << ": " << strBuff << LOG_ENDL;
    }
    else
    {
        MC_HIGH << "[DECK_RXPACKET] " << m_nCount << ": " << strBuff << LOG_ENDL;
    }
#else
    Q_UNUSED(bWrite);
    Q_UNUSED(pPacket);
    Q_UNUSED(nMaxSize);
#endif
}

