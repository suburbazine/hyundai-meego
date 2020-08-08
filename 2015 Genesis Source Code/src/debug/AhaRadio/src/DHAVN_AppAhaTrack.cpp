#include "DHAVN_AppAhaTrack.h"
#include "DHAVN_AppAha_QmlProperty.h"
#include "DHAVN_AppAha_ModelChild.h"
#include "DHAVN_AppAhaStationList.h"
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <DHAVN_QRPMClient.h>

CAppAhaTrack* CAppAhaTrack::_instance = NULL;

CAppAhaTrack::CAppAhaTrack(CAhaInterface&  inAhaInterface):
    m_AhaInterface(inAhaInterface)
#ifdef FRAMEWORK_ENABLED
  , m_pRpmClientObj(NULL)
#endif
  , nUserRating(0)
  , m_bPlayPause(-1) //hsryu_0612_initialize_flag_status_bt_call
  , bReceiveTrackInfo (1) //wsuk.kim ISV_77919 repeat_key
{
    if(!_instance) _instance = this;

    m_pSignals = &(m_AhaInterface.GetSignalProvider());

    QString name = "AppAha";
#ifdef FRAMEWORK_ENABLED
    m_pRpmClientObj = new QRPMClient(&name);
#endif
    m_TrackInfo = new TrackInfo();

    mAlbumArtFilePath.clear();
    Qt::ConnectionType ConnType = Qt::QueuedConnection;

    connect(m_pSignals,
        SIGNAL(TrackUpdated(const quint32 )),
        this,
        SLOT(OnTrackUpdated(const quint32 )),
        ConnType);

    connect(m_pSignals,
        SIGNAL(TrackInfo(const AhaTSTrackInfo& )),
        this,
        SLOT(OnTrackInfoReceived(const AhaTSTrackInfo& )),
        ConnType);

    connect(m_pSignals,
        SIGNAL(Result(const ERequestType , const EAhaError )),
        this,
        SLOT(OnRequestCompleted(const ERequestType , const EAhaError )),
        ConnType);

    connect(m_pSignals,
        SIGNAL(AlbumArt(const QString&)),
        this,
        SLOT(OnAlbumArtReceived(const QString&)),
        ConnType);

    connect(m_pSignals,
        SIGNAL(PartnerLogo(const QString&)),
        this,
        SLOT(OnPartnerLogoReceived(const QString&)),
        ConnType);

    connect(m_pSignals,
        SIGNAL(LikeImage(const int,const QString&)),
        this,
        SLOT(OnLikeImageReceived(const int, const QString&)));

    connect(m_pSignals,
        SIGNAL(UpdateLikeDislikeIcon(const uint8_t, const uint8_t)),
        this,
        SLOT(OnUpdateLikeDislikeIcon(const uint8_t, const uint8_t)));

    connect(m_pSignals,
        SIGNAL(RatingImage(const QString&)),
        this,
        SLOT(OnRatingImageReceived(const QString&)),
        ConnType);

    connect(m_pSignals,
        SIGNAL(StationLogo(const QString&)),
        this,
        SLOT(OnStationLogoReceived(const QString&)),
        ConnType);
//wsuk.kim ISV_77919 repeat_key
    connect(m_pSignals,
        SIGNAL(RepeatPressedSkipBack(const bool)),
        this,
        SLOT(OnRepeatPressedSkipBackReceived(const bool)));
//wsuk.kim ISV_77919 repeat_key
    m_nTrackToken = 0;

    LOG_INIT_V2("Aha");
}

CAppAhaTrack::~CAppAhaTrack()
{
    if(m_TrackInfo) delete m_TrackInfo;

#ifdef FRAMEWORK_ENABLED
    if(m_pRpmClientObj) delete m_pRpmClientObj;
#endif
}

CAppAhaTrack* CAppAhaTrack::getInstance()
{
    return _instance;
}

//hsryu_0404_restore_playmode
void CAppAhaTrack::ClearCache()
{
     nUserRating = 0 ;
     //hsryu_0612_initialize_flag_status_bt_call
     m_bPlayPause = 1 ;
     //hsryu_0603_cluster_update
     m_TrackInfo->setArtist("");
}

