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
#ifndef PREPAREUIDLIST_H
#define PREPAREUIDLIST_H

#include <DHAVN_IPodController_iAPPacket.h>

class CPrepareUIDList : public CIAPPacket
{
public:
    CPrepareUIDList(int ,int,int transId,QByteArray& arr,bool);
    QByteArray marshall()        ;
     void  unmarshall(QByteArray)        ;
private:
     int m_SectCurrent;
     int m_SectMax;
     QByteArray m_UIDBytes;
};

#endif // PREPAREUIDLIST_H
