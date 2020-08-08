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
 *                 This is the main detector class HID
 *                 device connected or removed from the USB       *
 *                 will be notified here and controller will take *
 *                 action according to this.                      *
 *                 emits the signal on connection and removal of  *
 *                 the device                                     *                                           *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef HIDDEVICEDETECTOR_H
#define HIDDEVICEDETECTOR_H

#include <libusb-1.0/libusb.h>
#include <QObject>
#include <qsocketnotifier.h>
#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <DHAVN_LogUtil.h>
#include <DHAVN_IPodController_Def.h> // Added by jonghwan.cho@lge.com 2013.05.06.

class CHidDeviceDetector : public QObject
{
    Q_OBJECT
private:
    USE_LOG_UTIL
public:
    CHidDeviceDetector(QObject* parent,  EIPodPorts portNum = eIPOD_PORT_NONE);
    ~CHidDeviceDetector();
    int registerForTheNotification();
    int startEnumerationAndSetDevicesForHIDMode();

private:
    EIPodPorts getHidPortNum(QString sysPath);

signals:

    void hidDetetcted(QString devPath);

public slots:

    void HidDeviceDetected();
public:
    bool m_DetectionAvailable;  // added by sungha.choi 2013.04.02
    bool m_SavedDetection;

private:
    QSocketNotifier* m_Notifier;
    struct udev *m_Udev;
    struct udev_monitor *m_Mon;
    int m_VendorId;
    int m_ProductId;
    EIPodPorts m_currentIPodPortNum;

};

#endif // HIDDEVICEDETECTOR_H

