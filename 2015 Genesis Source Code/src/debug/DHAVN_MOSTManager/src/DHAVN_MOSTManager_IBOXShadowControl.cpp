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
 * 7  Sep 2011  Neelima Khedad           Updated for comments     *
 * 15 Sep 2011  Amit Koparde             Added Debug Statement    *
 * 12 Oct 2011  Neelima Khedad           Updated structures for   *
 *                                       GISData and POIdata      *
 * 27 Oct 2011  Neelima Khedad           Adedd parameter poicount to Signal POIDataAvailable
 * 09 Jan 2012  Neelima Khedad           Modified the structure definition used for POIRequest Data.
 *                                       Now is defined as array of structure.
 * 26 Jan 2012  Neelima Khedad           Fixed Coverity Error.Initialized all structure varibles before using.
 * 06 Feb 2012  Neelima Khedad           Updated the extraction of data  QbyteArray  from QByteArray::fromrawdata()
 * 08 Feb 2011  Neelima Khedad           Using the latest Version of LOG_INIT function Logger module
 * 07 May 2012  Shiva Kumar              Added new signal GISDataAvailableError
 * 21 Jun 2012  Shiva Kumar              Modified for FCAT 1.17.1, FCAT 1.17.2 and FCAT 1.17.3
 * 17 Jul 2012  Shiva Kumar              Modified for FCAT 1.18.2
 * 26 Jul 2012  Shiva Kumar              Modified for CR 12089
 * 19 Sep 2012  Shiva Kumar              Modified for FCAT 1.19.0, 1.20.1, 1.20.2
 * 15 Nov 2012  Shiva Kumar              Modified for CR 15573
 * 19 Mar 2013  Hanhong Keum             Modified for FCAT 1.22.7
 *******************************************************************
 */

#include "DHAVN_MOSTManager_IBOXShadowControl.h"


/*
 *******************************************************************
 * Function:CIBOXShadowControl
 *
 * Description :This is constructor of CIBOXShadowControl class.
 *              creates the instances for all the defined parameters
 * Parameters :
 *    context    rw   It holds the instance of CMarshaledACSApplication.
 *
 * Returns :
 *
 *******************************************************************
 */

CIBOXShadowControl::CIBOXShadowControl(CMarshaledACSApplication *context)
    : CControlThreadBase(context, LOG_TAG_IBOX_CNT)
{
    LOG_INIT_V2(LOG_PREFIX_MOST)    

    /** Instance of IBox Shadow */
    m_IBoxShadow = new CIBox(0x01);
    m_pMOSTElement = m_IBoxShadow;

    /** IBox Property Listeners and Handlers */
    m_WiFiMode = new CWiFiMode(m_IBoxShadow);
    m_WifiModeListener = new CWifiModeListener(this);

    m_AppInstallStatus = new CAppInstallStatus(m_IBoxShadow);
    m_AppInstallStatusListener = new CAppInstallStatusListener(this);

    m_GISDataAvailable = new CGISDataAvailable(m_IBoxShadow);
    m_GISDataAvailableListener = new CGISDataAvailableListener(this);

    m_OpenBrowser = new COpenBrowser(m_IBoxShadow);
    m_OpenBrowserHandler = new COpenBrowserHandler(this);

    m_GISRequest = new CGISRequest(m_IBoxShadow);
    m_GISRequestHandler = new CGISRequestHandler(this);

    m_GISDataRequest = new CGISDataRequest(m_IBoxShadow);
    m_GISDataRequestHandler = new CGISDataRequestHandler(this);

    m_POIDataAvailable = new CPOIDataAvailable(m_IBoxShadow);
    m_POIDataAvailableListener = new CPOIDataAvailableListener(this);

    m_POIDataRequest = new CPOIDataRequest(m_IBoxShadow);
    m_POIDataRequestHandler = new CPOIDataRequestHandler(this);

    m_POFDataAvailable = new CReceivedPOF(m_IBoxShadow);
    m_POFDataAvailableListener = new CPOFDataAvailableListener(this);

    m_ReceivedPOI = new CReceivedPOI(m_IBoxShadow);
    m_ReceivedPOIListener = new CReceivedPOIListener(this);

    m_VoiceSearchOnline = new CVoiceSearchOnline(m_IBoxShadow);
    m_VoiceSearchOnlineHandler = new CVoiceSearchOnlineHandler(this);

    m_LSDataAvailable = new CLSDataAvailable(m_IBoxShadow);
    m_LSDataAvailableListener = new CLSDataAvailableListener(this);

    m_LSRequest = new CLSRequest(m_IBoxShadow);
    m_LSRequestHandler = new CLSRequestHandler(this);

    m_LSDataRequest = new CLSDataRequest(m_IBoxShadow);
    m_LSDataRequestHandler = new CLSDataRequestHandler(this);

    m_LSDataRequest = new CLSDataRequest(m_IBoxShadow);
    m_LSDataRequestHandler = new CLSDataRequestHandler(this);

    m_ACNActivated = new CACNActivated(m_IBoxShadow);
    m_ACNActivatedListener = new CACNActivatedListener(this);

    m_IBoxFblockStateListener = new CIBOXFblockStateListener(m_IBoxShadow,this);

    m_numberOfBlock = 0;

}

/*
 *******************************************************************
 * Function:~CIBOXShadowControl
 *
 * Description :Is a Destructor,removes all the registered listeners.
 *              and destroys all the instances
 * Parameters : none
 *
 * Returns : none
 *
 *******************************************************************
 */

CIBOXShadowControl::~CIBOXShadowControl()
{
    RemoveListeners();

    delete m_POFDataAvailableListener;
    delete m_POFDataAvailable;

    delete m_POIDataRequestHandler;
    delete m_POIDataRequest;

    delete m_POIDataAvailableListener;
    delete m_POIDataAvailable;

    delete m_GISDataRequestHandler;
    delete m_GISDataRequest;

    delete m_GISRequestHandler;
    delete m_GISRequest;

    delete m_OpenBrowserHandler;
    delete m_OpenBrowser;

    delete m_GISDataAvailableListener;
    delete m_GISDataAvailable;

    delete m_AppInstallStatusListener;
    delete m_AppInstallStatus;

    delete m_WifiModeListener;
    delete m_WiFiMode;

    delete m_IBoxShadow;

    delete m_ReceivedPOI;
    delete m_ReceivedPOIListener;

    delete m_VoiceSearchOnline;
    delete m_VoiceSearchOnlineHandler;

    delete m_LSDataAvailable;
    delete m_LSDataAvailableListener;

    delete m_LSRequest;
    delete m_LSRequestHandler;

    delete m_LSDataRequest;
    delete m_LSDataRequestHandler;

    delete m_ACNActivated;
    delete m_ACNActivatedListener;

    delete m_IBoxFblockStateListener;
}

/*
 *******************************************************************
 * Function:Initialize
 *
 * Description :Function initializes the MOST elements of IBox
 *             Shadow and adds to MOST stack.
 *
 * Parameters :None
 *
 * Returns :true or false
 *
 *******************************************************************
 */

bool CIBOXShadowControl::Initialize()
{
    /** Initialze notification for all parameters on Ibox Shadow */
    if(m_IBoxShadow->InitializeMOSTElement())
    {
        SetListeners();

        /** Setting the handlers for required parameters */
        m_OpenBrowser->SetHandler(m_OpenBrowserHandler);
        m_GISRequest->SetHandler(m_GISRequestHandler);
        m_GISDataRequest->SetHandler(m_GISDataRequestHandler);
        m_POIDataRequest->SetHandler(m_POIDataRequestHandler);
        m_VoiceSearchOnline->SetHandler(m_VoiceSearchOnlineHandler);
        m_LSRequest->SetHandler(m_LSRequestHandler);
        m_LSDataRequest->SetHandler(m_LSDataRequestHandler);

        m_Initialized = true;
    }

    return m_Initialized;
}

/*
 *******************************************************************
 * Function:SetListeners
 *
 * Description : Sets the listeners for all the properties defined
 *
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CIBOXShadowControl::SetListeners()
{
    /** Setting the listeners for Properties of Ibox*/

    m_WiFiMode->SetListener(m_WifiModeListener);
    m_AppInstallStatus->SetListener(m_AppInstallStatusListener);        
    m_GISDataAvailable->SetListener(m_GISDataAvailableListener);
    m_POIDataAvailable->SetListener(m_POIDataAvailableListener);
    m_POFDataAvailable->SetListener(m_POFDataAvailableListener);
    m_ReceivedPOI->SetListener(m_ReceivedPOIListener);
    m_IBoxShadow->SetListener(m_IBoxFblockStateListener);
    m_LSDataAvailable->SetListener(m_LSDataAvailableListener);
    m_ACNActivated->SetListener(m_ACNActivatedListener);

}

/*
 *******************************************************************
 * Function:RemoveListeners
 *
 * Description : Removes the listeners for all the properties registered.
 *
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CIBOXShadowControl::RemoveListeners()
{
    /** Removing the listeners for Properties of Ibox */   
    m_WiFiMode->RemoveListener(m_WifiModeListener);   
    m_AppInstallStatus->RemoveListener(m_AppInstallStatusListener);     
    m_GISDataAvailable->RemoveListener(m_GISDataAvailableListener);
    m_POIDataAvailable->RemoveListener(m_POIDataAvailableListener);
    m_POFDataAvailable->RemoveListener(m_POFDataAvailableListener);
    m_ReceivedPOI->RemoveListener(m_ReceivedPOIListener);
    m_LSDataAvailable->RemoveListener(m_LSDataAvailableListener);
    m_ACNActivated->RemoveListener(m_ACNActivatedListener);
}

