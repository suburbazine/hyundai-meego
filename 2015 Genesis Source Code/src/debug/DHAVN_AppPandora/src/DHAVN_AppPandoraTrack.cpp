#include "DHAVN_AppPandoraTrack.h"
#include "DHAVN_AppPandoraBase.h"
#include "DHAVN_AppPandora_Notifier.h"
#include <DHAVN_PndrStateHandler.h>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <DHAVN_QRPMClient.h>

//{ modified by yongkyun.lee 2014-08-25 for : Branding Image
#define BRANDING_IMAGE_PATH "/app/share/images/pandora/"
#define BRANDING_IMAGE_NAME "Logo_HYUNDAI_240x100.png" //added by wonseok.heo for Branding Image DH PE 2015.07.03
//} modified by yongkyun.lee 2014-08-25

/**
 *******************************************************************
 * Track constructor
 *
 * @param[in] inPndrInterface   interface reference to Pandora MW
 *
 * @return  None
 *
 *******************************************************************
 */
CAppPandoraTrack::CAppPandoraTrack(CAppPandoraBase* inMainEngine):
    m_pMainEngine(inMainEngine)
  , m_PndrInterface(NULL)    // added by esjang 2013.05.22 for static test # 20849
  , m_TrackInfo(NULL)
  , m_nTrackToken(0) // added by esjang 2012.11.12 for Static Test CID 13218
  , m_pDeclarativeView(NULL) // added by esjang 2012.11.12 for Static Test CID 13218
  , m_pSignals(NULL)
  , nPlayPause(-1)
  , mAlbumArtFilePath("")
  , m_nCurrentStationToken(0)
  , m_eRateReq(REQ_TYPE_RATE_NONE)
  , m_PlayTimer( new QTimer(this)) //{ modified by yongkyun.lee 2014-03-11 for : BT streaming Mute-play control
  , m_BRImage(false)//{ modified by yongkyun.lee 2014-10-08 for : Branding - Middleware
  , m_eReq(REQ_TYPE_NONE)
{
    LOG_INIT_V2("Pandora");  
}
void CAppPandoraTrack::SetInterface(CPndrInterface*  inPndrInterface)
{    
    LOG_CRITICAL<<"CAppPandoraTrack::SetInterface"<<LOG_ENDL;    
    if(m_pSignals){
        disconnect(m_pSignals,0,this,0);
        m_pSignals=NULL;
    }
    m_PndrInterface = NULL;
    m_PndrInterface = inPndrInterface;

    
    //{ modified by yongkyun.lee 2014-06-16 for : ITS 240304
    if(m_PlayTimer)
    {
        disconnect(m_PlayTimer , SIGNAL(timeout()), this, SLOT(onPlayTimer()) );  
        //delete m_PlayTimer;
        //m_PlayTimer = NULL;
    }
    //} modified by yongkyun.lee 2014-06-16 


    if(inPndrInterface == NULL)
    {

        LOG_CRITICAL<<"No interface set"<<LOG_ENDL;
        return;
    }
    m_pSignals = &(m_PndrInterface->GetSignalProvider());

    Qt::ConnectionType ConnType = Qt::QueuedConnection;
    connect(m_pSignals,
        SIGNAL(TrackUpdated(const quint32 )),
        this,
        SLOT(OnTrackUpdated(const quint32 )),
        ConnType);

    connect(m_pSignals,
        SIGNAL(UpdateElapsedTime(const quint16 )),
        this,
        SLOT(OnUpdateElapsedTime(const quint16 )),
        ConnType );

    connect(m_pSignals,
        SIGNAL(TrackExplain(const QString&)),
        this,
        SLOT(OnTrackExplain(const QString&)),
        ConnType);

    connect(m_pSignals,
        SIGNAL(TrackInfo(const TSTrackInfo& )),
        this,
        SLOT(OnTrackInfoReceived(const TSTrackInfo& )),
        ConnType);

    connect(m_pSignals,
        SIGNAL(AlbumArt(const QString&)),
        this,
        SLOT(OnAlbumArtReceived(const QString&)),
        ConnType);

    connect(m_pSignals,
        SIGNAL(StationUpdated(const quint32)),
        this,
        SLOT(OnStationUpdated(const quint32 )));

    connect(m_pSignals,
           SIGNAL(StationDeleted(const quint32 )),
           this,
           SLOT(OnStationDeleted(const quint32 )),
           ConnType);

     //{ modified by yongkyun.lee 2014-03-11 for : BT streaming Mute-play control
     //{ modified by yongkyun.lee 2014-06-16 for : ITS 240304
     if(m_PlayTimer == NULL )
     {
         m_PlayTimer = new QTimer(this);
     }
     //} modified by yongkyun.lee 2014-06-16 
     
     m_PlayTimer->setSingleShot( true );
     m_PlayTimer->setInterval( 2000 ); //  5 sec
     connect( m_PlayTimer, SIGNAL(timeout()), this, SLOT(onPlayTimer()) );    
     //} modified by yongkyun.lee 2014-03-11
     m_BRImage = false;//{ modified by yongkyun.lee 2014-10-08 for : Branding - Middleware

}


