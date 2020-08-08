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
#ifndef COPROCESSORDATAPROVIDER_H
#define COPROCESSORDATAPROVIDER_H

#include <QObject>
#include <DHAVN_IPodController_i2c-cp.h>
#include <QDebug>
#include <DHAVN_LogUtil.h>

class CCoProcessorDataProvider : public QObject
{

    class cp_dev_info{
    public:
        int dev_version;
        int fw_version;
        int major_num;
        int minor_num;
        int dev_id;
    };

private:
    USE_LOG_UTIL

public:
    CCoProcessorDataProvider(QObject* parent);
    int deviceId(){return m_DevInfo.dev_id;}
    /******************************************************************************
    // Function                                            Parameters
    // CCoProcessorDataProvider::certificate          QByteArray (OUT), (void) (IN)
    //
    // Explanation
    // returns the certificate if it is available else create it and return
    // History
    // 09-Sep-2011      Pawan Gupta                        Added initial implementation
    *****************************************************************************/
    QByteArray certificate()
    {
                if(m_CertificateArr.length())
                {
                    //LOG_TRACE << "certificate found\n" << LOG_ENDL;
                    return m_CertificateArr;
                }
                else
                {
                  //LOG_TRACE <<"getting certificate Entry" << LOG_ENDL;
                  getCertificate();
                  //LOG_TRACE <<"getting certificate Exit" << LOG_ENDL;
                  return m_CertificateArr;
                }

        }
    int cerificateLength(){return m_CerLen;}
    QByteArray digitalSignature(QByteArray challengeBytes);
    ~CCoProcessorDataProvider();
    void getCertificate();

private:
    void initialize();
    int inti_cp_i2c();
    int get_device_info();
    int pch_i2c_read(unsigned short addr, char *data, int size);
    int pch_i2c_write(unsigned short addr, char *data, int size);
    int getDigitalSignature(QByteArray chall);

private:
    QByteArray m_CertificateArr;
    int m_CerLen;
    int m_FileDesc;
    cp_dev_info m_DevInfo;
    QByteArray signatureBytes;
};

#endif // COPROCESSORDATAPROVIDER_H
