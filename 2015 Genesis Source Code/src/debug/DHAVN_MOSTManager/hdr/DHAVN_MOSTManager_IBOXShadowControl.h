/*
 ******************************************************************
 * COPYRIGHT © <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER :  Neelima Khedad                                   *
 * DATE       :  29 Aug 2011                                      *
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
 *                  Ibox Shadow control, responsible to           *
 *                  create instance of and Shadow(0x01)           *
 *                  configure it and add to most stack.           *
 *                  Listeners and handlers will be part of this   *
 *                  class.                                        *
 *                  Communicates with ACS daemon                  *
 *                                                                *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT                  *
 ******************************************************************
 * 29 Aug 2011  Neelima Khedad           Draft version            *
 * 15 Sep 2011  Amit Koparde             Added Log_Tag define     *
 * 12 Oct 2011  Neelima Khedad           Updated structures for   *
 *                                       GISData and POIdata      *
 * 27 Oct 2011  Neelima Khedad           Adedd parameter poicount to Signal POIDataAvailable
 * 04 Apr 2012  Neelima Khedad           Added Signal POFDataAvailable to support fcat 1.15
 * 07 May 2012  Shiva Kumar              Added new signal GISDataAvailableError
 * 21 Jun 2012  Shiva Kumar              Modified for FCAT 1.17.1, FCAT 1.17.2 and FCAT 1.17.3
 * 17 Jul 2012  Shiva Kumar              Modified for FCAT 1.18.2
 * 26 Jul 2012  Shiva Kumar              Modified for CR 12089
 * 19 Sep 2012  Shiva Kumar              Modified for FCAT 1.19.0, 1.20.1, 1.20.2
 * 15 Nov 2012  Shiva Kumar              Modified for CR 15573
 * 19 Mar 2013  Hanhong Keum             Modified for FCAT 1.22.7
 ******************************************************************
 */


#ifndef DHAVN_MOSTMANAGER_IBOXSHADOWCONTROL_H
#define DHAVN_MOSTMANAGER_IBOXSHADOWCONTROL_H

#include <QtCore>
#include <QtDBus>
#include <QBuffer>
#include <QSharedMemory>

#include <DHAVN_LogUtil.h>

#include "DHAVN_MOSTManager_Config.h"
#include "DHAVN_MOSTManager_ACSApplication.h"
#include "DHAVN_MOSTManager_IBOXShadow.h"
#include "DHAVN_MOSTManager_MOSTStructures.h"
#include "MOSTErrorInfo.h"

#define LOG_TAG_IBOX_CNT "IBoxControl"

using namespace k2l::acs::samples;
using namespace k2l::acs::protocols::most;
using namespace acs::generated::shadow::IBox;

class CIBOXShadowControl;


/** Listener for property Wifi Mode of Ibox */
class CWifiModeListener : public IACSMOSTEventListener
{
    CIBOXShadowControl* m_IBOXShadowControl;

public:
    CWifiModeListener(CIBOXShadowControl* context);
    virtual void OnChange();
};

/** Listener for property AppInstallStatus of Ibox */
class CAppInstallStatusListener : public IACSMOSTEventListener
{
    CIBOXShadowControl* m_IBOXShadowControl;

public:
    CAppInstallStatusListener(CIBOXShadowControl* context);
    virtual void OnChange();
};


/** Listener for property GISDataAvailable of Ibox */
class CGISDataAvailableListener : public IACSMOSTEventListener
{
    CIBOXShadowControl* m_IBOXShadowControl;

public:
    CGISDataAvailableListener(CIBOXShadowControl* context);
    virtual void OnChange();
    virtual void OnError();

};

/** Handler For property Open Browser */
class COpenBrowserHandler : public IShadowMethodHandler
{
    CIBOXShadowControl* m_IBOXShadowControl;

public:
    COpenBrowserHandler(CIBOXShadowControl* context);

    /** Virtual functions of IShadowMethodHandler */
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};

/** Handler For property GIS Request */
class CGISRequestHandler : public IShadowMethodHandler
{
    CIBOXShadowControl* m_IBOXShadowControl;

public:
    CGISRequestHandler(CIBOXShadowControl* context);

    /** Virtual functions of IShadowMethodHandler */
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};

/** Handler For property GIS Data Request */
class CGISDataRequestHandler : public IShadowMethodHandler
{
    CIBOXShadowControl* m_IBOXShadowControl;

public:
    CGISDataRequestHandler(CIBOXShadowControl* context);

    /** Virtual functions of IShadowMethodHandler */
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};

/** Listener for property POIDataAvailable of Ibox */
class CPOIDataAvailableListener : public IACSMOSTEventListener
{
    CIBOXShadowControl* m_IBOXShadowControl;

public:
    CPOIDataAvailableListener(CIBOXShadowControl* context);
    virtual void OnChange();
};


