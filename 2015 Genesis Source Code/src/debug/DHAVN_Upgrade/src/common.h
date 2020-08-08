#ifndef	_COMMON_H_
#define	_COMMON_H_

#include "def.h"
#include <QString>

extern const char * g_variant_str[];
extern const char * g_variant_short_str[];

extern int g_variantCode;
extern int g_carTypeCode;
extern bool g_isReadonly;
extern bool g_variantGetSuccess;
extern int g_languageId;
bool gf_check_variant();
bool gf_remove_directory(const QString & name);

namespace VA{
	enum Variant{
		KOREA,
		NA,
		CHINA,
		GENERAL,
		ME,
		EUROPE,
		CANADA,
		CountOfVariant
	};
};

enum UpdateFailCode {
	CODE_EUID = 0,
	CODE_DBUS,
	CODE_VARIANT,	
	CODE_SYSTEMVERSION,
	CODE_ROOTDEVICE,
	CODE_CONFIGURATION_CORRUPTED
};

#include "updatelog.h"

#define ULOG						UpdateLog::instance()
#define ULOG_EXCEPTION				UpdateLog::instance()->exception_log(__FILE__, __func__, __LINE__)

#endif
