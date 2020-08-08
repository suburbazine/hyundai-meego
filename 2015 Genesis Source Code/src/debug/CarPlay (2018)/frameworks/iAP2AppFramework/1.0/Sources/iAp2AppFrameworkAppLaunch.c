
/*
    File:       iAp2AppFrameworkAppLaunch.c
    Package:    LG iAP2 Application Framework
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

#include <stdlib.h>

#include "iAp2AppFrameworkCommon.h"
#include "iAp2AppFrameworkAppLaunch.h"
#include "iAp2AppFrameworkPrivate.h"
#include "iAp2Connection.h"
#include "iAp2Marshal.h"
#include "iAp2Log.h"


//===========================================================================================================================
//    Definitions
//===========================================================================================================================

//===========================================================================================================================
//    Declarations
//===========================================================================================================================

//===========================================================================================================================
//    iAp2AppAppLaunch
//===========================================================================================================================
int iAp2AppRequestAppLaunch(iAp2AppRef inRef, RequestAppLaunchArg* inArg)
{
    int retValue = kIap2NoErr;

    LOGD("iAp2AppRequestAppLaunch() inRef[%p]", inRef);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    if (inArg == NULL) {
        LOGE("ERROR : inArg is NULL");
        return kIap2ParamErr;
    }

    uint32_t byteNum = 0;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};

    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*(100);
    uint32_t* pIpcData = (uint32_t*) malloc(mallocSize);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

    CommandHeader* pCommandHeader = (CommandHeader* )pIpcData;

    iAp2MarshalInit(&iAp2Marshal, pIpcData, mallocSize, sizeof(CommandHeader));

    iAp2MarshalPutBytes(&iAp2Marshal, inArg->AppBundleID.value, inArg->AppBundleID.size);    
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->mLaunchAlert.bPresent);
    if(inArg->mLaunchAlert.bPresent == TRUE) {
        iAp2MarshalPutUint32(&iAp2Marshal, inArg->mLaunchAlert.eLaunchAlert);
    }
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pCommandHeader->cmd = kCmdRequestAppLaunch;
    pCommandHeader->appHandle = (uint32_t)inRef;
    pCommandHeader->dataSize = byteNum;

    //LOGD("    cmd[%u] appHandle[%p] dataSize[%u]", pIpcData[0], pIpcData[1], pIpcData[2]);

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}

