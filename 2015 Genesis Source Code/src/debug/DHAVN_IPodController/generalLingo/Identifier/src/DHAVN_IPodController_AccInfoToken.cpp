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
#include <DHAVN_IPodController_AccInfoToken.h>
#include <QBitArray>
#include <qdebug.h>
#include <DHAVN_IPodController_CommonVars.h>
using namespace GeneralLingo;

CAccInfoToken::CAccInfoToken() :  CFIDToken(0,2)
{
//LOG_INIT_V2(gControllerLogName);
}

/******************************************************************************
// Function Name                                            Parameters
// CAccInfoToken::getToken()                             (QByteArray) (OUT), (void) (IN)
//
// Explanation
// gets the IAP packet for the CAccInfoToken token
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CAccInfoToken::getToken()
{
//    //LOG_TRACE << "CAccInfoToken::getToken() Entry " << LOG_ENDL;
//    QByteArray token1;
      int sizeOfPacket = 19;
//    token1.resize(sizeOfPacket+1); //adding one byte for the length byte
//    token1[0] = sizeOfPacket;
//    token1[1] = 0x00;
//    token1[2] = 0x02;
//    token1[3] = 0x01; /*accInfoType = name */
//    token1[4] = 'T';
//    token1[5] = 'e';
//    token1[6] = 's';
//    token1[7] = 't';
//    token1[8] = 0x00;
    QByteArray token1;
    token1.resize(sizeOfPacket+1); //adding one byte for the length byte
    token1[0] = sizeOfPacket;
    token1[1] = 0x00;
    token1[2] = 0x02;
    token1[3] = 0x01; /* AccInfoType = name */
    token1[4] = 'D';
    token1[5] = 'H';
    token1[6] = ' ';
    token1[7] = 'H';
    token1[8] = 'I';
    token1[9] = 'G';
    token1[10] = 'H';
    token1[11] = ' ';
    token1[12] = 'A';
    token1[13] = 'V';
    token1[14] = 'N';
    token1[15] = ' ';
    token1[16] = '2';
    token1[17] = '.';
    token1[18] = '0';
    token1[19] = 0x00;

    QByteArray token2;
    sizeOfPacket = 6;
    token2.resize(sizeOfPacket+1); //adding one byte for the length byte
    token2[0] = sizeOfPacket;
    token2[1] = 0x00;
    token2[2] = 0x02;
    token2[3] = 0x04; /*accInfoType = fw version */
    token2[4] = 0x00;
    token2[5] = 0x00;
    token2[6] = 0x01;


    QByteArray token3;
    sizeOfPacket = 6;
    token3.resize(sizeOfPacket+1); //adding one byte for the length byte
    token3[0] = sizeOfPacket;
    token3[1] = 0x00;
    token3[2] = 0x02;
    token3[3] = 0x05;/*accInfoType = hw version */
    token3[4] = 0x00;
    token3[5] = 0x00;
    token3[6] = 0x01;

    QByteArray token4;
    sizeOfPacket = 7;
    token4.resize(sizeOfPacket+1); //adding one byte for the length byte
    token4[0] = sizeOfPacket;
    token4[1] = 0x00;
    token4[2] = 0x02;
    token4[3] = 0x06;
    token4[4] = 'L';
    token4[5] = 'G';
    token4[6] = 'E';
    token4[7] = 0x00;

    QByteArray token5;
    sizeOfPacket = 7;
    token5.resize(sizeOfPacket+1); //adding one byte for the length byte
    token5[0] = sizeOfPacket;
    token5[1] = 0x00;
    token5[2] = 0x02;
    token5[3] = 0x07;/*accInfoType = model number*/
    token5[4] = '0';
    token5[5] = '.';
    token5[6] = '1';
    token5[7] = 0x00;

    QByteArray token6;
    sizeOfPacket = 7;
    token6.resize(sizeOfPacket+1); //adding one byte for the length byte
    token6[0] = sizeOfPacket;
    token6[1] = 0x00;
    token6[2] = 0x02;
    token6[3] = 0x0c; /*accInfoType = rf certification*/
    token6[4] = 0x00;
    token6[5] = 0x00;
    token6[6] = 0x00;
    token6[7] = 0x00;


    QByteArray token7;
    sizeOfPacket = 5;
    token7.resize(sizeOfPacket+1); //adding one byte for the length byte
    token7[0] = sizeOfPacket;
    token7[1] = 0x00;
    token7[2] = 0x02;
    token7[3] = 0x09; /*Incoming max packet size*/
    token7[4] = 0xFF;
    token7[5] = 0xF0;


    QByteArray token;
    token.append(token1);
    token.append(token2);
    token.append(token3);
    token.append(token4);
    token.append(token5);
    token.append(token6);
    token.append(token7);
//    //LOG_TRACE << "CAccInfoToken::getToken() Exit " << LOG_ENDL;
return token;
}

int CAccInfoToken::getNumberOfToken()
{
return 7;
}


