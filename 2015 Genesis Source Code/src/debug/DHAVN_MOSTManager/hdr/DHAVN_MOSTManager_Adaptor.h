/*
 ******************************************************************
 * COPYRIGHT ¨Ï <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     : MOSTManager                                       *
 * PROGRAMMER : Neelima Khedad,Venkatesh Aeturi                   *
 * DATE       : 28 July 2011                                      *
 * VERSION    :                                                   *
 *----------------------------------------------------------------*
 * MODULE SUMMARY : MOST Manager is a middle ware component which *
 *                  talks to the applications and controller over *
 *                  QtDbus.The data sent over QtDbus is further   *
 *                  communicated to other MOST devices through ACS*
 *                  daemon.This module is also responsible for    *
 *                  commands from ACS daemon.                     *
 *                  This file contains the definition for all     *
 *                  Adaptor classes which exposes APIs and signals*
 *                  over QtDbus to applications and controller.   *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS:
 *
 *    DATE         PROGRAMMER               COMMENT
 ******************************************************************
 * 28 Jul 2011  Neelima Khedad           Draft version
 * 08 Aug 2011  Neelima Khedad           Added methods to class CDLNAAdaptor
 * 09 Aug 2011  Sajeer Shamsudeen        Added methods to class CDVDAdaptor
 * 09 Aug 2011  Venkatesh Aeturi         Added methods to class CAuxInAdaptor
 * 10 Aug 2011  Venkatesh Aeturi         Added methods to class CDMBAdaptor
 * 11 Aug 2011  Neelima Khedad           Removed '&' for arguments of function in CAmFmTunerAdaptor
 * 16 Aug 2011  Sajeer Shamsudeen        Removed '&' for arguments of function in CDVDPlayerAdaptor
 * 22 Aug 2011  Neelima Khedad           Added methods to class CHMIAdaptor
 * 29 Aug 2011  Neelima Khedad           Added methods to class CIBOXAdaptor
 * 06 Sep  2011 Venkatesh Aeturi         Added definitions for methods CTelephoneAdaptor,CSpeechAdaptor
 *                                       CJukeBoxAdaptor,CVirtualDiscAdaptor,CVehicleAdaptor
 *                                       CBlueToothAdaptor.CSatelliteRadioAdaptor.
 * 06 Sep 2011  Shiva Kumar              Added methods to class CSWDLAdaptor
 * 07 Sep 2011  Neelima Khedad           Added Comenst for API of CTelephoneAdaptor
 * 09 Sep 2011  Shiva Kumar              Changed the parameters of functions SetReadModuleVersion,ReadModuleVersion,
 *                                       SetSWDLProgressInfo, SWDLProgressInfo and signal ReadModuleVersion, SWDLProgressInfo
 * 12 Sep 2011  Amit Koparde             Added methods to class CDABAdaptor
 * 12 Sep 2011  Venkatesh Aeturi         Added methods to class CNavigationAdaptor
 * 13 Sep 2011  Amit Koparde             Added methods to class CDiagnosisAdaptor
 * 16 Sep 2011  Neelima Khedad           Added new API SetNotiModeStatusHU to CHMIAdaptor
 * 16 Sep 2011  Amit Koparde             Added new API SetPowerState to CVehicleAdaptor
 * 19 Sep 2011  Neelima Khedad           Added new parameter to ModeStatus of CHMIAdaptor
 * 19 Sep 2011  Shiva Kumar              Changed the parameters for API currentsonginfo and currentvideoinfo for DVD
 * 27 Sep 2011  Neelima Khedad           Removed the IsRunning API from all the adaptors
 * 27 Sep 2011  Amit Koparde             Added new API SetCurrentSongInfo and SetTerminalMode to CAuxInAdaptor
 * 27 Sep 2011  Amit Koparde             Changed the parameters for API SetCurrentSongInfo of CBluetoothAdaptor and CVirtualDiscAdaptor *
 * 12 Oct 2011  Neelima Khedad           Added API SetSystemVisibilityHUError to CHMIAdaptor
 * 12 Oct 2011  Neelima Khedad           Added API SetTrackposition to CDLNAAdaptor
 * 14 Oct 2011  Neelima Khedad           Added API parameters for CbluetoothAdaptor
 * 26 Oct 2011  Amit Koparde             Added new API's SetModemLevel, SetUserMode, SetCallHistoryCopyStatus and SetMissedCallStatus for Bluetooth Adaptor
 * 26 Oct 2011  Shiva Kumar              Added new API's SetRDSSetting, SetTMCStatus and SetTMCInfo for CAmFmTunerAdaptor
 * 27 Oct 2011  Neelima Khedad           Adedd API SetDisplayControl to HMI Adaptor
 * 27 Oct 2011  Neelima Khedad           Adedd parameter poicount to Signal POIDataAvailable
 * 27 Oct 2011  Neelima Khedad           Removed Signal NewSMS and added MissedCallStatus for Telephone
 * 28 Oct 2011  Shiva Kumar              Removed CVirtualDisk Adaptor class
 * 04 Nov 2011  Shiva kumar              Added API SetUpdateMediaList for CJukeBoxAdaptor
 * 13 Dec 2011  Shiva Kumar              Changed parameter of SetTMCInfo in CAMFMAdaptor
 * 13 Dec 2011  Shiva Kumar              Changed parameter of SetTunedProgram and SetTPEGInfo of CDMBTunerAdaptor
 * 20 Jan 2012  Neelima Khedad           Updated AMFMTunr Adaptor to support for version 1.13.0
 * 20 Jan 2012  Neelima Khedad           Updated AuxIn  Adaptor to support for version 1.13.0
 * 01 Mar 2012  Neelima Khedad           Added Signal AbortPopUpRequestToHU to HMI Adaptor
 * 19 Mar 2012  Shiva Kumar              Modified API parameters for SetLCDBrightness
 * 04 Apr 2012  Neelima Khedad           Added Signal POFDataAvailable to support fcat 1.15 to IBOX adaptor
 * 04 Apr 2012  Neelima Khedad           Added API New SetDistanceUnit  and parameter DateType to API SetDate to support fcat 1.15 to HMI Apadtor
 * 05 Apr 2012  Shiva Kumar              Modified ModeStatus, SystemVisibility, SystemAudibility and LCDBrightness API and signals
 * 05 May 2012  Shiva Kumar              Added new signal DisplayOSD in HMI Adaptor
 * 07 May 2012  Shiva Kumar              Added new signals ShowstatusBar, LockControll in HMI Adaptor and GISDataAvailableError in IBOX Adaptor
 * 06 Jun 2012  Shiva Kumar              Added new signals and APIs in DLNA and HMI Adaptor for FCAT 1.17
 * 21 Jun 2012  Shiva Kumar              Modified for FCAT 1.17.1, FCAT 1.17.2 and FCAT 1.17.3
 * 17 Jul 2012  Shiva Kumar              Modified for FCAT 1.18.2
 * 27 Jul 2012  Shiva Kumar              Modified for CR 12089, 12357
 * 19 Sep 2012  Shiva Kumar              Modified for FCAT 1.19.0, 1.20.1, 1.20.2
 * 15 Nov 2012  Shiva Kumar              Modified for CR 15573
 * 27 Nov 2012  Shiva Kumar              Modified for CR 16038
 * 07 Dec 2012  Shiva Kumar              Modified for FCAT 1.22.1
 * 25 Jan 2013  Hanhong Keum             Deleted SetPBookStatus according to Fcat v1.22.6
 * 19 Mar 2013  Hanhong Keum             Modified for FCAT 1.22.7 and 1.22.8
 *****************************************************************
 */

