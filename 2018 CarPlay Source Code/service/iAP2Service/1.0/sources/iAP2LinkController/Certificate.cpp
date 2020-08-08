/*
    File:       Certificate.cpp
    Package:    LG iAP2 Service
    Author:     Yongil Park (yongil.park@lge.com)
    Copyright © [2014-2016] by LG Electronics Inc.

    This program or software including the accompanying associated documentation
    (“Software”) is the proprietary software of LG Electronics Inc. and or its
    licensors, and may only be used, duplicated, modified or distributed pursuant
    to the terms and conditions of a separate written license agreement between you
    and LG Electronics Inc. (“Authorized License”). Except as set forth in an
    Authorized License, LG Electronics Inc. grants no license (express or implied),
    rights to use, or waiver of any kind with respect to the Software, and LG
    Electronics Inc. expressly reserves all rights in and to the Software and all
    intellectual property therein. If you have no Authorized License, then you have
    no rights to use the Software in any ways, and should immediately notify LG
    Electronics Inc. and discontinue all use of the Software.
*/

#include "Certificate.h"
#include "iAp2ServiceLog.h"
#ifdef MIBGP
#include "bscommon.h"
#include "bsipc.h"
#endif

#define min(a, b)        ((a) < (b) ? (a) : (b))

#ifdef KOECN
    #define CP_DEV_MAJOR_NUM 237
    #define IOCTL_CP_CTRL_RESET           _IO(CP_DEV_MAJOR_NUM, 102)
#endif

