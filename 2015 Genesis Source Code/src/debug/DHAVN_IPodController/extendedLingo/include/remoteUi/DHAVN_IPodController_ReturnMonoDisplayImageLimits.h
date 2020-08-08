#ifndef DHAVN_IPODCONTROLLER_RETURNMONODISPLAYIMAGELIMITS_H
#define DHAVN_IPODCONTROLLER_RETURNMONODISPLAYIMAGELIMITS_H

#include <DHAVN_IPodController_iAPPacket.h>

namespace ExtendedLingo
{

    class CReturnMonoDisplayImageLimits : public CIAPPacket
    {
    public:
        CReturnMonoDisplayImageLimits(int transId,  bool isTidEnabled);
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

#endif // DHAVN_IPODCONTROLLER_RETURNMONODISPLAYIMAGELIMITS_H
