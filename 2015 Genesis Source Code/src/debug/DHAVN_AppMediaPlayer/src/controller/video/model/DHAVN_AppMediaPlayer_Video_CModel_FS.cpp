#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_FS.h"

AppMediaPlayer_Video_CModel_FS::AppMediaPlayer_Video_CModel_FS( QObject *parent )
    :AppMediaPlayer_Video_CModel_Base(parent)
{
    initModel(); // modified by kihyung 2013.07.20 for ITS 0179774
}


AppMediaPlayer_Video_CModel_FS::~AppMediaPlayer_Video_CModel_FS()
{
}


void AppMediaPlayer_Video_CModel_FS::setFilename( const QString &value, bool bUpdate )
{
    MP_LOG << "value =" << value << bUpdate << LOG_ENDL;
    if(bUpdate == true)
        m_filename = value;
    emit filenameChanged( value );
}

void AppMediaPlayer_Video_CModel_FS::setPlayingFilename( const QString &value )
{
    MP_LOG << "value =" << value << LOG_ENDL;
    m_playingFilename = value;
    emit playingFilenameChanged( value );
}

void AppMediaPlayer_Video_CModel_FS::setFolderName(const QString &value)
{
   MP_LOG << "value =" << value << LOG_ENDL;
   m_folderName = value;
   emit foldernameChanged( value );
}
//{added by aettie.ji 2013.01.31 for ISV 70943
QString AppMediaPlayer_Video_CModel_FS::getFolderNameStr(const QString &value)
{
    QString tmpStr = value;
    QString folderTmp = "";

    MP_LOG << "value =" << value << LOG_ENDL;

    if(!tmpStr.isEmpty())
    {
        folderTmp = tmpStr.section('/', -2, -2);
    }
    //modified by aettie 20130522 folder name for NP
    //if((folderTmp == "Video" && tmpStr.count('/') == 4)||((tmpStr.section('/',-3,-3).contains("usb"))&&tmpStr.count('/') == 4))
    if(  (folderTmp == "Video" && tmpStr.count('/') == 4)||
            (tmpStr.section('/',-3,-3).contains("front_usb"))||
            (tmpStr.section('/',-3,-3).contains("rear_usb"))    )
    {
        ////modified by aettie 2013 08 05 for ISV NA 85625
        //	folderTmp = "/..";
        // modified by aettie 20130812 for ITS 183630
        //folderTmp = "Root";
        // { modified by Sergey 26.11.2013 for Root folder translation
        m_bFolderIsRoot = true; 
        folderTmp = QApplication::translate("main",QT_TR_NOOP(  "STR_MEDIA_ROOT_FOLDER"));
    }
    else
    {
        m_bFolderIsRoot = false;
    }
    // } modified by Sergey 26.11.2013 for Root folder translation
    
    return folderTmp;
}
//}added by aettie.ji 2013.01.31 for ISV 70943
void AppMediaPlayer_Video_CModel_FS::setFullFolderName(const QString &value)
{
   m_fullFolderName = value;
   emit fullfoldernameChanged( value );
}

void AppMediaPlayer_Video_CModel_FS::setWidth( int value )
{
    m_width = value;
    emit widthChanged( value );
}


void AppMediaPlayer_Video_CModel_FS::setHeight( int value )
{
    m_height = value;
    emit heightChanged( value );
}

void AppMediaPlayer_Video_CModel_FS::setDuration( int value )
{
    m_unsupported = false; // added by kihyung 2013.07.09 for ITS 0179048 
    m_duration = value;
    emit durationChanged(value);
}

// { modified by kihyung 2013.09.11 for ITS 0189474 
void AppMediaPlayer_Video_CModel_FS::setDisplayPlaylistLength( int value )
{
    MP_LOG<<"value = "<<value << LOG_ENDL;
    m_displayPlaylistLength = value;
    emit playlistLengthChanged(value);
}

void AppMediaPlayer_Video_CModel_FS::setPlaylistPosition( int value )
{
    MP_LOG<<"value = "<<value << LOG_ENDL;
    m_playlistPosition = value;
}