/**
 *******************************************************************
 * Track destructor
 *
 * @param None
 *
 * @return None
 *
 *******************************************************************
 */
CAppPandoraTrack::~CAppPandoraTrack()
{
}

//{ added by esjang 2013.03.06 for DH Genesis GUI Design Guideline v1.1.2(2013.02.28)
/**
*******************************************************************
* Return the current play state .
*
* @param  None
*
* @return true: playing state
*         false: pause state        
*
*******************************************************************
*/

bool
CAppPandoraTrack::IsPlaying() const
{
    bool state = false;
    if(nPlayPause == 1)
        state = true;
    return state;
}
//} added by esjang 2013.03.06 for DH Genesis GUI Design Guideline v1.1.2(2013.02.28)

/**
*******************************************************************
* Clear the cache on back soft key .
*
* @param  None
*
* @return None
*
*******************************************************************
*/
void CAppPandoraTrack::ClearCache()
{
    LOG_TRACE<<"CAppPandoraTrack::ClearCache() play status : "<<nPlayPause<<LOG_ENDL;

    if(m_TrackInfo)
    {
         delete m_TrackInfo;
    }
     m_TrackInfo = NULL;
     nPlayPause = -1 ;
     m_nCurrentStationToken = 0;
     m_eRateReq = REQ_TYPE_RATE_NONE;
     m_eReq = REQ_TYPE_NONE;
}

/**
*******************************************************************
* Returns current track playing.
*
* @param  None
*
* @return TrackInfo structure
*
*******************************************************************
*/
TrackInfo* CAppPandoraTrack::GetCurrentTrackInfo()
{
    return m_TrackInfo;
}


//{ modified by yongkyun.lee 2014-08-21 for :  ITS 244978	
void CAppPandoraTrack::CheckTrackInfo()
{
    LOG_INFO<<"CAppPandoraTrack::CheckTrackInfo "<<LOG_ENDL;
    if(m_TrackInfo == NULL)
    {
        LOG_HIGH<<"CAppPandoraTrack::CheckTrackInfo :: trackInfo is NULL"  <<LOG_ENDL;
        RequestTrackInfo();
    }
    return;
}
//} modified by yongkyun.lee 2014-08-21 

/**
 *******************************************************************
 * Request from QML Layer to play current Track
 *
 * @param None
 *
 * @return None
 *
 *******************************************************************
 */
void CAppPandoraTrack::Play()
{
    LOG_HIGH<<"CAppPandoraTrack::Play() <- nPlayPause =" << nPlayPause<<", m_ePriState=" <<m_PndrInterface->GetPriState() <<LOG_ENDL;  //BCH_CHECK. nPlayPause : 1(Play), 0(Pause)
    //{ modified by yongkyun.lee 2014-03-11 for : BT streaming Mute-play control
    if(nPlayPause == 0)
    {
        if(( (m_pMainEngine->getModeStateHistoryFMode()) 
             && (m_pMainEngine->getModeStateHistoryPrev() == MODE_STATE_BT_AUDIO || m_pMainEngine->getModeStateHistoryPrev() == MODE_STATE_AHA_RADIO_BT)) )
        {
            LOG_HIGH<<"CAppPandoraTrack::Play() 2000 <-"<<LOG_ENDL;        
            startPlayTimer();
        }
        else
        {
            LOG_HIGH<<"CAppPandoraTrack::Play() now.. <-"<<LOG_ENDL;   
            HandleIfError( m_PndrInterface->Play());
        }
    }
    //{ modified by cheolhwan 2014-04-17. ITS 234891.
    else if(nPlayPause == 1)
    {
        LOG_CRITICAL<<"CAppPandoraTrack::Play() <- getModeStateHistoryFMode()="<<m_pMainEngine->getModeStateHistoryFMode()<<LOG_ENDL; 
        if(m_pMainEngine->getModeStateHistoryFMode())
        {
            startPlayTimer();
        }
    }
    //} modified by cheolhwan 2014-04-17. ITS 234891.
    //} modified by yongkyun.lee 2014-03-11
    LOG_INFO<<"CAppPandoraTrack::Play() ->"<<LOG_ENDL;
}


