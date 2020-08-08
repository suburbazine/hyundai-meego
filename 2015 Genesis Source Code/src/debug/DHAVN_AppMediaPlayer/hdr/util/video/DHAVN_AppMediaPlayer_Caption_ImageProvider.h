#ifndef DHAVN_APPMEDIAPLAYER_CAPTION_IMAGEPROVIDER_H
#define DHAVN_APPMEDIAPLAYER_CAPTION_IMAGEPROVIDER_H

//jejang
#include <qdeclarativeengine.h>
#include <qdeclarative.h>
#include <qdeclarativeitem.h>
#include <qdeclarativeimageprovider.h>
#include <qdeclarativeview.h>
#include <QImage>
#include <QPainter>
#include "controller/video/DHAVN_AppMediaPlayer_Video_ControllerFS.h"
//jejang

// { XSubtitle added by jeeeun 2013.04.04
class AppMediaPlayer_VideoCaptionProvider : public QDeclarativeImageProvider
 {
 public:
     AppMediaPlayer_VideoCaptionProvider();
     QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
         void setPixmap(QPixmap test);
         QPixmap test_pixmap;
         int test_num;
 };
// } XSubtitle added by jeeeun 2013.04.04
#endif // DHAVN_APPMEDIAPLAYER_CAPTION_IMAGEPROVIDER_H
