#include <DHAVN_AppAhaMenus.h>
#include <DHAVN_AppAhaMenus_def.h>

//hsryu_0306_change_menu
OPTION_MENU_T TRACKVIEW_MENU[] = {
    { AhaMenuItems::MENU,"STR_AHA_MENU",                false, false, false, true, true, NULL, 0 },
    { AhaMenuItems::StationList,"STR_AHA_STATIONLIST",  false, false, false, true, true, NULL, 0 },
    { AhaMenuItems::ContentsList,"STR_AHA_CONTENTSLIST",false, false, false, true, true, NULL, 0 },
    { AhaMenuItems::ThumbsUp, "",                       false, false, false, true, true, NULL, 0 , ""},/*wsuk.kim menu_option*/
    { AhaMenuItems::ThumbsDown, "",                     false, false, false, true, true, NULL, 0 , ""}, /*wsuk.kim menu_option*/
    { AhaMenuItems::Call, "STR_AHA_CALL",               false, false, false, true, true, NULL, 0 },
    { AhaMenuItems::Navigate, "STR_AHA_NAVIGATE",       false, false, false, true, true, NULL, 0 },
    { AhaMenuItems::SoundSetting, "STR_AHA_SOUNDSETTING", false,  false, false,  true,  true, NULL, 0 }
};

OPTION_MENU_T LISTVIEW_MENU[] = {
    { AhaMenuItems::MENU, "STR_AHA_MENU", false, false, false, true, true, NULL, 0 },
    { AhaMenuItems::NowListening, "STR_AHA_NOW_LISTENING",   false,  false, false, true, true, NULL, 0 }
    /*wsuk.kim remove_soundsetting_menu,
    { AhaMenuItems::SoundSetting, "STR_AHA_SOUNDSETTING",  false, false, false, true, true, NULL, 0 }*/
};

AhaMenus::AhaMenus(QObject *pParent)
{
    m_pOptTrackMenuModel = new OptionMenuModel(TRACKVIEW_MENU,(sizeof(TRACKVIEW_MENU)/sizeof(OPTION_MENU_T)));
    m_pOptListMenuModel = new OptionMenuModel(LISTVIEW_MENU,(sizeof(LISTVIEW_MENU)/sizeof(OPTION_MENU_T)));

    connect(m_pOptTrackMenuModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),this, SIGNAL(menuDataChanged(QModelIndex,QModelIndex)));
//    connect(m_pOptListMenuModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)),this, SIGNAL(menuDataChanged(QModelIndex,QModelIndex)));
}

//wsuk.kim dynamic_menu
void AhaMenus::InitEnabled(int menuCount)
{
    for(int i = 0; i < menuCount; i++)
    {
        TRACKVIEW_MENU[i].bEnabled = true;
    }
}
//wsuk.kim dynamic_menu

/*wsuk.kim menu_option*/
void AhaMenus::UpdateLikeDislikeIconInOptionsMenu(QString strCurrLikeIcon, QString strCurrDislikeIcon)
{
    if(strCurrLikeIcon != "")
    {
        TRACKVIEW_MENU[AhaMenuItems::ThumbsUp].sIcon = strCurrLikeIcon;
        TRACKVIEW_MENU[AhaMenuItems::ThumbsDown].sIcon = strCurrDislikeIcon;
    }
    else
    {
        TRACKVIEW_MENU[AhaMenuItems::ThumbsUp].sIcon = strCurrDislikeIcon;
        TRACKVIEW_MENU[AhaMenuItems::ThumbsDown].sIcon = "";
    }
}
/*wsuk.kim menu_option*/

