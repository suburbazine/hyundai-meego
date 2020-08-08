#ifndef	MSG_CLIENT_H_
#define	MSG_CLIENT_H_

#ifdef __cplusplus
extern "C"{
#endif

void send_module_status(int module_id, int module_progress, int module_status, const char * msg);

#ifdef __cplusplus
}
#endif

enum ModuleID{
	MID_BLUETOOTH,
	MID_CCP,
	MID_RRC,
	MID_DAB,
	MID_FPGA,
	MID_INIC,
	MID_TDMB,
	MID_MICOM,
	MID_IBOX,
	MID_GPS,
	MID_XM,
	MID_HDRADIO,
	MID_BIOS,
	MID_FRONT_LCD,
	MID_REAR_RIGHT_LCD,
        MID_REAR_LEFT_LCD,
        MID_SSD,             // SANGWOO_TEMP
	MID_GNSS
};

enum ModuleStatus{
	MODULE_STATUS_INIT,			// process kill applications which use same resource with AppUpgrade
	MODULE_STATUS_PROGRESS, 	// ordinary flow
	MODULE_STATUS_WARNING,		// there is a suspect thing, but it does not affect for updating
	MODULE_STATUS_RETRY,		// retry flow
	MODULE_STATUS_FAIL,			// update fail
	MODULE_STATUS_DONE,			// update finished
	MODULE_STATUS_OLD_VERSION,	// firmware version is older or equal than current
	MODULE_STATUS_INFO			// for debugging purpose
};
#endif
