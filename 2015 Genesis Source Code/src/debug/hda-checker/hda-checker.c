#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/io.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <syslog.h>

#include "audio_control.h"

#define hda_log(a, ...) syslog(LOG_INFO, a, ##__VA_ARGS__)

struct hda_verb{
	unsigned int verb;
	unsigned int response;
};

#define HDA_HW_PATH			"/dev/snd/hwC0D0"
#define HDA_IOCTL_VERB			_IOWR('H', 0x11, struct hda_verb)

#define HDA_VERB_GET_POWER_STATE	0xf05
#define HDA_VERB_SET_POWER_STATE	0x705

/* NID for Analog out 1 channel */
#define HDA_NID_DAC2			0x03
#define HDA_NID_LINEOUT1		0x0a


static unsigned int hda_write_verb(int nid, int verb, int param)
{
	int fd;
	struct hda_verb val;

	fd = open(HDA_HW_PATH, O_RDWR);

	if(fd < 0){
		hda_log("can't open hda device\n");
		return -1;
	}

	val.verb = ((nid) << 24 | (verb) << 8 | (param));


	if (ioctl(fd, HDA_IOCTL_VERB, &val) < 0){
		hda_log("hda verb ioctl fail\n");
		close(fd);
		return -1;
	}

	close(fd);

	return val.response;
}

static unsigned int hda_get_power_state(int nid)
{
	unsigned int power_state;
	
	power_state = hda_write_verb(nid, HDA_VERB_GET_POWER_STATE, 0x00);

	return power_state;
}

static int hda_set_power_state(int nid)
{
	/* 0x0 = D0 power state */
	unsigned int power_state  = 0x00;
	int ret;
	
	ret = hda_write_verb(nid, HDA_VERB_SET_POWER_STATE, power_state);

	return ret;
}

static void hda_set_volume(void)
{
	hda_log("HDA Audio Volume setting.\n");

	if( !hd_audio_set_volume(DEV_NAME_MASTER, VOL_100) )
		hda_log("Master volume setting fail.\n");

	if( !hd_audio_set_mute(DEV_NAME_MASTER, UNMUTE) )
		hda_log("Master unmute setting fail.\n");

	if( !hd_audio_set_volume(DEV_NAME_LINEOUT, VOL_100) )
		hda_log("Line-Out volume setting fail.\n");

	if( !hd_audio_set_mute(DEV_NAME_LINEOUT, UNMUTE) )
		hda_log("Line-Out unmute setting fail.\n");

	if( !hd_audio_set_mute(DEV_NAME_SPDIF, UNMUTE) )
		hda_log("SPDIF volume setting fail.\n");

	if( !hd_audio_set_mute(DEV_NAME_SPDIF_DEFAULT_PCM, MUTE) )
		hda_log("SPDIF Default PCM volume setting fail.\n");
}

int main(int argc, char **argv)
{
	int cnt;
	int result_lineout1 = 0;
	int power_state_lineout1 = 0;
#if 0
	int result_dac2 = 0;
	unsigned int power_state_dac2;
#endif
	
	/* if there are some problem then retry during 10 sec */
	for(cnt = 0; cnt < 20; cnt++) {

#if 0
		if(!result_dac2) {
			power_state_dac2 = hda_get_power_state(HDA_NID_DAC2);

			if(power_state_dac2 < 0) 
				hda_log("DAC2 : unknown state maybe there are some error.\n");

			if(power_state_dac2 == 0) {
				hda_log("DAC2 power state check OK.\n");
				result_dac2 = 1;
			}

			if(power_state_dac2 != 0) {
				hda_log("DAC2 power set state is 0x%01x, actual state is 0x%01x\n", 
						(power_state_dac2 & 0xf), ((power_state_dac2 >> 4) & 0xf));
				hda_log("Change DAC2 power state to D0.\n");

				hda_set_power_state(HDA_NID_DAC2);
			}

		}
#endif

		if(!result_lineout1) {
			power_state_lineout1 = hda_get_power_state(HDA_NID_LINEOUT1);

			if(power_state_lineout1 < 0) 
				hda_log("LineOut1 : unknown state maybe there are some error.\n");

			if(power_state_lineout1 == 0) {
				hda_log("LineOut1 power state check OK.\n");
				result_lineout1 = 1;
			}

			if(power_state_lineout1 != 0) {
				hda_log("LineOut1 power set state is 0x%01x, actual state is 0x%01x\n", 
						(power_state_lineout1 & 0xf), ((power_state_lineout1 >> 4) & 0xf));
				hda_log("Change LineOut1 power state to D0.\n");

				hda_set_power_state(HDA_NID_LINEOUT1);
			}

		}

#if 0
		if(result_dac2 && result_lineout1)
			break;
#else
		if(result_lineout1)
			break;
#endif

		/* after first time, we have some delay for driver */
		if(cnt != 0)
			usleep(500 * 1000);

	}

#if 0
	if(!result_dac2 || !result_lineout1)
#else
	if(!result_lineout1)
#endif
		hda_log("NAVI/VR channel power state set to D0 was finally fail.\n");


	/* amixer volume setting */
	hda_set_volume();

	return 0;
}
