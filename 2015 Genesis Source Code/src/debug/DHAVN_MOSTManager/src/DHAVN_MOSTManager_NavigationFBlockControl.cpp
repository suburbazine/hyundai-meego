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
 * DATE           PROGRAMMER               COMMENT
 ******************************************************************
 * 12 Sep 2011    Venkatesh Aeturi         Draft version
 * 15 Sep 2011    Amit Koparde             Added Debug Statement *
 * 31 Jan 2012    Shiva Kumar              Modified GPSRawData and GPSRawDataBessel
 *                                         by removing custom class *
 * 08 Feb 2011    Neelima Khedad           Using the latest Version of LOG_INIT function Logger module
 * 21 Jun 2012    Shiva Kumar              Modified for FCAT 1.17.1, FCAT 1.17.2 and FCAT 1.17.3
 * 19 Sep 2012    Shiva Kumar              Modified for FCAT 1.19.0, 1.20.1, 1.20.2
 * 19 Mar 2013    Hanhong Keum             Modified for FCAT 1.22.7 and 1.22.8
 ******************************************************************
 */

#include "DHAVN_MOSTManager_NavigationFBlockControl.h"


/*
 *******************************************************************
 * Function:CNavigationFblockControl
 *
 * Description :
 *
 * Parameters :
 *
 * Returns :
 *
 *******************************************************************
 */