#ifndef DHAVN_MOSTMANAGER_ADAPTOR_H
#define DHAVN_MOSTMANAGER_ADAPTOR_H


#include <QtCore/QObject>
#include <QtDBus/QDBusAbstractAdaptor>
#include <QtDBus/QtDBus>

#include <DHAVN_LogUtil.h>

#include "DHAVN_MOSTManager_Config.h"
#include "DHAVN_MOSTManager_MOSTManager.h"

#define LOG_TAG_DMB_ADAP "DMBTunerAdaptor"


/** Class definition for Auxillary Input adaptor */
class CAuxInAdaptor: public QObject
{
    Q_OBJECT

private:
    CAuxInFblockControl *m_CAuxInContext;

public:
    CAuxInAdaptor(CMOSTManager *context);
    ~CAuxInAdaptor();

public Q_SLOTS:

    /** Below methods are called by AuxIn controller to set info to the Fblock. */	

    bool SetAuxDeviceInfo(const QDBusMessage& msg);

signals:
    /** No Signals for this class */
    
};


/** Class definition for BlueTooth Adaptor */
class CBlueToothAdaptor : public QObject
{
    Q_OBJECT

private:
    CBluetoothFblockControl *m_CBluetoothControlContext;

public:
    CBlueToothAdaptor(CMOSTManager *context);
    ~CBlueToothAdaptor();

private Q_SLOTS:
    /** Below are the slots connected to the
     *  Signals from CBluetoothFblockControl
     */
    void StartHangupCallSlot(const uint senderHandle, const uint callId,const uint deviceID);
    void DialNumberRequestIBOXSlot(const uint senderHandle, const QString telNumber,const uint deviceID);
    void UserModeSlot(const uint usermode,const bool micOff);

public Q_SLOTS:
    bool SetCallState(const QDBusVariant &callStatedata);
    bool HangupCallResult(const uint senderHandle, const uint callId,const uint deviceID);
    bool DialNumberResultIBOX(const uint state, const uint senderHandle, const uint callID, const uint
                             deviceID);
    bool SetBTStatus(const uint btMode);
    bool SetUserMode(const uint usermode,const bool micOff);


signals:   
    void StartHangupCall(const uint senderHandle, const uint callId,const uint deviceID);
    void DialNumberRequestIBOX(const uint senderHandle, const QString telNumber, const uint deviceID);
    void UserMode(const uint usermode,const bool micOff);

};