bool CIBOXShadowControl::FBlockVisible()
{
    return m_IBoxShadow->FunctionBlockVisible();
}

/*
 *******************************************************************
 * Function:StartOpenBrowserRequest
 *
 * Description :Invoked by IBOXAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *      senderhandle     rw  handle for calling startresultack
 *      url              rw  Url of the link to be opened by browser
 *      target           rw  contains target display information
 *      opentype         rw  contains information such as url/search word
 * Returns : true if StartResultAck is sent else false
 *
 *******************************************************************
 */

bool CIBOXShadowControl::StartOpenBrowserRequest(const uint senderhandle,const uint target, const uint opentype, const QString text)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " StartOpenBrowserRequest API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " StartOpenBrowserRequest:Sender Handle = " << senderhandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " StartOpenBrowserRequest:target = " << target << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " StartOpenBrowserRequest:OpenType = " << opentype << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " StartOpenBrowserRequest:Text = " << text << MOST_LOG_ENDL;
*/

    m_OpenBrowser->SenderHandleValue(senderhandle);
    m_OpenBrowser->TargetValue((COpenBrowser::TTarget)target);
    m_OpenBrowser->OpenTypeValue(COpenBrowser::TOpenType(opentype));
    /** Updates the parameter URL  */
    m_OpenBrowser->TextString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                              reinterpret_cast<const uint8_t*>(text.toUtf8().data()),
                              text.length()+1);
    /** Sending start Result ACK */
    return m_OpenBrowser->StartResultAck();
}

/*
 *******************************************************************
 * Function:StartGISRequest
 *
 * Description :Invoked by IBOXAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *      gisRequestData   rw  Structure Which holds the GIS request data
 * Returns : true if StartResultAck is sent else false
 *
 *******************************************************************
 */

bool CIBOXShadowControl::StartGISRequest(const SGISRequestData gisRequestData)
{    
    m_GISRequest->SenderHandleValue(gisRequestData.senderhandle);
    m_GISRequest->URIString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                            reinterpret_cast<const uint8_t*>(gisRequestData.uri.toUtf8().data()),
                            gisRequestData.uri.length()+1);
    m_GISRequest->TIDString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                            reinterpret_cast<const uint8_t*>(gisRequestData.transactionID.toUtf8().data()),
                            gisRequestData.transactionID.length()+1);
    QByteArray data((const char *)gisRequestData.gisdata,gisRequestData.gisdata.length());		//hyunyong.sung 2013.2.18
    m_GISRequest->GISDataValue(reinterpret_cast<uint8_t*>(data.data()), data.length());
/*
    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " StartGISRequest API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " StartGISRequest:SenderHandle = " << gisRequestData.senderhandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " StartGISRequest:Uri = " << gisRequestData.uri.toAscii().data() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " StartGISRequest:GISData(Byte Array as Hex):: " << data.toHex().data() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " StartGISRequest:GISData Length :: " << data.length() << MOST_LOG_ENDL;
*/

// 2014.08.01, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_IBOX_CNT << " GISRequest_StartResultAck --> : SenderHandle = " 
	<< gisRequestData.senderhandle << " URI = " << gisRequestData.uri.toAscii().data() 
	<< " GISData = " << data.toHex().data() << MOST_LOG_ENDL;

    return m_GISRequest->StartResultAck();
}

/*
 *******************************************************************
 * Function:StartGISDataRequest
 *
 * Description :Invoked by IBOXAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *      gisRequestData   rw  Structure Which holds the GIS  request data
 * Returns : true if StartResultAck is sent else false
 *
 *******************************************************************
 */

bool CIBOXShadowControl::StartGISDataRequest(const SGISRequestData gisRequestData)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " StartGISDataRequest API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " StartGISDataRequest:SenderHandle = " << gisRequestData.senderhandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " StartGISDataRequest:Uri = " << gisRequestData.uri.toAscii().data() << MOST_LOG_ENDL;
*/
// 2014.08.01, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_IBOX_CNT << " GISDataRequest_StartResultAck --> : SenderHandle = " 
	<< gisRequestData.senderhandle << " URI = " << gisRequestData.uri.toAscii().data() << MOST_LOG_ENDL;

    m_GISDataRequest->SenderHandleValue(gisRequestData.senderhandle);
    m_GISDataRequest->URIString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                            reinterpret_cast<const uint8_t*>(gisRequestData.uri.toUtf8().data()),
                            gisRequestData.uri.length()+1);
    return m_GISDataRequest->StartResultAck();
}

/*
 *******************************************************************
 * Function:StartPOIDataRequest
 *
 * Description :Invoked by IBOXAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *      poiDataRequest   rw  Structure Which holds the POI  request data
 * Returns : true if StartResultAck is sent else false
 *
 *******************************************************************
 */

bool CIBOXShadowControl::StartPOIDataRequest(const SPOIDataRequest poiDataRequest)
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " StartPOIDataRequest API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " StartPOIDataRequest:SenderHandle = " << poiDataRequest.senderhandle << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " StartPOIDataRequest:uri = " << poiDataRequest.uri.toAscii().data() << MOST_LOG_ENDL;
*/
// 2014.08.01, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_IBOX_CNT << " POIDataRequest_StartResultAck --> : SenderHandle = " 
	<< poiDataRequest.senderhandle << " URI = " << poiDataRequest.uri.toAscii().data() << MOST_LOG_ENDL;

    m_POIDataRequest->SenderHandleValue(poiDataRequest.senderhandle);
    m_POIDataRequest->URIString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                reinterpret_cast<const uint8_t*>(poiDataRequest.uri.toUtf8().data()),
                                poiDataRequest.uri.length()+1);

    return m_POIDataRequest->StartResultAck();
}
/*
 *******************************************************************
 * Function:SetVoiceSearchOnline
 *
 * Description :Invoked by IBOXAdaptor to upadate data over ACS daemon
 *               to send it to Ibox
 * Parameters :
 *      voiceSearch   rw  contains Structure Which holds the voiceSearchOnline data
 *
 * Returns : true if StartResultAck is sent else false
 *
 *******************************************************************
 */

bool CIBOXShadowControl::SetVoiceSearchOnline(const uint voiceSearch)
{
    bool retValue = false;

//    MOST_LOG_SIGNAL << " SetVoiceSearchOnline called " << MOST_LOG_ENDL;

    /** Retrive the structure form QDbusArgument */
    if(voiceSearch == 0)
    {
        //shared memory connect
        m_xWavBlockStructureSharedMem.setKey("MostVRSharedData");
        m_xWavBlockStructureSharedMem.attach();
        m_xWavBlockStructureSharedMem.create(sizeof(m_xWavBlockStructure));

        //data read
        m_xWavBlockStructureSharedMem.lock();
        char *to = (char*)m_xWavBlockStructureSharedMem.data();
        memcpy( &m_xWavBlockStructure, to, sizeof(m_xWavBlockStructure));
        m_xWavBlockStructureSharedMem.unlock();

        MOST_LOG_INFO << " number of blocks " << m_xWavBlockStructure.nTotalBlockNumber << MOST_LOG_ENDL;

        if(voiceSearch < m_xWavBlockStructure.nTotalBlockNumber)
        {
            m_numberOfBlock++;
/*
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " blockType " << (uint)m_xWavBlockStructure.m_xWavBlock[voiceSearch].nBlockType << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " blockCounter " << (uint)m_xWavBlockStructure.m_xWavBlock[voiceSearch].nBlockCounter << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " blockSize " << m_xWavBlockStructure.m_xWavBlock[voiceSearch].nBlockSize << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " voiceData " << m_xWavBlockStructure.m_xWavBlock[voiceSearch].szWavBuffer << MOST_LOG_ENDL;
*/
            QByteArray voiceDataOnline((const char * )m_xWavBlockStructure.m_xWavBlock[voiceSearch].szWavBuffer,MAX_WAV_BUFFER_SIZE);	//hyunyong.sung 2013.2.18
            m_VoiceSearchOnline->SenderHandleValue(0x0001);
            m_VoiceSearchOnline->BlockTypeValue((CVoiceSearchOnline::TBlockType)m_xWavBlockStructure.m_xWavBlock[voiceSearch].nBlockType);
            m_VoiceSearchOnline->BlockCounterValue(m_xWavBlockStructure.m_xWavBlock[voiceSearch].nBlockCounter);
            m_VoiceSearchOnline->BlockSizeValue(m_xWavBlockStructure.m_xWavBlock[voiceSearch].nBlockSize);
            m_VoiceSearchOnline->VoiceDataValue(reinterpret_cast<uint8_t *>(voiceDataOnline.data()),voiceDataOnline.length());
            retValue = m_VoiceSearchOnline->StartResultAck();
         }
    }
    else if(voiceSearch < m_xWavBlockStructure.nTotalBlockNumber)
    {
/*
        MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " blockType " << (uint)m_xWavBlockStructure.m_xWavBlock[voiceSearch].nBlockType << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " blockCounter " << (uint)m_xWavBlockStructure.m_xWavBlock[voiceSearch].nBlockCounter << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " blockSize " << m_xWavBlockStructure.m_xWavBlock[voiceSearch].nBlockSize << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " voiceData " << m_xWavBlockStructure.m_xWavBlock[voiceSearch].szWavBuffer << MOST_LOG_ENDL;
*/
        QByteArray voiceDataOnline((const char * )m_xWavBlockStructure.m_xWavBlock[voiceSearch].szWavBuffer,MAX_WAV_BUFFER_SIZE);	//hyunyong.sung 2013.2.18
        m_VoiceSearchOnline->SenderHandleValue(0x0001);
        m_VoiceSearchOnline->BlockTypeValue((CVoiceSearchOnline::TBlockType)m_xWavBlockStructure.m_xWavBlock[voiceSearch].nBlockType);
        m_VoiceSearchOnline->BlockCounterValue(m_xWavBlockStructure.m_xWavBlock[voiceSearch].nBlockCounter);
        m_VoiceSearchOnline->BlockSizeValue(m_xWavBlockStructure.m_xWavBlock[voiceSearch].nBlockSize);
        m_VoiceSearchOnline->VoiceDataValue(reinterpret_cast<uint8_t *>(voiceDataOnline.data()),voiceDataOnline.length());
        retValue = m_VoiceSearchOnline->StartResultAck();
    }

// 2014.08.01, for log, hyunyong.sung
	MOST_LOG_HIGH << LOG_TAG_IBOX_CNT << " VoiceSearchOnline_StartResultAck --> : blockType = " 
	<< (uint)m_xWavBlockStructure.m_xWavBlock[voiceSearch].nBlockType << " blockCounter = "  
	<< (uint)m_xWavBlockStructure.m_xWavBlock[voiceSearch].nBlockCounter << " blockSize  = " 
	<< m_xWavBlockStructure.m_xWavBlock[voiceSearch].nBlockSize << " voiceData = "
	<< m_xWavBlockStructure.m_xWavBlock[voiceSearch].szWavBuffer << MOST_LOG_ENDL;

    return retValue;
}

