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
#ifndef SETREPEAT_H
#define SETREPEAT_H

#include <DHAVN_IPodController_iAPPacket.h>

class CSetRepeat : public CIAPPacket
{
public:
    CSetRepeat(int mode,int transId, bool isTidEnabled);
    QByteArray marshall()        ;
     void  unmarshall(QByteArray)        ;
private:
     int m_RepeatMode;
};

#endif // SETREPEAT_H
