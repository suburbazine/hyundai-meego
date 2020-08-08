#include "ahainterface.h"
#include <QtDBus/QtDBus>


CAhaInterface::CAhaInterface()
{
LOG_INIT_V2("Aha");
}

CAhaInterface::~CAhaInterface()
{
    DeInit();
}

CAhaStateHandler& CAhaInterface::GetSignalProvider()
{
    return m_AhaStateHandler;
}

int CAhaInterface::Init()
{
    if(DEVICE_TYPE_SMARTPHONE != m_AhaStateHandler.getDeviceType() )
    {
        DeInit();
    }

    connect(&m_AhaStateHandler, SIGNAL(contentListLoaded(QVector<AhaContentBriefInfo_T>&)),
            this, SIGNAL(contentListLoaded(QVector<AhaContentBriefInfo_T>&)));
    connect(&m_AhaStateHandler, SIGNAL(stationListLoaded(QVector<AhaStationBriefInfo_T>&)),
            this, SIGNAL(stationListLoaded(QVector<AhaStationBriefInfo_T>&)));

    return 0;
}

int CAhaInterface::DeInit()
{
    LOG_HIGH<<"[ CAhaInterface::DeInit()...............@@@@@@@@@ ] "<<LOG_ENDL;
    m_AhaStateHandler.Disconnect();
    return 0;
}

EAhaError CAhaInterface::Connect(EDeviceType inDeviceType)
{
    EAhaError nAhaErr;
    nAhaErr = m_AhaStateHandler.Connect(inDeviceType);
    return nAhaErr;
}

int CAhaInterface::Disconnect()
{
    LOG_HIGH<<"[ CAhaInterface::Disconnect()...............@@@@ ] "<<LOG_ENDL;
    m_AhaStateHandler.Disconnect();
    return 0;
}

void CAhaInterface::queryingPresetStations()
{
    m_AhaStateHandler.queryingPresetStations();
}

void CAhaInterface::queryingContentList()
{
    m_AhaStateHandler.queryingContentList();
}


void CAhaInterface::selectPresetsToPlay(int nIndex)
{
    uint64_t nStationID;
    nStationID = m_AhaStateHandler.m_AhaActiveFrameTxRx->m_PresetStationsInfo[nIndex].nStationID;
  	m_AhaStateHandler.selectStationToPlay(nStationID);
}

void CAhaInterface::selectPresetsToPlayID(uint64_t appID)
{
    m_AhaStateHandler.selectStationToPlay(appID);
}

void CAhaInterface::selectContentToPlay(int nIndex)
{
    uint64_t nContentID;

    if(0 < m_AhaStateHandler.m_AhaActiveFrameTxRx->m_ContentListsInfo.size())
    {
        // m_PresetQueryIndex MUST be a valid index position(i.e., 0 <= m_PresetQueryIndex < m_PresetStationsInfo.size()).
        if( (0 <= nIndex) && (m_AhaStateHandler.m_AhaActiveFrameTxRx->m_ContentListsInfo.size() > nIndex) )
        {
            nContentID = m_AhaStateHandler.m_AhaActiveFrameTxRx->m_ContentListsInfo[nIndex].nContentID;
            m_AhaStateHandler.selectContentToPlay(nContentID);
        }
    }
    else
    {
        // do nothing???? need to show popup for warning.   25.Feb.2014 heemin.kang@lge.com
    }

}

void CAhaInterface::selectContentToPlayID(uint64_t appID)
{
    m_AhaStateHandler.selectContentToPlay(appID);
}

void CAhaInterface::pause()
{
    m_AhaStateHandler.processRequest(AHA_PAUSE_REQUEST);
}

void CAhaInterface::playPause()
{
    LOG_HIGH << QString(__FUNCTION__)<<LOG_ENDL;
    bool bIsPlaying;
    AhaClientInfo_T* pAhaClientInfo;
    pAhaClientInfo = m_AhaStateHandler.m_AhaActiveFrameTxRx->getAhaClientInfo();
    bIsPlaying = (pAhaClientInfo->nPlaybackState == AHA_PLAYBACK_PLAY) ? true : false;

    if(pAhaClientInfo->nPlaybackState == AHA_PLAYBACK_BUFFERING)    //wsuk.kim buffering
    {
        pause();
        return;
    }

    if(bIsPlaying)
    {
        pause();
    }
    else
    {
        play();
    }
}

void CAhaInterface::play()
{
    m_AhaStateHandler.processRequest(AHA_PLAY_REQUEST);
}

void CAhaInterface::next()
{
    m_AhaStateHandler.processRequest(AHA_NEXT_REQUEST);
}

void CAhaInterface::back()
{
    m_AhaStateHandler.processRequest(AHA_BACK_REQUEST);
}

void CAhaInterface::logCall()
{
    m_AhaStateHandler.processRequest(AHA_LOG_CALL_REQUEST);

}
void CAhaInterface::logNavigate()
{
    m_AhaStateHandler.processRequest(AHA_LOG_NAVIGATE_REQUEST);
}

void CAhaInterface::rewind15()
{
    m_AhaStateHandler.processRequest(AHA_REWIND15_REQUEST);
}
void CAhaInterface::forward30()
{
    m_AhaStateHandler.processRequest(AHA_FORWARD30_REQUEST);
}

void CAhaInterface::likeButtonClicked()
{
    m_AhaStateHandler.processRequest(AHA_LIKE_VOTE_UNVOTE_REQUEST);
}

void CAhaInterface::dislikeButtonClicked()
{
    m_AhaStateHandler.processRequest(AHA_DISLIKE_VOTE_UNVOTE_REQUEST);
}

