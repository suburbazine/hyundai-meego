/*
    File:       CarPlayRemoveReceiver.cpp
    Package:    LG iAP2 Service
    Author:     Jihwan Cho (jihwan.cho@lge.com)

    Copyright ¨Ï [2014-2016] by LG Electronics Inc.

    This program or software including the accompanying associated documentation
    (¡°Software¡±) is the proprietary software of LG Electronics Inc. and or its
    licensors, and may only be used, duplicated, modified or distributed pursuant
    the terms and conditions of a separate written license agreement between you
    and LG Electronics Inc. (¡°Authorized License¡±). Except as set forth in an
    Authorized License, LG Electronics Inc. grants no license (express or implied),
    rights to use, or waiver of any kind with respect to the Software, and LG
    Electronics Inc. expressly reserves all rights in and to the Software and all
    intellectual property therein. If you have no Authorized License, then you have
    no rights to use the Software in any ways, and should immediately notify LG
    Electronics Inc. and discontinue all use of the Software.
*/

#include <stdlib.h>
#include <errno.h>
#include <iostream>
#include <sstream>
#include <fcntl.h>

#include "CarPlayRemoveReceiver.h"
#include "iAp2ServiceLog.h"
#include "utils.h"
#include "SharedContext.h"

namespace IAP2
{

static void* recvThreadFunc(void* data)
{
	CarPlayRemoveReceiver* recvThread= (CarPlayRemoveReceiver*) data;
	recvThread->recvThread(data);
	return NULL;
}

CarPlayRemoveReceiver::CarPlayRemoveReceiver(UsbDeviceMonitor* mpUsbDeviceMonitor) : mTid(0), mIsTerminated(false), mbThreadCreated(false)
{
	LOGI("CarPlayRemoveReceiver()\n");
	pthread_mutex_init(&mMutexRecvThSync, NULL);
	pthread_cond_init(&mCondRecvThSync, NULL);
	
	mpDeviceMonitor = mpUsbDeviceMonitor;

	if (pthread_create(&mThread, NULL, recvThreadFunc, (void*)this) == 0) {
		mbThreadCreated = true;
	}
	else {
		LOGE("	ERROR : fail to create thread");
	}

	pthread_mutex_lock(&mMutexRecvThSync);
	pthread_cond_wait(&mCondRecvThSync, &mMutexRecvThSync);
	pthread_mutex_unlock(&mMutexRecvThSync);
}

CarPlayRemoveReceiver::~CarPlayRemoveReceiver()
{
	LOGV("~CarPlayRemoveReceiver()");

	int retVal = 0;
	void* pThreadResult = NULL;

	mIsTerminated = true;
	if (mbThreadCreated == true) {
		retVal = pthread_join(mThread, &pThreadResult);
		if (retVal != 0) {}
			//LOGW("	WARNING : Thread join failed	error[%d][%s]", errno, strerror(errno));

		mbThreadCreated = false;
	}
	pthread_cond_destroy(&mCondRecvThSync);
	pthread_mutex_destroy(&mMutexRecvThSync);
}

void* CarPlayRemoveReceiver::recvThread(void* data)
{
	int ret;
	(void) data;
	int fd = -1;
	char buf;

	mTid= getTid();
	LOGV("monitorThread() mTid[%d]", mTid);

	pthread_mutex_lock(&mMutexRecvThSync);
	pthread_cond_signal(&mCondRecvThSync);
	pthread_mutex_unlock(&mMutexRecvThSync);

	while (!mIsTerminated) {
		LOGD("recvThread");
		fd = -1;
		memset(&buf, 0, sizeof(buf));
		fd = open("/dev/mfd-iap2", O_RDONLY);

		if (fd < 0) {
			LOGE("Fail to open mfd-iap2 file");
			usleep(100*1000); // if mfd-iap2 file doesn't exist, retry too fast.
			continue;
		}
		ret = read(fd, &buf, sizeof(buf));

		LOGI("buf[%c], ret[%d]", buf, ret);
		if (ret > 0) {
			if (buf == 'X') {
				LOGD("suspend called => disconnect device");
				mpDeviceMonitor->handleRemoveCarPlay();
			}
			else if (buf == 'O') {
				LOGD("add called => add device");
				mpDeviceMonitor->handleInsertCarPlay();
			}
			else {
				LOGE("Undefined Char for tty event");
			}
		}
		
		close(fd);
		usleep(100*1000);
	}

	return NULL;
}

} //IAP2
