#ifndef CRETURNPLAYSTATUS_H
#define CRETURNPLAYSTATUS_H

#include <DHAVN_IPodController_iAPPacket.h>
namespace ExtendedLingo
{
    class CReturnPlayStatus : public CIAPPacket
    {
        public:
           CReturnPlayStatus( bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           int getCurrentState();
        private:
            int m_State;
    };

} // end of namespace ExtendedLingo

#endif // CRETURNPLAYSTATUS_H
