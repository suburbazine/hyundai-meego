/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini / Venkat                    *
 * DATE       :  03 Dec 2011                                      *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                   This is the class which will fetch the       *
 *                   Artwork from iPod                            *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */

#include <DHAVN_IPodController_Def.h>
#include <DHAVN_IPodController_artworkFetchTaskHandler.h>
#include <QDir>
#include <DHAVN_IPodController_ack.h>
#include <DHAVN_IPodController_getArtworkTimesUID.h>
#include <DHAVN_IPodController_retArtworkTimesUID.h>
#include <DHAVN_IPodController_getArtworkDataUID.h>
#include <DHAVN_IPodController_retArtworkDataUID.h>

#include "DHAVN_EventLoop.h"
#include "DHAVN_EventLoopTimerExitCode.h"
//using namespace ExtendedLingo;

CArtworkFetchTaskHandler::CArtworkFetchTaskHandler(QObject *parent,
                                                   iPodControllerPrivateData* privateData,
                                                   CIAPPacketWriter* iapWriter,
                                                   CIAPPacketReader* iapReader) : CTaskHandler(parent)
{
    m_previous_packet_index = -1;
    m_nRunningLoopFromSignal = 0;
    m_ReadyToDeleteFromSignal = true;

    switch(privateData->m_currentPortNum)
    {
        case eIPOD_PORT_1:
            LOG_INIT_V2(giPodControllerLogName_1);
            break;
        case eIPOD_PORT_2:
            LOG_INIT_V2(giPodControllerLogName_2);
            break;
        default:
            LOG_INIT_V2(giPodControllerLogName);
    }
    //LOG_INIT_V2(gControllerLogName);

    //LOG_TRACE << "CArtworkFetchTaskHandler::CArtworkFetchTaskHandler() IN" << LOG_ENDL;
    setObjectName(gArtworkFetchTaskHandler);

    /* Variable initialization*/
    //m_TransID = 0;
    m_artWidth              = 0;
    m_artHeight             = 0;
    m_artFormat             = 0;
    m_artIndex              = -1;
    m_rowSize               = 0;
    m_totalImageSize        = 0;
    m_nBestFormatID         = -1;
    m_artWorkTotalCount     = -1;

    m_artData.clear();

    m_retTrackArtworkTimes  = NULL;
    m_retArtworkTimesUID    = NULL;
    m_retArtworkFormats     = NULL;
    m_isDisconnected        = false;
    QDir dir;
    dir.mkpath("/app/data/AppMedia/tmp/iPodData/");
    m_iPodPrivateData = privateData;

    m_iAPWriter = iapWriter;
    m_iAPReader = iapReader;

    m_ReadyToDelete         = true;
    m_pLoop                 = NULL;
    m_pWaitTimer            = NULL;
    m_isRunningLoop         = false;

    connect( m_iAPReader,  SIGNAL(packetAvailable(int, int, QByteArray)),       this,         SLOT(packetAvailable(int,int,QByteArray)));
    connect( m_iAPReader,  SIGNAL(largePacketAvailable(int, int, QByteArray)),  this,         SLOT(packetAvailable(int,int,QByteArray)));
    connect( this,         SIGNAL(sendiAPPacket(QByteArray)) ,                  m_iAPWriter,  SLOT(sendiAPPacket(QByteArray)));

    //LOG_TRACE << "CArtworkFetchTaskHandler::CArtworkFetchTaskHandler() OUT" << LOG_ENDL;
}

CArtworkFetchTaskHandler::~CArtworkFetchTaskHandler()
{
    //LOG_TRACE << "CArtworkFetchTaskHandler::~CArtworkFetchTaskHandler() IN" << LOG_ENDL;
    if(m_pLoop)    // added by sungha.choi 2013.04.12
    {
        delete m_pLoop;
        m_pLoop = NULL;
    }

    if(m_pWaitTimer)
    {
        delete m_pWaitTimer;
        m_pWaitTimer = NULL;
    }

    m_TriedArtworkIndexList.clear();  // added by sungha.choi 2013.04.02

    m_nBestFormatID = -1;

    if(m_retArtworkFormats)
    {
        delete m_retArtworkFormats;
        m_retArtworkFormats = NULL;
    }

    if(m_retTrackArtworkTimes)
    {
        delete m_retTrackArtworkTimes;
        m_retTrackArtworkTimes = NULL;
    }

    if(m_retArtworkTimesUID)
    {
        delete m_retArtworkTimesUID;
        m_retArtworkTimesUID = NULL;
    }

    //LOG_TRACE << "CArtworkFetchTaskHandler::~CArtworkFetchTaskHandler() OUT" << LOG_ENDL;
}


