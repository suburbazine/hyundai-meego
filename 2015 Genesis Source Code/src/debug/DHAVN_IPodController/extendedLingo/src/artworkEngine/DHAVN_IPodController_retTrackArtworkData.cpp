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
#include <DHAVN_IPodController_retTrackArtworkData.h>

using namespace ExtendedLingo;

CRetTrackArtworkData::CRetTrackArtworkData( bool isTidEnabled)
    :CIAPPacket(0x04,0x0011,-1,isTidEnabled),
    m_ArtworkFormat(-1),
    m_ArtworkRowSize(-1),
    m_PacketIndex(-1),
    m_ArtworkWidth(-1),
    m_ArtworkHeight(-1),
    m_RowSize(-1),
    m_TopLeftX(-1),
    m_TopLeftY(-1),
    m_BottomRightX(-1),
    m_BottomRightY(-1)
{
    m_Artwork.clear();
//        q << "in CRetTrackArtworkData constructor\n" ;
}

QByteArray CRetTrackArtworkData::marshall()
{
        QByteArray command;
        return command;
}

QByteArray CRetTrackArtworkData::getArtwork()
{
    return m_Artwork;
}

int CRetTrackArtworkData::getRowSize()
{
    return m_ArtworkRowSize;
}

 int CRetTrackArtworkData:: getArtworkWidth()
 {
    return m_ArtworkWidth;
 }

 int CRetTrackArtworkData:: getArtworkHeight()
 {
    return m_ArtworkHeight;
 }

 int CRetTrackArtworkData::getPacketIndex()
 {
     return m_PacketIndex;
 }

 int CRetTrackArtworkData::getArtworkFormat()
 {
    return m_ArtworkFormat;
 }

/*
 int CRetTrackArtworkData:: getTopLeftX()
 {
    return m_TopLeftX;
 }

 int CRetTrackArtworkData:: getTopLeftY()
 {
    return m_TopLeftY;
 }

 int CRetTrackArtworkData:: getBottomRightX()
 {
    return m_BottomRightX;
 }

 int CRetTrackArtworkData:: getBottomRightY()
 {
    return m_BottomRightY;
 }
*/
void  CRetTrackArtworkData::unmarshall(QByteArray returnCommand)
{
    int i = 2;//Lingo Index for Small iAP Packets.
    int len = 0;
    // Remove the checksum byte from the array in order to use mid to obtain the record string
    //chop(1) removes 1 byte from the end of the array
    returnCommand.chop(1);
    if( ((int)returnCommand[0] == 0x55) &&
        ((int)returnCommand[1] == 0x00) ){
        i = 4;//Lingo Index for Large iAP Packets.
        len = 0 | ((0x00FF & returnCommand[2])<<8) | (0x00FF & returnCommand[3]);
    }else{
        len = returnCommand[1];
    }
    if( ((int)returnCommand[i] == 0x04) &&
        ((int)returnCommand[i+1] == 0x00) &&
        ((int)returnCommand[i+2] == 0x11) ){
        int cmdStart = i;
        i += 2;
        if( m_isTidEnabled ){
            i +=2;
        }
        m_PacketIndex = 0 | ((0x00FF & returnCommand[i+1])<<8) | (0x00FF & returnCommand[i+2]);

        if(m_PacketIndex == 0){
            m_ArtworkFormat  = returnCommand[i+3];
            m_ArtworkWidth = 0 | ((0x00FF & returnCommand[i+4])<<8) | (0x00FF & returnCommand[i+5]);
            m_ArtworkHeight = 0 | ((0x00FF & returnCommand[i+6])<<8) | (0x00FF & returnCommand[i+7]);
            m_TopLeftX = 0 | ((0x00FF & returnCommand[i+8])<<8) | (0x00FF & returnCommand[i+9]);
            m_TopLeftY = 0 | ((0x00FF & returnCommand[i+10])<<8) | (0x00FF & returnCommand[i+11]);
            m_BottomRightX = 0 | ((0x00FF & returnCommand[i+12])<<8) | (0x00FF & returnCommand[i+13]);
            m_BottomRightY = 0 | ((0x00FF & returnCommand[i+14])<<8) | (0x00FF & returnCommand[i+15]);
            m_ArtworkRowSize = 0 | ((0x000000FF & returnCommand[i+16])<<24) | ((0x000000FF & returnCommand[i+17])<<16) | ((0x000000FF & returnCommand[i+18])<<8) | (0x000000FF & returnCommand[i+19]);
            i += 17;
       }
       i +=3;
       int dataSize = len -(i - cmdStart);
      // DBGLOGFun("CArtworkFetchTaskHandler::fetchArtwork: PacketIndex=%d, dataSize=%d",m_PacketIndex,dataSize);
       m_Artwork.insert(0,returnCommand.mid(i,dataSize)); // bytearray from 25 to the end
    }
    m_CommandBytes = returnCommand;
}



