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
#ifndef PLAYCURRENTSELECTION_H
#define PLAYCURRENTSELECTION_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

class CPlayCurrentSelection : public CIAPPacket
{
private:
    USE_LOG_UTIL
public:
    CPlayCurrentSelection(int index,int transId, bool isTidEnabled);
    QByteArray marshall()        ;
    void  unmarshall(QByteArray)        ;
private:
     int m_CurrentPlayingTrackIndex;
};

#endif // PLAYCURRENTSELECTION_H
