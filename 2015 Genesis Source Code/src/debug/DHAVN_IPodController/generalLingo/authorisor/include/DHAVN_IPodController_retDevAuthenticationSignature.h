/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  BaseMediaPlay                                    *
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
#ifndef RETDEVAUTHENTICATIONSIGNATURE_H
#define RETDEVAUTHENTICATIONSIGNATURE_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>
#include <DHAVN_IPodController_coprocessorDataProvider.h>

namespace GeneralLingo
{

class CRetDevAuthenticationSignature: public CIAPPacket
{
public:
    CRetDevAuthenticationSignature(bool isTidEnabled, int transIdArgs,
                                   QByteArray achallenge, int lengthOfchallenge,
                                   CCoProcessorDataProvider* dataProvider);
    QByteArray marshall();
    void  unmarshall(QByteArray);

private:
//    QByteArray getDigiSignature();
private:
    qint16 m_TransId;
    int m_LengthOfchallenge;
    QByteArray m_Challenge;
    CCoProcessorDataProvider* m_dataProvider;
};

}


#endif // RETDEVAUTHENTICATIONSIGNATURE_H
