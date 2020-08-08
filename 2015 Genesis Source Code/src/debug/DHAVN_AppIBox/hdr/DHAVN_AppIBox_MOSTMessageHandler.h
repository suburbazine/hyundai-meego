#ifndef DHAVN_APPIBOX_MOSTMESSAGEHANDLER_H
#define DHAVN_APPIBOX_MOSTMESSAGEHANDLER_H

#include <QObject>
#include <QtDBus>
#include <QDBusArgument>
#include <QVariant>
#include <QStringList>

#include "DHAVN_AppIBox_CANMessageSender.h"
#include "DHAVN_AppIBox_CANMessage_HU_CLU_PE_09.h"
#include "DHAVN_AppIBox_IStatusBarController.h"
#include "DHAVN_AppIBox_IEventSender.h"
#include "DHAVN_AppIBox_LogUtils.h"
#include "DHAVN_AppHomeScreen_Def.h"

#define MOST_MANAGER_SERVICE_NAME           "com.dhavn.mostmanager.qtdbus"
#define MOST_MANAGER_DLNA_PATH              "/dlna"
#define MOST_MANAGER_DLNA_INTERFACE         ""
#define MOST_MANAGER_HMI_PATH               "/hmi"
#define MOST_MANAGER_HMI_INTERFACE          ""
#define MOST_MANAGER_PHONE_PATH             "/telephone"
#define MOST_MANAGER_PHONE_INTERFACE        ""
#define MOST_MANAGER_IBOX_PATH              "/ibox"
#define MOST_MANAGER_IBOX_INTERFACE         ""


typedef enum
{
   DLNA_CS_NOT_CONNECTION = 0x0,
   DLNA_CS_CONNECTION = 0x1,
   DLNA_CS_INVALID = 0xFF,
} DLNA_CONN_STATUS_T;

typedef enum
{
   SILI_TITLE = 0,
   SILI_FILENAME,
   SILI_ARTIST,
   SILI_ALBUM,
   SILI_GENRE,
   SILI_MAX,
} SONG_INFO_LIST_INDEX_T;

struct SDisplayOSD{
    uint target;
    uint osdMode;
    uint mainIcon;
    uint subIcon;
    uint textColour1;
    uint textColour2;
    uint textColour3;
    QString OSDtext;
};

/** Declaring the structure over QtDbus */
Q_DECLARE_METATYPE(SDisplayOSD)

/** Marshalling SDisplayOSD to QDBusArgument */
inline QDBusArgument &operator<<(QDBusArgument &argument, const SDisplayOSD &displayOSD)
{
    argument.beginStructure();
    argument << displayOSD.target << displayOSD.osdMode << displayOSD.mainIcon << displayOSD.subIcon << displayOSD.textColour1
             << displayOSD.textColour2 << displayOSD.textColour3 << displayOSD.OSDtext;

    argument.endStructure();
    return argument;
}

/** Demarshalling QDBusArgument to SDisplayOSD */
inline const QDBusArgument &operator>>(const QDBusArgument &argument, SDisplayOSD &displayOSD)
{
    argument.beginStructure();
    argument >> displayOSD.target >> displayOSD.osdMode >> displayOSD.mainIcon >> displayOSD.subIcon >> displayOSD.textColour1
             >> displayOSD.textColour2 >> displayOSD.textColour3 >> displayOSD.OSDtext;

    argument.endStructure();
    return argument;
}

class MOSTMessageHandler : public QObject
{
    Q_OBJECT
public:
    explicit MOSTMessageHandler(Logger &pLogger,
                                IStatusBarController *pStatuBarController,
                                IEventSender *pEventSender,
                                CANMessageSender *pCanMessageSender,
                                QObject *parent = 0);
    ~MOSTMessageHandler();

   void sendBackKeyOn(bool isSoftKey);
   void sendIncrementTrackPosition();
   void sendDecrementTrackPosition();
   void sendPlayPause( bool isPlay );
   uint getCurDeckStatus() { return m_uiCurDeckStatus; }

   void requestStatusBarIndicatorInfo();

signals:
    void TimePosition(const int diskTime, const int trackTime, const int titleTime);
    void DLNAConnStatus(const uint status);
    void CurrentSongInfo(const uint playtime, const ushort trackno, const QStringList songinfo);
    void CurrentVideoInfo(const QString vtitle, const uint time, const ushort track, const QString vfilename);
    void SendDLNAMediaInfoToHome(const HOME_MEDIA_T *mediaInfo);
    void ShowStatusBar(const uint statusbar);

public slots:
    void onTimePosition(const int diskTime, const int trackTime, const int titleTime);
    void onCurrentSongInfo(const uint playtime, const ushort trackno, const QStringList songinfo);
    void onCurrentVideoInfo(const QString vtitle, const uint time, const ushort track, const QString vfilename);
    void onDeckStatus( const uint deckStatus );
    void onShowStatusBar(const uint statusbar);
    void OnDisplayOSD( const QDBusVariant &displayOSD );
    //void OnDisplayOSD( const uint target, const uint  osdMode, const uint mainIcon, const uint subIcon,
    //                  const uint textColour1, const uint textColour2, const uint textColour3, const QString OSDtext);

    //void OnDisplayOSDTest();

    /** DLNA setting */
    void onAdjustScreenFromIBOX( const int brightness, const int saturation, const int contrast, const int hue );
    void onLCDBrightnessFromIBOX( const int target, const int lcdFront, const int lcdRear1, const int lcdRear2 );
    void onIlluminationFromIBOX( const int illumination );
    void onVideoRatioFromIBOX( const int videoRatio );
    void onDisplayControlFromIBOX( const uint displayOnOff );
    void onVideoDefaultRequestIBOX( const uint deviceID, const ushort senderHandle, const uint sTarget );

    /** Status Bar */
    void onModemDataStatus( const uint status );
    void onWifiMode( const uint mode, const uint clientStatus );
    void onModemLevel( const uint modemLevel );
    void onUserMode( const uint userMode, const bool micOff );

private:
    void connectDBUS_MOST();
    void disconnectDBUS_MOST();

    void setAccessPointIcon( bool isEnable );
    void setWiFiAntennaIcon( int antenna );
    void setBluelinkCallIcon( bool isEnable );
    void setBluelinkMuteIcon( bool isEnable );
	void getModemStatus();

	bool modemServiceAvailable();

    DEFINE_LOGGER

    QDBusInterface *m_pMostDlnaQdbusInterface;
    QDBusInterface *m_pMostHMIQdbusInterface;
    QDBusReply<bool> m_DBusreply;

    CANMessageSender *m_pCanMessageSender;

    uint m_uiCurDeckStatus;

    IStatusBarController *m_pStatusBarController;
    IEventSender *m_pEventSender;
    //QDBusInterface *m_pMostHmiQdbusInterface;
    QString m_pTelNum;

	int m_modemStatus;
	int m_modemLevel;
};

#endif // DHAVN_APPIBOX_MOSTDLNAMESSAGEHANDLER_H
