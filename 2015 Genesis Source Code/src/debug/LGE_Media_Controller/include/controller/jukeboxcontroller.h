#ifndef JUKEBOXCONTROLLER_H
#define JUKEBOXCONTROLLER_H

#include <QObject>
#include "usbcontroller.h"


class JukeboxController : public USBController
{
    Q_OBJECT
public:
    #if USE_USB_SINGLE_INSTANCE
    static JukeboxController*  GetInstance();
    #else
    static JukeboxController*  GetFirstInstance();
    static JukeboxController*  GetSecondInstance();
    static JukeboxController*  GetThirdInstance();
    #endif

    ~JukeboxController();

private:
    JukeboxController(MEDIA_PLAYER_MODE eMode, QObject *parent = 0);

    #if USE_USB_SINGLE_INSTANCE
    static JukeboxController    *m_pInstance;
    #else
    static JukeboxController    *m_pFirstInstance;
    static JukeboxController    *m_pSecondInstance;
    static JukeboxController    *m_pThirdInstance;
    #endif
};


#endif // JUKEBOXCONTROLLER_H
