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
#include <DHAVN_IPodController_ackDevAuthenticationInfo.h>
#include <DHAVN_IPodController_CommonVars.h>
using namespace GeneralLingo;

CAckDevAuthenticationInfo::CAckDevAuthenticationInfo(int transIdArgs, bool isTidEnabled): m_TransId(transIdArgs),m_AuthenticationStatus(-1)
{
//    LOG_INIT_V2(gControllerLogName);
    m_isTidEnabled = isTidEnabled;
   // //LOG_TRACE << "CAckDevAuthenticationInfo::CAckDevAuthenticationInfo Entry " << LOG_ENDL;
}



QByteArray CAckDevAuthenticationInfo::marshall()
{
   // //LOG_TRACE << "CAckDevAuthenticationInfo::marshall" << LOG_ENDL;
    return NULL;
}

void  CAckDevAuthenticationInfo::unmarshall(QByteArray returnCommand)
{
    if( ((int)returnCommand[0] == 0x55) &&
       // ((int)returnCommand[1] == 0x03) &&
        ((int)returnCommand[2] == 0x00) &&
        ((int)returnCommand[3] == 0x16))
    {
        if( m_isTidEnabled ){
            m_AuthenticationStatus = (int)returnCommand[6];
        }else{
            m_AuthenticationStatus = (int)returnCommand[4];
        }
//       //LOG_INFO << "Authentication info Status = " << m_AuthenticationStatus << LOG_ENDL;
    }
}

int CAckDevAuthenticationInfo::getAuthenticationInfoStatus()
{
    return m_AuthenticationStatus;
}
