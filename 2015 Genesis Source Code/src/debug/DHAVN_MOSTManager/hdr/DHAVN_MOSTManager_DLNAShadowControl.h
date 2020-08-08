/*
 ******************************************************************
 * COPYRIGHT © <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER :  Neelima Khedad                                   *
 * DATE       :  1 Aug 2011                                       *
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
 *                  DLNA Tuner Shadow control, responsible to     *
 *                  create instance of Shadow, configure it and   *
 *                  add to most stack. if any listeners will      *
 *                  be part of this class.Communicates with ACS   *
 *                  daemon.                                       *
 *                                                                *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 * 01 Aug 2011  Neelima Khedad           Draft version
 * 08 Aug 2011  Neelima Khedad           Added namespaces for instance of generated code
 * 15 Sep 2011  Amit Koparde             Added Log_Tag define     *
 * 12 Oct 2011  Neelima Khedad           Added API SetTrackposition
 * 21 Jun 2012  Shiva Kumar              Modified for FCAT 1.17.1, FCAT 1.17.2 and FCAT 1.17.3
 * 26 Jul 2012  Shiva Kumar              Modified for CR 12357
 ******************************************************************
 */

#ifndef DHAVN_MOSTMANAGER_DLNASHADOWCONTROL_H
#define DHAVN_MOSTMANAGER_DLNASHADOWCONTROL_H

#include <QtCore>
#include <DHAVN_LogUtil.h>

#include "DHAVN_MOSTManager_Config.h"
#include "DHAVN_MOSTManager_ACSApplication.h"
#include "DHAVN_MOSTManager_DLNAShadow.h"


#define LOG_TAG_DLNA_CNT "DLNAControl"


using namespace acs::generated::shadow::DLNA;
using namespace k2l::acs::protocols::most;


class CDLNAShadowControl;
/** Listener for the property Time Position */
class CTimePostionListener : public IACSMOSTEventListener
{
    CDLNAShadowControl* m_CDLNAShadowControl;

public:
    CTimePostionListener(CDLNAShadowControl* context);
    virtual void OnChange();
};

/** Listener for the property DLNA Connection status*/
class CDLNAConnectionListener : public IACSMOSTEventListener
{
    CDLNAShadowControl* m_CDLNAShadowControl;

public:
    CDLNAConnectionListener(CDLNAShadowControl* context);
    virtual void OnChange();
};
/** Listener for the property Current Song info */
class CCurrentSongInfoListener : public IACSMOSTEventListener
{
    CDLNAShadowControl* m_CDLNAShadowControl;

public:
    CCurrentSongInfoListener(CDLNAShadowControl* context);
    virtual void OnChange();
};
/** Listener for the property Current video info */
class CCurrentVideoInfoListener : public IACSMOSTEventListener
{
    CDLNAShadowControl* m_CDLNAShadowControl;

public:
    CCurrentVideoInfoListener(CDLNAShadowControl* context);
    virtual void OnChange();
};
/** Listener for the property DeckStatus info */
class CDeckStatusListener : public IACSMOSTEventListener
{
    CDLNAShadowControl * m_CDLNAShadowControl;
public:
    CDeckStatusListener(CDLNAShadowControl* context);
    virtual void OnChange();
};
/** Listener for the property TrackPosition info */
class CTrackPositionListener : public IACSMOSTEventListener
{
    CDLNAShadowControl * m_CDLNAShadowControl;
public:
    CTrackPositionListener(CDLNAShadowControl* context);
    virtual void OnChange();
};

class CTrackInformationListener : public IACSMOSTEventListener
{
    CDLNAShadowControl * m_CDLNAShadowControl;
public:
    CTrackInformationListener(CDLNAShadowControl* context);
    virtual void OnChange();
};

class CDLNAShadowControl : public QObject, public CControlThreadBase
{
    Q_OBJECT

private:

    /** Pointer Object of DLNA Shadow of generated code */
    CDLNA *m_DLNAShadow;

    /** Pointer Objects of properties of DLNA along with the listeners */
    CTimePosition *m_TimePostion;
    CTimePostionListener *m_TimePostionListener;    

    CDLNAConnection *m_DLNAConnection;
    CDLNAConnectionListener *m_DLNAConnectionListener;

    CCurrentSongInfo *m_CurrentSongInfo;
    CCurrentSongInfoListener *m_CurrentSongInfoListener;

    CCurrentVideoInfo *m_CurrentVideoInfo;
    CCurrentVideoInfoListener *m_CurrentVideoInfoListener;

    CDeckStatus *m_DeckStatus;
    CDeckStatusListener *m_DeckStatusListener;

    CTrackPosition *m_TrackPosition;
    CTrackPositionListener *m_TrackPositionListener;

    CTrackInformation *m_TrackInformation;
    CTrackInformationListener *m_TrackInformationListener;    

public:
    CDLNAShadowControl(CMarshaledACSApplication *context);
    ~CDLNAShadowControl();

    virtual bool Initialize();

    void SetListeners();
    void RemoveListeners();

    bool FBlockVisible();

    /** Below are the functions called by OnChangeof respective listener */
    void TimePositionListener();   
    void DLNAConnStatusListener();
    void CurrentSongInfoListener();
    void CurrentVideoInfoListener();
    void DeckStatusListener();
    void TrackPositionListener();
    void TrackInformationListener();

    /** Below slots are called by the Application */
    bool SetDeckStatus(const uint deckStatus);
    bool SetTrackPosition(const uint trackPosition);
    bool SetIncrementTrackPosition(const uint nSteps);
    bool SetDecrementTrackPosition(const uint nSteps);

signals:
    /** Below are the Signals sent to CDLNAAdaptor */
    void TimePosition(const int diskTime, const int trackTime, const int titleTime);   
    void DLNAConnStatus(const uint status);
    void CurrentSongInfo(const uint playtime, const ushort trackno, const QStringList songinfo);
    void CurrentVideoInfo(const QString vtitle, const uint time, const ushort track, const QString vfilename);
    void Deckstatus(const uint deckStatus);
    void TrackPosition(const uint trackPosition);
    void TrackInformation(const uint currentNumTrack, const uint currentRelativeTrackPos);

};



#endif // DHAVN_MOSTMANAGER_DLNASHADOWCONTROL_H

