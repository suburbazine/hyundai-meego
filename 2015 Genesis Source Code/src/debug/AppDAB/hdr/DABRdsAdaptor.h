#ifndef DABRDSADAPTOR_H
#define DABRDSADAPTOR_H

#include <QObject>
#include <QSharedMemory>
#ifndef __DAB_DEBUG_MSG_OUTPUT_FILE__
#include <DHAVN_LogUtil.h>
#endif
#include "DABApplication_Def.h"
#include "DABProtocol.h"
#include "DABLogger.h"

#define RDS_SHARED_MEM_KEY "DABRdsSharedData"

class DABRdsAdaptor : public QObject
{
#ifndef __DAB_DEBUG_MSG_OUTPUT_FILE__
    USE_LOG_UTIL
#endif
    Q_OBJECT
public:
    explicit DABRdsAdaptor(QObject *parent = 0);

    struct DABSharedMemory{
        int eTAStatus;
        bool bAnnouncePopup;
    };

    void init(void);
    bool readAnnouncePopupStatus(void);
    void writeAnnouncePopupStatus(bool onOff);

signals:
    void reqNewsDABtoRDS(bool onOff);
    void reqTrafficDABToRDS(bool onOff);
    void cmdTrafficRDStoDAB(bool onOff);
    void cmdReqSetAnnounement(bool onOff);
    void cmdReqDisplayStatusBar(quint8 type, bool onOff);

public slots:   
    void onTrafficRDStoDAB(bool onOff);
    void onReqTrafficDABtoRDS(bool onOff);
    void onCheckRDSLanuch(void);
    bool getAnnouncePopupStatus(void) { return m_bAnnouncePopupFail; }

private:
    QSharedMemory m_xDABRdsSharedMem;
    DABSharedMemory m_xAnnouncementStatus;
    bool m_bTraffic;
    bool m_bAnnouncePopupFail;
};

#endif // DABRDSADAPTOR_H
