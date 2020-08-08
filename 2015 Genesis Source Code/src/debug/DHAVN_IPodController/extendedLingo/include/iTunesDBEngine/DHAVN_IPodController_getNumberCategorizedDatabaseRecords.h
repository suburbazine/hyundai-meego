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
#ifndef GETNUMBERCATEGORIZEDDATABASERECORDS_H
#define GETNUMBERCATEGORIZEDDATABASERECORDS_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <QString>
#include <DHAVN_LogUtil.h>

namespace ExtendedLingo
{

class CGetNumberCategorizedDatabaseRecords : public CIAPPacket
{
#ifdef DBUGLOGS
private:
    USE_LOG_UTIL
#endif
public:
    CGetNumberCategorizedDatabaseRecords(int transId,bool isTidEnabled);
    CGetNumberCategorizedDatabaseRecords(QString categoryValue,int transId,bool isTidEnabled);
    //void setCategory(QString categoryValue);
    QByteArray marshall()        ;
     void  unmarshall(QByteArray)        ;
private:
        QString m_Category;
};

}

#endif