#ifndef DYNAMIC_MENU //wsuk.kim dynamic_menu
void AhaMenus::UpdateAllowInOptionsMenu(bool bAllowLike, bool bAllowDislike, bool bAllowCall, bool bAllowNavigate, bool bNoNetwork) // modified ITS 225504, 230990
{
    int menuCount, menuCallNavi;

    // Disabling Like,Dislike if icon image is empty 2014.03.17
    bool AllowLikeVal, AllowDislikeVal;
    AllowLikeVal = bAllowLike;
    AllowDislikeVal = bAllowDislike;

    if ((bAllowLike || bAllowDislike) && TRACKVIEW_MENU[AhaMenuItems::ThumbsUp].sIcon == "")
    {
        AllowLikeVal = 0;
        AllowDislikeVal = 0;
    }

    menuCount = 0;
    menuCallNavi = 0;

    if(bAllowCall||bAllowNavigate)
    {
        menuCallNavi = 2;
    }
    menuCount = AllowLikeVal + AllowDislikeVal + menuCallNavi + 4;
    InitEnabled(menuCount);

    if(AllowLikeVal || AllowDislikeVal)
    {
        if(AllowLikeVal && AllowDislikeVal)
        {
            TRACKVIEW_MENU[AhaMenuItems::ThumbsUp].sText = "";
            TRACKVIEW_MENU[AhaMenuItems::ThumbsUp].nId = AhaMenuItems::ThumbsUp;
            TRACKVIEW_MENU[AhaMenuItems::ThumbsDown].sText = "";
            TRACKVIEW_MENU[AhaMenuItems::ThumbsDown].nId = AhaMenuItems::ThumbsDown;
            if(bAllowCall || bAllowNavigate)
            {
                TRACKVIEW_MENU[AhaMenuItems::Call].sText = "STR_AHA_CALL";
                TRACKVIEW_MENU[AhaMenuItems::Call].nId = AhaMenuItems::Call;
                TRACKVIEW_MENU[AhaMenuItems::Call].bEnabled = bAllowCall && !bNoNetwork ? true : false;

                TRACKVIEW_MENU[AhaMenuItems::Navigate].sText = "STR_AHA_NAVIGATE";
                TRACKVIEW_MENU[AhaMenuItems::Navigate].nId = AhaMenuItems::Navigate;
                TRACKVIEW_MENU[AhaMenuItems::Navigate].bEnabled = bAllowNavigate && !bNoNetwork ? true : false;

                TRACKVIEW_MENU[AhaMenuItems::SoundSetting].sText = "STR_AHA_SOUNDSETTING";
                TRACKVIEW_MENU[AhaMenuItems::SoundSetting].nId = AhaMenuItems::SoundSetting;
            }
            else
            {
                TRACKVIEW_MENU[AhaMenuItems::Call].sText = "STR_AHA_SOUNDSETTING";
                TRACKVIEW_MENU[AhaMenuItems::Call].nId = AhaMenuItems::SoundSetting;
            }
        }
        else if((AllowLikeVal && !AllowDislikeVal) || (!AllowLikeVal && AllowDislikeVal))
        {
            if(AllowLikeVal)
            {
                TRACKVIEW_MENU[AhaMenuItems::ThumbsUp].nId = AhaMenuItems::ThumbsUp;
            }
            else if(AllowDislikeVal)
            {
                TRACKVIEW_MENU[AhaMenuItems::ThumbsUp].nId = AhaMenuItems::ThumbsDown;
            }
            TRACKVIEW_MENU[AhaMenuItems::ThumbsUp].sText = "";
            TRACKVIEW_MENU[AhaMenuItems::ThumbsDown].sIcon = "";
            if(bAllowCall || bAllowNavigate)
            {
                TRACKVIEW_MENU[AhaMenuItems::ThumbsDown].sText = "STR_AHA_CALL";
                TRACKVIEW_MENU[AhaMenuItems::ThumbsDown].nId = AhaMenuItems::Call;
                TRACKVIEW_MENU[AhaMenuItems::ThumbsDown].bEnabled = bAllowCall && !bNoNetwork ? true : false;

                TRACKVIEW_MENU[AhaMenuItems::Call].sText = "STR_AHA_NAVIGATE";
                TRACKVIEW_MENU[AhaMenuItems::Call].nId = AhaMenuItems::Navigate;
                TRACKVIEW_MENU[AhaMenuItems::Call].bEnabled = bAllowNavigate && !bNoNetwork ? true : false;

                TRACKVIEW_MENU[AhaMenuItems::Navigate].sText = "STR_AHA_SOUNDSETTING";
                TRACKVIEW_MENU[AhaMenuItems::Navigate].nId = AhaMenuItems::SoundSetting;
            }
            else
            {
                TRACKVIEW_MENU[AhaMenuItems::ThumbsDown].sText = "STR_AHA_SOUNDSETTING";
                TRACKVIEW_MENU[AhaMenuItems::ThumbsDown].nId = AhaMenuItems::SoundSetting;
            }
        }
    }
    else if((!AllowLikeVal && !AllowDislikeVal) && (bAllowCall || bAllowNavigate))
    {
        TRACKVIEW_MENU[AhaMenuItems::ThumbsUp].sText = "STR_AHA_CALL";
        TRACKVIEW_MENU[AhaMenuItems::ThumbsUp].nId = AhaMenuItems::Call;
        TRACKVIEW_MENU[AhaMenuItems::ThumbsUp].bEnabled = bAllowCall && !bNoNetwork ? true : false;
        TRACKVIEW_MENU[AhaMenuItems::ThumbsUp].sIcon = "";

        TRACKVIEW_MENU[AhaMenuItems::ThumbsDown].sText = "STR_AHA_NAVIGATE";
        TRACKVIEW_MENU[AhaMenuItems::ThumbsDown].nId = AhaMenuItems::Navigate;
        TRACKVIEW_MENU[AhaMenuItems::ThumbsDown].bEnabled = bAllowNavigate && !bNoNetwork ? true : false;
        TRACKVIEW_MENU[AhaMenuItems::ThumbsDown].sIcon = "";

        TRACKVIEW_MENU[AhaMenuItems::Call].sText = "STR_AHA_SOUNDSETTING";
        TRACKVIEW_MENU[AhaMenuItems::Call].nId = AhaMenuItems::SoundSetting;
    }
    else if(!AllowLikeVal && !AllowDislikeVal && !bAllowCall && !bAllowNavigate)
    {
        TRACKVIEW_MENU[AhaMenuItems::ThumbsUp].sText = "STR_AHA_SOUNDSETTING";
        TRACKVIEW_MENU[AhaMenuItems::ThumbsUp].nId = AhaMenuItems::SoundSetting;
        TRACKVIEW_MENU[AhaMenuItems::ThumbsUp].sIcon = "";
    }

    // ITS 225504
    for(int inx = 0; inx < menuCount; inx++)
    {
        if(TRACKVIEW_MENU[inx].nId != AhaMenuItems::SoundSetting &&
                TRACKVIEW_MENU[inx].nId != AhaMenuItems::Call &&
                TRACKVIEW_MENU[inx].nId != AhaMenuItems::Navigate )
        {
            TRACKVIEW_MENU[inx].bEnabled  = !bNoNetwork;
        }
    }

    m_pOptTrackMenuModel->updateModel(TRACKVIEW_MENU, menuCount);
}
#endif

AhaMenus::~AhaMenus()
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
