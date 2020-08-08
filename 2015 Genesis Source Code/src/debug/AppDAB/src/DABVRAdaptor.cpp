
#include "DABVRAdaptor.h"
#include "ListModel.h"
#include "DABChannelItem.h"

#include <QDebug>

DABVRAdaptor::DABVRAdaptor(QObject *parent) :
    QObject(parent)
{
    m_presetListModel1 = NULL;
    m_presetListModel2 = NULL;
    memset(&m_xDABChannelStatus,0,sizeof(m_xDABChannelStatus));
}

void DABVRAdaptor::init(ListModel *presetList1)
{
    qDebug() << Q_FUNC_INFO;

    m_presetListModel1 = presetList1;

    m_xDABVRSharedMem.setKey(VR_SHARED_MEM_KEY);
    m_xDABVRSharedMem.attach();
    m_xDABVRSharedMem.create(sizeof(m_xDABChannelStatus));
}

void DABVRAdaptor::writePresetList()
{
    qDebug() << Q_FUNC_INFO;

    int iPresetListCount1 = m_presetListModel1->rowCount();
    DABChannelItem* pItem = NULL;

    memset(&m_xDABChannelStatus,0,sizeof(m_xDABChannelStatus));
    m_xDABChannelStatus.nMaxDABPreset = MAX_PRESETLIST_COUNT;

    for(int i = 0 ; i < iPresetListCount1 ; i++)
    {
        pItem = (DABChannelItem*) m_presetListModel1->dataItem(i);
#if 1
        if(pItem == NULL) break;
#else
        if(pItem == NULL) continue;
#endif
        QByteArray dataChName = pItem->label().toLocal8Bit();
        memcpy(m_xDABChannelStatus.nDABPresetChannel[pItem->presetIndex()], dataChName.data(), MAX_STATION_NAME_BUFFER_SIZE);
    }

    m_xDABVRSharedMem.lock();
    char *to = (char*)m_xDABVRSharedMem.data();
    memcpy(to, &m_xDABChannelStatus, sizeof(m_xDABChannelStatus));
    m_xDABVRSharedMem.unlock();
}
