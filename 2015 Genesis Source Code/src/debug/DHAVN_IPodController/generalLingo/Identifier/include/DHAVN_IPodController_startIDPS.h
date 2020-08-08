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
#ifndef STARTIDPS_H
#define STARTIDPS_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace GeneralLingo
{

class CStartIDPS: public CIAPPacket
{
public:
    CStartIDPS(int transIdArgs,  bool isTidEnabled);
    QByteArray marshall();
    void  unmarshall(QByteArray);
private:
    qint16 m_TransId;
};

}

#endif //STARTIDPS_H
