/*
 ******************************************************************
 *        COPYRIGHT © Teleca AB                                   *
 *----------------------------------------------------------------*
 * MODULE     :  IPodAppController                                *
 *
 * PROGRAMMERS : Pawan Gupta                                      *
 * DATE       :  16 jan 2012                                      *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :

 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef DHAVN_IPODAPPCONTROLLER_DEVACK_H
#define DHAVN_IPODAPPCONTROLLER_DEVACK_H

#include <DHAVN_IPodAppController_iAPPacket.h>

namespace GeneralLingo
{
class CDevAck: public CIAPPacket
{
public:
    CDevAck(int transIdArgs, qint8 commandId, QObject* parent = 0);
    QByteArray marshall();
    void  unmarshall(const QByteArray&);
private:
    qint16 m_TransId;
};
}

#endif // DHAVN_IPODAPPCONTROLLER_DEVACK_H
