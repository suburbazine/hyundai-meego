/*
 ******************************************************************
 * COPYRIGHT © 2011
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE       : rpmclient
 *
 * PROGRAMMER   : Hariprasad.Boga@teleca.com                        
 *                Kuladeep.Rayalla@teleca.com                       
 *                Jayan.John@teleca.com                             
 * DATE         : 01/08/2011                                        
 * VERSION      : Draft 0.6                                               
 *------------------------------------------------------------------*
 *
 * MODULE SUMMARY :                                                 
 * Resource Policy Manager is used by QT applications in the DH AVN 
 * project for audio management                                     
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS *
 * Created         Hariprasad.Boga@teleca.com       01/08/2011
 * Modified        Hariprasad.Boga@teleca.com       05/10/2011
 ******************************************************************/

#ifndef __RPM_CLIENT_H__
#define __RPM_CLIENT_H__

#include <QString>
#include <QObject>
#include <QVector>
#include <QList>
#include <QtDBus/QtDBus>
#include <QApplication>
#include <QtGlobal>
#include <QVariant>

#include <gst/gst.h>

#define NUM_OF_LAST_AUDIO 8

/*This enum lists all the error codes for RPM client request status.*/
typedef enum ERPMClientRequestStatus {
    eNoError,
    eOutOfRange,
    eBadArguement,
    eNotSupported,
    eFatalError,
    eTimeout,
    eNoResource,
    eActiveCallInProgress,
    eGeneralError
} TRPMClientRequestStatus;

/*This enum lists all audio modes used in the system and listed in MICOM spec. */
typedef enum EAudioMode
{
    eAudioNone,
    eAVOff,
    eFM1,
    eFM2,
    eAM,
    eDeckAudioI2S,
    eDeckAudioSPDIF,
    eDeckVideoI2S,
    eDeckVideoSPDIF,
    eUSB1AudioI2S,
    eUSB1AudioSPDIF,
    eUSB1VideoI2S, //11
    eUSB1VideoSPDIF,
    eUSB2AudioI2S,
    eUSB2AudioSPDIF,
    eUSB2VideoI2S,
    eUSB2VideoSPDIF,
	eUSB1MirLinkI2S,
	eUSB1MirLinkSPDIF,
	eUSB2MirLinkI2S,
	eUSB2MirLinkSPDIF,
    eIPOD1I2S, //21
    eIPOD1SPDIF,
    eIPOD1PandoraI2S,
    eIPOD1PandoraSPDIF,
    eIPOD2I2S,
    eIPOD2SPDIF,
    eIPOD2PandoraI2S,
    eIPOD2PandoraSPDIF,
    eIPOD1AhaRadioI2S,
    eIPOD1AhaRadioSPDIF,
    eIPOD2AhaRadioI2S, //31
    eIPOD2AhaRadioSPDIF,
    eVCDCI2S,
    eVCDCSPDIF,
    eJukeBoxAudioI2S,
    eJukeBoxAudioSPDIF,
    eJukeBoxVideoI2S,
    eJukeBoxVideoSPDIF,
    eVideoGuideI2S,
    eVideoGuideSPDIF,
    eBTAudio, //41
    eBTPandora,
    eBTMirlink,
    eBTAhaRadio,
    eAUX1Audio,
    eAUX1Video,
    eAUX2Audio,
    eAUX2Video,
    eDMB1,
    eDMB2,
    eCMMB1, //51
    eCMMB2,
    eXM1,
    eXM2,
    eDAB1,
    eDAB2,
    eDAB3,
    eHDRadio,
    eIBoxDLNA,
    eTTS,
    eRDSTA,  //61
    eRDSNews,
    eRDSAlarm,
    eIBoxWebPlayer,
    eIBoxVR,
    eInteractiveGuide,
    eHDRFM1,
    eHDRFM2,
    eHDRAM,
    eDABTA,
	eAVMute, //71
	eDABTAAlarm,
    eMax

} TAudioMode;

typedef enum EVolumeMode
{
    eMainVolume,
    eAux,
    eNavi,
    eVR,
    eBTPhone,
    eBTBell,
    eRDSTAVolume,
    eRDSNewsVolume,
    eRDSAlarmVolume,
    eTTSVolume,
    eWelcoming,
    eTMUCall,
    eTMUTBT,
    eIBoxPhone,
    eIBoxBell

} TVolumeMode;

/* This enum lists audio channels that can be mixed.  */
typedef enum EAudioMixChannel 
{
    eNaviChannel,
    eVRChannel,
    eBTPhoneChannel,
    eBTBellChannel,
    eIBoxPhoneFrontChannel,
    eIBoxPhoneRearChannel,
    eIBoxBellChannel,
    eIBoxVRChannel,
    eBTSiriChannel,
    eBTTtsChannel,
} TAudioMixChannel;

