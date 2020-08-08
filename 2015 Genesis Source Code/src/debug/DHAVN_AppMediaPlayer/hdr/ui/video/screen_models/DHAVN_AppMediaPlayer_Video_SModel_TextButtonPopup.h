// { modified by Sergey for CR#15744
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_TextButtonPopup.yml
 * 
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_TEXTBUTTONPOPUP_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_TEXTBUTTONPOPUP_H

#include <QObject>


class AppMediaPlayer_Video_SModel_TextButtonPopup : public QObject
{
    Q_OBJECT

    /** @property sText
     * Text on PopUp
     */
    Q_PROPERTY( QString sText
                READ sText
                NOTIFY sTextChanged)

    /** @property isWarningIcon
     * Show warning icon or not
     */
    Q_PROPERTY( bool isWarningIcon
                READ isWarningIcon
                NOTIFY isWarningIconChanged)

public:
    AppMediaPlayer_Video_SModel_TextButtonPopup(QObject *parent = NULL);


    QString sText() { return m_sText; }
    bool isWarningIcon() { return m_isWarningIcon; }

public slots:
    void setSText(QString);
    void setIsWarningIcon(bool);

signals:
    void sTextChanged(QString);
    void isWarningIconChanged(bool);

private:
    QString m_sText;
    bool m_isWarningIcon;    
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_TEXTBUTTONPOPUP_H
// } modified by Sergey for CR#15744.
