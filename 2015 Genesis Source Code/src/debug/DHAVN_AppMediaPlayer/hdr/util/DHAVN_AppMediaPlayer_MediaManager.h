/** @file DHAVN_AppMediaPlayer_MediaManager.h
 * This module implements in the single place the media sources
 * tracking. 
 * */

#ifndef APP_MEDIAPLAYER_MEDIA_MANAGER_H
#define APP_MEDIAPLAYER_MEDIA_MANAGER_H

#include <QObject>
#include <uistate_manager_def.h>
#include "controller/DHAVN_AppMediaPlayer_Controllers.h"
// #include "DHAVN_QRPMClient.h" // removed by kihyung 2012.11.03 for booting disable

/** @brief The device types application should be capable to operate with.
 * */
enum MEDIA_SOURCE_T
{
    MEDIA_SOURCE_JUKEBOX,
    MEDIA_SOURCE_USB1,
    MEDIA_SOURCE_USB2,
    MEDIA_SOURCE_DVDVIDEO,
    MEDIA_SOURCE_VCD,
    MEDIA_SOURCE_MP3,
    MEDIA_SOURCE_CDDA,
    MEDIA_SOURCE_DVDAUDIO,
    MEDIA_SOURCE_IPOD1,
    MEDIA_SOURCE_IPOD2,
    MEDIA_SOURCE_AUX1,
    MEDIA_SOURCE_AUX2,
    MEDIA_SOURCE_BLUETOOTH,

    MEDIA_SOURCE_MAX
};


/** @brief The USB device type application should be aware of.
 * */
enum USB_DEV_TYPE_T
{
    USB_DEV_TYPE_STORAGE,
    USB_DEV_TYPE_IPOD,

    USB_DEV_TYPE_MAX
};

// { commented by ruinseve for CR#12584
// moved to AppMediaPlayer_Controllers.h
//enum MEDIA_TYPE_T
//{
//    MEDIA_TYPE_MUSIC,
//    MEDIA_TYPE_VIDEO,
//    MEDIA_TYPE_PHOTO,

//    MEDIA_TYPE_MAX
//};
// } commented by ruinseve for CR#12584.

typedef struct
{
    QString mountPath;
    QString id;

} DEVICE_INFO;

class AppMediaPlayer_EngineMain;

/** @class AppMediaPlayer_MediaManager
 * This class provides an interface for the controllers with the
 * list of the devices, that are available for playback. The devices
 * are provided in a per-controller basis: while it has the single
 * definition of the media sources, the list of sources are specific
 * to the concrete media type.
 * */
class AppMediaPlayer_MediaManager : public QObject
{
    Q_OBJECT

public:
    // { modified by eugeny.novikov 2012.12.10 for CR 16564 fastboot

    AppMediaPlayer_MediaManager( AppMediaPlayer_EngineMain &aEngineMain,
                                 QObject *pParent = 0 );
    // } modified by eugeny.novikov 2012.12.10
    ~AppMediaPlayer_MediaManager() {}
    
    /** @brief Verifies either media source is available for application mode
     * @param aSource The media source to check for
     * @param aType The type of the media to restrict the result to.
     * @return true if media source has the data of a type aType, false
     * if it is not.
     * */
    bool IsSourceAvailable( MEDIA_SOURCE_T aSource, MEDIA_TYPE_T aType );

    /** @brief Verifies either media source is available for application mode
     * @param source The media source to update
     * @param type The type of the media which should be updated
     * @param status True if media source has the data of a type, false otherwise.
     * */
    void SetSourceAvailable(MEDIA_SOURCE_T source, MEDIA_TYPE_T type, bool status);

    /** @brief Find out where USB storage is mounted to.
     * @note The return result is valid only if
     * IsSourceAvailable(MEDIA_SOURCE_USB) returns true.
     * */

    QString GetMountPath( MEDIA_SOURCE_T aSource );

    QString GetID( MEDIA_SOURCE_T aSource );

    void ejectDisc( bool isForce );

    bool IsDiscEjected();  // added by kihyung 2012.07.27 for ITS 0136589 

    // removed by kihyung 2013.07.08
    void StartDiscControllers(); // added by kihyung 2013.4.25 for boot performance
    void StartJukeboxControllers();
    void StartUSBControllers();

    // { added by changjin 2012.12.14 for CR 16130
    int getDiscState(); 
	void loadDisc();
    // } added by changjin 2012.12.14

    void EmitDeviceAttached(MEDIA_SOURCE_T aMediaSource, MEDIA_TYPE_T aType, bool autoplay); // modified by kihyung 2013.1.26

    void setGstVideoMode(int mode); // added by kihyung 2013.2.3