/* This enum lists the Audio sinks. */
typedef enum EAudioSink
{
    eMain,
    eSecondary,
    eAMP,
    eMixer,
    eSWheelKeyMute,
    eMainMuteForPop,
    eUnmuteNoVolume
} TAudioSink;

/* This enum lists all Audio Setup values settings. */
typedef enum EAudioSetup 
{
    eBass,
    eMid,
    eTreble,
    eBalance,
    eFader,
    eVariableEQ,
    eSDVC,
    eSurround,
    eBeep,
    eVolumeRatio,
    eQuantumLogic,
    ePowerBass,
    eWelcome,
    eBassMidTrebleCenter,
    eBalanceFaderCenter,
    eAllInitialize
} TAudioSetup;

/* This enum lists all Audio Beep types.*/
typedef enum EAudioBeepType 
{
    eBeepType0,
    eBeepType1
} TAudioBeepType;

/* This enum lists control values for device mute/ unmute. */
typedef enum EMuteControl 
{
    eUnmute,
    eMute
} TMuteControl;

/* This enum lists values for variabled EQ sound setup setting. */
typedef enum EVariableEQ
{
    eNormal = 0x40,
    ePower = 0x80,
    eConcert = 0xC0,
    eOther = 0xFF
} TVariableEQ;

/* This enum gives On and Off values. */
typedef enum EOnOff
{
    eOff,
    eOn
} TOnOff;

/* This enum lists Error enums that RPM Client uses internally. */
enum Error {NoError, FatalError, GeneralError};



/* This enum lists command enums that RPM Client uses internally. */
typedef enum EControlType {
    eCntlAcquireMain,
    eCntlAcquireSub,
    eCntlMix,
    eCntlVolumeSet,
    eCntlVolumeInfoGet,
    eCntlAudioSetupValueSet,
    eCntlAudioSetupInfoGet,
    eCntlMute,
    eCntlAudioBeep,
    eCntlLastAudio
} TControlType;

/* This enum lists enums for Display Resources */
typedef enum EDisplayResourceType {
    eFrontDisplay,
    eRearDisplay,
} TDisplayResourceType;

/* This enum lists enums for Display Resource Users */
typedef enum EDisplayResourceUser {
    eHeadUnit,
    eIBox
} TDisplayResourceUser;

/* This structure contains all audio setup information */
struct SAudioSetupInfo 
{
    quint8 nBass;
    quint8 nMid;
    quint8 nTreble;
    quint8 nBalance;
    quint8 nFader;
    quint8 nVariableEQ;
    quint8 nAVCSDVC;
    quint8 nSurround;
    quint8 nBeep;
    quint8 nVolRatio;
    quint8 nQuantum;
    quint8 nPowerBass;
    quint8 nWelcoming;
};

/* This structure contains the volume info for various audio sources  */
struct SAudioVolumeInfo
{
    quint8 nMain;
    quint8 nAux;
    quint8 nNavi;
    quint8 nVR;
    quint8 nBTPhone;
    quint8 nBTBell;
    quint8 nRDSTA;
    quint8 nRDSNews;
    quint8 nRDSAlarm;
    quint8 nTTS;
    quint8 nWelcoming;
    quint8 nTMUCall;
    quint8 nTMUTBT;
    quint8 nIBoxPhone;
    quint8 nIBoxBell;
};

/* This structure contains list of the last audio sources recently used on the main channel. */
struct SLastAudioList
{
    TAudioMode eAudioModeList[NUM_OF_LAST_AUDIO];
};

/**
* \brief The Namespace for Resource Policy.
* All Resource Policy Qt APIs reside under the ResourcePolicy namespace.
*/

class QRPMClient: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(QRPMClient)
public:
    /**
    * Alternative constructor with additional parameters for setting
    * alwaysReply and autoRelease.
    * \param applicationClass This parameter defines the application class.
    * The application class is used to determine the priority order of the
    * application.
    * \param parent The parent of this class.
    */

    QRPMClient(const QString &applicationClass, QObject *parent);
    QRPMClient();

    QRPMClient(QString *);
    /**
    * The destructor
    */
    ~QRPMClient();

    /**
    * Acquire audio resource
    */
	TRPMClientRequestStatus RPMClientAcquire(TAudioMode eAudioMode, TAudioSink eAudioSink, bool bMuteMode);

    /**
    * Mix audio
    */
    TRPMClientRequestStatus RPMClientMix(TAudioMixChannel eAudioChannel, TOnOff eOnOff);

    /**
    * Set audio volume
    */
    TRPMClientRequestStatus RPMClientSetAudioVolume(TVolumeMode eVolumeMode, qint8 nVolumeLevel);

    /**
    * Get audio volume information
    */

    TRPMClientRequestStatus RPMClientGetAudioVolumeInfo();

    /**
    * Set audio setup value
    */
    TRPMClientRequestStatus RPMClientSetAudioSetupValue(TAudioSetup eAudioSetupValue,qint8 value);


    /**
    * Get audio setup information
    */
    TRPMClientRequestStatus RPMClientGetAudioSetupInfo();

    /**
    * set beep audio
    */
    TRPMClientRequestStatus RPMClientAudioBeepPlay(TAudioBeepType eAudioBeepType);

    /**
    * Audio mute control
    */
    TRPMClientRequestStatus RPMClientAudioMuteControl(TAudioSink eAudioSink, TMuteControl eMuteUnmute);

    /**
    * Get last audio list information
    */

    TRPMClientRequestStatus RPMClientGetLastAudioList();

