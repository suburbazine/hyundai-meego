
/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  BaseMediaPlay                                    *
 *
 * PROGRAMMERS : Sowmini Philip / Venkataramana      *
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
#include <DHAVN_IPodController_retUIDTrackInfo.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

using namespace ExtendedLingo;

CRetUIDTrackInfo::CRetUIDTrackInfo( bool isTidEnabled)
    :CIAPPacket(0x04,0x003F,-1, isTidEnabled),m_Data(""),m_InfoType(-1),m_UID(0)
{
    m_TrackDuration = 0;
}

QByteArray CRetUIDTrackInfo::marshall()
{
    QByteArray command;
    return command;
}

QString CRetUIDTrackInfo::getInfoData()
{
    return m_Data.fromUtf8(m_Data.toAscii().data());
}

int CRetUIDTrackInfo::getInfoType()
{
    return m_InfoType;
}

int CRetUIDTrackInfo::getTrackDuration()
{
    return m_TrackDuration;
}

quint64 CRetUIDTrackInfo::getUID()
{
    return m_UID;
}

void  CRetUIDTrackInfo::unmarshall(QByteArray returnCommand)
{
    // Remove the checksum byte from the array in order to use mid to obtain the record string
    //chop(1) removes 1 byte from the end of the array
    returnCommand.chop(1);
    if( ((int)returnCommand[0] == 0x55) &&
            ((int)returnCommand[2] == 0x04) &&
            ((int)returnCommand[3] == 0x00) &&
            ((int)returnCommand[4] == 0x3F) ){
        quint64 uidVal=0;
        int nStartIndex = -1;
        int nEndIndex = -1;
        if( m_isTidEnabled  )
        {
            nStartIndex = 7;
            nEndIndex = 15;
        }
        else
        {
            nStartIndex = 5;
            nEndIndex = 13;
        }
        for(int i = nStartIndex;i< nEndIndex;i++)
        {
            uidVal <<= 8;     uidVal |= (int)returnCommand[i] & 0xFF;
        }
        m_UID = uidVal;

        m_InfoType = (int)returnCommand[nEndIndex];

        switch(m_InfoType)
        {
        case 6:
        {
            for(int i = nEndIndex+1;i< nEndIndex+5;i++)
            {
                m_TrackDuration <<= 8;     m_TrackDuration |= (int)returnCommand[i] & 0xFF;
            }
            break;
        }
        default:
        {
            m_Data = QString(returnCommand.mid((nEndIndex+1),-1)); // bytearray from 11 to the end
            break;
        }
        }
    }
}



