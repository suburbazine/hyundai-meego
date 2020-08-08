/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Sowmini Philip / Venkataramana      *
 * DATE       :  2 Jan 2012                                   *
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
#include <DHAVN_IPodController_openiPodFeatureFile.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
 
using namespace StorageLingo;

COpeniPodFeatureFile::COpeniPodFeatureFile( int transId, bool isTidEnabled)
        :CIAPPacket(0x0C,0x12,transId, isTidEnabled)
{
}

QByteArray COpeniPodFeatureFile::marshall()
{
    QByteArray command;
    int i = -1;
    //Packing format =  Big Endian
    command[++i] = 0x55;
    command[++i] = 0x02;
    command[++i] = 0x0C;
    command[++i] = 0x12;

    if( m_isTidEnabled )
    {
        command[++i] = ((m_TransactionID & 0xff00) >> 8);
        command[++i] = m_TransactionID & 0x00ff;
    }

    command[++i] = 0x01; // iTunes tagging type
//file options mask
#if 0
    command[++i] = 0x00;
    command[++i] = 0x00;
    command[++i] = 0x00;
    command[++i] = 0x00;

    command[++i] = 0x00;
#endif
    command[1] = i-1;
    command.append(CIpodControllerGlobal::checkSum(command, i));
    m_CommandBytes = command;
    return command;
}

void  COpeniPodFeatureFile::unmarshall(QByteArray)
{
}