/**
 *******************************************************************
 * Request from QML Layer to Pause current Track
 *
 * @param None
 *
 * @return None
 *
 *******************************************************************
 */
void CAppPandoraTrack::Pause()
{
    LOG_HIGH<<"CAppPandoraTrack::Pause() <- stat ="<<nPlayPause<<LOG_ENDL;

    if(nPlayPause == 1)
        HandleIfError( m_PndrInterface->Pause() );

    LOG_INFO<<"CAppPandoraTrack::Pause() ->"<<LOG_ENDL;

}


/**
 *******************************************************************
 * Request from QML Layer to Skip current Track
 *
 * @param None
 *
 * @return None
 *
 *******************************************************************
 */
void CAppPandoraTrack::Skip()
{
    LOG_INFO<<"CAppPandoraTrack::Skip() <-"<<LOG_ENDL;

    if( m_TrackInfo )
    {
        if(m_TrackInfo->getAllowSkip())
        {
            HandleIfError( m_PndrInterface->Skip() );
            m_pMainEngine->setOSDFlag(true);// modified by yongkyun.lee 2014-09-23 for : ITS 248810            
        }
    }
    else
    {
        if( m_nCurrentStationToken > 0 )
        {
            m_PndrInterface->Skip() ;
            m_pMainEngine->setOSDFlag(true);// modified by yongkyun.lee 2014-09-23 for : ITS 248810            
        }
    }

    LOG_INFO<<"CAppPandoraTrack::Skip() ->"<<LOG_ENDL;
}


/**
 *******************************************************************
 * Request from QML Layer to Rate up the current track
 *
 * @param None
 *
 * @return None
 *
 *******************************************************************
 */
void CAppPandoraTrack::ThumbUp()
{
    LOG_INFO<<"CAppPandoraTrack::ThumbUp() <-"<<LOG_ENDL;
    //{ modified by yongkyun.lee 2014-08-20 for :  ITS 244978
     if(m_TrackInfo == NULL)
     {
         LOG_INFO<<"m_TrackInfo is NULL"<<LOG_ENDL;
     }
     else if(m_TrackInfo->getRating() == 1)
    {
        return;
    }
    //} modified by yongkyun.lee 2014-08-20 

    m_eRateReq =  REQ_TYPE_RATE_POS;
    if(m_pMainEngine->IsRunningInBG())
        m_pMainEngine->OSD(CExternalNotifier::EThumbsUp);    //{ modified by yongkyun.lee 2014-07-17 for : ITS 243240
    HandleIfError( m_PndrInterface->Rate(1) );

    LOG_INFO<<"CAppPandoraTrack::ThumbUp() ->"<<LOG_ENDL;

}


/**
 *******************************************************************
 * Request from QML Layer to rate down the current track
 *
 * @param None
 *
 * @return None
 *
 *******************************************************************
 */
void CAppPandoraTrack::ThumbDown()
{
    LOG_INFO<<"CAppPandoraTrack::ThumbDown() <-"<<LOG_ENDL;
    //{ modified by yongkyun.lee 2014-08-20 for :  ITS 244978
    if(m_TrackInfo == NULL)
     {
         LOG_INFO<<"m_TrackInfo is NULL"<<LOG_ENDL;
     }
     else if( m_TrackInfo->getRating() == 2)
     {
        return;
     }
    //} modified by yongkyun.lee 2014-08-20 

    m_eRateReq =  REQ_TYPE_RATE_NEG;
    if(m_pMainEngine->IsRunningInBG())
        m_pMainEngine->OSD(CExternalNotifier::EThumbsDown);    //{ modified by yongkyun.lee 2014-07-17 for : ITS 243240
    HandleIfError( m_PndrInterface->Rate( 2 ) );

    LOG_INFO<<"CAppPandoraTrack::ThumbDown() ->"<<LOG_ENDL;

}


/**
 *******************************************************************
 * Request from QML Layer to Bookmark the current track
 *
 * @param None
 *
 * @return None
 *
 *******************************************************************
 */
void CAppPandoraTrack::BookmarkSong()
{
    LOG_INFO<<"CAppPandoraTrack::BookmarkSong() <-"<<LOG_ENDL;

    HandleIfError( m_PndrInterface->BookmarkSong() );

    LOG_INFO<<"CAppPandoraTrack::BookmarkSong() ->"<<LOG_ENDL;
}


/**
 *******************************************************************
 * Request from QML Layer to Bookmark artist for current track
 *
 * @param None
 *
 * @return None
 *
 *******************************************************************
 */
