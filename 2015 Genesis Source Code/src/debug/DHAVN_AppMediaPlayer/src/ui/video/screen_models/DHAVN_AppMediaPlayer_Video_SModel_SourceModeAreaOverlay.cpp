
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_SourceModeAreaOverlay.yml
 * 
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include  <QtGui>

#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_SourceModeAreaOverlay.h"

AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::AppMediaPlayer_Video_SModel_SourceModeAreaOverlay( QObject *parent )
    : QObject(parent)
{
    m_activeTab = 0;
    // added by minho 20120823
    // { for Added active tab on Video
    m_isJukeboxTabVisible = false;
    m_isUSBTabVisible = false;
    m_isUSB2TabVisible = false;
    m_isDiscTabVisible = false;
    m_isAUX1TabVisible = false;
    m_isAUX2TabVisible = false;
    // deleted by minho 20121205 for removed tab on modearea
    /* m_isJukeboxTabSwitchable = false;
    m_isUSBTabSwitchable = false;
    m_isUSB2TabSwitchable = false;
    m_isDiscTabSwitchable = false;
    m_isAUX1TabSwitchable = false;
    m_isAUX2TabSwitchable = false; */
    // deleted by minho 
    // } added by minho
    m_currentFile = 0;
    m_filesNumber = 0;
    m_isMultipleUSB = false;
    m_isListBtnVisible = true; // added by Sergey for CR#14692
    m_isShown = false; // added by Sergey for 24.04.2013
    m_isCounterVisible = true; // added by Sergey 08.05.2013
	// { added by Sergey 09.05.2013
    m_isIconVisible = false;
    m_icon = "";
    m_isRTextVisible = false;
    m_rText = "";
    m_isMenuBtnVisible = true;
	// } added by Sergey 09.05.2013
	m_text = ""; // added by Sergey 16.05.2013
}



void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setActiveTab(int value)
{
    m_activeTab = value;
    emit activeTabChanged( m_activeTab );
}
// added by minho 20120823
// { for Added active tab on Video
void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setIsJukeboxTabVisible(bool value)
{
    m_isJukeboxTabVisible = value;
    emit isJukeboxTabVisibleChanged( m_isJukeboxTabVisible );
}
void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setIsUSBTabVisible(bool value)
{
    m_isUSBTabVisible = value;
    emit isUSBTabVisibleChanged( m_isUSBTabVisible );
}
void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setIsUSB2TabVisible(bool value)
{
    m_isUSB2TabVisible = value;
    emit isUSB2TabVisibleChanged( m_isUSB2TabVisible );
}
void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setIsDiscTabVisible(bool value)
{
    m_isDiscTabVisible = value;
    emit isDiscTabVisibleChanged( m_isDiscTabVisible );
}
void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setIsAUX1TabVisible(bool value)
{
    m_isAUX1TabVisible = value;
    emit isAUX1TabVisibleChanged( m_isAUX1TabVisible );
}
void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setIsAUX2TabVisible(bool value)
{
    m_isAUX2TabVisible = value;
    emit isAUX2TabVisibleChanged( m_isAUX2TabVisible );
}
// deleted by minho 20121205 for removed tab on modearea
/* void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setIsJukeboxTabSwitchable(bool value)
{
    m_isJukeboxTabSwitchable = value;
    emit isJukeboxTabSwitchableChanged( m_isJukeboxTabSwitchable );
}
void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setIsUSBTabSwitchable(bool value)
{
    m_isUSBTabSwitchable = value;
    emit isUSBTabSwitchableChanged( m_isUSBTabSwitchable );
}
void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setIsUSB2TabSwitchable(bool value)
{
    m_isUSB2TabSwitchable = value;
    emit isUSB2TabSwitchableChanged( m_isUSB2TabSwitchable );
}
void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setIsDiscTabSwitchable(bool value)
{
    m_isDiscTabSwitchable = value;
    emit isDiscTabSwitchableChanged( m_isDiscTabSwitchable );
}
void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setIsAUX1TabSwitchable(bool value)
{
    m_isAUX1TabSwitchable = value;
    emit isAUX1TabSwitchableChanged( m_isAUX1TabSwitchable );
}
void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setIsAUX2TabSwitchable(bool value)
{
    m_isAUX2TabSwitchable = value;
    emit isAUX2TabSwitchableChanged( m_isAUX2TabSwitchable );
} */
// deleted by minho 
//// } added by minho
void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setCurrentFile(int value)
{
    m_currentFile = value;
    emit currentFileChanged( m_currentFile );
}
void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setFilesNumber(int value)
{
    m_filesNumber = value;
    emit filesNumberChanged( m_filesNumber );
}
void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setIsMultipleUSB(bool value)
{
    m_isMultipleUSB = value;
    emit isMultipleUSBChanged( m_isMultipleUSB );
}
// { added by Sergey for CR#14692
void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setIsListBtnVisible(bool value)
{
    m_isListBtnVisible = value;
    emit isListBtnVisibleChanged( m_isListBtnVisible );
}
// } added by Sergey for CR#14692.

// { added by Sergey for 24.04.2013
void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setIsShown(bool value)
{
    m_isShown = value;
    emit isShownChanged( m_isShown );
}
// } added by Sergey for 24.04.2013

void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setCounterVisible(bool value)
{
    m_isCounterVisible = value;
    emit counterVisibleChanged( m_isCounterVisible );
} // added by Sergey 08.05.2013

void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setIconVisible(bool value)
{
    m_isIconVisible = value;
    emit iconVisibleChanged( m_isIconVisible );
} // added by Sergey 09.05.2013

void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setIcon(QString value)
{
    m_icon = value;
    emit iconChanged( m_icon );
} // added by Sergey 09.05.2013

void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setRTextVisible(bool value)
{
    m_isRTextVisible = value;
    emit rTextVisibleChanged( m_isRTextVisible );
} // added by Sergey 09.05.2013

void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setRText(QString value)
{
    m_rText = value;
    emit rTextChanged( m_rText );
} // added by Sergey 09.05.2013

void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setMenuBtnVisible(bool value)
{
    m_isMenuBtnVisible = value;
    emit menuBtnVisibleChanged( m_isMenuBtnVisible );
} // added by Sergey 09.05.2013

void AppMediaPlayer_Video_SModel_SourceModeAreaOverlay::setText(QString value)
{
    m_text = value;

    emit textChanged( m_text );
} // added by Sergey 16.05.2013
