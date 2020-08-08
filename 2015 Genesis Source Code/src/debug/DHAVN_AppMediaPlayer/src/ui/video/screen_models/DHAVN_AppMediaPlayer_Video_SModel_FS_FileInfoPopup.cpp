
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_FS_FileInfoPopup.yml
 * 
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include  <QtGui>

#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_FS_FileInfoPopup.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_FS.h"

AppMediaPlayer_Video_SModel_FS_FileInfoPopup::AppMediaPlayer_Video_SModel_FS_FileInfoPopup( QObject *parent )
    : QObject(parent)
{
    m_filename = QString("");
    // { modified by Sergey for CR#15303
    m_duration = QString("");
    // } modified by Sergey for CR#15303.
    m_resolution = QString("");
	m_bindedAppMediaPlayer_Video_CModel_FS = NULL; // modified by eugene 2012.08.22 for static analysis defect #6951
}

void AppMediaPlayer_Video_SModel_FS_FileInfoPopup::bindWith( AppMediaPlayer_Video_CModel_FS *target )
{
    m_bindedAppMediaPlayer_Video_CModel_FS = target;
    connect( target, SIGNAL(playingFilenameChanged(QString)),
             this, SLOT(setFilename(QString)));
    setFilename(target->playingFilename());
    connect( target, SIGNAL(widthChanged(int)),
             this, SLOT(onAppMediaPlayer_Video_CModel_FSwidthChanged(int)));
    connect( target, SIGNAL(heightChanged(int)),
             this, SLOT(onAppMediaPlayer_Video_CModel_FSheightChanged(int)));
    setResolution( QString("%1x%2").arg(m_bindedAppMediaPlayer_Video_CModel_FS->width()).arg(m_bindedAppMediaPlayer_Video_CModel_FS->height()) );
}

void AppMediaPlayer_Video_SModel_FS_FileInfoPopup::onAppMediaPlayer_Video_CModel_FSwidthChanged(int)
{
    setResolution( QString("%1x%2").arg(m_bindedAppMediaPlayer_Video_CModel_FS->width()).arg(m_bindedAppMediaPlayer_Video_CModel_FS->height()) );
}
void AppMediaPlayer_Video_SModel_FS_FileInfoPopup::onAppMediaPlayer_Video_CModel_FSheightChanged(int)
{
    setResolution( QString("%1x%2").arg(m_bindedAppMediaPlayer_Video_CModel_FS->width()).arg(m_bindedAppMediaPlayer_Video_CModel_FS->height()) );
}

void AppMediaPlayer_Video_SModel_FS_FileInfoPopup::setFilename(QString value)
{
    m_filename = value;
    emit filenameChanged( m_filename );
}

// { modified by Sergey for CR#15303
void AppMediaPlayer_Video_SModel_FS_FileInfoPopup::setDuration(QString value)
{
    m_duration = value;
    emit durationChanged( m_duration );
}
// } modified by Sergey for CR#15303.

void AppMediaPlayer_Video_SModel_FS_FileInfoPopup::setResolution(QString value)
{
    m_resolution = value;
    emit resolutionChanged( m_resolution );
}
