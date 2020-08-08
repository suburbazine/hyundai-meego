
#include<DHAVN_IPodController_retNowPlayingAppBundleName.h>
#include <QByteArray>
using namespace GeneralLingo;

CRetNowPlayingAppBundleName::CRetNowPlayingAppBundleName( bool isTidEnabled )
    :CIAPPacket(0x00,0x0C,-1, isTidEnabled), m_NowPlayingAppBundleName("")
{

}

QByteArray CRetNowPlayingAppBundleName::marshall()
{
        QByteArray command;
        return command;
}

QString CRetNowPlayingAppBundleName::getNowPlayingAppBundleName()
{
    return m_NowPlayingAppBundleName.fromUtf8(m_NowPlayingAppBundleName.toAscii().data());
}

void  CRetNowPlayingAppBundleName::unmarshall(QByteArray returnCommand)
{
        // Remove the checksum byte from the array in order to use mid to obtain the record string
        //chop(1) removes 1 byte from the end of the array
        returnCommand.chop(1);
        if( ((int)returnCommand[0] == 0x55) &&
                        ((int)returnCommand[2] == 0x00) &&
                        ((int)returnCommand[3] == 0x66) ){

                if( m_isTidEnabled  )
                {
                        m_TransactionID = 0x0000;
                        m_TransactionID = m_TransactionID | returnCommand[4];
                        m_TransactionID = m_TransactionID <<8;
                        m_TransactionID = m_TransactionID | returnCommand[5];
                        m_NowPlayingAppBundleName = QString(returnCommand.mid(6,-1)); // bytearray from 12 to the end
                }
                else
                        m_NowPlayingAppBundleName = QString(returnCommand.mid(4,-1)); // bytearray from 12 to the end                
        }
}





