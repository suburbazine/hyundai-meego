/*
******************************************************************
*        COPYRIGHT (C) Teleca AB      *
*----------------------------------------------------------------*
* MODULE     :  DHAVN_TrackerAbstractor                             *
*
* PROGRAMMER :  Chetan Nanda                                   *
* DATE       :  12 September 2011                                    *
* VERSION    :  1.0                                              *
*
*----------------------------------------------------------------*
*                                                                *
* MODULE SUMMARY : This file defines APIs which interact with    *
*                  different contect providers (e.g. Media, Text Search, Contact)             *
*                  and used by application to retrieve information from respective    *
*                  content provider.           *
*                                                                *
*----------------------------------------------------------------*
*
* MODIFICATION RECORDS :                                         *
*
* VERSION   DATE                     PROGRAMMER              COMMENT
******************************************************************
* 1.0       12 September 2011  Chetan Nanda           Draft version
******************************************************************
*/

#include "DHAVN_TrackerAbstractor.h"
#include "DHAVN_MediaDataProvider.h"
#include "DHAVN_CoverArtDataProvider.h"
#include "DHAVN_ContactsDataProvider.h"
#include "DHAVN_IpodDataProvider.h"
#include <DHAVN_TrackerDataUpdater.h>
#include <DHAVN_GracenoteDataProvider.h> //added by Divya for Gracenote normalized data 2013.01.07
//#include "DHAVN_DirListProvider.h" // removed by kihyung 2013.2.17
#include "DHAVN_DirTreeProvider.h"
#include "DHAVN_AppFramework_Def.h"
#include<QObject>
#include <QDebug>
#include <QtSql>
#include <QQueue>
#include <QTimer>

//#include <TrackerDataList/DHAVN_DataLinkedList.h>
//#include <TrackerDataList/DHAVN_DataListNode.h>
#include "DHAVN_XSampaInterface.h"
#include <DHAVN_LogUtil.h>

static CLogUtil m_Logger;

/* Query Info Structure */
class SRequestStateInfo
{
public:    
    int previousQueryId;
    int currentQueryId;
    int currentVolumeType;
    int PreviousVolumeType;
    int reqState;
    int ReqData_set;
    quint16 requestCounter;
    QString Stringparam1;
    QString Stringparam2;
    quint16 uint16Param;
    int intParam1;
    int intParam2;
    QStringList StringListParam1;
    QStringList StringListParam2;
    QStringList StringListParam3;
    int currentOffset;
    int previousOffset;
    int currentLimit;
    int previousLimit;
    uint requestUID;
    bool cdInserted;

    SRequestStateInfo() :
            previousQueryId(eDefault_Id),
            currentQueryId(eDefault_Id),
            currentVolumeType(eDefaultVolume),
            PreviousVolumeType(eDefaultVolume),
            reqState(eDefaultRequest),
            ReqData_set(eRequest_default),
            requestCounter(0),
            Stringparam1(""),
            Stringparam2(""),
            uint16Param(0),
            intParam1(0),
            intParam2(0),
            currentOffset(0),
            previousOffset(0),
            currentLimit(0),
            previousLimit(0),
            requestUID(0),
            cdInserted(false){}

    SRequestStateInfo(const SRequestStateInfo& other);
    void initialize();
};

void SRequestStateInfo::initialize()
{
    intParam1 		= 0;
    intParam2 		= 0;
    Stringparam1.clear();
    Stringparam2.clear();
    uint16Param 	= 0;
    StringListParam1.clear();
    StringListParam2.clear();
    StringListParam3.clear();
    currentOffset 	= 0;
    previousOffset 	= 0;
    currentLimit 	= 0;
    previousLimit 	= 0;
    requestUID         = 0;
    cdInserted          = false;
}

SRequestStateInfo::SRequestStateInfo(const SRequestStateInfo& other)
{
    currentQueryId 		= other.currentQueryId;
    currentVolumeType 	= other.currentVolumeType;
    previousQueryId 	= other.previousQueryId;
    PreviousVolumeType 	= other.PreviousVolumeType;
	reqState            = other.reqState; // added by eugene.seo 2013.03.27 for static test
    requestCounter 		= other.requestCounter;
    ReqData_set 		= other.ReqData_set;

    intParam1 			= other.intParam1;
    intParam2 			= other.intParam2;
    Stringparam1 		= other.Stringparam1;
    Stringparam2 		= other.Stringparam2;
    StringListParam1 	= other.StringListParam1;
    StringListParam2 	= other.StringListParam2;
    StringListParam3 	= other.StringListParam3;
    uint16Param 		= other.uint16Param;

    currentOffset 		= other.currentOffset;
    previousOffset 		= other.previousOffset;
    currentLimit 		= other.currentLimit;
    previousLimit 		= other.previousLimit;
    requestUID          = other.requestUID;
    cdInserted          = other.cdInserted;
}

static QString dataListToString(const QVector<QStringList> & dataList)
{
    QStringList result;
    Q_FOREACH(QStringList list, dataList) {
        result << QString::fromLatin1("[%1]").arg(list.join(", "));
    }

    return result.join(", ");
}

static QString errorTypeToString(int type)
{
    switch (type) {
    case eNullStringParameter:
        return "eNullStringParameter";
    case eEmptyString:
        return "eEmptyString";
    case eErrorInQueryExecution:
        return "eErrorInQueryExecution";
    case eErrorInQuery:
        return "eErrorInQuery";
    case eQueryMappingError:
        return "eQueryMappingError";
    case eCoverArtImageNotFound:
        return "eCoverArtImageNotFound";
    case eErrorEmptyURL:
        return "eErrorEmptyURL";
    case eErrorNoThumbnailData:
        return "eErrorNoThumbnailData";
    case eErrorDirPathNotExists:
        return "eErrorDirPathNotExists";
    case eErrorDeviceId:
        return "eErrorDeviceId";
    default:
        return "INVALUE ETrackerAbstarctor_ErrorType";
    }
}

static QString requestStateToString(int state)
{
    switch (state) {
    case eDefaultRequest:
        return "eDefaultRequest";
    case eProcessRequest:
        return "eProcessRequest";
    case eWaitRequest:
        return "eWaitRequest";
    case eCancelRequest:
        return "eCancelRequest";
    case eProcesscomplete:
        return "eProcesscomplete";
    default:
        return "INVALID ETrackerAbstarctor_RequestStates";
    }
}

static QString volumeTypeToString(int type)
{
    switch (type) {
    case eUSB:
        return "eUSB";
    case eJukeBox:
        return "eJukeBox";
    case eIPod:
        return "eIPod";
    case eDisc:
        return "eDisc";
    case eUSB_FRONT:
        return "eUSB_FRONT";
    case eUSB_REAR:
        return "eUSB_REAR";
    case eIPOD_FRONT:
        return "eIPOD_FRONT";
    case eIPOD_REAR:
        return "eIPOD_REAR";
    case eDefaultVolume:
        return "eDefaultVolume";
    default:
        return "INVALID ETrackerAbstarctor_VolumeType";
    }
}


static QString orderTypeToString(int type)
{
    switch (type) {
    case eAlbum:
        return "eAlbum";
    case eArtist:
        return "eArtist";
    case eGenre:
        return "eGenre";
    case eTitle:
        return "eTitle";
    case eUrl:
        return "eUrl";
    default:
        return "INVALID ETrackerAbstarctor_OrderType";
    }
}

static QString queryIdToString(int queryId)
{
    switch (queryId) {
    case eDefault_Id:
        return "eDefault_Id";
    case eSongsByFolderQuery_Id:
        return "eSongsByFolderQuery_Id";
    case eAlbumByFolderQuery_Id:
        return "eAlbumByFolderQuery_Id";
    case eArtistByFolderQuery_Id:
        return "eArtistByFolderQuery_Id";
    case eGetAllSongsQuery_Id:
        return "eGetAllSongsQuery_Id";
    case eFolderListQuery_Id:
        return "eFolderListQuery_Id";
    case eMusicAlbumListQuery_Id:
        return "eMusicAlbumListQuery_Id";
    case eSongsByNextAlbumQuery_Id:
        return "eSongsByNextAlbumQuery_Id";
    case eSongsByPrevAlbumQuery_Id:
        return "eSongsByPrevAlbumQuery_Id";
    case eGetAllAlbumsQuery_Id:
        return "eGetAllAlbumsQuery_Id";
    case eGetAllMusicArtistsQuery_Id:
        return "eGetAllMusicArtistsQuery_Id";
    case eGetAllMusicComposersQuery_Id:
        return "eGetAllMusicComposersQuery_Id";
    case eGetAllMusicGenresQuery_Id:
        return "eGetAllMusicGenresQuery_Id";
    case eGetMusicMetaDataQuery_Id:
        return "eGetMusicMetaDataQuery_Id";
    case eGetAllSongsByAlbumQuery_Id:
        return "eGetAllSongsByAlbumQuery_Id";
    case eGetAllSongsByArtistQuery_Id:
        return "eGetAllSongsByArtistQuery_Id";
    case eGetAllSongsByGenreQuery_Id:
        return "eGetAllSongsByGenreQuery_Id";
    case eGetAllSongsByComposerQuery_Id:
        return "eGetAllSongsByComposerQuery_Id";
    case eGetImagesByFolderQuery_Id:
        return "eGetImagesByFolderQuery_Id";
    case eGetVideosByFolderQuery_Id:
        return "eGetVideosByFolderQuery_Id";
    case eGetAllMusicByKeywordQuery_Id:
        return "eGetAllMusicByKeywordQuery_Id";
    case eGetMultipleArtistSongsQuery_Id:
        return "eGetMultipleArtistSongsQuery_Id";
    case eGetMultipleAlbumSongsQuery_Id:
        return "eGetMultipleAlbumSongsQuery_Id";
    case eGetMusicAlbumCoverArt_Id:
        return "eGetMusicAlbumCoverArt_Id";
    case eGetMusicTrackCoverArt_Id:
        return "eGetMusicTrackCoverArt_Id";
    case eGetImageThumbNail_Id:
        return "eGetImageThumbNail_Id";
    case eGetAllImagesByKeywordQuery_Id:
        return "eGetAllImagesByKeywordQuery_Id";
    case eGetAllVideosByKeywrodQuery_Id:
        return "eGetAllVideosByKeywrodQuery_Id";
    case eGetVideoFileThumbNail_Id:
        return "eGetVideoFileThumbNail_Id";
    case eGetVideoFileListThumbNail_Id:
        return "eGetVideoFileListThumbNail_Id";
    case eSongsCountByFolderQuery_Id:
        return "eSongsCountByFolderQuery_Id";
    case eGetImagesCountByFolderQuery_Id:
        return "eGetImagesCountByFolderQuery_Id";
    case eGetVideosCountByFolderQuery_Id:
        return "eGetVideosCountByFolderQuery_Id";
    case eGetAllSongsCountQuery_Id:
        return "eGetAllSongsCountQuery_Id";
    case eGetAllSongsFromVolumeQuery_Id:
        return "eGetAllSongsFromVolumeQuery_Id";
    case eGetAllAlbumsFromVolumeQuery_Id:
        return "eGetAllAlbumsFromVolumeQuery_Id";
    case eGetAllMusicArtistsFromVolumeQuery_Id:
        return "eGetAllMusicArtistsFromVolumeQuery_Id";
    case eGetAllMusicComposersFromVolumeQuery_Id:
        return "eGetAllMusicComposersFromVolumeQuery_Id";
    case eGetAllMusicGenresFromVolumeQuery_Id:
        return "eGetAllMusicGenresFromVolumeQuery_Id";
    case eGetAllImagesFromVolumeQuery_Id:
        return "eGetAllImagesFromVolumeQuery_Id";
    case eGetAllVideosFromVolumeQuery_Id:
        return "eGetAllVideosFromVolumeQuery_Id";
    case eGetAllSongsIDQuery_id:
        return "eGetAllSongsIDQuery_id";
    case eGetAllSongsWithMetaDataFromVolumeQuery_Id:
        return "eGetAllSongsWithMetaDataFromVolumeQuery_Id";
    case eGetAllSongsByGenreArtistAlbumQuery_Id:
        return "eGetAllSongsByGenreArtistAlbumQuery_Id";
    case eGetAllSongsByGenreComposerAlbumQuery_Id:
        return "eGetAllSongsByGenreComposerAlbumQuery_Id";
    case eGetAllSongsByArtistAlbumQuery_Id:
        return "eGetAllSongsByArtistAlbumQuery_Id";
    case eGetAllAlbumByGenreArtistQuery_Id:
        return "eGetAllAlbumByGenreArtistQuery_Id";
    case eGetAllAlbumByArtistQuery_Id:
        return "eGetAllAlbumByArtistQuery_Id";
    case eGetAllArtistByGenreQuery_Id:
        return "eGetAllArtistByGenreQuery_Id";
    case eGetAllsongsByKeywordFromVolumeQuery_Id:
        return "eGetAllsongsByKeywordFromVolumeQuery_Id";
    case eGetAllArtistsByKeywordFromVolumeQuery_Id:
        return "eGetAllArtistsByKeywordFromVolumeQuery_Id";
    case eGetAllAlbumsByKeywordFromVolumeQuery_Id:
        return "eGetAllAlbumsByKeywordFromVolumeQuery_Id";
    case eGetAllGenresByKeywordFromVolumeQuery_Id:
        return "eGetAllGenresByKeywordFromVolumeQuery_Id";
    case eGetAllSongsByAlbumFromVolumeQuery_Id:
        return "eGetAllSongsByAlbumFromVolumeQuery_Id";
    case eGetAllSongsbyPlaylistQuery_Id:
        return "eGetAllSongsbyPlaylistQuery_Id";
    case eGetlistofPlaylistsQuery_Id:
        return "eGetlistofPlaylistsQuery_Id";
    case eGetMostPlayedMusicSongsQuery_Id:
        return "eGetMostPlayedMusicSongsQuery_Id";
    case eGetMostRecentlyPlayedSongsQuery_Id:
        return "eGetMostRecentlyPlayedSongsQuery_Id";
    case eGetMusicPlaylistByKeywordQuery_Id:
        return "eGetMusicPlaylistByKeywordQuery_Id";
    case eGetAllSongsByComposerAlbumQuery_Id:
        return "eGetAllSongsByComposerAlbumQuery_Id";
    case eGetAllAlbumByGenreComposerQuery_Id:
        return "eGetAllAlbumByGenreComposerQuery_Id";
    case eGetAllAlbumByComposerQuery_Id:
        return "eGetAllAlbumByComposerQuery_Id";
    case eGetAllComposerByGenreQuery_Id:
        return "eGetAllComposerByGenreQuery_Id";
    case eGetMusicDetailFromIpodOnIndexBasis_Id:
        return "eGetMusicDetailFromIpodOnIndexBasis_Id";
    case eInsertIPodMusicTracksToDBaseQuery_Id:
        return "eInsertIPodMusicTracksToDBaseQuery_Id";
    case eInsertIPodPodcastToDBaseQuery_Id:
        return "eInsertIPodPodcastToDBaseQuery_Id";
    case eInsertIPodAudioBookToDBaseQuery_Id:
        return "eInsertIPodAudioBookToDBaseQuery_Id";
    case eInsertIPodMusicPlaylistToDBaseQuery_Id:
        return "eInsertIPodMusicPlaylistToDBaseQuery_Id";
    case eRemoveIPodMusicTrackDataQuery_Id:
        return "eRemoveIPodMusicTrackDataQuery_Id";
    case eRemoveIPodPodcastDataQuery_Id:
        return "eRemoveIPodPodcastDataQuery_Id";
    case eRemoveIPodAudioBookDataQuery_Id:
        return "eRemoveIPodAudioBookDataQuery_Id";
    case eRemoveIPodPlaylistDataQuery_Id:
        return "eRemoveIPodPlaylistDataQuery_Id";
    case eRemoveAllIPodInfoQuery_Id:
        return "eRemoveAllIPodInfoQuery_Id";
    case eGetAllPodcastsByTitleQuery_Id:
        return "eGetAllPodcastsByTitleQuery_Id";
    case eGetAllAudiobooksByTitleQuery_Id:
        return "eGetAllAudiobooksByTitleQuery_Id";
    case eGetAllPodcastsEpisodeNamesQuery_Id:
        return "eGetAllPodcastsEpisodeNamesQuery_Id";
    case eGetAllAudiobookChapterNamesQuery_Id:
        return "eGetAllAudiobookChapterNamesQuery_Id";
    case eGetAllVideosCountFromVolumeQuery_Id:
        return "eGetAllVideosCountFromVolumeQuery_Id";
    case eGetAllImagesCountFromVolumeQuery_Id:
        return "eGetAllImagesCountFromVolumeQuery_Id";
    case eGetMusicFromCDQuery_Id:
        return "eGetMusicFromCDQuery_Id";
    case eInsertAlbumCoverArtDataQuery_Id:
        return "eInsertAlbumCoverArtDataQuery_Id";
    case eGetMusicMetaDataFromCDQuery_Id:
        return "eGetMusicMetaDataFromCDQuery_Id";
    case eIsCoverArtExists_Id:
        return "eIsCoverArtExists_Id";
    case eGetImagesBySubFolderQuery_Id:
        return "eGetImagesBySubFolderQuery_Id";
    case eGetVideosBySubFolderQuery_Id:
        return "eGetVideosBySubFolderQuery_Id";
    case eGetAllAlbumsWithCoverArtFromVolumeQuery_Id:
        return "eGetAllAlbumsWithCoverArtFromVolumeQuery_Id";
    case eInsertGracenoteMetadataQuery_Id:
        return "eInsertGracenoteMetadataQuery_Id";
    case eDeleteGracenoteCDMetadataQuery_Id:
        return "eDeleteGracenoteCDMetadataQuery_Id";
    case eGetVideoFileListDir_Id:
        return "eGetVideoFileListDir_Id";
    case eGetMusicFileListDir_Id:
        return "eGetMusicFileListDir_Id";
    case eGetPhotoFileListDir_Id:
        return "eGetPhotoFileListDir_Id";
    case eGetVideoLimitFileListDir_Id:
        return "eGetVideoLimitFileListDir_Id";
    case eGetMusicLimitFileListDir_Id:
        return "eGetMusicLimitFileListDir_Id";
    case eGetPhotoLimitFileListDir_Id:
        return "eGetPhotoLimitFileListDir_Id";
    case eGetAllSongsWithoutAlbumFromVolumeQuery_Id:
        return "eGetAllSongsWithoutAlbumFromVolumeQuery_Id";
    case eIsThereAnySongWithoutAlbumFromVolumeQuery_Id:
        return "eIsThereAnySongWithoutAlbumFromVolumeQuery_Id";
    case eGetAllMusicByKeywordFromVolumeQuery_Id:
        return "eGetAllMusicByKeywordFromVolumeQuery_Id";
    case eInsertXSampeData_Id:
        return "eInsertXSampeData_Id";
    case eRetrieveXSampaData_Id:
        return "eRetrieveXSampaData_Id";
    case eCheckExistenceOfXSampaData_Id:
        return "eCheckExistenceOfXSampaData_Id";
    case eGetMusicMetaDataInfoFromUrlListQuery_Id:
        return "eGetMusicMetaDataInfoFromUrlListQuery_Id";
    case eRetrieveAllXSampaData_Id:
        return "eRetrieveAllXSampaData_Id";
    case eRetrieveXSampaDataForType_Id:
        return "eRetrieveXSampaDataForType_Id";
    case eRemoveAllMediaDataFromVolumeQuery_Id:
        return "eRemoveAllMediaDataFromVolumeQuery_Id";
    case eRetrieveCoverArtOnDemand_Id:
        return "eRetrieveCoverArtOnDemand_Id";
    case eGetAllAlbumsWithUrlFromVolumeQuery_Id:
        return "eGetAllAlbumsWithUrlFromVolumeQuery_Id";
    case eGetAllSongsByArtistFromVolumeQuery_Id:
        return "eGetAllSongsByArtistFromVolumeQuery_Id";
	case eGetAllSongsByGenreFromVolumeQuery_Id:
        return "eGetAllSongsByGenreFromVolumeQuery_Id";
    case eGetAllSongsCountFromVolumeQuery_Id:
        return "eGetAllSongsCountFromVolumeQuery_Id";
    case eGetAllSongsWithTitleCountFromVolumeQuery_Id:
        return "eGetAllSongsWithTitleCountFromVolumeQuery_Id";
    case eGetAllAlbumsCountFromVolumeQuery_Id:
        return "eGetAllAlbumsCountFromVolumeQuery_Id";
    case eGetAllMusicArtistsCountFromVolumeQuery_Id:
        return "eGetAllMusicArtistsCountFromVolumeQuery_Id";
    case eGetAllMusicComposersCountFromVolumeQuery_Id:
        return "eGetAllMusicComposersCountFromVolumeQuery_Id";
    case eGetAllMusicGenresCountFromVolumeQuery_Id:
        return "eGetAllMusicGenresCountFromVolumeQuery_Id";
    case eGetAllAlbumsWithCoverArtCountFromVolumeQuery_Id:
        return "eGetAllAlbumsWithCoverArtCountFromVolumeQuery_Id";
    case eGetAllSongsWithLimitFromVolumeQuery_Id:
        return "eGetAllSongsWithLimitFromVolumeQuery_Id";
    case eGetAllAlbumsWithLimitFromVolumeQuery_Id:
        return "eGetAllAlbumsWithLimitFromVolumeQuery_Id";
    case eGetAllMusicArtistsWithLimitFromVolumeQuery_Id:
        return "eGetAllMusicArtistsWithLimitFromVolumeQuery_Id";
    case eGetAllMusicComposersWithLimitFromVolumeQuery_Id:
        return "eGetAllMusicComposersWithLimitFromVolumeQuery_Id";
    case eGetAllMusicGenresWithLimitFromVolumeQuery_Id:
        return "eGetAllMusicGenresWithLimitFromVolumeQuery_Id";
    case eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id:
        return "eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id";
    case eGetAllSongsWithTitleIndexFromVolumeQuery_Id:
        return "eGetAllSongsWithTitleIndexFromVolumeQuery_Id";
    case eGetAllMusicArtistsIndexFromVolumeQuery_Id:
        return "eGetAllMusicArtistsIndexFromVolumeQuery_Id";
    case eGetAllMusicGenresIndexFromVolumeQuery_Id:
        return "eGetAllMusicGenresIndexFromVolumeQuery_Id";
    case eGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Id:
        return "eGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Id";
    case eGetMultipleAlbumSongsByArtistFromVolumeQuery_Id:
        return "eGetMultipleAlbumSongsByArtistFromVolumeQuery_Id";
    case eGetMultipleGenreSongsFromVolumeQuery_Id:
        return "eGetMultipleGenreSongsFromVolumeQuery_Id";
    case eGetMultipleArtistSongsByGenreFromVolumeQuery_Id:
        return "eGetMultipleArtistSongsByGenreFromVolumeQuery_Id";
    case eGetMultipleAlbumSongsByGenreArtistFromVolumeQuery_Id:
        return "eGetMultipleAlbumSongsByGenreArtistFromVolumeQuery_Id";
    case eGetMultipleFolderSongsQuery_Id:
        return "eGetMultipleFolderSongsQuery_Id";
    case eGetMultipleAlbumSongsFromVolumeQuery_Id:
        return "eGetMultipleAlbumSongsFromVolumeQuery_Id";
    case eGetMultipleArtistSongsFromVolumeQuery_Id:
        return "eGetMultipleArtistSongsFromVolumeQuery_Id";
    case eGetAlliTunesUByTitleQuery_Id:
        return "eGetAlliTunesUByTitleQuery_Id";
    case eGetAlliTunesUEpisodeNamesQuery_Id:
        return "eGetAlliTunesUEpisodeNamesQuery_Id";
    case eGetFolderTabList_Id:
        return "eGetFolderTabList_Id";
    case eGetXSampaId3NameQuery_Id:
        return "eGetXSampaId3NameQuery_Id";
    case eGetSongIndexFromVolumeQuery_Id:
        return "eGetSongIndexFromVolumeQuery_Id";
    case eGetAlbumIndexFromVolumeQuery_Id:
        return "eGetAlbumIndexFromVolumeQuery_Id";
    case eGetArtistIndexFromVolumeQuery_Id:
        return "eGetArtistIndexFromVolumeQuery_Id";
    case eGetAllMetaDataWithLimitFromVolumeQuery_Id:
        return "eGetAllMetaDataWithLimitFromVolumeQuery_Id";
    case eGetAllMusicByConnectedVolumesQuery_Id:
        return "eGetAllMusicByConnectedVolumesQuery_Id";
    case eUpdateCIDInfoInXSampaDB:
        return "eUpdateCIDInfoInXSampaDB";
    case eTrackerExtractMetadataQuery_Id:
        return "eTrackerExtractMetadataQuery_Id";
    case eGetPhotoMetaDataInfoFromUrlQuery_Id:
        return "eGetPhotoMetaDataInfoFromUrlQuery_Id";
    case eGetAllPhoneContactsQuery_Id:
        return "eGetAllPhoneContactsQuery_Id";
    case eGetAllRecentCallQuery_Id:
        return "eGetAllRecentCallQuery_Id";
    case eGetAllReceivedCallQuery_Id:
        return "eGetAllReceivedCallQuery_Id";
    case eGetAllMissedCallQuery_Id:
        return "eGetAllMissedCallQuery_Id";
    case eGetAllCallHistoryQuery_Id:
        return "eGetAllCallHistoryQuery_Id";
    case eGetAllFavoriteContactsQuery_Id:
        return "eGetAllFavoriteContactsQuery_Id";
    case eGetContactDetailsByNameQuery_Id:
        return "eGetContactDetailsByNameQuery_Id";
    case eGetAllContactsByKeywordForBDAdressQuery_Id:
        return "eGetAllContactsByKeywordForBDAdressQuery_Id";
    case eGetContactByNumberForForeignQuery_Id:
        return "eGetContactByNumberForForeignQuery_Id";
    case eGetContactByNumberForDomesticQuery_Id:
        return "eGetContactByNumberForDomesticQuery_Id";
    case eGetFavoriteContactByNameAndNumberQuery_Id:
        return "eGetFavoriteContactByNameAndNumberQuery_Id";
    case eGetContactByCallHistoryInfoQuery_Id:
        return "eGetContactByCallHistoryInfoQuery_Id";
    case eGetContactByNumberForForeignCallHistoryNameQuery_Id:
        return "eGetContactByNumberForForeignCallHistoryNameQuery_Id";
    case eGetContactByNumberForDomesticCallHistoryNameQuery_Id:
        return "eGetContactByNumberForDomesticCallHistoryNameQuery_Id";
    case eGetAllPhoneContactCountQuery_Id:
        return "eGetAllPhoneContactCountQuery_Id";
    case eGetAllRecentCallCountQuery_Id:
        return "eGetAllRecentCallCountQuery_Id";
    case eGetAllReceivedCallCountQuery_Id:
        return "eGetAllReceivedCallCountQuery_Id";
    case eGetAllMissedCallCountQuery_Id:
        return "eGetAllMissedCallCountQuery_Id";
    case eGetAllCallHistoryCountQuery_Id:
        return "eGetAllCallHistoryCountQuery_Id";
    case eGetAllFavoriteContactCountQuery_Id:
        return "eGetAllFavoriteContactCountQuery_Id";
    case eInsertContactsToDBaseQuery_Id:
        return "eInsertContactsToDBaseQuery_Id";
    case eRemovePhoneBookDataBySelectionQuery_Id:
        return "eRemovePhoneBookDataBySelectionQuery_Id";
    case eRemoveContactDataQuery_Id:
        return "eRemoveContactDataQuery_Id";
    case eRemoveContactDataByIndexQuery_Id:
        return "eRemoveContactDataByIndexQuery_Id";
	case eGetAllPhoneContactsForForeignQuery_Id:
		return "eGetAllPhoneContactsForForeignQuery_Id";
	case eGetAllPhoneContactsForDomesticQuery_Id:
		return "eGetAllPhoneContactsForDomesticQuery_Id";
	case eGetAllFavoriteContactsForForeignQuery_Id:
		return "eGetAllFavoriteContactsForForeignQuery_Id";
    case eGetAllFavoriteContactsForDomesticQuery_Id:
        return "eGetAllFavoriteContactsForDomesticQuery_Id";
	case eGetContactByCallHistoryInfoQuery_Id_KR_NA:
		return "eGetContactByCallHistoryInfoQuery_Id_KR_NA";
    case eGetContactByNumberForCallHistoryNameQuery_Id_KR:
         return "eGetContactByNumberForCallHistoryNameQuery_Id_KR";
    case eGetContactByNumberForCallHistoryNameQuery_Id_CN:
         return "eGetContactByNumberForCallHistoryNameQuery_Id_CN";
    case eGetContactByNumberForCallHistoryNameQuery_Id_NA:
         return "eGetContactByNumberForCallHistoryNameQuery_Id_NA";
    case eGetContactByNumberQuery_Id_KR:
                return "eGetContactByNumberQuery_Id_KR";
    case eGetContactByNumberQuery_Id_NA:
            return "eGetContactByNumberQuery_Id_NA";
    case eGetContactByNumberQuery_Id_CN:
            return "eGetContactByNumberQuery_Id_CN";
	case eGetPhoneBookHashValueByBDAddressQuery_Id:
		return "eGetPhoneBookHashValueByBDAddressQuery_Id";
    case eGetSongIndexWithTitleCountFromVolumeQuery_Id:    // { added by sungha.choi 2013.07.23
        return "eGetSongIndexWithTitleCountFromVolumeQuery_Id";
    case eGetAlbumIndexWithAlbumCountFromVolumeQuery_Id:
        return "eGetAlbumIndexWithAlbumCountFromVolumeQuery_Id";
    case eGetArtistIndexWithArtistCountFromVolumeQuery_Id:
        return "eGetArtistIndexWithArtistCountFromVolumeQuery_Id";
    case eGetAllSongsWithLimitCacheFromVolumeQuery_Id:
        return "eGetAllSongsWithLimitCacheFromVolumeQuery_Id";
    case eGetAllAlbumsWithCoverArtWithLimitCacheFromVolumeQuery_Id:
        return "eGetAllAlbumsWithCoverArtWithLimitCacheFromVolumeQuery_Id";
    case eGetAllMusicArtistsWithLimitCacheFromVolumeQuery_Id:
        return "eGetAllMusicArtistsWithLimitCacheFromVolumeQuery_Id";   // } added by sungha.choi 2013.07.23

    /////SUPPORT_MIDDLE_NAME

    case eMidGetAllFavoriteContactsForForeignQuery_Id:
        return "eMidGetAllFavoriteContactsForForeignQuery_Id";
    case eMidGetAllPhoneContactsForForeignQuery_Id:
        return "eMidGetAllPhoneContactsForForeignQuery_Id";
    case eMidGetAllFavoriteContactsForDomesticQuery_Id:
        return "eMidGetAllFavoriteContactsForDomesticQuery_Id";
    case eMidGetAllPhoneContactsForDomesticQuery_Id:
        return "eMidGetAllPhoneContactsForDomesticQuery_Id";

    case eMidGetContactByNumberForDomesticQuery_Id:
        return "eMidGetContactByNumberForDomesticQuery_Id";
    case eMidGetContactByNumberForDomesticCallHistoryNameQuery_Id:
        return "eMidGetContactByNumberForDomesticCallHistoryNameQuery_Id";
    case eMidGetContactByCallHistoryInfoQuery_Id:
        return "eMidGetContactByCallHistoryInfoQuery_Id";
    case eMidGetFavoriteContactByNameAndNumberQuery_Id:
        return "eMidGetFavoriteContactByNameAndNumberQuery_Id";

    case ePinyinGetAllSelectedPhoneContactsForDomesticQuery_id:
        return "ePinyinGetAllSelectedPhoneContactsForDomesticQuery_id";
    /* FEATURE_ORDER_BY_PINYIN */

    case eGetMusicNormalMetaDataQuery_Id:
        return "eGetMusicNormalMetaDataQuery_Id";

    default:
        return "INVALID ETrackerAbstarctor_Query_Ids";
    }
}

