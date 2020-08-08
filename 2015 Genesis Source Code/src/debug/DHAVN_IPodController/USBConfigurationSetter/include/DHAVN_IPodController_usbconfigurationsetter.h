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
 *                   This is the class which helps the con-       *
 *                   troller to go into the HID mode and set the  *
 *                   configuratuion to desired one.               *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef USBCONFIGURATIONSETTER_H
#define USBCONFIGURATIONSETTER_H

#include <libusb-1.0/libusb.h>
#include <QObject>
#include <DHAVN_LogUtil.h>
class CUsbConfigurationSetter : public QObject
{
    Q_OBJECT
private:
    USE_LOG_UTIL
public:
    CUsbConfigurationSetter(QObject* parent);
    int setDevice(int vendorId, int productId);
    int getConfiguration(libusb_config_descriptor configDesc);
    int setConfiguration(libusb_config_descriptor devConfig);
    void setDeviceResetValue(bool isDeviceResetRequested);
    bool getDeviceResetValue();
private:

    int set_config(libusb_device* dev, libusb_device_handle *devh, int config);
    libusb_device_handle* open_iphone(libusb_device **dev, int vendor_Id, int product_Id);

private:
    libusb_device_handle* m_DevHandler;
    bool m_IsDeviceResetRequested; //Added for CR 14001
};
#endif //USBCONFIGURATIONSETTER_H