void CAhaInterface::showCurrentLikeDislikeIcon()
{
    m_AhaStateHandler.ShowCurrentLikeDislikeIcon();
}

bool CAhaInterface::queryPhoneNumber(QString &strPhoneNumber)
{
    AhaContentInfo_T* pAhaContentInfo;
    if(m_AhaStateHandler.m_CurrentTrackInfo.bAllowCall)
    {
        pAhaContentInfo = m_AhaStateHandler.m_AhaActiveFrameTxRx->getAhaContentInfo();
        strPhoneNumber.clear();
        strPhoneNumber = pAhaContentInfo->strPhone;
        return true;
    }
    else
    {
        return false;
    }
}

bool CAhaInterface::queryAhaLBSValue(QString &strName,
                                     QString &strStateName,
                                     QString &strStreet,
                                     QString &strCity,
                                     QString &strZipCode,
                                     QString &strPhoneNumber,
                                     double &dbLon, double &dbLat)
{
    AhaContentInfo_T* pAhaContentInfo;
    int nContentInfoCount;

    if(m_AhaStateHandler.m_CurrentTrackInfo.bAllowNavigate)
    {
        pAhaContentInfo = m_AhaStateHandler.m_AhaActiveFrameTxRx->getAhaContentInfo();

        dbLon = pAhaContentInfo->dbLon;
        dbLat = pAhaContentInfo->dbLat;

        strName.clear();
        strStreet.clear();
        strStateName.clear();

        nContentInfoCount = pAhaContentInfo->strQVectContentInfoString.count();

        switch(nContentInfoCount)
        {
        case 3:
            strStateName = pAhaContentInfo->strQVectContentInfoString[2];
        case 2:
            strStreet = pAhaContentInfo->strQVectContentInfoString[1];
        case 1:
            strName = pAhaContentInfo->strQVectContentInfoString[0];
            break;
        default:
            if(nContentInfoCount > 3)
            {
                strStateName = pAhaContentInfo->strQVectContentInfoString[2];
                strStreet = pAhaContentInfo->strQVectContentInfoString[1];
                strName = pAhaContentInfo->strQVectContentInfoString[0];
            }//else do nothing
            break;
        }

        strCity.clear();
        strZipCode.clear();
        strPhoneNumber.clear();
        strPhoneNumber = pAhaContentInfo->strPhone;
        return true;
    }
    else
    {
        return false;
    }
}


bool CAhaInterface::reqSendContentInfoToNavigation(QString strName,
                                                   QString strStateName,
                                                   QString strStreet,
                                                   QString strCity,
                                                   QString strZipCode,
                                                   QString strPhoneNumber,
                                                   double dbLon, double dbLat)
{
#ifndef __WIN32__

#define XMDATA_NAVI_CONNECTION              "XMDataDbusConnectionToNavi"
//dbus write protocol
#define XMDATA_NAVI_DBUS_PATH                  "/com/Hyundai/navigation"
#define XMDATA_NAVI_DBUS_INTERFACE             "com.hyundai.navigation"
#define XMDATA_NAVI_SERVICE_NAME               "com.hyundai.navigation"
#define XMDATA_NAVI_DBUS_SIGNAL_FOR_FUEL       "GoFuelStationRequested"
#define XMDATA_NAVI_DBUS_SIGNAL_FOR_THEATER    "GoTheaterRequested"

    QDBusConnection m_DbusConnectionToNavi = QDBusConnection::connectToBus(QDBusConnection::SystemBus,
                                                           XMDATA_NAVI_CONNECTION);
    if(m_DbusConnectionToNavi.isConnected() == true)
    {
        qDebug(" connected to dbus daemon");
        QDBusMessage signal = QDBusMessage::createSignal(XMDATA_NAVI_DBUS_PATH,
                                                         XMDATA_NAVI_DBUS_INTERFACE,
                                                         XMDATA_NAVI_DBUS_SIGNAL_FOR_THEATER);

        signal << strName;
        signal << strStateName;
        signal << strStreet;
        signal << strCity;
        signal << strZipCode;
        signal << strPhoneNumber;
        signal << (double)dbLat;
        signal << (double)dbLon;
        signal << (int)0;
        signal << (int)0;

        if(m_DbusConnectionToNavi.send(signal))
        {
            return true;
        }
    }

    return false;
#else
    return false;
#endif
}


bool CAhaInterface::reqSendDataToBTPhone(QString phonenumber)
{
#ifndef __WIN32__
    //Connection with BT Phone
    #define XMDATA_BT_CONNECTION                "XMDataDbusConnectionToBt"
    #define XMDATA_BT_DBUS_PATH                 "/com/Hyundai/btphone"
    #define XMDATA_BT_DBUS_INTERFACE            "com.hyundai.Bt"
    #define XMDATA_BT_SERVICE_NAME              "com.hyundai.Bt"
    #define XMDATA_BT_DBUS_SIGNAL_FOR_CALL      "CallToFuelStationRequested"

    QDBusConnection m_DbusConnectionToBT = QDBusConnection::connectToBus(QDBusConnection::SystemBus,
                                                           XMDATA_BT_CONNECTION);
    if(m_DbusConnectionToBT.isConnected() == true)
    {
        QDBusMessage signal = QDBusMessage::createSignal(XMDATA_BT_DBUS_PATH,
                                                         XMDATA_BT_DBUS_INTERFACE,
                                                         XMDATA_BT_DBUS_SIGNAL_FOR_CALL);
        signal << phonenumber;
        if(m_DbusConnectionToBT.send(signal))
        {
            return true;
        }
    }

    return false;
#else
    XMDEBUG(LOG_INFOXM, " not implemented.");
    return false;
#endif
}