static QString requestDataToString(const SRequestStateInfo& info)
{
    QString result;
    switch (info.ReqData_set) {
    case eRequest_default:
        result = QString::fromLatin1("()");
        break;
    case eRequest_NoVol_NoParam:
        result = QString::fromLatin1("()");
        break;
    case eRequest_NoVol_StringParam:
        result = QString::fromLatin1("(\"%1\")")
                .arg(info.Stringparam1);
        break;
    case eRequest_NoVol_List_StringParam:
        result = QString::fromLatin1("([%1], \"%2\")")
                .arg(info.StringListParam1.join(", "))
                .arg(info.Stringparam1);
        break;
    case eRequest_NoVol_singleListParam:
        result = QString::fromLatin1("([%1])")
                .arg(info.StringListParam1.join(", "));
        break;
    case eRequest_NoVol_TwoListParams:
        result = QString::fromLatin1("([%1], [%2])")
                .arg(info.StringListParam1.join(", "))
                .arg(info.StringListParam2.join(", "));
        break;
    case eRequest_NoVol_ThreeListParams:
        result = QString::fromLatin1("([%1], [%2], [%3])")
                .arg(info.StringListParam1.join(", "))
                .arg(info.StringListParam2.join(", "))
                .arg(info.StringListParam3.join(", "));
        break;
    case eRequest_NoVol_TwoStringParams:
        result = QString::fromLatin1("(\"%1\", \"%2\")")
                .arg(info.Stringparam1)
                .arg(info.Stringparam2);
        break;
    case eRequest_NoVol_TwoIntParams:
        result = QString::fromLatin1("(%1, %2)")
                .arg(info.intParam1)
                .arg(info.intParam2);
        break;
    case eRequest_Vol_NoParam:
        result = QString::fromLatin1("(%1)")
                .arg(volumeTypeToString(info.currentVolumeType));
        break;
    case eRequest_Vol_SingleListParam:
        result = QString::fromLatin1("([%1], %2)")
                .arg(info.StringListParam1.join(", "))
                .arg(volumeTypeToString(info.currentVolumeType));
        break;
    case eRequest_Vol_StringParam:
        result = QString::fromLatin1("(\"%1\", %2)")
                .arg(info.Stringparam1)
                .arg(volumeTypeToString(info.currentVolumeType));
        break;
    case eRequest_Vol_OrdParam:
        result = QString::fromLatin1("(%1, %2)")
                .arg(orderTypeToString(info.intParam1))
                .arg(volumeTypeToString(info.currentVolumeType));
        break;
    case eRequest_Vol_TwoListParams:
        result = QString::fromLatin1("([%1], [%2], %3)")
                .arg(info.StringListParam1.join(", "))
                .arg(info.StringListParam2.join(", "))
                .arg(volumeTypeToString(info.currentVolumeType));
        break;
    case eRequest_NoVol_String_List_boolParam:
        result = QString::fromLatin1("(\"%1\", [%2], %3)")
                .arg(info.Stringparam1)
                .arg(info.StringListParam1.join(", "))
                .arg(info.cdInserted);
        break;
    default:
        return "(INVALID ETrackerAbstractor_RequestData)";
        break;
    }
    return result;
}


int CTrackerAbstractor::m_CountryVariant = 0;//added by junam 2013.05.23 for sorting
int CTrackerAbstractor::m_nSortingOrder = 0;//added by junam 2013.08.21 for iPod sort


CTrackerAbstractor::CTrackerAbstractor():
m_MediaDataProvider(NULL), // { added by eugene.seo 2013.03.27 for static test
m_CoverArtDataprovider(NULL),
m_ContactsDataProvider(NULL),
m_DataUpdater(NULL),
m_IpodDataProvider(NULL),
m_Album_CoverArtRequest(false),
m_Album_CoverArtRequestWithLimit(false),
m_Album_CoverArtRequestOffset(QUERY_OFFSET),
m_DirTreeProvider(0),
m_XSampaInterface(NULL),
m_VolumeType(eDefaultVolume),
m_Query_Offset(QUERY_OFFSET),
m_Query_Limit(QUERY_LIMIT),
m_ReqStateInfo(new SRequestStateInfo()), // added by kihyung 2013.2.17
m_CurrentReqStateInfo(NULL),
m_QueueRequestTimer(NULL), 
m_GracenoteNormalizedData(false),
m_GracenoteDataProvider(NULL), // } added by eugene.seo 2013.03.27 for static test
m_jukeBoxMinerFinished(false),  // { added by sungha.choi 2013.07.23
m_usb1MinerFinished(false),
m_usb2MinerFinished(false),  // } added by sungha.choi 2013.07.23
m_isPBData(false),   // added by sungha.choi 2013.08.25 (PB do not use queue)
m_DirTreeQueryId(0)   // added by sungha.choi 2013.09.09 for ISV 90608
{
    LOG_INIT_V2("Media_TrackerAbstractor")

    // QSparqlConnection object creation using tracker driver
    qDebug() << "SPARQL connection";
    m_pSparql_Connection = new QSparqlConnection("QTRACKER");

    if (m_pSparql_Connection->isValid()) {
        qDebug()<< "[TA] QSparql Connection Successful";
    }
    else {
        qDebug()<< "[TA] QSparql Connection Failed";
        return;
    }

    // media data provider object creation and conneting signal/slot
    m_MediaDataProvider = new CMediaDataProvider(m_pSparql_Connection);
    if(m_MediaDataProvider)
    {
        connect(m_MediaDataProvider, SIGNAL(DataFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)),
                this, SLOT(DataReceived(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)));
        connect(m_MediaDataProvider, SIGNAL(DataLimitFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, int)),
							 this, SLOT(DataLimitReceived(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, int)));

        connect(m_MediaDataProvider, SIGNAL(Error(int)), this, SLOT(ErrorReceived(int)));
    }

    m_CoverArtDataprovider = new CCoverArtDataprovider();
    if(m_CoverArtDataprovider)
    {
        connect(m_CoverArtDataprovider, SIGNAL(DataFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)),
                this, SLOT(DataReceived(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)));
        connect(m_CoverArtDataprovider, SIGNAL(Error(int)), this, SLOT(ErrorReceived(int)));

        connect(m_CoverArtDataprovider, SIGNAL(getCoverArt(QStringList,QStringList)),
                this, SLOT(getCoverArtSlot(QStringList,QStringList)));
    }

    // contact data provider object creation and conneting signal/slot
    m_ContactsDataProvider = new CContactsDataProvider();
    if(m_ContactsDataProvider)
    {
        connect(m_ContactsDataProvider, SIGNAL(DataFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)),
                this, SLOT(DataReceived(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)));

        connect(m_ContactsDataProvider, SIGNAL(Error(int)), this, SLOT(ErrorReceived(int)));
    }

    m_DataUpdater = new CTrackerDataUpdater(m_pSparql_Connection);

    if(m_DataUpdater)
    {
        //m_DataUpdater->CreateSqlConnectionForContacts();
    }
	
	//IpoddataProvider object creation and connect signal
    m_IpodDataProvider=new CIpodDataProvider(m_pSparql_Connection);
	if(m_IpodDataProvider)
	{
        connect(m_IpodDataProvider, SIGNAL(DataFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)),
                this, SLOT(DataReceived(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)));
        //IpoddataProvider object creation and connect signal
        connect(m_IpodDataProvider, SIGNAL(Error(int)), this, SLOT(ErrorReceived(int)));
    }

    // { removed by kihyung 2013.2.17
    /*
    m_DirListProvider = new CDirListProvider();
    if(m_DirListProvider)
    {
        connect(m_DirListProvider, SIGNAL(DataFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)),this, SLOT(DataReceived(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)));
        connect(m_DirListProvider, SIGNAL(Error(int)), this, SLOT(ErrorReceived(int)));
    }
    */
    // } removed by kihyung 2013.2.17

    //    m_MediaFolderList = NULL;
    //initializing music album list object
    //    m_MusicAlbumList = NULL;

    m_XSampaInterface = new CXSampaInterface;

    if(m_XSampaInterface)
    {
        //m_XSampaInterface->Init();        

        connect(m_XSampaInterface, SIGNAL(DataFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)),
                this, SLOT(DataReceived(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)));
    }

    m_QueueRequestTimer = new QTimer(this);
    if(m_QueueRequestTimer)
    {
        connect(m_QueueRequestTimer, SIGNAL(timeout()), this, SLOT(RequestTimeOut()));
    }

    m_Album_CoverArtRequest = false;
	m_Album_CoverArtRequestWithLimit = false;
	m_Album_CoverArtRequestOffset = QUERY_OFFSET;

	m_Query_Offset = QUERY_OFFSET;
	m_Query_Limit  = QUERY_LIMIT;

    m_CurrentReqStateInfo = NULL;
    
    // m_ReqStateInfo = new SRequestStateInfo(); // removed by kihyung 2013.2.17
    if(m_ReqStateInfo == 0) {
        qDebug()<< "[TA] ERROR m_ReqStateInfo is 0";
    }
    
	//{added by Divya for Gracenote normalized data 2013.01.07
	m_GracenoteNormalizedData = false;

    m_GracenoteDataProvider = new CGracenoteDataProvider();
    if(m_GracenoteDataProvider)
    {
        connect(m_GracenoteDataProvider, SIGNAL(DataFeched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)),
                this, SLOT(DataReceived(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)));

        connect(m_GracenoteDataProvider, SIGNAL(Error(int)), this, SLOT(ErrorReceived(int)));
    }
	//}added by Divya for Gracenote normalized data 2013.01.07
	m_Removed_DeviceId = "No_Removed_DeviceId"; // added by eugene.seo 2013.04.29

    //Tan 2014.04.17 must pass correct serial number when requesting coverart
    m_CurrentActiveIPodVolume = eDefaultVolume;
    m_deviceid_ipod_front.clear();
    m_deviceid_ipod_rear.clear();
}

CTrackerAbstractor::~CTrackerAbstractor()
{
    if(m_pSparql_Connection != NULL)
    {
        delete m_pSparql_Connection;
        m_pSparql_Connection = NULL;
    }
    if(m_MediaDataProvider != NULL)
    {
    	delete m_MediaDataProvider;
        m_MediaDataProvider = NULL;
    }    
    if(m_ContactsDataProvider != NULL)
    {
        delete m_ContactsDataProvider;
        m_ContactsDataProvider = NULL;
    }
    if(m_CoverArtDataprovider != NULL)
    {
        delete m_CoverArtDataprovider;
        m_CoverArtDataprovider = NULL;
    }
    if(m_DataUpdater !=NULL)
    {
        delete m_DataUpdater;
        m_DataUpdater = NULL;
    }

    if(m_IpodDataProvider !=NULL)
        delete m_IpodDataProvider;

    // { removed by kihyung 2013.2.17
    /*
    if(m_DirListProvider != NULL)
    {
        delete m_DirListProvider;
        m_DirListProvider = NULL;
    }
    */
    // } removed by kihyung 2013.2.17    

    // { added by kihyung 2013.2.17
    if(m_DirTreeProvider != NULL)
    {
        delete m_DirTreeProvider;
        m_DirTreeProvider = NULL;
    }
    // } added by kihyung 2013.2.17

    if(m_XSampaInterface)
    {
        delete m_XSampaInterface;
        m_XSampaInterface = NULL;
    }

    if(m_ReqStateInfo)
    {
        delete m_ReqStateInfo;
        m_ReqStateInfo = NULL;
    }

    if(firstIpodDB.isOpen())
        firstIpodDB.close();

    if(secondIpodDB.isOpen())
        secondIpodDB.close();
		
	if(m_QueueRequestTimer)
	{
		delete m_QueueRequestTimer;
		m_QueueRequestTimer = NULL;
	}		

    if(m_CurrentReqStateInfo)
    {
        delete m_CurrentReqStateInfo;
        m_CurrentReqStateInfo = NULL;
    }
//{added by Divya for Gracenote normalized data 2013.01.07
    if(m_GracenoteDataProvider)
    {
       delete m_GracenoteDataProvider;
       m_GracenoteDataProvider = NULL;
    }
//}added by Divya for Gracenote normalized data 2013.01.07
    /* if(m_MediaFolderList != NULL)
        delete m_MediaFolderList;*/
    /* if(m_MusicAlbumList != NULL)
        delete m_MusicAlbumList;*/
}

/******************************************************************************

// Function Name                                            Parameters

// CTrackerAbstractor::RequestData                 bool (OUT), QStringList (IN) - data to search
                                                                    ETrackerAbstarctor_VolumeType (IN) - volume to search,
                                                                     ETrackerAbstarctor_Query_Ids (IN)- respective query Id

//

// Explanation

// This method calls songs/abums/artist by genre/artist/ablum APIs of Media Provider using the query id

// It connects signal/slot for the query completion

//

// History

// 02-Nov-2011      Chetan Nanda                created for handingsongs/abums/artist by genre/artist/ablum
                                                                  related Queries of Media provider
*****************************************************************************/

bool CTrackerAbstractor::RequestData(QStringList searchList, ETrackerAbstarctor_VolumeType volumeType,
                                     ETrackerAbstarctor_Query_Ids queryId, uint reqUID)
{
    bool ret = false;

    m_ReqStateInfo->ReqData_set = eRequest_Vol_SingleListParam;
    m_ReqStateInfo->StringListParam1 = searchList;
    int reqState = RequestHandler(queryId,volumeType);
    if(reqState ==eDefaultRequest || reqState == eProcessRequest)
    {
        if(m_CurrentReqStateInfo)   // { added by sungha.choi 2013.08.20 for ipod coverart
        {
            if(m_CurrentReqStateInfo->currentQueryId != queryId || m_CurrentReqStateInfo->requestUID != reqUID)
            {
                delete m_CurrentReqStateInfo;
                m_CurrentReqStateInfo = NULL;
            }
        }   // } added by sungha.choi 2013.08.20 for ipod coverart
        if(m_CurrentReqStateInfo == NULL)
        {
            m_CurrentReqStateInfo = new SRequestStateInfo;
            m_CurrentReqStateInfo->requestUID = reqUID;
            m_CurrentReqStateInfo->currentQueryId = queryId;    // added by sungha.choi 2013.08.18 for blank list hotfix
        }
    }
    else if(reqState == eWaitRequest)
    {
        m_ReqStateInfo->requestUID = reqUID;
        QueueRequest();
        emit RequestState(eWaitRequest, reqUID);
        m_ReqStateInfo->initialize();
        return true;
    }
    else if(reqState == eCancelRequest)
    {
        emit RequestState(eCancelRequest, reqUID);
        return false;
    }

    if(m_QueueRequestTimer->isActive())
        m_QueueRequestTimer->stop();

    m_QueueRequestTimer->start(MAX_TIME_LIMIT);
    m_ReqStateInfo->reqState = eProcesscomplete;

    //Check if Gracenote Normalized data is selected by the user.
    bool normalizedData = GetGracenoteNormalizedData(); //added by Divya for Gracenote normalized data 2013.01.07

   switch(volumeType)
   {
	   case eIPod :
	   case eIPOD_FRONT:
	   {
	       // { Modified by Neelima, 2013.01.03 for CR13965
           QString devID = m_MediaDataProvider->getDeviceID(volumeType);
         //{modified by Divya for Gracenote normalized data 2013.01.07
		   if(normalizedData)
		                 ret = m_GracenoteDataProvider->RequestData(searchList, queryId, volumeType);
           else
		   
		   {
		   		if(createFirstIpodConnection(devID))
               		ret = m_MediaDataProvider->IpodRequestsearchData(searchList,queryId,firstIpodDB);    
		   		else
		   			ret= false;
		   }
		   //}modified by Divya for Gracenote normalized data 2013.01.07           
		   // } Modified by Neelima	 
	   }
	   		break;
	   case eIPOD_REAR:
	   {
	       // { Modified by Neelima, 2013.01.03 for CR13965
           QString devID = m_MediaDataProvider->getDeviceID(volumeType);
         //{modified by Divya for Gracenote normalized data 2013.01.07
		   if(normalizedData)
		                 ret = m_GracenoteDataProvider->RequestData(searchList, queryId, volumeType);
           else
		   {
		   		if(createSecondIpodConnection(devID))
               		ret = m_MediaDataProvider->IpodRequestsearchData(searchList,queryId,secondIpodDB);    
		   		else
		   			ret= false;
		   }     
			//}modified by Divya for Gracenote normalized data 2013.01.07      
		   // } Modified by Neelima	 
	   }
	   		break;
	   case eUSB :
	   case eUSB_FRONT:
	   case eUSB_REAR:
	   case eJukeBox:
	   {
          // switch case to call different queries of different data provider using query id
	      switch (queryId)
	      {
			    case eGetAllSongsByComposerAlbumQuery_Id:
			    {
				//{modified by Divya for Gracenote normalized data 2013.01.07
               if(normalizedData)
              {
                   qDebug() << "CTrackerAbstractor::RequestData normalized data is set";
                   ret = m_GracenoteDataProvider->RequestData(searchList, queryId, volumeType);
               }
               else
                   ret = m_MediaDataProvider->GetAllSongsByComposerAlbum(searchList, volumeType);
			    }
			    	break;
			    case eGetAllAlbumByGenreComposerQuery_Id:
			    {
            if(normalizedData)
                ret = m_GracenoteDataProvider->RequestData(searchList, queryId, volumeType);
            else
                ret = m_MediaDataProvider->GetMusicAlbumsByGenreComposer(searchList, volumeType);
			    }
			    	break;
			    case eGetAllAlbumByComposerQuery_Id:
			    {
             if(normalizedData)
                 ret = m_GracenoteDataProvider->RequestData(searchList, queryId, volumeType);
             else
                 ret = m_MediaDataProvider->GetMusicAblumsByComposer(searchList, volumeType);
			    }
			    	break;
			    case eGetAllComposerByGenreQuery_Id:
			    {
            if(normalizedData)
                ret = m_GracenoteDataProvider->RequestData(searchList, queryId, volumeType);
            else
                ret = m_MediaDataProvider->GetMusicComposersByGenre(searchList, volumeType);
			    }
			    	break;
			    case eGetAllSongsByGenreArtistAlbumQuery_Id:
			    {
            if(normalizedData)
                ret = m_GracenoteDataProvider->RequestData(searchList, queryId, volumeType);
            else
                 ret = m_MediaDataProvider->GetAllSongsByGenreArtistAlbum(searchList, volumeType);
			    }
			    	break;
			    case eGetAllSongsByGenreComposerAlbumQuery_Id:
			    {
            if(normalizedData)
                ret = m_GracenoteDataProvider->RequestData(searchList, queryId, volumeType);
            else
                ret = m_MediaDataProvider->GetAllSongsByGenreComposerAlbum(searchList, volumeType);
			    }
			    	break;
			    case eGetAllSongsByArtistAlbumQuery_Id:
			    {
            if(normalizedData)
                ret = m_GracenoteDataProvider->RequestData(searchList, queryId, volumeType);
            else
                 ret = m_MediaDataProvider->GetAllSongsByArtistAlbum(searchList, volumeType);
			    }
			    	break;
			    case eGetAllAlbumByGenreArtistQuery_Id:
			    {
            if(normalizedData)
                ret = m_GracenoteDataProvider->RequestData(searchList, queryId, volumeType);
            else
                 ret = m_MediaDataProvider->GetMusicAlbumsByGenreArtist(searchList, volumeType);
			    }
			    	break;
			    case eGetAllAlbumByArtistQuery_Id:
			    {
            if(normalizedData)
                ret = m_GracenoteDataProvider->RequestData(searchList, queryId, volumeType);
            else
                ret = m_MediaDataProvider->GetMusicAlbumsByArtist(searchList, volumeType);
			    }
			  		break;
			    case eGetAllArtistByGenreQuery_Id:
			    {
            if(normalizedData)
                ret = m_GracenoteDataProvider->RequestData(searchList, queryId, volumeType);
            else
                ret = m_MediaDataProvider->GetMusicArtistsByGenre(searchList, volumeType);
			    }
			    	break;
					//}modified by Divya for Gracenote normalized data 2013.01.07
			    case eGetMultipleGenreSongsFromVolumeQuery_Id:
			    {
			        ret = m_MediaDataProvider->GetMultipleGenreSongsFromVolume(searchList, volumeType);
			    }
			   		break;
			    case eGetMultipleAlbumSongsFromVolumeQuery_Id:
			    {
			        ret = m_MediaDataProvider->GetMultipleAlbumSongsFromVolume(searchList, volumeType);
			    }
				  	break;
			    case eGetMultipleArtistSongsFromVolumeQuery_Id:
			    {
			        ret = m_MediaDataProvider->GetMultipleArtistSongsFromVolume(searchList, volumeType);
			    }
			    break;		
			    default:
			        break;
			    }
	   }
	   case eDefaultVolume:
	   default:
	       break;
   }
   if(!ret) m_ReqStateInfo->reqState = eDefaultRequest;
   return ret;
}

/******************************************************************************

// Function Name                                            Parameters

// CTrackerAbstractor::RequestData                 bool (OUT), int (IN) - StartIndex
                                                                                      int (IN) - endindex
                                                                     ETrackerAbstarctor_Query_Ids (IN)- respective query Id

//

// Explanation
                                                                     //This function will invoke the extraction of ipod music details
                                                                     //between the specific trackindex.
//

// History

// 06-Dec-2011      Pawan Gupta
*****************************************************************************/
bool CTrackerAbstractor::RequestData(int startIndex, int endIndex, ETrackerAbstarctor_Query_Ids queryId, uint reqUID)
{
    int ret=false;

    m_ReqStateInfo->ReqData_set = eRequest_NoVol_TwoIntParams;
    m_ReqStateInfo->intParam1 = startIndex;
    m_ReqStateInfo->intParam2 = endIndex;
    int reqState = RequestHandler(queryId,eDefaultVolume);
    if(reqState ==eDefaultRequest || reqState == eProcessRequest)
    {
        if(m_CurrentReqStateInfo)   // { added by sungha.choi 2013.08.20 for ipod coverart
        {
            if(m_CurrentReqStateInfo->currentQueryId != queryId || m_CurrentReqStateInfo->requestUID != reqUID)
            {
                delete m_CurrentReqStateInfo;
                m_CurrentReqStateInfo = NULL;
            }
        }   // } added by sungha.choi 2013.08.20 for ipod coverart
        if(m_CurrentReqStateInfo == NULL)
        {
            m_CurrentReqStateInfo = new SRequestStateInfo;
            m_CurrentReqStateInfo->requestUID = reqUID;
            m_CurrentReqStateInfo->currentQueryId = queryId;    // added by sungha.choi 2013.08.18 for blank list hotfix
        }
    }
    else if(reqState == eWaitRequest)
    {
        m_ReqStateInfo->requestUID = reqUID;
        QueueRequest();
        emit RequestState(eWaitRequest, reqUID);
        m_ReqStateInfo->initialize();
        return true;
    }
    else if(reqState == eCancelRequest)
    {
        emit RequestState(eCancelRequest, reqUID);
        return false;
    }

    if(m_QueueRequestTimer->isActive())
        m_QueueRequestTimer->stop();

    m_QueueRequestTimer->start(MAX_TIME_LIMIT);
    m_ReqStateInfo->reqState = eProcesscomplete;

    switch(queryId)
    {
	    case eGetMusicDetailFromIpodOnIndexBasis_Id:
	        ret = m_MediaDataProvider->GetMusicTracksBetweenIndexFromIpod(startIndex, endIndex);
	        break;
	    default:
	        break;
    }
    if(!ret) m_ReqStateInfo->reqState = eDefaultRequest;
    return ret;
}

/******************************************************************************

// Function Name                                            Parameters

// CTrackerAbstractor::RequestData                 bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume to search,
                                                                     ETrackerAbstarctor_OrderType (IN) - order of result data
                                                                     ETrackerAbstarctor_Query_Ids (IN)- respective query Id

//

// Explanation

// This method calls all songs with metadata  Volume query APIs of Media Provider using the query id

// It connects signal/slot for the query completion

//

// History

// 29-Oct-2011      Chetan Nanda                created for handing volume related Queries of Media provider
*****************************************************************************/

bool CTrackerAbstractor::RequestData(ETrackerAbstarctor_VolumeType volumeType,
                                     ETrackerAbstarctor_OrderType orderType, ETrackerAbstarctor_Query_Ids queryId, uint reqUID)
{
    bool ret = false;

    m_ReqStateInfo->ReqData_set = eRequest_Vol_OrdParam;
    m_ReqStateInfo->intParam1 = orderType;
    int reqState = RequestHandler(queryId,volumeType);
    if(reqState ==eDefaultRequest || reqState == eProcessRequest)
    {
        if(m_CurrentReqStateInfo)   // { added by sungha.choi 2013.08.20 for ipod coverart
        {
            if(m_CurrentReqStateInfo->currentQueryId != queryId || m_CurrentReqStateInfo->requestUID != reqUID)
            {
                delete m_CurrentReqStateInfo;
                m_CurrentReqStateInfo = NULL;
            }
        }   // } added by sungha.choi 2013.08.20 for ipod coverart
        if(m_CurrentReqStateInfo == NULL)
        {
            m_CurrentReqStateInfo = new SRequestStateInfo;
            m_CurrentReqStateInfo->requestUID = reqUID;
            m_CurrentReqStateInfo->currentQueryId = queryId;    // added by sungha.choi 2013.08.18 for blank list hotfix
        }
    }
    else if(reqState == eWaitRequest)
    {
        m_ReqStateInfo->requestUID = reqUID;
        QueueRequest();
        emit RequestState(eWaitRequest, reqUID);
        m_ReqStateInfo->initialize();
        return true;
    }
    else if(reqState == eCancelRequest)
    {
        emit RequestState(eCancelRequest, reqUID);
        return false;
    }

    if(m_QueueRequestTimer->isActive())
        m_QueueRequestTimer->stop();

    m_QueueRequestTimer->start(MAX_TIME_LIMIT);
    m_ReqStateInfo->reqState = eProcesscomplete;

    bool normalizedData = GetGracenoteNormalizedData(); //added by Divya for Gracenote normalized data 2013.01.07

    if(volumeType == eIPod || volumeType == eIPOD_FRONT)
    {
        if(queryId == eGetAllMetaDataWithLimitFromVolumeQuery_Id)
        {
		//{added by Divya for Gracenote normalized data 2013.01.07
            if(normalizedData)
            {
                m_GracenoteDataProvider->m_Query_Offset = m_MediaDataProvider->m_Query_Offset;
                m_GracenoteDataProvider->RequestData(orderType, queryId, volumeType);
            }
            else
			{
			//}added by Divya for Gracenote normalized data 2013.01.07
			// { Added by Neelima, 2013.01.03 for CR13965
            	QString devID = m_MediaDataProvider->getDeviceID(volumeType);

            	if(createFirstIpodConnection(devID)) // } Added by Neelima
                	m_MediaDataProvider->IpodRequestData(eGetAllMetaDataWithLimitFromVolumeQuery_Id, firstIpodDB, orderType);
        	}
		}//added by Divya for Gracenote normalized data 2013.01.07
    }
    else if(volumeType == eIPOD_REAR)
    {
        if(queryId == eGetAllMetaDataWithLimitFromVolumeQuery_Id)
        {
           //{added by Divya for Gracenote normalized data 2013.01.07
            if(normalizedData)
            {
                m_GracenoteDataProvider->m_Query_Offset = m_MediaDataProvider->m_Query_Offset;
                m_GracenoteDataProvider->RequestData(orderType, queryId, volumeType);
            }
            else
			{
			//}added by Divya for Gracenote normalized data 2013.01.07
			// { Added by Neelima, 2013.01.03 for CR13965
            	QString devID = m_MediaDataProvider->getDeviceID(volumeType);

            	if(createSecondIpodConnection(devID)) // } Added by Neelima
                	m_MediaDataProvider->IpodRequestData(eGetAllMetaDataWithLimitFromVolumeQuery_Id, secondIpodDB, orderType);
        	}
		}//}added by Divya for Gracenote normalized data 2013.01.07
    }
    else
    {
        // switch case to call different queries of different data provider using query id
        switch (queryId)
        {
	        case eGetAllSongsWithMetaDataFromVolumeQuery_Id:
	        {
	            ret = m_MediaDataProvider->GetAllSongsWithMetaDataFromVolume(volumeType, orderType);
	        }
	            break;
	        case eGetAllMetaDataWithLimitFromVolumeQuery_Id:
	        {
			//{added by Divya for Gracenote normalized data 2013.01.07
                if(normalizedData)
                {
                    m_GracenoteDataProvider->m_Query_Offset = m_MediaDataProvider->m_Query_Offset;
                    m_GracenoteDataProvider->RequestData(orderType, queryId, volumeType);
                }
                else
				//}added by Divya for Gracenote normalized data 2013.01.07
                    ret = m_MediaDataProvider->GetAllMetadataWithLimitFromVolume(volumeType, orderType);
	        }
	            break;
	        default:
	            break;
        }
    }
    if(!false) m_ReqStateInfo->reqState = eDefaultRequest;
    return ret;
}

