/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Sowmini / Venkat                    *
 * DATE       :  2 Jan 2012
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                   This is the class which will help control    *
 *                   to put the device to enter into the remote   *
 *                   UI mode                                      *
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
#include <DHAVN_IPodController_eventloopTimer.h>
#include <DHAVN_IPodController_StorageLingoiPodAck.h>
#include <DHAVN_IPodController_iTunesTagTaskHandler.h>
#include <DHAVN_IPodController_getiPodCaps.h>          
#include <DHAVN_IPodController_retiPodCaps.h>
#include <DHAVN_IPodController_getiPodFreeSpace.h>     
#include <DHAVN_IPodController_retiPodFileHandle.h>
#include <DHAVN_IPodController_openiPodFeatureFile.h>  
#include <DHAVN_IPodController_retiPodFreeSpace.h>
#include <DHAVN_IPodController_closeiPodFile.h>
#include <DHAVN_IPodController_WriteiPodFileData.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include "DHAVN_EventLoop.h"
#include "DHAVN_EventLoopTimerExitCode.h"


//#define TRANS_ID 5
#define CHUNK_SIZE 234

using namespace StorageLingo;
#define PATH_TO_PLIST_FILES                      "/app/data/AppRadio/IPodData"
#define PATH_TO_PLIST_SECOND_FILES               "/app/data/AppXMAudio/IPodData"
#define ITUNESTAG_TASKHANDLER_CONNECTION         "IpodDbusTagConnection"

#define IPODCONTROLLER_DBUS_PATH       "/radio/ipodcontroller/path"
#define IPODCONTROLLER_DBUS_INTERFACE  "radio.ipodcontroller.interface"
#define TAG_WRITE_RESULT_SIGNAL        "iTunesTaggingResult"

#define HD_RADIO_DBUS_PATH       "/com/Hyundai/ipodcontroller"
#define HD_RADIO_DBUS_INTERFACE  "com.hyundai.ipodcontroller"
#define HD_RADIO_SERVICE_NAME     "com.hyundai.ipodcontroller"
#define HD_RADIO_DBUS_SIGNAL     "iTunesTagUpdated"

