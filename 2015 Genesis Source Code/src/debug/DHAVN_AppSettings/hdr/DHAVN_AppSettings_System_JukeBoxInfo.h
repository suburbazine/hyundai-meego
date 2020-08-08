#ifndef DHAVN_APPSETTINGS_SYSTEM_JUKEBOXINFO_H
#define DHAVN_APPSETTINGS_SYSTEM_JUKEBOXINFO_H

#include <QObject>
#include <QTimer>
#include <QVariant>
#include "DHAVN_AppSettings_AppEngine.h"

extern bool JukeboxEnable;

class USBController;

class AppSettings_System_JukeBoxInfo : public QObject
{
    Q_OBJECT
public:
    explicit AppSettings_System_JukeBoxInfo(SettingsAppEngine* pEngine, QObject *parent = 0);

    Q_INVOKABLE int GetJukeBoxMaxScaleSize();
    Q_INVOKABLE int GetJukeBoxMaxMBSize();
    Q_INVOKABLE int GetUsedJukeBoxScaleSize();

    //Q_INVOKABLE QString GetStringUsedAndAllSize();
    Q_INVOKABLE void InitiateJukeBoxFormat(int screen);

Q_SIGNALS:
    void jukeBoxInfoUpdated(QVariant maxMb, QVariant usedMb, int usedScale);
    void jukeBoxInfoUpdatedString(QString currentSize, int usedScale);
    void formattingStarted(int screen);
    void formatComplete();

public slots:
    void FormattingComleteHandler(int status);
    void StartJukeBoxFormat();
    void UpdateInfo();
    Q_INVOKABLE void startUpdateInfo(bool useTimer);

private:
    void InfoRecalculation();

private:
    SettingsAppEngine* m_pEngine;
    double jukeBoxMaxMbSize;
    int jukeBoxMaxScaleSize;
    double usedJukeBoxMbSize;
    int usedJukeBoxScaleSize;

    USBController *fileInterface;

    QTimer *m_pFormatTimer;
};


#endif // DHAVN_APPSETTINGS_SYSTEM_JUKEBOXINFO_H