/******************************************************************************

// Function Name                                            Parameters

// CTrackerAbstractor::RequestData                 bool (OUT), ETrackerAbstarctor_VolumeType (IN) - volume to search,
                                                                    ETrackerAbstarctor_Query_Ids (IN)- respective query Id

//

// Explanation

// This method calls respective Volume query APIs of Media Provider using the query id

// It connects signal/slot for the query completion

//

// History

// 22-Oct-2011      Chetan Nanda                created for handing volume related Queries of Media provider
*****************************************************************************/

bool CTrackerAbstractor::RequestData(ETrackerAbstarctor_VolumeType volumeType, ETrackerAbstarctor_Query_Ids queryId, uint reqUID)
{
    bool ret = false;    

    // { added by kihyung 2013.08.22 for ITS 0185245
    if(CDirTreeProvider::IsDirTreeQuery(queryId) == true)
    {
        CDirTreeProvider *pTree = GetDirTreeProvider();
        if(pTree == NULL) return ret;
        
        m_DirTreeQueryId = queryId;
        if(queryId == eGetMusicLimitFileListDir_Id)  ret = pTree->GetAllAudioFileList(volumeType, reqUID);
        m_DirTreeQueryId = 0;

        return ret;
    }
    // } added by kihyung 2013.08.22 for ITS 0185245
      
    m_ReqStateInfo->ReqData_set = eRequest_Vol_NoParam;
    int reqState;
    reqState = RequestHandler(queryId,volumeType);
    if(reqState ==eDefaultRequest || reqState == eProcessRequest)
    {
        if(m_CurrentReqStateInfo)   // { added by sungha.choi 2013.08.20 for ipod coverart
        {
            if(m_CurrentReqStateInfo->currentQueryId != queryId || m_CurrentReqStateInfo->requestUID != reqUID)
            {
                delete m_CurrentReqStateInfo;
                m_CurrentReqStateInfo = NULL;
            }
        }   // } added by sungha.choi 2013.08.20 for ipod coverart
        if(m_CurrentReqStateInfo == NULL)
        {
            m_CurrentReqStateInfo = new SRequestStateInfo;
            m_CurrentReqStateInfo->requestUID = reqUID;
            m_CurrentReqStateInfo->currentQueryId = queryId;    // added by sungha.choi 2013.08.18 for blank list hotfix
        }				
         // removed by eugene.seo 2013.05.02
    }
    else if(reqState == eWaitRequest)
    {    	
	    m_ReqStateInfo->requestUID = reqUID;
	    QueueRequest();
	    emit RequestState(eWaitRequest, reqUID);
	    m_ReqStateInfo->initialize();

        // { added by eugene.seo 2013.05.17
		if(	(volumeType == eUSB_FRONT || volumeType == eUSB_REAR) &&
				//queryId == eGetAllAlbumsWithUrlFromVolumeQuery_Id &&
					  m_CoverArtDataprovider->GetIPODRequestComplete())
    	{
	    	LOG_TRACE "Warning::CanceliPodCoverArtRequest!" << LOG_ENDL;
			m_CoverArtDataprovider->CanceliPodCoverArtRequest();
    	}
		// } added by eugene.seo 2013.05.17

		return true;		
    }
    else if(reqState == eCancelRequest)
    {
        emit RequestState(eCancelRequest, reqUID);
        return false;
    }

    if(m_QueueRequestTimer->isActive())
        m_QueueRequestTimer->stop();

    m_QueueRequestTimer->start(MAX_TIME_LIMIT);
    m_ReqStateInfo->reqState = eProcesscomplete;


    bool normalizedData = GetGracenoteNormalizedData(); //added by Divya for Gracenote normalized data 2013.01.07
    if(volumeType==eIPOD_FRONT||volumeType==eIPod)
    {
         // { Modified by Neelima, 2013.01.03 for CR13965
         QString devID = m_MediaDataProvider->getDeviceID(volumeType); 
        if(queryId == eGetAllPodcastsByTitleQuery_Id||queryId == eGetAllAudiobooksByTitleQuery_Id ||
             queryId == eGetAlliTunesUByTitleQuery_Id)
        {
               if(createFirstIpodConnection(devID))
            ret = m_IpodDataProvider->GetAllIPODAudiobookspodcastdata(queryId,firstIpodDB);
			   else 
			   		ret = false;
        }
        else
        {
		//{modified by Divya for Gracenote normalized data 2013.01.07
                if(normalizedData)
                    ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                else
				{
				//}by Divya for Gracenote normalized data 2013.01.07
                    if(createFirstIpodConnection(devID))
                		ret = m_MediaDataProvider->IpodRequestData(queryId,firstIpodDB);
					else
						ret = false;
				}//by Divya for Gracenote normalized data 2013.01.07
        }
			// } Modified by Neelima
    }
    else if(volumeType==eIPOD_REAR)
    {
		 // { Modified by Neelima, 2013.01.03 for CR13965
         QString devID = m_MediaDataProvider->getDeviceID(volumeType); 

         if(queryId == eGetAllAudiobooksByTitleQuery_Id || queryId == eGetAllPodcastsByTitleQuery_Id ||
             queryId == eGetAlliTunesUByTitleQuery_Id)
        {
            if(createSecondIpodConnection(devID))
            	ret = m_IpodDataProvider->GetAllIPODAudiobookspodcastdata(queryId,secondIpodDB);
			else
				ret = false;
           }
        else
        {
		//{modified by Divya for Gracenote normalized data 2013.01.07
              if(normalizedData)
                  ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
              else
			  {
			  //}by Divya for Gracenote normalized data 2013.01.07
                  if(createSecondIpodConnection(devID))
             		ret = m_MediaDataProvider->IpodRequestData(queryId,secondIpodDB);
			 	  else
			 		ret = false;
				  
			  }//by Divya for Gracenote normalized data 2013.01.07
        }
		  // } Modified by Neelima
    }
    else
    {
        // switch case to call different queries of different data provider using query id
	    switch (queryId)
	    {
		    case eGetAllSongsFromVolumeQuery_Id:
		    {
			//{modified by Divya for Gracenote normalized data 2013.01.07
                    if(normalizedData)
                    {
                        ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                    }
                    else
                        ret = m_MediaDataProvider->GetAllSongsFromVolume(volumeType);
		    }
		        break;
		    case eGetAllAlbumsFromVolumeQuery_Id:
		    {
                    if(normalizedData)
                        ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                    else
                        ret = m_MediaDataProvider->GetAllAlbumsFromVolume(volumeType);
		    }
		        break;
		    case eGetAllMusicArtistsFromVolumeQuery_Id:
                {
                    if(normalizedData)
                        ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                    else
                        ret = m_MediaDataProvider->GetAllMusicArtistsFromVolume(volumeType);
		    }
		        break;
		    case eGetAllMusicComposersFromVolumeQuery_Id:
		    {
                    if(normalizedData)
                        ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                    else
                        ret = m_MediaDataProvider->GetAllMusicComposersFromVolume(volumeType);
		    }
		        break;
		    case eGetAllMusicGenresFromVolumeQuery_Id:
		    {
                    if(normalizedData)
                        ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                    else
                        ret = m_MediaDataProvider->GetAllMusicGenresFromVolume(volumeType);
		    }
		        break;
				//}modified by Divya for Gracenote normalized data 2013.01.07
		    case eGetAllImagesFromVolumeQuery_Id:
		    {
		        ret = m_MediaDataProvider->GetAllImagesFromVolume(volumeType);
		    }
		        break;
		    case eGetAllVideosFromVolumeQuery_Id:
		    {
		        ret = m_MediaDataProvider->GetAllVideosFromVolume(volumeType);
		    }
		        break;
		    case eGetlistofPlaylistsQuery_Id:
		    {
		        ret = m_MediaDataProvider->GetlistofPlaylists(volumeType);
		    }
		        break;
		    case eGetMostPlayedMusicSongsQuery_Id:
		    {
		        ret = m_MediaDataProvider->GetMostPlayedMusicSongs(volumeType);
		    }
		        break;
		    case eGetMostRecentlyPlayedSongsQuery_Id:
		    {
		        ret = m_MediaDataProvider->GetMostRecentlyPlayedSongs(volumeType);
		    }
		        break;
		    case eGetAllPodcastsByTitleQuery_Id:
		    {
		        ret = m_IpodDataProvider->GetAllPodcastsByTitle(volumeType);
		    }
		        break;
		    case eGetAllAudiobooksByTitleQuery_Id:
		    {
		        ret = m_IpodDataProvider->GetAllAudiobooksByTitle(volumeType);
		    }
		        break;
		    case eGetAllVideosCountFromVolumeQuery_Id:
		    {
		        ret = m_MediaDataProvider->GetAllVideosCountFromVolume(volumeType);
		    }
		        break;
		    case eGetAllImagesCountFromVolumeQuery_Id:
		    {
		        ret = m_MediaDataProvider->GetAllImagesCountFromVolume(volumeType);
		    }
		        break;
		    case eGetAllAlbumsWithCoverArtFromVolumeQuery_Id:
		    {
		        ret = m_MediaDataProvider->GetAllAlbumsWithCoverArtFromVolume(volumeType);
		    }
		        break;
		    case eGetAllSongsWithoutAlbumFromVolumeQuery_Id:
		    {
			//{modified by Divya for Gracenote normalized data 2013.01.07
                    if(normalizedData)
                        ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                    else
                        ret = m_MediaDataProvider->GetAllSongsWithoutMetaInfoFromVolume(volumeType);
		    }
		        break;
				//}modified by Divya for Gracenote normalized data 2013.01.07
		    case eIsThereAnySongWithoutAlbumFromVolumeQuery_Id:
		    {
		        ret = m_MediaDataProvider->IsThereAnySongWithoutAlbumFromVolume(volumeType);
		    }
		        break;
            // { removed by kihyung 2013.2.17
            /*
		    case eGetMusicFileListDir_Id:
		    {
                ret = m_DirListProvider->GetMusicListByDirList(volumeType);
                m_ReqStateInfo->reqState = eDefaultRequest;
		    }
		        break;
		    case eGetVideoFileListDir_Id:
		    {
                // { modified by kihyung 2013.2.17
		        // ret = m_DirListProvider->GetVideoListByDirList(volumeType);
		        ret = m_DirTreeProvider->GetAllVideoFileList(volumeType);
				m_ReqStateInfo->reqState = eDefaultRequest;
                // } modified by kihyung 2013.2.17
		    }
		        break;
		    case eGetPhotoFileListDir_Id:
		    {
                // { modified by kihyung 2013.2.17
		        // ret = m_DirListProvider->GetPhotoListByDirList(volumeType);
		        ret = m_DirTreeProvider->GetAllPhotoFileList(volumeType);
		        // } modified by kihyung 2013.2.17
				m_ReqStateInfo->reqState = eDefaultRequest;
		    }
		        break;
		    */
		    // } modified by kihyung 2013.2.17
                    case eGetVideoLimitFileListDir_Id:
                    {
                m_VolumeType = volumeType;
                // { modified by kihyung 2013.2.17
                        // ret = m_DirListProvider->GetLimitedVideoListByDirList(volumeType);
                        CDirTreeProvider *pTree = GetDirTreeProvider();
                if(pTree) {
                            ret = pTree->GetAllVideoFileList(volumeType);
                }
                // } modified by kihyung 2013.2.17
                                m_ReqStateInfo->reqState = eDefaultRequest;
                    }
                        break;
                    case eGetPhotoLimitFileListDir_Id:
                    {
                m_VolumeType = volumeType;
                // { modified by kihyung 2013.2.17
                        // ret = m_DirListProvider->GetLimitedPhotoListByDirList(volumeType);
                CDirTreeProvider *pTree = GetDirTreeProvider();
                if(pTree) {
                            ret = pTree->GetAllPhotoFileList(volumeType);
                }
                // } modified by kihyung 2013.2.17
                                m_ReqStateInfo->reqState = eDefaultRequest;
                    }
                        break;
		    case eRemoveAllMediaDataFromVolumeQuery_Id:
		    {
		        ret = m_MediaDataProvider->RemoveAllMediaData(volumeType);
                        if(m_QueueRequestTimer->isActive()) // temporary fix // { added by sungha.choi 2013.08.24 for ITS 0186401
                            m_QueueRequestTimer->stop();
                        m_QueueRequestTimer->start(100); // temporary fix // } added by sungha.choi 2013.08.24 for ITS 0186401
		    }
		        break;
		    case eGetAllAlbumsWithUrlFromVolumeQuery_Id:
		    {
			//{modified by Divya for Gracenote normalized data 2013.01.07
                    if(normalizedData)
                        ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                    else
                        ret = m_MediaDataProvider->GetAllAlbumsWithUrlFromVolume(volumeType);
		    }
		        break;
		    case eGetAllSongsCountFromVolumeQuery_Id:
		    {
			    m_VolumeType = volumeType;
		        ret = m_MediaDataProvider->GetAllSongsCountFromVolume(volumeType);
		    }
		      	break;
		    case eGetAllSongsWithTitleCountFromVolumeQuery_Id:
		    {
                    if(normalizedData)
                        ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                    else
                        ret = m_MediaDataProvider->GetAllSongsWithTitleCountFromVolume(volumeType);
		    }
		    	break;
		    case eGetAllAlbumsCountFromVolumeQuery_Id:
		    {
                    if(normalizedData)
                        ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                    else
                        ret = m_MediaDataProvider->GetAllAlbumsCountFromVolume(volumeType);
		    }
		    	break;
		    case eGetAllMusicArtistsCountFromVolumeQuery_Id:
		    {
                    if(normalizedData)
                        ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                    else
                        ret = m_MediaDataProvider->GetAllMusicArtistsCountFromVolume(volumeType);
		    }
		    	break;  
		    case eGetAllMusicComposersCountFromVolumeQuery_Id:
		    {
                    if(normalizedData)
                        ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                    else
                        ret = m_MediaDataProvider->GetAllMusicComposersCountFromVolume(volumeType);
		    }
		    	break;    
		    case eGetAllMusicGenresCountFromVolumeQuery_Id:
		    {
                    if(normalizedData)
                        ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                    else
                        ret = m_MediaDataProvider->GetAllMusicGenresCountFromVolume(volumeType);
		    }
		    	break;
		    case eGetAllAlbumsWithCoverArtCountFromVolumeQuery_Id:
		    {
		        ret = m_MediaDataProvider->GetAllAlbumsWithCoverArtCountFromVolume(volumeType);
		    }
		    	break;
		    case eGetAllSongsWithLimitFromVolumeQuery_Id:
		    {
                    if(normalizedData)
                    {
                         m_GracenoteDataProvider->m_Query_Offset = m_MediaDataProvider->m_Query_Offset;
                         m_GracenoteDataProvider->m_Query_Limit = m_MediaDataProvider->m_Query_Limit;
                        ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                    }
                    else
                        ret = m_MediaDataProvider->GetAllSongsWithLimitFromVolume(volumeType);
		    }
		        break;
		    case eGetAllAlbumsWithLimitFromVolumeQuery_Id:
		    {
                    if(normalizedData)
                    {
                        m_GracenoteDataProvider->m_Query_Offset = m_MediaDataProvider->m_Query_Offset;
                        m_GracenoteDataProvider->m_Query_Limit = m_MediaDataProvider->m_Query_Limit;
                        ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                    }
                    else
                        ret = m_MediaDataProvider->GetAllAlbumsWithLimitFromVolume(volumeType);
		    }
		        break;
		    case eGetAllMusicArtistsWithLimitFromVolumeQuery_Id:
		    {
                    if(normalizedData)
                    {
                        m_GracenoteDataProvider->m_Query_Offset = m_MediaDataProvider->m_Query_Offset;
                        m_GracenoteDataProvider->m_Query_Limit = m_MediaDataProvider->m_Query_Limit;
                        ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                    }
                    else
                        ret = m_MediaDataProvider->GetAllMusicArtistsWithLimitFromVolume(volumeType);
		    }
		        break;
		    case eGetAllMusicComposersWithLimitFromVolumeQuery_Id:
		    {
                    if(normalizedData)
                    {
                        m_GracenoteDataProvider->m_Query_Offset = m_MediaDataProvider->m_Query_Offset;
                        m_GracenoteDataProvider->m_Query_Limit = m_MediaDataProvider->m_Query_Limit;
                        ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                    }
                    else
                        ret = m_MediaDataProvider->GetAllMusicComposersWithLimitFromVolume(volumeType);
		    }
		        break;
		    case eGetAllMusicGenresWithLimitFromVolumeQuery_Id:
		    {
                    if(normalizedData)
                    {
                        m_GracenoteDataProvider->m_Query_Offset = m_MediaDataProvider->m_Query_Offset;
                        m_GracenoteDataProvider->m_Query_Limit = m_MediaDataProvider->m_Query_Limit;
                        ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                    }
                    else
                        ret = m_MediaDataProvider->GetAllMusicGenresWithLimitFromVolume(volumeType);
		    }
		        break;
		    case eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id:
		    {
                    if(normalizedData)
                    {
                        m_GracenoteDataProvider->m_Query_Offset = m_MediaDataProvider->m_Query_Offset;
                        m_GracenoteDataProvider->m_Query_Limit = m_MediaDataProvider->m_Query_Limit;
                        ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                    }
                    else
                        ret = m_MediaDataProvider->GetAllAlbumsWithCoverArtWithLimitFromVolume(volumeType);
		    }
		      	break;
				//}modified by Divya for Gracenote normalized data 2013.01.07
                    // { added by sungha.choi 2013.07.23
                    case eGetAllSongsWithLimitCacheFromVolumeQuery_Id:
                        {
                            if(normalizedData)
                            {
                                m_GracenoteDataProvider->m_Query_Offset = m_MediaDataProvider->m_Query_Offset;
                                m_GracenoteDataProvider->m_Query_Limit = m_MediaDataProvider->m_Query_Limit;
                                ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                            }
                            else
                                ret = m_MediaDataProvider->GetAllSongsWithLimitCacheFromVolume(volumeType);
                        }
                        break;
                    case eGetAllAlbumsWithCoverArtWithLimitCacheFromVolumeQuery_Id:
                        {
                            if(normalizedData)
                            {
                                m_GracenoteDataProvider->m_Query_Offset = m_MediaDataProvider->m_Query_Offset;
                                m_GracenoteDataProvider->m_Query_Limit = m_MediaDataProvider->m_Query_Limit;
                                ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                            }
                            else
                                ret = m_MediaDataProvider->GetAllAlbumsWithCoverArtWithLimitCacheFromVolume(volumeType);
                        }
                        break;
                    case eGetAllMusicArtistsWithLimitCacheFromVolumeQuery_Id:
                        {
                            if(normalizedData)
                            {
                                m_GracenoteDataProvider->m_Query_Offset = m_MediaDataProvider->m_Query_Offset;
                                m_GracenoteDataProvider->m_Query_Limit = m_MediaDataProvider->m_Query_Limit;
                                ret = m_GracenoteDataProvider->RequestData(queryId, volumeType);
                            }
                            else
                                ret = m_MediaDataProvider->GetAllMusicArtistsWithLimitCacheFromVolume(volumeType);
                        }
                        break;
                    // } added by sungha.choi 2013.07.23
		    default:
		        break;
  		}
  	}
    qDebug() << "end>>>>>>>>>>";
    if(!ret) m_ReqStateInfo->reqState = eDefaultRequest;
  	return ret;
}

/******************************************************************************

// Function Name                                            Parameters

// CTrackerAbstractor::RequestData                 bool (OUT), QString (IN) - folder to search (if require),
                                                                    ETrackerAbstarctor_Query_Ids (IN)- respective query Id

//

// Explanation

// This method calls respective query APIs of Providers using the query id

// It connects signal/slot for the query completion

//

// History

// 12-Sept-2011      Chetan Nanda                created for handing Music related Queries

//                                                                 and Text Seach Query
*****************************************************************************/

bool CTrackerAbstractor::RequestData(QString searchData, ETrackerAbstarctor_Query_Ids queryId, uint reqUID)
{
    bool ret = false;

    // { added by kihyung 2013.08.22 for ITS 0185245
    if(CDirTreeProvider::IsDirTreeQuery(queryId) == true)
    {
        CDirTreeProvider *pTree = GetDirTreeProvider();
        if(pTree == NULL) return ret;

        m_DirTreeQueryId = queryId;
        if(queryId == eGetFolderTabList_Id)                ret = pTree->GetSubAudioFileList(searchData, eDefaultVolume, reqUID);
        m_DirTreeQueryId = 0;

        return ret;
    }
    // } added by kihyung 2013.08.22 for ITS 0185245
    
    if(searchData.isNull())
    {
        HandleError(eNullStringParameter);
        return false;
    }

    m_ReqStateInfo->ReqData_set = eRequest_NoVol_StringParam;
    m_ReqStateInfo->Stringparam1 = searchData;
    int reqState = RequestHandler(queryId,eDefaultVolume);
    if(reqState ==eDefaultRequest || reqState == eProcessRequest)
    {
        if(m_CurrentReqStateInfo)   // { added by sungha.choi 2013.08.20 for ipod coverart
        {
            if(m_CurrentReqStateInfo->currentQueryId != queryId || m_CurrentReqStateInfo->requestUID != reqUID)
            {
                delete m_CurrentReqStateInfo;
                m_CurrentReqStateInfo = NULL;
            }
        }   // } added by sungha.choi 2013.08.20 for ipod coverart
        if(m_CurrentReqStateInfo == NULL)
        {
            m_CurrentReqStateInfo = new SRequestStateInfo;
            m_CurrentReqStateInfo->requestUID = reqUID;
            m_CurrentReqStateInfo->currentQueryId = queryId;    // added by sungha.choi 2013.08.18 for blank list hotfix
        }
    }
    else if(reqState == eWaitRequest)
    {
        m_ReqStateInfo->requestUID = reqUID;
        QueueRequest();
        emit RequestState(eWaitRequest, reqUID);
        m_ReqStateInfo->initialize();
        return true;
    }
    else if(reqState == eCancelRequest)
    {
        emit RequestState(eCancelRequest, reqUID);
        return false;
    }

    if(m_QueueRequestTimer->isActive())
        m_QueueRequestTimer->stop();

    m_QueueRequestTimer->start(MAX_TIME_LIMIT);
    m_ReqStateInfo->reqState = eProcesscomplete;

    bool normalizedData = GetGracenoteNormalizedData(); //added by Divya for Gracenote normalized data 2013.01.07

    // switch case to call different queries of different data provider using query id
    switch (queryId)
    {
	    case  eSongsByFolderQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetSongsByFolder(searchData);
	    }
	        break;
	    case  eArtistByFolderQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetArtistByFolder(searchData);
	    }
	        break;
	    case  eAlbumByFolderQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetAlbumsByFolder(searchData);
	    }
	        break;
	    case  eGetAllSongsQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetAllSongs();
	    }
	        break;
	    case  eGetAllSongsIDQuery_id:
	    {
	        ret = m_MediaDataProvider->GetAllSongsIDs();
	    }
	        break;
	    case eFolderListQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetFolderList(searchData);
	    }
	        break;
	    case eMusicAlbumListQuery_Id:
	    {
	        // ret = m_MediaDataProvider->GetAlbumListByFolder(searchData);
	    }
	        break;
	    case eSongsByNextAlbumQuery_Id:
	    {
	        /* if(m_MusicAlbumList != NULL)
	            {
	                ret = m_MediaDataProvider->GetSongsByNextAlbum(m_MusicAlbumList->getNext()->getData(),
	                                                                                                eSongsByNextAlbumQuery_Id);
	            }*/
	        /* if(m_MediaFolderList != NULL)
	            {
	                ret = m_MediaDataProvider->GetSongsByNextFolder(m_MediaFolderList->getNext()->getData(),
	                                                                                                eGetSongsByNextFolderQuery_Id);
	            }*/
	    }
	        break;
	    case eSongsByPrevAlbumQuery_Id:
	    {
	        /* if(m_MusicAlbumList != NULL)
	            {
	                ret = m_MediaDataProvider->GetSongsByPrevAlbum(m_MusicAlbumList->getPrev()->getData(),
	                                                                                                eSongsByPrevAlbumQuery_Id);
	            }*/
	        /*if(m_MediaFolderList != NULL)
	            {
	                ret = m_MediaDataProvider->GetSongsByPrevFolder(m_MediaFolderList->getPrev()->getData(),
	                                                                                                eGetSongsByPrevFolderQuery_Id);
	            }*/
	    }
	        break;
	    case  eGetAllAlbumsQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetAllAlbums();
	    }
	        break;
	    case  eGetAllMusicArtistsQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetAllMusicArtists();
	    }
	        break;
	    case  eGetAllMusicComposersQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetAllMusicComposers();
	    }
	        break;
	    case  eGetAllMusicGenresQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetAllMusicGenres();
	    }
	        break;
	    case  eGetMusicMetaDataQuery_Id:
            case  eGetMusicSimpleMetaDataQuery_Id://added by junam 2013.03.27 for ISV72425
	    {
                if(searchData.startsWith(gVolumePath_IPod))//added by junam 2013.03.27 for ISV72425
	        {
                    QList<QSqlDatabase> dbConnList;
                    // { Modified by Neelima, 2013.01.03 for CR13965

                    QString devID = m_MediaDataProvider->getDeviceID(eIPod);

                    if(devID.isNull() || devID == "Empty")//modofied by edo.lee 2013.01.31
                    {
                        devID.clear();
                        devID = m_MediaDataProvider->getDeviceID(eIPOD_FRONT);
                    }

                    if(createFirstIpodConnection(devID))
                        dbConnList.append(firstIpodDB);

                    devID.clear();
                    devID = m_MediaDataProvider->getDeviceID(eIPOD_REAR);

                    if(createSecondIpodConnection(devID))
                        dbConnList.append(secondIpodDB);
                    // } Modified by Neelima

                    if(dbConnList.count() > 0)
                        m_MediaDataProvider->SetIPodDBConnectionList(dbConnList);
		}

                //{added by junam 2013.03.27 for ISV72425
                if(queryId == eGetMusicSimpleMetaDataQuery_Id)
                    ret = m_MediaDataProvider->GetMusicSimpleMetaData(searchData);
                else //} added by junam 2013.03.27
                    ret = m_MediaDataProvider->GetMusicMetaData(searchData);
            }
	        break;
	    case  eGetAllSongsByAlbumQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetAllSongsByAlbum(searchData);
	    }
	        break;
	    case  eGetAllSongsByArtistQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetAllSongsByArtist(searchData);
	    }
	        break;
	    case  eGetAllSongsByGenreQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetAllSongsByGenre(searchData);
	    }
	        break;
	    case  eGetAllSongsByComposerQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetAllSongsByComposer(searchData);
	    }
	        break;
            case  eGetImagesByFolderQuery_Id:
            {
                        // ret = m_MediaDataProvider->GetImagesByFolder(searchData);
                // ret = m_MediaDataProvider->GetMediaFileListByFolder(searchData, eGetImagesByFolderQuery_Id);
            CDirTreeProvider *pTree = GetDirTreeProvider();
            if(pTree) {
                    ret = pTree->GetPhotoFileListByFolder(searchData);
            }
            }
                break;
            case  eGetVideosByFolderQuery_Id:
            {
                        // ret = m_MediaDataProvider->GetVideosByFolder(searchData);
                // ret = m_MediaDataProvider->GetMediaFileListByFolder(searchData, eGetVideosByFolderQuery_Id);
                CDirTreeProvider *pTree = GetDirTreeProvider();
            if(pTree) {
                ret = pTree->GetVideoFileListByFolder(searchData);
            }
            }
                break;
	    case  eGetAllMusicByKeywordQuery_Id:
	    {        
		      //{modified by Divya for Gracenote normalized data 2013.01.07
			    //Need to check
                QStringList volumeList;
                if(normalizedData)
                    ret = m_GracenoteDataProvider->GetAllMusicByKeyword(searchData, volumeList);
                else
				//}modified by Divya for Gracenote normalized data 2013.01.07
        			ret = m_MediaDataProvider->GetAllMusicByKeywordForVR(searchData);
	    }
	    break;
	    case eGetAllImagesByKeywordQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetAllImagesByKeyword(searchData);
	    }
	        break;
	    case eGetAllVideosByKeywrodQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetAllVideosByKeyword(searchData);
	    }
	        break;
	    case eGetImageThumbNail_Id:
	    {
	        ret = m_CoverArtDataprovider->RequestPhotoThumbnail(searchData);
			m_ReqStateInfo->reqState = eDefaultRequest;
	    }
	        break;
	    case eGetVideoFileThumbNail_Id:
	    {
	        ret = m_CoverArtDataprovider->RequestVideoThumbnail(searchData);
	    }
	        break;
	    case eSongsCountByFolderQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetSongsCountByFolder(searchData);
	    }
	        break;
	    case eGetImagesCountByFolderQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetImagesCountByFolder(searchData);
	    }
	        break;
	    case eGetVideosCountByFolderQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetVideosCountByFolder(searchData);
	    }
	        break;
	    case eGetAllSongsCountQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetAllSongsCount();
	    }
	        break;
	    case eGetMusicFromCDQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetMusicInfoFromCD(searchData);
	    }
	        break;
            case eGetImagesBySubFolderQuery_Id:
            {
                        // ret = m_MediaDataProvider->GetImagesWithSubFolder(searchData);
                        // { modified by kihyung 2013.2.17
                // ret = m_MediaDataProvider->GetMediaFileListWithSubFolder(searchData, eGetImagesBySubFolderQuery_Id);
                CDirTreeProvider *pTree = GetDirTreeProvider();
            if(pTree) {
                ret = pTree->GetPhotoFileListWithSubFolder(searchData);
            }
                // } modified by kihyung
            }
                break;
            case eGetVideosBySubFolderQuery_Id:
            {
                        // ret = m_MediaDataProvider->GetVideosWithSubFolder(searchData);
                        // { modified by kihyung 2013.2.17
                // ret = m_MediaDataProvider->GetMediaFileListWithSubFolder(searchData, eGetVideosBySubFolderQuery_Id);
                CDirTreeProvider *pTree = GetDirTreeProvider();
            if(pTree) {
                ret = pTree->GetVideoFileListWithSubFolder(searchData);
            }
                // } modified by kihyung
            }
                break;

        // { removed by kihyung 2013.2.17
        /*
	    case eGetVideoFileListDir_Id:
	    {
            ret = m_DirListProvider->GetVideoListByDirList(searchData);
            m_ReqStateInfo->reqState = eDefaultRequest;
	    }
			break;		
	    */
	    // } removed by kihyung 2013.2.17

        // { removed by kihyung 2013.2.17
        /*
	    case eGetMusicFileListDir_Id:
	    {
            // { modified by kihyung 2013.2.17
	        ret = m_DirListProvider->GetMusicListByDirList(searchData);
			m_ReqStateInfo->reqState = eDefaultRequest;		
	    }
			break;		
	    */
	    // } removed by kihyung 2013.2.17

        // { removed by kihyung 2013.2.17
        /*
	    case eGetPhotoFileListDir_Id:
	    {
	        ret = m_DirListProvider->GetPhotoListByDirList(searchData);
			m_ReqStateInfo->reqState = eDefaultRequest;
	    }
			break;		
        */
	    // } removed by kihyung 2013.2.17

        // { removed by kihyung 2013.2.17
        /*
        case eGetVideoLimitFileListDir_Id:
	    {
	        ret = m_DirListProvider->GetLimitedVideoListByDirList(searchData);
			m_ReqStateInfo->reqState = eDefaultRequest;		
	    }
			break;
	    */
	    // } removed by kihyung 2013.2.17

        // { removed by kihyung 2013.2.17
        /*
	    case eGetPhotoLimitFileListDir_Id:
	    {
	        ret = m_DirListProvider->GetLimitedPhotoListByDirList(searchData);
			m_ReqStateInfo->reqState = eDefaultRequest;
	    }
		break;
        */
	    // } removed by kihyung 2013.2.17
	    
	    case eRetrieveXSampaDataForType_Id:
	    {
	        ret = m_XSampaInterface->RetrievePhoneticDataForType(searchData);
	    }
	        break;
        case eGetVideoFolderTabList_Id:
        {
            CDirTreeProvider *pTree = GetDirTreeProvider();
            if(pTree) {
                ret = pTree->GetSubVideoFileList(searchData);
            }
        }
            break;
        case eGetPhotoFolderTabList_Id:
        {
            CDirTreeProvider *pTree = GetDirTreeProvider();
            if(pTree) {
                ret = pTree->GetSubPhotoFileList(searchData);
            }
        }
            break;
	    case eGetXSampaId3NameQuery_Id:
	    {
	            ret = m_XSampaInterface->GetID3NameFromXSampaSqlDB(searchData);
	    }
	        break;
	    case eTrackerExtractMetadataQuery_Id:
	    {
	            ret = m_MediaDataProvider->RequestTrackerMetadataExtraction(searchData);
	    }
	        break;
		// { added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
	    case eGetPhotoMetaDataInfoFromUrlQuery_Id: 
		{
				ret = m_MediaDataProvider->GetPhotoMetaDataInfoFromUrl(searchData);
	    }
		    break;
		// } added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
        // removed by dk 2013.05.03

        // { added by kihyung 2013.07.23 for ITS 0178868
        case eGetFirstImagePathByGivenFolder:
        {
            CDirTreeProvider *pTree = GetDirTreeProvider();
            if(pTree) {
                ret = pTree->GetFirstImagePathByGivenFolder(searchData);
            }
            break;
        }

        case eGetImageCountByGivenFolder:
        {
            CDirTreeProvider *pTree = GetDirTreeProvider();
            if(pTree) {
                ret = pTree->GetImageCountByGivenFolder(searchData);
            }
            break;
        }

        case eGetFirstImageAndCountByGivenFolder:
        {
            CDirTreeProvider *pTree = GetDirTreeProvider();
            if(pTree) {
                ret = pTree->GetFirstImageAndCountByGivenFolder(searchData);
            }
            break;
        }
        // } added by kihyung 2013.07.23 for ITS 0178868
        case eGetMusicNormalMetaDataQuery_Id:    // { added by sungha.choi 2013.09.06 for ITS 0188361
        {
            ret = m_MediaDataProvider->GetMusicNormalMetaData(searchData);
            break;
        }
	    default:
	        break;
    }
    if(!ret) m_ReqStateInfo->reqState = eDefaultRequest;
    return ret;
}

