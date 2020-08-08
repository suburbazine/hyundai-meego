#define LOG_TAG "LG_TSE" // for android log sys
#define LOG_NDEBUG 0

/*
 *    @file        <tseFilter.h>
 *    @brief       Filter API of LG Electronics Preprocessor
 *    @version     1.0.0
 *    @date        2015-02-11  1.0.0 initial version
 *    @author      speech team
 */

#ifdef __ANDROID_API__
#include "utils/Log.h"
#include <cutils/properties.h>
#else
#include "properties.h"
#endif
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>

#ifdef TSE_LIB_DYNAMIC_LOAD_ENABLE
  #include <dlfcn.h>
#endif

#include "tseFilter.h"
#include "tseFilter_priv.h"
#include "tseFilterDebug.h"
#include "tseFilterDebugAPI.h"

#define STRING_CARPLAY "CarPlay"
#define STRING_BLUETOOTH "Bluetooth"
#define STRING_SIRI "Siri"
#define STRING_TELEMATICS "Telematics" 
#define STRING_SPEECH_RECOG "SpeechRecog"

#define DEBUG_API_ENABLE_ENGINEER_DEBUG  "lgtse.EngineerDebug" // enable engineering mode debug set
#define DFTCFG_API_ENABLE_ENGINEER_DEBUG "0"

#define DEBUG_API_ENABLE_DEBUG  "lgtse.debuggable" // enable debug mode for tuning tool connection
#define DFTCFG_API_ENABLE_DEBUG "0"
#define DEBUG_API_USER_TSF_NAME "lgtse.tsfName"  // user defined TSF name
#define DFTCFG_API_USER_TSF_NAME "%s_%dhz.tsf" // default TSF format ex) CarPlay_16000hz.tsf
#define DEBUG_API_USER_TSF_PATH "lgtse.tsfPath"    // user defined TSF path
#define DFTCFG_API_USER_TSF_PATH TSF_FILE_PATH      // default TSF path ("/etc/tse")

//#define DL_TEST // test dynamic loading

#ifndef TSE_LIB_DYNAMIC_LOAD_ENABLE // DL not supported filter case
int (*_pTseCreate)(ttseMain **Main) = &tseCreate;
int (*_pTseInitialize)(ttseMain *pMain, const ttseConfigData *pConfigData) = &tseInitialize;
int (*_pTseProcess)(ttseMain* pMain,
				const ttseAudio MicIn,
				const ttseAudio RecvIn,
				const ttseAudio RefIn,
				ttseAudio MicOut,
				ttseAudio RecvOut,
				ttseVolumeInfo * pVolumeInfo,
				ttseDebug *pDebugData ) = &tseProcess;
int (*_pTseDestroy)(ttseMain **pMain ) = &tseDestroy;
int (*_pTseReset)(ttseMain *pMain ) = &tseReset;
int (*_pTseSetData)(ttseMain *pMain,
				const ttseDataID DataID,
				const int iChannel,
				const unsigned int iSize,
				const void *pData ) = &tseSetData;
int (*_pTseGetData)(ttseMain *pMain,
				const ttseDataID DataID,
				const unsigned int iChannel,
				unsigned int *pSize,
				void *pData ) = &tseGetData;
int (*_pTseGetErrorMessage)(const ttseMain *pMain, 
						char *pErrorMessage) = &tseGetErrorMessage;
void (*_pTseGetVersion)(unsigned int *pArrayLen,
					const int **pVersionArray,
					const char **pVersionString,
					const char **pVersionComment ) = &tseGetVersion;
int (*_pTseLoadTSFtoConfig)(ttseConfigData *pConfigData, char *strTSFpath) = &tseLoadTSFtoConfig;
int (*_pTseCheckTsf)(ttseMain* pMain, const ttseConfigData* pConfigData) = &tseCheckTsf;

#else // DL supported filter case
int (*_pTseCreate)(ttseMain **Main);
int (*_pTseInitialize)(ttseMain *pMain, const ttseConfigData *pConfigData);
int (*_pTseProcess)(ttseMain* pMain,
				const ttseAudio MicIn,
				const ttseAudio RecvIn,
				const ttseAudio RefIn,
				ttseAudio MicOut,
				ttseAudio RecvOut,
				ttseVolumeInfo * pVolumeInfo,
				ttseDebug *pDebugData );
int (*_pTseDestroy)(ttseMain **pMain );
int (*_pTseReset)(ttseMain *pMain );
int (*_pTseSetData)(ttseMain *pMain,
				const ttseDataID DataID,
				const int iChannel,
				const unsigned int iSize,
				const void *pData );
int (*_pTseGetData)(ttseMain *pMain,
				const ttseDataID DataID,
				const unsigned int iChannel,
				unsigned int *pSize,
				void *pData );
int (*_pTseGetErrorMessage)(const ttseMain *pMain, 
						char *pErrorMessage);
void (*_pTseGetVersion)(unsigned int *pArrayLen,
					const int **pVersionArray,
					const char **pVersionString,
					const char **pVersionComment );
int (*_pTseLoadTSFtoConfig)(ttseConfigData *pConfigData, char *strTSFpath);
int (*_pTseCheckTsf)(ttseMain* pMain, const ttseConfigData* pConfigData);

#endif


/** @brief get prefix string for TSF file name
 *
 *  @param mode input mode.
 *  @return prefix string array.
 *  @remark 주의사항 없음
 */
