
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_SourceModeAreaOverlay.yml
 * 
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_SOURCEMODEAREAOVERLAY_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_SOURCEMODEAREAOVERLAY_H

#include <QObject>

// modified by eugene 2012.10.29 for static analysis defect #7796
//#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_SourceModeAreaOverlay.h" // added by minho 20120823 for Added active tab on Video

class AppMediaPlayer_Video_SModel_SourceModeAreaOverlay : public QObject
{
    Q_OBJECT

    /** @property activeTab
     * The current tab we are playing from
     */
    Q_PROPERTY( int activeTab
                READ activeTab
                NOTIFY activeTabChanged)
    // added by minho 20120823
    // { for Added active tab on Video
    /** @property isJukeboxTabVisible
    * Flag set when Jukebox tab should be visible
    */
    Q_PROPERTY( bool isJukeboxTabVisible
               READ isJukeboxTabVisible
               NOTIFY isJukeboxTabVisibleChanged)
    /** @property isUSBTabVisible
    * Flag set when USB tab should be visible
    */
    Q_PROPERTY( bool isUSBTabVisible
               READ isUSBTabVisible
               NOTIFY isUSBTabVisibleChanged)
    /** @property isUSB2TabVisible
    * Flag set when USB2 tab should be visible
    */
    Q_PROPERTY( bool isUSB2TabVisible
               READ isUSB2TabVisible
               NOTIFY isUSB2TabVisibleChanged)
    /** @property isDiscTabVisible
    * Flag set when Disc tab should be visible
    */
    Q_PROPERTY( bool isDiscTabVisible
               READ isDiscTabVisible
               NOTIFY isDiscTabVisibleChanged)
    /** @property isAUX1TabVisible
    * Flag set when AUX1 tab should be visible
    */
    Q_PROPERTY( bool isAUX1TabVisible
               READ isAUX1TabVisible
               NOTIFY isAUX1TabVisibleChanged)
    /** @property isAUX2TabVisible
    * Flag set when AUX2 tab should be visible
    */
    Q_PROPERTY( bool isAUX2TabVisible
               READ isAUX2TabVisible
               NOTIFY isAUX2TabVisibleChanged)
    /** @property isJukeboxTabSwitchable
    * State reflects if user can switch to this tab
    */
    // deleted by minho 20121205 for removed tab on modearea
    /* Q_PROPERTY( bool isJukeboxTabSwitchable
               READ isJukeboxTabSwitchable
               NOTIFY isJukeboxTabSwitchableChanged) */
    /** @property isUSBTabSwitchable
    * State reflects if user can switch to this tab
    */
    // deleted by minho 20121205 for removed tab on modearea
    /* Q_PROPERTY( bool isUSBTabSwitchable
               READ isUSBTabSwitchable
              NOTIFY isUSBTabSwitchableChanged) */
    /** @property isUSB2TabSwitchable
    * State reflects if user can switch to this tab
    */
    // deleted by minho 20121205 for removed tab on modearea
    /* Q_PROPERTY( bool isUSB2TabSwitchable
               READ isUSB2TabSwitchable
               NOTIFY isUSB2TabSwitchableChanged) */
    /** @property isDiscTabSwitchable
    * State reflects if user can switch to this tab
    */
    // deleted by minho 20121205 for removed tab on modearea
    /* Q_PROPERTY( bool isDiscTabSwitchable
               READ isDiscTabSwitchable
               NOTIFY isDiscTabSwitchableChanged) */
    /** @property isAUX1TabSwitchable
    * State reflects if user can switch to this tab
    */
    // deleted by minho 20121205 for removed tab on modearea
    /* Q_PROPERTY( bool isAUX1TabSwitchable
               READ isAUX1TabSwitchable
               NOTIFY isAUX1TabSwitchableChanged) */
    /** @property isAUX2TabSwitchable
    * State reflects if user can switch to this tab
    */
    // deleted by minho 20121205 for removed tab on modearea
    /* Q_PROPERTY( bool isAUX2TabSwitchable
               READ isAUX2TabSwitchable
               NOTIFY isAUX2TabSwitchableChanged) */
    // } added by minho
    /** @property currentFile
     * currently playing file.
     */
    Q_PROPERTY( int currentFile
                READ currentFile
                NOTIFY currentFileChanged)
    /** @property filesNumber
     * total number of files.
     */
    Q_PROPERTY( int filesNumber
                READ filesNumber
                NOTIFY filesNumberChanged)
    /** @property isMultipleUSB
     * check we have 1 or 2 USB devices.
     */
    Q_PROPERTY( bool isMultipleUSB
                READ isMultipleUSB
                NOTIFY isMultipleUSBChanged)
    // { added by Sergey for CR#14692
    /** @property isListBtnVisible
     * is List button visible.
     */
    Q_PROPERTY( bool isListBtnVisible
                READ isListBtnVisible
                NOTIFY isListBtnVisibleChanged)
    // } added by Sergey for CR#14692.

