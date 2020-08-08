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
#include <DHAVN_IPodController_BundleSeedIDPrefToken.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
using namespace GeneralLingo;

CBundleSeedIDPrefToken::CBundleSeedIDPrefToken() :  CFIDToken(0,5)
{
    LOG_INIT_V2(gControllerLogName);
}

/******************************************************************************
// Function Name                                            Parameters
// CBundleSeedIDPrefToken::getToken()                             (QByteArray) (OUT), (void) (IN)
//
// Explanation
// gets the IAP packet for the token
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
//#ifdef DHAVN_COMPILE_FOR_AHA // modified by jungae, 2012.12.01 for AhaRadio application
QByteArray CBundleSeedIDPrefToken::getPandoraToken()
//#else
//QByteArray CBundleSeedIDPrefToken::getToken()
//#endif
{
//#ifdef DHAVN_COMPILE_FOR_AHA // modified by jungae, 2012.12.01 for AhaRadio application
    //LOG_TRACE << " CBundleSeedIDPrefToken::getPandoraToken() Entry" << LOG_ENDL;
//#else
//	//LOG_TRACE << " CBundleSeedIDPrefToken::getToken() Entry" << LOG_ENDL;
//#endif
    QString pandoraBundleSeedString("6KVQ6HHK5F");
    QByteArray pandoraBundleSeedArray(pandoraBundleSeedString.toUtf8());

    QByteArray token;
    token.resize(4); //adding one byte for the length byte
    token[0] = 0x0D;
    token[1] = 0x00;
    token[2] = 0x05;
    token.insert(3, pandoraBundleSeedArray);
    token[13] = '\0';
    //CIpodControllerGlobal::printArray(token);
//#ifdef DHAVN_COMPILE_FOR_AHA // modified by jungae, 2012.12.01 for AhaRadio application
    //LOG_TRACE << " CBundleSeedIDPrefToken::getPandoraToken() Exit" << LOG_ENDL;
//#else
//	//LOG_TRACE << " CBundleSeedIDPrefToken::getToken() Exit" << LOG_ENDL;
//#endif
    return token;
}

//#ifdef DHAVN_COMPILE_FOR_AHA // added by jungae, 2012.12.01 for AhaRadio application
QByteArray CBundleSeedIDPrefToken::getAhaToken()
{
    //LOG_TRACE << " CBundleSeedIDPrefToken::getAhaToken() Entry" << LOG_ENDL;
    QString ahaBundleSeedString("2BTMS635EB");
    QByteArray ahaBundleSeedArray(ahaBundleSeedString.toUtf8());
    QByteArray token;
    token.resize(4); //adding one byte for the length byte
    token[0] = 0x0D;
    token[1] = 0x00;
    token[2] = 0x05;
    token.insert(3, ahaBundleSeedArray);
    token[13] = '\0';
    //CIpodControllerGlobal::printArray(token);
    //LOG_TRACE << " CBundleSeedIDPrefToken::getAhaToken() Exit" << LOG_ENDL;
    return token;
}
//#endif