static char *_getTsfPrefixString(int mode)
{
	switch (mode)
	{
		case CARPLAY_TELEPHONY_MODE :
			return (char*)STRING_CARPLAY;
			break;
		//case CARPLAY_FACETIME_MODE : //not used -> carplay
		//	return (char*)"FaceTime";
		//	break;
		case HANDSFREE_BLUETOOTH_MODE :
			return (char*)STRING_BLUETOOTH;
			break;
		case CARPLAY_SIRI_MODE :
			return (char*)STRING_SIRI;
			break;
		case TELEMATICS_MODE :
			return (char*)STRING_TELEMATICS;
			break;
		case SPEECH_RECOG_MODE :
			return (char*)STRING_SPEECH_RECOG;
			break;
		default :
			ALOGD("not supported mode. \n");
			return ""; // return null string
			break;
	}
}

/** @brief get TSF file name from user defined system registery(or system property) setting
 *
 *  @param output pParameter format.
 *  @return Void.
 *  @remark 주의사항 없음
 */
static void _getParameterTsfFormat(char *pParameter)
{
#ifdef WIN32
	// windows target
	ALOGD("windows target \r\n");
	sprintf(pParameter, "%s", DFTCFG_API_USER_TSF_NAME);
	// NA
#else
	// normal linux or android target
	ALOGD("normal linux or android target \r\n");
	property_get (DEBUG_API_USER_TSF_NAME, pParameter, DFTCFG_API_USER_TSF_NAME);
#endif
}

/** @brief 입력되는 conf설정에 해당되는 TSF설정파일을 ConfigData buffer로 읽어들인다
 *
 *  @param ctx filter context structure handle
 *  @param pConfigData output config data buffer structure.
 *  @param conf input configuration from API layer.
 *  @return Void.
 *  @remark 주의사항 없음
 */
static int _loadConfigurationToBuffer(FilterContext *ctx, ttseConfigData *pConfigData, LGTSE_Config *conf)
{
	char tsfFileFullPath[2048]={0,};
	FILE* fd = 0;
	char strTSFname[512]={0,};
	int size = 0;
	int read_size = 0;
	char *readBuffer = NULL;
	char parameterFormat[128]={0,};
	int res = TSE_ERR_OK;
	char debug_property[2048] = {0,};


	_getParameterTsfFormat(parameterFormat);
	snprintf (strTSFname, sizeof(strTSFname), (const char*)parameterFormat, _getTsfPrefixString(conf->operationMode), conf->sampleRate); //fix 20150421 DefectId=97444
	ALOGD( "strTSFname:%s \r\n", strTSFname);

#ifdef __ANDROID_API__
	if(ctx->engineerDebugMode == 1) // engineer debug mode enabled
	{
		ALOGD("lgtse.debuggable not found! \n");

		FILE *fpTest = NULL;
		if ((fpTest=fopen("/mnt/sdcard/lgtse.tsfPath", "r"))!=NULL)
		{
			//sprintf(debug_property, "%s", "1");
			ALOGD("/mnt/sdcard/lgtse.tsfPath file exist! using SD card's tsf path \n");
			fclose(fpTest);

			sprintf(tsfFileFullPath,"%s/%s","/mnt/sdcard/tsf", strTSFname);
			goto Skip_User_TSF_Path_Set;
		}
		else
		{
			ALOGD("/mnt/sdcard/lgtse.tsfPath file not found! \n");
		}
	}
#endif

	property_get(DEBUG_API_USER_TSF_PATH, debug_property, DFTCFG_API_USER_TSF_PATH);
	if (strncmp(debug_property, DFTCFG_API_USER_TSF_PATH, strlen(DFTCFG_API_USER_TSF_PATH)) == 0)
	{
		// not set
		if(strlen(ctx->tsfPath)>0){
			ALOGD(">>> Manual TSF path setting API was called. <<< \r\n");
			ALOGD("ctx->tsfPath:%s \r\n", ctx->tsfPath); // set from LGTES_SetTsfPath
			sprintf(tsfFileFullPath,"%s/%s",ctx->tsfPath, strTSFname);
		}
		else
		{
			ALOGD("TSF_FILE_PATH:%s \r\n", TSF_FILE_PATH); // exported from makefile
			sprintf(tsfFileFullPath,"%s/%s",TSF_FILE_PATH, strTSFname);
		}
	}
	else
	{
		// set user defined tsf path
		ALOGD("user defined TSF file path apply.. \r\n");
		ALOGD("debug_property:%s \r\n", debug_property); // exported from makefile
		sprintf(tsfFileFullPath,"%s/%s",debug_property, strTSFname);
	}
#ifdef __ANDROID_API__
Skip_User_TSF_Path_Set:
#endif
	ALOGD( "load tsf setting file:%s \r\n", tsfFileFullPath);
	_pTseSetData(ctx->ptseMain, TSE_TSF_PATH_STR, 0, strlen(tsfFileFullPath), (void*)&tsfFileFullPath);

	fd = fopen (tsfFileFullPath, "r");
	if (fd)
	{
		fseek (fd, 0, SEEK_END);
		size = (unsigned long)ftell(fd);
		if (size > 0)
		{
			fseek(fd, 0L, SEEK_SET);
			readBuffer = (char *) malloc (size);
			if(readBuffer == NULL)
			{
				ALOGE( "ERROR. allocation memory!!! \n"); //fix 20150421 DefectId=97414
				return TSE_ERR_NULL_POINTER;
			}
			else
				read_size = fread (readBuffer, 1, size, fd);
		}
		else
		{
			ALOGE( "Open tsf:  size 0!!!! \n");
			if(fd)fclose(fd);
			return TSE_WARN_TSF_FILE_OPEN_FAILED;
		}
		if(fd)fclose(fd);
	}
	else
	{
		 ALOGE( "Open tsf: FAIL!!!! \n");
		 return TSE_WARN_TSF_FILE_OPEN_FAILED;
	}
	pConfigData->ConfigBuffer[0] = readBuffer;
	pConfigData->ConfigBufferSize[0] = read_size;
	pConfigData->ConfigBufferCnt = 1;

	return res;
}

