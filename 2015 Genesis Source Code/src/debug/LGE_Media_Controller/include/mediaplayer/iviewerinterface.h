#ifndef IVIEWERINTERFACE_H
#define IVIEWERINTERFACE_H

#include "mediaplayer_global.h"

class MEDIAPLAYERSHARED_EXPORT IViewerInterface 
{
public slots:
    virtual void        StartSlideShow()                = 0;
    virtual void        StopSlideShow()                 = 0;
    virtual void        Zoom(int level)                 = 0;
    virtual void        ZoomWithPinch(double level)     = 0;
    virtual void        Rotate (int angle)              = 0;
    virtual void        Scroll(int mode, int motion)    = 0;
    virtual void        SaveImage(const QString &strOldName, const QString &strNewName, const QString & destPath)= 0;
    virtual void        SetSlideShowDuration(int)       = 0;
};

#endif // IMEDIAINTERFACE_H
