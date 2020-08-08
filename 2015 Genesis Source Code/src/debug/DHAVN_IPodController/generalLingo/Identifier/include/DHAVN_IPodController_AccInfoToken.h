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
#ifndef ACCINFOTOKEN_H
#define ACCINFOTOKEN_H

#include <DHAVN_IPodController_FIDToken.h>
#include <DHAVN_LogUtil.h>
namespace GeneralLingo
{

class CAccInfoToken : public CFIDToken
{
public:
    CAccInfoToken();
    QByteArray getToken();
    int getNumberOfToken();
};
}

#endif // ACCINFOTOKEN_H
