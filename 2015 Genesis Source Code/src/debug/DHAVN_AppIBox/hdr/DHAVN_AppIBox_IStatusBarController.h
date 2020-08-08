#ifndef DHAVN_APPIBOX_STATUSBARCONTROLLER_H
#define DHAVN_APPIBOX_STATUSBARCONTROLLER_H

#include <DHAVN_AppFramework_Def.h>

class IStatusBarController
{
public:
    virtual void showStatusBar() = 0;
    virtual void hideStatusBar() = 0;
    virtual void showOSD(QString &osdCommad) = 0;

    virtual bool swapped() = 0;
    virtual CLONE_STATUS_T cloneStatus() = 0;
};

#endif // DHAVN_APPIBOX_STATUSBARCONTROLLER_H
