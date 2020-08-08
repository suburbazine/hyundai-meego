/*
 ******************************************************************
 * COPYRIGHT © <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER :  Neelima Khedad                                   *
 * DATE       :  06 Sep 2011                                      *
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
 *                  Telephone Shadow control, responsible to      *
 *                  create instance of and Shadow configure it    *
 *                  and add to most stack.                        *
 *                  If any listeners will be part of this class.  *
 *                  Communicates with ACS daemon                  *
 *                                                                *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT                  *
 ******************************************************************
 * 06 Sep 2011  Neelima Khedad           Draft version            *
 * 15 Sep 2011  Amit Koparde             Added Log_Tag define     *
 * 27 Oct 2011  Neelima Khedad           Removed Signal NewSMS and added MissedCallStatus for Telephone
 * 10 Feb 2011  Neelima Khedad           Added new API and SIgnal for property AcceptCall and HangUpCall
 * 25 Jan 2013  Hanhong Keum             Deleted SetPBookStatus according to Fcat v1.22.6
 ******************************************************************
 */


#ifndef DHAVN_MOSTMANAGER_TELEPHONESHADOWCONTROL_H
#define DHAVN_MOSTMANAGER_TELEPHONESHADOWCONTROL_H

#include <QtCore>
#include <QtDBus>

#include <DHAVN_LogUtil.h>

#include "DHAVN_MOSTManager_Config.h"
#include "DHAVN_MOSTManager_ACSApplication.h"
#include "DHAVN_MOSTManager_TelephoneShadow.h"
#include "DHAVN_MOSTManager_MOSTStructures.h"


#define LOG_TAG_TELE_CNT "TelephoneControl"

using namespace acs::generated::shadow::Telephone;
using namespace k2l::acs::protocols::most;


class CTelephoneShadowControl;

class CCollectPBookHandler : public IShadowMethodHandler
{
    CTelephoneShadowControl *m_TelephoneShadowControlContext;

public:
    CCollectPBookHandler(CTelephoneShadowControl *context);

    /// Callback for received ErrorAck OpType
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received ProcessingAck OpType
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received Processing OpType
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received Result OpType
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received ResultAck OpType
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received Interface OpType
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received Error OpType
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};

/** Handler for Dial Number */
class CDialNumberHandler : public IShadowMethodHandler
{
    CTelephoneShadowControl *m_TelephoneShadowControlContext;

public:
    CDialNumberHandler(CTelephoneShadowControl *context);

    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );    
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );    
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );    
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );    
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );    
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );    
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};

/** Handler for Accept call */
class CAcceptCallHandler : public IShadowMethodHandler
{
    CTelephoneShadowControl *m_TelephoneShadowControlContext;

public:
    CAcceptCallHandler(CTelephoneShadowControl *context);

    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );

    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};

/** Handler for Hangup Call */
class CHangupCallHandler : public IShadowMethodHandler
{
    CTelephoneShadowControl *m_TelephoneShadowControlContext;

public:
    CHangupCallHandler(CTelephoneShadowControl *context);

    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};

/** Listener for property PBook Status */
class CPBookStatusListener : public IACSMOSTEventListener
{
    CTelephoneShadowControl *m_TelephoneFblock;

public:
    CPBookStatusListener(CTelephoneShadowControl *context);
    virtual void OnChange();
};

/** Listener for property Call History Status */
class CCallHistoryInfoListener : public IACSMOSTEventListener
{
    CTelephoneShadowControl *m_TelephoneFblock;

public:
    CCallHistoryInfoListener(CTelephoneShadowControl *context);
    virtual void OnChange();
};

/** Listener for property Current Number Status */
class CCurrentNumberListener : public IACSMOSTEventListener
{
    CTelephoneShadowControl *m_TelephoneFblock;

public:
    CCurrentNumberListener(CTelephoneShadowControl *context);
    virtual void OnChange();
};

