#ifndef DHAVN_IPODCONTROLLER_OPENDATASESSION_H
#define DHAVN_IPODCONTROLLER_OPENDATASESSION_H
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

class COpenDataSessionProtocol : public CIAPPacket
{
public:
    COpenDataSessionProtocol(int transIdArgs,  bool isTidEnabled) : m_TransId(transIdArgs)
    {
        m_isTidEnabled = isTidEnabled;
        //uninitialized variables - coverity fix
        m_SessionId = 0;
        m_ProtocolIndex = -1;
    }



    QByteArray marshall()
    {
    return QByteArray(0);
    }

    void  unmarshall(QByteArray returnCommand)
    {
        if( ((int)returnCommand[0] == 0x55) \
                && ((int)returnCommand[2] == 0x00) \
                 &&((int)returnCommand[3] == 0x3f) )
        {

            if(m_isTidEnabled)
            {
                m_ProtocolIndex = returnCommand[8];

                m_SessionId=0;
                for(int i = 6;i<= 7;i++)
                {
                    m_SessionId <<= 8;     m_SessionId |= (int)returnCommand[i] & 0xFF;
                }
                m_TransactionID=0;
                for(int i = 4;i<= 5;i++)
                {
                    m_TransactionID <<= 8;     m_TransactionID |= (int)returnCommand[i] & 0xFF;
                }
            }
            else
            {
                m_ProtocolIndex = returnCommand[6];

                m_SessionId=0;
                for(int i = 4;i<= 5;i++)
                {
                    m_SessionId <<= 8;     m_SessionId |= (int)returnCommand[i] & 0xFF;
                }
            }
        }
    }

    void getProtocolIndexAndSessionId(qint16& sessionId, int& protocolIndex)
    {
        sessionId = m_SessionId;
        protocolIndex = m_ProtocolIndex;
    }
private:
    int m_TransId;
    qint16 m_SessionId;
    int m_ProtocolIndex;

};

#endif // DHAVN_IPODCONTROLLER_OPENDATASESSION_H
