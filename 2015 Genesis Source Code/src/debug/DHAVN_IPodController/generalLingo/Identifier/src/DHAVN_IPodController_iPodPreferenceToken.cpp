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
#include <DHAVN_IPodController_iPodPreferenceToken.h>
#include <QBitArray>
#include <qdebug.h>

using namespace GeneralLingo;

CIPodPreferenceToken::CIPodPreferenceToken() :  CFIDToken(0,2)
{

}

/******************************************************************************
// Function Name                                            Parameters
// CIPodPreferenceToken::getToken()                        (QByteArray) (OUT), (void) (IN)
//
// Explanation
// Construct the command to get the CIPodPreferenceToken token
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CIPodPreferenceToken::getToken()
{
    QByteArray token;
    token.resize(6); //adding one byte for the length byte
    token[0] = 0x05;
    token[1] = 0x00;
    token[2] = 0x03;
    token[3] = 0x03; //CHANGE WHEN IN USE
    token[4] = 0x01; //CHANGE WHEN IN USE
    token[5] = 0x01;
return token;
}


