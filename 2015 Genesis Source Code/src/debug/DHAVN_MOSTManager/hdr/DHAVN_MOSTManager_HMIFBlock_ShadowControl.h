/*
 ******************************************************************
 * COPYRIGHT © <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER :  Neelima Khedad                                   *
 * DATE       :  22 Aug 2011                                      *
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
 *                  HMI Fblock and Shadow control, responsible to *
 *                  create instance of Fblock(HMI 0x01) and Shadow*
 *                  (HMI 0x02) configure it and add to most stack.*
 *                  If any listeners will be part of this class.  *
 *                  Communicates with ACS daemon                  *
 *                                                                *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT                  *
 ******************************************************************
 * 22 Aug 2011  Neelima Khedad           Draft version            *
 * 15 Sep 2011  Amit Koparde             Added Log_Tag define     *
 * 16 Sep 2011  Neelima Khedad           Added new API SetNotiModeStatusHU
 * 19 Sep 2011  Neelima Khedad           Added new parameter to ModeStatus of CHMIAdaptor
 * 12 Oct 2011  Neelima Khedad           Added API SetSystemVisibilityHUError to HMI
 * 27 Oct 2011  Neelima Khedad           Added popupsize parameter to struct SPopUpRequestData
 * 27 Oct 2011  Neelima Khedad           Adedd API SetDisplayControl
 * 01 Mar 2012  Neelima Khedad           Added Signal AbortPopUpRequestToHU
 * 06 Mar 2012  Neelima Khedad           Updated the parameters for ModeStatus,SystemVisibility and SystemAudibility
 * 19 Mar 2012  Shiva Kumar              Modified SetLCDBrightness method parameters.
 * 04 Apr 2012  Neelima Khedad           Added API New SetDistanceUnit  and parameter DateType to API SetDate to support fcat 1.15.
 * 05 May 2012  Shiva Kumar              Added new signal DisplayOSD in HMI Adaptor
 * 07 May 2012  Shiva Kumar              Added new signal ShowStatusBar and LockController
 * 06 Jun 2012  Shiva Kumar              Added new signal MenuStatus for FCAT 1.17
 * 21 Jun 2012  Shiva Kumar              Modified for FCAT 1.17.1, FCAT 1.17.2 and FCAT 1.17.3
 * 17 Jul 2012  Shiva Kumar              Modified for FCAT 1.18.2
 * 26 Jul 2012  Shiva Kumar              Modified for CR 12089, 12357
 * 10 Aug 2012  Shiva Kumar              Modified for CR 11699
 * 19 Sep 2012  Shiva Kumar              Modified for FCAT 1.19.0, 1.20.1, 1.20.2
 * 27 Nov 2012  Shiva Kumar              Modified for CR 16038
 * 07 Dec 2012  Shiva Kumar              Modified for FCAT 1.22.1
 *******************************************************************
 */


#ifndef DHAVN_MOSTMANAGER_HMIFBLOCK_SHADOWCONTROL_H
#define DHAVN_MOSTMANAGER_HMIFBLOCK_SHADOWCONTROL_H

#include <QtCore>
#include <QtDBus>
#include <persistman.h>

#include <DHAVN_LogUtil.h>

#include "DHAVN_MOSTManager_Config.h"
#include "DHAVN_MOSTManager_ACSApplication.h"
#include "DHAVN_MOSTManager_MOSTStructures.h"
#include "MOSTErrorInfo.h"
#include "DHAVN_MOSTManager_HMIFBlock.h"
#include "DHAVN_MOSTManager_HMIShadow.h"

#define LOG_TAG_HMI_CNT "HMIControl"

using namespace k2l::acs::samples;
using namespace k2l::acs::protocols::most;


/** Instance ID's for head unit and Ibox */
#define HMI_HEADUNIT    0x01
#define HMI_IBOX        0x02


/** Forward Decleration of class */
class CHMIFBlockShadowControl;

/** Listener for property Mode Status of Ibox */
class CModeStatusListenerIBOX : public IACSMOSTEventListener
{
    CHMIFBlockShadowControl* m_HMIFBlockShadowControl;

public:
    CModeStatusListenerIBOX(CHMIFBlockShadowControl* context);
    virtual void OnChange();
    virtual void OnError();
};

/** Listener for property POpup progress bar of Ibox*/
class CPopupProgressbarListenerIBOX : public IACSMOSTEventListener
{
     CHMIFBlockShadowControl* m_HMIFBlockShadowControl;

public:
    CPopupProgressbarListenerIBOX(CHMIFBlockShadowControl* context);
    void OnChange();
};

