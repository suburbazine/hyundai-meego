#ifndef DHAVN_APPAHAMENUS_DEF_H
#define DHAVN_APPAHAMENUS_DEF_H
#include <QObject>


class AhaMenuItems : public QObject
{
    Q_OBJECT
    Q_ENUMS(MenuItems)

public:

    //hsryu_0306_change_menu
    enum MenuItems
    {
        NoItem = 0,
        StationList,
        ContentsList,
        ThumbsUp,
        ThumbsDown,
        Call,
        Navigate,
        SoundSetting,//wsuk.kim dynamic_menu
        NowListening,
        Favorite,
        Ban,
        MENU,
        END
    };
};
#endif // DHAVN_APPAHAMENUS_DEF_H
