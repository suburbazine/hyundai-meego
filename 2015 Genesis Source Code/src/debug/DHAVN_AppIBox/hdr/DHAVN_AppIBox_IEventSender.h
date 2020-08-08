#ifndef DHAVN_APPIBOX_IEVENTSENDER_H
#define DHAVN_APPIBOX_IEVENTSENDER_H

#include <DHAVN_AppFramework_Def.h>
#include <DHAVN_AppFramework_Event_Def.h>

class IEventSender
{
public:
    virtual bool sendEventToUISH( Event &aEvent ) = 0;
    virtual void sendEventToStatusBar( QString &string ) = 0;
    virtual void setStatusBarModemState( QString &str ) = 0;
};

#endif // DHAVN_APPIBOX_IEVENTSENDER_H
