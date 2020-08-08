#ifndef DHAVN_APPPANDORADEVICELISTINFO_H
#define DHAVN_APPPANDORADEVICELISTINFO_H
#include <QObject>
#include <QString>
#include <DHAVN_Pndr_Defs.h>
#include <uistate_manager_def.h>

#define CURR 0x01
#define INIT 0x02
#define CONNECT 0x04
#define SUCCESS 0x08

class DeviceInfo : public QObject
{
    Q_OBJECT
    Q_ENUMS(EConnectionType)

public:

    enum EConnectionType
    {
        ENoDevice = DEVICE_TYPE_NONE,
        EBTConn   = DEVICE_TYPE_SMARTPHONE,
        EUSBPort1 = DEVICE_TYPE_IPHONE_USB_FRONT,
        EUSBPort2 = DEVICE_TYPE_IPHONE_USB_REAR
    };

    DeviceInfo(EConnectionType type , USB_PORT_T  inPort = eUSB_PORT_INVALID):
    m_Port(inPort) ,
    m_Type(type)
    {
        m_nTrackToken = 0; m_nStates=0;
        m_nStationTokens = 0;
        m_eSortType = SORT_TYPE_NONE;
        m_nStToken = 0;
    }
    ~DeviceInfo(){}
    quint8 Token(){return m_nTrackToken;}
    void SetToken(quint8 inToken ){m_nTrackToken = inToken;}

    quint8& State(){return m_nStates;}

    void SaveStation(quint32 inStationTokens , ESortType inSortType )
    {
        m_nStationTokens = inStationTokens;
        m_eSortType = inSortType;
    }

    void StationInfo(quint32& inStationTokens , ESortType& inSortType )
    {
        inStationTokens = m_nStationTokens;
        inSortType = m_eSortType ;
    }

    quint32 StToken(){return m_nStToken;}
    void SetStToken(quint32 inToken ){m_nStToken = inToken;}
    void SetSource(QString inSrc);
    void Source(QString inSrc);
    USB_PORT_T port(){return m_Port;}

private:

    quint8 m_nTrackToken;
    quint8 m_nStates;
    quint32 m_nStationTokens ;
    ESortType m_eSortType;
    quint32 m_nStToken;
    QString m_DevSrc;    
    USB_PORT_T  m_Port ;
    EConnectionType m_Type;

};

#endif // DHAVN_APPPANDORADEVICELISTINFO_H
