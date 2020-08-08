/*
 ******************************************************************
 * COPYRIGHT © <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER : Venkatesh Aeturi                                  *
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
 *                  This file contains the class definition for   *
 *                  AuxIn Fblock control, responsible to          *
 *                  create instance of Fblock, configure it and   *
 *                  add to most stack. if any listeners will      *
 *                  be part of this class.Communicates with ACS   *
 *                  daemon.                                       *
 *                                                                *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT                  *
 ******************************************************************
 * 09 Aug 2011   Venkatesh Aeturi         Draft version
 * 10 Aug 2011   Venkatesh Aeturi         MOdified for comments
 * 15 Sep 2011   Amit Koparde             Added Log_Tag define    *
 * 27 Sep 2011   Amit Koparde             Added new API CurrentSongInfo and TerminalMOde to CAuxInFblockControl                                                     *
 * 20 Jan 2012   Neelima Khedad            Updated API to support for version 1.13.0
 * 17 Jul 2012  Shiva Kumar              Modified for FCAT 1.18.2
 ******************************************************************
 */

#ifndef DHAVN_MOSTMANAGER_AUXINFBLOCKCONTROL_H
#define DHAVN_MOSTMANAGER_AUXINFBLOCKCONTROL_H

#include <QtCore>
#include <QtDBus>

#include <DHAVN_LogUtil.h>

#include "DHAVN_MOSTManager_Config.h"
#include "DHAVN_MOSTManager_ACSApplication.h"
#include "DHAVN_MOSTManager_AuxInFBlock.h"


using namespace acs::generated::fblock::AuxIn;
using namespace k2l::acs::protocols::most;

#define LOG_TAG_AUX_CNT "AuxInControl"

#define AUX_ONE 1
#define AUX_TWO 2


class CAuxInFblockControl : public QObject, public CControlThreadBase
{    
    Q_OBJECT

private:
    /** Pointer Object of Tuner Fblock of generated code */
    CAuxIn *m_AuxInFblock_1;
    CAuxIn *m_AuxInFblock_2;    

public:
    CAuxInFblockControl(CMarshaledACSApplication *context);
    ~CAuxInFblockControl();

    virtual bool Initialize();
    virtual bool Register();
    virtual bool UnRegister();
    bool FBlockVisible();

    bool AuxDeviceInfo(const uint devicenum, const uint deviceclass, const uint devicetype, const QString &devicename, const uint auxIn_no);

};

#endif // DHAVN_MOSTMANAGER_AMFMTUNERFBLOCKCONTROL_H
