/*
 * micom_manager_table.cpp
 *
 *  Created on: 2011. 9. 3.
 *      Author: yongwoo1.park
 */


#include "micom_manager_table.h"


// TODO: Does struct need to split cmds for searching speed?
const S_CmdTable g_CmdTable[] = {
		// command | data_size | bus_index | cmd_name_index
		// data_size 0x00 means variable size

		/**************** Control Part ****************/

		//Audio Main CH. Change
		{ 0x0110,		1,	0, 0, false},
		// Audio Sub CH. Change
		{ 0x0111,		1, 	0, 1, true},
		{ 0x8111,		1,	0, 1, false},
		// Audio Mixing CH. Control
		{ 0x0112,		2,	0, 2, false},
		// Audio Volume Control
		{ 0x0113,		2, 	0, 3, false},
		// Audio Setup Control
		{ 0x0114,		2, 	0, 4, false},
		// Audio Beep Play
		{ 0x0115,		1,	0, 5, false},
		// Audio Mute Control
		{ 0x0116,		2,	0, 6, false},
		// Audio DTMF Control
		{ 0x0117,		1,	0, 48, false},
		// Audio Last memory
		{ 0x0119,		0x08,	0, 74, false},
		// Radio Freq. Change
		{ 0x0120,		3,	1, 7, false},
		// Radio Preset Recall
		{ 0x0121,		5,	1, 8, false},
		// Radio Preset Store
		{ 0x0122,		5,	1, 9, false},
		// Radio Station Search
		{ 0x0123,		1,	1, 10, false},
		// Radio DAB_Following Station Status Request
		{ 0x0124,		0,	1, 52, false},
		// Radio External Diagnosis Status
		{ 0x0125,		1,	1, 53, false},
		// Radio Preset RestoreAll 
		{ 0x0126,		0xd8,	1, 72, false},
		// Radio Reorder Preset Store  
		{ 0x0127,		0x0E,	1, 75, false},
		// RDS Option Set
		{ 0x0131,		1,	1, 32, false},
		// RDS Info Cancel
		{ 0x0132,		1,	1, 33, false},
		// RDS Info check Enable
		{ 0x0133,		1,	1, 34, false},
		// RDS Search Mode Set
		{ 0x0134,		3, 1, 46, false},
		// HDR Tune
		{ 0x0150,		0x04,   1, 28, false},
		// HDR_iTune_Tagging
		{ 0x0151,		0x80, 1, 11, false},
		// HDR_CA_Data
		{ 0x0152,		0x04, 1, 12, false},
		// HD_Module_Time_Data_Set
		{ 0x0153,		0x06, 1, 35, false},
		// HDR Signal ReAcquire	
		{ 0x0154,		0x01, 1, 62, false},
		// Diagnostic Info Request(Control)
		{ 0x0610,		0x02, 1, 50, true},
		{ 0x8610,		0x00, 1, 50, false},
		// HDM update transdata
		{ 0x0953,		0xf5, 1, 69, false},
		
		/**************** Ask Part ****************/
		// Ask Sound Setup Info
		{ 0x0210,		0x00, 0, 25, true},
		{ 0x8210,		0x0d, 0, 25, false},
		// Ask Volume Info
		{ 0x0211,		0x00, 0, 26, true},
		{ 0x8211,		0x0F, 0, 26, false},
		// Ask Audio Last Memory
		{ 0x0212,		0x00, 0, 27, true},
		{ 0x8212,		0x08, 0, 27, false},
		// Ask Broadcasting Info
		{ 0x0220,		0x01, 1, 13, true},
		{ 0x8220,		0x00, 1, 13, false},	// data size AM:0x04, FM:variable
		// Ask Preset
		{ 0x0221,		0x01, 1, 30, true},
		{ 0x8221,		0x00, 1, 30, false},
		// Ask Radio Each Preset Info
		{ 0x0222,		0x02, 1, 14, true},
		{ 0x8222,		0x00, 1, 14, false},	// data size 0x0F or 0x05
	 	// Ask RDS Option Setting
		{ 0x0230,		0x00, 1, 58, true},
		{ 0x8230,		0x01, 1, 58, false},
		// Ask HDR On/Off
		{ 0x0250,		0x00, 1, 44, true},
		{ 0x8250,		0x01, 1, 44, false},
		// Ask HDRadio CurStatus
		{ 0x0251,		0x00, 1, 60, true},
		{ 0x8251,		0x00, 1, 60, false},	// data size variable
		// Alignment Command
		{ 0X0620,		0X01, 1, 55, false},
		// Alignment Request Command
		{ 0X0621,		0X00, 1, 56, true},
		{ 0X8621,		0X17, 1, 56, false},
		// HDR Eng Command
		{ 0X0622,		0X04, 1, 61, false},
		// ANT Mode Setting Message
		{ 0x0615,		0x01, 1, 57, true},
		{ 0x8615,		0x01, 1, 57, false},
		// HDM version request
		{ 0x0950,		0x01, 1, 66, true},
		{ 0x8950,		0x24, 1, 66, false},
		// HDM update ready
		{ 0x0951,		0x01, 1, 67, true},
		{ 0x8951,		0x01, 1, 67, false},
		// HDM update reset
		{ 0x0952,		0x01, 1, 68, true},
		{ 0x8952,		0x01, 1, 68, false},
		// HDM update status
		{ 0x0954,		0x01, 1, 70, true},
		{ 0x8954,		0x01, 1, 70, false},
		// ASK Radio each preset info
		{ 0x0223,		0x01, 1, 71, true},
		{ 0x8223,		0x91, 1, 71, false},
		
		/**************** Event Part ****************/
		// Audio Volume Step Event
		{ 0x8310,		0x02, 0, 24, false},
		// Sound Setup Info. Event
		{ 0x8314,		0x0D, 0, 73, false},
		// Radio BSM Preset Update
		{ 0x8320,		0x00, 1, 15, false},
		// RadioNoti Search End
		{ 0x8321,		0x01, 1, 16, false},
		// Radio_ChkStereo
		{ 0x8322,		0x01, 1, 17, false},
		// Radio Frequency Update Event
		{ 0x8323,		0x05, 1, 18, false},
		// Radio DAB-Following Station Status
		{ 0x8324,		0x04, 1, 54, false},
		// RDS Info Start Event
		{ 0x8330,		0x0D, 1, 37, false},
		// RDS Info Finish Event
		{ 0x8331,		0x10, 1, 38, false},
		// RDS Expanded Data Event
		{ 0x8332,		0x10, 1, 39, false},
		// RDS Text Update Event
		{ 0x8333,		0x40, 1, 40, false},
		// RDS TP Loss Event
		{ 0x8334,		0x01, 1, 41, false},
		// TMC Info Update Event
		{ 0x8340,		0x0A, 1, 42, false},
		// TMC Info Decode Complete Event
		{ 0x8341,		0x0B, 1, 43, false},

		// RDS Clock Update Event
		{ 0x8335,		0x05, 1, 46, false},
		// RDS Search Mode Event
		{ 0x8336,		0x05, 1, 47, false},
		// HDR_PSD_Data
		{ 0x8350,		0x00, 1, 19, false},
		// HDR_SIS_Data
		{ 0x8351,		0x00, 1, 20, false},
		// RBDS Data Update
		{ 0x8352,		0x00, 1, 29, false},
		// HDR_Status_info
		{ 0x8353,		0x02, 1, 21, false},
		// HDR_iTune_Status
		{ 0x8354,		0x00, 1, 22, false},
		// HDR_iTune_Tagging_Token_Data
		{ 0x8355,		0x00, 1, 23, false},
		// HDR_Message_Status
		{ 0x8356,		0x00, 1, 36, false},
		// HDR OnOff Status event
		{ 0x8357,		0x01, 1, 45, false},
		// HDR Artist Experience event
		{ 0x8358,		0x82, 1, 49, false},
		// HDR_Commercial Message
		{ 0x8359,		0x94, 1, 59, false},
		// Diagnostic Info event
		{ 0x8610,		0x02, 1, 51, false},
		// HDR PSD Response
		{ 0x8613,		0x00, 1, 65, false},
		// Alignment Command
		{ 0X8620,		0X00, 1, 55, false},
		// Radio Signal Level Event
		{ 0x8325,		0x01, 1, 58, false},
		// AM Refresh Event
		{ 0x8326,		0x06, 1, 63, false},
		// AM Refresh Finish Event
		{ 0x8327,		0x01, 1, 64, false},
		{ 0, 			0, 0, 0, false}
};

