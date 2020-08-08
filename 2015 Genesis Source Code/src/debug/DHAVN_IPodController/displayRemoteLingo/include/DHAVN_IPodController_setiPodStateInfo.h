#ifndef SETIPODSTATEINFO_H
#define SETIPODSTATEINFO_H

#include <DHAVN_IPodController_iAPPacket.h>

enum InfoType
{
    eSetDisplayRemotePosition = 0x00,
    eSetDisplayRemotePlay = 0x01,
    eSetDisplayRemotePause = 0x02
};

namespace DisplayRemoteLingo
{

class CSetiPodStateInfo: public CIAPPacket
{
public:
    CSetiPodStateInfo(InfoType infoType, int time, int transId, bool isTidEnabled);
    QByteArray marshall();
    void  unmarshall(QByteArray);
private:
    int m_time;
    InfoType m_InfoType;
};

}

#endif  //  SETIPODSTATEINFO_H