void CAppPandoraTrack::BookmarkArtist()
{
    LOG_INFO<<"CAppPandoraTrack::BookmarkArtist() <-"<<LOG_ENDL;

    HandleIfError( m_PndrInterface->BookmarkArtist() );

    LOG_INFO<<"CAppPandoraTrack::BookmarkArtist() ->"<<LOG_ENDL;
}

/**
 *******************************************************************
 * Request from QML Layer for getting explaination for current track
 *
 * @param None
 *
 * @return None
 *
 *******************************************************************
 */
void CAppPandoraTrack::RequestExplaination()
{
    LOG_INFO<<"CAppPandoraTrack::RequestExplaination() <-"<<LOG_ENDL;

    HandleIfError( m_PndrInterface->RequestExplanation() );

    LOG_INFO<<"CAppPandoraTrack::RequestExplaination() ->"<<LOG_ENDL;
}

/**
 *******************************************************************
 * Request from QML Layer for getting track info for current track
 *
 * @param None
 *
 * @return None
 *
 *******************************************************************
 */
void CAppPandoraTrack::RequestTrackInfo()
{
   // LOG_INFO<<"CAppPandoraTrack::RequestTrackInfo() <-"<<LOG_ENDL;
    if(m_TrackInfo)
    {
        LOG_HIGH<<"CAppPandoraTrack::RequestTrackInfo() , track info already present "<<LOG_ENDL;

        QString stArt = IsStationArtPresent();
        if(stArt.length()>0){
            m_TrackInfo->setStationArt(stArt);
        }

        QObject * obj = qobject_cast<QObject*>(m_TrackInfo);
        m_pDeclarativeView->rootContext()->setContextProperty("InTrackInfo",QVariant::fromValue(obj));
        emit trackInfoReceived();
        if(mAlbumArtFilePath.length() > 0 || m_TrackInfo->AlbumArtLength == 0)
        {
            emit albumArtReceived(mAlbumArtFilePath);
        }
        emit trackShardReceived(); //added by wonseok.heo 2015.04.27 for DH PE new spec
    }
    else
    {
        LOG_HIGH<<"CAppPandoraTrack::RequestTrackInfo() Pending Req =  "<< m_eReq <<LOG_ENDL;
        if(m_eReq != REQ_TYPE_TRACK_INFO) {
            m_eReq = REQ_TYPE_TRACK_INFO;
            mAlbumArtFilePath.clear();
            HandleIfError( m_PndrInterface->RequestTrackInfo() );
        }
    }

    //LOG_INFO<<"CAppPandoraTrack::RequestTrackInfo() ->"<<LOG_ENDL;
}

 /**
 *******************************************************************
 * New station is selected from station list.
 *
 * @param None
 *
 * @return None
 *
 *******************************************************************
 */
void CAppPandoraTrack::ClearTrackInfo()
{
    if(m_TrackInfo)
    {
         delete m_TrackInfo;
    }
     m_TrackInfo = NULL;
     m_nCurrentStationToken = 0;
     m_eReq = REQ_TYPE_NONE;
}

