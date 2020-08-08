/*
 ******************************************************************
 * COPYRIGHT © <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER :  Venkatesh Aeturi                                 *
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
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 * 09 Aug 2011   Venkatesh Aeturi         Draft version
 * 10 Aug 2011   Venkatesh Aeturi         updated for comments
 * 15 Sep 2011   Amit Koparde             Added Debug Statement
 * 27 Sep 2011   Amit Koparde             Added new API CurrentSongInfo and TerminalMode to CAuxInFblockControl
 * 08 Feb 2011   Neelima Khedad           Using the latest Version of LOG_INIT function Logger module
 * 17 Jul 2012  Shiva Kumar              Modified for FCAT 1.18.2
 ******************************************************************
 */

#include "DHAVN_MOSTManager_AuxInFblockControl.h"
#include "DHAVN_MOSTManager_MOSTStructures.h"

/*
 *******************************************************************
 * Function:CAmFmTunerFblockControl
 *
 * Description :This is constructor of CAmFmTunerFblockControl class.
 *
 * Parameters :
 *    context    rw   It holds the instance of CMarshaledACSApplication.
 *
 * Returns : none
 *
 *******************************************************************
 */

CAuxInFblockControl::CAuxInFblockControl(CMarshaledACSApplication *context)
    : CControlThreadBase(context, LOG_TAG_AUX_CNT)
{
    LOG_INIT_V2(LOG_PREFIX_MOST)    

    m_AuxInFblock_1 = new CAuxIn(0x01);
    m_AuxInFblock_2 = new CAuxIn(0x02);
}

/*
 *******************************************************************
 * Function:~CAmFmTunerFblockControl
 *
 * Description :Is a Destructor
 *
 * Parameters : none
 *
 * Returns :none
 *
 *******************************************************************
 */

CAuxInFblockControl::~CAuxInFblockControl()
{
    delete m_AuxInFblock_1;
    delete m_AuxInFblock_2;
}

/*
 *******************************************************************
 * Function:Initialize
 *
 * Description :Function initializes the MOST elements to AuxIn 
 *              FBlock and adds to MOST.
 *
 * Parameters : none
 *    
 *
 * Returns :true or false
 *
 *******************************************************************
 */

bool CAuxInFblockControl::Initialize()
{
    if(m_AuxInFblock_1->InitializeMOSTElement() && m_AuxInFblock_2->InitializeMOSTElement())
    {
        // Implemente initail values.
        m_Initialized = true;
    }

    return m_Initialized;
}

bool CAuxInFblockControl::Register()
{
    bool result = false;

    MOST_LOG_INFO << LOG_TAG_AUX_CNT << " Register API got called " << MOST_LOG_ENDL;

    if(!m_Registered)
    {
        result = m_AppContext->MOST()->AddFunctionBlock(m_AuxInFblock_1);
        result &= m_AppContext->MOST()->AddFunctionBlock(m_AuxInFblock_2);

        if(result)
        {
            MOST_LOG_INFO << LOG_TAG_AUX_CNT << " Register FBlock successfully " << MOST_LOG_ENDL;
            m_Registered = true;
        }
        else
        {
            MOST_LOG_CRITICAL << LOG_TAG_AUX_CNT << " Failed to Register FBlock " << MOST_LOG_ENDL;
        }
    }
    else
    {
        MOST_LOG_INFO << LOG_TAG_AUX_CNT << " FBlock was already registered " << MOST_LOG_ENDL;
        // A case already registered
        result = true;
    }

    return result;
}

