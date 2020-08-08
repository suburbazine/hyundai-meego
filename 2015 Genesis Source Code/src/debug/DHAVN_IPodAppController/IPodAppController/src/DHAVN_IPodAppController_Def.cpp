/*
 ******************************************************************
 *        COPYRIGHT © Teleca AB                                   *
 *----------------------------------------------------------------*
 * MODULE     :  IPodAppController                                *
 *
 * PROGRAMMERS : Pawan Gupta                                      *
 * DATE       :  16 jan 2012                                      *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :

 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include "DHAVN_IPodAppController_Def.h"
#include <DHAVN_IPodAppController_Private.h>

/******************************************************************************
// Function                                            Parameters
// CIPodAppController::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
CIPodAppController::CIPodAppController(QObject* parant) : QObject(parant), m_IPodAppControllerPriv(NULL)
{
    LOG_INIT_V2(gIPodAppControllerLogs);
}

/******************************************************************************
// Function                                            Parameters
// CIPodAppController::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
CIPodAppController::~CIPodAppController()
{
    //LOG_TRACE << "CIPodAppController::~CIPodAppController() IN" << LOG_ENDL;;
    delete m_IPodAppControllerPriv;
    //LOG_TRACE << "CIPodAppController::~CIPodAppController() OUT" << LOG_ENDL;;
}

/******************************************************************************
// Function                                            Parameters
// CIPodAppController::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
EIPodAppErrors CIPodAppController::Init(const QString& AppId, const QString& BundleSeedId, int ipodNumber, int protocolIndex) // added by Tan 2013.01.05
{

    //LOG_TRACE << "CIAPPacketWriter::Init IN \n" << LOG_ENDL;;

    if(!m_IPodAppControllerPriv)
    {
        m_IPodAppControllerPriv = new CIPodAppControllerPriv(this);
        connect(m_IPodAppControllerPriv, SIGNAL(InitComplete()), this, SIGNAL(InitComplete()));
        connect(m_IPodAppControllerPriv, SIGNAL(DataReceived(QByteArray)), this, SIGNAL(DataReceived(QByteArray)));
        connect(m_IPodAppControllerPriv, SIGNAL(SessionClosed(EIPodAppErrors)), this, SIGNAL(SessionClosed(EIPodAppErrors)));
        connect(m_IPodAppControllerPriv, SIGNAL(SessionOpened(int)), this, SIGNAL(SessionOpened(int))); // added by Tan 2012.12.15
	connect(m_IPodAppControllerPriv, SIGNAL(NotifySessionClosed()), this, SIGNAL(NotifySessionClosed())); // added by Tan 2013.01.05
        connect(m_IPodAppControllerPriv, SIGNAL(AudioSampleRateChanged(int)), this, SIGNAL(AudioSampleRateChanged(int))); // added by Tan 2013.01.05
        connect(m_IPodAppControllerPriv, SIGNAL(Acknowledgement(EIPodAppErrors)), this, SIGNAL(Acknowledgement(EIPodAppErrors)));
        connect(m_IPodAppControllerPriv, SIGNAL(DeviceDisconnected(int)), this, SIGNAL(DeviceDisconnected(int)));
        //LOG_TRACE << "CIAPPacketWriter::Init end \n" << LOG_ENDL;;
        return m_IPodAppControllerPriv->Init(AppId, BundleSeedId, ipodNumber, protocolIndex); // modified by Tan 2013.01.05
    }
    else if(m_IPodAppControllerPriv && !m_IPodAppControllerPriv->IsInitialized())
    {
        //LOG_TRACE << "CIAPPacketWriter::Init end \n" << LOG_ENDL;;
        return m_IPodAppControllerPriv->Init(AppId, BundleSeedId, ipodNumber, protocolIndex); // modified by Tab 2013.01.05
    }
    else
    {
        //LOG_TRACE << "CIAPPacketWriter::Init end \n" << LOG_ENDL;;
        return IPOD_APPERROR_ALREADYINITIALIZED;
    }

    ////LOG_TRACE << "CIAPPacketWriter::Init end \n" << LOG_ENDL;;

}

/******************************************************************************
// Function                                            Parameters
// CIPodAppController::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
EIPodAppErrors CIPodAppController::D_Init()
{
    //LOG_TRACE << "CIAPPacketWriter::D_Init IN \n" << LOG_ENDL;;

    EIPodAppErrors res = IPOD_APPERROR_NOTINITIALIZED;
    if(m_IPodAppControllerPriv)
    {
        res = m_IPodAppControllerPriv->D_Init();
        delete m_IPodAppControllerPriv;
        m_IPodAppControllerPriv = NULL;
    }
    //LOG_TRACE << "CIAPPacketWriter::D_Init end \n" << LOG_ENDL;;

    return res;
}

/******************************************************************************
// Function                                            Parameters
// CIPodAppController::
//
// Explanation

// History
// 06-jan-2012      Pawan Gupta                        Added initial implementation
*****************************************************************************/
EIPodAppErrors CIPodAppController::Send(const QByteArray& dataArray)
{
    //LOG_TRACE << "CIAPPacketWriter::Send IN \n" << LOG_ENDL;;

    EIPodAppErrors res = IPOD_APPERROR_NOTINITIALIZED;
    if(m_IPodAppControllerPriv && m_IPodAppControllerPriv->IsInitialized())
    {
        res = m_IPodAppControllerPriv->Send(dataArray);
    }
    //LOG_TRACE << "CIAPPacketWriter::Send end \n" << LOG_ENDL;;

    return res;
}

EIPodAppErrors CIPodAppController::LaunchApplication(QString appBundleID)
{
    EIPodAppErrors res = IPOD_APPERROR_NOTINITIALIZED;
    if(m_IPodAppControllerPriv && m_IPodAppControllerPriv->IsInitialized())
    {
     res = m_IPodAppControllerPriv->RequestForAppLaunch(appBundleID);
    }
    return res;
}