/**
 *******************************************************************
 * The result of request from Pandora MW
 * Non network related errors should be handled here
 *
 * @param[in] inRequestType The request for which the reply is
 *                          received
 * @param[in] inPndrError   The result of the request
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraTrack::OnRequestCompleted(const ERequestType inRequestType, const EPndrError inPndrError)
{
    //LOG_SLOT<<"CAppPandoraTrack::OnRequestCompleted() <-"<<LOG_ENDL;

    LOG_INFO<<" CAppPandoraTrack::OnRequestCompleted Req = "<<inRequestType << " status = "<<inPndrError<<LOG_ENDL;

    if(E_SUCCESS == inPndrError)
    {
        switch(inRequestType)
        {
            case REQ_TYPE_PLAY:
            {
                LOG_HIGH<<"REQ_TYPE_PLAY"<<LOG_ENDL;
                nPlayPause = 1;
                emit playStarted();
            }
            break;
            case REQ_TYPE_PAUSE:
            {
                LOG_HIGH<<"REQ_TYPE_PAUSE"<<LOG_ENDL;
                nPlayPause = 0;
                emit pauseDone();
            }
            break;

            case REQ_TYPE_RATE:
            {
                if( m_eRateReq != REQ_TYPE_RATE_NONE )
                {

                    if(m_TrackInfo ){
//                       //Update the cached value of rating to be displayed when Back from list view
//                        LOG_INFO<<"nUserRating = " << nUserRating<<LOG_ENDL;
//                        LOG_INFO<<"m_TrackInfo->nRating  = " << m_TrackInfo->getRating()<<LOG_ENDL;

                        int rate =  (m_eRateReq == REQ_TYPE_RATE_POS) ? 1 : 2;
                        m_TrackInfo->setTrackRating(rate);
                        emit rated(rate);
                    }
                    m_eRateReq = REQ_TYPE_RATE_NONE;
                }
            }
            break;
            //added by esjang

            case REQ_TYPE_RATE_NONE:
            {
                if( m_eRateReq != REQ_TYPE_RATE_NONE )
                {
                    m_eRateReq = REQ_TYPE_RATE_NONE;
                    if( m_TrackInfo )
                    {
                        m_TrackInfo->setTrackRating(0);
                        emit rated(0);
                    }
                }
                else
                    LOG_HIGH<<"Error REQ_TYPE_RATE_NONE has come , while rate was not requested "<<LOG_ENDL;
            }
            break;            
            case REQ_TYPE_RATE_POS:
            {
                if( m_eRateReq != REQ_TYPE_RATE_NONE )
                {
                    m_eRateReq = REQ_TYPE_RATE_NONE;
                    if( m_TrackInfo )
                    {
                        m_TrackInfo->setTrackRating(1);
                        emit rated(1);
                    }
                }
                else
                    LOG_HIGH<<"Error REQ_TYPE_RATE_POS has come , while rate was not requested "<<LOG_ENDL;
            }
            break;            
            case REQ_TYPE_RATE_NEG:
            {
                if( m_eRateReq != REQ_TYPE_RATE_NONE )
                {
                    m_eRateReq = REQ_TYPE_RATE_NONE;
                    if( m_TrackInfo )
                    {
                        m_TrackInfo->setTrackRating(2);
                        emit rated(2);
                    }
                }
                else
                    LOG_HIGH<<"Error REQ_TYPE_RATE_NEG has come , while rate was not requested "<<LOG_ENDL;
            }
            break;            
            //added by esjang
            case REQ_TYPE_BOOKMARK_SONG:
            {
            //To DO: Update the cached value of bookmark to be displayed when Back from list view
            if( m_TrackInfo ) // added by wonseok.heo for ITS 266666
            {

            m_TrackInfo->setTrackBookmark(1);

            }

               emit songBookmarked();
            }
            break;
            case REQ_TYPE_BOOKMARK_ARTIST:
            {
                emit artistBookmarked();
            }
            break;
            default:
            {
                LOG_CRITICAL<<" Default :"<<inRequestType<<LOG_ENDL;
                break;
            }
        }
    }

   // LOG_SLOT<<"CAppPandoraTrack::OnRequestCompleted() ->"<<LOG_ENDL;
}

/**
 *******************************************************************
 * This is the slot to listen to signal from Pandora MW
 * for track update information
 *
 * @param[in] inTrackToken  The token ID for new track
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraTrack::OnTrackUpdated(const quint32 inTrackToken)
{
    LOG_SLOT<<"CAppPandoraTrack:: Track updates token = "<<inTrackToken <<LOG_ENDL;
    m_nTrackToken = inTrackToken;
    mAlbumArtFilePath.clear();
    m_eRateReq = REQ_TYPE_RATE_NONE;

    if(inTrackToken > 0)
    {
        m_eReq = REQ_TYPE_NONE;
        if(m_TrackInfo)delete m_TrackInfo;
        m_TrackInfo = NULL ;
        emit trackUpdated(inTrackToken);
    }
    else
    {
        //LOG_CRITICAL<<"PNDR_TRACK_NONE received , Emit Null data to UI "<<LOG_ENDL;
        TSTrackInfo trackInfo;
        QString stArt("");
        //{ added by esjang for Static test #22887: Uninitialized scalar variable
        trackInfo.nDuration = 0;
        trackInfo.nElapsed = 0;
        trackInfo.nRating = 0;
        trackInfo.bAllowRating = false;
        trackInfo.bAllowSkip = true;// modified by yongkyun.lee 2014-09-01 for : Pandora 4.25 TEST
        trackInfo.bAllowBookmark = false;
        trackInfo.bAllowExplain = false;
        //added by jyjeon 2014-04-01 for Pandora Link v3.
        trackInfo.bAd = false;
        trackInfo.bTrackBookmarked = false;
        trackInfo.bArtistBookmarked = false;
        //added by jyjeon 2014-04-01 for Pandora Link v3.
        trackInfo.bShared = false;
        trackInfo.Title = "";
        trackInfo.Album = "";
        trackInfo.Artist = "";
        trackInfo.AlbumArtUrl = "";
        trackInfo.CurrentStation = "";
        trackInfo.StationToken = 0;
        trackInfo.nAlbumArtLength = 0;
        //} added by esjang for Static test #22887: Uninitialized scalar variable

        if(m_TrackInfo)
        {
            trackInfo.bShared = m_TrackInfo->isSharedStation() ;
            trackInfo.CurrentStation = m_TrackInfo->getStationName();
            trackInfo.StationToken = m_TrackInfo->getStationToken();
            stArt = m_TrackInfo->getStationArt();
            delete m_TrackInfo;
            m_TrackInfo = NULL;
        }

        m_TrackInfo = new TrackInfo();
        m_TrackInfo->SetTrackProperties(trackInfo);
        m_TrackInfo->StationToken =  inTrackToken ;//m_nCurrentStationToken;
        m_TrackInfo->setStationArt(stArt);

        QObject * obj = qobject_cast<QObject*>(m_TrackInfo);
        m_pDeclarativeView->rootContext()->setContextProperty("InTrackInfo",QVariant::fromValue(obj));
        //emit clearScreenonTrackNone(nPlayPause);

         emit trackUpdated(inTrackToken);
    }
   // LOG_SLOT<<"CAppPandoraTrack::OnTrackUpdated() ->"<<LOG_ENDL;
}

/**
 *******************************************************************
 * This is the slot to listen to signal from Pandora MW
 * for listening to updat of elapsed time for current track
 *
 * @param[in] inElapsedTime elapsed time in sec
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraTrack::OnUpdateElapsedTime(const quint16 inElapsedTime)
{
    if(m_TrackInfo)
    {
        m_TrackInfo->setElapseTime(inElapsedTime);
    }
    emit trackElapsedTime(inElapsedTime);

    m_pMainEngine->GetPandoraNotifier().SendClusterData_Progress(inElapsedTime);
}

/**
 *******************************************************************
 * This is the slot to listen to signal from Pandora MW
 * for listening to track explain information
 *
 * @param[in] inTrackExplain the track explain information
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraTrack::OnTrackExplain(const QString& inTrackExplain)
{
  //  LOG_TRACE<<"CAppPandoraTrack::OnTrackExplain() <-"<<LOG_ENDL;

    if( inTrackExplain.length() >0 )
    {
        QString explaination(inTrackExplain);
        explaination.append("\n");
        emit trackExplainReceived(explaination);
    }

    //LOG_SLOT<<"CAppPandoraTrack::OnTrackExplain() ->"<<LOG_ENDL;
}

/**
 *******************************************************************
 * This is the slot to listen to signal from Pandora MW
 * for track information
 *
 * @param[in] inTrackInfo the track information
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraTrack::OnTrackInfoReceived(const TSTrackInfo& inTrackInfo)
{
    LOG_SLOT<<"CAppPandoraTrack::OnTrackInfoReceived() Req = " << m_eReq <<LOG_ENDL;

    int shared = m_eReq; // added by wonseok.heo 2015.04.27 for DH PE new spec

    if (m_eReq == REQ_TYPE_TRACK_INFO)
        m_eReq = REQ_TYPE_NONE;

    if(m_TrackInfo)
    {
        delete m_TrackInfo;
        m_TrackInfo = NULL;
    }
   //LOG_INFO<<"OnTrackInfoReceived::Rateing value ->"<<inTrackInfo.nRating<<LOG_ENDL;
    m_TrackInfo = new TrackInfo();
    m_TrackInfo->SetTrackProperties(inTrackInfo);
    m_TrackInfo->StationToken = m_nCurrentStationToken;

    /** // removed by jyjeon 2013.12.09 for UX Update
    if(inTrackInfo.CurrentStation.length() > 0 && inTrackInfo.bShared )
    {
       QString station(inTrackInfo.CurrentStation);
       station.append(" [Shared]");
       m_TrackInfo->setStation(station);
    }
    // removed by jyjeon 2013.12.09 for UX Update **/

    QString stArt = IsStationArtPresent();
    if(stArt.length()>0){
        m_TrackInfo->setStationArt(stArt);
    }
    QObject * obj = qobject_cast<QObject*>(m_TrackInfo);
    m_pDeclarativeView->rootContext()->setContextProperty("InTrackInfo",QVariant::fromValue(obj));
    emit trackInfoReceived();

    if(shared == 0){ //added by wonseok.heo 2015.04.27 for DH PE new spec
        emit trackShardReceived();

    }

    // { added by esjang 2012.11.20 CAN TP Message
    //emit trackTitleReceived(inTrackInfo.Title);
    // } added by esjang 2012.11.20 CAN TP Message
    m_pMainEngine->GetPandoraNotifier().UpdateTrackTitle(inTrackInfo.Title);
    LOG_HIGH<<"CAppPandoraTrack::OnTrackInfoReceived: inTrackInfo.Title=["<<inTrackInfo.Title << "]"<<LOG_ENDL;
    
    m_pMainEngine->isSeek = false; //add by wonseok.heo for OSD at BG 2015.02.04
    // if album art is rreceived before track info received , Then emit signal.
    if( mAlbumArtFilePath.length()>0 || inTrackInfo.nAlbumArtLength == 0)
    {
        emit albumArtReceived(mAlbumArtFilePath);
    }

    //LOG_SLOT<<"CAppPandoraTrack::OnTrackInfoReceived() ->"<<LOG_ENDL;
}

