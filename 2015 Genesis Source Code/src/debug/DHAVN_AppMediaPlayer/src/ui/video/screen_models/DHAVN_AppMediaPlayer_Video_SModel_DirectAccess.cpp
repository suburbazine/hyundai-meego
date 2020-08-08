
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_DirectAccess.yml
 * 
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include  <QtGui>

#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_DirectAccess.h"

AppMediaPlayer_Video_SModel_DirectAccess::AppMediaPlayer_Video_SModel_DirectAccess( QObject *parent )
    : QObject(parent)
{
    m_index1 = QString("");
    m_index2 = QString("");

}



void AppMediaPlayer_Video_SModel_DirectAccess::setIndex1(QString value)
{
    m_index1 = value;
    emit index1Changed( m_index1 );
}
void AppMediaPlayer_Video_SModel_DirectAccess::setIndex2(QString value)
{
    m_index2 = value;
    emit index2Changed( m_index2 );
}