void CArtworkFetchTaskHandler::stopCurrentTask()
{
    m_isDisconnected = true;
    //emit mainLoopQuit();    // added by sungha.choi 2013.04.02

    if(isWaitingForResponse() == true)
    {
        m_ReadyToDelete = false;
        m_pLoop->quit();
    }

    if(isWaitingForResponseFromSignal() > 0)  // added by sungha.choi 2013.04.26
    {
        m_ReadyToDeleteFromSignal = false;
        //emit getDBTrackInfoReturned();  // If signals will be added, emit the signal here.
        emit mainLoopQuit(ExitCode_AllExit);  // If signals will be added, emit the signal here.
    }

}

void CArtworkFetchTaskHandler::stopTaskHandler()
{
    if( m_isDisconnected == false )
    {
        disconnect(m_iAPReader,  SIGNAL(packetAvailable(int, int, QByteArray)),   this,        SLOT(packetAvailable(int,int,QByteArray)));
        disconnect(this,         SIGNAL(sendiAPPacket(QByteArray)),               m_iAPWriter, SLOT(sendiAPPacket(QByteArray)));
    }

    m_isDisconnected = true;
}

bool CArtworkFetchTaskHandler::isReadyToDelete()  // added by sungha.choi 2013.04.12
{
    return (m_ReadyToDelete && m_ReadyToDeleteFromSignal);
}


bool CArtworkFetchTaskHandler::isWaitingForResponse()    // added by sungha.choi 2013.04.12
{
    return m_isRunningLoop;
}

int CArtworkFetchTaskHandler::isWaitingForResponseFromSignal()    // added by sungha.choi 2013.04.26
{
    return m_nRunningLoopFromSignal;
}

int CArtworkFetchTaskHandler::waitForResponceFromExitCode(int timeOut, int nExitCode)
{
    ////LOG_TRACE << "CIPodDBFetchTaskHandler::waitForResponceFromExitCode() IN" << LOG_ENDL;
    int ret = 0;

    if(m_isDisconnected)
    {
        //LOG_TRACE << "CArtworkFetchTaskHandler::waitForResponceFromExitCode(), iPod Already disconnect" << LOG_ENDL;
        throw  IPOD_ERROR_DISCONNECT;
    }

    CIpodEventLoop localLoop(nExitCode);

    connect(this,SIGNAL(mainLoopQuit(int)),&localLoop,SLOT(mainLoopQuitSlot(int)));

    CEventloopTimerExitCode localWaitTimer;
    localWaitTimer.start(&localLoop,timeOut);

    m_nRunningLoopFromSignal ++;   // added by sungha.choi 2013.04.26
    ret = localLoop.exec();
    m_nRunningLoopFromSignal --;   // added by sungha.choi 2013.04.26

    if(m_nRunningLoopFromSignal == 0 && m_ReadyToDeleteFromSignal == false)  // added by sungha.choi 2013.04.26
    {
        m_ReadyToDeleteFromSignal = true;
    }

    localWaitTimer.stop();

    if((ret == IPOD_ERROR_DISCONNECT) || m_isDisconnected)
    {
        //LOG_TRACE << "CArtworkFetchTaskHandler::waitForResponceFromExitCode(), Loop exit with Error on disconnect" << LOG_ENDL;
        throw  IPOD_ERROR_DISCONNECT;
    }
    ////LOG_TRACE << "CIPodDBFetchTaskHandler::waitForResponceFromExitCode() OUT" << LOG_ENDL;
    return ret;//Return IPOD_SUCCESS or IPOD_ERROR_TIMEOUT
}

