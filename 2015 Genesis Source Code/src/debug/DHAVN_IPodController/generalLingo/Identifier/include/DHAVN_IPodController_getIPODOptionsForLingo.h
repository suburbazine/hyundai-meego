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
#ifndef GETIPODOPTIONSFORLINGO_H
#define GETIPODOPTIONSFORLINGO_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>
namespace GeneralLingo
{
    enum LingoType
    {
        GeneralLing =0x00,
        SimpleRemote=0x02,
        DisplayRemote=0x03,
        ExtendedInterface=0x04,
        AccessoryPower=0x05,
        USBHostMode=0x06,
        RFTuner=0x07,
        AccessoryEqualizer=0x08,
        Sports=0x09,
        DigitalAudio=0x0A,
        Storage=0x0C,
        IpodOut=0x0D,
        Location=0x0E
    };
class CGetIPODOptionsForLingo: public CIAPPacket
{
public:
    CGetIPODOptionsForLingo(int transIdArgs, LingoType type, bool isTidEnabled);
    QByteArray marshall();
    void  unmarshall(QByteArray);
private:
    qint16 m_TransId;
    LingoType m_LingoType;
};

}

#endif // GETIPODOPTIONSFORLINGO_H
