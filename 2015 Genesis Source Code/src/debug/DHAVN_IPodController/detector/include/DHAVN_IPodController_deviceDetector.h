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
 *                 This is the main detector class any
 *                 device connected or removed from the USB       *
 *                 will be notified here and controller will take *
 *                 action according to this.                      *
 *                 emits the signal on connection and removal of  *
 *                 the device                                     *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef DEVICEDETECTOR_H
#define DEVICEDETECTOR_H

#include <libusb-1.0/libusb.h>
#include <QObject>
#include <qsocketnotifier.h>
#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <DHAVN_IPodController_Def.h>   // Added by jonghwan.cho@lge.com   2013.05.06.
#include <DHAVN_IPodController_hidDeviceDetector.h>
#include <DHAVN_IPodController_audioDeviceDetector.h>
#include <DHAVN_IPodController_usbconfigurationsetter.h>
#include <DHAVN_LogUtil.h>
#include <QTimer>
#include <DHAVN_iPodController_Logger.h>    // added by sungha.choi 2013.04.12

enum EDetectorState
{
    eiPodDeviceDisconnected=0,
    eiPodDeviceConnected,
    eiPodHidDeviceConnected
};

class CDeviceDetector : public QObject
{
    Q_OBJECT
private:
    USE_LOG_UTIL
public:
    CDeviceDetector(QObject* parent, EIPodPorts portNum = eIPOD_PORT_NONE);
    ~CDeviceDetector();
    int registerForTheNotification();
    QString getiPodAudioDevPath();
    void resetIPod();
    void setDeviceResetValue(bool isDeviceReset);
    bool getDeviceResetValue();
    /*
     * Added by jonghwan.cho@lge.com	2013.03.18.
     * for plughw setting while boot up
     */
    bool    FindDevice();
    void    iPodDeviceDisconnectionComplete();    // added by sungha.choi 2013.04.12
    bool    m_DeviceConnected;
    bool    m_isAudioPathSet;//To suppress the multiple signal when connected.

    EDetectorState GetDetectorState();
private:
    bool isUnsupportedDevice(int nProductId);
    bool isAppleDeviceConnected();
    void onDeviceDisconnect();
    EIPodPorts getPortNum(QString sysPath);

signals:
    void iPodDeviceDetetcted(int vendorId, int productId);
    void iPodDeviceDisconnected();
#ifdef CONNECT_BOOTUP
    void IPodConnectStateForBootupSignal( bool );
#endif

    void iPodHidDetected(bool status,QString hidDevicePath);
    void iPodAudioDeviceDetected(QString deviceName);
    void onIPodDeviceConnectedSignal( QString, QString, EIPodPorts );
    void destroyTaskHandler();    // added by sungha.choi 2013.04.12

public slots:
    void iPodDeviceDetectedSlot();
    void hidDeviceDetetcedSlot(QString path);
    void audioDeviceDetetcedSlot(QString path);
    void startEnumerationAndSetDevices();
    void requestDestroyTask();    // added by sungha.choi 2013.04.02
    void onIPodAudioDeviceConnectedSLOT( QString, QString, EIPodPorts );

private:
    EDetectorState m_detectorState;
    QSocketNotifier* m_Notifier;
    struct udev *m_Udev;
    struct udev_monitor *m_Mon;

    CHidDeviceDetector* m_HidDevDetector;
    CAudioDeviceDetector* m_AudioDevDetector;
    CUsbConfigurationSetter* m_UsbConfSetter;
    int m_VendorId;
    int m_ProductId;
    bool m_DisconnectCompletion;    // added by sungha.choi 2013.04.02

    bool m_DetectionAvailable;  // added by sungha.choi 2013.04.02
    bool m_SavedDetection;

    QString m_iPodAudioDevPath;
    QString m_NodePath; // Added for CR 14001 
    bool m_IsDeviceReset;// Added for CR 14001 
public:
    EIPodPorts m_currentIPodPortNum;
    iPodLogger *piPodLog;   // added by sungha.choi 2013.04.12
};
#endif //DEVICEDETECTOR_H
