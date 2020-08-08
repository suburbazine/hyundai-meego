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
#ifndef RETURNCATEGORIZEDDATABASERECORDS_H
#define RETURNCATEGORIZEDDATABASERECORDS_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{
    class CReturnCategorizedDatabaseRecords : public CIAPPacket
    {
        public:
           CReturnCategorizedDatabaseRecords(bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           QString getRecordString();
           int getRecordIndex();
        private:
            QString m_RecordString;
            int m_RecordIndex;
    };

} // end of namespace ExtendedLingo
#endif // RETURNCATEGORIZEDDATABASERECORDS_H
