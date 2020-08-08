
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_DVDSearchCaption.yml
 *
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include  <QtGui>

#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_DVDSearchCaption.h"

AppMediaPlayer_Video_SModel_DVDSearchCaption::AppMediaPlayer_Video_SModel_DVDSearchCaption( QObject *parent )
    : QObject(parent)
{
    m_inputText = QString("");

}

void AppMediaPlayer_Video_SModel_DVDSearchCaption::setInputText(QString value)
{
    m_inputText = value;
    emit inputTextChanged( m_inputText );
}

void AppMediaPlayer_Video_SModel_DVDSearchCaption::setCaptionStringList(QStringList value)
{
    m_captionStringList = value;
    emit captionStringListChanged( m_captionStringList );
}