/** @brief get tse library full path from tsf_dir(with version_info.txt) and tse_lib_dir. 
 *
 *  @param pTsfPath TSF source directory path. 
 *  @param pTseLibPath tse library directory path.
 *  @return success: target version(printed in version_info.txt) tse library path return, error: NULL return.
 *  @remark 주의사항 없음
 */
const char *LGTSE_GetLibraryFullPath (char *pTsfPath, char *pTseLibPath)
{
#ifdef TSE_LIB_DYNAMIC_LOAD_ENABLE
	FILE *fd = NULL;
	char versionFilePath[2048] = {0,};
	char libtseFilePath[2048] = {0,};
	char *readBuffer;
	int size;
	// validation path
	if(pTseLibPath == NULL || pTseLibPath == NULL)
	{
		ALOGE("Failed to set tse debugging mode.\r\n");
		return NULL;
	}
	sprintf(versionFilePath,"%s/%s",pTsfPath, VERSION_FILE_NAME);
	ALOGD( "versionFilePath:%s \r\n", versionFilePath);
	fd = fopen (versionFilePath, "r");
	if (fd)
	{
		fseek (fd, 0, SEEK_END);
		size = (unsigned long)ftell(fd);
		if (size > 0)
		{
			fseek(fd, 0L, SEEK_SET);
			readBuffer = (char *) malloc (1024);
			if(readBuffer == NULL)
			{
				ALOGE( "ERROR. allocation memory!!! \n"); //fix 20150421 DefectId=97414
				return NULL;
			}
			else
			{
				int ch = getc(fd);
    			int count = 0;
    			ALOGD( "read version file \r\n");
				//read_size = fread (readBuffer, 1, size, fd);
				while ((ch != '\n') && (ch != EOF))
				{
					if (count == 1024-1)
					{
						break;
					}
					readBuffer[count] = (char)ch;
					count++;
					ch = getc(fd);
				}
				readBuffer[count] = '\0';
				ALOGD( "readBuffer:%s \r\n", readBuffer);
				sprintf(libtseFilePath, LIB_NAME_WITH_VER_FORMAT, pTseLibPath, readBuffer);
				ALOGD( "libtseFilePath:%s \r\n", libtseFilePath);
				char line[strlen(libtseFilePath)+1];
				strncpy(line, libtseFilePath, (strlen(libtseFilePath) + 1));
				free(readBuffer);
				ALOGD( "line:%s \r\n", line);
				const char *constLine = line;
				if(fd)fclose(fd);
				return constLine;
			}
		}
		else
		{
			ALOGE( "Open version file:  size 0!!!! \n");
			if(fd)fclose(fd);
			return NULL;
		}
		if(fd)fclose(fd);
	}
	else
	{
		 ALOGE( "Open version file: FAIL!!!! \n");
		 return NULL;
	}
	//read version_info.txt file and get libtse version
#else
	ALOGD("%s Dynamic Loading not supported! \r\n", __FUNCTION__);
	return NULL;
#endif
}

void *_gDlHandle = NULL;

/** @brief load dynamic library(libtse_x.x.x.so) and take function handles in so library
 *
 *  @param pLibraryFullPath dynamic loading target (libtse_x.x.x.so) path 
 *  @return 0 for succes, other for errors.
 *  @remark 주의사항 없음
 */
int LGTSE_StartDynamicLoading (const char *pLibraryFullPath)
{
#ifdef TSE_LIB_DYNAMIC_LOAD_ENABLE
	char *error = NULL;
	ALOGD( "+++dlpoen:%s \r\n", pLibraryFullPath);
	_gDlHandle = dlopen (pLibraryFullPath, RTLD_LAZY);
	if(!_gDlHandle)
	{
		ALOGE("%s\r\n", dlerror());
		return TSE_ERR_ERR;
	}
	_pTseCreate = dlsym(_gDlHandle, "tseCreate");
	if((error = (char *)dlerror()) != NULL)
	{
		ALOGE("%s\r\n", error);
		return TSE_ERR_ERR; 
	}
	_pTseInitialize = dlsym(_gDlHandle, "tseInitialize");
	if((error = (char *)dlerror()) != NULL)
	{
		ALOGE("%s\r\n", error);
		return TSE_ERR_ERR; 
	}
	_pTseProcess = dlsym(_gDlHandle, "tseProcess");
	if((error = (char *)dlerror()) != NULL)
	{
		ALOGE("%s\r\n", error);
		return TSE_ERR_ERR; 
	}
	_pTseDestroy = dlsym(_gDlHandle, "tseDestroy");
	if((error = (char *)dlerror()) != NULL)
	{
		ALOGE("%s\r\n", error);
		return TSE_ERR_ERR; 
	}
	_pTseReset = dlsym(_gDlHandle, "tseReset");
	if((error = (char *)dlerror()) != NULL)
	{
		ALOGE("%s\r\n", error);
		return TSE_ERR_ERR; 
	}
	_pTseSetData = dlsym(_gDlHandle, "tseSetData");
	if((error = (char *)dlerror()) != NULL)
	{
		ALOGE("%s\r\n", error);
		return TSE_ERR_ERR; 
	}
	_pTseGetData = dlsym(_gDlHandle, "tseGetData");
	if((error = (char *)dlerror()) != NULL)
	{
		ALOGE("%s\r\n", error);
		return TSE_ERR_ERR; 
	}
	_pTseGetErrorMessage = dlsym(_gDlHandle, "tseGetErrorMessage");
	if((error = (char *)dlerror()) != NULL)
	{
		ALOGE("%s\r\n", error);
		return TSE_ERR_ERR; 
	}
	_pTseGetVersion = dlsym(_gDlHandle, "tseGetVersion");
	if((error = (char *)dlerror()) != NULL)
	{
		ALOGE("%s\r\n", error);
		return TSE_ERR_ERR; 
	}
	_pTseLoadTSFtoConfig = dlsym(_gDlHandle, "tseLoadTSFtoConfig");
	if((error = (char *)dlerror()) != NULL)
	{
		ALOGE("%s\r\n", error);
		return TSE_ERR_ERR; 
	}
	_pTseCheckTsf = dlsym(_gDlHandle, "tseCheckTsf");
	if((error = (char *)dlerror()) != NULL)
	{
		ALOGE("%s\r\n", error);
		return TSE_ERR_ERR; 
	}
	ALOGD( "+++dlpoen:%s [OK] \r\n", pLibraryFullPath);
	return TSE_ERR_OK;
#else
	ALOGD("Dynamic Loading not supported! \r\n");
	return TSE_ERR_ERR;
#endif
}

