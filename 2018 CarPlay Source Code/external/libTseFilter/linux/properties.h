/*
 *    @file        <Properties.h>
 *    @brief       system properties API header of LG Electronics Preprocessor
 *    @version     1.0.0
 *    @date        2015-02-11  1.0.0 initial version
 *    @author      speech team
 */

#if defined( __cplusplus )
extern "C"
{
#endif
#ifndef __LINUX_PROPERTIES__
#define __LINUX_PROPERTIES__


#ifndef __ANDROID_API__
#ifndef PROPERTY_VALUE_MAX
#define PROPERTY_VALUE_MAX 92
#endif

#define DEFAULT_CONFIG_FILE_PATH "/etc/tse.config"
#ifndef CONFIG_FILE_PATH
#define CONFIG_FILE_PATH DEFAULT_CONFIG_FILE_PATH
#endif
void property_get(const char* name, char* target, const char *strDefault);

#endif


#endif
#if defined( __cplusplus )
}
#endif