CNavigationFblockControl::CNavigationFblockControl(CMarshaledACSApplication *context)
    : CControlThreadBase(context, LOG_TAG_NAVI_CNT)
{
    LOG_INIT_V2(LOG_PREFIX_MOST)

    m_NavigationFblock = new CNavigation(0x01);
    m_pMOSTElement = m_NavigationFblock;

    m_GPSRawData = new CGPSRawData(m_NavigationFblock);
    m_GPSRawDataBessel = new CGPSRawDataBessel(m_NavigationFblock);
    m_MapData = new CMapData(m_NavigationFblock);

    m_RequestLocalSearch = new CRequestLocalSearch(m_NavigationFblock);
    m_RequestLocalSearchHandler = new CRequestLocalSearchHandler(this);

    m_OpenLocalSearch = new COpenLocalSearch(m_NavigationFblock);
    m_OpenLocalSearchHandler = new COpenLocalSearchHandler(this);

    m_SearchPOI = new CSearchPOI(m_NavigationFblock);
    m_SearchPOIHandler = new CSearchPOIHandler(this);

    m_SearchRoute = new CSearchRoute(m_NavigationFblock);
    m_SearchRouteHandler = new CSearchRouteHandler(this);

    m_ShowMap = new CShowMap(m_NavigationFblock);
    m_ShowMapHandler = new CShowMapHandler(this);

    m_RegisterMyLocation = new CRegisterMyLocation(m_NavigationFblock);
    m_RegisterMyLocationHandler = new CRegisterMyLocationHandler(this);

    m_DestinationInfo =  new CDestinationInfo(m_NavigationFblock);

    m_FindNearBy = new CFindNearBy(m_NavigationFblock);
    m_FindNearByHandler = new CFindNearByHandler(this);

    m_CheckGPSBoundary = new CCheckGPSBoundary(m_NavigationFblock);		// FCat v15.3.4
    m_CheckGPSBoundaryHandler = new CCheckGPSBoundaryHandler(this);
	
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description :
 *
 * Parameters :
 *
 * Returns :
 *
 *******************************************************************
 */

CNavigationFblockControl::~CNavigationFblockControl()
{
    delete m_NavigationFblock;

    delete m_GPSRawData;
    delete m_GPSRawDataBessel;
    delete m_MapData;

    delete m_RequestLocalSearch;
    delete m_RequestLocalSearchHandler;

    delete m_OpenLocalSearch;
    delete m_OpenLocalSearchHandler;

    delete m_SearchPOI;
    delete m_SearchPOIHandler;

    delete m_SearchRoute;
    delete m_SearchRouteHandler;

    delete m_ShowMap;
    delete m_ShowMapHandler;

    delete m_RegisterMyLocation;
    delete m_RegisterMyLocationHandler;

    delete m_DestinationInfo;

    delete m_FindNearBy;
    delete m_FindNearByHandler;

    delete m_CheckGPSBoundary;
    delete m_CheckGPSBoundaryHandler;	
}

/*
 *******************************************************************
 * Function:Initialize
 *
 * Description :
 *
 * Parameters :
 *
 * Returns :
 *
 *******************************************************************
 */

bool CNavigationFblockControl::Initialize()
{
    if(m_NavigationFblock->InitializeMOSTElement())
    {
        SetHandler();

        /** Initializing the notification value with invalid */
        if(QFile::exists( SDCARD_NAVI_PATH ) == true)
        {
//            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " Navi SDCard is available " << MOST_LOG_ENDL;
            m_MapData->DataAvailabeValue((CMapData::TDataAvailabe)0x01);
            m_MapData->MapDataTypeValue((CMapData::TMapDataType)0x01);
// 2014.08.11, for log, hyunyong.sung
	    MOST_LOG_HIGH << LOG_TAG_NAVI_CNT << " MapData_Status --> : DataAvailable = "
	    << m_MapData->DataAvailabeValue() << " MapDataType = " << m_MapData->MapDataTypeValue() << MOST_LOG_ENDL;							
        }
        else
        {
//            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " Navi SDCard is not available " << MOST_LOG_ENDL;
            m_MapData->DataAvailabeValue((CMapData::TDataAvailabe)0x00);
            m_MapData->MapDataTypeValue((CMapData::TMapDataType)0x00);
// 2014.08.11, for log, hyunyong.sung
	    MOST_LOG_HIGH << LOG_TAG_NAVI_CNT << " MapData_Status --> : DataAvailable = "
	    << m_MapData->DataAvailabeValue() << " MapDataType = " << m_MapData->MapDataTypeValue() << MOST_LOG_ENDL;							
        }
        m_MapData->UpdateShadows();

        m_GPSRawData->AdditionalInformationFixValue(CGPSRawData::AdditionalInformationFixNotAvailable);
        m_GPSRawData->UpdateShadows();

        m_GPSRawDataBessel->AdditionalInformationFixValue(CGPSRawDataBessel::AdditionalInformationFixNotAvailable);
        m_GPSRawDataBessel->UpdateShadows();

        m_Initialized = true;
    }

    return m_Initialized;
}

bool CNavigationFblockControl::FBlockVisible()
{
    return GetIsRegistered();
}

void CNavigationFblockControl::SetHandler()
{
    m_RequestLocalSearch->SetHandler(m_RequestLocalSearchHandler);
    m_OpenLocalSearch->SetHandler(m_OpenLocalSearchHandler);
    m_SearchPOI->SetHandler(m_SearchPOIHandler);
    m_SearchRoute->SetHandler(m_SearchRouteHandler);
    m_ShowMap->SetHandler(m_ShowMapHandler);
    m_RegisterMyLocation->SetHandler(m_RegisterMyLocationHandler);
    m_FindNearBy->SetHandler(m_FindNearByHandler);
    m_CheckGPSBoundary->SetHandler(m_CheckGPSBoundaryHandler);
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

bool CNavigationFblockControl::SetGPSRawData(const QDBusVariant &gpsRawData)
{
    struct SGPSData gpsdata_wr;
    gpsRawData.variant().value<QDBusArgument>() >> gpsdata_wr;

    static int printLogCount = 0;

    if( (printLogCount++ % 10) == 0 )
    {
// 2014.08.11, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_NAVI_CNT << " GPSRawData_Status --> : Longitude = "<< gpsdata_wr.longitude
    << " Latitude = " << gpsdata_wr.latitude << " FixGPS = " << gpsdata_wr.FixGPS << " HDOP = " << gpsdata_wr.HDOP
    << " VDOP = " << gpsdata_wr.VDOP << " SpeedGPS = " << gpsdata_wr.speedGPS << "HeadingGPS = " << gpsdata_wr.headingGPS
    << " Height = " << gpsdata_wr.height << " Year = " << gpsdata_wr.year << " Month = " << gpsdata_wr.month
    << " Day = " << gpsdata_wr.day << " Hour = " << gpsdata_wr.hour << " Minute = " << gpsdata_wr.min 
    << " Second = " << gpsdata_wr.sec << MOST_LOG_ENDL;
/*	
        MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " SetGPSRawData API got called " << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawData:Longitude = " << gpsdata_wr.longitude << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawData:Latitude = " << gpsdata_wr.latitude << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawData:FixGPS = " << gpsdata_wr.FixGPS << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawData:HDOP = " << gpsdata_wr.HDOP << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawData:VDOP = " << gpsdata_wr.VDOP << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawData:SpeedGPS = " << gpsdata_wr.speedGPS << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawData:HeadingGPS = " << gpsdata_wr.headingGPS << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawData:Height = " << gpsdata_wr.height << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawData:Year = " << gpsdata_wr.year << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawData:Month = " << gpsdata_wr.month << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawData:Day = " << gpsdata_wr.day << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawData:Hour = " << gpsdata_wr.hour << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawData:Minute = " << gpsdata_wr.min << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawData:Second = " << gpsdata_wr.sec << MOST_LOG_ENDL;
*/        
    }

    /** Updating the data with the instance of the GPSRawdata Custom Function Class */
    m_GPSRawData->PositionWGS84LongitudeValue(gpsdata_wr.longitude);
    m_GPSRawData->PositionWGS84LatitudeValue(gpsdata_wr.latitude);
    m_GPSRawData->AdditionalInformationFixValue((CGPSRawData::TAdditionalInformationFix)gpsdata_wr.FixGPS);
    m_GPSRawData->AdditionalInformationHDOPValue(gpsdata_wr.HDOP);
    m_GPSRawData->AdditionalInformationVDOPValue(gpsdata_wr.VDOP);
    m_GPSRawData->AdditionalInformationSpeedValue(gpsdata_wr.speedGPS);
    m_GPSRawData->AdditionalInformationHeadingValue(gpsdata_wr.headingGPS);
    m_GPSRawData->AdditionalInformationHeightValue(gpsdata_wr.height);
    m_GPSRawData->DateDayValue(gpsdata_wr.day);
    m_GPSRawData->DateMonthValue(gpsdata_wr.month);
    m_GPSRawData->DateYearValue(gpsdata_wr.year);
    m_GPSRawData->TimeHourValue(gpsdata_wr.hour);
    m_GPSRawData->TimeMinuteValue(gpsdata_wr.min);
    m_GPSRawData->TimeSecondValue(gpsdata_wr.sec);

    return m_GPSRawData->UpdateShadows();
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

bool CNavigationFblockControl::SetGPSRawDataBessel(const QDBusVariant &gpsRawDataBessel)
{
    struct SGPSData gpsdata_wr;
    gpsRawDataBessel.variant().value<QDBusArgument>() >> gpsdata_wr;

// 2014.08.11, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_NAVI_CNT << " GPSRawDataBessel_Status --> : Longitude = "<< gpsdata_wr.longitude
    << " Latitude = " << gpsdata_wr.latitude << " FixGPS = " << gpsdata_wr.FixGPS << " HDOP = " << gpsdata_wr.HDOP
    << " VDOP = " << gpsdata_wr.VDOP << " SpeedGPS = " << gpsdata_wr.speedGPS << "HeadingGPS = " << gpsdata_wr.headingGPS
    << " Height = " << gpsdata_wr.height << " Year = " << gpsdata_wr.year << " Month = " << gpsdata_wr.month
    << " Day = " << gpsdata_wr.day << " Hour = " << gpsdata_wr.hour << " Minute = " << gpsdata_wr.min 
    << " Second = " << gpsdata_wr.sec << MOST_LOG_ENDL;
/*	
    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " SetGPSRawDataBessel API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawDataBessel:Longitude = " << gpsdata_wr.longitude << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawDataBessel:Latitude = " << gpsdata_wr.latitude << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawDataBessel:FixGPS = " << gpsdata_wr.FixGPS << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawDataBessel:HDOP = " << gpsdata_wr.HDOP << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawDataBessel:VDOP = " << gpsdata_wr.VDOP << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawDataBessel:SpeedGPS = " << gpsdata_wr.speedGPS << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawDataBessel:HeadingGPS = " << gpsdata_wr.headingGPS << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawDataBessel:Height = " << gpsdata_wr.height << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawDataBessel:Year = " << gpsdata_wr.year << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawDataBessel:Month = " << gpsdata_wr.month << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawDataBessel:Day = " << gpsdata_wr.day << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawDataBessel:Hour = " << gpsdata_wr.hour << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawDataBessel:Minute = " << gpsdata_wr.min << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SetGPSRawDataBessel:Second = " << gpsdata_wr.sec << MOST_LOG_ENDL;
*/
    /** Updating the data with the instance of the GPSRawdataBessel Custom Function Class */
    m_GPSRawDataBessel->BesselEllipsoidBesselLatitudeValue(gpsdata_wr.latitude);
    m_GPSRawDataBessel->BesselEllipsoidBesselLongitudeValue(gpsdata_wr.longitude);
    m_GPSRawDataBessel->AdditionalInformationFixValue((CGPSRawDataBessel::TAdditionalInformationFix)gpsdata_wr.FixGPS);
    m_GPSRawDataBessel->AdditionalInformationHDOPValue(gpsdata_wr.HDOP);
    m_GPSRawDataBessel->AdditionalInformationVDOPValue(gpsdata_wr.VDOP);
    m_GPSRawDataBessel->AdditionalInformationSpeedValue(gpsdata_wr.speedGPS);
    m_GPSRawDataBessel->AdditionalInformationHeadingValue(gpsdata_wr.headingGPS);
    m_GPSRawDataBessel->AdditionalInformationHeightValue(gpsdata_wr.height);
    m_GPSRawDataBessel->DateDayValue(gpsdata_wr.day);
    m_GPSRawDataBessel->DateMonthValue(gpsdata_wr.month);
    m_GPSRawDataBessel->DateYearValue(gpsdata_wr.year);
    m_GPSRawDataBessel->TimeHourValue(gpsdata_wr.hour);
    m_GPSRawDataBessel->TimeMinuteValue(gpsdata_wr.min);
    m_GPSRawDataBessel->TimeSecondValue(gpsdata_wr.sec);

    return  m_GPSRawDataBessel->UpdateShadows();
}
/*
 *******************************************************************
 * Function:SetMapData
 *
 * Description :Function called by the Navigation adaptor to set the GPS RawData Bessel.
 *
 * Parameters :
 *    dataAvailable     rw   holds the value about Navi data available/unavailable.
 *    mapDataType       rw   holds the value about the type of Navi values
 *
 * Returns :true or false
 *
 *******************************************************************
 */
bool CNavigationFblockControl::SetMapData(const uint dataAvailable, const uint mapDataType)
{
// 2014.08.11, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_NAVI_CNT << " MapData_Status --> : DataAvailable = "<< dataAvailable
    << " mapDataType = " << mapDataType << MOST_LOG_ENDL;
/*
    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " SetMapData API is called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " DataAvailable :: " << dataAvailable << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " mapDataType :: " << mapDataType << MOST_LOG_ENDL;
*/
    m_MapData->DataAvailabeValue((CMapData::TDataAvailabe)dataAvailable);
    m_MapData->MapDataTypeValue((CMapData::TMapDataType)mapDataType);

    return m_MapData->UpdateShadows();
}

/*
 *******************************************************************
 * Function: CRequestLocalSearchHandler
 *
 * Description :This is constructor of CRequestLocalSearchHandler class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CNavigationFblockControl.
 *
 * Returns : none
 *
 *******************************************************************
 */
CRequestLocalSearchHandler::CRequestLocalSearchHandler(CNavigationFblockControl *context)
{
    m_NavigationFblockControlContext = context;
}

void CRequestLocalSearchHandler::StartResultAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_NavigationFblockControlContext->RequestLoaclSearchHandler(sourceDeviceID, payloadLength, payload, METHOD_STARTRESULTACK);
}

void CNavigationFblockControl::RequestLoaclSearchHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uchar optype)
{
    ushort senderHandle;
    QString inputKeyword;

//    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " RequestLoaclSearchHandler API was called from IBOX" << MOST_LOG_ENDL;

    switch(optype)
    {
        case METHOD_STARTRESULTACK:
//            MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " RequestLoaclSearchHandler:OPTYPE = " << optype << MOST_LOG_ENDL;
            m_RequestLocalSearch->ParseStartResultAck(payloadLength, payload);
            senderHandle = m_RequestLocalSearch->SenderHandleValue();
            {
                const ::k2l::acs::protocols::most::string::IStringEncoding *pEncoding = NULL;
                uint8_t *pString = NULL;
                uint32_t stringLength = 0;
                m_RequestLocalSearch->InputKeywordString(&pEncoding, &pString, &stringLength);
                inputKeyword = QString::fromUtf8((const char *)pString, stringLength);
// 2014.08.11, for log, hyunyong.sung
		    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " RequestLoaclSearch_StartResultAck <-- : SenderHandle = " << senderHandle
		    << " InputKeyword = " <<  inputKeyword.toAscii().data() << MOST_LOG_ENDL;				
/*
                MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " RequestLoaclSearchHandler::SenderHandle : " << senderHandle << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " RequestLoaclSearchHandler::InputKeyword : " << inputKeyword.toAscii().data() << MOST_LOG_ENDL;
*/                
            }
            break;
        default:
            MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " Undefined Operation of RequestLoaclSearchHandler:OPTYPE = " << optype << MOST_LOG_ENDL;
            return;
    }

    emit RequestLocalSearch(optype, senderHandle, inputKeyword);
}


COpenLocalSearchHandler::COpenLocalSearchHandler(CNavigationFblockControl *context)
{
    m_NavigationFblockControlContext = context;
}

void COpenLocalSearchHandler::StartResultAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_NavigationFblockControlContext->OpenLocalSearchHandler(sourceDeviceID, payloadLength, payload, METHOD_STARTRESULTACK);
}

