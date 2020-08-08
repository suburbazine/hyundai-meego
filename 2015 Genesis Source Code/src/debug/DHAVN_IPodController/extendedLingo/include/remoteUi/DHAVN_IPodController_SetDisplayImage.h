#ifndef DHAVN_IPODCONTROLLER_SETDISPLAYIMAGE_H
#define DHAVN_IPODCONTROLLER_SETDISPLAYIMAGE_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{

    class CSetDisplayImage: public CIAPPacket
    {
    public:
        CSetDisplayImage(int transId, int index, int dataSize,QByteArray data,  bool isTidEnabled);
        QByteArray marshall();
         void  unmarshall(QByteArray);
         void setImageParams(int width, int height, int format);

    private:
        QByteArray m_imageData;
        int m_dataIndex;
        int m_dataSize;
        int m_imageWidth;
        int m_imageHeight;
        int m_imageFormat;

    };

}


#endif // DHAVN_IPODCONTROLLER_SETDISPLAYIMAGE_H
