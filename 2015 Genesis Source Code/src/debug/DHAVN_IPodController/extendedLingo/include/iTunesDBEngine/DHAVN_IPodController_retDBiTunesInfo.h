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
#ifndef RETDBITUNESINFO_H
#define RETDBITUNESINFO_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <stdint.h>
#include <QTime>
#include <QDate>
#include <DHAVN_LogUtil.h>

#ifndef iTUNESDBINFOTYPES
#define iTUNESDBINFOTYPES
enum iTunesDBInfoTypes
{
    DBUid = 0,
    lastSyncTime,
    totalTrackCount

};
#endif

namespace ExtendedLingo
{
    class CRetDBiTunesInfo : public CIAPPacket
    {
        public:
           CRetDBiTunesInfo(bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           int getInfoType();
           uint64_t getiTunesUid();
           QTime getLastSyncTime();
           QDate getLastSyncDate();
        private:
           int m_InfoType;
           uint64_t m_iTunesUid;
           int sec;
           int min;
           int hours;
           int day;
           int month;
           int year;
#ifdef DBUGLOGS
    private:
        USE_LOG_UTIL
#endif
    };

} // end of namespace ExtendedLingo

#endif // RETDBITUNESINFO_H
