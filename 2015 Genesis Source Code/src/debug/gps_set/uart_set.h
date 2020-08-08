#ifndef UART_SET_H
#define UART_SET_H

#define DEV_GPS_UART	"/dev/ttyPCH1"

int open_gps_uart();
void close_gps_uart(int fd);
int set_uart_comm(int fd, int set_num);
#endif
