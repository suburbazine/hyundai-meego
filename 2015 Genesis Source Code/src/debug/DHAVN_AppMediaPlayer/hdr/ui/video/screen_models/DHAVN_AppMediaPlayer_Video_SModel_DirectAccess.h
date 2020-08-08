
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_DirectAccess.yml
 * 
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_DIRECTACCESS_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_DIRECTACCESS_H

#include <QObject>



class AppMediaPlayer_Video_SModel_DirectAccess : public QObject
{
    Q_OBJECT

    /** @property index1
     * Entered value
     */
    Q_PROPERTY( QString index1
                READ index1
                NOTIFY index1Changed)
    /** @property index2
     * Entered value
     */
    Q_PROPERTY( QString index2
                READ index2
                NOTIFY index2Changed)


public:
    AppMediaPlayer_Video_SModel_DirectAccess( QObject *parent = NULL );


    QString index1() { return m_index1; }
    QString index2() { return m_index2; }


public slots:
    void setIndex1(QString);
    void setIndex2(QString);

signals:
    void index1Changed( QString );
    void index2Changed( QString );


private slots:


private:
    QString m_index1;    
    QString m_index2;    
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_DIRECTACCESS_H