void CArtworkFetchTaskHandler::packetAvailable(int lingoID,int commandID,const QByteArray& buf )
{
#ifdef DBUGLOGS
    //LOG_TRACE << "CArtworkFetchTaskHandler::packetAvailable:IN, lingo="<<lingoID<<", Cmd="<<commandID<<LOG_ENDL;
#endif
    try
    {
        switch(lingoID)
        {
            case CIAPCmds::ExtenededLingoId:
            {
                switch(commandID)
                {
                    case CIAPCmds::L4_01_iPodAck:
                    {
                        //LOG_TRACE << "   L4_01_iPodAck" << LOG_ENDL;
                        CACK ack(m_iPodPrivateData->m_isIDPS);
                        ack.unmarshall(buf);
                        int status = ack.getAckStatus();
                        int cmd = ack.getAcknowledgedCommand();
                        if(status!=0)//If there is failure.
                        {
                            switch(cmd)
                            {
                            case CIAPCmds::L4_4C_GetTrackArtworkTimesUID:
                                //LOG_TRACE << "  Bad Param L4_4C_GetTrackArtworkTimesUID" << LOG_ENDL;
                                emit mainLoopQuit(ExitCode_GetArtworkTimesUID);
                                break;
                            case CIAPCmds::L4_2A_GetTrackArtworkTimes:
                                //LOG_TRACE << "  Bad Param L4_2A_GetTrackArtworkTimes" << LOG_ENDL;
                                emit mainLoopQuit(ExitCode_GetTrackArtworkTimes);
                                break;
                            case CIAPCmds::L4_10_GetTrackArtworkData:
                                //LOG_TRACE << "  Bad Param L4_10_GetTrackArtworkData" << LOG_ENDL;
                                m_artIndex = BAD_PARAM_INDEX;
                                emit mainLoopQuit(ExitCode_GetTrackArtworkData);
                                break;
                            case CIAPCmds::L4_4E_GetTrackArtworkDataUID:
                                //LOG_TRACE << "  Bad Param L4_10_GetTrackArtworkDataUID" << LOG_ENDL;
                                m_previous_packet_index = BAD_PARAM_INDEX;
                                emit mainLoopQuit(ExitCode_GetArtworkDataUID);
                                break;
                            default:
                                break;
                            }
                        }
                    }
                        break;
                    case CIAPCmds::L4_0F_RetArtworkFormats://RetArtworkFormats
                    {
                        //LOG_TRACE << "   L4_0F_RetArtworkFormats" << LOG_ENDL;
                        //I will have to delete the old retartworkformats before allocating new one
                        //This is necessary to prevent memory leak.
                        //Normally, it will not be necessary, because, the GetArtworkFormats will only
                        //be called once, at a time. We will not have two instance of RetArtworkFormats.
                        CRetArtworkFormats *tmpCmd;
                        CRetArtworkFormats *cmd = new CRetArtworkFormats( m_iPodPrivateData->m_isIDPS );
                        if(NULL == cmd)
                            return;

                        cmd->unmarshall(buf);
                        tmpCmd = m_retArtworkFormats;
                        m_retArtworkFormats = cmd;

                        if(tmpCmd)
                        {
                            delete tmpCmd;
                            tmpCmd = NULL;
                        }

                        emit mainLoopQuit(ExitCode_GetArtworkFormats);
                    }
                        break;
                    case CIAPCmds::L4_2B_RetTrackArtworkTimes://RetTrackArtworkTimes
                    {
                        CRetTrackArtworkTimes *tmpCmd;
                        CRetTrackArtworkTimes *cmd = new CRetTrackArtworkTimes(m_iPodPrivateData->m_isIDPS );
                        if(NULL == cmd)
                            return;

                        cmd->unmarshall(buf);
                        tmpCmd = m_retTrackArtworkTimes;//this should  be null.
                        m_retTrackArtworkTimes = cmd;

                        if(tmpCmd)
                        {
                            delete tmpCmd;
                            tmpCmd = NULL;
                        }

                        emit mainLoopQuit(ExitCode_GetTrackArtworkTimes);
                    }
                        break;
                    case CIAPCmds::L4_11_RetTrackArtworkData://RetTrackArtworkData
                    {
                        CRetTrackArtworkData  cmd(m_iPodPrivateData->m_isIDPS );
                        cmd.unmarshall(buf);
                        int pi = cmd.getPacketIndex();
                        //LOG_TRACE<< "L4_11_RetTrackArtworkData, pi: "
                        //         << pi <<" length: "<< cmd.getArtwork().length()<<LOG_ENDL;
                        if( pi != (m_artIndex + 1) )
                        {
                            //LOG_INFO << "   L4_11_RetTrackArtworkData, ***Packet Missed***, prev = " << m_artIndex
                            //         << ", cur = " << pi << LOG_ENDL;
                            m_artData.clear();
                            m_artIndex = INVALID_PACKET_INDEX;
                            emit mainLoopQuit(ExitCode_GetTrackArtworkData);
                            // emit mainLoopQuit();
                        }

                        if(m_artIndex == INVALID_PACKET_INDEX)
                        {
                            return;
                        }

                        m_artIndex = pi;
                        if( pi == 0 )
                        {
                            m_artWidth  = cmd.getArtworkWidth();
                            m_artHeight = cmd.getArtworkHeight();
                            m_artFormat = cmd.getArtworkFormat();

                            m_rowSize   = cmd.getRowSize();

                            m_totalImageSize = m_artHeight * m_artWidth * 2;//16bpp
                            m_artData.clear();

                            //LOG_INFO << "   L4_11_RetTrackArtworkData, PacketIndex = " << m_artIndex << ", ArtworkFormat = "<< m_artFormat<< ", ImageWidth ="<< m_artWidth
                            //         << ", ImageHeight = "<< m_artHeight<< ", RowSize = "<< m_rowSize << LOG_ENDL;

                        }
                        else
                        {
                            //LOG_INFO << "   L4_11_RetTrackArtworkData, PacketIndex = " << m_artIndex << ", ArtworkFormat = "<< m_artFormat<< ", ImageWidth ="<< m_artWidth
                            //         << ", ImageHeight = "<< m_artHeight<< ", RowSize = "<< m_rowSize << LOG_ENDL;
                        }

                        m_artData.append(cmd.getArtwork());

                        if( m_artData.size() >= m_totalImageSize )
                        {
                            //LOG_TRACE << "   *********** Fetch Complete********" << LOG_ENDL;
                            //LOG_TRACE  << "CArtworkFetchTaskHandler::packetAvailable(), m_totalImageSize =" << m_totalImageSize << ", DataSize = "<< m_artData.size() << LOG_ENDL;
                            emit mainLoopQuit(ExitCode_GetTrackArtworkData);
                        }
                    }
                        break;
                    case CIAPCmds::L4_4D_RetTrackArtworkTimesUID :
                    {
                        CRetTrackArtworkTimesUID *tmpCmd;
                        CRetTrackArtworkTimesUID *cmd = new CRetTrackArtworkTimesUID( m_iPodPrivateData->m_isIDPS );

                        if(NULL == cmd)
                            return;

                        cmd->unmarshall(buf);
                        tmpCmd = m_retArtworkTimesUID;
                        m_retArtworkTimesUID = cmd;
                        if(tmpCmd)
                        {
                            delete tmpCmd;
                            tmpCmd = NULL;
                        }
                        emit mainLoopQuit(ExitCode_GetArtworkTimesUID);
                    }
                        break;
                    case 0x004F:
                    {
                        ////LOG_TRACE << " the debug print is=" << LOG_ENDL;
                        //CIpodControllerGlobal::printArray(buf);
                        retArtworkDataUID(buf);
                    }
                        break;
                    default://Do nothing
                        break;
                }//End switch(commandID)
            }//case ExtenededLingoId END
                break;
            default://Do nothing
                break;
        }//End switch(lingoID)
    }   // end try
    catch(EIPodErrors ex)
    {
        //LOG_TRACE << "CArtworkFetchTaskHandler::PacketAvailable() Execption Caught *******" << LOG_ENDL;
        return;
    }
    return;
}