void COpenLocalSearchHandler::AbortAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_NavigationFblockControlContext->OpenLocalSearchHandler(sourceDeviceID, payloadLength, payload, METHOD_ABORTACK);
}

void CNavigationFblockControl::OpenLocalSearchHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uchar optype)
{
    ushort senderHandle;
    uchar target = 0xFF;
    QString text;
/*
    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " OpenLocalSearchHandler API was called from IBOX" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " OpenLocalSearchHandler:OPTYPE = " << optype << MOST_LOG_ENDL;
*/
    switch(optype)
    {
        case METHOD_STARTRESULTACK:
            m_OpenLocalSearch->ParseStartResultAck(payloadLength, payload);
            senderHandle = m_OpenLocalSearch->SenderHandleValue();
            target = (uchar)m_OpenLocalSearch->TargetValue();
            {
                const ::k2l::acs::protocols::most::string::IStringEncoding *pEncoding = NULL;
                uint8_t *pString = NULL;
                uint32_t stringLength = 0;
                m_OpenLocalSearch->TextString(&pEncoding, &pString, &stringLength);
                text = QString::fromUtf8((const char *)pString, stringLength);

// 2014.08.11, for log, hyunyong.sung
		    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " OpenLocalSearch_StartResultAck <-- : SenderHandle = "<< senderHandle
		    << " Target = " << target << " InputKeyword = " << text.toAscii().data() << MOST_LOG_ENDL;		
/*				
                MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " OpenLocalSearchHandler::SenderHandle : " << senderHandle << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " OpenLocalSearchHandler::Target : " << target << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " OpenLocalSearchHandler::InputKeyword : " << text.toAscii().data() << MOST_LOG_ENDL;
*/                
            }
            break;
        case METHOD_ABORTACK:
            m_OpenLocalSearch->ParseAbortAck(payloadLength, payload);
            senderHandle = m_OpenLocalSearch->SenderHandleValue();
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " OpenLocalSearchHandler::SenderHandle : " << senderHandle << MOST_LOG_ENDL;
            break;
        default:
            MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " Undefined Operation of OpenLocalSearchHandler " << MOST_LOG_ENDL;
            return;
    }

    emit OpenLocalSearch(optype, senderHandle, target, text);
}

