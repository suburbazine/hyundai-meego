#include "util/video/DHAVN_AppMediaPlayer_Caption_ImageProvider.h"

// { XSubtitle added by jeeeun 2013.04.04
AppMediaPlayer_VideoCaptionProvider::AppMediaPlayer_VideoCaptionProvider(): QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)
{
    test_num = 0;
}

QPixmap AppMediaPlayer_VideoCaptionProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    MP_LOG << "AppMediaPlayer_VideoCaptionProvider::requestPixmap()" << LOG_ENDL;
    static int test_rgb = 0;
    int width = 300;
    int height =300;

    if (size)
    {
        if(test_num == 0)
            *size = QSize(width, height);
    }
    /*
        QPixmap pixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
                                   requestedSize.height() > 0 ? requestedSize.height() : height);
       */
    if(!test_pixmap.isNull())
    {
        test_pixmap.setMask(test_pixmap.createMaskFromColor(QColor(251,21,0)));
        test_pixmap.setMask(test_pixmap.createMaskFromColor(QColor(251,20,0)));
        test_pixmap.setMask(test_pixmap.createMaskFromColor(QColor(251,19,0)));
        test_pixmap.setMask(test_pixmap.createMaskFromColor(QColor(251,18,0)));
        test_pixmap.setMask(test_pixmap.createMaskFromColor(QColor(251,17,0)));
        MP_LOG << "AppMediaPlayer_VideoCaptionProvider::requestPixmap()"<< test_pixmap.width() << "ht"<< test_pixmap.height() << LOG_ENDL;
        return test_pixmap;
    }else
    {
        return test_pixmap;
    }
    //return pixmap;
}

void AppMediaPlayer_VideoCaptionProvider::setPixmap(QPixmap test)
{
    MP_LOG << "AppMediaPlayer_VideoCaptionProvider::setPixmap()" << LOG_ENDL;
    test_num = 1;
    test_pixmap = test;
}
// } XSubtitle added by jeeeun 2013.04.04
