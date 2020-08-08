
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_PopToast.yml
 * 
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include  <QtGui>

#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_PopToast.h"

AppMediaPlayer_Video_SModel_PopToast::AppMediaPlayer_Video_SModel_PopToast( QObject *parent )
    : QObject(parent)
{
    m_sText = QString("");

}



void AppMediaPlayer_Video_SModel_PopToast::setSText(QString value)
{
    m_sText = value;
    emit sTextChanged( m_sText );
}
