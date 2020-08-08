/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS :  Gupta/ Sowmini Philip / Venkataramana      *
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
#include <DHAVN_IPodController_retArtworkDataUID.h>

using namespace ExtendedLingo;

CRetArtworkDataUID::CRetArtworkDataUID( bool isTidEnabled)
    :CIAPPacket(0x04,CIAPCmds::L4_4F_RetTrackArtworkDataUID,-1,isTidEnabled),
      m_ArtworkFormat(-1),
      m_ArtworkRowSize(-1),
      m_PacketIndex(-1),
      m_TotalPacketIndex(-1), // variable not initialized- Coverity fix
      m_ArtworkWidth(-1),
      m_ArtworkHeight(-1),
      m_RowSize(-1),
      m_TopLeftX(-1),
      m_TopLeftY(-1),
      m_BottomRightX(-1),
      m_BottomRightY(-1)
{
#ifdef DBUGLOGS
    LOG_INIT_V2(gControllerLogName);
#endif
    m_Artwork.clear();
}

QByteArray CRetArtworkDataUID::marshall()
{
    QByteArray command;
    return command;
}

QByteArray CRetArtworkDataUID::getArtwork()
{
    return m_Artwork;
}

int CRetArtworkDataUID::getRowSize()
{
    return m_ArtworkRowSize;
}

int CRetArtworkDataUID:: getArtworkWidth()
{
    return m_ArtworkWidth;
}

int CRetArtworkDataUID:: getArtworkHeight()
{
    return m_ArtworkHeight;
}

int CRetArtworkDataUID::getPacketIndex()
{
    return m_PacketIndex;
}

int CRetArtworkDataUID::getTotalPacketIndex()
{
    return m_TotalPacketIndex;
}

int CRetArtworkDataUID::getArtworkFormat()
{
    return m_ArtworkFormat;
}

void  CRetArtworkDataUID::unmarshall(QByteArray returnCommand)
{
    int i = 2;//Lingo Index for Small iAP Packets.
    int len = 0;

    if( ((int)returnCommand[0] == 0x55) && ((int)returnCommand[1] == 0x00) )
    {
        i = 4;//Lingo Index for Large iAP Packets.
        len = 0 | ((0x00FF & returnCommand[2])<<8) | (0x00FF & returnCommand[3]);
    }
    else
    {
        len = returnCommand[1];
    }
    if( ((int)returnCommand[i] == 0x04) && //byte 4
            ((int)returnCommand[i+1] == 0x00) &&
            ((int)returnCommand[i+2] == 0x4F) )
    {
        int cmdStart = i;
        //skip for the command ID and trans ID
        if(m_isTidEnabled)
        {
            i += 4; //byte 8
        }
        else
        {
            i += 2;
        }
        m_PacketIndex = 0 | ((0x00FF & returnCommand[i+1])<<8) | (0x00FF & returnCommand[i+2]);
        m_TotalPacketIndex = 0 | ((0x00FF & returnCommand[i+3])<<8) | (0x00FF & returnCommand[i+4]);
        //skip for the current and total packet index
        i += 4; //byte 12
        if(m_PacketIndex == 0)
        {
            //skip for the identifier type and index
			// { Added by pawan 2013.05.15.
            if((int8_t)returnCommand[i+1] == 0x00)
            {
                i += 9; //byte 21
            }
            else
            {
			//} Added by pawan 2013.05.15.
            i += 5; //byte 17
            } // Added by pawan 2013.05.15.
            m_ArtworkFormat  = returnCommand[i+1];
            m_ArtworkWidth = 0 | ((0x00FF & returnCommand[i+2])<<8) | (0x00FF & returnCommand[i+3]);
            m_ArtworkHeight = 0 | ((0x00FF & returnCommand[i+4])<<8) | (0x00FF & returnCommand[i+5]);
            m_TopLeftX = 0 | ((0x00FF & returnCommand[i+6])<<8) | (0x00FF & returnCommand[i+7]);
            m_TopLeftY = 0 | ((0x00FF & returnCommand[i+7])<<8) | (0x00FF & returnCommand[i+9]);
            m_BottomRightX = 0 | ((0x00FF & returnCommand[i+10])<<8) | (0x00FF & returnCommand[i+11]);
            m_BottomRightY = 0 | ((0x00FF & returnCommand[i+12])<<8) | (0x00FF & returnCommand[i+13]);
            i += 13; //byte 28

            m_ArtworkRowSize = 0 | ((0x000000FF & returnCommand[i+1])<<24) | ((0x000000FF & returnCommand[i+2])<<16) | ((0x000000FF & returnCommand[i+3])<<8) | (0x000000FF & returnCommand[i+4]);

            i += 4; //byte 34
        }
        int dataSize = len -(i - cmdStart);
        m_Artwork.clear();

        //TODO: this is done temporarily to get the correct images will be removed once the
        // array is handled properly
        if(m_PacketIndex%2)
        {
            m_Artwork.insert(0,returnCommand.mid(i,dataSize)); // bytearray from 25 to the end
        }
        else
        {
            m_Artwork.insert(0,returnCommand.mid(i+1,dataSize)); // bytearray from 25 to the end
        }
    }
    m_CommandBytes = returnCommand;
}




