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
#include <DHAVN_IPodController_getDevAuthenticationSignature.h>
#include <DHAVN_IPodController_CommonVars.h>
using namespace GeneralLingo;

CGetDevAuthenticationSignature::CGetDevAuthenticationSignature(int transIdArgs, bool isTidEnabled) : m_TransId(transIdArgs),m_AuthenticationRetryCounter(-1)
{
//    LOG_INIT_V2(gControllerLogName);
    m_isTidEnabled = isTidEnabled;
   // //LOG_TRACE << "CGetDevAuthenticationSignature::CGetDevAuthenticationSignature" << LOG_ENDL;
}



QByteArray CGetDevAuthenticationSignature::marshall()
{
    return NULL;
}

/******************************************************************************
// Function Name                                            Parameters
// CGetDevAuthenticationSignature::unmarshall              (void) (OUT), (QByteArray) (IN)
//
// Explanation
// decode the data which is returned by the device
// extract the challange byte and store it for the
// future use
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void  CGetDevAuthenticationSignature::unmarshall(QByteArray returnCommand)
{
    // if cmdLength =  0x13 (19)
    int cmdLength = (int)returnCommand[1] ;
    if( ((int)returnCommand[0] == 0x55) &&
        //( cmdLength == 0x19 ) &&
        ((int)returnCommand[2] == 0x00) &&
        ((int)returnCommand[3] == 0x17))
    {
	    int cmdCount = 0;
        m_OfferedChallenge.resize(20);

        if( m_isTidEnabled ){
                cmdCount=6;
        }else{
                cmdCount = 4;
        }
        for(int challengeCounter=0; challengeCounter < 20; cmdCount++,challengeCounter++)
        {
            m_OfferedChallenge[challengeCounter] = returnCommand[cmdCount];
        }

        m_AuthenticationRetryCounter = (int)returnCommand[cmdLength+2]; // m_AuthenticationRetryCounter is byte no 21 = cmdLength+2
    }
}
/*
int CGetDevAuthenticationSignature::getAuthenticationRetryCounter()
{
    return m_AuthenticationRetryCounter;
}
*/
QByteArray CGetDevAuthenticationSignature::getOfferedChallenge()
{
    return m_OfferedChallenge;
}