void CAppAhaTrack::initialize()
{
//    const AhaTSTrackInfo inTrackInfo;
//    m_TrackInfo->SetTrackProperties(inTrackInfo);

    mAlbumArtFilePath.clear();
    nUserRating = 0 ;
    //hsryu_0612_initialize_flag_status_bt_call
    m_bPlayPause = -1 ;
    bReceiveTrackInfo = true; //wsuk.kim ISV_77919 repeat_key
}

void CAppAhaTrack::SetPlayPauseState(int nState)
{
    m_bPlayPause = nState;
}

QString CAppAhaTrack::GetCurrentTrackTitle()
{
    if(!m_TrackInfo) return QString();

    return m_TrackInfo->getTitle();
}

//hsryu_0228 ITS # 0158377
QString CAppAhaTrack::GetCurrentArtist()
{
    if(!m_TrackInfo) return QString();

    return m_TrackInfo->getArtist();
}

TrackInfo* CAppAhaTrack::getTrackInfo()
{
    return m_TrackInfo;
}

void CAppAhaTrack::Play()
{
    m_AhaInterface.play();
}

void CAppAhaTrack::PlayPause()
{
    m_AhaInterface.playPause();
}

void CAppAhaTrack::Pause()
{
    m_AhaInterface.pause();
}

void CAppAhaTrack::Skip()
{
    if(bReceiveTrackInfo)   //wsuk.kim ISV_77919 repeat_key
    {
        //hsryu_0422_can_test
        emit clearTpMessage();
        m_AhaInterface.next();
        bReceiveTrackInfo = false;
    }
}

void CAppAhaTrack::SkipBack()
{
    if(bReceiveTrackInfo)   //wsuk.kim ISV_77919 repeat_key
    {
        //hsryu_0422_can_test
        emit clearTpMessage();
        m_AhaInterface.back();
        bReceiveTrackInfo = false;
    }
}

void CAppAhaTrack::Rewind15()
{
    m_AhaInterface.rewind15(); 
}

void CAppAhaTrack::Forward30()
{
    m_AhaInterface.forward30();
}


void CAppAhaTrack::LikeButtonClicked()
{
    m_AhaInterface.likeButtonClicked();
}

