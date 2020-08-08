/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                   This is the Autherizor TaskHandler           *
 *                   it Starts and handle the whole auth-         *
 *                   ntication process,
 *                   and helps ipod to enter into UI mode         *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <DHAVN_IPodController_authorisor.h>
#include <DHAVN_IPodController_ack.h>
#include <QDebug>
#include <DHAVN_IPodController_CommonVars.h>
#include <DHAVN_IPodController_IAPPacketReader.h>
#include <DHAVN_IPodController_IAPPacketWriter.h>
#include <DHAVN_IPodController_getDevAuthenticationInfo.h>
#include <DHAVN_IPodController_retDevAuthenticationInfo.h>
#include <DHAVN_IPodController_ackDevAuthenticationInfo.h>
#include <DHAVN_IPodController_getDevAuthenticationSignature.h>
#include <DHAVN_IPodController_retDevAuthenticationSignature.h>
#include <DHAVN_IPodController_ackDevAuthenticationStatus.h>
#include <qtimer.h>
#include <DHAVN_IPodController_coprocessorDataProvider.h>
#include <DHAVN_IPodController_remoteUiModeTaskHandler.h>
#include <DHAVN_IPodController_retDevAuthenticationInfoNonIDPS.h>
#include <DHAVN_IPodController_remoteUiModeTaskHandler.h>
#include <DHAVN_IPodController_eventloopTimer.h>
#include <DHAVN_IPodController_Identifier.h>
#include <DHAVN_IPodController_Performance.h>

using namespace GeneralLingo;

#define NANO_6G_AUTHORISATION_TIME 8000

