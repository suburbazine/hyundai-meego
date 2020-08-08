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
#ifndef RETURNNUMBERCATEGORIZEDDATABASERECORDS_H
#define RETURNNUMBERCATEGORIZEDDATABASERECORDS_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{
    class CReturnNumberCategorizedDatabaseRecords : public CIAPPacket
    {
        public:
           CReturnNumberCategorizedDatabaseRecords(bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           int getCount();
        private:
            QString m_Category;
            int m_RecordCount;
    };

} // end of namespace ExtendedLingo

#endif //RETURNNUMBERCategorizedDATABASERECORDS_H
