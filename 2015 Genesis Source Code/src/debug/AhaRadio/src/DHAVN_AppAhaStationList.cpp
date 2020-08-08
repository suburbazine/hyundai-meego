
#include "DHAVN_AppAhaStationList.h"
#include "DHAVN_AppAha_ModelChild.h"
#include "DHAVN_AppAha_QmlProperty.h"

CAppAhaStationList* CAppAhaStationList::_instance = NULL;

CAppAhaStationList::CAppAhaStationList(CAhaInterface&  inAhaInterface):
    m_AhaInterface(inAhaInterface), m_pSignals(NULL)
{
    //LOG_INIT_V2("Aha");

    if(!_instance) _instance = this;

    m_pSignals = &(m_AhaInterface.GetSignalProvider());

    initialize();

    connect(m_pSignals,
        SIGNAL(stationListLoaded(QVector<AhaStationBriefInfo_T>&)),
        this,
        SLOT(OnStationListLoaded(QVector<AhaStationBriefInfo_T>&)));

    connect(m_pSignals,
        SIGNAL(contentListLoaded(QVector<AhaContentBriefInfo_T>&)),
        this,
        SLOT(OnContentListLoaded(QVector<AhaContentBriefInfo_T>&)));

     connect(m_pSignals,
         SIGNAL(notifyActiveContentIDChanged(const qulonglong)),
         this,
         SLOT(OnNotifyContentIDChanged(const qulonglong)));

     connect(m_pSignals,
         SIGNAL(notifyPlaybackStateChanged(uint64_t, uint8_t)),
         this,
         SLOT(OnNotifyPlaybackStateChangedSlot(uint64_t, uint8_t)));

     connect(m_pSignals,
         SIGNAL(StationLogo(const uint8_t, const qulonglong, const QString&)),
         this,
         SLOT(OnStationLogoReceived(const uint8_t, const qulonglong, const QString&)));

     i_timer_work_type = -1;

     LOG_INIT_V2("Aha");
}

CAppAhaStationList* CAppAhaStationList::getInstance()
{
    return _instance;
}

void CAppAhaStationList::initialize()
{
    b_once_request_station = false;
    select_station_id = 0;
    select_content_id = 0;

    current_play_id = 0;
    current_play_status = 0x0;

    i_select_station = STATION_PRESET;

//wsuk.kim TUNE
    i_tune_index = 0;
    b_first_set_tune_index = false;
    b_check_same_station_id = false;
    i_station_focus_index = -1;
    i_track_tune_index = -1;
//wsuk.kim TUNE
}

void CAppAhaStationList::modelClear()
{
    CAppAha_ModelPreset::getInstance()->clear();
    CAppAha_ModelLBS::getInstance()->clear();
    CAppAha_ModelContent::getInstance()->clear();
}

void CAppAhaStationList::requestStationList()
{
    LOG_HIGH <<"[CAppAhaStationList::requestStationList]...b_once_request_station:" <<b_once_request_station<<LOG_ENDL;
    if(b_once_request_station)
    {
        emitStationList();
        return;
    }
    b_once_request_station = true;

    m_AhaInterface.queryingPresetStations();
}

//wsuk.kim TUNE_TRACK
void CAppAhaStationList::requestStationListInTrack()
{
    LOG_HIGH <<"CAppAhaStationList::requestStationListInTrack..." <<LOG_ENDL;
    m_AhaInterface.queryingPresetStations();
}
//wsuk.kim TUNE_TRACK

void CAppAhaStationList::requestContentList()
{
    LOG_HIGH <<"CAppAhaStationList::requestContentList..." <<LOG_ENDL;
    CAppAhaQmlProperty::getInstance()->setFocusContentList(true);
    m_AhaInterface.queryingContentList();
}

void CAppAhaStationList::requestTimer(int interval, int type)
{
    i_timer_work_type = type;
    QTimer::singleShot(interval, this, SLOT(OnTimeout_DelayEvent()));
}

