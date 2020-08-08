#ifndef DHAVN_IPODCONTROLLER_GETACCSAMPLERATECAPS_H
#define DHAVN_IPODCONTROLLER_GETACCSAMPLERATECAPS_H
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

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>
namespace DigitalAudioLingo
{

class CRetAccSampleRateCaps : public CIAPPacket
{
private:
    USE_LOG_UTIL
public:
    CRetAccSampleRateCaps(int transIdArgs,bool isTidEnabled);
    QByteArray marshall();
    void  unmarshall(QByteArray);
private:
    qint16 m_TransId;
};

}

#endif // DHAVN_IPODCONTROLLER_GETACCSAMPLERATECAPS_H
