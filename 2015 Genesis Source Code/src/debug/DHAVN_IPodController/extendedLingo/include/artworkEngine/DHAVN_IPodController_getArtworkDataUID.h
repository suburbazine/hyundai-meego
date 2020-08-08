/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  BaseMediaPlay                                    *
 *
 * PROGRAMMERS : Pawan Gupta                                        *
 * DATE       :  18 May 2012                                       *
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
#ifndef DHAVN_IPODCONTROLLER_GETARTWORKDATAUID_H
#define DHAVN_IPODCONTROLLER_GETARTWORKDATAUID_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

namespace ExtendedLingo
{
    class CGetArtworkDataUID : public CIAPPacket
    {
#ifdef DBUGLOGS
    private:
        USE_LOG_UTIL
#endif
    public:
       CGetArtworkDataUID(int formatId,
                                        int timeOffset,
                                        int transId,bool isTidEnabled, quint64 uid);  // Modified by pawan 2013.05.15.
       QByteArray marshall();
        void  unmarshall(QByteArray);

    private:
        int m_FormatId;
        int m_TimeOffset;
        quint64 m_Uid;  // Added by pawan 2013.05.15
    };
}

#endif // DHAVN_IPODCONTROLLER_GETARTWORKDATAUID_H
