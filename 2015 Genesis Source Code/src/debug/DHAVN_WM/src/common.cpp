#include "common.h"

#include <QObject>

// Status Command Table
// Name , Argument Count, Priority
Commands g_status_table[] = {
	{ "RSE", 			2, 0, false, -1, ":images/indi/indi_rse" ,			1, -1},
	{ "COPY",			2, 1, false, -1, "qrc:/images/indi/indi_copy_0", 5, 1},

	{ "AP",				2, 1, false, -1,  ":images/indi/indi_ap",			1, -1},
	{ "WIFI_LOAD",		2, 1, false, -1, ":images/indi/indi_wifi_loading",	1, -1},
	{ "WIFI",			3, 1, false, -1, ":images/indi/infi_wifi_0", 		4, 0},


	{ "3G_UD",			2, 2, false, -1, ":images/indi/indi_3g_ud_0",		3, 1},
	{ "3G_D",			2, 2, false, -1, ":images/indi/indi_3g_d_0",		4, 0},
	{ "3G_U",			2, 2, false, -1, ":images/indi/indi_3g_u_0",		4, 0},
	{ "3G",				2, 2, false, -1, ":images/indi/indi_3g_default",	1, -1 },
	{ "HSDPA_RSSI",		3, 2, false, -1, ":images/indi/indi_rssi_hsdpa_0",	6, 0},
	{ "BT_RSSI",		3, 2, false, -1, ":images/indi/indi_rssi_bt_0",		5, 0},

	{ "MUTE",			2, 3, false, -1, ":images/indi/indi_mute",			1, -1},
	{ "BT_MUTE",		2, 3, false, -1, ":images/indi/indi_bt_mute",		1, -1},
	{ "CALL",			2, 3, false, -1, ":images/indi/indi_call",			1, -1},
	{ "BT_CALL",		2, 3, false, -1, ":images/indi/indi_bt_call",		1, -1},

	{ "BT_DOWN_PB",		2, 3, false, -1, ":images/indi/indi_down_phonebook", 1, -1},
	{ "BT_DOWN_RECENT",	2, 3, false, -1, ":images/indi/indi_down_recent",	1, -1},
	{ "BT_HF_AUDIO",	2, 3, false, -1, ":images/indi/indi_bt_handfree_audio", 1, -1},
	{ "BT_AUDIO",		2, 3, false, -1, ":images/indi/indi_bt_audio",		1, -1},
	{ "BT_HF",			2, 3, false, -1, ":images/indi/indi_bt_handsfree",	1, -1},

	{ "BT_BATTERY",		3, 4, false, -1, ":images/indi/indi_battery_0",		6, 0},

	{ "AV_MUTE",		2, 5, false, -1, ":images/indi/indi_audio_off",		1, -1},

	{ "NAVI_MUTE",		2, 6, false, -1, ":images/indi/indi_navi_off",		1, -1},

	{ "TRAFFIC",		2, 7, false, -1, ":images/indi/indi_traffic",		1, -1},
	{ "TMC",			2, 7, false, -1, ":images/indi/indi_tmc",			1, -1},
	{ "TPEG_NEW",		2, 7, false, -1, ":images/indi/indi_tpeg_new",		1, -1},
	{ "TPEG",			2, 7, false, -1, ":images/indi/indi_tpeg",			1, -1},

	{ "TA",				2, 8, false, -1, ":images/indi/indi_ta", 			1, -1},
	{ "NEWS",			2, 9, false, -1, ":images/indi/indi_news",			1, -1},
	{ "REG",			2, 10, false, -1, ":images/indi/indi_reg",			1, -1},
	{ 0,				0, 0, false, -1, 0, -1, -1}
};

#define SEPARATOR_ICON	":images/osd/divide_indi.png"

const char * g_osd_mode_icons[] = {
	"",
	":images/osd/ico_mode_radio.png",
	":images/osd/ico_mode_radio.png",
	":images/osd/ico_mode_radio.png",
	":images/osd/ico_mode_radio.png",
	":images/osd/ico_mode_radio.png",
	":images/osd/ico_mode_dmb.png",
	":images/osd/ico_mode_disc.png",
	":images/osd/ico_mode_ipod.png",
	":images/osd/ico_mode_aux.png",
	":images/osd/ico_mode_usb.png",
	":images/osd/ico_mode_jukebox.png",
	":images/osd/ico_mode_movie.png",
	":images/osd/ico_mode_movie.png",
	":images/osd/ico_mode_pandora.png",
	":images/osd/ico_mode_bt.png",
	":images/osd/ico_mode_voice.png",
	":images/osd/ico_mode_drive.png",
	":images/osd/ico_mode_height.png",
	":images/osd/ico_mode_seat.png",
	":images/osd/ico_mode_sxm_data.png",
	":images/osd/ico_mode_aha.png",
	":images/osd/ico_mode_bluelink.png",
	"",
	":images/osd/ico_mode_app.png",
	0
};

const char * g_osd_info_icons[] = {
	":images/osd/info_loading/info_loading_",
	":images/osd/ico_info_disable.png",
	":images/osd/ico_info_music.png",
	":images/osd/ico_info_program.png",
//	":images/osd/ico_info_ch.png",
	":images/osd/ico_info_endcall.png",
	":images/osd/ico_info_man.png",
	":images/osd/ico_info_sound.png",
	":images/osd/ico_info_receive.png",
	":images/osd/ico_info_up.png",
	":images/osd/ico_info_down.png",
	":images/osd/ico_mode_info.png",
	":images/osd/ico_mode_movie.png",
	":images/osd/ico_info_mute.png",
	":images/osd/ico_info_program.png",
	":images/osd/climate/ico_wind.png",
	":images/osd/ico_info_station.png",
	":images/osd/ico_info_calling.png",
	":images/osd/ico_mode_bluelink.png",
	":images/osd/ico_mode_voice.png",
	":images/osd/ico_info_send.png",
	0
};