/** Class definition for Diagnosis Adaptor */
class CDiagnosisAdaptor: public QObject
{
    Q_OBJECT

public:
    CDiagnosisAdaptor(CMOSTManager *context);
    ~CDiagnosisAdaptor();

private:
    CDiagnosisFBlockShadowControl *m_DiagnosisFBlockShadowControl;

private  Q_SLOTS:
    /** Below are the slots connected to the
     *  Signals from CDiagnosisFBlock_ShadowControl
     */
    void MainSWVersionStatusSlot(const QStringList mainSWVersion);
    void GetVariantCodeStatusSlot();

public Q_SLOTS:
    bool GetMainSWVersion();
    // Deleted
    bool SetVariantCode(const QDBusVariant &variantcode);

signals:
    void MainSWVersionStatus(const QStringList mainSWVersion);
    void GetVariantCodeStatus();
};

/** Class definition for DLNA Adaptor */
 class CDLNAAdaptor: public QObject
 {
     Q_OBJECT

 private:
     CDLNAShadowControl *m_CDLNAShadowControlContext;

 public:
     CDLNAAdaptor(CMOSTManager *context);
     ~CDLNAAdaptor();

 private slots:
     /** Below are the slots connected to the
      *  Signals from CDLNAShadowControl
      */	
     void TimePositionSlot(const int diskTime, const int trackTime, const int titleTime);    
     void DLNAConnStatusSlot(const uint status);
     void CurrentSongInfoSlot(const uint playtime, const ushort trackno, const QStringList songinfo);
     void CurrentVideoInfoSlot(const QString vtitle, const uint time, const ushort track, const QString vfilename);
     void DeckStatusSlot(const uint deckStatus);
     void TrackPositionSlot(const uint trackPosition);
     void TrackInformationSlot(const uint currentNumTrack, const uint currentRelativeTrackPos);


 public Q_SLOTS:     
     bool SetDeckStatus(const QDBusMessage& msg);
     bool SetTrackPosition(const QDBusMessage& msg);
     bool SetIncrementTrackPosition(const uint nSteps);
     bool SetDecrementTrackPosition(const uint nSteps);

 signals:
     void TimePosition(const int diskTime, const int trackTime, const int titleTime);
     void DLNAConnStatus(const uint status);
     /**
      songinfo[0] = title;
      songinfo[1] = filename;
      songinfo[2] = artist;
      songinfo[3] = album;
      songinfo[4] = genre;
     */
     void CurrentSongInfo(const uint playtime, const ushort trackno, const QStringList songinfo);
     void CurrentVideoInfo(const QString vtitle, const uint time, const ushort track, const QString vfilename);
     void DeckStatus(const uint deckStatus);
     void TrackPosition(const uint trackPosition);
     void TrackInformation(const uint currentNumTrack, const uint currentRelativeTrackPos);


 };

/** Class definition for HMI Adaptor */
class CHMIAdaptor: public QObject
{
    Q_OBJECT

public:
    CHMIAdaptor(CMOSTManager *context);
    ~CHMIAdaptor();
private:
    CHMIFBlockShadowControl *m_HMIFBlockShadowControl;

private Q_SLOTS:
    /** Below are the slots connected to the
     *  Signals from CHMIFBlock_ShadowControl
     */
    void ModeStatusIBOXSlot(const uint target, const uint mode);
    void ModeStatusErrorIBOXSlot(const uint errorCode, const QByteArray errorInfo);
    void ModeStatusHUSlot(const uint target, const uint mode);

    void SystemVisibilityHUSlot(const uint vTarget, const uint vSource);
    void SystemAudibilityHUSlot(const uint aTarget, const uint aSource);

    void KeyBoardSettingSlot(const QDBusVariant keyBoardData);
    void ProgressbarSlot(const uint status);

    void PopUpRequestToHUSlot(const QDBusVariant popUpRequestData);
    void AbortPopUpRequestToHUSlot(ushort senderHandle);
    void PopUpResultFromIBOXSlot(const QDBusVariant popUpResultData);
    void DisplayOSDSlot(const QDBusVariant displayOSD);
    void ShowStatusBarSlot(const uint statusbar);

    //void KeyInputStatusSlot(const uint keyInput);
    void KeyInputStatusSlot(const uint hardKey, const uint softKey);    // Fcat v13.9.3

    void DrivingRestrictionStatusSlot(const uint need);
    void AdjustScreenFromIBOXSlot(const int brightness, const int saturation, const int contrast, const int hue);
    void LCDBrightnessFromIBOXSlot(const int bTarget, const int lcdFront, const int lcdRear1, const int lcdRear2);
    void IlluminationFromIBOXSlot(const int illumination);
    void VideoRatioFromIBOXSlot(const int ratio);
    void DisplayControlFromIBOXSlot(const uint displayStatus);
    void VideoDefaultRequestIBOXSlot(const uint deviceID,const ushort senderHandle, const uint sTarget);
    void VideoDefaultResultIBOXSlot(const uint state, const uint senderHandle);
    void BeepSettingFromIBOXSlot(const uint beepSetting);
    void EmergencySetIBOXSlot(const uint emgcState, const uint controllerLock, const uint soundLock);

    void HMIFunctionBlockVisibleSlot(const bool state);

    // Fcat v13.07.1
    void SystemDefaultSetResultSlot(const uint state, const uint16_t senderHandle, const uint8_t errorCode, const QByteArray errorInfo);