/*
 *******************************************************************
 * Function:WifiMode
 *
 * Description : Invoked by the Listener for WifiMode of Ibox
 *               Sends the signal to IBOX Adaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CIBOXShadowControl::WifiMode()
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " WifiMode Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " WifiMode: Mode = " << (uint)m_WiFiMode->WiFiModeValue() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " WifiMode: Mode = " << (uint)m_WiFiMode->ClientStatusValue() << MOST_LOG_ENDL;
*/
// 2014.08.01, for log, hyunyong.sung
	MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " WifiMode_Status <-- : Wifimode = " << (uint)m_WiFiMode->WiFiModeValue()
	<< " ClientStatus = " << (uint)m_WiFiMode->ClientStatusValue() << MOST_LOG_ENDL;

    emit WifiMode(m_WiFiMode->WiFiModeValue(), m_WiFiMode->ClientStatusValue());
}

/*
 *******************************************************************
 * Function:AppInstallStatus
 *
 * Description : Invoked by the Listener for AppInstallStatus of Ibox
 *               Sends the signal to IBOX Adaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CIBOXShadowControl::AppInstallStatus()
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " AppInstallStatus Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " AppInstallStatus:App Status = " << (uint)m_AppInstallStatus->AppStatusValue() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " AppInstallStatus:App Progress = " <<  (uint)m_AppInstallStatus->AppProgressValue() << MOST_LOG_ENDL;
*/
    emit AppInstallStatus(m_AppInstallStatus->AppStatusValue(), m_AppInstallStatus->AppProgressValue());
}

/*
 *******************************************************************
 * Function:GISDataAvailable
 *
 * Description : Invoked by the Listener for GISDataAvailable of Ibox
 *               Sends the signal to IBOX Adaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CIBOXShadowControl::GISDataAvailable()
{
    const ::k2l::acs::protocols::most::string::IStringEncoding *pEncoding = NULL;
    uint8_t *pString = NULL;
    uint32_t stringLength = 0;
    m_GISDataAvailable->URIString(&pEncoding,&pString,&stringLength);
    QString uri = QString::fromUtf8((const char *)pString,stringLength);
/*
    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " GISDataAvailable Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " GISDataAvailable:Uri = " << uri.toAscii().data() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " GISDataAvailable:Available = " << (uint)m_GISDataAvailable->AvailableValue() << MOST_LOG_ENDL;
*/
// 2014.08.01, for log, hyunyong.sung
	MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " GISDataAvailable_Status <-- : URI = " << uri.toAscii().data()
	<< " Available = " << (uint)m_GISDataAvailable->AvailableValue() << MOST_LOG_ENDL;

    emit GISDataAvailable(uri,m_GISDataAvailable->AvailableValue());
}
/*
 *******************************************************************
 * Function:ErrorGISDataSlot
 *
 * Description : Invoked by the Listener for GISDataAvailableError of Ibox
 *               Sends the signal to IBOX Adaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CIBOXShadowControl::ErrorGISDataSlot()
{    
//    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " GISDataAvailableError Signal got received " << MOST_LOG_ENDL;

    uint8_t errorCode = 0;
    uint32_t errorInfoLength = 0;
    const uint8_t* errorInfoPayload = NULL;
    uint errorInfo = 0;

    if (k2l::acs::samples::CMOSTErrorInfo::GetMOSTError(
            m_IBoxShadow->Container(),
            errorCode,
            errorInfoLength,
            errorInfoPayload
            ))
    {
        //Retrieved error information correctly.
        for(int i=0; i<errorInfoLength; i++)
        {
            errorInfo |= errorInfoPayload[i] << (errorInfoLength-i-1)*8;
        }
/*
        MOST_LOG_CRITICAL << LOG_TAG_IBOX_CNT << " Error code : " << errorCode << MOST_LOG_ENDL;
        MOST_LOG_CRITICAL << LOG_TAG_IBOX_CNT << " Error Info Length : " << errorInfoLength << MOST_LOG_ENDL;
        MOST_LOG_CRITICAL << LOG_TAG_IBOX_CNT << " Error Info Payload : " << errorInfo << MOST_LOG_ENDL;
*/        
    }
// 2014.08.01, for log, hyunyong.sung
    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " GISDataAvailable_Error <-- :  errorcode "
    << errorCode << " errorInfo = " << errorInfo << MOST_LOG_ENDL;

    emit GISDataAvailableError(errorCode, errorInfo);
}

/*
 *******************************************************************
 * Function:OpenBrowserRequestResult
 *
 * Description : Invoked by the Handler for Open browser of Ibox
 *               Sends the result signal to IBOX Adaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CIBOXShadowControl::OpenBrowserRequestResult(const uint state, const uint32_t payloadLength, const uint8_t *payload)
{

    if(state == IBOX_PROCESSING_ACK)
    {        
        uint16_t senderHandle;
        m_OpenBrowser->ParseSenderHandle(payloadLength,payload,&senderHandle);
/*
        MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " OpenBrowserRequestResult Signal got received :: MOSTMSG_PROCESSING_ACK" << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " OpenBrowserRequestResult:Sender Handle = " << senderHandle  << MOST_LOG_ENDL;
*/
        emit OpenBrowserRequestResult(senderHandle,state);
    }
    else if(state == IBOX_RESULT_ACK)
    {
        m_OpenBrowser->ParseResultAck(payloadLength,payload);
/*
        MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " OpenBrowserRequestResult Signal got received :: MOSTMSG_RESULT_ACK" << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " OpenBrowserRequestResult:Sender Handle = " << m_OpenBrowser->SenderHandleValue()  << MOST_LOG_ENDL;
*/
        emit OpenBrowserRequestResult(m_OpenBrowser->SenderHandleValue(),state);
    }

}

/*
 *******************************************************************
 * Function:GISRequestResult
 *
 * Description : Invoked by the Handler for GISRequest of Ibox
 *               Sends the result signal to IBOX Adaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CIBOXShadowControl::GISRequestResult(const uint state, const uint32_t payloadLength, const uint8_t *payload)
{    
    /** Initialize the Structure variables */
    SGISRequestData requestResult;
    requestResult.state = state;
    requestResult.senderhandle = 0;
    requestResult.uri = (const char *)0;
    requestResult.transactionID = (const char *)0;
    requestResult.success = false;
    requestResult.gisdata.clear();

    if(state == IBOX_PROCESSING_ACK)
    {
        uint16_t senderHandle;
        m_GISRequest->ParseSenderHandle(payloadLength,payload,&senderHandle);
        requestResult.senderhandle = senderHandle;
/*
        MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " GISRequestResult Signal got received :: MOSTMSG_PROCESSING_ACK" << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "GISRequestResult:SenderHandle = " << requestResult.senderhandle  << MOST_LOG_ENDL;
*/        
    }
    else if(state == IBOX_RESULT_ACK)
    {
        m_GISRequest->ParseResultAck(payloadLength,payload);
        requestResult.senderhandle = m_GISRequest->SenderHandleValue();
        requestResult.success = m_GISRequest->SuccessValue();        
/*
        MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " GISRequestResult Signal got received :: MOSTMSG_RESULT_ACK" << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "GISRequestResult:SenderHandle = " << requestResult.senderhandle  << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "GISRequestResult:Success = " << requestResult.success << MOST_LOG_ENDL;
*/        
    }

// 2014.08.01, for log, hyunyong.sung
    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " GISRequest_ResultAck <-- :  SenderHandle "
    << requestResult.senderhandle << " success = " << requestResult.success << MOST_LOG_ENDL;

    /** Wrap the Structure in QDbusVariant */
    QVariant resultDataVar;
    QDBusVariant dBusResultDataVar;

    QDBusArgument resultDataArg;
    resultDataArg << requestResult;
    resultDataVar.setValue(resultDataArg);

    dBusResultDataVar.setVariant(resultDataVar);

    /** Send the Signal to HMI adaptor */
    emit GISRequestResult(dBusResultDataVar);
}