const S_BusDefinition	g_BusDefinition[] = {
		// service_name	| interface_name | method
		{"com.lge.car.micom", 					"com.lge.car.micom.audioif",	"EventMessage"	},
		{"com.lge.car.micom",					"com.lge.car.micom.radioif", 	"EventMessage"	},
};

const char *	g_CmdNameTable[] = {
		"Audio Main CH. Change(Control)",
		"Audio Sub CH. Change(Control)",
		"Audio Mixing CH. Control(Control)",
		"Audio Volume Control(Control)",
		"Audio Setup Control(Control)",
		"Audio Beep Play(Control)",	
		"Audio Mute Control(Control)",
		"Radio Freq. Change(Control)",
		"Radio Preset Recall(Control)",
		"Radio Preset Store(Control)",
		"Radio Station Search(Control)", // 10
		"HDR_iTune_Tagging(Control)",
		"HDR_CA_Data(Control)",
		"Ask Broadcasting Info(Ask)",
		"Ask Radio Each Preset Info(Ask)",
		"Radio BSM Preset Update(Event)",
		"RadioNoti Search End(Event)",
		"Radio_ChkStereo(Event)",
		"Radio Frequency Update Event(Event)",
		"HDR_PSD_Data(Event)",
		"HDR_SIS_Data(Event)",		// 20
		"HDR_Status_info(Event)",
		"HDR_iTune_Status(Event)",
		"HDR_iTune_Tagging_Token_Data(Event)",
		"Audio Volume Step Event(Event)",
		"Ask Sound Setup Info(Ask)",
		"Ask Volume Info(Ask)",
		"Ask Audio Last Memory(Ask)",
		"HDR Tune",
		"RBDS Data Update(Event)",	// 29
		"Ask Preset(Ask)",		// 30
		"RDS Text Update Event(Event)", // 31
		"RDS Option Set(Control)",
		"RDS Info Cancel(Control)",	// 33
		"RDS Info check Enable(Control)",
		"HD_Module_Time_Data_Set(Control)",// 35
		"HDR_Message_Status(Event)", 
		"RDS_Info_Start(Event)",			// 37
		"RDS_Info_Finish(Event)",
		"RDS_Expanded_Data_Update(Event)",	// 39		
		"RDS Text Update(Evnet)",
		"RDS TP Loss(Event)",				// 41
		"TMC Info Update(Event)",
		"TMC Info Decode Complete(Event)",	// 43
		"HDRadio On Off(Ask)",
		"HDR OnOff Status(Event)",			// 45
		"RDS Search Mode Set(Control)",
		"RDS Clock Update(Event)",			// 46
		"RDS Search Mode(Event)",
		"Audio DTMF(Control)",				// 48
		"RDS Artist Experience(Event)",
		"Diagnostic Info Request(Control)",	// 50
		"Diagnostic Info Event",			// 51
		"Radio DAB-Following Station Satus(Control)",	//52
		"Radio External Diagnosis Status",		//53
		"Radio DAB-Following Station Status(Event)",
		"Alignment Command",				//55
		"Alignment Request Command",		//56
		"ANT Mode Setting",					//57
		"Radio Signal Level Event",
		"HDR_Commercial Message",
		"Ask HDRadio CurStatus",			//60
		"HDR Eng Command",	
		"HDR Signal ReAcquire",	
		"AM Refresh Event",
		"AM Refresh Finish Event",
		"HDR PSD Response",					//65
		"HDM version request",
		"HDM update ready",
		"HDM update reset",
		"HDM update transdata",
		"HDM update status",				//70
		"ASK Radio each preset info",
		"Radio Preset RestoreAll", 
		"Sound Setup Info. Event",
		"Audio Last Memory Ctl",
		"Radio Reorder Preset Store"		//75
};