void CAppAhaTrack::DislikeButtonClicked()
{
    m_AhaInterface.dislikeButtonClicked();
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
void CAppAhaTrack::RequestTrackInfo()
{
    LOG_HIGH <<"CAppAhaTrack::RequestTrackInfo..." <<LOG_ENDL;
    CAppAhaQmlProperty::getInstance()->setFocusContentList(false);

    if(m_TrackInfo)
    {
        LOG_HIGH <<"CAppAhaTrack::RequestTrackInfo...m_TrackInfo not null" <<LOG_ENDL;
        emit trackInfoReceived();

        //hsryu_0314_default_albumart
//        if(0 < mAlbumArtFilePath.length())
//        {
//            emit albumArtReceived(mAlbumArtFilePath);
//        }

        /*
        if(m_TrackInfo->getStationArt().length() > 0)
        {
            //we do not need this code anymore
            emit stationArtReceived(0,m_TrackInfo->getStationArt());
        }*/

        if(m_StationLogoFilePath.length() >0)
        {
            emit stationLogoReceived(m_StationLogoFilePath);
        }
//hsryu_0314_default_albumart
        if(0 < m_PartnerLogoFilePath.length())
        {
            emit partnerLogoReceived(m_PartnerLogoFilePath);
        }
        if(0 < m_RatingImageFilePath.length())
        {
            emit ratingImageReceived(m_RatingImageFilePath);
        }
        //remove condition sentence to show blank Album art image.
        //if(0 < mAlbumArtFilePath.length())
        {
            emit albumArtReceived(mAlbumArtFilePath);
        }
//hsryu_0314_default_albumart

        //let us ask state handler to emit updatelikedislikeimage
        //hsryu_0326_like_dislike
        //ITS_222237, 223444
        m_AhaInterface.showCurrentLikeDislikeIcon();
    }
    else
    {
        LOG_HIGH <<"CAppAhaTrack::RequestTrackInfo...m_TrackInfo == null" <<LOG_ENDL;
        //hsryu_0612_initialize_flag_status_bt_call
        m_bPlayPause = -1;
    }
    CAppAha_ModelContent::getInstance()->clear();
    LOG_HIGH <<"CAppAhaTrack::RequestTrackInfo...out" <<LOG_ENDL;
}

//hsryu_0326_like_dislike
void CAppAhaTrack::DisplayCurrentLikeDislike()
{
    LOG_HIGH <<"[ CAppAhaTrack::DisplayCurrentLikeDislike ] send updateLikeDislikeFilePath signal...11111"<<LOG_ENDL;
    emit updateLikeDislikeFilePath(m_CurrentLikeUnSelected, m_CurrentLikeSelected,
                                   m_CurrentDisLikeUnSelected, m_CurrentDisLikeSelected);
}

void CAppAhaTrack::ClearCurrentLikeDislike()
{
    m_CurrentLikeUnSelected.clear();
    m_CurrentLikeSelected.clear();
    m_CurrentDisLikeUnSelected.clear();
    m_CurrentDisLikeSelected.clear();

    m_LikeUnselectedFilePath.clear();
    m_DislikeUnselectedFilePath.clear();
    m_LikeSelectedFilePath.clear();
    m_DislikeSelectedFilePath.clear();
}
//hsryu_0326_like_dislike
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
void CAppAhaTrack::ClearTrackInfo()
{          
    //    const AhaTSTrackInfo inTrackInfo;
    //    m_TrackInfo->SetTrackProperties(inTrackInfo);
    //hsryu_0612_update_optimize
//wsuk.kim 130910 ITS_0188998 like/dislike didnot display after loading fail.    m_AhaInterface.m_AhaStateHandler.clearContentImagesDisplay();
    m_AhaInterface.m_AhaStateHandler.cancelTrackViewImageDownload();

    m_TrackInfo->ClearStringData();
    m_TrackInfo->ClearTrackProperty();

    mAlbumArtFilePath.clear();
    nUserRating = 0 ;
    //hsryu_0612_initialize_flag_status_bt_call
    //m_bPlayPause = false ;
    bReceiveTrackInfo = true; //wsuk.kim ISV_77919 repeat_key
}

/**
 *******************************************************************
 * The result of request from Aha MW
 * Non network related errors should be handled here
 *
 * @param[in] inRequestType The request for which the reply is
 *                          received
 * @param[in] inAhaError   The result of the request
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppAhaTrack::OnRequestCompleted(const ERequestType inRequestType, const EAhaError inAhaError)
{
    LOG_HIGH <<"CAppAhaTrack::OnRequestCompleted...inRequestType" <<inRequestType<<LOG_ENDL;

    if(E_SUCCESS == inAhaError)
    {
        switch(inRequestType)
        {
            case REQ_TYPE_CONNECT:
                //TAN_PRINTF("REQ_TYPE_CONNECT\n");
                m_bPlayPause = 1;
                emit playStarted();
                break;
            case REQ_TYPE_PLAY:
                //TAN_PRINTF("REQ_TYPE_PLAY\n");
//hsryu_0612_initialize_flag_status_bt_call
                m_bPlayPause = 1;
                emit playStarted();
                break;
            case REQ_TYPE_PAUSE:
                //TAN_PRINTF("REQ_TYPE_PAUSE\n");
//hsryu_0612_initialize_flag_status_bt_call
                m_bPlayPause = 0;
                emit pauseDone();
                break;
            default:
                //TAN_PRINTF("default\n");
                m_bPlayPause = 1;
                emit playStarted();
                break;
        }
    }
}

/**
 *******************************************************************
 * This is the slot to listen to signal from Aha MW
 * for track update information
 *
 * @param[in] inTrackToken  The token ID for new track
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppAhaTrack::OnTrackUpdated(const quint32 inTrackToken)
{
    LOG_HIGH <<"CAppAhaTrack::OnTrackUpdated...inTrackToken:" <<inTrackToken<<LOG_ENDL;
    m_nTrackToken = inTrackToken;
    nUserRating = 0;
    if(inTrackToken > 0)
    {
        emit trackUpdated();
    }
}


/**
 *******************************************************************
 * This is the slot to listen to signal from Aha MW
 * for track information
 *
 * @param[in] inTrackInfo the track information
 *
 * @return  None
 *
 *******************************************************************
 */
void CAppAhaTrack::OnTrackInfoReceived(const AhaTSTrackInfo& inTrackInfo)
{
    LOG_HIGH <<"CAppAhaTrack::OnTrackInfoReceived..." <<LOG_ENDL;
    //hsryu_0326_like_dislike
    m_TrackInfo->ClearTrackProperty();

    m_TrackInfo->SetTrackProperties(inTrackInfo);
    //hsryu_0228 remove LF,space at front,end
    setElide60WordsArtistName(inTrackInfo.Artist);  //wsuk.kim_130628_ITS_0177003 40->60... words    m_TrackInfo->setArtist(inTrackInfo.Artist.trimmed());
    setElide60WordsTitleName(inTrackInfo.Title);    //wsuk.kim_130628_ITS_0177003 40->60... words    m_TrackInfo->setTitle(inTrackInfo.Title.trimmed());
    m_TrackInfo->setAlbum(inTrackInfo.Album.trimmed());

    emit trackInfoReceived();

    //hsryu_0322_TRANSPORT_CAN
    emit trackTitleReceived(inTrackInfo.Artist.trimmed());

    CAppAhaQmlProperty::getInstance()->setActiveStationName(inTrackInfo.CurrentStation);
    CAppAhaStationList::getInstance()->setStationSection();

    // if album art is rreceived before track info received , Then emit signal.

    //remove condition sentence to show blank Album art image.
    //if(0 < mAlbumArtFilePath.length())
    {
        emit albumArtReceived(mAlbumArtFilePath);
    }
    if(0 < m_PartnerLogoFilePath.length())
    {
        emit partnerLogoReceived(m_PartnerLogoFilePath);
    }
    if(0 < m_RatingImageFilePath.length())
    {
        emit ratingImageReceived(m_RatingImageFilePath);
    }
    if(0 < m_StationLogoFilePath.length())
    {
        emit stationLogoReceived(m_StationLogoFilePath);
    }

//wsuk.kim TUNE_TRACK
    if(CAppAhaQmlProperty::getInstance()->getFirstTrackEntrance())
    {
        emit receivingStationShowPopup();
        CAppAhaStationList::getInstance()->requestStationList();
        CAppAhaQmlProperty::getInstance()->setFirstTrackEntrance(false);
    }
//wsuk.kim TUNE_TRACK
}

/**
 *******************************************************************
 * This is the slot to listen to signal from Aha MW
 * for track album art
 *
 * @param[in] inAlbumArt the track album art path
 *******************************************************************
 */
 void CAppAhaTrack::OnAlbumArtReceived(const QString& inAlbumArt)
 {
     LOG_HIGH <<"(((CAppAhaTrack::OnAlbumArtReceived...inAlbumArt:" <<inAlbumArt<<", current:"<<mAlbumArtFilePath<<LOG_ENDL;
     if(mAlbumArtFilePath.compare(inAlbumArt) == 0) return;
      mAlbumArtFilePath = inAlbumArt;
      emit albumArtReceived(inAlbumArt);
 }

 void CAppAhaTrack::OnPartnerLogoReceived(const QString& strPartnerLogo)
 {
     LOG_HIGH <<"(((CAppAhaTrack::OnPartnerLogoReceived...strPartnerLogo:" <<strPartnerLogo<<", current:"<<m_PartnerLogoFilePath<<LOG_ENDL;
     if(m_PartnerLogoFilePath.compare(strPartnerLogo) == 0) return;

     m_PartnerLogoFilePath = strPartnerLogo;
     emit partnerLogoReceived(strPartnerLogo);
 }

 void CAppAhaTrack::OnRatingImageReceived(const QString& strRatingImage)
 {
     LOG_HIGH <<"(((CAppAhaTrack::OnRatingImageReceived...strRatingImage:"<<strRatingImage<<",current" <<m_RatingImageFilePath<<LOG_ENDL;
     if(m_RatingImageFilePath.compare(strRatingImage) == 0) return;

     m_RatingImageFilePath = strRatingImage;
     emit ratingImageReceived(strRatingImage);
 }

 void CAppAhaTrack::OnStationLogoReceived(const QString& strStationLogo)
 {
     LOG_HIGH <<"(((CAppAhaTrack::OnStationLogoReceived...strStationLogo:"<<strStationLogo<<", current:"<<m_StationLogoFilePath <<LOG_ENDL;
     if(m_StationLogoFilePath.compare(strStationLogo) == 0) return;

     m_StationLogoFilePath = strStationLogo;
     emit stationLogoReceived(strStationLogo);
 }

 void CAppAhaTrack::OnUpdateLikeDislikeIcon(const uint8_t nRateState, const uint8_t nCanUnvote)
 {
     LOG_HIGH <<"(((CAppAhaTrack::OnUpdateLikeDislikeIcon...state:" <<nRateState<<" nCanUnvote:"<<nCanUnvote<<LOG_ENDL;
     QString currLikeIcon, altLikeIcon;
     QString currDislikeIcon, altDislikeIcon;

     if(nRateState == AHA_RATE_LIKE_SELECTED)
     {
        currLikeIcon = m_LikeSelectedFilePath;
        currDislikeIcon = m_DislikeUnselectedFilePath;
        altDislikeIcon = m_DislikeSelectedFilePath;
        if(nCanUnvote)
        {
            altLikeIcon = m_LikeUnselectedFilePath;
        }
        else
        {
            altLikeIcon = currLikeIcon;
        }
     }
     else if(nRateState == AHA_RATE_DISLIKE_SELECTED)
     {
         currLikeIcon = m_LikeUnselectedFilePath;
         currDislikeIcon = m_DislikeSelectedFilePath;
         altLikeIcon = m_LikeSelectedFilePath;
         if(nCanUnvote)
         {
             altDislikeIcon = m_DislikeUnselectedFilePath;
         }
         else
         {
             altDislikeIcon = currDislikeIcon;
         }
     }
     else if(nRateState == AHA_RATE_NONE_SELECTED)
     {
         currLikeIcon = m_LikeUnselectedFilePath;
         currDislikeIcon = m_DislikeUnselectedFilePath;
         altLikeIcon = m_LikeSelectedFilePath;
         altDislikeIcon = m_DislikeSelectedFilePath;
     }
     else if(nRateState == AHA_RATE_BOTH_SELECTED)
     {
         currLikeIcon = m_LikeSelectedFilePath;
         currDislikeIcon = m_DislikeSelectedFilePath;
         if(nCanUnvote)
         {
            altLikeIcon = m_LikeUnselectedFilePath;
            altDislikeIcon = m_DislikeUnselectedFilePath;
         }
         else
         {
             altLikeIcon = currLikeIcon;
             altDislikeIcon = currDislikeIcon;
         }
     }

     //hsryu_0326_like_dislike
     m_CurrentLikeUnSelected.clear();
     m_CurrentLikeSelected.clear();
     m_CurrentDisLikeUnSelected.clear();
     m_CurrentDisLikeSelected.clear();
     m_CurrentLikeUnSelected = currLikeIcon;
     m_CurrentLikeSelected = altLikeIcon;
     m_CurrentDisLikeUnSelected = currDislikeIcon;
     m_CurrentDisLikeSelected = altDislikeIcon;

     LOG_HIGH <<"[ CAppAhaTrack::DisplayCurrentLikeDislike ] send updateLikeDislikeFilePath signal...222222111"<<LOG_ENDL;
     emit updateLikeDislikeFilePath(currLikeIcon, altLikeIcon, currDislikeIcon, altDislikeIcon);
 }

 void CAppAhaTrack::OnLikeImageReceived(const int nIndex, const QString &filePath)
 {
     LOG_HIGH <<"CAppAhaTrack::OnLikeImageReceived...nIndex:" <<nIndex<<LOG_ENDL;
     switch(nIndex)
     {
     case AHA_IMAGE_LIKE_S:
         m_LikeSelectedFilePath.clear();
         m_LikeSelectedFilePath = filePath;
         break;
     case AHA_IMAGE_LIKE_U:
         m_LikeUnselectedFilePath.clear();
         m_LikeUnselectedFilePath = filePath;
         break;
     case AHA_IMAGE_DISLIKE_S:
         m_DislikeSelectedFilePath.clear();
         m_DislikeSelectedFilePath = filePath;
         break;
     case AHA_IMAGE_DISLIKE_U:
         m_DislikeUnselectedFilePath.clear();
         m_DislikeUnselectedFilePath = filePath;
         break;
     default:
         break;
     }
 }
//wsuk.kim ISV_77919 repeat_key
void CAppAhaTrack::OnRepeatPressedSkipBackReceived(const bool bRepeatState)
{
    LOG_HIGH <<"CAppAhaTrack::OnRepeatPressedSkipBackReceived..." <<LOG_ENDL;
    bReceiveTrackInfo = bRepeatState;
}
//wsuk.kim ISV_77919 repeat_key
/**
 *******************************************************************
 * Handle the error
 *
 * @param[in] inAhaError error code
 *
 * @return None
 *
 *******************************************************************
 */
void CAppAhaTrack::HandleIfError(const EAhaError inAhaError)
{
    LOG_HIGH <<"CAppAhaTrack::HandleIfError...inAhaError:" <<inAhaError<<LOG_ENDL;
    if(E_SUCCESS != inAhaError)
    {
        LOG_CRITICAL<<"Error code :"<<inAhaError<<LOG_ENDL;
        switch(inAhaError)
        {
            //case E_INVALID_STATE:
            case E_INVALID_ARGUMENT:
            {
                // unexpected error
                emit handleError(inAhaError);
            }
            break;

            default:
                break;
        }
    }
}

/**
*******************************************************************
* Aquire audio path while connecting/resuming aha.
*
* @param  inDeviceType , Connected device type.
*
* @return None
*
*******************************************************************
*/
void CAppAhaTrack::SetAudioPathRPM (DeviceInfo::EConnectionType inDeviceType)
{
#if 0
#ifdef FRAMEWORK_ENABLED

    if (m_pRpmClientObj != NULL)
    {
        switch ( inDeviceType )
        {
           case DeviceInfo::EUSBPort1:
                m_pRpmClientObj->RPMClientAcquire(eIPOD1AhaRadioSPDIF, eMain);
                break;
            case DeviceInfo::EUSBPort2:
                m_pRpmClientObj->RPMClientAcquire(eIPOD2AhaRadioSPDIF, eMain);
                break;
            case DeviceInfo::EBTConn:
                m_pRpmClientObj->RPMClientAcquire (eBTAhaRadio, eMain);
                break;
            case DeviceInfo::ENoDevice:
            default:
                break;
        }
    }

#endif
#endif
}


/**
*******************************************************************
* AV_MODE has changed , Other AV apps currently running
* So Pause aha . Play aha on FG event.
*
* @param None
* @return None
*
*******************************************************************
*/
void CAppAhaTrack::HandleAVModeChange()
{
    //TAN_PRINTF("CAppAhaTrack::HandleAVModeChange()  m_bPlayPause = %d \n", m_bPlayPause);
    LOG_HIGH <<"CAppAhaTrack::HandleAVModeChange...m_bPlayPause:" <<m_bPlayPause<<LOG_ENDL;

    if(m_bPlayPause)
    {
        Pause();
    }
}

//wsuk.kim_130628_ITS_0177003 40->60... words
void CAppAhaTrack::setElide60WordsArtistName(const QString &trackInfoArtistName)
{
    QString inTrackInfoArtist;
    if( 57 < trackInfoArtistName.length())
    {
        inTrackInfoArtist = trackInfoArtistName.mid(0, 57);
        inTrackInfoArtist += "...";
    }
    else
    {
        inTrackInfoArtist = trackInfoArtistName;
    }
    m_TrackInfo->setArtist(inTrackInfoArtist.trimmed());
}

void CAppAhaTrack::setElide60WordsTitleName(const QString &trackInfoTitleName)
{
    QString inTrackInfoTitle;
    if( 57 < trackInfoTitleName.length())
    {
        inTrackInfoTitle = trackInfoTitleName.mid(0, 57);
        inTrackInfoTitle += "...";
    }
    else
    {
        inTrackInfoTitle = trackInfoTitleName;
    }
    m_TrackInfo->setTitle(inTrackInfoTitle.trimmed());
}
//wsuk.kim_130628_ITS_0177003 40->60... words
//End Of File

