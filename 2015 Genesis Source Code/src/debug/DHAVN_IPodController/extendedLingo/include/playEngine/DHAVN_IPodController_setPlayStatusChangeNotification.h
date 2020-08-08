/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  BaseMediaPlay                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini / Venkat                    *
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
#ifndef SETPLAYSTATUSCHANGENOTIFICATION_H
#define SETPLAYSTATUSCHANGENOTIFICATION_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

class CSetPlayStatusChangeNotification : public CIAPPacket
{
private:
    USE_LOG_UTIL
public:
    CSetPlayStatusChangeNotification(int, bool isTidEnabled,bool isUID = false, bool isSyncInProgress = false);
    QByteArray marshall()        ;
    void  unmarshall(QByteArray)        ;
private:
    bool m_isUIDBased;
    bool m_IsSyncInProgress;
};


#endif // SETPLAYSTATUSCHANGENOTIFICATION_H
