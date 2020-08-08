#include <DHAVN_IPodController_DBusCore.h>
#include <DHAVN_IPodController_hidRawPacketWriter.h>
#include <DHAVN_IPodController_Identifier.h>
#include <DHAVN_IPodController_PrivateData.h>

/******************************************************************************
// Function                                            Parameters
// ::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
CIPodController_DBus::CIPodController_DBus(QObject* parent) : QObject(parent)
{
    LOG_INIT_V2(gControllerLogName);
    //LOG_TRACE << "CIPodController_DBus::CIPodController_DBus Entry" << LOG_ENDL;
    CIdentifierTaskHandler* handler = parent->findChild<CIdentifierTaskHandler*>(gIdentifier);
    connect(handler, SIGNAL(IpodAppSessionClosed(QString,QString)), this, SLOT(OnIpodAppSessionClosed(QString,QString)));
    //connect(handler, SIGNAL(IPodDisconnected(QString)), this, SLOT(OnIPodDisconnected(QString)));
    //LOG_TRACE << "CIPodController_DBus::CIPodController_DBus Exit" << LOG_ENDL;
}

/******************************************************************************
// Function                                            Parameters
// ::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
CIPodController_DBus::~CIPodController_DBus()
{

}

/******************************************************************************
// Function                                            Parameters
// ::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QString CIPodController_DBus::GetHidPath(const QString& ipodNumber)
{
    //LOG_TRACE << "CIPodController_DBus::GetHidPath Entry" << LOG_ENDL;
    //CHIDRawPacketHandle* handler = parent()->findChild<CHIDRawPacketHandle*>(gRawPcktHandler);
    //QString path(handler->getDevHidPath());
    QString path( (parent()->findChild<iPodControllerPrivateData*>(gControllerPrivateData))->m_hidDevPath );
    //LOG_INFO << "path returned is: " << path.toAscii().data() <<
    //            " for ipod no: " << ipodNumber.toAscii().data() << LOG_ENDL;
    //LOG_TRACE << "CIPodController_DBus::GetHidPath Exit" << LOG_ENDL;
    return path;
}

/******************************************************************************
// Function                                            Parameters
// ::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
qint16 CIPodController_DBus::GetApplicationSessionID(const QString& AppId)
{
    //LOG_TRACE << "CIPodController_DBus::GetApplicationSessionID Entry" << LOG_ENDL;
    qint16 sessionId = 0;
    CIdentifierTaskHandler* handler = parent()->findChild<CIdentifierTaskHandler*>(gIdentifier);
    QList <AppSessionId> listt = handler->getSessionIdList();
    for(int i=0; i<listt.count() ;i++)
    {
        if(!AppId.compare(listt[i].m_AppId))
        {
            sessionId = listt[i].sessionId;
            break;
        }
    }
    //LOG_INFO << "sessionid returned is: "<< sessionId <<
    //            " for appid: " << AppId.toAscii().data() << LOG_ENDL;
    //LOG_TRACE << "CIPodController_DBus::GetApplicationSessionID Exit" << LOG_ENDL;
    return sessionId;
}

/******************************************************************************
// Function                                            Parameters
// ::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIPodController_DBus::OnIpodAppSessionClosed(const QString& ipodNumber, const QString& appId)
{
    //LOG_TRACE << "CIPodController_DBus::OnIpodAppSessionClosed Entry" << LOG_ENDL;
    emit IpodAppSessionClosed(ipodNumber, appId);
    //LOG_TRACE << "CIPodController_DBus::OnIpodAppSessionClosed Exit" << LOG_ENDL;
}

/******************************************************************************
// Function                                            Parameters
// ::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CIPodController_DBus::OnIPodDisconnected(const QString& ipodNumber)
{
    //LOG_TRACE << "CIPodController_DBus::OnIPodDisconnected Entry" << LOG_ENDL;
    emit IPodDisconnected(ipodNumber);
    //LOG_TRACE << "CIPodController_DBus::OnIPodDisconnected Exit" << LOG_ENDL;
}

void CIPodController_DBus::IpodConnected()
{
    CIdentifierTaskHandler* handler = parent()->findChild<CIdentifierTaskHandler*>(gIdentifier);
    connect(handler, SIGNAL(IpodAppSessionClosed(QString,QString)), this, SLOT(OnIpodAppSessionClosed(QString,QString)));
}

// End of file



