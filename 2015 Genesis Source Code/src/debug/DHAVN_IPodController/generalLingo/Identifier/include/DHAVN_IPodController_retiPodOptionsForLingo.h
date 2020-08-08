/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
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
#ifndef RETIPODOPTIONSFORLINGO_H
#define RETIPODOPTIONSFORLINGO_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace GeneralLingo
{

class CRetiPodOptionsForLingo: public CIAPPacket
{
#ifdef DBUGLOGS
private:
    USE_LOG_UTIL
#endif
public:
    CRetiPodOptionsForLingo(int transIdArgs,  bool isTidEnabled);
    QByteArray marshall();
    void  unmarshall(QByteArray returnCommand);
    int getLingoType();
    bool isLogoSupported();
    bool isiTunesTaggingSupported();
    double getExtendedLingoVersion();
    bool isiOSAppCommunicationSupported();
private:
    int m_TransId;
    int m_LingoType;
    bool m_iTunesTaggingSupport;
    bool m_isLogoSupported;
    double m_ExtendedLingoVersion;
    bool m_iOSAppCommunicationSupported;
};

}

#endif // RETIPODOPTIONSFORLINGO_H
