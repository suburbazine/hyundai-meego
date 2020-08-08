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
#ifndef FIDTOKEN_H
#define FIDTOKEN_H

#include <qbitarray.h>

namespace GeneralLingo
{
    class CFIDToken
    {
    public:
        CFIDToken(qint8 FIDtype, qint8 FIDSubType){m_Type=FIDtype;m_SubType=FIDSubType;}
    private:
        qint8 m_Type;
        qint8 m_SubType;
    };
}
#endif // FIDTOKEN_H
