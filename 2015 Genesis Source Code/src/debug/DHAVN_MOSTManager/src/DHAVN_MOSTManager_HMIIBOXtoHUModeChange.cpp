/*
 ******************************************************************
 * COPYRIGHT © <2011>,
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER :  Shiva Kumar                                      *
 * DATE       :  12 May 2012                                     *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 * MODULE SUMMARY : MOST Manager is a middle ware component which *
 *                  talks to the applications and controller over *
 *                  QtDbus.The data sent over QtDbus is further   *
 *                  communicated to other MOST devices through ACS*
 *                  daemon.This module is also responsible for    *
 *                  commands from ACS daemon.                     *
 *                  This file contains the class definitions for  *
 *                  HMI ModeChange, Visibility and Audibility     *
 *                  responsible to provide the custom class for   *                                          *
 *                  HMI property ModeStatus, SystemVisibility and *
 *                  SystemAudibility of HMI FBlock in order to    *
 *                  block the status sent from acsdaemon on       *
 *                  receiving Set message                         *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT                  *
 ******************************************************************
 * 12 May 2012  Shiva Kumar              Draft version            *
 ******************************************************************
 */

#include "DHAVN_MOSTManager_HMIIBOXtoHUModeChange.h"

/*
 *******************************************************************
 * Function:CModeStatusCustomClass
 *
 * Description :It is the constructor of class CModeStatusCustomClass.
 *
 * Parameters :none
 *
 *
 * Returns :none
 *
 *******************************************************************
 */

CModeStatusCustomClass::CModeStatusCustomClass()
{

    LOG_INIT_V2(LOG_PREFIX_MOST)

}
/*
 *******************************************************************
 * Function:~CModeStatusCustomClass
 *
 * Description :This is the destructor for CModeStatusCustomClass class.
 *
 * Parameters :none
 *
 *
 * Returns :none
 *
 *******************************************************************
 */
CModeStatusCustomClass::~CModeStatusCustomClass()
{


}
/*
 *******************************************************************
 * Function:SetOperation
 *
 * Description :This function is overridden from CMOSTFunctionClassArray and
 *              will be invoked on receiving Set message.
 *
 * Parameters :
 *    flags               rw   MOST flages
 *    messageID           rw   MOST Message id
 *    sourceDeviceID      rw   MOST source device id
 *    payloadLength       rw   length of the payload
 *    payload             rw   payload received in MOST message
 *
 *
 * Returns :none
 *
 *******************************************************************
 */
void CModeStatusCustomClass::SetOperation(uint32_t flags, uint32_t messageID, DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
/*
    LOG_SIGNAL << " MODESTATUS SET: SetOperation in INVOKED " << LOG_ENDL;
    LOG_SIGNAL << " source ID : " << sourceDeviceID << LOG_ENDL;
    LOG_SIGNAL << " payloadLength : " << payloadLength << LOG_ENDL;
*/    
    k2l::acs::base::CBigEndianStreamReader reader(payload, payloadLength);

    uint8_t  posx = 0;
    uint8_t  posy = 0;
    uint8_t  mode = 0;
    uint8_t  mode1 = 0;
    uint8_t  mode2 = 0;
    reader.Read8(&posx);
    reader.Read8(&posy);
    reader.Read8(&mode);
/*
    LOG_SIGNAL << " Position X : " << posx << LOG_ENDL;
    LOG_SIGNAL << " Position Y : " << posy << LOG_ENDL;
    LOG_SIGNAL << " mode : " << mode << LOG_ENDL;
*/    
    if(posx == 0)
    {
        reader.Read8(&mode1);
        reader.Read8(&mode2);
/*		
        LOG_SIGNAL << " mode1 : " << mode1 << LOG_ENDL;
        LOG_SIGNAL << " mode2 : " << mode2 << LOG_ENDL;
*/        
    }

// 2014.09.29, for log, hyunyong.sung
    LOG_SIGNAL << "HMIControl " << " ModeStatus_Set <-- : Position X = " << posx << " Position Y = "
    << posy << " mode = " << mode << " mode1 = " << mode1 << " mode2 = " << mode2 << LOG_ENDL;

    ModeStatusfromIBOXSlot(posx,mode);
}
/*
 *******************************************************************
 * Function:ModeStatusfromIBOXSlot
 *
 * Description :This function is used to emit the signal to HMI Adaptor
 *
 * Parameters :
 *     target  rw  Target
 *     mode    rw  Differnt modes
 *
 * Returns :none
 *
 *******************************************************************
 */
