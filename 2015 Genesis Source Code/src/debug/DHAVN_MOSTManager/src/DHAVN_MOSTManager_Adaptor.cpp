/*
 ******************************************************************
 * COPYRIGHT © <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER :  Neelima Khedad,Venkatesh Aeturi                  *
 * DATE       :  28 July 2011                                     *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 * MODULE SUMMARY : MOST Manager is a middle ware component which *
 *                  talks to the applications and controller over *
 *                  QtDbus.The data sent over QtDbus is further   *
 *                  communicated to other MOST devices through ACS*
 *                  daemon.This module is also responsible for    *
 *                  commands from ACS daemon.                     *
 *                  This file contains the definition for all     *
 *                  Adaptor classes which exposes APIs and signals*
 *                  over QtDbus to applications and controllers.  *
 *                                                                *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 * 28 Jul 2011  Neelima Khedad           Draft version
 * 08 Aug 2011  Neelima Khedad           Added definitions for methods
 *                                       of class CDLNAAdaptor
 * 11 Aug 2011  Neelima Khedad           Removed '&' for arguments of function in CAmFmTunerAdaptor
 * 16 Aug 2011  Sajeer Shamsudeen        Removed '&' for arguments of function in CDVDPlayerAdaptor and
 *                                       seperated the debug code using debug flag
 * 16 Aug 2011  Venkatesh Aeturi         Added definitions for methods
 *                                       of class CDMBTuner adaptor
 * 22 Aug 2011  Neelima Khedad           Added definitions for methods
 *                                       of class CHMIAdaptor
 * 29 Aug 2011  Neelima Khedad           Added definition for methods of class CIBOXAdaptor
 *
 * 6 Sep  2011  Venkatesh Aeturi         Added definitions for methods CTelephoneAdaptor,CSpeechAdaptor
 *                                       CJukeBoxAdaptor,CVirtualDiscAdaptor,CVehicleAdaptor
 *                                       CBlueToothAdaptor.CSatelliteRadioAdaptor.
 * 06 Sep 2011  Shiva Kumar              Added definitions for methods
 *                                       of class SWDLAdaptor
 * 09 Sep 2011  Shiva kumar              Changed definition of functions SetReadModuleVersion,ReadModuleVersion,
 *                                       SetSWDLProgressInfo, SWDLProgressInfo and changed parameters of
 *                                       signals ReadModuleVersion, SetSWDLProgressInfo and slots ReadModuleVersion,
 *                                       SetSWDLProgressInfo in connect method of SWDLAdaptor constructor.
 * 12 Sep 2011  Amit Koparde		     Added definitions for methods of class CDABAdaptor.
 * 12 Sep 2011  Venkatesh Aeturi         Added definitions for methods of class CNavigationAdaptor.
 * 13 Sep 2011  Amit Koparde             Added definitions for methods of class CDiagnosisAdaptor.
 * 16 Sep 2011  Neelima Khedad           Added definition for new API SetNotiModeStatusHU to CHMIAdaptor
 * 16 Sep 2011  Amit Koparde             Added definition for new API SetPowerState to CVehicleAdaptor
 * 19 Sep 2011  Neelima Khedad           Added new parameter to ModeStatus of CHMIAdaptor
 * 19 Sep 2011  Shiva Kumar              Changed the parameters for API currentsonginfo and currentvideoinfo for DVD
 * 27 Sep 2011  Neelima Khedad           Removed the IsRunning API from all the adaptors
 * 27 Sep 2011  Amit Koparde             Added definition for new API SetCurrentSongInfo and SetTerminalMode to CAuxInAdaptor
 * 27 Sep 2011  Amit Koparde             Changed the parameters for API SetCurrentSongInfo of CBluetoothAdaptor and CVirtualDiscAdaptor
 * 12 Oct 2011  Neelima Khedad           Added API SetSystemVisibilityHUError to CHMIAdaptor
 * 12 Oct 2011  Neelima Khedad           Added API SetTrackposition to CDLNAAdaptor
 * 14 Oct 2011  Neelima Khedad           Added API parameters for CbluetoothAdaptor
 * 26 Oct 2011  Amit Koparde             Added new API's SetModemLevel, SetUserMode, SetCallHistoryCopyStatus and SetMissedCallStatus for Bluetooth Adaptor
 * 26 Oct 2011  Shiva Kumar              Added definition for new API SetRDSSetting, SetTMCStatus and SetTMCInfo
 * 27 Oct 2011  Neelima Khedad           Adedd API SetDisplayControl to HMI Adaptor
 * 27 Oct 2011  Neelima Khedad           Adedd parameter poicount to Signal POIDataAvailable
 * 27 Oct 2011  Neelima Khedad           Removed Signal NewSMS and added MissedCallStatus for Telephone
 * 28 Oct 2011  Shiva Kumar              Removed definitions for CVirtualDiscAdaptor class
 * 04 Nov 2011  Shiva Kumar              Added definition for API SetUpdateMedialist for CJukeBoxAdaptor class
 * 08 Feb 2011  Neelima Khedad           Using the latest Version of LOG_INIT function Logger module
 * 05 May 2012  Shiva Kumar              Added new signal DisplayOSD in HMI Adaptor
 * 06 Jun 2012  Shiva Kumar              Added new signals and APIs in DLNA and HMI Adaptor for FCAT 1.17
 * 21 Jun 2012  Shiva Kumar              Modified for FCAT 1.17.1, FCAT 1.17.2 and FCAT 1.17.3
 * 17 Jul 2012  Shiva Kumar              Modified for FCAT 1.18.2
 * 26 Jul 2012  Shiva Kumar              Modified for CR 12089, 12357
 * 19 Sep 2012  Shiva Kumar              Modified for FCAT 1.19.0, 1.20.1, 1.20.2
 * 15 Nov 2012  Shiva Kumar              Modified for CR15573
 * 27 Nov 2012  Shiva Kumar              Modified for CR 16038
 * 07 Dec 2012  Shiva Kumar              Modified for FCAT 1.22.1
 ******************************************************************
 */

#include "DHAVN_MOSTManager_Adaptor.h"
#include "DHAVN_AppFramework_Def.h"
#include "DHAVN_MOSTManager_MOSTManager.h"

#include "uistate_manager_def.h"

/*
 *******************************************************************
 * Function:CAuxInAdaptor
 *
 * Description :Is a constructor
 *
 * Parameters :
 *    context     rw   Is instance of CMOSTManager
 *
 * Returns : none
 *
 *******************************************************************
 */

CAuxInAdaptor::CAuxInAdaptor(CMOSTManager *context)
{
    m_CAuxInContext = context->GetAuxInControlInstance();
}

/*
 *******************************************************************
 * Function:~CAuxInAdaptor
 *
 * Description :Is a Destructor
 *
 * Parameters :none
 *
 * Returns :none
 *
 *******************************************************************
 */

CAuxInAdaptor::~CAuxInAdaptor()
{
}

/*
 *******************************************************************
 * Function: SetAuxDeviceInfo
 *
 * Description :Function called by the AuxIn controller to set the Device Information.
 *
 * Parameters :
 *      devicenum       rw   It holds the Auxin devicenum value. It is unsigned value.
 *      deviceclass     rw   It holds the Auxin deviceclass value. It is unsigned value.
 *      devicetype      rw   It holds the Auxin devicetype value. It is unsigned value.
 *      devicename      rw   It holds the Auxin  devicenamesas aa list. It is string list value.
 *      auxIn_no        rw   It holds the auxilary input number.
 *
 * Returns : true or false.
 *
 *******************************************************************
 */

bool CAuxInAdaptor::SetAuxDeviceInfo(const QDBusMessage& msg)
{
    bool result = false;
    uint devicenum, deviceclass,devicetype,auxIn_no;
    QString devicename;

    const QList<QVariant> & args = msg.arguments();
    devicenum = (uint) args.at(0).toInt();
    deviceclass = (uint) args.at(1).toInt();
    devicetype = (uint) args.at(2).toInt();
    devicename = (QString) args.at(3).toString();
    auxIn_no = (uint) args.at(4).toInt();

    if(m_CAuxInContext->FBlockVisible())
    {
        result = m_CAuxInContext->AuxDeviceInfo(devicenum, deviceclass, devicetype, devicename, auxIn_no);
    }

    return result;
}

/*
 *******************************************************************
 * Function:CBlueToothAdaptor
 *
 * Description :Is a constructor
 *
 * Parameters :
 *    context     rw   Is instance of CMOSTManager
 *
 * Returns : none
 *
 *******************************************************************
 */

CBlueToothAdaptor::CBlueToothAdaptor(CMOSTManager *context)
{
    m_CBluetoothControlContext = context->GetBluetoothControlInstance();

    /** Connect to the signals received from Bluetooth control */
    connect(m_CBluetoothControlContext,SIGNAL(StartHangupCall(uint,uint,uint)),this,SLOT(StartHangupCallSlot(uint,uint,uint)));
    connect(m_CBluetoothControlContext,SIGNAL(DialNumberRequestIBOX(uint,QString,uint)),this,SLOT(DialNumberRequestIBOXSlot(uint,QString,uint)));
    connect(m_CBluetoothControlContext,SIGNAL(UserMode(uint,bool)),this,SLOT(UserModeSlot(uint,bool)));
}

/*
 *******************************************************************
 * Function:~CBlueToothAdaptor
 *
 * Description :Is a Destructor
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */

CBlueToothAdaptor::~CBlueToothAdaptor()
{

}

/*
 *******************************************************************
 * Function:SetCallState
 *
 * Description :Function called by the Bluetooth controller to set the CallState.
 *
 * Parameters :
 *    pos          rw   It holds the contact pos by the user. It is unsigned word value.
 *
 *    callState    rw   It holds the callState set by the user. It is unsigned byte value.
 *
 *    callType     rw   It holds the callType set by the user. It is unsigned byte value.
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CBlueToothAdaptor::SetCallState(const QDBusVariant &callStatedata)
{
    bool result = false;

    if(m_CBluetoothControlContext->FBlockVisible())
    {
        result = m_CBluetoothControlContext->CallState(callStatedata);
    }

    return result;
}


/*
 *******************************************************************
 * Function:HangupCallResult
 *
 * Description :Function called by the Bluetooth controller to set the MissedCallStatus.
 *
 * Parameters :
 *    senderHandle     rw   It holds the senderHandle value.
 *    callId            rw   It holds the callId from IBOX.
 *    deviceID          rw   It holds the deviceID.
 * Returns : true or false
 *
 *******************************************************************
 */
bool CBlueToothAdaptor::HangupCallResult(const uint senderHandle, const uint callId, const uint deviceID)
{
    bool result = false;

    if(m_CBluetoothControlContext->FBlockVisible())
    {
        result = m_CBluetoothControlContext->HangupCallResult(senderHandle,callId,deviceID);
    }

    return result;
}
/*
 *******************************************************************
 * Function:DialNumberResultIBOX
 *
 * Description :Function called by the Bluetooth App to set the DialNumber.
 *
 * Parameters :
 *    senderHandle     rw   It holds the senderHandle value.
 *    callId            rw   It holds the callId from IBOX.
 *    deviceID          rw   It holds the deviceID.
 * Returns : true or false
 *
 *******************************************************************
 */
bool CBlueToothAdaptor::DialNumberResultIBOX(const uint state, const uint senderHandle, const uint callID, const uint deviceID)
{
    bool result = false;

    if(m_CBluetoothControlContext->FBlockVisible())
    {
        result = m_CBluetoothControlContext->DialNumberResultIBOX(state,senderHandle,callID,deviceID);
    }

    return result;
}
/*
 *******************************************************************
 * Function:SetBTStatus
 *
 * Description :Function called by the BTApp to set the BT mode status.
 *
 * Parameters :
 *    btMode     rw   It holds the BT mode value set by user.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CBlueToothAdaptor::SetBTStatus(const uint btMode)
{
    bool result = false;

    if(m_CBluetoothControlContext->FBlockVisible())
    {
        result = m_CBluetoothControlContext->SetBTStatus(btMode);
    }

    return result;
}
/*
 *******************************************************************
 * Function:SetUserMode
 *
 * Description :Function called by the BTApp to set the user mode status.
 *
 * Parameters :
 *    usermode     rw   It holds the value set by user for BT.
 *    micOff       rw   It holds the callId from IBOX.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CBlueToothAdaptor::SetUserMode(const uint usermode,const bool micOff)
{
    bool result = false;

    if(m_CBluetoothControlContext->FBlockVisible())
    {
        result = m_CBluetoothControlContext->SetUserMode(usermode,micOff);
    }

    return result;
}

/*
 *******************************************************************
 * Function:StartHangupCallSlot
 *
 * Description :Function invoked by the Bluetooth controller signal StartHangupCall.
 *
 * Parameters :
 *    senderHandle     rw   It holds the senderHandle value.
 *    callId            rw   It holds the callId from IBOX.
 *    deviceID          rw   It holds the deviceID.
 * Returns : true or false
 *
 *******************************************************************
 */
void CBlueToothAdaptor::StartHangupCallSlot(const uint senderHandle, const uint callId, const uint deviceID)
{
    emit StartHangupCall(senderHandle,callId,deviceID);
}
/*
 *******************************************************************
 * Function:DialNumberRequestIBOXSlot
 *
 * Description :Function invoked by the Bluetooth controller signal DialNumber.
 *
 * Parameters :
 *    senderHandle     rw   It holds the senderHandle value.
 *    telNumber        rw   It holds the telephone number to dial from IBOX.
 *    deviceID         rw   It holds the deviceID of source device.
 * Returns : true or false
 *
 *******************************************************************
 */
void CBlueToothAdaptor::DialNumberRequestIBOXSlot(const uint senderHandle, const QString telNumber,const uint deviceID)
{
    emit DialNumberRequestIBOX(senderHandle,telNumber,deviceID);
}
/*
 *******************************************************************
 * Function:UserModeSlot
 *
 * Description :Function invoked by the Bluetooth controller signal usermode.
 *
 * Parameters :
 *    usermode     rw   It holds the value set by user for BT.
 *    micOff       rw   It holds the callId from IBOX.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
void CBlueToothAdaptor::UserModeSlot(const uint usermode,const bool micOff)
{
    emit UserMode(usermode,micOff);
}

/*
 *******************************************************************
 * Function:CDiagnosisAdaptor
 *
 * Description :Is a constructor
 *
 * Parameters :
 *    context     rw   Is instance of CMOSTManager
 *
 * Returns : none
 *
 *******************************************************************
 */

CDiagnosisAdaptor::CDiagnosisAdaptor(CMOSTManager *context)
{
    m_DiagnosisFBlockShadowControl = context->GetDiagnosisFBlockShadowControlInstance();

    //connect(m_DiagnosisFBlockShadowControl,SIGNAL(MainSWVersionStatus(QStringList)),this,SLOT(MainSWVersionStatusSlot(QStringList)));
    connect(m_DiagnosisFBlockShadowControl,SIGNAL(MainSWVersionStatus(QStringList)),this,SLOT(MainSWVersionStatusSlot(QStringList)));
    connect(m_DiagnosisFBlockShadowControl,SIGNAL(GetVariantCodeStatus()),this,SLOT(GetVariantCodeStatusSlot()));
}

/*
 *******************************************************************
 * Function:~CDiagnosisAdaptor
 *
 * Description :Is a Destructor
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */

CDiagnosisAdaptor::~CDiagnosisAdaptor()
{

}

/*
 *******************************************************************
 * Function: GetMainSWVersion
 *
 * Description : Function called by applications to get
 *               the MainSWVersion via QDBUS.
 *
 * Parameters :
 *      none
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CDiagnosisAdaptor::GetMainSWVersion()
{

    return m_DiagnosisFBlockShadowControl->GetMainSWVersion();

}

/*
 *******************************************************************
 * Function: VariantCode
 *
 * Description : Function called by the Diagnosis controller to set the VariantCode.
 *
 * Parameters :
 *      variantcode   rw    It holds the information about the VariantCode as QList<uint>
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CDiagnosisAdaptor::SetVariantCode(const QDBusVariant &variantcode)
{
    QList<uint> list;
    variantcode.variant().value<QDBusArgument>() >> list;

    return m_DiagnosisFBlockShadowControl->VariantCode(list);
}

/*
 *******************************************************************
 * Function: MainSWVersionStatusHUSlot
 *
 * Description : Emits the signal MainSWVersionStatusIBOX.
 *
 * Parameters :
 *      none
 *
 * Emits : MainSWVersionStatus Signal
 *
 *******************************************************************
 */
void CDiagnosisAdaptor::MainSWVersionStatusSlot(const QStringList mainSWVersion)
{
    emit MainSWVersionStatus(mainSWVersion);
}

/*
 *******************************************************************
 * Function: MainSWVersionStatusIBOXSlot
 *
 * Description : Emits the signal MainSWVersionStatusIBOX.
 *
 * Parameters :
 *      none
 *
 * Emits : MainSWVersionStatus Signal
 *
 *******************************************************************
 */

/*
 *******************************************************************
 * Function: MainSWVersionStatusAMPSlot
 *
 * Description : Emits the signal MainSWVersionStatusAMP.
 *
 * Parameters :
 *      none
 *
 * Emits : MainSWVersionStatus Signal
 *
 *******************************************************************
 */

/*
 *******************************************************************
 * Function: GetVariantCodeSlot
 *
 * Description : Emits the signal GetVariantCode.
 *
 * Parameters :
 *      none
 *
 * Emits : GetVariantCode Signal
 *
 *******************************************************************
 */

void CDiagnosisAdaptor::GetVariantCodeStatusSlot()
{
    emit GetVariantCodeStatus();
}

/*
 *******************************************************************
 * Function:CDLNAAdaptor
 *
 * Description :Is a constructor
 *
 * Parameters :
 *    context     rw   Is instance of CMOSTManager
 *
 * Returns : none
 *
 *******************************************************************
 */

CDLNAAdaptor::CDLNAAdaptor(CMOSTManager *context)
{
   m_CDLNAShadowControlContext = context->GetDLNAShadowControlInstance();

   /** Connect to the signals received from DLNA Shadow contorl */
   connect(m_CDLNAShadowControlContext,SIGNAL(TimePosition(int,int,int)),this,SLOT(TimePositionSlot(int,int,int)));   
   connect(m_CDLNAShadowControlContext,SIGNAL(DLNAConnStatus(uint)),this,SLOT(DLNAConnStatusSlot(uint)));
   connect(m_CDLNAShadowControlContext,SIGNAL(CurrentSongInfo(uint,ushort,QStringList)),this,SLOT(CurrentSongInfoSlot(uint,ushort,QStringList)));
   connect(m_CDLNAShadowControlContext,SIGNAL(CurrentVideoInfo(QString,uint,ushort,QString)),this,SLOT(CurrentVideoInfoSlot(QString,uint,ushort,QString)));
   connect(m_CDLNAShadowControlContext,SIGNAL(Deckstatus(uint)),this,SLOT(DeckStatusSlot(uint)));
   connect(m_CDLNAShadowControlContext,SIGNAL(TrackPosition(uint)),this,SLOT(TrackPositionSlot(uint)));
   connect(m_CDLNAShadowControlContext,SIGNAL(TrackInformation(uint,uint)),this,SLOT(TrackInformationSlot(uint,uint)));
}

/*
 *******************************************************************
 * Function:~CDLNAAdaptor
 *
 * Description :Is a Destructor
 *
 * Parameters :none
 *
 * Returns : none
 *
 *******************************************************************
 */

CDLNAAdaptor::~CDLNAAdaptor()
{

}

/*
 *******************************************************************
 * Function:TimePositionSlot
 *
 * Description : Connected to signal TimePosition from CDLNAShadowControl.
 *               Sends the same signal back to app/controller
 * Parameters :
 *  diskTime     rw   Time details in milliseconds related to begin of disk.
 *  trackTime    rw   Time details in milliseconds related to begin of current track.
 *  titleTime    rw   Time details in milliseconds related to the start of the current title
 * Returns :     void
 *
 *******************************************************************
 */

void CDLNAAdaptor::TimePositionSlot(const int diskTime, const int trackTime, const int titleTime)
{
    emit TimePosition(diskTime,trackTime,titleTime);
}

