/*
 ******************************************************************
 * COPYRIGHT © <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER :  Venkatesh Aeturi                                 *
 * DATE       :  12 Sep 2011                                      *
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
 *                  Navigation Fblock control, responsible to     *
 *                  create instance of Fblock, configure it and   *
 *                  add to most stack. if any listeners will      *
 *                  be part of this class.Communicates with ACS   *
 *                  daemon.                                       *
 *                                                                *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                         *
 *
 * DATE              PROGRAMMER               COMMENT
 ******************************************************************
 * 12 Sep 2011        Venkatesh Aeturi      Draft version
 * 15 Sep 2011        Amit Koparde          Added Log_Tag define  *
 * 21 Jun 2012        Shiva Kumar           Modified for FCAT 1.17.1, FCAT 1.17.2 and FCAT 1.17.3
 * 19 Sep 2012        Shiva Kumar           Modified for FCAT 1.19.0, 1.20.1, 1.20.2
 * 19 Mar 2013        Hanhong Keum          Modified for FCAT 1.22.7 and 1.22.8
 ******************************************************************
 */


#ifndef DHAVN_MOSTMANAGER_NAVIGATIONFBLOCKCONTROL_H
#define DHAVN_MOSTMANAGER_NAVIGATIONFBLOCKCONTROL_H

#include <QtCore>
#include <QtDBus/QtDBus>

#include <DHAVN_LogUtil.h>

#include "DHAVN_MOSTManager_Config.h"
#include "DHAVN_MOSTManager_ACSApplication.h"
#include "DHAVN_MOSTManager_NavigationFBlock.h"
#include "DHAVN_MOSTManager_MOSTStructures.h"

#define LOG_TAG_NAVI_CNT "NavigationControl"

const QString SDCARD_NAVI_PATH = "/navi/Bin/AppNavi";

using namespace acs::generated::fblock::Navigation;
using namespace k2l::acs::protocols::most;

class CNavigationFblockControl;

class CRequestLocalSearchHandler : public IFunctionBlockMethodHandler
{
    CNavigationFblockControl *m_NavigationFblockControlContext;

public:
    CRequestLocalSearchHandler(CNavigationFblockControl *context);

    /// Callback for received GetInterface OpType
    virtual void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received Start OpType
    virtual void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received Abort OpType
    virtual void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartResult OpType
    virtual void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartResultAck OpType
    virtual void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received AbortAck OpType
    virtual void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartAck OpType
    virtual void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
};

class COpenLocalSearchHandler : public IFunctionBlockMethodHandler
{
    CNavigationFblockControl *m_NavigationFblockControlContext;

public:
    COpenLocalSearchHandler(CNavigationFblockControl *context);

    /// Callback for received GetInterface OpType
    virtual void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received Start OpType
    virtual void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received Abort OpType
    virtual void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartResult OpType
    virtual void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartResultAck OpType
    virtual void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received AbortAck OpType
    virtual void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received StartAck OpType
    virtual void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
};

class CSearchPOIHandler : public IFunctionBlockMethodHandler
{
    CNavigationFblockControl *m_NavigationFblockControlContext;

public:
    CSearchPOIHandler(CNavigationFblockControl *context);

    /// Callback for received GetInterface OpType
    virtual void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received Start OpType
    virtual void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received Abort OpType
    virtual void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartResult OpType
    virtual void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartResultAck OpType
    virtual void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received AbortAck OpType
    virtual void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartAck OpType
    virtual void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
};

class CSearchRouteHandler : public IFunctionBlockMethodHandler
{
    CNavigationFblockControl *m_NavigationFblockControlContext;

public:
    CSearchRouteHandler(CNavigationFblockControl *context);

    /// Callback for received GetInterface OpType
    virtual void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received Start OpType
    virtual void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received Abort OpType
    virtual void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartResult OpType
    virtual void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartResultAck OpType
    virtual void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received AbortAck OpType
    virtual void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartAck OpType
    virtual void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
};

class CShowMapHandler : public IFunctionBlockMethodHandler
{
    CNavigationFblockControl *m_NavigationFblockControlContext;

public:
    CShowMapHandler(CNavigationFblockControl *context);

    /// Callback for received GetInterface OpType
    virtual void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received Start OpType
    virtual void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received Abort OpType
    virtual void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartResult OpType
    virtual void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartResultAck OpType
    virtual void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received AbortAck OpType
    virtual void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartAck OpType
    virtual void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
};

class CRegisterMyLocationHandler : public IFunctionBlockMethodHandler
{
    CNavigationFblockControl *m_NavigationFblockControlContext;

public:
    CRegisterMyLocationHandler(CNavigationFblockControl *context);

    /// Callback for received GetInterface OpType
    virtual void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received Start OpType
    virtual void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received Abort OpType
    virtual void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartResult OpType
    virtual void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartResultAck OpType
    virtual void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received AbortAck OpType
    virtual void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartAck OpType
    virtual void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
};


class CFindNearByHandler : public IFunctionBlockMethodHandler
{
    CNavigationFblockControl *m_NavigationFblockControlContext;

public:
    CFindNearByHandler(CNavigationFblockControl *context);

