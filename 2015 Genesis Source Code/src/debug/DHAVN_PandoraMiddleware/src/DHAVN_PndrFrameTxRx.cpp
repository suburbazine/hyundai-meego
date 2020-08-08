/*
 ******************************************************************
 *        COPYRIGHT © 2011, LG Electronics.  
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :   BT/USB Frame Tx Rx                                  *
 * PROGRAMMER :   Satyanarayana B                                 *
 * DATE       :   07/12/2011                                      *
 * VERSION    :   0.1                                             *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module will connect to active device. Send and receive data 
 *  over active connection
 *
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *  07/12/2011  0.1 Satyanarayana B  Cpp file created
 *  19/1/2012   0.2 Snigdha Satapathy Modified implementation     *
 ******************************************************************
 */
#include <QDataStream>
#include <QDebug>
#include "DHAVN_PndrFrameTxRx.h" 
#include "DHAVN_PndrBTFrameTxRx.h"
#include "DHAVN_PndrUSBFrameTxRx.h"
#include "DHAVN_Pndr_Defs.h"
#include "DHAVN_PndrLinkConstants.h"
#include "DHAVN_PndrFrameEncoder.h"



//Static variables initialization
//int CPndrFrameTxRx::m_nInstanceCounter = 0;
//CPndrFrameTxRx* CPndrFrameTxRx::m_pInstance = 0;

/**
 *******************************************************************
 * Constructor is a private member function to make this class as singleton
 *
 * @param   None
 *
 * @return  None
 *
 *******************************************************************
 */
CPndrFrameTxRx::CPndrFrameTxRx()
{
    LOG_INIT_V2("Pandora");
    LOG_TRACE<< "CPndrFrameTxRx::CPndrFrameTxRx():Entry" <<LOG_ENDL;
    m_pUsbTxRx = NULL;
    m_pBtTxRx = NULL;
    m_pActiveTxRx = NULL;

#ifdef DHAVN_PANDORA_TESTFRAMEWORK
    //connect to the timeout signal of Timer
    m_Timer.setInterval(200);
    connect(&m_Timer,SIGNAL(timeout()),this,SLOT(OnTimeout()));
#endif //DHAVN_PANDORA_TESTFRAMEWORK

    LOG_TRACE<< "CPndrFrameTxRx::CPndrFrameTxRx():Exit" <<LOG_ENDL;
}

/**
 *******************************************************************
 * Initializes the transport layers
 * This is to be called by DeviceManager/state handler after calling
 * GetInstance of CPndrFrameTxtRx
 *
 * @param   None
 *
 * @return  EPndrError
 *
 *******************************************************************
 */
