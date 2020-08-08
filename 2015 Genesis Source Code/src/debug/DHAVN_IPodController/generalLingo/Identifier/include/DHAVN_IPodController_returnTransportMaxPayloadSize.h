
/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Sowmini Philip                    *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                                                                *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef RETURNTRANSPORTMAXPAYLOADSIZE_H
#define RETURNTRANSPORTMAXPAYLOADSIZE_H


#include <DHAVN_IPodController_iAPPacket.h>

namespace GeneralLingo
{

class CReturnTransportMaxPayloadSize: public CIAPPacket
{
public:
    CReturnTransportMaxPayloadSize(int transId, bool isTidEnabled);
    QByteArray marshall();
    void  unmarshall(QByteArray);
    int getMaxPayloadSize();
private:
        int m_maxPayloadSize;
};

}

#endif //RETURNTRANSPORTMAXPAYLOADSIZE_H