CiTunesTagTaskHandler::CiTunesTagTaskHandler(QObject *parent, iPodLogger *iLog) : CTaskHandler(parent),m_FileHandleToBeClosed(true),m_DbusConnection(ITUNESTAG_TASKHANDLER_CONNECTION)
{
    m_nRunningLoopFromSignal = 0;
    m_ReadyToDeleteFromSignal = true;

    m_iPodPrivateData = parent->findChild<iPodControllerPrivateData*>(gControllerPrivateData);
    switch(m_iPodPrivateData->m_currentPortNum)
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
    //   LOG_INIT_V2(gControllerLogName);


    //LOG_TRACE << "CiTunesTagTaskHandler::CiTunesTagTaskHandler Entry " << LOG_ENDL;
    setObjectName(giTunesTagTaskHandler);
    m_isDisconnected = false;
    m_FileHandle = -1;
    m_firstTime = true;
    m_FreeSpaceOnDevice = 0;
    //m_iPodPrivateData = parent->findChild<iPodControllerPrivateData*>(gControllerPrivateData);
    m_pLoop = NULL;    // added by sungha.choi 2013.04.12
    m_pWaitTimer = NULL;
    m_ReadyToDelete = true;
    m_isRunningLoop = false;    // added by sungha.choi 2013.04.13
    piPodLog = iLog;


    qRegisterMetaType<QByteArray>("QByteArray");
    connect(((CIpodController*) parent)->m_Reader,SIGNAL(packetAvailable(int, int, QByteArray)),
            this, SLOT(packetAvailable(int,int,QByteArray)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(sendiAPPacket(QByteArray)) ,
            ((CIpodController*) parent)->m_Writer, SLOT(sendiAPPacket(QByteArray)),
            Qt::QueuedConnection);


    m_DbusConnection = QDBusConnection::connectToBus(QDBusConnection::SystemBus,ITUNESTAG_TASKHANDLER_CONNECTION);
    if(m_DbusConnection.isConnected() == true)
    {
        //LOG_TRACE << "CiTunesTagTaskHandler::connected to  dbus daemon" << LOG_ENDL;
        m_DbusConnection.connect("",HD_RADIO_DBUS_PATH,HD_RADIO_DBUS_INTERFACE,HD_RADIO_DBUS_SIGNAL,this, SLOT(updateTagListenerE(QString,QString)));
    }
    //else //LOG_TRACE << "CiTunesTagTaskHandler::NOT connected to  dbus daemon" << LOG_ENDL;

    //LOG_TRACE << "CiTunesTagTaskHandler::CiTunesTagTaskHandler Exit " << LOG_ENDL;
}

CiTunesTagTaskHandler::~CiTunesTagTaskHandler()
{
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

    //LOG_TRACE << "CiTunesTagTaskHandler::~CiTunesTagTaskHandler" << LOG_ENDL;
}


/* Stops the entair taskhandler. This will disconnect the taskhandler form iAP channel*
 * This can be called if the task handler is not required to operate.
 * This will improve the performance.
*/

void CiTunesTagTaskHandler::stopTaskHandler()
{
    //TODO
}

void CiTunesTagTaskHandler::updateTagListenerE(QString pListPath,QString pListName)
{
    try
    {
        if( (!(pListPath.isEmpty())) && (!(pListName.isEmpty())) )
        {
            //LOG_INFO << "CiTunesTagTaskHandler updateTagListener() pListPath = " << pListPath << ", pListName = " << pListName << LOG_ENDL;
            m_firstTime = true;
            if( pListPath.contains("HD") == true ) { // HD Radio
                //LOG_INFO << "CiTunesTagTaskHandler updateTagListener() This is HD Radio Path" << LOG_ENDL;
                iTunesTagDirectoryE(PATH_TO_PLIST_FILES);
            }
            else if( pListPath.contains("XM" ) == true ) {
                //LOG_INFO << "CiTunesTagTaskHandler updateTagListener() This is XM Radio Path" << LOG_ENDL;
                iTunesTagDirectoryE(PATH_TO_PLIST_SECOND_FILES);
            }
        }
    }
    catch(EIPodErrors ex)
    {
        //LOG_TRACE <<" Disconnected while tagging" << LOG_ENDL;
        return;
    }
}

void CiTunesTagTaskHandler::closeFileHandleE(int dFileHandle)
{
    //LOG_INFO << "CiTunesTagTaskHandler::closeFileHandle Entry = " << dFileHandle << LOG_ENDL;
    if(dFileHandle >= 0)
    {
        CCloseiPodFeatureFile cmdClose(dFileHandle,m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS );
        cmdClose.marshall();
        // send to writer
        //(parent()->findChild<CIAPPacketWriter*>(gWriter))->SendPacket(cmdClose);
        emit sendiAPPacket(cmdClose.getCommandBytes());
        waitForResponceFromExitCode(7000, ExitCode_CloseiPodFeatureFile);
    }
    //LOG_TRACE << "CiTunesTagTaskHandler::closeFileHandle Exit" << LOG_ENDL;
}

void CiTunesTagTaskHandler::startiTunesTagTaskHandlerE()
{
    iTunesTagDirectoryE(PATH_TO_PLIST_FILES);
    iTunesTagDirectoryE(PATH_TO_PLIST_SECOND_FILES);
}

void CiTunesTagTaskHandler::iTunesTagDirectoryE(QString plistPath)
{
    bool iTunesTaggingSucceeded = true;
    QDir dir(plistPath);
    QFileInfoList list = dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot |QDir::AllEntries);

    foreach(QFileInfo info, list)
    {
        if(info.isDir())
        {
            QDir subDir(info.absoluteFilePath());
            QStringList filters;
            filters << "*.plist" ;
            subDir.setNameFilters(filters);

            QStringList pLists = subDir.entryList(filters);
            //LOG_INFO << "Directory Path : " << info.absoluteFilePath()<< LOG_ENDL;
            int result = 0;
            if ( pLists.count() > 0 )
            {
                for(int i = 0; i < pLists.count(); i++)
                {
                    // QString singlepList = QString(PATH_TO_PLIST_FILES) + "/" + pLists.at(i);
                    QString singlepList = (info.absoluteFilePath())+ "/" + pLists.at(i);
                    //LOG_INFO << "Playist(singlepList) complete path : " << singlepList<< LOG_ENDL;
                    if(subDir.exists(singlepList) )
                    {
                        result = EmitiTunesTaggingStatusE(singlepList);
                        if ( result != iPodiTunes_SUCCESS)
                        {
                            iTunesTaggingSucceeded =  false;
                            break;
                        }
                    }
                }
                if ( iTunesTaggingSucceeded == true)
                {
                    //LOG_TRACE << "CiTunesTagTaskHandler:: Emitting Tagging Successfull from iTunesTagDirectory " << LOG_ENDL;
                    //emit IPodPopUp(iPodiTunes_SUCCESS);
                }
            }
        }
    }
}

