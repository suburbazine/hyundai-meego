
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_PbcMenu.yml
 * 
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include  <QtGui>

#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_PbcMenu.h"

AppMediaPlayer_Video_SModel_PbcMenu::AppMediaPlayer_Video_SModel_PbcMenu( QObject *parent )
    : QObject(parent)
{
    m_inputtext = QString("");

}



void AppMediaPlayer_Video_SModel_PbcMenu::setInputtext(QString value)
{
    m_inputtext = value;
    emit inputtextChanged( m_inputtext );
}