/**
 *******************************************************************
 * This is the slot to listen to signal from Pandora MW
 * for track album art
 *
 * @param[in] inAlbumArt the track album art path
 *
 * @return  None
 *
 *******************************************************************
 */
 void CAppPandoraTrack::OnAlbumArtReceived(const QString& inAlbumArt)
 {
      LOG_INFO<<"CAppPandoraTrack::OnAlbumArtReceived path = " << inAlbumArt<< "[ Time = "<< (QTime::currentTime()).toString("hh.mm.ss.zzz")<<"]" <<LOG_ENDL;

      mAlbumArtFilePath.clear();
      mAlbumArtFilePath = inAlbumArt;
      emit albumArtReceived(inAlbumArt);

     // LOG_INFO<<"CAppPandoraTrack::OnAlbumArtReceived ->"<<LOG_ENDL;

 }
 /**
 *******************************************************************
 * Slot to listen to signal from Pandora MW for getting info on
 * station update
 *
 * @param[in] inStationToken   station identifier for new station
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppPandoraTrack::OnStationUpdated(const quint32 inStationToken)
{
    LOG_INFO<<"CAppPandoraTrack::OnStationUpdated station token :"<< inStationToken<<LOG_ENDL;

    if(m_TrackInfo)
    {
        delete m_TrackInfo;
        m_TrackInfo = NULL ;
    }

    if(inStationToken > 0){
        m_nCurrentStationToken = inStationToken;
    }
}


void CAppPandoraTrack::OnStationDeleted(const quint32 inStationToken)
{
    LOG_INFO<<"CAppPandoraTrack::OnStationDeleted station token :"<< inStationToken<<LOG_ENDL;
    if(inStationToken ==  m_nCurrentStationToken)
    {
        m_nCurrentStationToken = 0;
       // emit handleError((quint16)E_STATION_DOES_NOT_EXIST);
    }
}

/**
 *******************************************************************
 * Request from QML layer for getting station art image if avialable
 *
 * @return filePath if exist ortherwise NULL
 *
 *******************************************************************
 */