int CiTunesTagTaskHandler::exportTagFileE(QString plistPath)
{
    //LOG_INFO << "CiTunesTagTaskHandler::exportTagFile Entry " << plistPath.toAscii().data() << LOG_ENDL;
    //int result = IPOD_SUCCESS; 	// Set error if any occurs - else return success
    bool result = false;	// Set success when entire plist file is written
    int errCodeResult = iPodiTunes_NONE; // Function return type is int, returning a Enum value sometimes result in Negative returns. Coverity fix- negative returns
    QString dpListFile = plistPath ;
    bool plistpathvar = plistPath.isEmpty();
    //LOG_INFO << "CiTunesTagTaskHandler::exportTagFile Entry " << plistpathvar << LOG_ENDL;
    if(!plistPath.isEmpty())
    {
        CGetiPodCaps cmd(m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS );
        cmd.marshall();
        // send to writer
        //(parent()->findChild<CIAPPacketWriter*>(gWriter))->SendPacket(cmd);
        emit sendiAPPacket(cmd.getCommandBytes());
        waitForResponceFromExitCode(7000, ExitCode_GetiPodCaps);
        if(m_CommandBuffer.length() > 0)
        {
            quint32 dMaxWriteSize = CHUNK_SIZE ;
            CRetiPodCaps cmd( m_iPodPrivateData->m_isIDPS );
            cmd.unmarshall(m_CommandBuffer);
            if(dMaxWriteSize > cmd.getMaxWriteSize())
                dMaxWriteSize = cmd.getMaxWriteSize();
            //LOG_INFO << "CiTunesTagTaskHandler::RetiPodCaps received total size uint = " << cmd.getTotalSpace()
            //         << ":max file size = "<< cmd.getMaxFileSize()
            //         << ": max write size = "<< cmd.getMaxWriteSize()
            //         << ": major = "<< cmd.getMajorVersion()
            //         << ": minor =  "<< cmd.getMinorVersion()<< LOG_ENDL;
            CGetiPodFreeSpace cmdGet(m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS );
            cmdGet.marshall();
            //(parent()->findChild<CIAPPacketWriter*>(gWriter))->SendPacket(cmdGet);
            emit sendiAPPacket(cmdGet.getCommandBytes());
            waitForResponceFromExitCode(7000, ExitCode_GetiPodFreeSpace);
            if(m_CommandBuffer.length() > 0)
            {
                CRetiPodFreeSpace cmd( m_iPodPrivateData->m_isIDPS );
                cmd.unmarshall(m_CommandBuffer);
                quint64 dFreeSpaceOnDevice = cmd.getFreeSpace();
                //LOG_INFO << "CiTunesTagTaskHandler::RetiPodFreeSpace received.FreeSpace = "
                //         << dFreeSpaceOnDevice << LOG_ENDL;
                if(dFreeSpaceOnDevice > 0)
                {
                    //TODO : if(m_FreeSpaceOnDevice > (size of plist list) )
                    {
                        COpeniPodFeatureFile cmd(m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS );
                        cmd.marshall();
                        //(parent()->findChild<CIAPPacketWriter*>(gWriter))->SendPacket(cmd);
                        emit sendiAPPacket(cmd.getCommandBytes());
                        waitForResponceFromExitCode(7000, ExitCode_OpeniPodFeatureFile);
                        if(m_CommandBuffer.length() > 0)
                        {
                            int dpListTotalSize = 0;
                            CRetiPodFileHandle cmd( m_iPodPrivateData->m_isIDPS );
                            cmd.unmarshall(m_CommandBuffer);
                            m_FileHandle = cmd.getFileHandle();
                            //LOG_INFO << "CiTunesTagTaskHandler::RetiPodFileHandle received.FileHandle= "
                            //         << m_FileHandle << LOG_ENDL;
                            //LOG_INFO << "CiTunesTagTaskHandler::FileName: " << dpListFile << LOG_ENDL;
                            if(m_FileHandle >= 0)
                            {
                                int dpListFd = -1;
                                struct stat filestatus;
                                //int ret = stat( dpListFile.toAscii().data(), &filestatus );
                                int ret = stat( dpListFile.toUtf8().data(), &filestatus );
                                dpListTotalSize = filestatus.st_size;
                                if(ret >= 0 && dpListTotalSize > 0)
                                {
                                    char buffer[dpListTotalSize+1];
                                    //LOG_INFO << "CiTunesTagTaskHandler::RetiPodFileHandle received.File size=" << dpListTotalSize << LOG_ENDL;
                                    memset( buffer,0,sizeof(buffer));
                                    //dpListFd = open((char*)dpListFile.toAscii().data() ,O_RDONLY) ;
                                    dpListFd = open((char*)dpListFile.toUtf8().data(), O_RDONLY);
                                    //LOG_INFO << "CiTunesTagTaskHandler::RetiPodFileHandle received.File Fd= " << dpListFd << LOG_ENDL;
                                    if(dpListFd >= 0)
                                    {
                                        int ret = 0;
                                        ret = read(dpListFd,buffer,dpListTotalSize);
                                        if(ret > 0)
                                        {
                                            int dCurrentOffset = 0;
                                            // { modified by jungae 2013.02.19, changed copy way for deep copy
                                            //QByteArray dpListContents;
                                            // { modified by jungae 2013.02.18, changed other function from fromRawData() to setRawData().
                                            //dpListContents = QByteArray::fromRawData(buffer, sizeof(buffer));
                                            //dpListContents.setRawData(buffer, sizeof(buffer));
                                            // } modified by jungae
                                            QByteArray dpListContents(buffer, sizeof(buffer));
                                            // } modified by jungae
                                            //LOG_INFO << "CiTunesTagTaskHandler read the contents ret=" << ret
                                            //         << ", ByteArray=" << dpListContents.size() << LOG_ENDL;
                                            QByteArray plistChunk = dpListContents.mid(dCurrentOffset,dMaxWriteSize);
                                            if(dCurrentOffset < dpListTotalSize)
                                            {
                                                CWriteiPodFileData cmd(m_iPodPrivateData->getTransactionId(),m_FileHandle,dCurrentOffset,plistChunk.size(),plistChunk, m_iPodPrivateData->m_isIDPS );
                                                cmd.marshall();
                                                dCurrentOffset = dCurrentOffset + plistChunk.size();
                                                //(parent()->findChild<CIAPPacketWriter*>(gWriter))->SendPacket(cmd);
                                                emit sendiAPPacket(cmd.getCommandBytes());
                                                waitForResponceFromExitCode(7000, ExitCode_WriteiPodFileData);
                                            }
                                            while(m_CommandBuffer.size() > 0)
                                            {
                                                //LOG_INFO << "CiTunesTagTaskHandler offset=" << dCurrentOffset << ", m_CommandBuffer.size() = " << m_CommandBuffer.size()
                                                //         << ", dCurrentOffset = " << dCurrentOffset << ", dpListTotalSize = " << dpListTotalSize << LOG_ENDL;
                                                plistChunk.clear();
                                                if(dCurrentOffset < dpListTotalSize)
                                                {
                                                    plistChunk = dpListContents.mid(dCurrentOffset,dMaxWriteSize);
                                                    CWriteiPodFileData cmd(m_iPodPrivateData->getTransactionId(),m_FileHandle,dCurrentOffset,plistChunk.size(),plistChunk, m_iPodPrivateData->m_isIDPS );
                                                    cmd.marshall();
                                                    dCurrentOffset = dCurrentOffset + plistChunk.size();
                                                    //LOG_INFO << "CiTunesTagTaskHandler plistChunk.size() = " << plistChunk.size() << LOG_ENDL;
                                                    //(parent()->findChild<CIAPPacketWriter*>(gWriter))->SendPacket(cmd);
                                                    emit sendiAPPacket(cmd.getCommandBytes());
                                                    waitForResponceFromExitCode(7000, ExitCode_WriteiPodFileData);
                                                    result = true;
                                                    errCodeResult = iPodiTunes_SUCCESS;
                                                    if ( m_firstTime )
                                                    {
                                                        m_firstTime = false;
                                                        emit IPodPopUp(iPodiTunes_SENDINGTAGS);
                                                        //LOG_TRACE << "CiTunesTagTaskHandler:: Emiting Sendig tags to iPod" << LOG_ENDL;
                                                    }
                                                }
                                                else
                                                    break;
                                            }
                                            if(m_CommandBuffer.size() <= 0)
                                            {
                                                result = false;
                                                errCodeResult = iPodiTunes_FAILED;
                                            }

                                        }
                                        close(dpListFd); //CR 15209 added by jungae 2012.10.29, Static_Test 0138233
                                    }
                                    else
                                        QFile::remove(plistPath);
                                }
                                else
                                    QFile::remove(plistPath);
                                //close(dpListFd); // deleted by jungae 2012.10.29, Static_Test 0138233
                                closeFileHandleE(m_FileHandle);
                            }
                            else
                            {
                                //LOG_INFO << "Failed to open plist file of size : " << dpListTotalSize << LOG_ENDL;
                                m_FileHandleToBeClosed = false;
                                result = false;
                                errCodeResult = iPodiTunes_FAILED;
                            }
                        }
                    }
                }
                else
                    errCodeResult = iPodiTunes_MEMORYFULL;
            }
        }//
        if(errCodeResult == iPodiTunes_SUCCESS)
            QFile::remove(plistPath);
        //LOG_INFO << "Dbus call " << LOG_ENDL;
        QDBusMessage signal = QDBusMessage::createSignal(IPODCONTROLLER_DBUS_PATH,IPODCONTROLLER_DBUS_INTERFACE,TAG_WRITE_RESULT_SIGNAL);
        signal << plistPath;
        signal << result;


        bool dBusvalue =m_DbusConnection.send(signal);
        //LOG_INFO << "Dbus Result : " << result << LOG_ENDL;
        //LOG_INFO << "DbusValue : " << dBusvalue << LOG_ENDL;

        if(m_DbusConnection.send(signal))
        {
            //LOG_INFO << " CiTunesTagTaskHandler::tag result signal: dbus sent success " << LOG_ENDL;
        } else
        {
            QDBusError error = m_DbusConnection.lastError();
            //LOG_INFO << " CiTunesTagTaskHandler::tag result signal: dbus sent fail: "
            //         << error.message().toAscii().data() << LOG_ENDL;
        }
        // End of addition

    }//
    else
        result = false;
    //LOG_TRACE << "CiTunesTagTaskHandler::exportTagFile Exit with errCodeResult = " << errCodeResult << LOG_ENDL;
    return errCodeResult;
}

