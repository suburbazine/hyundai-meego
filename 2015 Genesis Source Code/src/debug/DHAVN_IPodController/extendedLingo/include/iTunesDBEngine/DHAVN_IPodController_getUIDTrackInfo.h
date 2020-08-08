#ifndef DHAVN_IPODCONTROLLER_GETUIDTRACKINFO_H
#define DHAVN_IPODCONTROLLER_GETUIDTRACKINFO_H

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

namespace ExtendedLingo
{
    class CGetUIDTrackInfo : public CIAPPacket
    {
        public:
           CGetUIDTrackInfo(quint64 uid, qint8 mask, int transId,bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
        private:
            quint64 m_UID;
            qint8 m_Mask;
    };
} // end of namespace ExtendedLingo


#endif // DHAVN_IPODCONTROLLER_GETUIDTRACKINFO_H
