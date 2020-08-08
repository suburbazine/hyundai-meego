#include <DHAVN_IPodController_setiPodStateInfo.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
using namespace DisplayRemoteLingo;

CSetiPodStateInfo::CSetiPodStateInfo(InfoType infoType, int time,int transId, bool isTidEnabled)
    :CIAPPacket(0x03,0x0E,transId, isTidEnabled)
{
    m_time = time;
    m_InfoType = infoType;
}

QByteArray CSetiPodStateInfo::marshall()
{
        QByteArray command;

        //Packing format =  Big Endian
        //command.resize(11);
        int i = -1;

        command[++i] = 0x55;
        command[++i] = 0x07;
        command[++i] = 0x03;
        command[++i] = 0x0E;

        if( m_isTidEnabled ) {
            command[++i] = ((m_TransactionID & 0xff00) >> 8);
            command[++i] = m_TransactionID & 0x00ff;
        }

        switch(m_InfoType)
        {
        case 0x00:
            command[++i] = 0x00;    // info type
            command[++i] = (char)((m_time >> 24) & 0xff);    // byte 0
            command[++i] = (char)((m_time >> 16) & 0xff);    // byte 0
            command[++i] = (char)((m_time >> 8) & 0xff);    // byte 0
            command[++i] = (char)(m_time & 0xff);    // 1
            break;
        case 0x01:
            command[++i] = 0x03;    // info type
            command[++i] = 0x01;
            break;
        case 0x02:
            command[++i] = 0x03;    // info type
            command[++i] = 0x02;
            break;
        }
        qDebug() << "marshall **************************";
        command[1] = i-1;
        command.append(CIpodControllerGlobal::checkSum(command, i));

        qDebug() << "*************************** Command length = " << command.length();
        m_CommandBytes = command;
        return command;
}

void  CSetiPodStateInfo::unmarshall(QByteArray )
{
}