bool CTrackerAbstractor::RequestData(QStringList searchData,QString FolderName,ETrackerAbstarctor_Query_Ids queryId, uint reqUID){
    bool ret = false;
    if(searchData.length() <=0 || searchData.contains(QString()) ||  FolderName.isNull())
    {
        HandleError(eNullStringParameter);
        return false;
    }

    m_ReqStateInfo->ReqData_set = eRequest_NoVol_List_StringParam;
    m_ReqStateInfo->StringListParam1 = searchData;
    m_ReqStateInfo->Stringparam1 = FolderName;
    int reqState = RequestHandler(queryId,eDefaultVolume);
    if(reqState ==eDefaultRequest || reqState == eProcessRequest)
    {
        if(m_CurrentReqStateInfo)   // { added by sungha.choi 2013.08.20 for ipod coverart
        {
            if(m_CurrentReqStateInfo->currentQueryId != queryId || m_CurrentReqStateInfo->requestUID != reqUID)
            {
                delete m_CurrentReqStateInfo;
                m_CurrentReqStateInfo = NULL;
            }
        }   // } added by sungha.choi 2013.08.20 for ipod coverart
        if(m_CurrentReqStateInfo == NULL)
        {
            m_CurrentReqStateInfo = new SRequestStateInfo;
            m_CurrentReqStateInfo->requestUID = reqUID;
            m_CurrentReqStateInfo->currentQueryId = queryId;    // added by sungha.choi 2013.08.18 for blank list hotfix
        }
    }
    else if(reqState == eWaitRequest)
    {
        m_ReqStateInfo->requestUID = reqUID;
        QueueRequest();
        emit RequestState(eWaitRequest, reqUID);
        m_ReqStateInfo->initialize();
        return true;
    }
    else if(reqState == eCancelRequest)
    {
        emit RequestState(eCancelRequest, reqUID);
        return false;
    }

    if(m_QueueRequestTimer->isActive())
        m_QueueRequestTimer->stop();

    m_QueueRequestTimer->start(MAX_TIME_LIMIT);
    m_ReqStateInfo->reqState = eProcesscomplete;

    // switch case to call different queries of different data provider using query id
    switch (queryId)
    {
	    case  eGetMultipleAlbumSongsQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetMultipleAlbumSongs(searchData,FolderName);
	    }
	        break;
	    case  eGetMultipleArtistSongsQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetMultipleArtistSongs(searchData,FolderName);
	    }
	        break;
	    default:
	        break;
    }
    if(!false) m_ReqStateInfo->reqState = eDefaultRequest;
    return ret;
}

bool CTrackerAbstractor::RequestData(QStringList searchData, ETrackerAbstarctor_Query_Ids queryId, uint reqUID)
{
    bool ret = false;
    if(queryId != eGetAllMusicByConnectedVolumesQuery_Id)
    {
        if(searchData.length() <=0 || searchData.contains(QString()))
        {
            HandleError(eNullStringParameter);
            return false;
        }
    }

	m_ReqStateInfo->ReqData_set = eRequest_NoVol_singleListParam;
    m_ReqStateInfo->StringListParam1 = searchData;
    int reqState = RequestHandler(queryId,eDefaultVolume);
    if(reqState ==eDefaultRequest || reqState == eProcessRequest)
    {
        if(m_CurrentReqStateInfo)   // { added by sungha.choi 2013.08.20 for ipod coverart
        {
            if(m_CurrentReqStateInfo->currentQueryId != queryId || m_CurrentReqStateInfo->requestUID != reqUID)
            {
                delete m_CurrentReqStateInfo;
                m_CurrentReqStateInfo = NULL;
            }
        }   // } added by sungha.choi 2013.08.20 for ipod coverart
       if(m_CurrentReqStateInfo == NULL)
       {
           m_CurrentReqStateInfo = new SRequestStateInfo;
           m_CurrentReqStateInfo->requestUID = reqUID;
           m_CurrentReqStateInfo->currentQueryId = queryId;    // added by sungha.choi 2013.08.18 for blank list hotfix
       }
    }
    else if(reqState == eWaitRequest)
	{
        m_ReqStateInfo->requestUID = reqUID;
        QueueRequest();
        emit RequestState(eWaitRequest, reqUID);
        m_ReqStateInfo->initialize();
        return true;
	}
	else if(reqState == eCancelRequest)
	{
        emit RequestState(eCancelRequest, reqUID);
        return false;
	}

    if(m_QueueRequestTimer->isActive())
        m_QueueRequestTimer->stop();

    m_QueueRequestTimer->start(MAX_TIME_LIMIT);
   	m_ReqStateInfo->reqState = eProcesscomplete;

    // switch case to call different queries of different data provider using query id
    switch (queryId)
    {
	    case eGetImageThumbNail_Id:
	    {
	        ret = m_CoverArtDataprovider->RequestPhotoThumbnail(searchData);
	    }
	    	break;
	    case eGetVideoFileListThumbNail_Id:
	    {
	        ret = m_CoverArtDataprovider->RequestVideoThumbnail(searchData);
	    }
	    	break;
	    case  eGetMusicMetaDataInfoFromUrlListQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetMusicMetaDataInfoFromUrlList(searchData);
	    }
	    	break;
	    case eGetMultipleFolderSongsQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetMultipleFolderSongs(searchData);
	    }
	    	break;
	    case eGetAllMusicByConnectedVolumesQuery_Id:
	    {
	        QList<QSqlDatabase> dbConnList;
		// { Modified by Neelima, 2013.01.03 for CR13965
            QString devID = m_MediaDataProvider->getDeviceID(eIPod);

            if(devID.isNull() || devID == "Empty") //modofied by edo.lee 2013.01.31
            {
               devID.clear();
               devID = m_MediaDataProvider->getDeviceID(eIPOD_FRONT);           
            }

            if(createFirstIpodConnection(devID))
               dbConnList.append(firstIpodDB);

            devID.clear();
            devID = m_MediaDataProvider->getDeviceID(eIPOD_REAR);
     
            if(createSecondIpodConnection(devID))
                dbConnList.append(secondIpodDB);
    		// } Modified by Neelima
	        m_MediaDataProvider->SetIPodDBConnectionList(dbConnList);
	        ret = m_MediaDataProvider->GetAllMusicByConnectedVolumes(searchData);
	    }
	    	break;
	    default:
	    	break;
    }
    if(!ret) m_ReqStateInfo->reqState = eDefaultRequest;
    return ret;
}

bool CTrackerAbstractor::RequestData(QStringList Argumnet1, QStringList Argumnet2, ETrackerAbstarctor_Query_Ids queryId, uint reqUID)
{
    bool ret = false;
    if(Argumnet1.count() <= 0 /*|| Argumnet1.contains(QString())*/ || Argumnet2.count() <= 0 /*|| Argumnet2.contains(QString())*/ )
    {
        HandleError(eNullStringParameter);
        return false;
    }

    m_ReqStateInfo->ReqData_set = eRequest_NoVol_TwoListParams;
    m_ReqStateInfo->StringListParam1 = Argumnet1;
    m_ReqStateInfo->StringListParam2 = Argumnet2;
    int reqState = RequestHandler(queryId,eDefaultVolume);
    if(reqState ==eDefaultRequest || reqState == eProcessRequest)
    {
        if(m_CurrentReqStateInfo)   // { added by sungha.choi 2013.08.20 for ipod coverart
        {
            if(m_CurrentReqStateInfo->currentQueryId != queryId || m_CurrentReqStateInfo->requestUID != reqUID)
            {
                delete m_CurrentReqStateInfo;
                m_CurrentReqStateInfo = NULL;
            }
        }   // } added by sungha.choi 2013.08.20 for ipod coverart
        if(m_CurrentReqStateInfo == NULL)
        {
            m_CurrentReqStateInfo = new SRequestStateInfo;
            m_CurrentReqStateInfo->requestUID = reqUID;
            m_CurrentReqStateInfo->currentQueryId = queryId;    // added by sungha.choi 2013.08.18 for blank list hotfix
        }
    }
    else if(reqState == eWaitRequest)
    {
        m_ReqStateInfo->requestUID = reqUID;
        QueueRequest();
        emit RequestState(eWaitRequest, reqUID);
        m_ReqStateInfo->initialize();
        return true;
    }
    else if(reqState == eCancelRequest)
    {
        emit RequestState(eCancelRequest, reqUID);
        return false;
    }

    if(m_QueueRequestTimer->isActive())
        m_QueueRequestTimer->stop();

    m_QueueRequestTimer->start(MAX_TIME_LIMIT);
    m_ReqStateInfo->reqState = eProcesscomplete;

    // switch case to call different queries of different data provider using query id
    switch (queryId)
    {
	    case  eGetMusicAlbumCoverArt_Id:
	    {
	        ret = m_CoverArtDataprovider->GetMusicAlbumCoverArt(Argumnet1,Argumnet2);
	    }
	        break;
	    default:
	        break;
    }
    if(!ret) m_ReqStateInfo->reqState = eDefaultRequest;
    return ret;
}

bool CTrackerAbstractor::RequestData(QString searchData, ETrackerAbstarctor_VolumeType volumeType,
                                     ETrackerAbstarctor_Query_Ids queryId, uint reqUID)
{
    bool ret = false;
    qDebug() << "Request Data cK1 : Query ID :: " << queryId << " request UID :: " << reqUID;
    if(searchData.isNull())
    {
        HandleError(eNullStringParameter);
        return false;
    }    

    m_ReqStateInfo->ReqData_set = eRequest_Vol_StringParam;
    m_ReqStateInfo->Stringparam1 = searchData;
    int reqState = RequestHandler(queryId,volumeType);
    if(reqState ==eDefaultRequest || reqState == eProcessRequest)
    {
        if(m_CurrentReqStateInfo)   // { added by sungha.choi 2013.08.20 for ipod coverart
        {
            if(m_CurrentReqStateInfo->currentQueryId != queryId || m_CurrentReqStateInfo->requestUID != reqUID)
            {
                delete m_CurrentReqStateInfo;
                m_CurrentReqStateInfo = NULL;
            }
        }   // } added by sungha.choi 2013.08.20 for ipod coverart
        if(m_CurrentReqStateInfo == NULL)
        {
            m_CurrentReqStateInfo = new SRequestStateInfo;
            m_CurrentReqStateInfo->requestUID = reqUID;
            m_CurrentReqStateInfo->currentQueryId = queryId;    // added by sungha.choi 2013.08.18 for blank list hotfix
        }
        qDebug() << "Request Data cK2 : Query ID :: " << queryId << " request UID :: " << reqUID;
        qDebug() << "Request Data cK3 : Query ID :: " << queryId << " request UID :: " << m_CurrentReqStateInfo->requestUID;
    }
    else if(reqState == eWaitRequest)
    {
        qDebug() << "Request Data cK wait : Query ID :: " << queryId << " request UID :: " << reqUID;
		if(m_CurrentReqStateInfo)
            qDebug() << "Request Data cK wait : Query ID :: " << queryId << " request UID :: " << m_CurrentReqStateInfo->requestUID;
    	m_ReqStateInfo->requestUID = reqUID;
        QueueRequest();
        emit RequestState(eWaitRequest, reqUID);
        m_ReqStateInfo->initialize();
        return true;
    }
    else if(reqState == eCancelRequest)
    {
        emit RequestState(eCancelRequest, reqUID);
        return false;
    }

    if(m_QueueRequestTimer->isActive())
        m_QueueRequestTimer->stop();

    m_QueueRequestTimer->start(MAX_TIME_LIMIT);
    m_ReqStateInfo->reqState = eProcesscomplete;
    if(m_CurrentReqStateInfo)
        qDebug() << "Request Data cK4 : Query ID :: " << queryId << " request UID :: " << m_CurrentReqStateInfo->requestUID;
		bool normalizedData = GetGracenoteNormalizedData(); //added by Divya for Gracenote normalized data 2013.01.07

    if(volumeType==eIPod || volumeType==eIPOD_FRONT)
    {
	   // { Added by Neelima, 2013.01.03 for CR13965
       QString devID = m_MediaDataProvider->getDeviceID(volumeType);
       if(createFirstIpodConnection(devID))	
	   {   // } Added by Neelima
        switch (queryId)
        {
	        case eGetAllsongsByKeywordFromVolumeQuery_Id:
	        case eGetAllAlbumsByKeywordFromVolumeQuery_Id:
	        case eGetAllArtistsByKeywordFromVolumeQuery_Id:
	        case eGetAllGenresByKeywordFromVolumeQuery_Id:
	        case eGetAllSongsByAlbumFromVolumeQuery_Id:
	        case eGetAllSongsWithTitleIndexFromVolumeQuery_Id:
	        case eGetAllMusicGenresIndexFromVolumeQuery_Id:
	        case eGetAllMusicArtistsIndexFromVolumeQuery_Id:
	        case eGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Id:
	        case eGetAllSongsByArtistFromVolumeQuery_Id:
            case eGetAllSongsByGenreFromVolumeQuery_Id: //added by junam 2012.03.19 for ISV 76018
                 qDebug() << "in switch case eIPOD_FRONT " ;
				 //{modified by Divya for Gracenote normalized data 2013.01.07
             if(normalizedData)
                 ret = m_GracenoteDataProvider->RequestData(searchData,queryId,volumeType);
             else
                 ret = m_MediaDataProvider->GetSearchdataByKeywordFromVolume(searchData,queryId,firstIpodDB);
	             break;
        case eGetAllSongsbyPlaylistQuery_Id: //Not implemented in Gracenote data provider
            {
                  ret = m_MediaDataProvider->GetSearchdataByKeywordFromVolume(searchData,queryId,firstIpodDB);
             }
			 //}modified by Divya for Gracenote normalized data 2013.01.07
             break;
	        case eGetAllAudiobookChapterNamesQuery_Id:
	        case eGetAllPodcastsEpisodeNamesQuery_Id:
	        case eGetAlliTunesUEpisodeNamesQuery_Id:
                 qDebug() << "in switch case eIPOD_FRONT " ;
	              ret = m_IpodDataProvider->GetSearchIPODdataByKeywordFromVolume(searchData,queryId,firstIpodDB);
	              break;
	         case eGetMusicPlaylistByKeywordQuery_Id:
	              ret =  m_MediaDataProvider->GetMusicPlaylistByKeyword(volumeType,searchData);
	              break;
	        case eGetSongIndexFromVolumeQuery_Id:
	        case eGetAlbumIndexFromVolumeQuery_Id:
	        case eGetArtistIndexFromVolumeQuery_Id:
	        {
			//{modified by Divya for Gracenote normalized data 2013.01.07
                if(normalizedData)
                    ret = m_GracenoteDataProvider->RequestData(searchData,queryId,volumeType);
                else
	        		ret = m_MediaDataProvider->GetIndexDataFromVolumeForIPOD(searchData,queryId,firstIpodDB);
	        
			//}modified by Divya for Gracenote normalized data 2013.01.07
			}
	            break;
	        default:
	            break;
        }
	   } // { Added by Neelima, 2013.01.03 for CR13965
	   else
	   ret = false;
       // } Added by Neelima
    }

   else if(volumeType==eIPOD_REAR)
   {
        qDebug() << "in switch case" ;
		 // { Added by Neelima, 2013.01.03 for CR13965
         QString devID = m_MediaDataProvider->getDeviceID(volumeType);

         if(createSecondIpodConnection(devID))
		 { // } Added by Neelima
        switch (queryId)
        {
	        case eGetAllsongsByKeywordFromVolumeQuery_Id:
	        case eGetAllAlbumsByKeywordFromVolumeQuery_Id:
	        case eGetAllArtistsByKeywordFromVolumeQuery_Id:
	        case eGetAllGenresByKeywordFromVolumeQuery_Id:
	        case eGetAllSongsByAlbumFromVolumeQuery_Id:
	        case eGetAllSongsWithTitleIndexFromVolumeQuery_Id:
	        case eGetAllMusicGenresIndexFromVolumeQuery_Id:
	        case eGetAllMusicArtistsIndexFromVolumeQuery_Id:
	        case eGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Id:
	        case eGetAllSongsByArtistFromVolumeQuery_Id:
            case eGetAllSongsByGenreFromVolumeQuery_Id: //added by junam 2012.03.19 for ISV 76018
			//{modified by Divya for Gracenote normalized data 2013.01.07
            if(normalizedData)
                ret = m_GracenoteDataProvider->RequestData(searchData,queryId,volumeType);
            else
	             ret = m_MediaDataProvider->GetSearchdataByKeywordFromVolume(searchData,queryId,secondIpodDB);
	             break;
         case  eGetAllSongsbyPlaylistQuery_Id:
             {
                 ret = m_MediaDataProvider->GetSearchdataByKeywordFromVolume(searchData,queryId,secondIpodDB);
             }
			 //}modified by Divya for Gracenote normalized data 2013.01.07
             break;
	        case eGetAllAudiobookChapterNamesQuery_Id:
	        case eGetAllPodcastsEpisodeNamesQuery_Id:
	        case eGetAlliTunesUEpisodeNamesQuery_Id:
                 qDebug() << "in switch case eIPOD_REAR" ;
	             ret = m_IpodDataProvider->GetSearchIPODdataByKeywordFromVolume(searchData,queryId,secondIpodDB);
	             break;
	         case eGetMusicPlaylistByKeywordQuery_Id:
	             ret =  m_MediaDataProvider->GetMusicPlaylistByKeyword(volumeType,searchData);
	             break;
	        case eGetSongIndexFromVolumeQuery_Id:
	        case eGetAlbumIndexFromVolumeQuery_Id:
	        case eGetArtistIndexFromVolumeQuery_Id:	  
			//{modified by Divya for Gracenote normalized data 2013.01.07      
            {
                  if(normalizedData)
                      ret = m_GracenoteDataProvider->RequestData(searchData,queryId,volumeType);
                  else
                ret = m_MediaDataProvider->GetIndexDataFromVolumeForIPOD(searchData,queryId,secondIpodDB);
            }
			//}modified by Divya for Gracenote normalized data 2013.01.07
 		         break;
	        default:
	            break;
         }
		 } // { Added by Divya, 2013.01.03 for CR13965
		 else
		 ret = false;
		 // } Added by Neelima
    }
	else
    {
        // switch case to call different queries of different data provider using query id
	    switch (queryId)
	    {
		//{modified by Divya for Gracenote normalized data 2013.01.07
		    case eGetAllsongsByKeywordFromVolumeQuery_Id:
                if(normalizedData)
                    ret = m_GracenoteDataProvider->RequestData(searchData,queryId,volumeType);
                else
                    ret = m_MediaDataProvider->GetAllSongsByKeywordFromVolume(searchData,volumeType);
		        break;
		    case eGetAllAlbumsByKeywordFromVolumeQuery_Id:
                if(normalizedData)
                    ret = m_GracenoteDataProvider->RequestData(searchData,queryId,volumeType);
                else
                    ret = m_MediaDataProvider->GetAllAlbumsByKeywordFromVolume(searchData,volumeType);
		        break;
		    case eGetAllArtistsByKeywordFromVolumeQuery_Id:
                if(normalizedData)
                    ret = m_GracenoteDataProvider->RequestData(searchData,queryId,volumeType);
                else
                    ret = m_MediaDataProvider->GetAllArtistsByKeywordFromVolume(searchData,volumeType);
		        break;
		    case eGetAllGenresByKeywordFromVolumeQuery_Id:
                if(normalizedData)
                    ret = m_GracenoteDataProvider->RequestData(searchData,queryId,volumeType);
                else
                    ret = m_MediaDataProvider->GetAllGenresByKeywordFromVolume(searchData,volumeType);
		        break;

		    case eGetAllSongsByAlbumFromVolumeQuery_Id:
                if(normalizedData)
                    ret = m_GracenoteDataProvider->RequestData(searchData,queryId,volumeType);
                else
                    ret = m_MediaDataProvider->GetAllSongsByAlbumFromVolume(searchData,volumeType);
		        break;
		    case eGetAllSongsByArtistFromVolumeQuery_Id:		                    
                if(normalizedData)
                    ret = m_GracenoteDataProvider->RequestData(searchData,queryId,volumeType);
                else
                   ret = m_MediaDataProvider->GetAllSongsByArtistFromVolume(searchData, volumeType);                
		    	break;
			case eGetAllSongsByGenreFromVolumeQuery_Id:
				//if(normalizedData)
                //    ret = m_GracenoteDataProvider->RequestData(searchData,queryId,volumeType);
                //else
                   ret = m_MediaDataProvider->GetAllSongsByGenreFromVolume(searchData, volumeType);                
		    	break;
		    case eGetAllPodcastsEpisodeNamesQuery_Id:
		        ret = m_IpodDataProvider->GetAllPodcastsEpisodeNames(searchData,volumeType);
		        break;

		    case eGetAllAudiobookChapterNamesQuery_Id:
		        ret = m_IpodDataProvider->GetAllAudiobookChapterNames(searchData,volumeType);
		        break;
		    case  eGetAllSongsbyPlaylistQuery_Id:
		        ret =  m_MediaDataProvider->GetAllSongsbyPlaylist(searchData,volumeType);
		        break;
		    case eGetMusicPlaylistByKeywordQuery_Id:
		        ret =  m_MediaDataProvider->GetMusicPlaylistByKeyword(volumeType,searchData);
		        break;
		    case  eGetAllMusicByKeywordFromVolumeQuery_Id:		    
		        ret = m_MediaDataProvider->GetAllMusicByKeywordFromVolume(searchData, volumeType);		    
			    break;		   
		    case eGetAllSongsWithTitleIndexFromVolumeQuery_Id:
		    {
                    if(normalizedData)
                        ret = m_GracenoteDataProvider->RequestData(searchData,queryId,volumeType);
                    else
                        ret = m_MediaDataProvider->GetAllSongsWithTitleIndexFromVolume(searchData, volumeType);
		    }
		    	break;
		    case eGetAllMusicArtistsIndexFromVolumeQuery_Id:		    
                {
                    if(normalizedData)
                        ret = m_GracenoteDataProvider->RequestData(searchData,queryId,volumeType);
                    else
                        ret = m_MediaDataProvider->GetAllMusicArtistsIndexFromVolume(searchData, volumeType);
                }
		    	break;
		    case eGetAllMusicGenresIndexFromVolumeQuery_Id:		    
                {
                    if(normalizedData)
                        ret = m_GracenoteDataProvider->RequestData(searchData,queryId,volumeType);
                    else
                        ret = m_MediaDataProvider->GetAllMusicGenresIndexFromVolume(searchData, volumeType);
                }
		    	break;
		    case eGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Id:		    
                {
                    if(normalizedData)
                        ret = m_GracenoteDataProvider->RequestData(searchData,queryId,volumeType);
                    else
                        ret = m_MediaDataProvider->GetAllAlbumsWithCoverArtIndexFromVolume(searchData, volumeType);
                }
		        break;

		    case eGetSongIndexFromVolumeQuery_Id:		    
                {
                    if(normalizedData)
                        ret = m_GracenoteDataProvider->RequestData(searchData,queryId,volumeType);
                    else
                        ret = m_MediaDataProvider->GetIndexBySongFromVolume(searchData, volumeType);
                }
		        break;
		    case eGetAlbumIndexFromVolumeQuery_Id:		    
                {
                    if(normalizedData)
                        ret = m_GracenoteDataProvider->RequestData(searchData,queryId,volumeType);
                    else
                        ret = m_MediaDataProvider->GetIndexByAlbumFromVolume(searchData, volumeType);
                }
		        break;
		    case eGetArtistIndexFromVolumeQuery_Id:		    
                {
                    if(normalizedData)
                        ret = m_GracenoteDataProvider->RequestData(searchData,queryId,volumeType);
                    else
                        ret = m_MediaDataProvider->GetIndexByArtistFromVolume(searchData, volumeType);
                }
				//}modified by Divya for Gracenote normalized data 2013.01.07
		        break;
                    // { added by sungha.choi 2013.07.23
                    case eGetSongIndexWithTitleCountFromVolumeQuery_Id:
                        {
                            ret = m_MediaDataProvider->GetIndexWithCountBySongFromVolume(searchData, volumeType, getMinerStatus(volumeType));
                        }
                        break;
                    case eGetAlbumIndexWithAlbumCountFromVolumeQuery_Id:
                        {
                            ret = m_MediaDataProvider->GetIndexWithCountByAlbumFromVolume(searchData, volumeType, getMinerStatus(volumeType));
                        }
                        break;
                    case eGetArtistIndexWithArtistCountFromVolumeQuery_Id:
                        {
                            ret = m_MediaDataProvider->GetIndexWithCountByArtistFromVolume(searchData, volumeType, getMinerStatus(volumeType));
                        }
                        break;
                    // } added by sungha.choi 2013.07.23
		    default:
		        break;
    	}
 	}
    if(!ret) m_ReqStateInfo->reqState = eDefaultRequest;
    return ret;
}
/******************************************************************************

// Function Name                                            Parameters

// CTrackerAbstractor::RequestData                 bool (OUT), QString (IN) - data to search
                                                                    QString (IN) - data to search (if require),
                                                                    ETrackerAbstarctor_Query_Ids (IN)- respective query Id

//

// Explanation

// This method calls respective query APIs of Providers using the query id

// It connects signal/slot for the query completion

//

// History

// 22-Nov-2011      Chetan Nanda                created for handing contact and message related Queries
*****************************************************************************/
bool CTrackerAbstractor::RequestData(ETrackerAbstarctor_Query_Ids queryId, uint reqUID)
{
    bool ret = false;

    m_ReqStateInfo->ReqData_set = eRequest_NoVol_NoParam;
    int reqState;
    reqState = RequestHandler(queryId,eDefaultVolume);
    if(reqState ==eDefaultRequest || reqState == eProcessRequest)
    {
        if(m_CurrentReqStateInfo)   // { added by sungha.choi 2013.08.20 for ipod coverart
        {
            if(m_CurrentReqStateInfo->currentQueryId != queryId || m_CurrentReqStateInfo->requestUID != reqUID)
            {
                delete m_CurrentReqStateInfo;
                m_CurrentReqStateInfo = NULL;
            }
        }   // } added by sungha.choi 2013.08.20 for ipod coverart
        if(m_CurrentReqStateInfo == NULL)
        {
            m_CurrentReqStateInfo = new SRequestStateInfo;
            m_CurrentReqStateInfo->requestUID = reqUID;
            m_CurrentReqStateInfo->currentQueryId = queryId;    // added by sungha.choi 2013.08.18 for blank list hotfix
        }
    }
    else if(reqState == eWaitRequest)
    {
        m_ReqStateInfo->requestUID = reqUID;
        QueueRequest();
        emit RequestState(eWaitRequest, reqUID);
        m_ReqStateInfo->initialize();
        return true;
    }
    else if(reqState == eCancelRequest)
    {
        emit RequestState(eCancelRequest, reqUID);
        return false;
    }

    if(m_QueueRequestTimer->isActive())
        m_QueueRequestTimer->stop();

    m_QueueRequestTimer->start(MAX_TIME_LIMIT);
    m_ReqStateInfo->reqState = eProcesscomplete;

    switch(queryId)
    {
    case eRetrieveAllXSampaData_Id:
    {
        ret = m_XSampaInterface->RetrieveAllPhoneticData();
    }
        break;
    default:
        break;
    }
    if(!ret) m_ReqStateInfo->reqState = eDefaultRequest;
    return ret;
}

