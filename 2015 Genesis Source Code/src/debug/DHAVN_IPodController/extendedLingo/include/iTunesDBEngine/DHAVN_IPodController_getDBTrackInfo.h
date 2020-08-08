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
#ifndef GETDBTRACKINFO_H
#define GETDBTRACKINFO_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{
    class CGetDBTrackInfo : public CIAPPacket
    {
        public:
           CGetDBTrackInfo(int startIndex,
                                   int count,
                                   int transId,bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
        private:
            QString m_Category;
            int m_StartIndex;
            int m_TrackCount;
    };
} // end of namespace ExtendedLingo

#endif // GETDBTRACKINFO_H
