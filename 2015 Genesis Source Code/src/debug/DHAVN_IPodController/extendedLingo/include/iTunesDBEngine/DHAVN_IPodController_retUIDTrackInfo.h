#ifndef DHAVN_IPODCONTROLLER_RETUIDTRACKINFO_H
#define DHAVN_IPODCONTROLLER_RETUIDTRACKINFO_H

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

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_retUIDTrackInfo.h>

namespace ExtendedLingo
{
    class CRetUIDTrackInfo : public CIAPPacket
    {
        public:
           CRetUIDTrackInfo(bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
           QString getInfoData();
           int getInfoType();
           int getTrackUID();
           int getTrackDuration();
           quint64 getUID();
        private:
           QString m_Data;
           int m_InfoType;
           int m_TrackDuration;
           quint64 m_UID;
    };

} // end of namespace ExtendedLingo


#endif // DHAVN_IPODCONTROLLER_RETUIDTRACKINFO_H