bool CTrackerAbstractor::RequestData(QString searchData1, QString searchData2, ETrackerAbstarctor_Query_Ids queryId, uint reqUID)
{
    bool ret = false;

    if(searchData1.isNull() || searchData2.isNull())
    {
        HandleError(eNullStringParameter);
        return false;
    }

    m_ReqStateInfo->ReqData_set = eRequest_NoVol_TwoStringParams;
    m_ReqStateInfo->Stringparam1 = searchData1;
    m_ReqStateInfo->Stringparam2 = searchData2;
    int reqState = RequestHandler(queryId,eDefaultVolume);
    if(reqState ==eDefaultRequest || reqState == eProcessRequest)
    {
        if(m_CurrentReqStateInfo)   // { added by sungha.choi 2013.08.20 for ipod coverart
        {
            if(m_CurrentReqStateInfo->currentQueryId != queryId || m_CurrentReqStateInfo->requestUID != reqUID)
            {
                delete m_CurrentReqStateInfo;
                m_CurrentReqStateInfo = NULL;
            }
        }   // } added by sungha.choi 2013.08.20 for ipod coverart
        if(m_CurrentReqStateInfo == NULL)
        {
            m_CurrentReqStateInfo = new SRequestStateInfo;
            m_CurrentReqStateInfo->requestUID = reqUID;
            m_CurrentReqStateInfo->currentQueryId = queryId;    // added by sungha.choi 2013.08.18 for blank list hotfix
        }
    }
    else if(reqState == eWaitRequest)
    {
        m_ReqStateInfo->requestUID = reqUID;
        QueueRequest();
        emit RequestState(eWaitRequest, reqUID);
        m_ReqStateInfo->initialize();
        return true;
    }
    else if(reqState == eCancelRequest)
    {
        emit RequestState(eCancelRequest, reqUID);
        return false;
    }

    if(m_QueueRequestTimer->isActive())
        m_QueueRequestTimer->stop();

    m_QueueRequestTimer->start(MAX_TIME_LIMIT);
    m_ReqStateInfo->reqState = eProcesscomplete;

    switch(queryId)
    {
	    case eGetMusicMetaDataFromCDQuery_Id:
	    {
	        ret = m_MediaDataProvider->GetMusicMetaDataInfoFromCD(searchData1, searchData2);
	    }
	        break;
	    case eIsCoverArtExists_Id:
	    {
	        ret = m_CoverArtDataprovider->IsCoverArtExists(searchData1,searchData2);
			m_ReqStateInfo->reqState = eDefaultRequest;
	    }
	        break;
	    case eRetrieveXSampaData_Id:
	    {
	        ret = m_XSampaInterface->RetrievePhoneticData(searchData1, searchData2);
	    }
	        break;
	    case eCheckExistenceOfXSampaData_Id:
	    {
	        ret = m_XSampaInterface->CheckIfPhoneticDataExists(searchData1, searchData2);
			m_ReqStateInfo->reqState = eDefaultRequest;
	    }
	        break;
		default:
			break;
    }
    if(!ret) m_ReqStateInfo->reqState = eDefaultRequest;

    return ret;
}


/*******************************************************************************
 * For contacts DB
 ******************************************************************************/

// added by dk 2013.05.03
bool CTrackerAbstractor::RequestPBData(const QString& searchData, const ETrackerAbstarctor_Query_Ids queryId, const uint reqUID)
{
    bool ret = false;
    m_isPBData = true;  // added by sungha.choi 2013.08.25 (PB do not use queue)
	
    if(searchData.isNull())
    {
        HandleError(eNullStringParameter);
        return false;
    }

    switch (queryId)
    {
/////BEGIN - count query
	    case eGetAllPhoneContactCountQuery_Id:
			ret = m_ContactsDataProvider->GetAllPhoneBookDataCountBySelectionFromSQLiteDB(searchData, DB_PHONE_BOOK);
			break;
            
		case eGetAllRecentCallCountQuery_Id:
			ret = m_ContactsDataProvider->GetAllPhoneBookDataCountBySelectionFromSQLiteDB(searchData, DB_RECENT_CALLS);
			break;
            
		case eGetAllReceivedCallCountQuery_Id:
			ret = m_ContactsDataProvider->GetAllPhoneBookDataCountBySelectionFromSQLiteDB(searchData, DB_RECEIVED_CALLS);
			break;
            
		case eGetAllMissedCallCountQuery_Id:
			ret = m_ContactsDataProvider->GetAllPhoneBookDataCountBySelectionFromSQLiteDB(searchData, DB_MISSED_CALLS);
			break;	
            
		case eGetAllCallHistoryCountQuery_Id:
			ret = m_ContactsDataProvider->GetAllPhoneBookDataCountBySelectionFromSQLiteDB(searchData, DB_CALL_HISTORY);
			break;
            
	    case eGetAllFavoriteContactCountQuery_Id:
			ret = m_ContactsDataProvider->GetAllPhoneBookDataCountBySelectionFromSQLiteDB(searchData, DB_FAVORITE_PHONE_BOOK);
			break;
/////END - count query
        
		case eGetAllPhoneContactsForForeignQuery_Id:        // BT - contacts
			ret = m_ContactsDataProvider->GetAllPhoneBookDataBySelectionForForeignFromSQLiteDB(searchData, DB_PHONE_BOOK);
			break;
            
		case eGetAllPhoneContactsForDomesticQuery_Id:       // BT - contacts
			ret = m_ContactsDataProvider->GetAllPhoneBookDataBySelectionForDomesticFromSQLiteDB(searchData, DB_PHONE_BOOK);
			break;
            
		case eGetAllFavoriteContactsForForeignQuery_Id:     // BT - favorites
			ret = m_ContactsDataProvider->GetAllPhoneBookDataBySelectionForForeignFromSQLiteDB(searchData, DB_FAVORITE_PHONE_BOOK);
			break;
/////ORDER_BY_Idx
        case eGetAllFavoriteContactsForDomesticQuery_Id:    // BT - favorites
			ret = m_ContactsDataProvider->GetAllPhoneBookDataBySelectionForDomesticFromSQLiteDB(searchData, DB_FAVORITE_PHONE_BOOK);
			break;
/////ORDER_BY_Idx

/////SUPPORT_MIDDLE_NAME


    case eMidGetAllPhoneContactsForForeignQuery_Id:
        ret = m_ContactsDataProvider->GetAllPhoneBookDataBySelectionForForeignFromSQLiteDB(searchData, DB_MID_PHONEBOOK);
        break;

    case eMidGetAllPhoneContactsForDomesticQuery_Id:
        ret = m_ContactsDataProvider->GetAllPhoneBookDataBySelectionForDomesticFromSQLiteDB(searchData, DB_MID_PHONEBOOK);
        break;

    case eMidGetAllFavoriteContactsForForeignQuery_Id:
            ret = m_ContactsDataProvider->GetAllPhoneBookDataBySelectionForForeignFromSQLiteDB(searchData, DB_MID_FAVORITE);
            break;

/////ORDER_BY_Idx
    case eMidGetAllFavoriteContactsForDomesticQuery_Id:
            ret = m_ContactsDataProvider->GetAllPhoneBookDataBySelectionForDomesticFromSQLiteDB(searchData, DB_MID_FAVORITE);
            break;
/////ORDER_BY_Idx

        case ePinyinGetAllSelectedPhoneContactsForDomesticQuery_id:
            ret = m_ContactsDataProvider->GetAllPhoneBookDataBySelectionForDomesticFromSQLiteDB(searchData, DB_PINYIN_PHONEBOOK);
            break;
    /* FEATURE_ORDER_BY_PINYIN */

/////SUPPORT_MIDDLE_NAME
            
		case eGetPhoneBookHashValueByBDAddressQuery_Id:
			ret = m_ContactsDataProvider->GetPhoneBookHashValueByBDAddressFromSQLiteDB(searchData);
			break;
            
	    default:
			LOG_TRACE << __FUNCTION__ <<" Need To Check !! queryID = " << queryId << LOG_ENDL;
			break;
    }

    return ret;
}


// added by dk 2013.05.03
bool CTrackerAbstractor::RequestPBData(const QString& searchData1, const QString& searchData2, const ETrackerAbstarctor_Query_Ids queryId, const uint reqUID)
{
    bool ret = false;
    m_isPBData = true;  // added by sungha.choi 2013.08.25 (PB do not use queue)

    if(searchData1.isNull() || searchData2.isNull())
    {
        HandleError(eNullStringParameter);
        return false;
    }

    switch(queryId)
    {
        case eGetContactDetailsByNameQuery_Id:              // BT - NOT_USED
    		ret = m_ContactsDataProvider->GetContactDetailsByNameFromSQLiteDB(searchData1, searchData2);
    		break;
            
        case eGetAllContactsByKeywordForBDAdressQuery_Id:   // BT - NOT USED
    		ret = m_ContactsDataProvider->GetAllContactsByKeywordForBDAddressFromSQLiteDB(searchData1, searchData2);
    		break;
            
        case eGetContactByNumberForForeignQuery_Id:         // BT - search
            //MERGED ret = m_ContactsDataProvider->GetContactByNumberForForeignFromSQLiteDB(searchData1, searchData2);
            ret = m_ContactsDataProvider->GetContactByNumberFromSQLiteDB(CContactsDataProvider::FOREIGN, searchData1, searchData2);
            break;     
            
        case eGetContactByNumberForDomesticQuery_Id:        // BT - search
            //MERGED ret = m_ContactsDataProvider->GetContactByNumberForDomesticFromSQLiteDB(searchData1, searchData2);
            ret = m_ContactsDataProvider->GetContactByNumberFromSQLiteDB(CContactsDataProvider::DOMESTIC, searchData1, searchData2);
            break;

//////SUPPORT_MIDDLE_NAME
        // TODO: case eGetContactByNumberForDomesticQuery_Id:
        case eMidGetContactByNumberForDomesticQuery_Id:        // BT - search
            // TODO: ret = m_ContactsDataProvider->GetContactByNumberForDomesticFromSQLiteDB(searchData1, searchData2);
            ret = m_ContactsDataProvider->GetContactByNumberFromSQLiteDB(CContactsDataProvider::DOMESTIC_MIDDLE, searchData1, searchData2);
            break;
//////SUPPORT_MIDDLE_NAME

        case eGetContactByNumberForDomesticCallHistoryNameQuery_Id:     // BT_- update recents name
            //MERGED ret = m_ContactsDataProvider->GetContactByNumberForDomesticCallHistoryNameFromSQLiteDB(searchData1, searchData2);
            ret = m_ContactsDataProvider->GetContactByNumberCallHistoryNameFromSQLiteDB(CContactsDataProvider::DOMESTIC
                                                                                        , searchData1, searchData2);
    		break;
            
        case eGetContactByNumberForForeignCallHistoryNameQuery_Id:      // BT - update recents name
            //MERGED ret = m_ContactsDataProvider->GetContactByNumberForForeignCallHistoryNameFromSQLiteDB(searchData1, searchData2);
            ret = m_ContactsDataProvider->GetContactByNumberCallHistoryNameFromSQLiteDB(CContactsDataProvider::FOREIGN
                                                                                        , searchData1, searchData2);
            break;

/////SUPPORT_MIDDLE_NAME
        // TODO: case eGetContactByNumberForDomesticCallHistoryNameQuery_Id:
        case eMidGetContactByNumberForDomesticCallHistoryNameQuery_Id:  // BT - update recents name
            ret = m_ContactsDataProvider->GetContactByNumberCallHistoryNameFromSQLiteDB(CContactsDataProvider::DOMESTIC_MIDDLE
                                                                                        , searchData1, searchData2);
            break;
/////SUPPORT_MIDDLE_NAME

        default:
    		LOG_TRACE << __FUNCTION__ <<" Need To Check !! queryID = " << queryId << LOG_ENDL;
    		break;
    }
	
    return ret;
}


bool CTrackerAbstractor::RequestPBData(const QString& bd_address, const BT_PBAP_DB_SORTING sortBy, const ETrackerAbstarctor_Query_Ids queryId, const uint reqUID)
{
    bool ret = false;
    m_isPBData = true;  // added by sungha.choi 2013.08.25 (PB do not use queue)

    if(bd_address.isNull())
    {
        HandleError(eNullStringParameter);
        return false;
    }

    switch(queryId)
    {
    	case eGetAllPhoneContactsQuery_Id:  // BT - NOT_USED
            ret = m_ContactsDataProvider->GetAllPhoneBookDataBySelectionFromSQLiteDB(bd_address, sortBy, DB_PHONE_BOOK);
    		break;
            
    	case eGetAllRecentCallQuery_Id:         // BT - recents(name fields not used)
            ret = m_ContactsDataProvider->GetAllPhoneBookDataBySelectionFromSQLiteDB(bd_address, sortBy, DB_RECENT_CALLS);
    		break;
            
    	case eGetAllReceivedCallQuery_Id:       // BT - recents(name fields not used)
            ret = m_ContactsDataProvider->GetAllPhoneBookDataBySelectionFromSQLiteDB(bd_address, sortBy, DB_RECEIVED_CALLS);
    		break;
            
    	case eGetAllMissedCallQuery_Id:         // BT - recents(name fields not used)
            ret = m_ContactsDataProvider->GetAllPhoneBookDataBySelectionFromSQLiteDB(bd_address, sortBy, DB_MISSED_CALLS);
    		break;
            
    	case eGetAllCallHistoryQuery_Id:        // BT - recents(name fields not used)
            ret = m_ContactsDataProvider->GetAllPhoneBookDataBySelectionFromSQLiteDB(bd_address, sortBy, DB_CALL_HISTORY);
    		break;
            
    	case eGetAllFavoriteContactsQuery_Id:   // BT - NOT USED
            ret = m_ContactsDataProvider->GetAllPhoneBookDataBySelectionFromSQLiteDB(bd_address, sortBy, DB_FAVORITE_PHONE_BOOK);
    		break;
            
        default:
    		LOG_TRACE << __FUNCTION__ <<" Need To Check !! queryID = " << queryId << LOG_ENDL;
            break;
    }
	
    return ret;
}


// added by dk 2013.05.03
bool CTrackerAbstractor::RequestPBData(const QString& bd_address
                                        , const QString& firstName, const QString& lastName
                                        , const QString& phoneNumber
                                        , const ETrackerAbstarctor_Query_Ids queryId, const uint reqUID/* = 0 */)
{
    bool ret = false;
    m_isPBData = true;  // added by sungha.choi 2013.08.25 (PB do not use queue)

    if(bd_address.isNull() || firstName.isNull() || lastName.isNull() || phoneNumber.isNull() )
    {
        HandleError(eNullStringParameter);
        return false;
    }

    switch(queryId)
    {
        case eGetFavoriteContactByNameAndNumberQuery_Id:
            ret = m_ContactsDataProvider->GetFavoriteContactByNameAndNumberFromSQLiteDB(bd_address, firstName, lastName, phoneNumber);
            break;
            
        case eGetContactByCallHistoryInfoQuery_Id:          // BT - search
            ret = m_ContactsDataProvider->GetContactByCallHistoryInfoFromSQLiteDB(bd_address, firstName, lastName, phoneNumber);
            break;
            
        default:
    		LOG_TRACE << __FUNCTION__ <<" Need To Check !! queryID = " << queryId << LOG_ENDL;
            break;
    }
	
    return ret;
}


/////SUPPORT_MIDDLE_NAME
bool CTrackerAbstractor::RequestPBData(const QString& bd_address
                                        , const QString& firstName, const QString& lastName, const QString& middleName
                                        , const QString& phoneNumber
                                        , const ETrackerAbstarctor_Query_Ids queryId, const uint reqUID/* = 0 */)
{
    bool ret = false;
    m_isPBData = true;  // added by sungha.choi 2013.08.25 (PB do not use queue)

    if(bd_address.isNull() || firstName.isNull() || lastName.isNull() || middleName.isNull() || phoneNumber.isNull() )
    {
        HandleError(eNullStringParameter);
        return false;
    }

    switch(queryId)
    {

        case eMidGetFavoriteContactByNameAndNumberQuery_Id:
            ret = m_ContactsDataProvider->MidGetFavoriteContactByNameAndNumberFromSQLiteDB(bd_address
                                                                                           , firstName, lastName, middleName
                                                                                           , phoneNumber);
            break;
        case eMidGetContactByCallHistoryInfoQuery_Id:
            ret = m_ContactsDataProvider->MidGetContactByCallHistoryInfoFromSQLiteDB(bd_address
                                                                                , firstName, lastName, middleName
                                                                                , phoneNumber);
            break;
            
        default:
    		LOG_TRACE << __FUNCTION__ <<" Need To Check !! queryID = " << queryId << LOG_ENDL;
            break;
    }
	
    return ret;
}
//////SUPPORT_MIDDLE_NAME


// added by dk 2013.05.03
bool CTrackerAbstractor::RequestPBData(const QString& bd_address, const QString& phoneNumber, const int section, const ETrackerAbstarctor_Query_Ids queryId, const uint reqUID)
{
    bool ret = false;
    m_isPBData = true;  // added by sungha.choi 2013.08.25 (PB do not use queue)

    if(bd_address.isNull() || phoneNumber.isNull())
    {
        HandleError(eNullStringParameter);
        return false;
    }

    switch(queryId)
    {
    case eGetContactByNumberQuery_Id_KR:
        ret = m_ContactsDataProvider->GetContactByNumberFromSQLiteDB_KR(bd_address, phoneNumber, section);
        break;
    case eGetContactByNumberQuery_Id_NA:
        ret = m_ContactsDataProvider->GetContactByNumberFromSQLiteDB_NA(bd_address, phoneNumber, section);
        break;
    case eGetContactByNumberQuery_Id_CN:
        ret = m_ContactsDataProvider->GetContactByNumberFromSQLiteDB_CN(bd_address, phoneNumber, section);
        break;
    case eGetContactByNumberForCallHistoryNameQuery_Id_KR:
        ret = m_ContactsDataProvider->GetContactByNumberForCallHistoryNameFromSQLiteDB_KR(bd_address, phoneNumber, section);
        break;
    case eGetContactByNumberForCallHistoryNameQuery_Id_NA:
        ret = m_ContactsDataProvider->GetContactByNumberForCallHistoryNameFromSQLiteDB_NA(bd_address, phoneNumber, section);
        break;
    case eGetContactByNumberForCallHistoryNameQuery_Id_CN:
        ret = m_ContactsDataProvider->GetContactByNumberForCallHistoryNameFromSQLiteDB_CN(bd_address, phoneNumber, section);
        break;
    default:
        LOG_TRACE << __FUNCTION__ <<" Need To Check !! queryID = " << queryId << LOG_ENDL;
        break;
    }
	
    return ret;
}


// added by dk 2013.05.03
bool CTrackerAbstractor::RequestPBData(const QString& bd_address
                                    , const QString& firstName, const QString& lastName
                                    , const QString& phoneNumber
                                    , const int section, const ETrackerAbstarctor_Query_Ids queryId, const uint reqUID/* = 0 */)
{
    bool ret = false;
    m_isPBData = true;  // added by sungha.choi 2013.08.25 (PB do not use queue)

    if(bd_address.isNull() || firstName.isNull() || lastName.isNull() || phoneNumber.isNull() )
    {
        HandleError(eNullStringParameter);
        return false;
    }

    switch(queryId)
    {
        case eGetContactByCallHistoryInfoQuery_Id_KR_NA:    // BT - search
            ret = m_ContactsDataProvider->GetContactByCallHistoryInfoFromSQLiteDB_KR_NA(bd_address, firstName, lastName, phoneNumber, section);
            break;

        default:
            LOG_TRACE << __FUNCTION__ <<" Need To Check !! queryID = " << queryId << LOG_ENDL;
            break;
    }
	
    return ret;
}



/******************************************************************************

// Function Name                                            Parameters

// CTrackerAbstractor::UpdateData   bool (OUT), void*, ETrackerAbstarctor_Query_Ids (IN)

//

// Explanation

// This method Update the data to the database

//

// History

// 15-Nov-2011      Pawan Gupta
*****************************************************************************/
// added by dk 2013.05.03
bool CTrackerAbstractor::UpdatePBData(void* data, const ETrackerAbstarctor_Query_Ids queryId)
{
    bool ret = false;
    m_isPBData = true;  // added by sungha.choi 2013.08.25 (PB do not use queue)
	
    switch(queryId)
    {
	    case eInsertContactsToDBaseQuery_Id:
	    {
			QList<BT_PBAP_DB_CONTACT>* listt = (QList<BT_PBAP_DB_CONTACT>*) data;
			ret = m_DataUpdater->UpdateSqlContactData(*listt);
            break;
		}

		case eRemovePhoneBookDataBySelectionQuery_Id:
		{
			SRemoveBTContact *list = (SRemoveBTContact*) data;
			ret = m_DataUpdater->RemovePhoneBookDataBySelectionFromSQLiteDB(list->bd_address, list->dbType);
            break;
		}
        
		case eRemoveContactDataQuery_Id:
		{
			SRemoveBTContact *list = (SRemoveBTContact*) data;
			ret = m_DataUpdater->RemoveContactDataByNameFromSQLiteDB(list->bd_address, list->first_name, list->memoryType);
            break;
		}
            
		case eRemoveContactDataByIndexQuery_Id:
		{
			SRemoveBTContact* listt = (SRemoveBTContact*) data;
			ret = m_DataUpdater->RemoveContactDataByIndexFromSQLiteDB(listt->bd_address, listt->index, listt->memoryType);
            break;
		}

	    default:
	        break;
    }
	
    return ret;
}

bool CTrackerAbstractor::UpdateData(void* data, ETrackerAbstarctor_Query_Ids queryId)
{
    //qDebug() "in CTrackerAbstractor::UpdateData" ;
    bool ret=false;
    switch(queryId)
    {
	case eInsertIPodMusicTracksToDBaseQuery_Id:
    {
        SAppleInfoList* listt;
        listt = (SAppleInfoList*) data;
        ret = m_DataUpdater->UpdateIPodMusicTrackData(*listt);
    }
		break;
    case eInsertIPodAudioBookToDBaseQuery_Id:
    case eInsertIPodPodcastToDBaseQuery_Id:
    case eInsertIPodMusicPlaylistToDBaseQuery_Id:
    {
        SAppleHierarchyInfoList* listt;
        listt = (SAppleHierarchyInfoList*) data;
        ret = m_DataUpdater->UpdateIPodTypeData(*listt, queryId);
    }
		break;
    case eRemoveIPodMusicTrackDataQuery_Id:
    {
        SIPodTrackRemoveStruct* listt;
        listt = (SIPodTrackRemoveStruct*) data;
        ret = m_DataUpdater->RemoveIPodMusicTrackData(*listt);
    }
		break;
    case eRemoveIPodPodcastDataQuery_Id:
    {
        SIPodTypeRemoveInfo* listt;
        listt = (SIPodTypeRemoveInfo*) data;
        ret = m_DataUpdater->RemoveIPodPodcastData(*listt);
    }
		break;        
    case eRemoveIPodAudioBookDataQuery_Id:
    {
        SIPodTypeRemoveInfo* listt;
        listt = (SIPodTypeRemoveInfo*) data;
        ret = m_DataUpdater->RemoveIPodAudioBookData(*listt);
    }
		break;
    case eRemoveIPodPlaylistDataQuery_Id:
    {
        SIPodTypeRemoveInfo* listt;
        listt = (SIPodTypeRemoveInfo*) data;
        ret = m_DataUpdater->RemoveIPodPlaylistData(*listt);
    }
		break;
    case eRemoveAllIPodInfoQuery_Id:
    {
        QString* listt;
        listt = (QString*) data;
        ret = m_DataUpdater->RemoveAllIPodInfo(*listt);
    }
		break;
    case eInsertAlbumCoverArtDataQuery_Id:
    {

        SCoverartMusicData* listt;
        listt = (SCoverartMusicData*) data;
        ret = m_DataUpdater->UpdateCoverArtData(*listt);
    }
    	break;
    case eInsertGracenoteMetadataQuery_Id:
    {
        TrackerMusicData* gracenoteData;
        gracenoteData = (TrackerMusicData*) data;
        ret = m_DataUpdater->UpdateTrackerWithGracenoteData(*gracenoteData);
    }
		break;    
    case eDeleteGracenoteCDMetadataQuery_Id:
    {
        QString* CDTOC;
        CDTOC = (QString*) data;
        ret = m_DataUpdater->DeleteCDData(*CDTOC);
    }
		break;
        // removed by dk 2013.05.03
    default:
        break;
    }
    return ret;
}

bool CTrackerAbstractor::UpdateData(QString updateCriteria, void* data, ETrackerAbstarctor_Query_Ids queryId)
{
    //qDebug() << "in CTrackerAbstractor::UpdateData" ;
    bool ret=false;
    switch(queryId)
    {
	    case eInsertIPodMusicPlaylistToDBaseQuery_Id:
	    {
	        PlaylistResults* playlistRes;
	        playlistRes= (PlaylistResults *) data;
	        ret = m_DataUpdater->UpdateTrackerWithPlaylistInfo(updateCriteria, *playlistRes);
	    }
	        break;	    
	    case eUpdateCIDInfoInXSampaDB:
	    {
	        QStringList* updateData;
	        updateData = (QStringList*) data;

	        bool ok;
	        //In this case, updateCriteria has the Collection ID
	        int CID = updateCriteria.toInt(&ok, 10);
	        m_XSampaInterface->UpdateCollectionIDInfo(CID, updateData->at(0),updateData->at(1));
	        
	    }
	    	break;    
	    case eInsertXSampeData_Id:
	    {
	            QVector<QStringList>* list;
	            list = (QVector<QStringList>*) data;
	            //LOG_TRACE << "CTrackerAbstractor::UpdateData list count for UpdatePhoneticData" << list->count() << LOG_ENDL;

	            bool ok;
	            //In this case, updateCriteria has the Collection ID
	            int CID = updateCriteria.toInt(&ok, 10);
	            m_XSampaInterface->UpdatePhoneticData(*list, CID);
	     }
	    	break;
	    default:
	        break;
    }
    return ret;
}

/******************************************************************************

// Function Name                                            Parameters

// CTrackerAbstractor::CreateTrackerDataList   void (OUT), QVector<QStringList> (IN) - Music Album list

//

// Explanation

// This method creates the Music Album list required for next and previous song list

//

// History

// 20-Sept-2011      Chetan Nanda                created for creation of the Music Album list
*****************************************************************************/

void CTrackerAbstractor::CreateTrackerDataList(QVector<QStringList> /*dataList*/)
{
    /*    QString temp;
    QStringList albumList;

    // creating album list
    for(int i=0; i<dataList.count(); i++)
    {
        temp = dataList[i][0];
//        folderList.append(temp.section('/',-1));
        albumList.append(temp);
    }
    if(m_MusicAlbumList != NULL)
    {
        delete m_MusicAlbumList;
        m_MusicAlbumList = NULL;
    }

    // creating the linked list of album names
    m_MusicAlbumList = new CDataLinkedList();
    for(int j=0; j<albumList.count(); j++)
        m_MusicAlbumList->addToEnd(new CDataListNode(albumList[j]));*/

}

/******************************************************************************

// Function Name                                            Parameters

// CTrackerAbstractor::DataReceived                 QVector<QStringList> (OUT) - Query Result,
                                                                    ETrackerAbstarctor_Query_Ids (OUT)- respective query Id

//

// Explanation

// This is a slot called when DataFetched signal emited by respective provider

// Signal is emited once the query completed and emits finished signal

//

// History

// 12-Sept-2011      Chetan Nanda                created for handing Music related Queries

//                                                                 and Text Seach Query results when provider emits signal
*****************************************************************************/

