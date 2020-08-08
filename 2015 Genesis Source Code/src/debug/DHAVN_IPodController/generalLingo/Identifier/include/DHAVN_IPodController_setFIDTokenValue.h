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
#ifndef SETFIDTOKENVALUE_H
#define SETFIDTOKENVALUE_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <QBitArray>
#include <DHAVN_LogUtil.h>
namespace GeneralLingo
{
class FIDTokenValue
{
/*"IdentifyToken" ,"AccCapsToken", "AccInfoToken",
  "iPodPreferenceToken", "EAProtocolToken",
  "BundleSeedIDPrefToken", "ScreenInfoToken",
  "EAProtocolMetadataToken", "MicrophoneCapsToken"*/

public:
    FIDTokenValue(QString tokenName) : FIDType(tokenName){}
    QString FIDType;
};


class CSetFIDTokenValue: public CIAPPacket
{
public:
    CSetFIDTokenValue(int transIdArgs, QList<FIDTokenValue> fidTokenValues, QBitArray supportedLingoArray, QBitArray supportedDeviceOptions,
                      bool isTidEnabled, bool bUsVariantIOSSupport, bool isInterleaved=true);
    QByteArray marshall();
    void  unmarshall(QByteArray);
private:
    int m_TransId;
    QBitArray m_SupportedLingoArray;
    QBitArray m_SupportedDeviceOptions;
    QList<FIDTokenValue> m_FidTokenValues;
    bool m_IsInterleaved;
    bool m_bUsVariantIOSSupport;
};

}

#endif // SETFIDTOKENVALUE_H