void CModeStatusCustomClass::ModeStatusfromIBOXSlot(const uint target,const uint mode)
{
    emit ModeStatusfromIBOX(target,mode);
}
/*
 *******************************************************************
 * Function:CSystemVisibilityCustomClass
 *
 * Description :It is the constructor of class CSystemVisibilityCustomClass.
 *
 * Parameters :none
 *
 *
 * Returns :none
 *
 *******************************************************************
 */
CSystemVisibilityCustomClass::CSystemVisibilityCustomClass()
{
    LOG_INIT_V2(LOG_PREFIX_MOST)
}
/*
 *******************************************************************
 * Function:~CSystemVisibilityCustomClass
 *
 * Description :This is the destructor for CSystemVisibilityCustomClass class.
 *
 * Parameters :none
 *
 *
 * Returns :none
 *
 *******************************************************************
 */
CSystemVisibilityCustomClass::~CSystemVisibilityCustomClass()
{

}
/*
 *******************************************************************
 * Function:SystemVisibilityfromIBOXSlot
 *
 * Description :This function is used to emit the signal to HMI Adaptor
 *
 * Parameters :
 *     target  rw  Target
 *     mode    rw  Differnt modes
 *
 * Returns :none
 *
 *******************************************************************
 */
void CSystemVisibilityCustomClass::SystemVisibilityfromIBOXSlot(const uint target,const uint source)
{
    emit SystemVisibilityfromIBOX(target,source);
}
/*
 *******************************************************************
 * Function:SetOperation
 *
 * Description :This function is overridden from CMOSTFunctionClassArray and
 *              will be invoked on receiving Set message.
 *
 * Parameters :
 *    flags               rw   MOST flages
 *    messageID           rw   MOST Message id
 *    sourceDeviceID      rw   MOST source device id
 *    payloadLength       rw   length of the payload
 *    payload             rw   payload received in MOST message
 *
 *
 * Returns :none
 *
 *******************************************************************
 */
void CSystemVisibilityCustomClass::SetOperation(uint32_t flags, uint32_t messageID, DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
/*
    LOG_SIGNAL << " SYSTEMVISIBILITY SET: SetOperation in INVOKED " << LOG_ENDL;
    LOG_SIGNAL << " Source Device Address = " << sourceDeviceID << LOG_ENDL;
    LOG_SIGNAL << " PayloadLength = " << payloadLength << LOG_ENDL;
*/
    uint8_t  posx = 0;
    uint8_t  posy = 0;
    uint8_t  mode = 0;
    uint8_t  mode1 = 0;
    uint8_t  mode2 = 0;

    k2l::acs::base::CBigEndianStreamReader reader(payload, payloadLength);
    reader.Read8(&posx);
    reader.Read8(&posy);
    reader.Read8(&mode);
/*
    LOG_SIGNAL << " Position X : " << posx << LOG_ENDL;
    LOG_SIGNAL << " Position Y : " << posy << LOG_ENDL;
    LOG_SIGNAL << " mode : " << mode << LOG_ENDL;
*/
    if(posx == 0)
    {
        reader.Read8(&mode1);
        reader.Read8(&mode2);
/*		
        LOG_SIGNAL << " mode1 : " << mode1 << LOG_ENDL;
        LOG_SIGNAL << " mode2 : " << mode2 << LOG_ENDL;
*/    
    }
// 2014.09.29, for log, hyunyong.sung
    LOG_SIGNAL << "HMIControl" << " SystemVisibility_Set <-- : Position X = " << posx << " Position Y = "
    << posy << " mode = " << mode << " mode1 = " << mode1 << " mode2 = " << mode2 << LOG_ENDL;

    SystemVisibilityfromIBOXSlot(posx,mode);
}
/*
 *******************************************************************
 * Function:CSystemAudibilityCustomClass
 *
 * Description :It is the constructor of class CSystemAudibilityCustomClass.
 *
 * Parameters :none
 *
 *
 * Returns :none
 *
 *******************************************************************
 */
