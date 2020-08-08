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
#ifndef DHAVN_IPODAPPCONTROLLER_DEVDATATRANSFER_H
#define DHAVN_IPODAPPCONTROLLER_DEVDATATRANSFER_H

#include <DHAVN_IPodAppController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace GeneralLingo
{

class CDevDataTransfer: public CIAPPacket
{
    USE_LOG_UTIL
public:
    CDevDataTransfer(QByteArray data, int transIdArgs, int sessionId, QObject *parent);
    QByteArray marshall();
    void  unmarshall(const QByteArray&);
private:
    qint16 m_TransId;
    QByteArray m_DataArray;
    int m_SessionId;
};

}

#endif // DHAVN_IPODAPPCONTROLLER_DEVDATATRANSFER_H
