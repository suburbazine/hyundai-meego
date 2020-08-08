#ifndef DHAVN_APPUSERMANUAL_DEF_H
#define DHAVN_APPUSERMANUAL_DEF_H

#include <QObject>

class MANUALMENU : public QObject
{
    Q_OBJECT

    Q_ENUMS(MANUAL_OPTIONS)

public:

    typedef enum
    {
        MANUAL_MAIN,
        MANUAL_PDF_SCREEN,
        MANUAL_EXIT_SEARCH,
        MANUAL_EXIT_ZOOM
    }MANUAL_MENU_TYPE;

    typedef enum
    {
        OPTION_MANUAL_MENU,
        OPTION_MANUAL_SEARCH,
        OPTION_MANUAL_GOTO,
        OPTION_MANUAL_EXIT_SEARCH,
        OPTION_MANUAL_EXIT_ZOOM
    }MANUAL_OPTIONS;

   typedef enum
    {
       JOG_CENTER = 0,
       JOG_UP,
       JOG_DOWN,
       JOG_LEFT,
       JOG_RIGHT,
       JOG_TOP_RIGHT,
       JOG_BOTTOM_RIGHT,
       JOG_TOP_LEFT,
       JOG_BOTTOM_LEFT,
       JOG_WHEEL_LEFT,
       JOG_WHEEL_RIGHT,
       JOG_WHEEL_LONG_LEFT,
       JOG_WHEEL_LONG_RIGHT,
       JOG_NONE
    }EJOG_EVENTS_T;
};

#endif // DHAVN_APPUSERMANUAL_DEF_H
