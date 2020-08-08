/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPOdController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  03 Dec 2011                                   *
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
#include <DHAVN_IPodController_retArtworkFormats.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <QByteArray>
#include <DHAVN_IPodController_Def.h>

using namespace ExtendedLingo;


CRetArtworkFormats::CRetArtworkFormats( bool isTidEnabled)
    :CIAPPacket(CIAPCmds::ExtenededLingoId,CIAPCmds::L4_0F_RetArtworkFormats,0, isTidEnabled)
{
    m_formatId.clear();
    m_pixelFormat.clear();
    m_imageWidth.clear();
    m_imageHeight.clear();
    m_formatCount = 0;
}

CRetArtworkFormats::~CRetArtworkFormats()
{

}

QByteArray CRetArtworkFormats::marshall()
{
    return NULL;

}

void CRetArtworkFormats::unmarshall(QByteArray cmd)
{
    //cmd.chop(1);//Chop the checksum
    if( ((int)cmd[0] == 0x55) &&
        ((int)cmd[2] == CIAPCmds::ExtenededLingoId) &&
        ((int)cmd[3] == 0x00) &&
        ((int)cmd[4] == CIAPCmds::L4_0F_RetArtworkFormats) ){

        int i = 5;
        int size = (int)cmd[1];
        size -= 3;
        if( m_isTidEnabled ){
            i = 7;
            size -= 2;
        }
        m_formatCount = size / 7;
       // m_formatId = new int(m_formatCount);
       // m_pixelFormat = new int(m_formatCount);
       // m_imageWidth = new int(m_formatCount);
       // m_imageHeight = new int(m_formatCount);

        int nFormatID;
        int nPixelFormat, nWidth, nHeight;
        for(int cnt = 0;cnt < m_formatCount; cnt++ )
        {
            nFormatID = (0x000000FF & cmd[i]);
            nFormatID = nFormatID << 8;
            nFormatID = nFormatID | (0x000000FF & cmd[i+1]);

            nPixelFormat = (0x000000FF & cmd[i+2]);

            nWidth = (0x000000FF & cmd[i+3]);
            nWidth = nWidth << 8;
            nWidth = nWidth | (0x000000FF & cmd[i+4]);

            nHeight = (0x000000FF & cmd[i+5]);
            nHeight = nHeight << 8;
            nHeight = nHeight | (0x000000FF & cmd[i+6]);

            m_formatId.append(nFormatID);
            m_pixelFormat.append(nPixelFormat);
            m_imageWidth.append(nWidth);
            m_imageHeight.append(nHeight);

            i+=7;

            /*
            int tmp = 0;
            //CR 16144,15669 :  Changed for proper artwork fetching S
            bool ok;
            //tmp =  0|((0x000000FF && cmd[i])<<8) | cmd[i+1];
            QString string = QString::number((int)cmd[i+1], 16);
            QString subString = string.mid((string.length()-2),2);
            subString = ((QString::number((int)cmd[i], 16)).append(subString));
            // Converting a hex string back to a number
            tmp = QString(subString).toInt(&ok ,16);
            //CR 16144,15669 :  Changed for proper artwork fetching E
            m_formatId.append(tmp);

            tmp = cmd[i+2];
            m_pixelFormat.append(tmp);
            //CR 16144,15669 :  Changed for proper artwork fetching S
            //tmp = ((0x000000FF && cmd[i+3])<<8) | cmd[i+4];
            string = QString::number((int)cmd[i+4], 16);
            subString = string.mid((string.length()-2),2);
            subString = ((QString::number((int)cmd[i+3], 16)).append(subString));
            tmp = QString(subString).toInt(&ok ,16);
            //CR 16144,15669 :  Changed for proper artwork fetching E
            m_imageWidth.append(tmp);

            //CR 16144,15669 :  Changed for proper artwork fetching S
            //tmp = ((0x000000FF && cmd[i+5]) << 8) | cmd[i+6];
            string = QString::number((int)cmd[i+6], 16);
            subString = string.mid((string.length()-2),2);
            subString = ((QString::number((int)cmd[i+5], 16)).append(subString));
            tmp = QString(subString).toInt(&ok ,16);
            //CR 16144,15669 :  Changed for proper artwork fetching E
            m_imageHeight.append(tmp);
            i+=7;
            */
        }
    }
    m_CommandBytes = cmd;
}

int CRetArtworkFormats::getFormatCount()
{
    return m_formatCount;
}

int CRetArtworkFormats::getFormatID(int formatIndex)
{
    if(formatIndex < m_formatCount)
        return m_formatId[formatIndex];
    return IPOD_ERROR_GENERIC;
}

int CRetArtworkFormats::getPixelFormat(int formatIndex)
{
    if(formatIndex < m_formatCount)
        return m_pixelFormat[formatIndex];
    return IPOD_ERROR_GENERIC;
}

int CRetArtworkFormats::getImageWidth(int formatIndex)
{
    if(formatIndex < m_formatCount)
        return m_imageWidth[formatIndex];
    return IPOD_ERROR_GENERIC;
}
int CRetArtworkFormats::getimageHeight(int formatIndex)
{
    if(formatIndex < m_formatCount)
        return m_imageHeight[formatIndex];
    return IPOD_ERROR_GENERIC;
}
