/*
	File:       SettingUtils.cpp
	Package: 	LG CarPlay Application Framework.
	Author:     Soohwan Oh (soohwan.oh@lge.com)

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

#include "SettingUtils.h"

#include <string>
#include <map>

#include "APSCommonServices.h"
#include "APSDebugServices.h"
#include "ThreadUtils.h"

using namespace std;

typedef map<string, int32_t>      MapInt32;
typedef map<string, int64_t>      MapInt64;
typedef map<string, const char*>  MapString;

static MapInt32                   gInt32Map;
static MapInt64                   gInt64Map;
static MapString                  gStringMap;

static pthread_mutex_t            gInt32Lock    = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t            gInt64Lock    = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t            gStringLock   = PTHREAD_MUTEX_INITIALIZER;

EXPORT_GLOBAL
OSStatus CarPlaySettings_SetInt32( const char *inKey, int32_t inValue )
{
	OSStatus     err = kNoErr;

	pthread_mutex_lock( &gInt32Lock );

	gInt32Map[string(inKey)] = inValue;

	pthread_mutex_unlock( &gInt32Lock );

	return( err );
}

EXPORT_GLOBAL
int32_t CarPlaySettings_GetInt32( const char *inKey, OSStatus *outErr )
{
	int32_t      ret;
	OSStatus     err;

	pthread_mutex_lock( &gInt32Lock );

	MapInt32::iterator it = gInt32Map.find( inKey );

	if( it != gInt32Map.end() )
	{
		err = kNoErr;
		ret = it->second;
	}
	else
	{
		err = kNotFoundErr;
		ret = 0;
	}

	pthread_mutex_unlock( &gInt32Lock );

	if( outErr ) *outErr = err;
	return( ret );
}

EXPORT_GLOBAL
OSStatus CarPlaySettings_SetInt64( const char *inKey, int64_t inValue )
{
	OSStatus     err = kNoErr;

	pthread_mutex_lock( &gInt64Lock );

	gInt64Map[string(inKey)] = inValue;

	pthread_mutex_unlock( &gInt64Lock );

	return( err );
}

EXPORT_GLOBAL
int64_t CarPlaySettings_GetInt64( const char *inKey, OSStatus *outErr )
{
	int64_t      ret;
	OSStatus     err;

	pthread_mutex_lock( &gInt64Lock );

	MapInt64::iterator it = gInt64Map.find( inKey );

	if( it != gInt64Map.end() )
	{
		err = kNoErr;
		ret = it->second;
	}
	else
	{
		err = kNotFoundErr;
		ret = 0;
	}

	pthread_mutex_unlock( &gInt64Lock );

	if( outErr ) *outErr = err;
	return( ret );
}

EXPORT_GLOBAL
OSStatus CarPlaySettings_SetString( const char *inKey, const char * inStr, size_t inLen )
{
	OSStatus     err = kNoErr;

	(void) inLen;

	pthread_mutex_lock( &gStringLock );

	gStringMap[string(inKey)] = inStr;

	pthread_mutex_unlock( &gStringLock );

	return( err );
}

EXPORT_GLOBAL
char * CarPlaySettings_GetString( const char *inKey, char * inBuf, size_t inMaxLen, OSStatus *outErr )
{
	OSStatus err;

	pthread_mutex_lock( &gStringLock );

	MapString::iterator it = gStringMap.find( inKey );

	if( it != gStringMap.end() )
	{
		err = kNoErr;
		snprintf( inBuf, inMaxLen, "%s", it->second );
	}
	else
	{
		err = kNotFoundErr;
	}

	pthread_mutex_unlock( &gStringLock );

	if( outErr ) *outErr = err;
	if( err )    return( NULL );
	else         return( inBuf );
}