EPndrError CPndrFrameTxRx::InitDevice(const EDeviceType inDeviceType)
{
    LOG_TRACE<< "CPndrFrameTxRx::Init:Entry" <<LOG_ENDL;

     EPndrError ret = E_INIT_FAILED;

     switch(inDeviceType)
     {
         case DEVICE_TYPE_SMARTPHONE:
         {
             //Create BtTxRx object if not already existing

             if(m_pBtTxRx != NULL)
             {
                 ret = E_SUCCESS;
                 //LOG_CRITICAL<< "CPndrFrameTxRx::Init:failed since m_pBtTxRx is NOT NULL before Init" <<LOG_ENDL;
                 break;

             }

            m_pBtTxRx = new CPndrBTFrameTxRx();
            if(m_pBtTxRx == NULL)
            {
                LOG_CRITICAL<< "CPndrFrameTxRx::Init:failed since m_pBtTxRx allocation failed." <<LOG_ENDL;
                break;
            }

             //BT Phone is connected. So IniT BT.
             LOG_TRACE<< "CPndrFrameTxRx::BT Init" <<LOG_ENDL;

             ret = m_pBtTxRx->Init();

             LOG_INFO<< "CPndrFrameTxRx::BT Init returns "<< ret <<LOG_ENDL;

             if(E_SUCCESS == ret)
             {
                 connect(m_pBtTxRx,SIGNAL(BTDataReceived(const QByteArray&)),this,SLOT(EmitDataReceivedSignal(const QByteArray&)));
                 connect(m_pBtTxRx,SIGNAL(BTDeviceStatusUpdated(const EDeviceType,bool)),this,SLOT(EmitDeviceState(const EDeviceType,bool)));

             }
             break;
         }
         case DEVICE_TYPE_IPHONE_USB_FRONT:
         case DEVICE_TYPE_IPHONE_USB_REAR:
         {
             //Create USBTxRx object if not already existing

             if(m_pUsbTxRx != NULL)
             {
                 ret = E_SUCCESS;
                 //LOG_CRITICAL<< "CPndrFrameTxRx::Init:failed since m_pUsbTxRx is NOT NULL before Init" <<LOG_ENDL;
                 break;
             }

            m_pUsbTxRx =  new CPndrUSBFrameTxRx();
            if(m_pUsbTxRx == NULL)
            {
                LOG_CRITICAL<< "CPndrFrameTxRx::Init:failed since m_pUsbTxRx allocation failed." <<LOG_ENDL;
                break;
            }

            //USB is connected. So Init IPOd.
            LOG_TRACE<< "CPndrFrameTxRx::USB Init" <<LOG_ENDL;

            ret = m_pUsbTxRx->Init();

            LOG_INFO<< "CPndrFrameTxRx::USB Init returns "<< ret <<LOG_ENDL;
            if(E_SUCCESS == ret)
            {
                connect(m_pUsbTxRx,SIGNAL(USBDataReceived(const QByteArray&)),this,SLOT(EmitDataReceivedSignal(const QByteArray&)));
                connect(m_pUsbTxRx,SIGNAL(USBDeviceStatusUpdated(const EDeviceType,bool)),this,SIGNAL(DeviceStatusUpdated(const EDeviceType,bool)));

                connect(m_pUsbTxRx,SIGNAL(ApplicationLaunched(EPndrError)),this,SIGNAL(ApplicationLaunched(EPndrError)));
                connect(m_pUsbTxRx, SIGNAL(PndrSessionClosed()), this, SIGNAL(PndrSessionClosed())); //added by esjang 2013.06.27 for ISV #71393, ITS #167008
                connect(m_pUsbTxRx, SIGNAL(AudioSamplingChanged(int)), this, SIGNAL(AudioSamplingChanged(int)));

            }

             break;
         }
         default:
         {
             // This will be set for DEVICE_TYPE_NONE when SetTransport deinitializes
             // since No Init is needed in this case
             ret = E_SUCCESS;
            break;
         }
     }

     LOG_TRACE<< "CPndrFrameTxRx::Init:Exit with return " << ret <<LOG_ENDL;

     return ret;
}

/**
 *******************************************************************
 *  DeInit Function for releasing memory
 *
 *
 * @param      None
 *
 * @return     None
 *
 *******************************************************************
 */

void CPndrFrameTxRx::DeInit()
{
    LOG_TRACE << "In CPndrFrameTxRx::DeInit():Entry" << LOG_ENDL;

    if(m_pActiveTxRx)
    {
       // m_pActiveTxRx->Disconnect();
        m_pActiveTxRx = NULL;
    }

    if(m_pUsbTxRx)
    {
        LOG_TRACE << "In CPndrFrameTxRx::DeInit():m_pUsbTxRx" << LOG_ENDL;      
        delete m_pUsbTxRx;
        m_pUsbTxRx = NULL;
    }

    if(m_pBtTxRx)
    {
        LOG_TRACE << "In CPndrFrameTxRx::DeInit():m_pBtTxRx" << LOG_ENDL;
        
        delete m_pBtTxRx;
        m_pBtTxRx = NULL;
    }


#ifdef DHAVN_PANDORA_TESTFRAMEWORK
    if( m_Timer.isActive())
    {
        m_Timer.stop();
    }
#endif //DHAVN_PANDORA_TESTFRAMEWORK
    LOG_TRACE << "In CPndrFrameTxRx::DeInit():Exit" << LOG_ENDL;

}

