#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SCREENS_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SCREENS_H

#include <QString>

/** @class AppMediaPlayer_Video_Screens
 * Placeholder for the string constants identifying screens and views.
 * We cannot hardcode them becous this would likely cause misprints and
 * bugs. So enjoy.
 * */
class AppMediaPlayer_Video_Screens
{
public:
    static const QString SCREEN_JUKEBOX_PLAYBACK;
    static const QString SCREEN_USB_PLAYBACK;
    static const QString SCREEN_USB2_PLAYBACK;
    static const QString SCREEN_DVD_PLAYBACK;
    static const QString SCREEN_VCD_PLAYBACK;
    static const QString SCREEN_AUX1_PLAYBACK;
    static const QString SCREEN_AUX2_PLAYBACK;
    static const QString SCREEN_DVD_TITLES;
    static const QString SCREEN_DVD_CHAPTERS;
    static const QString SCREEN_DVD_SETTINGS;
    static const QString SCREEN_VCD_CHAPTERS;
    static const QString SCREEN_FS_CAPTIONS;
    static const QString SCREEN_OPTIONS;
    static const QString SCREEN_FS_FILEINFO_POPUP;
    static const QString SCREEN_DVD_TITLE_DISC_MENU;
    static const QString SCREEN_FULL;
    // removed by Sergey 10.08.2013 for 182892
    static const QString SCREEN_DIVXREG; // modified by Sergey. Removed Divx part.

    static const QString OVERLAY_SOURCE_MODEAREA;
    static const QString SCREEN_VCD_PBCMENU;
    static const QString SCREEN_DVD_SEARCHCAPTION;
    static const QString SCREEN_VCD_DIRECTACCESS;

    static const QString SCREEN_POP_PROGRESS;
    static const QString SCREEN_POP_TOAST;
    static const QString SCREEN_POP_TEXTBUTTON;

    static const QString VIEW_PLAYBACK;
    static const QString VIEW_DVD_TITLES;
    static const QString VIEW_DVD_CHAPTERS;
    static const QString VIEW_DVD_SETTINGS;
    static const QString VIEW_VCD_CHAPTERS ;
    static const QString VIEW_FS_CAPTIONS;
    static const QString VIEW_SOURCE_MODEAREA_OVERLAY;
    static const QString VIEW_VCD_PBCMENU;
    static const QString VIEW_VCD_DIRECTACCESS;
    static const QString VIEW_OPTIONS;
    static const QString VIEW_FS_FILEINFO_POPUP;
    static const QString VIEW_DVD_SEARCHCAPTION;
    static const QString VIEW_DVD_TITLE_DISC_MENU;
    static const QString VIEW_POP_PROGRESS;
    static const QString VIEW_POP_TOAST;
    static const QString VIEW_POP_TEXTBUTTON;
	static const QString VIEW_EMPTY; //added by edo.lee 2013.02.06
	static const QString VIEW_DIVXREG; // modified by Sergey. Removed Divx part.
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SCREENS_H
