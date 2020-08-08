#include <DHAVN_IPodController_returnNumOfPlayingTracks.h>

using namespace ExtendedLingo;

CReturnNumOfPlayingTracks::CReturnNumOfPlayingTracks( bool isTidEnabled)
:CIAPPacket(0x04,0x001F,-1, isTidEnabled),m_Index(-1)
{
	//        qDebug() << "in CReturnCurrentPlayingTrackIndex constructor\n" ;
}

QByteArray CReturnNumOfPlayingTracks::marshall()
{
	QByteArray command;
		return command;
}

int CReturnNumOfPlayingTracks::getPlayListCount()
{
	return m_Index;
}

void  CReturnNumOfPlayingTracks::unmarshall(QByteArray returnCommand)
{
	if( ((int)returnCommand[0] == 0x55) &&
			((int)returnCommand[2] == 0x04) &&
			((int)returnCommand[3] == 0x00) &&
                        ((int)returnCommand[4] == 0x36) ){
		int value = 0;
		int nStartIndex = -1;
		int nEndIndex = -1;
                if( m_isTidEnabled  )
		{
			nStartIndex = 7;
			nEndIndex = 11;
		}
		else
		{
			nStartIndex = 5;
			nEndIndex = 9;
		}
		for(int i = nStartIndex;i< nEndIndex;i++)
		{
			value <<= 8;     value |= (int)returnCommand[i] & 0xFF;
		}
		m_Index = value;
	}
	//            qDebug() << m_Index  ;
}


