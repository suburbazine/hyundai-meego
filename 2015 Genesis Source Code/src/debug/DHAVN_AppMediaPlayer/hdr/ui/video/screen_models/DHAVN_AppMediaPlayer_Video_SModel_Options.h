#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_OPTIONS_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_OPTIONS_H

#include <QObject>

#include "DHAVN_OptionMenuModel.h"
#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_FS.h"
#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_Options.h"



class AppMediaPlayer_Video_CModel_FS;


class AppMediaPlayer_Video_SModel_Options : public QObject
{
    Q_OBJECT

    /** @property optionMenu
     * Model for options menu
     */
   Q_PROPERTY( OptionMenuModel* optionMenuModel
               READ getOptionMenuModel
               WRITE setOptionMenuModel
               NOTIFY optionMenuModelChanged );

Q_SIGNALS:
   void optionMenuModelChanged();

public:
    AppMediaPlayer_Video_SModel_Options( QObject *parent = NULL );
    ~AppMediaPlayer_Video_SModel_Options();

    OptionMenuModel* getOptionMenuModel() { return m_pOptionMenuModel; }

    void setOptionMenuModel( OptionMenuModel* optionMenuModel );

private:
    OptionMenuModel* m_pOptionMenuModel;
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_OPTIONS_H
