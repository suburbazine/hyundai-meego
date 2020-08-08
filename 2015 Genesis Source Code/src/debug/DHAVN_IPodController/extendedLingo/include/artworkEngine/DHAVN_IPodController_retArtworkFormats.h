#ifndef DHAVN_IPODCONTROLLER_RETARTWORKFORMATS_H
#define DHAVN_IPODCONTROLLER_RETARTWORKFORMATS_H


#include <DHAVN_IPodController_iAPPacket.h>
namespace ExtendedLingo
{
    class CRetArtworkFormats : public CIAPPacket
    {
        public:
           CRetArtworkFormats(bool isTidEnabled);
           ~CRetArtworkFormats();
           QByteArray marshall();
            void  unmarshall(QByteArray);

            int getFormatCount();
            int getFormatID(int formatIndex);
            int getPixelFormat(int formatIndex);
            int getImageWidth(int formatIndex);
            int getimageHeight(int formatIndex);


        private:
            int m_formatCount;
            QList <int> m_formatId;
            QList <int>m_imageWidth;
            QList <int>m_imageHeight;
            QList <int>m_pixelFormat;
    };
}
#endif // DHAVN_IPODCONTROLLER_RETARTWORKFORMATS_H
