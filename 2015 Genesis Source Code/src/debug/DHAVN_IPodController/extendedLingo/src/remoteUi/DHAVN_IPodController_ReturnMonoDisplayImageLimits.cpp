/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Mohana/ Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  17 Apr 2012                                   *
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
#include <DHAVN_IPodController_ReturnMonoDisplayImageLimits.h>
#include <DHAVN_IPodController_iPodControllerGlobal.h>

using namespace ExtendedLingo;

CReturnMonoDisplayImageLimits::CReturnMonoDisplayImageLimits(int transId, bool isTidEnabled): m_imageWidth(0),m_imageHeight(0), m_imageFormat(0)
{        
    m_TransactionID = transId;
    m_isTidEnabled = isTidEnabled;
}

QByteArray CReturnMonoDisplayImageLimits::marshall()
{
    return NULL;
}

void  CReturnMonoDisplayImageLimits::unmarshall(QByteArray returnCommand)
{


    if( ((int)returnCommand[0] == 0x55) &&
               ((int)returnCommand[2] == 0x04) &&
               ((int)returnCommand[4] == 0x34) ){
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

int  CReturnMonoDisplayImageLimits::getImageFormat()
{
    return m_imageFormat;
}

int  CReturnMonoDisplayImageLimits::getImageHeight()
{
    return m_imageHeight;
}

int  CReturnMonoDisplayImageLimits::getImageWidth()
{
    return m_imageWidth;
}