/*
 *******************************************************************
 * Function:DLNAConnStatusSlot
 *
 * Description : Connected to signal DLNAConnStatus from CDLNAShadowControl.
 *               Sends the same signal back to app/controller
 *
 * Parameters :
 *  status         rw   DLNA connection information
 * Returns :        void
 *
 *******************************************************************
 */

void CDLNAAdaptor::DLNAConnStatusSlot(const uint status)
{   
    emit DLNAConnStatus(status);
}

/*
 *******************************************************************
 * Function:CurrentSongInfoSlot
 *
 * Description : Connected to signal CurrentSongInfo from CDLNAShadowControl.
 *               Sends the same signal back to app/controller
 *
 * Parameters :
 *  playtime     rw   Playing time in milliseconds
 *  trackno      rw   Number of the particular track
 *  songinfo[0] = title  :: Name of the song
 *  songinfo[1] = filename ::Necessary information for a file access.
 *  songinfo[2] = artist
 *  songinfo[3] = album
 *  songinfo[4] = genre
 * Returns :        void
 *
 *******************************************************************
 */
void CDLNAAdaptor::CurrentSongInfoSlot(const uint playtime, const ushort trackno, const QStringList songinfo)
{
    emit CurrentSongInfo(playtime,trackno,songinfo);
}

/*
 *******************************************************************
 * Function:CurrentVideoInfoSlot
 *
 * Description : Connected to signal CurrentVideoInfo from CDLNAShadowControl.
 *               Sends the same signal back to app/controller
 *
 * Parameters :
 *  vtitle      rw   Name of the video item.
 *  time        rw   Running time in milliseconds
 *  track       rw   Number of the particular track
 *  vfilename   rw   Necessary information for file access
 * Returns :        void
 *
 *******************************************************************
 */

void CDLNAAdaptor::CurrentVideoInfoSlot(const QString vtitle, const uint time, const ushort track, const QString vfilename)
{  
    emit CurrentVideoInfo(vtitle,time,track,vfilename);
}
/*
 *******************************************************************
 * Function:DeckStatusSlot
 *
 * Description : Connected to signal DeckStatus from CDLNAShadowControl.
 *               Sends the same signal back to app/controller
 *
 * Parameters :
 *    deckStatus  rw contains deckstatus information set by user
 *
 * Returns : void
 *
 *******************************************************************
 */
void CDLNAAdaptor::DeckStatusSlot(const uint deckStatus)
{
    emit DeckStatus(deckStatus);
}
/*
 *******************************************************************
 * Function:TrackPositionSlot
 *
 * Description : Connected to signal TrackPosition from CDLNAShadowControl.
 *               Sends the same signal back to app/controller
 *
 * Parameters :
 *   trackPosition rw   contains trackposition information for DLNA
 *
 * Returns :true or false
 *
 *******************************************************************
 */
void CDLNAAdaptor::TrackPositionSlot(const uint trackPosition)
{
    emit TrackPosition(trackPosition);
}
/*
 *******************************************************************
 * Function:TrackInformationSlot
 *
 * Description : Connected to signal TrackInformation from CDLNAShadowControl.
 *               Sends the same signal back to app/controller
 *
 * Parameters :
 *   currentNumTrack         rw   contains current track num information from DLNA
 *   currentRelativeTrackPos rw   contains relative track position info from DLNA
 *
 * Returns :true or false
 *
 *******************************************************************
 */
void CDLNAAdaptor::TrackInformationSlot(const uint currentNumTrack, const uint currentRelativeTrackPos)
{
    emit TrackInformation(currentNumTrack,currentRelativeTrackPos);
}

/*
 *******************************************************************
 * Function:SetDeckStatus
 *
 * Description : Sets deck status value to DLNA FBlock in IBOX
 *
 * Parameters :
 *    deckStatus  rw contains deckstatus information set by user
 *
 * Returns :true or false
 *
 *******************************************************************
 */
// Connect_ UISH DBus Call (i/f > message), 2014.2.27
bool CDLNAAdaptor::SetDeckStatus(const QDBusMessage& msg)
{
    bool result = false;
    uint deckStatus;

    const QList<QVariant> & args = msg.arguments();
    deckStatus = (uint) args.at(0).toInt();

    if(m_CDLNAShadowControlContext->FBlockVisible())
    {
        result = m_CDLNAShadowControlContext->SetDeckStatus(deckStatus);
    }

    return result;
}
/*
 *******************************************************************
 * Function:SetTrackPosition
 *
 * Description : Sets track position information to DLNA Fblock in IBOX
 *
 * Parameters :
 *   trackPosition rw   contains trackposition information for DLNA
 *
 * Returns :true or false
 *
 *******************************************************************
 */
// Connect_ UISH DBus Call (i/f > message), 2014.2.27
bool CDLNAAdaptor::SetTrackPosition(const QDBusMessage& msg)
{
    bool result = false;
    uint trackPosition;

    const QList<QVariant> & args = msg.arguments();
    trackPosition = (uint) args.at(0).toInt();

    if(m_CDLNAShadowControlContext->FBlockVisible())
    {
        result = m_CDLNAShadowControlContext->SetTrackPosition(trackPosition);
    }

    return result;
}
/*
 *******************************************************************
 * Function:SetIncrementTrackPosition
 *
 * Description : Increments track position information to DLNA Fblock in IBOX
 *
 * Parameters :
 *    nSteps   rw   contains nSteps information to be incremented by user
 *
 * Returns :true or false
 *
 *******************************************************************
 */
bool CDLNAAdaptor::SetIncrementTrackPosition(const uint nSteps)
{
    bool result = false;

    if(m_CDLNAShadowControlContext->FBlockVisible())
    {
        result = m_CDLNAShadowControlContext->SetIncrementTrackPosition(nSteps);
    }

    return result;
}
/*
 *******************************************************************
 * Function:SetDecrementTrackPosition
 *
 * Description : Decrements track position information to DLNA Fblock in IBOX
 *
 * Parameters :
 *    nSteps   rw   contains nSteps information to be decremented by user
 *
 * Returns :true or false
 *
 *******************************************************************
 */
bool CDLNAAdaptor::SetDecrementTrackPosition(const uint nSteps)
{
    bool result = false;

    if(m_CDLNAShadowControlContext->FBlockVisible())
    {
        result = m_CDLNAShadowControlContext->SetDecrementTrackPosition(nSteps);
    }

    return result;
}

/*
 *******************************************************************
 * Function:CHMIAdaptor
 *
 * Description :Is a constructor. Keeps connected to the Signals from CHMIFBlock_Shadow Control
 *
 * Parameters :
 *    context     rw   Is instance of CMOSTManager
 *
 * Returns : none
 *
 *******************************************************************
 */

CHMIAdaptor::CHMIAdaptor(CMOSTManager *context)
{
    m_HMIFBlockShadowControl = context->GetHMIFBlockShadowControl();

    /** Keep Connected to the signals */
    connect(m_HMIFBlockShadowControl,SIGNAL(ModeStatusIBOX(uint,uint)),this,SLOT(ModeStatusIBOXSlot(uint,uint)));

    connect(&m_HMIFBlockShadowControl->m_ModeStatusHU->FunctionInstance(),SIGNAL(ModeStatusfromIBOX(uint,uint)),this,SLOT(ModeStatusHUSlot(uint,uint)));

    connect(&m_HMIFBlockShadowControl->m_SystemAudibilityHU->FunctionInstance(),SIGNAL(SystemAudibilityfromIBOX(uint,uint)),this,SLOT(SystemAudibilityHUSlot(uint,uint)));
    connect(&m_HMIFBlockShadowControl->m_SystemVisibilityHU->FunctionInstance(),SIGNAL(SystemVisibilityfromIBOX(uint,uint)),this,SLOT(SystemVisibilityHUSlot(uint,uint)));

    connect(m_HMIFBlockShadowControl,SIGNAL(KeyBoardSetting(QDBusVariant)),this,SLOT(KeyBoardSettingSlot(QDBusVariant)));
    connect(m_HMIFBlockShadowControl,SIGNAL(Progressbar(uint)),this,SLOT(ProgressbarSlot(uint)));

    connect(m_HMIFBlockShadowControl,SIGNAL(PopUpResultFromIBOX(QDBusVariant)),this,SLOT(PopUpResultFromIBOXSlot(QDBusVariant)));
    connect(m_HMIFBlockShadowControl,SIGNAL(AbortPopUpRequestToHU(ushort)),this,SLOT(AbortPopUpRequestToHUSlot(ushort)));
    connect(m_HMIFBlockShadowControl,SIGNAL(PopUpRequestToHU(QDBusVariant)),this,SLOT(PopUpRequestToHUSlot(QDBusVariant)));
    connect(m_HMIFBlockShadowControl,SIGNAL(DisplayOSD(QDBusVariant)),this,SLOT(DisplayOSDSlot(QDBusVariant)));
    connect(m_HMIFBlockShadowControl,SIGNAL(ShowStatusBar(uint)),this,SLOT(ShowStatusBarSlot(uint)));
    connect(m_HMIFBlockShadowControl,SIGNAL(KeyInputStatus(uint, uint)),this,SLOT(KeyInputStatusSlot(uint, uint)));
    connect(m_HMIFBlockShadowControl,SIGNAL(DrivingRestrictionStatus(uint)),this,SLOT(DrivingRestrictionStatusSlot(uint)));
    connect(m_HMIFBlockShadowControl,SIGNAL(LCDBrightnessFromIBOX(int,int,int,int)),this,SLOT(LCDBrightnessFromIBOXSlot(int,int,int,int)));
    connect(m_HMIFBlockShadowControl,SIGNAL(AdjustScreenFromIBOX(int,int,int,int)),this,SLOT(AdjustScreenFromIBOXSlot(int,int,int,int)));
    connect(m_HMIFBlockShadowControl,SIGNAL(IlluminationFromIBOX(int)),this,SLOT(IlluminationFromIBOXSlot(int)));
    connect(m_HMIFBlockShadowControl,SIGNAL(VideoRatioFromIBOX(int)),this,SLOT(VideoRatioFromIBOXSlot(int)));
    connect(m_HMIFBlockShadowControl,SIGNAL(DisplayControlFromIBOX(uint)),this,SLOT(DisplayControlFromIBOXSlot(uint)));
    connect(m_HMIFBlockShadowControl,SIGNAL(VideoDefaultRequestIBOX(uint,ushort,uint)),this,SLOT(VideoDefaultRequestIBOXSlot(uint,ushort,uint)));
    connect(m_HMIFBlockShadowControl,SIGNAL(VideoDefaultResultIBOX(uint,uint)),this,SLOT(VideoDefaultResultIBOXSlot(uint,uint)));
    connect(m_HMIFBlockShadowControl,SIGNAL(BeepSettingFromIBOX(uint)),this,SLOT(BeepSettingFromIBOXSlot(uint)));
    connect(m_HMIFBlockShadowControl,SIGNAL(EmergencySetIBOX(uint,uint,uint)),this,SLOT(EmergencySetIBOXSlot(uint,uint,uint)));
    connect(m_HMIFBlockShadowControl,SIGNAL(ModeStatusErrorIBOX(uint,QByteArray)),this,SLOT(ModeStatusErrorIBOXSlot(uint,QByteArray)));

    connect(m_HMIFBlockShadowControl,SIGNAL(HMIFunctionBlockVisible(bool)),this,SLOT(HMIFunctionBlockVisibleSlot(bool)));

    // Fcat v13.07.1
    connect(m_HMIFBlockShadowControl,SIGNAL(SystemDefaultSetResult(uint,uint16_t,uint8_t,QByteArray)),this,SLOT(SystemDefaultSetResultSlot(uint,uint16_t,uint8_t,QByteArray)));

    // Fcat v13.07.2
    connect(m_HMIFBlockShadowControl,SIGNAL(MuteStatusSet(bool)),this,SLOT(MuteStatusSetSlot(bool)));
}

/*
 *******************************************************************
 * Function:~CHMIAdaptor
 *
 * Description :Is a Destructor
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */

CHMIAdaptor::~CHMIAdaptor()
{

}

/*
 *******************************************************************
 * Function:ModeStatusIBOXSlot
 *
 * Description :This slot is connected to signal ModeStatusIBOX
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *    target  rw     The target value for which mode needs to be changed
 *    mode    rw     The mdoe to be set fro teh target
 * Returns : none
 *
 *******************************************************************
 */

void CHMIAdaptor::ModeStatusIBOXSlot(const uint target, const uint mode)
{
    emit ModeStatusIBOX(target,mode);
}
/*
 *******************************************************************
 * Function:ModeStatusErrorIBOXSlot
 *
 * Description :This slot is connected to signal ModeStatusErrorIBOX
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *    errorCode  rw     The value for the type of error
 *
 * Returns : none
 *
 *******************************************************************
 */
void CHMIAdaptor::ModeStatusErrorIBOXSlot(const uint errorCode, const QByteArray errorInfo)
{
    emit ModeStatusErrorIBOX(errorCode, errorInfo);
}

/*
 *******************************************************************
 * Function:ModeStatusHUSlot
 *
 * Description :This slot is connected to signal ModeStatusHU
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *    target  rw     The target value for which mode needs to be changed
 *    mode    rw     The mode to be set for the target
 * Returns : none
 *
 *******************************************************************
 */

void CHMIAdaptor::ModeStatusHUSlot(const uint target, const uint mode)
{
    emit ModeStatusHU(target,mode);
}

/*
 *******************************************************************
 * Function:SystemAudibilityHUSlot
 *
 * Description :This slot is connected to signal SystemAudibilityHU
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *    aTarget    rw    The target value for which audio needs to be changed
 *    aSource    rw    The audio source to be set for the target
 * Returns : none
 *
 *******************************************************************
 */
void CHMIAdaptor::SystemAudibilityHUSlot(const uint aTarget, const uint aSource)
{
    emit SystemAudibilityHU(aTarget,aSource);
}

/*
 *******************************************************************
 * Function:SystemVisibilityHUSlot
 *
 * Description :This slot is connected to signal SystemVisibilityHU
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *    vTarget    rw    The target value for which video needs to be changed
 *    vSource    rw    The video source to be set for the target
 * Returns : none
 *
 *******************************************************************
 */

void CHMIAdaptor::SystemVisibilityHUSlot(const uint vTarget, const uint vSource)
{
    emit SystemVisibilityHU(vTarget,vSource);
}

/*
 *******************************************************************
 * Function:KeyBoardSettingSlot
 *
 * Description :This slot is connected to signal KeyBoard
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *    KeyBoardSettingSlot    rw    The keyboard setting from IBOX
 * Returns : none
 *
 *******************************************************************
 */

void CHMIAdaptor::KeyBoardSettingSlot(const QDBusVariant keyBoardData)
{
    emit KeyBoardSetting(keyBoardData);
}

/*
 *******************************************************************
 * Function:ProgressbarSlot
 *
 * Description :This slot is connected to signal Progressbar
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *    status    rw    The status of progress bar
 * Returns : none
 *
 *******************************************************************
 */

void CHMIAdaptor::ProgressbarSlot(const uint status)
{
    emit Progressbar(status);
}

/*
 *******************************************************************
 * Function:PopUpRequestToHUSlot
 *
 * Description :This slot is connected to signal PopUpRequestToHU
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *    popUpRequestData    rw    Contains structure to hold poprequest data
 * Returns : none
 *
 *******************************************************************
 */

void CHMIAdaptor::PopUpRequestToHUSlot(const QDBusVariant popUpRequestData)
{
    emit PopUpRequestToHU(popUpRequestData);
}

/*
 *******************************************************************
 * Function:AbortPopUpRequestToHUSlot
 *
 * Description :This slot is connected to signal AbortPopUpRequestToHU
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *    senderHandle    rw    SenderHandle value
 * Returns : none
 *
 *******************************************************************
 */

void CHMIAdaptor::AbortPopUpRequestToHUSlot(ushort senderHandle)
{
    emit AbortPopUpRequestToHU(senderHandle);
}


/*
 *******************************************************************
 * Function:PopUpResultFromIBOXSlot
 *
 * Description :This slot is connected to signal PopUpResultFromIBOX
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *    popUpRequestData    rw    Contains structure to hold popup result data
 * Returns : none
 *
 *******************************************************************
 */

void CHMIAdaptor::PopUpResultFromIBOXSlot(const QDBusVariant popUpResultData)
{
    emit PopUpResultFromIBOX(popUpResultData);
}

/*
 *******************************************************************
 * Function:DisplayOSDSlot
 *
 * Description :This slot is connected to signal DisplayOSD
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to HMI.
 * Parameters :
 *    displayOSD    rw    Contains structure to hold Display OSD data
 * Returns : none
 *
 *******************************************************************
 */
void CHMIAdaptor::DisplayOSDSlot(const QDBusVariant displayOSD)
{
    emit DisplayOSD(displayOSD);
}
/*
 *******************************************************************
 * Function:ShowStatusBarSlot
 *
 * Description :This slot is connected to signal ShowStatusBar
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to HMI.
 * Parameters :
 *    statusbar    rw    Contains data regarding statusbar show and hide status
 * Returns : none
 *
 *******************************************************************
 */
void CHMIAdaptor::ShowStatusBarSlot(const uint statusbar)
{
    emit ShowStatusBar(statusbar);
}
/*
 *******************************************************************
 * Function:KeyInputStatusSlot
 *
 * Description :This slot is connected to signal KeyInputStatus
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to HMI.
 * Parameters :
 *    keyInput    rw    contains data regarding the keyInput status
 * Returns : none
 *
 *******************************************************************
 */
void CHMIAdaptor::KeyInputStatusSlot(const uint hardKey, const uint softKey)
{
    emit KeyInputStatus(hardKey, softKey);
}
/*
 *******************************************************************
 * Function:DrivingRestrictionStatusSlot
 *
 * Description :This slot is connected to signal DrivingRestrictionStatus
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to HMI.
 * Parameters :
 *    need    rw    contains data regarding the DrivingRestriction status
 * Returns : none
 *
 *******************************************************************
 */
void CHMIAdaptor::DrivingRestrictionStatusSlot(const uint need)
{
    emit DrivingRestrictionStatus(need);
}
/*
 *******************************************************************
 * Function:AdjustScreenFromIBOXSlot
 *
 * Description :This slot is connected to signal AdjustScreen
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to HMI.
 * Parameters :For all below parameters
 *              The IBOX allows possible settings from -10...+10 in steps by 1 (21 levels),
 *              default setting 0.
 *      brightness    rw
 *      saturation    rw
 *      contrast      rw
 *      hue           rw
 * Returns : none
 *
 *******************************************************************
 */
void CHMIAdaptor::AdjustScreenFromIBOXSlot(const int brightness, const int saturation, const int contrast, const int hue)
{
    emit AdjustScreenFromIBOX(brightness,saturation,contrast,hue);
}
/*
 *******************************************************************
 * Function:LCDBrightnessFromIBOXSlot
 *
 * Description :This slot is connected to signal LCDBrightness
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to HMI.
 * Parameters :
 *  bTarget                     rw    bTarget holds the value of target display
 *  lcdFront,lcdRear1,lcdRaer2  rw    The HMI allows possible settings from -10...+10 in steps by 1 (21 levels),
 *                                    default setting 0.
 * Returns : none
 *
 *******************************************************************
 */
void CHMIAdaptor::LCDBrightnessFromIBOXSlot(const int bTarget, const int lcdFront, const int lcdRear1, const int lcdRear2)
{
    emit LCDBrightnessFromIBOX(bTarget,lcdFront,lcdRear1,lcdRear2);
}
/*
 *******************************************************************
 * Function:IlluminationFromIBOXSlot
 *
 * Description :This slot is connected to signal Illumination
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to HMI.
 * Parameters :
 *  illumination       rw        Illumination values
 * Returns : none
 *
 *******************************************************************
 */
void CHMIAdaptor::IlluminationFromIBOXSlot(const int illumination)
{
    emit IlluminationFromIBOX(illumination);
}
/*
 *******************************************************************
 * Function:VideoRatioFromIBOXSlot
 *
 * Description :This slot is connected to signal VideoRatio
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to HMI.
 * Parameters :
 *  ratio      rw       the ratio for which video should be adjusted
 * Returns : none
 *
 *******************************************************************
 */
void CHMIAdaptor::VideoRatioFromIBOXSlot(const int ratio)
{
    emit VideoRatioFromIBOX(ratio);
}
/*
 *******************************************************************
 * Function:DisplayControlFromIBOXSlot
 *
 * Description :This slot is connected to signal DisplayControl
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to HMI.
 * Parameters :
 *  displayStatus   rw  Display On/Off status
 * Returns : none
 *
 *******************************************************************
 */
