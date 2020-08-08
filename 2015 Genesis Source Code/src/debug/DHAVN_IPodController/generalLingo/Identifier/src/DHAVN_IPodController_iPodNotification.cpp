
#include<DHAVN_IPodController_iPodNotification.h>
#include <QByteArray>
#include <DHAVN_IPodController_iPodControllerGlobal.h>   // Added by pawan 2013.05.15.
using namespace GeneralLingo;

CiPodNotification::CiPodNotification( bool isTidEnabled )
    :CIAPPacket(0x00,0x0C,-1, isTidEnabled), m_BundleName("")
{
    m_NotificationType = -1;
    LOG_INIT_V2(giPodControllerLogName);   // Added by pawan 2013.05.15.
    m_IsDBChanged = false; // Added by pawan 2013.05.15.
}

QByteArray CiPodNotification::marshall()
{
    QByteArray command;
    return command;
}

int CiPodNotification::getNotificationType()
{
    return m_NotificationType;
}

QString CiPodNotification::getBundleName()
{
    return m_BundleName.fromUtf8(m_BundleName.toAscii().data());
}

void  CiPodNotification::unmarshall(QByteArray returnCommand)
{
    // { Modified by pawan 2013.05.15.
    //LOG_TRACE << "dbchange printing command " << LOG_ENDL;
    CIpodControllerGlobal::printArray(returnCommand);

    // Remove the checksum byte from the array in order to use mid to obtain the record string
    //chop(1) removes 1 byte from the end of the array
    returnCommand.chop(1);
    if( ((int)returnCommand[0] == 0x55) &&
            ((int)returnCommand[2] == 0x00) &&
            ((int)returnCommand[3] == 0x4A))
    {
        m_NotificationType = (int) returnCommand[6];
        if(m_NotificationType == 0x0A || m_NotificationType==0x13)
        {
            //LOG_TRACE << "ios6 nowplaying app notification received" << LOG_ENDL;
            // } Modified by pawan 2013.05.15.
            if( m_isTidEnabled  )
            {
                m_TransactionID = 0x0000;
                m_TransactionID = m_TransactionID | returnCommand[4];
                m_TransactionID = m_TransactionID <<8;
                m_TransactionID = m_TransactionID | returnCommand[5];
                m_BundleName = QString(returnCommand.mid(7,-1)); // bytearray from 12 to the end
            }
            else
                m_BundleName = QString(returnCommand.mid(5,-1)); // bytearray from 12 to the end
        }
        // { Modified by pawan 2013.05.15.
        else if(m_NotificationType == 0x09)
        {
            m_IsDBChanged = true;
            //LOG_TRACE << "ios6 db change notification received" << LOG_ENDL;
        }
    }
    // } Modified by pawan 2013.05.15.
}






