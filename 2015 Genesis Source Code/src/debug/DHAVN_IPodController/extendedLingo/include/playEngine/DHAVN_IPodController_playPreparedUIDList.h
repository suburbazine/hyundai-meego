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
#ifndef PLAYPREPAREUIDLIST_H
#define PLAYPREPAREUIDLIST_H

#include <DHAVN_IPodController_iAPPacket.h>

class CPlayPreparedUIDList : public CIAPPacket
{
public:
    CPlayPreparedUIDList(int transId,QByteArray arr,bool);
    QByteArray marshall()        ;
     void  unmarshall(QByteArray)        ;
private:
     QByteArray m_startUIDBytes;
};

#endif // PREPAREUIDLIST_H
