#ifndef DHAVN_QCANCONTROLLERLISTENER_H
#define DHAVN_QCANCONTROLLERLISTENER_H

#include <QObject>
#include <DHAVN_QCANController_ApplnTxRx.h>
#include <DHAVN_QCANController_ApplnTxRxStruct.h>
#include <DHAVN_LogUtil.h>
#include "DHAVN_AppStandBy_AppEngine.h"
#include <DHAVN_AppStandBy_LoadSettings.h>
#include <DHAVN_AppSettings_Def.h>
#include <DHAVN_AppStandBy_CAN_Def.h>

#include <QFile>
#include <QTextStream>
#include <QString>

//class SettingsAppEngine;
class StandByAppEngine;
class QCANControllerListener : public QObject
{
    Q_OBJECT
public:
    QCANControllerListener(StandByAppEngine *m_uiAppEngine, QObject *parent = 0);
    ~QCANControllerListener();
    void sendCAN_HU_CLOCK_P_01(uint baseTime);

signals:
    void Can_filter_msg_id(const SQCanIdList &canIdList);
    void Can_data_send(const SQCanFrame &canframe);

public slots:
    void CQCANControllerTest_Filter_can_id();
    void updateTime();
    void onSettingsModechange(bool);
    void onAccStatus(int);

private:
    const CanMessageFieldType *getCANMessageField(ClockCANDef::S_CAN_ID_T CAN_id);
    void setData(SQCanFrame *result, uint startByte, uint startBit, uint bitSize, quint64 data);
    quint64 getData(const SQCanFrame *data, uint startByte, uint startBit, uint bitSize);
    uint DecimalToHexa(const uint value);
    void debugCANFrame(const SQCanFrame *data);
    uint getSecond( int hour, int min, int sec );
    uint getSecondFromSystemTime();

    StandByAppEngine* m_uiAppEngine;//m_pApplicationEngine;
    int m_nCnt;
    CQCANController_ApplnTxRx *mAppln;
//    QScopedPointer<LoadStandBySetting> m_pLoadStandBySetting;
//    AppClock_NaviUtils *m_pNaviUtils;
//    NaviAVSharedData *m_navidata;
    QList<uint> canIdList;
    ClockCANDef::S_CAN_HC_E_00_HU_TimeSetting_VAL_T m_eHUTimeSetting;
    bool m_bIsReceived_CH_E_01;
};

#endif // DHAVN_QCANCONTROLLERLISTENER_H