/** @brief unload dynamic library
 *
 *  @param pLibraryFullPath dynamic loading target (libtse_x.x.x.so) path 
 *  @return 0 for succes, other for errors.
 *  @remark 주의사항 없음
 */
int LGTSE_FinishDynamicLoading (void)
{
#ifdef TSE_LIB_DYNAMIC_LOAD_ENABLE
	ALOGD( "+++dlclose \r\n");
	dlclose(_gDlHandle);
	return TSE_ERR_OK;
#else
	ALOGD("Dynamic Loading not supported! \r\n");
	return TSE_ERR_ERR;
#endif
}


/** @brief create filter handle
 *
 *  @return filter context pointer, NULL for error.
 *  @remark 주의사항 없음
 */
void * LGTSE_FilterCreate (void)
{
	FilterContext *ctx;
	char debug_property[PROPERTY_VALUE_MAX] = {0,};
	char debug_property_eng[PROPERTY_VALUE_MAX] = {0,};

#ifdef DL_TEST
{
	printf(" \t test dynamic linking mode .....\r\n");
	const char *res = LGTSE_GetLibraryFullPath ("/etc/tse", "/etc/tse/lib");
	LGTSE_StartDynamicLoading (res);	
}
	//LGTSE_FinishDynamicLoading ();
#endif

	if(_pTseCreate == NULL)
	{
		ALOGE("Error _pTseCreate function not loaded!!! check LGTSE_StartDynamicLoading() result! \r\n");
		return NULL;
	}

	ctx = (FilterContext *) calloc (1, sizeof(FilterContext));
	if (ctx == NULL)
		goto error;

#ifdef __ANDROID_API__
	property_get(DEBUG_API_ENABLE_ENGINEER_DEBUG, debug_property_eng, DFTCFG_API_ENABLE_ENGINEER_DEBUG);
#endif
	property_get(DEBUG_API_ENABLE_DEBUG, debug_property, DFTCFG_API_ENABLE_DEBUG);
	if(strncmp(debug_property_eng, "1", 1) == 0)
	{
		ALOGD("engineerging debug mode enabled.\r\n");
		ctx->engineerDebugMode = 1;
	}
	if (strncmp(debug_property, "1", 1) == 0 || strncmp(debug_property_eng, "1", 1) == 0)
	{
		if (setupDebugMode(ctx) == iTRUE)
		{
			ALOGD("tse debugging mode enabled.\r\n");
		}
		else
		{
			ALOGE("Failed to set tse debugging mode.\r\n");
		}
	}
	else
	{
		ALOGD("normal mode skip setupDebugMode().\r\n");
	}

	if (_pTseCreate(&(ctx->ptseMain)) != TSE_ERR_OK) 
	{
		ALOGE("tse Create failed.\r\n");
		goto error;
	}
	return ctx;

error:
	if (ctx != NULL)
	{
		if (ctx->pMemory != NULL)
			free (ctx->pMemory);
		if(ctx->debugger != NULL)//fix 20150421 DefectId=97034
		{
			unsetDebugMode(ctx);
			//free (ctx->debugger);
		}
		free (ctx);
	}
	return NULL;
}

/** @brief destroy filter
 *
 *  @param handle filter handle for destroy.
 *  @return Void.
 *  @remark 주의사항 없음
 */
void LGTSE_FilterDestroy (void *handle)
{
	FilterContext *ctx = (FilterContext *) handle;
	ALOGD("LGTSE_FilterDestroy \n");
	if (ctx != NULL)
	{
		unsetDebugMode(ctx);
		if(_pTseDestroy == NULL)
		{
			ALOGE("Error _pTseDestroy function not loaded!!! check LGTSE_StartDynamicLoading() result!\r\n");
		}
		else
			_pTseDestroy (&ctx->ptseMain);
		//freeConfigData (ctx);
		if (ctx->dummyRecvIn != NULL)
			free (ctx->dummyRecvIn);
		if (ctx->pMemory != NULL)			
			free (ctx->pMemory);
		free (ctx);
	}
	ALOGD("LGTSE_FilterDestroy finish.\n");
	tseFilter_log_deinit();
#ifdef DL_TEST
	LGTSE_FinishDynamicLoading ();
#endif
}

