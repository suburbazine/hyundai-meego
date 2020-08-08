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
#ifndef DHAVN_IPODAPPCONTROLLER_IPODDATATRANSFER_H
#define DHAVN_IPODAPPCONTROLLER_IPODDATATRANSFER_H

#include <DHAVN_IPodAppController_iAPPacket.h>
#include <DHAVN_LogUtil.h>
#include <DHAVN_IPodAppController_CommonVars.h>
class CIPodDataTransfer : public CIAPPacket
{
USE_LOG_UTIL
public:
    CIPodDataTransfer(qint16 sessionId) : m_SessionId(sessionId),m_Len( 0x0000) {LOG_INIT_V2(gIPodAppControllerLogs);}
    QByteArray marshall();
     void  unmarshall(const QByteArray&);
     QByteArray getData();

     int getPayloadLen()
     {
        return m_Len - 6;
     }
private:
        QByteArray m_DataArray;
        qint16 m_SessionId;

        // Temp fix
        qint16 m_Len;
};

#endif // DHAVN_IPODAPPCONTROLLER_IPODDATATRANSFER_H
