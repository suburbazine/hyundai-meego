/*
 ******************************************************************
 * COPYRIGHT © <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER :  Venkatesh                                        *
 * DATE       :  25 Aug 2011                                      *
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
 *                  SpeechRecognition FBlock control, responsible *
 *                  to create instance of and Shadow(0x01)        *
 *                  configure it and add to most stack.           *
 *                  Listeners and handlers will be part of this   *
 *                  class.                                        *
 *                  Communicates with ACS daemon                  *
 *                                                                *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                         *
 *
 *   DATE         PROGRAMMER               COMMENT                *
 ******************************************************************
 * 25 Aug 2011  Venkatesh Aeturi          Draft version           *
 * 15 Sep 2011  Amit Koparde              Added Log_Tag define    *
 * 21 Jun 2012  Shiva Kumar              Modified for FCAT 1.17.1, FCAT 1.17.2 and FCAT 1.17.3
 ******************************************************************
 */

#ifndef DHAVN_MOSTMANAGER_SPEECHFBLOCKCONTROL_H
#define DHAVN_MOSTMANAGER_SPEECHFBLOCKCONTROL_H

#include <QtCore>
#include <DHAVN_LogUtil.h>

#include "DHAVN_MOSTManager_Config.h"
#include "DHAVN_MOSTManager_ACSApplication.h"
#include "DHAVN_MOSTManager_SpeechRecognitionFBlock.h"


#define LOG_TAG_SPREC_CNT "SpeechRecognitionControl"

using namespace acs::generated::fblock::SpeechRecognition;
using namespace k2l::acs::protocols::most;


class CSpeechRecognitionFblockControl;

class CSpeechHandler : public IFunctionBlockMethodHandler
{

public:
    CSpeechRecognitionFblockControl* m_CSpeechRecognitionFblockControlContext;

    CSpeechHandler(CSpeechRecognitionFblockControl* context);

    /** Callback for received StartResultAck OpType */
    void StartResultAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload);

    void GetInterface(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload) ;

    /** Callback for received Start OpType */
    void Start(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload) ;

    /** Callback for received Abort OpType */
    void Abort(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload);

    /** Callback for received StartResult OpType */
    void StartResult(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload) ;


    /** Callback for received AbortAck OpType */
    void AbortAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload) ;

    /** Callback for received StartAck OpType */
    void StartAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload) ;
};

class CSpeechRecognitionFblockControl : public QObject, public CControlThreadBase
{
    Q_OBJECT

private:

    CSpeechRecognition *m_SpeechRecognitionFblock;

    acs::generated::fblock::SpeechRecognition::CTextToSpeech *m_TTSData;
    CSpeechHandler *m_CMethodHandler;

    acs::generated::fblock::SpeechRecognition::CManualInput *m_ManualInput;

public:
    CSpeechRecognitionFblockControl(CMarshaledACSApplication *m_AppContext);
    ~CSpeechRecognitionFblockControl();

    bool Initialize();
    bool FBlockVisible();

    bool SetTTSResult(const uint senderHandle, const uint length, const uint deviceid, const uint state);
    bool SetManualInput(const uint inputTarget, const QString inputKeyWord);
    void TTSDataHandler(const quint32 sourceDeviceID, const quint32 payloadLength, const uint8_t *payload, const uint state);

    /** Method Invoked by handler class methods */
    void SpeechHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state);

signals:
    void StartTTSData(const uint sourceDeviceID, const uint senderHandle, const uint length, const QByteArray ttsData);
    void AbortTTS(const uint sourceDeviceID, const uint senderHandle);
};

#endif // DHAVN_MOSTMANAGER_SPEECHFBLOCKCONTROL_H