void CHMIAdaptor::DisplayControlFromIBOXSlot(const uint displayStatus)
{
    emit DisplayControlFromIBOX(displayStatus);
}
/*
 *******************************************************************
 * Function:VideoDefaultRequestIBOXSlot
 *
 * Description :This slot is connected to signal VideoDefaultRequestIBOX
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to HMI.
 * Parameters :
 *  deviceID      rw     holds the source deviceID from which request has sent.
 *  sTarget       rw     holds the VideoDefaultSetting value for ibox
 *  senderHandle  rw     holds the senderHandle value of the request message.
 *
 * Returns : none
 *
 *******************************************************************
 */
void CHMIAdaptor::VideoDefaultRequestIBOXSlot(const uint deviceID,const ushort senderHandle, const uint sTarget)
{
    emit VideoDefaultRequestIBOX(deviceID, senderHandle, sTarget);
}
/*
 *******************************************************************
 * Function:VideoDefaultResultIBOXSlot
 *
 * Description :This slot is connected to signal VideoDefaultResultIBOX
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to HMI.
 * Parameters :
 *  state         rw     holds the state of result value to be sent to ibox
 *  senderHandle  rw     holds the senderHandle value of the request message.
 *
 * Returns : none
 *
 *******************************************************************
 */
void CHMIAdaptor::VideoDefaultResultIBOXSlot(const uint state, const uint senderHandle)
{
    emit VideoDefaultResultIBOX(state,senderHandle);
}
/*
 *******************************************************************
 * Function:BeepSettingFromIBOXSlot
 *
 * Description :This slot is connected to signal BeepSetting
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to HMI.
 * Parameters :
 *  beepSetting  rw   holds the beep on/off value from ibox.
 *
 * Returns : none
 *
 *******************************************************************
 */
void CHMIAdaptor::BeepSettingFromIBOXSlot(const uint beepSetting)
{
    emit BeepSettingFromIBOX(beepSetting);
}
/*
 *******************************************************************
 * Function:EmergencySetIBOXSlot
 *
 * Description :This slot is connected to signal EmergencySet
 *              from CHMIFBlock_ShadowControl.Sends the same signal over QtDbus
 *              to HMI.
 * Parameters :
 *  emgcState       rw   holds the type of emergency state.
 *  controllerLock  rw   holds the value to lock/unlock the controller.
 *  soundLock       rw   holds the value to mute/unmute the sound
 *
 * Returns : none
 *
 *******************************************************************
 */
void CHMIAdaptor::EmergencySetIBOXSlot(const uint emgcState, const uint controllerLock, const uint soundLock)
{
    emit EmergencySetIBOX(emgcState,controllerLock,soundLock);
}


void CHMIAdaptor::HMIFunctionBlockVisibleSlot(const bool state)
{
    emit HMIFunctionBlockVisible(state);
}

// Fcat v13.07.1
void CHMIAdaptor::SystemDefaultSetResultSlot(const uint state, const uint16_t senderHandle, const uint8_t errorCode, const QByteArray errorInfo)
{
    if(state == METHOD_ERRORACK)
    {
        emit SystemDefaultSetError(state, senderHandle, errorCode, errorInfo);
    }
    else    // METHOD_RESULTACK, METHOD_PROCESSINGACK
    {
        emit SystemDefaultSetResult(state, senderHandle);
    }
}

// Fcat v13.07.2
void CHMIAdaptor::MuteStatusSetSlot(const bool muteStatus)
{
    emit MuteStatusSet(muteStatus);
}


/*
 *******************************************************************
 * Function:SetLanguage
 *
 * Description :  API called over QtDbus by app/controller
 *                  which sends the Language info further to Fblock.
 * Parameters :
 *      systemLanguageCode  rw System language is in ISO 639-1 coding + Number
 * Returns : boolean value returned by function  SetLanguage of Control class
 *
 *******************************************************************
 */

bool CHMIAdaptor::SetLanguage(const QString systemLanguageCode)
{
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetLanguage(systemLanguageCode);
    }

    return result;
}
/*
 *******************************************************************
 * Function:SetVolumeStatus
 *
 * Description :  API called over QtDbus by app/controller
 *                  which sends the volume info further to Fblock.
 * Parameters :
 *      sourceType     rw source for whihc volume need to be set
 *      defaultVolume  rw Default volume level
 *      currentVolume  rw Current volume level
 * Returns : boolean value returned by function  SetVolumeStatus of Control class
 *
 *******************************************************************
 */

