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
#include <DHAVN_IPodController_AccCapsToken.h>
#include <QBitArray>
#include <DHAVN_IPodController_CommonVars.h>
using namespace GeneralLingo;

CAccCapsToken::CAccCapsToken(bool isInterleaved, bool bUsVariantIOSSupport) :  CFIDToken(0,1)
{
    m_IsInterleaved = isInterleaved;
    m_bUsVariantIOSSupport = bUsVariantIOSSupport;
//    LOG_INIT_V2(gControllerLogName);
}

/******************************************************************************
// Function Name                                            Parameters
// CAccCapsToken::getToken()                             (QByteArray) (OUT), (void) (IN)
//
// Explanation
// gets the IAP packet for the token
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CAccCapsToken::getToken()
{
//    //LOG_TRACE << "CAccCapsToken::getToken() Entry " << LOG_ENDL;
    QByteArray token;
    int sizeOfPacket = 10; //1+1+8
    token.resize(sizeOfPacket+1); //adding one byte for the length byte
    token[0] = sizeOfPacket;
    token[1] = 0x00;
    token[2] = 0x01;
    token[3] = 0x00;
    token[4] = 0x00;
    token[5] = 0x00;
    token[6] = 0x00;
    token[7] = 0x00;
    if(m_IsInterleaved)
        token[8] = 0x08;
    else
        token[8] = 0x00;
    if(m_bUsVariantIOSSupport)
    {
        token[9] = 0x02;
    }
    else
    {
        token[9] = 0x0;
    }
    token[10] = 0x10;
//    //LOG_TRACE << "CAccCapsToken::getToken() Exit " << LOG_ENDL;
    return token;
}

