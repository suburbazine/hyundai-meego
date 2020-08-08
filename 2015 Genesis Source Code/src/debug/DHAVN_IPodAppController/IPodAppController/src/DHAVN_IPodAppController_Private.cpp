/*
 ******************************************************************
 *        COPYRIGHT © Teleca AB                                   *
 *----------------------------------------------------------------*
 * MODULE     :  IPodAppController                                *
 *
 * PROGRAMMERS : Pawan Gupta                                      *
 * DATE       :  16 jan 2012                                      *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :

 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include "DHAVN_IPodAppController_Private.h"
#include <DHAVN_IPodAppController_hidRawPacketHandler.h>
#include <DHAVN_IPodAppController_IAPPacketReader.h>
#include <DHAVN_IPodAppController_IAPPacketWriter.h>
#include <DHAVN_IPodAppController_DevDataTransfer.h>
#include <DHAVN_IPodAppController_ACK.h>
#include <DHAVN_IPodAppController_global.h>
#include <DHAVN_IPodAppController_iPodDataTransfer.h>
#include <DHAVN_IPodAppController_DevAck.h>
#include <DHAVN_IPodAppController_RequestAppLaunch.h>

//#include <QFile>
//Added for CR 16819: Fix for ATS
qint16 CIPodAppControllerPriv::m_TransId = 1000;

using namespace GeneralLingo;

/******************************************************************************
// Function                                            Parameters
// CIPodAppControllerPriv::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
CIPodAppControllerPriv::CIPodAppControllerPriv(QObject* parant) : QObject(parant), m_TransactionId(0),m_IsInitialized(false)
{
    LOG_INIT_V2(gIPodAppControllerLogs);
    m_ResendStruct.count=0;
    m_Handle.m_DeviceNumber = -1;
    m_Handle.m_ProtocolIndex = -1; // added by Tan 2013.01.05
    m_ResendStruct.m_Data.clear();
    m_DBusInterface = new OrgLgeDbusTestInterface("org.lge.dbusTest","/", QDBusConnection::sessionBus(),0);
    if(m_DBusInterface->isValid())
    {
        //LOG_TRACE << "connection to dbus is valid"<< LOG_ENDL;
        connect(m_DBusInterface, SIGNAL(IPodDisconnected(QString)), this, SLOT(OnIPodDisconnected(QString)));
        connect(m_DBusInterface, SIGNAL(IpodAppSessionClosed(QString,QString)), this, SLOT(OnIpodAppSessionClosed(QString,QString)));
    }
    else
    {
        //LOG_TRACE << "connection to dbus is not valid"<< LOG_ENDL;
    }

    // { added by jungae 2012.11.10, Static_Test 13236
        m_RawPacketHandler = NULL;
        m_IAPWriter = NULL;
        m_IAPReader = NULL;
        m_RawPacketThread = NULL;
        m_SendCommandTimeOutWatcher = NULL;
        m_ReqTransportTransactId = 0;
   // } added by jungae

}

/******************************************************************************
// Function                                            Parameters
// CIPodAppControllerPriv::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
CIPodAppControllerPriv::~CIPodAppControllerPriv()
{
    //LOG_TRACE << "CIPodAppControllerPriv::~CIPodAppControllerPriv() IN"<< LOG_ENDL;
    if( m_IsInitialized )
    {
        Deinitialize();
    }
    delete m_DBusInterface;
    //LOG_TRACE << "CIPodAppControllerPriv::~CIPodAppControllerPriv() OUT"<< LOG_ENDL;
}

/******************************************************************************
// Function                                            Parameters
// CIPodAppControllerPriv::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
EIPodAppErrors CIPodAppControllerPriv::Init(const QString& sAppId, const QString& BundleSeedId, 
								int ipodNumber, int protocolIndex) // modified by Tan 2013.01.05
{
    //LOG_TRACE << "CIPodAppControllerPriv::Init() IN"<< LOG_ENDL;
    QString AppId("");
    AppId = sAppId + DOT + QString::number(ipodNumber);	
    switch((EIPodPorts)ipodNumber)
    {
	    case eIPOD_PORT_1:
	    case eIPOD_PORT_2:
		    break;
	    default:
		    AppId = sAppId;
    }

    if(!m_Handle.m_AppId.compare(AppId) && !m_Handle.m_SeedId.compare(BundleSeedId))
        return IPOD_APPERROR_ALREADYINITIALIZED;
    m_IsInitialized = true;
    //reset vars
    m_Handle.m_AppId.append(AppId);
    m_Handle.m_SeedId.append(BundleSeedId);
    m_Handle.m_DeviceNumber = ipodNumber;
    m_Handle.m_MaxPayloadSize=0;
    m_Handle.m_SessionID=0;
    m_Handle.m_ProtocolIndex = protocolIndex; // added by Tan 2013.01.05

    //reset members
    m_SendCommandTimeOutWatcher = NULL;
    m_RawPacketHandler = NULL;
    m_IAPWriter = NULL;
    m_IAPReader = NULL;
    m_SendCommandTimeOutWatcher = NULL;

    //initialize handlers
    m_RawPacketHandler = new CHIDRawPacketHandle(QSocketNotifier::Read);
    m_IAPReader = new CIAPPacketReader(this);
    m_IAPWriter = new CIAPPacketWriter(this);

    //open the ipod HID device
    //LOG_TRACE << "the ipod number passed is :" << ipodNumber << LOG_ENDL;
    QString devNumber;
    devNumber.setNum(ipodNumber);

    QString path;
    path.append(GetHidPathFromFile());

    EIPodAppErrors res = m_RawPacketHandler->hid_open(path);

    m_RawPacketThread = new QThread();
    if(m_RawPacketThread)
    {
        //LOG_TRACE <<"Thread successfully created"<<LOG_ENDL;
        //printf("Thread successfully created\n");
        connect(m_RawPacketThread, SIGNAL(finished()), m_RawPacketHandler, SLOT(deleteLater()));
        connect(m_RawPacketThread, SIGNAL(finished()), m_RawPacketThread, SLOT(deleteLater()));
        m_RawPacketHandler->moveToThread(m_RawPacketThread);
        m_RawPacketThread->start();
    }
    else
    {
        //LOG_TRACE <<"Thread creating failed"<<LOG_ENDL;
        //printf("Thread creating failed\n");
    }

    //connect on signals from reader
    connect(m_IAPReader, SIGNAL(packetAvailable(int, int, QByteArray&)), this, SLOT(packetAvailable(int, int, QByteArray&)));
    connect(m_IAPReader, SIGNAL(largePacketAvailable(int, int, QByteArray&)), this, SLOT(largePacketAvailable(int, int, QByteArray&)));


    m_TransId++; //Added for CR 16819: Fix for ATS

    //Tan: CR_InitComplete_1
    //initialize it to whatever number it can never have.
    m_ReqTransportTransactId = -1 ; // added by Tan 2012.12.15
	
    //request for the size limit
    if(res == IPOD_APPSUCCESS)
    {
        res = RequestTransPortMaxPayLoadSize();
        //create the resend logic timer
        m_SendCommandTimeOutWatcher = new QTimer(this);
        connect(m_SendCommandTimeOutWatcher, SIGNAL(timeout()), this, SLOT(SendCommandTimerExpired()));
        m_SendCommandTimeOutWatcher->setInterval(TimeToWaitForSendCommand);
    }
    else
    {
        Deinitialize();
    }
    //LOG_TRACE << "CIPodAppControllerPriv::Init() OUT"<< LOG_ENDL;

    return res;
}

/******************************************************************************
// Function                                            Parameters
// CIPodAppControllerPriv::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
EIPodAppErrors CIPodAppControllerPriv::D_Init()
{
    //LOG_TRACE << "CIPodAppControllerPriv::D_Init IN"<< LOG_ENDL;
    Deinitialize();
    //LOG_TRACE << "CIPodAppControllerPriv::D_Init() OUT"<< LOG_ENDL;
    return IPOD_APPSUCCESS;
}

/******************************************************************************
// Function                                            Parameters
// CIPodAppControllerPriv::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
EIPodAppErrors CIPodAppControllerPriv::Send(const QByteArray& dataArray)
{
    //LOG_TRACE << "CIPodAppControllerPriv::Send IN"<< LOG_ENDL;
    EIPodAppErrors res = IPOD_APPERROR_GENERIC;

    //get session ID
    qint16 sessionId = GetSessionId();

    if(sessionId <= 0)
    {
        res = IPOD_APPERROR_APPNOTOPENED;
    }
    else if(m_SendCommandTimeOutWatcher->isActive()) //previous command is in progress
    {
        res = IPOD_APPERROR_BUSY;
    }
    else
    {
        //m_TransactionId++;
        //Added for CR 16819: Fix for ATS S
        qint16 transid = getTransactionId();
        CDevDataTransfer command(dataArray, sessionId, transid/*m_TransactionId*/, this);
        //Added for CR 16819: Fix for ATS E
        command.marshall();
        int byteWriteen = m_IAPWriter->SendPacket(command);
        if(byteWriteen > 0)
            res = IPOD_APPSUCCESS;

        //By hanhong.keum@lge.com        
        //IPodAppController need to resend the Pandora packet,
        //because the packet will be resent on application level.
        switch(dataArray[0])
        {
        case 0x7E:  // 0x7E : PNDR_FRAME_START
            break;
        default:
            //start the resend timer except for Pandora packets.
            //StartResendLogic(command.getCommandBytes());
            break;
        }
    }
    //LOG_TRACE << "CIPodAppControllerPriv::Send OUT"<< LOG_ENDL;
    return res;
}

