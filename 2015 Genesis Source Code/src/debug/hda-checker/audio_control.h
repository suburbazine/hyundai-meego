#ifndef _AUDIO_CONTROL_H_
#define _AUDIO_CONTROL_H_

#define MUTE	0
#define UNMUTE	1
#define VOL_100		115	/* Range: 0(0%) ~ 115(100%) */

#define DEV_NAME_MASTER				"Master"
#define DEV_NAME_PCM				"PCM"
#define DEV_NAME_LINEOUT			"Line-Out"
#define DEV_NAME_SPDIF				"IEC958"
#define DEV_NAME_SPDIF_DEFAULT_PCM	"IEC958 Default PCM"

int hd_audio_set_volume(char *dev_name, int volume);
int hd_audio_set_mute(char *dev_name, int volume);

#endif /*_AUDIO_CONTROL_H_*/