/** @brief set parameter values in Tse engine
 *
 *  @param ctx filter context handle.
 *  @param conf filter input configurations.
 *  @return 0 for succes, other for errors.
 *  @remark 주의사항 없음
 */
static int LGTSE_SetData (FilterContext *ctx, LGTSE_Config *conf)
{
	int step = 0;
	int res = TSE_ERR_OK; //fix 20150421 DefectId=97037
	char errorMsg[TSE_MAX_STRING_SIZE];
//__printDbg(ctx, __FUNCTION__, __LINE__);
	ALOGD("LGTSE_SetData \r\n");
	ALOGD("conf->operationMode = %d \r\n", conf->operationMode);
	if(_pTseSetData == NULL)
	{
		ALOGE("Error _pTseSetData function not loaded.\r\n");
		res = TSE_ERR_NULL_POINTER;
		goto finish;
	}
	res = _pTseSetData(ctx->ptseMain, TSE_OPERATION_MODE, 0, sizeof(conf->operationMode),
		(void*) &conf->operationMode);
	step++;
	if (TSE_ERR_OK != res)
	{
		goto finish;
	}
	ALOGD("conf->frameSize = %d \r\n", conf->frameSize);
	res = _pTseSetData(ctx->ptseMain, TSE_FRAME_SHIFT, 0, sizeof(conf->frameSize),
		(void*) &conf->frameSize);
	step++;
	if (TSE_ERR_OK != res)
	{
		goto finish;
	}
	res = _pTseSetData(ctx->ptseMain, TSE_SAMPLERATE, 0, sizeof(conf->sampleRate),
		(void*) &conf->sampleRate);
	step++;
	if (TSE_ERR_OK != res)
	{
		goto finish;
	}
	/*
	res = _pTseSetData(ctx->ptseMain, TSE_SEND_IN_SIGNAL_NUM, 0, sizeof(conf->micChannel),
		(void*) &conf->micChannel);
	step++;
	if (TSE_ERR_OK != res)
	{
		goto finish;
	}
	res = _pTseSetData(ctx->ptseMain, TSE_RECV_IN_SIGNAL_NUM, 0, sizeof(conf->recvChannel),
		(void*) &conf->recvChannel);
	step++;
	if (TSE_ERR_OK != res)
	{
		goto finish;
	}
	res = _pTseSetData(ctx->ptseMain, TSE_REF_IN_SIGNAL_NUM, 0, sizeof(conf->refChannel),
		(void*) &conf->refChannel);
	step++;
	*/
	if (TSE_ERR_OK != res)
	{
		goto finish;
	}
	finish:
	if (TSE_ERR_OK != res)
	{
		if(_pTseGetErrorMessage != NULL)
			_pTseGetErrorMessage(ctx->ptseMain, errorMsg);
		ALOGD("tseSetData Failed - %s, step - %d \r\n", errorMsg, step);
	}
	return res;
}

/** @brief TSF validation check API
 *
 *  @param mode input speech audio mode.
 *  @param samplerate samplerate of audio.
 *  @param path TSF directory path.
 *  @return 0 for succes, other for errors(0x0101:TSF_BUFFER_NULL,0x0102:TSF_CONVERT_FAILED,0x0103:TSF_CRC_CHECK_FAILED,0x0104:TSF_DECODE_FAILED,0x0105:TSF_VERSION_MISMATCH,0x0106:TSF_FILE_OPEN_FAILED)
 *  @remark 주의사항 없음
 */
int LGTSE_CheckTsf (int mode, int samplerate, const char *path)
{
	int result = TSE_ERR_OK;
	void *FilterHandle = NULL;
	FilterContext *ctx = NULL;
	ttseConfigData ConfigData;
	LGTSE_Config conf;

	conf.operationMode = (short int)mode;
	conf.frameSize = (short int)128;
	conf.sampleRate = (short int)samplerate;
	conf.micChannel = (short int)1;
	conf.recvChannel = 1;//(short int)channels;
	conf.refChannel = 0;

	ALOGD("setting values from LGTSE_CheckTsf API argument \r\n");
	ALOGD("operation mode= %d \r\n",conf.operationMode);
	ALOGD("frameSize= %d \r\n",conf.frameSize);
	ALOGD("sampleRate= %d \r\n",conf.sampleRate);
	ALOGD("micChannel= %d \r\n",conf.micChannel);
	ALOGD("recvChannel= %d \r\n",conf.recvChannel);
	ALOGD("refChannel= %d \r\n",conf.refChannel);
	ALOGD("setting values from LGTSE_CheckTsf API argument \r\n");

	//if (&ConfigData != NULL)
	{
		int i;
		for (i = 0; i < ConfigData.ConfigBufferCnt; i ++)
		{
			if(ConfigData.ConfigBuffer[i] != NULL)
			{
				free (ConfigData.ConfigBuffer[i]);
			}
		}
	}

	FilterHandle = LGTSE_FilterCreate ();
	if(FilterHandle != NULL)
	{
		ALOGD("create handle ok \r\n");

		ctx = (FilterContext *) FilterHandle;		
		ALOGD("LGTSE_SetTsfPath \r\n");
		LGTSE_SetTsfPath(FilterHandle, path);
		ALOGD("_loadConfigurationToBuffer \r\n");
		result = _loadConfigurationToBuffer(ctx, &ConfigData, &conf);
		if(result != TSE_ERR_OK)
		{
			ALOGE("open tsf Failed!!! \r\n");
			LGTSE_FilterDestroy(FilterHandle);
			return result;
		}
		if(_pTseCheckTsf == NULL)
		{
			ALOGE("Error _pTseCheckTsf function not loaded.\r\n");
			result = TSE_ERR_NULL_POINTER;
		}
		else
			result = _pTseCheckTsf(ctx->ptseMain, (const ttseConfigData*)&ConfigData);
		//if (&ConfigData != NULL)
		{
			int i;
			for (i = 0; i < ConfigData.ConfigBufferCnt; i ++)
			{
				if(ConfigData.ConfigBuffer[i] != NULL)
				{
					free (ConfigData.ConfigBuffer[i]);
				}
			}
		}
		//result = _pTseCheckTsf (FilterHandle, mode, samplerate, 1, 128);
		if(result == TSE_ERR_OK){
			ALOGD("TSF check OK \r\n");
		}
		else
		{
			ALOGD("TSF check Failed! \r\n");
		}
		
		LGTSE_FilterDestroy(FilterHandle);
	}
	else
	{
		return TSE_ERR_NOT_CREATED;
	}
	return result;
}
/** @brief manual setup TSF file directory
 *
 *  @param handle filter handle for path setting.
 *  @param path directory path.
 *  @return 0 for succes, other for errors.
 *  @remark 해당 API 호출이 없을경우 makefile의 TSF_FILE_PATH 값이 기본 적용된다.
 */
