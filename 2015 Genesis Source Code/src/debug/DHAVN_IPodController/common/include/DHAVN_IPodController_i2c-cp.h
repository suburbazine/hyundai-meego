/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini / Venkat                    *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                                                                *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <stdio.h>
#include <linux/types.h>




/* ----- commands for the ioctl like i2c_command call:
 *  * note that additional calls are defined in the algorithm and hw
 *   *      dependent layers - these can be listed here, or see the
 *    *      corresponding header files.
 *     */
                                /* -> bit-adapter specific ioctls       */
#define I2C_RETRIES     0x0701  /* number of times a device address      */
                                /* should be polled when not            */
                                /* acknowledging                        */
#define I2C_TIMEOUT     0x0702  /* set timeout - call with int          */


/* this is for i2c-dev.c        */
#define I2C_SLAVE       0x0703  /* Change slave address                 */
                                /* Attn.: Slave address is 7 or 10 bits */
#define I2C_SLAVE_FORCE 0x0706  /* Change slave address                 */
                                /* Attn.: Slave address is 7 or 10 bits */
                                /* This changes the address, even if it */
                                /* is already taken!                    */
#define I2C_TENBIT      0x0704  /* 0 for 7 bit addrs, != 0 for 10 bit   */

#define I2C_FUNCS       0x0705  /* Get the adapter functionality */
#define I2C_RDWR        0x0707  /* Combined R/W transfer (one stop only)*/
#define I2C_PEC         0x0708  /* != 0 for SMBus PEC                   */

#define I2C_SMBUS       0x0720  /* SMBus-level access */



/*
 *  * I2C Message - used for pure i2c transaction, also from /dev interface
 *   */
struct i2c_msg {
        __u16 addr;     /* slave address                        */
        unsigned short flags;
#define I2C_M_TEN       0x10    /* we have a ten bit chip address       */
#define I2C_M_RD        0x01
#define I2C_M_NOSTART   0x4000
#define I2C_M_REV_DIR_ADDR      0x2000
#define I2C_M_IGNORE_NAK        0x1000
#define I2C_M_NO_RD_ACK         0x0800
        short len;              /* msg length                           */
        char *buf;              /* pointer to msg data                  */
};


/* This is the structure as used in the I2C_RDWR ioctl call */
struct SI2cRdWrIoctlData {
        struct i2c_msg *msgs;   /* pointers to i2c_msgs */
        int nmsgs;              /* number of i2c_msgs */
};

int inti_cp_i2c();
struct cp_dev_info *get_device_info();
int get_accessory_certificate(char **buf);
int generate_signatire(struct challenge *chall, struct signature *sign);
void get_dev_info(int reg,int size);
