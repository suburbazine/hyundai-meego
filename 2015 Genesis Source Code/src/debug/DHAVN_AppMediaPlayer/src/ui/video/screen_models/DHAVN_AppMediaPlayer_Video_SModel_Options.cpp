
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_Options.yml
 *
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#include "util/DHAVN_AppMediaPlayer_LogHelper.h"
#include  <QtGui>

#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_Options.h"

AppMediaPlayer_Video_SModel_Options::AppMediaPlayer_Video_SModel_Options( QObject *parent )
    : QObject(parent),
      m_pOptionMenuModel(NULL)
{
}

void AppMediaPlayer_Video_SModel_Options::setOptionMenuModel( OptionMenuModel *optionMenuModel )
{
    if(m_pOptionMenuModel != optionMenuModel)
    {
        // Fix memory leak
        if(m_pOptionMenuModel != NULL)
        {
           delete m_pOptionMenuModel;
        }
        m_pOptionMenuModel = optionMenuModel;
        emit optionMenuModelChanged();
    }
}

AppMediaPlayer_Video_SModel_Options::~AppMediaPlayer_Video_SModel_Options()
{
    delete m_pOptionMenuModel;
    setOptionMenuModel(NULL);
}