    /// Callback for received GetInterface OpType
    virtual void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received Start OpType
    virtual void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received Abort OpType
    virtual void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartResult OpType
    virtual void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartResultAck OpType
    virtual void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received AbortAck OpType
    virtual void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartAck OpType
    virtual void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
};

//FCat v15.3.4
class CCheckGPSBoundaryHandler : public IFunctionBlockMethodHandler
{
    CNavigationFblockControl *m_NavigationFblockControlContext;

public:
    CCheckGPSBoundaryHandler(CNavigationFblockControl *context);

    /// Callback for received GetInterface OpType
    virtual void GetInterface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received Start OpType
    virtual void Start( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received Abort OpType
    virtual void Abort( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartResult OpType
    virtual void StartResult( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartResultAck OpType
    virtual void StartResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload );
    /// Callback for received AbortAck OpType
    virtual void AbortAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
    /// Callback for received StartAck OpType
    virtual void StartAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){};
};


class CNavigationFblockControl : public QObject, public CControlThreadBase
{
    Q_OBJECT

private:

    /** Pointer Object of Tuner Fblock of generated code */
    CNavigation *m_NavigationFblock;

    CGPSRawData *m_GPSRawData;
    CGPSRawDataBessel *m_GPSRawDataBessel;

    CMapData *m_MapData;

    CRequestLocalSearch *m_RequestLocalSearch;
    CRequestLocalSearchHandler *m_RequestLocalSearchHandler;

    COpenLocalSearch *m_OpenLocalSearch;
    COpenLocalSearchHandler *m_OpenLocalSearchHandler;

    CSearchPOI *m_SearchPOI;
    CSearchPOIHandler *m_SearchPOIHandler;

    CSearchRoute *m_SearchRoute;
    CSearchRouteHandler *m_SearchRouteHandler;

    CShowMap *m_ShowMap;
    CShowMapHandler *m_ShowMapHandler;

    CRegisterMyLocation *m_RegisterMyLocation;
    CRegisterMyLocationHandler *m_RegisterMyLocationHandler;

    CDestinationInfo *m_DestinationInfo;

    CFindNearBy *m_FindNearBy;
    CFindNearByHandler *m_FindNearByHandler;

    CCheckGPSBoundary *m_CheckGPSBoundary;			// FCat v15.3.4
    CCheckGPSBoundaryHandler *m_CheckGPSBoundaryHandler;
	

public:
    CNavigationFblockControl(CMarshaledACSApplication *context);
    ~CNavigationFblockControl();

    virtual bool Initialize();    
    bool FBlockVisible();

    void SetHandler();

    bool SetGPSRawData(const QDBusVariant &gpsRawData);
    bool SetGPSRawDataBessel(const QDBusVariant &gpsRawDataBessel);
    bool SetMapData(const uint dataAvailable, const uint mapDataType);

    // Below callback functions are called by handler
    void RequestLoaclSearchHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uchar optype);
    void OpenLocalSearchHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uchar optype);
    void SearchPOIHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uchar optype);
    void SearchRouteHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uchar optype);
    void ShowMapHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uchar optype);
    void RegisterMyLocationHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uchar optype);
    void FindNearByHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uchar optype);
    void CheckGPSBoundaryHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uchar optype);	//FCat v15.3.4

    // Below functions are called by adaptor thread
    bool SendRequestLocalSearch(const uchar optype, const ushort senderHandle);
    bool SendOpenLocalSearch(const uchar optype, const ushort senderHandle);
    bool SendSearchPOI(const uchar optype, const ushort senderHandle, const uchar success);
    bool SendSearchRoute(const uchar optype, const ushort senderHandle, const uchar success);
    bool SendShowMap(const uchar optype, const ushort senderHandle, const uchar success);
    bool SendRegisterMyLocation(const uchar optype, const ushort senderHandle, const uchar success);
    bool SendDestinationInfo(const uchar optype, const uchar routeSet, const SPOIData &poiData);
    bool SendFindNearBy(const uchar optype, const ushort senderHandle, const uchar success);
	bool SendCheckGPSBoundary(const uchar optype, const ushort senderHandle, const uchar gpsstate);		// FCat v15.3.4

signals:
    // Emit signals to adaptor thread
    void RequestLocalSearch(const uchar optype, const ushort senderHandle, const QString inputKeyword);
    void OpenLocalSearch(const uchar optype, const ushort senderHandle, const uchar target, const QString text);
    void SearchPOI(const uchar optype, const ushort senderHandle, const uchar target, const QString inputKeyword);
    void SearchRoute(const QDBusVariant &searchRoute);
    void ShowMap(const QDBusVariant &showMap);
    void RegisterMyLocation(const QDBusVariant &registerMyLocation);
    void FindNearBy(const ushort senderHandle, const uchar target, const uchar poiKind);
    void CheckGPSBoundary(const QDBusVariant &checkGPSBoundary);		//FCat v15.3.4
};

#endif // DHAVN_MOSTMANAGER_NAVIGATIONFBLOCKCONTROL_H
