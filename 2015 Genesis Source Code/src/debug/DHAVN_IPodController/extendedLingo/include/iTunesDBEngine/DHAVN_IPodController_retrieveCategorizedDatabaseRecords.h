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
#ifndef RETRIEVECATEGORIZEDDATABASERECORDS_H
#define RETRIEVECATEGORIZEDDATABASERECORDS_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{
    class CRetrieveCategorizedDatabaseRecords : public CIAPPacket
    {
        public:
           CRetrieveCategorizedDatabaseRecords(int transId,bool isTidEnabled)  : CIAPPacket(0x04,0x001A,transId,isTidEnabled){
           m_StartRecord = -1; m_ReadCount = -1;  //CR 15209 added by jungae 2012.10.29, Static_Test 0134442
           }
           CRetrieveCategorizedDatabaseRecords(QString category,
                                                                   int startRecord,
                                                                   int readCount,
                                                                   int transId,bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           void setParameters( QString category,int startRecord,int readCount);
        private:
            QString m_Category;
            int m_StartRecord;
            int m_ReadCount;
    };

} // end of namespace ExtendedLingo

#endif // RETRIEVECATEGORIZEDDATABASERECORDS_H
