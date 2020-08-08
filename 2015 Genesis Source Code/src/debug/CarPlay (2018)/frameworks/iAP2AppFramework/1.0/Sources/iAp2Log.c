/*
    File:       iAp2Log.c
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

#include "iAp2Log.h"

#ifdef MEEGO
#include "LogAssistance.h"
#endif

#include <stdio.h> 
#include <sys/time.h>
//#include "utils.h"
#include <sys/syscall.h>

#ifndef MIBGP
#define DISPLAY_TIME
#endif

#ifdef LOG_TO_FILE
extern FILE* gLogFile;
#endif // #ifdef LOG_TO_FILE

#if defined( USE_EXTERNEL_LOG_SYSTEM )
#include "iAp2ExternelLogUtils.h"
#endif
// ---------------------------------------------------------------------
// Level           Time                         PID                    Application               Tag                     Text
//    D       12-20 12:27:53.561      1413               system_process     StateMachine    handleMessage: E msg.what=131155

#ifndef ANDROID
static pid_t getTid(void)
{
    return syscall( __NR_gettid );
}  

char * getCurrentTimeString(void){
    static char timeBuff[16];
    
    struct timeval mytime;
    gettimeofday(&mytime, NULL);

    long sec_of_day = mytime.tv_sec % (24 * 60 * 60);
    int hour       = sec_of_day / (60 * 60);
    int minute       = sec_of_day % (60 * 60) / 60;
    int second       = mytime.tv_sec % 60;
    
    snprintf(timeBuff, sizeof(timeBuff), "%02d:%02d:%02d:%03ld", hour, minute, second, mytime.tv_usec/1000);
    
    return timeBuff;
}

static int sLevel = LOG_VERBOSE_IAP2;

void logPrint(int level, const char* tag, const char* fileName, int lineNumber, const char* fmt, ...) {
    #define LOG_BUF_SIZE    1024//4096// 1024

    int nStr =0;
    va_list ap;
    char buf[LOG_BUF_SIZE];

    char levelCh = 'U';
    if (level < sLevel || level > LOG_ERROR_IAP2)
        return;

    switch (level) {
        case LOG_VERBOSE_IAP2:
            levelCh = 'V';
            break;
        case LOG_DEBUG_IAP2:
            levelCh = 'D';
            break;
        case LOG_INFO_IAP2:
            levelCh = 'I';
            break;
        case LOG_WARN_IAP2:
            levelCh = 'W';
            break;
        case LOG_ERROR_IAP2:
            levelCh = 'E';
            break;
        default :
            levelCh = 'U';
            break;
    }
        

#ifdef DISPLAY_TIME
    if ( fileName != NULL ) {
        char* pFileNameOnly = (char*)strrchr(fileName, '/') + 1;
        nStr = snprintf(buf, LOG_BUF_SIZE, "%c %s %22s %5d %s [%d] ", levelCh, tag, pFileNameOnly/*fileName*/, lineNumber, getCurrentTimeString(), getTid());
    }
    else
        nStr = snprintf(buf, LOG_BUF_SIZE, "%c %s %s [%d]", levelCh, tag, getCurrentTimeString(), getTid());
#else
    if ( fileName != NULL ) {
        char* pFileNameOnly = (char*)strrchr(fileName, '/')+1;
        nStr = snprintf(buf, LOG_BUF_SIZE, "%c %s %22s %5d ", levelCh, tag, pFileNameOnly/*fileName*/, lineNumber);
    }
    else
        nStr = snprintf(buf, LOG_BUF_SIZE, "%c %s ", levelCh, tag);
#endif
    
        #ifdef WIN32
    char* pFmtTrimed = (char*)fmt;
    unsigned int lenStr = strlen(fmt);
    int is = 0;

    for(is = 0; is < lenStr; is++) {
        if(*pFmtTrimed == ' ' || *pFmtTrimed == '\t' ||  *pFmtTrimed == '\n')
            pFmtTrimed++;
        else
            break;
    }

    if(is != lenStr) {
        va_start(ap, pFmtTrimed);
        vsnprintf(&buf[nStr], LOG_BUF_SIZE- nStr - is, pFmtTrimed, ap);
        va_end(ap);
    } else {
        va_start(ap, fmt);
        vsnprintf(&buf[nStr], LOG_BUF_SIZE- nStr, fmt, ap);
        va_end(ap);
    }
        #else // #ifdef WIN32
    va_start(ap, fmt);
    vsnprintf(&buf[nStr], LOG_BUF_SIZE- nStr, fmt, ap);
    va_end(ap);
        #endif // #ifdef WIN32

    #ifdef LOG_TO_FILE
    if (gLogFile != NULL) {
        #if 0
        fprintf(stderr, "%s\n", buf);
        #else
        fprintf(gLogFile, "%s\n", buf);
        fflush(gLogFile);
        #endif
    }
    #else // #ifdef LOG_TO_FILE

    #if defined( USE_EXTERNEL_LOG_SYSTEM )
    {    

#ifdef MIBGP
#define MIBMAXLOGBUF 96
        uint32_t loopcount = 0;
        char tmp_buf[MIBMAXLOGBUF*2];
        char* pbuf = buf;
        loopcount = strlen(buf) /MIBMAXLOGBUF;
        
        while(loopcount > 0) {
            memset(tmp_buf, 0x00, MIBMAXLOGBUF*2);
            strncpy(tmp_buf, pbuf, MIBMAXLOGBUF);
            iAp2ExternelLogWriteAddQueue( level, tmp_buf, strlen(tmp_buf) + 1);
            pbuf = pbuf + MIBMAXLOGBUF;
            loopcount--;
        }
        iAp2ExternelLogWriteAddQueue( level, pbuf, strlen(pbuf) + 1);
#else
        iAp2ExternelLogWriteAddQueue( level, buf, strlen(buf) + 1);
#endif	
    }    
    #else
        fprintf(stderr, "%s\n", buf);
    #endif    
        
        

#endif // #ifdef LOG_TO_FILE

}

void setLogLevel(int level) {
    sLevel = level;
}

#endif // #ifndef ANDROID