    bool isAutoplay( MEDIA_TYPE_T player, USB_DATA_CONTENT_MASK_T content ); // added by kihyung 2013.2.17
    bool isDiscReload(){ return m_isReloaded;}//{ modified by yongkyun.lee 2013-10-08 for : ISV 92155
    int  isDiscState(){return m_eDiscState;}//ys-20131011 disc state
signals:
    /** @brief Signal emitted once external device is connected (including CD insert).
     * @param aSource Connected device type.
     * @param aType Provides the type of player which should handle the connected event.
     * */
    void deviceAttached( MEDIA_SOURCE_T aSource, MEDIA_TYPE_T aType, bool autoplay);

    /** @brief Signal emitted once external device is removed (including CD eject).
     * @param aSource Removed device type.
     * @param aType Provides the type of player which should handle remove event.
     * */
    void deviceRemoved( MEDIA_SOURCE_T aSource, MEDIA_TYPE_T aType );
	void catreedeviceRemoved(); //added by wonseok.heo for ITS 205774 2013.11.01

    /* All stuff below is not intended to be used by controllers. These
     * methods are designed to be used from the EgineMain to make it possible
     * to maintain the state of the removable devices.
     */

    /** @brief Signal to be handled in one place: the idea is to notify
     * outside world about deck state changes in order to update homescreen
     * or switch the application mode to somewhat clever.
     * */
    void deckStateChanged( DISC_STATE state );

    void sourceAvailableChanged(MEDIA_SOURCE_T source, MEDIA_TYPE_T type, bool status); // added by Sergey for #16250

    void discLoaded(bool);

    void discLoading();
    void notifyIPodReady(MEDIA_SOURCE_T);
#ifdef BOOT_LOADING_SCREEN
    void isReadytoPlay();
#endif
public slots:
    
    /** @brief Method that MUST be called when USB device is attached.
     * This method is used to make resource availability functionality
     * work. This applies to USB storage devices. Does NOT handle IPod.
     * @param mountPath Mount path USB storage is mounted on.
     * */
    bool onUSBDevConnected( USB_PORT_T portNumber, const QString UUID, const QString mountPath); // modified by kihyung 2013.07.05 for ISV 84474

    /** @brief Method that MUST be called when USB device is detached.
     * This method is used to make resource availability functionality work.
     * */
    void onUSBDevDisconnected( USB_PORT_T portNumber );

    /** @brief Method that MUST be called when IPOD is attached.
     * @param mountPath Mount path USB storage is mounted on.
     * */
    void onIPODConnected( USB_PORT_T portNumber, const QString UUID, const QString mountPath, USB_DATA_CONTENT_MASK_T content );

    /** @brief Method that MUST be called when IPOD is detached.
     * This method is used to make resource availability functionality work.
     * */
    void onIPODDisconnected(USB_PORT_T portNumber);

    /** @brief Method that MUST be called when AUX device is attached.
     * This method is used to make resource availability functionality work.
     * */
    void onAUXConnected(MEDIA_TYPE_T type);

    /** @brief Method that MUST be called when AUX device is detached.
     * This method is used to make resource availability functionality work.
     * */
    void onAUXDisconnected();

    void onAUX2Connected(MEDIA_TYPE_T type );
    void onAUX2Disconnected( );

private slots:
    void onDeckScanCompleted(int);
    //{removed by junam 2013.03.26 for ISV75917
    //void onIPod1Authorized(bool);
    //void onIPod2Authorized(bool);
    //}removed by junam 2013.03.26
    void onDeckError(int);
    void onGstPipelineState(int);  // added by kihyung 2013.2.3


private:
    void handleDiscEject();
    void sendDiscStatusToUISH( EVENT_DISC_STATUS_T discStatus );
    void initializeDeviceInfo();
    bool      m_bSourceAvailability[MEDIA_SOURCE_MAX][MEDIA_TYPE_MAX];

    DEVICE_INFO m_tableDevInfo[ MEDIA_SOURCE_MAX + 1 ];

    bool      m_isReloaded;
    DeckController  *m_pDeckController;
    AppMediaPlayer_EngineMain &m_EngineMain;

    int m_eDiscState; // added by kihyung 2012.07.27 for ITS 0136589 
    int m_bSendDiscStateToUISH;
    int m_bSendDiscTypeToUISH;
    int m_bGstVideoMode;   // added by kihyung 2013.2.3
    int m_bEjectingReceived; // modified by cychoi 2013.06.02 for DQA issue - OSD is not displayed if eject Disc on Disc loading.
};

Q_DECLARE_METATYPE(MEDIA_SOURCE_T);

#endif // APP_MEDIAPLAYER_MEDIA_MANAGER_H