/*
 *******************************************************************
 * Function:GISDataRequestResult
 *
 * Description : Invoked by the Handler for GISDataRequest of Ibox
 *               Sends the result signal to IBOX Adaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CIBOXShadowControl::GISDataRequestResult(const uint state, const uint32_t payloadLength, const uint8_t *payload)
{
    /** Initialize the Structure variables */
    const ::k2l::acs::protocols::most::string::IStringEncoding *pEncoding = NULL;
    uint8_t *pString = NULL;
    uint32_t stringLength = 0;

    SGISRequestData requestResult;
    requestResult.state = state;
    requestResult.senderhandle = 0;
    requestResult.uri = (const char *)0;
    requestResult.transactionID = (const char *)0;
    requestResult.success = false;
    requestResult.gisdata.clear();

    if(state == IBOX_PROCESSING_ACK)
    {
        uint16_t senderHandle;
        m_GISDataRequest->ParseSenderHandle(payloadLength,payload,&senderHandle);
        requestResult.senderhandle = senderHandle;
/*
        MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " GISDataRequestResult Signal got received :: MOSTMSG_PROCESSING_ACK " << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "GISDataRequestResult:SenderHandle = " << requestResult.senderhandle  << MOST_LOG_ENDL;
*/        
    }
    else if(state == IBOX_RESULT_ACK)
    {
        m_GISDataRequest->ParseResultAck(payloadLength,payload);
        requestResult.senderhandle = m_GISDataRequest->SenderHandleValue();
        m_GISDataRequest->URIString(&pEncoding,&pString,&stringLength);
        requestResult.uri = QString::fromUtf8((const char *)pString,stringLength);
        m_GISDataRequest->TIDString(&pEncoding,&pString,&stringLength);
        requestResult.transactionID = QString::fromUtf8((const char *)pString,stringLength);        

        QByteArray gisdata((const char *)m_GISDataRequest->GISDataValue(), m_GISDataRequest->GISDataStreamLength());
        requestResult.gisdata = gisdata;
/*
        MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " GISDataRequestResult Signal got received :: MOSTMSG_RESULT_ACK" << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "GISDataRequestResult:SenderHandle = " << requestResult.senderhandle  << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "GISDataRequestResult:Uri = " << requestResult.uri.toAscii().data() << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "GISDataRequestResult:transactionID " << requestResult.transactionID.toAscii().data() << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "GISDataRequestResult:GISData(Byte Array as Hex):: " << requestResult.gisdata.toHex().data() << MOST_LOG_ENDL;
*/        
    }
// 2014.08.01, for log, hyunyong.sung
    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " GISDataRequest_ResultAck <-- :  SenderHandle "
    << requestResult.senderhandle << " URI = " << requestResult.uri.toAscii().data() 
    << " transactionID = " << requestResult.transactionID.toAscii().data() << " GISData = "
    << requestResult.gisdata.toHex().data() << MOST_LOG_ENDL;

    /** Wrap the Structure in QDbusVariant */
    QVariant resultDataVar;
    QDBusVariant dBusResultDataVar;

    QDBusArgument resultDataArg;
    resultDataArg << requestResult;
    resultDataVar.setValue(resultDataArg);

    dBusResultDataVar.setVariant(resultDataVar);

    /** Send the Signal to HMI adaptor */
    emit GISDataRequestResult(dBusResultDataVar);
}

/*
 *******************************************************************
 * Function:POIDataAvailable
 *
 * Description : Invoked by the Listener for POIDataAvailable of Ibox
 *               Sends the signal to IBOX Adaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CIBOXShadowControl::POIDataAvailable()
{    
    const ::k2l::acs::protocols::most::string::IStringEncoding *pEncoding = NULL;
    uint8_t *pString = NULL;
    uint32_t stringLength = 0;

    uint target = (uint)m_POIDataAvailable->TargetValue();
    uint available = (uint)m_POIDataAvailable->AvailableValue();
    m_POIDataAvailable->URIString(&pEncoding,&pString,&stringLength);
    QString uri = QString::fromUtf8((const char *)pString,stringLength);
/*
    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " POIDataAvailable Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " POIDataAvailable:Target = " << target << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " POIDataAvailable:Available = " << available << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " POIDataAvailable:uri = " << uri.toAscii().data() << MOST_LOG_ENDL;
*/
// 2014.08.01, for log, hyunyong.sung
    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " POIDataAvailable_Status <-- : target = "
    << target << " available = " << available << " uri = " << uri.toAscii().data() << MOST_LOG_ENDL;

    emit POIDataAvailable(target, available, uri);
}
/*
 *******************************************************************
 * Function:ReceivedPOI
 *
 * Description : Invoked by the Listener for ReceivedPOI of Ibox
 *               Sends the signal to IBOX Adaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CIBOXShadowControl::ReceivedPOI()
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " ReceivedPOI Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " ReceivedPOI:Available = " << (uint)m_ReceivedPOI->AvailableValue() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " ReceivedPOI:POI Count = " << m_ReceivedPOI->POICountValue() << MOST_LOG_ENDL;
*/
// 2014.08.01, for log, hyunyong.sung
    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " ReceivedPOI_Status <-- : available = "
    << (uint)m_ReceivedPOI->AvailableValue() << " poiCount = " << m_ReceivedPOI->POICountValue() << MOST_LOG_ENDL;

    emit ReceivedPOI(m_ReceivedPOI->AvailableValue(), m_ReceivedPOI->POICountValue());
}

/*
 *******************************************************************
 * Function:POFDataAvailable
 *
 * Description : Invoked by the Listener for POFDataAvailable of Ibox
 *               Sends the signal to IBOX Adaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CIBOXShadowControl::POFDataAvailable()
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " POFDataAvailable Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " POFDataAvailable:Available = " << (uint)m_POFDataAvailable->AvailableValue() << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " POFDataAvailable:POF Count = " << m_POFDataAvailable->POFCountValue() << MOST_LOG_ENDL;
*/
// 2014.08.01, for log, hyunyong.sung
    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " ReceivedPOF_Status <-- : available = "
    << (uint)m_POFDataAvailable->AvailableValue() << " pofCount = " << m_POFDataAvailable->POFCountValue() << MOST_LOG_ENDL;

    emit POFDataAvailable(m_POFDataAvailable->AvailableValue(), m_POFDataAvailable->POFCountValue());
}
/*
 *******************************************************************
 * Function:IBOXFunctionBlockInvisibleIBOX
 *
 * Description : Invoked by the Listener for IBOX Fblock visibility
 *               Sends the signal to IBOX Adaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CIBOXShadowControl::IBOXFunctionBlockInvisibleIBOX()
{    
    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " IBOX FBlock is Not Visible " << MOST_LOG_ENDL;
    emit IBOXFunctionBlockInvisible(true);
}

/*
 *******************************************************************
 * Function:POIDataRequestResult
 *
 * Description : Invoked by the Handler for POIDataRequest of Ibox
 *               Sends the result signal to IBOX Adaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CIBOXShadowControl::POIDataRequestResult(const uint state, const uint32_t payloadLength, const uint8_t *payload)
{
    /** Initialize the Structure variables */
    SPOIDataRequest requestResult;
    requestResult.state = state;
    requestResult.senderhandle = 0;
    requestResult.uri = (const char *)0;
    requestResult.arrayCount = 0;
    requestResult.attributionsInfo = (const char *)0;
    requestResult.poiDataList.clear();

    if(state == IBOX_PROCESSING_ACK)
    {
        uint16_t senderHandle;
        m_POIDataRequest->ParseSenderHandle(payloadLength,payload,&senderHandle);
        requestResult.senderhandle = senderHandle;
/*
        MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " POIDataRequestResult Signal got received :: MOSTMSG_PROCESSING_ACK " << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:SenderHandle = " << requestResult.senderhandle  << MOST_LOG_ENDL;
*/
    }
    else if(state == IBOX_RESULT_ACK)
    {
//        MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " POIDataRequestResult Signal got received :: MOSTMSG_RESULT_ACK" << MOST_LOG_ENDL;

        if(m_POIDataRequest->ParseResultAck(payloadLength,payload))
        {
            uint32_t index = 0;
            const ::k2l::acs::protocols::most::string::IStringEncoding *pEncoding = NULL;
            uint8_t *pString = NULL;
            uint32_t stringLength = 0;

            while(m_POIDataRequest->POIDataDataType().LoadNext())
            {
                struct SPOIData poiData;

                poiData.src = m_POIDataRequest->SrcValue();
                poiData.lang  = m_POIDataRequest->LangValue();
                poiData.cate = m_POIDataRequest->CateValue();

                m_POIDataRequest->RatingString(&pEncoding, &pString, &stringLength);
                poiData.rating = QString::fromUtf8((const char *)pString,stringLength);

                m_POIDataRequest->PhoneString(&pEncoding, &pString, &stringLength);
                poiData.phone = QString::fromUtf8((const char *)pString,stringLength);


                m_POIDataRequest->AddrString(&pEncoding, &pString, &stringLength);
                poiData.addr = QString::fromUtf8((const char *)pString,stringLength);

                m_POIDataRequest->ZipString(&pEncoding, &pString, &stringLength);
                poiData.zip = QString::fromUtf8((const char *)pString,stringLength);

                m_POIDataRequest->NameString(&pEncoding, &pString, &stringLength);
                poiData.name = QString::fromUtf8((const char *)pString,stringLength);

                m_POIDataRequest->NoteString(&pEncoding, &pString, &stringLength);
                poiData.note = QString::fromUtf8((const char *)pString,stringLength);

                m_POIDataRequest->UrlString(&pEncoding, &pString, &stringLength);
                poiData.url = QString::fromUtf8((const char *)pString,stringLength);

                poiData.angle = m_POIDataRequest->AngleValue();

                m_POIDataRequest->DistanceString(&pEncoding, &pString, &stringLength);
                poiData.distance = QString::fromUtf8((const char *)pString,stringLength);

                m_POIDataRequest->DirectionString(&pEncoding, &pString, &stringLength);
                poiData.direction = QString::fromUtf8((const char *)pString,stringLength);

                poiData.lat = m_POIDataRequest->LatValue();
                poiData.lon = m_POIDataRequest->LonValue();
                poiData.type = m_POIDataRequest->TypeValue();
                poiData.alt = m_POIDataRequest->AltValue();

                requestResult.poiDataList.append(poiData);
                ++index;
                requestResult.arrayCount = index;
            }

            requestResult.senderhandle = m_POIDataRequest->SenderHandleValue();

            m_POIDataRequest->URIString(&pEncoding,&pString,&stringLength);
            requestResult.uri = QString::fromUtf8((const char *)pString,stringLength);

            m_POIDataRequest->AttributionsInfoString(&pEncoding,&pString,&stringLength);
            requestResult.attributionsInfo = QString::fromUtf8((const char *)pString,stringLength);
/*
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:SenderHandle = " << requestResult.senderhandle  << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:ArrayCount = " << requestResult.arrayCount  << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:URI = " << requestResult.uri.toAscii().data() << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:AttributionsInfo = " << requestResult.attributionsInfo.toAscii().data() << MOST_LOG_ENDL;
*/
            if(requestResult.arrayCount > 0)
            {
/*
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:Src[0] = " << requestResult.poiDataList.at(0).src  << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:Lang[0] = " << requestResult.poiDataList.at(0).lang  << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:Cate[0 = " << requestResult.poiDataList.at(0).cate  << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:Rating[0] = " << requestResult.poiDataList.at(0).rating  << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:Phone[0] = " << requestResult.poiDataList.at(0).phone  << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:Addr[0] = " << requestResult.poiDataList.at(0).addr  << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:Zip[0] = " << requestResult.poiDataList.at(0).zip  << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:Name[0] = " << requestResult.poiDataList.at(0).name  << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:Note[0] = " << requestResult.poiDataList.at(0).note  << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:url[0] = " << requestResult.poiDataList.at(0).url  << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:Angle[0] = " << requestResult.poiDataList.at(0).angle  << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:Distance[0] = " << requestResult.poiDataList.at(0).distance  << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:Direction[0] = " << requestResult.poiDataList.at(0).direction  << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:Lat[0] = " << requestResult.poiDataList.at(0).lat  << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:Lon[0] = " << requestResult.poiDataList.at(0).lon  << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:Alt[0] = " << requestResult.poiDataList.at(0).alt  << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "POIDataRequestResult:Type[0] = " << requestResult.poiDataList.at(0).type << MOST_LOG_ENDL;
*/                
            }
        }
    }