/** Handler For property POI Data Request */
class CPOIDataRequestHandler : public IShadowMethodHandler
{
    CIBOXShadowControl* m_IBOXShadowControl;

public:
    CPOIDataRequestHandler(CIBOXShadowControl* context);

    /** Virtual functions of IShadowMethodHandler */
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};

/** Handler For property VoiceSearchOnline Data Request */
class CVoiceSearchOnlineHandler : public IShadowMethodHandler
{
    CIBOXShadowControl* m_IBOXShadowControl;

public:
    CVoiceSearchOnlineHandler(CIBOXShadowControl* context);

    /** Virtual functions of IShadowMethodHandler */
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
};

/** Listener For property LSDataAvailable Data Request */
class CLSDataAvailableListener : public IACSMOSTEventListener
{
    CIBOXShadowControl* m_IBOXShadowControl;

public:
    CLSDataAvailableListener(CIBOXShadowControl* context);
    virtual void OnChange();
    virtual void OnError();
};

/** Handler For method LSRequest Data Request */
class CLSRequestHandler : public IShadowMethodHandler
{
    CIBOXShadowControl* m_IBOXShadowControl;

public:
    CLSRequestHandler(CIBOXShadowControl* context);

    /** Virtual functions of IShadowMethodHandler */
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
};

/** Handler For method LSDataRequest Data Request */
class CLSDataRequestHandler : public IShadowMethodHandler
{
    CIBOXShadowControl* m_IBOXShadowControl;

public:
    CLSDataRequestHandler(CIBOXShadowControl* context);

    /** Virtual functions of IShadowMethodHandler */
    virtual void ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    virtual void Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    virtual void ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    virtual void Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    virtual void Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
};

/** Listener For property ACNActivated Data of Ibox */
class CACNActivatedListener : public IACSMOSTEventListener
{
    CIBOXShadowControl* m_IBOXShadowControl;

public:
    CACNActivatedListener(CIBOXShadowControl* context);
    virtual void OnChange();
};


/** Listener for property POFDataAvailable of Ibox */
class CPOFDataAvailableListener : public IACSMOSTEventListener
{
    CIBOXShadowControl* m_IBOXShadowControl;

public:
    CPOFDataAvailableListener(CIBOXShadowControl* context);
    virtual void OnChange();
};

/** Listener for property ReceivedPOI of Ibox */
class CReceivedPOIListener : public IACSMOSTEventListener
{
    CIBOXShadowControl* m_IBOXShadowControl;

public:
    CReceivedPOIListener(CIBOXShadowControl* context);
    virtual void OnChange();
};
/** structure used for storing VoiceSearchOnline data for IBOX Adaptor*/
struct WavBlock{
public:
    int nBlockType; //0: start 1: inline 2: end
    int nBlockCounter;
    int nBlockSize;//szWavBuffer size
    char szWavBuffer[MAX_WAV_BUFFER_SIZE];
public:
    WavBlock();
};

struct WavBlockStructure{
public:
    int nTotalBlockNumber; //Total number of block
    WavBlock m_xWavBlock[MAX_WAV_FRAME];
public:
    WavBlockStructure();
};

/** Listener for checking visibility of IBOX FBlock */
class CIBOXFblockStateListener : public IACSMOSTEventListener
{
    CIBox *m_Ibox;
    CIBOXShadowControl* m_IBOXShadowControl;
public:
    CIBOXFblockStateListener(CIBox *iboxContext,CIBOXShadowControl* context);
    virtual void OnChange();
};

/** Class definition */
class CIBOXShadowControl: public QObject, public CControlThreadBase
{
    Q_OBJECT

private:

    /** Pointer Object of IBox Shadow of generated code */
    CIBox *m_IBoxShadow;

    /** Listeners and Handlers for properties of IBox */
    CWiFiMode *m_WiFiMode;
    CWifiModeListener *m_WifiModeListener;

    CAppInstallStatus *m_AppInstallStatus;
    CAppInstallStatusListener *m_AppInstallStatusListener;    

    CGISDataAvailable *m_GISDataAvailable;
    CGISDataAvailableListener *m_GISDataAvailableListener;

    COpenBrowser *m_OpenBrowser;
    COpenBrowserHandler *m_OpenBrowserHandler;

    CGISRequest *m_GISRequest;
    CGISRequestHandler *m_GISRequestHandler;

    CGISDataRequest *m_GISDataRequest;
    CGISDataRequestHandler *m_GISDataRequestHandler;

    CPOIDataAvailable *m_POIDataAvailable;
    CPOIDataAvailableListener *m_POIDataAvailableListener;

