#include <DHAVN_IPodController_retiPodCaps.h>

using namespace StorageLingo;

CRetiPodCaps::CRetiPodCaps( bool isTidEnabled)
:CIAPPacket(0x0C,0x02,-1,isTidEnabled),m_TotalSize(0),m_MaxFileSize(0),m_MaxWriteSize(0),m_MajorVersion(0),m_MinorVersion(0)
{
	//        qDebug() << "in CRetiPodCaps constructor\n" ;
}

QByteArray CRetiPodCaps::marshall()
{
	QByteArray command;
	return command;
}

quint64 CRetiPodCaps::getTotalSpace()
{
	return m_TotalSize;
}

quint32 CRetiPodCaps::getMaxFileSize()
{
	return m_MaxFileSize;
}

quint32 CRetiPodCaps::getMaxWriteSize()
{
	return m_MaxWriteSize;
}

int CRetiPodCaps::getMajorVersion()
{
	return m_MajorVersion;
}

int CRetiPodCaps::getMinorVersion()
{
	return m_MinorVersion;
}

void  CRetiPodCaps::unmarshall(QByteArray returnCommand)
{
	if( ((int)returnCommand[0] == 0x55) &&
			((int)returnCommand[2] == 0x0C) &&
			((int)returnCommand[3] == 0x02) ){
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
			m_TotalSize <<= 8;     m_TotalSize |= (qint64)returnCommand[i] & 0xFF;
		}
		nStartIndex = nEndIndex;
		nEndIndex += 4;
		for(int i = nStartIndex;i< nEndIndex;i++)
		{
			m_MaxFileSize <<= 8;     m_MaxFileSize |= (int)returnCommand[i] & 0xFF;
		}
		nStartIndex = nEndIndex;
		nEndIndex += 2;
		for(int i = nStartIndex;i< nEndIndex;i++)
		{
			m_MaxWriteSize <<= 8;     m_MaxWriteSize |= (int)returnCommand[i] & 0xFF;
		}
		nStartIndex = nEndIndex + 6;
		m_MajorVersion = (int)returnCommand[nStartIndex];
		++nStartIndex;
		m_MinorVersion = (int)returnCommand[nStartIndex];
	}
}
