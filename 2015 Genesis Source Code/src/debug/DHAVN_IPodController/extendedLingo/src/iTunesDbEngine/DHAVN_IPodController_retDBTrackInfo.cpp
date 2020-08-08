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
#include <DHAVN_IPodController_retDBTrackInfo.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

using namespace ExtendedLingo;

CRetDBTrackInfo::CRetDBTrackInfo( bool isTidEnabled)
    :CIAPPacket(0x04,0x0041,-1, isTidEnabled),m_Data(""),m_InfoType(-1),m_TrackDuration(0)
{
    // unintialized variables. coverity fix
    m_TrackIndex = -1;
    m_ChaptorCount = -1;
    m_AlbumTrackIndex = -1;
    m_Capability = -1;
    m_UID = 0;
}

QByteArray CRetDBTrackInfo::marshall()
{
    QByteArray command;
    return command;
}

QString CRetDBTrackInfo::getInfoData()
{
    return m_Data.fromUtf8(m_Data.toAscii().data());
}

int CRetDBTrackInfo::getInfoType()
{
    return m_InfoType;
}

int CRetDBTrackInfo::getTrackIndex()
{
    return m_TrackIndex;
}

int CRetDBTrackInfo::getTrackDuration()
{
    return m_TrackDuration;
}

CapabityType CRetDBTrackInfo::getCaps()
{
    CapabityType type = isTrack;
    if(m_Capability & 0x00000001)
        type = isAudioBook;
    if(m_Capability & 0x00000010)
        type = isPodcast;
    if(m_Capability & 0x00004000)
        type = isITuneEpisode;
    return type;
}

void  CRetDBTrackInfo::unmarshall(QByteArray returnCommand)
{
    // Remove the checksum byte from the array in order to use mid to obtain the record string
    //chop(1) removes 1 byte from the end of the array
    returnCommand.chop(1);
    if( ((int)returnCommand[0] == 0x55) &&
            ((int)returnCommand[2] == 0x04) &&
            ((int)returnCommand[3] == 0x00) &&
            ((int)returnCommand[4] == 0x41) ){
        int value = 0;
        int nStartIndex = -1;
        int nEndIndex = -1;
        if( m_isTidEnabled  )
        {
            nStartIndex = 7;
            nEndIndex = 11;
        }
        else
        {
            nStartIndex = 5;
            nEndIndex = 9;
        }
        for(int i = nStartIndex;i< nEndIndex;i++)
        {
            value <<= 8;     value |= (int)returnCommand[i] & 0xFF;
        }
        m_TrackIndex = value;
        m_InfoType = (int)returnCommand[nEndIndex];

        switch(m_InfoType)
        {
        case 0x07:
        {
            m_UID = 0;

            for(int i = nEndIndex+1;i< nEndIndex+9;i++)
            {
                m_UID <<= 8;     m_UID |= (quint8)returnCommand[i] & 0xFF;
            }
            break;
        }
        case 0x00:
        {
            for(int i = nEndIndex+1;i< nEndIndex+5;i++)
            {
                m_Capability <<= 8;     m_Capability |= (int)returnCommand[i] & 0xFF;
            }
            break;
        }
        case 6:
        {
            for(int i = nEndIndex+1;i< nEndIndex+5;i++)
            {
                m_TrackDuration <<= 8;     m_TrackDuration |= (int)returnCommand[i] & 0xFF;
            }
            break;
        }
        case 8:
        {
            for(int i = nEndIndex+1;i< nEndIndex+3;i++)
            {
                m_ChaptorCount <<= 8;     m_ChaptorCount |= (int)returnCommand[i] & 0xFF;
            }
            break;
        }
        case 13:
        {
            for(int i = nEndIndex+1;i< nEndIndex+3;i++)
            {
                m_AlbumTrackIndex <<= 8;     m_AlbumTrackIndex |= (int)returnCommand[i] & 0xFF;
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