/**
 *******************************************************************
 * Destructor
 *
 * @param   None
 *
 * @return  Instance of CPndrFrameTxRx
 *
 *******************************************************************
 */
CPndrFrameTxRx::~CPndrFrameTxRx()
{
    LOG_TRACE<< "CPndrFrameTxRx::~CPndrFrameTxRx:Entry" <<LOG_ENDL;
    DeInit();
    LOG_TRACE<< "CPndrFrameTxRx::~CPndrFrameTxRx:Exit" <<LOG_ENDL;

}



void CPndrFrameTxRx::ReleaseConnection()
{

    if(m_pUsbTxRx)
    {
        disconnect(m_pUsbTxRx,0,this,0);
        m_pUsbTxRx->ReleaseConnection();
    }
}

/**
 *******************************************************************
 * Get the instance of this class
 *
 * @param   None
 *
 * @return  Instance of CPndrFrameTxRx
 *
 *******************************************************************
 */
//CPndrFrameTxRx* CPndrFrameTxRx::GetInstance()
//{
//    USE_LOG_UTIL;
//    LOG_INIT_V2("Pandora");
//    LOG_TRACE<< "CPndrFrameTxRx::GetInstance:Entry" <<LOG_ENDL;

//    if(m_pInstance == NULL)
//    {
//        m_pInstance = new CPndrFrameTxRx();
//    }

//    //The counter is incremented only if m_pInstance allocation is succesfull.
//    if(m_pInstance)
//    {
//        m_nInstanceCounter++;
//        LOG_TRACE<< "CPndrFrameTxRx::m_nInstanceCounter = " << m_nInstanceCounter<<LOG_ENDL;

//    }
//    else
//    {
//        LOG_INFO << "CPndrFrameTxRx GetInstance Failed." << LOG_ENDL;
//    }
//    LOG_TRACE<< "CPndrFrameTxRx::GetInstance:Exit" <<LOG_ENDL;

//    return m_pInstance;
//}


/**
 *******************************************************************
 * Releases instance of CPndrFrameTxRx
 *
 * @param    None
 *
 * @return     None
 *
 *******************************************************************
 */
//void CPndrFrameTxRx::ReleaseInstance()
//{
//    USE_LOG_UTIL;
//    LOG_INIT_V2("Pandora");
//    LOG_TRACE<< "CPndrFrameTxRx::ReleaseInstance:Entry" <<LOG_ENDL;

//    LOG_TRACE<< "FrameTxRx Instance to be deleted is m_nInstanceCounter = " << m_nInstanceCounter <<LOG_ENDL;

//    m_nInstanceCounter--;
//    if (!m_nInstanceCounter)
//    {
//        LOG_TRACE<< "CPndrFrameTxRx::deleted" <<LOG_ENDL;
//        DeInit();
//        delete m_pInstance;
//        m_pInstance = NULL;
//    }
//    LOG_TRACE<< "CPndrFrameTxRx::ReleaseInstance:Exit" <<LOG_ENDL;

//}

/**
 *******************************************************************
 * Set and connect currently selected transport. If already
 * connected then return the instance else try to connect
 *
 * Called by DeviceManager while trying to connect
 * @param[in]   inDeviceType: The device to be used for sending and
 *                              receiving information. E_DEVICE_NONE
 *                              if device is disconnected.
 *
 * @return     None
 *
 *******************************************************************
 */