void CAppAhaStationList::emitStationList()
{
    LOG_HIGH <<"CAppAhaStationList::emitStationList..." <<LOG_ENDL;
    CAppAhaQmlProperty* qmlProperty = CAppAhaQmlProperty::getInstance();
    if(CAppAha_ModelPreset::getInstance()->isSelect())
    {
        i_select_station = STATION_PRESET;
        qmlProperty->setFocusArea(5);
        qmlProperty->setFocusScreen(STATION_PRESET);
    }

    if(CAppAha_ModelLBS::getInstance()->isSelect())
    {
        i_select_station = STATION_LBS;
        qmlProperty->setFocusArea(7);
        qmlProperty->setFocusScreen(STATION_LBS);
    }
    emit modelPresetReady(CAppAha_ModelPreset::getInstance()->getSelectRow(), CAppAha_ModelPreset::getInstance()->rowCount());
    emit modelLBSReady(CAppAha_ModelLBS::getInstance()->getSelectRow(), CAppAha_ModelLBS::getInstance()->rowCount());

    emit modelStationNowListenning(getStationNowListenning());
}

void CAppAhaStationList::OnTimeout_DelayEvent()
{
    // 0 ~ 20 : QML Value /  40 ~ 60 : CPP Value

    //1  Preset on Left Menu,  3  LBS on Left Menu,  5  Preset list,  7  LBS list,  9  Title,  11 Track View,
    // 13 ContentList,  15 StationList
    if(i_timer_work_type == 1)
    {
    }
}

void CAppAhaStationList::playStation()
{
    m_AhaInterface.selectPresetsToPlayID(select_station_id);
}

void CAppAhaStationList::playContent()
{
    m_AhaInterface.selectContentToPlayID(select_content_id);
}

QString CAppAhaStationList::getLastSelectedStation()
{
    return (i_select_station == STATION_LBS) ? "LBS" : "PRESET";
}

//wsuk.kim TUNE
QString CAppAhaStationList::getStationNameTune(int tuneIndex, bool bTrackView)
{
    if(bTrackView)
    {
        if(i_select_station == STATION_PRESET)
            return CAppAha_ModelPreset::getInstance()->getStationNameIndex(tuneIndex);
        else
            return CAppAha_ModelLBS::getInstance()->getStationNameIndex(tuneIndex);
    }
    else
    {
        if(CAppAhaQmlProperty::getInstance()->getFocusScreen() == STATION_PRESET)
            return CAppAha_ModelPreset::getInstance()->getStationNameIndex(tuneIndex);
        else
            return CAppAha_ModelLBS::getInstance()->getStationNameIndex(tuneIndex);
    }
}

QString CAppAhaStationList::getStationArtTune(int tuneIndex)    //wsuk.kim 130902 ISV_90329 to display station art image that tune search.
{
    QFileInfo fileInfo;
    QString strStationArtTune;
    QString strDefaultArtTune = "/app/share/images/aha/bg_station_list.png";

    if(i_select_station == STATION_PRESET)
    {
        strStationArtTune = CAppAha_ModelPreset::getInstance()->getStationArtIndex(tuneIndex);
        fileInfo = strStationArtTune;

        if(strStationArtTune != "null" &&  fileInfo.exists())       // ITS 0229151
        {
            return strStationArtTune;
        }
        else
        {
            return strDefaultArtTune;
        }
    }
    else
    {
        strStationArtTune = CAppAha_ModelLBS::getInstance()->getStationArtIndex(tuneIndex);
        fileInfo = strStationArtTune;

        if(strStationArtTune != "null" &&  fileInfo.exists())       // ITS 0229151
        {
            return strStationArtTune;
        }
        else
        {
            return strDefaultArtTune;
        }
    }
}

int CAppAhaStationList::getStationIdTune()
{
    return i_select_station;
}

int CAppAhaStationList::getStationIndexUsedfromStationIdTune()
{
    int presetLBSIndex = 0;

    presetLBSIndex = (i_select_station == STATION_PRESET)? getPresetIndex(): getLBSIndex();

    return presetLBSIndex;
}

int CAppAhaStationList::getStationIdTuneUp()
{
    if(i_tune_index > 0)
    {
        i_tune_index -= 1;
    }
    else
    {
        i_tune_index = (i_select_station == STATION_PRESET)? getPresetCount()-1: getLBSCount()-1;
    }
    //b_check_same_station_id = checkSameStationId(i_tune_index); //0319

    return i_tune_index;
}

int CAppAhaStationList::getStationIdTuneDown()
{
    int nTuneMax = 0;

    nTuneMax = (i_select_station == STATION_PRESET)? getPresetCount()-1: getLBSCount()-1;

    if(i_tune_index < nTuneMax)
    {
        i_tune_index += 1;
    }
    else
    {
        i_tune_index = 0;
    }

    return i_tune_index;
}