QString CAppPandoraTrack::IsStationArtPresent()
{   
    QString filePath(STATION_ART_PATH);
    QString stationNo;
    stationNo.setNum(m_nCurrentStationToken);
    filePath.append(stationNo);
    filePath.append(".jpeg");

    //If the same file exists
    if(!QFile::exists(filePath))
    {
         //LOG_HIGH<<"Error Track view station art either does not exist of not able to load : "<<filePath <<LOG_ENDL;
         filePath = "";
    }  
    return filePath;
}

/**
 *******************************************************************
 * Handle the error
 *
 * @param[in] inPndrError error code
 *
 * @return None
 *
 *******************************************************************
 */
void CAppPandoraTrack::HandleIfError(const EPndrError inPndrError)
{
    //LOG_INFO<<"CAppPandoraTrack::HandleIfError <-"<<LOG_ENDL;
    if(E_SUCCESS != inPndrError)
    {
        LOG_CRITICAL<<"Error code :"<<inPndrError<<LOG_ENDL;
        switch(inPndrError)
        {
            //case E_INVALID_STATE:
            case E_INVALID_ARGUMENT:
            {
                // unexpected error
                emit handleError(inPndrError);
            }
            break;
            default:
            {
                LOG_CRITICAL<<"Default :"<<inPndrError<<LOG_ENDL;
                break;
            };
        }
    }
    //LOG_INFO<<"CAppPandoraTrack::HandleIfError ->"<<LOG_ENDL;
}

