// sungha.choi created 2013.03.05
#ifndef UCSCONTROL_H
#define UCSCONTROL_H

class CUcsControl
{
public:
    CUcsControl();
    static void restartUcs();
    static void stopUcs();
    static bool startUcs();
private:
    static int i2c_io_read(int dev_fd, unsigned dev_addr, unsigned index, unsigned char *buf, int rx_size);
    static int i2c_io_write(int dev_fd, unsigned dev_addr, unsigned index, unsigned char *buf, int tx_size);
    static int open_dev_ucs8100x(int *builtin_state);

};

#endif  //  UCSCONTROL_H