void AppMediaPlayer_Video_CModel_FS::setDisplayPlaylistPos( int value )
{
    MP_LOG<<"value = "<<value << LOG_ENDL;
    m_displayPlaylistPos = value;
    emit playlistPositionChanged( value );
}
// } modified by kihyung 2013.09.11 for ITS 0189474

void AppMediaPlayer_Video_CModel_FS::setCaptionSize( CAPTION_SIZE_T value )
{
   m_nCaptionSize = value;
   emit captionSizeChanged( value );
}

void AppMediaPlayer_Video_CModel_FS::setCaptionEnable( bool value )
{
   m_bCaptionEnable = value;
   emit captionEnableChanged( value );
}


 // { added by Sergey 20.08.2013 for ITS#184640 
void AppMediaPlayer_Video_CModel_FS::setCaptionLangList( QStringList value )
{
   m_captionLangList = value;
   emit captionLangListChanged( value );
}

void AppMediaPlayer_Video_CModel_FS::setCaptionCurId( int value )
{
   m_captionCurId = value;
   emit captionCurIdChanged( value );
}
 // } added by Sergey 20.08.2013 for ITS#184640

 
void AppMediaPlayer_Video_CModel_FS::setMultipleCaptions( bool value )
{
   m_bMultipleCaptions = value;
   emit multipleCaptionsChanged( value );
} // added by Sergey 09.10.2013 for ITS#187939


void AppMediaPlayer_Video_CModel_FS::setSmiCaptions(bool value)
{
   m_bSMICaptions = value;
   emit smiCaptionsChanged( value );
} // added by Sergey 09.10.2013 for ITS#187939


void AppMediaPlayer_Video_CModel_FS::setCopyPercentage( int value )
{
   m_copyPercentage = value;
   emit copyPercentageChanged( value );
}

void AppMediaPlayer_Video_CModel_FS::setSubtitleStr( const QString &value )
{
    m_subtitleStr = value;
    emit subtileStrChanged( value );
}

void AppMediaPlayer_Video_CModel_FS::setRestoreFile( const QString &value )
{
   MP_LOG<<"value = "<<value << LOG_ENDL;
   m_restoreFile = value;
}

void AppMediaPlayer_Video_CModel_FS::setFileFromFM( const QString &value )
{
   MP_LOG<<"value = "<<value << LOG_ENDL;
   m_fileFromFM = value;
}

void AppMediaPlayer_Video_CModel_FS::setRestorePosition( int value )
{
   MP_LOG<<"value = "<<value << LOG_ENDL;
   m_restorePosition = value;
}
void AppMediaPlayer_Video_CModel_FS::setLanguageCode(QStringList langeCodelist)
{
    langcodelist =langeCodelist;
}

void AppMediaPlayer_Video_CModel_FS::setCurrentStreamId(int currentstreamid)
{
    m_currentStreamId =currentstreamid;
}

void AppMediaPlayer_Video_CModel_FS::setVoiceLangEnabled(bool value)
{
    m_bVoiceLangEnabled = value;
    emit voiceLangEnabledChanged(m_bVoiceLangEnabled);
} // added by Sergey 09.10.2013 for ITS#192050

// removed by Sergey 07.09.2013 for ITS#185543

// { added by Sergey for CR#16015
void AppMediaPlayer_Video_CModel_FS::setDrmUseCount(int value)
{
    m_drmUseCount = value;
    emit drmUseCountChanged(value);
}

void AppMediaPlayer_Video_CModel_FS::setDrmUseLimit(int value)
{
    m_drmUseLimit = value;
    emit drmUseLimitChanged(value);
}

void AppMediaPlayer_Video_CModel_FS::setLastPlayedUrl(QString value)
{
    m_lastPlayedUrl = value;
    emit lastPlayedFileChanged(value);
}

void AppMediaPlayer_Video_CModel_FS::setLastPlayedPos(int value)
{
    m_lastPlayedPos = value;
    emit lastPlayedPosChanged(value);
}

