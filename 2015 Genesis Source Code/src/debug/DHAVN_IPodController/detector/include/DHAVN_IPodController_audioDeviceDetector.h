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
#ifndef AUDIODEVICEDETECTOR_H
#define AUDIODEVICEDETECTOR_H

#include <libusb-1.0/libusb.h>
#include <QObject>
#include <qsocketnotifier.h>
#include <libudev.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <DHAVN_LogUtil.h>
#include <DHAVN_IPodController_hidDeviceDetector.h>

class CAudioDeviceDetector : public QObject
{
    Q_OBJECT
private:
    USE_LOG_UTIL
public:
    CAudioDeviceDetector(QObject* parent,  EIPodPorts portNum = eIPOD_PORT_NONE);
    ~CAudioDeviceDetector();
    int registerForTheNotification();
private:
    EIPodPorts getAudioPortNum(QString sysPath);
signals:

    void audioDetected(QString devPath);
	/*
	 * Added by jonghwan.cho@lge.com	2013.03.21.
	 * for changing UI Transition Procedure
	 */
	void onIPodAudioDeviceConnectedSignal( QString, QString, EIPodPorts );
	/*
	 * Added by jonghwan.cho@lge.com	2013.03.21.
	 * for changing UI Transition Procedure
	 */

public slots:

    void deviceDetected();//Callback from udev.
public:
    bool m_DetectionAvailable;  // added by sungha.choi 2013.04.02
    bool m_SavedDetection;

private:
    QSocketNotifier* m_Notifier;
    struct udev *m_Udev;
    struct udev_monitor *m_Mon;
    EIPodPorts m_currentPortNum;
};

#endif // AUDIODEVICEDETECTOR_H

