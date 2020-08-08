/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                 The class which provides the data from the co- *
 *                 processor chip, it help to get the certificate *
 *                 , Signature to authorize the IPOD.             *
 *                 and also provide the details like device ID,   *
 *                 version numbers etc.                           *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <fcntl.h>
#include <linux/ioctl.h>
#include <sys/ioctl.h>
#include <string>
#include <DHAVN_IPodController_CommonVars.h>
#include <stdio.h>
#include <qthread.h>
#include <DHAVN_IPodController_logoImageTransmitter.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <DHAVN_IPodController_PrivateData.h>

#define min(a, b)		((a) < (b) ? (a) : (b))
#define BPP16_HEADER_BYTES           70
#define BPP16_BYTES                  2



CLogoImageTrasmitter::CLogoImageTrasmitter(iPodControllerPrivateData *privateData, bool isTidEnabled, int maxWidth, int maxHeight, int format)
{
    switch(privateData->m_currentPortNum)
    {
    case eIPOD_PORT_1:
        LOG_INIT_V2(giPodControllerLogName_1);
        break;
    case eIPOD_PORT_2:
        LOG_INIT_V2(giPodControllerLogName_2);
        break;
    default:
        LOG_INIT_V2(giPodControllerLogName);
        break;
    }

    //LOG_TRACE << "CLogoImageTrasmitter::CLogoImageTrasmitter() IN" << LOG_ENDL;
    m_logoMaxWidth  = maxWidth;
    m_logoMaxHeight = maxHeight;
    m_logoFormat    = format;

    m_logoWidth     = maxWidth;
    m_logoHeight    = maxHeight;

    m_logoCompletIndex       = -1;
    m_logoImageFd            = -1;
    m_logoTotalCount         = 0;
    m_logoChunkSize          = 224;
    m_logoTotalImageSize     = 0;
    m_logoCompletedImageSize = 0;
    m_isTidEnabled           = isTidEnabled;

    init();

    //LOG_TRACE << "CLogoImageTrasmitter::CLogoImageTrasmitter() OUT" << LOG_ENDL;
}

CLogoImageTrasmitter::~CLogoImageTrasmitter()
{
    deInit();
}

void CLogoImageTrasmitter::init()
{
    //LOG_TRACE << "CLogoImageTrasmitter::init() IN" << LOG_ENDL;

    int     ret = 0;
    QString logoFile = getLogoFile();
    QImage  logo(logoFile);

    //LOG_TRACE << "   logo.width = " << logo.width() << ", logo.height = " << logo.height() << LOG_ENDL;

    m_logoWidth  = logo.width();
    m_logoHeight = logo.height();

    QString logoFileNew = "/app/data/AppMedia/share/images/iPodData/.ipod_logo.bmp";
    m_logoImageFd = open(logoFileNew.toAscii().data() ,O_RDWR) ;

    if( m_logoImageFd < 0 )
    {
        m_logoImageFd = open(logoFile.toAscii().data() ,O_RDWR);

        if( m_logoImageFd < 0 )
        {
            LOG_HIGH  << "CLogoImageTrasmitter::init(), failed to open the image file" << LOG_ENDL;
        }
        else
        {
            //LOG_TRACE << "   Success to open the image file" << LOG_ENDL;

            struct stat filestatus;
            ret = fstat( m_logoImageFd, &filestatus );
            if( ret <0 )
            {
                LOG_HIGH << "CLogoImageTrasmitter::init(), error in fstat() of the logo file returning = " << ret <<LOG_ENDL;
                return;
            }

            //LOG_TRACE << "   Image total size = " << filestatus.st_size << ", logoFile = " << logoFile.toAscii().data() <<LOG_ENDL;
            char buf[m_logoWidth * m_logoHeight*BPP16_BYTES];
            char header[BPP16_HEADER_BYTES];
            ret = read( m_logoImageFd, header, BPP16_HEADER_BYTES );

            QByteArray data_reverse(buf,(m_logoWidth*m_logoHeight*BPP16_BYTES));
            int ret1 = lseek(m_logoImageFd,BPP16_HEADER_BYTES,0);
            ret = read(m_logoImageFd,buf,m_logoWidth*m_logoHeight*BPP16_BYTES);

            QByteArray data(buf,(m_logoWidth*m_logoHeight*BPP16_BYTES));
            if( ret >0 && ret1 >= 0 )
            {
                //LOG_TRACE << "   Success to read entire the image file " << LOG_ENDL;
                if(data.size() > 0 )
                {
                    data_reverse.clear();
                    int i = (m_logoWidth*m_logoHeight*BPP16_BYTES);
                    int y = 0;
                    while(i >= 0)
                    {
                        data_reverse[i] = data[y];
                        i--;
                        y++;
                    }
                    data_reverse.prepend(header,BPP16_HEADER_BYTES);
                    m_logoTotalImageSize = (m_logoWidth*m_logoHeight*BPP16_BYTES);//filestatus.st_size ;

                    int m_logoImageNewFd = open(logoFileNew.toAscii().data() ,O_CREAT | O_RDWR, S_IRWXU);
                    if(m_logoImageNewFd < 0)
                    {
                        //LOG_TRACE << "   Could not open hidden bmp file" << LOG_ENDL;
                    }
                    else
                    {
                        if(write(m_logoImageNewFd,data_reverse.data(),(m_logoWidth*m_logoHeight*BPP16_BYTES)+BPP16_HEADER_BYTES) > 0 )
                        {
                            //LOG_TRACE << "   Could not write into hidden bmp file " << LOG_ENDL;
                        }

                        if(m_logoImageFd < 0)
                        {
                            LOG_HIGH << "CLogoImageTrasmitter::init(), failed to open the image file" << LOG_ENDL;
                            return;
                        }
                        else
                        {
                            if( lseek(m_logoImageFd,BPP16_HEADER_BYTES,0) < 0 )
                            {
                                //LOG_TRACE << "   failed to leek file" << LOG_ENDL;
                            }
                        }

                        if( lseek(m_logoImageFd, BPP16_HEADER_BYTES, 0) < 0 )
                        {
                            //LOG_TRACE << "   failed to leek file" << LOG_ENDL;
                        }

                        if( m_logoImageFd < 0 )
                        {
                            LOG_HIGH << "CLogoImageTrasmitter::init(), failed to open the image file" << LOG_ENDL;
                            return;
                        }
                    }

                    if( m_logoImageNewFd > -1 )
                    {
						close(m_logoImageNewFd);
						m_logoImageNewFd = -1;
					}
				}
			}
			else
			{
				//LOG_TRACE << "   Failure to read entire the image file" << LOG_ENDL;
			}
		}// if(m_logoImageFd >= 0)
	} //if(m_logoImageFd < 0)
	else
	{
		//LOG_TRACE << "   logo file exists" << LOG_ENDL;
        if( lseek(m_logoImageFd,BPP16_HEADER_BYTES,0) < 0 )
        {
            //LOG_TRACE << "   failed to leek file" << LOG_ENDL;
        }
        m_logoTotalImageSize = (m_logoWidth*m_logoHeight*BPP16_BYTES);
    }
    // void function doesn't need a return  type. so deleted return.Coverity fix
    //LOG_TRACE << "CLogoImageTrasmitter::init() OUT" << LOG_ENDL;
}

