/*
 ******************************************************************
 * COPYRIGHT © 2011, 
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
 
#ifndef DHAVN_RPMCLIENT_INTERNAL_H
#define DHAVN_RPMCLIENT_INTERNAL_H

#include "DHAVN_QRPMClient.h"
#include<QtGlobal>

const quint16 c_CntlAcquireMainCommand = 0x0110;
const quint16 c_CntlAcquireSubCommand = 0x0111;
const quint16 c_CntlMixCommand = 0x0112;
const quint16 c_CntlVolumeSetCommand = 0x0113;
const quint16 c_CntlVolumeInfoGetCommand = 0x0211;
const quint16 c_CntlAudioSetupValueSetCommand = 0x0114;
const quint16 c_CntlAudioSetupInfoGetCommand = 0x0210;
const quint16 c_CntlAudioBeepCommand = 0x0115;
const quint16 c_CntlMuteCommand = 0x0116;
const quint16 c_CntlLastAudioCommand = 0x0212;
const quint16 c_CntlLastAudioListCommand = 0x0119;


const quint16 c_ResAcquireMainCommand = 0x8110;
const quint16 c_ResAcquireSubCommand = 0x8111;
const quint16 c_ResMixCommand = 0x8112;
const quint16 c_ResMuteCommand = 0x8116;
const quint16 c_ResVolumeInfoGetCommand = 0x8211;
const quint16 c_ResAudioSetupInfoGetCommand = 0x8210;
const quint16 c_ResLastAudioCommand = 0x8212;

const quint16 c_ResVolumeStepUpSignal = 0x8310;

struct CntlAcquireArg
{
    quint8 sourceId;
	quint8 muteMode;
};

struct CntlMixArg
{
    TAudioMixChannel audioChannel;
    TOnOff onOff;
};

struct CntlSetVolumeArg
{
    qint8 sourceId;
    qint8 volume;
};

struct CntlAudioSetupArg
{
    TAudioSetup type;
    qint8 value;
};

struct CntlMuteArg
{
    qint8 audioSink;
    TMuteControl muteUnmute;
};

struct AUDIO_MODE_TO_AUDIO_CHANNEL
{
	EAudioMode ucMicomMode;
	quint8 i8AudioChannel;
};

static const AUDIO_MODE_TO_AUDIO_CHANNEL AUDIO_MODE_TO_AUDIO_CHANNEL_MAP[] =
{
	{eAudioNone,			0xFF},
	{eAVOff,				0x00},
	{eFM1,					0x10},
	{eFM2,					0x11},
	{eAM,					0x12},
	{eDeckAudioI2S,			0x20},
	{eDeckAudioSPDIF,		0x21},
	{eDeckVideoI2S,			0x22},
	{eDeckVideoSPDIF,		0x23},
	{eUSB1AudioI2S,			0x30},
	{eUSB1AudioSPDIF,		0x31},
	{eUSB1VideoI2S,			0x32},
	{eUSB1VideoSPDIF,		0x33},
	{eUSB2AudioI2S,			0x34},
	{eUSB2AudioSPDIF,		0x35},
	{eUSB2VideoI2S,			0x36},
	{eUSB2VideoSPDIF,		0x37},
	{eUSB1MirLinkI2S,		0x38},
	{eUSB1MirLinkSPDIF,		0x39},
	{eUSB2MirLinkI2S,		0x3A},
	{eUSB2MirLinkSPDIF,		0x3B},
	{eIPOD1I2S,				0x40},
	{eIPOD1SPDIF,			0x41},
	{eIPOD1PandoraI2S,		0x42},
	{eIPOD1PandoraSPDIF,	0x43},
	{eIPOD2I2S,				0x44},
	{eIPOD2SPDIF,			0x45},
	{eIPOD2PandoraI2S,		0x46},
	{eIPOD2PandoraSPDIF,	0x47},
	{eIPOD1AhaRadioI2S,		0x48},
	{eIPOD1AhaRadioSPDIF,	0x49},
	{eIPOD2AhaRadioI2S,		0x4A},
	{eIPOD2AhaRadioSPDIF,	0x4B},
	{eVCDCI2S,				0x50},
	{eVCDCSPDIF,			0x51},
	{eJukeBoxAudioI2S,		0x60},
	{eJukeBoxAudioSPDIF,	0x61},
	{eJukeBoxVideoI2S,		0x62},
	{eJukeBoxVideoSPDIF,	0x63},
	{eVideoGuideI2S,		0x64},
	{eVideoGuideSPDIF,		0x65},
	{eBTAudio,				0x70},
	{eBTPandora,			0x71},
	{eBTMirlink,			0x72},
	{eBTAhaRadio,			0x73},
	{eAUX1Audio,			0x80},
	{eAUX1Video,			0x81},
	{eAUX2Audio,			0x82},
	{eAUX2Video,			0x83},
	{eDMB1,					0x90},
	{eDMB2,					0x91},
	{eCMMB1,				0xA0},
	{eCMMB2,				0xA1},
	{eXM1,					0xB0},
	{eXM2,					0xB1},
	{eDAB1,					0xC0},
	{eDAB2,					0xC1},
	{eDAB3,					0xC2},
	{eHDRadio,				0xD0},
	{eIBoxDLNA,				0xD1},
	{eIBoxWebPlayer,		0xD2},
	{eIBoxVR,				0xD3},
	{eTTS,					0xD4},
	{eHDRFM1,				0xD5},
	{eHDRFM2,				0xD6},
	{eHDRAM,				0xD7},
	{eRDSTA,				0xE0},
	{eRDSNews,				0xE1},
	{eRDSAlarm,				0xE2},
	{eDABTA, 				0xE3},
	{eInteractiveGuide,		0xE5},
	{eAVMute,				0xFE},
	{eDABTAAlarm,       0xE3},
	{eMax,					0xFF}
};

struct MUTE_MODE_TO_MUTE_CHANNEL
{
	EAudioSink ucMuteMode;
	quint8 i8MuteChannel;
};

static const MUTE_MODE_TO_MUTE_CHANNEL MUTE_MODE_TO_MUTE_CHANNEL_MAP[] =
{
	{eAMP,				0x00},
	{eMain,				0x01},
	{eSecondary,		0x02},
	{eMixer, 			0x03},
	{eSWheelKeyMute,	0x04},
	{eMainMuteForPop,	0x05},
	{eUnmuteNoVolume,	0x06}
};

#endif // DHAVN_RPMCLIENT_INTERNAL_H
