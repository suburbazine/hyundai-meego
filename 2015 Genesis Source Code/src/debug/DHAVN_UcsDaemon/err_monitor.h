#ifndef _ERR_MONITOR_H_
#define _ERR_MONITOR_H_

#define UCS_ERR_OVER_CURRENT	0x80
#define UCS_ERR_CLEARED_STATE	0x00

#define DH_USB_POWER_ON			1
#define DH_USB_POWER_OFF		0

#define DH_REAR_USB_RECONNECT		"/sys/bus/usb/devices/2-1.2/hub_port_power_reset"

#define DH_PCI_BUS_USB1_REMOVE	"/sys/bus/pci/devices/0000:02:02.1/remove"
#define DH_PCI_BUS_USB2_REMOVE	"/sys/bus/pci/devices/0000:02:08.1/remove"
#define DH_PCI_BUS_USB3_REMOVE	"/sys/bus/pci/devices/0000:02:02.0/remove"
#define DH_PCI_BUS_USB4_REMOVE	"/sys/bus/pci/devices/0000:02:08.0/remove"
#define DH_PCI_BUS_USB5_REMOVE	"/sys/bus/pci/devices/0000:02:08.1/remove"
#define DH_PCI_BUS_USB6_REMOVE	"/sys/bus/pci/devices/0000:02:08.2/remove"

#define DH_PCI_BUS_RESCAN	"/sys/bus/pci/rescan"

#define DH_USB_ERROR_COUNT			"/proc/dh_usb_errr_cnt"
#define DH_USB_PORT_RESET			"/proc/dh_usb_port_reset"
#define DH_USB_BLK_ERROR_DEV		"/proc/dh_usb_blk_err_dev"
#define DH_USB_RESET_REQ_PORT_NONE	0
#define DH_USB_RESET_REQ_PORT_FRONT	1
#define DH_USB_RESET_REQ_PORT_REAR	2
#define DH_USB_RESET_REQ_PORT_ALL	3

#define MAX_USB_ERROR_STATE_CNT		100
#define MAX_USB_MEMORY_RESET		3

typedef struct {
	int i2c_fd;			/* fd number for ucs8100x i2c device */
	int delay;			/* polling time, second */
	int builtin_front;	/* 1 = builtin / 0 = none */
	int builtin_rear;	/* 1 = builtin / 0 = none */
}polling_info_t;

typedef struct {
	int irq_fd;			/* fd number for ucs8100x i2c device */
	int alert_state;	/* 1 = normal / 0 = alert */
	int alert_monitor;	/* 1 = on / 0 = off */
}alert_info_t;

typedef struct {
	char dev_name[8];	/* block dev name. (/dev/sd[b-z])*/
	int err_state;			/* 1 = IO error detect / 0 = clear */
	unsigned int reset_cnt;	/* reset count. max 3 times */
}usb_mem_info_t;

void *check_ucs_interrupt_state(void *arg);
void *check_ucs_alert_pin(void *arg);
void *check_usb_port_error_state(void *arg);
void usb_error_state_count_clear(void);
void usb_port_reset(unsigned int port_num);

#endif /* _ERR_MONITOR_H_ */