    Q_PROPERTY( bool isShown
               READ isShown
               NOTIFY isShownChanged) // added by Sergey 24.04.2013

    Q_PROPERTY( bool counterVisible
               READ counterVisible
               NOTIFY counterVisibleChanged) // added by Sergey 08.05.2013

    Q_PROPERTY( bool iconVisible
               READ iconVisible
               NOTIFY iconVisibleChanged) // added by Sergey 09.05.2013

    Q_PROPERTY( QString icon
               READ icon
               NOTIFY iconChanged) // added by Sergey 09.05.2013

    Q_PROPERTY( bool rTextVisible
               READ rTextVisible
               NOTIFY rTextVisibleChanged) // added by Sergey 09.05.2013

    Q_PROPERTY( QString rText
               READ rText
               NOTIFY rTextChanged) // added by Sergey 09.05.2013

    Q_PROPERTY( bool menuBtnVisible
               READ menuBtnVisible
               NOTIFY menuBtnVisibleChanged) // added by Sergey 09.05.2013

    Q_PROPERTY( QString text
               READ text
               NOTIFY textChanged) // added by Sergey 16.05.2013

public:
    AppMediaPlayer_Video_SModel_SourceModeAreaOverlay( QObject *parent = NULL );


    int activeTab() { return m_activeTab; }
    // added by minho 20120823
    // { for Added active tab on Video
    bool isJukeboxTabVisible() { return m_isJukeboxTabVisible; }
    bool isUSBTabVisible() { return m_isUSBTabVisible; }
    bool isUSB2TabVisible() { return m_isUSB2TabVisible; }
    bool isDiscTabVisible() { return m_isDiscTabVisible; }
    bool isAUX1TabVisible() { return m_isAUX1TabVisible; }
    bool isAUX2TabVisible() { return m_isAUX2TabVisible; }
    // deleted by minho 20121205 for removed tab on modearea
    /* bool isJukeboxTabSwitchable() { return m_isJukeboxTabSwitchable; }
    bool isUSBTabSwitchable() { return m_isUSBTabSwitchable; }
    bool isUSB2TabSwitchable() { return m_isUSB2TabSwitchable; }
    bool isDiscTabSwitchable() { return m_isDiscTabSwitchable; }
    bool isAUX1TabSwitchable() { return m_isAUX1TabSwitchable; }
    bool isAUX2TabSwitchable() { return m_isAUX2TabSwitchable; } */
    // } added by minho
    int currentFile() { return m_currentFile; }
    int filesNumber() { return m_filesNumber; }
    bool isMultipleUSB() { return m_isMultipleUSB; }
    bool isListBtnVisible() { return m_isListBtnVisible; } // added by Sergey for CR#14692
    bool isShown() { return m_isShown; } // added by Sergey 24.04.2013
    bool counterVisible() { return m_isCounterVisible; } // added by Sergey 08.05.2013
	// { added by Sergey 09.05.2013
    bool iconVisible() { return m_isIconVisible; }
    QString icon() { return m_icon; }
    bool rTextVisible() { return m_isRTextVisible; }
    QString rText() { return m_rText; }
    bool menuBtnVisible() { return m_isMenuBtnVisible; }
	// } added by Sergey 09.05.2013
	QString text() { return m_text; }

public slots:
    void setActiveTab(int);
    // added by minho 20120823
    // { for Added active tab on Video
    void setIsJukeboxTabVisible(bool);
    void setIsUSBTabVisible(bool);
    void setIsUSB2TabVisible(bool);
    void setIsDiscTabVisible(bool);
    void setIsAUX1TabVisible(bool);
    void setIsAUX2TabVisible(bool);
    // deleted by minho 20121205 for removed tab on modearea
    /* void setIsJukeboxTabSwitchable(bool);
    void setIsUSBTabSwitchable(bool);
    void setIsUSB2TabSwitchable(bool);
    void setIsDiscTabSwitchable(bool);
    void setIsAUX1TabSwitchable(bool);
    void setIsAUX2TabSwitchable(bool); */
    // } added by minho
    void setCurrentFile(int);
    void setFilesNumber(int);
    void setIsMultipleUSB(bool);
    void setIsListBtnVisible(bool); // added by Sergey for CR#14692
    void setIsShown(bool); // added by Sergey 24.04.2013
    void setCounterVisible(bool); // added by Sergey 08.05.2013
	// { added by Sergey 09.05.2013
    void setIconVisible(bool);
    void setIcon(QString);
    void setRTextVisible(bool);
    void setRText(QString);
    void setMenuBtnVisible(bool);
	// } added by Sergey 09.05.2013
	void setText(QString); // added by Sergey 16.05.2013

signals:
    void activeTabChanged( int );
    // added by minho 20120823
    // { for Added active tab on Video
    void isJukeboxTabVisibleChanged( bool );
    void isUSBTabVisibleChanged( bool );
    void isUSB2TabVisibleChanged( bool );
    void isDiscTabVisibleChanged( bool );
    void isAUX1TabVisibleChanged( bool );
    void isAUX2TabVisibleChanged( bool );
    // deleted by minho 20121205 for removed tab on modearea
    /* void isJukeboxTabSwitchableChanged( bool );
    void isUSBTabSwitchableChanged( bool );
    void isUSB2TabSwitchableChanged( bool );
    void isDiscTabSwitchableChanged( bool );
    void isAUX1TabSwitchableChanged( bool );
    void isAUX2TabSwitchableChanged( bool ); */
    // } added by minho
    void currentFileChanged( int );
    void filesNumberChanged( int );
    void isMultipleUSBChanged( bool );
    void isListBtnVisibleChanged(bool); // added by Sergey for CR#14692
    void isShownChanged(bool); // added by Sergey 24.04.2013
    void counterVisibleChanged(bool); // added by Sergey 08.05.2013
	// { added by Sergey 09.05.2013
    void iconVisibleChanged(bool);
    void iconChanged(QString);
    void rTextVisibleChanged(bool);
    void rTextChanged(QString);
    void menuBtnVisibleChanged(bool);
	// } added by Sergey 09.05.2013
	void textChanged(QString); // added by Sergey 16.05.2013


private slots:


private:
    int m_activeTab;
    // added by minho 20120823
    // { for Added active tab on Video
    bool m_isJukeboxTabVisible;
    bool m_isUSBTabVisible;
    bool m_isUSB2TabVisible;
    bool m_isDiscTabVisible;
    bool m_isAUX1TabVisible;
    bool m_isAUX2TabVisible;
    // deleted by minho 20121205 for removed tab on modearea
    /* bool m_isJukeboxTabSwitchable;
    bool m_isUSBTabSwitchable;
    bool m_isUSB2TabSwitchable;
    bool m_isDiscTabSwitchable;
    bool m_isAUX1TabSwitchable;
    bool m_isAUX2TabSwitchable; */
    // } added by minho
    int m_currentFile;    
    int m_filesNumber;    
    bool m_isMultipleUSB;
    bool m_isListBtnVisible; // added by Sergey for CR#14692
    bool m_isShown; // added by Sergey 24.04.2013
    bool m_isCounterVisible; // added by Sergey 08.05.2013
	// { added by Sergey 09.05.2013
    bool m_isIconVisible;
    QString m_icon;
    bool m_isRTextVisible;
    QString m_rText;
    bool m_isMenuBtnVisible;
	// } added by Sergey 09.05.2013
	QString m_text; // added by Sergey 16.05.2013
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_SOURCEMODEAREAOVERLAY_H