/** Listener for property KeyBoard of Head Unit*/
class CKeyBoardListenerHU : public IACSMOSTEventListener
{
    CHMIFBlockShadowControl* m_HMIFBlockShadowControl;

public:
    CKeyBoardListenerHU(CHMIFBlockShadowControl* context);
    void OnChange();
};
/** Listener for property DisplayOSD of Head Unit*/
class CDisplayOSDListener : public IACSMOSTEventListener
{
    CHMIFBlockShadowControl* m_HMIFBockShadowControl;
public:
    CDisplayOSDListener(CHMIFBlockShadowControl* context);
    void OnChange();
};
/** Listener for property ShowStatusBar of Head Unit*/
class CShowStatusBarListener : public IACSMOSTEventListener
{
    CHMIFBlockShadowControl* m_HMIFBockShadowControl;
public:
    CShowStatusBarListener(CHMIFBlockShadowControl* context);
    void OnChange();
};
/** Listener for property LockController of Head Unit*/
class CEmergencySetListener : public IACSMOSTEventListener
{
    CHMIFBlockShadowControl* m_HMIFBockShadowControl;
public:
    CEmergencySetListener(CHMIFBlockShadowControl* context);
    void OnChange();
};
/** Listener for property KeyInputStatus of Head Unit*/
class CKeyInputStatusListener : public IACSMOSTEventListener
{
    CHMIFBlockShadowControl* m_HMIFBockShadowControl;
public:
    CKeyInputStatusListener(CHMIFBlockShadowControl* context);
    void OnChange();
};
/** Listener for property drivingRestriction of IBOX */
class CDrivingRestrictionListener : public IACSMOSTEventListener
{
    CHMIFBlockShadowControl* m_HMIFBockShadowControl;
public:
    CDrivingRestrictionListener(CHMIFBlockShadowControl* context);
    void OnChange();
};
/** Listener for property LCDBrightness of HU*/
class CLCDBrightnessIBOXListener : public IACSMOSTEventListener
{
    CHMIFBlockShadowControl* m_HMIFBockShadowControl;
public:
    CLCDBrightnessIBOXListener(CHMIFBlockShadowControl* context);
    void OnChange();
};
/** Listener for property AdjustScreen of HU*/
class CAdjustScreenIBOXListener : public IACSMOSTEventListener
{
    CHMIFBlockShadowControl* m_HMIFBockShadowControl;
public:
    CAdjustScreenIBOXListener(CHMIFBlockShadowControl* context);
    void OnChange();
};
/** Listener for property Illumination setting of HU */
class CIlluminationSettingIBOXListener : public IACSMOSTEventListener
{
    CHMIFBlockShadowControl* m_HMIFBockShadowControl;
public:
    CIlluminationSettingIBOXListener(CHMIFBlockShadowControl* context);
    void OnChange();
};
/** Listener for property VideoRatio of HU */
class CVideoRatioIBOXListener : public IACSMOSTEventListener
{
    CHMIFBlockShadowControl* m_HMIFBockShadowControl;
public:
    CVideoRatioIBOXListener(CHMIFBlockShadowControl* context);
    void OnChange();
};
/** Listener for proprty DisplayControl of HU*/
class CDisplayControlIBOXListener : public IACSMOSTEventListener
{
    CHMIFBlockShadowControl* m_HMIFBockShadowControl;
public:
    CDisplayControlIBOXListener(CHMIFBlockShadowControl* context);
    void OnChange();
};
/** Listner for property BeepSetting of HU*/
class CBeepSettingListener : public IACSMOSTEventListener
{
    CHMIFBlockShadowControl* m_HMIFBockShadowControl;
public:
    CBeepSettingListener(CHMIFBlockShadowControl* context);
    void OnChange();
};

/** Hanlder for PopUpRequest From Ibox*/
class CPopUpRequestHandlerIBOX : public IShadowMethodHandler
{
    CHMIFBlockShadowControl* m_HMIFBlockShadowControl;

public:
    CPopUpRequestHandlerIBOX(CHMIFBlockShadowControl* context);
    /** Virtual functions of IShadowMethodHandler */
     void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};