// 2014.08.01, for log, hyunyong.sung
        MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " POIDataRequest_ResultAck <-- : SenderHandle = " 
        << requestResult.senderhandle << " uri = " << requestResult.uri.toAscii().data() << " attributionsInfo = " 
        << requestResult.attributionsInfo.toAscii().data() << " arrayCount = " << requestResult.arrayCount << " src = " 
        << requestResult.poiDataList.at(0).src << " Lang = " << requestResult.poiDataList.at(0).lang << " cate = " 
        << requestResult.poiDataList.at(0).cate << " rating = " << requestResult.poiDataList.at(0).rating << " phone = " 
        << requestResult.poiDataList.at(0).phone << " addr = " << requestResult.poiDataList.at(0).addr << " zip = " 
        << requestResult.poiDataList.at(0).zip << " name = " << requestResult.poiDataList.at(0).name << " note = " 
        << requestResult.poiDataList.at(0).note << " url = " << requestResult.poiDataList.at(0).url << " angle = "
        << requestResult.poiDataList.at(0).angle << " distance = " << requestResult.poiDataList.at(0).distance << " direction = " 
        << requestResult.poiDataList.at(0).direction << " lat = " << requestResult.poiDataList.at(0).lat << " lon = " 
        << requestResult.poiDataList.at(0).lon << " alt = " << requestResult.poiDataList.at(0).alt << " type = " 
        << requestResult.poiDataList.at(0).type << MOST_LOG_ENDL;

    /** Wrap the Structure in QDbusVariant */
    QVariant resultDataVar;
    QDBusVariant dBusResultDataVar;

    QDBusArgument resultDataArg;
    resultDataArg << requestResult;
    resultDataVar.setValue(resultDataArg);

    dBusResultDataVar.setVariant(resultDataVar);

    /** Send the Signal to IBOX adaptor */
    emit POIDataRequestResult(dBusResultDataVar);
}
/*
 *******************************************************************
 * Function:VoiceSearchOnlineHandler
 *
 * Description : Invoked by the Handler for VoiceSearchOnline of Ibox
 *               Sends the result signal to IBOX Adaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CIBOXShadowControl::VoiceSearchOnlineHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state)
{    
//    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " VoiceSearchOnline Signal got recived from source " << sourceDeviceID << MOST_LOG_ENDL;

    uint16_t senderHandle = 0;
    uint blockType = 0;
    uint blockCounter = 0;
    uint messageState = 0;

    switch(state)
    {
        case STATUS_PROCESSINGACK:
            {
                messageState = IBOX_PROCESSING_ACK;
                m_VoiceSearchOnline->ParseSenderHandle(payloadLength,payload,&senderHandle);
/*
                MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " VoiceSearchOnline Signal got received :: MOSTMSG_PROCESSING_ACK" << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " VoiceSearchOnline : senderHandle " << senderHandle << MOST_LOG_ENDL;
*/				
            }
            break;
        case STATUS_RESULTACK:
            {
                messageState = IBOX_RESULT_ACK;
                m_VoiceSearchOnline->ParseResultAck(payloadLength,payload);
                senderHandle = m_VoiceSearchOnline->SenderHandleValue();
                blockType = m_VoiceSearchOnline->BlockTypeValue();
                blockCounter = m_VoiceSearchOnline->BlockCounterValue();
/*
                MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " VoiceSearchOnline Signal got received :: MOSTMSG_RESULT_ACK" << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " VoiceSearchOnline : senderHandle " << senderHandle << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " VoiceSearchOnline : BlockType " << blockType << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " VoiceSearchOnline : BlockCounter " << blockCounter << MOST_LOG_ENDL;
*/                
            }
            break;
        case STATUS_ERRORACK:
            messageState = IBOX_ERROR_ACK;
            break;
        case STATUS_UNDEFINED:
            break;
        default:
            break;
    }

// 2014.08.01, for log, hyunyong.sung
	MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " VoiceSearchOnline_ResultAck <-- : senderHandle = "
	<< senderHandle << " blocktype = " << blockType << " blockcounter = " << blockCounter << MOST_LOG_ENDL;

    emit VoiceSearchOnlineStatus(messageState,senderHandle,blockType,blockCounter);
}

