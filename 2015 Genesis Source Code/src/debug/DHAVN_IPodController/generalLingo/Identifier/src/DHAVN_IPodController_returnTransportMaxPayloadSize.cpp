/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Sowmini Philip       *
 * DATE       :  12 November 2011                                   *
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
#include <QByteArray>
#include <DHAVN_IPodController_returnTransportMaxPayloadSize.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

using namespace GeneralLingo;

CReturnTransportMaxPayloadSize::CReturnTransportMaxPayloadSize(int transId, bool isTidEnabled)
    :CIAPPacket(CIAPCmds::GeneralLingoId,CIAPCmds::L0_12_ReturnTransportMaxPayloadSize,transId, isTidEnabled),m_maxPayloadSize(0)
{
}

QByteArray CReturnTransportMaxPayloadSize::marshall()
{
    QByteArray tmp;
    return tmp;
}



void  CReturnTransportMaxPayloadSize::unmarshall(QByteArray returnCommand)
{
    if( ((int)returnCommand[0] == 0x55) &&
        ((int)returnCommand[2] == CIAPCmds::GeneralLingoId) &&
        ((int)returnCommand[3] == CIAPCmds::L0_12_ReturnTransportMaxPayloadSize))
    {
        int i = 4;//start index to read max playload.
        if( m_isTidEnabled )
        {
            i = 6;
        }
        m_maxPayloadSize = ((0x000000FF && returnCommand[i]) << 8) | (0x000000FF && returnCommand[i+1]);
    }
    m_CommandBytes = returnCommand;
}

int CReturnTransportMaxPayloadSize::getMaxPayloadSize()
{
    return m_maxPayloadSize;
}
