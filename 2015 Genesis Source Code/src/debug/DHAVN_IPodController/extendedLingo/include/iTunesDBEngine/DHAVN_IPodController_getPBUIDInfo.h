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
#ifndef GETPBUIDINFO_H
#define GETPBUIDINFO_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace ExtendedLingo
{
    class CGetPBUIDInfo : public CIAPPacket
    {
        public:
           CGetPBUIDInfo(int startIndex,
                                   int count,
                                   int transId,bool isTidEnabled);
           QByteArray marshall();
           void  unmarshall(QByteArray);
        private:
           USE_LOG_UTIL;
            QString m_Category;
            int m_StartIndex;
            int m_TrackCount;
    };
} // end of namespace ExtendedLingo

#endif // GETDBTRACKINFO_H
