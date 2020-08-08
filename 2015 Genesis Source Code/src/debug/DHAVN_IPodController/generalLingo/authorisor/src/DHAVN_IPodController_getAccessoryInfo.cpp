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

#include <DHAVN_IPodController_getAccessoryInfo.h>
#include <DHAVN_IPodController_CommonVars.h>
using namespace GeneralLingo;

CGetAccessoryInfo::CGetAccessoryInfo(int transIdArgs) : m_TransId(transIdArgs),m_InFoType(-1)
{
//LOG_INIT_V2(gControllerLogName);
}



QByteArray CGetAccessoryInfo::marshall()
{
    return NULL;
}

void  CGetAccessoryInfo::unmarshall(QByteArray returnCommand)
{
    if(((int)returnCommand[0] == 0x55) &&
       ((int)returnCommand[1] == 0x03) &&
       ((int)returnCommand[2] == 0x00) &&
       ((int)returnCommand[3] == 0x27))
    {
//        //LOG_INFO << "CGetAccessoryInfo::unmarshall(QByteArray) successfull infoType= " << m_InFoType << LOG_ENDL;
        m_InFoType = (int)returnCommand[4];
    }
}

int CGetAccessoryInfo::getInfoType()
{
        return m_InFoType;
}