const OSDStatusIcon g_osd_status_icons[] = {
	{ "(SE)", ":images/osd/ico_scan.png", 0, 0},
	{ "(LO)", ":images/osd/loading/loading_", 16, 1},
	{ "(HD)", ":images/osd/ico_hd.png", 0, 0},
	{ "(SH)", ":images/osd/ico_shuffle.png", 0, 0},
	{ "(RP1)", ":images/osd/ico_repeat_one.png", 0, 0},
	{ "(RPA)", ":images/osd/ico_repeat_all.png", 0, 0},
	{ "(RPF)", ":images/osd/ico_repeat_folder.png", 0, 0},
	{ "(SF)", ":images/osd/ico_scan_folder.png", 0, 0},
        { "(FF)", ":images/osd/icon_ff30.png", 0, 0},
        { "(REW)", ":images/osd/icon_rew15.png", 0, 0},
        { "(AS)", ":images/osd/ico_radio_autotune.png", 0, 0}, //dmchoi
	{ 0, 0, 0, 0}
};

int g_string_to_media_index(const QString & str){
	if ( str == "USB")
		return e_OSD_USB;
	else if ( str == "DISC")
		return e_OSD_DISC;
	else if ( str == "IPOD")
		return e_OSD_IPOD;
	else if ( str == "AUX")
		return e_OSD_AUX;
	else if ( str == "JUKEBOX")
		return e_OSD_JUKEBOX;
	else if ( str == "DVD")
		return e_OSD_DVD;
	else if ( str == "VCD")
		return e_OSD_VCD;
	else if ( str == "PANDORA")
		return e_OSD_PANDORA;
	else if ( str == "BLUETOOTH")
		return e_OSD_BLUETOOTH;
	else if ( str == "VR")
		return e_OSD_VR;
	else if ( str == "AHA")
		return e_OSD_AHA;

	return 0;
}

QString g_translate(const QString & str){
	QString tranStr = str;
	if ( tranStr.startsWith("%") && tranStr.endsWith("%")){
		tranStr.remove(0, 1);
		tranStr.remove(tranStr.length() - 1, 1);
		if ( tranStr == "STR_USB")
			tranStr = QObject::tr("STR_USB");
		else if ( tranStr == "STR_USB_READING")
			tranStr = QObject::tr("STR_USB_READING");
		else if ( tranStr == "STR_USB_NOT_SUPPORTED")
			tranStr = QObject::tr("STR_USB_NOT_SUPPORTED");
		else if ( tranStr == "STR_USB_DETACHED")
			tranStr = QObject::tr("STR_USB_DETACHED");
		else if ( tranStr == "STR_BT_MUTE")
			tranStr = QObject::tr("STR_BT_MUTE");
		else if ( tranStr == "STR_BLUELINK_MUTE")
			tranStr = QObject::tr("STR_BLUELINK_MUTE");
		else if ( tranStr == "STR_NAVI_MUTE")
			tranStr = QObject::tr("STR_NAVI_MUTE");
		else if ( tranStr == "STR_AV_MUTE")
			tranStr = QObject::tr("STR_AV_MUTE");
		else if ( tranStr == "STR_VR")
			tranStr = QObject::tr("STR_VR");
		else if ( tranStr == "STR_VR_BOOTING")
			tranStr = QObject::tr("STR_VR_BOOTING");
		else if ( tranStr == "STR_BLOCK_ENTER_APP")
			tranStr = QObject::tr("STR_BLOCK_ENTER_APP");
		else if ( tranStr == "STR_IPOD")
			tranStr = QObject::tr("STR_IPOD");
		else if ( tranStr == "STR_IPOD_READING")
			tranStr = QObject::tr("STR_IPOD_READING");
		else if ( tranStr == "STR_IPOD_DETACHED")
			tranStr = QObject::tr("STR_IPOD_DETACHED");
		else if ( tranStr == "STR_IPOD_NOT_SUPPORTED")
			tranStr = QObject::tr("STR_IPOD_NOT_SUPPORTED");
		else if ( tranStr == "STR_LOW_BATTERY")
			tranStr = QObject::tr("STR_LOW_BATTERY");
		else if ( tranStr == "STR_PANDORA")
			tranStr = QObject::tr("STR_PANDORA");
		else if ( tranStr == "STR_VR_CALL")
			tranStr = QObject::tr("STR_VR_CALL");
                else if ( tranStr == "AV OFF")
                        tranStr = QObject::tr("STR_AV_OFF");
	}
	return tranStr;
}

// LANG TABLE
/*
	%STR_USB%
	%STR_USB_READING%
	%STR_USB_NOT_SUPPORTED%
	%STR_USB_DETACHED%
	%STR_BT_MUTE%
	%STR_BLUELINK_MUTE%
	%STR_NAVI_MUTE%
	%STR_AV_MUTE%
	%STR_VR%
	%STR_VR_BOOTING%
	%STR_VR_CALL%
	%STR_BLOCK_ENTER_APP%
	%STR_IPOD%
	%STR_IPOD_READING%
	%STR_IPOD_DETACHED%
	%STR_LOW_BATTERY%
*/
