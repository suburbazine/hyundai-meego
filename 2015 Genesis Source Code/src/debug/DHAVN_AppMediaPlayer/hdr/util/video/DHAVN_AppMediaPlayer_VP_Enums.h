// { added by Sergey 28.08.2013 for ITS#186507 
#ifndef DHAVN_APPMEDIAPLAYER_VP_ENUMS_H
#define DHAVN_APPMEDIAPLAYER_VP_ENUMS_H


#include <QObject>

// { added by Sergey 09.09.2013 for ITS#188283
typedef enum
{
    FM_FILE_NEW = 0,
    FM_FILE_SAME,
    FM_FILE_MAX
} FM_FILE_T;
// } added by Sergey 09.09.2013 for ITS#188283

// { added by Sergey 16.10.2013 for ITS#195505
typedef enum
{
    VLIGHT_REW = 1,
    VLIGHT_CENTER = 2,
    VLIGHT_FF = 3,
    VLIGHT_MAX
} VLIGHT_T;
// } added by Sergey 16.10.2013 for ITS#195505

// { added by sjhyun 2013.11.08 for ITS 207579
typedef enum
{
    FOCUS_MODE_MODEAREA = 0,
    FOCUS_MODE_ALL,
    FOCUS_MODE_MAX
} FOCUS_MODE_T;
// } added by sjhyun

class VPEnum : public QObject
{
    Q_OBJECT;

    Q_ENUMS(VDISP_T);

public:

    typedef enum
    {
        VDISP_FRONT = 0,
        VDISP_REAR,
        VDISP_MAX
    } VDISP_T;

};

#endif // DHAVN_APPMEDIAPLAYER_VP_ENUMS_H
// } added by Sergey 28.08.2013 for ITS#186507 
