#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDataStream>
#include <QTextCodec>
#include <QTime>
#include <QApplication>
#include "DABCommManager.h"
#include "DABCommReceiverThread.h"
#include "DABProtocol.h"
#include "DABApplication_Def.h"
#include "DABLogger.h"

EBU_CHAR_TABLE g_EBULatinBased[] = {
        {0x80, 0xE1},
        {0x81, 0xE0},
        {0x82, 0xE9},
        {0x83, 0xE8},
        {0x84, 0xED},
        {0x85, 0xEC},
        {0x86, 0xF3},
        {0x87, 0xF2},
        {0x88, 0xFA},
        {0x89, 0xF9},
        {0x8A, 0xD1},
        {0x8B, 0xC7},
        {0x8C, 0x15E},
        {0x8D, 0xDF},
        {0x8E, 0x49},
        {0x8F, 0x132},
        {0x90, 0xE2},
        {0x91, 0xE4},
        {0x92, 0xEA},
        {0x93, 0xEB},
        {0x94, 0xEE},
        {0x95, 0xEF},
        {0x96, 0xF4},
        {0x97, 0xF6},
        {0x98, 0xFB},
        {0x99, 0xFC},
        {0x9A, 0xF1},
        {0x9B, 0xE7},
        {0x9C, 0x15F},
        {0x9D, 0x11F},
        {0x9E, 0x69},
        {0x9F, 0x133},
        {0xA0, 0xAA},
        {0xA1, 0x3B1},
        {0xA2, 0xA9},
        {0xA3, 0x2030},
        {0xA4, 0x11E},
        {0xA5, 0x115},
        {0xA6, 0x148},
        {0xA7, 0x151},
        {0xA8, 0x3C0},
        {0xA9, 0x152},
        {0xAA, 0xA3},
        {0xAB, 0x24},
        {0xAC, 0x2190},
        {0xAD, 0x2191},
        {0xAE, 0x2192},
        {0xAF, 0x2193},
        {0xB0, 0xBA},
        {0xB1, 0xB9},
        {0xB2, 0xB2},
        {0xB3, 0xB3},
        {0xB4, 0xB1},
        {0xB5, 0x130},
        {0xB6, 0x144},
        {0xB7, 0x171},
        {0xB8, 0xB5},
        {0xB9, 0xBF},
        {0xBA, 0xF7},
        {0xBB, 0xB0},
        {0xBF, 0xA7},
        {0xC0, 0xC1},
        {0xC1, 0xC0},
        {0xC2, 0xC9},
        {0xC3, 0xC8},
        {0xC4, 0xCD},
        {0xC5, 0xCC},
        {0xC6, 0xD3},
        {0xC7, 0xD2},
        {0xC8, 0xDA},
        {0xC9, 0xD9},
        {0xCA, 0x158},
        {0xCB, 0x10C},
        {0xCC, 0x160},
        {0xCD, 0x17D},
        {0xCE, 0x110},
        {0xCF, 0x141},
        {0xD0, 0xC2},
        {0xD1, 0xC4},
        {0xD2, 0xCA},
        {0xD3, 0xCB},
        {0xD4, 0xCE},
        {0xD5, 0xCF},
        {0xD6, 0xD4},
        {0xD7, 0xD6},
        {0xD8, 0xDB},
        {0xD9, 0xDC},
        {0xDA, 0x159},
        {0xDB, 0x10D},
        {0xDC, 0x161},
        {0xDD, 0x17E},
        {0xDE, 0x111},
        {0xDF, 0x141},
        {0xE0, 0xC3},
        {0xE1, 0xC5},
        {0xE2, 0xC6},
        {0xE3, 0x152},
        {0xE4, 0x177},
        {0xE5, 0xDD},
        {0xE6, 0xD5},
        {0xE7, 0xD8},
        {0xE8, 0xDE},
        {0xE9, 0x14A},
        {0xEA, 0x154},
        {0xEB, 0x106},
        {0xEC, 0x15A},
        {0xED, 0x179},
        {0xEF, 0xF0},
        {0xF0, 0xE3},
        {0xF1, 0xE5},
        {0xF2, 0xE6},
        {0xF3, 0x153},
        {0xF4, 0x175},
        {0xF5, 0xFD},
        {0xF6, 0xF5},
        {0xF7, 0xF8},
        {0xF8, 0xFE},
        {0xF9, 0x14B},
        {0xFA, 0x155},
        {0xFB, 0x107},
        {0xFC, 0x15B},
        {0xFD, 0x17A}
};

const char *DLPlus_ContentType[64] =
{
        "DUMMY"									// 0
        , "ITEM.TITLE"					// 1
        , "ITEM.ALBUM"					// 2
        , "ITEM.TRACNUMBER"			// 3
        , "ITEM.ARTIST"					// 4
        , "ITEM.COMPOSITION"		// 5
        , "ITEM.MOVEMENT"				// 6
        , "ITEM.CONDUCTOR"			// 7
        , "ITEM.COMPOSER"				// 8
        , "ITEM.BAND"						// 9
        , "ITEM.COMMENT"				// 10
        , "ITEM.GENRE"					// 11
        , "INFO.NEWS"						// 12
        , "INFO.NEWS.LOCAL"			// 13
        , "INFO.STOCKMARKET"		// 14
        , "INFO.SPORT"					// 15
        , "INFO.LOTTERY"				// 16
        , "INFO.HOROSCOPE"			// 17
        , "INFO.DAILY_DIVERSION"// 18
        , "INFO.HEALTH"					// 19
        , "INFO.EVENT"					// 20
        , "INFO.SCENE"					// 21
        , "INFO.CINEMA"					// 22
        , "INFO.TV"							// 23
        , "INFO.DATE_TIME"			// 24
        , "INFO.WEATHER"				// 25
        , "INFO.TRAFFIC"				// 26
        , "INFO.ALARM"					// 27
        , "INFO.ADVERTISEMENT"	// 28
        , "INFO.URL"						// 29
        , "INFO.OTHER"					// 3O
        , "STATIONNAME.SHORT"		// 31
        , "STATIONNAME.LONG"		// 32
        , "PROGRAMME.NOW"				// 33
        , "PROGRAMME.NEXT"			// 34
        , "PROGRAMME.PART"			// 35
        , "PROGRAMME.HOST"			// 36
        , "PROGRAMME.EDITORIAL_STAFF"	// 37
        , "PROGRAMME.FREQUENCY"	// 38
        , "PROGRAMME.HOMEPAGE"	// 39
        , "PROGRAMME.SUBCHANNEL"// 4O
        , "PHONE.HOTLIME"				// 41
        , "PHONE.STUDIO"				// 42
        , "PHONE.OTHER"					// 43
        , "SMS.STUDIO"					// 44
        , "SMS.OTHERT"					// 45
        , "EMAIL.HOTLINE"				// 46
        , "EMAIL.STUDIO"				// 47
        , "EMAIL.OTHER"					// 48
        , "MMS.OTHER"						// 49
        , "CHAT"								// 50
        , "CHAT.CENTER"					// 51
        , "VOTE.QUESTION"				// 52
        , "VOTE.CENTRE"					// 53
        , "RFU"									// 54
        , "RFU"									// 55
        , "Private classes"			// 56
        , "Private classes"			// 57
        , "Private classes"			// 58
        , "DESCRIPTOR.PLACE"		// 59
        , "DESCRIPTOR.APPOINTMENT"	// 60
        , "DESCRIPTOR.IDENTIFIER"		// 61
        , "DESCRIPTOR.PURCHASE"			// 62
        , "DESCRIPTOR.GET_DATA"			// 63
};

int g_EBULatinBasedNum = sizeof(g_EBULatinBased) / sizeof(EBU_CHAR_TABLE);
quint8 cmdReqGetVersion[]  = { 0xa5, 0x0D, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x84 };

DABCommManager::DABCommManager(QObject *parent) : QObject(parent), m_ComUart(parent), m_ComSpi(parent), pCommDataHandler(NULL), pSPIPacketInfo(NULL)    //# 20130327 prevent
{  
    BState = PlaySvcNone;
    ModState = ModBooting;
    ScanState = StopScan;

    //qWarning() << Q_FUNC_INFO;
    connect(&process, SIGNAL(readyReadStandardError()), this, SLOT(onUpdateOutputText()));
    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onProcessFinished(int,QProcess::ExitStatus)));
    connect(&process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(onProcessError(QProcess::ProcessError)));

#ifndef __UNIX_DESKTOP__
    if(m_ComUart.Open())
    {
        qWarning("================= UART Device Open Success !! ===================");
    }
    else
    {
        qWarning("================= SPI Device Open Fail !! ===================");
    }

    pCommReceiverThread = new DABCommReceiverThread(&(this->m_ComUart) , parent);   
    QObject::connect(pCommReceiverThread, SIGNAL(receivedCommand(unsigned char,unsigned char,QByteArray,int)), this, SLOT(onProcessCommand(unsigned char,unsigned char,QByteArray,int)));
#endif

#ifndef __UNIX_DESKTOP__       
    if(m_ComSpi.Open())
    {
        qWarning("================= SPI Device Open Success !! ===================");
        pCommDataHandler = new DABCommDataHandler(&(this->m_ComSpi), parent);

        QObject::connect(pCommDataHandler, SIGNAL(receivedCommand(unsigned char,unsigned char,unsigned char,QByteArray,int,int)),
                         this, SLOT(onSPIProcessCommand(unsigned char,unsigned char,unsigned char,QByteArray,int,int)));

        pSPIPacketInfo = new DABCommSPIPacketInfo();
        pCommDataHandler->setCommDataHandler(pSPIPacketInfo);
    }
    else
    {
        qWarning("================= SPI Device Open Fail !! ===================");
    }
    qWarning() << "SPI Open() Success";
#endif

    memset(&m_xDataBackup,0,sizeof(m_xDataBackup));
}

DABCommManager::~DABCommManager()
{
    if(pCommReceiverThread != NULL){
        pCommReceiverThread->stop();
    }

    if(pCommDataHandler != NULL){
        pCommDataHandler->stop();
    }
}

void
DABCommManager::restartUART(void)
{
   m_ComUart.Close();

    if(m_ComUart.Open())
    {
        DABLogger::instance()->Log((QString(" DABCommManager::restartUART() : ================= UART Device Open Success !! =================== !!")));
    }
    else
    {
        DABLogger::instance()->Log((QString(" DABCommManager::restartUART() : ================= UART Device Open Fail !! =================== !!")));
    }
}

void
DABCommManager::startComm(void)
{
    if(pCommReceiverThread != NULL)
    {
        pCommReceiverThread->start();
    }
    if(pCommDataHandler != NULL){
        qWarning() << "CommDataHandler Start() !!";
        pCommDataHandler->start();
    }
    else
    {
        qWarning() << "CommDataHandler start FAIL !!";
    }
}

void
DABCommManager::stopComm(void)
{
    if(pCommReceiverThread != NULL){
        pCommReceiverThread->stop();
    }

    if(pCommDataHandler != NULL){
        qWarning() << "CommDataHandler Stop() !!";
        pCommDataHandler->stop();
    }
    else
    {
        qWarning() << "CommDataHandler start FAIL !!";
    }
}

void DABCommManager::onUpdateOutputText()
{
    //qWarning() << Q_FUNC_INFO;

    QByteArray newData = process.readAllStandardError();
    //qWarning() << newData;

    DABLogger::instance()->Log((QString(" lDABCommManager::onUpdateOutputText() :: readAllStandardError = %1").arg(QString(newData))));
}

void DABCommManager::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    //qWarning() << Q_FUNC_INFO;

    if(exitStatus == QProcess::CrashExit)
    {
        //qWarning() << "GStream launcher crashed !! ";
        DABLogger::instance()->Log((QString("GStream launcher crashed !! ")));
    }
    else if(exitCode != 0)
    {
        //qWarning() << "GStream launcher failed ";
        DABLogger::instance()->Log((QString("GStream launcher failed !! ")));
    }
    else
    {
        //qWarning() << "GStream launcher success exit !! ";
        DABLogger::instance()->Log((QString("GStream launcher success exit !! ")));
    }
}

void DABCommManager::onProcessError(QProcess::ProcessError error)
{
    //qWarning() << Q_FUNC_INFO;

    if(error == QProcess::FailedToStart)
    {
        //qWarning() << "GStream launcher not found!!";
        DABLogger::instance()->Log((QString("GStream launcher not found!!")));
    }
}

void DABCommManager::onProcessCommand(unsigned char type, unsigned char command, QByteArray bArray, int dataLen)
{
    onProcessCommand(type, command, (unsigned char*)bArray.data(), dataLen);
}

void DABCommManager::onSPIProcessCommand(unsigned char type, unsigned char command, unsigned char framepostion, QByteArray bArray, int dataLen, int totalLen)
{
    onSPIProcessCommand(type, command, framepostion, (unsigned char*)bArray.data(), dataLen, totalLen);
}

void DABCommManager::onSPIProcessCommand(unsigned char type, unsigned char command, unsigned char framepostion, unsigned char *pData, int dataLen, int totalLen)
{
    //qWarning() << Q_FUNC_INFO << " : Type: " << type << ", Command: " << command << " datalen: " << dataLen << " Data: " << pData;
    if(type == DAB_SPI_EVENT_COMMAND_TYPE)
    {
        switch(command)
        {
        case CMD_EVENT_SPI_JPEG:
            //qWarning() << "Command ==> CMD_EVT_SPI_JPEG";
            this->EvtSPIUpdateSLS(pData, dataLen, command, framepostion, totalLen);
            break;

        case CMD_EVENT_SPI_PNG:
            //qWarning() << "Command ==> CMD_EVNT_SPI_PNG";
            this->EvtSPIUpdateSLS(pData, dataLen, command, framepostion, totalLen);
            break;

        case CMD_EVENT_SPI_BWS:
            //qWarning() << "Command ==> CMD_EVNT_SPI_BWS";
            break;

        case CMD_EVENT_SPI_TPEG:
            //qWarning() << "Command ==> CMD_EVNT_SPI_TPEG";
            this->EvtSPIUpdateTPEG(pData, dataLen);
            break;

        case CMD_EVENT_SPI_EPG:
            //qWarning() << "Command ==> CMD_EVNT_SPI_EPG";
            this->EvtSPIUpdateEPG(pData, dataLen);
            break;

        case CMD_EVENT_SPI_STATION_LOGO:
            //qWarning() << "Command ==> CMD_EVENT_SPI_STATION_LOGO";
            this->EvtSPIUpdateStationLogo(pData, dataLen, framepostion);
            break;

        default:
            break;
        }
    }
    else
    {
        qWarning() << " ERROR! Unknown Command Type: " << type;
    }
}