EPndrError CPndrFrameTxRx::SetTransport(const EDeviceType inDeviceType)
{
    LOG_TRACE<< "CPndrFrameTxRx::SetTransport:Entry" <<LOG_ENDL;

    EPndrError err = E_SUCCESS;
#ifndef DHAVN_PANDORA_TESTFRAMEWORK
    bool ret = false;

    err = InitDevice(inDeviceType);

    if(E_SUCCESS == err)
    {
        if(DEVICE_TYPE_SMARTPHONE == inDeviceType)
        {
            ret = m_pBtTxRx->Connect();
            if(ret)
            {
                m_pActiveTxRx = m_pBtTxRx;
            }
            else
            {
                err = E_DEVICE_NOT_FOUND;
            }

        }
        else if( DEVICE_TYPE_IPHONE_USB_FRONT  == inDeviceType
                || DEVICE_TYPE_IPHONE_USB_REAR == inDeviceType)
        {
            ret = m_pUsbTxRx->Connect(inDeviceType);
            if(ret)
            {
                m_pActiveTxRx = m_pUsbTxRx;
            }
            else
            {
                err = E_DEVICE_NOT_FOUND;
            }
        }
        else
        {
            //No Active Device
            LOG_TRACE<< "CPndrFrameTxRx::SetTransport:No Active Device" <<LOG_ENDL;

            if(m_pActiveTxRx)
            {
                LOG_TRACE<< "CPndrFrameTxRx::Disconnect Active Device" <<LOG_ENDL;
                m_pActiveTxRx->Disconnect();
                m_pActiveTxRx = NULL;

            }
        }
    }
#endif //DHAVN_PANDORA_BT_ENABLED

#ifdef DHAVN_PANDORA_TESTFRAMEWORK
    EmitDeviceState(inDeviceType,true);
#endif //DHAVN_PANDORA_TESTFRAMEWORK

    LOG_TRACE<< "CPndrFrameTxRx::SetTransport:Exit" <<LOG_ENDL;

    return err;
}

/**
 *******************************************************************
 * Send the data over currently active transport
 *
 * @param[in]   inData: The data to be sent
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrFrameTxRx::Send(const QByteArray& inData)
{
   // LOG_TRACE<< "CPndrFrameTxRx::Send:Entry" <<LOG_ENDL;

#ifdef DHAVN_PANDORA_TESTFRAMEWORK
    // push inData in queue
    reqListArray.append(inData);
    if(!m_Timer.isActive())
    {
        m_Timer.start();
    }


#else
    if(m_pActiveTxRx)
    {
        //LOG_TRACE<< "CPndrFrameTxRx::m_pActiveTxRx Send" <<LOG_ENDL;
        m_pActiveTxRx->Send(inData);
    }
#endif
   // LOG_TRACE<< "CPndrFrameTxRx::Send:Exit" <<LOG_ENDL;

}


/**
 *******************************************************************
 * Disconnect currently active device. This will be called by
 * Frame Validator as part of request from Application OR as
 * part of timeout for any eack expected OR as part of state handler
 * request in absence of an expected reply within given time slot.
 *
 * @param      None
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrFrameTxRx::Disconnect()
{
   // LOG_TRACE<< "CPndrFrameTxRx::Disconnect:Entry" <<LOG_ENDL;
    m_pActiveTxRx->Disconnect();
    //LOG_TRACE<< "CPndrFrameTxRx::Disconnect:Exit" <<LOG_ENDL;

}

/**
 *******************************************************************
 * Function for returning the max payload length supported by the
 * Active transport
 *
 * @param[in]   None
 *
 * @return     quint32
 *
 *******************************************************************
 */
quint32 CPndrFrameTxRx::GetMaxPayloadLength()
{
    //LOG_TRACE<< "Inside CPndrFrameTxRx::GetMaxPayloadLength" <<LOG_ENDL;
    return m_pActiveTxRx->GetMaxPayloadLength();
}

