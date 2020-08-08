#ifndef DHAVN_APPIBOX_IVIEWWIDGETCONTROLLER_H
#define DHAVN_APPIBOX_IVIEWWIDGETCONTROLLER_H

#include <QWidget>
#include <gst/gst.h>
#include <QGst/Element>

#include "uistate_manager_def.h"
#include "DHAVN_AppFramework_QML_Def.h"

#include "DHAVN_AppIBox_Def.h"

class IViewWidgetController
{
public:
    virtual WId winId() = 0;
    //virtual void setGeometry(int ax, int ay, int aw, int ah) = 0;

    virtual void showViewWidget() = 0;
    virtual void hideViewWidget() = 0;
    virtual void reInit() = 0;
    //virtual void setVideoSink( QGst::ElementPtr aVideoSink ) = 0;
    //virtual void releaseVideoSink() = 0;

    bool getNeedToShowStatusBarFlag() { return m_bNeedToShowStatusBar; }

protected:
    bool m_bNeedToShowStatusBar;

};

#endif // DHAVN_APPIBOX_IVIEWWIDGETCONTROLLER_H
