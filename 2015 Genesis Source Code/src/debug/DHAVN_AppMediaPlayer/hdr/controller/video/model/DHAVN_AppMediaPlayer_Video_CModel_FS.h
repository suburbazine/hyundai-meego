#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_CMODEL_FS_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_CMODEL_FS_H

#include "DHAVN_AppMediaPlayer_Video_CModel_Base.h"

class AppMediaPlayer_Video_CModel_FS : public AppMediaPlayer_Video_CModel_Base
{
    Q_OBJECT
public:
    /** @brief The caption size*/
    enum CAPTION_SIZE_T
    {
        CAPTION_SIZE_SMALL  = 0,
        CAPTION_SIZE_NORMAL = 1,
        CAPTION_SIZE_LARGE  = 2
    };
    
    AppMediaPlayer_Video_CModel_FS( QObject *parent = NULL );
    virtual ~AppMediaPlayer_Video_CModel_FS();

    /** @property filename
     * @brief Reflects the playing file's name or some file name in playlist
     * during tune rotation. This filename is shown on jukebox/USB playback screen .
     * Read-only.
     * When playing not from jukebox or USB the value is undefined.
     * The property is read-only.
     * */
    QString filename() { return m_filename; }
    void setFilename( const QString &value , bool bUpdate = true);

    /** @property playingFilename
     * @brief Reflects the playing file's name. Read-only.
     * When playing not from jukebox or USB the value is undefined.
     * The property is read-only.
     * */
    QString playingFilename() { return m_playingFilename; }
    void setPlayingFilename( const QString &value );

    /** @property folderName
     * @brief Reflects the playing folder's name. Read-only.
     * When playing not from jukebox or USB the value is undefined.
     * The property is read-only.
     * */
    QString foldername() { return m_folderName; }
    void setFolderName( const QString &value );

    //for getting root folder name string as "/.."
    QString getFolderNameStr(const QString &value);//added by aettie.ji 2013.01.31 for ISV 70943

    /** @property fullFolderName
     * @brief Reflects the playing full folder's name. Read-only.
     * When playing not from jukebox or USB the value is undefined.
     * The property is read-only.
     * */
    QString fullfoldername() { return m_fullFolderName; }
    void setFullFolderName( const QString &value );

    /** @property width
     * @brief Reflects the width of the currently played media.
     * When playing not from jukebox or USB the value is undefined.
     * The property should be considered read only.
     */
    int width() { return m_width; }
    void setWidth( int value );
   
    /** @property height
     * @brief Reflects the height of the currently played media.
     * When playingnot from jukebox or USB the value is undefined.
     * The property should be considered to be read only.
     */
    int height() { return m_height; }
    void setHeight( int value );
   
    /** @property duration
     * @brief Reflects the duration of the currently played media in milliseconds.
     * When playing not from the jukebox or USB the value is undefined.
     * The property should be considered to be read-only.
     */
    int duration() { return m_duration; }
    void setDuration( int value );

    // { modified by kihyung 2013.09.11 for ITS 0189474
    /** @property displayPlaylistLength
     * @brief The number of files in the playlist.
     * When playing not from the jukebox or USB, the value is undefined.
     * The property should be considered to be read-only.
     */
    int displayPlaylistLength() { return m_displayPlaylistLength; }
    void setDisplayPlaylistLength( int value );
   
    /** @property playlistPosition
     * @brief Currently played item in the playlist. Zero-based.
     * When playing not from Jukebox or USB, the value is undefined.
     * The property should be considered to be read-only.
     */
    int playlistPosition() { return m_playlistPosition; }
    void setPlaylistPosition( int value );

    int displayPlaylistPos() { return m_displayPlaylistPos; }
    void setDisplayPlaylistPos( int value );
    // } modified by kihyung 2013.09.11 for ITS 0189474
   
    /** @property captionSize
     * @brief Currently size of the caption.
     * 0 - small size of the caption, 1 - normal size of the caption,
     * 2 - large size of the caption.
     */
    CAPTION_SIZE_T captionSize() { return m_nCaptionSize; }
    void setCaptionSize( CAPTION_SIZE_T value );
   
