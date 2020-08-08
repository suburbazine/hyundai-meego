/*
    File:       SharedContext.cpp
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

#include "SharedContext.h"
#include "iAp2ServiceLog.h"

namespace IAP2
{

SharedContext* SharedContext::mInstance = NULL;

SharedContext::SharedContext() : mbRsSuccess(false), retVal(0)
{
	LOGD("SharedContext()");

	pthread_mutex_init(&mMutexRs, NULL);
	pthread_cond_init(&mCondRs, NULL);
}

SharedContext::~SharedContext()
{
	LOGD("~SharedContext()");

	pthread_cond_destroy(&mCondRs);
	pthread_mutex_destroy(&mMutexRs);	
}

SharedContext* SharedContext::getInstance()
{
	if (mInstance == NULL) {
		mInstance = new SharedContext();
	}

    return mInstance;
}


}// namespace IAP2

