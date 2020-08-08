/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                                                                *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 *21-Sept-2012  Pawan Gupta             Seperate the UID and index based handling
 ******************************************************************
 *
 ******************************************************************
 */
#include <DHAVN_IPodController_iPodDBFetchTaskHandler.h>
#include <DHAVN_IPodController_resetDBSelection.h>
#include <DHAVN_IPodController_retrieveCategorizedDatabaseRecords.h>
#include <DHAVN_IPodController_getNumberCategorizedDatabaseRecords.h>
#include <DHAVN_IPodController_returnNumberCategorizedDatabaseRecords.h>
#include <DHAVN_IPodController_returnCategorizedDatabaseRecords.h>
#include <DHAVN_IPodController_selectDBRecord.h>
#include <DHAVN_IPodController_ack.h>
#include <QDebug>
#include <DHAVN_IPodController_eventloopTimer.h>
#include <DHAVN_IPodController_requestLingoProtocolVersion.h>
#include <DHAVN_IPodController_returnLingoProtocolVersion.h>
#include <DHAVN_IPodController_requestiPodSerialNumber.h>
#include <DHAVN_IPodController_returniPodSerialNumber.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
#include <DHAVN_IPodController_playControl.h>
#include <DHAVN_IPodController_setShuffle.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <QDate>
#include <QTime>
#include <DHAVN_IPodController_setRepeat.h>
#include <DHAVN_IPodController_selectSortDBRecord.h>

#include "DHAVN_EventLoop.h"
#include "DHAVN_EventLoopTimerExitCode.h"


using namespace ExtendedLingo;
using namespace GeneralLingo;

#define NUM_DBTRACK_INFO 6
#define LISTFETCH_TIME 2000
CIPodDBFetchTaskHandler::CIPodDBFetchTaskHandler(QObject* parent,
                                                 iPodControllerPrivateData* privateData,
                                                 CIAPPacketWriter* iapWriter,
                                                 CIAPPacketReader* iapReader, bool connectToReader) :CTaskHandler(parent)
{
    switch(privateData->m_currentPortNum){
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
    listCount = 0;
    //m_iPodPrivateData->m_TransId = 0;
    m_isDisconnected = false;
    m_commandID = 0;
    m_iPodPrivateData = privateData;
    m_iAPWriter = iapWriter;
    m_iAPReader = iapReader;
    m_SerialNumber.clear();
    m_ExpectedCommandID =0;

    m_getListMap.clear();
    m_getListCount = 0;
    m_ReadyToDelete = true;    // added by sungha.choi 2013.04.12
    m_isRunningLoop = false;    // added by sungha.choi 2013.04.13
    m_ReadyToDeleteFromSignal = true;   // added by sungha.choi 2013.04.26
    m_nRunningLoopFromSignal = 0;   // added by sungha.choi 2013.04.26
    m_pLoop = NULL;
    m_pWaitTimer = NULL;

    m_CommandStatus = false;

    qDebug()<<"CIPodDBFetchTaskHandler: writer="<<iapWriter<<", Reader="<<iapReader;
    if(connectToReader)
    {
        connect(m_iAPReader,SIGNAL(packetAvailable(int, int, QByteArray)),this, SLOT(packetAvailable(int,int,QByteArray)));
        connect(this, SIGNAL(sendiAPPacket(QByteArray)) ,m_iAPWriter, SLOT(sendiAPPacket(QByteArray)));
        connect(&m_ListFetchtimer,SIGNAL(timeout()),this,SLOT(getListTimeOut()) );
        m_ListFetchtimer.setSingleShot(true);
    }
    setObjectName(giTunesTaskHandler);
}
CIPodDBFetchTaskHandler::~CIPodDBFetchTaskHandler()
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
    // { added by sungha.choi 2013.04.02
    m_getListMap.clear();
    // Modified by jonghwan.cho@lge.com 2013.05.06.
    //(&m_getListMap)->~QMap();
    // } added by sungha.choi 2013.04.02

   //LOG_TRACE << "CIPodDBFetchTaskHandler::~CIPodDBFetchTaskHandler " << LOG_ENDL;
}