void AppMediaPlayer_Video_CModel_FS::setPlayingUrl(QString value)
{
    m_playingUrl = value;
    emit playingUrlChanged(value);
}
// } added by Sergey for CR#16015.

// removed by Sergey for Loading popup

// { added by Sergey for CR#11607
void AppMediaPlayer_Video_CModel_FS::setReadingFile( bool value )
{
    if(m_readingFile != value)
    {
        m_readingFile = value;
        emit readingFileChanged(value);
    }
}
// } added by Sergey for CR#11607

// { modified by ravikanth 04-04-13
void AppMediaPlayer_Video_CModel_FS::setXSubCaptionVisibility(bool show)
{
    emit xsubCaptionVisibilityChanged(show);
}
// } modified by ravikanth 04-04-13


// added by kihyung 2013.06.14
void AppMediaPlayer_Video_CModel_FS::showCaption(bool bShow)
{
    MP_LOG << bShow << LOG_ENDL;
    emit subtileStrChanged("");
    emit captionEnableChanged( bShow );
}

// { modified by kihyung 2013.09.11 for ITS 0189474
// added by kihyung 2013.06.18
void AppMediaPlayer_Video_CModel_FS::backupForUnsupported()
{
    if(m_unsupported == false) // added by kihyung 2013.11.05 for ITS 0206194 
    {
        m_unsupported           = true;
        m_prevFolderName        = m_folderName;
        m_prevfullFolderName    = m_fullFolderName;
        m_prevFilename          = m_filename;
        m_prevPlaylistLength    = m_displayPlaylistLength;
        m_prevPlaylistPosition  = m_displayPlaylistPos;

        MP_LOG << m_prevFolderName << LOG_ENDL;
        MP_LOG << m_prevfullFolderName << LOG_ENDL;
        MP_LOG << m_prevFilename << LOG_ENDL;
        MP_LOG << m_prevPlaylistLength << LOG_ENDL;
        MP_LOG << m_prevPlaylistPosition << LOG_ENDL;
    }
}

void AppMediaPlayer_Video_CModel_FS::restoreForUnsupported()
{
    if(m_unsupported == true) {
        m_unsupported           = false;
        m_folderName            = m_prevFolderName;
        m_fullFolderName        = m_prevfullFolderName;
        m_filename              = m_prevFilename;
        m_displayPlaylistLength = m_prevPlaylistLength;
        m_displayPlaylistPos    = m_prevPlaylistPosition;

        MP_LOG << m_folderName << LOG_ENDL;
        MP_LOG << m_fullFolderName << LOG_ENDL;
        MP_LOG << m_filename << LOG_ENDL;
        MP_LOG << m_displayPlaylistLength << LOG_ENDL;
        MP_LOG << m_displayPlaylistPos << LOG_ENDL;
    }
}
// } modified by kihyung 2013.09.11 for ITS 0189474

// added by kihyung 2013.08.22 for 0185761 
void AppMediaPlayer_Video_CModel_FS::setCurrentSubtitleChanged(int mediaSource)
{
    emit currentSubtitleSettingChanged(mediaSource);
}

// { modified by kihyung 2013.07.20 for ITS 0179774
void AppMediaPlayer_Video_CModel_FS::setDeviceID(QString deviceID)
{
    m_deviceID = deviceID;
}

void AppMediaPlayer_Video_CModel_FS::setMountPath(QString mountPath)
{
    m_mountPath = mountPath;
}

void AppMediaPlayer_Video_CModel_FS::copyHistory(AppMediaPlayer_Video_CModel_FS *pOther)
{
    initModel();

    this->setFolderName(pOther->foldername());
    this->setFullFolderName(pOther->fullfoldername());
    this->setFilename(pOther->filename());
    this->setPosition(pOther->position());
    this->setPlaylistPosition(pOther->playlistPosition());
    this->setCaptionEnable(pOther->captionEnable());
    this->setCaptionSize(pOther->captionSize());
    this->setRandomMode(pOther->randomMode());
    this->setRepeatMode(pOther->repeatMode());    
    this->setDeviceID(pOther->deviceID());
    this->setMountPath(pOther->mountPath());
}