int LGTSE_SetTsfPath (void *handle, const char *path)
{
	FilterContext *ctx = NULL;
	int res = TSE_ERR_OK;

	if(path == NULL || strlen(path)<=0 || handle == NULL)
		return TSE_ERR_NULL_POINTER;
	ctx = (FilterContext *) handle;

	strcpy(ctx->tsfPath, path);
	ALOGD("TSF path saved = %s \r\n", ctx->tsfPath);

	return res;
}

/** @brief initialize filter + initialize Tse engine
 *
 *  @param handle filter handle for initialize.
 *  @param mode input speech audio mode.
 *  @param samplerate samplerate of audio.
 *  @param channels total channels for processing.
 *  @param frameSize frame size of audio.
 *  @return 0 for succes, other for errors(0x0101:TSF_BUFFER_NULL,0x0102:TSF_CONVERT_FAILED,0x0103:TSF_CRC_CHECK_FAILED,0x0104:TSF_DECODE_FAILED,0x0105:TSF_VERSION_MISMATCH,0x0106:TSF_FILE_OPEN_FAILED)
 *  @remark 주의사항 없음
 */
int LGTSE_FilterInit (void *handle, int mode, int samplerate, int channels, int frameSize)
{
	FilterContext *ctx = (FilterContext *) handle;
	LGTSE_Config conf;
	int res = TSE_ERR_OK; //fix 20150421 DefectId=97038
	char errorMsg[TSE_MAX_STRING_SIZE]={0,};

	ttseConfigData ConfigData;

	if(handle == NULL)
		return TSE_ERR_NULL_POINTER;

	if(_pTseInitialize == NULL)
	{
		ALOGE("Error _pTseInitialize function not loaded!!! check LGTSE_StartDynamicLoading() result!\r\n");
		return TSE_ERR_NULL_POINTER;
	}
	if(_pTseSetData == NULL)
	{
		ALOGE("Error _pTseSetData function not loaded!!! check LGTSE_StartDynamicLoading() result!\r\n");
		return TSE_ERR_NULL_POINTER;
	}

	memset(&ConfigData, 0, sizeof(ttseConfigData));

	ctx->frameCount = frameSize;
	ctx->channels = channels;

	// samplerate validation
	if(mode == CARPLAY_SIRI_MODE) // Siri mode
	{
		if(samplerate != 24000)
		{
			ALOGD("input TSE_SampleRate: %d \r\n", samplerate);
			ALOGD("Siri sampleRate invalid!!![%d] set forced samplate 24000! \r\n", samplerate);
			snprintf(errorMsg, TSE_MAX_STRING_SIZE, "Siri sampleRate invalid!!![%d] set forced samplate 24000! \r\n", samplerate);
			_pTseSetData(ctx->ptseMain, TSE_FILTER_LOG, 0, strlen(errorMsg), (void*)&errorMsg);
			samplerate = 24000;
		}
		else
		{
			ALOGD("input TSE_SampleRate: %d [check Ok] \r\n", samplerate);
		}
	}
	conf.operationMode = (short int)mode;
	conf.frameSize = (short int)frameSize;
	conf.sampleRate = (short int)samplerate;
	conf.micChannel = (short int)channels;
	conf.recvChannel = 1;//(short int)channels;
	conf.refChannel = 0;

	ctx->dummyRecvIn = calloc (1, frameSize * channels * sizeof (short));

	ALOGD("setting values from LGTSE_FilterInit API argument \r\n");
	ALOGD("operation mode= %d \r\n",conf.operationMode);
	ALOGD("frameSize= %d \r\n",conf.frameSize);
	ALOGD("sampleRate= %d \r\n",conf.sampleRate);
	ALOGD("micChannel= %d \r\n",conf.micChannel);
	ALOGD("recvChannel= %d \r\n",conf.recvChannel);
	ALOGD("refChannel= %d \r\n",conf.refChannel);
	ALOGD("setting values from LGTSE_FilterInit API argument \r\n");

	res = LGTSE_SetData (ctx, &conf);
	if (TSE_ERR_OK != res)
	{
		ALOGD("tseSetData Failed!\n");
		goto finish;
	}

	//if (&ConfigData != NULL)
	{
		int i;
		for (i = 0; i < ConfigData.ConfigBufferCnt; i ++)
		{
			if(ConfigData.ConfigBuffer[i] != NULL)
			{
				free (ConfigData.ConfigBuffer[i]);
			}
		}
	}

	res = _loadConfigurationToBuffer(ctx, &ConfigData, &conf);
	if(res != TSE_ERR_OK)
	{
		ALOGE("open tsf Failed!!! \r\n");
	}

	res = _pTseInitialize(ctx->ptseMain, (const ttseConfigData*)&ConfigData);

	//if (&ConfigData != NULL)
	{
		int i;
		for (i = 0; i < ConfigData.ConfigBufferCnt; i ++)
		{
			if(ConfigData.ConfigBuffer[i] != NULL)
			{
				free (ConfigData.ConfigBuffer[i]);
			}
		}
	}

	if (TSE_ERR_OK != res)
	{
		goto finish;
	}

	finish:
	if (TSE_ERR_OK != res)
	{
		_pTseGetErrorMessage(ctx->ptseMain, errorMsg);
		ALOGD("tseFilterInit Failed - %s \r\n", errorMsg);
	}

	//if(res > TSE_WARN_START && res < TSE_WARN_END) // filtering warning messages
	//	return TSE_ERR_OK;

	return res;// != TSE_ERR_OK;
}

