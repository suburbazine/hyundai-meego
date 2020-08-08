#include "statustable.h"

Commands g_status_table[] = {
	{ "RSE", 			2, 0, false, -1, "qrc:/images/indi/indi_rse" ,			1, -1},

	{ "COPY",			2, 1, false, -1, "qrc:/images/indi/indi_copy_0", 5, 1},

	{ "AP",				2, 1, false, -1,  "qrc:/images/indi/indi_ap",			1, -1},
	{ "WIFI_LOAD",		2, 1, false, -1, "qrc:/images/indi/indi_wifi_loading",	1, -1},
	{ "WIFI",			3, 1, false, -1, "qrc:/images/indi/infi_wifi_0", 		4, 0},


	{ "3G_UD",			2, 2, false, -1, "qrc:/images/indi/indi_3g_ud_0",		3, 1},
	{ "3G_D",			2, 2, false, -1, "qrc:/images/indi/indi_3g_d_0",		4, 0},
	{ "3G_U",			2, 2, false, -1, "qrc:/images/indi/indi_3g_u_0",		4, 0},
	{ "3G",				2, 2, false, -1, "qrc:/images/indi/indi_3g_default",	1, -1 },
	{ "HSDPA_RSSI",		3, 2, false, -1, "qrc:/images/indi/indi_rssi_hsdpa_0",	6, 0},
	{ "BT_RSSI",		3, 2, false, -1, "qrc:/images/indi/indi_rssi_bt_0",		5, 0},

	{ "MUTE",			2, 3, false, -1, "qrc:/images/indi/indi_mute",			1, -1},
	{ "BT_MUTE",		2, 3, false, -1, "qrc:/images/indi/indi_bt_mute",		1, -1},
	{ "CALL",			2, 3, false, -1, "qrc:/images/indi/indi_call",			1, -1},
	{ "BT_CALL",		2, 3, false, -1, "qrc:/images/indi/indi_bt_call",		1, -1},

	{ "BT_DOWN_PB",		2, 3, false, -1, "qrc:/images/indi/indi_down_phonebook", 1, -1},
	{ "BT_DOWN_RECENT",	2, 3, false, -1, "qrc:/images/indi/indi_down_recent",	1, -1},
	{ "BT_HF_AUDIO",	2, 3, false, -1, "qrc:/images/indi/indi_bt_handfree_audio", 1, -1},
	{ "BT_AUDIO",		2, 3, false, -1, "qrc:/images/indi/indi_bt_audio",		1, -1},
	{ "BT_HF",			2, 3, false, -1, "qrc:/images/indi/indi_bt_handsfree",	1, -1},

	{ "BT_BATTERY",		3, 4, false, -1, "qrc:/images/indi/indi_battery_0",		6, 0},

	{ "AV_MUTE",		2, 5, false, -1, "qrc:/images/indi/indi_audio_off",		1, -1},

	{ "NAVI_MUTE",		2, 6, false, -1, "qrc:/images/indi/indi_navi_off",		1, -1},

	{ "TRAFFIC",		2, 7, false, -1, "qrc:/images/indi/indi_traffic",		1, -1},
	{ "TMC",			2, 7, false, -1, "qrc:/images/indi/indi_tmc",			1, -1},
	{ "TPEG_NEW",		2, 7, false, -1, "qrc:/images/indi/indi_tpeg_new",		1, -1},
	{ "TPEG",			2, 7, false, -1, "qrc:/images/indi/indi_tpeg",			1, -1},
/*
	{ "TA",				2, 8, false, -1, "qrc:/images/indi/indi_ta", 			1, -1},
	{ "NEWS",			2, 9, false, -1, "qrc:/images/indi/indi_news",			1, -1},
	{ "REG",			2, 10, false, -1, "qrc:/images/indi/indi_reg",			1, -1},
*/
	{ 0,				0, 0, false, -1, 0, -1, -1}
};


