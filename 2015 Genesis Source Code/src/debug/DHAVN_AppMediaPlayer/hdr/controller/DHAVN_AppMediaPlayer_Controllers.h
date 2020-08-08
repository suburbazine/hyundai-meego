/**
 * AppMediaPlayer_ControllerBase.h
 *
 * @author  Golyshkin A.
 * @CreatedDate 29-Nov-2011 1:55:11 PM
 */
#if !defined( APP_MEDIA_PLAYER_CONTROLLERS_H )
#define APP_MEDIA_PLAYER_CONTROLLERS_H

#include <typeinfo>
#include <usbcontroller.h>
#include <jukeboxcontroller.h>
#include <deckcontroller.h>
#include <DHAVN_IPodController_Def.h>
#include <QDateTime> // added by kihyung 2013.07.08
#include "util/DHAVN_AppMediaPlayer_LogHelper.h" // added by kihyung 2013.07.08

// #include <DHAVN_QRPMClient.h> // removed by kihyung 2012.11.03 for booting disable

#include <QMap>

/** @brief Application-global resources.
 * Part of the resource management policy for the controllers. See
 * AppMediaPlayer_Controllers class documentation for more details.
 * @see AppMediaPlayer_Controllers
 * */
enum MEDIAPLAYER_RESOURCE_T
{
    MEDIAPLAYER_RESOURCE_AUDIO        = 0x01,
    MEDIAPLAYER_RESOURCE_GRAPHICS     = 0x02,
    /* May be continued to acquire graphics on second display */
    
    MEDIAPLAYER_RESOURCE_MAX
};
Q_DECLARE_FLAGS(MEDIAPLAYER_RESOURCES_T, MEDIAPLAYER_RESOURCE_T);
Q_DECLARE_OPERATORS_FOR_FLAGS(MEDIAPLAYER_RESOURCES_T);

// { added by ruinseve for CR#12584
enum MEDIA_TYPE_T
{
    MEDIA_TYPE_MUSIC,
    MEDIA_TYPE_VIDEO,
    MEDIA_TYPE_PHOTO,

    MEDIA_TYPE_MAX
};
// } added by ruinseve for CR#12584.

/** @brief Common place where controllers can be found.
 * As it is, it does not look to be reasonable entity. However we might
 * use it to share controller instances among upper-level controllers.
 *
 * While we cannot predict how will our indian friends implement simultaneous
 * playback of audio and showing photos, we cannot bind the guarding
 * mechanism to the controllers themselves. Instead we'll need to implement
 * a higher level abstraction. Note also, that we need to remember that
 * we will need to move the application to rare display one day, and this
 * will require us to manage controllers much less trivially than now.
 *
 * We will introduce two kind of the resources, that higher level controller
 * could acquire: the music resource and graphics resource, and make
 * upper-level controllers to respect the following rules:
 *   - ControllerAudio needs audio resource to do the playback
 *   - ControllerPhoto needs graphics resource to do the image showing
 *   - ControllerVideo needs both resources in order to do the playback.
 * 
 * When the resource is acquired by one of the controllers, previous owner
 * will receive notification that resource is no longer available and it
 * must shutdown the playback, disconnect from the low-level controllers
 * and release any shared resource it occupied.
 *
 * Implementation details
 * We're going to abuse Qt-ish style of communications between the components.
 * The AppMediaPlayer_Controllers instance will send a signal to the owner
 * of the resource when it should stop using it.
 * */

class AppMediaPlayer_Controllers : public QObject
{
    Q_OBJECT
public:    
    AppMediaPlayer_Controllers(int nVehicleType);
    ~AppMediaPlayer_Controllers();

    /** @brief Request for resources.
     * This method notifies the resource owner to release the resources.
     * Upon complete, the resource owner must notify the
     * AppMediaPlayer_Controllers that resources are clear.
     *
     * @param resources The combination of MEDIAPLAYER_RESOURCE_T flags.
     * @param owner The object that requests the resources.
     * 
     * The resource requester must provide the slot onResourceGranted(),
     * which will be invoked once the resource becomes available.
     * The resource requester must provide the slot onReleaseResources()
     * which will be called when the resource is requested by anyone else.
     *
     * The onResourceGranted has no parameters, the resources granted will
     * be the same as passed by RequestResources().
     *
     * The onReleaseResources() slot should have a paramter of type
     * MEDIAPLAYER_RESOURCES_T. The resources that owner must release will
     * be sended as an argument. It is required that the owner will call
     * the onResourceReleased() slot of the AppMediaPlayer_Controllers.
     * */
    void RequestResources( MEDIAPLAYER_RESOURCES_T resources, QObject *owner );

    /** @brief Notify RPM that we're going to play something.
     * @param aMode the enum to distinguish what is going to be played.
     * */
    // void SetAudioPath( TAudioMode aMode ); // removed by kihyung 2012.11.03 for booting disable
    
    USBController * GetUSBController()
    {
        if ( !m_pUSB1Controller )
        {
            MP_MEDIUM << "Create USB1 Controller" << LOG_ENDL; // added by kihyung 2013.07.08
            m_pUSB1Controller = USBController::GetFirstInstance();

            if(m_pUSB1Controller == NULL)
            {
                MP_HIGH << "Create USB1 Controller Failed!!" << LOG_ENDL;
            }
        }

        return m_pUSB1Controller;
    }