/** Hanlder for PopUpRequest From head unit */
class CPopUpRequestHandlerHU : public IFunctionBlockMethodHandler
{
    CHMIFBlockShadowControl* m_HMIFBlockShadowControl;

public:
    CPopUpRequestHandlerHU(CHMIFBlockShadowControl* context);
    /** Virtual functions of IFunctionBlockMethodHandler */
     void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};

class CVideoDefaultSetHandlerHU : public IFunctionBlockMethodHandler
{
    CHMIFBlockShadowControl* m_HMIFBlockShadowControl;
public:
    CVideoDefaultSetHandlerHU(CHMIFBlockShadowControl* context);
    /** Virtual functions of IFunctionBlockMethodHandler */
     void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );

};

class CVideoDefaultSetHandlerIBOX : public IShadowMethodHandler
{
    CHMIFBlockShadowControl* m_HMIFBlockShadowControl;
public:
    CVideoDefaultSetHandlerIBOX(CHMIFBlockShadowControl* context);
    /** Virtual functions of IShadowMethodHandler */
     void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
     void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};

//#include <sys/time.h>
/** Listener for checking visibility of HMI FBlock */
class CHMIFblockStateListener : public QObject, public IACSMOSTEventListener
{
    Q_OBJECT

    QTimer *m_Timer;
    acs::generated::shadow::HMI::CHMI *m_HMI;
    CHMIFBlockShadowControl *m_HMIFBlockShadowControl;    

    USE_LOG_UTIL
    //struct timeval tv;

public:
    CHMIFblockStateListener(acs::generated::shadow::HMI::CHMI *hmi, CHMIFBlockShadowControl* context);
    virtual void OnChange();

private slots:
    void OnWaitForTimeout();
};

// Fcat v13.07.1
class CSystemDefaultSetHandler : public IShadowMethodHandler
{
    CHMIFBlockShadowControl* m_HMIFBlockShadowControl;
public:
    CSystemDefaultSetHandler(CHMIFBlockShadowControl* context);
    /** Virtual functions of IFunctionBlockMethodHandler */
    void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
};

// Fcat v13.07.2
class CMuteControlListener : public IACSMOSTEventListener
{
    CHMIFBlockShadowControl* m_HMIFBlockShadowControl;

public:
    CMuteControlListener(CHMIFBlockShadowControl* context);
    virtual void OnChange();
};


/** Definition of CHMIFBlockShadowControl */
class CHMIFBlockShadowControl:public QObject, public CControlThreadBase
{
    Q_OBJECT

private:

    /** Instance of HMI(0x01) on Head unit side hence Function Block */
    acs::generated::fblock::HMI::CHMI *m_HMIFBlock;
    /** Instance of HMI(0x02) on Ibox side hence Sahdow Block */
    acs::generated::shadow::HMI::CHMI *m_HMIShadow;

    /** Instances of Properties and Listeners of HMI on head unit side */

    acs::generated::fblock::HMI::CSystemLanguage *m_SystemLanguage;

    acs::generated::fblock::HMI::CPopUpRequest *m_PopUpRequestHU;
    CPopUpRequestHandlerHU *m_PopUpRequestHandlerHU;

    acs::generated::fblock::HMI::CKeyBoard *m_keyBoardHU;
    CKeyBoardListenerHU *m_KeyBoardListenerHU;

    /** Instances of Properties and Listeners of HMI on Ibox side */
    acs::generated::shadow::HMI::CPopUpRequest *m_PopUpRequestIBOX;
    CPopUpRequestHandlerIBOX *m_PopUpRequestHandlerIBOX;

    acs::generated::shadow::HMI::CModeStatus *m_ModeSatusIBOX;
    CModeStatusListenerIBOX *m_ModeStatusListenerIBOX;

    acs::generated::shadow::HMI::CPopUpProgressBar *m_PopUpProgressBarIBOX;
    CPopupProgressbarListenerIBOX *m_PopUpProgressBarListenerIBOX;

    acs::generated::fblock::HMI::CLCDBrightness *m_LCDBrightnessHU;
    CLCDBrightnessIBOXListener *m_LCDBrightnessIBOXListener;

    acs::generated::fblock::HMI::CDisplayOSD *m_DisplayOSD;
    CDisplayOSDListener *m_DisplayOSDListener;

    acs::generated::fblock::HMI::CShowStatusBar *m_ShowStatusBar;
    CShowStatusBarListener *m_ShowStatusBarListener;

    acs::generated::fblock::HMI::CEmergencySet *m_Emergencyset;
    CEmergencySetListener *m_EmergencySetListener;

