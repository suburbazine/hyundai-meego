#ifndef DHAVN_QCANCONTROLLERLISTENER_H
#define DHAVN_QCANCONTROLLERLISTENER_H

#include <QObject>
#include <DHAVN_QCANController_ApplnTxRx.h>
#include <DHAVN_QCANController_ApplnTxRxStruct.h>
#include "DHAVN_AppSettings_AppEngine.h"
#include<DHAVN_AppSettings_SaveSettings.h>
#include <DHAVN_AppSettings_Def.h>
#include <DHAVN_AppSettings_CAN_Def.h>

#include <QFile>
#include <QTextStream>
#include <QString>
#include<QTimer>

class SettingsAppEngine;
class QCANControllerListener : public QObject
{
    Q_OBJECT
public:
    QCANControllerListener(SettingsAppEngine* uiAppEngine,SettingsStorage *, QObject *parent = 0);
    ~QCANControllerListener();

    void sendCAN_HU_CLOCK_P_01(uint baseTime);

signals:
    void Can_filter_msg_id(const SQCanIdList &canIdList);
    void Can_data_send(const SQCanFrame &canframe);


public slots:
    void CQCANControllerTest_Filter_can_id();
    void onCAN_data_received(const SQCanFrameList &receivedData);
    //void updateGpsTime();
    void onTimeFormateChange(int);
    void onSettingsModechange(bool);
    void onAccStatus(int);
    void onTimePickerPopupStatus(bool);

    //for Rear Screen Lock
    void sendReakScreenLockStatus(bool status);

    void sendCAN_HU_CLOCK_E_00();


private:
    const CanMessageFieldType *getCANMessageField(SettingsCANDef::S_CAN_ID_T CAN_id);
    void loadValueFromDB();
    void setData(SQCanFrame *result, uint startByte, uint startBit, uint bitSize, quint64 data);
    quint64 getData(const SQCanFrame *data, uint startByte, uint startBit, uint bitSize);
    uint DecimalToHexa(const uint value);
    void debugCANFrame(const SQCanFrame *data);
    uint getSecond( int hour, int min, int sec );
    uint getSecondFromSystemTime();

    QTimer *m_pSendTimer;

    SettingsAppEngine* m_uiAppEngine;
    CQCANController_ApplnTxRx *mAppln;
    SettingsStorage *m_pSettingsDB;
    QList<uint> canIdList;
    SettingsCANDef::S_CAN_HC_E_00_HU_TimeFormat_VAL_T m_eHUTimeFormat;
    SettingsCANDef::S_CAN_HC_E_00_HU_TimeSetting_VAL_T m_eHUTimeSetting;
    SettingsCANDef::S_CAN_HC_E_00_HU_Status_VAL_T m_eHUStatus;
    bool m_bIsReceived_CH_E_01;
};

#endif // DHAVN_QCANCONTROLLERLISTENER_H
