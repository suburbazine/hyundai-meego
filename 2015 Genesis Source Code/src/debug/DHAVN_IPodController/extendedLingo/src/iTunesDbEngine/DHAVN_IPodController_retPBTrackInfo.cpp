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
#include <DHAVN_IPodController_retPBTrackInfo.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

using namespace ExtendedLingo;

CRetPBTrackInfo::CRetPBTrackInfo( bool isTidEnabled)
    :CIAPPacket(0x04,0x0043,-1, isTidEnabled),m_Data(""),m_InfoType(-1),m_UID(0)
{
    //uninitialized variables - coverity fix
    m_TrackIndex = -1;
    m_TrackDuration = 0;
    m_ChaptorCount = -1;
    m_Capability = -1;
    m_AlbumTrackIndex = -1;
}

QByteArray CRetPBTrackInfo::marshall()
{
    QByteArray command;
    return command;
}

QString CRetPBTrackInfo::getInfoData()
{
    return m_Data.fromUtf8(m_Data.toAscii().data());
}

int CRetPBTrackInfo::getInfoType()
{
    return m_InfoType;
}

int CRetPBTrackInfo::getTrackIndex()
{
    return m_TrackIndex;
}
int CRetPBTrackInfo::getTrackDuration()
{
    return m_TrackDuration;
}

quint64 CRetPBTrackInfo::getUID()
{
    return m_UID;
}

bool CRetPBTrackInfo::hasChapter()
{
    if(m_Capability & 0x00000002)
    {
        return true;
    }
    else
    {
        return false;
    }
}

CapabityType CRetPBTrackInfo::getCaps()
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

void  CRetPBTrackInfo::unmarshall(QByteArray returnCommand)
{
    // Remove the checksum byte from the array in order to use mid to obtain the record string
    //chop(1) removes 1 byte from the end of the array
    returnCommand.chop(1);

    int offsetLarge = 0;

    if( ((int)returnCommand[0] == 0x55) && ((int)returnCommand[1] == 0x00) )
    {
        offsetLarge = 2;  //Lingo Index for Large iAP Packets.
    }

    if( ((int)returnCommand[0] == 0x55) &&
            ((int)returnCommand[offsetLarge + 2] == 0x04) &&
            ((int)returnCommand[offsetLarge + 3] == 0x00) &&
            ((int)returnCommand[offsetLarge + 4] == 0x43) )
    {
        int value = 0;
        int nStartIndex = -1;
        int nEndIndex = -1;
        if( m_isTidEnabled  )
        {
            nStartIndex = offsetLarge + 7;
            nEndIndex = offsetLarge + 11;
        }
        else
        {
            nStartIndex = offsetLarge + 5;
            nEndIndex = offsetLarge + 9;
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
            for(int i = nEndIndex+1;i< nEndIndex+9;i++)
            {
                m_UID <<= 8;     m_UID |= (int)returnCommand[i] & 0xFF;
            }

            //m_Data.setNum(UID);
            break;
        case 0x00:
            for(int i = nEndIndex+1;i< nEndIndex+5;i++)
            {
                m_Capability <<= 8;     m_Capability |= (int)returnCommand[i] & 0xFF;
            }
            break;
        case 6:
            for(int i = nEndIndex+1;i< nEndIndex+5;i++)
            {
                m_TrackDuration <<= 8;     m_TrackDuration |= (int)returnCommand[i] & 0xFF;
            }
            break;
        case 8:
            for(int i = nEndIndex+1;i< nEndIndex+3;i++)
            {
                m_ChaptorCount <<= 8;     m_ChaptorCount |= (int)returnCommand[i] & 0xFF;
            }
            break;
        case 13:
            for(int i = nEndIndex+1;i< nEndIndex+3;i++)
            {
                m_AlbumTrackIndex <<= 8;     m_AlbumTrackIndex |= (int)returnCommand[i] & 0xFF;
            }
            break;
        default:
            m_Data = QString(returnCommand.mid((nEndIndex+1),-1)); // bytearray from 11 to the end
            break;
        }
    }
}



