#ifndef DHAVN_APPAHADEVICELISTINFO_H
#define DHAVN_APPAHADEVICELISTINFO_H

#include <QObject>
#include "ahaCommonUtility.h"

class DeviceInfo : public QObject
{
    Q_OBJECT
    Q_ENUMS(EConnectionType)

public:

    enum EConnectionType
    {
        ENoDevice = DEVICE_TYPE_NONE,
        EBTConn = DEVICE_TYPE_SMARTPHONE,
        EUSBPort1 = DEVICE_TYPE_IPHONE_USB_FRONT,
        EUSBPort2 = DEVICE_TYPE_IPHONE_USB_REAR
    };

    explicit DeviceInfo(){}
    ~DeviceInfo(){}
};

#endif // DHAVN_APPAHADEVICELISTINFO_H
