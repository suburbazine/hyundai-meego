#ifndef DHAVN_MOSTMANAGER_SATRADIOFBLOCKCONTROL_H
#define DHAVN_MOSTMANAGER_SATRADIOFBLOCKCONTROL_H

#include <QtCore>
#include <QtDBus/QtDBus>

#include <DHAVN_LogUtil.h>

#include "DHAVN_MOSTManager_Config.h"
#include "DHAVN_MOSTManager_ACSApplication.h"
#include "DHAVN_MOSTManager_SatRadioFBlock.h"
#include "DHAVN_MOSTManager_MOSTStructures.h"

#define LOG_TAG_RADIO_CNT "SatRadioControl"

using namespace acs::generated::fblock::SatRadio;
using namespace k2l::acs::protocols::most;
using namespace k2l::acs::protocols::events;

class CSatRadioFblockControl;

class CStationSelectListener : public IACSMOSTEventListener
{
    CSatRadioFblockControl *m_SatRadioFblockControlContext;

public:
    CStationSelectListener(CSatRadioFblockControl *context);
    virtual void OnChange();
};

class CServiceProviderListener : public IACSMOSTEventListener
{
    CSatRadioFblockControl *m_SatRadioFblockControlContext;

public:
    CServiceProviderListener(CSatRadioFblockControl *context);
    virtual void OnChange();
};

class CTMCDataRequestHandler : public IFunctionBlockMethodHandler//, public IACSEventListener
{
    USE_LOG_UTIL

    CSatRadioFblockControl *m_SatRadioFblockControlContext;    

public:
    CTMCDataRequestHandler(CSatRadioFblockControl *context);

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
    //virtual void OnEvent (uint8_t channel, uint8_t eventID, uint32_t payloadLength, const uint8_t *pPayload);
};

class CSatRadioFblockControl : public QObject, public CControlThreadBase
{
    Q_OBJECT

private:

    /** FBlock class declaration */
    CSatRadio *m_SatRadioFblock;

    /** Function class declaration */
    CStationSelect *m_StationSelect;
    CServiceProvider *m_ServiceProvider;
    CServiceStatus *m_ServiceStatus;
    CTMCStatus *m_TMCStatus;
    CTMCDataRequest *m_TMCDataRequest;

    /** Listeners for properties of SatRadio */
    CStationSelectListener *m_StationSelectListener;
    CServiceProviderListener *m_ServiceProviderListener;

    /** Handlers for methods of SatRadio */
    CTMCDataRequestHandler *m_TMCDataRequestHandler;

public:
    CSatRadioFblockControl(CMarshaledACSApplication *context);
    ~CSatRadioFblockControl();

    bool Initialize();
    bool FBlockVisible();

    void SetHandler();
    void SetListener();
    bool RemoveListener();

    //void SetACSEventListener(void);

    /** Below are the methods invoked by Handlers and Listeners */
    void TMCDataRequestHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t * payload, const uchar optype);
    void StationSelectListener();
    void ServiceProviderListener();

    /** Below are the Methods invoked by the DMBTuner Adaptor */
    bool SendStationSelect(const uchar optype, const uchar station, const uchar presetNo);
    bool SendServiceProvider(const uchar optype, const uchar serviceProvider);
    bool SendServiceStatus(const uchar optype, const bool audio, const bool stationList, const uchar subscription);
    bool SendTMCStatus(const uchar optype, const uchar tmcStaus);

signals:
    /** Below are the signals sent to DMBTuner Adaptor */
    void StationSelect(const uchar type, const uchar station);
    void ServiceProvider(const uchar serviceProvider);
};

#endif // DHAVN_MOSTMANAGER_SATRADIOFBLOCKCONTROL_H
