#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <syslog.h>

#include "audio_control.h"

#define hda_log(a, ...) syslog(LOG_INFO, a, ##__VA_ARGS__)

int hd_audio_set_volume(char *dev_name, int volume)
{
	int ret;

	snd_mixer_t *handle;
	snd_mixer_selem_id_t *sid;
	const char *card = "hw:0";
	const char *selem_name = dev_name;

	if((ret = snd_mixer_open(&handle, 0)) < 0) {
		hda_log("snd_mixer_open fail %d(%s)\n", ret, dev_name);
		return 0;
	}

	if((ret = snd_mixer_attach(handle, card)) < 0) {
		hda_log("snd_mixer_attach fail %d(%s)\n", ret, snd_strerror(ret));
		return 0;
	}

	if((ret = snd_mixer_selem_register(handle, NULL, NULL)) < 0) {
		hda_log("snd_mixer_selem_register fail %d(%s)\n", ret, snd_strerror(ret));
		return 0;
	}

	if((ret = snd_mixer_load(handle)) < 0) {
		hda_log("snd_mixer_load fail %d(%s)\n", ret, snd_strerror(ret));
		return 0;
	}

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, selem_name);

	snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

	/* Set volue */
	if (snd_mixer_selem_has_playback_volume(elem)) {
		snd_mixer_selem_set_playback_volume_all(elem, (long)volume);
		if( ret < 0 ) {
			hda_log("snd_mixer_selem_set_playback_volume_all fail %d(%s)\n", 
					ret, snd_strerror(ret));
			return 0;
		}
	}

	snd_mixer_close(handle);

	return 1;
}

int hd_audio_set_mute(char *dev_name, int mute)
{
	int ret;

	snd_mixer_t *handle;
	snd_mixer_selem_id_t *sid;
	const char *card = "hw:0";
	const char *selem_name = dev_name;

	if((ret = snd_mixer_open(&handle, 0)) < 0) {
		hda_log("snd_mixer_open fail %d(%s)\n", ret, dev_name);
		return 0;
	}

	if((ret = snd_mixer_attach(handle, card)) < 0) {
		hda_log("snd_mixer_attach fail %d(%s)\n", ret, snd_strerror(ret));
		return 0;
	}

	if((ret = snd_mixer_selem_register(handle, NULL, NULL)) < 0) {
		hda_log("snd_mixer_selem_register fail %d(%s)\n", ret, snd_strerror(ret));
		return 0;
	}

	if((ret = snd_mixer_load(handle)) < 0) {
		hda_log("snd_mixer_load fail %d(%s)\n", ret, snd_strerror(ret));
		return 0;
	}

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, selem_name);

	snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

	/* Set mute or unmute */
	if (snd_mixer_selem_has_playback_switch(elem)) {
		ret = snd_mixer_selem_set_playback_switch_all(elem, mute);
		if( ret < 0 ) {
			hda_log("snd_mixer_selem_set_playback_switch_all fail %d(%s)\n", 
					ret, snd_strerror(ret));
			return 0;
		}

	}

	snd_mixer_close(handle);

	return 1;
}