CSearchPOIHandler::CSearchPOIHandler(CNavigationFblockControl *context)
{
    m_NavigationFblockControlContext = context;
}

void CSearchPOIHandler::StartResultAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_NavigationFblockControlContext->SearchPOIHandler(sourceDeviceID, payloadLength, payload, METHOD_STARTRESULTACK);
}

void CNavigationFblockControl::SearchPOIHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uchar optype)
{
    ushort senderHandle;
    uchar target = 0xFF;
    QString inputKeyword;
/*
    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " SearchPOIHandler API was called from IBOX" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SearchPOIHandler:OPTYPE = " << optype << MOST_LOG_ENDL;
*/
    switch(optype)
    {
        case METHOD_STARTRESULTACK:
            m_SearchPOI->ParseStartResultAck(payloadLength, payload);
            senderHandle = m_SearchPOI->SenderHandleValue();
            target = (uchar)m_SearchPOI->TargetValue();
            {
                const ::k2l::acs::protocols::most::string::IStringEncoding *pEncoding = NULL;
                uint8_t *pString = NULL;
                uint32_t stringLength = 0;
                m_SearchPOI->InputKeywordString(&pEncoding, &pString, &stringLength);
                inputKeyword = QString::fromUtf8((const char *)pString, stringLength);
// 2014.08.11, for log, hyunyong.sung
		    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " SearchPOI_StartResultAck <-- : SenderHandle = "<< senderHandle
		    << " Target = " <<  target << " InputKeyword = " << inputKeyword.toAscii().data() << MOST_LOG_ENDL;		
/*							
                MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SearchPOIHandler::SenderHandle : " << senderHandle << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SearchPOIHandler::Target : " << target << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SearchPOIHandler::InputKeyword : " << inputKeyword.toAscii().data() << MOST_LOG_ENDL;
*/                
            }
            break;

        default:
            MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " Undefined Operation of SearchPOIHandler " << MOST_LOG_ENDL;
            return;
    }

    emit SearchPOI(optype, senderHandle, target, inputKeyword);
}



CSearchRouteHandler::CSearchRouteHandler(CNavigationFblockControl *context)
{
    m_NavigationFblockControlContext = context;
}

void CSearchRouteHandler::StartResultAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_NavigationFblockControlContext->SearchRouteHandler(sourceDeviceID, payloadLength, payload, METHOD_STARTRESULTACK);
}

void CNavigationFblockControl::SearchRouteHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uchar optype)
{
    SSearchRoute sSearchRoute;

    sSearchRoute.optype = optype;
    sSearchRoute.senderHandle = 0;
    sSearchRoute.target = 0xFF;
    sSearchRoute.lat = 0;
    sSearchRoute.lon = 0;
/*
    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " SearchRouteHandler API was called from IBOX" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SearchRouteHandler:OPTYPE = " << sSearchRoute.optype << MOST_LOG_ENDL;
*/
    switch(optype)
    {
        case METHOD_STARTRESULTACK:
            m_SearchRoute->ParseStartResultAck(payloadLength, payload);
            sSearchRoute.senderHandle = m_SearchRoute->SenderHandleValue();
            sSearchRoute.target = (uchar)m_SearchRoute->TargetValue();
            sSearchRoute.lat = m_SearchRoute->LatValue();
            sSearchRoute.lon = m_SearchRoute->LonValue();
            {
                const ::k2l::acs::protocols::most::string::IStringEncoding *pEncoding = NULL;
                uint8_t *pString = NULL;
                uint32_t stringLength = 0;
                m_SearchRoute->NameString(&pEncoding, &pString, &stringLength);
                sSearchRoute.name = QString::fromUtf8((const char *)pString, stringLength);
            }
// 2014.08.11, for log, hyunyong.sung
	    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " SearchRoute_StartResultAck <-- : SenderHandle = "<< sSearchRoute.senderHandle
	    << " Target = " <<  sSearchRoute.target << " Lat = " << sSearchRoute.lat << " Lon = "
	    << sSearchRoute.lon << " Name = " << sSearchRoute.name.toAscii().data() << MOST_LOG_ENDL;		
/*			
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SearchRouteHandler::SenderHandle : " << sSearchRoute.senderHandle << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SearchRouteHandler::Target : " << sSearchRoute.target << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SearchRouteHandler::Lat : " << sSearchRoute.lat << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SearchRouteHandler::Lon : " << sSearchRoute.lon << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SearchRouteHandler::Name : " << sSearchRoute.name.toAscii().data() << MOST_LOG_ENDL;
*/            
            break;

        default:
            MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " Undefined Operation of SearchRouteHandler " << MOST_LOG_ENDL;
            return;
    }

    QDBusArgument searchRouteQDBArg;
    searchRouteQDBArg << sSearchRoute;
    QVariant searchRouteQVar;
    searchRouteQVar.setValue(searchRouteQDBArg);
    QDBusVariant searchRouteQDBVar;
    searchRouteQDBVar.setVariant(searchRouteQVar);

    emit SearchRoute(searchRouteQDBVar);
}

CShowMapHandler::CShowMapHandler(CNavigationFblockControl *context)
{
    m_NavigationFblockControlContext = context;
}

void CShowMapHandler::StartResultAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_NavigationFblockControlContext->ShowMapHandler(sourceDeviceID, payloadLength, payload, METHOD_STARTRESULTACK);
}

