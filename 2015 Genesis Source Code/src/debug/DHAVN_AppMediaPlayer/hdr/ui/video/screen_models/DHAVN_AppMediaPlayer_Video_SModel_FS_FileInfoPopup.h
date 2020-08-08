 // { modified by Sergey for CR#15303
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_FS_FileInfoPopup.yml
 * 
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_FS_FILEINFOPOPUP_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_FS_FILEINFOPOPUP_H

#include <QObject>


#include "controller/video/model/DHAVN_AppMediaPlayer_Video_CModel_FS.h"

class AppMediaPlayer_Video_CModel_FS;
class AppMediaPlayer_Video_SModel_FS_FileInfoPopup : public QObject
{
    Q_OBJECT

    /** @property filename
     * Reflects the playing file's name
     */
    Q_PROPERTY( QString filename
                READ filename
                NOTIFY filenameChanged)

    /** @property duration
     * Reflects the duration of the currently played media in HH:MM:SS.
     */
    Q_PROPERTY( QString duration
                READ duration
                NOTIFY durationChanged)

    /** @property resolution
     * format played media width + "x" + played media height
     */
    Q_PROPERTY( QString resolution
                READ resolution
                NOTIFY resolutionChanged)


public:
    AppMediaPlayer_Video_SModel_FS_FileInfoPopup( QObject *parent = NULL );

    void bindWith( AppMediaPlayer_Video_CModel_FS * );

    QString filename() { return m_filename; }
    QString duration() { return m_duration; }
    QString resolution() { return m_resolution; }


public slots:
    void setFilename(QString);
    void setDuration(QString);
    void setResolution(QString);

signals:
    void filenameChanged( QString );
    void durationChanged( QString );
    void resolutionChanged( QString );


private slots:
    void onAppMediaPlayer_Video_CModel_FSwidthChanged(int);
    void onAppMediaPlayer_Video_CModel_FSheightChanged(int);


private:
    QString m_filename;
    QString m_duration;
    QString m_resolution;    
    AppMediaPlayer_Video_CModel_FS *m_bindedAppMediaPlayer_Video_CModel_FS;
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_FS_FILEINFOPOPUP_H
 // } modified by Sergey for CR#15303.