    // Fcat v13.07.2
    void MuteStatusSetSlot(const bool muteStatus);

public Q_SLOTS:
    bool SetLanguage(const QString systemLanguageCode);
    bool SetVolumeStatus(const uint sourceType, const uint defaultVolume, const uint currentVolume);
    /**
     * time[0] = Time Type
     * time[1] = hour;
     * time[2] = min;
     * time[3] = sec;
    */
    bool SetTime(const QByteArray time, const int timeZone, const uint dst);

    bool SetDate(const QDBusVariant systemDate);
    bool SetModeStatusIBOX(const QDBusMessage& msg);
    bool SetModeStatusHU(const QDBusMessage& msg);
    bool SetSoundStatus(const QDBusMessage& msg);
    bool SetSystemVisibilityHU(const QDBusMessage& msg);
    //bool SetSystemVisibilityHU(const uint vTarget, const QByteArray vSource);
    bool SetSystemVisibilityHUError(const QDBusMessage& msg);
    bool SetSystemAudibilityHU(const QDBusMessage& msg);
    bool SetSystemAudibilityHUError(const QDBusMessage& msg);
    bool SetScreen(const int brightness, const int saturation, const int contrast, const int hue);
    bool SetLCDBrightness(const int bTarget,const int lcdFront, const int lcdRear1, const int lcdRear2);
    bool SetIllumination(const int illumination);
    bool SetVideoRatio(const int ratio);
    bool SetKeyBoard(const QDBusVariant keyBoardData);
    bool SetProgressbar(const uint status);
    bool SetDisplayControl(const uint displayStatus);
    bool SetDistanceUnit(const uint distanceUnit);

    bool SetPopUpRequestToIBOX(const QDBusVariant popUpRequestData);
    bool SetPopUpResultFromHU(const QDBusMessage &msg);
    //bool SetPopUpResultFromHU(const QDBusVariant popUpResultData);
    bool SetBeepSetting(const uint beepSetting);

    //bool SetKeyInputStatus(const uint keyInput);
    bool SetKeyInputStatus(const uint hardKey, const uint softKey); // Fcat v13.9.3

    bool SetVideoDefaultResultIBOX(const uint state, const uint deviceID, const uint senderHandle);
    bool SetVideoDefaulttoIBOX(const uint senderHandle, const uint sTarget);
    bool SetEmergencySetIBOX(const uint emgcState, const uint controllerLock, const uint soundLock);
    bool SetPhonePriority(const uint phonePriority);
    //Fcat v13.06.3
    bool SetDisplayOSDStatus(const uint osdId, const uint osdStatus);
    // Fcat v13.07.1
    bool SetSystemDefaultSet(const uint senderHandle);
    // Fcat v13.07.2
    bool SetMuteStatusStatus(const QDBusMessage& msg);
    bool SetMuteStatusError(const QDBusMessage& msg);

signals:
    void ModeStatusIBOX(const uint target, const uint mode);
    void ModeStatusErrorIBOX(const uint errorCode, const QByteArray errorInfo);
    void ModeStatusHU(const uint target, const uint mode);

    void SystemVisibilityHU(const uint vTarget, const uint vSource);
    void SystemAudibilityHU(const uint aTarget, const uint aSource);

    void KeyBoardSetting(const QDBusVariant keyBoardData);
    void Progressbar(const uint status);

    void PopUpRequestToHU(const QDBusVariant popUpRequestData);
    void AbortPopUpRequestToHU(const uint senderHandle);
    void PopUpResultFromIBOX(const QDBusVariant popUpResultData);
    void DisplayOSD(const QDBusVariant displayOSD);
    void ShowStatusBar(const uint statusbar);

    //void KeyInputStatus(const uint keyInput);
    void KeyInputStatus(const uint hardKey, const uint softKey);    // Fcat v13.9.3

    void DrivingRestrictionStatus(const uint need);
    void AdjustScreenFromIBOX(const int brightness, const int saturation, const int contrast, const int hue);
    void LCDBrightnessFromIBOX(const int bTarget, const int lcdFront, const int lcdRear1, const int lcdRear2);
    void IlluminationFromIBOX(const int illumination);
    void VideoRatioFromIBOX(const int ratio);
    void DisplayControlFromIBOX(const uint displayStatus);
    void VideoDefaultRequestIBOX(const uint deviceID,const uint senderHandle, const uint sTarget);
    void VideoDefaultResultIBOX(const uint state, const uint senderHandle);
    void BeepSettingFromIBOX(const uint beepSetting);
    void EmergencySetIBOX(const uint emgcState, const uint controllerLock, const uint soundLock);
    void HMIFunctionBlockVisible(const bool state);

    // Fcat v13.07.1
    void SystemDefaultSetError(const uint state, const uint senderHandle, const uint errorCode, const QByteArray errorInfo);
    void SystemDefaultSetResult(const uint state, const uint senderHandle);

    // Fcat v13.07.2
    void MuteStatusSet(const bool muteStatus);
};

