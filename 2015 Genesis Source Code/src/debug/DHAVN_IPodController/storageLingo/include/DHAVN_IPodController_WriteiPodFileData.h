#ifndef DHAVN_IPODCONTROLLER_WRITEIPODFILEDATA_H
#define DHAVN_IPODCONTROLLER_WRITEIPODFILEDATA_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace StorageLingo
{
	class CWriteiPodFileData: public CIAPPacket
	{
		public:
                        CWriteiPodFileData(int transId, int , int,int dataSize,QByteArray data, bool isTidEnabled);
			QByteArray marshall();
			void  unmarshall(QByteArray);

		private:
			QByteArray m_plistData;
			int m_dataOffset;
			int m_Handle;
			int m_dataSize;
	};
}


#endif // DHAVN_IPODCONTROLLER_WRITEIPODFILEDATA_H
