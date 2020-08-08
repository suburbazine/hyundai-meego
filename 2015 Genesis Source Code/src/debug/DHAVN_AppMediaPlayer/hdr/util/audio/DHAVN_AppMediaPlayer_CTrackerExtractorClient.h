#ifndef DHAVN_APPMEDIAPLAYER_CTRACKEREXTRACTORCLIENT_H
#define DHAVN_APPMEDIAPLAYER_CTRACKEREXTRACTORCLIENT_H

#include <QObject>
#include <QStringList>
#include <QVector>
#include <QTimer>
#include <QDebug>
#include <QtDBus/QtDBus>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusConnectionInterface>

class TrackerExtractorThread; // modified by ravikanth 16-07-13 for ISV 87299, 86965

class CTrackerExtractorClient : public QObject
{
    Q_OBJECT
public:
    explicit CTrackerExtractorClient(QObject *parent = 0);

    Q_PROPERTY ( bool isMinerReady READ isMinerReady );

    bool isMinerReady();
    void pauseMiner();
    void resumeMiner();
	bool getJukeboxStatus(); // added by eugene 2012.11.14 for jukebox miner status
    void DBusTestAck(); // added by honggi.shin 2014.02.13 for testing Dbus

signals:
    // { modified by  yongkyun.lee 2012.11.21 for  xSampa check.
    //void minerFinished(bool isReady);
    void minerFinished(bool isReady, QString usbMiningPath);
    // } modified by  yongkyun.lee 
    void minerInProgress(int percentage);
    void needToUseQDir(QString mountPath);
	
	// modified by ravikanth 16-07-13 for ISV 87299, 86965
    void pauseMinerAsync();
    void resumeMinerAsync();
    void dbusTestAckAsync(); // added by honggi.shin 2014.02.13 for testing Dbus

private slots:
    // { modified by eugene 2012.11.09 for emitting miner start signal with usb path
    //void onMinerStatus(QString minerType, QString minerStatus);
	void onMinerStatus(QString minerType, QString minerStatus, QString usbMiningPath = "");
    // } modified by eugene 2012.11.09 for emitting miner start signal with usb path
    void onMinerStatusInDetail(QString minerType, QString minerStatus, int percentage);
    void onDBusSendTest(); // added by honggi.shin 2014.02.13 for testing Dbus

private:
    bool m_bIsMinerReady;
    bool m_bIsMinerStarted;
    bool m_bIsJukeboxMiningFinished; // added by eugene 2012.11.14 for jukebox miner status
    QDBusConnection connection;
	// modified by ravikanth 16-07-13 for ISV 87299, 86965
    TrackerExtractorThread* m_ExtractorAsync;
    QThread m_ExtractorThread;
    bool m_isMinerStatePaused; // modified by ravikanth 20-08-13
};

// modified by ravikanth 16-07-13 for ISV 87299, 86965
class TrackerExtractorThread : public QObject
{
    Q_OBJECT
public:
    explicit TrackerExtractorThread(QObject* parent = 0);
    ~TrackerExtractorThread();

public slots:
    void pauseMinerAsync();
    void resumeMinerAsync();
    void dbusTestAckAsync(); // added by honggi.shin 2014.02.13 for testing Dbus

private:
    bool pauseMinerState;
};

#endif // DHAVN_APPMEDIAPLAYER_CTRACKEREXTRACTORCLIENT_H
