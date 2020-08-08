// { modified by Sergey for DRM 30.07.2013
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_Divx.yml
 * 
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_Divx.h"

AppMediaPlayer_Video_SModel_Divx::AppMediaPlayer_Video_SModel_Divx( QObject *parent )
    : QObject(parent)
{
    m_regCode = "";
    m_deregCode = "";

}


void AppMediaPlayer_Video_SModel_Divx::setRegCode(QString value)
{
    // removed by Sergey 02.08.2103 for ITS#181512
    m_regCode = value;
    emit regCodeChanged(m_regCode);
}

void AppMediaPlayer_Video_SModel_Divx::setDeregCode(QString value)
{
    m_deregCode = value;
    emit deregCodeChanged(m_deregCode);
}

void AppMediaPlayer_Video_SModel_Divx::setScrState(QString value)
{
    m_scrState = value;
    emit scrStateChanged(m_scrState);
}
// } modified by Sergey for DRM 30.07.2013