void CiTunesTagTaskHandler::stopCurrentTask()
{
    m_isDisconnected = true; //Crash fix

    //emit mainLoopQuit();    // modified by sungha.choi 2013.04.12
    piPodLog->WriteLog("Here 1-1 m_pLoop->quit() before CiTunesTagTaskHandler");
    if(isWaitingForResponse() == true)
    {
        m_ReadyToDelete = false;
        m_pLoop->quit();
        piPodLog->WriteLog("Here 1-2 m_pLoop->quit() after CiTunesTagTaskHandler");
    }

    if(isWaitingForResponseFromSignal() > 0)  // added by sungha.choi 2013.04.26
    {
        m_ReadyToDeleteFromSignal = false;
        //emit getDBTrackInfoReturned();  // If signals will be added, emit the signal here.
        emit mainLoopQuit(ExitCode_AllExit);  // If signals will be added, emit the signal here.
    }


    //LOG_TRACE << "CiTunesTagTaskHandler::stopTask Exit " << LOG_ENDL;
}

bool CiTunesTagTaskHandler::isReadyToDelete()  // added by sungha.choi 2013.04.12
{
    return (m_ReadyToDelete && m_ReadyToDeleteFromSignal);
}

bool CiTunesTagTaskHandler::isWaitingForResponse()    // added by sungha.choi 2013.04.12
{
    return m_isRunningLoop;
}

