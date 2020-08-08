#include <DHAVN_IPodAppController_RequestAppLaunch.h>
#include <DHAVN_IPodAppController_global.h>

using namespace GeneralLingo;

CRequesdtAppLaunch::CRequesdtAppLaunch(QString bundleId, int transIdArgs, QObject *parent, bool relaunch) : CIAPPacket(0x00, 0x64, transIdArgs, parent)
{
    LOG_INIT_V2(gIPodAppControllerLogs);
m_BundleId.append(bundleId);
m_relaunch = relaunch;
}

QByteArray CRequesdtAppLaunch::marshall()
{
    QByteArray ProtocolArray(m_BundleId.toUtf8());
    int sizeOfPacket = 7 + ProtocolArray.size() + 1;

    int i=-1;
    QByteArray command;
    command[++i] = 0x55;
    command[++i] = sizeOfPacket;
    command[++i] = 0x00;
    command[++i] = 0x64;
    command[++i] = (m_TransactionID & 0xff00) >> 8;
    command[++i] =  m_TransactionID & 0x00ff;
    command[++i] =  0x00;
    if(m_relaunch)
        command[++i] =  0x01;
    else
        command[++i] =  0x02;
    command[++i] =  0x00;
    command.insert(++i, ProtocolArray);
    i = i + ProtocolArray.count();
    command[i] =  '\0';
    command[i+1] = CIpodAppControllerGlobal::checkSum(command, i);
    m_CommandBytes = command;
    return command;
}

void  CRequesdtAppLaunch::unmarshall(const QByteArray&)
{

}