void CNavigationFblockControl::ShowMapHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uchar optype)
{
    SShowMap sShowMap;

    sShowMap.optype = optype;
    sShowMap.senderHandle = 0;
    sShowMap.target = 0xFF;
    sShowMap.lat = 0;
    sShowMap.lon = 0;
/*
    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " ShowMapHandler API was called from IBOX" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " ShowMapHandler:OPTYPE = " << sShowMap.optype << MOST_LOG_ENDL;
*/
    switch(optype)
    {
        case METHOD_STARTRESULTACK:
            m_ShowMap->ParseStartResultAck(payloadLength, payload);
            sShowMap.senderHandle = m_ShowMap->SenderHandleValue();
            sShowMap.target = (uchar)m_ShowMap->TargetValue();
            sShowMap.lat = m_ShowMap->LatValue();
            sShowMap.lon = m_ShowMap->LonValue();
            {
                const ::k2l::acs::protocols::most::string::IStringEncoding *pEncoding = NULL;
                uint8_t *pString = NULL;
                uint32_t stringLength = 0;
                m_ShowMap->NameString(&pEncoding, &pString, &stringLength);
                sShowMap.name = QString::fromUtf8((const char *)pString, stringLength);
            }
// 2014.08.11, for log, hyunyong.sung
	    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " ShowMap_StartResultAck <-- : SenderHandle = "<< sShowMap.senderHandle
	    << " Target = " << sShowMap.target << " Lat = " << sShowMap.lat << " Lon = " << sShowMap.lon 
	    << " Name = " << sShowMap.name.toAscii().data() << MOST_LOG_ENDL;		
/*							
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " ShowMapHandler::SenderHandle : " << sShowMap.senderHandle << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " ShowMapHandler::Target : " << sShowMap.target << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " ShowMapHandler::Lat : " << sShowMap.lat << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " ShowMapHandler::Lon : " << sShowMap.lon << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " ShowMapHandler::Name : " << sShowMap.name.toAscii().data() << MOST_LOG_ENDL;
*/            
            break;

        default:
            MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " Undefined Operation of ShowMapHandler " << MOST_LOG_ENDL;
            return;
    }

    QDBusArgument showMapQDBArg;
    showMapQDBArg << sShowMap;
    QVariant showMapQVar;
    showMapQVar.setValue(showMapQDBArg);
    QDBusVariant showMapQDBVar;
    showMapQDBVar.setVariant(showMapQVar);

    emit ShowMap(showMapQDBVar);
}


CRegisterMyLocationHandler::CRegisterMyLocationHandler(CNavigationFblockControl *context)
{
    m_NavigationFblockControlContext = context;
}

void CRegisterMyLocationHandler::StartResultAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_NavigationFblockControlContext->RegisterMyLocationHandler(sourceDeviceID, payloadLength, payload, METHOD_STARTRESULTACK);
}

void CNavigationFblockControl::RegisterMyLocationHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uchar optype)
{
    SRegisterMyLocation sRegisterMyLocation;

    sRegisterMyLocation.optype = optype;
    sRegisterMyLocation.senderHandle = 0;
    sRegisterMyLocation.target = 0xFF;
    sRegisterMyLocation.lat = 0;
    sRegisterMyLocation.lon = 0;
/*
    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " RegisterMyLocationHandler API was called from IBOX" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " RegisterMyLocationHandler:OPTYPE = " << sRegisterMyLocation.optype << MOST_LOG_ENDL;
*/
    switch(optype)
    {
        case METHOD_STARTRESULTACK:
            m_RegisterMyLocation->ParseStartResultAck(payloadLength, payload);
            sRegisterMyLocation.senderHandle = m_RegisterMyLocation->SenderHandleValue();
            sRegisterMyLocation.target = (uchar)m_RegisterMyLocation->TargetValue();
            sRegisterMyLocation.lat = m_RegisterMyLocation->LatValue();
            sRegisterMyLocation.lon = m_RegisterMyLocation->LonValue();
            {
                const ::k2l::acs::protocols::most::string::IStringEncoding *pEncoding = NULL;
                uint8_t *pString = NULL;
                uint32_t stringLength = 0;
                m_RegisterMyLocation->NameString(&pEncoding, &pString, &stringLength);
                sRegisterMyLocation.name = QString::fromUtf8((const char *)pString, stringLength);
            }
// 2014.08.11, for log, hyunyong.sung
	    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " RegisterMyLocation_StartResultAck <-- : SenderHandle = "<< sRegisterMyLocation.senderHandle
	    << " Target = " << sRegisterMyLocation.target << " Lat = " << sRegisterMyLocation.lat << " Lon = " 
	    << sRegisterMyLocation.lon << " Name = " << sRegisterMyLocation.name.toAscii().data() << MOST_LOG_ENDL;		
            break;

        default:
            MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " Undefined Operation of RegisterMyLocationHandler " << MOST_LOG_ENDL;
            return;
    }

    QDBusArgument registerMyLocationQDBArg;
    registerMyLocationQDBArg << sRegisterMyLocation;
    QVariant registerMyLocationQVar;
    registerMyLocationQVar.setValue(registerMyLocationQDBArg);
    QDBusVariant registerMyLocationQDBVar;
    registerMyLocationQDBVar.setVariant(registerMyLocationQVar);

    emit RegisterMyLocation(registerMyLocationQDBVar);
}


CFindNearByHandler::CFindNearByHandler(CNavigationFblockControl *context)
{
    m_NavigationFblockControlContext = context;
}

void CFindNearByHandler::StartResultAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_NavigationFblockControlContext->FindNearByHandler(sourceDeviceID, payloadLength, payload, METHOD_STARTRESULTACK);
}

void CNavigationFblockControl::FindNearByHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uchar optype)
{
    ushort senderHandle = 0;
    uchar target = 0xFF;
    uchar poiKind = 0xFF;

//    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " FindNearByHandler API was called from IBOX" << MOST_LOG_ENDL;

    switch(optype)
    {
        case METHOD_STARTRESULTACK:
            m_FindNearBy->ParseStartResultAck(payloadLength, payload);
            senderHandle = m_FindNearBy->SenderHandleValue();
            target = m_FindNearBy->TargetValue();
            poiKind = m_FindNearBy->POIKindValue();
// 2014.08.11, for log, hyunyong.sung
	    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " FindNearBy_StartResultAck <-- : SenderHandle = "<< senderHandle
	    << " Target = " << target << " POIKind = " << poiKind << MOST_LOG_ENDL;		
/*					
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " FindNearByHandler::SenderHandle : " << senderHandle << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " FindNearByHandler::Target : " << target << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " FindNearByHandler::POIKind : " << poiKind << MOST_LOG_ENDL;
*/            
            break;

        default:
            MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " Undefined Operation of FindNearByHandler " << MOST_LOG_ENDL;
            return;
    }

    emit FindNearBy(senderHandle, target, poiKind);
}