bool CAuxInFblockControl::UnRegister()
{
    bool result = false;

    MOST_LOG_INFO << LOG_TAG_AUX_CNT << " UnRegister API got called " << MOST_LOG_ENDL;

    if(m_Registered)
    {
        result = m_AppContext->MOST()->RemoveFunctionBlock(m_AuxInFblock_1);
        result &= m_AppContext->MOST()->RemoveFunctionBlock(m_AuxInFblock_2);

        if(result)
        {
            MOST_LOG_INFO << LOG_TAG_AUX_CNT << " UnRegister FBlock successfully " << MOST_LOG_ENDL;
            m_Registered = false;
        }
        else
        {
            MOST_LOG_CRITICAL << LOG_TAG_AUX_CNT << " Failed to Unregister FBlock " << MOST_LOG_ENDL;
        }
    }
    else
    {
        MOST_LOG_INFO << LOG_TAG_AUX_CNT << " FBlock was already Unregistered " << MOST_LOG_ENDL;
        result = true;
    }

    return result;
}

bool CAuxInFblockControl::FBlockVisible()
{
    return GetIsRegistered();
}


/*
 *******************************************************************
 * Function:AuxDeviceInfo
 *
 * Description :Function called by AuxIn Adaptor to send the AuxDevice information.
 *
 * Parameters :
 *    devicenum      rw   Is unsigned value  which holds the devicenum.
 *    deviceclass    rw   Is unsigned value  which holds the deviceclass.
 *    devicetype     rw   Is unsigned value  which holds the devicetype.
 *    auxIn_no       rw   Is unsigned byte which holds the auxin number.
 *
 * Returns :true or false
 *
 *******************************************************************
 */

bool CAuxInFblockControl::AuxDeviceInfo(const uint devicenum, const uint deviceclass, const uint devicetype, const QString &devicename, const uint auxIn_no)
{
// 2014.07.21, for log, hyunyong.sung
    MOST_LOG_SIGNAL << LOG_TAG_AUX_CNT << "AuxDeviceInfo_Status --> : Number = " << devicenum 
	<< " Class = " << deviceclass << " Type = " << devicetype << " Name = " << devicename << " AuxInNum = " << auxIn_no << MOST_LOG_ENDL;

/*
    MOST_LOG_SIGNAL << LOG_TAG_AUX_CNT << " AuxDeviceInfo API got called " << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_AUX_CNT << " AuxDeviceInfo:Device Number = " << devicenum << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_AUX_CNT << " AuxDeviceInfo:Device Class = " << deviceclass << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_AUX_CNT << " AuxDeviceInfo:Device Type = " << devicetype << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_AUX_CNT << " AuxDeviceInfo:Device Name = " << devicename << MOST_LOG_ENDL;
    MOST_LOG_INFO << LOG_TAG_AUX_CNT << " AuxDeviceInfo:AuxIn Number = " << auxIn_no << MOST_LOG_ENDL;
*/

    bool bRetVal = false;

    if(auxIn_no == AUX_ONE)
    {
        CAuxDeviceInfo deviceInfoProperty(m_AuxInFblock_1);
       
        deviceInfoProperty.DeviceNumValue(devicenum);
        deviceInfoProperty.DeviceClassValue((CAuxDeviceInfo::TDeviceClass) deviceclass);
        deviceInfoProperty.DeviceTypeValue((CAuxDeviceInfo::TDeviceType) devicetype);


        deviceInfoProperty.DeviceNameString(&k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                            reinterpret_cast<const uint8_t*>(devicename.toUtf8().data()),
                                            devicename.length()+1
                                            );

        bRetVal = deviceInfoProperty.UpdateShadows();
    }

    else if(auxIn_no == AUX_TWO)
    {
        CAuxDeviceInfo deviceInfoProperty(m_AuxInFblock_2);
        
        deviceInfoProperty.DeviceNumValue(devicenum);
        deviceInfoProperty.DeviceClassValue((CAuxDeviceInfo::TDeviceClass) deviceclass);
        deviceInfoProperty.DeviceTypeValue((CAuxDeviceInfo::TDeviceType) devicetype);

        deviceInfoProperty.DeviceNameString( &k2l::acs::protocols::most::string::CStringEncoding::UTF8(),
                                             reinterpret_cast<const uint8_t*>(devicename.toUtf8().data()),
                                             devicename.length()+1
                                             );

        bRetVal = deviceInfoProperty.UpdateShadows();
    }

    return bRetVal;
}