CSystemAudibilityCustomClass::CSystemAudibilityCustomClass()
{
    LOG_INIT_V2(LOG_PREFIX_MOST)
}
/*
 *******************************************************************
 * Function:~CSystemAudibilityCustomClass
 *
 * Description :This is the destructor for CSystemAudibilityCustomClass class.
 *
 * Parameters :none
 *
 *
 * Returns :none
 *
 *******************************************************************
 */
CSystemAudibilityCustomClass::~CSystemAudibilityCustomClass()
{

}
/*
 *******************************************************************
 * Function:SystemAudibilityfromIBOXSlot
 *
 * Description :This function is used to emit the signal to HMI Adaptor
 *
 * Parameters :
 *     target  rw  Target
 *     mode    rw  Differnt modes
 *
 * Returns :none
 *
 *******************************************************************
 */
void CSystemAudibilityCustomClass::SystemAudibilityfromIBOXSlot(const uint target,const uint source)
{
    emit SystemAudibilityfromIBOX(target,source);
}
/*
 *******************************************************************
 * Function:SetOperation
 *
 * Description :This function is overridden from CMOSTFunctionClassArray and
 *              will be invoked on receiving Set message.
 *
 * Parameters :
 *    flags               rw   MOST flages
 *    messageID           rw   MOST Message id
 *    sourceDeviceID      rw   MOST source device id
 *    payloadLength       rw   length of the payload
 *    payload             rw   payload received in MOST message
 *
 *
 * Returns :none
 *
 *******************************************************************
 */
void CSystemAudibilityCustomClass::SetOperation(uint32_t flags, uint32_t messageID, DeviceID_t sourceDeviceID, uint32_t payloadLength, const uint8_t *payload)
{
//    LOG_SIGNAL << " SYSTEMAUDIBILITY SET: SetOperation in INVOKED " << LOG_ENDL;
    k2l::acs::base::CBigEndianStreamReader reader(payload, payloadLength);

    uint8_t  posx = 0;
    uint8_t  posy = 0;
    uint8_t  mode = 0;
    uint8_t  mode1 = 0;
    uint8_t  mode2 = 0;
    reader.Read8(&posx);
    reader.Read8(&posy);
    reader.Read8(&mode);
/*
    LOG_SIGNAL << " Position X : " << posx << LOG_ENDL;
    LOG_SIGNAL << " Position Y : " << posy << LOG_ENDL;
    LOG_SIGNAL << " mode : " << mode << LOG_ENDL;
*/
    if(posx == 0)
    {
        reader.Read8(&mode1);
        reader.Read8(&mode2);
/*		
        LOG_SIGNAL << " mode1 : " << mode1 << LOG_ENDL;
        LOG_SIGNAL << " mode2 : " << mode2 << LOG_ENDL;
*/        
    }
// 2014.09.29, for log, hyunyong.sung
    LOG_SIGNAL << "HMIControl" << " SystemAudibility_Set <-- : Position X = " << posx << " Position Y = "
    << posy << " mode = " << mode << " mode1 = " << mode1 << " mode2 = " << mode2 << LOG_ENDL;	

    SystemAudibilityfromIBOXSlot(posx,mode);
}