/** Class definition for IBox Adaptor */
class CIBoxAdaptor: public QObject
{
    Q_OBJECT

private:
    CIBOXShadowControl *m_IBOXShadowControl;

public:
    CIBoxAdaptor(CMOSTManager *context);
    ~CIBoxAdaptor();
private Q_SLOTS:
    /** Below are the slots connected to the
     *  Signals from CIBOXShadowControl
     */
    void WifiModeSlot(const uint mode,const uint clientStatus);
    void AppInstallStatusSlot(const uint appStatus, const uint appProgress);       
    void GISDataAvailableSlot(const QString uri, const uint available);
    void GISDataAvailableErrorSlot(const uint errorCode, const uint errorInfo);
    void OpenBrowserRequestResultSlot(const ushort senderHandle, const uint state);
    void GISRequestResultSlot(const QDBusVariant gisResultData);
    void GISDataRequestResultSlot(const QDBusVariant gisDataResult);
    void POIDataAvailableSlot(const uint target, const uint available, const QString uri);
    void ReceivedPOISlot(const uint available, const uint poiCount);
    void POIDataRequestResultSlot(const QDBusVariant poiDataResult);
    void POFDataAvailableSlot(const uint available,const uint pofCount);
    void VoiceSearchOnlineStatusSlot(const uint state, const ushort senderHandle, const uint blockType, const uint  blockCounter);
    void IBOXFunctionBlockInvisibleSlot(const bool status );    
    void LSDataAvailableSlot(const uchar optype, const QString uri, const uchar available);
    void LSDataAvailableErrorSlot(const uint errorCode, const uint errorInfo);
    void LSRequestSlot(const uchar optype, const ushort senderHandle, const uchar success);
    void LSDataRequestSlot(const uchar optype, const ushort senderHandle, const uchar target, const QString uri, QByteArray lsdata);
    void ACNActivatedSlot(const bool ACNActivatedValue);

public Q_SLOTS:    
    // Below methods are called by applications via qdbus
    bool StartOpenBrowserRequest(const QDBusMessage& msg);
    bool StartGISRequest(const QDBusVariant gisRequestData);
    bool StartGISDataRequest(const QDBusVariant gisDataRequestData);
    bool StartPOIDataRequest(const QDBusVariant poiDataRquest);
    bool SetVoiceSearchOnline(const uint voiceSearch);
    bool GetWiFiMode(void);
    bool SetLSRequest(const uint optype, const uint senderHandle, const QString uri, const QByteArray lsdata);
    bool SetLSDataRequest(const uint optype, const uint senderHandle, const uint target, const QString uri);

signals:
    void WifiMode(const uint mode,const uint clientStatus);
    void AppInstallStatus(const uint appStatus, const uint appProgress);        
    void GISDataAvailable(const QString uri, const uint available);
    void GISDataAvailableError(const uint errorCode, const uint errorInfo);
    void OpenBrowserRequestResult(const uint senderHandle, const uint state);
    void GISRequestResult(const QDBusVariant gisResultData);
    void GISDataRequestResult(const QDBusVariant gisDataResult);
    void POIDataAvailable(const uint target, const uint available, const QString uri);
    void ReceivedPOI(const uint available,const uint poiCount);
    void POIDataRequestResult(const QDBusVariant poiDataResult);
    void POFDataAvailable(const uint available,const uint pofCount);
    void VoiceSearchOnlineStatus(const uint state, const uint senderHandle, const uint blockType, const uint  blockCounter);
    void IBOXFunctionBlockInvisible(const bool status);
    void onLSDataAvailable(const uint optype, const QString uri, const uint available);
    void onLSDataAvailableError(const uint errorCode, const uint errorInfo);
    void onLSRequest(const uint optype, const uint senderHandle, const uint success);
    void onLSDataRequest(const uint optype, const uint senderHandle, const uint target, const QString uri, QByteArray lsdata);
    void ACNActivated(const bool ACNActivatedValue);
};

/** Class definition for Navigation Adaptor */
class CNavigationAdaptor: public QObject
{
    Q_OBJECT

private:
    //USE_LOG_UTIL