/** @brief processing audio frames with Tse engine
 *
 *  @param pHandle handle for processing.
 *  @param pMicIn input MIC pcm sound frame data array.
 *  @param pMicOut output processed MIC pcm sound frame data array.
 *  @param pRecvIn input Receive-in pcm sound frame data array.
 *  @param pRecvOut output processed Receive-in pcm sound frame data array.
 *  @param pVolInfo current system's volume information.
 *  @return 0 for succes, other for errors.
 *  @remark 주의사항 없음
 */
int LGTSE_FilterProcess (void *pHandle, char *pMicIn, char *pMicOut, char *pRecvIn, char *pRecvOut, unsigned int *pVolInfo)
{
	FilterContext *ctx = (FilterContext *) pHandle;
	char errorMsg[TSE_MAX_STRING_SIZE];
	int res = TSE_ERR_OK;
	int iTseDebugInBytesAvailable = 0;
	char pTseDebugBufferIn[iDEBUG_BUFFER_IN_MAXSIZE] = {0,};
	int iTseDebugOutBytesAvailable = 0;
	char pTseDebugBufferOut[iDEBUG_BUFFER_OUT_MAXSIZE] = {0,}; // fix 20150421 DefectId=97521
	int iTxRingbufferInserted;
	ttseDebug *stseDebugBuffer = NULL;
	tsShared * psShared = NULL;
	ttseVolumeInfo *pVolumeInfo = NULL;

	if(_pTseProcess == NULL)
	{
		ALOGE("Error _pTseProcess function not loaded!!! check LGTSE_StartDynamicLoading() result!\r\n");
		return TSE_ERR_NULL_POINTER;
	}
	if(pHandle == NULL || pMicIn == NULL || pMicOut == NULL || pRecvIn == NULL || pRecvOut == NULL)
	{
		ALOGE( "ERROR! input/output buffer pointer null error! \n" );
		return TSE_ERR_NULL_POINTER;
	}

	if(pVolInfo != NULL)
		pVolumeInfo = (ttseVolumeInfo *)pVolInfo;

	if (ctx->debugger != NULL)
	{
		psShared = ctx->debugger->sShared; //fix 20150311 DefectId=84238
		stseDebugBuffer = ctx->debugger->sTseDebugBuffer;
		if (psShared->iClientConnected == iTRUE)
		{
			iTseDebugInBytesAvailable = extractFromRingbuffer(psShared->psRxRingbuffer, pTseDebugBufferIn, iDEBUG_BUFFER_IN_MAXSIZE);

			if (0 > iTseDebugInBytesAvailable)
			{
				ALOGE( "ERROR   : processThread() : extractFromRingbuffer() failed\n" );
			} 
			else if (0 <= iTseDebugInBytesAvailable)
			{
				stseDebugBuffer->InBuffer = pTseDebugBufferIn;
				stseDebugBuffer->InBufferSize = iDEBUG_BUFFER_IN_MAXSIZE;
				stseDebugBuffer->InBufferUsedSize = (unsigned int)iTseDebugInBytesAvailable;

				stseDebugBuffer->OutBuffer = pTseDebugBufferOut;
				stseDebugBuffer->OutBufferSize = iDEBUG_BUFFER_OUT_MAXSIZE;
				stseDebugBuffer->OutBufferUsedSize = 0;
			}
		}
	}
	ctx->micIn [0] = (short *) pMicIn;
	ctx->micOut [0] = (short *) pMicOut;
	if (pRecvIn)
		ctx->recvIn [0] = (short *) pRecvIn;
	else
		ctx->recvIn [0] = (short *) ctx->dummyRecvIn;
	ctx->recvOut [0] = (short *) pRecvOut;

	if (1)
	{
		res = _pTseProcess(ctx->ptseMain, ctx->micIn, ctx->recvIn, NULL, ctx->micOut, ctx->recvOut, pVolumeInfo, stseDebugBuffer);
		if (ctx->debugger != NULL)
		{
			iTseDebugOutBytesAvailable = stseDebugBuffer->OutBufferUsedSize;
		}
	}
	else
	{
		int size = ctx->frameCount * ctx->channels * sizeof (short);
		if (pMicOut != NULL && pMicIn != NULL)
			memcpy (pMicOut, pMicIn, size);

		if (pRecvOut != NULL && pRecvIn != NULL)
			memcpy (pRecvOut, pRecvIn, size);

		if (ctx->debugger != NULL) {
			iTseDebugOutBytesAvailable = iTseDebugInBytesAvailable;
			memcpy( pTseDebugBufferOut, pTseDebugBufferIn, iTseDebugOutBytesAvailable );
		}
	}

	if (ctx->debugger != NULL && iTRUE == psShared->iClientConnected && iTseDebugOutBytesAvailable > 0)
	{
		iTxRingbufferInserted = insertIntoRingbuffer(psShared->psTxRingbuffer, pTseDebugBufferOut, iTseDebugOutBytesAvailable);

		if (iTxRingbufferInserted != iTseDebugOutBytesAvailable)
			ALOGE("processThread() : tx ringbuffer full. Droping %i bytes\n", iTseDebugOutBytesAvailable - iTxRingbufferInserted);

		int iError = DbgSemaPost(psShared->phTxSema);

		if (iError != 0)
			ALOGE("processThread() : sem_post() failed\n");
	}

	if (res != TSE_ERR_OK)
	{
		_pTseGetErrorMessage(ctx->ptseMain, errorMsg);
		ALOGD("tseProcess is NOK - %s \r\n", errorMsg);
	}

	return res != TSE_ERR_OK;
}

