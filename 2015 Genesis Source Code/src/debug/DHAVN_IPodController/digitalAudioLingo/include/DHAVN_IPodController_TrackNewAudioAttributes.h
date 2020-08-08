#ifndef DHAVN_IPODCONTROLLER_TRACKNEWAUDIOATTRIBUTES_H
#define DHAVN_IPODCONTROLLER_TRACKNEWAUDIOATTRIBUTES_H
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

class CTrackNewAudioAttributes: public CIAPPacket
{
private:
    USE_LOG_UTIL
public:
    CTrackNewAudioAttributes(int transIdArgs, bool isTidEnabled);
    QByteArray marshall();
    void  unmarshall(QByteArray returnCommand);

    int GetSampleRate();
//    int GetSoundCheckVal();
//    int GetTrackVolumeAdjustment();
private:
    int m_TransId;
    quint32 m_SampleRate;
    quint32 m_SoundCheckVal;
    quint32 m_VolumeAdjustment;
};

}
#endif // DHAVN_IPODCONTROLLER_TRACKNEWAUDIOATTRIBUTES_H
