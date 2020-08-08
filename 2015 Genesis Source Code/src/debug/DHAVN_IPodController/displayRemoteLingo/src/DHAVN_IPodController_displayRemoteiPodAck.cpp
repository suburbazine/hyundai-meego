#include <DHAVN_IPodController_displayRemoteiPodAck.h>
using namespace DisplayRemoteLingo;


CDisplayRemoteIPodAck::CDisplayRemoteIPodAck(int transIdArgs,  bool isTidEnabled) :  m_TransId(transIdArgs)
{
    m_isTidEnabled = isTidEnabled;
    m_Status = -1;
    m_CommadId = -1;
}



QByteArray CDisplayRemoteIPodAck::marshall()
{
    return NULL;
}

void  CDisplayRemoteIPodAck::unmarshall(QByteArray returnCommand)
{
    if( ((int)returnCommand[0] == 0x55) &&
        ((int)returnCommand[2] == 0x0A) &&
        ((int)returnCommand[3] == 0x01) )
    {
        int i = 4;
        if(m_isTidEnabled){
            i = 6;
        }
        m_Status = returnCommand[i];
        m_CommadId = returnCommand[i+1];
    }
}
