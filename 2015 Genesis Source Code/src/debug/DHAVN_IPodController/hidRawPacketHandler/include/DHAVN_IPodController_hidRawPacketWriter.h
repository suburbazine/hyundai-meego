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
 *                  This is the main class which interact with    *
 *                  the device directly, opens the device, wait   *
 *                  for the notification from the IPOD for the    *
 *                  read, writes to the device etc.               *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef HIDRAWPACKETWRITER_H
#define HIDRAWPACKETWRITER_H

#include <QObject>
#include <qsocketnotifier.h>
#include <libusb-1.0/libusb.h>
#include <DHAVN_IPodController_Def.h>
#include <DHAVN_LogUtil.h>


static const int hidReports[][2] = {
{ 0x0d, 0x05 },
{ 0x0e, 0x09 },
{ 0x0f, 0x0d },
{ 0x10, 0x11 },
{ 0x11, 0x19 },
{ 0x12, 0x31 },
{ 0x13, 0x5f },
{ 0x14, 0xc1 },
{ 0x15, 0xff },
{0,0}
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
class CHIDRawPacketWriter : public QObject
{
    Q_OBJECT
private:
    USE_LOG_UTIL
public:
    CHIDRawPacketWriter(EIPodPorts nCurrrentPortNum);
    int hidWrite(QByteArray hidData);
    int setHidDevice(int vendorId, int productId, QString hidDevPath);
    void onDeviceDisconnected();
    void closeOpenedFileDescriptor();
    ~CHIDRawPacketWriter();
    int DispatchiAPToHid(QByteArray iAPPacketBytes);

#ifdef FILEBASEDAPPHANDLER
    void iPodReadyForApps(bool);
#endif
#ifdef DBUSBASEDAPPHANDLER
    QString getDevHidPath(){return m_DevPath;}
#endif
    int UsbAudioClassSetInterface(int interface_number, int alternate_setting);
    int HIDVendorSpecificRequest(unsigned short request, unsigned short value, short index);

private:
    void setPath(QString hidDevPath);
    hid_device * hid_open(unsigned short vendor_id, unsigned short product_id, char *serial_number);
    hid_device * hid_open_path(const char *path);
    hid_device* new_hid_device();
    int uses_numbered_reports(quint8 *report_descriptor, quint32 size);
    int lookupReport(int size, int& size_out);
    void setMaxReportSize();
    void printOutArray(QByteArray);

public:
    hid_device *m_Handle;
    int m_SocketNumber;
    QString m_DevPath;

    int m_maxReportSize;
private:
    QString m_IPodHidDevicePath;
    QString m_IPodPandoraAppSessionID;
//#ifdef DHAVN_COMPILE_FOR_AHA // added by jungae, 2012.12.01 for AhaRadio application
    QString m_IPodAhaAppSessionID;
//#endif
    bool m_isDisconnected;    // added by sungha.choi 2013.04.15
};


#endif //HIDRAWPACKETWRITER_H
