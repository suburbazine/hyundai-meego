/*
    File:       SessionConnection.h
    Package:    LG iAP2 Service
    Author:     Yongil Park (yongil.park@lge.com)
    Copyright © [2014-2016] by LG Electronics Inc.

    This program or software including the accompanying associated documentation
    (“Software”) is the proprietary software of LG Electronics Inc. and or its
    licensors, and may only be used, duplicated, modified or distributed pursuant
    to the terms and conditions of a separate written license agreement between you
    and LG Electronics Inc. (“Authorized License”). Except as set forth in an
    Authorized License, LG Electronics Inc. grants no license (express or implied),
    rights to use, or waiver of any kind with respect to the Software, and LG
    Electronics Inc. expressly reserves all rights in and to the Software and all
    intellectual property therein. If you have no Authorized License, then you have
    no rights to use the Software in any ways, and should immediately notify LG
    Electronics Inc. and discontinue all use of the Software.
*/

#ifndef SESSIONCONNECTION_H_
#define SESSIONCONNECTION_H_

#include "LinkController.h"
namespace IAP2
{
        class SessionConnection
    {
        public :


        private : 
            
        public :
            SessionConnection();
            ~SessionConnection();
            int handleConnectionCmd(int currentFd, uint32_t cmd, uint32_t handle, uint8_t* pRxData, LinkController* pLnkCtrler);

        private:            
            int reSettingUtf8Param(CmdSaveUtf8Param* utf8Param, MarshalIo* iAp2Marshal);
    };

} // namespace IAP2

#endif // #ifdef SESSIONCONNECTION_H_