void CIPodDBFetchTaskHandler::stopTaskHandler()
{
    //if ipod already disconneted, bellow method call may lead to crash.
    if(!m_isDisconnected)
    {
        disconnect(m_iAPReader,SIGNAL(packetAvailable(int, int, QByteArray)),this, SLOT(packetAvailable(int,int,QByteArray)));
        disconnect(m_iAPReader,SIGNAL(packetAvailableForDBTaskHandler(int,int,QByteArray)),this, SLOT(packetAvailable(int,int,QByteArray)));
        disconnect(this, SIGNAL(sendiAPPacket(QByteArray)) ,m_iAPWriter, SLOT(sendiAPPacket(QByteArray)));
        disconnect(&m_ListFetchtimer,SIGNAL(timeout()),this,SLOT(getListTimeOut()) );
    }
    m_isDisconnected = true;
}

void CIPodDBFetchTaskHandler::stopCurrentTask()
{   
    m_isDisconnected = true;
    // { deleted by sungha.choi 2013.04.02, this routine move to destructor
    //m_getListMap.clear();
    //(&m_getListMap)->~QMap();
    // } deleted by sungha.choi 2013.04.02

    //emit mainLoopQuit();    // modified by sungha.choi
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

   // //LOG_TRACE << "CIPodDBFetchTaskHandler::stopCurrentTask Exit " << LOG_ENDL;
}

bool CIPodDBFetchTaskHandler::isReadyToDelete()  // added by sungha.choi 2013.04.12
{
    //LOG_TRACE << " CIPodDBFetchTaskHandler:destroyTaskHandler1 : m_iPodPrivateData->m_isWaitingMutexUnlock: " << m_iPodPrivateData->m_isWaitingMutexUnlock << LOG_ENDL;
    //LOG_TRACE << " CIPodDBFetchTaskHandler:destroyTaskHandler1 : m_ReadyToDelete: " << m_ReadyToDelete << LOG_ENDL;
    //LOG_TRACE << " CIPodDBFetchTaskHandler:destroyTaskHandler1 : m_ReadyToDeleteFromSignal: " << m_ReadyToDeleteFromSignal << LOG_ENDL;
    if(m_iPodPrivateData->m_isWaitingMutexUnlock <= 0) // added by sungha.choi 2013.04.22
        return (m_ReadyToDelete && m_ReadyToDeleteFromSignal);    // modified by sungha.choi 2013.04.26
    else
        return false;   // mutex waiting
}

bool CIPodDBFetchTaskHandler::isWaitingForResponse()    // added by sungha.choi 2013.04.12
{
    return m_isRunningLoop;
}

int CIPodDBFetchTaskHandler::isWaitingForResponseFromSignal()    // added by sungha.choi 2013.04.26
{
    return m_nRunningLoopFromSignal;
}

void CIPodDBFetchTaskHandler::setMutexLock()    // added by sungha.choi 2013.04.22
{
    m_iPodPrivateData->m_isWaitingMutexUnlock ++;
    //LOG_TRACE << "CIPodDBFetchTaskHandler::setMutexLock IN count = " << m_iPodPrivateData->m_isWaitingMutexUnlock /*<< " Thread ID = " << QThread::currentThreadId()*/ << LOG_ENDL;
    m_iPodPrivateData->m_Mutex.lock();  // added by sungha.choi 2013.04.22
    m_iPodPrivateData->m_isWaitingMutexUnlock --;
    //LOG_TRACE << "CIPodDBFetchTaskHandler::setMutexLock OUT count = " << m_iPodPrivateData->m_isWaitingMutexUnlock /*<< " Thread ID = " << QThread::currentThreadId()*/ << LOG_ENDL;

    if(m_isDisconnected)
    {
        /* Tan : Coverity: if it has to be checked, might as well check it up there.
        if( m_iPodPrivateData )
            m_iPodPrivateData->m_Mutex.unlock();
        */
        m_iPodPrivateData->m_Mutex.unlock();
        throw  IPOD_ERROR_DISCONNECT;
    }
}

