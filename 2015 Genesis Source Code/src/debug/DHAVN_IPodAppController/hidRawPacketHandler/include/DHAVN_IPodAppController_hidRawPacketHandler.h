/*
 ******************************************************************
 *        COPYRIGHT © Teleca AB                                   *
 *----------------------------------------------------------------*
 * MODULE     :  IPodAppController                                *
 *
 * PROGRAMMERS : Pawan Gupta                                      *
 * DATE       :  16 jan 2012                                      *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :

 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef HIDRAWPACKETHANDLER_H
#define HIDRAWPACKETHANDLER_H

#include <QObject>
#include <qsocketnotifier.h>
#include <DHAVN_IPodAppController_CommonVars.h>
#include <DHAVN_LogUtil.h>

enum TypeOfData
{
    ReadData=0,
    WriteData
};

class hid_device {
public:
    ~hid_device()
    {
        if(device_handle > 0)
            close(device_handle);
    }
    int device_handle;
    int blocking;
    int uses_numbered_reports;
};

/** hidapi info structure */
class hid_device_info {
public:
    /** Platform-specific device path */
    char *path;
    /** Device Vendor ID */
    unsigned short vendor_id;
    /** Device Product ID */
    unsigned short product_id;
    /** Serial Number */
    char *serial_number;
    /** Device Release Number in binary-coded decimal,
                        also known as Device Version Number */
    unsigned short release_number;
    /** Manufacturer String */
    char *manufacturer_string;
    /** Product string */
    char *product_string;
    /** Usage Page for this Device/Interface
                        (Windows/Mac only). */
    unsigned short usage_page;
    /** Usage for this Device/Interface
                        (Windows/Mac only).*/
    unsigned short usage;
    /** The USB interface which this logical device
                        represents (Linux/libusb implementation only). */
    int interface_number;

    /** Pointer to the next device */
    struct hid_device_info *next;
};
class CHIDRawPacketHandle : public QObject
{
    USE_LOG_UTIL
    Q_OBJECT
public:
    CHIDRawPacketHandle(QSocketNotifier::Type type);
    EIPodAppErrors hid_open(const QString& hidPath);
    int read(QByteArray& hidData);
    int write(QByteArray& hidData);
    ~CHIDRawPacketHandle();
private:
    hid_device * hid_open_path(const char *path);
    hid_device* new_hid_device();
    int uses_numbered_reports(quint8 *report_descriptor, quint32 size);
    void InitializaSocketNotifier(int socketNumber);
public slots:
    void Notified(int socket);
    void writeToSock(QByteArray hidData);

signals:

    void hidPacketReceived(QByteArray hidPacket);

    void emitToWrite(QByteArray hidData);

public:
    QSocketNotifier* m_notifier;
    QSocketNotifier::Type m_Type;
    hid_device *m_Handle;
    int m_SocketNumber;
};


#endif //HIDRAWPACKETHANDLER_H
