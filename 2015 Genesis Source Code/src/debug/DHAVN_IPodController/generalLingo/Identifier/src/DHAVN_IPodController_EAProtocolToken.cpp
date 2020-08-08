/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  10 Jan 2012                                   *
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
#include <DHAVN_IPodController_EAProtocolToken.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
using namespace GeneralLingo;

CEAProtocolToken::CEAProtocolToken() :  CFIDToken(0,4)
{
    LOG_INIT_V2(gControllerLogName);
}

/******************************************************************************
// Function Name                                            Parameters
// CEAProtocolToken::getToken()                             (QByteArray) (OUT), (void) (IN)
//
// Explanation
// gets the IAP packet for the token
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
//#ifdef DHAVN_COMPILE_FOR_AHA // modified by jungae, 2012.12.01 for AhaRadio application
QByteArray CEAProtocolToken::getPandoraToken()
//#else
//QByteArray CEAProtocolToken::getToken()
//#endif
{
//#ifdef DHAVN_COMPILE_FOR_AHA // modified by jungae, 2012.12.01 for AhaRadio application
    //LOG_TRACE << " CEAProtocolToken::getPandoraToken() Entry" << LOG_ENDL;
//#else
//	//LOG_TRACE << " CEAProtocolToken::getToken() Entry" << LOG_ENDL;
//#endif
    QString pandoraProtocolString("com.pandora.link.v1");
    QByteArray pandoraProtocolArray(pandoraProtocolString.toUtf8());
    QByteArray token;
    int sizeOfPacket = 3 + pandoraProtocolArray.size() + 1;
    token.resize(5); //adding one byte for the length byte
    token[0] = sizeOfPacket;
    token[1] = 0x00;
    token[2] = 0x04;
    token[3] = gPandoraProtocolIndex;
    token.insert(4, pandoraProtocolArray);
    token[sizeOfPacket] = '\0';
    //CIpodControllerGlobal::printArray(token);
//#ifdef DHAVN_COMPILE_FOR_AHA // modified by jungae, 2012.12.01 for AhaRadio application
    //LOG_TRACE << " CEAProtocolToken::getPandoraToken() Exit" << LOG_ENDL;
//#else
//    //LOG_TRACE << " CEAProtocolToken::getToken() Exit" << LOG_ENDL;
//#endif
    return token;
}

//#ifdef DHAVN_COMPILE_FOR_AHA // added by jungae, 2012.12.01 for AhaRadio application
QByteArray CEAProtocolToken::getAhaToken()
{
    //LOG_TRACE << " CEAProtocolToken::getAhaToken() Entry" << LOG_ENDL;
    QString ahaProtocolString("com.ahamobile.link.v1");
    QByteArray ahaProtocolArray(ahaProtocolString.toUtf8());
    QByteArray token;
    int sizeOfPacket = 3 + ahaProtocolArray.size() + 1;
    token.resize(5); //adding one byte for the length byte
    token[0] = sizeOfPacket;
    token[1] = 0x00;
    token[2] = 0x04;
    token[3] = gAhaProtocolIndex;
    token.insert(4, ahaProtocolArray);
    token[sizeOfPacket] = '\0';
    //CIpodControllerGlobal::printArray(token);
    //LOG_TRACE << " CEAProtocolToken::getAhaToken() Exit" << LOG_ENDL;
    return token;
}
//#endif
