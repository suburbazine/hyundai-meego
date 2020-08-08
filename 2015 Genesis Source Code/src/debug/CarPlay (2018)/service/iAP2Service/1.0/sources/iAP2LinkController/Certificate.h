/*
    File:       Certificate.h
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

#ifndef CERTIFICATE_H_
#define CERTIFICATE_H_


//#include <QObject>
#include <IAP2PredefinedValues.h>
#include <fcntl.h>
//#include <QDebug>
#include <sys/ioctl.h>
#include "ByteArray.h"

namespace IAP2
{

    struct CpDevInfo {
        int devVersion;
        int fwVersion;
        int majorNum;
        int minorNum;
        int devId;
    };

    class Certificate {

        public:
            Certificate();
            ByteArray getCertificate();
            ByteArray getDigitalSign(ByteArray challengeBytes);
        private:
            void initialize();
            int inti_cp_i2c();
            int get_device_info();

            int getCertFromAuthCp();
            int getDigitalSignFromCp(ByteArray chall);

            int pchI2cRead(unsigned short addr, char *data, int size);
            int pchI2cWrite(unsigned short addr, char *data, int size);

            CpDevInfo mDevInfo;

            ByteArray m_CertificateArr;
            ByteArray m_signatureBytes;

        public :
        
    };

} // namespace IAP2

#endif // #ifndef CERTIFICATE_H_