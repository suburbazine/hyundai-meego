/*
 ******************************************************************
 * COPYRIGHT © <2011>,
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER :  Shiva Kumar                                      *
 * DATE       :  12 May 2012                                     *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 * MODULE SUMMARY : MOST Manager is a middle ware component which *
 *                  talks to the applications and controller over *
 *                  QtDbus.The data sent over QtDbus is further   *
 *                  communicated to other MOST devices through ACS*
 *                  daemon.This module is also responsible for    *
 *                  commands from ACS daemon.                     *
 *                  This file contains the class definitions for  *
 *                  HMI ModeChange, Visibility and Audibility     *
 *                  responsible to provide the custom class for   *                                          *
 *                  HMI property ModeStatus, SystemVisibility and *
 *                  SystemAudibility of HMI FBlock in order to    *
 *                  block the status sent from acsdaemon on       *
 *                  receiving Set message                         *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT                  *
 ******************************************************************
 * 12 May 2012  Shiva Kumar              Draft version            *
 ******************************************************************
 */
#ifndef DHAVN_MOSTMANAGER_HMIMODESTATUS_H
#define DHAVN_MOSTMANAGER_HMIMODESTATUS_H

#include <QtCore>
#include <QtDBus>

#include <stdint.h>

#include <K2LSystem.h>
#include <ACSStreams.h>
#include <ACSMessage.h>
#include <K2LACSClient.h>
#include <K2LACSMOST.h>
#include <K2LACSEvents.h>

#include <DHAVN_LogUtil.h>

#include "DHAVN_MOSTManager_ACSApplication.h"
#include "DHAVN_MOSTManager_MOSTStructures.h"

#define LOG_TAG_HMIMODE_CNT "HMIIBOXtoHUModeChange"

using namespace k2l::acs::protocols::most;

class CModeStatusCustomClass : public QObject, public k2l::acs::protocols::most::CMOSTFunctionClassArray
{
    Q_OBJECT
    USE_LOG_UTIL
public:
    CModeStatusCustomClass();
    ~CModeStatusCustomClass();
public slots:
    void ModeStatusfromIBOXSlot(const uint target,const uint mode);
    void SetOperation(uint32_t flags, uint32_t messageID, DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload);
signals:
    void ModeStatusfromIBOX(const uint target, const uint mode);
};

class CSystemVisibilityCustomClass : public QObject, public k2l::acs::protocols::most::CMOSTFunctionClassArray
{
    Q_OBJECT
    USE_LOG_UTIL
public:
    CSystemVisibilityCustomClass();
    ~CSystemVisibilityCustomClass();
public slots:
    void SystemVisibilityfromIBOXSlot(const uint target,const uint source);
    void SetOperation(uint32_t flags, uint32_t messageID, DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload);
signals:
    void SystemVisibilityfromIBOX(const uint target, const uint source);
};

class CSystemAudibilityCustomClass : public QObject , public k2l::acs::protocols::most::CMOSTFunctionClassArray
{
    Q_OBJECT
    USE_LOG_UTIL
public:
    CSystemAudibilityCustomClass();
    ~CSystemAudibilityCustomClass();
public slots:
    void SystemAudibilityfromIBOXSlot(const uint target,const uint source);
    void SetOperation(uint32_t flags, uint32_t messageID, DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload);
signals:
    void SystemAudibilityfromIBOX(const uint target, const uint source);
};

#endif // DHAVN_MOSTMANAGER_HMIMODESTATUS_H