int CiTunesTagTaskHandler::isWaitingForResponseFromSignal()    // added by sungha.choi 2013.04.26
{
    return m_nRunningLoopFromSignal;
}

int CiTunesTagTaskHandler::waitForResponceFromExitCode(int timeOut, int nExitCode)
{
    ////LOG_TRACE << "CIPodDBFetchTaskHandler::waitForResponceFromExitCode Entry " << LOG_ENDL;
    int ret = 0;

    if(m_isDisconnected){
        //LOG_TRACE << "CIPodDBFetchTaskHandler::waitForResponceFromExitCode: iPod Already disconnect" << LOG_ENDL;
        //if( m_iPodPrivateData ) m_iPodPrivateData->m_Mutex.unlock();
        throw  IPOD_ERROR_DISCONNECT;
    }

    //QEventLoop localLoop;
    CIpodEventLoop localLoop(nExitCode);
    connect(this,SIGNAL(mainLoopQuit(int)),&localLoop,SLOT(mainLoopQuitSlot(int)));
    CEventloopTimerExitCode localWaitTimer;
    localWaitTimer.start(&localLoop, timeOut);

    m_nRunningLoopFromSignal ++;   // added by sungha.choi 2013.04.26
    ret = localLoop.exec();
    m_nRunningLoopFromSignal --;   // added by sungha.choi 2013.04.26
    if(m_nRunningLoopFromSignal == 0 && m_ReadyToDeleteFromSignal == false)  // added by sungha.choi 2013.04.26
    {
        m_ReadyToDeleteFromSignal = true;
    }

    localWaitTimer.stop();

    ////LOG_INFO << "CIPodDBFetchTaskHandler::waitForResponceFromExitCode: Loop exit,ret= " << ret << LOG_ENDL;
    if((ret == IPOD_ERROR_DISCONNECT) || m_isDisconnected){
        //LOG_TRACE << "CIPodDBFetchTaskHandler::waitForResponceFromExitCode: Loop exit with Error on disconnect" << LOG_ENDL;
        //You are right, thanks... This should not be unlock here.
        //if( m_iPodPrivateData ) m_iPodPrivateData->m_Mutex.unlock();
        throw  IPOD_ERROR_DISCONNECT;
    }
    ////LOG_TRACE << "CIPodDBFetchTaskHandler::waitForResponceFromExitCode Exit " << LOG_ENDL;
    return ret;//Return IPOD_SUCCESS or IPOD_ERROR_TIMEOUT
}

