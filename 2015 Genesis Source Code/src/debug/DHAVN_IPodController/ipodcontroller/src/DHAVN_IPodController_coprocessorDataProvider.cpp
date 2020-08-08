/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini Philip / Venkataramana      *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                 The class which provides the data from the co- *
 *                 processor chip, it help to get the certificate *
 *                 , Signature to authorize the IPOD.             *
 *                 and also provide the details like device ID,   *
 *                 version numbers etc.                           *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <fcntl.h>
#include <linux/ioctl.h>
#include <sys/ioctl.h>
#include <string>
#include <DHAVN_IPodController_coprocessorDataProvider.h>
#include <DHAVN_IPodController_CommonVars.h>
#include <stdio.h>
#include <qthread.h>
#include <DHAVN_IPodController_PrivateData.h>

#define min(a, b)		((a) < (b) ? (a) : (b))

CCoProcessorDataProvider::CCoProcessorDataProvider(QObject* parent) : QObject(parent)
{
    iPodControllerPrivateData *iPodPrivateData = parent->findChild<iPodControllerPrivateData*>(gControllerPrivateData);
    EIPodPorts portNum;
    portNum = iPodPrivateData->getiPodPortNum();
    switch(portNum)
    {
        case eIPOD_PORT_1:
            LOG_INIT_V2(giPodControllerLogName_1);
            break;
        case eIPOD_PORT_2:
            LOG_INIT_V2(giPodControllerLogName_2);
            break;
        default:
            LOG_INIT_V2(giPodControllerLogName);
    }
    //LOG_INIT_V2(gControllerLogName);
    //LOG_TRACE << "CCoProcessorDataProvider::CCoProcessorDataProvider() IN" << LOG_ENDL;
    setObjectName(gCoprocessorDataProvider);
    initialize();
    //LOG_TRACE << "CCoProcessorDataProvider::CCoProcessorDataProvider() OUT" << LOG_ENDL;
}

CCoProcessorDataProvider::~CCoProcessorDataProvider()
{
    if(m_FileDesc > 0)
        close(m_FileDesc);
}

/******************************************************************************
// Function                                            Parameters
// CCoProcessorDataProvider::digitalSignature          QByteArray (OUT), (QByteArray) (IN)
//
// Explanation
// returns the digital signature prepared on the basis of the chanllenge bytes
// provided by the caller
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
QByteArray CCoProcessorDataProvider::digitalSignature(QByteArray challengeBytes)
{
    //LOG_TRACE << "CCoProcessorDataProvider::digitalSignature() IN" << LOG_ENDL;
    for( int count = 0 ; count < 3 ; count++ )
    {
        signatureBytes.resize(0);
        int ret(getDigitalSignature(challengeBytes));
        if(ret > 0)
        {
            //LOG_TRACE << "   Success, " << "ret = "<< ret << ", try count = " << count << LOG_ENDL;
            break;
        }
        else
        {
            LOG_HIGH << "CCoProcessorDataProvider::digitalSignature, DigitalSignature Failed = " << ret << ", try count= "<< count << LOG_ENDL;
        }
    }
    //LOG_TRACE << "CCoProcessorDataProvider::digitalSignature() OUT" << LOG_ENDL;
    return signatureBytes;
}

/******************************************************************************
// Function                                            Parameters
// CCoProcessorDataProvider::initialize          void (OUT), (void) (IN)
//
// Explanation
// initalizes the I2C and gets the device info
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CCoProcessorDataProvider::initialize()
{
    ////LOG_TRACE << "CCoProcessorDataProvider::initialize() Entry " << LOG_ENDL;
    if(inti_cp_i2c())
        goto end;
    get_device_info();
    getCertificate();
    return;
    end:
    return;
    //LOG_TRACE << "CCoProcessorDataProvider::initialize()" << LOG_ENDL;
}

int CCoProcessorDataProvider::inti_cp_i2c()
{
    ////LOG_TRACE << "CCoProcessorDataProvider::inti_cp_i2c() Entry " << LOG_ENDL;
    int ret = 0;

    /* Open device */
    m_FileDesc = open(I2C_DEV_FILE_NAME, O_RDWR);
    if (m_FileDesc < 0)
    {
        return -1;
    }

    /* To set the slave address size*/
    ret = ioctl(m_FileDesc, I2C_TENBIT, 0);
    if (ret)
    {
//        //LOG_TRACE << "ERROR: IOCTL I2C_TENBIT failed to set address size " << LOG_ENDL;
        goto initfail;
    }
    /* To set the slave address*/
    ret = ioctl(m_FileDesc, I2C_SLAVE, I2C_CP_SLAVE_ADDRESS);
    if (ret)
    {
//        //LOG_TRACE << "ERROR: IOCTL I2C_SLAVE failed " << LOG_ENDL;
        goto initfail;
    }
