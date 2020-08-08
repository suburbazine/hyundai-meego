
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_DVDSearchCaption.yml
 *
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_DVDSEARCHCAPTION_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_DVDSEARCHCAPTION_H

#include <QObject>
#include <QStringList>

// modified by eugene 2012.08.23 for static analysis defect #7472
//#include "ui/video/screen_models/DHAVN_AppMediaPlayer_Video_SModel_DVDSearchCaption.h" 

class AppMediaPlayer_Video_SModel_DVDSearchCaption : public QObject
{
    Q_OBJECT

    /** @property inputText
     * Text entered by user
     */
    Q_PROPERTY( QString inputText
                READ inputText
                WRITE setInputText // added by lssanh 2013.04.19 ITS158176
                NOTIFY inputTextChanged)
    /** @property captionStringList
     * List of found captions
     */
    Q_PROPERTY( QStringList captionStringList
                READ captionStringList
                NOTIFY captionStringListChanged)


public:
    AppMediaPlayer_Video_SModel_DVDSearchCaption( QObject *parent = NULL );


    QString inputText() { return m_inputText; }
    QStringList captionStringList() { return m_captionStringList; }


public slots:
    void setInputText(QString);
    void setCaptionStringList(QStringList);

signals:
    void inputTextChanged( QString );
    void captionStringListChanged( QStringList );


private slots:


private:
    QString m_inputText;
    QStringList m_captionStringList;
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_DVDSEARCHCAPTION_H