void CAppAhaStationList::setContentListTuneIndex(int contentTuneIndex)
{
    i_tune_index = contentTuneIndex;
}

int CAppAhaStationList::getContentListTuneIndex()   //wsuk.kim 131104 displayed at OSD control by tune.
{
    return i_tune_index;
}

bool CAppAhaStationList::checkSameStationId(int tuneIndex)
{
    bool bReturnValue = false;
    int nGetIndex = 0;

    nGetIndex = (i_select_station == STATION_PRESET)? getPresetIndex(): getLBSIndex();

    if(tuneIndex == nGetIndex)
    {
        bReturnValue = true;
    }
    LOG_HIGH<<"$$$$$$$$$$  checkSameStaionID tuneIndex:"<<tuneIndex<<", return true?"<<bReturnValue<<LOG_ENDL;
    return bReturnValue;
}

void CAppAhaStationList::setStationListFocusIndex(int stationFocusInex)
{
    i_station_focus_index = stationFocusInex;
}

int CAppAhaStationList::getStationListFocusIndex()
{
    return i_station_focus_index;
}

void CAppAhaStationList::setTrackViewTuneIndex(int trackTuneIndex)
{
    i_track_tune_index = trackTuneIndex;
}

int CAppAhaStationList::getTrackViewTuneIndex()
{
    return i_track_tune_index;
}
//wsuk.kim TUNE

bool CAppAhaStationList::getStationNowListenning()
{
    if(CAppAha_ModelPreset::getInstance()->isSelect()) return true;

    if(CAppAha_ModelLBS::getInstance()->isSelect()) return true;

    return false;
}

int CAppAhaStationList::getPresetIndex()
{
    return CAppAha_ModelPreset::getInstance()->getSelectRow();
}

int CAppAhaStationList::getLBSIndex()
{
    return CAppAha_ModelLBS::getInstance()->getSelectRow();
}

int CAppAhaStationList::getContentIndex()
{
    return CAppAha_ModelContent::getInstance()->getSelectRow();
}

int CAppAhaStationList::getPresetCount()
{
    return CAppAha_ModelPreset::getInstance()->rowCount();
}

int CAppAhaStationList::getLBSCount()
{
    return CAppAha_ModelLBS::getInstance()->rowCount();
}

int CAppAhaStationList::getContentCount()
{
    return CAppAha_ModelContent::getInstance()->rowCount();
}

void CAppAhaStationList::selectPresetID(qulonglong appID)
{
    CAppAha_ModelLBS::getInstance()->reset();

    i_select_station = STATION_PRESET;

    //ITS_227494
    if(appID != 0)
    {
        select_station_id = appID;
    }
    CAppAha_ModelPreset::getInstance()->updateCurrentSelect(select_station_id);
}

void CAppAhaStationList::selectPresetIndex(int new_index)
{
    CAppAha_ModelLBS::getInstance()->reset();

    i_select_station = STATION_PRESET;
    select_station_id = CAppAha_ModelPreset::getInstance()->getID(new_index);
}

void CAppAhaStationList::selectLBSID(qulonglong appID)
{
    CAppAha_ModelPreset::getInstance()->reset();

    i_select_station = STATION_LBS;

    // modified ITS 230430
    if(appID != 0)
    {
        select_station_id = appID;
    }
    CAppAha_ModelLBS::getInstance()->updateCurrentSelect(select_station_id);
}

void CAppAhaStationList::selectLBSIndex(int new_index)
{
    CAppAha_ModelPreset::getInstance()->reset();

    i_select_station = STATION_LBS;
    select_station_id = CAppAha_ModelLBS::getInstance()->getID(new_index);
}

