/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  BaseMediaPlay                                    *
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
#include <DHAVN_IPodController_setShuffle.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
CSetShuffle::CSetShuffle(int mode,int transId, bool isTidEnabled)
    :CIAPPacket(0x04,0x002E,transId, isTidEnabled),m_ShuffleMode(mode)
{
}

QByteArray CSetShuffle::marshall()
{
    QByteArray command;

    //Packing format =  Big Endian

//    command.resize(10);
    int i = -1;

    command[++i] = 0x55;
    command[++i] = 0x07;
    command[++i] = 0x04;
    command[++i] = 0x00;
    command[++i] = 0x2E;
                if( m_isTidEnabled )
		{
                    command[++i] = ((m_TransactionID & 0xff00) >> 8);
                    command[++i] = m_TransactionID & 0x00ff;
		}
    switch(m_ShuffleMode)
    {
    case 0 :
        {
            command[++i]  = 0x00;
            break;
        }
   case 1 :
        {
            command[++i]  = 0x01;
            break;
        }
   case 2 :
        {
            command[++i]  = 0x02;
            break;
        }
    }
    command[++i] = 0x00;
    command[1] = i-1;
    command.append(CIpodControllerGlobal::checkSum(command, i));
    m_CommandBytes = command;
    return command;}

void  CSetShuffle::unmarshall(QByteArray )
{
}