// FCat v15.3.4
CCheckGPSBoundaryHandler::CCheckGPSBoundaryHandler(CNavigationFblockControl *context)
{
    m_NavigationFblockControlContext = context;
}

void CCheckGPSBoundaryHandler::StartResultAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_NavigationFblockControlContext->CheckGPSBoundaryHandler(sourceDeviceID, payloadLength, payload, METHOD_STARTRESULTACK);
}

void CNavigationFblockControl::CheckGPSBoundaryHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uchar optype)
{
    SCheckGPSBoundary sCheckGPSBoundary;

    sCheckGPSBoundary.optype = optype;
    sCheckGPSBoundary.senderHandle = 0;
    sCheckGPSBoundary.lat = 0;
    sCheckGPSBoundary.lon = 0;

	switch(optype)
	{
		case METHOD_STARTRESULTACK:
			m_CheckGPSBoundary->ParseStartResultAck(payloadLength, payload);
			sCheckGPSBoundary.senderHandle = m_CheckGPSBoundary ->SenderHandleValue();
			sCheckGPSBoundary.lat = m_CheckGPSBoundary->LatValue();
			sCheckGPSBoundary.lon = m_CheckGPSBoundary->LonValue();

			MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " CheckGPSBoundary_StartResultAck <-- : SenderHandle = "<< sCheckGPSBoundary.senderHandle
			<< " Lat = " << sCheckGPSBoundary.lat << " Lon = " << sCheckGPSBoundary.lon << MOST_LOG_ENDL;		
			break;

		default:
			MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " Undefined Operation of CheckGPSBoundaryr " << MOST_LOG_ENDL;
			return;
	}
	
    QDBusArgument checkGPSBoundaryQDBArg;
    checkGPSBoundaryQDBArg << sCheckGPSBoundary;
    QVariant checkGPSBoundaryQVar;
    checkGPSBoundaryQVar.setValue(checkGPSBoundaryQDBArg);
    QDBusVariant checkGPSBoundaryQDBVar;
    checkGPSBoundaryQDBVar.setVariant(checkGPSBoundaryQVar);
	
    emit CheckGPSBoundary(checkGPSBoundaryQDBVar);

}


bool CNavigationFblockControl::SendRequestLocalSearch(const uchar optype, const ushort senderHandle)
{
    bool result = false;
/*	
    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " SendRequestLocalSearch API got called to send to IBOX" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendRequestLocalSearch:OPTYPE = " << optype << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendRequestLocalSearch:SenderHandle = " << senderHandle << MOST_LOG_ENDL;
*/
    switch(optype)
    {
        case METHOD_ERRORACK:
            m_RequestLocalSearch->ErrorAck(IBOX_DEVICE_ADDRESS, senderHandle, 0x41, 0, NULL);
            result = true;
            break;
        case METHOD_PROCESSINGACK:
            m_RequestLocalSearch->ProcessingAck(IBOX_DEVICE_ADDRESS, senderHandle);
            result = true;
            break;
        case METHOD_RESULTACK:
// 2014.08.11, for log, hyunyong.sung
	    MOST_LOG_HIGH << LOG_TAG_NAVI_CNT << " RequestLocalSearch_ResultAck --> : SenderHandle = "
	    << senderHandle << MOST_LOG_ENDL;					
            m_RequestLocalSearch->SenderHandleValue(senderHandle);
            m_RequestLocalSearch->ResultAck(IBOX_DEVICE_ADDRESS);
            result = true;
            break;
        default:
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " Undefined Operation of SendRequestLocalSearch = " << optype << MOST_LOG_ENDL;
            break;
    }
    return result;
}

bool CNavigationFblockControl::SendOpenLocalSearch(const uchar optype, const ushort senderHandle)
{
    bool result = false;
/*	
    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " SendOpenLocalSearch API got called to send to IBOX" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendOpenLocalSearch:OPTYPE = " << optype << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendOpenLocalSearch:SenderHandle = " << senderHandle << MOST_LOG_ENDL;
*/
    switch(optype)
    {
        case METHOD_ERRORACK:
            m_OpenLocalSearch->ErrorAck(IBOX_DEVICE_ADDRESS, senderHandle, 0x41, 0, NULL);
            result = true;
            break;
        case METHOD_PROCESSINGACK:
            m_OpenLocalSearch->ProcessingAck(IBOX_DEVICE_ADDRESS, senderHandle);
            result = true;
            break;
        case METHOD_RESULTACK:
// 2014.08.11, for log, hyunyong.sung
	    MOST_LOG_HIGH << LOG_TAG_NAVI_CNT << " OpenLocalSearch_ResultAck --> : SenderHandle = "
	    << senderHandle << MOST_LOG_ENDL;					
            m_OpenLocalSearch->SenderHandleValue(senderHandle);
            m_OpenLocalSearch->ResultAck(IBOX_DEVICE_ADDRESS);
            result = true;
            break;
        default:
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " Undefined Operation of SendOpenLocalSearch = " << optype << MOST_LOG_ENDL;
            break;
    }

    return result;
}

bool CNavigationFblockControl::SendSearchPOI(const uchar optype, const ushort senderHandle, const uchar success)
{
    bool result = false;
// 2014.08.11, for log, hyunyong.sung
    MOST_LOG_HIGH << LOG_TAG_NAVI_CNT << " SearchPOI_ResultAck --> : SenderHandle = "
    << senderHandle << MOST_LOG_ENDL;		
/*	
    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " SendSearchPOI API got called to send to IBOX" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendSearchPOI:OPTYPE = " << optype << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendSearchPOI:SenderHandle = " << senderHandle << MOST_LOG_ENDL;
*/
    switch(optype)
    {
        case METHOD_ERRORACK:
            m_SearchPOI->ErrorAck(IBOX_DEVICE_ADDRESS, senderHandle, 0x41, 0, NULL);
            result = true;
            break;
        case METHOD_PROCESSINGACK:
            m_SearchPOI->ProcessingAck(IBOX_DEVICE_ADDRESS, senderHandle);
            result = true;
            break;
        case METHOD_RESULTACK:
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " onRegisterMyLocation:Success = " << success << MOST_LOG_ENDL;
            m_SearchPOI->SenderHandleValue(senderHandle);
            m_SearchPOI->SuccessValue((acs::generated::fblock::Navigation::CSearchPOI::TSuccess)success);
            m_SearchPOI->ResultAck(IBOX_DEVICE_ADDRESS);
            result = true;
            break;
        default:
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " Undefined Operation of SendSearchPOI = " << optype << MOST_LOG_ENDL;
            break;
    }

    return result;
}

