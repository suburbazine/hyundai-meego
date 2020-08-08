/*
    File:       main.cpp
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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "SendEventToApp.h"
#include "LinkController.h"
#include "CmdHandler.h"

#include "iAP2Defines.h"
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __ANDROID__
#include <android/log.h>
#endif

#ifdef IAP2SERVICE_BACKTRACE
#include <signal.h>
#include <execinfo.h>
#endif // #ifdef IAP2SERVICE_BACKTRACE

#include "iAp2ServiceLog.h"

#if defined( USE_EXTERNEL_LOG_SYSTEM )
#include "iAp2ServiceExternelLogUtils.h"
#endif

#ifdef KOECN
#define CP_DEV_MAJOR_NUM 237

#define IOCTL_CP_CTRL_PWR            _IO(CP_DEV_MAJOR_NUM, 101)
#define IOCTL_CP_CTRL_RESET          _IO(CP_DEV_MAJOR_NUM, 102)
#define IOCTL_CP_GET_VERSION         _IO(CP_DEV_MAJOR_NUM, 104)
#endif

using namespace IAP2;
static void logRedirect();

#ifdef IAP2SERVICE_BACKTRACE
static void signalConnection();
#endif // #ifdef IAP2SERVICE_BACKTRACE

BOOL CheckCarplayOnOffParam(char* CaplayOnOffParam);
int main(int argc, char *argv[])
{
	int retVal;	
	int curvalue = 0;
	BOOL CarplayOnOff = TRUE;
	void* pThreadResult = NULL;
	uint32_t err = kIap2NoErr;
	
    //(void) argc;
	//(void) argv;
#if defined( USE_EXTERNEL_LOG_SYSTEM )
		err = iAp2ServiceExternelLogWriteInit();
		if (err) iAp2ServiceExternelLogWriteStop();	
		LOGD("Print ExternelLogSystem");
#endif

#if defined( USE_IAP2_LOG_REDIRECT )
	LOGD("Print file - LogRedirect");
	logRedirect();
#endif	

	LOGI("+++++++++++++++++++ iAP2Service ++++++++++++++++");
	LOGI("##### iAP2Service VERSION : %s #####", VERSION);
#ifdef IAP2SERVICE_BACKTRACE
	signalConnection();
#endif // #ifdef IAP2SERVICE_BACKTRACE
/*
#ifdef KOECN
	LOGD("KOECN CP reset");
    unsigned int ver = 0;
    int ret = 0;
	int fd = open("/dev/lg-cp", O_RDWR);
    int checkClose = 0;

	if (fd < 0) {
		LOGE("Fail to open lg-cp");
    }
    else {
        //ioctl(fd, IOCTL_CP_CTRL_PWR, 1);
        ret = ioctl(fd, IOCTL_CP_CTRL_RESET, 0);
        if (ret < 0) {
            LOGE("RESET (DOWN) Down");
        }
        usleep(200); //in CP Spec, Reset 200us 
        ret = ioctl(fd, IOCTL_CP_CTRL_RESET, 1);
        if (ret < 0) {
            LOGE("RESET (UP) Error");
        }
        ret = ioctl(fd, IOCTL_CP_GET_VERSION, &ver);
        if (ret < 0) {
            LOGE("Get Version Error");
        }
        LOGD("cp ver=0x%02X\n", ver);
        checkClose = close(fd);
        if (checkClose < 0) {
            LOGE("Close Return : %d ", checkClose);
        }
    }
#endif
*/
	retVal = nice(-5);
	LOGD(" nice retVal[%d]", retVal);
	std::vector<int>  mSockFdVector;
	SendEventToApp mSendEventToApp(&mSockFdVector);
	LinkController miAP2LnkCtrler(&mSendEventToApp);
	CmdHandler mCmdHandler(&miAP2LnkCtrler, &mSockFdVector);	
	if(miAP2LnkCtrler.mIdentification.GetIdentificationInformation() < 0)
	{
		LOGE("Couldn't setting Identification Configuaration");		
	}
	
	for(curvalue = 1; curvalue < argc; curvalue++)
	{
		if(strcmp(argv[curvalue], "-c") == 0)
		{
            		CarplayOnOff = CheckCarplayOnOffParam(argv[curvalue + 1]);
		}	
    }
   	miAP2LnkCtrler.mConnection.CarplayOnoff = CarplayOnOff;
	miAP2LnkCtrler.createIAP2LinkRunLoopForAccessory();
#ifndef MIBGP
	miAP2LnkCtrler.mpDevMonitor = new UsbDeviceMonitor( &miAP2LnkCtrler.mLikCtrlConnCbs);
#else
	miAP2LnkCtrler.mpDevMonitor = new USBEventMonitor( &miAP2LnkCtrler.mLikCtrlConnCbs);