    JukeboxController * GetJukeBoxController()
    {
        if ( !m_pJukeBox1Controller )
        {
            MP_MEDIUM << "Create Jukebox Controller" << LOG_ENDL; // added by kihyung 2013.07.08
            m_pJukeBox1Controller = JukeboxController::GetFirstInstance();

            if(m_pJukeBox1Controller == NULL)
            {
                MP_HIGH << "Create Jukebox Controller Failed!!" << LOG_ENDL;
            }
        }

        return m_pJukeBox1Controller;
    }

    USBController * GetUSBPhotoController1()
    {
        if ( !m_pUSB2Controller )
        {
            MP_MEDIUM << "Create USB Photo1 Controller" << LOG_ENDL; // added by kihyung 2013.07.08
            m_pUSB2Controller = USBController::GetSecondInstance();

            if(m_pUSB2Controller == NULL)
            {
                MP_HIGH << "Create USB Photo1 Controller Failed!!" << LOG_ENDL;
            }
        }

        return m_pUSB2Controller;
    }

    USBController * GetUSBPhotoController2()
    {
        if ( !m_pUSB3Controller )
        {
            MP_MEDIUM << "Create USB Photo2 Controller" << LOG_ENDL; // added by kihyung 2013.07.08
            m_pUSB3Controller = USBController::GetThirdInstance();

            if(m_pUSB3Controller == NULL)
            {
                MP_HIGH << "Create USB Photo2 Controller Failed!!" << LOG_ENDL;
            }
        }

        return m_pUSB3Controller;
    }

    JukeboxController * GetJukeBoxPhotoController()
    {
        if ( !m_pJukeBox2Controller )
        {
            MP_MEDIUM << "Create Jukebox Photo Controller" << LOG_ENDL; // added by kihyung 2013.07.08
            m_pJukeBox2Controller = JukeboxController::GetSecondInstance();

            if(m_pJukeBox2Controller == NULL)
            {
                MP_HIGH << "Create Jukebox Photo Controller Failed!!" << LOG_ENDL;
            }
        }

        return m_pJukeBox2Controller;
    }
    
    DeckController * GetDeckController()
    {
        if ( !m_pDeckController )
        {
            MP_MEDIUM << "Create Deck Controller" << LOG_ENDL; // added by kihyung 2013.07.08
            m_pDeckController = DeckController::GetInstance();

            if(m_pDeckController == NULL)
            {
                MP_HIGH << "Create Deck Controller Failed!!" << LOG_ENDL;
            }
        }

        return m_pDeckController;
    }

    CIpodController * GetIPod1Controller()
    {
        if ( !m_pIPod1Controller )
        {
            MP_MEDIUM << "Create IPod1 Controller" << LOG_ENDL; // added by kihyung 2013.07.08
            m_pIPod1Controller = new CIpodController(0, eIPOD_PORT_1, m_nVehicleType);
            // m_pIPod1Controller->start(); // added by eugeny.novikov 2012.12.10 for CR 16564 fastboot // removed by kihyung 2013.1.24 ISV70478

            if(m_pIPod1Controller == NULL)
            {
                MP_HIGH << "Create IPod1 Controller Failed!!" << LOG_ENDL;
            }
        }

        return m_pIPod1Controller;
    }

    CIpodController * GetIPod2Controller()
    {
        if ( !m_pIPod2Controller )
        {
            MP_MEDIUM << "Create IPod2 Controller" << LOG_ENDL; // added by kihyung 2013.07.08
            m_pIPod2Controller = new CIpodController(0, eIPOD_PORT_2, m_nVehicleType);
            // m_pIPod2Controller->start(); // added by eugeny.novikov 2012.12.10 for CR 16564 fastboot // removed by kihyung 2013.1.24 ISV70478

            if(m_pIPod2Controller == NULL)
            {
                MP_HIGH << "Create IPod2 Controller Failed!!" << LOG_ENDL;
            }
        }

        return m_pIPod2Controller;
    }

    QStringList getExtList(MEDIA_TYPE_T type);  // { added by ruinseve for CR#12584

public slots:
    /** @brief Method the MUST be called when resource is released.
     * While we stop using the pipeline immidiately, we need to notify
     * others when the resource can be used.
     * */
    void onResourceReleased( MEDIAPLAYER_RESOURCES_T resource );

    // { removed by kihyung 2012.11.03 for booting disable
    /*
private slots:
    void onRPMAcquireResponse(TAudioMode aAudioMode, TAudioSink aAudioSink);
    */
    // } removed by kihyung 2012.11.03 for booting disable
    
private:
    void tryGrantResources();
    void initExtLists(); // { added by ruinseve for CR#12584

    USBController *m_pUSB1Controller;

    USBController *m_pUSB2Controller;

    USBController *m_pUSB3Controller;

    JukeboxController *m_pJukeBox1Controller;

    JukeboxController *m_pJukeBox2Controller;

    DeckController *m_pDeckController;

    CIpodController *m_pIPod1Controller;

    CIpodController *m_pIPod2Controller;

    // QRPMClient                 *m_pRPMClient; // removed by kihyung 2012.11.03 for booting disable

    struct RESOURCE_REQUEST_T
    {
        MEDIAPLAYER_RESOURCES_T   resources;
        QObject                  *owner;

        RESOURCE_REQUEST_T() 
        {
            resources = 0;
            owner = NULL;
        }
    };
    RESOURCE_REQUEST_T                      m_pendingResourceRequest;
    QMap<MEDIAPLAYER_RESOURCE_T, QObject *> m_resourceMap;

    // { added by ruinseve for CR#12584
    QStringList m_audioExtList;
    QStringList m_videoExtList;
    QStringList m_photoExtList;
    // } added by ruinseve for CR#12584.

    int m_nVehicleType;
};

#endif // !defined( APP_MEDIA_PLAYER_CONTROLLER_BASE_H )