void DABCommManager::onProcessCommand(unsigned char type, unsigned char command, unsigned char *pData, int dataLen)
{
//    qDebug() << Q_FUNC_INFO << " : Type: " << type << ", Command: " << command << " datalen: " << dataLen << " Data: " << pData;

    if(type ==DAB_REQUEST_COMMAND_TYPE)
    {
        qWarning() << " : receive REQUEST Command in onPrecessCommand. It's ERROR: Type: " << type << ", Command: " << command;
    }
    else if(type == DAB_RESPONSE_COMMAND_TYPE)
    {
        switch (command)
        {
        case RSP_GET_VERSION:
            //qWarning() << "Command ==> RSP_GET_VERSION";
            this->RspGetVersion(pData, dataLen);
            break;

        case RSP_SET_FULL_SCAN:
            //qWarning() << "Command ==> RSP_SET_FULL_SCAN";
            break;

        case RSP_SET_SINGLE_SCAN:
            //qWarning() <<  "Command ==> RSP_SET_SINGLE_SCAN";
            break;

        case RSP_SET_SEEK_SERVICE:
            //qWarning() << "Command ==> RSP_SET_SEEK_SERVICE";
            this->RspSetSeekService(pData, dataLen);
            break;

        case RSP_SET_SELECT_SERVICE:
            //qWarning() << "Command ==> RSP_SET_SELECT_SERVICE";
            this->RspSetSelectService(pData, dataLen);
            break;

        case RSP_SET_ANNOUNCEMENT_FLAG:
            //qWarning() << "Command ==> RSP_SET_ANNOUNCEMENT_FLAG";
            this->RspSetAnnouncementFlag(pData, dataLen);
            break;

        case RSP_GET_SERVICE_LIST:
            //qWarning() << "Command ==> RSP_GET_SERVICE_LIST";
            this->RspGetServiceList(pData, dataLen);
            break;

        case RSP_GET_DLS:
            //qWarning() << "Command ==> RSP_GET_DLS";
            this->RspGetDLS(pData, dataLen);
            break;

        case RSP_GET_SLS:
            //qWarning() <<  "Command ==> RSP_GET_SLS";
            this->RspGetSLS(pData, dataLen);
            break;

        case RSP_GET_EPG:
            //qWarning() << "Command ==> RSP_GET_EPG";
            qWarning() << "RSP_GET_EPG Command not used.";
            break;

        case RSP_SET_MODULE_MODE:
            qWarning() << "Command ==> RSP_SET_MODULE_MODE";
//            this->RspSetModuleMode(pData, dataLen);
            break;

        case RSP_DOWNLOAD_IMAGE:
            qWarning() << "Command ==> RSP_DOWNLOAD_IMAGE";
            break;

        case RSP_SET_EVENT_SEND_TIME:
            //qWarning() << "Command ==> RSP_SET_EVENT_SEND_TIME";
            this->RspSetEvtSendTime(pData, dataLen);
            break;
        case RSP_SET_DRC_FLAG:
            //qWarning() << "Command ==> RSP_SET_DRC_FLAG";
            this->RspSetDrcFlag(pData, dataLen);
            break;

        case RSP_SET_MUTE:
            //qWarning() << "Command ==> RSP_SET_MUTE";
            break;

        case RSP_SET_SEEK_STOP:
            //qWarning() << "Command ==> RSP_SET_SEEK_STOP";
            this->RspSetSeekStop(pData, dataLen);
            break;

        case RSP_SET_SELECT_STOP:
            //qWarning() << "Command ==> RSP_SET_SELECT_STOP";
            this->RspSetSelectStop(pData, dataLen);
            break;

        case RSP_SET_SERVICE_FOLLOWING_FLAG:
            //qWarning() << "Command ==> RSP_SET_SERVICE_FOLLOWING_FLAG";
            this->RspSetServiceFollowing(pData, dataLen);
            break;

        case RSP_SET_SELECT_ANNOUNCEMENT_SERVICE:
            //qWarning() << "Command ==> RSP_SET_SERVICE_FOLLOWING_FLAG";
            this->RspSetSelectAnnouncementService(pData, dataLen);
            break;

        case RSP_SET_AUTO_SCAN:
            //qWarning() << "Command ==> RSP_SET_AUTO_SCAN";
            this->RspSetAutoScan(pData, dataLen);
            break;

        case RSP_SET_FM_SERVICE:
            //qWarning() << "Command ==> RSP_SET_FM_SERVICE";
            this->RspSetFMService(pData, dataLen);
            break;

        case RSP_SET_BROADCASTING:
            //qWarning() << "Command ==> RSP_SET_BROADCASTING";
            this->RspSetBroadcasting(pData, dataLen);
            break;

        case RSP_GET_AUDIO_INFO:
            //qWarning() << "Command ==> RSP_GET_AUDIO_INFO";
            this->RspGetAudioInfo(pData, dataLen);
            break;

        case RSP_SET_BAND_SELECTION:
            //qWarning() << "Command ==> RSP_SET_BAND_SELECTION";
            this->RspSetBandSelection(pData, dataLen);
            break;

        case RSP_SET_SEL_SERVICE_INFO:
            //qWarning() << "Command ==> RSP_SET_SEL_SERVICE_INFO";
            this->RspSetBandSelection(pData, dataLen);
            break;

        case RSP_SET_FIG_DATA_STATUS:
            //qWarning() << "Command ==> RSP_SET_FIG_DATA_STATUS";
            this->RspSetBandSelection(pData, dataLen);
            break;

        case RSP_SET_ANNOUNCE_DELAY:
            //qWarning() << "Command ==> RSP_SET_ANNOUNCE_DELAY";
            this->RspSetAnnounceDelay(pData, dataLen);
            break;

        case RSP_SET_ANNOUNCE_TIMEOUT:
            //qWarning() << "Command ==> RSP_SET_ANNOUNCE_TIMEOUT";
            this->RspSetAnnounceTimeout(pData, dataLen);
            break;

        case RSP_SET_SERVLINK_MUTE_TIMEOUT:
            //qWarning() << "Command ==> RSP_SET_SERVLINK_MUTE_TIMEOUT";
            this->RspSetServLinkTimeout(pData, dataLen);
            break;

        case RSP_SET_SERVLINK_CER_VALUE:
            //qWarning() << "Command ==> RSP_SET_SERVLINK_CER_VALUE";
            this->RspSetServLinkCERValue(pData, dataLen);
            break;

        case RSP_SET_SIGSTATUS_CER_VALUE:
            //qWarning() << "Command ==> RSP_SET_SIGSTATUS_CER_VALUE";
            this->RspSetSigStatusCERValue(pData, dataLen);
            break;

        case RSP_SET_ANNOUNCEMENT_WEAK_TIMEOUT:
            //qWarning() << "Command ==> RSP_SET_ANNOUNCEMENT_WEAK_TIMEOUT";
            this->RspSetAnnouncementWeakTimeout(pData, dataLen);
            break;

        case RSP_SET_PINKNOISE_CER_VALUE:
            //qWarning() << "Command ==> RSP_SET_ANNOUNCEMENT_WEAK_TIMEOUT";
            this->RspSetPinknoiseCERValue(pData, dataLen);
            break;

        case RSP_SET_ANTENNA_STATUS:
            qDebug() << "Command ==> RSP_SET_ANNOUNCEMENT_WEAK_TIMEOUT";
            this->RspSetAntennaStatus(pData, dataLen);
            break;

        case RSP_GET_ANNOUNCEMENT_STATUS:
            qDebug() << "Command ==> RSP_GET_ANNOUNCEMENT_STATUS";
            this->RspGetAnnouncementStatus(pData, dataLen);
            break;

        case RSP_GET_PINK_STATUS:
            qDebug() << "Command ==> RSP_GET_PINK_STATUS";
            this->RspGetPinkStatus(pData, dataLen);
            break;

        default:
            qWarning(" Unknown Response Command : 0x%04X", command);
            break;
        }
    }
    else if(type == DAB_EVENT_COMMAND_TYPE)
    {
        switch(command)
        {
            case EVT_SEND_MODULE_STATUS:
            {
                //qWarning() << "Command ==> EVT_SEND_MODULE_STATUS";
                stEVT_SEND_MODULE_STATUS* pStatus = (stEVT_SEND_MODULE_STATUS*)pData;
                this->EvtSendModuleStatus(pStatus->ModuleState, pStatus->ServiceState);
                break;
            }

            case EVT_SEND_SCAN_STATUS:
            {
                //qWarning() << "Command ==> EVT_SEND_SCAN_STATUS";
                break;
            }

            case EVT_SEND_CHANGE_SERVICE:
            {
                //qWarning() << "Command ==> EVT_SEND_CHANGE_SERVICE";
                this->EvtSendChangeService(pData, dataLen);
                break;
            }

            case EVT_SEND_ANNOUNCEMENT:
            {
                //qWarning() << "Command ==> EVT_SEND_ANNOUNCEMENT";
                this->EvtSendAnnouncement(pData, dataLen);
                break;
            }

            case EVT_SEND_TIME:
            {
                //qWarning() << "Command ==> EVT_SEND_TIME";
                this->EvtSendTime(pData, dataLen);
                break;
            }

            case EVT_SEND_QOS:
            {
                //qWarning() << "Command ==> EVT_SEND_QOS";
                this->EvtSendQOS(pData, dataLen);
                break;
            }

            case EVT_UPDATE_DLS:
            {
                //qWarning() << "Command ==> EVT_UPDATE_DLS";
                this->EvtUpdateDLS(pData, dataLen);
                break;
            }

            case EVT_UPDATE_SLS:
            {
                //qWarning() << "Command ==> EVT_UPDATE_SLS";
                this->EvtUpdateSLS(pData, dataLen);
                break;
            }

            case EVT_UPDATE_EPG:
            {
                //qWarning() << "Command ==> EVT_UPDATE_EPG";
                this->EvtUpdateEPG(pData, dataLen);;
                break;
            }

            case EVT_DOWNLOAD_STATUS:
            {
                //qWarning() << "Command ==> EVT_DOWNLOAD_STATUS";
                break;
            }

            case EVT_DATA_INFO:
            {
                //qWarning() << "Command ==> EVT_DATA_INFO";
                this->EvtDataInfo(pData, dataLen);
                break;
            }

            case EVT_SERVICE_LINKING_INFOMATION:
            {
                //qWarning() << "Command ==> EVT_SERVICE_LINKING_INFOMATION";
                break;
            }

            case EVT_SEND_AUTO_SCAN_INFO:
            {
                //qWarning() << "Command ==> EVT_SEND_AUTO_SCAN_INFO";
                this->EvtSendAutoScanInfo(pData, dataLen);
                break;
            }

            case EVT_SEND_SIGNAL_INFO:
            {
                //qWarning() << "Command ==> EVT_SEND_SIGNAL_INFO";
                this->EvtSendSignalInfo(pData, dataLen);
                break;
            }

            case EVT_SERVICE_FOLLOWING_STATE:
            {
                //qWarning() << "Command ==> EVT_SERVICE_FOLLOWING_STATE";
                this->EvtServiceFollowingState(pData, dataLen);
                break;
            }

            case EVT_SEND_PI_ARRAY:
            {
                //qWarning() << "Command ==> EVT_SEND_PI_ARRAY";
                break;
            }

            case EVT_SEND_DTC_UART:
            {
                //qWarning() << "Command ==> EVT_SEND_DTC_UART";
                emit evtSendDtcUart();
                break;
            }

            case EVT_SEND_COUNTRY_INFO:
            {
                //qWarning() << "Command ==> EVT_SEND_COUNTRY_INFO";
                break;
            }

            case EVT_SEND_PINK_STATUS:
            {
                //qWarning() << "Command ==> EVT_SEND_PINK_STATUS";
                this->EvtSendPinkStatus(pData, dataLen);
                break;
            }

            case EVT_SEND_USER_APP_TYPE:
                //qWarning() << "Command ==> EVT_SEND_USER_APP_TYPE";
                this->EvtSendUserAppType(pData, dataLen);
                break;

            case EVT_SEND_ANNOUNCEMENT_TA_INVALID:
                //qWarning() << "Command ==> EVT_SEND_ANNOUNCEMENT_TA_INVALID";
                this->EvtSendAnnouncementTAInvalid(pData, dataLen);
                break;

            default:
                break;
        }
    }
    else
    {
        qDebug() << " ERROR! Unknown Command Type: " << type;
    }
}

////////////////////////////////////////////////////////////////////////////////////////
// Request/Response Command from DAB Controller.. (Naming from DAB Module Specification Document)
////////////////////////////////////////////////////////////////////////////////////////
void
DABCommManager::ReqGetVersion(void)
{
    qWarning() << Q_FUNC_INFO;

    int size = sizeof(cmdReqGetVersion);
    DABLogger::instance()->Log((QString(" =================DABController::ReqGetVersion(void) :: m_bGetVersionRequested is true !! ====================")));
    int returnSize = this->writeCmd(cmdReqGetVersion, sizeof(cmdReqGetVersion));
    if(size != returnSize)
    {
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;
        DABLogger::instance()->Log((QString(" ERROR: Send size error.. Cmd Size: %1, send size = %2").arg(size).arg(returnSize)));
    }
}

void
DABCommManager::RspGetVersion(unsigned char* buff, int size)
{
    //qWarning() << Q_FUNC_INFO;

    if(size <= 0)
    {
        qWarning() << "Error!. Size is " << size;
        DABLogger::instance()->Log((QString(" RspGetVersion() :: Error!. Size is = %1").arg(size)));
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        DABLogger::instance()->Log((QString(" RspGetVersion() :: Error!. Buff is NULL ")));
        return;
    }

    stRSP_GET_VERSION* versionInfo = (stRSP_GET_VERSION*) buff;
//    qWarning() << "Verion Bootloader : v"<<versionInfo->BootloaderMajorVersion <<"."<<versionInfo->BootloaderMinorVersion;
//    qWarning() << "           Kernel : v" << versionInfo->KernelMajorVersion <<"."<<versionInfo->KernelMinorVersion;
//    qWarning() << "      Application : v"<< versionInfo->ApplicationMajorVersion<<"."<<versionInfo->ApplicationMinorVersion<<"."<<versionInfo->ApplicationMicroVersion;
    QString strBuildTime((char*)versionInfo->BuildTime);
//    qWarning() << "       Build Time : " << strBuildTime;

    emit rspGetVersion(versionInfo->BootloaderMajorVersion, versionInfo->BootloaderMinorVersion,
                       versionInfo->KernelMajorVersion, versionInfo->KernelMinorVersion,
                       versionInfo->ApplicationMajorVersion, versionInfo->ApplicationMinorVersion, versionInfo->ApplicationMicroVersion,
                       strBuildTime, versionInfo->BootCount);
    return;
}

void
DABCommManager::ReqSetSeekService(quint8 band, quint8 direction, quint8 filterType, quint8 filterValueLen, QByteArray filterValue, quint32 frequency, quint32 serviceID, quint8 subChId)
{
    if(direction != 0x00 && direction != 0x01){
        qWarning()<<"ERROR Unknown direction:"<<direction;
        return;
    }

    quint8 cmd[] = { 0xA5, 0x0D, 0x16, 0x00,
                     0x01,       // Packet Type
                     0x03, 0x00, // Packet Command
                     0x00,       // Packet Reservced
                     0x00, 0x00, // Packet Length

                     0x00,       // Band[10]
                     0x00,       // Direction[11]

                     0x00,       // Frequency //v1.3[12]
                     0x00,
                     0x00,
                     0x00,
                     0x00,       // ServiceID //v1.3[16]
                     0x00,
                     0x00,
                     0x00,
                     0x00,       // SubChId //v1.3[20]

                     0x00,       // FilterType[21]
                     0x00,       // Filter Value Length[22]

                     0x00, 0x00, 0x00, // Padding

                     0x00, 0x00 //CRC
                     };

    //Set Band
    cmd[10] = band;

    //Set Direction
    cmd[11] = direction;
    quint32* freq = (quint32*)(cmd+12);
    *freq = frequency;
    quint32* SID = (quint32*)(cmd+16);
    *SID = serviceID;
    cmd[20] = subChId;
    cmd[21] = filterType;
    cmd[22] = filterValueLen;

    if(filterType == 0x00) // Full Serarch
    {
        quint16* pPacketLen = (quint16*)(cmd+8);
        quint16 packetLen = 0x0D;
        *pPacketLen = packetLen;
    }
    else
    {
        int paddingCount = 0;
        int paddingIndex = 0;

        quint16* pPacketLen = (quint16*)(cmd+8);
        quint16 packetLen = 0x0D + filterValueLen;
        *pPacketLen = packetLen;
        paddingCount = (4-(packetLen % 4))%4;
//        qWarning("FilterValue : cmd[9] = 0x%02X", cmd[9]);
//        qWarning("FilterValue : cmd[10] = 0x%02X", cmd[10]);
        for(int i=1; i<filterValue.size(); i++)
        {
            if(filterValue.at(i) == '1')
            {
                cmd[23+paddingIndex] = i;
//                qWarning("FilterValue : cmd[%d] = %d", 23+paddingIndex, i);
                paddingIndex++;
            }
        }

        qWarning(" packetLen : %d Padding count : %d", packetLen, paddingCount);
        for(int i=0; i<paddingCount; i++)
        {
            cmd[10+packetLen+i] = 0x00;
//            qWarning("Padding cmd[%d] = 0x00", 10+packetLen+i);
        }
    }
    //Calculate CRC
    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    // Send Command
    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
    {
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;
    }

    memset(&m_xDataBackup,0,sizeof(m_xDataBackup));
    memcpy(m_xDataBackup.data, cmd, size);
    m_xDataBackup.size = size;
    m_xDataBackup.cmd = REQ_SET_SEEK_SERVICE;
    return;
}