bool CHMIAdaptor::SetVolumeStatus(const uint sourceType, const uint defaultVolume, const uint currentVolume)
{
    bool result = false;

//    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    if(m_HMIFBlockShadowControl->FBlockVisible(IBOX_DEVICE_ADDRESS))		// 2014.11.4 hyunyong.sung
    {
        result = m_HMIFBlockShadowControl->SetVolumeStatus(sourceType,defaultVolume,currentVolume);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetTime
 *
 * Description :  API called over QtDbus by app/controller
 *                  which sends the time info further to Fblock.
 * Parameters :
 *      time     rw System time
 *      time[0]     Time Type
 *      time[1]     Current Hour
 *      time[2]     Current Min
 *      time[3]     Current Sec
 * Returns : boolean value returned by function  SetTime of Control class
 *
 *******************************************************************
 */

bool CHMIAdaptor::SetTime(const QByteArray time, const int timeZone, const uint dst)
{   
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetTime(time, timeZone, dst);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetDate
 *
 * Description :  API called over QtDbus by app/controller
 *                  which sends the date info further to Fblock.
 * Parameters :
 *      date     rw System Date
 *      date[0]     Current Year
 *      date[1]     Current Month
 *      date[2]     Current Date
 *      datetype    Format of date
 * Returns : boolean value returned by function  SetDate of Control class
 *
 *******************************************************************
 */

bool CHMIAdaptor::SetDate(const QDBusVariant systemDate)
{   
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetDate(systemDate);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetModeStatusIBOX
 *
 * Description :  API called over QtDbus by app/controller
 *                  which sends the Mode info further to Fblock.
 * Parameters :
 *      target  rw  Target
 *      mode    rw  Differnt modes
 * Returns : boolean value returned by function  SetModeStatusIBOX of Control class
 *
 *******************************************************************
 */
/*
bool CHMIAdaptor::SetModeStatusIBOX(const uint target, const uint mode)
{    
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(IBOX_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetModeStatusIBOX(target,mode);
    }

    return result;
}
*/
// Connect_ UISH DBus Call (i/f > message), 2014.2.27
bool CHMIAdaptor::SetModeStatusIBOX(const QDBusMessage& msg)
{    
    bool result = false;
    uint target, mode;
    
    const QList<QVariant> & args = msg.arguments();
    target = (uint) args.at(0).toInt();
    mode = (uint) args.at(1).toInt();

    if(m_HMIFBlockShadowControl->FBlockVisible(IBOX_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetModeStatusIBOX(target,mode);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetModeStatusHU
 *
 * Description :  API called over QtDbus by app/controller
 *                  which sends the Mode info further to Fblock.
 * Parameters :
 *      target  rw  Target
 *      mode    rw  Differnt modes
 * Returns : boolean value returned by function  SetModeStatusHU of Control class
 *
 *******************************************************************
 */
 /*
bool CHMIAdaptor::SetModeStatusHU(const uint target, const uint mode)
{
    bool result = false;

    // This API must be sychronous to shadow so that network connection has to be checked before updating.
    if( (GetAcsBridgeState() == false) && (GetAcsDaemonState() == false) )
    {
        return false;
    }

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetModeStatusHU(target,mode);
    }

    return result;
}
*/
// Connect_ UISH DBus Call (i/f > message), 2014.2.27
bool CHMIAdaptor::SetModeStatusHU(const QDBusMessage& msg)
{
    bool result = false;
    uint target, mode;
    
    // This API must be sychronous to shadow so that network connection has to be checked before updating.
    if( (GetAcsBridgeState() == false) && (GetAcsDaemonState() == false) )
    {
        return false;
    }
    
    const QList<QVariant> & args = msg.arguments();
    target = (uint) args.at(0).toInt();
    mode = (uint) args.at(1).toInt();

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetModeStatusHU(target,mode);
    }
    return result;
}


/*
 *******************************************************************
 * Function:SetSoundStatus
 *
 * Description :  API called over QtDbus by app/controller
 *                  which sends the Sound Status info further to Fblock.
 * Parameters :
 *      soundStatus  rw  Background sound
 * Returns : boolean value returned by function  SetSoundStatus of Control class
 *
 *******************************************************************
 */
/*
bool CHMIAdaptor::SetSoundStatus(const uint soundStatus)
{
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetSoundStatus(soundStatus);
    }

    return result;
}
*/
// Connect_ UISH DBus Call (i/f > message), 2014.2.27
bool CHMIAdaptor::SetSoundStatus(const QDBusMessage& msg)
{
    bool result = false;
    uint soundStatus;
    
    const QList<QVariant> & args = msg.arguments();
    soundStatus = (uint) args.at(0).toInt();
    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetSoundStatus(soundStatus);
    }

    return result;
}
/*
 *******************************************************************
 * Function:SetSystemVisibilityHU
 *
 * Description :  API called over QtDbus by app/controller
 *                  which sends the System Visibility info further to Fblock.
 * Parameters :
 *      vTarget    rw  Video Target
 *      vSource    rw  Video Source
 * Returns : boolean value returned by function  SetSystemVisibilityHU of Control class
 *
 *******************************************************************
 */
// Connect_ UISH DBus Call (i/f > message), 2014.2.27
bool CHMIAdaptor::SetSystemVisibilityHU(const QDBusMessage& msg)
{
    bool result = false;
    uint vTarget;
    QByteArray vSource;

    const QList<QVariant> & args = msg.arguments();
    vTarget = (uint) args.at(0).toInt();
    vSource = (QByteArray) args.at(1).toByteArray();
    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetSystemVisibilityHU(vTarget,vSource);
    }

    return result;
}
/*
bool CHMIAdaptor::SetSystemVisibilityHU(const QDBusMessage& msg const uint vTarget, const QByteArray vSource)
{
    bool result = false;
    uint vTarget;
    QByteArray vSource;

    const QList<QVariant> & args = msg.arguments();
    vTarget = (uint) args.at(0).toInt();
    vSource = (uint) args.at(1).toInt();

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetSystemVisibilityHU(vTarget,vSource);
    }

    return result;
}
*/
/*
 *******************************************************************
 * Function:SetSystemAudibilityHU
 *
 * Description :  API called over QtDbus by app/controller
 *                  which sends the System Audibility info further to Fblock.
 * Parameters :
 *      aTarget    rw  Audio Target
 *      aSource    rw  Audio Source
 * Returns : boolean value returned by function  SetSystemAudibilityHU of Control class
 *
 *******************************************************************
 */
// Connect_ UISH DBus Call (i/f > message), 2014.2.27
bool CHMIAdaptor::SetSystemAudibilityHU(const QDBusMessage& msg)
{
    bool result = false;
    uint aTarget, aSource;

    const QList<QVariant> & args = msg.arguments();
    aTarget = (uint) args.at(0).toInt();
    aSource = (uint) args.at(1).toInt();
    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetSystemAudibilityHU(aTarget,aSource);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetSystemVisibilityHUError
 *
 * Description :  API called over QtDbus by app/controller
 *                  which sends the System Visibility Error info further to Fblock.
 * Parameters :
 *      errorCode    rw  Error Code
 *      errorInfo    rw  Error Info
 * Returns : boolean value returned by function  SetSystemVisibilityHUError of Control class
 *
 *******************************************************************
 */
// Connect_ UISH DBus Call (i/f > message), 2014.2.27
bool CHMIAdaptor::SetSystemVisibilityHUError(const QDBusMessage& msg)
{
    bool result = false;
    uint errorCode;
    QString errorInfo;

    const QList<QVariant> & args = msg.arguments();
    errorCode = (uint) args.at(0).toInt();
    errorInfo = (QString) args.at(1).toString();
    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetSystemVisibilityHUError(errorCode,errorInfo);
    }

    return result;
}
// Connect_ UISH DBus Call (i/f > message), 2014.2.27
bool CHMIAdaptor::SetSystemAudibilityHUError(const QDBusMessage& msg)
{
    bool result = false;
    uint errorCode;
    QString errorInfo;

    const QList<QVariant> & args = msg.arguments();
    errorCode = (uint) args.at(0).toInt();
    errorInfo = (QString) args.at(1).toString();
    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetSystemAudibilityHUError(errorCode,errorInfo);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetScreen
 *
 * Description :  API called over QtDbus by app/controller
 *                  which sends the Screen setting info further to Fblock.
 * Parameters : For all below parameters
 *              The HMI allows possible settings from -10...+10 in steps by 1 (21 levels),
 *              default setting 0.
 *      brightness    rw
 *      saturation    rw
 *      contrast      rw
 *      hue           rw
 * Returns : boolean value returned by function  SetScreen of Control class
 *
 *******************************************************************
 */
bool CHMIAdaptor::SetScreen(const int brightness, const int saturation, const int contrast, const int hue)
{
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetScreen(brightness,saturation,contrast,hue);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetLCDBrightness
 *
 * Description :  API called over QtDbus by app/controller
 *                  which sends the LCd Brightness info further to Fblock.
 * Parameters :
 *  bTarget                     rw    bTarget holds the value of target display
 *  lcdFront,lcdRear1,lcdRaer2  rw    The HMI allows possible settings from -10...+10 in steps by 1 (21 levels),
 *                                    default setting 0.
 * Returns : boolean value returned by function  SetLCDBrightness of Control class
 *
 *******************************************************************
 */

bool CHMIAdaptor::SetLCDBrightness(const int bTarget,const int lcdFront, const int lcdRear1, const int lcdRear2)
{
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetLCDBrightness(bTarget,lcdFront,lcdRear1,lcdRear2);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetIllumination
 *
 * Description :  API called over QtDbus by app/controller
 *                  which sends the Illumination info further to Fblock.
 * Parameters :
 *  illumination rw  Illumination values
 * Returns : boolean value returned by function  SetIllumination of Control class
 *
 *******************************************************************
 */

bool CHMIAdaptor::SetIllumination(const int illumination)
{
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetIllumination(illumination);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetKeyBoard
 *
 * Description :  API called over QtDbus by app/controller
 *                  which sends the Key board info further to Fblock.
 * Parameters :
 *  keyboard rw  Type of keyboard to be used
 * Returns : boolean value returned by function  SetKeyBoard of Control class
 *
 *******************************************************************
 */

bool CHMIAdaptor::SetKeyBoard(const QDBusVariant keyBoardData)
{
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetKeyBoard(keyBoardData);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetVideoRatio
 *
 * Description :  API called over QtDbus by app/controller
 *                  which sends the Video ratio info further to Fblock.
 * Parameters :
 *  ratio rw  the ratio for which video should be adjusted
 * Returns : boolean value returned by function  SetVideoRatio of Control class
 *
 *******************************************************************
 */

bool CHMIAdaptor::SetVideoRatio(const int ratio)
{
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetVideoRatio(ratio);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetProgressbar
 *
 * Description :  API called over QtDbus by app/controller
 *                  which sends the PopUprequest progress bar info further to Fblock.
 * Parameters :
 *  ratio rw  the ratio for which video should be adjusted
 * Returns : boolean value returned by function  SetProgressbar of Control class
 *
 *******************************************************************
 */

bool CHMIAdaptor::SetProgressbar(const uint status)
{
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetProgressbar(status);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetDisplayControl
 *
 * Description :  API called over QtDbus by app/controller
 *                  which sends the display control status info further to Fblock.
 * Parameters :
 *  displayStatus   rw  Display On/Off status
 * Returns : boolean value returned by function  SetDisplayControl of Control class
 *
 *******************************************************************
 */

bool CHMIAdaptor::SetDisplayControl(const uint displayStatus)
{
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetDisplayControl(displayStatus);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetDistanceUnit
 *
 * Description :  API called over QtDbus by app/controller
 *                  which sends the distance unit info further to Fblock.
 * Parameters :
 *  distanceUnit         rw  Distance unit
 * Returns : boolean value returned by function  SetDistanceUnit of Control class
 *
 *******************************************************************
 */

bool CHMIAdaptor::SetDistanceUnit(const uint distanceUnit)
{
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetDistanceUnit(distanceUnit);
    }

    return result;
}


/*
 *******************************************************************
 * Function:SetPopUpRequestToIBOX
 *
 * Description :  API called over QtDbus by app/controller
 *                which sends the PopUprequest info further to Fblock.
 * Parameters :
 *  popUpRequestData rw  Structure whihc holds PopUprequest Data
 * Returns : boolean value returned by function  SetPopUpRequestToIBOX of Control class
 *
 *******************************************************************
 */

bool CHMIAdaptor::SetPopUpRequestToIBOX(const QDBusVariant popUpRequestData)
{    
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(IBOX_DEVICE_ADDRESS))
    {
        /** Retrive the structure form QDbusArgument */
        struct SPopUpRequestData requestData;
        popUpRequestData.variant().value<QDBusArgument>() >> requestData;

        result = m_HMIFBlockShadowControl->SetPopUpRequestToIBOX(requestData);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetPopUpResultFromHU
 *
 * Description :  API called over QtDbus by app/controller
 *                which sends the PopUpResult info further to Fblock.
 * Parameters :
 *  popUpResultData rw  Structure whihc holds PopUpResult Data
 * Returns : boolean value returned by function  SetPopUpResultFromHU of Control class
 *
 *******************************************************************
 */
/*
bool CHMIAdaptor::SetPopUpResultFromHU(const QDBusVariant popUpResultData)
{
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        // Retrive the structure form QDbusArgument
        struct SPopUpResultData resultData;
        popUpResultData.variant().value<QDBusArgument>() >> resultData;

        result = m_HMIFBlockShadowControl->SetPopUpResultFromHU(resultData);
    }

    return result;
}
*/
// Connect_ UISH DBus Call (i/f > message), 2014.3.18
bool CHMIAdaptor::SetPopUpResultFromHU(const QDBusMessage& msg)
{
    bool result = false;

    const QList<QVariant> & args = msg.arguments();
    struct SPopUpResultData resultData;

    QDBusVariant popUpResultData;

    popUpResultData = (QDBusVariant) args.at(0).value<QDBusVariant>();
    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        /** Retrive the structure form QDbusArgument */
        //struct SPopUpResultData resultData;
        popUpResultData.variant().value<QDBusArgument>() >> resultData;

        result = m_HMIFBlockShadowControl->SetPopUpResultFromHU(resultData);
    }

    return result;
}
/*
 *******************************************************************
 * Function:SetBeepSetting
 *
 * Description :  API called over QtDbus by app/controller
 *                which sends the BeepSetting info further to shadow.
 * Parameters :
 *  beepSetting   rw   It contains beepSetting information
 *
 * Returns : boolean value returned by function  SetPopUpResultFromHU of Control class
 *
 *******************************************************************
 */
bool CHMIAdaptor::SetBeepSetting(const uint beepSetting)
{
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetBeepSetting(beepSetting);
    }

    return result;
}
/*
 *******************************************************************
 * Function:SetKeyInputStatus
 *
 * Description :  API called over QtDbus by app/controller
 *                which sends the SetKeyInputStatus info further to shadow.
 * Parameters :
 *  keyInput   rw   It contains keyInput information to IBOX
 *
 * Returns : boolean value returned by function  SetPopUpResultFromHU of Control class
 *
 *******************************************************************
 */
bool CHMIAdaptor::SetKeyInputStatus(const uint hardKey, const uint softKey)
{
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(IBOX_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetKeyInputStatus(hardKey, softKey);
    }

    return result;
}
/*
 *******************************************************************
 * Function:SetVideoDefaultResultIBOX
 *
 * Description :  API called over QtDbus by app/controller
 *                which sends the VideoDefaultSet result further to shadow.
 * Parameters :
 *  state          rw    It contains type of result information to IBOX
 *  deviceID       rw    It contains device ID of the requested device.
 *  senderHandle   rw    It contains the senderHandle of the requ
 *
 * Returns : boolean value returned by function  SetPopUpResultFromHU of Control class
 *
 *******************************************************************
 */
bool CHMIAdaptor::SetVideoDefaultResultIBOX(const uint state, const uint deviceID, const uint senderHandle)
{
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetVideoDefaultResultIBOX(state, deviceID, (const ushort)senderHandle);
    }

    return result;
}
/*
 *******************************************************************
 * Function:SetVideoDefaulttoIBOX
 *
 * Description :  API called over QtDbus by app/controller
 *                which sets the VideoDefaultSet.
 * Parameters :
 *  sTarget        rw    It contains value of target which needs to be set to default value.
 *  senderHandle   rw    It contains the senderHandle of the requ
 *
 * Returns : boolean value returned by function  SetVideoDefaulttoIBOX of Control class
 *
 *******************************************************************
 */
bool CHMIAdaptor::SetVideoDefaulttoIBOX(const uint senderHandle, const uint sTarget)
{
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(IBOX_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetVideoDefaulttoIBOX(senderHandle, (const ushort)sTarget);
    }

    return result;
}
/*
 *******************************************************************
 * Function:SetEmergencySetIBOX
 *
 * Description :  API called over QtDbus by app/controller
 *                which sends the status message.
 * Parameters :
 *  emgcState       rw   holds the type of emergency state.
 *  controllerLock  rw   holds the value to lock/unlock the controller.
 *  soundLock       rw   holds the value to mute/unmute the sound
 *
 * Returns : boolean value returned by function  SetEmergencySetIBOX of Control class
 *
 *******************************************************************
 */
bool CHMIAdaptor::SetEmergencySetIBOX(const uint emgcState, const uint controllerLock, const uint soundLock)
{
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetEmergencySetIBOX(emgcState,controllerLock,soundLock);
    }

    return result;
}
/*
 *******************************************************************
 * Function:SetPhonePriority
 *
 * Description :  API called over QtDbus by app/controller
 *                which sends the status message.
 * Parameters :
 *  phonePriority       rw   holds the value which has priority for bluelink/bluetooth.
 *
 * Returns : boolean value returned by function  SetPhonePriority of Control class
 *
 *******************************************************************
 */
bool CHMIAdaptor::SetPhonePriority(const uint phonePriority)
{
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetPhonePriority(phonePriority);
    }

    return result;
}

//Fcat v13.06.3
bool CHMIAdaptor::SetDisplayOSDStatus(const uint osdId,  const uint osdStatus)
{
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetDisplayOSDStatus(osdId, osdStatus);
    }

    return result;
}

// Fcat v13.07.1
bool CHMIAdaptor::SetSystemDefaultSet(const uint senderHandle)
{
    bool result = false;

    if(m_HMIFBlockShadowControl->FBlockVisible(IBOX_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetSystemDefaultSet(senderHandle);
    }

    return result;
}

// Fcat v13.07.2
// Connect_ UISH DBus Call (i/f > message), 2014.2.27
bool CHMIAdaptor::SetMuteStatusStatus(const QDBusMessage& msg)
{
    bool result = false;
    bool muteStatus;

    const QList<QVariant> & args = msg.arguments();
    muteStatus = (bool) args.at(0).toBool();

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetMuteStatusStatus(muteStatus);
    }

    return result;
}
// Connect_ UISH DBus Call (i/f > message), 2014.2.27
bool CHMIAdaptor::SetMuteStatusError(const QDBusMessage& msg)
{
    bool result = false;
    uint errorCode;

    const QList<QVariant> & args = msg.arguments();
    errorCode = (uint) args.at(0).toInt();

    if(m_HMIFBlockShadowControl->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_HMIFBlockShadowControl->SetMuteStatusError(errorCode);
    }

    return result;
}

/*
 *******************************************************************
 * Function:CIBoxAdaptor
 *
 * Description :Is a constructor, keeps connectd to the signals from IBOXShadow Control
 *
 * Parameters :
 *    context     rw   Is instance of CMOSTManager
 *
 * Returns : none
 *
 *******************************************************************
 */

CIBoxAdaptor::CIBoxAdaptor(CMOSTManager *context)
{
    m_IBOXShadowControl = context->GetIBOXShadowControlInstance();

    connect(m_IBOXShadowControl,SIGNAL(WifiMode(uint,uint)),this,SLOT(WifiModeSlot(uint,uint)));
    connect(m_IBOXShadowControl,SIGNAL(AppInstallStatus(uint,uint)),this,SLOT(AppInstallStatusSlot(uint,uint)));
    connect(m_IBOXShadowControl,SIGNAL(GISDataAvailable(QString,uint)),this,SLOT(GISDataAvailableSlot(QString,uint)));
    connect(m_IBOXShadowControl,SIGNAL(GISDataAvailableError(uint,uint)),this,SLOT(GISDataAvailableErrorSlot(uint,uint)));
    connect(m_IBOXShadowControl,SIGNAL(OpenBrowserRequestResult(ushort,uint)),this,SLOT(OpenBrowserRequestResultSlot(ushort,uint)));
    connect(m_IBOXShadowControl,SIGNAL(GISRequestResult(QDBusVariant)),this, SLOT(GISRequestResultSlot(QDBusVariant)));
    connect(m_IBOXShadowControl,SIGNAL(GISDataRequestResult(QDBusVariant)),this, SLOT(GISDataRequestResultSlot(QDBusVariant)));
    connect(m_IBOXShadowControl,SIGNAL(POIDataAvailable(uint,uint,QString)),this, SLOT(POIDataAvailableSlot(uint,uint,QString)));
    connect(m_IBOXShadowControl,SIGNAL(ReceivedPOI(uint,uint)),this,SLOT(ReceivedPOISlot(uint,uint)));
    connect(m_IBOXShadowControl,SIGNAL(POIDataRequestResult(QDBusVariant)),this, SLOT(POIDataRequestResultSlot(QDBusVariant)));
    connect(m_IBOXShadowControl,SIGNAL(POFDataAvailable(uint,uint)),this, SLOT(POFDataAvailableSlot(uint,uint)));
    connect(m_IBOXShadowControl,SIGNAL(VoiceSearchOnlineStatus(uint,ushort,uint,uint)),this,SLOT(VoiceSearchOnlineStatusSlot(uint,ushort,uint,uint)));
    connect(m_IBOXShadowControl,SIGNAL(IBOXFunctionBlockInvisible(bool)),this,SLOT(IBOXFunctionBlockInvisibleSlot(bool)));
    connect(m_IBOXShadowControl,SIGNAL(LSDataAvailable(uchar,QString,uchar)),this,SLOT(LSDataAvailableSlot(uchar,QString,uchar)));
    connect(m_IBOXShadowControl,SIGNAL(LSDataAvailableError(uint,uint)),this,SLOT(LSDataAvailableErrorSlot(uint,uint)));
    connect(m_IBOXShadowControl,SIGNAL(LSRequest(uchar,ushort,uchar)),this,SLOT(LSRequestSlot(uchar,ushort,uchar)));
    connect(m_IBOXShadowControl,SIGNAL(LSDataRequest(uchar,ushort,uchar,QString,QByteArray)),this,SLOT(LSDataRequestSlot(uchar,ushort,uchar,QString,QByteArray)));
    connect(m_IBOXShadowControl,SIGNAL(ACNActivated(bool)),this,SLOT(ACNActivatedSlot(bool)));
}

/*
 *******************************************************************
 * Function:~CIBoxAdaptor
 *
 * Description :Is a Destructor
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */

CIBoxAdaptor::~CIBoxAdaptor()
{

}



/*
 *******************************************************************
 * Function:WifiModeSlot
 *
 * Description :This slot is connected to signal WifiMode
 *              from CIBOXShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *       mode  rw information about the mode of WiFi
 * Returns : none
 *
 *******************************************************************
 */

void CIBoxAdaptor::WifiModeSlot(const uint mode, const uint clientStatus)
{
    emit WifiMode(mode,clientStatus);
}


/*
 *******************************************************************
 * Function:AppInstallStatusSlot
 *
 * Description :This slot is connected to signal AppInstallStatus
 *              from CIBOXShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *     appStatus  rw application install status
 *     appProgress rw application progress
 * Returns : none
 *
 *******************************************************************
 */

void CIBoxAdaptor::AppInstallStatusSlot(const uint appStatus, const uint appProgress)
{
    emit AppInstallStatus(appStatus,appProgress);
}

/*
 *******************************************************************
 * Function:GISDataAvailableSlot
 *
 * Description :This slot is connected to signal GISDataAvailable
 *              from CIBOXShadowControl.Sends the same signal over QtDbus
 *              to Navi App.
 * Parameters :
 *    serviceID  rw   Service ID of the data to download
 *    available  rw   Indicates that GIS Data is available for download
 * Returns : none
 *
 *******************************************************************
 */

void CIBoxAdaptor::GISDataAvailableSlot(const QString uri, const uint available)
{
    emit GISDataAvailable(uri,available);
}
/*
 *******************************************************************
 * Function:GISDataAvailableErrorSlot
 *
 * Description :This slot is connected to signal GISDataAvailableError
 *              from CIBOXShadowControl.Sends the same signal over QtDbus
 *              to Navi App.
 * Parameters :
 *    serviceID  rw   Service ID of the data to download
 *    available  rw   Indicates that GIS Data is available for download
 * Returns : noneGISDataAvailableErrorSlot
 *
 *******************************************************************
 */
void CIBoxAdaptor::GISDataAvailableErrorSlot(const uint errorCode, const uint errorInfo)
{
    emit GISDataAvailableError(errorCode, errorInfo);
}

/*
 *******************************************************************
 * Function:OpenBrowserRequestResultSlot
 *
 * Description :This slot is connected to signal OpenBrowserRequestResult
 *              from CIBOXShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *    senderHandle  rw   handle to start the Open browser request
 *    state         rw   State of result
 * Returns : none
 *
 *******************************************************************
 */

void CIBoxAdaptor::OpenBrowserRequestResultSlot(const ushort senderHandle, const uint state)
{
    emit OpenBrowserRequestResult((const uint)senderHandle,state);
}

/*
 *******************************************************************
 * Function:GISRequestResultSlot
 *
 * Description :This slot is connected to signal GISRequestResult
 *              from CIBOXShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *    gisResultData  rw   Contains a structure about request Result
 * Returns : none
 *
 *******************************************************************
 */

void CIBoxAdaptor::GISRequestResultSlot(const QDBusVariant gisResultData)
{
    emit GISRequestResult(gisResultData);
}

/*
 *******************************************************************
 * Function:GISDataRequestResultSlot
 *
 * Description :This slot is connected to signal GISDataRequestResult
 *              from CIBOXShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *    gisDataResult  rw   Contains a structure about data result
 * Returns : none
 *
 *******************************************************************
 */

void CIBoxAdaptor::GISDataRequestResultSlot(const QDBusVariant gisDataResult)
{
    emit GISDataRequestResult(gisDataResult);
}

/*
 *******************************************************************
 * Function:POIDataAvailableSlot
 *
 * Description :This slot is connected to signal POIDataAvailable
 *              from CIBOXShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *    available  rw    POI Data information is available for download or not
 *
 * Returns : none
 *
 *******************************************************************
 */

void CIBoxAdaptor::POIDataAvailableSlot(const uint target, const uint available, const QString uri)
{
     emit POIDataAvailable(target, available, uri);
}
/*
 *******************************************************************
 * Function:ReceivedPOISlot
 *
 * Description :This slot is connected to signal ReceivedPOI
 *              from CIBOXShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *    available  rw    POI Data information is available for download or not
 *    poicount  rw     POI Data count
 * Returns : none
 *
 *******************************************************************
 */
void CIBoxAdaptor::ReceivedPOISlot(const uint available, const uint poiCount)
{
    emit ReceivedPOI(available,poiCount);
}

/*
 *******************************************************************
 * Function:POIDataRequestResultSlot
 *
 * Description :This slot is connected to signal POIDataRequestResult
 *              from CIBOXShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *    poiDataResult  rw     Contains a structure about POI request result
 * Returns : none
 *
 *******************************************************************
 */

void CIBoxAdaptor::POIDataRequestResultSlot(const QDBusVariant poiDataResult)
{
    emit POIDataRequestResult(poiDataResult);
}
/*
 *******************************************************************
 * Function:POFDataAvailableSlot
 *
 * Description :This slot is connected to signal POFDataAvailable
 *              from CIBOXShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *    available  rw    POF Data information is available for download or not
 *    pofcount  rw     POF Data count
 * Returns : none
 *
 *******************************************************************
 */

void CIBoxAdaptor::POFDataAvailableSlot(const uint available, const uint pofCount)
{
    emit POFDataAvailable(available,pofCount);
}
/*
 *******************************************************************
 * Function:VoiceSearchOnlineStatusSlot
 *
 * Description :This slot is connected to signal VoiceSearchOnline
 *              from CIBOXShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *    state         rw     holds the status of the message from IBOX
 *    senderHandle  rw     holds the senderHandle value requested for VoiceSearchOnline
 *    blockType     rw     holds the blocktype value sent for request
 *    blockCounter  rw     holds the blockCounter value sent for request.
 * Returns : none
 *
 *******************************************************************
 */
void CIBoxAdaptor::VoiceSearchOnlineStatusSlot(const uint state, const ushort senderHandle, const uint blockType, const uint blockCounter)
{
    emit VoiceSearchOnlineStatus(state, (const uint)senderHandle, blockType, blockCounter);
}
/*
 *******************************************************************
 * Function:IBOXFunctionBlockInvisibleSlot
 *
 * Description :This slot is connected to signal IBOXFunctionBlockInvisible
 *              from CIBOXShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *    status        rw     holds the status of ibox fblock visibility
 *
 * Returns : none
 *
 *******************************************************************
 */
void CIBoxAdaptor::IBOXFunctionBlockInvisibleSlot(const bool status)
{
    emit IBOXFunctionBlockInvisible(status);
}

void CIBoxAdaptor::LSDataAvailableSlot(const uchar optype, const QString uri, const uchar available)
{
    emit onLSDataAvailable((const uint)optype, uri, (const uint)available);
}

void CIBoxAdaptor::LSDataAvailableErrorSlot(const uint errorCode, const uint errorInfo)
{
    emit onLSDataAvailableError(errorCode, errorInfo);
}

void CIBoxAdaptor::LSRequestSlot(const uchar optype, const ushort senderHandle, const uchar success)
{
    emit onLSRequest((const uint)optype, (const uint)senderHandle, (const uint)success);
}

void CIBoxAdaptor::LSDataRequestSlot(const uchar optype, const ushort senderHandle, const uchar target, const QString uri, QByteArray lsdata)
{
    emit onLSDataRequest((const uint)optype, (const uint)senderHandle, (const uint)target, uri, lsdata);
}

/*
 *******************************************************************
 * Function:StartOpenBrowserRequest
 *
 * Description :  API called over QtDbus by UIState Handler
 *                which sends request to Open browser.
 * Parameters :
 *  senderhandle     rw  Handle to start the request
 *  url              rw  URL of the browser to open
 *  target           rw  contains target display information
 *  opentype         rw  contains information such as url/search word
 * Returns : boolean value returned by function  StartOpenBrowserRequest of Control class
 *
 *******************************************************************
 */
// Connect_ UISH DBus Call (i/f > message), 2014.2.27
bool CIBoxAdaptor::StartOpenBrowserRequest(const QDBusMessage& msg)
{
    bool result = false;
    uint senderhandle, target, opentype;
    QString text;

    const QList<QVariant> & args = msg.arguments();
    senderhandle = (uint) args.at(0).toInt();
    target = (uint) args.at(1).toInt();
    opentype = (uint) args.at(2).toInt();
    text = (QString) args.at(3).toString();
    if(m_IBOXShadowControl->FBlockVisible())
    {
        result = m_IBOXShadowControl->StartOpenBrowserRequest(senderhandle,target,opentype,text);
    }

    return result;
}

/*
 *******************************************************************
 * Function:StartGISRequest
 *
 * Description :  API called over QtDbus by UIState Handler
 *                which sends request to Start GIS Data download.
 * Parameters :
 *  gisRequestData rw  Holds the structure for GIS request
 * Returns : boolean value returned by function  StartGISRequest of Control class
 *
 *******************************************************************
 */

bool CIBoxAdaptor::StartGISRequest(const QDBusVariant gisRequestData)
{
    bool result = false;

    if(m_IBOXShadowControl->FBlockVisible())
    {
        /** Retrive the structure form QDbusArgument */
        SGISRequestData requestData;
        gisRequestData.variant().value<QDBusArgument>() >> requestData;

        result = m_IBOXShadowControl->StartGISRequest(requestData);
    }

    return result;
}

/*
 *******************************************************************
 * Function:StartGISDataRequest
 *
 * Description :  API called over QtDbus by UIState Handler
 *                which sends request to to fetch GIS data which is downloaded.
 * Parameters :
 *  gisRequestData rw  Holds the structure for GIS request
 * Returns : boolean value returned by function  StartGISDataRequest of Control class
 *
 *******************************************************************
 */

bool CIBoxAdaptor::StartGISDataRequest(const QDBusVariant gisRequestData)
{
    bool result = false;

    if(m_IBOXShadowControl->FBlockVisible())
    {
        /** Retrive the structure form QDbusArgument */
        SGISRequestData requestData;
        gisRequestData.variant().value<QDBusArgument>() >> requestData;

        result = m_IBOXShadowControl->StartGISDataRequest(requestData);
    }

    return result;
}

/*
 *******************************************************************
 * Function:StartPOIDataRequest
 *
 * Description :  API called over QtDbus by UIState Handler
 *                which sends request to to fetch POI data which is downloaded.
 * Parameters :
 *  poiDataRquest rw  Holds the structure for POI request
 * Returns : boolean value returned by function  StartPOIDataRequest of Control class
 *
 *******************************************************************
 */

bool CIBoxAdaptor::StartPOIDataRequest(const QDBusVariant poiDataRquest)
{
    bool result = false;

    if(m_IBOXShadowControl->FBlockVisible())
    {
        /** Retrive the structure form QDbusArgument */
        SPOIDataRequest requestData;
        poiDataRquest.variant().value<QDBusArgument>() >> requestData;

        result = m_IBOXShadowControl->StartPOIDataRequest(requestData);
    }

    return result;
}
/*
 *******************************************************************
 * Function:SetVoiceSearchOnline
 *
 * Description :  API called over QtDbus by VR App
 *                which sends request to for voicesearch to IBOX
 * Parameters :
 *  voiceSearch rw  Holds the structure for voicedata
 * Returns : boolean value returned by function  StartPOIDataRequest of Control class
 *
 *******************************************************************
 */
bool CIBoxAdaptor::SetVoiceSearchOnline(const uint voiceSearch)
{
    bool result = false;

    if(m_IBOXShadowControl->FBlockVisible())
    {
        result = m_IBOXShadowControl->SetVoiceSearchOnline(voiceSearch);
    }

    return result;
}

bool CIBoxAdaptor::GetWiFiMode(void)
{
    bool result = false;

    if(m_IBOXShadowControl->FBlockVisible())
    {
        m_IBOXShadowControl->WifiMode();
        result = true;
    }

    return result;
}

bool CIBoxAdaptor::SetLSRequest(const uint optype, const uint senderHandle, const QString uri, const QByteArray lsdata)
{
    bool result = false;

    if(m_IBOXShadowControl->FBlockVisible())
    {
        result = m_IBOXShadowControl->SendLSRequest((const uchar)optype, (const ushort)senderHandle, uri, lsdata);
    }

    return result;
}

bool CIBoxAdaptor::SetLSDataRequest(const uint optype, const uint senderHandle, const uint target, const QString uri)
{
    bool result = false;

    if(m_IBOXShadowControl->FBlockVisible())
    {
        result = m_IBOXShadowControl->SendLSDataRequest((const uchar)optype, (const ushort)senderHandle, (const uchar)target, uri);
    }

    return result;
}

/*
 *******************************************************************
 * Function:ACNActivatedSlot
 *
 * Description :This slot is connected to signal ACNActivated
 *              from CIBOXShadowControl.Sends the same signal over QtDbus
 *              to UI state handler.
 * Parameters :
 *       ACNActivated  rw information about the setting value of ACN Activated
 * Returns : none
 *
 *******************************************************************
 */

void CIBoxAdaptor::ACNActivatedSlot(const bool ACNActivatedValue)
{
    emit ACNActivated(ACNActivatedValue);
}


/*
 *******************************************************************
 * Function:CNavigationAdaptor
 *
 * Description :Is a constructor
 *
 * Parameters :
 *    context     rw   Is instance of CMOSTManager
 *
 * Returns : none
 *
 *******************************************************************
 */

CNavigationAdaptor::CNavigationAdaptor(CMOSTManager *context)
{
    m_NavigationControlContext = context->GetNavigationControlInstance();

    connect(m_NavigationControlContext,SIGNAL(RequestLocalSearch(uchar,ushort,QString)),this,SLOT(RequestLocalSearchSlot(uchar,ushort,QString)));
    connect(m_NavigationControlContext,SIGNAL(OpenLocalSearch(uchar,ushort,uchar,QString)),this,SLOT(OpenLocalSearchSlot(uchar,ushort,uchar,QString)));
    connect(m_NavigationControlContext,SIGNAL(SearchPOI(uchar,ushort,uchar,QString)),this,SLOT(SearchPOISlot(uchar,ushort,uchar,QString)));
    connect(m_NavigationControlContext,SIGNAL(SearchRoute(QDBusVariant)),this,SLOT(SearchRouteSlot(QDBusVariant)));
    connect(m_NavigationControlContext,SIGNAL(ShowMap(QDBusVariant)),this,SLOT(ShowMapSlot(QDBusVariant)));
    connect(m_NavigationControlContext,SIGNAL(RegisterMyLocation(QDBusVariant)),this,SLOT(RegisterMyLocationSlot(QDBusVariant)));
    connect(m_NavigationControlContext,SIGNAL(FindNearBy(ushort,uchar,uchar)),this,SLOT(FindNearBySlot(ushort,uchar,uchar)));
//    connect(m_NavigationControlContext,SIGNAL(CheckGPSBoundary(ushort,int,int)),this,SLOT(CheckGPSBoundarySlot(ushort,int,int)));	
    connect(m_NavigationControlContext,SIGNAL(CheckGPSBoundary(QDBusVariant)),this,SLOT(CheckGPSBoundarySlot(QDBusVariant)));	
}

/*
 *******************************************************************
 * Function:~CNavigationAdaptor
 *
 * Description :Is a Destructor
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */

CNavigationAdaptor::~CNavigationAdaptor()
{

}
/*
 *******************************************************************
 * Function:SetGPSRawData
 *
 * Description :Function called by the Navigation adaptor to set the GPS Raw Data.
 *
 * Parameters :
 *    gpsrawdata     rw   Is ByteArray that holds GPS Raw data.
 *
 * Returns :true or false
 *
 *******************************************************************
 */

bool CNavigationAdaptor::SetGPSRawData(const QDBusVariant &gpsRawData)
{
    bool result = false;

    if(m_NavigationControlContext->FBlockVisible())
    {
        result = m_NavigationControlContext->SetGPSRawData(gpsRawData);
    }

    return result;
}

/*
 *******************************************************************
 * Function:gpsrawdatabessel
 *
 * Description :Function called by the Navigation adaptor to set the GPS RawData Bessel.
 *
 * Parameters :
 *    gpsrawdatabessel     rw   Is ByteArray that holds GPS RawData Bessel.
 *
 * Returns :true or false
 *
 *******************************************************************
 */

bool CNavigationAdaptor::SetGPSRawDataBessel(const QDBusVariant &gpsRawDataBessel)
{
    bool result = false;

    if(m_NavigationControlContext->FBlockVisible())
    {
        result = m_NavigationControlContext->SetGPSRawDataBessel(gpsRawDataBessel);
    }

    return result;
}

bool CNavigationAdaptor::SetMapData(const uint dataAvailable, const uint mapDataType)
{
    bool result = false;

    if(m_NavigationControlContext->FBlockVisible())
    {
        result = m_NavigationControlContext->SetMapData(dataAvailable,mapDataType);
    }

    return result;
}

bool CNavigationAdaptor::SetRequestLocalSearch(const uint optype, const uint senderHandle)
{
    bool result = false;

    if(m_NavigationControlContext->FBlockVisible())
    {
        result = m_NavigationControlContext->SendRequestLocalSearch((const uchar)optype, (const ushort)senderHandle);
    }

    return result;
}

bool CNavigationAdaptor::SetOpenLocalSearch(const uint optype, const uint senderHandle)
{
    bool result = false;

    if(m_NavigationControlContext->FBlockVisible())
    {
        result = m_NavigationControlContext->SendOpenLocalSearch((const uchar)optype, (const ushort)senderHandle);
    }

    return result;
}

bool CNavigationAdaptor::SetSearchPOI(const uint optype, const uint senderHandle, const uint success)
{
    bool result = false;

    if(m_NavigationControlContext->FBlockVisible())
    {
        result = m_NavigationControlContext->SendSearchPOI((const uchar)optype, (const ushort)senderHandle, (const uchar)success);
    }

    return result;
}

bool CNavigationAdaptor::SetSearchRoute(const uint optype, const uint senderHandle, const uint success)
{
    bool result = false;

    if(m_NavigationControlContext->FBlockVisible())
    {
        result = m_NavigationControlContext->SendSearchRoute((const uchar)optype, (const ushort)senderHandle, (const uchar)success);
    }

    return result;
}

bool CNavigationAdaptor::SetShowMap(const uint optype, const uint senderHandle, const uint success)
{
    bool result = false;

    if(m_NavigationControlContext->FBlockVisible())
    {
        result = m_NavigationControlContext->SendShowMap((const uchar)optype, (const ushort)senderHandle, (const uchar)success);
    }

    return result;
}

bool CNavigationAdaptor::SetRegisterMyLocation(const uint optype, const uint senderHandle, const uint success)
{
    bool result = false;

    if(m_NavigationControlContext->FBlockVisible())
    {
        result = m_NavigationControlContext->SendRegisterMyLocation((const uchar)optype, (const ushort)senderHandle, (const uchar)success);
    }

    return result;
}

bool CNavigationAdaptor::SetDestinationInfo(const uint optype, const uint routeSet, const QDBusVariant &poiData)
{
    bool result = false;

    if(m_NavigationControlContext->FBlockVisible())
    {
        /** Retrive the structure form QDbusArgument */
        SPOIData _poiData;
        poiData.variant().value<QDBusArgument>() >> _poiData;

        result = m_NavigationControlContext->SendDestinationInfo((const uchar)optype, (const uchar)routeSet, _poiData);
    }

    return result;
}

bool CNavigationAdaptor::SetFindNearBy(const uint optype, const uint senderHandle, const uint success)
{
    bool result = false;

    if(m_NavigationControlContext->FBlockVisible())
    {
        result = m_NavigationControlContext->SendFindNearBy((const uchar)optype, (const ushort)senderHandle, (const uchar)success);
    }

    return result;
}

// FCat v15.3.5
bool CNavigationAdaptor::SetCheckGPSBoundary(const uint optype, const uint senderHandle, const uint gpsstate)
{
    bool result = false;

    if(m_NavigationControlContext->FBlockVisible())
    {
        result = m_NavigationControlContext->SendCheckGPSBoundary((const uchar)optype, (const ushort)senderHandle, (const uchar)gpsstate);
    }

    return result;
}



void CNavigationAdaptor::RequestLocalSearchSlot(const uchar optype, const ushort senderHandle, const QString inputKeyword)
{
    emit onRequestLocalSearch((const uint)optype, (const uint)senderHandle, inputKeyword);
}

void CNavigationAdaptor::OpenLocalSearchSlot(const uchar optype, const ushort senderHandle, const uchar target, const QString text)
{
    emit onOpenLocalSearch((const uint)optype, (const uint)senderHandle, (const uint)target, text);
}

void CNavigationAdaptor::SearchPOISlot(const uchar optype, const ushort senderHandle, const uchar target, const QString inputKeyword)
{
    emit onSearchPOI((const uint)optype, (const uint)senderHandle, (const uint)target, inputKeyword);
}

void CNavigationAdaptor::SearchRouteSlot(const QDBusVariant &searchRoute)
{
    emit onSearchRoute(searchRoute);
}

void CNavigationAdaptor::ShowMapSlot(const QDBusVariant &showMap)
{
    emit onShowMap(showMap);
}

void CNavigationAdaptor::RegisterMyLocationSlot(const QDBusVariant &registerMyLocation)
{
    emit onRegisterMyLocation(registerMyLocation);
}

void CNavigationAdaptor::FindNearBySlot(const ushort senderHandle, const uchar target, const uchar poiKind)
{
    emit onFindNearBySlot((const uint)senderHandle, (const uint)target, (const uint)poiKind);
}

void CNavigationAdaptor::CheckGPSBoundarySlot(const QDBusVariant &checkGPSBoundary)
{
    emit onCheckGPSBoundarySlot(checkGPSBoundary);
}




/*
 *******************************************************************
 * Function:CSWDLAdaptor
 *
 * Description :Is a constructor. Connects to the signals from SWDLFBlock control.
 *
 * Parameters :
 *    context     rw   Is instance of CMOSTManager
 *
 * Returns : none
 *
 *******************************************************************
 */

CSWDLAdaptor::CSWDLAdaptor(CMOSTManager *context)
{
    m_CSWDLFBlockControlContext = context->GetSWDLFBlockControlInstance();

    connect(m_CSWDLFBlockControlContext,SIGNAL(ReadHWIdentifier(uint,QString)),this,SLOT(ReadHWIdentifierSlot(uint,QString)));
    connect(m_CSWDLFBlockControlContext,SIGNAL(ReadDevName(uint,QString)),this,SLOT(ReadDevNameSlot(uint,QString)));
    connect(m_CSWDLFBlockControlContext,SIGNAL(ReadHWVersion(uint,QString)),this,SLOT(ReadHWVersionSlot(uint,QString)));
    connect(m_CSWDLFBlockControlContext,SIGNAL(ReadNumberofModule(uint,QByteArray)),this,SLOT(ReadNumberofModuleSlot(uint,QByteArray)));
    connect(m_CSWDLFBlockControlContext,SIGNAL(ReadModuleVersion(uint,QDBusVariant)),this,SLOT(ReadModuleVersionSlot(uint,QDBusVariant)));
    connect(m_CSWDLFBlockControlContext,SIGNAL(ReprogrammingMode(QDBusVariant)),this,SLOT(ReprogrammingModeSlot(QDBusVariant)));
    connect(m_CSWDLFBlockControlContext,SIGNAL(InitUpdateResult(QDBusVariant)),this,SLOT(InitUpdateResultSlot(QDBusVariant)));
    connect(m_CSWDLFBlockControlContext,SIGNAL(EraseMemory(QDBusVariant)),this,SLOT(EraseMemorySlot(QDBusVariant)));
    connect(m_CSWDLFBlockControlContext,SIGNAL(RequestDownload(QDBusVariant)),this,SLOT(RequestDownloadSlot(QDBusVariant)));
    connect(m_CSWDLFBlockControlContext,SIGNAL(PrepareFileTransfer(QDBusVariant)),this,SLOT(PrepareFileTransferSlot(QDBusVariant)));
    connect(m_CSWDLFBlockControlContext,SIGNAL(TransferDataAMS(QDBusVariant)),this,SLOT(TransferDataAMSSlot(QDBusVariant)));
    connect(m_CSWDLFBlockControlContext,SIGNAL(TransferDataAMSError(uint,uchar,QByteArray)),this,SLOT(TransferDataAMSErrorSlot(uint,uchar,QByteArray)));
    connect(m_CSWDLFBlockControlContext,SIGNAL(TransferDataCMS(QDBusVariant)),this,SLOT(TransferDataCMSSlot(QDBusVariant)));
    connect(m_CSWDLFBlockControlContext,SIGNAL(CheckActivateModule(QDBusVariant)),this,SLOT(CheckActivateModuleSlot(QDBusVariant)));
    connect(m_CSWDLFBlockControlContext,SIGNAL(RequestTransferExit(QDBusVariant)),this,SLOT(RequestTransferExitSlot(QDBusVariant)));
    connect(m_CSWDLFBlockControlContext,SIGNAL(SWDLProgressInfo(uint,uint,uint)),this,SLOT(SWDLProgressInfoSlot(uint,uint,uint)));
    connect(m_CSWDLFBlockControlContext,SIGNAL(UpdateProgressingInfoStatus(uint,uint)),this,SLOT(UpdateProgressingInfoStatusSlot(uint,uint)));

    // iBox SWDL FBlock checkling for iBox Update, 2014.2.24
    connect(m_CSWDLFBlockControlContext,SIGNAL(iBoxSWDLFBlockVisible(bool)),this,SLOT(iBoxSWDLFBlockVisibleSlot(bool)));
    
}

/*
 *******************************************************************
 * Function:~CSWDLAdaptor
 *
 * Description :Is a Destructor
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */

CSWDLAdaptor::~CSWDLAdaptor()
{

}

/*
 *******************************************************************
 * Function:SetReadHWIdentifier
 *
 * Description :API called over QtDbus by Software upgrade controller
 *              which sends the HWidentifier info further to SWDL FBlock shadow control.
 *
 * Parameters :
 *    swdlid         rw   It holds data which represents FBlock and Shadows of IBOX and Amplifier
 *                        in order to update the respective properties.
 *    hwidentifier   rw   Is a string of that contains hardware identifier of MOST devices.
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CSWDLAdaptor::SetReadHWIdentifier(const uint swdlid, const uint state, const QString hwidentifier)
{
    bool result = false;
    uint deviceID = 0;

    if(swdlid == SWDLID_HU)        deviceID = HU_DEVICE_ADDRESS;
    else if(swdlid == SWDLID_IBOX) deviceID = IBOX_DEVICE_ADDRESS;
    else if(swdlid == SWDLID_AMP)  deviceID = AMP_DEVICE_ADDRESS;
    else                           return false;

    if(m_CSWDLFBlockControlContext->FBlockVisible(deviceID))
    {
        result = m_CSWDLFBlockControlContext->SetReadHWIdentifier(swdlid,state,hwidentifier);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetReadDevName
 *
 * Description :API called over QtDbus by Software upgrade controller
 *              which sends the devicename info further to SWDL FBlock shadow control.
 *
 * Parameters :
 *    swdlid       rw   It holds data which represents FBlock and Shadows of IBOX and Amplifier
 *                      in order to update the respective properties.
 *    deviceName   rw   Is a string of that contains device name.
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CSWDLAdaptor::SetReadDevName(const uint swdlid, const QString deviceName)
{
    bool result = false;
    uint deviceID = 0;

    if(swdlid == SWDLID_HU)        deviceID = HU_DEVICE_ADDRESS;
    else if(swdlid == SWDLID_IBOX) deviceID = IBOX_DEVICE_ADDRESS;
    else if(swdlid == SWDLID_AMP)  deviceID = AMP_DEVICE_ADDRESS;
    else                           return false;

    if(m_CSWDLFBlockControlContext->FBlockVisible(deviceID))
    {
        result = m_CSWDLFBlockControlContext->SetReadDevName(swdlid,deviceName);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetReadHWVersion
 *
 * Description :API called over QtDbus by Software upgrade controller
 *              which sends the HWversion info further to SWDL FBlock shadow control
 *
 * Parameters :
 *    swdlid      rw   It holds data which represents FBlock and Shadows of IBOX and Amplifier
 *                      in order to update the respective properties.
 *    hwversion   rw   Is a string of that contains hardware version.
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CSWDLAdaptor::SetReadHWVersion(const uint swdlid, const QString hwversion)
{
    bool result = false;
    uint deviceID = 0;

    if(swdlid == SWDLID_HU)        deviceID = HU_DEVICE_ADDRESS;
    else if(swdlid == SWDLID_IBOX) deviceID = IBOX_DEVICE_ADDRESS;
    else if(swdlid == SWDLID_AMP)  deviceID = AMP_DEVICE_ADDRESS;
    else                           return false;

    if(m_CSWDLFBlockControlContext->FBlockVisible(deviceID))
    {
        result = m_CSWDLFBlockControlContext->SetReadHWVersion(swdlid,hwversion);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetReadNumberofModule
 *
 * Description :API called over QtDbus by Software upgrade controller
 *              which sends the number of modules info further to SWDL FBlock shadow control.
 *
 * Parameters :
 *    swdlid        rw   It holds data which represents FBlock and Shadows of IBOX and Amplifier
 *                       in order to update the respective properties.
 *    modulevalue   rw   Is a string of that contains code,data and boot value.
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CSWDLAdaptor::SetReadNumberofModule(const uint swdlid, const QByteArray modulevalue)
{
    bool result = false;
    uint deviceID = 0;

    if(swdlid == SWDLID_HU)        deviceID = HU_DEVICE_ADDRESS;
    else if(swdlid == SWDLID_IBOX) deviceID = IBOX_DEVICE_ADDRESS;
    else if(swdlid == SWDLID_AMP)  deviceID = AMP_DEVICE_ADDRESS;
    else                           return false;

    if(m_CSWDLFBlockControlContext->FBlockVisible(deviceID))
    {
        result = m_CSWDLFBlockControlContext->SetReadNumberofModule(swdlid,modulevalue);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetReadModuleVersion
 *
 * Description :API called over QtDbus by Software upgrade controller
 *              which sends the module version info further to SWDL FBlock shadow control.
 *
 * Parameters :
 *    swdlid          rw   It holds data which represents FBlock and Shadows of IBOX and Amplifier
 *                         in order to update the respective properties.
 *    moduleversion   rw   Is a string of that contains module version.
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CSWDLAdaptor::SetReadModuleVersion(const uint swdlid, const QDBusVariant &moduleversion)
{
    bool result = false;
    uint deviceID = 0;

    if(swdlid == SWDLID_HU)        deviceID = HU_DEVICE_ADDRESS;
    else if(swdlid == SWDLID_IBOX) deviceID = IBOX_DEVICE_ADDRESS;
    else if(swdlid == SWDLID_AMP)  deviceID = AMP_DEVICE_ADDRESS;
    else                           return false;

    if(m_CSWDLFBlockControlContext->FBlockVisible(deviceID))
    {
        result = m_CSWDLFBlockControlContext->SetReadModuleVersion(swdlid,moduleversion);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetSWDLProgressInfo
 *
 * Description :API called over QtDbus by Software upgrade controller
 *              which sends the SWDLProgressinfo further to SWDL FBlock shadow control.
 *
 * Parameters :
 *    swdlOpMode    rw   Is unsigned byte which holds the SWDL operation mode set by the user.
 *    swdlProgress  rw   Is unsigned byte which holds the progress status.
 *
 * Returns :true or false
 *
 *******************************************************************
 */

bool CSWDLAdaptor::SetSWDLProgressInfo(const uint swdlOpMode, const uint swdlProgress)
{
    bool result = false;

    if(m_CSWDLFBlockControlContext->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_CSWDLFBlockControlContext->SetSWDLProgressInfo(swdlOpMode,swdlProgress);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetReprogrammingMode
 *
 * Description :API called over QtDbus by Software upgrade controller
 *              which sends the Reprogramming mode further to SWDL FBlock shadow control
 *
 * Parameters :
 *    reprogrammingmode      rw   It is QDBusVariant datatype which holds the structure
 *                                SReporgrammingMode containing software download mode
 *                                (normal mode or reprogramming mode), software download id,
 *                                device id, sender handle.
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CSWDLAdaptor::SetReprogrammingMode(const QDBusVariant &reprogrammingmode)
{    
    bool result = false;
    uint deviceID = 0;

    /** Unpacking structure form arugument of type QDBusVariant  */
    struct SReporgrammingMode reprogrammingmode_wr;
    reprogrammingmode.variant().value<QDBusArgument>() >> reprogrammingmode_wr;

    if(reprogrammingmode_wr.swdlid == SWDLID_HU)        deviceID = HU_DEVICE_ADDRESS;
    else if(reprogrammingmode_wr.swdlid == SWDLID_IBOX) deviceID = IBOX_DEVICE_ADDRESS;
    else if(reprogrammingmode_wr.swdlid == SWDLID_AMP)  deviceID = AMP_DEVICE_ADDRESS;
    else                                                return false;

    if(m_CSWDLFBlockControlContext->FBlockVisible(deviceID))
    {
        result = m_CSWDLFBlockControlContext->SetReprogrammingMode(reprogrammingmode_wr);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetInitUpdateResult
 *
 * Description :API called over QtDbus by Software upgrade controller
 *              which sends the InitUpdate further to SWDL FBlock shadow control
 *
 * Parameters :
 *    initUpdate      rw   It is QDBusVariant datatype which holds the structure
 *                         SInitUpdate containing diskid, date of the update,
 *                         software download id, device id, sender handle.
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CSWDLAdaptor::SetInitUpdateResult(const QDBusVariant &initUpdate)
{
    bool result = false;
    uint deviceID = 0;

    /** Unpacking structure form arugument of type QDBusVariant  */
    struct SInitUpdate sinitupdate_wr;
    initUpdate.variant().value<QDBusArgument>() >> sinitupdate_wr;

    if(sinitupdate_wr.swdlid == SWDLID_HU)        deviceID = HU_DEVICE_ADDRESS;
    else if(sinitupdate_wr.swdlid == SWDLID_IBOX) deviceID = IBOX_DEVICE_ADDRESS;
    else if(sinitupdate_wr.swdlid == SWDLID_AMP)  deviceID = AMP_DEVICE_ADDRESS;
    else                                          return false;

    if(m_CSWDLFBlockControlContext->FBlockVisible(deviceID))
    {
        result = m_CSWDLFBlockControlContext->SetInitUpdateResult(sinitupdate_wr);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetEraseMemory
 *
 * Description :API called over QtDbus by Software upgrade controller
 *              which sends the EraseMemory further to SWDL FBlock shadow control
 *
 * Parameters :
 *    eraseMemory      rw   It is QDBusVariant datatype which holds the structure
 *                         SEraseMemory containing module type, module index
 *                         software download id, device id, sender handle.
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CSWDLAdaptor::SetEraseMemory(const QDBusVariant &eraseMemory)
{
    bool result = false;
    uint deviceID = 0;

    /** Unpacking structure form arugument of type QDBusVariant  */
    struct SEraseMemory serasememory_wr;
    eraseMemory.variant().value<QDBusArgument>() >> serasememory_wr;

    if(serasememory_wr.swdlid == SWDLID_HU)        deviceID = HU_DEVICE_ADDRESS;
    else if(serasememory_wr.swdlid == SWDLID_IBOX) deviceID = IBOX_DEVICE_ADDRESS;
    else if(serasememory_wr.swdlid == SWDLID_AMP)  deviceID = AMP_DEVICE_ADDRESS;
    else                                           return false;

    if(m_CSWDLFBlockControlContext->FBlockVisible(deviceID))
    {
        result = m_CSWDLFBlockControlContext->SetEraseMemory(serasememory_wr);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetRequestDownload
 *
 * Description :API called over QtDbus by Software upgrade controller
 *              which sends the Requestdownload further to SWDL FBlock shadow control
 *
 * Parameters :
 *    requestdownload     rw    It is QDBusVariant datatype which holds the structure
 *                              SRequestDownload containing blocksize and channel to be
 *                              used for transmission of data,
 *                              software download id, device id, sender handle.
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CSWDLAdaptor::SetRequestDownload(const QDBusVariant &requestdownload)
{
    bool result = false;
    uint deviceID = 0;

    /** Unpacking structure form arugument of type QDBusVariant  */
    struct SRequestDownload srequestdownload_wr;
    requestdownload.variant().value<QDBusArgument>() >> srequestdownload_wr;

    if(srequestdownload_wr.swdlid == SWDLID_HU)        deviceID = HU_DEVICE_ADDRESS;
    else if(srequestdownload_wr.swdlid == SWDLID_IBOX) deviceID = IBOX_DEVICE_ADDRESS;
    else if(srequestdownload_wr.swdlid == SWDLID_AMP)  deviceID = AMP_DEVICE_ADDRESS;
    else                                           return false;

    if(m_CSWDLFBlockControlContext->FBlockVisible(deviceID))
    {
        result = m_CSWDLFBlockControlContext->SetRequestDownload(srequestdownload_wr);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetPrepareFileTransfer
 *
 * Description :API called over QtDbus by Software upgrade controller
 *              which sends the PrepareFileTransfer further to SWDL FBlock shadow control
 *
 * Parameters :
 *    PrepareFileTransfer     rw    It is QDBusVariant datatype which holds the structure
 *                                  SPrepareFileTransfer containing file size, file name,
 *                                  software download id, device id, sender handle.
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CSWDLAdaptor::SetPrepareFileTransfer(const QDBusVariant &prepareFileTransfer)
{
    bool result = false;
    uint deviceID = 0;

    /** Unpacking structure form arugument of type QDBusVariant  */
    struct SPrepareFileTransfer spreparefiletransfer_wr;
    prepareFileTransfer.variant().value<QDBusArgument>() >> spreparefiletransfer_wr;

    if(spreparefiletransfer_wr.swdlid == SWDLID_HU)        deviceID = HU_DEVICE_ADDRESS;
    else if(spreparefiletransfer_wr.swdlid == SWDLID_IBOX) deviceID = IBOX_DEVICE_ADDRESS;
    else if(spreparefiletransfer_wr.swdlid == SWDLID_AMP)  deviceID = AMP_DEVICE_ADDRESS;
    else
        return false;

    if(m_CSWDLFBlockControlContext->FBlockVisible(deviceID))
    {
        result = m_CSWDLFBlockControlContext->SetPrepareFileTransfer(spreparefiletransfer_wr);
    }

    return result;
}

bool CSWDLAdaptor::SetPrepareFileTransferIBOX(const QDBusVariant &fileInfo)
{
    bool result = false;    

    /** Unpacking structure form arugument of type QDBusVariant  */
    struct SFileInfoforSWDL fileInfo_wr;
    fileInfo.variant().value<QDBusArgument>() >> fileInfo_wr;

    if(m_CSWDLFBlockControlContext->FBlockVisible(IBOX_DEVICE_ADDRESS))
    {
        result = m_CSWDLFBlockControlContext->SetPrepareFileTransferIBOX(fileInfo_wr);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetTransferDataCMS
 *
 * Description :API called over QtDbus by Software upgrade controller
 *              which sends the TransferDataCMS further to SWDL FBlock shadow control
 *
 * Parameters :
 *    transferdataCMS     rw    It is QDBusVariant datatype which holds the structure
 *                              STransferDataCMSAMS containing block counter and data,
 *                              software download id, device id, sender handle.
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CSWDLAdaptor::SetTransferDataCMS(const QDBusVariant &transferdataCMS)
{
    bool result = false;
    uint deviceID = 0;

    /** Unpacking structure form arugument of type QDBusVariant  */
    struct STransferDataCMSAMS stransferdataCMS_wr;
    transferdataCMS.variant().value<QDBusArgument>() >> stransferdataCMS_wr;

    if(stransferdataCMS_wr.swdlid == SWDLID_HU)        deviceID = HU_DEVICE_ADDRESS;
    else if(stransferdataCMS_wr.swdlid == SWDLID_IBOX) deviceID = IBOX_DEVICE_ADDRESS;
    else if(stransferdataCMS_wr.swdlid == SWDLID_AMP)  deviceID = AMP_DEVICE_ADDRESS;
    else                                           return false;

    if(m_CSWDLFBlockControlContext->FBlockVisible(deviceID))
    {
        result = m_CSWDLFBlockControlContext->SetTransferDataCMS(stransferdataCMS_wr);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetTransferDataAMS
 *
 * Description :API called over QtDbus by Software upgrade controller
 *              which sends the TransferDataAMS further to SWDL FBlock shadow control
 *
 * Parameters :
 *    transferdataAMS     rw    It is QDBusVariant datatype which holds the structure
 *                              STransferDataCMSAMS containing block counter and data,
 *                              software download id, device id, sender handle.
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CSWDLAdaptor::SetTransferDataAMS(const QDBusVariant &transferdataAMS)
{
    bool result = false;
    uint deviceID = 0;

    /** Unpacking structure form arugument of type QDBusVariant  */
    struct STransferDataCMSAMS stransferdataAMS_wr;
    transferdataAMS.variant().value<QDBusArgument>() >> stransferdataAMS_wr;

    if(stransferdataAMS_wr.swdlid == SWDLID_HU)        deviceID = HU_DEVICE_ADDRESS;
    else if(stransferdataAMS_wr.swdlid == SWDLID_IBOX) deviceID = IBOX_DEVICE_ADDRESS;
    else if(stransferdataAMS_wr.swdlid == SWDLID_AMP)  deviceID = AMP_DEVICE_ADDRESS;
    else                                           return false;

    if(m_CSWDLFBlockControlContext->FBlockVisible(deviceID))
    {
        result = m_CSWDLFBlockControlContext->SetTransferDataAMS(stransferdataAMS_wr);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetCheckActivateModule
 *
 * Description :API called over QtDbus by Software upgrade controller
 *              which sends the CheckActivateModule further to SWDL FBlock shadow control
 *
 * Parameters :
 *   checkactivatemodule     rw    It is QDBusVariant datatype which holds the structure
 *                                  SCheckActivateModule containing checksum,
 *                                  software download id, device id, sender handle.
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CSWDLAdaptor::SetCheckActivateModule(const QDBusVariant &checkactivatemodule)
{
    bool result = false;
    uint deviceID = 0;

    /** Unpacking structure form arugument of type QDBusVariant  */
    struct SCheckActivateModule checkactivationmodule_wr;
    checkactivatemodule.variant().value<QDBusArgument>() >> checkactivationmodule_wr;

    if(checkactivationmodule_wr.swdlid == SWDLID_HU)        deviceID = HU_DEVICE_ADDRESS;
    else if(checkactivationmodule_wr.swdlid == SWDLID_IBOX) deviceID = IBOX_DEVICE_ADDRESS;
    else if(checkactivationmodule_wr.swdlid == SWDLID_AMP)  deviceID = AMP_DEVICE_ADDRESS;
    else                                           return false;

    if(m_CSWDLFBlockControlContext->FBlockVisible(deviceID))
    {
        result = m_CSWDLFBlockControlContext->SetCheckActivateModule(checkactivationmodule_wr);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetRequestTransferExit
 *
 * Description :API called over QtDbus by Software upgrade controller
 *              which sends the RequestTransferExit further to SWDL FBlock shadow control
 *
 * Parameters :
 *    requesttransferexit   rw    It is QDBusVariant datatype which holds the structure
 *                                SRequestTransferExit software download id, device id, sender handle.
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CSWDLAdaptor::SetRequestTransferExit(const QDBusVariant &requesttransferexit)
{
    bool result = false;
    uint deviceID = 0;

    /** Unpacking structure form arugument of type QDBusVariant  */
    struct SRequestTransferExit requesttransferexit_wr;
    requesttransferexit.variant().value<QDBusArgument>() >> requesttransferexit_wr;

    if(requesttransferexit_wr.swdlid == SWDLID_HU)        deviceID = HU_DEVICE_ADDRESS;
    else if(requesttransferexit_wr.swdlid == SWDLID_IBOX) deviceID = IBOX_DEVICE_ADDRESS;
    else if(requesttransferexit_wr.swdlid == SWDLID_AMP)  deviceID = AMP_DEVICE_ADDRESS;
    else                                           return false;

    if(m_CSWDLFBlockControlContext->FBlockVisible(deviceID))
    {
        result = m_CSWDLFBlockControlContext->SetRequestTransferExit(requesttransferexit_wr);
    }

    return result;
}
/*
 *******************************************************************
 * Function:SetUpdateProgressingInfo
 *
 * Description :API called over QtDbus by Software upgrade controller
 *              which sends the UpdateProgressingInfo further to SWDL FBlock shadow control
 *
 * Parameters :
 *    updateStatus   rw    It holds the status of update progress info to IBOX and AMP
 *
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CSWDLAdaptor::SetUpdateProgressingInfo(const uint updateStatus)
{
    bool result = false;

    if(m_CSWDLFBlockControlContext->FBlockVisible(HU_DEVICE_ADDRESS))
    {
        result = m_CSWDLFBlockControlContext->SetUpdateProgressingInfo(updateStatus);
    }

    return result;
}

/*
 *******************************************************************
 * Function:ReadHWIdentifierSlot
 *
 * Description : Invoked by the listener function for hwidentifier To HU from IBOX or Amplifier.
 *               Sends the signal to SWDLAdaptor
 * Parameters :
 *    swdlid         rw   It holds data which represents FBlock and Shadows of IBOX and Amplifier
 *                        in order to update the respective properties.
 *    hwidentifier   rw   Is a string of that contains hardware identifier of MOST devices.

 * Returns :None
 *
 *******************************************************************
 */

void CSWDLAdaptor::ReadHWIdentifierSlot(const uint swdlid, const QString hwidentifier)
{
    emit ReadHWIdentifier(swdlid,hwidentifier);
}

/*
 *******************************************************************
 * Function:ReadDevNameSlot
 *
 * Description : Invoked by the listener function for devicename To HU from IBOX or Amplifier.
 *               Sends the signal to SWDLAdaptor
 * Parameters :
 *    swdlid       rw   It holds data which represents FBlock and Shadows of IBOX and Amplifier
 *                      in order to update the respective properties.
 *    deviceName   rw   Is a string of that contains device name.
 * Returns :None
 *
 *******************************************************************
 */

void CSWDLAdaptor::ReadDevNameSlot(const uint swdlid, const QString deviceName)
{
    emit ReadDevName(swdlid,deviceName);
}

/*
 *******************************************************************
 * Function:ReadHWVersionSlot
 *
 * Description : Invoked by the listener function for hwversion To HU from IBOX or Amplifier.
 *               Sends the signal to SWDLAdaptor
 * Parameters :
 *    swdlid      rw   It holds data which represents FBlock and Shadows of IBOX and Amplifier
 *                      in order to update the respective properties.
 *    hwversion   rw   Is a string of that contains hardware version.
 *
 * Returns :None
 *
 *******************************************************************
 */

void CSWDLAdaptor::ReadHWVersionSlot(const uint swdlid, const QString hwversion)
{
    emit ReadHWVersion(swdlid,hwversion);
}

/*
 *******************************************************************
 * Function:ReadNumberofModuleSlot
 *
 * Description : Invoked by the listener function for number of modules To HU from IBOX or Amplifier.
 *               Sends the signal to SWDLAdaptor
 * Parameters :
 *    swdlid      rw   It holds data which represents FBlock and Shadows of IBOX and Amplifier
 *                      in order to update the respective properties.
 *    modulevalue   rw   Is a string of that contains code,data and boot value.

 * Returns :None
 *
 *******************************************************************
 */

void CSWDLAdaptor::ReadNumberofModuleSlot(const uint swdlid, const QByteArray modulevalue)
{
    emit ReadNumberofModule(swdlid,modulevalue);
}

/*
 *******************************************************************
 * Function:ReadModuleVersionSlot
 *
 * Description : Invoked by the listener function for module version To HU from IBOX or Amplifier.
 *               Sends the signal to SWDLAdaptor
 * Parameters :
 *    swdlid          rw   It holds data which represents FBlock and Shadows of IBOX and Amplifier
 *                         in order to update the respective properties.
 *    moduleversion   rw   Is a string of that contains module version.
 *
 * Returns :None
 *
 *******************************************************************
 */

void CSWDLAdaptor::ReadModuleVersionSlot(const uint swdlid, const QDBusVariant &moduleversion)
{
    emit ReadModuleVersion(swdlid,moduleversion);
}

/*
 *******************************************************************
 * Function:ReprogrammingModeSlot
 *
 * Description : Invoked by the handler function for reprogramming mode To HU from IBOX or Amplifier.
 *               Sends the signal to SWDLAdaptor
 * Parameters :
 *    reprogrammingmode      rw   It is QDBusVariant datatype which holds the structure
 *                                SReporgrammingMode containing software download mode
 *                                (normal mode or reprogramming mode), software download id,
 *                                device id, sender handle.
 * Returns :None
 *
 *******************************************************************
 */

void CSWDLAdaptor::ReprogrammingModeSlot(const QDBusVariant &reprogrammingmode)
{
    emit ReprogrammingMode(reprogrammingmode);
}

/*
 *******************************************************************
 * Function:InitUpdateResultSlot
 *
 * Description : Invoked by the handler function for InitUpdate To HU from IBOX or Amplifier.
 *               Sends the signal to SWDLAdaptor
 * Parameters :
 *    initUpdate      rw   It is QDBusVariant datatype which holds the structure
 *                         SInitUpdate containing diskid, date of the update,
 *                         software download id, device id, sender handle.
 *
 * Returns :None
 *
 *******************************************************************
 */

void CSWDLAdaptor::InitUpdateResultSlot(const QDBusVariant &initUpdate)
{
    emit InitUpdateResult(initUpdate);
}

/*
 *******************************************************************
 * Function:EraseMemorySlot
 *
 * Description : Invoked by the handler function for EraseMemory To HU from IBOX or Amplifier.
 *               Sends the signal to SWDLAdaptor
 * Parameters :
 *    eraseMemory      rw   It is QDBusVariant datatype which holds the structure
 *                         SEraseMemory containing module type, module index
 *                         software download id, device id, sender handle.
 *
 * Returns :None
 *
 *******************************************************************
 */

void CSWDLAdaptor::EraseMemorySlot(const QDBusVariant &eraseMemory)
{
    emit EraseMemory(eraseMemory);
}

/*
 *******************************************************************
 * Function:RequestDownloadSlot
 *
 * Description : Invoked by the handler function for RequestDownload To HU from IBOX or Amplifier.
 *               Sends the signal to SWDLAdaptor
 * Parameters :
 *    requestdownload     rw    It is QDBusVariant datatype which holds the structure
 *                              SRequestDownload containing blocksize and channel to be
 *                              used for transmission of data,
 *                              software download id, device id, sender handle.
 *
 * Returns :None
 *
 *******************************************************************
 */

void CSWDLAdaptor::RequestDownloadSlot(const QDBusVariant &requestdownload)
{
    emit RequestDownload(requestdownload);
}

/*
 *******************************************************************
 * Function:PrepareFileTransferSlot
 *
 * Description : Invoked by the handler function for PrepareFileTransfer To HU from IBOX or Amplifier.
 *               Sends the signal to SWDLAdaptor
 * Parameters :
 *    PrepareFileTransfer     rw    It is QDBusVariant datatype which holds the structure
 *                                  SPrepareFileTransfer containing file size, file name,
 *                                  software download id, device id, sender handle.
 *
 * Returns :None
 *
 *******************************************************************
 */

void CSWDLAdaptor::PrepareFileTransferSlot(const QDBusVariant &preparefiletransfer)
{
    emit PrepareFileTransfer(preparefiletransfer);
}

/*
 *******************************************************************
 * Function:TransferDataCMSSlot
 *
 * Description : Invoked by the handler function for TransferDataCMS To HU from IBOX or Amplifier.
 *               Sends the signal to SWDLAdaptor
 * Parameters :
 *    transferdataCMS     rw    It is QDBusVariant datatype which holds the structure
 *                              STransferDataCMSAMS containing block counter and data,
 *                              software download id, device id, sender handle.
 *
 * Returns :None
 *
 *******************************************************************
 */

void CSWDLAdaptor::TransferDataCMSSlot(const QDBusVariant &transferdataCMS)
{
    emit TransferDataCMS(transferdataCMS);
}

/*
 *******************************************************************
 * Function:TransferDataAMSSlot
 *
 * Description : Invoked by the handler function for TransferDataAMS To HU from IBOX or Amplifier.
 *               Sends the signal to SWDLAdaptor
 * Parameters :
 *    transferdataAMS     rw    It is QDBusVariant datatype which holds the structure
 *                              STransferDataCMSAMS containing block counter and data,
 *                              software download id, device id, sender handle.
 * Returns :None
 *
 *******************************************************************
 */

void CSWDLAdaptor::TransferDataAMSSlot(const QDBusVariant &transferdataAMS)
{
    emit TransferDataAMS(transferdataAMS);
}

void CSWDLAdaptor::TransferDataAMSErrorSlot(const uint swdlid, const uchar errorCode, const QByteArray errorInfo)
{
    emit TransferDataAMSError(swdlid, errorCode, errorInfo);
}

/*
 *******************************************************************
 * Function:CheckActivateModuleSlot
 *
 * Description : Invoked by the handler function for checkActivatemodule To HU from IBOX or Amplifier.
 *               Sends the signal to SWDLAdaptor
 * Parameters :
 *   checkactivatemodule     rw    It is QDBusVariant datatype which holds the structure
 *                                  SCheckActivateModule containing checksum,
 *                                  software download id, device id, sender handle.
 *
 * Returns :None
 *
 *******************************************************************
 */

void CSWDLAdaptor::CheckActivateModuleSlot(const QDBusVariant &checkactivatemodule)
{
    emit CheckActivateModule(checkactivatemodule);
}

/*
 *******************************************************************
 * Function:RequestTransferExitSlot
 *
 * Description : Invoked by the handler function for Request Transfer Exit To HU from IBOX or Amplifier.
 *               Sends the signal to SWDLAdaptor
 * Parameters :
 *    requesttransferexit   rw    It is QDBusVariant datatype which holds the structure
 *                                SRequestTransferExit software download id, device id, sender handle.
 *
 * Returns :None
 *
 *******************************************************************
 */

void CSWDLAdaptor::RequestTransferExitSlot(const QDBusVariant &requesttransferexit)
{
    emit RequestTransferExit(requesttransferexit);
}

/*
 *******************************************************************
 * Function:SWDLProgressInfoSlot
 *
 * Description : Invoked by the Listener function for SWDLProgressInfo To HU from IBOX or Amplifier.
 *               Sends the signal to SWDLAdaptor
 * Parameters :
 *    swdlid        rw   It holds data which represents FBlock and Shadows of IBOX and Amplifier
 *                       in order to update the respective properties.
 *    swdlOpMode    rw   Is unsigned byte which holds the SWDL operation mode set by the user.
 *    swdlProgress  rw   Is unsigned byte which holds the progress status.
 *
 * Returns :None
 *
 *******************************************************************
 */

void CSWDLAdaptor::SWDLProgressInfoSlot(const uint swdlid, const uint swdlOpMode, const uint swdlProgress)
{
     emit SWDLProgressInfo(swdlid,swdlOpMode,swdlProgress);
}
/*
 *******************************************************************
 * Function:UpdateProgressingInfoStatusSlot
 *
 * Description : Invoked by the Listener function for UpdateProgressingInfo To HU from IBOX or Amplifier.
 *               Sends the signal to SWDLAdaptor
 * Parameters :
 *    swdlid         rw   It holds the node id like IBOX/AMP.
 *    updateStatus   rw   It holds the updateProgress info from IBOX or AMP
 *
 * Returns :None
 *
 *******************************************************************
 */
void CSWDLAdaptor::UpdateProgressingInfoStatusSlot(const uint swdlid, const uint updateStatus)
{
    emit UpdateProgressingInfoStatus(swdlid,updateStatus);
}

// iBox SWDL FBlock checkling for iBox Update, 2014.2.24
void CSWDLAdaptor::iBoxSWDLFBlockVisibleSlot(const bool state)
{
	emit iBoxSWDLFBlockVisible(state);
}

/*
 *******************************************************************
 * Function:CSpeechAdaptor
 *
 * Description :Is a constructor
 *
 * Parameters :
 *    context     rw   Is instance of CMOSTManager
 *
 * Returns : none
 *
 *******************************************************************
 */

CSpeechAdaptor::CSpeechAdaptor(CMOSTManager *context)
{
    m_CSpeechRecognitionControlContext = context->GetSpeechRecognitionControlInstance();

    connect(m_CSpeechRecognitionControlContext,SIGNAL(StartTTSData(uint,uint,uint,QByteArray)),this,SLOT(StartTTSDataSlot(uint,uint,uint,QByteArray)));
    connect(m_CSpeechRecognitionControlContext,SIGNAL(AbortTTS(uint,uint)),this,SLOT(AbortTTSSlot(uint,uint)));
}

/*
 *******************************************************************
 * Function:~CSpeechAdaptor
 *
 * Description :Is a Destructor
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */

CSpeechAdaptor::~CSpeechAdaptor()
{

}

/*
 *******************************************************************
 * Function:SetttsResult
 *
 * Description :Function called by the SpeechRecognition controller to set the ttsResult.
 *
 * Parameters :
 *    senderHandle    rw   It holds the senderHandle set by the user. It is unsigned word value.
 *
 *    length          rw   It holds the length set by the user. It is unsigned word value.
 *
 * Returns : true or false
 *
 *******************************************************************
 */

bool CSpeechAdaptor::SetTTSResult(const uint senderHandle, const uint length, const uint deviceid, const uint state)
{
    bool result = false;

    if(m_CSpeechRecognitionControlContext->FBlockVisible())
    {
        result = m_CSpeechRecognitionControlContext->SetTTSResult(senderHandle,length,deviceid,state);
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetManualInput
 *
 * Description :Function called by the SpeechRecognition controller to set the ttsResult.
 *
 * Parameters :
 *    inputTarget    rw   It holds the input target set by the user.
 *
 *    inputKeyWord   rw   It holds the inputKeyWord set by the user.
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CSpeechAdaptor::SetManualInput(const uint inputTarget, const QString inputKeyWord)
{
    bool result = false;

    if(m_CSpeechRecognitionControlContext->FBlockVisible())
    {
        result = m_CSpeechRecognitionControlContext->SetManualInput(inputTarget,inputKeyWord);
    }

    return result;
}

/*
 *******************************************************************
 * Function:TTSDataSlot
 *
 * Description :
 *
 * Parameters :
 *    senderHandle    rw   It holds the senderHandle set by the user. It is unsigned word value.
 *
 *    length          rw   It holds the length set by the user. It is unsigned word value.
 *
 * Returns : true or false
 *
 *******************************************************************
 */

void CSpeechAdaptor::StartTTSDataSlot(const uint sourceDeviceID, const uint senderHandle, const uint length, const QByteArray ttsData)
{
    emit StartTTSData(sourceDeviceID,senderHandle,length,ttsData);
}

/*
 *******************************************************************
 * Function:AbortTTSSlot
 *
 * Description :Function called by the SpeechRecognition controller to set the ttsResult.
 *
 * Parameters :
 *    senderHandle    rw   It holds the senderHandle set by the user. It is unsigned word value.
 *
 *    length          rw   It holds the length set by the user. It is unsigned word value.
 *
 * Returns : true or false
 *
 *******************************************************************
 */

void CSpeechAdaptor::AbortTTSSlot(const uint sourceDeviceID, const uint senderHandle)
{
    emit AbortTTS(sourceDeviceID,senderHandle);
}

/*
 *******************************************************************
 * Function:CTelephoneAdaptor
 *
 * Description :Is a constructor
 *
 * Parameters :
 *    context     rw   Is instance of CMOSTManager
 *
 * Returns : none
 *
 *******************************************************************
 */

CTelephoneAdaptor::CTelephoneAdaptor(CMOSTManager *context)
{
    m_TelephoneShadowControl = context->GetTelephoneShadowControlInstance();

    /** Connect to the signals received from telephone shadow control */
    connect(m_TelephoneShadowControl,SIGNAL(CurrentNumber(QString)),this,SLOT(CurrentNumberSlot(QString)));
    connect(m_TelephoneShadowControl,SIGNAL(CollectPBookResult(QDBusVariant)),this,SLOT(CollectPBookResultSlot(QDBusVariant)));
    connect(m_TelephoneShadowControl,SIGNAL(PBookStatus(ushort)),this,SLOT(PBookStatusSlot(ushort)));
    connect(m_TelephoneShadowControl,SIGNAL(CallState(QDBusVariant)),this,SLOT(CallStateSlot(QDBusVariant)));
    connect(m_TelephoneShadowControl,SIGNAL(CallDuration(QDBusVariant)),this,SLOT(CallDurationSlot(QDBusVariant)));
    connect(m_TelephoneShadowControl,SIGNAL(ModemLevel(uint)),this,SLOT(ModemLevelSlot(uint)));
    connect(m_TelephoneShadowControl,SIGNAL(RecipientInfo(uint,QString)),this,SLOT(RecipientInfoSlot(uint,QString)));
    connect(m_TelephoneShadowControl,SIGNAL(MissedCallStatus(uint,uint)),this,SLOT(MissedCallStatusSlot(uint,uint)));
    connect(m_TelephoneShadowControl,SIGNAL(UserMode(uint,bool)),this,SLOT(UserModeSlot(uint,bool)));
    connect(m_TelephoneShadowControl,SIGNAL(ModemDataStatus(uint)),this,SLOT(ModemDataStatusSlot(uint)));
    connect(m_TelephoneShadowControl,SIGNAL(CallHistoryInfo(QDBusVariant)),this,SLOT(CallHistoryInfoSlot(QDBusVariant)));
    connect(m_TelephoneShadowControl,SIGNAL(DialNumberResult(QDBusVariant)),this,SLOT(DialNumberResultSlot(QDBusVariant)));
    connect(m_TelephoneShadowControl,SIGNAL(AcceptCallResult(uint,uint)),this,SLOT(AcceptCallResultSlot(uint,uint)));
    connect(m_TelephoneShadowControl,SIGNAL(HangupCallResult(uint,uint)),this,SLOT(HangupCallResultSlot(uint,uint)));
    connect(m_TelephoneShadowControl,SIGNAL(TMServiceStatus(uint, uint)),this,SLOT(TMServiceStatusSlot(uint, uint)));
    connect(m_TelephoneShadowControl,SIGNAL(CallHistoryStatus(uint)),this,SLOT(CallHistoryStatusSlot(uint)));   // Fcat v13.9.2
}

/*
 *******************************************************************
 * Function:~CTelephoneAdaptor
 *
 * Description :Is a Destructor
 *
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */

CTelephoneAdaptor::~CTelephoneAdaptor()
{

}

/*
 *******************************************************************
 * Function:CurrentNumberSlot
 *
 * Description :This slot is connected to signal CurrentNumber
 *              from CTelephoneShadowControl.Sends the same signal over QtDbus
 *              for respective application access.
 * Parameters :
 *    telNumber  rw    Last dialed number.
 * Returns : none
 *
 *******************************************************************
 */

void CTelephoneAdaptor::CurrentNumberSlot(const QString telNumber)
{
    emit CurrentNumber(telNumber);
}

/*
 *******************************************************************
 * Function:CallStateSlot
 *
 * Description :This slot is connected to signal CallState
 *              from CTelephoneShadowControl.Sends the same signal over QtDbus
 *              for respective application access.
 * Parameters :
 *    callStatedata  rw    holds the data with in a struct for Call State.
 * Returns : none
 *
 *******************************************************************
 */

void CTelephoneAdaptor::CallStateSlot(const QDBusVariant &callStatedata)
{
    emit CallState(callStatedata);
}

/*
 *******************************************************************
 * Function:CallDurationSlot
 *
 * Description :This slot is connected to signal CallDuration
 *              from CTelephoneShadowControl.Sends the same signal over QtDbus
 *              for respective application access.
 * Parameters :
 *    callDuration  rw    holds the data with in a struct for Call Duration.
 * Returns : none
 *
 *******************************************************************
 */

void CTelephoneAdaptor::CallDurationSlot(const QDBusVariant &callDuration)
{
    emit CallDuration(callDuration);
}

/*
 *******************************************************************
 * Function:ModemLevelSlot
 *
 * Description :This slot is connected to signal ModemLevel
 *              from CTelephoneShadowControl.Sends the same signal over QtDbus
 *              for respective application access.
 * Parameters :
 *    modemlevel  rw   level of the receive sensitivity in modem
 * Returns : none
 *
 *******************************************************************
 */

void CTelephoneAdaptor::ModemLevelSlot(const uint modemlevel)
{
    emit ModemLevel(modemlevel);
}

/*
 *******************************************************************
 * Function:RecipientInfoSlot
 *
 * Description :This slot is connected to signal RecipientInfo
 *              from CTelephoneShadowControl.Sends the same signal over QtDbus
 *              for respective application access.
 * Parameters :
 *    contact  rw   Type of Contact
 *    name     rw   Name of the Contact
 * Returns : none
 *
 *******************************************************************
 */

void CTelephoneAdaptor::RecipientInfoSlot(const uint contact, const QString &name)
{
    emit RecipientInfo(contact,name);
}

/*
 *******************************************************************
 * Function:MissedCallStatusSlot
 *
 * Description :This slot is connected to signal NewSMS
 *              from CTelephoneShadowControl.Sends the same signal over QtDbus
 *              for respective application access.
 * Parameters :
 *    msdCallStatus    rw   Missed call status
 *    msdCallCount     rw   Missed call count
 * Returns : none
 *
 *******************************************************************
 */

void CTelephoneAdaptor::MissedCallStatusSlot(const uint msdCallStatus, const uint msdCallCount)
{
    emit MissedCallStatus(msdCallStatus,msdCallCount);
}

/*
 *******************************************************************
 * Function:UserModeSlot
 *
 * Description :This slot is connected to signal UserMode
 *              from CTelephoneShadowControl.Sends the same signal over QtDbus
 *              for respective application access.
 * Parameters :
 *    usermode          rw   Mode selected by User
 * Returns : none
 *
 *******************************************************************
 */

void CTelephoneAdaptor::UserModeSlot(const uint usermode,const bool micOff)
{
    emit UserMode(usermode,micOff);
}

/*
 *******************************************************************
 * Function:ModemDataStatusSlot
 *
 * Description :This slot is connected to signal ModemDataStatus
 *              from CTelephoneShadowControl.Sends the same signal over QtDbus
 *              for respective application access.
 * Parameters :
 *    modemstatus          rw   Modem Status
 * Returns : none
 *
 *******************************************************************
 */

void CTelephoneAdaptor::ModemDataStatusSlot(const uint modemstatus)
{
    emit ModemDataStatus(modemstatus);
}

/*
 *******************************************************************
 * Function:CallHistoryInfoSlot
 *
 * Description :This slot is connected to signal CallHistoryInfo
 *              from CTelephoneShadowControl.Sends the same signal over QtDbus
 *              for respective application access.
 * Parameters :
 *    callHistoryInfo  rw   holds the data with in a struct for Call History info.
 * Returns : none
 *
 *******************************************************************
 */

void CTelephoneAdaptor::CallHistoryInfoSlot(const QDBusVariant &callHistoryInfo)
{
    emit CallHistoryInfo(callHistoryInfo);
}

/*
 *******************************************************************
 * Function:CollectPBookResultSlot
 *
 * Description :This slot is connected to signal CollectPBookResult
 *              from CTelephoneShadowControl.Sends the same signal over QtDbus
 *              for respective application access.
 * Parameters :
 *    pBookDataResult  rw   holds the result data with in a struct for Collect pbook result.
 * Returns : none
 *
 *******************************************************************
 */

void CTelephoneAdaptor::CollectPBookResultSlot(const QDBusVariant &pBookDataResult)
{
    emit CollectPBookResult(pBookDataResult);
}

/*
 *******************************************************************
 * Function:PBookStatusSlot
 *
 * Description :This slot is connected to signal PBookStatus
 *              from CTelephoneShadowControl.Sends the same signal over QtDbus
 *              for respective application access.
 * Parameters :
 *    pBookStatus  rw   phone book information for VAD/Global Search
 * Returns : none
 *
 *******************************************************************
 */

void CTelephoneAdaptor::PBookStatusSlot(const ushort totalPBEntry)
{
    emit PBookStatus(totalPBEntry);
}

/*
 *******************************************************************
 * Function:DialNumberResultSlot
 *
 * Description :This slot is connected to signal DialNumberResult
 *              from CTelephoneShadowControl.Sends the same signal over QtDbus
 *              for respective application access.
 * Parameters :
 *    dialNumData  rw   holds the result data with in a struct for Dial Number.
 * Returns : none
 *
 *******************************************************************
 */

void CTelephoneAdaptor::DialNumberResultSlot(const QDBusVariant &dialNumData)
{    
    emit DialNumberResult(dialNumData);
}

/*
 *******************************************************************
 * Function:AcceptCallResultSlot
 *
 * Description :This slot is connected to signal AcceptCallResult
 *              from CTelephoneShadowControl.Sends the same signal over QtDbus
 *              for respective application access.
 * Parameters :
 *    senderHandle  rw   holds the senderHandle value for Accept Call.
 *    callId        rw   holds the callId value for Accept Call.
 *
 * Returns : none
 *
 *******************************************************************
 */

void CTelephoneAdaptor::AcceptCallResultSlot(const uint senderHandle, const uint callId)
{
    emit AcceptCallResult(senderHandle,callId);
}

/*
 *******************************************************************
 * Function:HangupCallResultSlot
 *
 * Description :This slot is connected to signal HangupCallResult
 *              from CTelephoneShadowControl.Sends the same signal over QtDbus
 *              for respective application access.
 * Parameters :
 *    senderHandle  rw   holds the senderHandle value for Hangup Call.
 *    callId        rw   holds the callId value for Hangup Call.
 *
 * Returns : none
 *
 *******************************************************************
 */

void CTelephoneAdaptor::HangupCallResultSlot(const uint senderHandle, const uint callId)
{
    emit HangupCallResult(senderHandle,callId);
}


/*
 *******************************************************************
 * Function:TMServiceStatusSlot
 *
 * Description :This slot is connected to signal TMServiceStatus
 *              from CTelephoneShadowControl.Sends the same signal over QtDbus
 *              for respective application access.
 * Parameters :
 *    tmService        r   holds the TMService value.
 *
 * Returns : none
 *
 *******************************************************************
 */
void CTelephoneAdaptor::TMServiceStatusSlot(const uint tmStatus, const uint provStatus)
{
    emit TMServiceStatus(tmStatus, provStatus);
}

void CTelephoneAdaptor::CallHistoryStatusSlot(const uint status)
{
    emit CallHistoryStatus(status);
}

/*
 *******************************************************************
 * Function:StartCollectPBook
 *
 * Description :  API called over QtDbus by Dialler,Messaging or PhoneBook
 *                which sends request to to fetch PBook data
 * Parameters :
 *  pBookDataRequest rw  Holds the structure for Collect PBook request
 * Returns : boolean value returned by function  StartCollectPBook of Control class
 *
 *******************************************************************
 */
/*
bool CTelephoneAdaptor::StartCollectPBook(const QDBusVariant &pBookDataRequest)
{
    bool result = false;

    if(m_TelephoneShadowControl->FBlockVisible())
    {
        SCollectPBook pBookData;
        pBookDataRequest.variant().value<QDBusArgument>() >> pBookData;

        result = m_TelephoneShadowControl->StartCollectPBook(pBookData);
    }

    return result;
}
*/
bool CTelephoneAdaptor::StartCollectPBook(const uint senderHandle, const uint pBookType)
{
    bool result = false;

    if(m_TelephoneShadowControl->FBlockVisible())
    {
        result = m_TelephoneShadowControl->StartCollectPBook(senderHandle, pBookType);
    }

    return result;
}

/*
 *******************************************************************
 * Function:StartDialNumber
 *
 * Description :  API called over QtDbus by Dialler,Messaging or PhoneBook
 *                which sends request to Dial a number
 * Parameters :
 *  dialNumData rw  Holds the structure for Dial number request
 * Returns : boolean value returned by function  StartDialNumber of Control class
 *
 *******************************************************************
 */

bool CTelephoneAdaptor::StartDialNumber(const QDBusVariant &dialNumData)
{
    bool result = false;

    if(m_TelephoneShadowControl->FBlockVisible())
    {
        SDialNumberData dialNumRequest;
        dialNumData.variant().value<QDBusArgument>() >> dialNumRequest;

        result = m_TelephoneShadowControl->StartDialNumber(dialNumRequest);
    }

    return result;
}

/*
 *******************************************************************
 * Function:StartAcceptCall
 *
 * Description :  API called over QtDbus by Dialler,Messaging or PhoneBook
 *                which sends request to Accept Call
 * Parameters :
 *    senderHandle  rw   holds the senderHandle value for Accept Call.
 *    callId        rw   holds the callId value for Accept Call.
 *
 * Returns : boolean value returned by function  StartAcceptCall of Control class
 *
 *******************************************************************
 */
// Connect_ UISH DBus Call (i/f > message), 2014.2.27
bool CTelephoneAdaptor::StartAcceptCall(const QDBusMessage& msg)
{
    bool result = false;
    uint senderHandle, callId;

    const QList<QVariant> & args = msg.arguments();
    senderHandle = (uint) args.at(0).toInt();
    callId = (uint) args.at(1).toInt();

    if(m_TelephoneShadowControl->FBlockVisible())
    {
        result = m_TelephoneShadowControl->StartAcceptCall(senderHandle,callId);
    }

    return result;
}

/*
 *******************************************************************
 * Function:StartHangupCall
 *
 * Description :  API called over QtDbus by Dialler,Messaging or PhoneBook
 *                which sends request to Hangup Call
 * Parameters :
 *    senderHandle  rw   holds the senderHandle value for Hangup Call.
 *    callId        rw   holds the callId value for Hangup Call.
 *
 * Returns : boolean value returned by function  StartHangupCall of Control class
 *
 *******************************************************************
 */

bool CTelephoneAdaptor::StartHangupCall(const uint senderHandle, const uint callId)
{
    bool result = false;

    if(m_TelephoneShadowControl->FBlockVisible())
    {
        result = m_TelephoneShadowControl->StartHangupCall(senderHandle,callId);
    }

    return result;
}
/*
 *******************************************************************
 * Function:GetModemDataStatus
 *
 * Description :  API called over QtDbus by VR to get the ModemStatus
 *
 * Parameters :
 *      none
 *
 * Returns : true or false
 *
 *******************************************************************
 */
bool CTelephoneAdaptor::GetModemDataStatus(void)
{
    bool result = false;

    if(m_TelephoneShadowControl->FBlockVisible())
    {
        result = m_TelephoneShadowControl->GetModemDataStatus();
    }

    return result;
}

bool CTelephoneAdaptor::GetModemLevel(void)
{
    bool result = false;

    if(m_TelephoneShadowControl->FBlockVisible())
    {
        result = m_TelephoneShadowControl->GetModemLevel();
    }

    return result;
}

bool CTelephoneAdaptor::GetPBookStatus(void)
{
    bool result = false;

    if(m_TelephoneShadowControl->FBlockVisible())
    {
        result = m_TelephoneShadowControl->GetPBookStatus();
    }

    return result;
}

bool CTelephoneAdaptor::GetTMService(void)
{
    bool result = false;

    if(m_TelephoneShadowControl->FBlockVisible())
    {
        result = m_TelephoneShadowControl->GetTMSerivce();
    }

    return result;
}

bool CTelephoneAdaptor::GetCallHistoryStatus(void)
{
    bool result = false;

    if(m_TelephoneShadowControl->FBlockVisible())
    {
        result = m_TelephoneShadowControl->GetCallHistoryStatus();
    }

    return result;
}

/*
 *******************************************************************
 * Function:SetPBookStatus
 *
 * Description :  API called over QtDbus by Dialler,Messaging or PhoneBook
 *                which sends PBook status
 * Parameters :
 *  pbookstatus rw  information of Pbook
 * Returns : boolean value returned by function  SetPBookStatus of Control class
 *
 *******************************************************************
 */
#if 0
bool CTelephoneAdaptor::SetPBookStatus(const uint pbookstatus)
{
    return m_TelephoneShadowControl->SetPBookStatus(pbookstatus);
}
#endif

/*
 *******************************************************************
 * Function:CallHistoryInfoRequest
 *
 * Description :  API called over QtDbus by Dialler,Messaging or PhoneBook
 *                which sends Call History Info Request
 * Parameters :
 * Returns : boolean value returned by function  CallHistoryInfoRequest of Control class
 *
 *******************************************************************
 */

bool CTelephoneAdaptor::CallHistoryInfoRequest()
{
    bool result = false;

    if(m_TelephoneShadowControl->FBlockVisible())
    {
        result = m_TelephoneShadowControl->CallHistoryInfoRequest();
    }

    return result;
}


/*
 *******************************************************************
 * Function:CDMBTunerAdaptor
 *
 * Description :Is a constructor
 *
 * Parameters :
 *    context     rw   Is instance of CMOSTManager
 *
 * Returns : none
 *
 *******************************************************************
 */
CDMBTunerAdaptor::CDMBTunerAdaptor(CMOSTManager *context)
{

    m_DMBTunerControlContext = context->GetDMBTunerFblockControlInstance();
//==== 2014.7.21 for log, hyunyong.sung ===//
/*
    LOG_INIT_V2(LOG_PREFIX_MOST)

    m_DMBTunerControlContext = context->GetDMBTunerFblockControlInstance();

    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    if( !sessionBus.isConnected() )
    {
        LOG_CRITICAL << LOG_TAG_DMB_ADAP << " Cannot connect to the D-Bus session bus " << LOG_ENDL;
        return;
    }

    sessionBus.connect(DMB_SERVICE, DMB_OBJECT, DMB_INTERFACE, "sendTPEGData", this, SLOT(SetTPEGStatus(QByteArray,int)));
*/    
}

CDMBTunerAdaptor::~CDMBTunerAdaptor()
{
//==== 2014.7.21 for log, hyunyong.sung ===//
/*
    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    if( !sessionBus.isConnected() )
    {
        LOG_CRITICAL << LOG_TAG_DMB_ADAP << " Cannot connect to the D-Bus session bus " << LOG_ENDL;
        return;
    }

    sessionBus.disconnect(DMB_SERVICE, DMB_OBJECT, DMB_INTERFACE, "sendTPEGData", this, SLOT(SetTPEGStatus(QByteArray,int)));
*/  
}

bool CDMBTunerAdaptor::SetSignalQuality(const uint dmbSignalQuality)
{
    bool result = false;

    if(m_DMBTunerControlContext->FBlockVisible())
    {
        result = m_DMBTunerControlContext->SendSignalQuality(PROPERTY_STATUS, (const uchar)dmbSignalQuality);
    }

    return result;
}

bool CDMBTunerAdaptor::SetTPEGStatus(const QByteArray &tpegData, int length)
{
    bool result = false;

    if(m_DMBTunerControlContext->FBlockVisible())
    {
        result = m_DMBTunerControlContext->SendTPEGStatus(PROPERTY_STATUS, CTPEGStatus::TPEGStatusNewData);
    }

    return result;
}

bool CDMBTunerAdaptor::SetTPEGInfo(const QDBusVariant &tpegInfo)
{
    bool result = false;

    if(m_DMBTunerControlContext->FBlockVisible())
    {
        STPEGInfo _TPEGInfo;
        tpegInfo.variant().value<QDBusArgument>() >> _TPEGInfo;

        result = m_DMBTunerControlContext->SendTPEGInfo(PROPERTY_STATUS, _TPEGInfo);
    }

    return result;
}

bool CDMBTunerAdaptor::SetDMBStauts(const uint dmbStauts, const uint currentBrowserListLength)
{
    bool result = false;

    if(m_DMBTunerControlContext->FBlockVisible())
    {
        result = m_DMBTunerControlContext->SendDMBStauts(PROPERTY_STATUS, (const uchar)dmbStauts, (const uchar)currentBrowserListLength);
    }

    return result;
}


/*
 *******************************************************************
 * Function:CSatRadioAdaptor
 *
 * Description :Is a constructor
 *
 * Parameters :
 *    context     rw   Is instance of CMOSTManager
 *
 * Returns : none
 *
 *******************************************************************
 */
CSatRadioAdaptor::CSatRadioAdaptor(CMOSTManager *context)
{
    m_SatRadioControlContext = context->GetSatRadioFblockControlInstance();

    connect(m_SatRadioControlContext, SIGNAL(StationSelect(uchar,uchar)), this, SLOT(StationSelectSlot(uchar, uchar)));
    connect(m_SatRadioControlContext, SIGNAL(ServiceProvider(uchar)), this, SLOT(ServiceProviderSlot(uchar)));
}

CSatRadioAdaptor::~CSatRadioAdaptor()
{
}

void CSatRadioAdaptor::StationSelectSlot(const uchar type, const uchar station)
{
    emit onStationSelect((const uint)type, (const uint)station);
}

void CSatRadioAdaptor::ServiceProviderSlot(const uchar serviceProvider)
{
    emit onServiceProvider((const uint)serviceProvider);
}

bool CSatRadioAdaptor::SetStationSelect(const uint optype, const uint station, const uint presetNo)
{
    bool result = false;

    if(m_SatRadioControlContext->FBlockVisible())
    {
        result = m_SatRadioControlContext->SendStationSelect((const uchar)optype, (const uchar)station, (const uchar)presetNo);
    }

    return result;
}

bool CSatRadioAdaptor::SetServiceProvider(const uint optype, const uint serviceProvider)
{
    bool result = false;

    if(m_SatRadioControlContext->FBlockVisible())
    {
        result = m_SatRadioControlContext->SendServiceProvider((const uchar)optype, (const uchar)serviceProvider);
    }

    return result;
}

bool CSatRadioAdaptor::SetServiceStatus(const uint optype, const bool audio, const bool stationList, const uint subscription)
{
    bool result = false;

    if(m_SatRadioControlContext->FBlockVisible())
    {
        result = m_SatRadioControlContext->SendServiceStatus((const uchar)optype, audio, stationList, (const uchar)subscription);
    }

    return result;
}

bool CSatRadioAdaptor::SetTMCStatus(const uint optype, const uint tmcStatus)
{
    bool result = false;

    if(m_SatRadioControlContext->FBlockVisible())
    {
        result = m_SatRadioControlContext->SendTMCStatus((const uchar)optype, (const uchar)tmcStatus);
    }

    return result;
}


/*
 *******************************************************************
 * Function:CAppStateAdaptor
 *
 * Description :Is adaptor for re-init mostmanager
 *
 * Parameters :
 *    context     rw   Is instance of CMOSTManager
 *
 * Returns : none
 *
 *******************************************************************
 */
CAppStateAdaptor::CAppStateAdaptor(CMOSTManager *context)
{
    LOG_INIT_V2(LOG_PREFIX_MOST)

    m_MostManager = context;
    memset(&m_AppState, 0, sizeof(m_AppState));
}

CAppStateAdaptor::~CAppStateAdaptor()
{
}

void CAppStateAdaptor::RegisterFBlocktoServer(void)
{    
    uint appState = 0;
    memcpy(&appState, &m_AppState, sizeof(m_AppState));

    if( ((appState & AUXIN_FBLOCK) == AUXIN_FBLOCK)&& !m_MostManager->GetAuxInControlInstance()->GetIsRegistered() )
    {
        m_MostManager->GetAuxInControlInstance()->Initialize();    	// 2014.11.6, hyunyong.sung, need to init after register object. 
        m_MostManager->GetAuxInControlInstance()->Register();
    }

    if( ((appState & SR_FBLOCK) == SR_FBLOCK) && !m_MostManager->GetSpeechRecognitionControlInstance()->GetIsRegistered() )
    {
        m_MostManager->GetSpeechRecognitionControlInstance()->Initialize();    	// 2014.11.6, hyunyong.sung, need to init after register object. 
        m_MostManager->GetSpeechRecognitionControlInstance()->Register();
    }

    if( ((appState & DLNA_SHADOW) == DLNA_SHADOW) && !m_MostManager->GetDLNAShadowControlInstance()->GetIsRegistered() )
    {
        m_MostManager->GetDLNAShadowControlInstance()->Initialize();    // 2014.11.10, hyunyong.sung, need to init after register object. 
        m_MostManager->GetDLNAShadowControlInstance()->Register();
    }

    if( ((appState & BT_FBLOCK) == BT_FBLOCK) && !m_MostManager->GetBluetoothControlInstance()->GetIsRegistered() )
    {
       m_MostManager->GetBluetoothControlInstance()->Initialize();    	// 2014.11.6, hyunyong.sung, need to init after register object.            
       m_MostManager->GetBluetoothControlInstance()->Register();
    }

    if( ((appState & TELEPHONE_SHADOW) == TELEPHONE_SHADOW) && !m_MostManager->GetTelephoneShadowControlInstance()->GetIsRegistered() )
    {
        m_MostManager->GetTelephoneShadowControlInstance()->Initialize();    // 2014.11.10, hyunyong.sung, need to init after register object. 
        m_MostManager->GetTelephoneShadowControlInstance()->Register();
    }

    if( ((appState & DIAG_FBLOCK) == DIAG_FBLOCK) && !m_MostManager->GetDiagnosisFBlockShadowControlInstance()->GetIsRegistered() )
    {
        m_MostManager->GetDiagnosisFBlockShadowControlInstance()->Initialize();
        m_MostManager->GetDiagnosisFBlockShadowControlInstance()->Register();
    }

    if( ((appState & HMI_FBLOCK) == HMI_FBLOCK) && !m_MostManager->GetHMIFBlockShadowControl()->GetIsRegistered() )
    {
        m_MostManager->GetHMIFBlockShadowControl()->Initialize();    	// 2014.11.6, hyunyong.sung, need to init after register object.            
        m_MostManager->GetHMIFBlockShadowControl()->Register();
    }

    if( ((appState & NAVI_FBLOCK) == NAVI_FBLOCK) && !m_MostManager->GetNavigationControlInstance()->GetIsRegistered() )
    {
        m_MostManager->GetNavigationControlInstance()->Initialize();
        m_MostManager->GetNavigationControlInstance()->Register();
    }

    if( ((appState & IBOX_SHADOW) == IBOX_SHADOW) && !m_MostManager->GetIBOXShadowControlInstance()->GetIsRegistered() )
    {
	m_MostManager->GetIBOXShadowControlInstance()->Initialize();    // 2014.11.10, hyunyong.sung, need to init after register object. 
	m_MostManager->GetIBOXShadowControlInstance()->Register();
    }

    if( ((appState & SWDL_FBLOCK) == SWDL_FBLOCK) && !m_MostManager->GetSWDLFBlockControlInstance()->GetIsRegistered() )
    {
/* 2014.11.6, hyunyong.sung, need to init after register object. 
        if( !m_MostManager->GetDiagnosisFBlockShadowControlInstance()->GetIsRegistered() )
        {
            // SWDL needs Diagnosis FBlock.
            m_MostManager->GetDiagnosisFBlockShadowControlInstance()->Initialize();
            m_MostManager->GetDiagnosisFBlockShadowControlInstance()->Register();
        }
*/
        m_MostManager->GetSWDLFBlockControlInstance()->Initialize();
        m_MostManager->GetSWDLFBlockControlInstance()->Register();
    }

    if( ((appState & DMB_FBLOCK) == DMB_FBLOCK) && !m_MostManager->GetDMBTunerFblockControlInstance()->GetIsRegistered() )
    {
        m_MostManager->GetDMBTunerFblockControlInstance()->Initialize();    // 2014.11.6, hyunyong.sung, need to init after register object. 
        m_MostManager->GetDMBTunerFblockControlInstance()->Register();
    }

    if( ((appState & SATRADIO_FBLOCK) == SATRADIO_FBLOCK) && !m_MostManager->GetSatRadioFblockControlInstance()->GetIsRegistered() )
    {
        m_MostManager->GetSatRadioFblockControlInstance()->Initialize();    // 2014.11.6, hyunyong.sung, need to init after register object.   
        m_MostManager->GetSatRadioFblockControlInstance()->Register();
    }

}

void CAppStateAdaptor::UnRegisterFBlockfromServer(void)
{
    uint appState = 0;
    memcpy(&appState, &m_AppState, sizeof(m_AppState));

    if( m_MostManager->GetDiagnosisFBlockShadowControlInstance()->GetIsRegistered()
        && ((appState & DIAG_FBLOCK) != DIAG_FBLOCK)
        && ((appState & SWDL_FBLOCK) != SWDL_FBLOCK)
    )
    {
        m_MostManager->GetDiagnosisFBlockShadowControlInstance()->UnRegister();
    }

    if( m_MostManager->GetSWDLFBlockControlInstance()->GetIsRegistered() && ((appState & SWDL_FBLOCK) != SWDL_FBLOCK) )
    {
        m_MostManager->GetSWDLFBlockControlInstance()->UnRegister();
    }

    if( m_MostManager->GetNavigationControlInstance()->GetIsRegistered() && ((appState & NAVI_FBLOCK) != NAVI_FBLOCK) )
    {
        m_MostManager->GetNavigationControlInstance()->UnRegister();
    }
}

bool CAppStateAdaptor::onAppStateChanged(const QDBusMessage& msg)
{
    bool result = false;
    QByteArray appState;

    const QList<QVariant> & args = msg.arguments();
    for(int i = 0; i<args.size(); i++)
    {
        appState.append((uint) args.at(i).toInt());
    }
    result = onAppStateChanged(appState);
    return result;
}




bool CAppStateAdaptor::onAppStateChanged(const QByteArray appState)
{    
    uint appid;
    uint state;

    bool isUpdated = false;

    m_AppState.appUISH = 1;

    for(int i=0; i<appState.size(); )
    {
        appid = appState.at(i++);
        state = appState.at(i++);

        switch(appid)
        {
            /*
            case APP_IDLE:  // clock app is not used anymore, 20130830
                m_AppState.appClock = state;
                isUpdated = true;
                break;
            */
            case APP_STANDBY:
                m_AppState.appStandby = state;
                isUpdated = true;
                break;
            case APP_BTPHONE:
                m_AppState.appBTPhone = state;
                isUpdated = true;
                break;
            case APP_SETTINGS:
                m_AppState.appSetting = state;
                isUpdated = true;
                break;
            case APP_NAVIGATION:
                m_AppState.appNavi = state;
                isUpdated = true;
                break;
            case APP_VR:
                m_AppState.appVR = state;
                isUpdated = true;
                break;
            case APP_IBOX:
                m_AppState.appiBox = state;
                isUpdated = true;
                break;
            case APP_ENGINEERING:
                m_AppState.appEng = state;
                isUpdated = true;
                break;
            case APP_UPGRADE:
                m_AppState.appUpgrade = state;
                isUpdated = true;
                break;
            case APP_MOBILETV:
                m_AppState.appDMB = state;
                isUpdated = true;
                break;
            case APP_XM_AUDIO:
                m_AppState.appXMAudio = state;
                isUpdated = true;
                break;
            default:
                isUpdated = false;
                break;
        }

        if(isUpdated == true)
        {
            if(state == APP_RUNNING)
            {
                RegisterFBlocktoServer();
            }
            /*
            else if(state == APP_TERMINATED)
            {
                UnRegisterFBlockfromServer();
            }
            */
        }
    }

    return true;
}

void CAppStateAdaptor::RegisterAll(void)
{
    // Register all FBlocks except for HMI
    char appState1[] = {
        APP_BTPHONE, APP_RUNNING,
        APP_SETTINGS, APP_RUNNING,
        APP_NAVIGATION, APP_RUNNING,
        APP_VR, APP_RUNNING,
        APP_ENGINEERING, APP_RUNNING,
        APP_UPGRADE, APP_RUNNING,
        APP_MOBILETV, APP_RUNNING,
        APP_XM_AUDIO, APP_RUNNING
    };

    onAppStateChanged(appState1);

    // To check if applications for HMI FBlock, APP_STANDBY and APP_IBOX, are ready or not
    char buf[SHM_APP_ALIVE_STATUS_SIZE];
    QByteArray appState2;

    LOG_HIGH << "CAppStateAdaptor" << " Read shared memory" << LOG_ENDL;

    if( !m_AppStateSharedMem.isAttached() )
    {
        m_AppStateSharedMem.detach();
    }

    m_AppStateSharedMem.setKey("UISHStatusInformation");

    if( !m_AppStateSharedMem.attach(QSharedMemory::ReadOnly) )
    {
        LOG_CRITICAL << "CAppStateAdaptor" << " Cannot access to the shared memory for app state data" << LOG_ENDL;
        return;
    }

    m_AppStateSharedMem.lock();
    const char *from = (const char *)m_AppStateSharedMem.constData() + SHM_OFFSET_APP_ALIVE_STATUS_INFO;
    (void)memcpy(buf, from, SHM_APP_ALIVE_STATUS_SIZE);
    m_AppStateSharedMem.unlock();

    if(buf[0] == 1)    // Flag for indicating MOSTManager restarted by UISH
    {
        if(buf[APP_STANDBY] == APP_RUNNING)
        {
            appState2.append(APP_STANDBY);
            appState2.append(APP_RUNNING);
        }

        if(buf[APP_IBOX] == APP_RUNNING)
        {
            appState2.append(APP_IBOX);
            appState2.append(APP_RUNNING);
        }

        if( !appState2.isEmpty() )
        {
            LOG_HIGH << "CAppStateAdaptor" << " Apps state for HMI FBlock : " << appState2.toHex().data() << LOG_ENDL;
            onAppStateChanged(appState2);
        }
    }

    m_AppStateSharedMem.detach();
}



