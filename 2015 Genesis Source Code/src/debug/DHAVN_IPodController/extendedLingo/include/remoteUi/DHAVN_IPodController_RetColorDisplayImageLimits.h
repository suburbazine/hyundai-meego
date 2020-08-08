#ifndef DHAVN_IPODCONTROLLER_RETCOLORDISPLAYIMAGELIMITS_H
#define DHAVN_IPODCONTROLLER_RETCOLORDISPLAYIMAGELIMITS_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{

    class CRetColorDisplayImageLimits : public CIAPPacket
    {
    public:
        CRetColorDisplayImageLimits(int transId,  bool isTidEnabled);
        QByteArray marshall();
         void  unmarshall(QByteArray);
         int getImageWidth();
         int getImageHeight();
         int getImageFormat();
    private:
         int m_imageWidth;
         int m_imageHeight;
         int m_imageFormat;
    };

}


#endif // DHAVN_IPODCONTROLLER_RETCOLORDISPLAYIMAGELIMITS_H