bool CNavigationFblockControl::SendSearchRoute(const uchar optype, const ushort senderHandle, const uchar success)
{
    bool result = false;
/*
    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " SendSearchRoute API got called to send to IBOX" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendSearchRoute:OPTYPE = " << optype << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendSearchRoute:SenderHandle = " << senderHandle << MOST_LOG_ENDL;
*/
    switch(optype)
    {
        case METHOD_ERRORACK:
            m_SearchRoute->ErrorAck(IBOX_DEVICE_ADDRESS, senderHandle, 0x41, 0, NULL);
            result = true;
            break;
        case METHOD_PROCESSINGACK:
            m_SearchRoute->ProcessingAck(IBOX_DEVICE_ADDRESS, senderHandle);
            result = true;
            break;
        case METHOD_RESULTACK:
// 2014.08.11, for log, hyunyong.sung
	    MOST_LOG_HIGH << LOG_TAG_NAVI_CNT << " SearchRoute_ResultAck --> : SenderHandle = "
	    << senderHandle << " success = " << success << MOST_LOG_ENDL;				
//            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " onRegisterMyLocation:Success = " << success << MOST_LOG_ENDL;
            m_SearchRoute->SenderHandleValue(senderHandle);
            m_SearchRoute->SuccessValue((CSearchRoute::TSuccess)success);
            m_SearchRoute->ResultAck(IBOX_DEVICE_ADDRESS);
            result = true;
            break;
        default:
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " Undefined Operation of SendSearchRoute = " << optype << MOST_LOG_ENDL;
            break;
    }

    return result;
}

bool CNavigationFblockControl::SendShowMap(const uchar optype, const ushort senderHandle, const uchar success)
{
    bool result = false;
/*
    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " SendShowMap API got called to send to IBOX" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendShowMap:OPTYPE = " << optype << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendShowMap:SenderHandle = " << senderHandle << MOST_LOG_ENDL;
*/
    switch(optype)
    {
        case METHOD_ERRORACK:
            m_ShowMap->ErrorAck(IBOX_DEVICE_ADDRESS, senderHandle, 0x41, 0, NULL);
            result = true;
            break;
        case METHOD_PROCESSINGACK:
            m_ShowMap->ProcessingAck(IBOX_DEVICE_ADDRESS, senderHandle);
            result = true;
            break;
        case METHOD_RESULTACK:
// 2014.08.11, for log, hyunyong.sung
	    MOST_LOG_HIGH << LOG_TAG_NAVI_CNT << " ShowMap_ResultAck --> : SenderHandle = "
	    << senderHandle << " success = " << success << MOST_LOG_ENDL;				
//            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " onRegisterMyLocation:Success = " << success << MOST_LOG_ENDL;
            m_ShowMap->SenderHandleValue(senderHandle);
            m_ShowMap->SuccessValue((CShowMap::TSuccess)success);
            m_ShowMap->ResultAck(IBOX_DEVICE_ADDRESS);
            result = true;
            break;
        default:
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " Undefined Operation of SendShowMap = " << optype << MOST_LOG_ENDL;
            break;
    }

    return result;
}

bool CNavigationFblockControl::SendRegisterMyLocation(const uchar optype, const ushort senderHandle, const uchar success)
{
    bool result = false;
/*
    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " SendRegisterMyLocation API got called to send to IBOX" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendRegisterMyLocation:OPTYPE = " << optype << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendRegisterMyLocation:SenderHandle = " << senderHandle << MOST_LOG_ENDL;
*/
    switch(optype)
    {
        case METHOD_ERRORACK:
            m_RegisterMyLocation->ErrorAck(IBOX_DEVICE_ADDRESS, senderHandle, 0x41, 0, NULL);
            result = true;
            break;
        case METHOD_PROCESSINGACK:
            m_RegisterMyLocation->ProcessingAck(IBOX_DEVICE_ADDRESS, senderHandle);
            result = true;
            break;
        case METHOD_RESULTACK:
// 2014.08.11, for log, hyunyong.sung
	    MOST_LOG_HIGH << LOG_TAG_NAVI_CNT << " RegisterMyLocation_ResultAck --> : SenderHandle = "
	    << senderHandle << " success = " << success << MOST_LOG_ENDL;				
//            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendRegisterMyLocation:Success = " << success << MOST_LOG_ENDL;
            m_RegisterMyLocation->SenderHandleValue(senderHandle);
            m_RegisterMyLocation->SuccessValue((CRegisterMyLocation::TSuccess)success);
            m_RegisterMyLocation->ResultAck(IBOX_DEVICE_ADDRESS);
            result = true;
            break;
        default:
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " Undefined Operation of SendRegisterMyLocation = " << optype << MOST_LOG_ENDL;
            break;
    }

    return result;
}

