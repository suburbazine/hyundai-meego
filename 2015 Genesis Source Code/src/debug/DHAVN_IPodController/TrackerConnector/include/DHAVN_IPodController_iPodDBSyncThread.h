#ifndef IPODDBSYNCTHREAD_H
#define IPODDBSYNCTHREAD_H


#include <QThread>
#include <DHAVN_IPodController_trackerConnector.h>
#include <DHAVN_IPodController_IAPPacketReader.h>
#include <DHAVN_IPodController_IAPPacketWriter.h>

class CiPodDBSyncThread : public QThread
{
    Q_OBJECT

public:
    CiPodDBSyncThread(QObject* controller, CTrackerConnector *trackerConnector);
    ~CiPodDBSyncThread();
    void run();
    void stopDBSync();
    void fetchArtwork(QStringList albumList, QStringList artistList, bool forceFlag);
public slots:
    void requestDBSync();    // added by sungha.choi 2013.04.16
signals:
    void startDBSync();
    void getArtWork(QStringList, QStringList, bool);
    void requestQuit(); // added by sungha.choi 2013.04.16

    void OnDBIndexingStatusChanged(bool pause);
    void OnChangeDBIndexingStatus(bool pause);

public:
    CIAPPacketWriter* m_iAPWriter;
    CIAPPacketReader* m_iAPReader;
private:
    CTrackerConnector *m_trackerConnector;
private:
    bool m_isDisconnected;  // added by sungha.choi 2013.04.15
    USE_LOG_UTIL
};
#endif // IPODDBSYNCTHREAD_H
