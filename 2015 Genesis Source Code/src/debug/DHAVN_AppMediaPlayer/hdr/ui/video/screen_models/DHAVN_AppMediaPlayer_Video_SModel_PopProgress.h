
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_PopProgress.yml
 * 
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_POPPROGRESS_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_POPPROGRESS_H

#include <QObject>


#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_FS.h"

class AppMediaPlayer_Video_CModel_FS;
class AppMediaPlayer_Video_SModel_PopProgress : public QObject
{
    Q_OBJECT

    /** @property copyPercentage
     * The current percentage of copying a file.
     */
    Q_PROPERTY( int copyPercentage
                READ copyPercentage
                NOTIFY copyPercentageChanged)


public:
    AppMediaPlayer_Video_SModel_PopProgress( QObject *parent = NULL );

    void bindWith( AppMediaPlayer_Video_CModel_FS * );

    int copyPercentage() { return m_copyPercentage; }


public slots:
    void setCopyPercentage(int);

signals:
    void copyPercentageChanged( int );


private slots:


private:
    int m_copyPercentage;    
    AppMediaPlayer_Video_CModel_FS *m_bindedAppMediaPlayer_Video_CModel_FS;
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_POPPROGRESS_H

