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
#ifndef IPODPREFERENCETOKEN_H
#define IPODPREFERENCETOKEN_H
#include <DHAVN_IPodController_FIDToken.h>

namespace GeneralLingo
{

class CIPodPreferenceToken : public CFIDToken
{
public:
    CIPodPreferenceToken();
    QByteArray getToken();
};
}
#endif // IPODPREFERENCETOKEN_H