int CArtworkFetchTaskHandler::getSupportedArtworkFormatsE(int &nBestFormatID)
{
    int ret = -1;

    if(m_retArtworkFormats)
    {
        nBestFormatID = m_nBestFormatID;
        return IPOD_SUCCESS;
    }

    CGetArtworkFormats cmd(m_iPodPrivateData->getTransactionId(),m_iPodPrivateData->m_isIDPS );
    cmd.marshall();
    emit sendiAPPacket(cmd.getCommandBytes());

    ret = waitForResponceFromExitCode(7000, ExitCode_GetArtworkFormats);

    if(IPOD_SUCCESS != ret)
    {
        return ret;
    }

    int nMaxWidth = -1;
    int nWidth, nHeight, nFormatID, nPixelFormat;
    if(m_retArtworkFormats!=NULL)
    {
        int cnt = m_retArtworkFormats->getFormatCount();
        for(int i = 0; i<cnt; i++)
        {
            nWidth = m_retArtworkFormats->getImageWidth(i);
            nHeight = m_retArtworkFormats->getimageHeight(i);
            nFormatID = m_retArtworkFormats->getFormatID(i);
            nPixelFormat= m_retArtworkFormats->getPixelFormat(i);
            //LOG_INFO << "CArtworkFetchTaskHandler::getSupportedArtworkFormatsE(), FormatId = " << nFormatID
            //         << ", PixelFormat = "<< nPixelFormat
            //         << ", Width = "<< nWidth
            //         << ", Height =" << nHeight<< LOG_ENDL;

            if(nWidth > nMaxWidth) //get the largest (perhaps is the best)
            {
                nBestFormatID = nFormatID;
                nMaxWidth = nWidth;
            }
        }
        m_nBestFormatID = nBestFormatID;
    }
    else
    {
        return IPOD_ERROR_GENERIC;
    }
    return IPOD_SUCCESS;
}


