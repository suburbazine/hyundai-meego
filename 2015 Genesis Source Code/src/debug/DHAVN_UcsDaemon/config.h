#ifndef _CONFIG_H_
#define _CONFIG_H_

#define PARM_IGNORE_KEYBOARD	"/sys/module/micom/parameters/ignore_keyboard"
#define OPTS_IGNORE_KEYBOARD	"/etc/lgeconfig/ignore_keyboard"

typedef struct {
	unsigned int delay;
	unsigned char ucs_mode;
}ucsd_conf_t;

ucsd_conf_t get_ucsd_config(unsigned char *filename);
void ignore_keyboard(void);

#endif	/* _CONFIG_H_ */
