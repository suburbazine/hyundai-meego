// { modified by Sergey for CR#15744
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_TextButtonPopup.yml
 * 
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_TextButtonPopup.h"

AppMediaPlayer_Video_SModel_TextButtonPopup::AppMediaPlayer_Video_SModel_TextButtonPopup( QObject *parent )
    : QObject(parent)
{
    m_sText = QString("");
    m_isWarningIcon = false;
}

void AppMediaPlayer_Video_SModel_TextButtonPopup::setSText(QString value)
{
    m_sText = value;
    emit sTextChanged( m_sText );
}

void AppMediaPlayer_Video_SModel_TextButtonPopup::setIsWarningIcon(bool value)
{
    m_isWarningIcon = value;
    emit isWarningIconChanged( m_isWarningIcon );
}
// } modified by Sergey for CR#15744.