int CArtworkFetchTaskHandler::fetchArtworkE(int trackIndex,QString &artPath, bool isUIDSupported, bool forceFlag, quint64 aUID)  // Modified by pawan 2013.05.15.
{
    //LOG_TRACE << "basicp CArtworkFetchTaskHandler::fetchArtwork() IN, trackIndex = " << trackIndex << ", UID = " << aUID << ", forceFlag = " << forceFlag << LOG_ENDL;

    if(m_isDisconnected)
    {
        //LOG_TRACE << "basicp CArtworkFetchTaskHandler::fetchArtwork(), iPod is disconnected so return  " << LOG_ENDL;
        throw IPOD_ERROR_DISCONNECT;
    }

    int indexArtwork = (aUID > 0 ) ? aUID : trackIndex;
    if(m_TriedArtworkIndexList.contains(indexArtwork))
    {        
        //LOG_TRACE << "   already fetched data " << LOG_ENDL;

        if(!m_iPodPrivateData->m_IsCurrentPlayingMP || forceFlag)
        {
            //LOG_TRACE <<"   currently playing song is not of MP or related playlist so fetch artwork" << LOG_ENDL;
        }
        else
        {
            QString fileName = QString("%1artwork_%2.jpg").arg(TEMP_ARTWORK_LOCATION).arg(indexArtwork);
            if(QFile::exists(fileName))
            {
                artPath.append(fileName);
                return IPOD_SUCCESS;
            }
            else
            {
                //LOG_TRACE << "   marked fetched but not exist " << LOG_ENDL;
            }
        }
    }
    else
    {
        m_TriedArtworkIndexList.append(indexArtwork);
    }

    int formatId     = -1;
    int ret          = -1;
    int timeOffset   = -1;

    m_artWidth       = 0;
    m_artHeight      = 0;
    m_artFormat      = 0;
    m_artIndex       = -1;
    m_rowSize        = 0;

    m_totalImageSize = 0;
    m_artData.clear();

    //Tan: this function ret the best artwork format for a
    //particular device. It is not song dependent, so it only
    //has to be called once.The best format id is the largest resolution.
    ret = getSupportedArtworkFormatsE(formatId);

    //LOG_TRACE << "basicp   formatId = "<< formatId << LOG_ENDL;
    //LOG_TRACE << "basicp   aUID = "<< aUID << LOG_ENDL;
    //LOG_TRACE << "basicp   getSupportedArtworkFormatsE = "<< ret << LOG_ENDL;

    if(IPOD_SUCCESS != ret)
        return ret;    

    /* Get supported artwork formats available for the track */
    if((aUID <=0) || !m_iPodPrivateData->m_IsCurrentPlayingMP || !isUIDSupported) // Modified by pawan 2013.05.15.
    {
        if(formatId == -1)
        {
            return IPOD_ERROR_GENERIC;
        }

        /* Get TimeOffset for the format ID */
        //LOG_TRACE << "basicp   Get TimeOffset for the format ID = "<< formatId << LOG_ENDL;
        CGetTrackArtworkTimes cmdArtTimes(trackIndex,formatId,0,1,m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS );
        cmdArtTimes.marshall();
        emit sendiAPPacket(cmdArtTimes.getCommandBytes());
        ret = waitForResponceFromExitCode(7000, ExitCode_GetTrackArtworkTimes);

        if(IPOD_SUCCESS != ret)
        {
            //LOG_TRACE << "basicp   Failed for RetTrackArtworkTimes" << LOG_ENDL;
            //LOG_TRACE << "basicp CArtworkFetchTaskHandler::fetchArtwork() OUT" << LOG_ENDL;
            return ret;
        }

        if(m_retTrackArtworkTimes!=NULL)
        {
            timeOffset = m_retTrackArtworkTimes->getTimeOffset();
            //LOG_TRACE << "basicp   timeOffset = " << timeOffset << LOG_ENDL;

            if( m_retTrackArtworkTimes )
            {
                delete m_retTrackArtworkTimes;
                m_retTrackArtworkTimes = NULL;
            }

            int nRetryCount = 0;
            //LOG_TRACE << "basicp   Get Artwork Data Start" << LOG_ENDL;

            while( nRetryCount < 10 )
            {
                nRetryCount++;
                m_artIndex = -1;
                //LOG_TRACE << "CArtworkFetchTaskHandler::fetchArtworkE(), Request Artwork for Track Index = " << trackIndex << LOG_ENDL;
                m_artData.clear();
                CGetTrackArtworkData cmdGetData(trackIndex,formatId,timeOffset,m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS );
                cmdGetData.marshall();
                emit sendiAPPacket(cmdGetData.getCommandBytes());
                ret = waitForResponceFromExitCode(3000, ExitCode_GetTrackArtworkData);
                if(m_artIndex == BAD_PARAM_INDEX)
                {
                    //LOG_TRACE<<"Failed artwork because of bad param"<<LOG_ENDL;
                    break;
                }
                else if(m_artIndex != INVALID_PACKET_INDEX && ret == 0 && !(( m_artData.size() < m_totalImageSize ) || m_totalImageSize <= 0 ))
                {
                    //LOG_TRACE<<"Successful artwork nRetryCount: "<< nRetryCount <<LOG_ENDL;
                    break;
                }
                else
                {
                    //LOG_TRACE<<"Failed artwork nRetryCount: "<< nRetryCount << LOG_ENDL;
                    waitForResponceFromExitCode(1000, ExitCode_AllExit);
                }
			}
            if( IPOD_SUCCESS != ret )
            {
                //LOG_TRACE << "basicp   Faied for RetTrackArtworkData" << LOG_ENDL;
                //LOG_TRACE << "basicp CArtworkFetchTaskHandler::fetchArtwork() OUT" << LOG_ENDL;
                return ret;
            }
            //LOG_TRACE << "CArtworkFetchTaskHandler::fetchArtworkE(), Get Artwork Data SUCCESS, Track Index = " << trackIndex << LOG_ENDL;
        }
        else
        {
            //LOG_TRACE << "basicp CArtworkFetchTaskHandler::fetchArtwork() OUT" << LOG_ENDL;
            return IPOD_ERROR_GENERIC;
        }
    }
    else
    {
        ret = fetchArtWorkForUIDDeviceE(aUID);
        if(ret != 0)
        {
            //LOG_TRACE << "basicp   call fetchArtworkForUIDDeviceE() trackIndex = " << trackIndex << ", ret = " << ret << LOG_ENDL;
            //LOG_TRACE << "basicp CArtworkFetchTaskHandler::fetchArtwork() OUT" << LOG_ENDL;
            return ret;
        }
    }

    //LOG_TRACE << "basicp   m_artWidth = " << m_artWidth << ", m_artHeight = " << m_artHeight  << ", m_artData.size() = " << m_artData.size() << ", m_totalImageSize = " << m_totalImageSize << ", m_totalImageSize = " << m_totalImageSize << LOG_ENDL;
    if(( m_artData.size() < m_totalImageSize ) || m_totalImageSize <= 0 )
    {
        //LOG_TRACE << "basicp   retrieval of the artwork data is failed" << LOG_ENDL;
        //LOG_TRACE << "basicp CArtworkFetchTaskHandler::fetchArtwork() OUT" << LOG_ENDL;
        return IPOD_ERROR_GENERIC;
    }

    QImage artImage((uchar*)m_artData.data(),m_artWidth, m_artHeight, m_rowSize, QImage::Format_RGB16);

    //LOG_TRACE << " basicp   artImage.format = "<< artImage.format() <<", artImage.width =" << artImage.width() <<", artImage.height = " << artImage.height() << LOG_ENDL;

    QString path;
    if(aUID > 0 && trackIndex == -1)
    {
        path.append(QString("%1artwork_%2.jpg").arg(TEMP_ARTWORK_LOCATION).arg(aUID));
    }
    else
    {
        static unsigned int anyuniquevalue = 0;
        anyuniquevalue++;
        if(!m_iPodPrivateData->m_IsCurrentPlayingMP)
        {
            path.append(QString("%1artwork_otherapps_%2_uv%3.jpg").arg(TEMP_ARTWORK_LOCATION).arg(trackIndex).arg(anyuniquevalue));
        }
        else if(forceFlag)
        {
            path.append(QString("%1artwork_forced_%2_uv%3.jpg").arg(TEMP_ARTWORK_LOCATION).arg(trackIndex).arg(anyuniquevalue));
        }
        else
        {
         path.append(QString("%1artwork_%2_uv%3.jpg").arg(TEMP_ARTWORK_LOCATION).arg(trackIndex).arg(anyuniquevalue));
        }
    }
    if(artImage.save(path,"JPG", -1))
    {
        artPath.append(path);
        //LOG_TRACE << "basicp   Save Success, track Index = " << trackIndex << path << LOG_ENDL;
    }
    else
    {
        //LOG_TRACE << "basicp   Save Failed,  track Index = " << trackIndex << LOG_ENDL;
        //LOG_TRACE << "basicp CArtworkFetchTaskHandler::fetchArtwork() OUT" << LOG_ENDL;
        return IPOD_ERROR_GENERIC;
    }
    //LOG_TRACE << "basicp CArtworkFetchTaskHandler::fetchArtwork() OUT" << LOG_ENDL;
    return IPOD_SUCCESS;
}