bool CNavigationFblockControl::SendDestinationInfo(const uchar optype, const uchar routeSet, const SPOIData &poiData)
{
    bool result = false;    
/*
    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " SendDestinationInfo API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendDestinationInfo:OPTYPE = " << optype << MOST_LOG_ENDL;
*/
    switch(optype)
    {
        case PROPERTY_ERROR:
            result = m_DestinationInfo->Error(IBOX_DEVICE_ADDRESS, 0x41, 0, NULL);
            break;
        case PROPERTY_STATUS:
// 2014.08.11, for log, hyunyong.sung
	    MOST_LOG_HIGH << LOG_TAG_NAVI_CNT << " DestinationInfo_Status --> : RouteSet = " << routeSet
	    << " src = " << poiData.src << " lang = " << poiData.lang << " cate = " << poiData.cate << " rating = "
	    << poiData.rating << " phone = " << poiData.phone << " addr = " << poiData.addr << " zip = " << poiData.zip
	    << " name = " << poiData.name << " note = " << poiData.note << " url = " << poiData.url << " angle = "
	    << poiData.angle << " distance = " << poiData.distance << " direction = " << poiData.direction << " lat = "
	    << poiData.lat << " lon = " << poiData.lon << " type = " << poiData.type << MOST_LOG_ENDL;		
/*			
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendDestinationInfo:RouteSet = " << routeSet << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendDestinationInfo:POIData.src = " << poiData.src << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendDestinationInfo:POIData.lang = " << poiData.lang << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendDestinationInfo:POIData.cate = " << poiData.cate << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendDestinationInfo:POIData.rating = " << poiData.rating << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendDestinationInfo:POIData.phone = " << poiData.phone << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendDestinationInfo:POIData.addr = " << poiData.addr << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendDestinationInfo:POIData.zip = " << poiData.zip << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendDestinationInfo:POIData.name = " << poiData.name << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendDestinationInfo:POIData.note = " << poiData.note << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendDestinationInfo:POIData.url = " << poiData.url << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendDestinationInfo:POIData.angle = " << poiData.angle << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendDestinationInfo:POIData.distance = " << poiData.distance << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendDestinationInfo:POIData.direction = " << poiData.direction << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendDestinationInfo:POIData.lat = " << poiData.lat << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendDestinationInfo:POIData.lon = " << poiData.lon << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendDestinationInfo:POIData.type = " << poiData.type << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendDestinationInfo:POIData.alt = " << poiData.alt << MOST_LOG_ENDL;
*/
            m_DestinationInfo->RouteSetValue((CDestinationInfo::TRouteSet)routeSet);                        
            m_DestinationInfo->SrcValue((CDestinationInfo::TSrc)poiData.src);
            m_DestinationInfo->LangValue((CDestinationInfo::TLang)poiData.lang);
            m_DestinationInfo->CateValue((CDestinationInfo::TCate)poiData.cate);
            m_DestinationInfo->RatingString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                            reinterpret_cast<const uint8_t*>(poiData.rating.toUtf8().data()),
                                            poiData.rating.toUtf8().length()+1);
            m_DestinationInfo->PhoneString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                            reinterpret_cast<const uint8_t*>(poiData.phone.toUtf8().data()),
                                            poiData.phone.toUtf8().length()+1);
            m_DestinationInfo->AddrString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                            reinterpret_cast<const uint8_t*>(poiData.addr.toUtf8().data()),
                                            poiData.addr.toUtf8().length()+1);
            m_DestinationInfo->ZipString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                            reinterpret_cast<const uint8_t*>(poiData.zip.toUtf8().data()),
                                            poiData.zip.toUtf8().length()+1);
            m_DestinationInfo->NameString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                            reinterpret_cast<const uint8_t*>(poiData.name.toUtf8().data()),
                                            poiData.name.toUtf8().length()+1);
            m_DestinationInfo->NoteString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                            reinterpret_cast<const uint8_t*>(poiData.note.toUtf8().data()),
                                            poiData.note.toUtf8().length()+1);
            m_DestinationInfo->UrlString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                            reinterpret_cast<const uint8_t*>(poiData.url.toUtf8().data()),
                                            poiData.url.toUtf8().length()+1);
            m_DestinationInfo->AngleValue(poiData.angle);
            m_DestinationInfo->DistanceString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                            reinterpret_cast<const uint8_t*>(poiData.distance.toUtf8().data()),
                                            poiData.distance.toUtf8().length()+1);
            m_DestinationInfo->DirectionString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                            reinterpret_cast<const uint8_t*>(poiData.direction.toUtf8().data()),
                                            poiData.direction.toUtf8().length()+1);
            m_DestinationInfo->LatValue(poiData.lat);
            m_DestinationInfo->LonValue(poiData.lon);
            m_DestinationInfo->TypeValue((CDestinationInfo::TType)poiData.type);
            m_DestinationInfo->AltValue(poiData.alt);
            m_DestinationInfo->POIDataDataType().ResetWriter();
            m_DestinationInfo->POIDataSaveNext();

            result = m_DestinationInfo->UpdateShadows();
            break;
        default:
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " Undefined Operation of SendDestinationInfo" << MOST_LOG_ENDL;
            break;
    }

    return result;
}

bool CNavigationFblockControl::SendFindNearBy(const uchar optype, const ushort senderHandle, const uchar success)
{
    bool result = false;
/*
    MOST_LOG_SIGNAL << LOG_TAG_NAVI_CNT << " SendFindNearBy API got called to send to IBOX" << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendFindNearBy:OPTYPE = " << optype << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendFindNearBy:SenderHandle = " << senderHandle << MOST_LOG_ENDL;
*/
    switch(optype)
    {
        case METHOD_ERRORACK:
            m_FindNearBy->ErrorAck(IBOX_DEVICE_ADDRESS, senderHandle, 0x41, 0, NULL);
            result = true;
            break;
        case METHOD_PROCESSINGACK:
            m_FindNearBy->ProcessingAck(IBOX_DEVICE_ADDRESS, senderHandle);
            result = true;
            break;
        case METHOD_RESULTACK:
// 2014.08.11, for log, hyunyong.sung
	    MOST_LOG_HIGH << LOG_TAG_NAVI_CNT << " FindNearBy_ResultAck --> : Success = " << success << MOST_LOG_ENDL;		
					
//            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " SendFindNearBy:Success = " << success << MOST_LOG_ENDL;

            m_FindNearBy->SenderHandleValue(senderHandle);
            m_FindNearBy->SuccessValue((CFindNearBy::TSuccess)success);
            m_FindNearBy->ResultAck(IBOX_DEVICE_ADDRESS);
            result = true;
            break;
        default:
            MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " Undefined Operation of SendFindNearBy = " << optype << MOST_LOG_ENDL;
            break;
    }

    return result;
}


// Fcat v15.3.4
bool CNavigationFblockControl::SendCheckGPSBoundary(const uchar optype, const ushort senderHandle, const uchar gpsstate)
{
	bool result = false;
	switch(optype)
	{
		case METHOD_ERRORACK:
			m_CheckGPSBoundary->ErrorAck(IBOX_DEVICE_ADDRESS, senderHandle, 0x41, 0, NULL);
			result = true;
			break;
			
		case METHOD_PROCESSINGACK:
			m_CheckGPSBoundary->ProcessingAck(IBOX_DEVICE_ADDRESS, senderHandle);
			result = true;
			break;
			
		case METHOD_RESULTACK:
			MOST_LOG_HIGH << LOG_TAG_NAVI_CNT << " CheckGPSBoundary_ResultAck --> : GPSState = " << gpsstate << MOST_LOG_ENDL;		
			m_CheckGPSBoundary->SenderHandleValue(senderHandle);
			m_CheckGPSBoundary->GPSStateValue((CCheckGPSBoundary::TGPSState)gpsstate);	
			m_CheckGPSBoundary->ResultAck(IBOX_DEVICE_ADDRESS);
			result = true;
			break;
			
		default:
			MOST_LOG_INFO << LOG_TAG_NAVI_CNT << " Undefined Operation of SendCheckGPSBoundary = " << optype << MOST_LOG_ENDL;
			break;
	}

	return result;
}