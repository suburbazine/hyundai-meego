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
#ifndef GETDBITUNESINFO_H
#define GETDBITUNESINFO_H

#include <DHAVN_IPodController_iAPPacket.h>

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
    class CGetDBiTunesInfo : public CIAPPacket
    {
        public:
           CGetDBiTunesInfo(int infoType, int transId, bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
        private:
            int m_infoType;
    };
} // end of namespace ExtendedLingo

#endif // GETDBITUNESINFO_H