    /** @property captionEnable
     * @brief Currently caption is enabled or disabled.
     * true - caption is enable, false - caption is disable.
     */
    bool captionEnable() { return m_bCaptionEnable; }
    void setCaptionEnable( bool value );
   
    QStringList captionLangList() { return m_captionLangList; }
    void setCaptionLangList(QStringList value);  // added by Sergey 20.08.2013 for ITS#184640 

    int captionCurId() { return m_captionCurId; }
    void setCaptionCurId(int value);  // added by Sergey 20.08.2013 for ITS#184640 

    bool multipleCaptions() { return m_bMultipleCaptions; }
    void setMultipleCaptions(bool value); // added by Sergey 09.10.2013 for ITS#187939

    bool smiCaptions() { return m_bSMICaptions; }
    void setSmiCaptions(bool value); // added by Sergey 09.10.2013 for ITS#187939

    /** @property copyPercentage
     * @brief Current percentage of copy operation. Zero-based.
     * When playing not from USB, the value is undefined.
     * The property should be considered to be read-only.
     */
    int copyPercentage() { return m_copyPercentage; }
    void setCopyPercentage( int value );
   
    QString subtileStr() { return m_subtitleStr; }
    void setSubtitleStr( const QString &value );
   
    QString restoreFile() { return m_restoreFile; }
    void setRestoreFile( const QString &value );
   
    QString fileFromFM() { return m_fileFromFM; }
    void setFileFromFM( const QString &value );
   
    int restorePosition() { return m_restorePosition; }
    void setRestorePosition( int value );
     
    /*Language Settings*/
    QStringList getLanguageCode(){ return langcodelist;}
    void setLanguageCode(QStringList langeCodelist);
   
    int getCurrentStreamId(){return m_currentStreamId;}
    void setCurrentStreamId(int currentstreamid);

    bool voiceLangEnabled() { return m_bVoiceLangEnabled; }
    void setVoiceLangEnabled(bool value); // added by Sergey 09.10.2013 for ITS#192050
   
    // removed by Sergey 07.09.2013 for ITS#185543
   
    // { added by Sergey for CR#16015
    int drmUseCount() { return m_drmUseCount; }
    void setDrmUseCount( int value );
   
    int drmUseLimit() { return m_drmUseLimit; }
    void setDrmUseLimit( int value );
   
    QString lastPlayedUrl() { return m_lastPlayedUrl; }
    void setLastPlayedUrl( QString value );
   
    int lastPlayedPos() { return m_lastPlayedPos; }
    void setLastPlayedPos( int value );
   
    QString playingUrl() { return m_playingUrl; }
    void setPlayingUrl( QString value );
    // } added by Sergey for CR#16015.
   
   // removed by Sergey for Loading popup
    
    // { added by Sergey for CR#11607
    bool readingFile() { return m_readingFile; }
    void setReadingFile(bool);
    // } added by Sergey for CR#11607
   
    void setXSubCaptionVisibility(bool show); // modified by ravikanth 04-04-13
   
    void showCaption(bool bShow);
   
    void backupForUnsupported(); // added by kihyung 2013.06.18
    void restoreForUnsupported(); // added by kihyung 2013.06.18

    void setCurrentSubtitleChanged(int mediaSource); // added by kihyung 2013.08.22 for 0185761 

    // { added by kihyung 2013.07.19 for ITS 0179774
    QString deviceID() { return m_deviceID; }
    void setDeviceID(QString deviceID);

    QString mountPath() { return m_mountPath; }
    void setMountPath(QString mountPath);

