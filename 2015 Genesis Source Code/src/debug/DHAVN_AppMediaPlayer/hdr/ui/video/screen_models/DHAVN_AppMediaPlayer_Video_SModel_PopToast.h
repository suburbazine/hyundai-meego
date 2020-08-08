
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_PopToast.yml
 * 
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_POPTOAST_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_POPTOAST_H

#include <QObject>



class AppMediaPlayer_Video_SModel_PopToast : public QObject
{
    Q_OBJECT

    /** @property sText
     * Text on PopUp
     */
    Q_PROPERTY( QString sText
                READ sText
                NOTIFY sTextChanged)


public:
    AppMediaPlayer_Video_SModel_PopToast( QObject *parent = NULL );


    QString sText() { return m_sText; }


public slots:
    void setSText(QString);

signals:
    void sTextChanged( QString );


private slots:


private:
    QString m_sText;    
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_POPTOAST_H

