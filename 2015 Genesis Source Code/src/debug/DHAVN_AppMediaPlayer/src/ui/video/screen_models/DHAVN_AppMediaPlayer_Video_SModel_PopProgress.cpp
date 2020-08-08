
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_PopProgress.yml
 * 
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include  <QtGui>

#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_PopProgress.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_FS.h"

AppMediaPlayer_Video_SModel_PopProgress::AppMediaPlayer_Video_SModel_PopProgress( QObject *parent )
    : QObject(parent),
    m_bindedAppMediaPlayer_Video_CModel_FS(NULL) // modified by eugene 2012.08.22 for static analysis defect #6954
{
    m_copyPercentage = 0;

}

void AppMediaPlayer_Video_SModel_PopProgress::bindWith( AppMediaPlayer_Video_CModel_FS *target )
{
    m_bindedAppMediaPlayer_Video_CModel_FS = target;
    connect( target, SIGNAL(copyPercentageChanged(int)),
             this, SLOT(setCopyPercentage(int)));
    setCopyPercentage(target->copyPercentage());
}


void AppMediaPlayer_Video_SModel_PopProgress::setCopyPercentage(int value)
{
    m_copyPercentage = value;
    emit copyPercentageChanged( m_copyPercentage );
}