    CPOIDataRequest *m_POIDataRequest;
    CPOIDataRequestHandler *m_POIDataRequestHandler;

    CReceivedPOF *m_POFDataAvailable;
    CPOFDataAvailableListener *m_POFDataAvailableListener;

    CReceivedPOI *m_ReceivedPOI;
    CReceivedPOIListener *m_ReceivedPOIListener;

    CVoiceSearchOnline *m_VoiceSearchOnline;
    CVoiceSearchOnlineHandler *m_VoiceSearchOnlineHandler;

    CLSDataAvailable *m_LSDataAvailable;
    CLSDataAvailableListener *m_LSDataAvailableListener;

    CLSRequest *m_LSRequest;
    CLSRequestHandler *m_LSRequestHandler;

    CLSDataRequest *m_LSDataRequest;
    CLSDataRequestHandler *m_LSDataRequestHandler;

    CACNActivated *m_ACNActivated;
    CACNActivatedListener *m_ACNActivatedListener;

    QSharedMemory m_xWavBlockStructureSharedMem;
    WavBlockStructure m_xWavBlockStructure;

    CIBOXFblockStateListener *m_IBoxFblockStateListener;

    int m_numberOfBlock;


public:
    CIBOXShadowControl(CMarshaledACSApplication *context);
    ~CIBOXShadowControl();

    virtual bool Initialize();

    void SetListeners();
    void RemoveListeners();
    bool FBlockVisible();

    /** Below API's are invoked by CIBOXAdaptor */
    bool StartOpenBrowserRequest(const uint senderhandle,const uint target, const uint opentype, const QString text);
    bool StartGISRequest(const SGISRequestData gisRequestData);
    bool StartGISDataRequest(const SGISRequestData gisDataRequestData);
    bool StartPOIDataRequest(const SPOIDataRequest poiDataRequest);
    bool SetVoiceSearchOnline(const uint voiceSearch);
    bool SendLSRequest(const uchar optype, const ushort senderHandle, const QString uri, const QByteArray lsdata);
    bool SendLSDataRequest(const uchar optype, const ushort senderHandle, const uchar target, const QString uri);

    /** Below APis are invoked by Listeners and handlers */
    void WifiMode();
    void AppInstallStatus();   
    void GISDataAvailable();
    void ErrorGISDataSlot();
    void OpenBrowserRequestResult(const uint state, const uint32_t payloadLength, const uint8_t *payload);
    void GISRequestResult(const uint state, const uint32_t payloadLength, const uint8_t *payload);
    void GISDataRequestResult(const uint state, const uint32_t payloadLength, const uint8_t *payload);
    void POIDataAvailable();
    void ReceivedPOI();
    void POFDataAvailable();
    void IBOXFunctionBlockInvisibleIBOX();
    void POIDataRequestResult(const uint state, const uint32_t payloadLength, const uint8_t *payload);
    void OpenBrowserHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state);
    void GISRequestHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state);
    void GISDataRequestHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state);
    void POIDataRequestHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state);
    void VoiceSearchOnlineHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state);
    void LSDataAvailable(const uchar optype);
    void LSRequestHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uchar optype);
    void LSDataRequestHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uchar optype);
    void ACNActivated();

signals:
    /** Below are the signals sent to Adaptor class */    
    void WifiMode(const uint mode,const uint clientStatus);
    void AppInstallStatus(const uint appStatus, const uint appProgress);       
    void GISDataAvailable(const QString uri, const uint available);
    void GISDataAvailableError(const uint errorCode, const uint errorInfo);
    void OpenBrowserRequestResult(const ushort senderHandle, const uint state);
    void GISRequestResult(const QDBusVariant gisRequestResult);
    void GISDataRequestResult(const QDBusVariant gisDataRequestResult);
    void POIDataAvailable(const uint target, const uint available, const QString uri);
    void ReceivedPOI(const uint available,const uint poiCount);
    void POIDataRequestResult(const QDBusVariant poiDataRequestResult);
    void POFDataAvailable(const uint available,const uint poiCount);
    void VoiceSearchOnlineStatus(const uint state, const ushort senderHandle, const uint blockType, const uint  blockCounter);
    void IBOXFunctionBlockInvisible(const bool status);
    void LSDataAvailable(const uchar optype, const QString uri, const uchar available);
    void LSDataAvailableError(const uint errorCode, const uint errorInfo);
    void LSRequest(const uchar optype, const ushort senderHandle, const uchar success);
    void LSDataRequest(const uchar optype, const ushort senderHandle, const uchar target, const QString uri, const QByteArray lsdata);
    void ACNActivated(const bool ACNActivatedValue);
};

#endif // DHAVN_MOSTMANAGER_IBOXSHADOWCONTROL_H
