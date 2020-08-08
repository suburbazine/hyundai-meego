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
#ifndef SETSHUFFLE_H
#define SETSHUFFLE_H

#include <DHAVN_IPodController_iAPPacket.h>

class CSetShuffle : public CIAPPacket
{
public:
    CSetShuffle(int mode,int transId, bool isTidEnabled);
    QByteArray marshall()        ;
     void  unmarshall(QByteArray)        ;
private:
     int m_ShuffleMode;
};

#endif // SETSHUFFLE_H
