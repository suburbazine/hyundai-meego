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

#include <DHAVN_IPodController_getDevAuthenticationInfo.h>
#include <DHAVN_IPodController_CommonVars.h>

using namespace GeneralLingo;

CGetDevAuthenticationInfo::CGetDevAuthenticationInfo(int transIdArgs, bool isTidEnabled) :  m_TransId(transIdArgs)
{
//    LOG_INIT_V2(gControllerLogName);
    m_isTidEnabled = isTidEnabled;
}



QByteArray CGetDevAuthenticationInfo::marshall()
{
    return NULL;
}

void  CGetDevAuthenticationInfo::unmarshall(QByteArray returnCommand)
{
    if(((int)returnCommand[0] == 0x55) &&
     //  ((int)returnCommand[1] == 0x02) &&
       ((int)returnCommand[2] == 0x00) &&
       ((int)returnCommand[3] == 0x14))
    {
        if((int) returnCommand[1] == 0x04)
        {
            m_TransactionID = 0;
            int nStartIndex = 4;
            int nEndIndex = 5;
            for(int i = nStartIndex;i<= nEndIndex;i++)
            {
                m_TransactionID <<= 8;     m_TransactionID |= (int)returnCommand[i] & 0xFF;
            }
        }
//        //LOG_TRACE << "CGetDevAuthenticationInfo::unmarshall(QByteArray) successfull" << LOG_ENDL;
    }
}
