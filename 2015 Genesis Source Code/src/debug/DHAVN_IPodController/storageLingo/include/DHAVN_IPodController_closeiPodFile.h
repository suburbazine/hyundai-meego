#ifndef CRETIPODCLOSEFEATUREFILE_H
#define CRETIPODCLOSEFEATUREFILE_H

#include <DHAVN_IPodController_iAPPacket.h>
namespace StorageLingo
{
    class CCloseiPodFeatureFile : public CIAPPacket
    {
        public:
           CCloseiPodFeatureFile(int handle,int transId, bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
	private:
	   int m_Handle;
    };
} // end of namespace StorageLingo

#endif // CRETIPODCLOSEFEATUREFILE_H
