/*
 ******************************************************************
 *        COPYRIGHT  2011, LG Electronics.
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :                                   *
 * PROGRAMMER : Vandana Ahuja                                     *
 * DATE       : 21/12/2011                                        *
 * VERSION    : 0.1                                               *
 *---------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module will be responsible for collecting data from
 *  Aha MW to be displayed in station list view.
 *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS                                           *
 *  21/12/2011  0.1 Vandana Ahuja   header file created
 *
 ******************************************************************
 */

#ifndef DHAVN_AppAhaStationList_H
#define DHAVN_AppAhaStationList_H

#include "ahainterface.h"
#include "DHAVN_AppAhaTrack.h"  //wsuk.kim ISV_77919 repeat_key
#include <QTimer>

#define STATION_PRESET  1
#define STATION_LBS     2
#define CONTENT_LIST    7

class CAppAhaStationList : public QObject
{
    Q_OBJECT
public:
    explicit CAppAhaStationList(CAhaInterface&  inAhaInterface);
    static CAppAhaStationList* getInstance();

    void initialize();
    Q_INVOKABLE void modelClear(); //hsryu_0322_model_clear
    void setStationSection();

signals:
    void modelPresetReady(int model_index, int nStationCount);
    void modelLBSReady(int model_index, int nStationCount);
    void modelContentReady(int model_index, int nContentCount);
    void modelContentScroll(int model_index, int nContentCount);
    void modelStationNowListenning(bool isPlay);
    void Content1stSkipBack(const bool bRepeatState); //wsuk.kim ISV_77919 repeat_key
    void receivingStationHidePopup();    //wsuk.kim TUNE_TRACK
    void setBufferingStatus(int playBackStatus);   //wsuk.kim buffering
public slots:
    void OnStationListLoaded(QVector<AhaStationBriefInfo_T>&qVectStationInfo);
    void OnContentListLoaded(QVector<AhaContentBriefInfo_T> &qVectContentInfo);
    void OnNotifyContentIDChanged(const qulonglong appID);

    void OnNotifyPlaybackStateChangedSlot(uint64_t appID, uint8_t play_status);

    void OnStationLogoReceived(const uint8_t nIsLBS, const qulonglong appID, const QString &imagePath);
    void OnTimeout_DelayEvent();

    void requestTimer(int interval, int type);
    void requestStationList();
    void requestContentList();

    void requestStationListInTrack(); //wsuk.kim TUNE_TRACK

    void playStation();
    void playContent();

    void selectPresetID(qulonglong appID);
    void selectPresetIndex(int index);
    void selectLBSID(qulonglong appID);
    void selectLBSIndex(int index);
    void selectContentID(qulonglong appID);
    void selectContentIndex(int index);

    QString getLastSelectedStation();
//wsuk.kim TUNE
    QString getStationNameTune(int tuneIndex, bool bTrackView);
    QString getStationArtTune(int tuneIndex);   //wsuk.kim 130902 ISV_90329 to display station art image that tune search.
    int getStationIdTune();
    int getStationIndexUsedfromStationIdTune();
    int getStationIdTuneUp();
    int getStationIdTuneDown();
    bool checkSameStationId(int tuneIndex);

    void setStationListFocusIndex(int stationFocusInex);
    int getStationListFocusIndex();

    void setTrackViewTuneIndex(int trackTuneIndex);
    int getTrackViewTuneIndex();

    void setContentListTuneIndex(int contentTuneIndex);
    int getContentListTuneIndex();  //wsuk.kim 131104 displayed at OSD control by tune.
//wsuk.kim TUNE

    int getPresetIndex();
    int getLBSIndex();
    int getContentIndex();

    int getPresetCount();
    int getLBSCount();
    int getContentCount();
private:
    bool getStationNowListenning();
    void emitStationList();

private:
    static CAppAhaStationList* _instance;

    CAhaInterface& m_AhaInterface;
    CAhaStateHandler* m_pSignals ;

    qulonglong select_station_id;
    qulonglong select_content_id;

    qulonglong current_play_id;
    unsigned char current_play_status;

    int i_select_station;
    int i_timer_work_type;

    bool b_once_request_station;

//wsuk.kim TUNE
    int i_tune_index;
    bool b_first_set_tune_index;
    bool b_check_same_station_id;
    int i_station_focus_index;
    int i_track_tune_index;
//wsuk.kim TUNE

public:
    USE_LOG_UTIL
};

#endif // DHAVN_AppAhaStationList_H
