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
#ifndef ACCCAPSTOKEN_H
#define ACCCAPSTOKEN_H

#include <DHAVN_IPodController_FIDToken.h>
#include <DHAVN_LogUtil.h>

namespace GeneralLingo
{

class CAccCapsToken : public CFIDToken
{
public:
    CAccCapsToken(bool isInterleaved, bool bUsVariantIOSSupport);
    QByteArray getToken();
private:
    bool m_IsInterleaved;
    bool m_bUsVariantIOSSupport;
};
}

#endif // ACCCAPSTOKEN_H
