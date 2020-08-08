#include "tab2protocol.h"
#include "tab2driver.h"
#include "util.h"

Tab2Protocol::Tab2Protocol(QObject *parent) : QObject(parent),
m_pDriver(0)
{
    m_pDriver = new Tab2Driver();

    #if 1
    connect(m_pDriver, SIGNAL(ReadReady()), this, SIGNAL(ReadReady()));
    #else
    connect(m_pDriver, SIGNAL(ReadReady()), this, SIGNAL(ReadReady()), Qt::DirectConnection);
    #endif
}

Tab2Protocol::~Tab2Protocol()
{
    MC_LOG << "~Tab2Protocol()" << LOG_ENDL;
    
    if(m_pDriver) {
        m_pDriver->Close();
        delete m_pDriver;
    }

    m_pDriver = 0;
}

int Tab2Protocol::Open()
{
    return m_pDriver->Open();
}

int Tab2Protocol::Close()
{
    return m_pDriver->Close();
}

int Tab2Protocol::SendMessage(const void *pData, int nLen)
{
    return m_pDriver->Write(pData, nLen, false, 0);
}

int Tab2Protocol::RecvMessage(void *pData, int nMaxLen)
{
    return m_pDriver->Read(pData, nMaxLen, false);
}

int Tab2Protocol::SendNRecvMessage(const void *pSendBuf, int nLen, void *pRecvBuf, int nMaxLen)
{
    UINT1  *pSendPacket = (UINT1*)pSendBuf;
    int     nStatusCode = GetResponeCode((int)pSendPacket[0]);
    if(nStatusCode < 0) return 0;
    
    if(m_pDriver->Write(pSendBuf, nLen, true, nStatusCode) <= 0) {
        return 0;
    }

    m_pDriver->Read(pRecvBuf, nMaxLen, true);
    
    return 0;
}

int Tab2Protocol::GetResponeCode(int nRequestCode)
{
    int nStatusCode = -1;

    if(nRequestCode == 0x31) return  DS_CODE_TOC_STATUS;
    if(nRequestCode == 0x30) return  DS_CODE_TOC_STATUS2;
    if(nRequestCode == 0x34) return  DS_CODE_DIR_TREE_STATUS;
    if(nRequestCode == 0x36) return  DS_CODE_TAG_STATUS;
    if(nRequestCode == 0x5C) return  DS_CODE_DVD_VR_TITLE_STATUS;
    if(nRequestCode == 0x5D) return  DS_CODE_DVD_VR_INFOR_STATUS;
    if(nRequestCode == 0x3A) return  DS_CODE_CD_TEXT_STATUS;
    if(nRequestCode == 0x3B) return  DS_CODE_CD_TEST_INFOR_STATUS;
    if(nRequestCode == 0x35) return  DS_CODE_FILE_NAME_STATUS;
    if(nRequestCode == 0x37) return  DS_CODE_FOLDER_NAME_STATUS;
    if(nRequestCode == 0x5F) return  DS_CODE_PICTURE_SETTING_STATUS;
    if(nRequestCode == 0x6E) return  DS_CODE_INIT_SET1_STATUS;
    if(nRequestCode == 0xBF) return  DS_CODE_INIT_SET2A_STATUS;
    if(nRequestCode == 0x6D) return  DS_CODE_INIT_SET3_STATUS;
    if(nRequestCode == 0x63) return  DS_CODE_COMMON_STATUS;
    if(nRequestCode == 0x64) return  DS_CODE_DVD_AUDIOVIDEOVR_STATUS;
    if(nRequestCode == 0x66) return  DS_CODE_CD_STATUS;
    if(nRequestCode == 0x67) return  DS_CODE_VIDEO_CD_STATUS;
    if(nRequestCode == 0x68) return  DS_CODE_COMP_AUDIO_STATUS;
    if(nRequestCode == 0x69) return  DS_CODE_PARENTAL_CHANGE_REQUEST;
    if(nRequestCode == 0x6B) return  DS_CODE_TEST_SP_STATUS;
    if(nRequestCode == 0x3E) return  DS_CODE_FOLDERFILE4_STATUS;
    if(nRequestCode == 0x6C) return  DS_CODE_DIRECT_TOUCH;
    if(nRequestCode == 0x6A) return  DS_CODE_BONUS_GRP_PIN_NUM_REQ;
    if(nRequestCode == 0x97) return  DS_CODE_FILEFONDER_TOTAL_NUMBER;
    if(nRequestCode == 0xB6) return  DS_CODE_TAG_STATUS2;
    
    return nStatusCode;
}
