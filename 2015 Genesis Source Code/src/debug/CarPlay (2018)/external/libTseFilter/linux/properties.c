/*
 *    @file        <Properties.c>
 *    @brief       system properties API of LG Electronics Preprocessor
 *    @version     1.0.0
 *    @date        2015-02-11  1.0.0 initial version
 *    @author      speech team
 */


#ifndef __ANDROID_API__
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include "properties.h"
#include <string.h>
#include "tseFilterDebug.h"
#include <ctype.h>


#define MAXLEN 80
static char *_trim (char * s)
{
	/* Initialize start, end pointers */
	char *s1 = s, *s2 = &s[strlen (s) - 1];

	/* Trim and delimit right side */
	while ( (isspace (*s2)) && (s2 >= s1) )
	s2--;
	*(s2+1) = '\0';

	/* Trim left side */
	while ( (isspace (*s1)) && (s1 < s2) )
	s1++;

	/* Copy finished string */
	strcpy (s, s1);
	return s;
}
static void _parse_config_unit (const char* member_name, char * target)
{
	char *s, buff[256];
	FILE *fp = fopen (CONFIG_FILE_PATH, "r");
	if (fp == NULL)
	{
		ALOGE("WARNING: config file not found! \r\n"); 
		return;
	}

	/* Read next line */
	while ((s = fgets (buff, sizeof buff, fp)) != NULL)
	{
		/* Skip blank lines and comments */
		if (buff[0] == '\n' || buff[0] == '#')
			continue;
		/* Parse name/value pair from line */
		char name[MAXLEN], value[MAXLEN];
		s = strtok (buff, "=");
		if (s==NULL)
			continue;
		else
			strncpy (name, s, MAXLEN);
		s = strtok (NULL, "=");
		if (s==NULL)
			continue;
		else
			strncpy (value, s, MAXLEN);
		_trim (value);

		/* Copy into correct entry in parameters struct */
		if (strcmp(name, member_name)==0)
		{
			ALOGD( "match n found value: [%s]\n", value);
			strncpy (target, value, MAXLEN);
		}
		else
		{
			//ALOGE("WARNING: %s/%s: Unknown name/value pair!\n", name, value);
		}
	}
	fclose (fp);
}

/** @brief get system property value
 *
 *  @param name property name.
 *  @param target output buffer pointer.
 *  @param strDefault default property value.
 *  @return Void.
 *  @remark 주의사항 없음
 */
void property_get(const char* name, char* target, const char *strDefault)
{
	if(name == NULL || target == NULL || strDefault == NULL)
		return;
	ALOGD( "[%s]defult values:(%s) length(%d)\n", name, strDefault, strlen(strDefault));
	strncpy(target, strDefault, strlen(strDefault));
	_parse_config_unit(name ,target);
	ALOGD( "[%s]Get values:(%s)\n", name, target);
}
#endif





