#include <DHAVN_IPodController_returnCurrentPlayingTrackIndex.h>

using namespace ExtendedLingo;

CReturnCurrentPlayingTrackIndex::CReturnCurrentPlayingTrackIndex( bool isTidEnabled)
:CIAPPacket(0x04,0x001F,-1, isTidEnabled),m_Index(-1)
{
	//        qDebug() << "in CReturnCurrentPlayingTrackIndex constructor\n" ;
}

QByteArray CReturnCurrentPlayingTrackIndex::marshall()
{
	QByteArray command;
		return command;
}

int CReturnCurrentPlayingTrackIndex::getCurrentIndex()
{
	return m_Index;
}

void  CReturnCurrentPlayingTrackIndex::unmarshall(QByteArray returnCommand)
{
	if( ((int)returnCommand[0] == 0x55) &&
			((int)returnCommand[2] == 0x04) &&
			((int)returnCommand[3] == 0x00) &&
			((int)returnCommand[4] == 0x1F) ){
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


