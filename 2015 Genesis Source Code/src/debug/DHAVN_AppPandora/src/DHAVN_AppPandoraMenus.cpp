#include <DHAVN_AppPandoraMenus.h>
#include <DHAVN_AppPandoraMenus_def.h>

OPTION_MENU_T TRACKVIEW_MENU[] = {
    { PandoraMenuItems::MENU,"STR_PANDORA_MENU", false, false, false, true, true, NULL , 0 },
    { PandoraMenuItems::StationList,"STR_PANDORA_STATIONLIST",false,false, false, true, true,  NULL, 0 },
    { PandoraMenuItems::ThumbsUp, "STR_PANDORA_THUMBSUP", false,  false, false, true, true, NULL, 0, cThumbsUpEnable },
    { PandoraMenuItems::ThumbsDown, "STR_PANDORA_THUMBSDOWN",  false,  false, false,  true, true, NULL, 0, cThumbsDownEnable },
    { PandoraMenuItems::BookmarkSong, "STR_PANDORA_BOOKMARK_TRACK",  false, false, false, true, true, NULL, 0 },
   // { PandoraMenuItems::BookmarkArtist, "STR_PANDORA_BOOKMARK_ARTIST", false, false, false, true,  true, NULL, 0 },// modified by wonseok.heo for PANDORA certification 2014.11.04
    { PandoraMenuItems::WhyThisSongIsPlaying, "STR_PANDORA_WHY_THIS_SONG", false,  false, false,  true,  true, NULL, 0 },
    { PandoraMenuItems::SoundSetting, "STR_PANDORA_SOUNDSETTING",  false,  false, false,  true,  true, NULL, 0 }
};


OPTION_MENU_T SORTBY_MENU[] = {
    { PandoraMenuItems::MENU, "STR_PANDORA_MENU", false, false, false, true, true, NULL , 0 },
    { PandoraMenuItems::Alphabet, "STR_MENU_SORTBY_ALPHABET",false,true, false, true, true, NULL, 0 },
    { PandoraMenuItems::Date, "STR_MENU_SORTBY_DATE", false,true,false,true, true, NULL, 0 }
};

OPTION_MENU_T LISTVIEW_MENU[] = {
    { PandoraMenuItems::MENU, "STR_PANDORA_MENU", false, false, false, true, true, NULL , 0 },
    { PandoraMenuItems::NowListening, "STR_PANDORA_NOW_LISTENING",   false,  false, false, true, true, NULL, 0 },
    { PandoraMenuItems::SortBy, "STR_MENU_SORTBY", false,  false, false, true,  true, SORTBY_MENU, sizeof( SORTBY_MENU ) / sizeof( OPTION_MENU_T ) },
    { PandoraMenuItems::Search, "STR_PANDORA_NEW_STATION",  false,  false, false,  true, true, NULL, 0 },
    //{ PandoraMenuItems::SoundSetting, "STR_PANDORA_SOUNDSETTING",  false, false, false, true, true, NULL, 0 } //deleted by esjang 2013.08.08 ITS #183284 
};

OPTION_MENU_T NOSTATION_MENU[] = {
    { PandoraMenuItems::MENU, "STR_PANDORA_MENU", false, false, false, false, false, NULL , 0 },
    { PandoraMenuItems::NowListening, "STR_PANDORA_NOW_LISTENING",   false,  false, false, false, false, NULL, 0 },
    { PandoraMenuItems::SortBy, "STR_MENU_SORTBY", false,  false, false, false,  false, SORTBY_MENU, sizeof( SORTBY_MENU ) / sizeof( OPTION_MENU_T ) },
    { PandoraMenuItems::Search, "STR_PANDORA_NEW_STATION",  false,  false, false,  true, true, NULL, 0 },
//    { PandoraMenuItems::SoundSetting, "STR_PANDORA_SOUNDSETTING",  false, false, false, true, true, NULL, 0 } // deleted by esjang 2013.08.08 ITS #183284 
};

PandoraMenus::PandoraMenus(QObject *parent)
{
    m_pOptTrackMenuModel = new OptionMenuModel(TRACKVIEW_MENU,(sizeof(TRACKVIEW_MENU)/sizeof(OPTION_MENU_T)));
    m_pOptListMenuModel = new OptionMenuModel(LISTVIEW_MENU,(sizeof(LISTVIEW_MENU)/sizeof(OPTION_MENU_T)));
    m_pOptNoStationMenuModel = new OptionMenuModel(NOSTATION_MENU,(sizeof(NOSTATION_MENU)/sizeof(OPTION_MENU_T)));

    connect(m_pOptTrackMenuModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),this, SIGNAL(menuDataChanged(QModelIndex,QModelIndex)));
    connect(m_pOptListMenuModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),this, SIGNAL(menuDataChanged(QModelIndex,QModelIndex)));
}

void PandoraMenus::UpdateRatingInOptionsMenu(int item, bool rated)
{
    if(item == PandoraMenuItems::ThumbsUp)
    {
        if(rated)
        {
            TRACKVIEW_MENU[2].sIcon = cThumbsUpHighlighted; // thumbs up pressed
            TRACKVIEW_MENU[3].sIcon = cThumbsDownEnable;
        }
        else
        {            
            TRACKVIEW_MENU[2].sIcon = cThumbsUpEnable;
        }
    }
    else if(item == PandoraMenuItems::ThumbsDown)
    {
        if(rated)
        {
            TRACKVIEW_MENU[3].sIcon = cThumbsDownHighlighted;
            TRACKVIEW_MENU[2].sIcon = cThumbsUpEnable;
        }
        else
        {
            TRACKVIEW_MENU[3].sIcon = cThumbsDownEnable;
        }
    }
    m_pOptTrackMenuModel->updateModel(TRACKVIEW_MENU,(sizeof(TRACKVIEW_MENU)/sizeof(OPTION_MENU_T)));
}

void PandoraMenus::UpdateRatingDisableInOptionsMenu()
{
    TRACKVIEW_MENU[2].sIcon = cThumbsUpDisable;
    TRACKVIEW_MENU[3].sIcon = cThumbsDownDisable;
    m_pOptTrackMenuModel->updateModel(TRACKVIEW_MENU,(sizeof(TRACKVIEW_MENU)/sizeof(OPTION_MENU_T)));
}

PandoraMenus::~PandoraMenus()
{
    if(m_pOptTrackMenuModel)
    {
        delete m_pOptTrackMenuModel;
        m_pOptTrackMenuModel = NULL;
    }
    if(m_pOptListMenuModel)
    {
        delete m_pOptListMenuModel;
        m_pOptListMenuModel = NULL;
    }
}

void PandoraMenus::SetSortListMenuModel( int item)
{
    if(item == PandoraMenuItems::Alphabet)
    {
        SORTBY_MENU[1].bSelected = true;
        SORTBY_MENU[2].bSelected = false;
    }
    else if(item == PandoraMenuItems::Date)
    {
        SORTBY_MENU[1].bSelected = false;
        SORTBY_MENU[2].bSelected = true;
    }
    else{

    }
    m_pOptListMenuModel->updateModel(LISTVIEW_MENU,(sizeof(LISTVIEW_MENU)/sizeof(OPTION_MENU_T)));

}