/**
    * Save last audio list 
 */	
    void SendLastAudioList(SLastAudioList lastAudioList, int count);

    /**
    * Acquire display resource
    */
    TRPMClientRequestStatus RPMClientAcquireDisplay(TDisplayResourceType eDisplayResource, TDisplayResourceUser eDisplayUser);

	void InitAudioResponseConnection();
public slots:
    void MuteAudioResponseSlot(QByteArray reply);
    void MainAudioResponseSlot(QByteArray reply);
    void MixAudioResponseSlot(QByteArray reply);
    void LastAudioResponseSlot(QByteArray reply);
    void DBusMicomResponseHandler(QDBusMessage reply);
	void ErrorResponseHandlerMainAudio( void );
	void ErrorResponseHandlerSubAudio( void );
	void ErrorResponseHandlerMixAudio( void );
	void ErrorResponseHandlerVolumeInfo( void );
	void ErrorResponseHandlerSetupInfo( void );
	void ErrorResponseHandlerLastAudio( void );
    void DBusMicomSignalHandler(QDBusMessage signal);
    void DBusASMSignalHandler(QDBusMessage );

signals:
    void MICOMVolumeStepSignal(TVolumeMode eVolumeMode, qint8 nVolumeValue);
    void RPMClientAcquireResponse(TAudioMode eAudioMode, TAudioSink eAudioSink);
    void RPMClientGetVolumeResponse(SAudioVolumeInfo sAudioVolumeInfo);
    void RPMClientGetAudioSetupResponse(SAudioSetupInfo sAudioSetupInfo);
    void RPMClientGetLastAudioListResponse(SLastAudioList sLastAudioList);
    void ASMModeChangeSignal();
    void ASMMixingInfoSignal(qint8);
    void ASMCallInProgressSignal();
    void VRNotFesibleSignal();

	void RPMClientAvModeResponse(TAudioMode eAudioMode, TAudioSink eAudioSink);
	void RPMClientMixResponse(TAudioMixChannel eAudioChannel, bool bResult);
	void RPMClientMuteResponse(EAudioSink eMuteMode, bool bResult);
	void RPMClientGetVolumeInfo(SAudioVolumeInfo sAudioVolumeInfo);

private:
    TRPMClientRequestStatus AudioControl(TControlType nCommand, void *Data);
    void SendAcquireResponsetoApp(QList<QVariant> list,TAudioSink eAudioSink);
    void SendVolumeGetResponsetoApp(QList<QVariant> list);
    void SendGetAudioSetupResponsetoApp(QList<QVariant> list);
    void SendLastAudioResponsetoApp(QList<QVariant> list);

	void SendAvModeResponseToASM(QList<QVariant> list, TAudioSink eAudioSink);
	void SendMixResponseToASM(QList<QVariant> list);
	void SendMuteResponseToASM(QList<QVariant> list);
	void SendVolumeInfoToASM(QList<QVariant> list);

	quint8 MapToAudioChannelFromAudioMode( EAudioMode ucMicomMode );
	EAudioMode MapToAudioModeFromAudioChannel( quint8 i8AudioChannel );
	quint8 MapToMuteChannelFromMuteMode( EAudioSink ucMuteMode );
	void WriteLog( QString strLog );

private:
    GstElement *front_display_pipeline;
    GstElement *front_video_src;
    GstElement *front_csp;
    GstElement *front_mqueue;
    GstElement *front_deinterlace;
    GstElement *front_capsfilter;
    GstElement *front_mainvideosink;
    GstBus *front_bus;
    GstElement *rear_display_pipeline;
    GstElement *rear_video_src;
    GstElement *rear_csp;
    GstElement *rear_mqueue;
    GstElement *rear_deinterlace;
    GstElement *rear_capsfilter;
    GstElement *rear_mainvideosink;
    GstBus *rear_bus;
    QDBusInterface *m_pAudio_Chennel_Interface;
};

#endif