void CTrackerAbstractor::DataReceived(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId)
{
    // check for music album list query ( it is used by tracker abstractor internally )
    // else emit signal for application to get notification

    if( queryId != eGetVideoFileListThumbNail_Id )  //added by honggi.shin, 2014.02.27, for removing heavy log
    LOG_HIGH << "[TA] Data Received queryId = " << queryId << LOG_ENDL;
    if(m_isPBData == false) // added by sungha.choi 2013.08.25 (PB do not use queue and singleton member object), So this routine is not needed.
    {
    if(m_CurrentReqStateInfo) {
        if(m_CurrentReqStateInfo->currentQueryId != queryId) { // { added by sungha.choi 2013.08.18 for blank list hotfix
            if( queryId != eGetVideoFileListThumbNail_Id )  //added by honggi.shin, 2014.02.27, for removing heavy log
            qDebug() << "[TA] Not my query. my query = " << m_CurrentReqStateInfo->currentQueryId << "recvd query id = " << queryId;
            return;
        } // } added by sungha.choi 2013.08.18 for blank list hotfix
        if( queryId != eGetVideoFileListThumbNail_Id )  //added by honggi.shin, 2014.02.27, for removing heavy log
        qDebug() << "[TA] Data Received req UID " << m_CurrentReqStateInfo->requestUID ;
    } else {    // { added by sungha.choi 2013.08.23 for ITS 0185231
        return;
    }   // } added by sungha.choi 2013.08.23 for ITS 0185231
    }

    if(m_QueueRequestTimer->isActive()) // { moved by sungha.choi 2013.08.24 for ITS 0186401
        m_QueueRequestTimer->stop(); // } moved by sungha.choi 2013.08.24 for ITS 0186401

    if(queryId == eGetAllAlbumsWithCoverArtFromVolumeQuery_Id)
    {
        qDebug() << "[TA] eGetAllAlbumsWithCoverArtFromVolumeQuery " ;
        if(dataList.isEmpty())
        {
             qDebug() << "List is Empty for WMA case.so emit the signal";
             if(m_CurrentReqStateInfo) {
                 LOG_TRACE << Q_FUNC_INFO << " emit RequesteComplete("
                           << dataListToString(dataList) << ", "
                           << queryIdToString(queryId) << ", "
                           << m_CurrentReqStateInfo->requestUID << ")" << LOG_ENDL;
                 emit RequesteComplete(dataList, queryId, m_CurrentReqStateInfo->requestUID);
             } else {
                 LOG_TRACE << Q_FUNC_INFO << " emit RequesteComplete("
                           << dataListToString(dataList) << ", "
                           << queryIdToString(queryId)  << ")" << LOG_ENDL;
                 emit RequesteComplete(dataList, queryId);
             }
        }
        else
        {
             m_Album_CoverArtRequest = true;
             RequestAlbumArts(dataList);
        }
    }
    else if(queryId == eGetMusicAlbumCoverArt_Id)
    {
    	if(m_Album_CoverArtRequest){
            qDebug() << "[TA] eGetMusicAlbumCoverArt_Id " ;
	        m_Album_CoverArtRequest = false;
	        PrepareDataListForAlbumWithCoverArt(dataList);
    	}
    	else if(m_Album_CoverArtRequestWithLimit){
            qDebug() << "[TA] eGetMusicAlbumCoverArtWithLimit_Id " ;
	        m_Album_CoverArtRequestWithLimit = false;
	        PrepareDataListForAlbumWithCoverArtWithLimit(dataList,
			m_Album_CoverArtRequestOffset);
    	}
		else
		{
            qDebug() << "[TA] request Complete"  ;
            if(m_CurrentReqStateInfo) {
                LOG_TRACE << Q_FUNC_INFO << " emit RequesteComplete("
                          << dataListToString(dataList) << ", "
                          << queryIdToString(queryId) << ", "
                          << m_CurrentReqStateInfo->requestUID << ")" << LOG_ENDL;
                emit RequesteComplete(dataList, queryId, m_CurrentReqStateInfo->requestUID);
            } else {
                LOG_TRACE << Q_FUNC_INFO << " emit RequesteComplete("
                          << dataListToString(dataList) << ", "
                          << queryIdToString(queryId)  << ")" << LOG_ENDL;
                emit RequesteComplete(dataList, queryId);
            }
		}
    }
    else if(queryId == eGetAllSongsCountFromVolumeQuery_Id)
    {
        qDebug() << "[TA] request Complete songcount"  ;
        if(m_CurrentReqStateInfo) {
            LOG_TRACE << Q_FUNC_INFO << " emit RequesteComplete("
                      << dataListToString(dataList) << ", "
                      << queryIdToString(queryId) << ", "
                      << volumeTypeToString(m_VolumeType) << ", "
                      << m_CurrentReqStateInfo->requestUID << ")" << LOG_ENDL;
            emit RequesteComplete(dataList, queryId, m_VolumeType, m_CurrentReqStateInfo->requestUID);
        } else {
            LOG_TRACE << Q_FUNC_INFO << " emit RequesteComplete("
                      << dataListToString(dataList) << ", "
                      << queryIdToString(queryId)  << ", "
                      << volumeTypeToString(m_VolumeType) << ")" << LOG_ENDL;
            emit RequesteComplete(dataList, queryId, m_VolumeType);
        }
    }
    else if(queryId == eGetMusicLimitFileListDir_Id ||
            queryId == eGetVideoLimitFileListDir_Id ||
            queryId == eGetPhotoLimitFileListDir_Id)
    {
        qDebug() << "[TA] request Complete eGetMusicLimitFileListDir_Id"  ;
        if(m_CurrentReqStateInfo) {
            LOG_TRACE << Q_FUNC_INFO << " emit RequesteComplete("
                      << dataListToString(dataList) << ", "
                      << queryIdToString(queryId) << ", "
                      << volumeTypeToString(m_VolumeType) << ", "
                      << m_CurrentReqStateInfo->requestUID << ")" << LOG_ENDL;
            emit RequesteComplete(dataList, queryId, m_VolumeType, m_CurrentReqStateInfo->requestUID);
        } else {
            LOG_TRACE << Q_FUNC_INFO << " emit RequesteComplete("
                      << dataListToString(dataList) << ", "
                      << queryIdToString(queryId)  << ", "
                      << volumeTypeToString(m_VolumeType) << ")" << LOG_ENDL;
            emit RequesteComplete(dataList, queryId, m_VolumeType);
        }
    }
    else
    {
        if( queryId != eGetVideoFileListThumbNail_Id )  //added by honggi.shin, 2014.02.27, for removing heavy log
        qDebug() << "[TA] request Complete "  ;
        if(m_CurrentReqStateInfo) {
            if( queryId != eGetVideoFileListThumbNail_Id )  //added by honggi.shin, 2014.02.27, for removing heavy log
            LOG_TRACE << Q_FUNC_INFO << " emit RequesteComplete("
                      << dataListToString(dataList) << ", "
                      << queryIdToString(queryId) << ", "
                      << m_CurrentReqStateInfo->requestUID << ")" << LOG_ENDL;
            emit RequesteComplete(dataList, queryId, m_CurrentReqStateInfo->requestUID);
        } else {
            if( queryId != eGetVideoFileListThumbNail_Id )  //added by honggi.shin, 2014.02.27, for removing heavy log
            LOG_TRACE << Q_FUNC_INFO << " emit RequesteComplete("
                      << dataListToString(dataList) << ", "
                      << queryIdToString(queryId)  << ")" << LOG_ENDL;
            emit RequesteComplete(dataList, queryId);
        }
    }

    if(m_CurrentReqStateInfo)
    {
		delete m_CurrentReqStateInfo;
		m_CurrentReqStateInfo = NULL;
    } 

    checkQueuedRequests();
}

/******************************************************************************

// Function Name                                            Parameters

// CTrackerAbstractor::DataLimitReceived                 QVector<QStringList> (OUT) - Query Result,
                                                         ETrackerAbstarctor_Query_Ids (OUT)- respective query Id
														 int (OUT) - offset number
//

// Explanation

// This is a slot called when DataLimitReceived signal emited by respective provider

// Signal is emited once the query completed and emits finished signal

//

// History

// 8-Mar-2012     Byungjo Hyun                created for handing Music related Queries

//                                                                 and Text Seach Query results when provider emits signal
*****************************************************************************/
void CTrackerAbstractor::DataLimitReceived(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId, int offset)
{
	if(m_QueueRequestTimer->isActive())
	    m_QueueRequestTimer->stop();

    qDebug() << "[TA] Data Limit Received QueryID " << queryId ;
    if(m_CurrentReqStateInfo)
        qDebug() << "[TA] Data Limit Received req UID " << m_CurrentReqStateInfo->requestUID ;

//    if(queryId == eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id)
//    {
//		qDebug() << "[TA] eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id " ;
//        if(dataList.isEmpty())
//         {
//             qDebug() << "List is Empty for WMA case.so emit the signal";
//             emit RequesteComplete(dataList, queryId, offset);
//         }
//         else
//         {
//             m_Album_CoverArtRequestWithLimit = true;
//			 m_Album_CoverArtRequestOffset = offset;
//             RequestAlbumArts(dataList);
//         }
//    }
//    else

    {
        qDebug() << "[TA] Request Complete "  ;
        if(m_CurrentReqStateInfo) {
            LOG_TRACE << Q_FUNC_INFO << " emit RequesteComplete("
                      << dataListToString(dataList) << ", "
                      << queryIdToString(queryId) << ", "
                      << offset << ", "
                      << m_CurrentReqStateInfo->requestUID << ")" << LOG_ENDL;
            emit RequesteComplete(dataList, queryId, offset, m_CurrentReqStateInfo->requestUID);
        } else {
            LOG_TRACE << Q_FUNC_INFO << " emit RequesteComplete("
                      << dataListToString(dataList) << ", "
                      << queryIdToString(queryId)  << ", "
                      << offset << ")" << LOG_ENDL;
            emit RequesteComplete(dataList, queryId, offset);
        }
    }

    if(m_CurrentReqStateInfo)
    {
		delete m_CurrentReqStateInfo;
		m_CurrentReqStateInfo = NULL;
    }

	checkQueuedRequests();
}

/******************************************************************************

// Function Name                                            Parameters

// CTrackerAbstractor::ErrorReceived                int - error id
//

// Explanation

// This is a slot called when error signal emited by respective provider

//

// History

// 26-Nov-2011      Chetan Nanda                created for handing error signal
*****************************************************************************/

void CTrackerAbstractor::ErrorReceived(int nError)
{
    if(m_QueueRequestTimer->isActive())
        m_QueueRequestTimer->stop();

    if(m_CurrentReqStateInfo) {
        LOG_TRACE << Q_FUNC_INFO
                  << " errorType: " << errorTypeToString(nError)
                  << " requestUID: " << m_CurrentReqStateInfo->requestUID
                  << LOG_ENDL;
        emit Error(nError, m_CurrentReqStateInfo->requestUID);
    } else {
        LOG_TRACE << Q_FUNC_INFO << " errorType: " << errorTypeToString(nError) << LOG_ENDL;
        emit Error(nError);
    }

	checkQueuedRequests();
}

void CTrackerAbstractor::HandleError(int nError)
{
    if(m_QueueRequestTimer->isActive())
        m_QueueRequestTimer->stop();

    if(m_CurrentReqStateInfo) {
        LOG_TRACE << Q_FUNC_INFO
                  << " errorType: " << errorTypeToString(nError)
                  << " requestUID: " << m_CurrentReqStateInfo->requestUID
                  << LOG_ENDL;
        emit Error(nError, m_CurrentReqStateInfo->requestUID);
    } else {
        LOG_TRACE << Q_FUNC_INFO << " errorType: " << errorTypeToString(nError) << LOG_ENDL;
        emit Error(nError);
    }

	checkQueuedRequests();
}

/******************************************************************************

// Function Name                                            Parameters

// CTrackerAbstractor::RequestAlbumArts                dataList - complete datalist
//

// Explanation

// This is a function called for requesting album arts

//

// History

// 6-Jan-2012      Vishal Jain                created for requesting album arts
*****************************************************************************/
void CTrackerAbstractor::RequestAlbumArts(QVector<QStringList> dataList)
{

    QStringList albumNameList;
    QStringList performerNameList;

    for (int index = 0 ; index < dataList.count() ; index++)
    {
        albumNameList.append(dataList[index].at(0));
        performerNameList.append(dataList[index].at(1));
    }

    m_AlbumNameList = albumNameList;
    //Clear the old list
    dataList.clear();

    //Request for album arts for the albums specified in list
    RequestData(albumNameList,performerNameList, eGetMusicAlbumCoverArt_Id);

}

/******************************************************************************

// Function Name                                            Parameters

// CTrackerAbstractor::PrepareDataListForAlbumWithCoverArt dataList - complete datalist
//

// Explanation

// This is a function called for preparing data list for
// eGetAllAlbumsWithCoverArtFromVolumeQuery_Id

// History

// 6-Jan-2012      Vishal Jain          created for preparing data list
*****************************************************************************/

void CTrackerAbstractor::PrepareDataListForAlbumWithCoverArt(QVector<QStringList> dataList)
{
    QVector<QStringList> modifiedDataList;
    QStringList singleList;

    if(m_AlbumNameList.count() == dataList.count())
    {
        for(int index = 0; index < m_AlbumNameList.count(); index++)
        {
            singleList.append(m_AlbumNameList.at(index));
            singleList.append(dataList.at(index));

            modifiedDataList.append(singleList);

            singleList.clear();
        }
    }
	
    dataList.clear(); m_AlbumNameList.clear();

    if(m_CurrentReqStateInfo) {
        LOG_TRACE << Q_FUNC_INFO << " emit RequesteComplete("
                  << dataListToString(modifiedDataList) << ", "
                  << queryIdToString(eGetAllAlbumsWithCoverArtFromVolumeQuery_Id) << ", "
                  << m_CurrentReqStateInfo->requestUID << ")" << LOG_ENDL;
        emit RequesteComplete(modifiedDataList, eGetAllAlbumsWithCoverArtFromVolumeQuery_Id, m_CurrentReqStateInfo->requestUID);
    } else {
        LOG_TRACE << Q_FUNC_INFO << " emit RequesteComplete("
                  << dataListToString(modifiedDataList) << ", "
                  << queryIdToString(eGetAllAlbumsWithCoverArtFromVolumeQuery_Id)  << ")" << LOG_ENDL;
        emit RequesteComplete(modifiedDataList, eGetAllAlbumsWithCoverArtFromVolumeQuery_Id);
    }
}


void CTrackerAbstractor::setActiveIPod(ETrackerAbstarctor_VolumeType volumeType)
{
    m_CurrentActiveIPodVolume = volumeType;
}

/******************************************************************************

// Function Name                                            Parameters

// CTrackerAbstractor::setDeviceId(ETrackerAbstarctor_VolumeType volumeType, QString DeviceId,QString MountPath)
//

// Explanation

// To set the type of device connected so that tracker query can  get the list data.
// History

// 19-Apr-2012      mahesh kumar          created for setting device id
*****************************************************************************/

bool CTrackerAbstractor::setDeviceId(ETrackerAbstarctor_VolumeType volumeType, QString DeviceId,QString MountPath)
{
    //return(m_MediaDataProvider->setDeviceId(volumeType,DeviceId));
    bool ret = false;
    switch(volumeType)
    {
        case eUSB:
        case eUSB_FRONT:
        case eUSB_REAR:
        {
            ret = m_MediaDataProvider->setDeviceURL(volumeType,DeviceId,MountPath);
            // ret = m_DirListProvider->setDeviceURL(volumeType,DeviceId,MountPath); // removed by kihyung 2013.2.17
	    	ret = m_GracenoteDataProvider->setDeviceURL(volumeType,DeviceId,MountPath); //added by Divya for Gracenote normalized data 2013.01.07
        }
        	break;
        case eJukeBox:
        {
            ret = m_MediaDataProvider->setDeviceURL(volumeType,DeviceId,MountPath);
            // ret = m_DirListProvider->setDeviceURL(volumeType,DeviceId,MountPath); // removed by kihyung 2013.2.17
            ret = m_GracenoteDataProvider->setDeviceURL(volumeType,DeviceId,MountPath); //added by Divya for Gracenote normalized data 2013.01.07
        }
        	break;
        case eIPod:
        case eIPOD_FRONT:
            qDebug() << " [TA] eIPOD_FRONT ";
            m_deviceid_ipod_front = DeviceId;
            if(m_deviceid_ipod_front.contains(m_Removed_DeviceId)) // added by eugene.seo 2013.04.29
			{
				m_Removed_DeviceId = "No_Removed_DeviceId";				
			}
            qDebug() << "device id ::: "<< m_deviceid_ipod_front;
	        ret = createFirstIpodConnection(DeviceId);
            ret = m_GracenoteDataProvider->setDeviceURL(volumeType,DeviceId,MountPath); //added by Divya for Gracenote normalized data 2013.01.07
			ret = m_MediaDataProvider->setDeviceURL(volumeType,DeviceId,MountPath); // Added by Neelima, 2013.01.03 for CR13965

	        break;
        case eIPOD_REAR:
            qDebug() << " [TA] eIPOD_REAR ";
            m_deviceid_ipod_rear = DeviceId;
            if(m_deviceid_ipod_rear.contains(m_Removed_DeviceId)) // added by eugene.seo 2013.04.29
			{
				m_Removed_DeviceId = "No_Removed_DeviceId";
			}
            qDebug() << "device id ::: "<< m_deviceid_ipod_rear;
            ret = createSecondIpodConnection(DeviceId);
            ret = m_GracenoteDataProvider->setDeviceURL(volumeType,DeviceId,MountPath); //added by Divya for Gracenote normalized data 2013.01.07
			ret = m_MediaDataProvider->setDeviceURL(volumeType,DeviceId,MountPath); // Added by Neelima, 2013.01.03 for CR13965

	        break;
        default:
            HandleError(eErrorDeviceId);
        break;
    }
    return ret;
}
bool CTrackerAbstractor::createFirstIpodConnection(QString DeviceId)
{
    QString  dbName("first");

     // { Added by Neelima, 2013.01.03 for CR13965
     QString firstIpodDBPath = "/dev/shm/";
     firstIpodDBPath = firstIpodDBPath.append(DeviceId);

     //If DB is present in /dev/shm use tmpfs DB path
     if(!QFile::exists(firstIpodDBPath))
     {
         //Check if the file is present in File System
          firstIpodDBPath.clear();
          firstIpodDBPath.append("/app/data/AppMedia/share/iPodDBPath/"); // modified by eugene.seo 2013.04.04
          firstIpodDBPath.append(DeviceId);

          if(!QFile::exists(firstIpodDBPath))
          {
              qDebug() << " [TA] Database does not exist both on FS and tmpfs";
              return false;
          }
     }
	 // } Added by Neelima
	 
    if(QSqlDatabase::contains(dbName))
    {
       qDebug() << "[TA] createFirstIpodConnection if DB Exists "<< DeviceId ;
       firstIpodDB = QSqlDatabase::database(dbName);
       // { deleted by Neelima, 2013.01.03 for CR13965
       //QString firstIpodDBPath = "/dev/shm/";
	   
       //firstIpodDBPath = firstIpodDBPath.append(DeviceId);
       // } deleted by Neelima
       firstIpodDB.setDatabaseName(firstIpodDBPath);
       //qDebug() <<"in"<< firstIpodDB<<dbName;
       if (!firstIpodDB.open())
       {
           qDebug() << " [TA] error opening database";
           return false;
       }
       else
       {
           qDebug() << " [TA] Database opened successfully";
           return true;
       }
    }
    else
    {
        qDebug() << "[TA] createFirstIpodConnection NEW "<< DeviceId ;
        firstIpodDB = QSqlDatabase::addDatabase("QSQLITE",dbName);
       // { deleted by Neelima, 2013.01.03 for CR13965
       //  QString firstIpodDBPath = "/dev/shm/";
       // firstIpodDBPath = firstIpodDBPath.append(DeviceId);
       // } deleted by Neelima

        firstIpodDB.setDatabaseName(firstIpodDBPath);
        qDebug() <<"in"<< firstIpodDB<<dbName;
        if (!firstIpodDB.open())
        {
           qDebug() << " [TA] error opening database";
           return false;
        }
        else
        {
           qDebug() << " [TA] Database opened successfully";
           return true;
        }
   }
 }

bool CTrackerAbstractor::createSecondIpodConnection(QString DeviceId)
{
   QString dbName("second");

    // { Added by Neelima, 2013.01.03 for CR13965
    QString secondIpodDBPath = "/dev/shm/";
    secondIpodDBPath = secondIpodDBPath.append(DeviceId);

    //If DB is present in /dev/shm use tmpfs DB path
    if(!QFile::exists(secondIpodDBPath))
    {
        //Check if the file is present in File System
         secondIpodDBPath.clear();
         secondIpodDBPath.append("/app/data/AppMedia/share/iPodDBPath/"); // modified by eugene.seo 2013.04.04
         secondIpodDBPath.append(DeviceId);

         if(!QFile::exists(secondIpodDBPath))
         {
             qDebug() << " [TA] Database does not exist both on FS and tmpfs";
             return false;
         }
    }
	// } Added by Neelima
	
   if(QSqlDatabase::contains(dbName))
   {
      qDebug() << "[TA] createSecondIpodConnection if DB Exists "<< DeviceId ;
      secondIpodDB = QSqlDatabase::database(dbName);
      // { Deleted by Neelima, 2013.01.03 for CR13965
      //QString secondIpodDBPath = "/dev/shm/";
      //secondIpodDBPath = secondIpodDBPath.append(DeviceId);
      // } Deleted by Neelima
      secondIpodDB.setDatabaseName(secondIpodDBPath);
      if (!secondIpodDB.open())
      {
          qDebug() << " [TA] second database error opening ";
          return false;
      }
      else
      {
          qDebug() << "[TA] second Database opened successfully";
          return true;
      }
   }
   else
   {
       qDebug() << "[TA] createSecondIpodConnection NEW "<< DeviceId ;
       secondIpodDB = QSqlDatabase::addDatabase("QSQLITE",dbName);
       // { Deleted by Neelima, 2013.01.03 for CR13965
       //QString secondIpodDBPath = "/dev/shm/";
       //secondIpodDBPath = secondIpodDBPath.append(DeviceId);
       // } Deleted by Neelima
       secondIpodDB.setDatabaseName(secondIpodDBPath);
       if (!secondIpodDB.open())
       {
           qDebug() << " [TA] second database error opening ";
           return false;
       }
       else
       {
           qDebug() << "[TA] second Database opened successfully";
           return true;
       }
    }
}


/******************************************************************************

// Function Name                                            Parameters

// CTrackerAbstractor::getDeviceURL(ETrackerAbstarctor_VolumeType volumeType)
//

// Explanation

// To get the URL of the given volume type.
// History

// 19-Apr-2012      mahesh kumar          created for get device url
*****************************************************************************/

QString CTrackerAbstractor::getDeviceURL(ETrackerAbstarctor_VolumeType volumeType)
{
    return (m_MediaDataProvider->getDeviceURL(volumeType));
}

/******************************************************************************

// Function Name                                            Parameters

// CTrackerAbstractor::RequestData                 bool (OUT), albumList (IN) - album list
                                                   artistList(IN) - artist list
                                                   filePaths(IN) - file paths
                                                   ETrackerAbstarctor_Query_Ids (IN)- respective query Id

// Explanation

// This method calls songs/abums/artist by genre/artist/ablum APIs of Media Provider using the query id
// It connects signal/slot for the query completion

// History

// 04-Apr-2012      Vishal Jain                created for handling on demand query
*****************************************************************************/
bool CTrackerAbstractor::RequestData(QStringList albumList, QStringList artistList,
                                     QStringList filePaths, ETrackerAbstarctor_Query_Ids queryId, uint reqUID)
{
    bool ret = false;
    if(albumList.count() <= 0 || artistList.count() <= 0 ||  filePaths.count() <= 0 )
    {
        HandleError(eNullStringParameter);
        return false;
    }

    m_ReqStateInfo->ReqData_set = eRequest_NoVol_ThreeListParams;
    m_ReqStateInfo->StringListParam1 = albumList;
    m_ReqStateInfo->StringListParam2 = artistList;
    m_ReqStateInfo->StringListParam3 = filePaths;
    int reqState = RequestHandler(queryId,eDefaultVolume);
    if(reqState ==eDefaultRequest || reqState == eProcessRequest)
    {
        if(m_CurrentReqStateInfo)   // { added by sungha.choi 2013.08.20 for ipod coverart
        {
            if(m_CurrentReqStateInfo->currentQueryId != queryId || m_CurrentReqStateInfo->requestUID != reqUID)
            {
                delete m_CurrentReqStateInfo;
                m_CurrentReqStateInfo = NULL;
            }
        }   // } added by sungha.choi 2013.08.20 for ipod coverart
        if(m_CurrentReqStateInfo == NULL)
        {
            m_CurrentReqStateInfo = new SRequestStateInfo;
            m_CurrentReqStateInfo->requestUID = reqUID;
            m_CurrentReqStateInfo->currentQueryId = queryId;    // added by sungha.choi 2013.08.18 for blank list hotfix
        }
    }
    else if(reqState == eWaitRequest)
    {
        m_ReqStateInfo->requestUID = reqUID;
        QueueRequest();
        emit RequestState(eWaitRequest, reqUID);
        m_ReqStateInfo->initialize();
        return true;
    }
    else if(reqState == eCancelRequest)
    {
        emit RequestState(eCancelRequest, reqUID);
        return false;
    }

    if(m_QueueRequestTimer->isActive())
        m_QueueRequestTimer->stop();

    m_QueueRequestTimer->start(MAX_TIME_LIMIT);
    m_ReqStateInfo->reqState = eProcesscomplete;

    // switch case to call different queries from different data providers
    switch (queryId)
    {
	    case eRetrieveCoverArtOnDemand_Id:
	    {
			QString filePath = "";
			if(filePaths.count() > 0)
				filePath = filePaths.at(0);
		    if(!filePath.contains(m_Removed_DeviceId)) // added by eugene.seo 2013.04.29
	        	ret = m_CoverArtDataprovider->RequestCoverArtOnDemand(albumList, artistList, filePaths);
	    }
	    default:
	        break;
    }

    if(!ret) m_ReqStateInfo->reqState = eDefaultRequest;

    return ret;
}

/******************************************************************************

// Function Name                                   Parameters

// CTrackerAbstractor::onCoverArtsFromIPOD         coverArts(IN) - cover arts

// Explanation

// This slot will be hit, once the cover arts from IPOD are retrieved

// History

// 04-Apr-2012      Vishal Jain                created for handling callback from IPOD
*****************************************************************************/
void CTrackerAbstractor::onCoverArtsFromIPOD(QStringList coverArts) // rollback
{
	// removed by eugene.seo 2013.04.06
	/*
    if(coverArts.count() == 0)
    {
        qDebug() << "No cover arts available from iPod : 11725";
        return;
    }
    */
	if(!m_CoverArtDataprovider->GetIPODRequestComplete()) // added by eugene.seo 2013.05.17
		return;
	else
	    m_CoverArtDataprovider->CoverArtsFromIPOD(coverArts);
}

/******************************************************************************

// Function Name                                   Parameters

// CTrackerAbstractor::getCoverArtSlot         coverArts(IN) - cover arts

// Explanation

// This slot will be hit, once the cover arts from IPOD are retrieved

// History

// 21-may-2012     venkatesh aeturi              created for handling callback from IPOD
*****************************************************************************/
void CTrackerAbstractor::getCoverArtSlot(QStringList albumList, QStringList artistList)
{
    //Tan 2014.04.17 must pass correct serial number when requesting coverart
    if(m_CurrentActiveIPodVolume == eIPOD_FRONT)
    {
        emit getCoverArt(albumList, artistList,m_deviceid_ipod_front);
    }
    else if(m_CurrentActiveIPodVolume == eIPOD_REAR)
    {
        emit getCoverArt(albumList, artistList,m_deviceid_ipod_rear);
    }
    else
    {
        LOG_CRITICAL << "[TA] getCoverArtSlot no ipod volume" << LOG_ENDL;
    }
}

/******************************************************************************

// Function Name                                            Parameters

// CTrackerAbstractor::PrepareDataListForAlbumWithCoverArtWithLimit dataList - complete datalist offset - offset number
//

// Explanation

// This is a function called for preparing data list for
// eGetAllAlbumsWithCoverArtWihtLimitFromVolumeQuery_Id

// History

// 8-Mar-2012     Byungjo Hyun         created for preparing data list
*****************************************************************************/
void CTrackerAbstractor::PrepareDataListForAlbumWithCoverArtWithLimit(QVector<QStringList> dataList, int offset)
{
    QVector<QStringList> modifiedDataList;
    QStringList singleList;

    if(m_AlbumNameList.count() == dataList.count())
    {
        for(int index = 0; index < m_AlbumNameList.count(); index++)
        {
            singleList.append(m_AlbumNameList.at(index));
            singleList.append(dataList.at(index));

            modifiedDataList.append(singleList);

            singleList.clear();
        }
    }

    dataList.clear(); m_AlbumNameList.clear();

    if(m_CurrentReqStateInfo) {
        LOG_TRACE << Q_FUNC_INFO << " emit RequesteComplete("
                  << dataListToString(modifiedDataList) << ", "
                  << queryIdToString(eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id) << ", "
                  << offset << ", "
                  << m_CurrentReqStateInfo->requestUID << ")" << LOG_ENDL;
        emit RequesteComplete(modifiedDataList, eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id, offset, m_CurrentReqStateInfo->requestUID);
    } else {
        LOG_TRACE << Q_FUNC_INFO << " emit RequesteComplete("
                  << dataListToString(modifiedDataList) << ", "
                  << queryIdToString(eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id) << ", "
                  << offset << ")" << LOG_ENDL;
        emit RequesteComplete(modifiedDataList, eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id, offset);
    }
}

/******************************************************************************

// Function Name                            Parameters

// CTrackerAbstractor::setQueryOffset 		offset - offset of each query 
//

// Explanation

// This is a function called to set a offset for specified queries

// History

// 28-Feb-2012      Mahesh          created to set a offset variable
*****************************************************************************/
void CTrackerAbstractor::setQueryOffset(int offset)
{
	m_Query_Offset = offset;
}

void CTrackerAbstractor::setRequestOffset(int offset)
{
    m_MediaDataProvider->m_Query_Offset = offset;
    m_IpodDataProvider->m_Query_Offset = offset;
}

/******************************************************************************

// Function Name                             Parameters

// CTrackerAbstractor::setQueryLimit 		limit - limit of each query  
//

// Explanation

// This is a function called to set a limit for specified queries

// History

// 28-Feb-2012      Mahesh          created to set a limit variable
*****************************************************************************/
void CTrackerAbstractor::setQueryLimit(int limit)
{
	m_Query_Limit = limit;
}

void CTrackerAbstractor::setRequestLimit(int limit)
{
    m_MediaDataProvider->m_Query_Limit = limit;
    m_IpodDataProvider->m_Query_Limit = limit;
}