int CArtworkFetchTaskHandler::fetchArtWorkForUIDDeviceE(quint64 uid) // Modified by pawan 2013.05.15.
{
    //LOG_TRACE << "CArtworkFetchTaskHandler::fetchArtWorkForUIDDeviceE() IN" << LOG_ENDL;

    bool gotArtWork = false;
    int cnt = m_retArtworkFormats->getFormatCount();
    //LOG_TRACE << "   format count is = " << cnt << LOG_ENDL;
    int i = 0;

	if(cnt > 0)
	{
        for( i = 0 ; i < cnt ; i++)
		{
            //LOG_TRACE << "   FormatId = " << m_retArtworkFormats->getFormatID(i) << ", PixelFormat = "<< m_retArtworkFormats->getPixelFormat(i)
            //    << ", Width = " << m_retArtworkFormats->getImageWidth(i)
            //    << ", Height = " << m_retArtworkFormats->getimageHeight(i) << LOG_ENDL;

            emit sendiAPPacket(getArtworkTimesUID(m_retArtworkFormats->getFormatID(i), uid));  // Modified by pawan 2013.05.15.
            int ret = waitForResponceFromExitCode(7000, ExitCode_GetArtworkTimesUID);
            if (m_retArtworkTimesUID != NULL)
			{
                int artwork_time = m_retArtworkTimesUID->getTimeOffset();

                if( m_retArtworkTimesUID )
                {
                    delete m_retArtworkTimesUID;
                    m_retArtworkTimesUID = NULL;
                }

				if(ret == 0 && (artwork_time != -1))
				{
					m_artWidth = m_retArtworkFormats->getImageWidth(i);
					m_artHeight = m_retArtworkFormats->getimageHeight(i);
                    int nRetryCount = 0;
                    while(nRetryCount < 10)
                    {
                        nRetryCount++;
                        m_previous_packet_index = -1;
                        m_artData.clear();
                        //LOG_TRACE << "CArtworkFetchTaskHandler::fetchArtWorkForUIDDeviceE(), Request artwork for UID = " << uid << LOG_ENDL;
                        emit sendiAPPacket(getArtWorkUID(artwork_time, m_retArtworkFormats->getFormatID(i), uid)); // Modified by pawan 2013.05.15.
                        ret = waitForResponceFromExitCode(3000, ExitCode_GetArtworkDataUID);

                        if(m_previous_packet_index == BAD_PARAM_INDEX)
                        {
                            //LOG_TRACE << "UID Failed artwork because of bad param"<<LOG_ENDL;
                            break;
                        }
                        else if(m_previous_packet_index != INVALID_PACKET_INDEX && ret == 0 && !(( m_artData.size() < m_totalImageSize ) || m_totalImageSize <= 0 ))
                        {
                            //break if success
                            //LOG_TRACE << "UID Successful artwork nRetryCount: "<< nRetryCount <<LOG_ENDL;
                            break;
                        }
                        else
                        {
                            //LOG_TRACE << "UID Failed artwork nRetryCount: "<< nRetryCount <<LOG_ENDL;
                            waitForResponceFromExitCode(1000, ExitCode_AllExit);
                        }
                    }

                    if(ret == 0)
					{
                        //LOG_INFO << "   artwork retrieval is success" << LOG_ENDL;
						gotArtWork = true;
					}
					break;
				}
			}
			else
			{
                //LOG_INFO << "   getArtworkTimesUID is failed " << LOG_ENDL;
                //LOG_TRACE << "CArtworkFetchTaskHandler::fetchArtWorkForUIDDeviceE() OUT" << LOG_ENDL;
                return IPOD_ERROR_GENERIC;
			}
		}
    }

    //LOG_TRACE << "CArtworkFetchTaskHandler::fetchArtWorkForUIDDeviceE() OUT" << LOG_ENDL;
    if(!gotArtWork)
        return IPOD_ERROR_GENERIC;
    else
        return IPOD_SUCCESS;
}

