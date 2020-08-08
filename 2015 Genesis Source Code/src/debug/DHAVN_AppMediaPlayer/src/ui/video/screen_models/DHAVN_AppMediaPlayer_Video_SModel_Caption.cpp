
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_Caption.yml
 * 
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include  <QtGui>

#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_Caption.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_FS.h"

AppMediaPlayer_Video_SModel_Caption::AppMediaPlayer_Video_SModel_Caption( QObject *parent )
    : QObject(parent)
{
    m_captionEnable = false;
    m_captionSize = 0;
    m_bMultipleCaptions = false;
    m_bSMICaptions = true; // added by Sergey 09.10.2013 for ITS#187939
    m_uiState = QString("CAPTION_VIEW"); // modified by Sergey 20.08.2013 for ITS#184640 
	m_bindedAppMediaPlayer_Video_CModel_FS = NULL; // modified by eugene 2012.08.22 for static analysis defect #6948
}

void AppMediaPlayer_Video_SModel_Caption::bindWith( AppMediaPlayer_Video_CModel_FS *target )
{
    m_bindedAppMediaPlayer_Video_CModel_FS = target;
    connect( target, SIGNAL(captionEnableChanged(bool)),
             this, SLOT(setCaptionEnable(bool)));
    setCaptionEnable(target->captionEnable());

    connect( target, SIGNAL(captionSizeChanged(int)),
             this, SLOT(setCaptionSize(int)));
    setCaptionSize(target->captionSize());

    connect( target, SIGNAL(multipleCaptionsChanged(bool)),
             this, SLOT(setMultipleCaptions(bool)));
    setMultipleCaptions(target->multipleCaptions()); // added by Sergey 09.10.2013 for ITS#187939

    connect( target, SIGNAL(smiCaptionsChanged(bool)),
             this, SLOT(setSmiCaptions(bool)));
    setSmiCaptions(target->smiCaptions()); // added by Sergey 09.10.2013 for ITS#187939
}


void AppMediaPlayer_Video_SModel_Caption::setCaptionEnable(bool value)
{
    m_captionEnable = value;
    emit captionEnableChanged( m_captionEnable );
}
void AppMediaPlayer_Video_SModel_Caption::setCaptionSize(int value)
{
    m_captionSize = value;
    emit captionSizeChanged( m_captionSize );
}
// { modified by Sergey 23.10.2013 for ITS#196877
void AppMediaPlayer_Video_SModel_Caption::setUiState(QString value)
{
    if(value != m_uiState)
    {
        m_uiState = value;
        emit uiStateChanged( m_uiState );
    }
}
// } modified by Sergey 23.10.2013 for ITS#196877

void AppMediaPlayer_Video_SModel_Caption::setMultipleCaptions( bool value )
{
   m_bMultipleCaptions = value;
   emit multipleCaptionsChanged( value );
} // added by Sergey 09.10.2013 for ITS#187939


void AppMediaPlayer_Video_SModel_Caption::setSmiCaptions(bool value)
{
   m_bSMICaptions = value;
   emit smiCaptionsChanged(value);
} // added by Sergey 09.10.2013 for ITS#187939