int CTrackerAbstractor::RequestHandler(ETrackerAbstarctor_Query_Ids queryId,ETrackerAbstarctor_VolumeType volumeType)
{
    if(m_waitReqList.count() >= MAX_QUEUE_SIZE) {
        m_ReqStateInfo->reqState = eCancelRequest;
    } else if(m_ReqStateInfo->reqState == eDefaultRequest) {
		setRequestOffset(m_Query_Offset);
		setRequestLimit(m_Query_Limit);

        m_ReqStateInfo->currentQueryId = queryId;
        m_ReqStateInfo->previousQueryId = queryId;
        m_ReqStateInfo->currentVolumeType = volumeType;
        m_ReqStateInfo->PreviousVolumeType = volumeType;
        m_ReqStateInfo->reqState = eProcessRequest;

		m_ReqStateInfo->currentOffset = m_Query_Offset;
		m_ReqStateInfo->previousOffset = m_Query_Offset;
		m_ReqStateInfo->currentLimit = m_Query_Limit;
		m_ReqStateInfo->previousLimit = m_Query_Limit;
    } else if(m_ReqStateInfo->reqState == eProcessRequest) {
        m_ReqStateInfo->currentQueryId = queryId;
        m_ReqStateInfo->previousQueryId = queryId;
        m_ReqStateInfo->currentVolumeType = volumeType;
        m_ReqStateInfo->PreviousVolumeType = volumeType;

		m_ReqStateInfo->currentOffset = m_Query_Offset;
		m_ReqStateInfo->previousOffset = m_Query_Offset;		
		m_ReqStateInfo->currentLimit = m_Query_Limit;
		m_ReqStateInfo->previousLimit = m_Query_Limit;	
    } else {
        m_ReqStateInfo->currentQueryId = queryId;
        m_ReqStateInfo->currentVolumeType = volumeType;
		m_ReqStateInfo->currentOffset = m_Query_Offset;		
		m_ReqStateInfo->currentLimit = m_Query_Limit;

        switch(m_ReqStateInfo->ReqData_set) {
        case eRequest_NoVol_NoParam :
        case eRequest_NoVol_StringParam:
        case eRequest_NoVol_List_StringParam:
        case eRequest_NoVol_singleListParam:
        case eRequest_NoVol_TwoListParams:
        case eRequest_NoVol_ThreeListParams:
        case eRequest_NoVol_TwoStringParams:
        case eRequest_NoVol_TwoIntParams:
        case eRequest_NoVol_String_List_boolParam:
        {
            if(m_ReqStateInfo->currentQueryId != m_ReqStateInfo->previousQueryId)
            {
                m_ReqStateInfo->reqState = eWaitRequest;
                m_ReqStateInfo->requestCounter++;
                m_ReqStateInfo->previousQueryId = queryId;
                m_ReqStateInfo->PreviousVolumeType = volumeType;
                m_ReqStateInfo->previousOffset = m_Query_Offset;
                m_ReqStateInfo->previousLimit = m_Query_Limit;
            }
            else
            {
                m_ReqStateInfo->reqState = eWaitRequest;
                m_ReqStateInfo->requestCounter++;
                m_ReqStateInfo->previousQueryId = queryId;
                m_ReqStateInfo->PreviousVolumeType = volumeType;
                m_ReqStateInfo->previousOffset = m_Query_Offset;
                m_ReqStateInfo->previousLimit = m_Query_Limit;
            }
        }
        break;
        case eRequest_Vol_NoParam:
        case eRequest_Vol_SingleListParam:
        case eRequest_Vol_StringParam:
        case eRequest_Vol_OrdParam:
        case eRequest_Vol_TwoListParams:
        {
            if(m_ReqStateInfo->currentQueryId != m_ReqStateInfo->previousQueryId)
            {
                m_ReqStateInfo->reqState = eWaitRequest;
                m_ReqStateInfo->requestCounter++;
                m_ReqStateInfo->previousQueryId = queryId;
                m_ReqStateInfo->PreviousVolumeType = volumeType;
                m_ReqStateInfo->previousOffset = m_Query_Offset;
                m_ReqStateInfo->previousLimit = m_Query_Limit;
            }
            else
            {
                if( (m_ReqStateInfo->currentOffset != m_ReqStateInfo->previousOffset) ||
                        (m_ReqStateInfo->currentLimit != m_ReqStateInfo->previousLimit) )
                {
                    m_ReqStateInfo->reqState = eWaitRequest;
                    m_ReqStateInfo->requestCounter++;
                    m_ReqStateInfo->previousQueryId = queryId;
                    m_ReqStateInfo->PreviousVolumeType = volumeType;
                    m_ReqStateInfo->previousOffset = m_Query_Offset;
                    m_ReqStateInfo->previousLimit = m_Query_Limit;
                }
                else
                {
                    m_ReqStateInfo->reqState = eWaitRequest;
                    m_ReqStateInfo->requestCounter++;
                    m_ReqStateInfo->previousQueryId = queryId;
                    m_ReqStateInfo->PreviousVolumeType = volumeType;
                    m_ReqStateInfo->previousOffset = m_Query_Offset;
                    m_ReqStateInfo->previousLimit = m_Query_Limit;
                }
            }
        }
        break;
        default:
            qDebug() << "[TA] RequestHandler ::  No matched Request Type";
            break;
        }
    }

    if( queryId != eGetVideoFileListThumbNail_Id ) //added by honggi.shin, 2014.02.27, for removing heavy log
        LOG_HIGH << Q_FUNC_INFO
              << " query: " << (queryIdToString(queryId) + requestDataToString(*m_ReqStateInfo))
              << " / volume type: " << volumeTypeToString(volumeType)
              << " / request state: " << requestStateToString(m_ReqStateInfo->reqState)
              <<  LOG_ENDL;
    return m_ReqStateInfo->reqState;
}

void CTrackerAbstractor::checkQueuedRequests()
{
    if(m_ReqStateInfo->requestCounter > 0)
    {
        processQueuedRequests();

        if(m_ReqStateInfo->requestCounter > 0)
            m_ReqStateInfo->requestCounter--;
    }
    else if(m_ReqStateInfo->requestCounter <= 0)
    {
		if(!m_waitReqList.isEmpty())
           	m_waitReqList.clear();
		m_ReqStateInfo->reqState = eDefaultRequest;
    }
}

void CTrackerAbstractor::DirTreeDataReceived(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId, uint reqUID)
{
    if(m_DirTreeQueryId == queryId)
        emit RequesteComplete(dataList, queryId, reqUID);
}

void CTrackerAbstractor::DirTreeDataReceived(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId, ETrackerAbstarctor_VolumeType volumeType, uint reqUID)
{
    if(m_DirTreeQueryId == queryId)
        emit RequesteComplete(dataList, queryId, volumeType, reqUID);
}

CDirTreeProvider* CTrackerAbstractor::GetDirTreeProvider()
{
    if(m_DirTreeProvider == 0) 
    {
        m_DirTreeProvider = CDirTreeProvider::GetInstance();
        if(m_DirTreeProvider)
        {
            // { modified by kihyung 2013.08.23 for ITS 0185245 
            connect(m_DirTreeProvider, SIGNAL(DataDirectFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, uint)),
                    this,              SLOT(DirTreeDataReceived(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, uint)));

            connect(m_DirTreeProvider, SIGNAL(DataDirectFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, ETrackerAbstarctor_VolumeType, uint)),
                    this,              SLOT(DirTreeDataReceived(QVector<QStringList>, ETrackerAbstarctor_Query_Ids, ETrackerAbstarctor_VolumeType, uint)));

            connect(m_DirTreeProvider, SIGNAL(DataFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)),
                    this,              SLOT(DataReceived(QVector<QStringList>, ETrackerAbstarctor_Query_Ids)));

            connect(m_DirTreeProvider, SIGNAL(ScanCompleted(ETrackerAbstarctor_VolumeType, int, int)), 
                    this,              SIGNAL(ScanCompleted(ETrackerAbstarctor_VolumeType, int, int)));
            
            connect(m_DirTreeProvider, SIGNAL(ReadCompleted(ETrackerAbstarctor_VolumeType, int)), 
                    this,              SIGNAL(ReadCompleted(ETrackerAbstarctor_VolumeType, int)));
            
            connect(m_DirTreeProvider, SIGNAL(MediaFileFound(ETrackerAbstarctor_VolumeType, int, int, int, int)), 
                    this,              SIGNAL(MediaFileFound(ETrackerAbstarctor_VolumeType, int, int, int, int)));
            
            connect(m_DirTreeProvider, SIGNAL(Error(int)), 
                    this,              SLOT(ErrorReceived(int)));
            // } modified by kihyung 2013.08.23 for ITS 0185245 
        }
        else 
        {
            qDebug("CTrackerAbstractor::GetDirTreeProvider() m_DirTreeProvider is NULL");
        }
    }

    return m_DirTreeProvider;
}

void CTrackerAbstractor::processQueuedRequests()
{
    bool ret;
    if(m_waitReqList.isEmpty())
    {
        LOG_TRACE << Q_FUNC_INFO << " [TA] Error: Queue is Empty" << LOG_ENDL;
		m_ReqStateInfo->requestCounter = 0;
		m_ReqStateInfo->reqState = eDefaultRequest;
		return;
    }		

    if(m_CurrentReqStateInfo)
    {
        delete m_CurrentReqStateInfo;
        m_CurrentReqStateInfo = NULL;
    }

    m_CurrentReqStateInfo = m_waitReqList.takeFirst();

    if(m_CurrentReqStateInfo == NULL)
    {
        LOG_TRACE << Q_FUNC_INFO << " [TA] Error: Queue Data is NULL" << LOG_ENDL;
		return;
    }
    m_ReqStateInfo->reqState = eProcessRequest;
    m_ReqStateInfo->ReqData_set = m_CurrentReqStateInfo->ReqData_set;
/*
   qDebug() << "processQueuedRequests :: Current volumeType : " << Stateinfo->currentVolumeType;
   qDebug() << "processQueuedRequests :: Current QueryId : " << Stateinfo->currentQueryId;
   qDebug() << "processQueuedRequests :: previous volumeType : " << Stateinfo->PreviousVolumeType;
   qDebug() << "processQueuedRequests :: previous QueryId : " << Stateinfo->previousQueryId;
   qDebug() << "processQueuedRequests :: Request State" << Stateinfo->reqState;
   qDebug() << "processQueuedRequests() :: intParam1 " << Stateinfo->intParam1;
   qDebug() << "processQueuedRequests() :: intParam2" << Stateinfo->intParam2;
   qDebug() << "processQueuedRequests() :: ReqData_set" << Stateinfo->ReqData_set;
   qDebug() << "processQueuedRequests() :: requestCounter" << Stateinfo->requestCounter;
   qDebug() << "processQueuedRequests() :: Stringparam1" << Stateinfo->Stringparam1;
   qDebug() << "processQueuedRequests() :: Stringparam1" << Stateinfo->Stringparam2;
   qDebug() << "processQueuedRequests() :: uint16Param" << Stateinfo->uint16Param;
   qDebug() << "processQueuedRequests() :: StringListParam1 " << Stateinfo->StringListParam1;
   qDebug() << "processQueuedRequests() :: StringListParam2 " << Stateinfo->StringListParam2;
   qDebug() << "processQueuedRequests() :: StringListParam3 " << Stateinfo->StringListParam3;
   qDebug() << "[TA] processQueuedRequests :: ReqData_set:"<<m_ReqStateInfo->ReqData_set;
*/
//	setRequestOffset(Stateinfo->currentOffset);
//	setRequestLimit(Stateinfo->currentLimit);
    setRequestOffset(m_CurrentReqStateInfo->currentOffset);
    setRequestLimit(m_CurrentReqStateInfo->currentLimit);

    switch(m_ReqStateInfo->ReqData_set)
    {
	    case eRequest_NoVol_NoParam:
                ret = RequestData(ETrackerAbstarctor_Query_Ids(m_CurrentReqStateInfo->currentQueryId), m_CurrentReqStateInfo->requestUID);
	        break;
	    case eRequest_NoVol_StringParam:
                ret = RequestData(m_CurrentReqStateInfo->Stringparam1,ETrackerAbstarctor_Query_Ids(m_CurrentReqStateInfo->currentQueryId), m_CurrentReqStateInfo->requestUID);
	        break;
	    case eRequest_NoVol_List_StringParam:
                ret =RequestData(m_CurrentReqStateInfo->StringListParam1,m_CurrentReqStateInfo->Stringparam1,ETrackerAbstarctor_Query_Ids(m_CurrentReqStateInfo->currentQueryId), m_CurrentReqStateInfo->requestUID);
	        break;
	    case eRequest_NoVol_singleListParam:
                ret =  RequestData(m_CurrentReqStateInfo->StringListParam1,ETrackerAbstarctor_Query_Ids(m_CurrentReqStateInfo->currentQueryId), m_CurrentReqStateInfo->requestUID);
	        break;
	    case eRequest_NoVol_TwoListParams:
                ret = RequestData(m_CurrentReqStateInfo->StringListParam1, m_CurrentReqStateInfo->StringListParam2, ETrackerAbstarctor_Query_Ids(m_CurrentReqStateInfo->currentQueryId), m_CurrentReqStateInfo->requestUID);
	        break;
	    case eRequest_NoVol_ThreeListParams:
                ret = RequestData(m_CurrentReqStateInfo->StringListParam1, m_CurrentReqStateInfo->StringListParam2, m_CurrentReqStateInfo->StringListParam3, ETrackerAbstarctor_Query_Ids(m_CurrentReqStateInfo->currentQueryId), m_CurrentReqStateInfo->requestUID);
	        break;
	    case eRequest_NoVol_TwoStringParams:
                ret = RequestData(m_CurrentReqStateInfo->Stringparam1, m_CurrentReqStateInfo->Stringparam2,ETrackerAbstarctor_Query_Ids(m_CurrentReqStateInfo->currentQueryId), m_CurrentReqStateInfo->requestUID);
	        break;
	    case eRequest_NoVol_TwoIntParams:
                ret = RequestData(m_CurrentReqStateInfo->intParam1,m_CurrentReqStateInfo->intParam2,ETrackerAbstarctor_Query_Ids(m_CurrentReqStateInfo->currentQueryId), m_CurrentReqStateInfo->requestUID);
	        break;
	    case eRequest_Vol_NoParam:
                ret = RequestData(ETrackerAbstarctor_VolumeType(m_CurrentReqStateInfo->currentVolumeType),ETrackerAbstarctor_Query_Ids(m_CurrentReqStateInfo->currentQueryId), m_CurrentReqStateInfo->requestUID);
	        break;
	    case  eRequest_Vol_SingleListParam:
	        ret = RequestData(m_CurrentReqStateInfo->StringListParam1,ETrackerAbstarctor_VolumeType(m_CurrentReqStateInfo->currentVolumeType),
                                  ETrackerAbstarctor_Query_Ids(m_CurrentReqStateInfo->currentQueryId), m_CurrentReqStateInfo->requestUID);
	        break;
	    case eRequest_Vol_StringParam:
            qDebug() << "Request Data cK process : Query ID :: " << m_CurrentReqStateInfo->currentQueryId << " request UID :: " << m_CurrentReqStateInfo->requestUID;
	        ret = RequestData(m_CurrentReqStateInfo->Stringparam1,ETrackerAbstarctor_VolumeType(m_CurrentReqStateInfo->currentVolumeType),
                                  ETrackerAbstarctor_Query_Ids(m_CurrentReqStateInfo->currentQueryId), m_CurrentReqStateInfo->requestUID);
	        break;
	    case eRequest_Vol_OrdParam:
                RequestData(ETrackerAbstarctor_VolumeType(m_CurrentReqStateInfo->currentVolumeType),ETrackerAbstarctor_OrderType(m_CurrentReqStateInfo->intParam1), ETrackerAbstarctor_Query_Ids(m_CurrentReqStateInfo->currentQueryId), m_CurrentReqStateInfo->requestUID);
	        break;
	    case  eRequest_Vol_TwoListParams:
	        ret = RequestData(m_CurrentReqStateInfo->StringListParam1, m_CurrentReqStateInfo->StringListParam2, ETrackerAbstarctor_VolumeType(m_CurrentReqStateInfo->currentVolumeType),
                                  ETrackerAbstarctor_Query_Ids(m_CurrentReqStateInfo->currentQueryId), m_CurrentReqStateInfo->requestUID);
	        break;
	    case  eRequest_NoVol_String_List_boolParam:
	        ret = RequestData(m_CurrentReqStateInfo->Stringparam1, m_CurrentReqStateInfo->StringListParam1,
                                  m_CurrentReqStateInfo->cdInserted, ETrackerAbstarctor_Query_Ids(m_CurrentReqStateInfo->currentQueryId), m_CurrentReqStateInfo->requestUID);  // modified by sungha.choi 2013.08.18
	        break;
	    default:
            LOG_TRACE << Q_FUNC_INFO << " No matched API" << LOG_ENDL;
	        break;
    }
}

void CTrackerAbstractor::QueueRequest()
{
    SRequestStateInfo *buffer = new SRequestStateInfo(*m_ReqStateInfo);
    m_waitReqList.enqueue(buffer);

    LOG_TRACE << Q_FUNC_INFO
              << " queue length: " << m_waitReqList.count()
              << " m_ReqStateInfo->requestCounter: " << m_ReqStateInfo->requestCounter
              <<  LOG_ENDL;

    for (int i = m_waitReqList.count() - 1; i >= 0; --i) {
        if( m_waitReqList[i]->currentQueryId != eGetVideoFileListThumbNail_Id ) //added by honggi.shin, 2014.02.27, for removing heavy log
        LOG_TRACE << Q_FUNC_INFO
                  << " query[" << i << "] " << (queryIdToString(m_waitReqList[i]->currentQueryId) + requestDataToString(*m_waitReqList[i]))
                  <<  LOG_ENDL;
    }
}

/******************************************************************************

// Function Name                                            Parameters

// CTrackerAbstractor::RequestData                 bool (OUT), QStringList (IN) - argument list1
                                                                    QStringList (IN) - argument list2 ,
                                                                    ETrackerAbstarctor_VolumeType (IN) - volume type
                                                                    ETrackerAbstarctor_Query_Ids (IN)- respective query Id

// Explanation

// This method calls songs/abums/artist by genre/artist/ablum APIs of Media Provider using the query id
// It connects signal/slot for the query completion

// History

// 14-May-2012      Chetan Nanda                created for handingsongs/abums/artist by genre/artist/ablum
                                                                  related Queries of Media provider
*****************************************************************************/

bool CTrackerAbstractor::RequestData(QStringList argument1, QStringList argument2,
                                                        ETrackerAbstarctor_VolumeType volumeType,
                                                        ETrackerAbstarctor_Query_Ids queryId, uint reqUID)
{
    bool ret = false;

    if(argument1.count() <= 0 || argument2.count() <= 0 )
    {
        HandleError(eNullStringParameter);
        return false;
    }

    m_ReqStateInfo->ReqData_set = eRequest_Vol_TwoListParams;
    m_ReqStateInfo->StringListParam1 = argument1;
    m_ReqStateInfo->StringListParam2 = argument2;
    m_ReqStateInfo->requestUID = reqUID;
    int reqState = RequestHandler(queryId,volumeType);
    if(reqState ==eDefaultRequest || reqState == eProcessRequest)
    {
        if(m_CurrentReqStateInfo)   // { added by sungha.choi 2013.08.20 for ipod coverart
        {
            if(m_CurrentReqStateInfo->currentQueryId != queryId || m_CurrentReqStateInfo->requestUID != reqUID)
            {
                delete m_CurrentReqStateInfo;
                m_CurrentReqStateInfo = NULL;
            }
        }   // } added by sungha.choi 2013.08.20 for ipod coverart
        if(m_CurrentReqStateInfo == NULL)
        {
            m_CurrentReqStateInfo = new SRequestStateInfo;
            m_CurrentReqStateInfo->requestUID = reqUID;
            m_CurrentReqStateInfo->currentQueryId = queryId;    // added by sungha.choi 2013.08.18 for blank list hotfix
        }
    }
    else if(reqState == eWaitRequest)
    {
        QueueRequest();
        emit RequestState(eWaitRequest, reqUID);
        m_ReqStateInfo->initialize();
        return true;
    }
    else if(reqState == eCancelRequest)
    {
        emit RequestState(eCancelRequest, reqUID);
        return false;
    }

    if(m_QueueRequestTimer->isActive())
        m_QueueRequestTimer->stop();

    m_QueueRequestTimer->start(MAX_TIME_LIMIT);
    m_ReqStateInfo->reqState = eProcesscomplete;

    switch(queryId)
    {
        case eGetMultipleAlbumSongsByArtistFromVolumeQuery_Id:
        {
            ret = m_MediaDataProvider->GetMultipleAlbumSongsByArtistFromVolume(argument1, argument2[0],                                                                                                                volumeType);
        }
        	break;
        case eGetMultipleArtistSongsByGenreFromVolumeQuery_Id:
        {
            ret = m_MediaDataProvider->GetMultipleArtistSongsByGenreFromVolume(argument1, argument2[0],volumeType);
        }
    	    break;
        case eGetMultipleAlbumSongsByGenreArtistFromVolumeQuery_Id:
        {
            ret = m_MediaDataProvider->GetMultipleAlbumSongsByGenreArtistFromVolume(argument1, argument2[0],argument2[1], volumeType);
        }
	        break;
        default:
        break;
    }
    if(!ret) m_ReqStateInfo->reqState = eDefaultRequest;
    return ret;
}

/******************************************************************************

// Function Name                                            Parameters

// CTrackerAbstractor::RequestData                 bool (OUT), QString (IN) - keyword to serach
                                                                    QStringList (IN) - volume list , bool (IN) - cd inserted or not
                                                                    ETrackerAbstarctor_Query_Ids (IN)- respective query Id

// Explanation

// This method calls global search API of Media Provider using the query id
// It connects signal/slot for the query completion

// History

// 14-June-2012      Chetan Nanda                created for handing global search
                                                                  related Queries of Media provider
*****************************************************************************/
bool CTrackerAbstractor::RequestData(QString keyword, QStringList volumeList, bool cdInserted, ETrackerAbstarctor_Query_Ids queryId, uint reqUID)
{
    bool ret = false;

    m_ReqStateInfo->ReqData_set = eRequest_NoVol_String_List_boolParam;
    m_ReqStateInfo->Stringparam1 = keyword;
    m_ReqStateInfo->StringListParam1 = volumeList;
    m_ReqStateInfo->cdInserted = cdInserted;
    int reqState = RequestHandler(queryId,eDefaultVolume);
    if(reqState ==eDefaultRequest || reqState == eProcessRequest)
    {
        if(m_CurrentReqStateInfo)   // { added by sungha.choi 2013.08.20 for ipod coverart
        {
            if(m_CurrentReqStateInfo->currentQueryId != queryId || m_CurrentReqStateInfo->requestUID != reqUID)
            {
                delete m_CurrentReqStateInfo;
                m_CurrentReqStateInfo = NULL;
            }
        }   // } added by sungha.choi 2013.08.20 for ipod coverart
        if(m_CurrentReqStateInfo == NULL)
        {
            m_CurrentReqStateInfo = new SRequestStateInfo;
            m_CurrentReqStateInfo->requestUID = reqUID;
            m_CurrentReqStateInfo->currentQueryId = queryId;    // added by sungha.choi 2013.08.18 for blank list hotfix
        }
    }
    else if(reqState == eWaitRequest)
    {
        m_ReqStateInfo->requestUID = reqUID;
        QueueRequest();
        emit RequestState(eWaitRequest, reqUID);
        m_ReqStateInfo->initialize();
        return true;
    }
    else if(reqState == eCancelRequest)
    {
        emit RequestState(eCancelRequest, reqUID);
        return false;
    }

    if(m_QueueRequestTimer->isActive())
        m_QueueRequestTimer->stop();

    m_QueueRequestTimer->start(MAX_TIME_LIMIT);
    m_ReqStateInfo->reqState = eProcesscomplete;

    //Check if Gracenote Normalized data is selected by the user.
    bool normalizedData = GetGracenoteNormalizedData(); //added by Divya for Gracenote normalized data 2013.01.07

    switch(queryId)
    {
        case  eGetAllMusicByKeywordQuery_Id:
        {
            QList<QSqlDatabase> dbConnList;
	        // { Added by Neelima, 2013.01.03 for CR13965            
	        QString devID = m_MediaDataProvider->getDeviceID(eIPod);
            if(devID.isNull() || devID == "Empty")
            {
                LOG_TRACE << "devID is Empty" << LOG_ENDL;
                devID.clear();
                devID = m_MediaDataProvider->getDeviceID(eIPOD_FRONT);
            }

			if(devID.isNull() || devID == "Empty")
			{
			    LOG_TRACE << "devID is Empty" << LOG_ENDL;
			}
            else if(createFirstIpodConnection(devID))
                dbConnList.append(firstIpodDB);

            devID.clear();
            devID = m_MediaDataProvider->getDeviceID(eIPOD_REAR);

			if(devID.isNull() || devID == "Empty")
		    {
		        LOG_TRACE << "devID is Empty" << LOG_ENDL;
				//return ret; Removed by Divya. Code should not return here.
			}
            else if(createSecondIpodConnection(devID))
               dbConnList.append(secondIpodDB);
             // } Added by Neelima
             m_MediaDataProvider->SetIPodDBConnectionList(dbConnList);
			 //modified by Divya for Gracenote normalized data 2013.01.07
             if(normalizedData)
             {
                m_GracenoteDataProvider->m_isCDInserted = cdInserted;

                if(firstIpodDB.isOpen())
                    m_GracenoteDataProvider->m_IPOD_FRONT = true;

                if(secondIpodDB.isOpen())
                    m_GracenoteDataProvider->m_IPOD_REAR = true;

                ret = m_GracenoteDataProvider->GetAllMusicByKeyword(keyword, volumeList);
             }
             else
             {
                 ret = m_MediaDataProvider->GetAllMusicByKeyword(keyword, volumeList, cdInserted);
             }
			 //}modified by Divya for Gracenote normalized data 2013.01.07
         }
        	break;
        default:
        	break;
    }
    if(!ret) m_ReqStateInfo->reqState = eDefaultRequest;
    return ret;

}

/******************************************************************************

// Function Name                                            Parameters

// CTrackerAbstractor::RemoveDeviceId                 void (OUT), QString (IN) - ipod device id

// Explanation

// This method closes the ipod connection of given device id

// History

// 14-June-2012      Chetan Nanda                created for closing the ipod connection of given device id
*****************************************************************************/
void CTrackerAbstractor::RemoveDeviceId(QString deviceId)
{	
	m_Removed_DeviceId = deviceId.remove("/"); // added by eugene.seo 2013.04.29
	m_CoverArtDataprovider->ResetIPODRequestComplete(); // added by eugene.seo 2013.04.06

    if(firstIpodDB.isOpen())
    {
        QString dbName;
        dbName = firstIpodDB.databaseName();
        if(dbName.contains(deviceId))
            firstIpodDB.close();
    }

    if(secondIpodDB.isOpen())
    {
        QString dbName;
        dbName = secondIpodDB.databaseName();
        if(dbName.contains(deviceId))
            secondIpodDB.close();
    }
}

void CTrackerAbstractor::RequestTimeOut()
{
    //LOG_HIGH << "***** RequestTimeOut() m_ReqStateInfo->requestCounter  = " << (m_ReqStateInfo->requestCounter) << LOG_ENDL;
    checkQueuedRequests();
}


//{added by Divya for Gracenote normalized data 2013.01.07
/******************************************************************************
// Function Name                                            Parameters
// CTrackerAbstractor::SetGracenoteNormalizedData                 bool (IN)- normalizedDataflag

// Explanation
// Set m_GracenoteNormalizedData to true if the user selects Gracenote Normalized Data.
//Else it is set to false.

// History
// 15-Nov-2012      Divya DSouza                created for displaying Gracenote Normalized Data
*****************************************************************************/
void CTrackerAbstractor::SetGracenoteNormalizedData(bool normalizedDataflag)
{
    m_GracenoteNormalizedData = normalizedDataflag;
}


/******************************************************************************
// Function Name                                            Parameters
// CTrackerAbstractor::GetGracenoteNormalizedData                 bool (OUT)- normalizedDataflag

// Explanation
// Returns m_GracenoteNormalizedData value

// History
// 15-Nov-2012      Divya DSouza                created for displaying Gracenote Normalized Data
*****************************************************************************/
bool CTrackerAbstractor::GetGracenoteNormalizedData()
{
    return m_GracenoteNormalizedData;
}
//}added by Divya for Gracenote normalized data 2013.01.07
//End of file

void CTrackerAbstractor::SetAVNSWVersion(QString strVersion)
{
    CDirTreeProvider *pTree = GetDirTreeProvider();
    if(pTree) {
        pTree->SetSWVersion(strVersion);
    }
}

void CTrackerAbstractor::SetFileSystemScanMode(bool bScan)
{
    CDirTreeProvider *pTree = GetDirTreeProvider();
    if(pTree) {
        pTree->SetFileSystemScanMode(bScan);
    }
}

bool CTrackerAbstractor::StartScanFileSystem(ETrackerAbstarctor_VolumeType volumeType, bool bRescanMode)
{
    CDirTreeProvider *pTree = GetDirTreeProvider();
    if(pTree) {
        return pTree->StartScanFileSystem(volumeType, bRescanMode);
    }
    return false;
}

bool CTrackerAbstractor::StopScanFileSystem(ETrackerAbstarctor_VolumeType volumeType)
{
    if(m_MediaDataProvider) {   // { added by sungha.choi 2013.07.30
        m_MediaDataProvider->DeleteLimitCache(volumeType);
    }   // } added by sungha.choi 2013.07.30
    CDirTreeProvider *pTree = GetDirTreeProvider();
    if(pTree) {    
        return pTree->StopScanFileSystem(volumeType);
    }
    return false;
}

bool CTrackerAbstractor::StartReadScanResult(ETrackerAbstarctor_VolumeType volumeType)
{
    CDirTreeProvider *pTree = GetDirTreeProvider();
    if(pTree) {    
        return pTree->StartReadScanResult(volumeType);
    }
    return false;
}

bool CTrackerAbstractor::StopReadScanResult(ETrackerAbstarctor_VolumeType volumeType)
{
    CDirTreeProvider *pTree = GetDirTreeProvider();
    if(pTree) {       
        return pTree->StopReadScanResult(volumeType);
    }
    return false;
}

int CTrackerAbstractor::GetAudioFileNum(ETrackerAbstarctor_VolumeType  volumeType)
{
    CDirTreeProvider *pTree = GetDirTreeProvider();
    if(pTree) {      
        return pTree->GetAudioFileNum(volumeType);
    }
    return 0;
}

int CTrackerAbstractor::GetVideoFileNum(ETrackerAbstarctor_VolumeType  volumeType)
{
    CDirTreeProvider *pTree = GetDirTreeProvider();
    if(pTree) {      
        return pTree->GetVideoFileNum(volumeType);
    }
    return 0;
}

int CTrackerAbstractor::GetPhotoFileNum(ETrackerAbstarctor_VolumeType  volumeType)
{
    CDirTreeProvider *pTree = GetDirTreeProvider();
    if(pTree) {        
        return pTree->GetPhotoFileNum(volumeType);
    }
    return 0;
}

QString CTrackerAbstractor::GetFirstAudioFolder(ETrackerAbstarctor_VolumeType volumeType)
{
    CDirTreeProvider *pTree = GetDirTreeProvider();
    if(pTree) {     
        return pTree->GetFirstAudioFolder(volumeType);
    }
    return QString();
}

QString CTrackerAbstractor::GetFirstVideoFolder(ETrackerAbstarctor_VolumeType volumeType)
{
    CDirTreeProvider *pTree = GetDirTreeProvider();
    if(pTree) {       
        return pTree->GetFirstVideoFolder(volumeType);    
    }
    return QString();
}

QString CTrackerAbstractor::GetFirstPhotoFolder(ETrackerAbstarctor_VolumeType volumeType)
{
    CDirTreeProvider *pTree = GetDirTreeProvider();
    if(pTree) {      
        return pTree->GetFirstPhotoFolder(volumeType);    
    }
    return QString();
}

