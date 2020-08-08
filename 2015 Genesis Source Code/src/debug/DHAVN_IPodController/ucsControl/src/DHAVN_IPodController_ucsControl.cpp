// sungha.choi created 2013.03.05
#include <DHAVN_IPodController_ucsControl.h>

#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

/* UCS Switch configuration register values */
#define UCS_ALL_SWITCH_OFF			0x00
#define UCS_PWR_SWITCH_OFF			0x02
#define UCS_MODE_CDP				0xBE
#define UCS_MODE_DCP				0xB6
#define UCS_MODE_DCE				0xAE
#define UCS_MODE_SDP				0x8E
#define UCS_MODE_DATA_PATH_THROUGH	0x92

/* UCS8100x I2C device address on DH */
#define SMSC_USB_IC_ADDR_FRONT		0x54
#define SMSC_USB_IC_ADDR_REAR		0x33

/* UCS8100x I2C register address */
#define SMSC_USB_IC_PIN_STATE_REG	0x14
#define SMSC_USB_IC_SWITCH_CONF_REG	0x17
#define SMSC_USB_IC_INT_STATE_REG	0x10

/* process files info */
#define SMSC_USB_IC_I2C_DEV_FILE	"/dev/i2c-1"
#define SMSC_USB_IC_IRQ_DEV_FILE	"/dev/dhavn-ucs"
#define SMSC_USB_IC_CONF_FILE		"/etc/ucsd.conf"
#define UCS_ALERT_DEV_MODULE		"dhavn_ucs_alert"
#define SYS_CMD_MODPROBE			"modprobe"

/* UCS8100x builtin state, front and rear */
#define DH_UCS_FRONT_BUILTIN	1
#define DH_UCS_REAR_BUILTIN		2

CUcsControl::CUcsControl()
{

}

int CUcsControl::i2c_io_read(int dev_fd, unsigned dev_addr, unsigned index, unsigned char *buf, int rx_size)
{
        int ret;
        unsigned char addr_buf[32] = {0, };
        struct i2c_rdwr_ioctl_data 	msgset;
        struct i2c_msg 				msgs[2];

        msgset.msgs = msgs;
        msgset.nmsgs = 2;

        msgs[0].addr = dev_addr;
        msgs[0].flags = 0;
        msgs[0].len = 1;
        msgs[0].buf = addr_buf;

        addr_buf[0] = index;

        msgs[1].addr = dev_addr;
        msgs[1].flags = I2C_M_RD;
        msgs[1].len = rx_size;
        msgs[1].buf = buf;

        ret = ioctl(dev_fd, I2C_RDWR, (struct i2c_rdwr_ioctl_data *) &msgset);

        return ret;
}

int CUcsControl::i2c_io_write(int dev_fd, unsigned dev_addr, unsigned index, unsigned char *buf, int tx_size)
{
        int ret;
        int idx;
        char cmd_buf[32] = {0, };
        struct i2c_rdwr_ioctl_data 	msgset;
        struct i2c_msg 				msgs[1];

        msgset.msgs = msgs;
        msgset.nmsgs = 1;

        msgs[0].addr = dev_addr;
        msgs[0].flags = 0;
        msgs[0].len = tx_size + 1;
        msgs[0].buf = (__u8*)cmd_buf;

        cmd_buf[0] = index;

        for(idx = 0; idx < tx_size; idx++) {
                cmd_buf[idx+1] = buf[idx];
        }

        ret = ioctl(dev_fd, I2C_RDWR, (struct i2c_rdwr_ioctl_data *) &msgset);

        return ret;
}

int CUcsControl::open_dev_ucs8100x(int *builtin_state)
{
        int fd;
        fd = open(SMSC_USB_IC_I2C_DEV_FILE, O_RDWR);
        if( fd < 0 ) {
            return fd;
        }

        /* set device for slave */
        if(!ioctl(fd, I2C_SLAVE, SMSC_USB_IC_ADDR_FRONT))
                *builtin_state += DH_UCS_FRONT_BUILTIN;

        if(!ioctl(fd, I2C_SLAVE, SMSC_USB_IC_ADDR_REAR))
                *builtin_state += DH_UCS_REAR_BUILTIN;

        return fd;
}

void CUcsControl::restartUcs()
{
    int ucs_fd = 0;
    int ucs_builtin_state = 0;
    unsigned char tx_buf[4] = {0, };
    ucs_fd = open_dev_ucs8100x(&ucs_builtin_state);

	// Added by jonghwan.cho@lge.com	2013.03.29.
	// for static test for CR20177
	if( ucs_fd < 0 )	return;

    /* Turn off UCS8100x */
    tx_buf[0] = UCS_MODE_CDP ^ UCS_PWR_SWITCH_OFF;
    i2c_io_write(ucs_fd, SMSC_USB_IC_ADDR_FRONT, SMSC_USB_IC_SWITCH_CONF_REG, tx_buf, 1);

    /* Turn on UCS8100x mode is CDP */
    tx_buf[0] = UCS_MODE_CDP;
    i2c_io_write(ucs_fd, SMSC_USB_IC_ADDR_FRONT, SMSC_USB_IC_SWITCH_CONF_REG, tx_buf, 1);

    close(ucs_fd);
}

void CUcsControl::stopUcs()
{
    int ucs_fd = 0;
    int ucs_builtin_state = 0;
    unsigned char tx_buf[4] = {0, };
    ucs_fd = open_dev_ucs8100x(&ucs_builtin_state);

	// Added by jonghwan.cho@lge.com	2013.03.29.
	// for static test for CR20178
	if( ucs_fd < 0 )	return;

    /* Turn off UCS8100x */
    tx_buf[0] = UCS_MODE_CDP ^ UCS_PWR_SWITCH_OFF;
    i2c_io_write(ucs_fd, SMSC_USB_IC_ADDR_FRONT, SMSC_USB_IC_SWITCH_CONF_REG, tx_buf, 1);

    close(ucs_fd);
}

bool CUcsControl::startUcs()
{
    int ucs_fd = 0;
    int ucs_builtin_state = 0;
    unsigned char tx_buf[4] = {0, };
    unsigned char rx_buf[4] = {0, };
    bool ret = false;
    ucs_fd = open_dev_ucs8100x(&ucs_builtin_state);

	// Added by jonghwan.cho@lge.com	2013.03.29.
	// for static test CR20179
	if( ucs_fd < 0 )	return false;

    i2c_io_read(ucs_fd, SMSC_USB_IC_ADDR_FRONT, SMSC_USB_IC_SWITCH_CONF_REG, rx_buf, 1);

    if(rx_buf[0] == UCS_MODE_CDP){       // switch off status
        ret = false;
    }else{
        /* Turn on UCS8100x mode is CDP */
        tx_buf[0] = UCS_MODE_CDP;
        i2c_io_write(ucs_fd, SMSC_USB_IC_ADDR_FRONT, SMSC_USB_IC_SWITCH_CONF_REG, tx_buf, 1);
        ret = true;
    }
    close(ucs_fd);
    return ret;
}