    CNavigationFblockControl *m_NavigationControlContext;

public:
    CNavigationAdaptor(CMOSTManager *context);
    ~CNavigationAdaptor();

public Q_SLOTS:   
    // Below methods are called by applications via qdbus
    bool SetGPSRawData(const QDBusVariant &gpsRawData);
    bool SetGPSRawDataBessel(const QDBusVariant &gpsRawDataBessel);
    bool SetMapData(const uint dataAvailable, const uint mapDataType);
    bool SetRequestLocalSearch(const uint optype, const uint senderHandle);
    bool SetOpenLocalSearch(const uint optype, const uint senderHandle);
    bool SetSearchPOI(const uint optype, const uint senderHandle, const uint success);
    bool SetSearchRoute(const uint optype, const uint senderHandle, const uint success);
    bool SetShowMap(const uint optype, const uint senderHandle, const uint success);
    bool SetRegisterMyLocation(const uint optype, const uint senderHandle, const uint success);
    bool SetDestinationInfo(const uint optype, const uint routeSet, const QDBusVariant &poiData);
    bool SetFindNearBy(const uint optype, const uint senderHandle, const uint success);
    bool SetCheckGPSBoundary(const uint optype, const uint senderHandle, const uint success);		// FCat v15.3.4

private Q_SLOTS:
    // Below methods are called by CNavigationFBlockControl thread
    void RequestLocalSearchSlot(const uchar optype, const ushort senderHandle, const QString inputKeyword);
    void OpenLocalSearchSlot(const uchar optype, const ushort senderHandle, const uchar target, const QString text);
    void SearchPOISlot(const uchar optype, const ushort senderHandle, const uchar target, const QString inputKeyword);
    void SearchRouteSlot(const QDBusVariant &searchRoute);
    void ShowMapSlot(const QDBusVariant &showMap);
    void RegisterMyLocationSlot(const QDBusVariant &registerMyLocation);
    void FindNearBySlot(const ushort senderHandle, const uchar target, const uchar poiKind);
//	void CheckGPSBoundarySlot(const ushort senderHandle, const int lat, const int lon);		// FCat v15.3.4
    void CheckGPSBoundarySlot(const QDBusVariant &checkGPSBoundary);		// FCat v15.3.4


signals:  
    // Emit the below signals to application via qdbus
    void onRequestLocalSearch(const uint optype, const uint senderHandle, const QString inputKeyword);
    void onOpenLocalSearch(const uint optype, const uint senderHandle, const uint target, const QString text);
    void onSearchPOI(const uint optype, const uint senderHandle, const uint target, const QString inputKeyword);
    void onSearchRoute(const QDBusVariant &searchRoute);
    void onShowMap(const QDBusVariant &showMap);
    void onRegisterMyLocation(const QDBusVariant &registerMyLocation);
    void onFindNearBySlot(const uint senderHandle, const uint target, const uint poiKind);
//	void onCheckGPSBoundarySlot(const uint senderHandle, const int lat, const int lon);		// FCat v15.3.4
    void onCheckGPSBoundarySlot(const QDBusVariant &checkGPSBoundary);		// FCat v15.3.4
	
};

/** Class definition for SWDL Adaptor */
class CSWDLAdaptor: public QObject
{
    Q_OBJECT

private:
    CSWDLFBlockShadowControl* m_CSWDLFBlockControlContext;

public:
    CSWDLAdaptor(CMOSTManager *context);
    ~CSWDLAdaptor();

private slots:
    /** Below are the slots connected to the
     *  Signals from CSWDLFBlock_ShadowControl
     */
    void ReadHWIdentifierSlot(const uint swdlid, const QString hwidentifier);
    void ReadDevNameSlot(const uint swdlid, const QString deviceName);
    void ReadHWVersionSlot(const uint swdlid, const QString hwversion);
    void ReadNumberofModuleSlot(const uint swdlid, const QByteArray modulevalue);
    void ReadModuleVersionSlot(const uint swdlid, const QDBusVariant &moduleversion);
    void ReprogrammingModeSlot(const QDBusVariant &reprogrammingmode);
    void InitUpdateResultSlot(const QDBusVariant &initUpdate);
    void EraseMemorySlot(const QDBusVariant &eraseMemory);
    void RequestDownloadSlot(const QDBusVariant &requestdownload);
    void PrepareFileTransferSlot(const QDBusVariant &preparefiletransfer);
    void TransferDataCMSSlot(const QDBusVariant &transferdataCMS);
    void TransferDataAMSSlot(const QDBusVariant &transferdataAMS);
    void TransferDataAMSErrorSlot(const uint swdlid, const uchar errorCode, const QByteArray errorInfo);
    void CheckActivateModuleSlot(const QDBusVariant &checkactivatemodule);
    void RequestTransferExitSlot(const QDBusVariant &requesttransferexit);
    void SWDLProgressInfoSlot(const uint swdlid, const uint swdlOpMode, const uint swdlProgress);
    void UpdateProgressingInfoStatusSlot(const uint swdlid, const uint updateStatus);    
	// iBox SWDL FBlock checkling for iBox Update, 2014.2.24
    void iBoxSWDLFBlockVisibleSlot(const bool state);

public Q_SLOTS:
    bool SetReadHWIdentifier(const uint swdlid, const uint state, const QString hwidentifier);
    bool SetReadDevName(const uint swdlid, const QString deviceName);
    bool SetReadHWVersion(const uint swdlid, const QString hwversion);
    bool SetReadNumberofModule(const uint swdlid, const QByteArray modulevalue);
    bool SetReadModuleVersion(const uint swdlid, const QDBusVariant &moduleversion);
    bool SetSWDLProgressInfo(const uint swdlOpMode, const uint swdlProgress);
    bool SetReprogrammingMode(const QDBusVariant &reprogrammingmode);
    bool SetInitUpdateResult(const QDBusVariant &initUpdate);
    bool SetEraseMemory(const QDBusVariant &eraseMemory);
    bool SetRequestDownload(const QDBusVariant &requestdownload);
    bool SetPrepareFileTransfer(const QDBusVariant &PrepareFileTransfer);
    bool SetPrepareFileTransferIBOX(const QDBusVariant &fileInfo);
    bool SetTransferDataCMS(const QDBusVariant &transferdataAMS);
    bool SetTransferDataAMS(const QDBusVariant &transferdataCMS);
    bool SetCheckActivateModule(const QDBusVariant &checkactivatemodule);
    bool SetRequestTransferExit(const QDBusVariant &requesttransferexit);
    bool SetUpdateProgressingInfo(const uint updateStatus);


signals:
    void ReadHWIdentifier(const uint swdlid, const QString hwidentifier);
    void ReadDevName(const uint swdlid, const QString deviceName);
    void ReadHWVersion(const uint swdlid, const QString hwversion);
    void ReadNumberofModule(const uint swdlid, const QByteArray modulevalue);
    void ReadModuleVersion(const uint swdlid, const QDBusVariant &moduleversion);
    void ReprogrammingMode(const QDBusVariant &reprogrammingmode);
    void InitUpdateResult(const QDBusVariant &initUpdate);
    void EraseMemory(const QDBusVariant &eraseMemory);
    void RequestDownload(const QDBusVariant &requestdownload);
    void PrepareFileTransfer(const QDBusVariant &PrepareFileTransfer);
    void TransferDataCMS(const QDBusVariant &transferdataAMS);
    void TransferDataAMS(const QDBusVariant &transferdataCMS);
    void TransferDataAMSError(const uint swdlid, const uchar errorCode, const QByteArray errorInfo);
    void CheckActivateModule(const QDBusVariant &checkactivatemodule);
    void RequestTransferExit(const QDBusVariant &requesttransferexit);
    void SWDLProgressInfo(const uint swdlid, const uint swdlOpMode, const uint swdlProgress);
    void UpdateProgressingInfoStatus(const uint swdlid, const uint updateStatus);

