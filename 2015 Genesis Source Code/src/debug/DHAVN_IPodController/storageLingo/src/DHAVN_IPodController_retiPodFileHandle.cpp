#include <DHAVN_IPodController_retiPodFileHandle.h>

using namespace StorageLingo;

CRetiPodFileHandle::CRetiPodFileHandle( bool isTidEnabled )
:CIAPPacket(0x0C,0x04,-1, isTidEnabled),m_Handle(-1)
{
	//        qDebug() << "in CRetiPodFileHandle constructor\n" ;
}

QByteArray CRetiPodFileHandle::marshall()
{
	QByteArray command;
	return command;
}

int CRetiPodFileHandle::getFileHandle()
{
	return m_Handle;
}

void  CRetiPodFileHandle::unmarshall(QByteArray returnCommand)
{
	if( ((int)returnCommand[0] == 0x55) &&
			((int)returnCommand[2] == 0x0C) &&
			((int)returnCommand[3] == 0x04) )
	{
		int nHandleByte = -1;
                if( m_isTidEnabled  )
		{
			nHandleByte = 6;
		}
		else
		{
			nHandleByte = 4;
		}
		m_Handle = 0;
		m_Handle |= (int)returnCommand[nHandleByte] & 0xFF;
	}
}
