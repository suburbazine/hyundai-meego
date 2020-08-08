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
#include <DHAVN_IPodController_retDevAuthenticationSignature.h>
#include <DHAVN_IPodController_CommonVars.h>

#include <DHAVN_IPodController_iPodControllerGlobal.h>
#include <DHAVN_IPodController_Def.h>
using namespace GeneralLingo;

CRetDevAuthenticationSignature::CRetDevAuthenticationSignature(bool isTidEnabled, int transIdArgs, QByteArray achallenge, int challengeLength,CCoProcessorDataProvider* dataProvider)
    : m_TransId(transIdArgs), m_LengthOfchallenge(challengeLength), m_Challenge(achallenge)
{
//    LOG_INIT_V2(gControllerLogName);
    m_isTidEnabled = isTidEnabled;
    m_dataProvider = dataProvider;
    ////LOG_TRACE << "CRetDevAuthenticationSignature::CRetDevAuthenticationSignature Entry " << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CRetDevAuthenticationSignature::marshall                 (QByteArray) (OUT), (void) (IN)
//
// Explanation
// Construct the command
// gets the digital signature from the data provider on the basis of the chall bytes
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CRetDevAuthenticationSignature::marshall()
{
//    //LOG_TRACE << "CRetDevAuthenticationSignature::marshall() Entry " << LOG_ENDL;
    QByteArray command;
    int i = -1;
    //Packing format =  Big Endianll
    //command.resize(6);
    command[++i] = 0x55;
    command[++i] = 0x00;
    command[++i] = 0x00;
    command[++i] = 0x18;
    if(m_isTidEnabled){
        command[++i] = ((m_TransId & 0xff00) >> 8);
        command[++i] = m_TransId & 0x00ff;
    }

   // CCoProcessorDataProvider* dataProvider = parent()->findChild<CCoProcessorDataProvider*>(gCoprocessorDataProvider);
//    //LOG_TRACE << "CRetDevAuthenticationSignature::marshall() get signature " << LOG_ENDL;
    QByteArray arr = m_dataProvider->digitalSignature(m_Challenge);


//    //LOG_TRACE << "signature bytes in CRetDevAuthenticationSignature::marshall()" << LOG_ENDL;
    //CIpodControllerGlobal::printArray(arr);
    command.append(arr);
    i += arr.count();
    command[1] = i -1;
//    //LOG_TRACE << "CRetDevAuthenticationSignature::marshall() get signature1" << LOG_ENDL;
    command.append(CIpodControllerGlobal::checkSum(command,i ));
//    //LOG_INFO << "CRetDevAuthenticationSignature::marshall():Length of this command = " <<  command.length() << LOG_ENDL;
    m_CommandBytes = command;
//    //LOG_TRACE << "CRetDevAuthenticationSignature::marshall() Exit" << LOG_ENDL;
    return command;
}

void  CRetDevAuthenticationSignature::unmarshall(QByteArray /*returnCommand*/)
{

}
/*
QByteArray CRetDevAuthenticationSignature::getDigiSignature()
{
    return QByteArray(0);
}
*/