	// iBox SWDL FBlock checkling for iBox Update, 2014.2.24
	void iBoxSWDLFBlockVisible(const bool state);
};

/** Class definition for Speech Adaptor */
class CSpeechAdaptor : public QObject
{
    Q_OBJECT

public:
    CSpeechAdaptor(CMOSTManager *context);
    ~CSpeechAdaptor();

private:
    CSpeechRecognitionFblockControl *m_CSpeechRecognitionControlContext;

private Q_SLOTS:
    void StartTTSDataSlot(const uint sourceDeviceID, const uint senderHandle, const uint length, const QByteArray ttsData);
    void AbortTTSSlot(const uint sourceDeviceID, const uint senderHandle);

public Q_SLOTS:    
    bool SetTTSResult(const uint senderHandle, const uint lengthconst, const uint deviceid, const uint state);
    bool SetManualInput(const uint inputTarget, const QString inputKeyWord);

signals:

    void StartTTSData(const uint sourceDeviceID, const uint senderHandle, const uint length, const QByteArray ttsData);
    void AbortTTS(const uint sourceDeviceID, const uint senderHandle);

};

/** Class definition for Telephone Adaptor */
class CTelephoneAdaptor: public QObject
{
    Q_OBJECT

public:
    CTelephoneAdaptor(CMOSTManager *context);
    ~CTelephoneAdaptor();

private:
    CTelephoneShadowControl *m_TelephoneShadowControl;

private Q_SLOTS:
    /** Below are the slots connected to the
     *  Signals from CTelephoneShadowControl
     */
    void CurrentNumberSlot(const QString telNumber);
    void CallStateSlot(const QDBusVariant &callStatedata);
    void CallDurationSlot(const QDBusVariant &callDuration);
    void ModemLevelSlot(const uint modemlevel);
    void RecipientInfoSlot(const uint contact, const QString &name);
    void MissedCallStatusSlot(const uint msdCallStatus, const uint msdCallCount);
    void UserModeSlot(const uint usermode,const bool micOff);
    void ModemDataStatusSlot(const uint modemstatus);
    void CallHistoryInfoSlot(const QDBusVariant &callHistoryInfo);
    void CollectPBookResultSlot(const QDBusVariant &pBookDataResult);
    void PBookStatusSlot(const ushort totalPBEntry);
    void DialNumberResultSlot(const QDBusVariant &dialNumData);
    void AcceptCallResultSlot(const uint senderHandle, const uint callId);
    void HangupCallResultSlot(const uint senderHandle, const uint callId);
    void TMServiceStatusSlot(const uint tmStatus, const uint provStatus);     //For Fcat v13.9.5
    void CallHistoryStatusSlot(const uint status);      //For Fcat v13.9.2

public Q_SLOTS:    
    /** Below methods are called by Dialler,Messaging or PhoneBook */
    //bool StartCollectPBook(const QDBusVariant &pBookDataRequest);
    bool StartCollectPBook(const uint senderHandle, const uint pBookType);
    bool StartDialNumber(const QDBusVariant &dialNumData);
    //bool SetPBookStatus(const uint pbookstatus);  // Deleted in Fcat v1.22.6
    bool CallHistoryInfoRequest();
    bool StartAcceptCall(const QDBusMessage& msg);
    bool StartHangupCall(const uint senderHandle, const uint callId);
    bool GetModemDataStatus(void);
    bool GetModemLevel(void);
    bool GetPBookStatus(void);
    bool GetTMService(void);
    bool GetCallHistoryStatus(void);    //For Fcat v13.9.2

signals:
    /** Below signals are sent to Dialler,Messaging or PhoneBook
     * application which are received from iBox
     */
    void CurrentNumber(const QString telNumber);
    void CallState(const QDBusVariant &callStatedata);
    void CallDuration(const QDBusVariant &callDuration);
    void ModemLevel(const uint modemlevel);
    void RecipientInfo(const uint contact, const QString &name);
    void MissedCallStatus(const uint msdCallStatus, const uint msdCallCount);
    void UserMode(const uint usermode,const bool micOff);
    void ModemDataStatus(const uint modemstatus);
    void CallHistoryInfo(const QDBusVariant &callHistoryInfo);
    void CollectPBookResult(const QDBusVariant &pBookDataResult);
    void PBookStatus(const uint totalPBEntry);
    void DialNumberResult(const QDBusVariant &dialNumData);
    void AcceptCallResult(const uint senderHandle, const uint callId);
    void HangupCallResult(const uint senderHandle, const uint callId);
    void TMServiceStatus(const uint tmStatus, const uint provStatus);     //For Fcat v13.9.5
    void CallHistoryStatus(const uint status);      //For Fcat v13.9.2
};

