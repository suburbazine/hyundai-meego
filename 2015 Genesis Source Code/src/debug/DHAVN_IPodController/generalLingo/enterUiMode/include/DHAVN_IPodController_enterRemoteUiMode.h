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
#ifndef ENTERREMOTEUIMODE_H
#define ENTERREMOTEUIMODE_H

#include <DHAVN_IPodController_iAPPacket.h>
#include <DHAVN_LogUtil.h>

namespace GeneralLingo
{

class CEnterRemoteUiMode: public CIAPPacket
{
public:
    CEnterRemoteUiMode(int transIdArgs,  bool isTidEnabled);
    QByteArray marshall();
    void  unmarshall(QByteArray);
private:
    qint16 m_TransId;
};

}

#endif // ENTERREMOTEUIMODE_H