void
DABCommManager::RspSetSeekService(unsigned char* buff, int size)
{
    if(size <= 0){
        qWarning() << "Error!. Size is " << size;
        return;
    }else if( size < (int)sizeof(stRSP_SET_SEEK_SERVICE)){
        qWarning() << "Error!. Data size("<<size<<") is smaller than stRSP_SET_SEEK_SERVICE(" << sizeof(stRSP_SET_SEEK_SERVICE) <<")";
        return;
    }
    if(buff == NULL){
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    stRSP_SET_SEEK_SERVICE* resp = (stRSP_SET_SEEK_SERVICE*)buff;
//    qWarning() << " Band: " << resp->Band;
//    qWarning() << " Directoin: " << resp->Direction << " (0:Upward, 1:Download)";
//    qWarning() << " Filter Type: " << resp->FilterType << " (0: None, 1:Pty, 2:Language)";
//    qWarning() << " Filter Value: " << resp->FilterValue;
//    qWarning() << " Frequency: " << resp->Frequency;
//    qWarning() << " Service ID: " << resp->ServiceID;
//    qWarning() << " SubChID: " << resp->SubChId;

    emit rspSetSeekService((eREQ_SET_FULL_SCAN_BAND)resp->Band, resp->Direction, resp->FilterType, resp->FilterValue, resp->Frequency, resp->ServiceID, resp->SubChId);
    return;
}

void
DABCommManager::ReqSetSeekStop(void)
{
    quint8 cmd[] = { 0xA5, 0x0D, 0x06, 0x00,
                     0x01,       //Packet Type
                     0x011, 0x00, //Packet Command
                     0x00,       //Packet Reserved
                     0x00, 0x00, //Packet Data Length
                     0x00, 0x00, //CRC
                   };

    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    int size = sizeof(cmd); //cmdReqGetVersion is global const variable..
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;

    return;
}

void
DABCommManager::RspSetSeekStop(unsigned char* buff, int size)
{
    if(size == 0)
    {
        qWarning() << " =====> No Channel List";
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }
}

void
DABCommManager::ReqSetSelectStop(void)
{    
    quint8 cmd[] = { 0xA5, 0x0D, 0x06, 0x00,
                     0x01,       //Packet Type
                     0x012, 0x00, //Packet Command
                     0x00,       //Packet Reserved
                     0x00, 0x00, //Packet Data Length
                     0x00, 0x00, //CRC
                   };

    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    int size = sizeof(cmd); //cmdReqGetVersion is global const variable..
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;

    return;
}

void
DABCommManager::RspSetSelectStop(unsigned char* buff, int size)
{  
    if(size == 0)
    {
        qWarning() << " =====> No Channel List";
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }
}

void
DABCommManager::RegSetServiceFollowing(int mode)
{
    quint8 cmd[] = { 0xA5, 0x0D, 0x0A, 0x00,
                     0x01,       //Packet Type
                     0x013, 0x00, //Packet Command
                     0x00,       //Packet Reserved
                     0x01, 0x00, //Packet Data Length
                     0x00,
                     0x00, 0x00, 0x00,  //Padding
                     0x00, 0x00, //CRC
                   };

    cmd[10] = (quint8)mode;

    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;

    return;
}

void
DABCommManager::RspSetServiceFollowing(unsigned char* buff, int size)
{
    if(size == 0)
    {
        qWarning() << " =====> No Channel List";
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

//    quint8 flag = buff[0];

//    switch(flag)
//    {
//        case 0x00:  //AUTO
//            qWarning() << "RspSetServiceFollowing : flag = AUTO";
//            break;
//        case 0x01:  //DAB to DAB
//            qWarning() << "RspSetServiceFollowing : flag = DAB to DAB";
//            break;
//        case 0x02:  //DAB to FM
//            qWarning() << "RspSetServiceFollowing : flag = DAB to FM";
//            break;
//    }
}

void
DABCommManager::ReqSetSelectService(quint32 frequency, quint8 serviceType, quint32 serviceID, quint8 subChId, quint16 bitrate, quint8 subCHSize, quint16 address, quint8 ps, const char* label, quint8 picodeCount, quint16* picode)
{
    quint8 cmd[sizeof(stREQ_SET_SELECT_SERVICE)] = {0,};
    stREQ_SET_SELECT_SERVICE* pCmd = (stREQ_SET_SELECT_SERVICE *)cmd;

    pCmd->startSymbol               = DAB_FRAME_START_SYMBOL;
    pCmd->continuity                = (0x00 | DAB_CONTINUITY_ONE_N_ONLY_PACKET |  DAB_DEVICE_UART1);//0x0D;
    if(pCmd->continuity != 0x0D) qWarning() << " Error in Frame!!!!!!!!!! 0x0D error.";
    pCmd->frameLength               = (1+2+1+2+ sizeof(stRSP_SET_SELECT_SERVICE));
    pCmd->packetType                = DAB_PACKET_TYPE_REQUEST;//Request:0x01, Response:0x02, Event: 0x04
    pCmd->packetCmd                 = REQ_SET_SELECT_SERVICE;
    pCmd->packetReserved            = 0x00;
    pCmd->packetLength              = sizeof(stRSP_SET_SELECT_SERVICE);
    pCmd->packetData.Frequency      = frequency;
    pCmd->packetData.ServiceType    = serviceType;
    pCmd->packetData.ServiceID      = serviceID;
    pCmd->packetData.SubChId        = subChId;
    pCmd->packetData.Bitrate        = bitrate;
    pCmd->packetData.SubCHSize      = subCHSize;
    pCmd->packetData.Address        = address;
    pCmd->packetData.ps             = ps;
    int labelLength = strlen(label);
    if( labelLength > 16)
    {
        qDebug() << "Label size is exceed buffer. size is " << labelLength; //sizeof(label); //# 20130327 prevent
        labelLength = 16;
    }
    memcpy(pCmd->packetData.Label, label, labelLength);
    pCmd->packetData.PICodeCount    = picodeCount;
    int picodeValueLength;
    if(picodeCount == 0)
    {
        picodeValueLength = 0;
    }
    else
    {
        for(int i=0; i<picodeCount; i++)
        {
            DABLogger::instance()->Log((QString(" DABCommManager.cpp :: ==================ReqSetSelectService() : PI Code[%1]").arg(picode[i])));
            qDebug() << "DABCommManager.cpp :: ==================ReqSetSelectService() : picode [" << i << "] = " << picode[i];
            pCmd->packetData.PICodeValue[i] = picode[i];
        }
    }
    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    pCmd->frameCRC      = crc;

    int size = sizeof(cmd); //cmdReqGetVersion is global const variable..
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;

    memset(&m_xDataBackup,0,sizeof(m_xDataBackup));
    memcpy(m_xDataBackup.data, cmd, size);
    m_xDataBackup.size = size;
    m_xDataBackup.cmd = REQ_SET_SELECT_SERVICE;
    return;
}

void
DABCommManager::RspSetSelectService(unsigned char* buff, int size)
{
    if(size <= 0){
        qWarning() << "Error!. Size is " << size;
        return;
    }
    if(buff == NULL){
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    stRSP_SET_SELECT_SERVICE* resp = (stRSP_SET_SELECT_SERVICE*)buff;
//    qWarning(" Frequency   : 0x%08X (%d)", resp->Frequency, resp->Frequency);
//    qWarning(" Service Type: 0x%02X (DAB:00, DAB+:01, DMB: 02)", resp->ServiceType);
//    qWarning(" Service ID  : 0x%08X, %d", resp->ServiceID, resp->ServiceID);
//    qWarning(" SubChID     : 0x%02X", resp->SubChId);
//    qWarning(" Bitrate     : 0x%04X", resp->Bitrate);

    char tempBuffer[17] = {0,};
    copyLabelBufferAndRemoveSpace(tempBuffer, sizeof(tempBuffer), (char*)resp->Label, sizeof(resp->Label));
    QString Label((const char*)tempBuffer);
//    qWarning() << " Label       : " << Label << ", " << Label.remove(QChar(' '), Qt::CaseInsensitive);
    emit rspSetSelectService(resp->Frequency, resp->ServiceType, resp->ServiceID, resp->SubChId, resp->Bitrate, Label);
    return;
}

void
DABCommManager::ReqSetAnnouncementFlag(quint16 flag)
{
//    qWarning("Flag: 0x%02X", flag);
//    if(flag & 0x0001) qWarning() << " Alarm is On";
//    if(flag & 0x0002) qWarning() << " Traffic is On";
//    if(flag & 0x0004) qWarning() << " Travel is On";
//    if(flag & 0x0008) qWarning() << " Warning is On";
//    if(flag & 0x0010) qWarning() << " News is On";
//    if(flag & 0x0020) qWarning() << " Weather is On";
//    if(flag & 0x0040) qWarning() << " Event is On";
//    if(flag & 0x0080) qWarning() << " Special is On";
//    if(flag & 0x0100) qWarning() << " Rad_info is On";
//    if(flag & 0x0200) qWarning() << " Sports is On";
//    if(flag & 0x0400) qWarning() << " Finance is On";

    quint8 cmd[] = { 0xA5, 0x0D, 0x0A, 0x00,
                     0x01,       // Packet Type: Request 0x01
                     0x05, 0x00, // Packet Command: REQ_SET_ANNOUNCEMENT_FLAG: 0x0005
                     0x00,       // Packet Reserved
                     0x02, 0x00, // Packet Size
                     0x00, 0x00, //flag
                     0x00, 0x00, //padding
                     0xFF, 0xFF, //CRC
                   };
    //Set Flag
    quint16* pFlag = (quint16*)(cmd+10);
    *pFlag = flag;

    //Calculate CRC
    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    // Send Command
    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
    {
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;
    }
}

void
DABCommManager::RspSetAnnouncementFlag(unsigned char* buff, int size)
{
    if(size <= 0)
    {
        qWarning() << "Error!. Size is " << size;
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    stRSP_SET_ANNOUNCEMENT_FLAG* resp = (stRSP_SET_ANNOUNCEMENT_FLAG*) buff;

//    qWarning() << "Flag     : " << resp->Flag;
//    qWarning() << " Alam    : " << ((resp->Flag&0x0001)?"On":"Off");
//    qWarning() << " Traffic : " << ((resp->Flag&0x0002)?"On":"Off");
//    qWarning() << " Travel  : " << ((resp->Flag&0x0004)?"On":"Off");
//    qWarning() << " Warning : " << ((resp->Flag&0x0008)?"On":"Off");
//    qWarning() << " News    : " << ((resp->Flag&0x0010)?"On":"Off");
//    qWarning() << " Weather : " << ((resp->Flag&0x0020)?"On":"Off");
//    qWarning() << " Event   : " << ((resp->Flag&0x0040)?"On":"Off");
//    qWarning() << " Special : " << ((resp->Flag&0x0080)?"On":"Off");
//    qWarning() << " Rad_info: " << ((resp->Flag&0x0100)?"On":"Off");
//    qWarning() << " Sports  : " << ((resp->Flag&0x0200)?"On":"Off");
//    qWarning() << " Finance : " << ((resp->Flag&0x0400)?"On":"Off");

    emit rspSetAnnouncementFlag(resp->Flag);
    return;
}

void
DABCommManager::ReqGetServiceList(unsigned int frequency)
{
    quint8 cmdReqGetServiceList[] = { 0xA5, 0x0D, 0x0A, 0x00,
                                      0x01,
                                      0x06, 0x00,
                                      0x00,
                                      0x04, 0x00,
                                      0x00, 0x00, 0x00, 0x00,
                                      0xCF, 0x29
                                    };

//    qWarning(" frequency: %d, 0x%08X => %02X %02X %02X %02X", frequency, frequency, cmdReqGetServiceList[10],cmdReqGetServiceList[11],cmdReqGetServiceList[12],cmdReqGetServiceList[13]);
    cmdReqGetServiceList[10] = frequency & 0xFF;
    cmdReqGetServiceList[11] = (frequency>>8) & 0xFF;
    cmdReqGetServiceList[12] = (frequency>>16) & 0xFF;
    cmdReqGetServiceList[13] = (frequency>>24) & 0xFF;

    ///Calculate CRC
    quint16 crc = calculateCRC16((unsigned char*)cmdReqGetServiceList, sizeof(cmdReqGetServiceList)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmdReqGetServiceList + sizeof(cmdReqGetServiceList)-2);
    *pCrc = crc;

    int size = sizeof(cmdReqGetServiceList); //cmdReqGetServiceList is global variable..
    int returnSize = this->writeCmd(cmdReqGetServiceList, size);
    if(size != returnSize)
    {
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;
    }
}

void
DABCommManager::RspGetServiceList(unsigned char* buff, int size)
{
    if(size <= 0)
    {
        qWarning() << __FUNCTION__ << "Error!. Size is " << size;
//        emit rspGetServiceList();
        return;
    }
    if(buff == NULL)
    {
        qWarning() << __FUNCTION__ << "Error!. Buff is NULL: " << buff ;
        return;
    }
    int index = 0;
    //Ensemble Count
    int ensembleCount = buff[index++];
    //index++;
    int sCount = 0;
    stRSP_GET_ENSEMBLE_LIST* pEnsemble = NULL;
    stRSP_GET_SERVICE_LIST* pService = NULL;
    char labelBuffer[17]; // Label size is 16 in Spec Document.
    QString sLabel;
    QString eLabel;
    for(int i = 0; i < ensembleCount; i++)
    {
        pEnsemble = (stRSP_GET_ENSEMBLE_LIST*)(buff+index);
        index += sizeof(stRSP_GET_ENSEMBLE_LIST);
        sCount = pEnsemble->ServiceCount;
//        qWarning(" ===Ensemble: %d ===================", i+1);
//        qWarning(" Frequency    : 0x%08X (%d)", pEnsemble->Frequency, pEnsemble->Frequency);
//        qWarning(" EnsembleID   : 0x%04X (%d)", pEnsemble->EnsembleID, pEnsemble->EnsembleID);
        copyLabelBufferAndRemoveSpace(labelBuffer, sizeof(labelBuffer), (char*)pEnsemble->EnsembleLabel, sizeof(pEnsemble->EnsembleLabel));

        QTextCodec *codec = NULL;
        QByteArray byteArray;
        switch(pEnsemble->EnsembleCharset)
        {
            case 0x88:
                byteArray.append((const char*)labelBuffer, sizeof(labelBuffer));
                codec = QTextCodec::codecForName("EUC-KR");
                eLabel = codec->toUnicode(byteArray.data());
                break;

            case 0x60:
            case 0x15:
                byteArray.append((const char*)labelBuffer, sizeof(labelBuffer));
                codec = QTextCodec::codecForName("UTF-16"); //for USC2
                eLabel = codec->toUnicode(byteArray.data());
                break;

            case 0x00: //EBU Latin based Character
                eLabel = ebuLatinToUnicode((unsigned char*)labelBuffer, sizeof(labelBuffer));
                break;

            case 0x06:
            default:
                byteArray.append((const char*)labelBuffer, sizeof(labelBuffer));
                codec = QTextCodec::codecForName("UTF-8");
                eLabel = codec->toUnicode(byteArray.data());
                break;
        }
        if(eLabel == "")
        {
            qDebug() << __FUNCTION__ << " ==================== Ensemble label is Null !!. so the frequency is ignored !!";
            return;
        }
        qDebug() << " ================================= EnsembleLabel : " << eLabel << " ===================================";
        qDebug(" ServiceCount : 0x%02X (%d)", pEnsemble->ServiceCount, pEnsemble->ServiceCount);
        for(int j = 0; j < sCount; j++)
        {
            pService = (stRSP_GET_SERVICE_LIST*)(buff+index);
            qDebug() << "    ==== Service:" << j+1 << " =======";
            qDebug("      Service ID        : 0x%08X (%d)", pService->ServiceID, pService->ServiceID);
            qDebug("      Service Type      : 0x%02X (%d)", pService->ServiceType, pService->ServiceType);
            qDebug("      SubChId           : 0x%02X (%d)", pService->SubChId, pService->SubChId);
            qDebug("      Bitrate           : 0x%04X (%d)", pService->Bitrate, pService->Bitrate);
            qDebug("      PS                : 0x%02X (%d)", pService->PS, pService->PS);
            qDebug("      Charset           : 0x%02X (%d)", pService->Charset, pService->Charset);
            qDebug("      CharsetFlag       : 0x%02X (%d)", pService->CharsetFlag, pService->CharsetFlag);
            qDebug("      PTY               : 0x%02X (%d)", pService->ProgrammeType, pService->ProgrammeType);
            copyLabelBufferAndRemoveSpace(labelBuffer, sizeof(labelBuffer), (char*)pService->Label, sizeof(pService->Label));
            QTextCodec *codec = NULL;
            QByteArray tempArray;
            switch(pService->Charset)
            {
                case 0x88:
                    tempArray.append((const char*)labelBuffer, sizeof(labelBuffer));
                    codec = QTextCodec::codecForName("EUC-KR");
                    sLabel = codec->toUnicode(tempArray.data());
                    break;

                case 0x60:
                case 0x15:
                    tempArray.append((const char*)labelBuffer, sizeof(labelBuffer));
                    codec = QTextCodec::codecForName("UTF-16"); //for USC2
                    sLabel = codec->toUnicode(tempArray.data());
                    break;

                case 0x00: //EBU Latin based Character
                    sLabel = ebuLatinToUnicode((unsigned char*)labelBuffer, sizeof(labelBuffer));
                    break;

                case 0x06:
                default:
                    tempArray.append((const char*)labelBuffer, sizeof(labelBuffer));
                    codec = QTextCodec::codecForName("UTF-8");
                    sLabel = codec->toUnicode(tempArray.data());
                    break;
            }

            qDebug() << " Serivce Label: " << sLabel;
            qDebug("      ASU Flags         : 0x%04X (%d)", pService->ASUFlags, pService->ASUFlags);
            qDebug("      ASU ClusterCount  : 0x%02X (%d)", pService->ASUClusterCount, pService->ASUClusterCount);
            for(int k=0; k < pService->ASUClusterCount; k++)
            {
            qDebug("      ASU ClusterIDs[%d]: 0x%02X (%d)", k, pService->ASUClusterIDs[k], pService->ASUClusterIDs[k]);
            }
            qDebug("      PICode Count      : 0x%02X (%d)", pService->PICodeCount, pService->PICodeCount);
            for(int k=0; k < pService->PICodeCount; k++)
            {
            qDebug("      PICode Type[%d]   : 0x%02X (%d)", k, pService->PICodeType[k], pService->PICodeType[k]);
            qDebug("      PICode Value[%d]  : 0x%04X (%d)", k, pService->PICodeValue[k], pService->PICodeValue[k]);
            }
            qDebug("      PICode Validation : 0x%04X (%d)", pService->PICodeValidation, pService->PICodeValidation);
            qDebug("      SUB Channel Size  : 0x%02X (%d)", pService->SubCHSize, pService->SubCHSize);
            qDebug("      Address           : 0x%04X (%d)", pService->Address, pService->Address);

            index += sizeof(stRSP_GET_SERVICE_LIST);

            stASU_INFO asuInfo;
            memset(&asuInfo, 0x00, sizeof(asuInfo));

            asuInfo.ASUFlags = pService->ASUFlags;
            asuInfo.ASUClusterCount = pService->ASUClusterCount;

            if(asuInfo.ASUClusterCount > 22)
                return;

            memcpy((quint8*)asuInfo.ASUClusterIDs, (quint8*)pService->ASUClusterIDs, sizeof(pService->ASUClusterIDs));

            stPICODE_INFO picodeInfo;
            memset(&picodeInfo, 0x00, sizeof(picodeInfo));
            picodeInfo.PICodeCount = pService->PICodeCount;

            if(picodeInfo.PICodeCount > 11)
                return;

            memcpy((quint8*)picodeInfo.PICodeType, (quint8*)pService->PICodeType, sizeof(pService->PICodeType));
            memcpy((quint16*)picodeInfo.PICodeValue, (quint16*)pService->PICodeValue, sizeof(pService->PICodeValue));
            picodeInfo.PICodeValidation = pService->PICodeValidation;

            emit sendChannelItem(pEnsemble->Frequency,
                                   pEnsemble->EnsembleID,
                                   pEnsemble->InternationalTableID,
                                   eLabel,
                                   pEnsemble->ServiceCount,
                                   pService->ServiceID,
                                   pService->ServiceType,
                                   pService->SubChId,
                                   pService->Bitrate,
                                   pService->PS,
                                   pService->Charset,
                                   pService->ProgrammeType,
                                   pService->Language,
                                   sLabel,
                                   asuInfo,
                                   picodeInfo,
                                   pService->SubCHSize,
                                   pService->Address
                                   );
        }
    }

    emit rspGetServiceList();
    return;
}

void
DABCommManager::ReqGetDLS(void)
{
    quint8 cmd[] = { 0xA5, 0x0D, 0x06, 0x00,
                     0x01,       //Packet Type
                     0x07, 0x00, //Packet Command
                     0x00,       //Packet Reserved
                     0x00, 0x00, //Packet Data Length
                     0x0E, 0x63, //CRC
                   };

    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
    {
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;
    }
    return;
}

void
DABCommManager::RspGetDLS(unsigned char* buff, int size)
{
    if(size == 0){
        qWarning() << " =====> No Channel List";
        return;
    }
    if(buff == NULL){
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    int index = 0;    
    int iDLSStringLength = buff[index++];

    for(int i=index; i<iDLSStringLength; i++)
    {
        if(buff[i] >= 0x01 && buff[i] <= 0x1F)
        {
            qDebug() << __FUNCTION__ << " =========================== This character is control character !!. so replaced space character !! =========================";
            buff[i] = 0x20;
        }
    }

    index += iDLSStringLength;
    unsigned char charset = buff[index++];
    QTextCodec *codec = NULL;
    QByteArray byteArray;
    QString strDLS;

    switch(charset)
    {
        case 0x88:
            byteArray.append((const char*)(buff+1), iDLSStringLength);
            codec = QTextCodec::codecForName("EUC-KR");
            strDLS = codec->toUnicode(byteArray.data());
            break;

        case 0x60:
        case 0x15:
            byteArray.append((const char*)(buff+1), iDLSStringLength);
            codec = QTextCodec::codecForName("UTF-16"); //for USC2
            strDLS = codec->toUnicode(byteArray.data());
            break;

        case 0x00: //EBU Latin based Character
            strDLS = ebuLatinToUnicode(buff+1, iDLSStringLength);
            break;

        case 0x06:
        default:
            byteArray.append((const char*)(buff+1), iDLSStringLength);
            codec = QTextCodec::codecForName("UTF-8");
            strDLS = codec->toUnicode(byteArray.data());
            break;
    }

    if(iDLSStringLength != 0 && (strDLS.length() == 0))
    {
        qWarning() << " ==================== incorrect DLS Data!!. skip!!!";
        return;
    }

    int DLSPlusCommandLength = buff[index++];

    quint8 mByteBlock = 0;
    quint8 mCld, mIT, mIR, mNT, mContentType, mStartMarker, mLengthMarker = 0;
    QString strContent = "";
    QString sTitle = "";
    QString sArtist = "";
    QString sAlbum = "";
    bool mIRInterrupted = false;

    if(DLSPlusCommandLength > 0)
    {
        mByteBlock = buff[index++];
        mCld = (mByteBlock & 0xF0) >> 4;
        if(mCld == 0)
        {
            mIT = (mByteBlock & 0x08) >> 3;
            mIR = (mByteBlock & 0x04) >> 2;
            mNT = (mByteBlock & 0x03);

            if(mIR == 0)
            {
                mIRInterrupted = true;
                qDebug("() =======================  mIRInterrupted = true ================");
            }

            for( int iTag =0; iTag <= mNT/* && !mIRInterrupted*/; iTag++)
            {
                mByteBlock = buff[index++];
                mContentType = (mByteBlock & 0x7F);
                mByteBlock = buff[index++];
                mStartMarker = (mByteBlock & 0x7F);
                mByteBlock = buff[index++];
                mLengthMarker = (mByteBlock & 0x7F);

                if((mContentType + mStartMarker + mLengthMarker) == 0)
                {
                    qWarning() << " DLS Lenght == 0 !!";
                    break;
                }

                if(mContentType < 64)
                {
                    QTextCodec *codec = NULL;
                    codec = QTextCodec::codecForName("UTF-8");
                    strContent += codec->toUnicode(DLPlus_ContentType[mContentType]);
                    strContent += " : ";

                    if((mStartMarker + mLengthMarker) <= iDLSStringLength)
                    {
                        if(mContentType == 1) //Title
                        {
                            sTitle = strDLS.mid(mStartMarker, mLengthMarker+1);
                        }
                        else if(mContentType == 2) //Album
                        {
                            sAlbum = strDLS.mid(mStartMarker, mLengthMarker+1);
                        }
                        else if(mContentType == 4) //Artist
                        {
                            sArtist = strDLS.mid(mStartMarker, mLengthMarker+1);
                        }
                        else
                        {
                            strContent += strDLS.mid(mStartMarker, mLengthMarker+1);
                            strContent += " , ";
                        }
                    }
                    else
                    {
                        qWarning() << " Here is return;";
                        break;
                    }
                }
            }
        }
    }

//    qDebug() <<"============DLS==========================";
//    qDebug() << "  DLS  : " << strDLS;
//    qDebug() <<"============DLS PLUS=====================";
//    qDebug() << "Title  : " << sTitle;
//    qDebug() << "Album  : " << sAlbum;
//    qDebug() << "Artist : " << sArtist;
//    qDebug() << " ETC   : " << strContent;
//    qDebug() <<"============END==========================";

#if 1
    if(mIRInterrupted)
    {
        sTitle  = QApplication::translate("StringInfo", "STR_DAB_PLAYER_NO_INFO");
        sArtist = QApplication::translate("StringInfo", "STR_DAB_PLAYER_NO_INFO");
        sAlbum  = QApplication::translate("StringInfo", "STR_DAB_PLAYER_NO_INFO");
    }
#endif

    emit rspGetDLS(strDLS, sTitle, sAlbum, sArtist);
    return;
}

void
DABCommManager::ReqGetSLS(void)
{
    quint8 cmd[] = { 0xA5, 0x0D, 0x06, 0x00,
                     0x01,       //Packet Type
                     0x08, 0x00, //Packet Command : REQ_GET_SLS: 0x0008
                     0x00,       //Packet Reserved
                     0x00, 0x00, //Packet Data Length
                     0xF7, 0x06, //CRC
                   };

    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
    {
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;
    }
    return;
}

void
DABCommManager::RspGetSLS(unsigned char* buff, int size)
{
    if(size <= 0)
    {
        qWarning() << "Error!. Size is " << size;
        return;
    }

    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

//    int index = 0;
//    quint8 ObjectType = buff[index++];
//    switch(ObjectType)
//    {
//        case 0x00: //JPEG
//            break;

//        case 0x01: //PNG
//            break;

//        default:
//            qWarning() << " ERROR! Unknown Object Type: " << ObjectType;
//            break;
//    }
    return;
}

void
DABCommManager::ReqGetEPG(void)
{
    quint8 cmd[] = { 0xA5, 0x0D, 0x06, 0x00,
                     0x01,       //Packet Type
                     0x09, 0x00, //Packet Command : REQ_GET_EPG: 0x0009
                     0x00,       //Packet Reserved
                     0x00, 0x00, //Packet Data Length
                     0x6D, 0x2C, //CRC
                   };

    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
    {
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;
    }
    return;
}

void
DABCommManager::ReqSetEvtSendTime(quint8 status)
{
    quint8 cmd[]  = { 0xA5, 0x0D,
                      0x0A, 0x00,           //Packet Length
                      0x01,                 //Packet Type
                      0x0D, 0x00,           //Packet Command
                      0x00,                 //Packet Reserved
                      0x01, 0x00,           //Packet Data Length
                      0x00,                 //Packet Data
                      0x00, 0x00,0x00,      //Padding
                      0x00, 0x00            // CRC
                    };

    //set EvtSendTime status
    cmd[10] = status;

    //Calculate CRC
    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    // Send Command
    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
    {
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;
    }
    return;
}

void
DABCommManager::RspSetEvtSendTime(unsigned char* buff, int size)
{
    qWarning() << Q_FUNC_INFO;
    if(size < 0)
    {
        qWarning() << "Error!. Size is " << size;
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    quint8 status = buff[0];

    switch(status)
    {
        case 0x00: //stop
            qWarning() << " EVT_SEND_TIME Event Stop..";
            break;
        case 0x01:  //start
            qWarning() << " EVT_SEND_TIME Event start..";
            break;
    }
    return;
}

void
DABCommManager::ReqSetDrcFlag(quint8 flag)
{
    quint8 cmd[]  = { 0xA5, 0x0D,
                      0x0A, 0x00,           //Packet Length
                      0x01,                 //Packet Type
                      0x0E, 0x00,           //Packet Command
                      0x00,                 //Packet Reserved
                      0x01, 0x00,           //Packet Data Length
                      0x00,                 //Packet Data
                      0x00, 0x00,0x00,      //Padding
                      0x00, 0x00            // CRC
                    };

    //set DRC Flag
    cmd[10] = flag;

    //Calculate CRC
    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    // Send Command
    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
    {
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;
    }
    return;
}

void
DABCommManager::RspSetDrcFlag(unsigned char* buff, int size)
{
    if(size < 0)
    {
        qWarning() << "Error!. Size is " << size;
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

//    quint8 flag = buff[0];

//    switch(flag)
//    {
//        case 0x00:  //DRC Off
//            qWarning() << " SET DRC FLAG Event DRC Off..";
//            break;
//        case 0x01:  //DRC Half
//            qWarning() << " SET DRC FLAG Event DRC Half..";
//            break;
//        case 0x02:  //DRC Full
//            qWarning() << " SET DRC FLAG Event DRC Full..";
//            break;
//    }

    return;
}

void
DABCommManager::ReqSetSelectAnnouncementService(quint32 frequency, quint8 serviceType, quint32 serviceID, quint8 subChId, quint16 bitrate, quint8 subCHSize, quint16 address, const char* label, quint8 picodeCount, const char* picodeValue, quint16 flags, quint8 status)
{
//    qWarning() << Q_FUNC_INFO;

//    qWarning(" Frequency   : 0x%08X (%d)", frequency, frequency);
//    qWarning(" Service Type: 0x%02X (DAB:00, DAB+:01, DMB: 02)", serviceType);
//    qWarning(" Service ID  : 0x%08X, %d", serviceID, serviceID);
//    qWarning(" SubChID     : 0x%02X", subChId);
//    qWarning(" Bitrate     : 0x%04X", bitrate);
//    qWarning() << " Label       : " << label;
//    qWarning(" Flags       : 0x%04X", flags);
//    qWarning(" Status      : 0x%02X", status);

    quint8 cmd[sizeof(stREQ_SET_SELECT_ANNOUNCEMENT_SERVICE)] = {0,};

    stREQ_SET_SELECT_ANNOUNCEMENT_SERVICE* pCmd = (stREQ_SET_SELECT_ANNOUNCEMENT_SERVICE *)cmd;

    pCmd->startSymbol               = DAB_FRAME_START_SYMBOL;
    pCmd->continuity                = (0x00 | DAB_CONTINUITY_ONE_N_ONLY_PACKET |  DAB_DEVICE_UART1);    //0x0D;
    pCmd->frameLength               = (1+2+1+2+1+sizeof(stRSP_SET_SELECT_ANNOUNCEMENT_SERVICE)); // packetType+packetCmd+packetReserved+packetData+padding
    pCmd->packetType                = DAB_PACKET_TYPE_REQUEST;                                          //Request:0x01, Response:0x02, Event: 0x04
    pCmd->packetCmd                 = REQ_SET_SELECT_ANNOUNCEMENT_SERVICE;
    pCmd->packetReserved            = 0x00;
    pCmd->packetLength              = sizeof(stRSP_SET_SELECT_ANNOUNCEMENT_SERVICE);
    pCmd->packetData.Frequency      = frequency;
    pCmd->packetData.ServiceType    = serviceType;
    pCmd->packetData.ServiceID      = serviceID;
    pCmd->packetData.SubChId        = subChId;
    pCmd->packetData.Bitrate        = bitrate;
    pCmd->packetData.Flags          = flags;
    pCmd->packetData.Status         = status;
    pCmd->packetData.SubCHSize      = subCHSize;
    pCmd->packetData.Address        = address;
    pCmd->packetData.ps             = 0x01;
    pCmd->padding                   = 0x00;

    int labelLength = strlen(label);
    if( labelLength > 16)
    {
        qWarning() << "Label size is exceed buffer. size is " << labelLength;   //sizeof(label);  //# 20130327 prevent
        labelLength = 16;
    }
    memcpy(pCmd->packetData.Label, label, labelLength);

    pCmd->packetData.PICodeCount    = picodeCount;
    int picodeValueLength;
    if(picodeCount == 0)
    {
        picodeValueLength = 0;
    }
    else
    {
        picodeValueLength = strlen(picodeValue);
        if( picodeValueLength > 12)
        {
            qWarning() << "picodeTypeLength size is exceed buffer. size is " << sizeof(picodeValueLength);
            picodeValueLength = 12;
        }

        memcpy(pCmd->packetData.PICodeValue, picodeValue, picodeValueLength);
    }

    //padding check;
    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    pCmd->frameCRC      = crc;

    int size = sizeof(cmd); //cmdReqGetVersion is global const variable..
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;

    return;
}

void
DABCommManager::RspSetSelectAnnouncementService(unsigned char* buff, int size)
{
//    qWarning() << Q_FUNC_INFO << "Size:" << size;

    if(size <= 0){
        qWarning() << "Error!. Size is " << size;
        return;
    }

    if(buff == NULL){
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

//    stRSP_SET_SELECT_ANNOUNCEMENT_SERVICE* resp = (stRSP_SET_SELECT_ANNOUNCEMENT_SERVICE*)buff;
//    qWarning(" Frequency   : 0x%08X (%d)", resp->Frequency, resp->Frequency);
//    qWarning(" Service Type: 0x%02X (DAB:00, DAB+:01, DMB: 02)", resp->ServiceType);
//    qWarning(" Service ID  : 0x%08X, %d", resp->ServiceID, resp->ServiceID);
//    qWarning(" SubChID     : 0x%02X", resp->SubChId);
//    qWarning(" Bitrate     : 0x%04X", resp->Bitrate);
//    char tempBuffer[17] = {0,};
//    copyLabelBufferAndRemoveSpace(tempBuffer, sizeof(tempBuffer), (char*)resp->Label, sizeof(resp->Label));
//    QString Label((const char*)tempBuffer);
//    qWarning() << " Label       : " << Label << ", " << Label.remove(QChar(' '), Qt::CaseInsensitive);
//    qWarning(" Flags       : 0x%04X", resp->Flags);
//    qWarning(" Bitrate     : 0x%02X", resp->Status);
}

void
DABCommManager::ReqGetAnnouncementStatus(void)
{
    qDebug() << Q_FUNC_INFO;
    quint8 cmd[] = { 0xA5, 0x0D, 0x06, 0x00,
                     0x01,       //Packet Type
                     0x30, 0x00, //Packet Command
                     0x00,       //Packet Reserved
                     0x00, 0x00, //Packet Data Length
                     0x0E, 0x63, //CRC
                   };

    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
    {
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;
    }
    return;
}

void
DABCommManager::RspGetAnnouncementStatus(unsigned char* buff, int size)
{
    qDebug() << Q_FUNC_INFO <<"Buff:"<<buff<< "Size:" <<size;

    if(size <= 0)
    {
        qDebug() << "Error!. buffLen is " << size;
        return;
    }
    if(buff == NULL)
    {
        qDebug() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    stEVT_SEND_ANNOUNCEMENT* resp = (stEVT_SEND_ANNOUNCEMENT*)buff;

    qDebug() << "Flag     : " << bin << resp->Flag;
    qDebug() << " Alam    : " << ((resp->Flag&0x0001)?"On":"Off");
    qDebug() << " Traffic : " << ((resp->Flag&0x0002)?"On":"Off");

    switch(resp->Status)
    {
        case 0x00: // Start
            qDebug() << " Announcement Finished...";
            break;
        case 0x01: // Finish
            qDebug() << " Announcement Start.";
            break;
        default:
            qDebug(" ERROR Unknown Announcement Status: 0x%02X", resp->Status);
            break;
    }

    char labelBuffer[17] = {0,};
    copyLabelBufferAndRemoveSpace(labelBuffer, sizeof(labelBuffer), (char*)resp->taService.Label, sizeof(resp->taService.Label));
    QString label = ebuLatinToUnicode((unsigned char*)labelBuffer, sizeof(labelBuffer));
    emit evtSendAnnouncement(label, resp->Flag, (eEVT_SEND_ANNOUNCEMENT_STATUS)resp->Status, resp->taService.SubChId);
    return;
}

void
DABCommManager::ReqGetPinkStatus(void)
{
    qDebug() << Q_FUNC_INFO;
    quint8 cmd[] = { 0xA5, 0x0D, 0x06, 0x00,
                     0x01,       //Packet Type
                     0x31, 0x00, //Packet Command
                     0x00,       //Packet Reserved
                     0x00, 0x00, //Packet Data Length
                     0x0E, 0x63, //CRC
                   };

    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
    {
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;
    }
    return;
}

void
DABCommManager::RspGetPinkStatus(unsigned char* buff, int buffLen)
{
    if(buffLen <= 0)
    {
        qWarning() << "Error!. buffLen is " << buffLen;
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }
    quint8 status = buff[0];
    emit evtSendPinkStatus(status);
}

void
DABCommManager::ReqSetAutoScan(quint8 band, quint8 action, quint8 onOff)
{
    quint8 cmd[] = { 0xA5, 0x0D, 0x0A, 0x00,
                     0x01,       // Packet Type: Request 0x01
                     0x15, 0x00, // Packet Command: REQ_SET_ANNOUNCEMENT_FLAG: 0x0005
                     0x00,       // Packet Reserved
                     0x03, 0x00, // Packet Size
                     0x00,       // Band
                     0x00,       // Action
                     0x00,       // Debug On/Off
                     0x00,       // padding
                     0xFF, 0xFF, // CRC
                   };

//    qWarning(" Band        : 0x%02X", band);
//    qWarning(" Action      : 0x%02X", action);

    cmd[10] = band;
    cmd[11] = action;
    cmd[12] = onOff;

    //Calculate CRC
    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    // Send Command
    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
    {
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;
    }
}

void
DABCommManager::RspSetAutoScan(unsigned char* buff, int size)
{
    if(size <= 0){
        qWarning() << "Error!. Size is " << size;
        return;
    }

    if(buff == NULL){
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

//    _stRSP_SET_AUTO_SCAN* resp = (_stRSP_SET_AUTO_SCAN*) buff;

//    switch(resp->band)
//    {
//        case 0x00:
//            qWarning() << " SET FM SERVICE : Band = KOREA..";
//            break;
//        case 0x01:
//            qWarning() << " SET FM SERVICE : Band = EROUPE..";
//            break;
//        case 0x02:
//            qWarning() << " SET FM SERVICE : Band = BAND-III..";
//            break;
//        case 0x03:
//            qWarning() << " SET FM SERVICE : Band = L-BAND..";
//            break;
//    }

//    switch(resp->action)
//    {
//        case 0x00:
//            qWarning() << " SET AUTO SCAN : Action = Auto Scan Start..";
//            break;
//        case 0x01:
//            qWarning() << " SET AUTO SCAN : Action = Auto Scan Stop..";
//            break;
//    }
}

void
DABCommManager::ReqSetFMService(quint8 band, quint32 piCode)
{
    quint8 cmd[] = { 0xA5, 0x0D, 0x0E, 0x00,
                     0x01,       // Packet Type: Request 0x01
                     0x16, 0x00, // Packet Command: REQ_SET_ANNOUNCEMENT_FLAG: 0x0005
                     0x00,       // Packet Reserved
                     0x05, 0x00, // Packet Size
                     0x00,       // Band
                     0x00, 0x00, // piCode
                     0x00, 0x00,
                     0x00,       // padding
                     0x00,       // padding
                     0x00,       // padding
                     0xFF, 0xFF, // CRC
                   };

//    qWarning(" Band        : 0x%02X", band);
//    qWarning(" PI Code     : 0x%08X", piCode);

    //Set Flag
    cmd[10] = band;
    quint32* pPICode = (quint32*)(cmd+11);
    *pPICode = piCode;

    //Calculate CRC
    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    // Send Command
    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
    {
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;
    }
}

void
DABCommManager::RspSetFMService(unsigned char* buff, int size)
{
    if(size <= 0){
        qWarning() << "Error!. Size is " << size;
        return;
    }

    if(buff == NULL){
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

//    _stRSP_SET_FM_SERVICE* resp = (_stRSP_SET_FM_SERVICE*) buff;

//    switch(resp->band)
//    {
//        case 0x00:
//            qWarning() << " SET FM SERVICE : Band = KOREA..";
//            break;
//        case 0x01:
//            qWarning() << " SET FM SERVICE : Band = EROUPE..";
//            break;
//        case 0x02:
//            qWarning() << " SET FM SERVICE : Band = BAND-III..";
//            break;
//        case 0x03:
//            qWarning() << " SET FM SERVICE : Band = L-BAND..";
//            break;
//    }

//    qWarning() << " PI Code = " << resp->PICode;
}


void
DABCommManager::ReqSetBroadcasting(quint8 band)
{
    quint8 cmd[] = { 0xA5, 0x0D, 0x0A, 0x00,
                     0x01,       //Packet Type
                     0x017, 0x00, //Packet Command
                     0x00,       //Packet Reserved
                     0x01, 0x00, //Packet Data Length
                     0x00,
                     0x00, 0x00, 0x00,  //Padding
                     0x00, 0x00, //CRC
                   };

    cmd[10] = (quint8)band;

    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    int size = sizeof(cmd); //cmdReqGetVersion is global const variable..
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;

    return;
}

void
DABCommManager::RspSetBroadcasting(unsigned char* buff, int size)
{
    if(size == 0){
        qWarning() << " =====> No Channel List";
        return;
    }
    if(buff == NULL){
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

//    quint8 band = buff[0];

//    switch(band)
//    {
//        case 0x00:  //AUTO
//            qWarning() << "RspSetBroadcasting : band = DAB Only";
//            break;
//        case 0x01:  //DAB to DAB
//            qWarning() << "RspSetBroadcasting : band = DAB + DMB";
//            break;
//    }
}

void
DABCommManager::ReqSetBandSelection(quint8 band)
{
    quint8 cmd[] = { 0xA5, 0x0D, 0x0A, 0x00,
                     0x01,       //Packet Type
                     0x019, 0x00, //Packet Command
                     0x00,       //Packet Reserved
                     0x01, 0x00, //Packet Data Length
                     0x00,
                     0x00, 0x00, 0x00,  //Padding
                     0x00, 0x00, //CRC
                   };

    cmd[10] = (quint8)band;

    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    int size = sizeof(cmd); //cmdReqGetVersion is global const variable..
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;

    return;
}

void
DABCommManager::ReqGetAudioInfo(void)
{
    quint8 cmd[] = { 0xA5, 0x0D, 0x06, 0x00,
                     0x01,       //Packet Type
                     0x18, 0x00, //Packet Command : REQ_GET_AUDIO_INFO: 0x0018
                     0x00,       //Packet Reserved
                     0x00, 0x00, //Packet Data Length
                     0xF7, 0x06, //CRC
                   };

    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
    {
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;
    }
    return;
}

void
DABCommManager::RspGetAudioInfo(unsigned char* buff, int size)
{
    if(size <= 0){
        qDebug() << Q_FUNC_INFO << "Error!. Size is " << size;
        return;
    }else if( size < (int)sizeof(stRSP_GET_AUDIO_INFO)){
        qDebug() << Q_FUNC_INFO << "Error!. Data size("<<size<<") is smaller than stRSP_GET_AUDIO_INFO(" << sizeof(stRSP_GET_AUDIO_INFO) <<")";
        return;
    }
    if(buff == NULL){
        qDebug() << Q_FUNC_INFO << "Error!. Buff is NULL: " << buff ;
        return;
    }

    stRSP_GET_AUDIO_INFO* resp = (stRSP_GET_AUDIO_INFO*)buff;
//    qDebug() << "======================== RSP GET AUDIO INFO ===========================";
//    qDebug() << " Frequency         : " << resp->Frequency;
//    qDebug() << " Label             : " << resp->Label;
//    qDebug() << " Service Type      : " << resp->ServiceType << " (00: DAB, 01:DAB+/DMB-A, 02:DMB)";
//    qDebug() << " Bitrate           : " << resp->Bitrate;
//    qDebug() << " DAC Sampling rate : " << resp->DACSamplingRate;
//    qDebug() << " Channel Config    : " << resp->ChannelConfig << " (00: Mono, 01:Stereo)";
//    qDebug() << " SBR               : " << resp->Sbr << " (00: Not used, 01:used)";
//    qDebug() << " Protect Type      : " << resp->ProtectType;
//    qDebug() << " Protect Level     : " << resp->ProtectLevel;
//    qDebug() << " Protect Option    : " << resp->ProtectOption << " (00: Not used, 01:used)";

    emit cmdAudioInfo(resp->ProtectType, resp->ProtectLevel, resp->ProtectOption);
}

void
DABCommManager::RspSetBandSelection(unsigned char* buff, int size)
{
    if(size == 0)
    {
        qWarning() << " =====> No Channel List";
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

//    quint8 band = buff[0];

//    switch(band)
//    {
//        case 0x00:  //KOREA
//            qWarning() << "RspSetBandSelection : band = KOREA";
//            break;
//        case 0x01:  //EUROPE
//            qWarning() << "RspSetBandSelection : band = EUROPE";
//            break;
//        case 0x02:  //BAND-III
//            qWarning() << "RspSetBandSelection : band = BAND-III";
//            break;
//        case 0x03:  //L-BAND
//            qWarning() << "RspSetBandSelection : band = L-BAND";
//            break;
//    }
}

void
DABCommManager::ReqSetServiceInfo(quint32 frequency, quint8 serviceType, quint32 serviceID, quint8 subChId, quint16 bitrate, quint8 labelCharset, const char* label, quint8 picodeCount, const char* picodeValue)
{
    quint8 cmd[sizeof(stREQ_SET_SERVICE_INFO)] = {0,};
    stREQ_SET_SERVICE_INFO* pCmd = (stREQ_SET_SERVICE_INFO *)cmd;

    pCmd->startSymbol               = DAB_FRAME_START_SYMBOL;
    pCmd->continuity                = (0x00 | DAB_CONTINUITY_ONE_N_ONLY_PACKET |  DAB_DEVICE_UART1);//0x0D;
    pCmd->frameLength               = (1+2+1+2+ sizeof(stSET_SERVICE_INFO));
    pCmd->packetType                = DAB_PACKET_TYPE_REQUEST;//Request:0x01, Response:0x02, Event: 0x04
    pCmd->packetCmd                 = REQ_SET_SERVICE_INFO;
    pCmd->packetReserved            = 0x00;
    pCmd->packetLength              = sizeof(stSET_SERVICE_INFO);
    pCmd->packetData.Frequency      = frequency;
    pCmd->packetData.ServiceType    = serviceType;
    pCmd->packetData.ServiceID      = serviceID;
    pCmd->packetData.SubChId        = subChId;
    pCmd->packetData.Bitrate        = bitrate;
    pCmd->packetData.labelCharset    = labelCharset;
    int labelLength = strlen(label);
    if( labelLength > 16)
    {
        qWarning() << "Label size is exceed buffer. size is " << labelLength;
        labelLength = 16;
    }
    memcpy(pCmd->packetData.Label, label, labelLength);

    pCmd->packetData.PICodeCount    = picodeCount;
    int picodeValueLength;
    if(picodeCount == 0)
    {
        picodeValueLength = 0;
    }
    else
    {
        picodeValueLength = strlen(picodeValue);
        if( picodeValueLength > 12)
        {
            qWarning() << "picodeTypeLength size is exceed buffer. size is " << sizeof(picodeValueLength);
            picodeValueLength = 12;
        }

        memcpy(pCmd->packetData.PICodeValue, picodeValue, picodeValueLength);
    }
    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    pCmd->frameCRC      = crc;

    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);
//    DABLogger::instance()->Log((QString("  RspGetVersion() : ReqSetServiceInfo called!! cmd size = %1 picodeValueLength =%2 returnSize =%3").arg(size).arg(picodeValueLength).arg(returnSize)));
    if(size != returnSize)
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;
}

void
DABCommManager::onReqSetAnnounceDelay(quint32 time)
{
    quint8 cmd[] = { 0xA5, 0x0D, 0x0A, 0x00,
                     0x01,       //Packet Type
                     0x22, 0x00, //Packet Command
                     0x00,       //Packet Reserved
                     0x04, 0x00, //Packet Data Length
                     0x00, 0x00, 0x00, 0x00, //Packet Data
                     0x00, 0x00, //CRC
                   };

    quint32* pTime = (quint32*)(cmd+10);
    *pTime = time;

    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;

    return;
}

void
DABCommManager::RspSetAnnounceDelay(unsigned char* buff, int size)
{
    if(size == 0)
    {
        qWarning() << " Error!. size is 0";
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

//    quint32* pTime = (quint32*)(buff);
//    qWarning() << Q_FUNC_INFO << " time = " << *pTime;
}


void
DABCommManager::onReqSetAnnounceTimeout(quint32 time)
{
    quint8 cmd[] = { 0xA5, 0x0D, 0x0A, 0x00,
                     0x01,       //Packet Type
                     0x23, 0x00, //Packet Command
                     0x00,       //Packet Reserved
                     0x04, 0x00, //Packet Data Length
                     0x00, 0x00, 0x00, 0x00, //Packet Data
                     0x00, 0x00, //CRC
                   };

    quint32* pTime = (quint32*)(cmd+10);
    *pTime = time;

//    qWarning() << __FUNCTION__ << " time = " << *pTime;

    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;

    return;
}

void
DABCommManager::RspSetAnnounceTimeout(unsigned char* buff, int size)
{
    if(size == 0)
    {
        qWarning() << " Error!. size is 0";
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

//    quint32* pTime = (quint32*)(buff);
//    qWarning() << Q_FUNC_INFO << " time = " << *pTime;
}


void
DABCommManager::onReqSetServLinkTimeout(quint32 time)
{
    quint8 cmd[] = { 0xA5, 0x0D, 0x0A, 0x00,
                     0x01,       //Packet Type
                     0x24, 0x00, //Packet Command
                     0x00,       //Packet Reserved
                     0x04, 0x00, //Packet Data Length
                     0x00, 0x00, 0x00, 0x00, //Packet Data
                     0x00, 0x00, //CRC
                   };

    quint32* pTime = (quint32*)(cmd+10);
    *pTime = time;

    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;

    return;
}

void
DABCommManager::RspSetServLinkTimeout(unsigned char* buff, int size)
{
    if(size == 0)
    {
        qWarning() << " Error!. size is 0";
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

//    quint32* pTime = (quint32*)(buff);
//    qWarning() << Q_FUNC_INFO << " time = " << *pTime;
}


void
DABCommManager::ReqSetServLinkCERValue(quint32 dab_worst, quint32 dab_bad, quint32 dab_nogood, quint32 dab_good, quint32 dabPlus_worst, quint32 dabPlus_bad, quint32 dabPlus_nogood, quint32 dabPlus_good)
{
    quint8 cmd[sizeof(stREQ_SET_SERVLINK_CER_VALUE)] = {0,};
    stREQ_SET_SERVLINK_CER_VALUE* pCmd = (stREQ_SET_SERVLINK_CER_VALUE *)cmd;

    pCmd->startSymbol                           = DAB_FRAME_START_SYMBOL;
    pCmd->continuity                            = (0x00 | DAB_CONTINUITY_ONE_N_ONLY_PACKET |  DAB_DEVICE_UART1);//0x0D;
    pCmd->frameLength                           = (1+2+1+2+sizeof(stRSP_SET_SERVLINK_CER_VALUE));
    pCmd->packetType                            = DAB_PACKET_TYPE_REQUEST;//Request:0x01, Response:0x02, Event: 0x04
    pCmd->packetCmd                             = REQ_SET_SERVLINK_CER_VALUE;
    pCmd->packetReserved                        = 0x00;
    pCmd->packetLength                          = sizeof(stRSP_SET_SERVLINK_CER_VALUE);
    pCmd->packetdata.dab_worst                  = dab_worst;
    pCmd->packetdata.dab_bad                    = dab_bad;
    pCmd->packetdata.dab_nogood                 = dab_nogood;
    pCmd->packetdata.dab_good                   = dab_good;
    pCmd->packetdata.dabPlus_worst              = dabPlus_worst;
    pCmd->packetdata.dabPlus_bad                = dabPlus_bad;
    pCmd->packetdata.dabPlus_nogood             = dabPlus_nogood;
    pCmd->packetdata.dabPlus_good               = dabPlus_good;

    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    pCmd->frameCRC      = crc;

    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);

    if(size != returnSize)
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;
}

void
DABCommManager::RspSetServLinkCERValue(unsigned char* buff, int size)
{
//    qWarning() << Q_FUNC_INFO;

    if(size == 0)
    {
        qWarning() << " Error!. size is 0";
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    stRSP_SET_SERVLINK_CER_VALUE* resp = (stRSP_SET_SERVLINK_CER_VALUE*) buff;
    qDebug() << __FUNCTION__ << " : DAB     Worst   : " << resp->dab_worst;
    qDebug() << __FUNCTION__ << " : DAB       BAD   : " << resp->dab_bad;
    qDebug() << __FUNCTION__ << " : DAB    NoGood   : " << resp->dab_nogood;
    qDebug() << __FUNCTION__ << " : DAB      Good   : " << resp->dab_good;
    qDebug() << __FUNCTION__ << " : DAB+    Worst   : " << resp->dabPlus_worst;
    qDebug() << __FUNCTION__ << " : DAB+      BAD   : " << resp->dabPlus_bad;
    qDebug() << __FUNCTION__ << " : DAB+   NoGood   : " << resp->dabPlus_nogood;
    qDebug() << __FUNCTION__ << " : DAB+     Good   : " << resp->dabPlus_good;
}

void
DABCommManager::ReqSetSigStatusCERValue(quint8 count, quint32 dab_bad, quint32 dab_nogood, quint32 dab_good, quint32 dabPlus_bad, quint32 dabPlus_nogood, quint32 dabPlus_good)
{
    quint8 cmd[sizeof(stREQ_SET_SIG_STATUS_CER_VALUE)] = {0,};
    stREQ_SET_SIG_STATUS_CER_VALUE* pCmd = (stREQ_SET_SIG_STATUS_CER_VALUE *)cmd;

    pCmd->startSymbol                           = DAB_FRAME_START_SYMBOL;
    pCmd->continuity                            = (0x00 | DAB_CONTINUITY_ONE_N_ONLY_PACKET | DAB_DEVICE_UART1);//0x0D;
    pCmd->frameLength                           = (1+2+1+2+sizeof(stRSP_SET_SIG_STATUS_CER_VALUE));
    pCmd->packetType                            = DAB_PACKET_TYPE_REQUEST;      //Request:0x01, Response:0x02, Event: 0x04
    pCmd->packetCmd                             = REQ_SET_SIGSTATUS_CER_VALUE;
    pCmd->packetReserved                        = 0x00;
    pCmd->packetLength                          = sizeof(stRSP_SET_SIG_STATUS_CER_VALUE);
    pCmd->packetdata.status_count               = count;
    pCmd->packetdata.dab_bad                    = dab_bad;
    pCmd->packetdata.dab_nogood                 = dab_nogood;
    pCmd->packetdata.dab_good                   = dab_good;
    pCmd->packetdata.dabPlus_bad                = dabPlus_bad;
    pCmd->packetdata.dabPlus_nogood             = dabPlus_nogood;
    pCmd->packetdata.dabPlus_good               = dabPlus_good;

    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    pCmd->frameCRC                              = crc;

    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);

    if(size != returnSize)
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;
}

void
DABCommManager::ReqSetPinknoiseCERValue(quint8 unmute_count, quint8 mute_count, quint32 dab_bad, quint32 dab_good, quint32 dabPlus_bad, quint32 dabPlus_good)
{
    quint8 cmd[sizeof(stREQ_PINKNOISE_CER_VALUE)] = {0,};
    stREQ_PINKNOISE_CER_VALUE* pCmd = (stREQ_PINKNOISE_CER_VALUE *)cmd;

    pCmd->startSymbol                           = DAB_FRAME_START_SYMBOL;
    pCmd->continuity                            = (0x00 | DAB_CONTINUITY_ONE_N_ONLY_PACKET | DAB_DEVICE_UART1);//0x0D;
    pCmd->frameLength                           = (1+2+1+2+sizeof(stRSP_SET_PINKNOISE_CER_VALUE));
    pCmd->packetType                            = DAB_PACKET_TYPE_REQUEST;      //Request:0x01, Response:0x02, Event: 0x04
    pCmd->packetCmd                             = REQ_SET_PINKNOISE_CER_VALUE;
    pCmd->packetReserved                        = 0x00;
    pCmd->packetLength                          = sizeof(stRSP_SET_PINKNOISE_CER_VALUE);
    pCmd->packetdata.unmute_status_count        = unmute_count;
    pCmd->packetdata.mute_status_count          = mute_count;
    pCmd->packetdata.dab_bad                    = dab_bad;
    pCmd->packetdata.dab_good                   = dab_good;
    pCmd->packetdata.dabPlus_bad                = dabPlus_bad;
    pCmd->packetdata.dabPlus_good               = dabPlus_good;

    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    pCmd->frameCRC                              = crc;

    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);
    qDebug() << Q_FUNC_INFO << " : " << unmute_count << ", " << mute_count << ", " << dab_bad << "," << dab_good << ", " << dabPlus_bad << ", " << dabPlus_good;
    if(size != returnSize)
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;
}


void
DABCommManager::RspSetPinknoiseCERValue(unsigned char* buff, int size)
{
    qDebug() << Q_FUNC_INFO;

    if(size == 0)
    {
        qDebug() << " Error!. size is 0 size = " << size ;
        return;
    }
    if(buff == NULL)
    {
        qDebug() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    stRSP_SET_PINKNOISE_CER_VALUE* resp = (stRSP_SET_PINKNOISE_CER_VALUE*) buff;
    qDebug() << __FUNCTION__ << " : Pinknoise unMute Count   : " << resp->unmute_status_count;
    qDebug() << __FUNCTION__ << " : Pinknoise Mute   Count   : " << resp->mute_status_count;
    qDebug() << __FUNCTION__ << " :       DAB       BAD      : " << resp->dab_bad;
    qDebug() << __FUNCTION__ << " :       DAB      Good      : " << resp->dab_good;
    qDebug() << __FUNCTION__ << " :       DAB+      BAD      : " << resp->dabPlus_bad;
    qDebug() << __FUNCTION__ << " :       DAB+     Good      : " << resp->dabPlus_good;
}

void
DABCommManager::RspSetSigStatusCERValue(unsigned char* buff, int size)
{
//    qWarning() << Q_FUNC_INFO;

    if(size == 0)
    {
        qWarning() << " Error!. size is 0";
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    stRSP_SET_SIG_STATUS_CER_VALUE* resp = (stRSP_SET_SIG_STATUS_CER_VALUE*) buff;
    qDebug() << __FUNCTION__ << " : Signal Status Count   : " << resp->status_count;
    qDebug() << __FUNCTION__ << " :       DAB       BAD   : " << resp->dab_bad;
    qDebug() << __FUNCTION__ << " :       DAB    NoGood   : " << resp->dab_nogood;
    qDebug() << __FUNCTION__ << " :       DAB      Good   : " << resp->dab_good;
    qDebug() << __FUNCTION__ << " :       DAB+      BAD   : " << resp->dabPlus_bad;
    qDebug() << __FUNCTION__ << " :       DAB+   NoGood   : " << resp->dabPlus_nogood;
    qDebug() << __FUNCTION__ << " :       DAB+     Good   : " << resp->dabPlus_good;
}

void
DABCommManager::ReqSetAntennaStatus(quint8 flag)
{
//    qWarning() << Q_FUNC_INFO;

    quint8 cmd[] = { 0xA5, 0x0D, 0x0A, 0x00,
                     0x01,       //Packet Type
                     0x29, 0x00, //Packet Command
                     0x00,       //Packet Reserved
                     0x01, 0x00, //Packet Data Length
                     0x00,
                     0x00, 0x00, 0x00,  //Padding
                     0x00, 0x00, //CRC
                   };

    cmd[10] = (quint8)flag;

    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2);
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;

    return;
}

void
DABCommManager::RspSetAntennaStatus(unsigned char* buff, int size)
{
//    qWarning() << Q_FUNC_INFO;

    if(size == 0)
    {
        qWarning() << " Error!. size is 0";
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    quint8 band = buff[0];

    switch(band)
    {
        case 0x00:  // Antenna Off
            qDebug() << "RspSetAntennaStatus : Antenna Off";
            break;
        case 0x01:  // Antenna On
            qDebug() << "RspSetAntennaStatus : Antenna On";
            break;
    }
}

void
DABCommManager::onReqSetAnnounceWeakTimeout(quint32 time)
{
//    qWarning() << Q_FUNC_INFO << " time = " << time;

    quint8 cmd[] = { 0xA5, 0x0D, 0x0A, 0x00,
                     0x01,       //Packet Type
                     0x28, 0x00, //Packet Command
                     0x00,       //Packet Reserved
                     0x04, 0x00, //Packet Data Length
                     0x00, 0x00, 0x00, 0x00, //Packet Data
                     0x00, 0x00, //CRC
                   };

    quint32* pTime = (quint32*)(cmd+10);
    *pTime = time;

    quint16 crc = calculateCRC16((unsigned char*)cmd, sizeof(cmd)-2); //except CRC 2 byte.
    quint16* pCrc = (quint16*)(cmd + sizeof(cmd)-2);
    *pCrc = crc;

    int size = sizeof(cmd);
    int returnSize = this->writeCmd(cmd, size);
    if(size != returnSize)
        qWarning() << "ERROR: Send size error.. Cmd Size:" << size << ", Send Size:" << returnSize;

    return;
}

void
DABCommManager::RspSetAnnouncementWeakTimeout(unsigned char* buff, int size)
{
//    qWarning() << Q_FUNC_INFO;

    if(size == 0)
    {
        qWarning() << " Error!. size is 0";
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

//    quint32* pTime = (quint32*)(buff);
//    qWarning() << Q_FUNC_INFO << " time = " << *pTime;
}

////////////////////////////////////////////////////////////////////////////////////////
//Receive Event from DABCommReceiverThread (Naming from DAB Module Specification Document)
////////////////////////////////////////////////////////////////////////////////////////

//EVT_SEND_MODULE_STATUS
void // Implementation OK >> Test OK >> Not Connect to DABController.
DABCommManager::EvtSendModuleStatus(quint8 moduleStatus, quint8 serviceStatus)
{
    //qWarning() << Q_FUNC_INFO;
    //qWarning() << " Module State: " << moduleStatus << ", Service State: " << serviceStatus;

    emit evtSendModuleStatus((eEVT_SEND_MODULE_STATUS)moduleStatus, (eEVT_SEND_SERVICE_STATUS) serviceStatus);
    return;
}

void
DABCommManager::EvtSendChangeService(unsigned char* buff, int buffLen)
{
    //qWarning() << Q_FUNC_INFO <<"Buff:"<<buff<< "Size:" <<buffLen;
    if(buffLen <= 0)
    {
        qWarning() << "Error!. buffLen is " << buffLen;
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    stEVT_SEND_CHANGE_SERVICE* resp = (stEVT_SEND_CHANGE_SERVICE*)buff;

    qWarning(" Frequency    : 0x%08X (%d)", resp->Frequency, resp->Frequency);
    qWarning(" Service Type : 0x%02X (%d)", resp->ServiceType, resp->ServiceType);
    qWarning(" ServiceID    : 0x%08X (%d)", resp->ServiceID, resp->ServiceID);
    qWarning(" SubChId      : 0x%02X (%d)", resp->SubChId, resp->SubChId);
    qWarning(" Bitrate      : 0x%04X (%d)", resp->Bitrate, resp->Bitrate);
    qWarning(" Pty          : 0x%02X (%d)", resp->Pty, resp->Pty);

    char labelBuffer[17] = {0,};
    char ensembleLabelBuffer[17] = {0,};

    copyLabelBufferAndRemoveSpace(labelBuffer, sizeof(labelBuffer), (char*)resp->Label, sizeof(resp->Label));

    QString label;
    QTextCodec *codec = NULL;
    QByteArray tempArray;
    switch(resp->LabelCharset)
    {
        case 0x88:
            tempArray.append((const char*)labelBuffer, sizeof(labelBuffer));
            codec = QTextCodec::codecForName("EUC-KR");
            label = codec->toUnicode(tempArray.data());
            break;

        case 0x60:
        case 0x15:
            tempArray.append((const char*)labelBuffer, sizeof(labelBuffer));
            codec = QTextCodec::codecForName("UTF-16"); //for USC2
            label = codec->toUnicode(tempArray.data());
            break;

        case 0x00: //EBU Latin based Character
            label = ebuLatinToUnicode((unsigned char*)labelBuffer, sizeof(labelBuffer));
            break;

        case 0x06:
        default:
            tempArray.append((const char*)labelBuffer, sizeof(labelBuffer));
            codec = QTextCodec::codecForName("UTF-8");
            label = codec->toUnicode(tempArray.data());
            break;
    }

    copyLabelBufferAndRemoveSpace(ensembleLabelBuffer, sizeof(ensembleLabelBuffer), (char*)resp->EnsembleLabel, sizeof(resp->EnsembleLabel));
    QTextCodec *codec2 = NULL;
    QByteArray byteArray;
    QString ensembleLabel;
    switch(resp->EnsembleCharset)
    {
        case 0x88:
            byteArray.append((const char*)ensembleLabelBuffer, sizeof(ensembleLabelBuffer));
            codec2 = QTextCodec::codecForName("EUC-KR");
            ensembleLabel = codec2->toUnicode(byteArray.data());
            break;

        case 0x60:
        case 0x15:
            byteArray.append((const char*)ensembleLabelBuffer, sizeof(ensembleLabelBuffer));
            codec2 = QTextCodec::codecForName("UTF-16"); //for USC2
            ensembleLabel = codec2->toUnicode(byteArray.data());
            break;

        case 0x00: //EBU Latin based Character
            ensembleLabel = ebuLatinToUnicode((unsigned char*)ensembleLabelBuffer, sizeof(ensembleLabelBuffer));
            break;

        case 0x06:
        default:
            byteArray.append((const char*)ensembleLabelBuffer, sizeof(ensembleLabelBuffer));
            codec2 = QTextCodec::codecForName("UTF-8");
            ensembleLabel = codec2->toUnicode(byteArray.data());
            break;
    }

    emit evtSendChangeService(resp->Frequency,
                              resp->ServiceType,
                              resp->ServiceID,
                              resp->SubChId,
                              resp->Bitrate,
                              resp->Pty,
                              label,
                              ensembleLabel,
                              (eEVT_SEND_CHANGE_SERVICE_EVENT)resp->Event); //eEVT_SEND_CHANGE_SERVICE_EVENT

    return;
}

void
DABCommManager::EvtSendAnnouncement(unsigned char* buff, int buffLen)
{
    if(buffLen <= 0)
    {
        qWarning() << "Error!. buffLen is " << buffLen;
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    stEVT_SEND_ANNOUNCEMENT* resp = (stEVT_SEND_ANNOUNCEMENT*)buff;

//    qWarning() << "Flag     : " << bin << resp->Flag;
//    qWarning() << " Alam    : " << ((resp->Flag&0x0001)?"On":"Off");
//    qWarning() << " Traffic : " << ((resp->Flag&0x0002)?"On":"Off");
//    qWarning() << " Travel  : " << ((resp->Flag&0x0004)?"On":"Off");
//    qWarning() << " Warning : " << ((resp->Flag&0x0008)?"On":"Off");
//    qWarning() << " News    : " << ((resp->Flag&0x0010)?"On":"Off");
//    qWarning() << " Weather : " << ((resp->Flag&0x0020)?"On":"Off");
//    qWarning() << " Event   : " << ((resp->Flag&0x0040)?"On":"Off");
//    qWarning() << " Special : " << ((resp->Flag&0x0080)?"On":"Off");
//    qWarning() << " Rad_info: " << ((resp->Flag&0x0100)?"On":"Off");
//    qWarning() << " Sports  : " << ((resp->Flag&0x0200)?"On":"Off");
//    qWarning() << " Finance : " << ((resp->Flag&0x0400)?"On":"Off");

//    switch(resp->Status)
//    {
//        case 0x00: // Start
//            qWarning() << " Announcement Finished...";
//            break;
//        case 0x01: // Finish
//            qWarning() << " Announcement Start.";
//            break;
//        default:
//            qWarning(" ERROR Unknown Announcement Status: 0x%02X", resp->Status);
//            break;
//    }

    char labelBuffer[17] = {0,};
    copyLabelBufferAndRemoveSpace(labelBuffer, sizeof(labelBuffer), (char*)resp->taService.Label, sizeof(resp->taService.Label));
    QString label = ebuLatinToUnicode((unsigned char*)labelBuffer, sizeof(labelBuffer));
    emit evtSendAnnouncement(label, resp->Flag, (eEVT_SEND_ANNOUNCEMENT_STATUS)resp->Status, resp->taService.SubChId);
    return;
}

void
DABCommManager::EvtSendTime(unsigned char* buff, int buffLen)
{
    if(buffLen <= 0)
    {
        qWarning() << "Error!. buffLen is " << buffLen;
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    stEVT_SEND_TIME* resp = (stEVT_SEND_TIME*)buff;
//    qWarning() << " Year  :"            << resp->Year;
//    qWarning() << " Month :"            << resp->Month;
//    qWarning() << " Day   :"            << resp->Day;
//    qWarning() << " Hour  :"            << resp->Hour;
//    qWarning() << " Minute:"            << resp->Minute;
//    qWarning() << " Second:"            << resp->Second;
//    qWarning() << " MJD   :"            << resp->MJD;
//    qWarning() << " UTC_Flag :"         << resp->UTC_Flag;
//    qWarning() << " UTC_Hour :"         << resp->UTC_Hour;
//    qWarning() << " UTC_Minute :"       << resp->UTC_Minute;
//    qWarning() << " UTC_Second :"       << resp->UTC_Second;
//    qWarning() << " UTC_Milisecond :"   << resp->UTC_Milisecond;

//    qWarning() << "If received Time value, Stop SEND TIME EVENT !!";
#if 0  // requested from LG MJY
    this->ReqSetEvtSendTime(EVT_SEND_TIME_STOP);
#endif
    emit evtSendTime(resp->Year, resp->Month, resp->Day, resp->Hour, resp->Minute, resp->Second);
    return;
}

void
DABCommManager::EvtSendQOS(unsigned char* buff, int buffLen)
{
    if(buffLen <= 0)
    {
        qWarning() << "Error!. buffLen is " << buffLen;
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    stEVT_SEND_QOS* resp = (stEVT_SEND_QOS*)buff;
    //qWarning(" CER(Channel Decoder Error) : 0x%04X, (%d)", resp->CER, resp->CER);
    //qWarning(" SNR(Signal to noise rate)  : 0x%02X, (%d)", resp->SNR, resp->SNR);
    //qWarning(" RSSI(Received SIgnal Strength Indicate: 0x%08X, (%d)", resp->RSSI, resp->RSSI);

//    emit evtSendQOS(resp->CER, resp->SNR, resp->RSSI);
    emit evtSendQOS(resp->CER, resp->SNR, resp->RSSI, resp->CER_sub, resp->SNR_sub, resp->RSSI_sub);
    return;
}

void
DABCommManager::EvtUpdateDLS(unsigned char* buff, int buffLen)
{
    Q_UNUSED(buffLen);
    quint8 update = buff[0];

    switch(update)
    {
        case 0x00:
            break;
        case 0x01:
            break;
    }

    emit evtUpdateDLS(update);
    return;
}

void
DABCommManager::EvtUpdateSLS(unsigned char* buff, int buffLen)
{
    Q_UNUSED(buff);
    Q_UNUSED(buffLen);
    emit evtUpdateSLS();
    return;
}

void
DABCommManager::EvtUpdateEPG(unsigned char* buff, int buffLen)
{
    Q_UNUSED(buff);
    Q_UNUSED(buffLen);
    emit evtUpdateEPG();
    return;
}

void
DABCommManager::EvtDataInfo(unsigned char* buff, int buffLen)
{
    if(buffLen <= 0)
    {
        qWarning() << "Error!. buffLen is " << buffLen;
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    stEVT_DATA_INFO* resp = (stEVT_DATA_INFO*)buff;

//    qWarning(" User Application Type    : 0x%04X (%d)", resp->userApplicationType, resp->userApplicationType);
//    qWarning(" Sub Channel ID           : 0x%02X (%d)", resp->subChannelID, resp->subChannelID);
//    qWarning(" Frequency                : 0x%08X (%d)", resp->frequency, resp->frequency);
//    qWarning(" Bitrate                  : 0x%04X (%d)", resp->bitrate, resp->bitrate);

    char labelBuffer[17] = {0,};

    copyLabelBufferAndRemoveSpace(labelBuffer, sizeof(labelBuffer), (char*)resp->label, sizeof(resp->label));
    QString label(labelBuffer);

//    qWarning() << " Label        :" << label;

    switch(resp->userApplicationType)
    {
    case DAB_APPLICATION_BWS:
//        qWarning() << "User Application Type : BWS";
        break;
    case DAB_APPLICATION_EPG:
//        qWarning() << "User Application Type : EPG";
        break;
    default:
        break;
    }

}

void
DABCommManager::EvtSendAutoScanInfo(unsigned char* buff, int buffLen)
{
    if(buffLen <= 0)
    {
        qWarning() << "Error!. buffLen is " << buffLen;
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    stEVT_SEND_AUTO_SCAN* resp = (stEVT_SEND_AUTO_SCAN*)buff;

    emit evtAutoScanInfo(resp->status, resp->frequency, resp->lock);
}

void
DABCommManager::EvtSendSignalInfo(unsigned char* buff, int buffLen)
{
    if(buffLen <= 0)
    {
        qWarning() << "Error!. buffLen is " << buffLen;
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    quint8 status = buff[0];
    emit evtSendSignalInfo(status);
}

void
DABCommManager::EvtServiceFollowingState(unsigned char* buff, int buffLen)
{
    if(buffLen <= 0)
    {
        qWarning() << "Error!. buffLen is " << buffLen;
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }
    quint8 state = buff[0];
    emit evtServiceFollowingState(state);
}

void
DABCommManager::EvtSendCountryInfo(unsigned char* buff, int buffLen)
{
    if(buffLen <= 0)
    {
        qWarning() << "Error!. buffLen is " << buffLen;
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

//    stEVT_SEND_COUNTRY_INFO* resp = (stEVT_SEND_COUNTRY_INFO*)buff;

//    qWarning(" ECC                  : 0x%02X", resp->ecc);
//    qWarning(" COUNTRY ID           : 0x%02X", resp->countryID);
//    qWarning(" LTO Sense            : 0x%02X", resp->ltoSense);
//    qWarning(" LTO                  : 0x%02X", resp->lto);
//    qWarning(" Inter Table ID       : 0x%02X", resp->interTableID);
}

void
DABCommManager::EvtSendPinkStatus(unsigned char* buff, int buffLen)
{
    if(buffLen <= 0)
    {
        qWarning() << "Error!. buffLen is " << buffLen;
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }
    quint8 status = buff[0];
    emit evtSendPinkStatus(status);
}

void
DABCommManager::EvtSendUserAppType(unsigned char* buff, int buffLen)
{
//    qWarning() << Q_FUNC_INFO <<"Buff:"<<buff<< "Size:" <<buffLen;

    if(buffLen <= 0)
    {
        qWarning() << "Error!. buffLen is " << buffLen;
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

#if 0
    int index = 0;
    quint16* pCount = (quint16*)(buff+1);
    index = index + 3;
    quint16 count = 0;
    count = *pCount;

    quint16 appType[count];
    quint16* pType = 0x00;
    qDebug() << "=================== User App Type :: count = " << count;

    for(int i=0; i<count; i++)
    {
        appType[i] = 0x00;
        pType = (quint16*)(buff+index);
        appType[i] = *pType;
        index = index + 2;
        qDebug() << "=================== User App Type :: Application Type[ " << i << "] = " << appType[i];
    }
#endif	
}

void
DABCommManager::EvtSendAnnouncementTAInvalid(unsigned char *buff, int buffLen)
{
    qDebug() << Q_FUNC_INFO <<"Buff:"<<buff<< "Size:" <<buffLen;

    if(buffLen <= 0)
    {
        qDebug() << "Error!. buffLen is " << buffLen;
        return;
    }

    if(buff == NULL)
    {
        qDebug() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    stEVT_SEND_TA_INVALID_INFO* resp = (stEVT_SEND_TA_INVALID_INFO*)buff;

    qDebug() << Q_FUNC_INFO << " ::     Pre Flags = " << resp->preFlag;
    qDebug() << Q_FUNC_INFO << " :: Currnet Flags = " << resp->currentFlag;

    emit evtSendAnnouncementTAInvalid(resp->currentFlag);
}

void
DABCommManager::EvtSPIUpdateSLS(unsigned char* buff, int buffLen, int value, unsigned char framepostion, int totalLen)
{
    qWarning() << Q_FUNC_INFO <<" ========================= Buff : " << buff << "Size:" << buffLen << " value : " << value << " framepostion = " << framepostion << ", totalLen = " << totalLen;

    QString slsFileName;

    if(buffLen <= 0)
    {
        qWarning() << __FUNCTION__ << "Error!. buffLen is " << buffLen;
        pSPIPacketInfo->setSLSFileName("");
        return;
    }
    if(buff == NULL)
    {
        qWarning() << __FUNCTION__ << "Error!. Buff is NULL: " << buff ;
         pSPIPacketInfo->setSLSFileName("");
        return;
    }

    if(framepostion == SPI_INTERMEDIATE_PACKET || framepostion == SPI_LAST_PACKET)
    {
        QString filename = pSPIPacketInfo->getSLSFileName();
        if(filename == NULL)
        {
            qWarning("filename is NULL. Stop!!!");
            pSPIPacketInfo->setSLSFileName("");
            return;
        }

        if(QFile::exists(filename))
        {
            QFile SLSFile(pSPIPacketInfo->getSLSFileName(), this);
            if (!SLSFile.open(QIODevice::WriteOnly | QIODevice::Append))
            {
                qWarning() << __FUNCTION__ << " Save ERROR : sls File Not found:" << pSPIPacketInfo->getSLSFileName();
                pSPIPacketInfo->setSLSFileName("");
                SLSFile.close();
                return;
            }
            QDataStream out(&SLSFile);
            out.writeRawData((const char*)buff, buffLen);
            SLSFile.close();
        }
        else
        {
            qWarning("File is not exist. STOP !!");
            pSPIPacketInfo->setSLSFileName("");
            return;
        }
    }
    else
    {
        qWarning() << " =======================================RSP SLS =====START============================";
        QDate date = QDate::currentDate();
        QTime time = QTime::currentTime();

        slsFileName = DAB_DATA_ROOT + date.toString("yyyy_MM_dd")+"_"+time.toString("HH_mm_ss")+"_";
        switch(value)
        {
            case 0x00: //JPEG
                slsFileName += "DAB_SLS.JPEG";
                break;
            case 0x01: //PNG
                slsFileName += "DAB_SLS.png";
                break;
            default:
                slsFileName += "DAB_SLS.png";
                break;
        }

        pSPIPacketInfo->setSLSFileName(slsFileName);

        QFile SLSFile(slsFileName, this);
        if (!SLSFile.open(QIODevice::WriteOnly))
        {
            qWarning() << " Save ERROR : sls File Not found:" << slsFileName;
            pSPIPacketInfo->setSLSFileName("");
            SLSFile.close();
            return;
        }
        QDataStream out(&SLSFile);
        out.writeRawData((const char*)buff, buffLen);
        SLSFile.close();
    }

    if(framepostion == SPI_LAST_PACKET || framepostion == SPI_ONE_AND_ONLY_PACKET)
    {        
        QFile slsFile(pSPIPacketInfo->getSLSFileName());
        int size = slsFile.size();
        qWarning() << "Data Packet Parsing Success !! File Name : " << pSPIPacketInfo->getSLSFileName() << ", total len = " << totalLen << ", sls size = " << size;
        if(size == totalLen)
        {
            emit rspGetSLS(pSPIPacketInfo->getSLSFileName());
        }
        else
        {
            qWarning() << " =======================================SLS FILE REMOVE============================";
            slsFile.remove();
        }
        pSPIPacketInfo->setSLSFileName("");
    }
    else
    {
        qWarning() << "Data Packet Parsing Continue";
    }

    return;
}

void
DABCommManager::EvtSPIUpdateStationLogo(unsigned char* buff, int buffLen, unsigned char framepostion)
{
//    qWarning() << Q_FUNC_INFO <<"Buff:"<<buff<< "Size:" <<buffLen;

    if(buffLen <= 0)
    {
        qWarning() << "Error!. buffLen is " << buffLen;
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    quint32* pServiceID = NULL;
    quint32 serviceID = 0;
    quint8 mimeType = 0;
    int index = 0;

    if(framepostion == SPI_INTERMEDIATE_PACKET || framepostion == SPI_LAST_PACKET)
    {
        QString filename;
        if(pSPIPacketInfo->getExistLogo())
        {
            filename = pSPIPacketInfo->getTempLogoFilename();
        }
        else
        {
            filename = pSPIPacketInfo->getLogoFilename();
        }

        if(filename == NULL)
        {
            qWarning("EPG LOGO File Name is NULL. Stop!!!");
            return;
        }
        else
        {
            qWarning() << "Current EPG LOGO File Name is " << filename;
        }

        if(QFile::exists(filename))
        {
            QFile EPGLogoFile(filename, this);
            if (!EPGLogoFile.open(QIODevice::WriteOnly | QIODevice::Append))
            {
                qWarning() << " Save ERROR : EPG LOGO File Not found:" << filename;
                EPGLogoFile.close();
                return;
            }
            QDataStream out(&EPGLogoFile);
            out.writeRawData((const char*)buff, buffLen);
            EPGLogoFile.close();
        }
        else
        {
            qWarning("EPG LOGO File is not exist. STOP !!");
            return;
        }
    }
    else
    {
        pServiceID = (quint32*)(buff+index);
        serviceID = *pServiceID;
        index += sizeof(quint32);

        mimeType = buff[index++];

        QDir dir(DAB_STATION_LOGO_ROOT);
        if(!dir.exists())
        {
            if(!dir.mkdir(DAB_STATION_LOGO_ROOT))
            {
                qWarning() << "make directory Fail !!";
                return;
            }
        }

        QString strServiceID = QString::number(serviceID,16);
        QString sEPGLogoFileName = DAB_STATION_LOGO_ROOT + strServiceID;
        switch(mimeType)
        {
            case 0x00: //GIF
                sEPGLogoFileName += ".gif";
                break;
            case 0x01: //JPG
                sEPGLogoFileName += ".jpg";
                break;
            case 0x02: //BMP
                sEPGLogoFileName += ".bmp";
                break;
            case 0x03: //PNG
                sEPGLogoFileName += ".png";
                break;
            default:
                sEPGLogoFileName += ".png";
                break;
        }

        if(QFile::exists(sEPGLogoFileName))
        {
            sEPGLogoFileName.insert(0, "Temp");
            pSPIPacketInfo->setTempLogoFilename(sEPGLogoFileName);
            pSPIPacketInfo->setExistLogo(true);
        }
        else
        {
            pSPIPacketInfo->setLogoFilename(sEPGLogoFileName);
        }

        QFile file(sEPGLogoFileName, this);
        if (!file.open(QIODevice::WriteOnly))
        {
            qWarning() << " Save ERROR : EPG LOGO File Not found:" << sEPGLogoFileName;
            file.close();
            return;
        }

        QDataStream out(&file);
        out.writeRawData((const char*)buff+index, buffLen-5);
        file.close();
    }

    if(framepostion == SPI_LAST_PACKET || framepostion == SPI_ONE_AND_ONLY_PACKET)
    {
        if(pSPIPacketInfo->getExistLogo())
        {
            QString newFilename = pSPIPacketInfo->getTempLogoFilename();
            QString oldFilename = pSPIPacketInfo->getLogoFilename();
            QFile newFile(newFilename);
            QFile oldFile(oldFilename);

            if(newFile.size() == oldFile.size())
            {
                if(!newFile.remove())
                {
                    qWarning() << __FUNCTION__ << " File remove fail!! filename = " << newFilename;
                }
            }
            else
            {
                if(oldFile.remove())
                {
                    qWarning() << __FUNCTION__ << " File remove success!!";
                    QString temp = newFilename;
                    newFilename.remove(0, 4);
                    QFile::rename(temp, newFilename);
                }
                else
                {
                    qWarning() << __FUNCTION__ << " File remove fail!!";
                }
            }
            newFile.close();
            oldFile.close();
        }
        else
        {
            emit cmdAddStationLogo(pSPIPacketInfo->getLogoFilename());
        }

        pSPIPacketInfo->setLogoFilename("");
        pSPIPacketInfo->setTempLogoFilename("");
        pSPIPacketInfo->setExistLogo(false);
    }
    else
    {
        qWarning() << "Data Packet Parsing Continue";
    }
    return;
}

void
DABCommManager::EvtSPIUpdateTPEG(unsigned char* buff, int buffLen)
{
//    qWarning() << Q_FUNC_INFO << "buff:" << buff  << "Size:" << buffLen;

    if(buffLen <= 0)
    {
        qWarning() << "Error!. Size is " << buffLen;
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    emit cmdRspTPEGData(QByteArray((const char*)buff, buffLen));

#if 0 // for debugging
    QString tpegFileName;

    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();

    tpegFileName = DAB_DATA_ROOT + time.toString("HH_mm_ss_zzz")+"_";
    tpegFileName += "tpeg.bin";

    pSPIPacketInfo->setFileName(tpegFileName);

    QFile tpegFile(tpegFileName, this);
    if (!tpegFile.open(QIODevice::WriteOnly))
    {
        qWarning() << " Save ERROR : TPEG File Not found:" << tpegFileName;
        return;
    }
    QDataStream out(&tpegFile);
    out.writeRawData((const char*)buff, buffLen);
    tpegFile.close();
    pSPIPacketInfo->washOffData();
#endif
}

void
DABCommManager::EvtSPIUpdateEPG(unsigned char* buff, int buffLen)
{
    qWarning() << Q_FUNC_INFO << "buff:" << buff  << "Size:" << buffLen;

    if(buffLen <= 0)
    {
        qWarning() << "Error!. Size is " << buffLen;
        return;
    }
    if(buff == NULL)
    {
        qWarning() << "Error!. Buff is NULL: " << buff ;
        return;
    }

    int index = 0;
    quint32 serviceID = 0;
    quint32* pServiceID = 0;
    quint32 mjd = 0;
    quint32* pMjd = 0;
    int programmeCount = 0;
    quint8 Hour = 0;
    quint8 Minute = 0;
    quint8 Second = 0;
    quint16 Duration = 0;
    quint16* pDuration = 0;
    quint16* length;
    quint16 ProgramLabelLength = 0;
    QString ProgramLabel;
    quint8 DescriptionLength = 0;
    QString Description;

    QByteArray byteArray;
    QTextCodec *pCodec = NULL;

    pServiceID = (quint32*)(buff+index);
    serviceID = *pServiceID;
    index += sizeof(quint32);
    qWarning() << "Service ID : " << serviceID;

    pMjd = (quint32*)(buff+index);
    mjd = *pMjd;
    index += sizeof(quint32);

    programmeCount = buff[index++];

    QDate currentDate = QDate::currentDate();
    QDate addDate = currentDate.addDays(6);
    QDate epgDate = QDate::fromJulianDay(mjd+2400001);

    qWarning() << __FUNCTION__ << " : Current Date : " << currentDate.toString() << ", EPG Date : " << epgDate.toString();

    if(epgDate < currentDate)
    {
        qDebug() << __FUNCTION__ << " : Received EPG date is less than current date, ignored!! " << ". current date = " << currentDate.toString();
        return;
    }

    if(epgDate > addDate)
    {
        qDebug() << __FUNCTION__ << " : Received EPG date is big than 7 days including currnet date, ignored!! " << addDate.toString();
        return;
    }

    if(programmeCount > 0 )
    {
        emit removeDuplicatEPGData(serviceID, mjd);
    }

    for(int i = 0; i < programmeCount; i++)
    {
        Hour = buff[index++];
        Minute = buff[index++];
        Second = buff[index++];

        pDuration = (quint16*)(buff+index);
        Duration = *pDuration;
        index += sizeof(quint16);

        length = (quint16*)(buff + index++);
        index++;        
        ProgramLabelLength = *length;

        pCodec = QTextCodec::codecForName("UTF-8");
        byteArray.clear();
        byteArray.append((const char*)(buff+index), ProgramLabelLength);
        ProgramLabel = pCodec->toUnicode(byteArray.data());
        ProgramLabel = ProgramLabel.simplified();

        index += ProgramLabelLength;
        length = (quint16*)(buff + index++);
        index++;
        DescriptionLength = *length;

        pCodec = QTextCodec::codecForName("UTF-8");
        byteArray.clear();
        byteArray.append((const char*)(buff+index), DescriptionLength);
        Description = pCodec->toUnicode(byteArray.data());

        index += DescriptionLength;

//        qDebug("Time = %d : %d : %d", Hour, Minute, Second);
//        qDebug() << "Label : " << ProgramLabel;
//        qDebug() << "Description : " << Description;
//        qDebug() << Hour <<":"<<Minute<<":"<<Second<<"(" <<(Duration/60)<<" minutes)";
//        qDebug() << "       ("<<ProgramLabelLength<<") " << ProgramLabel;
//        qDebug() << "       ("<<DescriptionLength<<") " << Description;
#if 1
        if(epgDate == currentDate || epgDate > currentDate)
        {
            emit cmdAddEPGItem(mjd, serviceID, Hour, Minute, Second, Duration, ProgramLabel, Description);
        }
#else

        QTime cTime = QTime::currentTime();
        QTime rTime(Hour, Minute, Second);
        QTime eTime = rTime.addSecs(Duration);

        if(epgDate == currentDate)
        {
            if(eTime >= cTime || rTime >= cTime)
                emit cmdAddEPGItem(mjd, serviceID, Hour, Minute, Second, Duration, ProgramLabel, Description);
        }
        else if(epgDate > currentDate)
        {
            emit cmdAddEPGItem(mjd, serviceID, Hour, Minute, Second, Duration, ProgramLabel, Description);
        }
#endif
    }
    emit rspGetEPG(mjd, serviceID);
    return;
}

int
DABCommManager::writeCmd(quint8* cmdBuf, int cmdLen)
{
    int writeSize = 0;
    writeSize = m_ComUart.Write(cmdBuf, cmdLen);
    return writeSize;

}

void
DABCommManager::copyLabelBufferAndRemoveSpace(char* pDest, int destSize, const char* pSrc, int srcSize)
{
    pDest[destSize-1] = 0;
    memcpy(pDest, pSrc, srcSize);
    for(int i = srcSize-1; i >=0; i--)
    {
        if(pDest[i] == ' ')
            pDest[i] = 0;
        else
            break;
    }
    return;
}

DABCommSPIPacketInfo*
DABCommManager::getSPIPacketInfo(void)
{
    if(this->pSPIPacketInfo == NULL)
        qWarning() << Q_FUNC_INFO << " DABCommSPIPacketInfo is NULL.";

    return this->pSPIPacketInfo;
}

QString
DABCommManager::ebuLatinToUnicode(unsigned char* latinChar, int strLength)
{
    bool isFind = false;
    QString unicodeBuffer;
    QChar data[strLength+1];    //# 20130327 prevent

    memset(data, 0x00, strLength+1);    //# 20130327 prevent

//    qWarning() << "Total String Length : "<< strLength;
//    qWarning() << "EBU Latin Encodding START !!";

    for(int i=0; i<strLength; i++)
    {
        if(latinChar[i] < 0x80)
        {
//            qWarning("Normal Character[%d] !! 0x%2X", i, latinChar[i]);
            data[i] = latinChar[i];
        }
        else
        {
//            qWarning("EBU Latin Character[%d] !! 0x%2X", i, latinChar[i]);
            isFind = false;
            for(int j=0; j<g_EBULatinBasedNum; j++)
            {
//                if(latinChar[i] == 0xBC)
//                    qWarning("EBU Latin Character!! [%d] ascii = 0x%2X, unicode = 0x%4X", j, latinChar[i], g_EBULatinBased[j].m_ascll);

                if(latinChar[i] == g_EBULatinBased[j].m_ascll)
                {
                    data[i] = g_EBULatinBased[j].m_unicode;
                    isFind = true;
//                    qWarning("EBU Latin Character Find !! ascii = 0x%2X, unicode = 0x%4X", latinChar[i], g_EBULatinBased[j].m_unicode);
                    break;
                }
            }

            if(!isFind)
            {
//                qWarning("EBU Latin Character not Find !!  Caaracter is 0x%2X", data[i]);
                data[i] = latinChar[i];
            }
        }
    }

    //    qWarning() << "EBU Latin Encodding End !!";
    //    qWarning("data[strLength] = '\0';",strLength);
    data[strLength+1] = '\0';   //# 20130327 prevent
    //    qWarning() <<"unicodeBuffer = QString(data)";
    unicodeBuffer = QString(data);

    return unicodeBuffer;
}

void
DABCommManager::sendBackupCommandFromRebooting(void)
{
    DABLogger::instance()->Log((QString("DABCommManager::sendBackupCommandFromRebooting() : Command = %1").arg(m_xDataBackup.cmd)));
    DABLogger::instance()->Log((QString("DABCommManager::sendBackupCommandFromRebooting() : size    = %1").arg(m_xDataBackup.size)));

    if(m_xDataBackup.cmd == 0x00)
        return;

    int returnSize = this->writeCmd(m_xDataBackup.data, m_xDataBackup.size);
    if(m_xDataBackup.size != returnSize)
        DABLogger::instance()->Log((QString("DABCommManager::sendBackupCommandFromRebooting() : ERROR: Send size error.. Cmd Size = %1, Send Size =%2").arg(m_xDataBackup.size).arg(returnSize)));
}
