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
#ifndef IDPSSTATUS_H
#define IDPSSTATUS_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace GeneralLingo
{

class CIdpsStatus: public CIAPPacket
{
public:
    CIdpsStatus(int transIdArgs,  bool isTidEnabled);
    QByteArray marshall();
    void  unmarshall(QByteArray);
    int getIdpsStatus();

private:
    int m_TransId;
    int m_idpsStatus;
};

}
#endif // IDPSSTATUS_H