/**
 *******************************************************************
 * Function for emitting Signal for informing that data is received
 *
 * @param[in]   inData: The data to be sent
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrFrameTxRx::EmitDataReceivedSignal(const QByteArray& inData)
{
    //qDebug() << "emitted frame is" << inData.toHex();
    //LOG_SIGNAL<< "CPndrFrameTxRx::EmitDataReceivedSignal" <<LOG_ENDL;
    emit DataReceived(inData);
}

/**
 *******************************************************************
 * Function for emitting Signal for device status
 *
 * @param[in]   inDeviceType: The device type for which status
 *                              needs to be emitted
 * @param[in]   isConnected: The device is connected or disconnected
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrFrameTxRx::EmitDeviceState(const EDeviceType inDeviceType, bool isConnected)
{
    //LOG_SIGNAL<< "CPndrFrameTxRx::EmitDeviceState" <<LOG_ENDL;
    emit DeviceStatusUpdated(inDeviceType, isConnected);
}



/**
  * API to be used to launch an application
  * on an iOS device.
  *
  * parameter - QString is the application bundle identifier.
  * example for Pandora is com.pandora
  *
  */
void CPndrFrameTxRx::LaunchApplication(QString appBundleID)
{
    //LOG_TRACE<< "CPndrFrameTxRx::LaunchApplication:Entry" <<LOG_ENDL;
    if(m_pActiveTxRx == m_pUsbTxRx)
    {
        if(!appBundleID.isNull() && !appBundleID.isEmpty())
        {
            m_pUsbTxRx->LaunchApplication(appBundleID);
        }
        else
        {
            emit DeviceStatusUpdated(DEVICE_TYPE_IPHONE_USB_FRONT, false);
        }
    }
   // LOG_TRACE<< "CPndrFrameTxRx::LaunchApplication:Exit" <<LOG_ENDL;

}


#ifdef DHAVN_PANDORA_TESTFRAMEWORK
/**
 *******************************************************************
 * OnTimeout
 *
 * @param      None
 *
 * @return     None
 *
 *******************************************************************
 */
void CPndrFrameTxRx::OnTimeout()
{
    //qDebug() << "reqListArray.count()" << reqListArray.count();
    if(reqListArray.count())
    {

        //emit ACK
        QByteArray ackFrame;
        ackFrame[0]=  PNDR_FRAME_START;
        ackFrame[1]=  PNDR_FRAME_TYPE_ACK;
        ackFrame[2]=  (int)reqListArray.first().at(2) ^1;
        ackFrame[3]= 0x00;
        ackFrame[4]= 0x00;
        ackFrame[5]= 0x00;
        ackFrame[6]= 0x00;

        QByteArray crcData;
        QDataStream crcDs(&crcData,QIODevice::WriteOnly);
        crcDs.setByteOrder(QDataStream::BigEndian);
        //Calculate the CRC for the encodedFrame excluding the first byte(start byte)
        QByteArray crcByteArray = ackFrame.right(ackFrame.size() - 1);
        quint16 crc = CPndrFrameEncoder::CalculateCRC(crcByteArray,
                                   crcByteArray.size());
        crcDs << crc;
        ackFrame.append(crcData);
        ackFrame.append(PNDR_FRAME_END);
       /* QByteArray junk;
        junk[0] = 0x4a;
        junk[1] = 0x00;
        junk[2] = 0x00;
        junk[3] = 0x00;
        junk[4] = 0x00;
        junk[5] = 0x00;
        ackFrame.append(junk);

        qDebug() << " ackFrame " << ackFrame.toHex()<< "crc" << crc;
        EmitDataReceivedSignal(ackFrame);
        EmitDataReceivedSignal(ackFrame);*/
        EmitDataReceivedSignal(ackFrame);

       /* QByteArray dup (reqListArray.first());
        reqListArray.first().append(dup);
        reqListArray.first().append(dup);
        reqListArray.first().append(junk);*/
        EmitDataReceivedSignal(reqListArray.first());
        reqListArray.removeFirst();


/*
        //case1 : ACK + /Part Data Frame
        QByteArray ackFrame;
        ackFrame[0]=  PNDR_FRAME_START;
        ackFrame[1]=  PNDR_FRAME_TYPE_ACK;
        ackFrame[2]=  (int)reqListArray.first().at(2) ^1;
        ackFrame[3]= 0x00;
        ackFrame[4]= 0x00;
        ackFrame[5]= 0x00;
        ackFrame[6]= 0x00;
        QByteArray crcData;
        QDataStream crcDs(&crcData,QIODevice::WriteOnly);
        crcDs.setByteOrder(QDataStream::BigEndian);
        //Calculate the CRC for the encodedFrame excluding the first byte(start byte)
        QByteArray crcByteArray = ackFrame.right(ackFrame.size() - 1);
        quint16 crc = CPndrFrameEncoder::CalculateCRC(crcByteArray,
                                   crcByteArray.size());
        crcDs << crc;
        ackFrame.append(crcData);
        ackFrame.append(PNDR_FRAME_END);
        */
/*
        ackFrame.append(reqListArray.first().mid(0,3));
        EmitDataReceivedSignal(ackFrame);

        EmitDataReceivedSignal(reqListArray.first().right(reqListArray.first().size()-3));
        reqListArray.removeFirst();
        */

        /*
        EmitDataReceivedSignal(ackFrame);
        EmitDataReceivedSignal(reqListArray.first().mid(0,3));

        EmitDataReceivedSignal(reqListArray.first().right(reqListArray.first().size()-3));
*/


     }

}
#endif //DHAVN_PANDORA_TESTFRAMEWORK



