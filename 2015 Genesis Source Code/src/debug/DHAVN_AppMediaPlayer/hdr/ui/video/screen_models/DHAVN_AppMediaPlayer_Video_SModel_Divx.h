// { added by Sergey 30.07.2013
/* WARNING: This file was automatically generated from
 * AppMediaPlayer_Video_SModel_Divx.yml
 * 
 * All manual changes would be lost. To regenerate this file run tools/GenModels.py
 * from the root directory of the AppMediaPlayer sources.
 */

#ifndef DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_DIVX_H
#define DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_DIVX_H

#include <QObject>



class AppMediaPlayer_Video_SModel_Divx : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString regCode READ regCode NOTIFY regCodeChanged)
    Q_PROPERTY(QString deregCode READ deregCode NOTIFY deregCodeChanged)
    Q_PROPERTY(QString scrState READ scrState NOTIFY scrStateChanged)

public:
    AppMediaPlayer_Video_SModel_Divx( QObject *parent = NULL );

    QString regCode() { return m_regCode; }
    QString deregCode() { return m_deregCode; }
    QString scrState() { return m_scrState; }

public slots:
    void setRegCode(QString);
    void setDeregCode(QString);
    void setScrState(QString);

signals:
    void regCodeChanged(QString);
    void deregCodeChanged(QString);
    void scrStateChanged(QString);


private:
    QString m_regCode;
    QString m_deregCode;
    QString m_scrState;
};

#endif // DHAVN_APPMEDIAPLAYER_VIDEO_SMODEL_DIVX_H
// } added by Sergey 30.07.2013