void CTrackerAbstractor::setMinerStatus(ETrackerAbstarctor_VolumeType volumeType, bool bFinished)  // added by sungha.choi 2013.07.23
{
    switch(volumeType)
    {
        case eJukeBox:
            m_jukeBoxMinerFinished = bFinished;
            break;
        case eUSB_FRONT:
            m_usb1MinerFinished = bFinished;
            break;
        case eUSB_REAR:
            m_usb2MinerFinished = bFinished;
            break;
        default:
            break;
    }
}

bool CTrackerAbstractor::getMinerStatus(ETrackerAbstarctor_VolumeType volumeType)  // added by sungha.choi 2013.07.23
{
    bool ret = false;
    switch(volumeType)
    {
        case eJukeBox:
            ret = m_jukeBoxMinerFinished;
            break;
        case eUSB_FRONT:
            ret = m_usb1MinerFinished;
            break;
        case eUSB_REAR:
            ret = m_usb2MinerFinished;
            break;
        default:
            break;
    }
    return ret;
}

bool CTrackerAbstractor::FileExists(QString fullPath, ETrackerAbstarctor_VolumeType volumeType)   // { added by sungha.choi 2013.07.31 for scandir performance
{
    bool ret = false;
    CDirTreeProvider *pTree = GetDirTreeProvider();
    if(pTree) {
        ret = pTree->FileExists(fullPath, volumeType);
    }
    return ret;
}   // } added by sungha.choi 2013.07.31 for scandir performance

// added by kihyung 2013.08.07 for ITS 0181967 
bool CTrackerAbstractor::GetFSFolderInfo(QList<FSFolderInfo> &tFolderInfo, ETrackerAbstarctor_VolumeType volumeType, ETrackerAbstarctor_MediaType mediaType)
{
    bool ret = false;
    CDirTreeProvider *pTree = GetDirTreeProvider();
    if(pTree) {
        ret = pTree->GetFSFolderInfo(tFolderInfo, volumeType, mediaType);
    }
    return ret;
}

void CTrackerAbstractor::clearCategoryListCache(ETrackerAbstarctor_VolumeType  volumeType)   // { added by sungha.choi 2013.08.21 for ITS 0184905
{
    if(m_MediaDataProvider) {
        m_MediaDataProvider->DeleteLimitCache(volumeType);
    }
}   // } added by sungha.choi 2013.08.21 for ITS 0184905
    
//{added by junam 2013.08.21 for iPod file compare
bool CTrackerAbstractor::getIPodFileUID(QString filePath, quint64 &uid)
{
    QString uidStr;

   // int i  = filePath.lastIndexOf("/");
    int _i = filePath.indexOf("_");
    int i_ = filePath.lastIndexOf("_");

    if( _i == i_ )
    {
        uidStr.append(filePath.mid(_i+1, filePath.size()));
    }
    else
    {
        int len = i_ - (_i+1);
        uidStr.append(filePath.mid(_i+1, len));
    }

    bool ok = false;
    uid = uidStr.toULongLong(&ok);
    return ok;
}
//}added by junam

void  CTrackerAbstractor::GetMediaEntryList(const QString &strSubDirPath, QList<QStringList> *entryList, ETrackerAbstarctor_VolumeType volumeType)   // added by sungha.choi 2013.09.10
{
    if(!entryList)
        return;
    entryList->clear();

    CDirTreeProvider *pTree = GetDirTreeProvider();
    if(pTree) {
        pTree->GetMediaEntryList(strSubDirPath, entryList, volumeType);
    }
}

void CTrackerAbstractor::GetAudioEntryList(const QString &strSubDirPath, QList<QStringList> *entryList, ETrackerAbstarctor_VolumeType volumeType)   // added by sungha.choi 2013.09.10
{
    if(!entryList)
        return;
    entryList->clear();

    CDirTreeProvider *pTree = GetDirTreeProvider();
    if(pTree) {
        pTree->GetAudioEntryList(strSubDirPath, entryList, volumeType);
    }
}

void  CTrackerAbstractor::GetVideoEntryList(const QString &strSubDirPath, QList<QStringList> *entryList, ETrackerAbstarctor_VolumeType volumeType)   // added by sungha.choi 2013.09.10
{
    if(!entryList)
        return;
    entryList->clear();

    CDirTreeProvider *pTree = GetDirTreeProvider();
    if(pTree) {
        pTree->GetVideoEntryList(strSubDirPath, entryList, volumeType);
    }
}

void  CTrackerAbstractor::GetPictureEntryList(const QString &strSubDirPath, QList<QStringList> *entryList, ETrackerAbstarctor_VolumeType volumeType)   // added by sungha.choi 2013.09.10
{
    if(!entryList)
        return;
    entryList->clear();

    CDirTreeProvider *pTree = GetDirTreeProvider();
    if(pTree) {
        pTree->GetPictureEntryList(strSubDirPath, entryList, volumeType);
    }
}
// { added by kihyung 2013.10.02 for factory reset
void CTrackerAbstractor::FactoryReset()
{
    LOG_HIGH << "SHG CTrackerAbstractor FactoryReset!!!" << LOG_ENDL;

    CDirTreeProvider *pTree = GetDirTreeProvider();
    if(pTree) {
        pTree->RemoveScanResultFile(eJukeBox);
        pTree->RemoveScanResultFile(eUSB_FRONT);
        pTree->RemoveScanResultFile(eUSB_REAR);        
    }

	// { added by dwkim 2013.10.02 for factory reset
	system(QString("tracker-control -r").toAscii().data()); // 1. open tracker reset

	QDir tempMediaArtDataDir("/home/meego/.cache/media-art/");	// 2. delete CoverArt Img.
    if(tempMediaArtDataDir.exists())
    {
        QStringList entryList = tempMediaArtDataDir.entryList();
        foreach(QString entry, entryList)
        {
            tempMediaArtDataDir.remove(entry);
        }
    }

	QDir tempGraceNoteDataDir("/app/data/gracenote/");	// 3. for gracenote
    if(tempGraceNoteDataDir.exists())
    {
        QStringList entryList = tempGraceNoteDataDir.entryList();
        foreach(QString entry, entryList)
        {
        	if(entry == "configuration")
        	{
				continue;
        	}
            tempGraceNoteDataDir.remove(entry);
        }
    }
	// } added by dwkim 2013.10.02 for factory reset
	
}
// { added by kihyung 2013.10.02 for factory reset

// { added by honggi.shin, 2014.03.12, for shutdown just after JB format
void CTrackerAbstractor::RemoveJBScanResultFile()
{
    CDirTreeProvider *pTree = GetDirTreeProvider();
    if(pTree) {
        pTree->RemoveScanResultFile(eJukeBox);
    }
}
// } added by honggi.shin, 2014.03.12, END

// { added by kihyung 2013.10.02 to get language setting 
void CTrackerAbstractor::SetLanguage(int lang)
{
    m_lang = lang; 
    m_MediaDataProvider->SetLanguage(m_lang); 
}

//{added by junam 2013.11.07 for coverart loading performance
QString CTrackerAbstractor::GetMediaArt( const QString &artist, const QString &album)
{
    //{ added by jaehwan 2014.04.01 to fix ITS 232877
    QString artistName, albumName;

    if (artist.trimmed().isEmpty())
        artistName = "Unknown Artists";
    else
        artistName = artist;

    if (album.trimmed().isEmpty())
        albumName = "Unknown Albums";
    else
        albumName = album;
    //} added by jaehwan

    MediaArt::Info AlbumArt = MediaArt::Album(artistName, albumName); //modified by jaehwan 2014.04.01 to fix ITS 232877

    if(!AlbumArt.exists())
        return QString();

    return AlbumArt.potentialPath().toString();
}
//}added by junam

//{ added by jaehwan 2013.10.16 for translation from latin extended char to alphabet char
wchar_t LATIN_EXTEND_ALPAH_TAB [] = {
'A',  //U+00C0
'A',  //U+00C1
'A',  //U+00C2
'A',  //U+00C3
'A',  //U+00C4
'A',  //U+00C5
'A',  //U+00C6 //modified by jaehwan 2013.11.18
'C',  //U+00C7
'E',  //U+00C8
'E',  //U+00C9
'E',  //U+00CA
'E',  //U+00CB
'I',  //U+00CC
'I',  //U+00CD
'I',  //U+00CE
'I',  //U+00CF
' ',  //U+00D0
'N',  //U+00D1
'O',  //U+00D2
'O',  //U+00D3
'O',  //U+00D4
'O',  //U+00D5
'O',  //U+00D6
' ',  //U+00D7
'O',  //U+00D8
'U',  //U+00D9
'U',  //U+00DA
'U',  //U+00DB
'U',  //U+00DC
'Y',  //U+00DD
' ',  //U+00DE
'S',  //U+00DF
'A',  //U+00E0
'A',  //U+00E1
'A',  //U+00E2
'A',  //U+00E3
'A',  //U+00E4
'A',  //U+00E5
' ',  //U+00E6
'C',  //U+00E7
'E',  //U+00E8
'E',  //U+00E9
'E',  //U+00EA
'E',  //U+00EB
'I',  //U+00EC
'I',  //U+00ED
'I',  //U+00EE
'I',  //U+00EF
' ',  //U+00F0
'N',  //U+00F1
'O',  //U+00F2
'O',  //U+00F3
'O',  //U+00F4
'O',  //U+00F5
'O',  //U+00F6
' ',  //U+00F7
'O',  //U+00F8
'U',  //U+00F9
'U',  //U+00FA
'U',  //U+00FB
'U',  //U+00FC
'Y',  //U+00FD
' ',  //U+00FE
'Y',  //U+00FF
'A',  //U+0100
'A',  //U+0101
'A',  //U+0102
'A',  //U+0103
'A',  //U+0104
'A',  //U+0105
'C',  //U+0106
'C',  //U+0107
'C',  //U+0108
'C',  //U+0109
'C',  //U+010A
'C',  //U+010B
'C',  //U+010C
'C',  //U+010D
'D',  //U+010E
'D',  //U+010F
'D',  //U+0110
'D',  //U+0111
'E',  //U+0112
'E',  //U+0113
'E',  //U+0114
'E',  //U+0115
'E',  //U+0116
'E',  //U+0117
'E',  //U+0118
'E',  //U+0119
'E',  //U+011A
'E',  //U+011B
'G',  //U+011C
'G',  //U+011D
'G',  //U+011E
'G',  //U+011F
'G',  //U+0120
'G',  //U+0121
'G',  //U+0122
'G',  //U+0123
'H',  //U+0124
'H',  //U+0125
'H',  //U+0126
'H',  //U+0127
'I',  //U+0128
'I',  //U+0129
'I',  //U+012A
'I',  //U+012B
'I',  //U+012C
'I',  //U+012D
'I',  //U+012E
'I',  //U+012F
'I',  //U+0130
'I',  //U+0131
' ',  //U+0132
' ',  //U+0133
'J',  //U+0134
'J',  //U+0135
'K',  //U+0136
'K',  //U+0137
' ',  //U+0138
'L',  //U+0139
'L',  //U+013A
'L',  //U+013B
'L',  //U+013C
'L',  //U+013D
'L',  //U+013E
'L',  //U+013F
'L',  //U+0140
'L',  //U+0141
'L',  //U+0142
'N',  //U+0143
'N',  //U+0144
'N',  //U+0145
'N',  //U+0146
'N',  //U+0147
'N',  //U+0148
'N',  //U+0149
' ',  //U+014A
' ',  //U+014B
'O',  //U+014C
'O',  //U+014D
'O',  //U+014E
'O',  //U+014F
'O',  //U+0150
'O',  //U+0151
' ',  //U+0152
' ',  //U+0153
'R',  //U+0154
'R',  //U+0155
'R',  //U+0156
'R',  //U+0157
'R',  //U+0158
'R',  //U+0159
'S',  //U+015A
'S',  //U+015B
'S',  //U+015C
'S',  //U+015D
'S',  //U+015E
'S',  //U+015F
'S',  //U+0160
'S',  //U+0161
'T',  //U+0162
'T',  //U+0163
'T',  //U+0164
'T',  //U+0165
'T',  //U+0166
'T',  //U+0167
'U',  //U+0168
'U',  //U+0169
'U',  //U+016A
'U',  //U+016B
'U',  //U+016C
'U',  //U+016D
'U',  //U+016E
'U',  //U+016F
'U',  //U+0170
'U',  //U+0171
'U',  //U+0172
'U',  //U+0173
'W',  //U+0174
'W',  //U+0175
'Y',  //U+0176
'Y',  //U+0177
'Y',  //U+0178
'Z',  //U+0179
'Z',  //U+017A
'Z',  //U+017B
'Z',  //U+017C
'Z',  //U+017D
'Z',  //U+017E
'S',  //U+017F
'B',  //U+0180
'B',  //U+0181
'B',  //U+0182
'B',  //U+0183
' ',  //U+0184
' ',  //U+0185
'O',  //U+0186
'C',  //U+0187
'C',  //U+0188
'D',  //U+0189
'D',  //U+018A
'D',  //U+018B
'D',  //U+018C
' ',  //U+018D
'E',  //U+018E
' ',  //U+018F
'E',  //U+0190
'F',  //U+0191
'F',  //U+0192
'G',  //U+0193
'G',  //U+0194
' ',  //U+0195
' ',  //U+0196
'I',  //U+0197
'K',  //U+0198
'K',  //U+0199
'L',  //U+019A
'L',  //U+019B
'M',  //U+019C
'N',  //U+019D
'N',  //U+019E
'O',  //U+019F
'O',  //U+01A0
'O',  //U+01A1
' ',  //U+01A2
' ',  //U+01A3
'P',  //U+01A4
'P',  //U+01A5
' ',  //U+01A6
' ',  //U+01A7
' ',  //U+01A8
' ',  //U+01A9
' ',  //U+01AA
'T',  //U+01AB
'T',  //U+01AC
'T',  //U+01AD
'T',  //U+01AE
'U',  //U+01AF
'U',  //U+01B0
' ',  //U+01B1
'V',  //U+01B2
'Y',  //U+01B3
'Y',  //U+01B4
'Z',  //U+01B5
'Z',  //U+01B6
' ',  //U+01B7
' ',  //U+01B8
' ',  //U+01B9
' ',  //U+01BA
' ',  //U+01BB
' ',  //U+01BC
' ',  //U+01BD
' ',  //U+01BE
' ',  //U+01BF
' ',  //U+01C0
' ',  //U+01C1
' ',  //U+01C2
' ',  //U+01C3
' ',  //U+01C4
' ',  //U+01C5
' ',  //U+01C6
' ',  //U+01C7
' ',  //U+01C8
' ',  //U+01C9
' ',  //U+01CA
' ',  //U+01CB
' ',  //U+01CC
'A',  //U+01CD
'A',  //U+01CE
'I',  //U+01CF
'I',  //U+01D0
'O',  //U+01D1
'O',  //U+01D2
'U',  //U+01D3
'U',  //U+01D4
'U',  //U+01D5
'U',  //U+01D6
'U',  //U+01D7
'U',  //U+01D8
'U',  //U+01D9
'U',  //U+01DA
'U',  //U+01DB
'U',  //U+01DC
'E',  //U+01DD
'A',  //U+01DE
'A',  //U+01DF
'A',  //U+01E0
'A',  //U+01E1
' ',  //U+01E2
' ',  //U+01E3
'G',  //U+01E4
'G',  //U+01E5
'G',  //U+01E6
'G',  //U+01E7
'K',  //U+01E8
'K',  //U+01E9
'O',  //U+01EA
'O',  //U+01EB
'O',  //U+01EC
'O',  //U+01ED
' ',  //U+01EE
' ',  //U+01EF
'J',  //U+01F0
' ',  //U+01F1
' ',  //U+01F2
' ',  //U+01F3
'G',  //U+01F4
'G',  //U+01F5
' ',  //U+01F6
' ',  //U+01F7
'N',  //U+01F8
'N',  //U+01F9
'A',  //U+01FA
'A',  //U+01FB
' ',  //U+01FC
' ',  //U+01FD
'O',  //U+01FE
'O',  //U+01FF
'A',  //U+0200
'A',  //U+0201
'A',  //U+0202
'A',  //U+0203
'E',  //U+0204
'E',  //U+0205
'E',  //U+0206
'E',  //U+0207
'I',  //U+0208
'I',  //U+0209
'I',  //U+020A
'I',  //U+020B
'O',  //U+020C
'O',  //U+020D
'O',  //U+020E
'O',  //U+020F
'R',  //U+0210
'R',  //U+0211
'R',  //U+0212
'R',  //U+0213
'U',  //U+0214
'U',  //U+0215
'U',  //U+0216
'U',  //U+0217
'S',  //U+0218
'S',  //U+0219
'T',  //U+021A
'T',  //U+021B
' ',  //U+021C
' ',  //U+021D
'H',  //U+021E
'H',  //U+021F
'N',  //U+0220
'D',  //U+0221
' ',  //U+0222
' ',  //U+0223
'Z',  //U+0224
'Z',  //U+0225
'A',  //U+0226
'A',  //U+0227
'E',  //U+0228
'E',  //U+0229
'O',  //U+022A
'O',  //U+022B
'O',  //U+022C
'O',  //U+022D
'O',  //U+022E
'O',  //U+022F
'O',  //U+0230
'O',  //U+0231
'Y',  //U+0232
'Y',  //U+0233
'L',  //U+0234
'N',  //U+0235
'T',  //U+0236
'J',  //U+0237
' ',  //U+0238
'A',  //U+0239
'C',  //U+023A
'C',  //U+023B
'C',  //U+023C
'L',  //U+023D
'T',  //U+023E
'S',  //U+023F
'Z',  //U+0240
' ',  //U+0241
' ',  //U+0242
'B',  //U+0243
'U',  //U+0244
'V',  //U+0245
'E',  //U+0246
'E',  //U+0247
'J',  //U+0248
'J',  //U+0249
'Q',  //U+024A
'Q',  //U+024B
'R',  //U+024C
'R',  //U+024D
'Y',  //U+024E
'Y'   //U+024F
};
//}
//{ added by jaehwan.lee. table for translation hangul jamo to first char of grouping
wchar_t HANGUL_JAMO_TRANS_TAB[] = {
    0x0000,      //3130
    0x3131,      //3131
    0x3131,      //3132
    0x3131,      //3133
    0x3134,      //3134
    0x3134,      //3135
    0x3134,      //3136
    0x3137,      //3137
    0x3137,      //3138
    0x3139,      //3139
    0x3139,      //313A
    0x3139,      //313B
    0x3139,      //313C
    0x3139,      //313D
    0x3139,      //313E
    0x3139,      //313F
    0x3139,      //3140
    0x3141,      //3141
    0x3142,      //3142
    0x3142,      //3143
    0x3142,      //3144
    0x3145,      //3145
    0x3145,      //3146
    0x3147,      //3147
    0x3148,      //3148
    0x3148,      //3149
    0x314A,      //314A
    0x314B,      //314B
    0x314C,      //314C
    0x314D,      //314D
    0x314E,      //314E
    0x3147,      //314F
    0x3147,      //3150
    0x3147,      //3151
    0x3147,      //3152
    0x3147,      //3153
    0x3147,      //3154
    0x3147,      //3155
    0x3147,      //3156
    0x3147,      //3157
    0x3147,      //3158
    0x3147,      //3159
    0x3147,      //315A
    0x3147,      //315B
    0x3147,      //315C
    0x3147,      //315D
    0x3147,      //315E
    0x3147,      //315F
    0x3147,      //3160
    0x3147,      //3161
    0x3147,      //3162
    0x3147,      //3163
    0x0000,      //3164
    0x3134,      //3165
    0x3134,      //3166
    0x3134,      //3167
    0x3134,      //3168
    0x3139,      //3169
    0x3139,      //316A
    0x3139,      //316B
    0x3139,      //316C
    0x3139,      //316D
    0x3141,      //316E
    0x3141,      //316F
    0x3141,      //3170
    0x3141,      //3171
    0x3142,      //3172
    0x3142,      //3173
    0x3142,      //3174
    0x3142,      //3175
    0x3142,      //3176
    0x3142,      //3177
    0x3142,      //3178
    0x3142,      //3179
    0x3145,      //317A
    0x3145,      //317B
    0x3145,      //317C
    0x3145,      //317D
    0x3145,      //317E
    0x3145,      //317F
    0x3147,      //3180
    0x3147,      //3181
    0x3147,      //3182
    0x3147,      //3183
    0x314D,      //3184
    0x314E,      //3185
    0x314E,      //3186
    0x3147,      //3187
    0x3147,      //3188
    0x3147,      //3189
    0x3147,      //318A
    0x3147,      //318B
    0x3147,      //318C
    0x3147,      //318D
    0x3147,      //318E
    0x0000       //318F
};
//}

wchar_t getAlphabetFromLatinExtend(int ucode)
{
    int start_index = 0x00C0;

    if (ucode < 0x00C0 || ucode > 0x024F )
        return wchar_t(' ');

    return LATIN_EXTEND_ALPAH_TAB[ucode - start_index];
}

wchar_t getFirstCharFromHangulJamo(int ucode)
{
   int start_index = 0x3130;

    if (ucode < 0x3130 || ucode > 0x318F ) //modified from "ucode > 0x3190" to "ucode > 0x318F" by honggi.shin 2013.10.25, "0x3190" can make out-of-bound error. catched by static test.
        return wchar_t(0x0000);

    return HANGUL_JAMO_TRANS_TAB[ucode - start_index];
}

//{ added by jaehwan 2014.01.08 for pinyin sorting
wchar_t CTrackerAbstractor::GetIndexChar(QString title, bool bIPodSort, bool bPinyinSort)
{
    //{ modified by jaehwan.lee 2014.01.02 for pinyin sorting
    if (!title.isEmpty())
    {
        wchar_t IndexChar;
        int key = title.at(0).unicode();

        //LOG_TRACE <<"GetSortingOrder() = "<<GetSortingOrder()<<LOG_ENDL;
        if ( (key >= 0x4E00 && key <= 0x9FFF) && bPinyinSort )
        {
            key = key - 0x4E00;
            if (key >= 0 && key < PINYIN_TABLE_SIZE )
                IndexChar = CMediaDataProvider::pinyinTable[key];
            else
                IndexChar = wchar_t(' ');
        }
        else
        {
            IndexChar = GetFirstChar(title, bIPodSort);
        }
        return IndexChar;
    }
    else
    {
        return wchar_t(' ');
    }
}

wchar_t CTrackerAbstractor::GetFirstChar(QString title, bool bIPodSort)
{
    static const wchar_t koreanTable[19] = { 0x3131, 0x3132, 0x3134, 0x3137, 0x3138, 0x3139, 0x3141,
                                               0x3142, 0x3143, 0x3145, 0x3146, 0x3147, 0x3148, 0x3149,
                                               0x314A, 0x314B, 0x314C, 0x314D, 0x314E };
    static const QString exceptionWords[]={"a", "an", "the"};
    static const QChar exceptionChars[]={'\''};

    title = title.trimmed();

    bool isKorean = true;
    int nKeyCode;
    bool bExceptionWordFound = false;

    while (!title.isEmpty())
    {
        if(title.at(0).isSpace()
                || title.at(0) == 0xAD) //added by junam 2013.12.03 for arabic
        {
            title.remove(0,1);
            continue;
        }

        if(bIPodSort)
        {
            int idx = 0;

            for( idx = 0; idx < int(sizeof(exceptionChars)/sizeof(QChar)); idx++)
            {
                if(title.at(0)== exceptionChars[idx])
                {
                    title.remove(0,1);
                    break;
                }
            }
            if( idx < int(sizeof(exceptionChars)/sizeof(QChar)) )
            {
                continue;
            }

            if(!bExceptionWordFound)
            {
                for( idx = 0; idx < int(sizeof(exceptionWords)/sizeof(QString)); idx++)
                {
                    if(exceptionWords[idx].size() < title.size() && title.startsWith(exceptionWords[idx], Qt::CaseInsensitive))
                    {
                        if(title.at(exceptionWords[idx].size()).isSpace())
                        {
                            title.remove(0,exceptionWords[idx].size());
                            bExceptionWordFound = true;
                            break;
                        }
                    }
                }
                if( idx < int(sizeof(exceptionWords)/sizeof(QString)) )
                {
                    continue;
                }
            }
        }
        break;
    }

    if( title.isEmpty() )
        nKeyCode = QChar(' ').unicode();
    else
        nKeyCode = title.at(0).unicode();

    int UniValue = 0;

    if( nKeyCode >= 0x3130 && nKeyCode <= 0x318F)
    {
        if(!bIPodSort) {
           wchar_t firstChar = getFirstCharFromHangulJamo(nKeyCode);
           if (firstChar == 0x0000)
               return wchar_t(nKeyCode);
           else
               return firstChar;
        }
        UniValue = nKeyCode - 0x3130;
    }
    else if(nKeyCode >= 0xAC00 && nKeyCode <= 0xD7AF )
    {
        UniValue = nKeyCode - 0xAC00;
    }
    else
    {
        UniValue = nKeyCode;
        isKorean = false;
    }

    if( isKorean )
    {
        int final_con = UniValue % 28;
        int initial_con = ( ( UniValue - final_con ) / 28 ) / 21;

        if (initial_con == 1 || initial_con == 4 || initial_con == 8 ||
                initial_con == 10 || initial_con == 13)
        {
            initial_con--;
        }

        if(initial_con < 19)
        {
            return wchar_t(koreanTable[initial_con]);
        }
    }
    else if ( ( nKeyCode >= 0x0041 && nKeyCode <= 0x005A ) //alphabet uppercase
             || ( nKeyCode >= 0x0600 && nKeyCode <= 0x06FF )  // arabic
             )
    {
        return wchar_t(nKeyCode);
    }
    else if ( nKeyCode >= 0x0061 && nKeyCode <= 0x007A ) // alphabet lowercase
    {
        return wchar_t(nKeyCode - ('a'-'A')); // change to upper case  modified jaehwan. 2013.11.12
    }
    else
    {
        // { added by jaehwan to fix ITS 208215 ( symbol/special character grouping)
        if (!bIPodSort)
        {
            if ( (nKeyCode >= 0x0000 && nKeyCode <= 0x001F) /* control characters */ //added by jaehwan 2013.11.18
                || (nKeyCode >= 0x00A0 && nKeyCode <= 0x00B4) /* symbol & puntuation (latin1 supplement) */
                || (nKeyCode >= 0x00B6 && nKeyCode <= 0x00B9) /* symbol & puntuation (latin1 supplement) */
                || (nKeyCode >= 0x00BB && nKeyCode <= 0x00BF) /* symbol & puntuation (latin1 supplement) */
                || (nKeyCode == 0x00D7 || nKeyCode == 0x00F7) /* symbol & puntuation (latin1 supplement) */
                //|| (nKeyCode >= 0x01C0 && nKeyCode <= 0x01C3) /* symbol & puntuation (latin extended B)*/ /* removed by jaehwan 2013.11.14 */
                || (nKeyCode >= 0x02B9 && nKeyCode <= 0x02DF) /* Spacing Modifier Letters */
                || (nKeyCode >= 0x02E4 && nKeyCode <= 0x02FF) /* Spacing Modifier Letters */
                || (nKeyCode >= 0x0300 && nKeyCode <= 0x0362) /* Combining Diacritical Marks  */  /* modified by jaehwan 2013.11.14 */
                || (nKeyCode == 0x0374 || nKeyCode == 0x0375  /* Greek and Coptic */
                    || nKeyCode == 0x037E || nKeyCode == 0x0384
                    || nKeyCode == 0x0385 || nKeyCode == 0x0387)
                || (nKeyCode >= 0x0482 && nKeyCode <= 0x0489) /* Cyrillic */
                || (nKeyCode >= 0x0590 && nKeyCode <= 0x05CF) /* Hebrew */
                || (nKeyCode >= 0x2000 && nKeyCode <= 0x205E) /* General Punctuation */
                || (nKeyCode >= 0x2070 && nKeyCode <= 0x209F) /* Superscripts and subscripts */
                || (nKeyCode >= 0x20A0 && nKeyCode <= 0x20CF) /* Currency symbolx */
                || (nKeyCode >= 0x2100 && nKeyCode <= 0x214F) /* Letterlike Symbols */
                || (nKeyCode >= 0x2150 && nKeyCode <= 0x218F) /* Number Forms */
                || (nKeyCode >= 0x2190 && nKeyCode <= 0x21FF) /* Arrows */
                || (nKeyCode >= 0x2200 && nKeyCode <= 0x22FF) /* Mathematical Operators */
                || (nKeyCode >= 0x2300 && nKeyCode <= 0x23FF) /* Miscellaneous Technical */
                || (nKeyCode >= 0x2460 && nKeyCode <= 0x24FF) /* Enclosed Alphanumerics */
                || (nKeyCode >= 0x2500 && nKeyCode <= 0x257F) /* Box Drawing */
                || (nKeyCode >= 0x25A0 && nKeyCode <= 0x25FF) /* Geometric Shapes */
                || (nKeyCode >= 0x2600 && nKeyCode <= 0x26FF) /* MIscellaneous Symbols */
                || (nKeyCode >= 0x2700 && nKeyCode <= 0x27BF) /* Dingbats */
                || (nKeyCode >= 0x3000 && nKeyCode <= 0x303F) /* CJK Symbols and Punctuation */
                || (nKeyCode >= 0x3200 && nKeyCode <= 0x32FF) /* Enclosed CJK Letters and Months */
                || (nKeyCode >= 0x3372 && nKeyCode <= 0x33FF) /* CJK Compatibility */
            )
            {
                return wchar_t('#');
            }
        }
        //} added by jaehwan
        if ( (nKeyCode >= 0x0021 && nKeyCode <= 0x0040)
                 || (nKeyCode >= 0x005B && nKeyCode <= 0x0060)
                 || (nKeyCode >= 0x007B && nKeyCode <= 0x007E)
                 )
        {
            return wchar_t('#');
        }
        else if ( nKeyCode >= 0x00C0 && nKeyCode <= 0x024F ) // Latin Supplement, Extented..
        {
            wchar_t key = getAlphabetFromLatinExtend(nKeyCode);
            if (key == wchar_t(' '))
                return wchar_t(nKeyCode);
            else
                return wchar_t(key);
        }
        else if ( nKeyCode == 0x00BA)
        {
            return wchar_t('O');
        }
        else
        {
            return wchar_t(nKeyCode);
        }
    }

    return 0;
}
void CTrackerAbstractor::LoadPinyinTable()
{
    CMediaDataProvider::LoadPinyinTable();
}
//} added by jaehwan.lee