//added by Tan 2013.04.26.
//Fixing database for 1.13 device.
//this waitForResponceFromExitCode do not terminate on emit MainLoopQuit.

int CIPodDBFetchTaskHandler::waitForResponceFromExitCode(int timeOut, int nExitCode)
{
    #ifdef DBUGLOGS
    //LOG_TRACE << "CIPodDBFetchTaskHandler::waitForResponceFromSignal Entry " << LOG_ENDL;
    #endif
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
    localWaitTimer.start(&localLoop,timeOut);

    m_nRunningLoopFromSignal ++;   // added by sungha.choi 2013.04.26
    ret = localLoop.exec();
    m_nRunningLoopFromSignal --;   // added by sungha.choi 2013.04.26

    if(m_nRunningLoopFromSignal == 0 && m_ReadyToDeleteFromSignal == false)  // added by sungha.choi 2013.04.26
    {
        m_ReadyToDeleteFromSignal = true;
    }

    localWaitTimer.stop();
#ifdef DBUGLOGS
    ////LOG_INFO << "CIPodDBFetchTaskHandler::waitForResponceFromExitCode: Loop exit,ret= " << ret << LOG_ENDL;
#endif
    if((ret == IPOD_ERROR_DISCONNECT) || m_isDisconnected){
        //LOG_TRACE << "CIPodDBFetchTaskHandler::waitForResponceFromExitCode: Loop exit with Error on disconnect" << LOG_ENDL;
        //You are right, thanks... This should not be unlock here.
        //if( m_iPodPrivateData ) m_iPodPrivateData->m_Mutex.unlock();
        throw  IPOD_ERROR_DISCONNECT;
    }
    #ifdef DBUGLOGS
    ////LOG_TRACE << "CIPodDBFetchTaskHandler::waitForResponceFromExitCode Exit " << LOG_ENDL;
    #endif
    return ret;//Return IPOD_SUCCESS or IPOD_ERROR_TIMEOUT
}

/******************************************************************************
// Function Name                                            Parameters
// CPlayEngineTaskHandler::getList                           void (OUT), (QString,int,int) (IN)
//
// Explanation
// This method gets the list of items under a particular category
// Takes the category name and the index of the first item in the list to be retrieved.
// It also takes as input the offset(number of items from the start index)
// Category name be Artist, Album, Track, Playlist.
//
// History
// 09-Sep-2011      Sowmini Philip                        Added initial implementation
*****************************************************************************/
void CIPodDBFetchTaskHandler::getList(QString category,int startIndex, int offset)
{
#ifdef DBUGLOGS
    //LOG_TRACE << "CIPodDBFetchTaskHandler::getList Entry startIndex =  " <<startIndex << "offset = " <<offset<< LOG_ENDL;
#endif
    //qDebug("In CIPodDBFetchTaskHandler::getList(); \n");
    //int transId = 6;//m_iPodPrivateData->getTransactionId();
    CRetrieveCategorizedDatabaseRecords cmdRetrieve( m_iPodPrivateData->getTransactionId(),  m_iPodPrivateData->m_isIDPS);
    cmdRetrieve.setParameters(category,startIndex,offset);
    cmdRetrieve.marshall();
    m_ExpectedCommandID = 0x001B;
/*
    countSoFarMap.insert(transId,0);
    transactionIdAndListMap.clear();
    transactionIdAndCountMap.insert(transId,offset);
*/
//   send cmdNumber using iAPWriter
        //(parent()->findChild<CIAPPacketWriter*>(gWriter))->SendPacket(cmdRetrieve);

    m_getListMap.clear();
    m_getListCount = offset;
    emit sendiAPPacket(cmdRetrieve.getCommandBytes());

    if(m_ListFetchtimer.isActive())
    {
        m_ListFetchtimer.stop();
        m_ListFetchtimer.start(LISTFETCH_TIME); //timer for Listfetch
    }
    else
    {
        m_ListFetchtimer.start(LISTFETCH_TIME);
    }

        //qDebug("Command that will be sent: " << cmdRetrieve.getCommandBytes().toHex());
#ifdef DBUGLOGS
    //LOG_TRACE << "CIPodDBFetchTaskHandler::getList Exit " << LOG_ENDL;
#endif
}