//    //LOG_TRACE << "CCoProcessorDataProvider::inti_cp_i2c() Exit " << LOG_ENDL;
    return 0;
initfail:
    close(m_FileDesc);
    return -1;
}


int CCoProcessorDataProvider::get_device_info()
{
//    //LOG_TRACE << "CCoProcessorDataProvider::get_device_info() Entry " << LOG_ENDL;
    int ret;
    char buf[8] = "";
    int size = 8;
    int addr = DEV_VERSION_REG_ADDR;

//    //LOG_INFO << "CCoProcessorDataProvider::get_device_info : DeviceInfo - " <<m_FileDesc<<LOG_ENDL;
    ret = pch_i2c_read(addr, buf, size);
    if(ret<=0){
//        //LOG_INFO << "CCoProcessorDataProvider::get_device_info : failed to read the dev info form Reg=%#02x - "
//                 <<DEV_VERSION_REG_ADDR<<LOG_ENDL;
        return -1;
    }

    if(ret == size){
        m_DevInfo.dev_version = 0x0000|buf[0];
        m_DevInfo.fw_version = 0x0000|buf[1];
        m_DevInfo.major_num = 0x0000|buf[2];
        m_DevInfo.minor_num = 0x0000|buf[3];
        m_DevInfo.dev_id = ((0x0000 | buf[4])<<24)
                           | ((0x0000 | buf[5])<<16)
                           | ((0x0000 | buf[6])<<8)
                           | ((0x0000 | buf[7])) ;

    }
//    //LOG_TRACE << "CCoProcessorDataProvider::get_device_info() Exit " << LOG_ENDL;
    return 0;
}