void AppMediaPlayer_Video_CModel_FS::copyHistoryFromExtended(AppMediaPlayer_Video_CModel_FS *pOther, QString mountPath)
{
    initModel();

    this->setFolderName(pOther->foldername()); // modified by Sergey 25.07.2013 ITS#0179774
    this->setFullFolderName(mountPath + pOther->fullfoldername());
    this->setFilename(pOther->filename());
    this->setPosition(pOther->position());
    this->setPlaylistPosition(pOther->playlistPosition());
    this->setCaptionEnable(pOther->captionEnable());
    this->setCaptionSize(pOther->captionSize());
    this->setRandomMode(pOther->randomMode());
    this->setRepeatMode(pOther->repeatMode()); 
    this->setDeviceID(pOther->deviceID());
    this->setMountPath(pOther->mountPath());
}

void AppMediaPlayer_Video_CModel_FS::copyHistoryToExtended(AppMediaPlayer_Video_CModel_FS *pOther)
{
    int nIndex = pOther->mountPath().size();

    if(nIndex <= 1) {
        MP_HIGH << nIndex << "return..." << LOG_ENDL;
        copyHistory(pOther);
        return;
    }

    initModel();

    this->setFolderName(pOther->foldername().mid(nIndex));
    this->setFullFolderName(pOther->fullfoldername().mid(nIndex));
    this->setFilename(pOther->filename());
    this->setPosition(pOther->position());
    this->setPlaylistPosition(pOther->playlistPosition());
    this->setCaptionEnable(pOther->captionEnable());
    this->setCaptionSize(pOther->captionSize());
    this->setRandomMode(pOther->randomMode());
    this->setRepeatMode(pOther->repeatMode()); 
    this->setDeviceID(pOther->deviceID());
    this->setMountPath(pOther->mountPath());    
}

void AppMediaPlayer_Video_CModel_FS::initModel()
{
    MP_LOG << LOG_ENDL;
    
    // { modified by kihyung 2013.09.11 for ITS 0189474
    m_filename              = QString("");
    m_playingFilename       = QString("");
    m_folderName            = QString("");
    m_subtitleStr           = QString("");
    m_fullFolderName        = QString("");
    m_restoreFile           = QString("");
    m_fileFromFM            = QString("");
    m_restorePosition       = -1;
    m_width                 = 0;
    m_height                = 0;
    m_duration              = 0;
    m_displayPlaylistLength = 0;
    m_displayPlaylistPos    = 0;
    m_nCaptionSize          = CAPTION_SIZE_NORMAL;
    m_bCaptionEnable        = false;
    m_bMultipleCaptions     = false;
    m_bSMICaptions          = true; // added by Sergey 09.10.2013 for ITS#187939
    m_copyPercentage        = 0;
    m_currentStreamId       = 0;
    langcodelist            = QStringList("");
    m_bVoiceLangEnabled     = false; // added by Sergey 09.10.2013 for ITS#192050
    m_drmUseCount           = 0;
    m_drmUseLimit           = 0;
    m_playingUrl            = QString("");
    m_lastPlayedUrl         = QString("");
    m_lastPlayedPos         = 0;
    m_readingFile           = false;
    m_unsupported           = false;
    m_playlistPosition      = -1;
    m_prevPlaylistLength    = 0;
    m_prevPlaylistPosition  = 0;
    m_mountPath             = QString("");
    m_deviceID              = QString("");
    m_bFolderIsRoot         = false; // added by Sergey 26.11.2013 for Root folder translation
    // } modified by kihyung 2013.09.11 for ITS 0189474
    m_captionCurId          = 0; // added by Sergey 08.01.2014 for ITS#211412
}
// } modified by kihyung 2013.07.20 for ITS 0179774
 