void CAppAhaStationList::setStationSection()
{
    LOG_HIGH <<"CAppAhaStationList::setStationSection..." <<LOG_ENDL;
    if(select_content_id != 0) return;

    AhaStationInfo_T* pAhaStationInfo = m_AhaInterface.m_AhaStateHandler.m_AhaActiveFrameTxRx->getAhaStationInfo();
    AhaClientInfo_T* pAhaClientInfo = m_AhaInterface.m_AhaStateHandler.m_AhaActiveFrameTxRx->getAhaClientInfo();
    if(pAhaStationInfo->nIsLBS)
    {
        i_select_station = STATION_LBS;
        select_station_id = pAhaClientInfo->nActiveStationID;
        CAppAhaQmlProperty::getInstance()->setFocusArea(7);
        CAppAhaQmlProperty::getInstance()->setFocusScreen(STATION_LBS);
    }
    else
    {
        i_select_station = STATION_PRESET;
        select_station_id = pAhaClientInfo->nActiveStationID;
        CAppAhaQmlProperty::getInstance()->setFocusArea(5);
        CAppAhaQmlProperty::getInstance()->setFocusScreen(STATION_PRESET);
    }
    LOG_HIGH <<"CAppAhaStationList::setStationSection...out..." <<LOG_ENDL;
}

void CAppAhaStationList::selectContentID(qulonglong appID)
{
    select_content_id = appID;
    CAppAha_ModelContent::getInstance()->updateCurrentSelect(appID);
}

void CAppAhaStationList::selectContentIndex(int new_index)
{
    select_content_id = CAppAha_ModelContent::getInstance()->getID(new_index);
}

void CAppAhaStationList::OnStationListLoaded(QVector<AhaStationBriefInfo_T> &qVectStationInfo)
{
    LOG_HIGH <<"CAppAhaStationList::OnStationListLoaded..." <<LOG_ENDL;
    int i_station_count = qVectStationInfo.count();

    TAN_PRINTF("i_station_count = %d \n", i_station_count);
    //LOG_HIGH << "i_station_count = " << i_station_count << LOG_ENDL;

    if(i_station_count <= 0 /*|| i_station_count > MAX_STATION_COUNT*/) //wsuk.kim 131120 REVERT. remain receiving station popup.
        return;

    CAppAha_ModelPreset::getInstance()->clear();
    CAppAha_ModelLBS::getInstance()->clear();

    bool b_isSelected = false;

    AhaClientInfo_T* pAhaClientInfo = m_AhaInterface.m_AhaStateHandler.m_AhaActiveFrameTxRx->getAhaClientInfo();
    CAppAha_ModelPreset* modelPreset = CAppAha_ModelPreset::getInstance();
    CAppAha_ModelLBS* modelLBS = CAppAha_ModelLBS::getInstance();

    for (int k = 0; k < i_station_count; k++)
    {
        b_isSelected = false;

        if(qVectStationInfo[k].nIsLBS)
        {
            if(qVectStationInfo[k].nStationID == pAhaClientInfo->nActiveStationID)
            {
                b_isSelected = true;
                i_select_station = STATION_LBS;
                select_station_id = qVectStationInfo[k].nStationID;
                CAppAhaQmlProperty::getInstance()->setFocusArea(7);
                CAppAhaQmlProperty::getInstance()->setFocusScreen(STATION_LBS);
            }

            modelLBS->addData(qVectStationInfo[k].nStationID, b_isSelected, qVectStationInfo[k].strStationFullName);
        }
        else
        {
            if(qVectStationInfo[k].nStationID == pAhaClientInfo->nActiveStationID)
            {
                b_isSelected = true;
                i_select_station = STATION_PRESET;
                select_station_id = qVectStationInfo[k].nStationID;
                //CAppAhaQmlProperty::getInstance()->setFocusArea(5); // removed 2014.03.04 wrong focus
                CAppAhaQmlProperty::getInstance()->setFocusScreen(STATION_PRESET);
            }

            modelPreset->addData(qVectStationInfo[k].nStationID, b_isSelected, qVectStationInfo[k].strStationFullName);
        }
    }

    pAhaClientInfo = m_AhaInterface.m_AhaStateHandler.m_AhaActiveFrameTxRx->getAhaClientInfo();
    CAppAha_ModelPreset::getInstance()->updateCurrentPlay(select_station_id, pAhaClientInfo->nPlaybackState);
    CAppAha_ModelLBS::getInstance()->updateCurrentPlay(select_station_id, pAhaClientInfo->nPlaybackState);

    emit modelPresetReady(modelPreset->getSelectRow(), modelPreset->rowCount());
    emit modelLBSReady(modelLBS->getSelectRow(), modelLBS->rowCount());
    emit modelStationNowListenning(getStationNowListenning());
    emit receivingStationHidePopup();    //wsuk.kim TUNE_TRACK
    LOG_HIGH <<"CAppAhaStationList::OnStationListLoaded...out" <<LOG_ENDL;
}