void CLogoImageTrasmitter::deInit()
{
    if(m_logoImageFd > -1)
    {
        close(m_logoImageFd);
        m_logoImageFd = -1;
    }
}

QString CLogoImageTrasmitter::getLogoFile()
{
    QString logoFile("/app/data/AppMedia/share/images/iPodData/ipod_image.bmp");
    return logoFile;
}

QByteArray CLogoImageTrasmitter::TransmittLogo( int index, int m_TransID, int format)
{
    //LOG_TRACE << "CLogoImageTrasmitter::TransmittLogo() IN" << LOG_ENDL;

    int    cur_size;
    int    ret;
    char   buf[ m_logoChunkSize + 1 ];

    QByteArray cmd_bytes;
    cmd_bytes.clear();
    m_logoFormat = format;

    if(( m_logoCompletIndex + 1 ) != index )
    {
        //LOG_TRACE << "   Index mismatch ERROR, m_logoCompletIndex = " << m_logoCompletIndex << ", current index = " << index << LOG_ENDL;
        return cmd_bytes;

    }
    cur_size = m_logoTotalImageSize - m_logoCompletedImageSize;
    if(cur_size == 0)
    {
        //LOG_TRACE <<"   Transmition Completed!!" <<LOG_ENDL;
        //LOG_TRACE << "CLogoImageTrasmitter::TransmittLogo() OUT" << LOG_ENDL;
        return cmd_bytes;
    }

    cur_size = cur_size > m_logoChunkSize ? m_logoChunkSize : cur_size;
    ret = read(m_logoImageFd,buf,m_logoChunkSize);
    if(ret >0)
        cur_size = ret;
    else
    {
        //LOG_TRACE << "   Failed to read forn image File" << LOG_ENDL;
        //LOG_TRACE << "CLogoImageTrasmitter::TransmittLogo() OUT" << LOG_ENDL;
        return cmd_bytes;
    }

    QByteArray data(buf,cur_size);
    CSetDisplayImage cmd(m_TransID, index, cur_size, data, m_isTidEnabled);
    cmd.setImageParams(m_logoWidth, m_logoHeight, m_logoFormat);
    cmd.marshall();

    m_logoCompletIndex++;
    m_logoCompletedImageSize += cur_size;

    //LOG_TRACE << "   ret = "<< ret << ", m_logoCompletIndex = " << m_logoCompletIndex << ", m_logoCompletedImageSize = " << m_logoCompletedImageSize << LOG_ENDL;
    //LOG_TRACE << "CLogoImageTrasmitter::TransmittLogo() OUT" << LOG_ENDL;
    return cmd.getCommandBytes();
}