    acs::generated::shadow::HMI::CKeyInputStatus *m_KeyInputStatus;
    CKeyInputStatusListener *m_KeyInputStatusListener;

    acs::generated::fblock::HMI::CBeepSetting *m_Beepsetting;
    CBeepSettingListener *m_BeepSettingListener;

    acs::generated::shadow::HMI::CDrivingRestriction *m_DrivingRestriction;
    CDrivingRestrictionListener *m_DrivingRestrictionListener;

    acs::generated::fblock::HMI::CAdjustScreen *m_AdjustScreenIBOX;
    CAdjustScreenIBOXListener *m_AdjustScreenIBOXListener;

    acs::generated::fblock::HMI::CIlluminationSetting *m_IlluminationSettingIBOX;
    CIlluminationSettingIBOXListener *m_IlluminationSettingIBOXListener;

    acs::generated::fblock::HMI::CVideoRatio *m_VideoRatioIBOX;
    CVideoRatioIBOXListener *m_VideoRatioIBOXListener;

    acs::generated::fblock::HMI::CDisplayControl *m_DisplayControlIBOX;
    CDisplayControlIBOXListener *m_DisplayControlIBOXListener;

    acs::generated::fblock::HMI::CSoundStatus *m_SoundStatus;

    acs::generated::fblock::HMI::CVideoDefaultSet *m_VideoDefaultSetHU;
    CVideoDefaultSetHandlerHU *m_VideoDefaultSetHandlerHU;

    acs::generated::shadow::HMI::CVideoDefaultSet *m_VideoDefaultSetIBOX;
    CVideoDefaultSetHandlerIBOX *m_VideoDefaultSetHandlerIBOX;

    acs::generated::fblock::HMI::CSystemTime *m_SystemTime;
    acs::generated::fblock::HMI::CSystemDate *m_SystemDate;

    acs::generated::fblock::HMI::CPhonePriority *m_PhonePriority;

    CHMIFblockStateListener *m_HMIFblockStateListener;

    // Fcat v13.07.1
    acs::generated::shadow::HMI::CSystemDefaultSet *m_SystemDefaultSet;
    CSystemDefaultSetHandler *m_SystemDefaultSetHandler;

    // Fcat v13.07.2
    acs::generated::fblock::HMI::CMuteControl *m_MuteContorl;
    CMuteControlListener *m_MuteControlListener;

public:
    acs::generated::fblock::HMI::CModeStatus *m_ModeStatusHU;

    acs::generated::fblock::HMI::CSystemVisibility *m_SystemVisibilityHU;

    acs::generated::fblock::HMI::CSystemAudibility *m_SystemAudibilityHU;

    CHMIFBlockShadowControl(CMarshaledACSApplication *context);
    ~CHMIFBlockShadowControl();

    virtual bool Initialize();
    virtual bool Register();
    virtual bool UnRegister();

    bool FBlockVisible(const uint deviceID);

    void SetListeners();

    void RemoveListeners();

    /** Methods invoked by the Adaptor API's */
    bool SetLanguage(const QString systemLanguageCode);

    bool SetVolumeStatus(const uint sourceType, const uint defaultVolume, const uint currentVolume);

    bool SetTime(const QByteArray time, const int timeZone, const uint dst);

    bool SetDate(const QDBusVariant systemDate);

    bool SetModeStatusIBOX(const uint target, const uint mode);

    bool SetModeStatusHU(const uint target, const uint mode);

    bool SetSoundStatus(const uint soundStatus);

    // Deprecated, replaced to bool SetSystemVisibilityHU(const uint vTarget, const QByteArray vSource)
    bool SetSystemVisibilityHU(const uint vTarget, const uint vSource);

    bool SetSystemVisibilityHU(const uint vTarget, const QByteArray vSource);

    bool SetSystemVisibilityHUError(const uint errorCode, const QString &errorInfo);

    bool SetSystemAudibilityHU(const uint aTarget, const uint aSource);

    bool SetSystemAudibilityHUError(const uint errorCode, const QString &errorInfo);

    bool SetScreen(const int brightness, const int saturation, const int contrast, const int hue);

    bool SetLCDBrightness(const int bTarget,const int lcdFront, const int lcdRear1, const int lcdRear2);

    bool SetIllumination(const int illumination);

    bool SetVideoRatio(const int ratio);

    bool SetKeyBoard(const QDBusVariant keyBoardData);

    bool SetProgressbar(const uint status);