/**
*******************************************************************
* Aquire audio path while connecting/resuming pandora.
*
* @param  inDeviceType , Connected device type.
*
* @return None
*
*******************************************************************
*/
#if 0
void CAppPandoraTrack::SetAudioPathRPM (DeviceInfo::EConnectionType inDeviceType)
{

#ifdef FRAMEWORK_ENABLED
    LOG_TRACE<<"CAppPandoraTrack::SetAudioPathRPM <-"<<LOG_ENDL;

    if (m_pRpmClientObj != NULL)
    {
        switch ( inDeviceType )
        {
           case DeviceInfo::EUSBPort1:
                m_pRpmClientObj->RPMClientAcquire(eIPOD1PandoraSPDIF, eMain);
                break;
            case DeviceInfo::EUSBPort2:
                m_pRpmClientObj->RPMClientAcquire(eIPOD2PandoraSPDIF, eMain);
                break;
            case DeviceInfo::EBTConn:
                m_pRpmClientObj->RPMClientAcquire (eBTPandora, eMain);
                break;
            case DeviceInfo::ENoDevice:
            default:
                LOG_CRITICAL<<"It should not come here!!!"<<LOG_ENDL;
                break;
        }
    }
    LOG_TRACE<<"CAppPandoraTrack::SetAudioPathRPM ->"<<LOG_ENDL;
#endif

}
#endif

#if 0
#ifdef FRAMEWORK_ENABLED
void CAppPandoraTrack::OnMICOMVolumeStepSignal(TAudioMode /*eAudioMode*/, qint8 /*nVolumeValue*/)
{
   //ToDo:: Handle the volume control
}

void CAppPandoraTrack::OnASMCallInProgressSignal()
{
    if(nPlayPause == 1)
        {

            m_PndrInterface->Pause();
        }
}
void CAppPandoraTrack::OnASMModeChangeSignal()
{
    //ToDo:: handle the mode change event based on the previous satus of pandora.
}

#endif
#endif
/**
*******************************************************************
* AV_MODE has changed , Other AV apps currently running
* So Pause pandora . Play pandora on FG event.
*
* @param None
* @return None
*
*******************************************************************
*/
void CAppPandoraTrack::HandleAVModeChange()
{
    LOG_HIGH<<"CAppPandoraTrack::HandleAVModeChange() nPlayPause:"<<nPlayPause<< LOG_ENDL;
    //LOG_INFO<<QDateTime::currentDateTime().toString("hh:mm:ss.zzz").toUtf8()<<" CAppPandoraTrack::HandleAVModeChangePause"<<LOG_ENDL;
    if(nPlayPause)
    {
        Pause();
    }
}


/**
 *******************************************************************
 * Request from QML Layer to get elapsed time for current track
 *
 * @param None
 *
 * @return None
 *
 *******************************************************************
 */
void CAppPandoraTrack::SetTrackElapsedPolling(bool inEnabled)
{
    m_PndrInterface->SetTrackElapsedPolling(inEnabled);

    //LOG_INFO<<"SetTrackElapsedPolling() status : "<<inEnabled<<LOG_ENDL;
}


void CAppPandoraTrack::SaveState(DeviceInfo* inDevice)
{    
    inDevice->SetStToken(m_nCurrentStationToken);
}

void CAppPandoraTrack::RetriveState(DeviceInfo* inDevice)
{
    m_nCurrentStationToken = inDevice->StToken();
    LOG_HIGH<<"RetriveState() m_nCurrentStationToken : "<<m_nCurrentStationToken<<LOG_ENDL;
    if(m_nCurrentStationToken)
        nPlayPause = 0 ;

}

//{ modified by yongkyun.lee 2014-03-11 for : BT streaming Mute-play control
void CAppPandoraTrack::startPlayTimer()
{
    LOG_HIGH<<"CAppPandoraTrack::startPlayTimer :"  << LOG_ENDL;
    m_PlayTimer->start();
}

void CAppPandoraTrack::onPlayTimer()
{
    LOG_HIGH<<"CAppPandoraTrack::onPlayTimer" << LOG_ENDL;
    m_PlayTimer->stop();
    if(1 == nPlayPause && 5/*PRI_STATE_PLAYING*/ == m_PndrInterface->GetPriState())
    {
        m_pMainEngine->startPlayMuteTimer();
        m_pMainEngine->setModeStateHistoryFMode(false);
    }
    else
        HandleIfError(m_PndrInterface->Play());
}
//} modified by yongkyun.lee 2014-03-11 

//{ modified by yongkyun.lee 2014-08-25 for : Branding Image ftw
void CAppPandoraTrack::onBrandingImageStart()
{
     //return;
     LOG_INFO<<"[leeyk1]CAppPandoraTrack::onBrandingImageStart():m_BRImage="<< m_BRImage <<LOG_ENDL;
     if(m_BRImage)
         return ;
     m_PndrInterface->RequestBrandingImage(BRANDING_IMAGE_PATH,BRANDING_IMAGE_NAME );
     m_BRImage = true;

    return;
}
//{ modified by yongkyun.lee 2014-08-25
//End Of File

