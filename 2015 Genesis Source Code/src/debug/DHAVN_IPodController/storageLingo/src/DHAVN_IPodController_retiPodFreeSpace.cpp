#include <DHAVN_IPodController_retiPodFreeSpace.h>
#include <DHAVN_IPodController_CommonVars.h>


using namespace StorageLingo;

CRetiPodFreeSpace::CRetiPodFreeSpace( bool isTidEnabled)
:CIAPPacket(0x0C,0x02,-1, isTidEnabled),m_FreeSpace(0)
{
	//        qDebug() << "in CRetiPodFreeSpace constructor\n" ;
    LOG_INIT_V2(gControllerLogName);
}

QByteArray CRetiPodFreeSpace::marshall()
{
	QByteArray command;
	return command;
}

quint64 CRetiPodFreeSpace::getFreeSpace()
{
	return m_FreeSpace;
}

void  CRetiPodFreeSpace::unmarshall(QByteArray returnCommand)
{
	if( ((int)returnCommand[0] == 0x55) &&
			((int)returnCommand[2] == 0x0C) &&
			((int)returnCommand[3] == 0x11) ){
		int nStartIndex = -1;
		int nEndIndex = -1;
                if( m_isTidEnabled  )
		{
			nStartIndex = 6;
			nEndIndex = 14;
		}
		else
		{
			nStartIndex = 4;
			nEndIndex = 12;
		}
		for(int i = nStartIndex;i< nEndIndex;i++)
		{
			m_FreeSpace <<= 8;     m_FreeSpace |= (qint64)returnCommand[i] & 0xFF;
            //LOG_INFO << " CiTunesTagTaskHandler: FreeSpace = " << m_FreeSpace << LOG_ENDL;
		}
	}
}
