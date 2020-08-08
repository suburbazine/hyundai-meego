#include <DHAVN_IPodController_returnPlayStatus.h>

using namespace ExtendedLingo;

CReturnPlayStatus::CReturnPlayStatus( bool isTidEnabled)
:CIAPPacket(0x04,0x001D,-1, isTidEnabled),m_State(-1)
{
	//        qDebug() << "in CReturnPlayStatus constructor\n" ;
}

QByteArray CReturnPlayStatus::marshall()
{
	QByteArray command;
	return command;
}

int CReturnPlayStatus::getCurrentState()
{
	return m_State;
}

void  CReturnPlayStatus::unmarshall(QByteArray returnCommand)
{
	if( ((int)returnCommand[0] == 0x55) &&
			((int)returnCommand[2] == 0x04) &&
			((int)returnCommand[3] == 0x00) &&
			((int)returnCommand[4] == 0x1D) ){
		int value = 0;
                if( m_isTidEnabled )
		{
			value = 15;
		}
		else
		{
			value = 13;
		}
		m_State = (int)returnCommand[value];
	}
}