namespace IAP2
{

Certificate::Certificate()
{
    LOGD("Certificate()");
    m_CertificateArr.clear();
#ifdef MIBGP
    bs_global_sem_wait(GLOBAL_SEM_ID_APPLE_CP);
#endif
    initialize();
#ifdef MIBGP
    bs_global_sem_post(GLOBAL_SEM_ID_APPLE_CP);
#endif
}

ByteArray Certificate::getCertificate()
{
    if(m_CertificateArr.length()) {
        return m_CertificateArr;
    }
    else {
        if( getCertFromAuthCp() < -1)
        {
            LOGE("getCertFromAuthCp() Fail");
        }
        return m_CertificateArr;
    }
}


ByteArray Certificate::getDigitalSign(ByteArray challengeBytes)
{
    for( int count = 0; count < 3; count++ ){
        m_signatureBytes.resize(0);
        int ret(getDigitalSignFromCp(challengeBytes));
        
        if(ret > 0) {
            break;
        }
        else {
            LOGE("    DigitalSignature Failed ret[%d] try count[%d]", ret, count);
        }
    }
    return m_signatureBytes;
}

void Certificate::initialize() {
    if(inti_cp_i2c() < 0) {
        goto end;
    }
    if( get_device_info() < 0 ) {
        LOGE("get_device_info failed");
    }
    
    if( getCertFromAuthCp() < 0 ){
        LOGE("getCertFromAuthCp failed");
    }
    return;
    end:
    LOGE("Certificate end failed");
}

int Certificate::inti_cp_i2c()
{
    int ret = 0;
    int checkClose;

    /* Open device */
    LOGD("I2C_DEV_FILE_NAME : %s", I2C_DEV_FILE_NAME);
    int fd = open(I2C_DEV_FILE_NAME, O_RDWR);

    if (fd < 0) {
        LOGE("fd open failed - inti_cp_i2c");
        return -1;
    }

    /* To set the slave address size*/
    ret = ioctl(fd, I2C_TENBIT, 0);
    if (ret) {
        goto initfail;
    }
    /* To set the slave address*/
    ret = ioctl(fd, I2C_SLAVE, I2C_CP_SLAVE_ADDRESS);
    if (ret) {
        goto initfail;
    }
    
    checkClose = close(fd);
    if( checkClose < 0) {
            LOGE("Close return : %d", checkClose);    
    }
    return 0;
    
initfail:
    checkClose = close(fd);
    if( checkClose < 0) {
            LOGE("Close return : %d", checkClose);    
    }
    return -1;
}

int Certificate::get_device_info()
{
    int ret;
    char buf[8] = "";
    int size = 8;
    int addr = DEV_VERSION_REG_ADDR;

    ret = pchI2cRead(addr, buf, size);
    if(ret<=0){
        LOGE("pchI2cRead failed! - get_device_info");
        return -1;
    }

    if(ret == size){
        mDevInfo.devVersion = 0x0000|buf[0];
        mDevInfo.fwVersion = 0x0000|buf[1];
        mDevInfo.majorNum = 0x0000|buf[2];
        mDevInfo.minorNum = 0x0000|buf[3];
        mDevInfo.devId = ((0x0000 | buf[4])<<24)
                           | ((0x0000 | buf[5])<<16)
                           | ((0x0000 | buf[6])<<8)
                           | ((0x0000 | buf[7])) ;
    }
    return 0;
}

int Certificate::getCertFromAuthCp()
{
    int ret,j,count=0;
    unsigned short address =0;
    unsigned short dataSize = 0;
    char rxBuf[ACCES_CERTIFICATE_START_PAGE_SIZE];

    address = ACCES_CERTIFICATE_DATA_LEN_REG_ADDR;
    dataSize = ACCES_CERTIFICATE_DATA_LEN_REG_SIZE;
    ret = pchI2cRead(address,
                       rxBuf,
                       dataSize);
    if(ret < 0) {
        LOGE("pchI2cRead failed__1! - getCertFromAuthCp");
        return -1;
    }
    dataSize = (rxBuf[0]<<8)|(0x00FF&rxBuf[1]);

    for(j=0;j<ACCES_CERTIFICATE_NUM_PAGES;j++){
        int readSize = min(dataSize-count,ACCES_CERTIFICATE_START_PAGE_SIZE);
        if( readSize < 1)
            break;
        address = ACCES_CERTIFICATE_START_PAGE_REG_ADDR+j;
        ret = pchI2cRead(address, rxBuf, readSize);
        if(ret < 0 ) {
            LOGE("pchI2cRead failed__2! - getCertFromAuthCp");
            return -1;
        }

        m_CertificateArr.append(rxBuf, readSize);
        count += readSize;
    }

    return 0;
}

int Certificate::pchI2cRead(unsigned short addr, char *data, int size)
{
    int ret;
    struct SI2cRdWrIoctlData i2cData;
    struct i2c_msg msg[2];
    char tmp[2];
    int checkClose;

    int fd = open(I2C_DEV_FILE_NAME, O_RDWR);
    if(fd < 0) {
        LOGE("Failed to open fd - pchI2cRead");
        return -1;
    }

    //Two operation is necessary for read operation.
    //First determine offset address
    //and then receive data.

    i2cData.msgs = msg;
    i2cData.nmsgs = 2;     // two i2c_msg

    tmp[0] = addr;
    //read_cp_hexValue.setNum(addr,16);

    i2cData.msgs[0].addr = I2C_CP_SLAVE_ADDRESS;
    i2cData.msgs[0].flags = 0;     // write
    i2cData.msgs[0].len = 1;        // only one byte
    i2cData.msgs[0].buf = (char *)tmp;
    //      Second, read data from the EEPROM
    i2cData.msgs[1].addr = I2C_CP_SLAVE_ADDRESS;
    i2cData.msgs[1].flags = I2C_M_RD;        // read command
    i2cData.msgs[1].len = size;
    i2cData.msgs[1].buf = data;

    int retryCount = 0;
    do {
        ret = ioctl(fd, I2C_RDWR, &i2cData);
        retryCount++;
        //LOGD("    ret[%d] retryCount[%d]", ret, retryCount);
        usleep(10*1000);
    } while (ret == -1 && retryCount < 100);
    
    checkClose = close(fd);
    if( checkClose < 0) {
            LOGE("Close return : %d", checkClose);    
    }
    
    usleep(USLEEP_TIME);
    if (ret < 0) {
// ADD CP RESET FOR KOECN //
#ifdef KOECN
        LOGE(" KOECN CP RESET ");
        int cp_fd = -1;
        int cp_ret = -1;

        cp_fd = open("/dev/lg-cp", O_RDWR);
        if (cp_fd < 0) {
            LOGE("Fail to open lg_cp device");
            return ret;
        }
        cp_ret = ioctl(cp_fd, IOCTL_CP_CTRL_RESET, 0);
        if (cp_ret < 0) {
            goto initfail;
        }
        LOGD("send GPIO 16 Pin OFF");
        usleep(500); //in CP Spec, Warm Reset : 200us, Cold Reset 500us
        cp_ret = ioctl(cp_fd, IOCTL_CP_CTRL_RESET, 1);
        if (cp_ret < 0) {
            goto initfail;
        }
        LOGD("send GPIO 16 Pin ON");
        checkClose = close(cp_fd);
        if ( checkClose < 0 ) {
            LOGE(" %d Close return : %d", __LINE__, checkClose);
        }
        usleep(30*1000);
initfail:
    checkClose = close(cp_fd);
    if ( checkClose < 0 ) {
        LOGE(" %d Close return : %d", __LINE__, checkClose);
    }
#endif
        return ret;
    }
    return size;
}


int Certificate::getDigitalSignFromCp(ByteArray chall)
{
    int ret,i;
    unsigned short dataSize,len;
    unsigned short addr;
    char buf[128];
    //QString hexValue_addr;
    //QString hexValue_data_size;
    //QString sign_read;

    /* Writing challenge length to Cp */
    addr = CHALLENGE_DATA_LEN_REG_ADDR;
    dataSize = CHALLENGE_DATA_LEN_SIZE;
    len = chall.length();
    buf[1] = len & 0x00FF;
    buf[0] = (len & 0xFF00)>>8;
    ret = pchI2cWrite(addr, buf, dataSize);
    if(ret<0)
        return ret;

    /* Wring challenge data to CP */
    addr = CHALLENGE_DATA_REG_ADDR;
    dataSize = len;
    memcpy(buf,chall.data(),len);
    ret = pchI2cWrite(addr, buf, dataSize);
    if(ret<0)
        return ret;
    addr = SIG_DATA_LEN_REG_ADDR;
    dataSize = SIG_DATA_LEN_REG_SIZE;
    buf[1] = 0x80;
    buf[0] = 0x00;
    //hexValue_addr.setNum(addr,16);
    ret = pchI2cWrite( addr, buf, dataSize);
    if(ret<0)
        return ret;
    addr = AUTH_CONTROL_STATUS_REG_ADDR;
    dataSize = AUTH_CONTROL_STATUS_SIZE;
    buf[0] = 0x01;
    //hexValue_addr.setNum(addr,16);
    ret = pchI2cWrite(addr, buf, dataSize);
    if(ret<0){
        LOGE("Failed pchI2cWrite - getDigitalSignFromCp");
        return ret;
    }
    i = 0;
    ret = 0;
    while(i<50) {
        short status=0;
        short err = 0;
        char reg;
        usleep(USLEEP_TIME);
        ret = pchI2cRead(addr, buf, dataSize);
        if(ret <= 0) {
            ++i;
            continue;
        }
        reg = buf[0];
        err = (reg >>7)& 0x01;
        status = (reg >> 4) & 0x07;
        if(err == 1 && status == 0){
            addr = ERROR_CODE_REG_ADDR;
            dataSize = ERROR_CODE_REG_SIZE;
            //ret = pchI2cRead(addr, buf, dataSize); change code for StaticAnalysis
            //ret = -1;
            ret = pchI2cRead(addr, buf, dataSize); // change code for StaticAnalysis
            if(ret<0) {        
                LOGE("Failed pchI2cRead__3 - getDigitalSignFromCp");
            }
            break;
        }
        if(err == 0 && status == 1){
            //ret = 1;                        change code for StaticAnalysis
            break;
        }
        i++;
    }
    addr = SIG_DATA_LEN_REG_ADDR;
    dataSize = SIG_DATA_LEN_REG_SIZE;
    ret = pchI2cRead(addr, buf, dataSize);
    if(ret<0) {        
        LOGE("Failed pchI2cRead - getDigitalSignFromCp");
        return ret;
    }
    dataSize = ((0x0000|buf[0])<< 8) | (0x0000|buf[1]);
    //hexValue_data_size.setNum(dataSize,16);
    if(dataSize > SIG_DATA_REG_SIZE)
        dataSize = SIG_DATA_REG_SIZE;

    addr = SIG_DATA_REG_ADDR;
    ret = pchI2cRead(addr, buf, dataSize);
    if(ret<0){
        LOGE("Failed pchI2cRead__2 - getDigitalSignFromCp");
        return ret;
    }
    //sign_read.setNum(ret,16);
    if(ret > 0) {
        m_signatureBytes.append(buf, dataSize);// append(buf);
    }
    return ret;
}


int Certificate::pchI2cWrite(unsigned short addr, char *data, int size)
{
    int ret;
    struct SI2cRdWrIoctlData i2cData;
    struct i2c_msg msg[1];
    unsigned char buf[size+1];
    int checkClose;
    int fd = open(I2C_DEV_FILE_NAME, O_RDWR);

    if(fd < 0) {
        LOGE("ERROR : fail to open fd[%d]", fd);
        return -1;
    }
    buf[0] = addr;
    memcpy(&buf[1], data, size);

    i2cData.nmsgs = 1;             // use one message structure
    i2cData.msgs = msg;
    i2cData.msgs[0].addr = I2C_CP_SLAVE_ADDRESS;
    i2cData.msgs[0].flags = 0;     // don't need flags
    i2cData.msgs[0].len = size+1;
    i2cData.msgs[0].buf = (char *)buf;

    if((ret = ioctl(fd, I2C_RDWR, &i2cData)) < 0) {
        checkClose = close(fd);
        if( checkClose < 0) {
            LOGE("Close return : %d", checkClose);    
        }
            
        return ret;
    }
    checkClose = close(fd);
    if( checkClose < 0) {
            LOGE("Close return : %d", checkClose);    
    }
    usleep(USLEEP_TIME);
    return size+1;
}

} // namespace IAP2