/** Listener for property CallState */
class CCallStateListener : public IACSMOSTEventListener
{
    CTelephoneShadowControl *m_TelephoneFblock;

public:
    CCallStateListener(CTelephoneShadowControl *context);
    virtual void OnChange();
};

/** Listener for property CallDuration */
class CCallDurationListener : public IACSMOSTEventListener
{
    CTelephoneShadowControl *m_TelephoneFblock;

public:
    CCallDurationListener(CTelephoneShadowControl *context);
    virtual void OnChange();
};

/** Listener for property ModemLevel */
class CModemLevelListener : public IACSMOSTEventListener
{
    CTelephoneShadowControl *m_TelephoneFblock;

public:
    CModemLevelListener(CTelephoneShadowControl *context);
    virtual void OnChange();
};

/** Listener for property Call Recipient Info */
class CRecipientInfoListener : public IACSMOSTEventListener
{
    CTelephoneShadowControl *m_TelephoneFblock;

public:
    CRecipientInfoListener(CTelephoneShadowControl *context);
    virtual void OnChange();
};

/** Listener for property New SMS */
class CMissedCallStatusListener : public IACSMOSTEventListener
{
    CTelephoneShadowControl *m_TelephoneFblock;

public:
    CMissedCallStatusListener(CTelephoneShadowControl *context);
    virtual void OnChange();
};

/** Listener for property User Mode */
class CUserModeListener : public IACSMOSTEventListener
{
    CTelephoneShadowControl *m_TelephoneFblock;

public:
    CUserModeListener(CTelephoneShadowControl *context);
    virtual void OnChange();
};

/** Listener for property Modem Data Status */
class CModemDataStatusListener : public IACSMOSTEventListener
{
    CTelephoneShadowControl *m_TelephoneFblock;

public:
    CModemDataStatusListener(CTelephoneShadowControl *context);
    virtual void OnChange();
};


class CTMServiceListener : public IACSMOSTEventListener
{
    CTelephoneShadowControl *m_TelephoneFblock;

public:
    CTMServiceListener(CTelephoneShadowControl *context);
    virtual void OnChange();
};


class CCallHistoryStatusListener : public IACSMOSTEventListener
{
    CTelephoneShadowControl *m_TelephoneFblock;

public:
    CCallHistoryStatusListener(CTelephoneShadowControl *context);
    virtual void OnChange();
};


/** Class definition For telephone Shadow Control */
class CTelephoneShadowControl : public QObject, public CControlThreadBase
{
    Q_OBJECT

    /** Telephone Shadow instance */
    CTelephone *m_TelephoneShadow;

    /** Listeners and Handlers for Properties of Telephone */
    CCollectPBook *m_CollectPBook;
    CCollectPBookHandler *m_CollectPBookHandler;

    CPBookStatus *m_PBookStatus;
    CPBookStatusListener *m_PBookStatusListener;

    CCallHistoryInfo *m_CallHistoryInfo;
    CCallHistoryInfoListener *m_CallHistoryInfoListener;

    CCurrentNumber *m_CurrentNumber;
    CCurrentNumberListener *m_CurrentNumberListener;

    acs::generated::shadow::Telephone::CCallState *m_CallState;
    CCallStateListener *m_CallStateListener;

    CCallDuration *m_CallDuration;
    CCallDurationListener *m_CallDurationListener;

    CModemLevel *m_ModemLevel;
    CModemLevelListener *m_ModemLevelListener;

    CRecipientInfo *m_RecipientInfo;
    CRecipientInfoListener *m_RecipientInfoListener;

    CMissedCallStatus *m_MissedCallStatus;
    CMissedCallStatusListener *m_MissedCallStatusListener;

    acs::generated::shadow::Telephone::CUserMode *m_UserMode;
    CUserModeListener *m_UserModeListener;

    CModemDataStatus *m_ModemDataStatus;
    CModemDataStatusListener *m_ModemDataStatusListener;