CAuthorisorTaskHandler::CAuthorisorTaskHandler(QObject *parent)
    :CTaskHandler(parent)
{

    m_iPodPrivateData = parent->findChild<iPodControllerPrivateData*>(gControllerPrivateData);

    switch( m_iPodPrivateData->m_currentPortNum )
    {
       case eIPOD_PORT_1:
            LOG_INIT_V2(giPodControllerLogName_1);
            break;
        case eIPOD_PORT_2:
            LOG_INIT_V2(giPodControllerLogName_2);
            break;
        default:
            LOG_INIT_V2(giPodControllerLogName);
    }

    //LOG_TRACE << "CAuthorisorTaskHandler::CAuthorisorTaskHandler() IN" << LOG_ENDL;

    //m_TransId = 1;
    m_CertIncrementCount        = 0;
    m_CertIncrementCountNonIDPS = 0;
    m_IsTimeToRespond           = false;
    m_authCount                 = 0;
    m_LocalTransId              = 0;
    m_WaitingResponseOfCommand  = -1;  // added for identify retry(CR13200)

    setObjectName(gAuthorisor);

    qRegisterMetaType<QByteArray>("QByteArray");

    connect(((CIpodController*) parent)->m_Reader, SIGNAL(packetAvailable(int, int, QByteArray)),
            this, SLOT(packetAvailable(int,int,QByteArray)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(sendiAPPacket(QByteArray)),
            ((CIpodController*) parent)->m_Writer, SLOT(sendiAPPacket(QByteArray)),
            Qt::QueuedConnection);

    connect(&m_authTimer, SIGNAL(timeout()),
            this, SLOT(retDevAuthentication()) );
    m_authTimer.setSingleShot(true);
    //LOG_TRACE << "CAuthorisorTaskHandler::CAuthorisorTaskHandler() OUT" << LOG_ENDL;
}

CAuthorisorTaskHandler::~CAuthorisorTaskHandler()
{

}


/* Stops the entair taskhandler. This will disconnect the taskhandler form iAP channel*
 * This can be called if the task handler is not required to operate.
 * This will improve the performance.
*/
void CAuthorisorTaskHandler::stopTaskHandler()
{
    disconnect(((CIpodController*) parent())->m_Reader,SIGNAL(packetAvailable(int, int, QByteArray)),this, SLOT(packetAvailable(int,int,QByteArray)));
    disconnect(this, SIGNAL(sendiAPPacket(QByteArray)) ,((CIpodController*) parent())->m_Writer, SLOT(sendiAPPacket(QByteArray)) );

}

void CAuthorisorTaskHandler::stopCurrentTask()
{
    //TODO
}


/******************************************************************************
// Function Name                                      Parameters
// CAuthorisorTaskHandler::Authorisor_restart_slot    (void) (OUT), (void) (IN)
//
// Explanation
// Clear Authenticaation relative information and ready to authenticate again
//
// History
// 09-Dec-2012      SK Paek                        Added initial implementation
*****************************************************************************/
void CAuthorisorTaskHandler::Authorisor_restart_slot()
{
    //LOG_TRACE << "CAuthorisorTaskHandler::Authorisor_restart_slot() IN" << LOG_ENDL;
    m_CertIncrementCount = 0;
    m_CertIncrementCountNonIDPS = 0;

    m_WaitingResponseOfCommand = CIAPCmds::L0_14_GetAccessoryAuthenticationInfo;
    //LOG_TRACE << "CAuthorisorTaskHandler::Authorisor_restart_slot() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CAuthorisorTaskHandler::retDevAuthentication             (void) (OUT), (void) (IN)
//
// Explanation
// Send the command for the retDevAuthentication
// get the certificate from data provider
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CAuthorisorTaskHandler::retDevAuthentication()
{
    PT_START_TIME(PT_OBJ_AUTHENTICATION, 0);
    
    //LOG_TRACE << "CAuthorisorTaskHandler::retDevAuthentication() IN" << LOG_ENDL;

    CCoProcessorDataProvider* dataProvider = parent()->findChild<CCoProcessorDataProvider*>(gCoprocessorDataProvider);
    //LOG_TRACE << "   certificate is fetched" << LOG_ENDL;
    int productID =((CIpodController*)parent())->m_Writer->getProductID();
    //LOG_TRACE << "   productID = " << productID << LOG_ENDL;

    /*  All IDPS devices are working fine when the certificate data is sent as large packet
        but Nano6G is not working. Hence sendig as small packet. */
    int packetSize = 0;
    if ( productID == NANO_6G_PRODUCT_ID || productID == NANO_7G_PRODUCT_ID)
        packetSize = MAXCERLENGTHSMALLPACKET;
    else
         packetSize = parent()->findChild<CIdentifierTaskHandler*>(gIdentifier)->getMaxPacketSize();

    //LOG_TRACE << "   packetSize = " << packetSize << LOG_ENDL;
    CRetDevAuthenticationInfo cmd(m_LocalTransId, dataProvider->certificate(), dataProvider->cerificateLength(),
                   m_CertIncrementCount, m_iPodPrivateData->m_isIDPS,packetSize );
    cmd.marshall();
    if(cmd.getCurrentSection() <= cmd.getTotalSections())
    {
        m_CertIncrementCount++;
        emit sendiAPPacket(cmd.getCommandBytes());
    }

    //LOG_TRACE << "CAuthorisorTaskHandler::retDevAuthentication() OUT" << LOG_ENDL;
}

void CAuthorisorTaskHandler::retDevAuthenticationNonIDPS()
{
    //LOG_TRACE << "CAuthorisorTaskHandler::retDevAuthenticationNonIDPS() IN" << LOG_ENDL;

    CCoProcessorDataProvider* dataProvider = parent()->findChild<CCoProcessorDataProvider*>(gCoprocessorDataProvider);
    CRetDevAuthenticationInfoNonIDPS cmd(m_LocalTransId, dataProvider->certificate(), dataProvider->cerificateLength(), m_CertIncrementCountNonIDPS);
    cmd.marshall();

    //LOG_TRACE << "   cmd.getCurrentSection = " << cmd.getCurrentSection() << ", cmd.getTotalSections = "<< cmd.getTotalSections() << LOG_ENDL;
    if(cmd.getCurrentSection() <= cmd.getTotalSections())
    {
        m_CertIncrementCountNonIDPS++;
        emit sendiAPPacket(cmd.getCommandBytes());
    }
    //LOG_TRACE << "CAuthorisorTaskHandler::retDevAuthenticationNonIDPS() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CAuthorisorTaskHandler::retDevAuthenticationSignature    (void) (OUT), (QByteArray) (IN)
//
// Explanation
// Send the command for the retDevAuthenticationSignature
// in the CRetDevAuthenticationSignature packet class the signature
// will be generated and will be sent in command
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CAuthorisorTaskHandler::retDevAuthenticationSignature(QByteArray chal)
{
    //LOG_TRACE << "CAuthorisorTaskHandler::retDevAuthenticationSignature() IN" << LOG_ENDL;
    CRetDevAuthenticationSignature cmd( m_iPodPrivateData->m_isIDPS , m_LocalTransId, chal, chal.length(), parent()->findChild<CCoProcessorDataProvider*>(gCoprocessorDataProvider) );
    cmd.marshall();
    emit sendiAPPacket(cmd.getCommandBytes());
    //LOG_TRACE << "CAuthorisorTaskHandler::retDevAuthenticationSignature() OUT" << LOG_ENDL;
}

/******************************************************************************
// Function Name                                            Parameters
// CAuthorisorTaskHandler::packetAvailable                 (void) (OUT), (int, int, QByteArray) (IN)
//
// Explanation
// Slot to handle the command which is read by the raw packet handler
// check for the last command status and on the basis of the status
// takes next action
// handle the whole flow of the authentication process
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CAuthorisorTaskHandler::packetAvailable(int lingoID,int commandID,const QByteArray& buf )
{
#ifdef DBUGLOGS
    //LOG_TRACE << "In CAuthorisorTaskHandler::packetAvailable() Entry" << LOG_ENDL;
#endif
    try
    {
        // if authentication process is retrying(CR 13200)
        if(m_WaitingResponseOfCommand == CIAPCmds::L0_14_GetAccessoryAuthenticationInfo)
        {
            // waiting Ack for CIAPCmds::L0_14_GetAccessoryAuthenticationInfo
            if(lingoID == 0x00) // for general Lingo
            {
                if(commandID == CIAPCmds::L0_14_GetAccessoryAuthenticationInfo)     // waiting first command in Authenticatoin process
                {
                    m_WaitingResponseOfCommand = -1;      // Clear waiting status of CIAPCmds::L0_14_GetAccessoryAuthenticationInfo
                }
                else
                {
                    //LOG_TRACE << "   lingoID = " << lingoID << ", command ID = "  << commandID << " is ignored until Ack for CIAPCmds::L0_14_GetAccessoryAuthenticationInfo" << LOG_ENDL;
                    return; // ignore all commands/response to prohibit unexpected working until CIAPCmds::L0_14_GetAccessoryAuthenticationInfo
                }
            }
            else
            {
                //LOG_TRACE << "   lingoID = " << lingoID << ", command ID = "  << commandID << " is ignored until Ack for CIAPCmds::L0_14_GetAccessoryAuthenticationInfo" << LOG_ENDL;
                return; // ignore all commands/response to prohibit unexpected working until CIAPCmds::L0_14_GetAccessoryAuthenticationInfo
            }
        }

        if(lingoID == 0x00) // for general Lingo
        {
            switch(commandID)
            {
                case 0x02: //ACK = 0x02
                {
                    CACK cmd(m_iPodPrivateData->m_isIDPS);
                    cmd.unmarshall(buf);
                    int ackCmd= cmd.getAcknowledgedCommand();
                    //LOG_INFO << "   ack command status = " << cmd.getAckStatus() << LOG_ENDL;
                    switch(ackCmd)
                    {
                        case 0x15: //retdevAuthenticationInfo
                        {
                            //LOG_TRACE << "   retdevAuthenticationInfo is received" << LOG_ENDL;
                            if( m_iPodPrivateData->m_isIDPS )
                            {
                                if( (parent()->findChild<CIdentifierTaskHandler*>(gIdentifier))->m_EndIdpsState == true )
                                {
                                    retDevAuthentication();
                                }
                            }
                            else
                            {
                                retDevAuthenticationNonIDPS();
                            }
                        }
                            break;
                        default :
                            break;
                    }
                }
                    break;
                case 0x14: //GetDevAuthenticationInfo
                {
                    //LOG_TRACE << "   ack of GetDevAuthenticationInfo" << LOG_ENDL;
                    CGetDevAuthenticationInfo cmd(m_iPodPrivateData->getTransactionId(false),  m_iPodPrivateData->m_isIDPS );
                    cmd.unmarshall(buf);
                    m_LocalTransId = cmd.getTransactionId();
                    if( ((CIpodController*)parent())->m_IsIDPS == true  )
                    {
                        if( (parent()->findChild<CIdentifierTaskHandler*>(gIdentifier))->m_EndIdpsState == true )
                        {
                            //LOG_TRACE << "   GetDevAuthenticationInfo is IDPS" << LOG_ENDL;
                            int productID =((CIpodController*)parent())->m_Writer->getProductID();
                            //CR14645: remove the delay as the Kernel has fixed the issue
                            //if (productID == NANO_6G_PRODUCT_ID)
                            //{
                            //    if(m_authCount == 0)
                            //    {
                            //        m_authCount++;
                            //        m_authTimer.stop();
                            //        m_authTimer.start(NANO_6G_AUTHORISATION_TIME);
                            //        //LOG_TRACE << "m_authCount: " << m_authCount <<LOG_ENDL;
                            //    }
                            //    //LOG_TRACE << "It is a Nano 6G" << productID << LOG_ENDL;
                            //}
                            //else
                            //{
                            //LOG_TRACE << "   It is not a Nano 6G" << productID << LOG_ENDL;
                            retDevAuthentication();
                            //}
                        }
                    }
                    else
                    {
                        //LOG_TRACE << "   GetDevAuthenticationInfo is NON IDPS" << LOG_ENDL;
			            retDevAuthenticationNonIDPS();
                    }
                    break;
                }
                case 0x16: //AckDevAuthenticationInfo
                {
                    //LOG_TRACE << "   ack of AckDevAuthenticationInfo" << LOG_ENDL;
                    CAckDevAuthenticationInfo cmd(m_iPodPrivateData->getTransactionId(false),  m_iPodPrivateData->m_isIDPS );
                    cmd.unmarshall(buf);

                    if(cmd.getAuthenticationInfoStatus())
                    {
                        //LOG_TRACE << "   Authentication failed with the error status = " << cmd.getAuthenticationInfoStatus() << LOG_ENDL;
                        //LOG_TRACE << "   emit authorised(false)_"  << LOG_ENDL;
                        emit authorised(false);
                    }

                    m_CertIncrementCount = 0;
                    m_CertIncrementCountNonIDPS = 0;
                    //LOG_TRACE << "   ack of AckDevAuthenticationInfo is passed" << LOG_ENDL;
                }
                    break;
                case 0x17: //getdevAuthenticationSignature
                {
                    /*
                    if( ((CIpodController*)parent())->m_IsIDPS == false  )
                    {
                        if( m_IsTimeToRespond == false ) break;
                    }
                    */

                    CGetDevAuthenticationSignature cmd(m_LocalTransId,  m_iPodPrivateData->m_isIDPS );
                    cmd.unmarshall(buf);
                    //int retryCounter = cmd.getAuthenticationRetryCounter();
                    QByteArray challenge = cmd.getOfferedChallenge();
                    //LOG_TRACE << "   CGetDevAuthenticationSignature" << LOG_ENDL;
                    retDevAuthenticationSignature(challenge);
                }
                    break;
                case 0x19: //AckDevAuthenticationStatus
                {
                    CAckDevAuthenticationStatus cmd(m_iPodPrivateData->getTransactionId(false),  m_iPodPrivateData->m_isIDPS );
                    cmd.unmarshall(buf);
                    int status = cmd.getAuthenticationStatus();
                    if(status == 0x00)
                    {
                        //LOG_TRACE << "   Authenitication Passed AckDevAuthenticationStatus case executed with passed status now entering into UI mode" << LOG_ENDL;
                        //LOG_TRACE << "   emit authorised(true)_"  << LOG_ENDL;
                        emit authorised(true);
                    }
                    else
                    {
                        //LOG_TRACE << "   Authenitication Failed AckDevAuthenticationStatus case executed with failed status" << LOG_ENDL;
                        //LOG_TRACE << "   emit authorised(false)_"  << LOG_ENDL;
                        emit authorised(false);
                    }
                }
                    break;
                default:
                    break;
                    //LOG_TRACE << "   Not my lingo...ignoring!" << LOG_ENDL;
            } //end of switch
        }
        else
        {
#ifdef DBUGLOGS
            //LOG_TRACE << "Not my lingo...ignoring!" << LOG_ENDL;
#endif
            return;
        }
#ifdef DBUGLOGS
        //LOG_TRACE << "CAuthorisorTaskHandler::packetAvailable() OUT" << LOG_ENDL;
#endif
    }   // end try
    catch(EIPodErrors ex)
    {
        //LOG_HIGH << "CAuthorisorTaskHandler::PacketAvailable() Execption Caught *******"<< LOG_ENDL;
    }
    return;
}


void CAuthorisorTaskHandler::RemoteUiModeActivated(bool status)
{
    //LOG_TRACE << "CAuthorisorTaskHandler::RemoteUiModeActivated() IN" << LOG_ENDL;
    if(status)
    {
        //LOG_TRACE << "   ############      As Passed     #############" << LOG_ENDL;
    }
    else
    {
        //LOG_TRACE << "   ############      As Failed     #############" << LOG_ENDL;
    }
   //LOG_TRACE << "In CAuthorisorTaskHandler::RemoteUiModeActivated() Exit" << LOG_ENDL;
}
