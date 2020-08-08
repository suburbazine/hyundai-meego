#ifndef DHAVN_EVENTLOOP_H
#define DHAVN_EVENTLOOP_H

#include <QObject>
#include <QEventLoop>
#include "DHAVN_IPodController_IAPPacketWriter.h"


enum ExitCodeType_T{
    ExitCode_AllExit,
    ExitCode_default,
    ExitCode_ChannelBusy,
    ExitCode_IndexedTrackMetadata,
    ExitCode_GetDBTrackInfo,
    ExitCode_GetNumberCategorizedDatabaseRecords,
    ExitCode_SelectDBRecord,
    ExitCode_SelectSortDBRecord,
    ExitCode_ResetDBSelection,
    ExitCode_GetDBiTunesInfo,
    ExitCode_RequestiPodSerialNumber,
    ExitCode_GetPBTrackInfoTune,
    ExitCode_GetPBTrackInfoMetadataIndex,
    ExitCode_PlayCurrentSelection,
    ExitCode_PlayControl,
    ExitCode_SetRepeat,
    ExitCode_SetShuffle,
    ExitCode_SetPlayStatusChangeNotification,
    ExitCode_GetCurrentPlayingTrackIndex,
    ExitCode_GetPlayStatus,
    ExitCode_PreparedUIDList,
    ExitCode_PlayPreparedUIDList,
    ExitCode_GetNumOfPlayingTracks,
    ExitCode_SetCurrentPlayingTrack,
    ExitCode_SetiPodStateInfo,
    ExitCode_GetIndexedPlayingTrackInfo,
    ExitCode_GetIndexedPlayingTrackTitle,
    ExitCode_GetIndexedPlayingTrackArtistName,
    ExitCode_GetIndexedPlayingTrackAlbumName,
    ExitCode_GetIndexedPlayingTrackDuration,
    ExitCode_GetList,
    ExitCode_GetTrackArtworkData,
    ExitCode_GetArtworkTimesUID,
    ExitCode_GetArtworkDataUID,
    ExitCode_CloseiPodFeatureFile,
    ExitCode_GetiPodCaps,
    ExitCode_GetiPodFreeSpace,
    ExitCode_OpeniPodFeatureFile,
    ExitCode_WriteiPodFileData,
    ExitCode_GetNowPlayingApplicationBundleName,
    ExitCode_RetrieveCategorizedDatabaseRecords,
    ExitCode_GetArtworkFormats,
    ExitCode_GetTrackArtworkTimes,
    ExitCode_SetRemoteEventNotification,
    ExitCode_PauseIndexingStatus,
    ExitCode_ResumeIndexingStatus
};


class CIpodEventLoop: public QEventLoop
{
    Q_OBJECT
public:
    //Create the loop which waiting for nExitCode to exit.
    CIpodEventLoop(int nExitCode);
    int nGetExitCode();
private:
    int m_nExitCode;
public slots:
    void mainLoopQuitSlot(int nExitCode);
};

#endif // DHAVN_EVENTLOOP_H