/** Class definition for DMBTuner Adaptor */
class CDMBTunerAdaptor: public QObject
{
    Q_OBJECT

    USE_LOG_UTIL

private:
    CDMBTunerFblockControl *m_DMBTunerControlContext;

public:
    CDMBTunerAdaptor(CMOSTManager *context);
    ~CDMBTunerAdaptor();

public Q_SLOTS:
    // Below methods are called by applications via qdbus
    bool SetSignalQuality(const uint dmbSignalQuality);
    bool SetTPEGInfo(const QDBusVariant &tpegInfo);
    bool SetDMBStauts(const uint dmbStauts, const uint currentBrowserListLength);

    // Invoked by sendTPEGData signal from DMB application via qdbus
    bool SetTPEGStatus(const QByteArray &tpegData, int length);
};

/** Class definition for SatRadio Adaptor */
class CSatRadioAdaptor: public QObject
{
    Q_OBJECT

private:
    CSatRadioFblockControl *m_SatRadioControlContext;

public:
    CSatRadioAdaptor(CMOSTManager *context);
    ~CSatRadioAdaptor();

public Q_SLOTS:
    // Below methods are called by applications via qdbus
    bool SetStationSelect(const uint optype, const uint station, const uint presetNo);
    bool SetServiceProvider(const uint optype, const uint serviceProvider);
    bool SetServiceStatus(const uint optype, const bool audio, const bool stationList, const uint subscription);
    bool SetTMCStatus(const uint optype, const uint tmcStatus);

private Q_SLOTS:
    void StationSelectSlot(const uchar type, const uchar station);
    void ServiceProviderSlot(const uchar serviceProvider);

signals:
    // Emit the below signals to application via qdbus
    void onStationSelect(const uint type, const uint station);
    void onServiceProvider(const uint serviceProvider);
};


typedef struct APP_STATE_T
{
    //uint appClock   :1;   // clock app is not used anymore, 20130830
    uint appStandby :1;
    uint appBTPhone :1;
    uint appSetting :1;
    uint appNavi    :1;

    uint appVR      :1;
    uint appiBox    :1;
    uint appEng     :1;
    uint appUpgrade :1;

    uint appUISH    :1;
    uint appDMB     :1;
    uint appXMAudio :1;
} APP_STATE;


class CAppStateAdaptor: public QObject
{
    Q_OBJECT
    USE_LOG_UTIL

    CMOSTManager *m_MostManager;

    APP_STATE m_AppState;
    QSharedMemory m_AppStateSharedMem;

    static const uint SR_FBLOCK    = 0x100;        // appUISH
    static const uint DLNA_SHADOW  = 0x100;        // appUISH
    static const uint AUXIN_FBLOCK = 0x100;        // appUISH
    static const uint BT_FBLOCK    = 0x102;        // appUISH, appBTPhone
    static const uint TELEPHONE_SHADOW = 0x112;    // appUISH, appVR, appBTPhone
    static const uint DIAG_FBLOCK  = 0x140;        // appUISH, appEng
    static const uint SWDL_FBLOCK  = 0x180;        // appUISH, appUpgrade
    //static const uint HMI_FBLOCK   = 0x121;        // appUISH, appiBox, appClock    // Do not wait for App Clock anymore.
    static const uint HMI_FBLOCK   = 0x121;        // appUISH, appiBox, appStandby
    static const uint NAVI_FBLOCK  = 0x108;        // appUISH, appNavi
    static const uint IBOX_SHADOW  = 0x130;        // appUISH, appiBox, appVR
    static const uint DMB_FBLOCK   = 0x300;        // appMobileTV, appUISH
    static const uint SATRADIO_FBLOCK  = 0x500;    // appXMAudio, appUISH

public:
    CAppStateAdaptor(CMOSTManager *context);
    ~CAppStateAdaptor();

    enum
    {
        APP_TERMINATED = 0,
        APP_RUNNING = 1
    };

    void RegisterFBlocktoServer(void);
    void UnRegisterFBlockfromServer(void);

    void RegisterAll(void);
    bool onAppStateChanged(const QByteArray appState);

public Q_SLOTS:
    bool onAppStateChanged(const QDBusMessage& msg);
    /** Below methods are called by UISH via D-BUS */
    //bool onAppStateChanged(const QByteArray appState);
};

#endif // DHAVN_MOSTMANAGER_ADAPTOR_H
