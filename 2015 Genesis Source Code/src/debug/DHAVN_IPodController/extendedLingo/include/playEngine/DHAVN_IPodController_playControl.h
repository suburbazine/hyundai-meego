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
#ifndef PLAYCONTROL_H
#define PLAYCONTROL_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

class CPlayControl : public CIAPPacket
{
#ifdef DBUGLOGS
private:
    USE_LOG_UTIL
#endif
public:
    CPlayControl(QString act,int transId, bool isTidEnabled, bool isNano1GFlag = false);
    QByteArray marshall()        ;
     void  unmarshall(QByteArray)        ;
private:
     QString m_Action;
     bool isNano1GPodcastFlag;
};

#endif // PLAYCONTROL_H
