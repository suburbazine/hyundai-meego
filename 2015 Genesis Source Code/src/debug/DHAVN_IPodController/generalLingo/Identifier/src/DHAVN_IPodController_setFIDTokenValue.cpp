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
#include <DHAVN_IPodController_setFIDTokenValue.h>
#include <iostream>
#include <DHAVN_IPodController_IdentifyToken.h>
#include <DHAVN_IPodController_AccCapsToken.h>
#include <DHAVN_IPodController_AccInfoToken.h>
#include <DHAVN_IPodController_iPodPreferenceToken.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
#include <DHAVN_IPodController_EAProtocolToken.h>
#include <DHAVN_IPodController_BundleSeedIDPrefToken.h>

using namespace GeneralLingo;

CSetFIDTokenValue::CSetFIDTokenValue(int transIdArgs, QList<FIDTokenValue> fidTokenValues, QBitArray supportedLingoArray, QBitArray supportedDeviceOptions,
                                     bool isTidEnabled, bool bUsVariantIOSSupport, bool isInterleaved) : CIAPPacket()
{
//    LOG_INIT_V2(gControllerLogName);
    m_TransId = transIdArgs;
    m_isTidEnabled = isTidEnabled;
    m_SupportedLingoArray = supportedLingoArray;
    m_SupportedDeviceOptions = supportedDeviceOptions;
    m_FidTokenValues = fidTokenValues;
    m_IsInterleaved = isInterleaved;
    m_bUsVariantIOSSupport = bUsVariantIOSSupport;
}

/******************************************************************************
// Function Name                                            Parameters
// CSetFIDTokenValue::marshall()                          (QByteArray) (OUT), (void) (IN)
//
// Explanation
// Construct the command to set the FID token on the
// basis of the array passed from the caller
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CSetFIDTokenValue::marshall()
{
//    //LOG_TRACE << "CSetFIDTokenValue::marshall() Entry" << LOG_ENDL;
    QByteArray tokenArray;
    tokenArray.resize(0);
    int numberOfToken = 0;
    int count = m_FidTokenValues.count();
    for(int i=0;i<count;i++)
    {
        if(!m_FidTokenValues[i].FIDType.compare("IdentifyToken"))
        {
            CIdentifyToken token(m_SupportedLingoArray, m_SupportedDeviceOptions);
            tokenArray.append(token.getToken());
            numberOfToken++;
        }
        else if(!m_FidTokenValues[i].FIDType.compare("AccCapsToken"))
        {
            CAccCapsToken token(m_IsInterleaved, m_bUsVariantIOSSupport);
            tokenArray.append(token.getToken());
            numberOfToken++;
        }
        else if(!m_FidTokenValues[i].FIDType.compare("AccInfoToken"))
        {
            CAccInfoToken token;
            tokenArray.append(token.getToken());
            numberOfToken += token.getNumberOfToken();
        }
        else if(!m_FidTokenValues[i].FIDType.compare("iPodPreferenceToken"))
        {
            CIPodPreferenceToken token;
            tokenArray.append(token.getToken());
            numberOfToken++;
        }
        else if(!m_FidTokenValues[i].FIDType.compare("EAProtocolToken") && m_bUsVariantIOSSupport)
        {
            CEAProtocolToken pandoraToken, ahaToken;
            tokenArray.append(pandoraToken.getPandoraToken());
            numberOfToken++;
            tokenArray.append(ahaToken.getAhaToken());
            numberOfToken++;
        }
        else if(!m_FidTokenValues[i].FIDType.compare("BundleSeedIDPrefToken")&& m_bUsVariantIOSSupport)
        {
            CBundleSeedIDPrefToken pandoraToken, ahaToken;
            tokenArray.append(pandoraToken.getPandoraToken());
            numberOfToken++;
            tokenArray.append(ahaToken.getAhaToken());
            numberOfToken++;
        }
        else
        {
//            //LOG_TRACE << "Token not supported" << LOG_ENDL;
        }
    }

    QByteArray command;
    int i = -1;
    //command.resize(7);
    command[++i] = 0x55;
    command[++i] = tokenArray.length() + 5;
    command[++i] = 0x00;
    command[++i] = 0x39;
    if( m_isTidEnabled ){
        command[++i] = ((m_TransId & 0xff00) >> 8);
        command[++i] = m_TransId & 0x00ff;
    }
    command[++i] = numberOfToken;
    command.append(tokenArray);
    i += tokenArray.count();
    command[1] = i - 1;
    command.append(CIpodControllerGlobal::checkSum(command, i));

    m_CommandBytes = command;
//    //LOG_TRACE << "CSetFIDTokenValue::marshall() Exit" << LOG_ENDL;
    return command;
}

void  CSetFIDTokenValue::unmarshall(QByteArray)
{

}
