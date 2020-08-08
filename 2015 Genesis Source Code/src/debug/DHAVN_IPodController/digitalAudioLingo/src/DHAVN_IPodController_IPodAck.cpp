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
#include <DHAVN_IPodController_IPodAck.h>
#include <DHAVN_IPodController_CommonVars.h>

using namespace DigitalAudioLingo;

CIPodAck::CIPodAck(int transIdArgs,  bool isTidEnabled) :  m_TransId(transIdArgs)
{
    LOG_INIT_V2(gControllerLogName);
    //LOG_TRACE << "in CIPodAck constructor" << LOG_ENDL;
    m_isTidEnabled = isTidEnabled;
    m_Status = -1;
    m_CommadId = -1;
}



QByteArray CIPodAck::marshall()
{
    return NULL;
}

void  CIPodAck::unmarshall(QByteArray returnCommand)
{
    if( ((int)returnCommand[0] == 0x55) &&
        ((int)returnCommand[2] == 0x0A) &&
        ((int)returnCommand[3] == 0x01) )
    {
        int i = 4;
        if(m_isTidEnabled){
            i = 6;
        }
        m_Status = returnCommand[i];
        m_CommadId = returnCommand[i+1];
    }
}

/*
int CIPodAck :: ReturnStatus()
{
    return m_Status;
}

int CIPodAck :: AckCommand()
{
    return m_CommadId;
}
*/



