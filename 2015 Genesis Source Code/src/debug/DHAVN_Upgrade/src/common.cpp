#include "common.h"
#include <QFileInfo>
#include <QDir>
#include <QFile>

bool		g_isReadonly = false;
int			g_variantCode = 0;	// Default Korea
int			g_carTypeCode = 0;  // Default DH
bool		g_variantGetSuccess = false;
int             g_languageId =0;
bool gf_check_variant(){
	return ( g_variantGetSuccess && g_variantCode >= 0 && g_variantCode < 8);
}

const char * g_variant_str[] = {
	"korea",
	"us",
	"china",
	"general",
	"me",
	"europe",
	"canada",
	"russia",
	NULL
};

const char * g_variant_short_str[] = {
	"KR",
	"US",
	"CH",
	"GE",
	"ME",
	"EU",
	"CA",
	"RU",
	NULL
};


bool gf_remove_directory(const QString & name){
	bool result = true;
	QDir dir(name);

	if ( dir.exists(name)){
		Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst)){
			if ( info.isDir()){
				result = gf_remove_directory(info.absoluteFilePath());
			}
			else{
				result = QFile::remove(info.absoluteFilePath());
			}

			if ( !result){
				return result;
			}
		}
		result = dir.rmdir(name);
	}
	return result;
}