/*
 *******************************************************************
 * Function:CWifiModeListener
 *
 * Description :Constructor for CWifiModeListener.
 *              Stores the context of CIBOXShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

CWifiModeListener::CWifiModeListener(CIBOXShadowControl *context)
{
    m_IBOXShadowControl = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property WifiMode
 *               is changed by Ibox.
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CWifiModeListener::OnChange()
{
    m_IBOXShadowControl->WifiMode();
}

/*
 *******************************************************************
 * Function:CAppInstallStatusListener
 *
 * Description :Constructor for CAppInstallStatusListener.
 *              Stores the context of CIBOXShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

CAppInstallStatusListener::CAppInstallStatusListener(CIBOXShadowControl *context)
{
    m_IBOXShadowControl = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property AppInstallStatus
 *               is changed by Ibox.
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CAppInstallStatusListener::OnChange()
{
    m_IBOXShadowControl->AppInstallStatus();
}


/*
 *******************************************************************
 * Function:CGISDataAvailableListener
 *
 * Description :Constructor for CGISDataAvailableListener.
 *              Stores the context of CIBOXShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

CGISDataAvailableListener::CGISDataAvailableListener(CIBOXShadowControl *context)
{
    m_IBOXShadowControl = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property GISDataAvailable
 *               is changed by Ibox.
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

void CGISDataAvailableListener::OnChange()
{
    m_IBOXShadowControl->GISDataAvailable();
}
/*
 *******************************************************************
 * Function:OnError
 *
 * Description : Method invoked by ACS Daemon when property GISDataAvailable.Error
 *               message sent by Ibox.
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CGISDataAvailableListener::OnError()
{
    m_IBOXShadowControl->ErrorGISDataSlot();
}
/*
 *******************************************************************
 * Function:CIBOXFblockStateListener
 *
 * Description :Constructor for CIBOXFblockStateListener.
 *              Stores the context of CIBOXShadowControl and ibox shadow
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
CIBOXFblockStateListener::CIBOXFblockStateListener(CIBox *iboxContext, CIBOXShadowControl *context)
{
    m_Ibox = iboxContext;
    m_IBOXShadowControl = context;
}
/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when IBOX Fblock is invisible
 *
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CIBOXFblockStateListener::OnChange()
{
    if( !m_Ibox->FunctionBlockVisible() )
    {
        m_IBOXShadowControl->IBOXFunctionBlockInvisibleIBOX();
    }
}
/*
 *******************************************************************
 * Function:COpenBrowserHandler
 *
 * Description :Constructor for COpenBrowserHandler.
 *              Stores the context of CIBOXShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

COpenBrowserHandler::COpenBrowserHandler(CIBOXShadowControl *context)
{
    m_IBOXShadowControl = context;
}
/** Callback methods for the OpenBrowser Handler */
void COpenBrowserHandler::Error(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_IBOXShadowControl->OpenBrowserHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
void COpenBrowserHandler::ErrorAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_IBOXShadowControl->OpenBrowserHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
void COpenBrowserHandler::Interface(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_IBOXShadowControl->OpenBrowserHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
void COpenBrowserHandler::Processing(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_IBOXShadowControl->OpenBrowserHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
void COpenBrowserHandler::Result(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
    m_IBOXShadowControl->OpenBrowserHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void COpenBrowserHandler::ProcessingAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{   
    m_IBOXShadowControl->OpenBrowserHandler(sourceDeviceID,payloadLength,payload,STATUS_PROCESSINGACK);
}

/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void COpenBrowserHandler::ResultAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{     
    m_IBOXShadowControl->OpenBrowserHandler(sourceDeviceID,payloadLength,payload,STATUS_RESULTACK);
}

/*
 *******************************************************************
 * Function:CGISRequestHandler
 *
 * Description :Constructor for CGISRequestHandler.
 *              Stores the context of CIBOXShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */

CGISRequestHandler::CGISRequestHandler(CIBOXShadowControl *context)
{
    m_IBOXShadowControl = context;
}
/** CallBack methods for GISRequest Handler*/
 void CGISRequestHandler::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
 {
     m_IBOXShadowControl->GISRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
 }

 void CGISRequestHandler::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
 {
     m_IBOXShadowControl->GISRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
 }
 void CGISRequestHandler:: Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
 {
     m_IBOXShadowControl->GISRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
 }
 void CGISRequestHandler::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
 {
     m_IBOXShadowControl->GISRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
 }
 void CGISRequestHandler::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
 {
     m_IBOXShadowControl->GISRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
 }

 /*
  *******************************************************************
  * Function:ProcessingAck
  *
  * Description :Callback for received ProcessingAck Operation
  *
  * Parameters :
  *    sourceDeviceID    rw   The Device which has sent this Message
  *    payloadLength     rw   The length of Payload data
  *    payload           rw   The data in the form of Payload
  * Returns :None
  *
  *******************************************************************
  */

void CGISRequestHandler::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_IBOXShadowControl->GISRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_PROCESSINGACK);
}

 /*
  *******************************************************************
  * Function:ResultAck
  *
  * Description :Callback for received ResultAck Operation
  *
  * Parameters :
  *    sourceDeviceID    rw   The Device which has sent this Message
  *    payloadLength     rw   The length of Payload data
  *    payload           rw   The data in the form of Payload
  * Returns :None
  *
  *******************************************************************
  */

void CGISRequestHandler::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_IBOXShadowControl->GISRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_RESULTACK);

}

 /*
  *******************************************************************
  * Function:CGISDataRequestHandler
  *
  * Description :Constructor for CGISDataRequestHandler.
  *              Stores the context of CIBOXShadowControl
  * Parameters : None
  *
  * Returns :None
  *
  *******************************************************************
  */

 CGISDataRequestHandler::CGISDataRequestHandler(CIBOXShadowControl *context)
 {
     m_IBOXShadowControl = context;
 }
/** Callback methods for GISDataRequest Handler */
 void CGISDataRequestHandler::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
 {
     m_IBOXShadowControl->GISDataRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
 }

 void CGISDataRequestHandler::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
 {
     m_IBOXShadowControl->GISDataRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
 }

 void CGISDataRequestHandler:: Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
 {
     m_IBOXShadowControl->GISDataRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
 }

 void CGISDataRequestHandler::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
 {
     m_IBOXShadowControl->GISDataRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
 }

 void CGISDataRequestHandler::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
 {
     m_IBOXShadowControl->GISDataRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
 }


 /*
  *******************************************************************
  * Function:ProcessingAck
  *
  * Description :Callback for received ProcessingAck Operation
  *
  * Parameters :
  *    sourceDeviceID    rw   The Device which has sent this Message
  *    payloadLength     rw   The length of Payload data
  *    payload           rw   The data in the form of Payload
  * Returns :None
  *
  *******************************************************************
  */

 void CGISDataRequestHandler::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
 {
    m_IBOXShadowControl->GISDataRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_PROCESSINGACK);
 }

 /*
  *******************************************************************
  * Function:ResultAck
  *
  * Description :Callback for received ResultAck Operation
  *
  * Parameters :
  *    sourceDeviceID    rw   The Device which has sent this Message
  *    payloadLength     rw   The length of Payload data
  *    payload           rw   The data in the form of Payload
  * Returns :None
  *
  *******************************************************************
  */

 void CGISDataRequestHandler::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
 {
      m_IBOXShadowControl->GISDataRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_RESULTACK);
 }

/*
*******************************************************************
* Function:CPOIDataAvailableListener
*
* Description :Constructor for CPOIDataAvailableListener.
*              Stores the context of CIBOXShadowControl
* Parameters : None
*
* Returns :None
*
*******************************************************************
*/

CPOIDataAvailableListener::CPOIDataAvailableListener(CIBOXShadowControl *context)
{
    m_IBOXShadowControl = context;
}

/*
*******************************************************************
* Function:OnChange
*
* Description : Method invoked by ACS Daemon when property POIDataAvailable
*               is changed by Ibox.
* Parameters : None
*
* Returns :None
*
*******************************************************************
*/

void CPOIDataAvailableListener::OnChange()
{
    m_IBOXShadowControl->POIDataAvailable();
}

/*
*******************************************************************
* Function:CPOFDataAvailableListener
*
* Description :Constructor for CPOFDataAvailableListener.
*              Stores the context of CIBOXShadowControl
* Parameters : None
*
* Returns :None
*
*******************************************************************
*/

CPOFDataAvailableListener::CPOFDataAvailableListener(CIBOXShadowControl *context)
{
    m_IBOXShadowControl = context;
}

/*
*******************************************************************
* Function:OnChange
*
* Description : Method invoked by ACS Daemon when property POFDataAvailable
*               is changed by Ibox.
* Parameters : None
*
* Returns :None
*
*******************************************************************
*/

void CPOFDataAvailableListener::OnChange()
{
    m_IBOXShadowControl->POFDataAvailable();
}
/*
*******************************************************************
* Function:CReceivedPOIListener
*
* Description :Constructor for CReceivedPOIListener.
*              Stores the context of CIBOXShadowControl
* Parameters : None
*
* Returns :None
*
*******************************************************************
*/
CReceivedPOIListener::CReceivedPOIListener(CIBOXShadowControl* context)
{
    m_IBOXShadowControl = context;
}
/*
*******************************************************************
* Function:OnChange
*
* Description : Method invoked by ACS Daemon when property ReceivedPOI
*               is changed by Ibox.
* Parameters : None
*
* Returns :None
*
*******************************************************************
*/
void CReceivedPOIListener::OnChange()
{
    m_IBOXShadowControl->ReceivedPOI();
}

WavBlock::WavBlock()
{
    nBlockType = 0;
    nBlockCounter = 0;
    nBlockSize = 0;
    memset(szWavBuffer, 0x00, sizeof(szWavBuffer));

}
WavBlockStructure::WavBlockStructure()
{
    nTotalBlockNumber = 0;
}

/*
*******************************************************************
* Function:CGISRequestHandler
*
* Description :Constructor for CGISRequestHandler.
*              Stores the context of CIBOXShadowControl
* Parameters : None
*
* Returns :None
*
*******************************************************************
*/

CPOIDataRequestHandler::CPOIDataRequestHandler(CIBOXShadowControl *context)
{
    m_IBOXShadowControl = context;
}
/** Callback methods for POIDataRequest handler */
void CPOIDataRequestHandler::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_IBOXShadowControl->POIDataRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
void CPOIDataRequestHandler::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_IBOXShadowControl->POIDataRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

void CPOIDataRequestHandler:: Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_IBOXShadowControl->POIDataRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

void CPOIDataRequestHandler::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_IBOXShadowControl->POIDataRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

void CPOIDataRequestHandler::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_IBOXShadowControl->POIDataRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}
/*
*******************************************************************
* Function:CVoiceSearchOnlineHandler
*
* Description :Constructor for CVoiceSearchOnlineHandler.
*              Stores the context of CIBOXShadowControl
* Parameters : None
*
* Returns :None
*
*******************************************************************
*/
CVoiceSearchOnlineHandler::CVoiceSearchOnlineHandler(CIBOXShadowControl* context)
{
    m_IBOXShadowControl = context;
}
/** Callback methods for VoicesearchOnline handler */
void CVoiceSearchOnlineHandler::ErrorAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_IBOXShadowControl->VoiceSearchOnlineHandler(sourceDeviceID,payloadLength,payload,STATUS_ERRORACK);
}

void CVoiceSearchOnlineHandler::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_IBOXShadowControl->VoiceSearchOnlineHandler(sourceDeviceID,payloadLength,payload,STATUS_PROCESSINGACK);
}