    bool SetDisplayControl(const uint displayStatus);

    bool SetDistanceUnit(const uint distanceUnit);

    bool SetPopUpRequestToIBOX(const SPopUpRequestData popUpRequestData);

    bool SetPopUpResultFromHU(const SPopUpResultData popUpResultData);

    bool SetKeyInputStatus(const uint hardKey, const uint softKey);

    bool SetBeepSetting(const uint beepSetting);

    bool SetVideoDefaultResultIBOX(const uint state, const uint deviceID, ushort const senderHandle);

    bool SetVideoDefaulttoIBOX(const uint senderHandle , ushort const sTarget);

    bool SetEmergencySetIBOX(const uint emgcState, const uint controllerLock, const uint soundLock);

    bool SetPhonePriority(const uint phonePriority);
    // Fcat v13.06.3
    bool SetDisplayOSDStatus(const uint osdId, const uint osdStatus);
    // Fcat v13.07.1
    bool SetSystemDefaultSet(const uint senderHandle);
    // Fcat v13.07.2
    bool SetMuteStatusStatus(const bool muteStatus);
    bool SetMuteStatusError(const uint errorCode);

    /** Methods invoked by listeners */
    void ModeStatusIBOX();

    void ModeStatusErrorIBOX();

    void KeyBoardSetting();

    void Progressbar();

    void DisplayOSDListener();

    void ShowStatusBarSlot();

    void EmergencySetSlot();

    void KeyInputStatusSlot();

    void DrivingRestrictionStatusSlot();

    void LCDBrightnessFromIBOXSlot();

    void AdjustScreenFromIBOXSlot();

    void IlluminationFromIBOXSlot();

    void VideoRatioFromIBOXSlot();

    void DisplayControlFromIBOXSlot();

    void PopUpRequestToHU(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload);

    void AbortPopUpRequestToHU(const uint32_t payloadLength, const uint8_t *payload);

    void PopUpResultFromIBOX(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uint state);

    void PopUpRequestHandlerIBOX(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uint state);

    void PopUpRequestHandlerHU(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uint state);

    void VideoDefaultSetHandlerHU(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uint state);

    void VideoDefaultSetHandlerIBOX(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uint state);

    void BeepSettingFromIBOXSlot();

    void HMIFblockStateListener(const bool state);

    // Fcat v13.07.1
    void SystemDefaultSetHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t * payload, const uint state);

    // Fcat v13.07.2
    void MuteStatusListener();

signals:
    /** Signals sent to HMIAdaptor */
    void ModeStatusIBOX(const uint target, const uint mode);

    void ModeStatusErrorIBOX(const uint errorCode, const QByteArray errorInfo);

    void KeyBoardSetting(const QDBusVariant keyBoardData);

    void Progressbar(const uint status);

    void PopUpRequestToHU(const QDBusVariant popUpRequestData);

    void AbortPopUpRequestToHU(ushort senderHandle);

    void PopUpResultFromIBOX(const QDBusVariant popUpResultData);

    void DisplayOSD(const QDBusVariant displayOSD);

    void ShowStatusBar(const uint statusbar);

    void LockController(const uint controller);

    void KeyInputStatus(const uint hardKey, const uint softKey);

    void DrivingRestrictionStatus(const uint need);

    void AdjustScreenFromIBOX(const int brightness, const int saturation, const int contrast, const int hue);

    void LCDBrightnessFromIBOX(const int bTarget, const int lcdFront, const int lcdRear1, const int lcdRear2);

    void IlluminationFromIBOX(const int illumination);

    void VideoRatioFromIBOX(const int ratio);

    void DisplayControlFromIBOX(const uint displayStatus);

    void VideoDefaultRequestIBOX(const uint deviceID, const ushort senderHandle, const uint sTarget);

    void VideoDefaultResultIBOX(const uint state, const uint senderHandle);

    void BeepSettingFromIBOX(const uint beepSetting);

    void EmergencySetIBOX(const uint emgcState, const uint controllerLock, const uint soundLock);

    void HMIFunctionBlockVisible(const bool state);

    // Fcat v 13.07.1
    void SystemDefaultSetResult(const uint state, const uint16_t senderHandle, const uint8_t errorCode, const QByteArray errorInfo);

    // Fcat v 13.07.1
    void MuteStatusSet(const bool status);
};

#endif // DHAVN_MOSTMANAGER_HMIFBLOCK_SHADOWCONTROL_H
