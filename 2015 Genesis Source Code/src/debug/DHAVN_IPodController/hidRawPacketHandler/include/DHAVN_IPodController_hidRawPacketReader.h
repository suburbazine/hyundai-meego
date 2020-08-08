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
#ifndef HIDRAWPACKETREADER_H
#define HIDRAWPACKETREADER_H

#include <QObject>
#include <qsocketnotifier.h>
#include <libusb-1.0/libusb.h>
#include <DHAVN_LogUtil.h>
#include <DHAVN_IPodController_Def.h>

class CHIDRawPacketReader : public QObject
{
    Q_OBJECT
private:
    USE_LOG_UTIL
public:
    CHIDRawPacketReader(EIPodPorts nCurrentPortNum);
    int setHidDevice(int vendorId, int productId, QString hidDevPath);
    void onDeviceDisconnected();
    void closeOpenedFileDescriptor();
    ~CHIDRawPacketReader();

private:
    void setPath(QString hidDevPath);
    int hid_open(unsigned short vendor_id, unsigned short product_id, char *serial_number);
    int  hid_open_path(const char *path);
    void InitializaSocketNotifier(int socketNumber);
    void printOutArray(QByteArray);

    void constructiAPPacket(QByteArray hidPacket);

public slots:
    void Notified(int socket);
    void requestQuitSlot(); // added by sungha.choi 2013.04.18

signals:
    void iAPPacketReceived(QByteArray hidPacket,bool isLargePacket);

private:
    QSocketNotifier* m_notifier;
    QByteArray m_ReadArray;
    int m_SocketNumber;
    QString m_DevPath;
    bool m_isDisconnected;
};


#endif //HIDRAWPACKETREADER_H
