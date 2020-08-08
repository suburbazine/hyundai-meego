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
#ifndef IDENTIFYTOKEN_H
#define IDENTIFYTOKEN_H

#include <DHAVN_IPodController_FIDToken.h>
#include <qstring.h>
#include <DHAVN_LogUtil.h>

namespace GeneralLingo
{
class CIdentifyToken : public CFIDToken
{
#ifdef DBUGLOGS
private:
    USE_LOG_UTIL
#endif
public:
    CIdentifyToken(QBitArray supportedLingoArray, QBitArray supportedDeviceOptions);
    QByteArray getToken();
private:
int numLingoes;
QBitArray supportedLingoes;
QBitArray deviceOptions;
int m_DevideId;
};
}


#endif // IDENTIFYTOKEN_H