    acs::generated::shadow::Telephone::CDialNumber *m_DialNumber;
    CDialNumberHandler *m_DialNumberHandler;

    acs::generated::shadow::Telephone::CAcceptCall *m_AcceptCall;
    CAcceptCallHandler *m_AcceptCallHandler;

    acs::generated::shadow::Telephone::CHangupCall *m_HangupCall;
    CHangupCallHandler *m_HangupCallHandler;

    CTMService *m_TMService;
    CTMServiceListener *m_TMServiceListener;

    CCallHistoryStatus *m_CallHistoryStatus;
    CCallHistoryStatusListener *m_CallHistoryStatusListener;

public:
    CTelephoneShadowControl(CMarshaledACSApplication *context);
    ~CTelephoneShadowControl();

    bool Initialize();
    bool FBlockVisible();

    void SetListeners();
    void RemoveListeners();

    /** Below are the methods invoked by Listeners and Handlers for
     * the properties Telephone
     */
    void CurrentNumber();
    void CallState();
    void CallDuration();
    void ModemLevel();
    void RecipientInfo();
    void MissedCallStatus();
    void UserMode();
    void ModemDataStatus();
    void CollectPBookResult(const uint state, const uint32_t payloadLength, const uint8_t * payload);
    void PBookStatus();
    void CallHistoryInfoRequestStatus();
    void DialNumberResult(const uint state, const uint32_t payloadLength, const uint8_t * payload);
    void AcceptCallResult(const uint state, const uint32_t payloadLength, const uint8_t * payload);
    void HangupCallResult(const uint state, const uint32_t payloadLength, const uint8_t * payload);

    void CollectPBookHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state);
    void DialNumberHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state);
    void AcceptCallHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state);
    void HangupCallHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state);

    void TMServiceStatus();     //For Fcat v13.6.1
    void CallHistoryStatus();   //For Fcat v13.9.2

    /** Below are the Methods invoked by the Telephone Adaptor */
    //bool StartCollectPBook(const SCollectPBook &pBookDataRequest);
    bool StartCollectPBook(const uint senderHandle, const uint pBookType);
    bool StartDialNumber(const SDialNumberData &dialNumData);
    // Deleted in Fcat v1.22.6
    //bool SetPBookStatus(const uint pbookstatus);    
    bool CallHistoryInfoRequest();
    bool StartAcceptCall(const uint senderHandle, const uint callId);
    bool StartHangupCall(const uint senderHandle, const uint callId);
    bool GetModemDataStatus(void);
    bool GetPBookStatus(void);
    bool GetModemLevel(void);
    bool GetTMSerivce(void);
    bool GetCallHistoryStatus(void);    //For Fcat v13.9.2

signals:
    /** Below are the signals sent to TelephoneAdaptor */
    void CurrentNumber(const QString telNumber);
    void CallState(const QDBusVariant &callStateData);
    void CallDuration(const QDBusVariant &callDurationData);
    void ModemLevel(const uint modemlevel);
    void RecipientInfo(const uint contact, const QString &name);
    void MissedCallStatus(const uint msdCallStatus, const uint msdCallCount);
    void UserMode(const uint usermode,const bool micOff);
    void ModemDataStatus(const uint modemstatus);
    void CollectPBookResult(const QDBusVariant &pBookDataResult);
    void PBookStatus(const ushort pBookStatus);
    void CallHistoryInfo(const QDBusVariant &callHistoryInfo);
    void DialNumberResult(const QDBusVariant &dialNumData);
    void AcceptCallResult(const uint senderHandle, const uint callId);
    void HangupCallResult(const uint senderHandle, const uint callId);
    void TMServiceStatus(const uint tmStatus, const uint provStatus);     //For Fcat v13.6.1
    void CallHistoryStatus(const uint status);      //For Fcat v13.9.2
};

#endif // DHAVN_MOSTMANAGER_TELEPHONESHADOWCONTROL_H