void CPndrFrameTxRx::LogCommandID(QByteArray dataArray ,bool isbt)
{
     bool ok;
     bool isACK = false;     
     QString logString="";
     QString inData="";
     int cmdid;
     
     inData = dataArray.toHex().mid(14, 2);
     cmdid = inData.toInt(&ok,16);
 
    if(ok == FALSE)
    {
        LOG_CRITICAL<< " CPndrFrameTxRx::LogCommandID : NOT OK.." <<LOG_ENDL;
        return ;
    }
    
    switch (cmdid)
    {
        case    0x00    :   logString = "   PNDR_SESSION_START  to mobile"   ; break;
        case    0x01    :   logString = "   PNDR_UPDATE_BRANDING_IMAGE   to mobile"   ; break;
        case    0x02    :   logString = "   PNDR_RETURN_BRANDING_IMAGE_SEGMENT  to mobile"   ; break;
        case    0x03    :   logString = "   PNDR_GET_STATUS     to mobile"   ; break;
        case    0x04    :   logString = "   PNDR_SESSION_FLAG_SIMULATE_RANDOM_ERRORS    to mobile"   ; break;
        case    0x05    :   logString = "   PNDR_SESSION_TERMINATE  to mobile"   ; break;
        case    0x08    :   logString = "   PNDR_TRACK_FLAG_ALLOW_EXPLAIN   to mobile"   ; break;
        case    0x10    :   logString = "   PNDR_GET_TRACK_INFO     to mobile"   ; break;
        case    0x11    :   logString = "   PNDR_GET_TRACK_TITLE    to mobile"   ; break;
        case    0x12    :   logString = "   PNDR_GET_TRACK_ARTIST   to mobile"   ; break;
        case    0x13    :   logString = "   PNDR_GET_TRACK_ALBUM    to mobile"   ; break;
        case    0x14    :   logString = "   PNDR_GET_TRACK_ALBUM_ART    to mobile"   ; break;
        case    0x15    :   logString = "   PNDR_SET_TRACK_ELAPSED_POLLING  to mobile"   ; break;
        case    0x16    :   logString = "   PNDR_GET_TRACK_INFO_EXTENDED    to mobile"   ; break;
        case    0x30    :   logString = "   PNDR_EVENT_TRACK_PLAY   to mobile"   ; break;
        case    0x31    :   logString = "   PNDR_EVENT_TRACK_PAUSE  to mobile"   ; break;
        case    0x32    :   logString = "   PNDR_EVENT_TRACK_SKIP   to mobile"   ; break;
        case    0x33    :   logString = "   PNDR_EVENT_TRACK_RATE_POSITIVE  to mobile"   ; break;
        case    0x34    :   logString = "   PNDR_EVENT_TRACK_RATE_NEGATIVE  to mobile"   ; break;
        case    0x35    :   logString = "   PNDR_EVENT_TRACK_EXPLAIN    to mobile"   ; break;
        case    0x36    :   logString = "   PNDR_GET_TRACK_EXPLAIN  to mobile"   ; break;
        case    0x37    :   logString = "   PNDR_EVENT_TRACK_BOOKMARK_SONG  to mobile"   ; break;
        case    0x38    :   logString = "   PNDR_EVENT_TRACK_BOOKMARK_ARTIST    to mobile"   ; break;
        case    0x40    :   logString = "   PNDR_GET_STATION_ACTIVE     to mobile"   ; break;
        case    0x41    :   logString = "   PNDR_GET_STATION_COUNT  to mobile"   ; break;
        case    0x42    :   logString = "   PNDR_GET_STATION_TOKENS     to mobile"   ; break;
        case    0x43    :   logString = "   PNDR_GET_ALL_STATION_TOKENS     to mobile"   ; break;
        case    0x44    :   logString = "   PNDR_GET_STATION_INFO   to mobile"   ; break;
        case    0x45    :   logString = "   PNDR_GET_STATIONS_ORDER     to mobile"   ; break;
        case    0x46    :   logString = "   PNDR_EVENT_STATIONS_SORT    to mobile"   ; break;
        case    0x47    :   logString = "   PNDR_EVENT_STATION_SELECT   to mobile"   ; break;
        case    0x4B    :
        {
            inData = dataArray.toHex().mid(6, 8);
            cmdid = inData.toInt(&ok,16);

            if(cmdid  == 0x00 )
            {
                logString = "   << ACK SEND >> type = " ; 
                inData = dataArray.toHex().mid(2, 2);
                logString =logString + inData; 

                logString = logString + "sequence  = " ; 
                inData = dataArray.toHex().mid(4, 2);
                logString =logString + inData; 
                isACK = true;
                break;
            }
            else
                logString = "   PNDR_GET_STATION_ART    to mobile"   ; 
            break;
        }
        
        case    0x4C    :   logString = "   PNDR_EVENT_CANCEL_STATION_ART   to mobile"   ; break;
        case    0x60    :   logString = "   PNDR_EVENT_SEARCH_AUTO_COMPLETE     to mobile"   ; break;
        case    0x61    :   logString = "   PNDR_EVENT_SEARCH_EXTENDED  to mobile"   ; break;
        case    0x62    :   logString = "   PNDR_GET_SEARCH_RESULT_INFO     to mobile"   ; break;
        case    0x63    :   logString = "   PNDR_EVENT_SEARCH_SELECT    to mobile"   ; break;
        case    0x64    :   logString = "   PNDR_SEARCH_DISCARD     to mobile"   ; break;
        case    0x7f    :   logString = "   PNDR_ECHO_REQUEST   to mobile"   ; break;
        
        case    0x80    :   logString = "   PNDR_GET_BRANDING_IMAGE  to Accessory   "   ; break;
        case    0x81    :   logString = "   PNDR_UPDATE_STATUS  to Accessory"   ; break;
        case    0x82    :   logString = "   PNDR_RETURN_STATUS  to Accessory"   ; break;
        case    0x83    :   logString = "   PNDR_UPDATE_NOTICE  to Accessory"   ; break;
        case    0x90    :   logString = "   PNDR_UPDATE_TRACK   to Accessory"   ; break;
        case    0x91    :   logString = "   PNDR_RETURN_TRACK_INFO  to Accessory"   ; break;
        case    0x92    :   logString = "   PNDR_RETURN_TRACK_TITLE     to Accessory"   ; break;
        case    0x93    :   logString = "   PNDR_RETURN_TRACK_ARTIST    to Accessory"   ; break;
        case    0x94    :   logString = "   PNDR_RETURN_TRACK_ALBUM     to Accessory"   ; break;
        case    0x95    :   logString = "   PNDR_RETURN_TRACK_ALBUM_ART_SEGMENT     to Accessory"   ; break;
        case    0x96    :   logString = "   PNDR_UPDATE_TRACK_ALBUM_ART     to Accessory"   ; break;
        case    0x97    :   logString = "   PNDR_UPDATE_TRACK_ELAPSED   to Accessory"   ; break;
        case    0x98    :   logString = "   PNDR_UPDATE_TRACK_RATING    to Accessory"   ; break;
        case    0x99    :   logString = "   PNDR_UPDATE_TRACK_EXPLAIN   to Accessory"   ; break;
        case    0x9a    :   logString = "   PNDR_RETURN_TRACK_EXPLAIN_SEGMENT   to Accessory"   ; break;
        case    0x9b    :   logString = "   PNDR_UPDATE_TRACK_BOOKMARK_SONG     to Accessory"   ; break;
        case    0x9c    :   logString = "   PNDR_UPDATE_TRACK_BOOKMARK_ARTIST   to Accessory"   ; break;
        case    0x9d    :   logString = "   PNDR_RETURN_TRACK_INFO_EXTENDED     to Accessory"   ; break;
        case    0x9e    :   logString = "   PNDR_UPDATE_TRACK_COMPLETED     to Accessory"   ; break;
        case    0xb1    :   logString = "   PNDR_RETURN_STATION_ACTIVE  to Accessory"   ; break;
        case    0xb2    :   logString = "   PNDR_RETURN_STATION_COUNT   to Accessory"   ; break;
        case    0xb3    :   logString = "   PNDR_RETURN_STATION_TOKENS  to Accessory"   ; break;
        case    0xb4    :   logString = "   PNDR_RETURN_STATION_INFO    to Accessory"   ; break;
        case    0xb5    :   logString = "   PNDR_RETURN_STATIONS_ORDER  to Accessory"   ; break;
        case    0xb6    :   logString = "   PNDR_UPDATE_STATIONS_ORDER  to Accessory"   ; break;
        case    0xb7    :   logString = "   PNDR_UPDATE_STATION_DELETED     to Accessory"   ; break;
        case    0xb8    :   logString = "   PNDR_RETURN_STATION_ART_SEGMENT     to Accessory"   ; break;
        case    0xba    :   logString = "   PNDR_UPDATE_STATION_ACTIVE  to Accessory"   ; break;
        case    0xd0    :   logString = "   PNDR_UPDATE_SEARCH  to Accessory"   ; break;
        case    0xd1    :   logString = "   PNDR_RETURN_SEARCH_RESULT_INFO  to Accessory"   ; break;
        case    0xd2    :   logString = "   PNDR_UPDATE_STATION_ADDED   to Accessory"   ; break;
        case    0xff    :   logString = "   PNDR_ECHO_RESPONSE  to Accessory"   ; break;
        case    0xe1    : 
        {
            inData = dataArray.toHex().mid(6, 8);
            cmdid = inData.toInt(&ok,16);
        
            if(cmdid  == 0x00 )
            {               
                logString = "   << ACK SEND >> type = " ; 
                inData = dataArray.toHex().mid(2, 2);
                logString =logString + inData; 

                logString = logString + "sequence  = " ; 
                inData = dataArray.toHex().mid(4, 2);
                logString =logString + inData; 
                isACK = true;
            }
        }
        break;
        default :
             logString = "unknown command ID "  + QString::number(cmdid, 16) ; break;
    }

    if(cmdid == 0x97 || isACK )
    {
        LOG_LOW << (isbt ? "[BT]" :"[USB]") << " CPndrFrameTxRx::LogCommandID :" << logString << LOG_ENDL;
    }
    else
    {
        LOG_CRITICAL << (isbt ? "[BT]" :"[USB]") << " CPndrFrameTxRx::LogCommandID :" << logString << LOG_ENDL;
    }

    return;
}

//End Of File