/******************************************************************************
// Function                                            Parameters
// CIPodAppControllerPriv::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
EIPodAppErrors CIPodAppControllerPriv::RequestTransPortMaxPayLoadSize()
{
    EIPodAppErrors res = IPOD_APPERROR_GENERIC;
    //Added for CR 16819: Fix for ATS S
    //m_TransactionId++;
    qint16 transid = getTransactionId();
    //Added for CR 16819: Fix for ATS E
    int i=-1;
    QByteArray command;
    command[++i] = 0x55;
    command[++i] = 0x04;
    command[++i] = 0x00;
    command[++i] = 0x11;
    //Added for CR 16819: Fix for ATS S
//    command[++i] = (m_TransactionId & 0xff00 >> 8);
//    command[++i] =  m_TransactionId & 0x00ff;
// { modified by Tan 2012.12.15
    //Tan: here is a bug, shift operator has higher precendence
    //than the bitwise operator!!!
    //command[++i] = (transid & 0xff00 >> 8);
	command[++i] = (transid & 0xff00) >> 8;
// } modified by Tan
    command[++i] =  transid & 0x00ff;
    //Added for CR 16819: Fix for ATS E
    command[i+1] = CIpodAppControllerGlobal::checkSum(command, i);
    CIAPPacket packet(command);
    int byteWriteen = m_IAPWriter->SendPacket(packet);
    if(byteWriteen > 0)
    {
        //Tan: CR_InitComplete_1
        m_ReqTransportTransactId = transid; // added by Tan 2012.12.15
        res = IPOD_APPSUCCESS;
    }
    return res;
}

/******************************************************************************
// Function                                            Parameters
// CIPodAppControllerPriv::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
//Tan: CR_InitComplete_1
//void CIPodAppControllerPriv::SetPayloadMaxSize(const QByteArray& data)
int CIPodAppControllerPriv::SetPayloadMaxSize(const QByteArray& data)
{
    //Tan: CR_InitComplete_1
    int nTransactionID; // added by Tan 2012.12.15
    //LOG_TRACE << "CIPodAppControllerPriv::SetPayloadMaxSize() IN"<< LOG_ENDL;
    qint16 size = 0;
    if( ((int)data[0] == 0x55) &&
            ((int)data[1] == 0x06) &&
            ((int)data[3] == 0x12) )
    {
        for(int i = 6;i<= 7;i++)
        {
            size <<= 8;     size |= (int)data[i] & 0xFF;
        }
    }
    // { added by Tan 2012.12.15
    nTransactionID = ((int)data[4] & 0xFF) << 8;
    nTransactionID = nTransactionID | ((int)data[5] & 0xFF);
    // } added by Tan

    //LOG_TRACE << "max data size to be written to ipod app is: " <<  size << LOG_ENDL;

    m_Handle.m_MaxPayloadSize = size;
    //LOG_TRACE << "CIPodAppControllerPriv::SetPayloadMaxSize() OUT"<< LOG_ENDL;
	
	return nTransactionID; // added by Tan 2012.12.15
}

/******************************************************************************
// Function                                            Parameters
// CIPodAppControllerPriv::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIPodAppControllerPriv::Deinitialize()
{
    //LOG_TRACE << "CIPodAppControllerPriv::Deinitialize() IN"<< LOG_ENDL;

    //printf("------------>Deinitiliaze\n");
    m_IsInitialized = false;

    //raw packer handler
    if(m_RawPacketThread) //if it was
    {
        //LOG_TRACE<< "Quitting Thread"<< LOG_ENDL;
        //printf("Quitting thread.\n");
        m_RawPacketThread->quit();
        m_RawPacketThread->wait(3000);
        //printf("Thread quited or 3 seconds past\n");
        //LOG_TRACE<< "Thread quited or 3 seconds past"<< LOG_ENDL;
        if(!m_RawPacketThread->isFinished())//if it is not quited.
        {
            //printf("Quitting thread failed.\n");
            //LOG_TRACE << "Thread not finished after 3 secs" << LOG_ENDL;
        }
        m_RawPacketHandler = NULL; //deletion will be done by delete later.
        m_RawPacketThread = NULL;//deletion will be done by delete later.
    }
    else
    {
        if(m_RawPacketHandler)
        {
            //printf("Thread was never created\n");
            //LOG_TRACE << "Thread was never created" << LOG_ENDL;
            delete m_RawPacketHandler;
            m_RawPacketHandler = NULL;
        }
    }


    //reader
    delete m_IAPReader;
    m_IAPReader=NULL;

    //writer
    delete m_IAPWriter;
    m_IAPWriter=NULL;

    //resend timer
    delete m_SendCommandTimeOutWatcher;
    m_SendCommandTimeOutWatcher = NULL;

    //clear the handle
    m_Handle.m_AppId.clear();
    m_Handle.m_MaxPayloadSize=0;
    m_Handle.m_SeedId.clear();
    m_Handle.m_SessionID=0;
    m_Handle.m_DeviceNumber = -1;
    m_Handle.m_ProtocolIndex = -1; // added by Tan 2013.01.05
    //LOG_TRACE << "CIPodAppControllerPriv::Deinitialize() out"<< LOG_ENDL;
}

/******************************************************************************
// Function                                            Parameters
// CIPodAppControllerPriv::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CIPodAppControllerPriv::GetSessionId()
{
    //LOG_TRACE << "CIPodAppControllerPriv::GetSessionId() IN"<< LOG_ENDL;
    if(m_Handle.m_SessionID > 0)
        return m_Handle.m_SessionID;

    //get the session id from the path
    //where IPodController has written it
    qint16 sessionId = 0;
//    if(m_DBusInterface->isValid())
//    {
//        sessionId = m_DBusInterface->GetApplicationSessionID(m_Handle.m_AppId);
//    }
//    else
//    {
        QString path(gIPodAppSessionIdPath);
        path.append(m_Handle.m_AppId);
        QFile file(path);
        //LOG_TRACE << "path to be opened: " <<  path.toAscii().data() <<LOG_ENDL;
        if(!file.open(QIODevice::ReadOnly))
        {
            //LOG_TRACE << "no session ID found"<< LOG_ENDL;
            return 0;
        }
        QTextStream in(&file);
        QString sessionIdstr;
        in >> sessionIdstr;
        file.close();
        sessionId = sessionIdstr.toInt();
//    }

    m_Handle.m_SessionID = sessionId;
    //LOG_TRACE << "Session id for App: " << m_Handle.m_AppId.toAscii().data() << "is " << sessionId << LOG_ENDL;
    //LOG_TRACE << "CIPodAppControllerPriv::GetSessionId() OUT"<< LOG_ENDL;
    return sessionId;
}

/******************************************************************************
// Function                                            Parameters
// CIPodAppControllerPriv::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIPodAppControllerPriv::EmitSessionClosedSignal(EIPodAppErrors error)
{
    //LOG_TRACE << "CIPodAppControllerPriv::EmitSessionClosedSignal() IN"<< LOG_ENDL;
    m_Handle.m_SessionID=0;
    emit SessionClosed(error);
    //LOG_TRACE << "CIPodAppControllerPriv::EmitSessionClosedSignal() out"<< LOG_ENDL;
}

/******************************************************************************
// Function                                            Parameters
// CIPodAppControllerPriv::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIPodAppControllerPriv::SendCommandTimerExpired()
{
    //By hanhong.keum@lge.com
    //To check the data being present in SResendStructure
    //since Pandora does not re-send messages in IPodAppController
    if(m_ResendStruct.m_Data.size() == 0)
    {
        return;
    }

    //LOG_TRACE << "CIPodAppControllerPriv::SendCommandTimerExpired() IN"<< LOG_ENDL;
    //stop timer
    m_SendCommandTimeOutWatcher->stop();
    //check if it reached to max trial
    if(m_ResendStruct.count == MAXSENDCOMMANDTRAIL)
    {
        //clear the resend logic and
        //acknoledge with error
        ClearResndLogic();
        emit Acknowledgement(IPOD_APPERROR_TIMEOUT);
        EmitSessionClosedSignal(IPOD_APPERROR_TIMEOUT);
        return;
    }
    //build the resnd logic
    QByteArray arrTemp(m_ResendStruct.m_Data);
    StartResendLogic(arrTemp);
    //LOG_TRACE << "CIPodAppControllerPriv::SendCommandTimerExpired() OUT"<< LOG_ENDL;
}

/******************************************************************************
// Function                                            Parameters
// CIPodAppControllerPriv::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIPodAppControllerPriv::OnIPodDisconnected(const QString& ipodNumber)
{
    //LOG_TRACE << "CIPodAppControllerPriv::OnIPodDisconnected() IN"<< LOG_ENDL;
    int Number = ipodNumber.toInt();
    if(m_Handle.m_DeviceNumber == Number)
    {
        Deinitialize();
        emit DeviceDisconnected(Number);
    }
    //LOG_TRACE << "CIPodAppControllerPriv::OnIPodDisconnected() OUT"<< LOG_ENDL;
}

/******************************************************************************
// Function                                            Parameters
// CIPodAppControllerPriv::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIPodAppControllerPriv::OnIpodAppSessionClosed(const QString& ipodNumber, const QString& appId)
{
    //LOG_TRACE << "CIPodAppControllerPriv::OnIpodAppSessionClosed() IN"<< LOG_ENDL;
    if(!appId.compare(m_Handle.m_AppId) && (m_Handle.m_DeviceNumber == ipodNumber.toInt()))
    {
        Deinitialize();
        emit SessionClosed(IPOD_APPERROR_SESSIONCLOSED);
    }
    //LOG_TRACE << "CIPodAppControllerPriv::OnIpodAppSessionClosed() OUT"<< LOG_ENDL;
}

/******************************************************************************
// Function                                            Parameters
// CIPodAppControllerPriv::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIPodAppControllerPriv::StartResendLogic(const QByteArray& data)
{
    //LOG_TRACE << "CIPodAppControllerPriv::StartResendLogic() IN"<< LOG_ENDL;
    m_ResendStruct.count++;
    //LOG_TRACE << "resend count is :" << m_ResendStruct.count << LOG_ENDL;
    m_ResendStruct.m_Data.clear();
    m_ResendStruct.m_Data.append(data);
    m_SendCommandTimeOutWatcher->start();
    //LOG_TRACE << "CIPodAppControllerPriv::StartResendLogic() OUT"<< LOG_ENDL;
}

/******************************************************************************
// Function                                            Parameters
// CIPodAppControllerPriv::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIPodAppControllerPriv::ClearResndLogic()
{
    //LOG_TRACE << "CIPodAppControllerPriv::ClearResndLogic() IN"<< LOG_ENDL;
    m_ResendStruct.count=-1;
    m_ResendStruct.m_Data.clear();
    m_SendCommandTimeOutWatcher->stop();
    //LOG_TRACE << "CIPodAppControllerPriv::ClearResndLogic() OUT"<< LOG_ENDL;
}

void CIPodAppControllerPriv::AcknowledgeIpodDataTransferCommenad(const QByteArray& dataarray)
{
    //LOG_TRACE << "CIPodAppControllerPriv::AcknowledgeIpodDataTransferCommenad() IN"<< LOG_ENDL;
    CIPodDataTransfer cmd1(GetSessionId());
    cmd1.unmarshall(dataarray);
    CDevAck cmd(cmd1.getTransactionId(), 67);
    cmd.marshall();
    int byteWriteen = m_IAPWriter->SendPacket(cmd);
    //if(byteWriteen <=0 )
    //LOG_TRACE << "CDevAck command Failed for the IPOD Data Transfer"<< LOG_ENDL;

    //LOG_TRACE << "PayloadLen: " << cmd1.getPayloadLen() << LOG_ENDL;
    emit DataReceived(cmd1.getData().mid(0, cmd1.getPayloadLen()));
    //LOG_TRACE << "CIPodAppControllerPriv::AcknowledgeIpodDataTransferCommenad() OUT"<< LOG_ENDL;
}

QString CIPodAppControllerPriv::GetHidPathFromFile()
{
	QString sIpodHidDevicePath("");
	switch((EIPodPorts)(m_Handle.m_DeviceNumber))
	{
	case eIPOD_PORT_1:
		sIpodHidDevicePath = gIPodHidDevicePathPort1;
		break;
	case eIPOD_PORT_2:
		sIpodHidDevicePath = gIPodHidDevicePathPort2;
		break;
	default:
		sIpodHidDevicePath = gIPodHidDevicePath;
	}
		
    QFile file(sIpodHidDevicePath);
    //LOG_TRACE << "path to be opened for dev path: "<< sIpodHidDevicePath.toAscii().data() <<LOG_ENDL;
    if(!file.open(QIODevice::ReadOnly))
    {
        return QString();
    }
    QTextStream in(&file);
    QString devPath;
    in >> devPath;
    file.close();
    return devPath;
}
/******************************************************************************
// Function                                            Parameters
// CIPodAppControllerPriv::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIPodAppControllerPriv::packetAvailable(int lingoId, int commandId, QByteArray& dataArray)
{
    //LOG_TRACE << "CIPodAppControllerPriv::packetAvailable() IN"<< LOG_ENDL;
    //LOG_TRACE << "lingo ID: " << lingoId <<", command ID: " << commandId << LOG_ENDL;
    if(lingoId == 0x00) // for general Lingo
    {
        switch(commandId)
        {
        case 0x02: //ACK = 0x02
        {
            CACK ack;
            ack.unmarshall(dataArray);
            int status = ack.getAckStatus();
            int command = ack.getAcknowledgedCommand();
            switch (command)
            {
            case 0x42:
            {
                switch(status)
                {
                case 0x00:
                {
                    //LOG_TRACE << "ACK command is success for 0x42"<< LOG_ENDL;
                    ClearResndLogic();
                    emit Acknowledgement(IPOD_APPSUCCESS);
                }
                    break;
                case 0x01:
                {
//                    //LOG_TRACE << "ACK command is failed for 0x42 with status 0x01"<< LOG_ENDL;
                    //LOG_CRITICAL<<" Error Lingo  : 0x00 , cmd : 0x02 , ackcmd : 0x42 , status : 0x01 dataarray :  " <<dataArray <<LOG_ENDL;
//                    ClearResndLogic();
//                    EmitSessionClosedSignal(IPOD_APPERROR_SESSIONCLOSED);
//                    emit Acknowledgement(IPOD_APPERROR_SESSIONCLOSED);
                }
                    break;
                case 0x17:
                {
                    // Added log.
                    //LOG_TRACE << "ACK SessionWriteFailed for 0x42 with status 0x17"<< LOG_ENDL;
                    SendCommandTimerExpired();
                }
                    break;
                }
            }
                break;
            case 0x64: //app launch
            {
                //LOG_TRACE << "0x64 command ack has came"<< LOG_ENDL;
                ClearResndLogic();
                if(status == 0x00)
                {
                    //LOG_TRACE << "ACK command is passsed for 0x64"<< LOG_ENDL;
                }
                else if(status == 0x04)
                {
                    //LOG_TRACE << "ACK command is bas param for 0x64"<< LOG_ENDL;
                    RequestForAppLaunch(m_Handle.m_BundleID, true);
                }
                else
                {
                    //LOG_TRACE << "ACK command is failed for 0x64"<< LOG_ENDL;
                    emit Acknowledgement(IPOD_APPERROR_APPNOTOPENED);
                }
            }
                break;
            }
        }
            break;
        case 0x4A:
        {
            //LOG_TRACE << "0x4A command ipod notification has come pawan"<< LOG_ENDL;
        }
            break;
        case 0x12: //ReturnTransPortMaxPayLoadSize
        {
            //Tan: CR_InitComplete_1, should only emit InitComplete when I am the caller.
			// { modified by Tan 2012.12.15
            //SetPayloadMaxSize(dataArray);
            //emit InitComplete();
            if( SetPayloadMaxSize(dataArray) == m_ReqTransportTransactId )           
            {
                m_ReqTransportTransactId = -1;
                emit InitComplete();
            } // } modified by Tan
        }
            break;
        case 0x3f:
        {
            //LOG_TRACE << "0x3f command has came"<< LOG_ENDL;
            int protocolindex = dataArray[8];
			// added by Tan 2013.01.05
            int nSessionID;

            nSessionID = ((int)dataArray[6] & 0xFF) << 8;
            nSessionID = nSessionID | ((int)dataArray[7] & 0xFF);

            //check if the protocol index returned is for this application
            //if yes, we now have a valid session id, let us save it so that
            //we do not have to read from the file
            if(protocolindex == m_Handle.m_ProtocolIndex)
            {
                m_Handle.m_SessionID = nSessionID;
            }
			// } added by Tan
            emit SessionOpened(protocolindex); // added by Tan 2012.12.15
            if(0x01 == protocolindex) // 0x01 - for pandora app agreed with IPodController    
            {
                //LOG_TRACE << "ACK command sending ot the launch app as the session is opened"<< LOG_ENDL;
                //LOG_TRACE << "sleeping for one second...................."<< LOG_ENDL;
                sleep(1); // to be in sync with the IPodController
                emit Acknowledgement(IPOD_APPSUCCESS);
            }
        }
            break;
        case 0x40:
        {
            //LOG_TRACE << "0x40 command has came"<< LOG_ENDL;
//            EmitSessionClosedSignal(IPOD_APPERROR_SESSIONCLOSED);
			// { added by Tan, 2013.01.05
            int nSessionID;
            //when you receive the 0x40, ipod told you that the session has been closed.
            //it only makes sense if  you notify the parent process (pandora/aha)
            nSessionID = ((int)dataArray[6] & 0xFF) << 8;
            nSessionID = nSessionID | ((int)dataArray[7] & 0xFF);
            if(nSessionID == m_Handle.m_SessionID)
            {
                m_Handle.m_SessionID = 0;
                emit NotifySessionClosed();
            }
			// } added by Tan
        }
            break;
        case 0x43:
        {
            //LOG_TRACE << "0x43 command has came"<< LOG_ENDL;
            AcknowledgeIpodDataTransferCommenad(dataArray);
        }
            break;
        }
        //LOG_TRACE << "CIPodAppControllerPriv::packetAvailable() OUT"<< LOG_ENDL;
    }
    else if(lingoId == 0x0A) //digital audio attributes
    {
        switch(commandId)
        {
        case 0x04:
            int isTidEnable = 1;
            int nSampleRate;
            int nTransactionID;
            QByteArray buf = dataArray;
            if( ((int)buf[0] == 0x55) &&
                ((int)buf[2] == 0x0A) &&
                ((int)buf[3] == 0x04) )
            {
                int i = 4;
                if(isTidEnable)
                {
                    nTransactionID = 0;
                    nTransactionID |= (int)buf[i++] & 0xFF;
                    nTransactionID <<= 8;
                    nTransactionID |= (int)buf[i++] & 0xFF;
                }
                nSampleRate = 0 | (0x000000FF & buf[i])<<24 | ( 0x000000FF & buf[i+1])<<16 | ( 0x000000FF & buf[i+2])<<8 | ( 0x000000FF & buf[i+3]);
                //LOG_TRACE<<"nSampleRate:" << nSampleRate << LOG_ENDL;
                emit AudioSampleRateChanged(nSampleRate);
            }
        }
    }
}

/******************************************************************************
// Function                                            Parameters
// CIPodAppControllerPriv::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIPodAppControllerPriv::largePacketAvailable(int lingoId, int commandId, QByteArray& dataArray)
{
    //LOG_TRACE << "CIPodAppControllerPriv::largePacketAvailable() IN"<< LOG_ENDL;
    //LOG_TRACE << "lingo ID: " <<  lingoId << "  command ID: " << commandId << LOG_ENDL;
    if(lingoId == 0x00) // for general Lingo
    {
        switch(commandId)
        {
        case 0x43:
        {
            //m_TransactionId++;
            CIPodDataTransfer cmd1(GetSessionId());
            cmd1.unmarshall(dataArray);
            CDevAck cmd(cmd1.getTransactionId(), 67);
            cmd.marshall();

            int byteWriteen = m_IAPWriter->SendPacket(cmd);
            if(byteWriteen <=0 )
            {
                //LOG_TRACE << "CDevAck command Failed for the IPOD Data Transfer"<< LOG_ENDL;
            }

            //emit DataReceived(cmd1.getData());
            //LOG_TRACE << "PayloadLen: " << cmd1.getPayloadLen() << LOG_ENDL;
            emit DataReceived(cmd1.getData().mid(0, cmd1.getPayloadLen()));
        }
            break;
        default:
            break;
        }
    }
    //LOG_TRACE << "CIPodAppControllerPriv::largePacketAvailable() OUT"<< LOG_ENDL;
}

/******************************************************************************
// Function                                            Parameters
// CIPodAppControllerPriv::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
EIPodAppErrors CIPodAppControllerPriv::RequestForAppLaunch(QString bundleID, bool internal)
{
    //LOG_TRACE << "CIPodAppControllerPriv::RequestForAppLaunch() IN"<< LOG_ENDL;
    qint16 sessionId = GetSessionId();
    if(sessionId > 0)
    {
        //LOG_TRACE << "CIPodAppControllerPriv::RequestForAppLaunch()  already launched"<< LOG_ENDL;
        QTimer::singleShot(200, this, SLOT(AppLaunchAcknowledgementSlot()));
        return IPOD_APPSUCCESS;
    }
    EIPodAppErrors res = IPOD_APPERROR_GENERIC;
    if(m_Handle.m_BundleID.length() <= 0)
        m_Handle.m_BundleID.append(bundleID);

//{ Uncommented by esjang 2013.07.18 for ITS # 180650, Apple has changed their policy and no longer require the authorization popup
 //{ Commented  by radhakrushna 2013.04.25 for Mfi certification issue,Authontication user prompt is must.
    //{ added by esjang 2013.03.25 for ITS # 159186
    //if(bundleID.compare("com.pandora") == 0)
    //{
        //LOG_TRACE << "CIPodAppControllerPriv::RequestForAppLaunch() Pandora will be launched"<< LOG_ENDL;
        //LOG_TRACE << "Do not show Launch application popup on iPod Device."<< LOG_ENDL;
        internal = true;
    //}
    //} added by esjang 2013.03.25 for ITS # 159186
 //} Commented  by radhakrushna 2013.04.25 for Mfi certification issue
//} Uncommented by esjang 2013.07.18 for ITS # 180650, Apple has changed their policy and no longer require the authorization popup
    CRequesdtAppLaunch appLaunchCmd(m_Handle.m_BundleID, ++m_TransactionId, this, internal);
    appLaunchCmd.marshall();

    int byteWriteen = m_IAPWriter->SendPacket(appLaunchCmd);
    if(byteWriteen > 0)
        res = IPOD_APPSUCCESS;
    StartResendLogic(appLaunchCmd.getCommandBytes());
    //LOG_TRACE << "CIPodAppControllerPriv::RequestForAppLaunch() OUT"<< LOG_ENDL;
    return res;
}

void CIPodAppControllerPriv::AppLaunchAcknowledgementSlot()
{
    //LOG_TRACE << "CIPodAppControllerPriv::AppLaunchAcknowledgementSlot() IN"<< LOG_ENDL;
    emit Acknowledgement(IPOD_APPSUCCESS);
    //LOG_TRACE << "CIPodAppControllerPriv::AppLaunchAcknowledgementSlot() OUT"<< LOG_ENDL;
}
//Added for CR 16819: Fix for ATS
qint16 CIPodAppControllerPriv::getTransactionId(bool increment)
{
    if(increment)
    {
        if(m_TransId<32760)
        {
            m_TransId++;
        }
        else
        {
            m_TransId = 1000;
        }

    }
    return m_TransId;
}
