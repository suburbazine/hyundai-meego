#ifndef DABVRADAPTOR_H
#define DABVRADAPTOR_H

#include <QObject>
#include <QSharedMemory>

#define VR_SHARED_MEM_KEY "DABVRSharedData"
#define MAX_PRESETLIST_COUNT 12 // PlanA:24, PlanB:12
#define MAX_STATION_NAME_BUFFER_SIZE    32

class ListModel;

class DABVRAdaptor : public QObject
{
    Q_OBJECT
public:
    explicit DABVRAdaptor(QObject *parent = 0);

    struct DABSharedMemory{
        int nMaxDABPreset;
        qint8 nDABPresetChannel[MAX_PRESETLIST_COUNT][MAX_STATION_NAME_BUFFER_SIZE];    //channel name
    };

    void init(ListModel *presetList1);

signals:

public slots:
    void writePresetList();

private:
    QSharedMemory m_xDABVRSharedMem;
    DABSharedMemory m_xDABChannelStatus;

    ListModel *m_presetListModel1;
    ListModel *m_presetListModel2;
};

#endif // DABVRADAPTOR_H