void CVoiceSearchOnlineHandler::Processing( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_IBOXShadowControl->VoiceSearchOnlineHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

void CVoiceSearchOnlineHandler::Result( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_IBOXShadowControl->VoiceSearchOnlineHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

void CVoiceSearchOnlineHandler::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_IBOXShadowControl->VoiceSearchOnlineHandler(sourceDeviceID,payloadLength,payload,STATUS_RESULTACK);
}

void CVoiceSearchOnlineHandler::Interface( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_IBOXShadowControl->VoiceSearchOnlineHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

void CVoiceSearchOnlineHandler::Error( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
    m_IBOXShadowControl->VoiceSearchOnlineHandler(sourceDeviceID,payloadLength,payload,STATUS_UNDEFINED);
}

/*
 *******************************************************************
 * Function:ProcessingAck
 *
 * Description :Callback for received ProcessingAck Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CPOIDataRequestHandler::ProcessingAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload )
{
   m_IBOXShadowControl->POIDataRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_PROCESSINGACK);

}

/*
 *******************************************************************
 * Function:ResultAck
 *
 * Description :Callback for received ResultAck Operation
 *
 * Parameters :
 *    sourceDeviceID    rw   The Device which has sent this Message
 *    payloadLength     rw   The length of Payload data
 *    payload           rw   The data in the form of Payload
 * Returns :None
 *
 *******************************************************************
 */

void CPOIDataRequestHandler::ResultAck( DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload ){

   m_IBOXShadowControl->POIDataRequestHandler(sourceDeviceID,payloadLength,payload,STATUS_RESULTACK);

}




CLSDataAvailableListener::CLSDataAvailableListener(CIBOXShadowControl* context)
{
    m_IBOXShadowControl = context;
}

void CLSDataAvailableListener::OnChange()
{
    m_IBOXShadowControl->LSDataAvailable(PROPERTY_STATUS);
}

void CLSDataAvailableListener::OnError()
{
    m_IBOXShadowControl->LSDataAvailable(PROPERTY_ERROR);
}

void CIBOXShadowControl::LSDataAvailable(const uchar optype)
{
//    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " LSDataAvailable API was called " << MOST_LOG_ENDL;

    QString uri;
    uchar available = 0xFF;

    if(optype == PROPERTY_STATUS)
    {
        const ::k2l::acs::protocols::most::string::IStringEncoding *pEncoding = NULL;
        uint8_t *pString = NULL;
        uint32_t stringLength = 0;
        m_LSDataAvailable->URIString(&pEncoding, &pString, &stringLength);
        uri = QString::fromUtf8((const char *)pString, stringLength);
        available = (uchar)m_LSDataAvailable->AvailableValue();
/*
        MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " LSDataAvailable:URI = " <<uri.toAscii().data() << MOST_LOG_ENDL;
        MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " LSDataAvailable:Available = " << available << MOST_LOG_ENDL;
*/
// 2014.08.01, for log, hyunyong.sung
	MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " LSDataAvailable_Status <-- : uri = " 
	<< uri.toAscii().data() << " available = " << available <<  MOST_LOG_ENDL;

        emit LSDataAvailable(optype, uri, available);
    }
    else if(optype == PROPERTY_ERROR)
    {
//        MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " LSDataAvailable Error got received " << MOST_LOG_ENDL;

        uint8_t errorCode = 0;
        uint32_t errorInfoLength = 0;
        const uint8_t* errorInfoPayload = NULL;
        if (k2l::acs::samples::CMOSTErrorInfo::GetMOSTError(
                m_IBoxShadow->Container(),
                errorCode,
                errorInfoLength,
                errorInfoPayload
                ))
        {
            //Retrieved error information correctly.
            uint errorInfo = 0;
            for(int i=0; i<errorInfoLength; i++)
            {
                errorInfo |= errorInfoPayload[i] << (errorInfoLength-i-1)*8;
            }
/*
            MOST_LOG_CRITICAL << LOG_TAG_IBOX_CNT << " Error code : " << errorCode << MOST_LOG_ENDL;
            MOST_LOG_CRITICAL << LOG_TAG_IBOX_CNT << " Error Info Length : " << errorInfoLength << MOST_LOG_ENDL;
            MOST_LOG_CRITICAL << LOG_TAG_IBOX_CNT << " Error Info Payload : " << errorInfo << MOST_LOG_ENDL;
*/
// 2014.08.01, for log, hyunyong.sung
		MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " LSDataAvailable_Error <-- : errorcode = " 
		<< errorCode << " errorInfo = " << errorInfo <<  MOST_LOG_ENDL;

            emit LSDataAvailableError(errorCode, errorInfo);
        }
    }
    else
    {
//        MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " LSDataAvailable:Undefined OPTYPE " << MOST_LOG_ENDL;
        return;
    }
}

CLSRequestHandler::CLSRequestHandler(CIBOXShadowControl* context)
{
    m_IBOXShadowControl = context;
}

void CLSRequestHandler::ErrorAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload)
{
    m_IBOXShadowControl->LSRequestHandler(sourceDeviceID, payloadLength, payload, METHOD_ERRORACK);
}

void CLSRequestHandler::ProcessingAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload)
{
    m_IBOXShadowControl->LSRequestHandler(sourceDeviceID, payloadLength, payload, METHOD_PROCESSINGACK);
}

void CLSRequestHandler::ResultAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload)
{
    m_IBOXShadowControl->LSRequestHandler(sourceDeviceID, payloadLength, payload, METHOD_RESULTACK);
}

void CIBOXShadowControl::LSRequestHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uchar optype)
{
    ushort senderHandle;
    uchar success = 0xFF;

//    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " LSRequestHandler API was called from IBOX" << MOST_LOG_ENDL;

    switch(optype)
    {
        case METHOD_ERRORACK:
//            MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " LSRequestHandler:OPTYPE = " << optype << MOST_LOG_ENDL;
            m_LSRequest->ParseSenderHandle(payloadLength, payload, &senderHandle);
            {
                uint8_t errorCode = 0;
                uint32_t errorInfoLength = 0;
                const uint8_t* errorInfoPayload = NULL;
                if (k2l::acs::samples::CMOSTErrorInfo::GetMOSTError(
                        m_IBoxShadow->Container(),
                        errorCode,
                        errorInfoLength,
                        errorInfoPayload
                        ))
                {
/*                
                    //Retrieved error information correctly.
                    MOST_LOG_CRITICAL << LOG_TAG_IBOX_CNT << " Error code " << errorCode << MOST_LOG_ENDL;
                    MOST_LOG_CRITICAL << LOG_TAG_IBOX_CNT << " Error Info Length " << errorInfoLength << MOST_LOG_ENDL;
                    MOST_LOG_CRITICAL << LOG_TAG_IBOX_CNT << " Error Info Payload " << errorInfoPayload << MOST_LOG_ENDL;
*/
// 2014.08.01, for log, hyunyong.sung
			MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " LSRequest_ErrorAck <-- : errorcode = " 
			<< errorCode << " errorInfo = " << errorInfoPayload <<  MOST_LOG_ENDL;
                }
            }
            break;
        case METHOD_PROCESSINGACK:
//            MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " LSRequestHandler:OPTYPE = " << optype << MOST_LOG_ENDL;
            m_LSRequest->ParseSenderHandle(payloadLength, payload, &senderHandle);
            break;
        case METHOD_RESULTACK:
            m_LSRequest->ParseResultAck(payloadLength, payload);
            senderHandle = m_LSRequest->SenderHandleValue();
            success = (uchar)m_LSRequest->SuccessValue();
/*
            MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " LSRequestHandler:OPTYPE = " << optype << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "LSRequestHandler:SenderHandle = " << senderHandle  << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "LSRequestHandler:Success = " << success << MOST_LOG_ENDL;
*/
// 2014.08.01, for log, hyunyong.sung
		MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " LSRequest_ResultAck <-- : senderhandle = " 
		<< senderHandle << " Success = " << success <<  MOST_LOG_ENDL;
            break;
        default:
//            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "Undefined Operation of LSRequestHandler :  " << optype << MOST_LOG_ENDL;
            return;
    }

    emit LSRequest(optype, senderHandle, success);
}

CLSDataRequestHandler::CLSDataRequestHandler(CIBOXShadowControl* context)
{
    m_IBOXShadowControl = context;
}

void CLSDataRequestHandler::ErrorAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload)
{
    m_IBOXShadowControl->LSDataRequestHandler(sourceDeviceID, payloadLength, payload, METHOD_ERRORACK);
}

void CLSDataRequestHandler::ProcessingAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload)
{
    m_IBOXShadowControl->LSDataRequestHandler(sourceDeviceID, payloadLength, payload, METHOD_PROCESSINGACK);
}

void CLSDataRequestHandler::ResultAck(DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t * payload)
{
    m_IBOXShadowControl->LSDataRequestHandler(sourceDeviceID, payloadLength, payload, METHOD_RESULTACK);
}

