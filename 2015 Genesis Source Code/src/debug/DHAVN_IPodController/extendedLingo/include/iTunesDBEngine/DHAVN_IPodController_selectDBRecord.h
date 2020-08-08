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
#ifndef SELECTDBRECORD_H
#define SELECTDBRECORD_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <QString>

namespace ExtendedLingo
{

class CSelectDBRecord : public CIAPPacket
{
public:
    CSelectDBRecord();
    CSelectDBRecord(QString categoryValue,int index,int transId,bool isTidEnabled);
    //void setCategoryAndIndex(QString categoryValue,int indexValue);
    QByteArray marshall()        ;
     void  unmarshall(QByteArray)        ;
private:
        QString m_Category;
        int m_Index;
};

} // end of namespace ExtendedLingo
#endif // SELECTDBRECORD_H
