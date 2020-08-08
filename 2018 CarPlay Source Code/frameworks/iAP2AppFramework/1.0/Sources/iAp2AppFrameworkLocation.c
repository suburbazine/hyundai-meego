/*
    File:       iAp2AppFrameworkLocation.c
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
#include "iAp2AppFrameworkLocation.h"
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
//    iAP2AppLocationUpdate
//===========================================================================================================================
int iAP2AppLocationInformation(iAp2AppRef inRef, LocationInformationArg* inArg)
{
    #define MARSHALING_BUFFER (10000)
        int retValue = kIap2NoErr;

    LOGD("iAP2AppLocationUpdate() inRef[%p]", inRef);

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
    
    //uint32_t ipcData[512/4];
//    int mallocSize = sizeof(CommandHeader) + sizeof(uint32_t)*6;
//    uint32_t* pIpcData = (uint32_t) malloc(mallocSize);
    uint32_t* pIpcData = (uint32_t*) malloc(MARSHALING_BUFFER);
    if (pIpcData == NULL) {
        LOGE("    ERROR : malloc error. pIpcData is NULL");
        return kIap2NoMemoryErr;
    }

    iAp2MarshalInit(&iAp2Marshal, pIpcData, MARSHALING_BUFFER, sizeof(CommandHeader));

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->gpsFixData.bPresent);
    if (inArg->gpsFixData.bPresent == TRUE) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->gpsFixData.value, inArg->gpsFixData.size);
        //LOGD("inArg->gpsFixData.value : %s , size : %d", inArg->gpsFixData.value, inArg->gpsFixData.size);
    }

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->recommMinSpecificGPSTransitData.bPresent);
    if (inArg->recommMinSpecificGPSTransitData.bPresent == TRUE) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->recommMinSpecificGPSTransitData.value, inArg->recommMinSpecificGPSTransitData.size);
        //LOGD("inArg->recommMinSpecificGPSTransitData.value : %s , size : %d", inArg->recommMinSpecificGPSTransitData.value, inArg->recommMinSpecificGPSTransitData.size);
    }
    
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->gpsSatellitesInView.bPresent);
    if (inArg->gpsSatellitesInView.bPresent == TRUE) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->gpsSatellitesInView.value, inArg->gpsSatellitesInView.size);
        //LOGD("inArg->gpsSatellitesInView.value : %s , size : %d", inArg->gpsSatellitesInView.value, inArg->gpsSatellitesInView.size);
    }

    iAp2MarshalPutUint32(&iAp2Marshal, inArg->vehicleSpeedData.bPresent);
    if (inArg->vehicleSpeedData.bPresent == TRUE) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->vehicleSpeedData.value, inArg->vehicleSpeedData.size);
        //LOGD("inArg->vehicleSpeedData.value : %s , size : %d", inArg->vehicleSpeedData.value, inArg->vehicleSpeedData.size);
    }
    
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->vehicleGyroData.bPresent);
    if (inArg->vehicleGyroData.bPresent == TRUE) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->vehicleGyroData.value, inArg->vehicleGyroData.size);
        //LOGD("inArg->vehicleGyroData.value : %s , size : %d", inArg->vehicleGyroData.value, inArg->vehicleGyroData.size);
    }
    
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->vehicleAccelData.bPresent);
    if (inArg->vehicleAccelData.bPresent == TRUE) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->vehicleAccelData.value, inArg->vehicleAccelData.size);
        //LOGD("inArg->vehicleAccelData.value : %s , size : %d", inArg->vehicleAccelData.value, inArg->vehicleAccelData.size);
    }
    
    iAp2MarshalPutUint32(&iAp2Marshal, inArg->vehicleHeadingData.bPresent);
    if (inArg->vehicleHeadingData.bPresent == TRUE) {
        iAp2MarshalPutBytes(&iAp2Marshal, inArg->vehicleHeadingData.value, inArg->vehicleHeadingData.size);    
        //LOGD("inArg->vehicleHeadingData.value : %s , size : %d", inArg->vehicleHeadingData.value, inArg->vehicleHeadingData.size);
    }
    
    byteNum = iAp2MarshalGetUsedSize(&iAp2Marshal);

    pIpcData[0] = kCmdLocationInformation;
    pIpcData[1] = (uint32_t)inRef;
    pIpcData[2] = byteNum;

//    LOGD("range bGPSFixData[%d] bRecommMinSpecificGPSTransitData[%d] bGPSStatelliteInView[%d] bVehicleSpeedData[%d] bVehicleGyroData[%d] bVehicleAccelData[%d] bVehicleHeadingData[%d]", pIpcData[3], pIpcData[4], pIpcData[5], pIpcData[6], pIpcData[7], pIpcData[8],pIpcData[9]);

    retValue = iAp2ConnectionSendCmd(inRef, pIpcData, byteNum);
    free(pIpcData);
    return retValue;
}


//===========================================================================================================================
//    iAp2ConectionHandleEventStartLocationInformation
//===========================================================================================================================
int iAp2ConectionHandleEventStartLocationInformation(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventStartLocationInformation() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    StartLocationInformationCbArg cbArg;
    MarshalIo iAp2Marshal = {NULL, 0, 0, NULL, 0};
    memset(&cbArg, 0x00, sizeof(cbArg));

    iAp2MarshalInitFromTxn(&iAp2Marshal, (void*)pRxData);

    cbArg.bGPSFixData = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.bRecommMinSpecificGPSTransitData = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.bGPSStatelliteInView = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.bVehicleSpeedData = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.bVehicleGyroData = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.bVehicleAccelData = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    cbArg.bVehicleHeadingData = (BOOL)iAp2MarshalGetUint32(&iAp2Marshal);
    

    //LOGD("    bGPSFixData value[%d]", cbArg.bGPSFixData);
    //LOGD("    bRecommMinSpecificGPSTransitData value[%d]", cbArg.bRecommMinSpecificGPSTransitData);
    //LOGD("    bGPSStatelliteInView value[%d]", cbArg.bGPSStatelliteInView);
    //LOGD("    bVehicleSpeedData value[%d]", cbArg.bVehicleSpeedData);
    //LOGD("    bVehicleGyroData value[%d]", cbArg.bVehicleGyroData);
    //LOGD("    bVehicleAccelData value[%d]", cbArg.bVehicleAccelData);
    //LOGD("    bVehicleHeadingData value[%d]", cbArg.bVehicleHeadingData);

    if ((handle->delegate != NULL) && (handle->delegate->startLocationInformation_f != NULL)) {
        result = handle->delegate->startLocationInformation_f(handle->delegate->context, &cbArg);
    }
    else {
        LOGW("    WARING : delegate.startLocationInformation_f is NULL");
    }
    
    return result;
}

//===========================================================================================================================
//    iAp2ConectionHandleEventStopLocationInformation
//===========================================================================================================================
int iAp2ConectionHandleEventStopLocationInformation(iAp2AppRef inRef, unsigned char* pRxData)
{
    int result = kIap2NoErr;
    struct iAp2AppPrivate* handle = inRef;
    
    LOGD("iAp2ConectionHandleEventStopLocationInformation() inRef[0x%x] pRxData[%p]", inRef, pRxData);

    if (inRef == NULL) {
        LOGE("ERROR : inRef is NULL");
        return kIap2ParamErr;
    }

    // No Param
    
    if ((handle->delegate != NULL) && (handle->delegate->stopLocationInformation_f != NULL)) {
        result = handle->delegate->stopLocationInformation_f(handle->delegate->context);
    }
    else {
        LOGW("    WARING : delegate.stopLocationInformation_f is NULL");
    }
    
    return result;
}




