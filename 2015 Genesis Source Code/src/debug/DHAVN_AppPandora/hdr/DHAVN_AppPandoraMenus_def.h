#ifndef DHAVN_APPPANDORAMENUS_DEF_H
#define DHAVN_APPPANDORAMENUS_DEF_H
#include <QObject>

//{ Removed by Radhakrushna Mohanty dated : 2012.08.20
//#define PANDORA_TRACKVIEW_MENU_TITLE                "MENU"
//#define PANDORA_TRACKVIEW_STATIONLIST_MENU_ITEM     "Station List"
//#define PANDORA_TRACKVIEW_THUMBSUP_MENU_ITEM        "Thumbs Up"
//#define PANDORA_TRACKVIEW_THUMBSDOWN_MENU_ITEM      "Thumbs Down"
//#define PANDORA_TRACKVIEW_BOOKMARKSONG_MENU_ITEM    "Bookmark Track"
//#define PANDORA_TRACKVIEW_BOOKMARKARTIST_MENU_ITEM  "Bookmark Artist"
//#define PANDORA_TRACKVIEW_EXPLAIN_MENU_ITEM         "Why this song is playing"
//#define PANDORA_TRACKVIEW_SOUNDSETTING_MENU_ITEM    "Sound Setting"
//#define PANDORA_TRACKVIEW_END_MENU_ITEM             "End"

//#define PANDORA_LISTVIEW_MENU_TITLE                 "MENU"
//#define PANDORA_LISTVIEW_NOWLISTENING_MENU_TITLE    "Now Listening"
//#define PANDORA_LISTVIEW_SORTBY_MENU_TITLE          "Sort by"
//#define PANDORA_LISTVIEW_SEARCH_MENU_TITLE          "Search"
//#define PANDORA_LISTVIEW_SOUNDSETTING_MENU_TITLE    "Sound Setting"
//#define PANDORA_LISTVIEW_END_MENU_TITLE             "End"

//#define PANDORA_LISTVIEW_SORTBYMENU_ALPHA_MENU_TITLE    "Alphabet"
//#define PANDORA_LISTVIEW_SORTBYMENU_DATE_MENU_TITLE     "Date"

//#define PANDORA_THUMBSUP_HIGHLIGHTED_ICON       "/app/share/images/pandora/ico_option_like_up_s.png"
//#define PANDORA_THUMBSUP_ENABLE_ICON            "/app/share/images/pandora/ico_option_like_up_n.png"

//#define PANDORA_THUMBSDOWN_HIGHLIGHTED_ICON     "/app/share/images/pandora/ico_option_like_down_s.png"
//#define PANDORA_THUMBSDOWN_ENABLE_ICON          "/app/share/images/pandora/ico_option_like_down_n.png"
//} Removed by Radhakrushna Mohanty dated : 2012.08.20

const QString cThumbsUpHighlighted= "/app/share/images/pandora/ico_option_like_up_s.png";
const QString cThumbsUpEnable= "/app/share/images/pandora/ico_option_like_up_n.png";
const QString cThumbsDownHighlighted= "/app/share/images/pandora/ico_option_like_down_s.png";
const QString cThumbsDownEnable= "/app/share/images/pandora/ico_option_like_down_n.png";
const QString cThumbsUpDisable= "/app/share/images/pandora/ico_option_like_up_d.png";
const QString cThumbsDownDisable= "/app/share/images/pandora/ico_option_like_down_d.png";


class PandoraMenuItems : public QObject
{
    Q_OBJECT
    Q_ENUMS(MenuItems)

public:

    enum MenuItems
    {
        NoItem = 0,
        StationList,
        ThumbsUp,
        ThumbsDown,
        BookmarkSong,
        BookmarkArtist,
        WhyThisSongIsPlaying,
        SoundSetting,
        SortBy,
        Alphabet,
        Date,
        NowListening,
        Search,
        MENU,
        END
    };
};
#endif // DHAVN_APPPANDORAMENUS_DEF_H
