#ifndef DHAVN_MOSTMANAGER_DMBTUNERFBLOCKCONTROL_H
#define DHAVN_MOSTMANAGER_DMBTUNERFBLOCKCONTROL_H

#include <QtCore>
#include <QtDBus/QtDBus>

#include <DHAVN_LogUtil.h>

#include "DHAVN_MOSTManager_Config.h"
#include "DHAVN_MOSTManager_ACSApplication.h"
#include "DHAVN_MOSTManager_DMBTunerFBlock.h"
#include "DHAVN_MOSTManager_MOSTStructures.h"

#define LOG_TAG_DMB_CNT "DMBTunerControl"

#define DMB_SERVICE         ""
#define DMB_OBJECT          "/DmbTpegService"
#define DMB_INTERFACE       "com.lge.car.dhavn.Dmb"

#define TPEG_SHARED_MEM_KEY     "DMBTPEGSharedData"
#define TPEG_DATA_SIZE          2048*10

struct TPEGData_T
{
    bool bWrite;            // True when DMB App write TPEG data
    qint32 nDataLength;     // Data length of TPEG data
    qint8 szTPEGData[TPEG_DATA_SIZE];  // TPEG Data
};

using namespace acs::generated::fblock::DMBTuner;
using namespace k2l::acs::protocols::most;
using namespace k2l::acs::protocols::events;

class CDMBTunerFblockControl;

class CTPEGDataRequestHandler : public IFunctionBlockMethodHandler//, public IACSEventListener
{
    USE_LOG_UTIL

    CDMBTunerFblockControl *m_DMBTunerFblockControlContext;    

public:
    CTPEGDataRequestHandler(CDMBTunerFblockControl *context);

    /// Callback for received GetInterface OpType
    virtual void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received Start OpType
    virtual void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received Abort OpType
    virtual void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartResult OpType
    virtual void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartResultAck OpType
    virtual void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received AbortAck OpType
    virtual void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartAck OpType
    virtual void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received ACS Event
    //virtual void OnEvent( uint8_t channel, uint8_t eventID, uint32_t payloadLength, const uint8_t *pPayload );
};

class CDMBTunerFblockControl : public QObject, public CControlThreadBase
{
    Q_OBJECT

private:

    CDMBTuner *m_DMBTunerFblock;

    CSignalQuality *m_SignalQuality;
    CTPEGStatus *m_TPEGStatus;
    CTPEGInfo *m_TPEGInfo;
    CDMBStatus *m_DMBStatus;
    CTPEGDataRequest *m_TPEGDataRequest;

    /** Handlers for methods of DMBTuner */
    CTPEGDataRequestHandler *m_TPEGDataRequestHandler;

    QSharedMemory m_xTPEGDataSharedMem;
    struct TPEGData_T m_xTPEGData;

    bool m_IsTPEGDataAvailable;

public:
    CDMBTunerFblockControl(CMarshaledACSApplication *context);
    ~CDMBTunerFblockControl();

    virtual bool Initialize();

    void SetHandler();
    //void SetACSEventListener(void);

    bool FBlockVisible();

    void SetTPEGDataState(bool state) { m_IsTPEGDataAvailable = state; }
    bool GetTPEGDataState(void) { return m_IsTPEGDataAvailable; }

    /** Below are the methods invoked by Handlers */
    void TPEGDataRequestHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t * payload, const uchar optype);    

    /** Below are the Methods invoked by the DMBTuner Adaptor */
    bool SendSignalQuality(const uchar optype, const uchar dmbSignalQuality);
    bool SendTPEGStatus(const uchar optype, const uchar tpegStatus);
    bool SendTPEGInfo(const uchar optye, const STPEGInfo &tpegInfo);
    bool SendDMBStauts(const uchar optype, const uchar dmbStauts, const uchar currentBrowserListLength);    

signals:
    /** Below are the signals sent to DMBTuner Adaptor */    
};

#endif // DHAVN_MOSTMANAGER_DMBTUNERFBLOCKCONTROL_H
