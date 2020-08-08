#ifndef RETIPODCAPS_H
#define RETIPODCAPS_H

#include <DHAVN_IPodController_iAPPacket.h>
namespace StorageLingo
{
    class CRetiPodCaps : public CIAPPacket
    {
        public:
           CRetiPodCaps( bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           quint64 getTotalSpace();
           quint32 getMaxFileSize();
           quint32 getMaxWriteSize();
	   int getMajorVersion();
	   int getMinorVersion();
        private:
            quint64 m_TotalSize;
            quint32 m_MaxFileSize;
            quint32 m_MaxWriteSize;
	    int m_MajorVersion;
	    int m_MinorVersion;
    };
} // end of namespace StorageLingo

#endif // RETIPODCAPS_H