/******************************************************************************
// Function                                            Parameters
// CCoProcessorDataProvider::getCertificate          void (OUT), (void) (IN)
//
// Explanation
// gets the certificate from the device and sets it to m_CertificateArr for future use.
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
void CCoProcessorDataProvider::getCertificate()
{
//    //LOG_TRACE << "CCoProcessorDataProvider::getCertificate() Entry " << LOG_ENDL;
    int ret,j,count=0;
    unsigned short address =0;
    unsigned short data_size = 0;
    char rx_buf[ACCES_CERTIFICATE_START_PAGE_SIZE];
    QString hexValue_data_size;

    address = ACCES_CERTIFICATE_DATA_LEN_REG_ADDR;
    data_size = ACCES_CERTIFICATE_DATA_LEN_REG_SIZE;
//    //LOG_TRACE << "CCoProcessorDataProvider::getCertificate() 1 " << LOG_ENDL;
    ret = pch_i2c_read(address,
                       rx_buf,
                       data_size);
    if( ret < 0 )
    {
//        //LOG_TRACE << "failed to fetch the certificate length" << LOG_ENDL;
        return;
    }
//    //LOG_TRACE << "CCoProcessorDataProvider::getCertificate() 2 " << LOG_ENDL;
    data_size = (rx_buf[0]<<8)|(0x00FF&rx_buf[1]);
    hexValue_data_size.setNum(data_size,16);
//    //LOG_INFO << " Size of the certificate: =0x " << hexValue_data_size << LOG_ENDL;

    for(j=0;j<ACCES_CERTIFICATE_NUM_PAGES;j++){
        int read_size = min(data_size-count,ACCES_CERTIFICATE_START_PAGE_SIZE);
        if( read_size < 1)
            break;
        address = ACCES_CERTIFICATE_START_PAGE_REG_ADDR+j;
        ret = pch_i2c_read(address,
                           rx_buf,
                           read_size);
        if(ret < 0 )
        {
//            //LOG_TRACE << "failed to fetch the certificate" << LOG_ENDL;
            return;
        }

        m_CertificateArr.append(rx_buf, read_size);
        count += read_size;
    }
//    //LOG_INFO << " Size of the buf: " << m_CertificateArr.length() << LOG_ENDL;
    m_CerLen =  m_CertificateArr.length();
//    //LOG_TRACE << "CCoProcessorDataProvider::getCertificate() Exit " << LOG_ENDL;
    return;
}
int CCoProcessorDataProvider::pch_i2c_read(unsigned short addr, char *data, int size)
{
//    //LOG_TRACE << "CCoProcessorDataProvider::pch_i2c_read() entry " << LOG_ENDL;
    int ret;
    struct SI2cRdWrIoctlData i2c_data;
    struct i2c_msg msg[2];
    char tmp[2];
    QString read_cp_hexValue;

    //Two operation is necessary for read operation.
    //First determine offset address
    //and then receive data.

    i2c_data.msgs = msg;
    i2c_data.nmsgs = 2;     // two i2c_msg

    tmp[0] = addr;
    read_cp_hexValue.setNum(addr,16);
//    //LOG_INFO << "CCoProcessorDataProvider::pch_i2c_read: read_cp: reading from Reg=0x " << read_cp_hexValue <<LOG_ENDL;
    i2c_data.msgs[0].addr = I2C_CP_SLAVE_ADDRESS;
    i2c_data.msgs[0].flags = 0;     // write
    i2c_data.msgs[0].len = 1;       // only one byte
    i2c_data.msgs[0].buf = (char *)tmp;
    //    Second, read data from the EEPROM
    i2c_data.msgs[1].addr = I2C_CP_SLAVE_ADDRESS;
    i2c_data.msgs[1].flags = I2C_M_RD;      // read command
    i2c_data.msgs[1].len = size;
    i2c_data.msgs[1].buf = data;
    ret = ioctl(m_FileDesc, I2C_RDWR, &i2c_data);
    usleep(USLEEP_TIME);
//    //LOG_INFO << " read_cp: read,ret= " << ret << LOG_ENDL;
    if (ret < 0) {
//        //LOG_TRACE << "read data fail" << LOG_ENDL;
        return ret;
    }
//    //LOG_TRACE << "CCoProcessorDataProvider::pch_i2c_read() Exit " << LOG_ENDL;
    return size;

}


int CCoProcessorDataProvider::pch_i2c_write(unsigned short addr, char *data, int size)
{
//    //LOG_TRACE << "CCoProcessorDataProvider::pch_i2c_write Entry " << LOG_ENDL;
    int ret;
    struct SI2cRdWrIoctlData i2c_data;
    struct i2c_msg msg[1];
    unsigned char buf[size+1];
//    //LOG_INFO << " in CCoProcessorDataProvider::pch_i2c_write fd = " << m_FileDesc << LOG_ENDL;

    buf[0] = addr;
    memcpy(&buf[1], data, size);

//    //LOG_INFO << "write_cp: writing" << size+1 << "bytes ++" <<LOG_ENDL;
    i2c_data.nmsgs = 1;             // use one message structure
    i2c_data.msgs = msg;
    i2c_data.msgs[0].addr = I2C_CP_SLAVE_ADDRESS;
    i2c_data.msgs[0].flags = 0;     // don't need flags
    i2c_data.msgs[0].len = size+1;
    i2c_data.msgs[0].buf = (char *)buf;
    if((ret = ioctl(m_FileDesc, I2C_RDWR, &i2c_data)) < 0) {
//        //LOG_TRACE << "write data fail\n" << LOG_ENDL;
        return ret;
    }
    usleep(USLEEP_TIME);
//    //LOG_INFO << " write_cp: write ret= " << ret << LOG_ENDL;
//    //LOG_TRACE << "CCoProcessorDataProvider::pch_i2c_write Exit " << LOG_ENDL;
    return size+1;
}

/******************************************************************************
// Function                                            Parameters
// CCoProcessorDataProvider::getDigitalSignature          void (OUT), (QByteArray) (IN)
//
// Explanation
// gets the digital signature from the device and sets it to signatureBytes for future use.
// History
// 09-Sep-2011      Pawan Gupta                        Added initial implementation
*****************************************************************************/
int CCoProcessorDataProvider::getDigitalSignature(QByteArray chall)
{
//    //LOG_TRACE << "CCoProcessorDataProvider::getDigitalSignature Entry " << LOG_ENDL;
    int ret,i;
    unsigned short data_size,len;
    unsigned short addr;
    char buf[128];
    QString hexValue_addr;
    QString hexValue_data_size;
    QString sign_read;

    /* Writing challenge length to Cp */
    addr = CHALLENGE_DATA_LEN_REG_ADDR;
    data_size = CHALLENGE_DATA_LEN_SIZE;
    len = chall.length();
//    //LOG_INFO << " lenght of chall: = " << len << LOG_ENDL;
    buf[1] = len & 0x00FF;
    buf[0] = (len & 0xFF00)>>8;
    ret = pch_i2c_write(addr, buf, data_size);
    if(ret<0)
        return ret;

    /* Wring challenge data to CP */
    addr = CHALLENGE_DATA_REG_ADDR;
    data_size = len;
    memcpy(buf,chall.data(),len);
//    //LOG_INFO << " Writing Challenge data to CP reg=0x " << hexValue_addr << LOG_ENDL;
    ret = pch_i2c_write(addr, buf, data_size);
    if(ret<0)
        return ret;
//    //LOG_INFO << " Writen challenge ret = " << ret << LOG_ENDL;
    addr = SIG_DATA_LEN_REG_ADDR;
    data_size = SIG_DATA_LEN_REG_SIZE;
    buf[1] = 0x80;
    buf[0] = 0x00;
    hexValue_addr.setNum(addr,16);
//    //LOG_INFO << " Writing 0x80 to SIG_DATA_LEN_REG  CP reg=0x " << hexValue_addr << LOG_ENDL;
    ret = pch_i2c_write( addr, buf, data_size);
    if(ret<0)
        return ret;
//    //LOG_INFO << "Writen, ret=" << ret << LOG_ENDL;

    /* Write Authentication control register PROC_CONTROL=1 to start signature generation process */
    addr = AUTH_CONTROL_STATUS_REG_ADDR;
    data_size = AUTH_CONTROL_STATUS_SIZE;
    buf[0] = 0x01;
    hexValue_addr.setNum(addr,16);
//    //LOG_INFO <<"Writing 0x01 to auth status control to CP reg=0x" << hexValue_addr << LOG_ENDL;
    ret = pch_i2c_write(addr, buf, data_size);
    if(ret<0)
        return ret;
//    //LOG_INFO <<"Written, ret=" << ret << LOG_ENDL;
    i = 0;
    ret = 0;
    while(i<50)
    {
        short status=0;
        short err = 0;
        char reg;
        usleep(USLEEP_TIME);
        ret = pch_i2c_read(addr, buf, data_size);
        if(ret <= 0) {
            ++i;
            continue;
        }
        reg = buf[0];
        err = (reg >>7)& 0x01;
        status = (reg >> 4) & 0x07;
        if(err == 1 && status == 0){
//            //LOG_TRACE << "Error .......!!\n" << LOG_ENDL;
            addr = ERROR_CODE_REG_ADDR;
            data_size = ERROR_CODE_REG_SIZE;
            ret = pch_i2c_read(addr, buf, data_size);
            ret = -1;
            break;
        }
        if(err == 0 && status == 1){
//            //LOG_TRACE << "Successfully generated the signature !!" << LOG_ENDL;
            ret = 1;
            break;
        }
        i++;
    }
//    //LOG_TRACE << "generate_signature:Reading the Signature from CP" << LOG_ENDL;
    addr = SIG_DATA_LEN_REG_ADDR;
    data_size = SIG_DATA_LEN_REG_SIZE;
    ret = pch_i2c_read(addr, buf, data_size);
    if(ret<0)
        return ret;
    data_size = ((0x0000|buf[0])<< 8) | (0x0000|buf[1]);
    hexValue_data_size.setNum(data_size,16);
//    //LOG_INFO <<"Signature Size=0x" << hexValue_data_size << LOG_ENDL;
    if(data_size > SIG_DATA_REG_SIZE)
        data_size = SIG_DATA_REG_SIZE;

    addr = SIG_DATA_REG_ADDR;
    ret = pch_i2c_read(addr, buf, data_size);
    if(ret<0)
        return ret;
    sign_read.setNum(ret,16);
//    //LOG_INFO << "generate_signature: Sign Read 0x:" << sign_read << " Bytes" <<LOG_ENDL;
    if( ret > 0 )
    {
        signatureBytes.append(buf, data_size);// append(buf);
    }
//    //LOG_TRACE << "CCoProcessorDataProvider::getDigitalSignature Exit " << LOG_ENDL;
    return ret;
}
