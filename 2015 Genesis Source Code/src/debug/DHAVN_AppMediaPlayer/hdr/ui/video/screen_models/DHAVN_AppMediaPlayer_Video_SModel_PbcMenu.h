
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_PbcMenu.yml
 * 
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_PBCMENU_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_PBCMENU_H

#include <QObject>



class AppMediaPlayer_Video_SModel_PbcMenu : public QObject
{
    Q_OBJECT

    /** @property inputtext
     * Entered value
     */
    Q_PROPERTY( QString inputtext
                READ inputtext
                NOTIFY inputtextChanged)


public:
    AppMediaPlayer_Video_SModel_PbcMenu( QObject *parent = NULL );


    QString inputtext() { return m_inputtext; }


public slots:
    void setInputtext(QString);

signals:
    void inputtextChanged( QString );


private slots:


private:
    QString m_inputtext;    
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_PBCMENU_H