QByteArray CArtworkFetchTaskHandler::getArtworkTimesUID(int formatID, quint64 uid) // Modified by pawan 2013.05.15.
{
    //LOG_INFO << "CArtworkFetchTaskHandler::getArtworkTimesUID() IN" << LOG_ENDL;
    CGetArtworkTimesUID cmd(m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS, formatID, uid); // Added by pawan 2013.05.15.
    cmd.marshall();
    //LOG_INFO << "CArtworkFetchTaskHandler::getArtworkTimesUID() OUT" << LOG_ENDL;
    return cmd.getCommandBytes();
}

QByteArray CArtworkFetchTaskHandler::getArtWorkUID(int timeOffset, int formatID, quint64 uid) // Added by pawan 2013.05.15.
{
    //LOG_INFO << " CArtworkFetchTaskHandler::getArtWorkUID() IN" << LOG_ENDL;
    CGetArtworkDataUID cmd(formatID, timeOffset, m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS, uid); // Modified by pawan 2013.05.15.
    cmd.marshall();
    //LOG_INFO << " CArtworkFetchTaskHandler::getArtWorkUID() OUT" << LOG_ENDL;
    return cmd.getCommandBytes();
}

void CArtworkFetchTaskHandler::retArtworkDataUID(QByteArray returnCommand)
{
    //LOG_INFO << " CArtworkFetchTaskHandler::retArtworkDataUID() IN" << LOG_ENDL;

    CRetArtworkDataUID cmd(m_iPodPrivateData->m_isIDPS);
    cmd.unmarshall(returnCommand);

    int packetIndex = cmd.getPacketIndex();
    int totalIndex  = cmd.getTotalPacketIndex();

    if(packetIndex != m_previous_packet_index+1)
    {
        m_previous_packet_index = INVALID_PACKET_INDEX;
        emit mainLoopQuit(ExitCode_GetArtworkDataUID);
    }

    if(m_previous_packet_index == INVALID_PACKET_INDEX)
    {
        return;
    }

    m_previous_packet_index = packetIndex;

    if( packetIndex == 0 )
    {
        m_artWorkTotalCount = totalIndex;
        m_artData.clear();
        m_artWidth  = cmd.getArtworkWidth();
        m_artHeight = cmd.getArtworkHeight();
        m_artFormat = cmd.getArtworkFormat();
        m_rowSize   = cmd.getRowSize();

        m_totalImageSize = m_artHeight * m_artWidth * 2;//16bpp
        //LOG_TRACE << "   PacketIndex = " << packetIndex << ", ArtworkFormat = " << m_artFormat<< ", ImageWidth = "<< m_artWidth
        //          << ", ImageHeight = "<< m_artHeight<< ", RowSize ="<< m_rowSize << LOG_ENDL;
    }
    m_artData.append(cmd.getArtwork());

    if(m_artWorkTotalCount == packetIndex)
    {
        //LOG_TRACE << "   *********** Fetch Complete********" << LOG_ENDL;
        //LOG_TRACE  << "CArtworkFetchTaskHandler::retArtworkDataUID(), m_totalImageSize = " << m_totalImageSize << ", DataSize = " << m_artData.size() << LOG_ENDL;
        emit mainLoopQuit(ExitCode_GetArtworkDataUID);
    }
    //LOG_TRACE << "CArtworkFetchTaskHandler::retArtworkDataUID() OUT" << LOG_ENDL;
}


bool CArtworkFetchTaskHandler::cleanupAfterArtworkFetch()
{
    //LOG_TRACE << "CArtworkFetchTaskHandler::cleanupAfterArtworkFetch() IN" << LOG_ENDL;

    QDir dir(TEMP_ARTWORK_LOCATION);
	bool result = false;

    QStringList sList = dir.entryList(QDir::Files);

    for(int i = 0; i < sList.count(); i++)
    {
        QString singleFile = QString(TEMP_ARTWORK_LOCATION) + "/" + sList.at(i);
        if(dir.exists(singleFile))
        {
            result = dir.remove(singleFile);
            if(result == true)
            {
                //LOG_TRACE << "   successfully removed fileName = " << singleFile.data() << LOG_ENDL;
            }
            else
            {
                //LOG_TRACE << "   failed to remove fileName =  " << singleFile.data() << LOG_ENDL;
            }
        }
    }

    //LOG_TRACE << "CArtworkFetchTaskHandler::cleanupAfterArtworkFetch() OUT" << LOG_ENDL;
    return result;
}
