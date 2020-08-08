/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
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
#include <DHAVN_IPodController_IdentifyToken.h>
#include <QBitArray>
#include <qdebug.h>
#include <DHAVN_IPodController_CommonVars.h>
using namespace GeneralLingo;

CIdentifyToken::CIdentifyToken(QBitArray supportedLingoArray, QBitArray supportedDeviceOptions) :  CFIDToken(0,0) , numLingoes(0), m_DevideId(0)
{
#ifdef DBUGLOGS
    LOG_INIT_V2(gControllerLogName);
#endif
    supportedLingoes = supportedLingoArray;
    deviceOptions = supportedDeviceOptions;
}

/******************************************************************************
// Function Name                                            Parameters
// CIdentifyToken::getToken()                             (QByteArray) (OUT), (void) (IN)
//
// Explanation
// gets the IAP packet for the CIdentifyToken token
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CIdentifyToken::getToken()
{
#ifdef DBUGLOGS
    //LOG_TRACE << "CIdentifyToken::getToken() Entry " << LOG_ENDL;
#endif
    int count = supportedLingoes.count(true);
#ifdef DBUGLOGS
    //LOG_INFO << "CIdentifyToken::getToken():Supported lingo count = " << count << LOG_ENDL;
#endif
    QByteArray token;
    int sizeOfPacket = 1 + 1 + 1 + count + 4 + 4;
    token.resize(sizeOfPacket+1-8); //adding one byte for the length byte
    token[0] = sizeOfPacket;
    token[1] = 0x00;
    token[2] = 0x00;
    token[3] = count;
    int j=3;
    for(int i=0; i<supportedLingoes.count() ; i++)
    {
        if(supportedLingoes[i])
        {
            j++;
            token[j] = i;
        }
    }
    //Device Options
    QByteArray deviceOptions;
    deviceOptions.resize(4);
    /*bit1:bit0 ---- 00
    bit3:bit2 --- 00 = low power mode, as defined in the section “Accessory Power Policy” in MFi Accessory
    Hardware Specification*/
    //byte1+byte2+byte3+00000010
    deviceOptions[0] = 0;
    deviceOptions[1] = 0;
    deviceOptions[2] = 0;
    deviceOptions[3] = 0x02;

    token.append(deviceOptions);
    //device ID
    //Device Options
    QByteArray deviceID;
    deviceID.resize(4);
    deviceID[0] = 0;
    deviceID[1] = 0;
    deviceID[2] = 0x02;
    deviceID[3] = 0;
    token.append(deviceID);
#ifdef DBUGLOGS
     //LOG_TRACE << "CIdentifyToken::getToken() Exit " << LOG_ENDL;
#endif
    return token;
}