#endif
	if (miAP2LnkCtrler.mpDevMonitor == NULL)	{
		LOGE("ERROR : iAP2LnkCtrler.mpDevMonitor is NULL");
	}

	retVal = pthread_join(miAP2LnkCtrler.mpDevMonitor->mThread, &pThreadResult);
	
	if (retVal != 0) {
        LOGE("Thread join failed  error[%d][%s]", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}	

    LOGI("+++++++++++++++++++ exit iAP2Service ++++++++++++++++");
#if defined( USE_EXTERNEL_LOG_SYSTEM )
	iAp2ServiceExternelLogWriteStop();
#endif
	return 0;
}

BOOL CheckCarplayOnOffParam(char* CaplayOnOffParam)
{
    BOOL CarplayOnOff = TRUE;
    if(CaplayOnOffParam == NULL)
    {
        LOGW("CarplayOnOff Value NULL!!");
        LOGW("CarplayOnOff Value set TRUE");
        CarplayOnOff = TRUE;
    }
    else
    {
        if((strcmp(CaplayOnOffParam, "0") != 0) && (strcmp(CaplayOnOffParam, "1") != 0))
        {
            LOGW("CarplayOnOff Value is inValid : %s", CaplayOnOffParam);
            LOGW("CarplayOnOff Value set TRUE");
            CarplayOnOff = TRUE;
        }
        else
        {
            CarplayOnOff = atoi(CaplayOnOffParam);
            LOGD("CarplayOnOff Value is %d", CarplayOnOff);
        }
    }
    return CarplayOnOff;
}

static void logRedirect()
{
#if 1
#ifdef ANDROID
	#define LOG_FILE_NAME "/mnt/sdcard/.iAP2ServiceLog"
#elif (defined(MFI_AUTH_DEVICE_IAP2LOGPATH))
	#define LOG_FILE_NAME MFI_AUTH_DEVICE_IAP2LOGPATH	
#else
	#define LOG_FILE_NAME "/log_data/log/.iAP2ServiceLog"
#endif
#else
	#define LOG_FILE_NAME "/dev/null"
#endif
	char fileName[50];
	FILE* pFile = NULL;

    LOGD("  ==> iAP2Service log[%s]", LOG_FILE_NAME);

	snprintf(fileName, sizeof(fileName), LOG_FILE_NAME);
	pFile = fopen(fileName, "w");

	if (pFile) {
		fclose(pFile);
		pFile = freopen(fileName, "w", stdout);
#ifndef MIBGP
		pFile = freopen(fileName, "w", stderr);
#endif
	}
	else {
		LOGE("ERROR : Unable to open [%s] for writing", fileName);
	}	
}

#ifdef IAP2SERVICE_BACKTRACE
static void projectionBackTrace(int num)
{
    void *array[50];
    size_t size;
    char **strings;
    size_t i;
    FILE *fp = fopen ("/log_data/log/iAP2Service_backtrace.txt", "a+");
    fprintf(fp,"iAP2Service Backtrace Log Signal %d\n", num);
    size = backtrace(array, 50);
    strings = backtrace_symbols(array, size);

    struct tm *t;
    time_t timer;
    timer = time(NULL);
    t = localtime(&timer);

    for(i = 2; i < size; i++){
        fprintf(fp, "%d: %04d-%02d-%02d %02d:%02d:%02d %s\n", i - 2,  t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, strings[i]);
        char buffer[256];
        char strBuffer[256];
        memset(buffer, 0x0, 256);
        memset(strBuffer, 0x0, 256);
        char *start = strstr(strings[i],"[");
        char *end = strstr(strings[i],"]");
        memcpy(strBuffer,start+1,end - start-1);
        fprintf(fp,"%s\n", strBuffer);
        sprintf(buffer,"addr2line -fe /app/bin/iAP2Service %s\n", strBuffer);
        FILE *addr2line = popen(buffer, "r");
        memset(strBuffer, 0x0, 256);
        if(addr2line == NULL)
        {
            perror("error : ");
        }
        else{
            while(fgets(strBuffer, 256, addr2line)!=NULL)
                fprintf(fp,"%s", strBuffer);
            pclose(addr2line);
        }
    }

    free(strings);
    fclose(fp);
}

static void signalHandler (int signum)
{
	LOGE("	signalHandler() signum[%d]", signum);

    projectionBackTrace(signum);   // Call stack save function
    //raise(signum);
}

static void signalConnection()
{
    struct sigaction act;
    memset(&act, 0x00, sizeof(struct sigaction));

    act.sa_flags = 0;
    act.sa_flags |= SA_RESETHAND;
    act.sa_handler = signalHandler;

    for ( int signumber = SIGHUP; signumber < SIGUNUSED; signumber++ ) {
        if ( signumber == SIGKILL || signumber == SIGSTOP ) continue;

        if ( sigaction(signumber, &act, (struct sigaction*)NULL) < 0 ){
            exit(1);
        }
    }
}

#endif // #ifdef IAP2SERVICE_BACKTRACE
#ifdef __cplusplus
}
#endif

