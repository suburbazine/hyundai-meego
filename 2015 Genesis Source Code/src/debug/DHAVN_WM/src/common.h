#ifndef	COMMON_H_
#define COMMON_H_

#include <QString>

// WindowManager StartOfHeader
#define WM_STH		'@'
// WindowManager UnitSeparator
#define WM_US		'^'	
// WindowManager Screen Separator on command
#define WM_SS		':'
// WindowManager Front Screen
#define WM_FRONT	"F"
#define WM_REAR		"R"
#define WM_FRONT_VAL	1
#define WM_REAR_VAL		2


#define QUERY_TREE	"QUERY_TREE"
#define IEGD_CMD	"IEGD"
#define STATUS_CMD	"STA"
#define WM_CMD		"WM"
#define EVT_CMD		"EVT"

// OSD Commands
#define OSD_CMD				"OSD"


// Operation Command
#define OPERATION_CMD		"OP"

// Operation SubCommand
#define OP_HIDE				"HIDE"
#define OP_SHIDE			"SHIDE"
#define OP_SHOW				"SHOW"
#define OP_SSHOW			"SSHOW"
#define OP_HOME				"HOME"
#define OP_PRE_HOME			"PRE_HOME"
#define OP_HOME_HIDE		"HOME_HIDE"
#define OP_PRE_HOME_HIDE	"PRE_HOME_HIDE"
#define OP_CLEAR			"CLEAR"
#define OP_HIDEALL			"HIDEALL"
#define OP_FIRST_HOME		"SHOW_FIRST_HOME"

// Event Command
#define EVT_MUTE			"MUTE"
#define EVT_LANG			"LANG"
#define EVT_LCD				"LCD"
#define EVT_MODE			"MODE"
#define EVT_DISP			"DISP"
#define EVT_BLUELINK		"BLUELINK"
#define EVT_SWAP			"SWAP"
#define EVT_LOCK			"LOCK"
#define EVT_UNLOCK			"UNLOCK"
#define EVT_TIME_CHANGE		"TIME_CHANGE"
#define EVT_CLONE		"CLONE"
#define EVT_CLEAR_ALL           "CLEAR_ALL"

// Event from StatusBar
#define EVT_REGIST			"REGIST"
#define EVT_HOME			"HOME"

#define UNKNOWN_VALUE	-1


// Variant
#define VARIANT_KOREA		0
#define VARIANT_NA			1
#define VARIANT_CHINA		2
#define VARIANT_GENERAL		3
#define VARIANT_ME			4
#define VARIANT_EUROPE		5
#define VARIANT_CANADA		6
#define VARIANT_RUSIA		7

// Status Command
typedef struct{
	const char * command;
	int argc;
	int priority;
	bool show;
	int value;

	const char * icon_name_prefix;
	int icon_count;	
	int icon_start_index;
} Commands;

enum e_osd_type{
	e_OSD_COMMON,
	e_OSD_RADIO,
	e_OSD_SXM,
	e_OSD_HDRADIO,
	e_OSD_DAB,
	e_OSD_RDS,
	e_OSD_DMB,
	e_OSD_DISC,
	e_OSD_IPOD,
	e_OSD_AUX,
	e_OSD_USB,
	e_OSD_JUKEBOX,
	e_OSD_DVD,
	e_OSD_VCD,
	e_OSD_PANDORA,
	e_OSD_BLUETOOTH,
	e_OSD_VR,
	e_OSD_DRIVE,
	e_OSD_HEIGHT,
	e_OSD_SEAT,
	e_OSD_SXM_DATA,
	e_OSD_AHA,
	e_OSD_BLUELINK,
	e_OSD_DLNA,
	e_OSD_APPLICATION
};

enum e_osd_info_type{
	e_INFO_LOADING,
	e_INFO_DISABLE,
	e_INFO_MUSIC,
	e_INFO_CH,
	e_INFO_ENDCALL,
	e_INFO_MAN,
	e_INFO_SOUND,
	e_INFO_RECEIVE,
	e_INFO_UP,
	e_INFO_DOWN,
	e_INFO_INFO,
	e_INFO_MOVIE,
	e_INFO_MUTE,
	e_INFO_PROGRAM,
	e_INFO_CLIMATE,
	e_INFO_STATION,
	e_INFO_CALL,
	e_INFO_BLUELINK,
	e_INFO_VOICE,
	e_INFO_OUTCALL
};

enum e_osd_subtype{
	e_COMMON_WARN,
	e_COMMON_VOLUME,
	e_COMMON_VOLUME_MUTE,
	e_RADIO_INFO,
	e_RADIO_INFO_DIAL
};

enum e_osd_status_icon{
	e_OSD_STATUS_SCAN,
	e_OSD_STATUS_LOADING,
	e_OSD_STATUS_HD,
	e_OSD_STATUS_AUTOSTORE = 10, //dmchoi
};

typedef struct{
	const char *	string;
	const char *	icon_name;
	int				ani_count;	
	int				start_ani_index;
} OSDStatusIcon;

extern const char * g_osd_mode_icons[];
extern const char * g_osd_info_icons[];

extern Commands g_status_table[];
extern const OSDStatusIcon g_osd_status_icons[];
extern int g_string_to_media_index(const QString &);

extern QString g_translate(const QString & str);

enum LanguageInfo {
	LANG_ko_KR = 1,	// Korea
	LANG_en_US,		// English(US)
	LANG_en_GB,		// English(UK)
	LANG_zh_CN,		// Chinese mandarin
	LANG_zh_HK,		// Chinese cantonese
	LANG_fr_FR,		// French
	LANG_es_ES,		// Spanish
	LANG_ar_SA,		// Arabic
	LANG_cs_CZ,		// Czech
	LANG_da_DK,		// Danish
	LANG_de_DE,		// German
	LANG_it_IT,		// Italian
	LANG_nl_NL,		// Dutch
	LANG_pl_PL,		// Polish
	LANG_pt_PT,		// Portuguese
	LANG_ru_RU,		// Russian
	LANG_sk_SK,		// Slovak
	LANG_sv_SE,		// Swedish
        LANG_tr_TR,		// Turkish
        LANG_fr_CA,             // French(US)
        LANG_es_US              // Spanish(US)
};


#endif
