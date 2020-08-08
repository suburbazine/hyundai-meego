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
#include <DHAVN_IPodController_retFIDTokenValue.h>

using namespace GeneralLingo;

CRetFIDTokenValue::CRetFIDTokenValue(int transIdArgs, bool isTidEnabled): m_TransId(transIdArgs)
{
    LOG_INIT_V2("Media_IPodController");
        m_AccCapsStatus = true;
        m_isTidEnabled = isTidEnabled;
}



QByteArray CRetFIDTokenValue::marshall()
{
return QByteArray(0);
}

void  CRetFIDTokenValue::unmarshall(QByteArray returnCommand)
{
    if( ((int)returnCommand[0] == 0x55) &&
            ((int)returnCommand[2] == 0x00) &&
            ((int)returnCommand[3] == 0x3A) &&
            ((int)returnCommand[4] == (m_TransId & 0xff00)) &&
            ((int)returnCommand[5] == (m_TransId & 0x00ff)))

    {
        int numOfTokenAck = returnCommand[6];
        int startOfNextTokenAck = 7;
        for(int i=0; i<numOfTokenAck;i++)
        {
            int LenOfToken = returnCommand[startOfNextTokenAck];

            if((int8_t)returnCommand[startOfNextTokenAck+1] == 0x00 && (int8_t)returnCommand[startOfNextTokenAck+2] == 0x01)
            {
                if((int8_t)returnCommand[startOfNextTokenAck+3] != 0x00)
                    m_AccCapsStatus = false;
               // //LOG_TRACE << "CRetFIDTokenValue AccCapstoken : status: " << m_AccCapsStatus << LOG_ENDL;
            }
            //LOG_TRACE << "CRetFIDTokenValue: status: " << m_AccCapsStatus << LOG_ENDL;
            printTheStatusOfAck(returnCommand, startOfNextTokenAck, LenOfToken);

            startOfNextTokenAck = startOfNextTokenAck + LenOfToken +1;
        }
    }
}
void CRetFIDTokenValue::printTheStatusOfAck(QByteArray /*returnCommand*/, int /*startOfNextTokenAck*/, int /*LenOfToken*/)
{
//    //LOG_INFO << "CRetFIDTokenValue::printTheStatusOfAck-> FID type  = " << returnCommand[startOfNextTokenAck+1] << LOG_ENDL;
//    //LOG_INFO << "CRetFIDTokenValue::printTheStatusOfAck-> FID sub type  = " << returnCommand[startOfNextTokenAck+2] << LOG_ENDL;
//    //LOG_INFO << "CRetFIDTokenValue::printTheStatusOfAck-> FID Ack Status  = " << returnCommand[startOfNextTokenAck+3] << LOG_ENDL;
}
