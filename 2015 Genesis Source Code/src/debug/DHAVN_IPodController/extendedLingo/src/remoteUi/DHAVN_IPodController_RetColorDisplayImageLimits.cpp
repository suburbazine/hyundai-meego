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
#include <DHAVN_IPodController_RetColorDisplayImageLimits.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>
using namespace ExtendedLingo;

CRetColorDisplayImageLimits::CRetColorDisplayImageLimits(int transId, bool isTidEnabled): m_imageWidth(0),m_imageHeight(0), m_imageFormat(0)
{
        //cout << "in CGetCurrentPlayingTrackIndex constructor\n" ;
    m_TransactionID = transId;
    m_isTidEnabled = isTidEnabled;
}

QByteArray CRetColorDisplayImageLimits::marshall()
{

    return NULL;
}

void  CRetColorDisplayImageLimits::unmarshall(QByteArray returnCommand)
{


    if( ((int)returnCommand[0] == 0x55) &&
               ((int)returnCommand[2] == 0x04) &&
               ((int)returnCommand[4] == 0x3A) ){
	    int i = 5;
        if( m_isTidEnabled ){
            i = 7;
        }

        m_imageWidth = ((0x000000FF & (int)returnCommand[i]) << 8) \
                       | (0x000000FF & (int)returnCommand[i+1]);
        m_imageHeight = ((0x000000FF & (int)returnCommand[i+2]) << 8) \
                        | (0x000000FF & (int)returnCommand[i+3]);
        m_imageFormat = (int)returnCommand[i+4];
        // TODO: Currently using only one format, the first format supported by Apple device.
        //TODO: Need to read all the supported formats and select the proper image.

    } else {
        m_imageFormat = 0;
        m_imageHeight = 0;
        m_imageWidth = 0;
    }

}

int  CRetColorDisplayImageLimits::getImageFormat()
{
    return m_imageFormat;
}

int  CRetColorDisplayImageLimits::getImageHeight()
{
    return m_imageHeight;
}

int  CRetColorDisplayImageLimits::getImageWidth()
{
    return m_imageWidth;
}