/** @brief set Filter log callback function
 *
 *  @param userCB log print callback function pointer.
 *  @return 0 for succes, other for errors.
 *  @remark 주의사항 없음
 */
int LGTSE_SetLog(LGTSE_logCB userCB)// for debug
{
	tseFilter_log_init(userCB);
	return TSE_ERR_OK;
}

/** @brief set MIC volume in Tse engine
 *
 *  @param pHandle handle for processing.
 *  @param volume MIC volume information. 0:-6dB, 1:-3dB, 2:0dB, 3:3dB, 4:6dB
 *  @return void.
 *  @remark 주의사항 없음
 */
void LGTSE_FilterMicVolume(void *pHandle, int volume)
{
	FilterContext *ctx = (FilterContext *) pHandle;

	if(pHandle == NULL)
		return;

	ALOGD("LGTSE_FilterMicVolume vol:%d \r\n", volume);
	_pTseSetData(ctx->ptseMain, TSE_MICOUT_VOLUME, 0, sizeof(volume),
		(void*) &volume);
}

/** @brief set MIC mute in Tse engine
 *
 *  @param pHandle handle for processing.
 *  @param mute MIC mute information. 0:mute off, 1:mute on
 *  @return void.
 *  @remark 주의사항 없음
 */
void LGTSE_FilterMicMute(void *pHandle, int mute)
{
	FilterContext *ctx = (FilterContext *) pHandle;
	if(pHandle == NULL)
		return;
	ALOGD("LGTSE_FilterMicMute mute:%d \r\n", mute);
	_pTseSetData(ctx->ptseMain, TSE_MICOUT_MUTE, 0, sizeof(mute),
		(void*) &mute);
}

/** @brief get current TSF checksum information.
 *
 *  @param pHandle handle for processing.
 *  @param checksum out TSF checksum value
 *  @param menuFacturer out TSF checksum value
 *  @param modelInfo out TSF model information
 *  @param tuningDate out TSF tuning date information
 *  @return 0 for succes, other for errors.
 *  @remark call LGTSE_FilterInit() recommanded before using API 
 */
int LGTSE_FilterGetChecksumInfo(void *pHandle, int *checksum, int *manufacturer, int *modelInfo, int *tuningDate)
{
	FilterContext *ctx = (FilterContext *) pHandle;
	//int checksum = -1;
	unsigned int getDataSize = 0;
	
	if(pHandle == NULL || checksum == NULL || manufacturer == NULL || modelInfo == NULL || tuningDate == NULL)
		return -1;
	if(_pTseGetData == NULL)
		return TSE_ERR_NULL_POINTER;

	_pTseGetData(ctx->ptseMain, TSE_TSF_CHECKSUM, 0, &getDataSize, (void*)checksum);
	if(getDataSize != sizeof(*checksum)){
		ALOGE("size mismatch!!! \r\n");
		return TSE_ERR_ERR;
	}
	_pTseGetData(ctx->ptseMain, TSE_TSF_INFO_CAR_MANUFACTURER, 0, &getDataSize, (void*)manufacturer);
	if(getDataSize != sizeof(*manufacturer)){
		ALOGE("size mismatch!!! \r\n");
		return TSE_ERR_ERR;
	}
	_pTseGetData(ctx->ptseMain, TSE_TSF_INFO_CAR_MODEL, 0, &getDataSize, (void*)modelInfo);
	if(getDataSize != sizeof(*modelInfo)){
		ALOGE("size mismatch!!! \r\n");
		return TSE_ERR_ERR;
	}
	_pTseGetData(ctx->ptseMain, TSE_TSF_INFO_TUNING_DATE, 0, &getDataSize, (void*)tuningDate);
	if(getDataSize != sizeof(*tuningDate)){
		ALOGE("size mismatch!!! \r\n");
		return TSE_ERR_ERR;
	}

	return TSE_ERR_OK;
}
