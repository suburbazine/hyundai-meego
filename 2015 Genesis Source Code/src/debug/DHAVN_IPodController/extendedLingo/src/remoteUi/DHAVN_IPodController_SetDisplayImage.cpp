/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  10 Oct 2011                                   *
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
#include <DHAVN_IPodController_SetDisplayImage.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
using namespace ExtendedLingo;

CSetDisplayImage::CSetDisplayImage(int transId, int index, int dataSize,QByteArray data, bool isTidEnabled): m_imageWidth(0), m_imageHeight(0), m_imageFormat(0)
{
        //cout << "in CGetCurrentPlayingTrackIndex constructor\n" ;
    m_TransactionID = transId;
    m_dataIndex = index;
    m_dataSize = dataSize;
    m_imageData = data;
    m_isTidEnabled = isTidEnabled;

}

QByteArray CSetDisplayImage::marshall()
{
    QByteArray command;

    int row_size = m_imageWidth * 2;// 2 Bytes per pixel( 16bpp)
    int i = -1;//Array index

    //Packing format =  Big Endian
    if(0 == m_dataIndex){
    //    command.resize(19 + m_dataSize);
        command[++i] = 0x55;
        command[++i] = 0x00;
        command[++i] = 0x04;//ExtendedLingo
        command[++i] = 0x00;
        command[++i] = 0x32;//SetDisplayImage
        if( m_isTidEnabled){
            command[++i] = (m_TransactionID & 0xFF00) >> 8;
            command[++i] =  m_TransactionID & 0x00FF;
        }

        command[++i] = (0xFF00 & m_dataIndex) >>8;
        command[++i] = 0x00FF & m_dataIndex;

        command[++i] = 0x000000FF& m_imageFormat;

        command[++i] = (0x0000FF00& m_imageWidth) >> 8;
        command[++i] = 0x000000FF& m_imageWidth;

        command[++i] = (0x0000FF00& m_imageHeight) >> 8;
        command[++i] = 0x000000FF& m_imageHeight;

        command[++i] = (0xFF000000& row_size) >> 24;
        command[++i] = (0x00FF0000& row_size) >> 16;
        command[++i] = (0x0000FF00& row_size) >> 8;
        command[++i] = 0x000000FF& row_size;

        command.append(m_imageData,m_dataSize);
        i += m_dataSize;
        command[1] = i - 1;

        command[i+1] = CIpodControllerGlobal::checkSum(command,i);
    } else {
       // command.resize(10 + m_dataSize);
        command[++i] = 0x55;
        command[++i] = 0x00;
        command[++i] = 0x04;//ExtendedLingo
        command[++i] = 0x00;
        command[++i] = 0x32;//SetDisplayImage
        if( m_isTidEnabled ){
            command[++i] = (m_TransactionID & 0xFF00) >> 8;
            command[++i] =  m_TransactionID & 0x00FF;
        }

        command[++i] = (0xFF00 & m_dataIndex) >>8;
        command[++i] = 0x00FF & m_dataIndex;

        command.append(m_imageData,m_dataSize);
        i += m_dataSize;
        command[1] = (0xFF & i) - 1 ;

        command[i+1] = CIpodControllerGlobal::checkSum(command,i);

    }

    m_CommandBytes = command;
    return command;
}

void  CSetDisplayImage::unmarshall(QByteArray)
{

}

void CSetDisplayImage::setImageParams(int width, int height, int format)
{
         m_imageWidth = width;
         m_imageHeight = height;
         m_imageFormat = format;
}