void CIBOXShadowControl::LSDataRequestHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload, const uchar optype)
{
    ushort senderHandle;
    uchar target = 0xFF;
    QString uri;
    QByteArray lsdata;

//    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " LSDataRequestHandler API was called from IBOX" << MOST_LOG_ENDL;

    switch(optype)
    {
        case METHOD_ERRORACK:
//            MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " LSDataRequestHandler:OPTYPE = " << optype << MOST_LOG_ENDL;
            m_LSDataRequest->ParseSenderHandle(payloadLength, payload, &senderHandle);
            {
                uint8_t errorCode = 0;
                uint32_t errorInfoLength = 0;
                const uint8_t* errorInfoPayload = NULL;
                if (k2l::acs::samples::CMOSTErrorInfo::GetMOSTError(
                        m_IBoxShadow->Container(),
                        errorCode,
                        errorInfoLength,
                        errorInfoPayload
                        ))
                {
/*                
                    //Retrieved error information correctly.
                    MOST_LOG_CRITICAL << LOG_TAG_IBOX_CNT << " Error code " << errorCode << MOST_LOG_ENDL;
                    MOST_LOG_CRITICAL << LOG_TAG_IBOX_CNT << " Error Info Length " << errorInfoLength << MOST_LOG_ENDL;
                    MOST_LOG_CRITICAL << LOG_TAG_IBOX_CNT << " Error Info Payload " << errorInfoPayload << MOST_LOG_ENDL;
*/
// 2014.08.01, for log, hyunyong.sung
			MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " LSDataRequest_ErrorAck <-- : errorcode = " 
			<< errorCode << " errorinfo = " << errorInfoPayload <<  MOST_LOG_ENDL;
                }
            }
            break;

        case METHOD_PROCESSINGACK:
//            MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " LSDataRequestHandler:OPTYPE = " << optype << MOST_LOG_ENDL;
            m_LSDataRequest->ParseSenderHandle(payloadLength, payload, &senderHandle);
            break;

        case METHOD_RESULTACK:
            MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " LSDataRequestHandler:OPTYPE = " << optype << MOST_LOG_ENDL;
            m_LSDataRequest->ParseResultAck(payloadLength,payload);
            senderHandle = m_LSDataRequest->SenderHandleValue();
            target = (uchar)m_LSDataRequest->TargetValue();
            {
                const ::k2l::acs::protocols::most::string::IStringEncoding *pEncoding = NULL;
                uint8_t *pString = NULL;
                uint32_t stringLength = 0;
                m_LSDataRequest->URIString(&pEncoding, &pString, &stringLength);
                uri = QString::fromUtf8((const char *)pString, stringLength);
                lsdata.append((const char *)m_LSDataRequest->LSDataValue(), m_LSDataRequest->LSDataStreamLength());
/*				
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "LSDataRequestHandler::SenderHandle : " << senderHandle << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "LSDataRequestHandler::target : " << target << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "LSDataRequestHandler::uri : " << uri.toAscii().data() << MOST_LOG_ENDL;
                MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "LSDataRequestHandler::lsdata : " << lsdata.data() << MOST_LOG_ENDL;
*/
// 2014.08.01, for log, hyunyong.sung
		MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " LSDataRequest_ResultAck <-- : SenderHandle = " 
		<< senderHandle << " target = " << target << " uri = " << uri.toAscii().data() << " lsdate = " 
		<<  lsdata.data() <<  MOST_LOG_ENDL;
            }
            break;

        default:
//            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "Undefined Operation of LSDataRequestHandler :  " << optype << MOST_LOG_ENDL;
            return;
    }

    emit LSDataRequest(optype, senderHandle, target, uri, lsdata);
}


/** Methods invoked by respective Handler */
void CIBOXShadowControl::OpenBrowserHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state)
{
    switch(state)
    {
        case STATUS_PROCESSINGACK:
            OpenBrowserRequestResult(IBOX_PROCESSING_ACK,payloadLength,payload);
            break;

        case STATUS_RESULTACK:
            OpenBrowserRequestResult(IBOX_RESULT_ACK,payloadLength,payload);
            break;

        case STATUS_UNDEFINED:
            break;

        default:
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "Source Device ID :: " << sourceDeviceID << MOST_LOG_ENDL;
            break;
    }
}

void CIBOXShadowControl::GISRequestHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state)
{
    switch(state)
    {
        case STATUS_PROCESSINGACK:
            GISRequestResult(IBOX_PROCESSING_ACK,payloadLength,payload);
            break;

        case STATUS_RESULTACK:
            GISRequestResult(IBOX_RESULT_ACK,payloadLength,payload);
            break;

        case STATUS_UNDEFINED:
            break;

        default:
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "Source Device ID :: " << sourceDeviceID << MOST_LOG_ENDL;
            break;
    }
}
void CIBOXShadowControl::GISDataRequestHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state)
{
    switch(state)
    {
        case STATUS_PROCESSINGACK:
            GISDataRequestResult(IBOX_PROCESSING_ACK,payloadLength,payload);
            break;

        case STATUS_RESULTACK:
            GISDataRequestResult(IBOX_RESULT_ACK,payloadLength,payload);
            break;

        case STATUS_UNDEFINED:
            break;

        default:
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "Source Device ID :: " << sourceDeviceID << MOST_LOG_ENDL;
            break;
    }
}

void CIBOXShadowControl::POIDataRequestHandler(const DeviceID_t sourceDeviceID, const uint32_t payloadLength, const uint8_t *payload,const uint state)
{
    switch(state)
    {
        case STATUS_PROCESSINGACK:
            POIDataRequestResult(IBOX_PROCESSING_ACK,payloadLength,payload);
            break;

        case STATUS_RESULTACK:
            POIDataRequestResult(IBOX_RESULT_ACK,payloadLength,payload);
            break;

        case STATUS_UNDEFINED:
            break;

        default:
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "Source Device ID :: " << sourceDeviceID << MOST_LOG_ENDL;
            break;
    }
}

bool CIBOXShadowControl::SendLSRequest(const uchar optype, const ushort senderHandle, const QString uri, const QByteArray lsdata)
{    
    bool result = false;

//    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " onLSRequest API got called to send to IBOX" << MOST_LOG_ENDL;

    switch(optype)
    {
        case METHOD_STARTRESULTACK:
/*
		MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " onLSRequest:OPTYPE = " << optype << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " onLSRequest:SenderHandle = " << senderHandle << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " onLSRequest:URI = " << uri.toAscii().data() << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " onLSRequest:LSData(Byte Array as Hex) = " << lsdata.toHex().data() << MOST_LOG_ENDL;
*/
// 2014.08.01, for log, hyunyong.sung
		MOST_LOG_HIGH << LOG_TAG_IBOX_CNT << " LSRequest_StartResultAck --> : SenderHandle = " 
		<< senderHandle << " URI = " << uri.toAscii().data() << " LSdate = " 
		<< lsdata.toHex().data() << MOST_LOG_ENDL;

            m_LSRequest->SenderHandleValue(senderHandle);
            m_LSRequest->URIString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                   reinterpret_cast<const uint8_t*>(uri.toUtf8().data()),
                                   uri.length()+1);
            m_LSRequest->LSDataValue((uint8_t*)(lsdata.data()), lsdata.length());
            m_LSRequest->StartResultAck();
            result = true;
            break;
        case METHOD_ABORTACK:
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " onLSRequest:OPTYPE = " << optype << MOST_LOG_ENDL;
            //m_LSRequest->SenderHandleValue(senderHandle);
            m_LSRequest->AbortAck(senderHandle);
            result = true;
            break;
        default:
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "Undefined Operation of onLSRequest = " << optype << MOST_LOG_ENDL;
            break;
    }
    return result;
}

bool CIBOXShadowControl::SendLSDataRequest(const uchar optype, const ushort senderHandle, const uchar target, const QString uri)
{
//    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " onLSDataRequest API got called to send to IBOX " << MOST_LOG_ENDL;
    bool result = false;

    switch(optype)
    {
        case METHOD_STARTRESULTACK:
/*			
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " onLSDataRequest:OPTYPE = " << optype << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " onLSDataRequest:SenderHandle = " << senderHandle << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " onLSDataRequest:Target = " << (uchar)target << MOST_LOG_ENDL;
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " onLSDataRequest:URI = " << uri.toAscii().data() << MOST_LOG_ENDL;
*/
// 2014.08.01, for log, hyunyong.sung
		MOST_LOG_HIGH << LOG_TAG_IBOX_CNT << " LSDataRequest_StartResultAck --> : SenderHandle = " 
		<< senderHandle << " Target = " << (uchar)target << " URI = " << uri.toAscii().data() << MOST_LOG_ENDL;

            m_LSDataRequest->SenderHandleValue(senderHandle);
            m_LSDataRequest->TargetValue((CLSDataRequest::TTarget)target);
            m_LSDataRequest->URIString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                   reinterpret_cast<const uint8_t*>(uri.toUtf8().data()),
                                   uri.length()+1);
            m_LSDataRequest->StartResultAck();
            result = true;
            break;
        case METHOD_ABORTACK:
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << " onLSDataRequest:OPTYPE = " << optype << MOST_LOG_ENDL;
            //m_LSDataRequest->SenderHandleValue(senderHandle);
            m_LSDataRequest->AbortAck(senderHandle);
            result = true;
            break;
        default:
            MOST_LOG_INFO << LOG_TAG_IBOX_CNT << "Undefined Operation of onLSDataRequest = " << optype << MOST_LOG_ENDL;
            break;
    }
    return result;
}

/*
 *******************************************************************
 * Function :   CACNActivatedListener
 *
 * Description :Constructor for CACNActivatedListener.
 *              Stores the context of CIBOXShadowControl
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
CACNActivatedListener::CACNActivatedListener(CIBOXShadowControl *context)
{
    m_IBOXShadowControl = context;
}

/*
 *******************************************************************
 * Function:OnChange
 *
 * Description : Method invoked by ACS Daemon when property ACNActivated
 *               is changed by Ibox.
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CACNActivatedListener::OnChange()
{
    m_IBOXShadowControl->ACNActivated();
}

/*
 *******************************************************************
 * Function :   ACNActivated
 *
 * Description : Invoked by the Listener for ACNActivated of Ibox
 *               Sends the signal to IBOX Adaptor with required data
 * Parameters : None
 *
 * Returns :None
 *
 *******************************************************************
 */
void CIBOXShadowControl::ACNActivated()
{
/*
    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " ACNActivated Signal got received " << MOST_LOG_ENDL;
    MOST_LOG_HIGH << LOG_TAG_IBOX_CNT << " ACNActivated:  = " << (bool)m_ACNActivated->Value() << MOST_LOG_ENDL;
*/
// 2014.08.01, for log, hyunyong.sung
    MOST_LOG_SIGNAL << LOG_TAG_IBOX_CNT << " ACNActivated_Status <-- : ACNActivated = " 
    << (bool)m_ACNActivated->Value() << MOST_LOG_ENDL;

    emit ACNActivated(m_ACNActivated->Value());
}