    void copyHistory(AppMediaPlayer_Video_CModel_FS *pOther);    
    void copyHistoryFromExtended(AppMediaPlayer_Video_CModel_FS *pOther, QString mountPath);
    void copyHistoryToExtended(AppMediaPlayer_Video_CModel_FS *pOther);    
    void initModel();
    // } added by kihyung 2013.07.19 for ITS 0179774
    bool isUnsupported() { return m_unsupported; } // added by Sergey 07.10.2013 for ITS#193743
    bool isRootFolder() { return m_bFolderIsRoot; } // added by Sergey 26.11.2013 for Root folder translation
    
signals:
    /// @{ property notifiers
    void filenameChanged( const QString &value );
    void playingFilenameChanged( const QString &value );
    void foldernameChanged( const QString &value );
    void subtileStrChanged( const QString &value );
    void fullfoldernameChanged( const QString &value );
    void widthChanged( int value );
    void heightChanged( int value );
    void tuneSameFileChanged(bool value);//added by yongkyun.lee  2012.10.17  for Current file is white color
   
    void durationChanged( int value );
    void playlistLengthChanged( int value );
    void playlistPositionChanged( int value );
    void captionSizeChanged( int value );
    void captionEnableChanged( bool value );
    void captionLangListChanged(QStringList value);  // added by Sergey 20.08.2013 for ITS#184640 
    void captionCurIdChanged(int value);  // added by Sergey 20.08.2013 for ITS#184640 
    void multipleCaptionsChanged(bool);
    void smiCaptionsChanged(bool); // added by Sergey 09.10.2013 for ITS#187939
    void copyPercentageChanged( int value );
    // removed by Sergey 07.09.2013 for ITS#185543
    // { added by Sergey for CR#16015
    void drmUseCountChanged(int);
    void drmUseLimitChanged(int);
    void lastPlayedFileChanged(QString);
    void lastPlayedPosChanged(int);
    void playingUrlChanged(QString);
    // { added by Sergey for CR#16015.
    // removed by Sergey for Loading popup
    void readingFileChanged(bool); // added by Sergey for CR#11607
    void xsubCaptionVisibilityChanged(bool); // modified by ravikanth 04-04-13
    void currentSubtitleSettingChanged(int); // added by kihyung 2013.08.22 for 0185761 
    void voiceLangEnabledChanged(bool); // added by Sergey 09.10.2013 for ITS#192050
    
private:
    QString         m_filename;
    QString         m_playingFilename;
    QString         m_folderName;
    QString         m_subtitleStr;
    QString         m_fullFolderName;
    QString         m_restoreFile;
    QString         m_fileFromFM;
    int             m_restorePosition;
    int             m_width;
    int             m_height;
    int             m_duration;
    // { modified by kihyung 2013.09.11 for ITS 0189474
    int             m_playlistPosition;
    int             m_displayPlaylistLength;    
    int             m_displayPlaylistPos;
    // } modified by kihyung 2013.09.11 for ITS 0189474
    
    CAPTION_SIZE_T  m_nCaptionSize;
    bool            m_bCaptionEnable;
    QStringList     m_captionLangList;
    bool            m_bMultipleCaptions;
    bool            m_bSMICaptions; // added by Sergey 09.10.2013 for ITS#187939
    int             m_captionCurId;  // added by Sergey 20.08.2013 for ITS#184640 
    int             m_copyPercentage;
    bool            m_bVoiceLangEnabled; // added by Sergey 09.10.2013 for ITS#192050
    QStringList     langcodelist;
    int             m_currentStreamId;
    int             m_drmUseCount;
    int             m_drmUseLimit;
    QString         m_lastPlayedUrl;
    int             m_lastPlayedPos;
    QString         m_playingUrl;
    bool            m_readingFile;
    
    bool            m_unsupported;
    QString         m_prevFolderName;
    QString         m_prevfullFolderName;
    QString         m_prevFilename;
    int             m_prevPlaylistLength;
    int             m_prevPlaylistPosition;

    // { added by kihyung 2013.07.19 for ITS 0179774
    QString         m_mountPath;
    QString         m_deviceID;
    // } added by kihyung 2013.07.19 for ITS 0179774
    bool            m_bFolderIsRoot; // added by Sergey 26.11.2013 for Root folder translation
};

Q_DECLARE_METATYPE(AppMediaPlayer_Video_CModel_FS::CAPTION_SIZE_T)

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_CMODEL_FS_H
