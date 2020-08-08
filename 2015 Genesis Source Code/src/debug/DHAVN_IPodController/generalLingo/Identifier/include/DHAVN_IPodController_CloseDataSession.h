#ifndef DHAVN_IPODCONTROLLER_CLOSEDATASESSION_H
#define DHAVN_IPODCONTROLLER_CLOSEDATASESSION_H
/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  30 Jan 2012                                   *
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
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_iAPPacket.h>

using namespace GeneralLingo;

class CCloseDataSessionProtocol : public CIAPPacket
{
public:
    CCloseDataSessionProtocol(int transIdArgs,  bool isTidEnabled) :  m_TransId(transIdArgs)
    {
        m_isTidEnabled = isTidEnabled;
        m_SessionId = 0; //uninitialized variable - coverity fix
    }



    QByteArray marshall()
    {
    return QByteArray(0);
    }

    void  unmarshall(QByteArray returnCommand)
    {
        if( ((int)returnCommand[0] == 0x55) \
                && ((int)returnCommand[2] == 0x00) \
                 &&((int)returnCommand[3] == 0x40) )
        {
			// { modified by Tan 2013.01.05
			// The m_SessionID is a member variable
			// But when calling unmarshall, it is defined again as  local variable with qint16 m_SessionID. 
			//qint16 m_SessionId=0;
			m_SessionId=0;
			// } modified by Tan
            for(int i = 6;i<= 7;i++)
            {
                m_SessionId <<= 8;     m_SessionId |= (int)returnCommand[i] & 0xFF;
            }

        }
    }

    void getSessionId(qint16& sessionId)
    {
        sessionId = m_SessionId;
    }
private:
    int m_TransId;
    qint16 m_SessionId;
};
#endif // DHAVN_IPODCONTROLLER_CLOSEDATASESSION_H