/******************************************************************************
// Function Name                                            Parameters
// CPlayEngineTaskHandler::resetPreviousSelections          void (OUT), void (IN)
//
// Explanation
// This method resets all database selections made prior to its call.
//
//
// History
// 09-Sep-2011      Sowmini Philip                        Added initial implementation
*****************************************************************************/
void CIPodDBFetchTaskHandler::resetPreviousSelectionsE()
{
    //LOG_TRACE << "CIPodDBFetchTaskHandler::resetPreviousSelectionsE() IN" << LOG_ENDL;
    m_CommandStatus = false;
    //set a timer for 25seconds. If no response was recv in 25secs the eventloop shud be woken up.else app will hang.
    CResetDBSelection cmd( m_iPodPrivateData->getTransactionId(),  m_iPodPrivateData->m_isIDPS);//m_iPodPrivateData->getTransactionId());
    cmd.marshall();
    m_ExpectedCommandID = 0x01;
    emit sendiAPPacket(cmd.getCommandBytes());
    waitForResponceFromExitCode(7000, ExitCode_ResetDBSelection);
    //LOG_TRACE << "CIPodDBFetchTaskHandler::resetPreviousSelectionsE() OUT" << LOG_ENDL;
}


/******************************************************************************
// Function Name                                            Parameters
// CIPodDBFetchTaskHandler::makeSelection                    void (OUT), (QString,int) (IN)
//
// Explanation
// This method makes a selection on the apple device's db. Playing a track depends on the selection thus made.
// Takes as input the category and the corresponding index to be selected.
// Category name be Artist, Album, Track, Playlist.
//
// History
// 09-Sep-2011      Sowmini Philip                        Added initial implementation
*****************************************************************************/
bool CIPodDBFetchTaskHandler::makeSelectionE(QString category,int index)
{
    //LOG_TRACE << "CIPodDBFetchTaskHandler::makeSelectionE Entry with category = " << category << "And Index = "<< index << LOG_ENDL ;

    m_CommandStatus = false;
    // SelectDBRecord of category and index.
    //setMutexLock(); // added by sungha.choi 2013.04.22
    CSelectDBRecord cmd(category,index,m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
    cmd.marshall();
    //set a timer for 25seconds. If no response was recv in 25secs the eventloop shud be woken up.else app will hang.
    //send to writer
    //(parent()->findChild<CIAPPacketWriter*>(gWriter))->SendPacket(cmd);
    emit sendiAPPacket(cmd.getCommandBytes());
    waitForResponceFromExitCode(7000, ExitCode_SelectDBRecord);
    //LOG_TRACE << "CIPodDBFetchTaskHandler::makeSelectionE Exit " << LOG_ENDL ;
    return m_CommandStatus;
}

/******************************************************************************
// Function Name                                            Parameters
// CPlayEngineTaskHandler::getCategorizedListCount          void (OUT), QString (IN)
//
// Explanation
// This method gets the number of items under a category
// Plays the first track in the Song list if no track has been selected.Else plays the paused track.
// Category name be Artist, Album, Track, Playlist.
//
// History
// 09-Sep-2011      Sowmini Philip                        Added initial implementation
*****************************************************************************/
int CIPodDBFetchTaskHandler::getCategorizedListCountE(QString category)
{
    //qDebug() << "In CIPodDBFetchTaskHandler::getCategorizedListCount(); \n";
    int count = 0;
    m_CommandStatus = false;
    //set a timer for 25seconds. If no response was recv in 25secs the eventloop shud be woken up.else app will hang.

    //setMutexLock(); // added by sungha.choi 2013.04.22
    CGetNumberCategorizedDatabaseRecords cmdNumber(category,m_iPodPrivateData->getTransactionId(),  m_iPodPrivateData->m_isIDPS);
    cmdNumber.marshall();
    m_ExpectedCommandID = 0x0019;
    ////qDebug() << "Command that will be sent " << cmd.getCommandBytes().toHex() << "\n";
    //send cmdNumber using iAPWriter
    //(parent()->findChild<CIAPPacketWriter*>(gWriter))->SendPacket(cmdNumber);
    emit sendiAPPacket(cmdNumber.getCommandBytes());

    waitForResponceFromExitCode(7000, ExitCode_GetNumberCategorizedDatabaseRecords);
    count = listCount;
    // in the case of timeout
    //we dnt want the result of prev GetNumberCategorizedDatabaseRecords to be returned.
    //Thats why the value is set to 0 again.
    listCount =  0 ;

    return count;
}

double CIPodDBFetchTaskHandler::lingoProtocolVersion(int lingo)
{
    double lingoVersion = 0.0;
    //LOG_TRACE << "CIPodDBFetchTaskHandler::lingoProtocolVersion Entry " << LOG_ENDL;
    switch(lingo)
    {
    case 0x04:
            //LOG_TRACE << "lingo protocol version is :  " << m_iPodPrivateData->m_ExtendedLingoVersion << LOG_ENDL;
            lingoVersion = m_iPodPrivateData->m_ExtendedLingoVersion;
        break;
    default:
        break;
    }
    return lingoVersion;
}


QString CIPodDBFetchTaskHandler::getCurrentSerialNumberE()
{
    if(m_SerialNumber.isEmpty())
    {
            CRequestiPodSerialNumber cmd(m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
            cmd.marshall();
            m_ExpectedCommandID = 0x0C;
            emit sendiAPPacket(cmd.getCommandBytes());
            waitForResponceFromExitCode(7000, ExitCode_RequestiPodSerialNumber);
            if(m_CommandBuffer.length() > 0)
            {
                    CReturniPodSerialNumber cmd( m_iPodPrivateData->m_isIDPS);
                    cmd.unmarshall(m_CommandBuffer);
                    QString sSerialNumber = cmd.getSerialNumber();
                    if(!sSerialNumber.isEmpty())
                            m_SerialNumber = sSerialNumber;
            }
    }
    //LOG_INFO << " CIPodDBFetchTaskHandler::getCurrentSerialNumber no = "
    //                                << m_SerialNumber.toAscii().data() << LOG_ENDL;
    return m_SerialNumber;
}

bool CIPodDBFetchTaskHandler::pauseForTracker()
{
    m_CommandStatus = false;
    //set a timer for 25seconds. If no response was recv in 25secs the eventloop shud be woken up.else app will hang.
    CPlayControl playCmd("Pause", m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS ); //TODO place the rite trans id
    playCmd.marshall();
    emit sendiAPPacket(playCmd.getCommandBytes());
    return true;
}

bool CIPodDBFetchTaskHandler::setShuffleE(int mode)
{

    m_CommandStatus = false;
    CSetShuffle shuffleCmd(mode,m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS ); //TODO place the rite trans id
    shuffleCmd.marshall();
    emit sendiAPPacket(shuffleCmd.getCommandBytes());
    return true;
}

/******************************************************************************
// Function Name                                            Parameters
// CPlayEngineTaskHandler::packetAvailable                  void (OUT), (int,int,QByteArray) (IN)
//
// Explanation
// This method is a callback that is called when a command packet is available from the apple device.
// The task handler checks the lingoId and commandId input params and does the needful with the
// command bytes in the QByteArray
// History
// 09-Sep-2011      Sowmini Philip                        Added initial implementation
*****************************************************************************/
void CIPodDBFetchTaskHandler::packetAvailable(int lingoID,int commandID,const QByteArray& buf )
{
#ifdef DBUGLOGS
    //LOG_TRACE << "CIPodDBFetchTaskHandler packetAvailable lingoID = " <<
                 lingoID<< "- CommandId =  "<< commandID <<
                " - m_ExpectedCommandID =  " << m_ExpectedCommandID <<" Entry" << LOG_ENDL;
#endif
    try {    // added by sungha.choi 2013.04.16
        m_commandID = commandID;
        QString commandId;
        commandId.setNum(m_commandID,16);

        //TODO : instead of commandID -> should be MSB and LSB seperately sent
        //commandID shall be got after <<8 of MSB and | with LSB
        //if(lingoID == 0x04 && commandID == m_ExpectedCommandID )
        if(lingoID == 0x04)
        {
            //        QVariantMap namesOfCategoryContents = QVariantMap();
            switch(commandID)
            {
            case 0x0019: //ReturnNumberCategorizedDatabaseRecords
            {
                CReturnNumberCategorizedDatabaseRecords retCmd( m_iPodPrivateData->m_isIDPS);
                retCmd.unmarshall(buf);
                listCount =  retCmd.getCount();
                emit mainLoopQuit(ExitCode_GetNumberCategorizedDatabaseRecords);
                break;
            }// end of case 0x0019
            case 0x001B:
            {
                CReturnCategorizedDatabaseRecords cmd( m_iPodPrivateData->m_isIDPS);
                cmd.unmarshall(buf);
                int index = cmd.getRecordIndex();
                QString recString = cmd.getRecordString();
                //CR 13753: Empty Content appended to MT S, CR 15277 commented not neccessary
                //                if(recString.isEmpty())
                //                {
                //                    recString = gUnknownTitle;
                //                }
                //CR 13753: Empty Content appended to MT E
                m_getListMap.insert(index, recString);
                ////LOG_INFO << "CIPodDBFetchTaskHandler::packetAvailable : CReturnCategorizedDatabaseRecords index = " << index << LOG_ENDL;
                if(m_getListMap.count() == m_getListCount)
                {
                    if(m_ListFetchtimer.isActive())
                    {
                        m_ListFetchtimer.stop();  //timer for Listfetch
                        emit getListComplete(m_getListMap,m_getListCount);
                    }
                }
                break;
            }//end of case st
                //ACK = 0x01
            case 0x01:
            {
                CACK cmd( m_iPodPrivateData->m_isIDPS);
                cmd.unmarshall(buf);
                int ackCmd= cmd.getAcknowledgedCommand();
                //LOG_TRACE << "Ack-ing " << ackCmd<<",Status:"<<cmd.getAckStatus()<<LOG_ENDL;
                ////LOG_TRACE<<"m_Loop.isRunning="<<m_Loop.isRunning() <<LOG_ENDL;
                switch(ackCmd)
                {
                //0x0017 - selectDBRecord
                //Find the next category in the heirarchy
                case 0x0017:
                {
                    if(!cmd.getAckStatus())
                        m_CommandStatus = true;
                    emit mainLoopQuit(ExitCode_SelectDBRecord);
                    break;
                }
                    //0x0038 - selectSortDBRecord
                case 0x0038:
                {
                    if(!cmd.getAckStatus())
                        m_CommandStatus = true;
                    emit mainLoopQuit(ExitCode_SelectSortDBRecord);
                    break;
                }
                //0x0016 - resetDBSelection
                case 0x0016:
                {
                    m_CommandStatus = true;
                    emit mainLoopQuit(ExitCode_ResetDBSelection);
                    break;
                    //DBGLOG() << "reset status = " << !cmd.getAckStatus() << " " << m_CommandStatus;
                }
                // 0x0000 - cancelCommand
                case 0x0000:
                {
                    //TODO : check the transaction id - should match that of the cancel command
                    //countSoFar = 0;
                    //Remove CancelCommand command from Q
                    //if(!getListCommandsQueue.isEmpty())
                    //   nextCommand = getListCommandsQueue.dequeue();
                }
                default:
                    break;
                }
                break;
            }
            } //end of switch
        }
        else if(lingoID == 0x00 && commandID == m_ExpectedCommandID)
        {
            switch(commandID)
            {
		    //ReturnLingoProtocolVersion
		    case 0x0010:
            {
                m_CommandBuffer.clear();
                m_CommandBuffer = buf;
#ifdef DBUGLOGS
                //LOG_INFO << "CIPodDBFetchTaskHandler:: general lingo m_CommandBuffer.length= " << m_CommandBuffer.length() << LOG_ENDL;
#endif
                emit mainLoopQuit();
                break;
            }
            //ReturniPodSerialNumber
		    case 0x0C:
            {
                m_CommandBuffer.clear();
                m_CommandBuffer = buf;
                //LOG_TRACE << "CIPodDBFetchTaskHandler:: general 0x0c" << LOG_ENDL;
                emit mainLoopQuit(ExitCode_RequestiPodSerialNumber);
                break;
            }
		    case 0x02:
            {
                CACK cmd( m_iPodPrivateData->m_isIDPS);
                cmd.unmarshall(buf);
                int ackCmd= cmd.getAcknowledgedCommand();
                //DBGLOG() << "Ack-ing " << ackCmd;
                switch(ackCmd)
                {
                //ReturnProtocolVersion
                case 0x000F:
                {
                    m_CommandBuffer.clear();
                    emit mainLoopQuit();
                    break;
                }
                }
            }
            }
        }
        else
        {
#ifdef DBUGLOGS
            //LOG_TRACE << "iTunesDBTH Not my lingo...ignoring!" << LOG_ENDL;
#endif
        }
#ifdef DBUGLOGS
        //LOG_TRACE << "CIPodDBFetchTaskHandler packetAvailable lingoID" <<
                     lingoID<< "- CommandId "<< commandID << " Exit" << LOG_ENDL;
#endif
    }   // end try
    catch(EIPodErrors ex){
        //LOG_TRACE << "CIPodDBFetchTaskHandler::PacketAvailable Execption Caught *******"<< LOG_ENDL;
    }
    return;
}

void CIPodDBFetchTaskHandler::clearList()
{
//    //LOG_TRACE << "CIPodDBFetchTaskHandler::clearList() Entry" << LOG_ENDL;
    m_TrackList.clear();
//    //LOG_TRACE << "CIPodDBFetchTaskHandler::clearList() Exit" << LOG_ENDL;
}
void CIPodDBFetchTaskHandler::getListTimeOut()
{
    //LOG_TRACE << "CIPodDBFetchTaskHandler::getListTimeOut() Entry" << LOG_ENDL;
    if(m_ListFetchtimer.isActive())
    {
        m_ListFetchtimer.stop();

//        QVariantMap namesOfCategoryContents = QVariantMap();
        QMap<int,QString> namesOfCategoryContents;
        namesOfCategoryContents.clear();
        emit getListComplete(namesOfCategoryContents,0);
    }
    //LOG_TRACE << "CIPodDBFetchTaskHandler::getListTimeOut() Exit" << LOG_ENDL;
}

//getTotalCountOfiPodTracksE should be called one time
int CIPodDBFetchTaskHandler::getTotalCountOfiPodTracksE()
{
    int dTrackCnt = 0;
    resetPreviousSelectionsE();
    dTrackCnt = getCategorizedListCountE("Track");
    //LOG_INFO<<"CIPodDBFetchTaskHandler::CGetNumberCategorizedDatabaseRecords = "<< dTrackCnt << LOG_ENDL;
    return dTrackCnt;
}

bool CIPodDBFetchTaskHandler::setRepeatE(int mode)
{
    // Added for CR 13659, 13646
    CSetRepeat repeatCmd(mode,m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS ); //TODO place the rite trans id
    repeatCmd.marshall();
    emit sendiAPPacket(repeatCmd.getCommandBytes());
    return m_CommandStatus;

}

bool CIPodDBFetchTaskHandler::makeSelectionSortedOnArtistE(QString category,int index, QString sortType)
{
    /* Added for CR 13678 */
    m_CommandStatus = false;
    //setMutexLock(); // added by sungha.choi 2013.04.22
    CSelectSortDBRecord cmd(category,index,sortType,m_iPodPrivateData->getTransactionId(), m_iPodPrivateData->m_isIDPS);
    cmd.marshall();
    m_ExpectedCommandID = 0x01;
    emit sendiAPPacket(cmd.getCommandBytes());
    waitForResponceFromExitCode(7000,ExitCode_SelectSortDBRecord);
    return m_CommandStatus;
}
