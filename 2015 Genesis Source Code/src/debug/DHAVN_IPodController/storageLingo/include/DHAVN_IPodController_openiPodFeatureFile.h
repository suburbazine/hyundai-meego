#ifndef CRETIPODOPENFEATUREFILE_H
#define CRETIPODOPENFEATUREFILE_H

#include <DHAVN_IPodController_iAPPacket.h>
namespace StorageLingo
{
    class COpeniPodFeatureFile : public CIAPPacket
    {
        public:
           COpeniPodFeatureFile(int transId, bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
    };
} // end of namespace StorageLingo

#endif // CRETIPODOPENFEATUREFILE_H
