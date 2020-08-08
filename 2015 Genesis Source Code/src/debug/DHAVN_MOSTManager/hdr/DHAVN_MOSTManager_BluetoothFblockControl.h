/*
 ******************************************************************
 * COPYRIGHT © <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER :  Venkatesh  Aeturi                                *
 * DATE       :  20 Aug 2011                                      *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 * MODULE SUMMARY : MOST Manager is a middle ware component which *
 *                  talks to the applications and controller over *
 *                  QtDbus.The data sent over QtDbus is further   *
 *                  communicated to other MOST devices through ACS*
 *                  daemon.This module is also responsible for    *
 *                  commands from ACS daemon.                     *
 *                  This file contains the class definition for   *
 *                  Bluetooth FBlock control, responsible to      *
 *                  create instance of and Shadow(0x01)           *
 *                  configure it and add to most stack.           *
 *                  Listeners and handlers will be part of this   *
 *                  class.                                        *
 *                  Communicates with ACS daemon                  *
 *                                                                *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT                  *
 ******************************************************************
 * 20 Aug 2011  Venkatesh Aeturi          Draft version           *
 * 15 Sep 2011  Amit Koparde              Added Log_Tag define    *
 * 27 Sep 2011  Amit Koparde              Changed the parameters for API CurrentSongInfo of CBluetoothFblockControl
 * 14 Oct 2011  Neelima Khedad            Added API parameters for CallSate and CallDuration
 * 26 Oct 2011  Amit Koparde              Added new API's ModemLevel, UserMode, CallHistoryCopyStatus and MissedCallStatus
 * 21 Jun 2012  Shiva Kumar              Modified for FCAT 1.17.1, FCAT 1.17.2 and FCAT 1.17.3
 * 19 Sep 2012  Shiva Kumar              Modified for FCAT 1.19.0, 1.20.1, 1.20.2
 ******************************************************************
 */

#ifndef DHAVN_MOSTMANAGER_BLUETOOTHFBLOCKCONTROL_H
#define DHAVN_MOSTMANAGER_BLUETOOTHFBLOCKCONTROL_H

#include <QtCore>
#include <QtDBus/QtDBus>

#include <DHAVN_LogUtil.h>

#include "DHAVN_MOSTManager_Config.h"
#include "DHAVN_MOSTManager_ACSApplication.h"
#include "DHAVN_MOSTManager_BluetoothFBlock.h"

#define LOG_TAG_BT_CNT "BluetoothControl"


using namespace acs::generated::fblock::Bluetooth;
using namespace k2l::acs::protocols::most;

class CBluetoothFblockControl;

/** Handler for Hangup call */
class CHangUpCallHandler : public IFunctionBlockMethodHandler
{
    CBluetoothFblockControl *m_BluetoothFblockControlContext;

public:
    CHangUpCallHandler(CBluetoothFblockControl *context);

    /// Callback for received GetInterface OpType
    virtual void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received Start OpType
    virtual void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received Abort OpType
    virtual void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received StartResult OpType
    virtual void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received StartResultAck OpType
    virtual void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received AbortAck OpType
    virtual void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received StartAck OpType
    virtual void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};

class CDialNumberHandlerBT : public IFunctionBlockMethodHandler
{
    CBluetoothFblockControl *m_BluetoothFblockControlContext;
public:
    CDialNumberHandlerBT(CBluetoothFblockControl *context);
    /// Callback for received GetInterface OpType
    virtual void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received Start OpType
    virtual void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received Abort OpType
    virtual void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received StartResult OpType
    virtual void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received StartResultAck OpType
    virtual void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received AbortAck OpType
    virtual void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received StartAck OpType
    virtual void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};
class CUserModeListenerBT : public IACSMOSTEventListener
{
    CBluetoothFblockControl *m_BluetoothFblockControlContext;
public:
    CUserModeListenerBT(CBluetoothFblockControl *context);
    virtual void OnChange();
};

class CBluetoothFblockControl : public QObject, public CControlThreadBase
{
    Q_OBJECT

private:
    //USE_LOG_UTIL

    /** Pointer Object of Tuner Fblock of generated code */
    CBluetooth *m_BluetoothFblock;

    /** Handlers for Properties of Bluetooth */
    CHangupCall *m_HangupCall;
    CHangUpCallHandler *m_HangupCallHandler;

    CCallState *m_CallState;

    CDialNumber *m_DialNumber;
    CDialNumberHandlerBT *m_DialNumberHandlerBT;

    CBTStatus *m_BTStatus;

    CUserMode *m_UserMode;
    CUserModeListenerBT *m_UserModeListenerBT;

public:
    CBluetoothFblockControl(CMarshaledACSApplication *context);
    ~CBluetoothFblockControl();

    virtual bool Initialize();
    bool FBlockVisible();

    /** Below are the methods invoked by Handlers for
     * the properties Bluetooth
     */
    void StartHangupCall(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload);
    void DialNumberRequestIBOXSlot(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state);
    void UserModeSlot();

    /** Below are the Methods invoked by the Bluetooth Adaptor */
    bool CallState(const QDBusVariant &callStatedata);
    bool HangupCallResult(const uint senderHandle, const uint callId,const uint deviceID);
    void HangUpCallHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state);
    bool DialNumberResultIBOX(const uint state, const uint senderHandle, const uint callID, const uint deviceID);
    bool SetBTStatus(const uint btMode);
    bool SetUserMode(const uint usermode,const bool micOff);
signals:
    /** Below are the signals sent to Bluetooth Adaptor */
    void StartHangupCall(const uint senderHandle, const uint callId,const uint deviceID);
    void DialNumberRequestIBOX(const uint senderHandle, const QString telNumber, const uint deviceID);
    void UserMode(const uint usermode,const bool micOff);

};

#endif // DHAVN_MOSTMANAGER_BLUETOOTHFBLOCKCONTROL_H