/******************************************************************************
// Function Name                                            Parameters
// CiTunesTagTaskHandler::packetAvailable                   (void) (OUT), (int, int, QByteArray) (IN)
//
// Explanation
// Slot to handle the command which is read by the raw packet handler
// check for the last command status and on the basis of the status
// takes next action
// handle the whole flow of the process
// History
// 02-Jan-2012      Sowmini Philip                        Added initial implementation
 *****************************************************************************/
#include <errno.h>
void CiTunesTagTaskHandler::packetAvailable(int lingoID,int commandID,const QByteArray& buf )
{
#ifdef DBUGLOGS
    //LOG_TRACE << "CiTunesTagTaskHandler::packetAvailable Entry" << LOG_ENDL;
#endif
    try {    // added by sungha.choi 2013.04.16
        if(lingoID == 0x0C) // for storage Lingo
        {
            switch(commandID)
            {
            //iPodACK
            case 0x00:
            {
                //LOG_TRACE << "CiTunesTagTaskHandler::iPodACK received" << LOG_ENDL;
                CIPodStorageLingoAck cmd( m_iPodPrivateData->m_isIDPS );
                cmd.unmarshall(buf);
                int ackCmd= cmd.getAcknowledgedCommand();
                switch(ackCmd)
                {
                case 0x01:
                {
                    //LOG_INFO << " Ack status sent for %x instead of response.Failed command" << ackCmd << LOG_ENDL;
                    m_CommandBuffer.clear();
                    emit mainLoopQuit(ExitCode_GetiPodCaps);
                    break;
                }
                case 0x10:
                {
                    //LOG_INFO << " Ack status sent for %x instead of response.Failed command" << ackCmd << LOG_ENDL;
                    m_CommandBuffer.clear();
                    emit mainLoopQuit(ExitCode_GetiPodFreeSpace);
                    break;
                }
                case 0x12:
                {
                    //LOG_INFO << "Ack status sent for %x instead of response.Failed command" << ackCmd << LOG_ENDL;
                    m_CommandBuffer.clear();
                    if(cmd.getAckStatus() == 3)
                    {
                        //LOG_TRACE << "Ack status sent for 0x12 instead of response.Failed command due to unavailable resources" << LOG_ENDL;
                    }
                    emit mainLoopQuit(ExitCode_OpeniPodFeatureFile);
                    break;
                }
                case 0x08:
                {
                    if(cmd.getAckStatus() == 0)
                    {
                        //LOG_TRACE << "CiTunesTagTaskHandler file handle closed successfully" << LOG_ENDL;
                    }
                    else
                        //LOG_TRACE << "CiTunesTagTaskHandler file handle failed to close" << LOG_ENDL;
                    m_FileHandleToBeClosed = false;
                    emit mainLoopQuit(ExitCode_CloseiPodFeatureFile);
                    break;
                }
                case 0x07:
                {
                    m_CommandBuffer.clear();
                    m_CommandBuffer = buf;
                    //LOG_TRACE << "CiTunesTagTaskHandler 0x07" << LOG_ENDL;
                    emit mainLoopQuit(ExitCode_WriteiPodFileData);
                    break;
                }
                }
                break;
            }
                //RetiPodCaps
            case 0x02:
            {
                m_CommandBuffer.clear();
                m_CommandBuffer = buf;
                //LOG_TRACE << "CiTunesTagTaskHandler 0x02" << LOG_ENDL;
                emit mainLoopQuit(ExitCode_GetiPodCaps);
                break;
            }
                //Retipodfreespace
            case 0x11:
            {
                m_CommandBuffer.clear();
                m_CommandBuffer = buf;
                //LOG_TRACE << "CiTunesTagTaskHandler 0x11" << LOG_ENDL;
                emit mainLoopQuit(ExitCode_GetiPodFreeSpace);
                break;
            }
                //retipodfilehandle //this is a responce to openipodfeaturefile
            case 0x04:
            {
                m_CommandBuffer.clear();
                m_CommandBuffer = buf;
                //LOG_TRACE << "CiTunesTagTaskHandler 0x041" << LOG_ENDL;
                emit mainLoopQuit(ExitCode_OpeniPodFeatureFile);
                break;
            }
            }
        }
#ifdef DBUGLOGS
        //LOG_TRACE << "CiTunesTagTaskHandler:: packetAvailable Exit" << LOG_ENDL;
#endif
    }   // end try
    catch(EIPodErrors ex){
        //LOG_TRACE << "CiTunesTagTaskHandler::PacketAvailable Execption Caught *******"<< LOG_ENDL;
    }
    return;
}

int CiTunesTagTaskHandler::EmitiTunesTaggingStatusE(QString plistPath)
{
    //LOG_TRACE << "CiTunesTagTaskHandler:: EmitiTunesTaggingStatus Entry" << LOG_ENDL;
    int result = iPodiTunes_NONE;
    result = exportTagFileE(plistPath);
    switch (result)
    {
    case iPodiTunes_MEMORYFULL:
    {
        //LOG_TRACE << "CiTunesTagTaskHandler:: Emiting Memory Ful" << LOG_ENDL;
        emit IPodPopUp(iPodiTunes_MEMORYFULL);
        break;
    }
    case iPodiTunes_FAILED:
    {
        //LOG_TRACE << "CiTunesTagTaskHandler:: Emitting Tagging Failed" << LOG_ENDL;
        emit IPodPopUp(iPodiTunes_FAILED);
        break;
    }
    case iPodiTunes_SUCCESS:
    default:
        break;
    }
    //LOG_TRACE << "CiTunesTagTaskHandler:: EmitiTunesTaggingStatus Exit result is " << result << LOG_ENDL;
    return result;
}