void CAppAhaStationList::OnStationLogoReceived(const uint8_t nIsLBS, const qulonglong appID, const QString &imagePath)
{
    LOG_HIGH <<"CAppAhaStationList::OnStationLogoReceived..." <<LOG_ENDL;
    if(nIsLBS)
        CAppAha_ModelLBS::getInstance()->updateImagePath(appID, imagePath);
    else
        CAppAha_ModelPreset::getInstance()->updateImagePath(appID, imagePath);
    LOG_HIGH <<"CAppAhaStationList::OnStationLogoReceived...out" <<LOG_ENDL;
}

void CAppAhaStationList::OnNotifyContentIDChanged(const qulonglong appID)
{
    LOG_HIGH <<"CAppAhaStationList::OnNotifyContentIDChanged..." <<LOG_ENDL;
    CAppAha_ModelContent* modelContent = CAppAha_ModelContent::getInstance();
    modelContent->updateCurrentSelect(appID);
    select_content_id = appID;

    emit modelContentScroll(modelContent->getSelectRow(), modelContent->rowCount());
    LOG_HIGH <<"CAppAhaStationList::OnNotifyContentIDChanged...out" <<LOG_ENDL;
}

void CAppAhaStationList::OnNotifyPlaybackStateChangedSlot(uint64_t appID, uint8_t play_status)
{
    LOG_HIGH <<"CAppAhaStationList::OnNotifyPlaybackStateChangedSlot..." <<LOG_ENDL;
    if((current_play_id == appID) && (current_play_status == play_status)) return;

    current_play_id = appID;
    current_play_status = play_status;

    if(CAppAhaQmlProperty::getInstance()->getFocusContentList())
    {
        CAppAha_ModelContent::getInstance()->updateCurrentPlay(appID, play_status);
    }
    else
    {
        if(i_select_station == STATION_PRESET)
            CAppAha_ModelPreset::getInstance()->updateCurrentPlay(play_status);
        else
            CAppAha_ModelLBS::getInstance()->updateCurrentPlay(play_status);
    }
    emit setBufferingStatus(play_status);   //wsuk.kim buffering
    LOG_HIGH <<"CAppAhaStationList::OnNotifyPlaybackStateChangedSlot...out" <<LOG_ENDL;
}

void CAppAhaStationList::OnContentListLoaded(QVector<AhaContentBriefInfo_T> &qVectContentInfo)
{
    LOG_HIGH <<"CAppAhaStationList::OnContentListLoaded..." <<LOG_ENDL;
    int i_content_count = qVectContentInfo.count();
    //hsryu_0604_content0_is_a_spec
    //if(i_content_count <= 0) return;

    // Contents Mode Clear
    CAppAha_ModelContent::getInstance()->clear();
    bool b_isSelected = false;

    AhaClientInfo_T* pAhaClientInfo = m_AhaInterface.m_AhaStateHandler.m_AhaActiveFrameTxRx->getAhaClientInfo();
    CAppAha_ModelContent* modelContent = CAppAha_ModelContent::getInstance();

    for (int k = 0; k < i_content_count; k++)
    {
        b_isSelected = false;

        if(qVectContentInfo[k].nContentID == pAhaClientInfo->nActiveContentID)
        {
            b_isSelected = true;
            select_content_id = qVectContentInfo[k].nContentID;
        }

        if(qVectContentInfo[k].strTitle.toAscii().length() == 0)    //wsuk.kim 130927 ContentList name empty
        {
            qVectContentInfo[k].strTitle = QApplication::translate("main", "STR_AHA_CONTENT_NAME_UNAVAILABLE");
        }
        //hsryu_0228 remove LF,space at front,end
        modelContent->addData(qVectContentInfo[k].nContentID, b_isSelected, qVectContentInfo[k].strTitle.trimmed());
    }

    pAhaClientInfo = m_AhaInterface.m_AhaStateHandler.m_AhaActiveFrameTxRx->getAhaClientInfo();
    CAppAha_ModelContent::getInstance()->updateCurrentPlay(pAhaClientInfo->nPlaybackState);

    emit modelContentReady(modelContent->getSelectRow(), modelContent->rowCount());

    LOG_HIGH <<"CAppAhaStationList::OnContentListLoaded...out" <<LOG_ENDL;
}
