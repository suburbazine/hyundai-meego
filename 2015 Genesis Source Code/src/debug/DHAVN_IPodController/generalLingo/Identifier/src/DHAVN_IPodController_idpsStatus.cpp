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
#include <DHAVN_IPodController_idpsStatus.h>
#include <DHAVN_IPodController_CommonVars.h>

using namespace GeneralLingo;

CIdpsStatus::CIdpsStatus(int transIdArgs,  bool isTidEnabled) : m_TransId(transIdArgs), m_idpsStatus(-1)
{
    m_isTidEnabled = isTidEnabled;
}



QByteArray CIdpsStatus::marshall()
{
return QByteArray(0);
}

void  CIdpsStatus::unmarshall(QByteArray returnCommand)
{
    if( ((int)returnCommand[0] == 0x55) \
            && ((int)returnCommand[2] == 0x00) \
             &&((int)returnCommand[3] == 0x3C) )
    {

        m_idpsStatus = (int)returnCommand[6];

    }
}

int CIdpsStatus::getIdpsStatus()
{
    return m_idpsStatus;
}
