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
* MODULE SUMMARY : This file defines constants    *
*                  used by different content providers and applications     *
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

#ifndef DHAVN_TRACKERABSTRACTOR_CONSTANTS_H
#define DHAVN_TRACKERABSTRACTOR_CONSTANTS_H

#include <QString>
#include <QMap>
#include <QStringList>

const int  MAX_QUEUE_SIZE =  10;
const int  MAX_TIME_LIMIT = 30000;
#define EVOLUME_TYPE_SIZE 10
#define QUERY_OFFSET 		0
#define QUERY_LIMIT 		10
#define NUM_NUMBER_TBL  	(1)
#define NUM_ENGLISH_TBL  	(26)
#define NUM_KOREAN_TBL  	(14)
#define NUM_EXTRA_TBL  		(4)
#define NUM_UNICODE_TBL  	(NUM_NUMBER_TBL+NUM_ENGLISH_TBL+NUM_KOREAN_TBL+NUM_EXTRA_TBL)

/**
   * Enums for volume type used to identify respective volume
   */
enum ETrackerAbstarctor_VolumeType
{
    eUSB = 0,       /**< USB volume type used for volume queries */
    eJukeBox,        /**< JukeBox volume type used for volume queries */
    eIPod,              /**< IPod volume type used for volume queries */
    eDisc,               /**< Disc volume type used for volume queries */
    eUSB_FRONT,
    eUSB_REAR,
    eIPOD_FRONT,
    eIPOD_REAR,
    eDefaultVolume  /** <For Common functionality which may or may not require Volumes */
};

/**
   * Enums for order type used for get all songs with metadata from voulume query
   */
enum ETrackerAbstarctor_OrderType
{
    eAlbum = 0,       /**< Order by album name */
    eArtist,       /**< Order by artist name */
    eGenre,     /**< Order by genre */
    eTitle,     /**< Order by song title */
    eUrl        /**< Order by song url */

};

enum ETrackerAbstarctor_ErrorType
{
    eNullStringParameter = 0,
    eEmptyString,
    eErrorInQueryExecution,
    eErrorInQuery,
    eQueryMappingError,
    eCoverArtImageNotFound,
    eErrorEmptyURL,
    eErrorNoThumbnailData,
    eErrorDirPathNotExists,
    eErrorDeviceId
};

// { added by kihyung 2013.08.07 for ITS 0181967 
enum ETrackerAbstarctor_MediaType
{
    eFSAudio = 0,
    eFSVideo, 
    eFSPhoto
};

struct FSFolderInfo
{
    unsigned int    nFileSize;
    unsigned int    nStartIdx;
    unsigned int    nErrorSize;
};
// } added by kihyung 2013.08.07 for ITS 0181967 

/**
   * volume name strings
   */
const QString gVolumeName_USB = "USB";
const QString gVolumeName_JukeBox = "JUKEBOX";

const QString gVolumePath_JukeBox = "file:///usr_data/media";
const QString gVolumePath_USB = "file:///media";
const QString gVolumePath_IPod = "file:///ipod";
const QString gVolumePath_DISC = "file:///cd";
const QString gPodcastTypeName = "Podcast";
const QString gAudioBookTypeName = "AudioBook";
const QString gGracenotePlaylistTypeName = "MoreLikeThis";
const QString gIpodPlaylistTypeName = "PlayList";
const QString FilePath_Prefix = "file://";


/**
   * Blank space string
   */
const QString gBlankSpace = " ";

/**
   * Escape character for blank space
   */
const QString gSpaceEscapeChar = "%20";
/* Signle Quote String */
const QString gSingeQuote = "\'";
/* Signle Quote String Escape */
const QString gSingeQuoteEscapeChar = "\\'";

/* Double Quote String */
const QString gDoubleQuote = "\"";
/* Double Quote String Escape */
const QString gDoubleQuoteEscapeChar = "\\'";
/* Underscore */
const QString gUnderscoreChar = "_";
/* ForwardSlace */
const QString gForwardSlace = "/";
/**
   * prefix character used in search
   */
const char gPrefixChar = '*';

const QString gPlusSign = "+";
const QString gPlusSignCorrected = "[+]";

const QString gLBracketSign = "(";
const QString gLBracketSignCorrected = "[(]";

const QString gRBracketSign = ")";
const QString gRBracketSignCorrected = "[)]";

const QString gDollarSign = "$";
const QString gDollarSignCorrected = "[$]";

/**
   * Mapping keys for Query Id enums used by data provider slot
   */
/*const QString gSongsByFoderQuery_Key = "gSongsByFolderQuery";
const QString gAlbumByFolderQuery_Key = "gAlbumByFolderQuery";
const QString gArtistByFolderQuery_Key = "gArtistByFolderQuery";
const QString gGetAllSongsQuery_Key = "gGetAllSongsQuery";
const QString gFolderListQuery_Key = "gFolderListQuery";
const QString gMusicAlbumListQuery_Key = "gMusicAlbumListQuery";
const QString gGetAllAlbumsQuery_Key = "gGetAllAlbumsQuery";
const QString gGetAllMusicArtistsQuery_Key = "gGetAllMusicArtistsQuery";
const QString gGetAllMusicComposersQuery_Key = "gGetAllMusicComposersQuery";
const QString gGetAllMusicGenresQuery_Key = "gGetAllMusicGenresQuery";
const QString gGetMusicMetaDataQuery_Key = "gGetMusicMetaDataQuery";
const QString gGetAllSongsByAlbumQuery_Key =  "gSongsByAlbumQuery";
const QString gGetAllSongsByArtistQuery_Key = "gGetAllSongsByArtistQuery";
const QString gGetAllSongsByGenreQuery_Key = "gGetAllSongsByGenreQuery";
const QString gGetAllSongsByComposerQuery_Key = "gGetAllSongsByComposerQuery";
const QString gGetImagesByFolderQuery_Key = "gGetImagesByFolderQuery";
const QString gGetVideosByFolderQuery_Key = "gGetVideosByFolderQuery";
const QString gGetAllMusicByKeywordQuery_Key = "gGetAllMusicByKeywordQuery";
const QString gGetMultipleAlbumSongsQuery_Key = "gGetMultipleAlbumSongsQuery";
const QString gGetMultipleArtistSongsQuery_Key = "gGetMultipleArtistSongsQuery";
const QString gGetMusicAlbumCoverArt_key = "gGetMusicAlbumCoverArt";
const QString gGetAllImagesByKeywordQuery_key = "gGetAllImagesByKeywordQuery";
const QString gGetAllVideosByKeywrodQuery_key = "gGetAllVideosByKeywrodQuery";
const QString gSongsCountByFolderQuery_Key = "gSongsCountByFolderQuery";
const QString gGetImagesCountByFolderQuery_Key = "gGetImagesCountByFolderQuery";
const QString gGetVideosCountByFolderQuery_Key = "gGetVideosCountByFolderQuery";
const QString gGetAllSongsCountQuery_Key = "gGetAllSongsCountQuery";
const QString gGetAllSongsFromVolumeQuery_Key = "gGetAllSongsFromVolumeQuery";
const QString gGetAllAlbumsFromVolumeQuery_Key = "gGetAllAlbumsFromVolumeQuery";
const QString gGetAllMusicArtistsFromVolumeQuery_Key = "gGetAllMusicArtistsFromVolumeQuery";
const QString gGetAllMusicComposersFromVolumeQuery_Key = "gGetAllMusicComposersFromVolumeQuery";
const QString gGetAllMusicGenresFromVolumeQuery_Key = "gGetAllMusicGenresFromVolumeQuery";
const QString gGetAllImagesFromVolumeQuery_Key = "gGetAllImagesFromVolumeQuery";
const QString gGetAllVideosFromVolumeQuery_Key = "gGetAllVideosFromVolumeQuery";
const QString gGetAllSongsIDQuery_Key = "gGetAllSongsIDQuery";
const QString gGetAllSongsWithMetaDataFromVolumeQuery_Key =  "gGetAllSongsWithMetaDataFromVolumeQuery";
const QString gGetAllSongsByGenreArtistAlbumQuery_Key = "gGetAllSongsByGenreArtistAlbumQuery";
const QString gGetAllSongsByGenreComposerAlbumQuery_Key = "gGetAllSongsByGenreComposerAlbumQuery";
const QString gGetAllSongsByArtistAlbumQuery_Key = "gGetAllSongsByArtistAlbumQuery";
const QString gGetAllAlbumByGenreArtistQuery_Key = "gGetAllAlbumByGenreArtistQuery";
const QString gGetAllAlbumByArtistQuery_Key = "gGetAllAlbumByArtistQuery";
const QString gGetAllArtistByGenreQuery_Key = "gGetAllArtistByGenreQuery";

const QString gGetAllSongsbyPlaylistQuery_Key="gGetAllSongsbyPlaylistQuery";
const QString gGetlistofPlaylistsQuery_Key="gGetlistofPlaylistsQuery";
const QString gGetMostPlayedMusicSongsQueryy_Key="gGetMostPlayedMusicSongsQuery";
const QString gGetMostRecentlyPlayedSongsQuery_Key="gGetMostRecentlyPlayedSongsQuery";
const QString gGetMusicPlaylistByKeywordQuery_Key="gGetMusicPlaylistByKeywordQuery";

const QString gGetAllSongsByKeywordFromVolumeQuery_Key = "gGetAllSongsByKeywordFromVolumeQuery";
const QString gGetAllArtistsByKeywordFromVolumeQuery_Key = "gGetAllArtistsByKeywordFromVolumeQuery";
const QString gGetAllAlbumsByKeywordFromVolumeQuery_Key = "gGetAllAlbumsByKeywordFromVolumeQuery";
const QString gGetAllGenresByKeywordFromVolumeQuery_Key = "gGetAllGenresByKeywordFromVolumeQuery";
const QString gGetAllSongsByAlbumFromVolumeQuery_Key="gGetAllSongsByAlbumFromVolumeQuery";

const QString gGetAllPodcastsByTitleQuery_Key = "gGetAllPodcastsByTitleQuery";
const QString gGetAllAudiobooksByTitleQuery_Key = "gGetAllAudiobooksByTitleQuery";
const QString gGetAllPodcastsEpisodeNamesQuery_Key = "gGetAllPodcastsEpisodeNamesQuery";
const QString gGetAllAudiobookChapterNamesQuery_Key = "gGetAllAudiobookChapterNamesQuery";
const QString gGetAllVideosCountFromVolumeQuery_Key = "gGetAllVideosCountFromVolumeQuery";
const QString gGetAllImagesCountFromVolumeQuery_Key = "gGetAllImagesCountFromVolumeQuery";
const QString gGetMusicFromCDQuery_Key = "gGetMusicFromCDQuery";
const QString gGetMusicMetaDataFromCDQuery_Key = "gGetMusicMetaDataFromCDQuery";
const QString gGetAllSongsByComposerAlbumQuery_Key ="gGetAllSongsByComposerAlbumQuery";
const QString gGetAllAlbumByGenreComposerQuery_Key="gGetAllAlbumByGenreComposerQuery";
const QString gGetAllAlbumByComposerQuery_Key ="gGetAllAlbumByComposerQuery";
const QString gGetAllComposerByGenreQuery_Key = "gGetAllComposerByGenreQuery";
const QString gGetMusicDetailFromIpodOnIndexBasisQuery_Key = "gGetMusicDetailFromIpodOnIndexBasisQuery";

const QString gGetImagesBySubFolderQuery_Key = "gGetImagesBySubFolderQuery";
const QString gGetVideosBySubFolderQuery_Key = "gGetVideosBySubFolderQuery";

const QString gGetAllAlbumsWithCoverArtFromVolumeQuery_Key = "gGetAllAlbumsWithCoverArtFromVolumeQuery";
const QString gGetAllSongsCountFromVolumeQuery_key = "gGetAllSongsCountFromVolumeQuery";
const QString gGetSongsCountWithMetaInfoFromVolumeQuery_key = "gGetSongsCountWithMetaInfoFromVolumeQuery";
const QString gGetAllSongsWithoutMetaInfoFromVolumeQuery_key = "gGetAllSongsWithoutMetaInfoFromVolumeQuery";
const QString gGetAllMusicByKeywordFromVolumeQuery_Key = "gGetAllMusicByKeywordFromVolumeQuery";
const QString gGetMusicMetaDataInfoFromUrlListQuery_Id = "gGetMusicMetaDataInfoFromUrlListQuery";  

const QString gGetAllAlbumsWithUrlFromVolumeQuery_Key = "gGetAllAlbumsWithUrlFromVolumeQuery";

const QString gGetAllSongsWithTitleCountFromVolumeQuery_Key = "gGetAllSongsWithTitleCountFromVolumeQuery";
const QString gGetAllAlbumsCountFromVolumeQuery_Key = "gGetAllAlbumsCountFromVolumeQuery";
const QString gGetAllMusicArtistsCountFromVolumeQuery_Key = "gGetAllMusicArtistsCountFromVolumeQuery";
const QString gGetAllMusicComposersCountFromVolumeQuery_Key = "gGetAllMusicComposersCountFromVolumeQuery";
const QString gGetAllMusicGenresCountFromVolumeQuery_Key = "gGetAllMusicGenresCountFromVolumeQuery";
const QString gGetAllAlbumsWithCoverArtCountFromVolumeQuery_Key = "gGetAllAlbumsWithCoverArtCountFromVolumeQuery";

const QString gGetAllSongsWithLimitFromVolumeQuery_Key = "gGetAllSongsWithLimitFromVolumeQuery";
const QString gGetAllAlbumsWithLimitFromVolumeQuery_Key = "gGetAllAlbumsWithLimitFromVolumeQuery";
const QString gGetAllMusicArtistsWithLimitFromVolumeQuery_Key = "gGetAllMusicArtistsWithLimitFromVolumeQuery";
const QString gGetAllMusicComposersWithLimitFromVolumeQuery_Key = "gGetAllMusicComposersWithLimitFromVolumeQuery";
const QString gGetAllMusicGenresWithLimitFromVolumeQuery_Key = "gGetAllMusicGenresWithLimitFromVolumeQuery";
const QString gGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Key = "gGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery";

const QString gGetAllSongsWithTitleIndexFromVolumeQuery_Key = "gGetAllSongsWithTitleIndexFromVolumeQuery";
const QString gGetAllMusicArtistsIndexFromVolumeQuery_Key = "gGetAllMusicArtistsIndexFromVolumeQuery";
const QString gGetAllMusicGenresIndexFromVolumeQuery_Key = "gGetAllMusicGenresIndexFromVolumeQuery";
const QString gGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Key = "gGetAllAlbumsWithCoverArtIndexFromVolumeQuery";
*/
/**
   * Enums for query Id used to identify respective query
   */
enum ETrackerAbstarctor_Query_Ids
{
    eDefault_Id = 0,    /**< Default Id for intialization */
    eSongsByFolderQuery_Id = 1,    /**< Songs by folder query id Count */
    eAlbumByFolderQuery_Id = 2,     /**< Album by folder query id */
    eArtistByFolderQuery_Id = 3,    /**< Artist by folder query id */
    eGetAllSongsQuery_Id = 4,   /**< All songs query id */
    eFolderListQuery_Id = 5,    /**< Folder list query id */
    eMusicAlbumListQuery_Id = 6,    /**< Album list query id  */
    eSongsByNextAlbumQuery_Id = 7,  /**< Songs by next album query id */
    eSongsByPrevAlbumQuery_Id = 8,  /**< Songs by previous album query id */
    eGetAllAlbumsQuery_Id = 9,  /**< All albums query id */
    eGetAllMusicArtistsQuery_Id = 10,    /**< All music artists by folder query id */
    eGetAllMusicComposersQuery_Id = 11,     /**< All music composers query id */
    eGetAllMusicGenresQuery_Id = 12,     /**< All music genres query id */
    eGetMusicMetaDataQuery_Id = 13,       /**< music metadata query id */
    eGetAllSongsByAlbumQuery_Id = 14,      /**< All songs by album query id */
    eGetAllSongsByArtistQuery_Id = 15,        /**< All songs by artist query id */
    eGetAllSongsByGenreQuery_Id = 16,        /**< All songs by genre query id */
    eGetAllSongsByComposerQuery_Id = 17,        /**< All songs by composer query id */
    eGetImagesByFolderQuery_Id = 18,        /**< Images by folder query id */
    eGetVideosByFolderQuery_Id = 19,        /**< Videos by folder query id */
    eGetAllMusicByKeywordQuery_Id = 20,        /**< All Music by keyword query id */
    eGetMultipleArtistSongsQuery_Id = 21,      /** <All Songs by LISt of Artists */
    eGetMultipleAlbumSongsQuery_Id = 22,        /** <All Songs by LISt of Albums */
    eGetMusicAlbumCoverArt_Id = 23,              /** <Album CoverArt for List of Albums */
    eGetMusicTrackCoverArt_Id = 24,              /** <Track CoverArt for List for tracks */
    eGetImageThumbNail_Id = 25,                   /** <Thumbnail for the image */
    eGetAllImagesByKeywordQuery_Id = 26,         /** <Search for all Images  by Keyword */
    eGetAllVideosByKeywrodQuery_Id = 27,          /** <Search for all videos  by Keyword */
    eGetVideoFileThumbNail_Id = 28,                   /** <Thumbnail for the Video file */
    eGetVideoFileListThumbNail_Id = 29,                /** <Thumbnail for the Video file list */
    eSongsCountByFolderQuery_Id = 30,                /** <Songs count by folder query id */
    eGetImagesCountByFolderQuery_Id = 31,                /** <Images count by folder query id */
    eGetVideosCountByFolderQuery_Id = 32,                /** <Videos count by folder query id */
    eGetAllSongsCountQuery_Id = 33,                /** <All songs count query id */
    eGetAllSongsFromVolumeQuery_Id = 34,                /** <All songs from volume query id */
    eGetAllAlbumsFromVolumeQuery_Id = 35,                /** <All albums from volume query id */
    eGetAllMusicArtistsFromVolumeQuery_Id = 36,                /** <All artists from volume query id */
    eGetAllMusicComposersFromVolumeQuery_Id = 37,                /** <All composers from volume query id */
    eGetAllMusicGenresFromVolumeQuery_Id = 38,                /** <All genres from volume query id */
    eGetAllImagesFromVolumeQuery_Id = 39,                /** <All images from volume query id */
    eGetAllVideosFromVolumeQuery_Id = 40,                /** <All videos from volume query id */
    eGetAllSongsIDQuery_id = 41,          /** All songs url , title and ID query id needed for GracenotePluginProcess */
    eGetAllSongsWithMetaDataFromVolumeQuery_Id = 42,                /** <All songs with metadata from volume query id */
    eGetAllSongsByGenreArtistAlbumQuery_Id = 43,                /** <All songs by genre artist album query id */
    eGetAllSongsByGenreComposerAlbumQuery_Id = 44,                /** <All songs by genre composer album query id */
    eGetAllSongsByArtistAlbumQuery_Id = 45,                /** <All songs by  Artist album query id */
    eGetAllAlbumByGenreArtistQuery_Id = 46,                /** <All Album by genre Artist query id */
    eGetAllAlbumByArtistQuery_Id = 47,                /** <All Album by Artist query id */
    eGetAllArtistByGenreQuery_Id = 48,                /** <All Artist by Genre query id */	
    eGetAllsongsByKeywordFromVolumeQuery_Id = 49,   /** <All songs matching by keyword from volume query id */
    eGetAllArtistsByKeywordFromVolumeQuery_Id = 50, /** <All Artists matching by keyword from volume query id */
    eGetAllAlbumsByKeywordFromVolumeQuery_Id = 51,  /** <All Albums matching by keyword from volume query id */
    eGetAllGenresByKeywordFromVolumeQuery_Id = 52,   /** <All Genres matching by keyword from volume query id */
    eGetAllSongsByAlbumFromVolumeQuery_Id = 53,     /** <All Songs matching by keyword from volume query id */
    eGetAllSongsbyPlaylistQuery_Id = 54,             /** <All Songs by Playlist query id */
    eGetlistofPlaylistsQuery_Id = 55,               /** <All list of Playlists query id */
    eGetMostPlayedMusicSongsQuery_Id = 56,          /** <All MostPlayedMusicSongs query id */
    eGetMostRecentlyPlayedSongsQuery_Id = 57,        /**  <All  MostRecentlyPlayedSongs query id */
	eGetMusicPlaylistByKeywordQuery_Id = 58,    /** <All MusicPlaylist By Keyword query id */    
	eGetAllSongsByComposerAlbumQuery_Id = 59, /**  <All  SongsByComposerAlbumQuery query id */
    eGetAllAlbumByGenreComposerQuery_Id = 60, /**  <All  AlbumByGenreComposerQuery query id */
    eGetAllAlbumByComposerQuery_Id = 61,      /**  <All  AlbumByComposerQuery query id */
    eGetAllComposerByGenreQuery_Id = 62,      /**  <All   ComposerByGenreQuery query id */
	/** <Get all the songs detail of the ipod between two index values
				1.	Title
				2.	Artist
				3.	Composer
				4.	musicAlbum
				5.	genre
				6.	TrackUid
				7.	TrackIndex
				8.	url
				9.	TypeName
					>  */
	eGetMusicDetailFromIpodOnIndexBasis_Id = 63,
//*****************************Insertion*************************

    /** <insert all Tracks by keyword query id
            Structure to be used: SAppleInfoList
             API to be used: CTrackerAbstractor::UpdateData()> */
    eInsertIPodMusicTracksToDBaseQuery_Id = 64,

    /** <insert the Podcast by keyword query id
            Structure to be used: SAppleHeirarchyInfoList
             API to be used: CTrackerAbstractor::UpdateData()> */
    eInsertIPodPodcastToDBaseQuery_Id = 65,

    /** <insert the AudioBook by keyword query id
            Structure to be used: SAppleHeirarchyInfoList
             API to be used: CTrackerAbstractor::UpdateData()> */
    eInsertIPodAudioBookToDBaseQuery_Id = 66,

/** <insert the  MusicPlaylist by keyword query id
            Structure to be used: SAppleHeirarchyInfoList
             API to be used: CTrackerAbstractor::UpdateData()>  */

    eInsertIPodMusicPlaylistToDBaseQuery_Id = 67,

    /** <remove the Track/song by keyword query id
            Structure to be used:
            struct SIPodTrackRemoveStruct
            {
            QString SerialNumber;
            int TrackIndex;
            int TrackUID;
            }
             API to be used: CTrackerAbstractor::UpdateData()>  */
    eRemoveIPodMusicTrackDataQuery_Id = 68,

    /** <remove the Podcast by keyword query id
            Structure to be used:
            struct SIPodTypeRemoveInfo
            {
            QString SerialNumber;
            QString IPodTypeName;
            }
             API to be used: CTrackerAbstractor::UpdateData()>  */
    eRemoveIPodPodcastDataQuery_Id = 69,

    /** <remove the AudioBook by keyword query id
            Structure to be used:
            struct SIPodTypeRemoveInfo
            {
            QString SerialNumber;
            QString IPodTypeName;
            }
             API to be used: CTrackerAbstractor::UpdateData()> */
    eRemoveIPodAudioBookDataQuery_Id = 70,

    /** <remove the Playlist by keyword query id
            Structure to be used:
            struct SIPodTypeRemoveInfo
            {
            QString SerialNumber;
            QString PodcastAudioBookPlaylistName;
            }
             API to be used: CTrackerAbstractor::UpdateData()>  */
    eRemoveIPodPlaylistDataQuery_Id = 71,

    /** <remove all information by keyword query id
            Structure to be used: QString - serial Number
             API to be used: CTrackerAbstractor::UpdateData()> */
    eRemoveAllIPodInfoQuery_Id = 72,
	eGetAllPodcastsByTitleQuery_Id = 73,
	eGetAllAudiobooksByTitleQuery_Id = 74,
	eGetAllPodcastsEpisodeNamesQuery_Id = 75,
	eGetAllAudiobookChapterNamesQuery_Id = 76,
    eGetAllVideosCountFromVolumeQuery_Id = 77, /** <Get all videos count from volume query id> */
    eGetAllImagesCountFromVolumeQuery_Id = 78, /** <Get all images count from volume query id> */
	eGetMusicFromCDQuery_Id = 79, /** Get info for CD songs  **/
	eInsertAlbumCoverArtDataQuery_Id = 80,   /** Insert Album Cover art  **/
	eGetMusicMetaDataFromCDQuery_Id = 81, /** Get info about a track for CD songs **/
	eIsCoverArtExists_Id = 82, /** Check for Cover Art with album & artist names */
    eGetImagesBySubFolderQuery_Id = 83,
    eGetVideosBySubFolderQuery_Id = 84,
    eGetAllAlbumsWithCoverArtFromVolumeQuery_Id = 85,
	eInsertGracenoteMetadataQuery_Id = 86,  /** Insert Gracenote Metadata in Tracker Store */
	eDeleteGracenoteCDMetadataQuery_Id = 87,  /** Insert Gracenote Metadata in Tracker Store */	
/**
   *	putting query id for File listing from QDir 98
   */
	eGetVideoFileListDir_Id = 88,
	eGetMusicFileListDir_Id = 89,
	eGetPhotoFileListDir_Id = 90, 
/**
   *	putting query id for file listing from QDir only for 10 files 101
   */
	eGetVideoLimitFileListDir_Id = 91,
	eGetMusicLimitFileListDir_Id = 92,
	eGetPhotoLimitFileListDir_Id = 93,
    eGetAllSongsWithoutAlbumFromVolumeQuery_Id = 94,      /** All songs from Volume Order As per MetaData (no Metadata comes first) **/
    eIsThereAnySongWithoutAlbumFromVolumeQuery_Id = 95,   /** Is there any song from Volume Order As per MetaData (no Metadata comes first) **/
	eGetAllMusicByKeywordFromVolumeQuery_Id = 96,         /**< All Music by keyword from volume query id */
    eInsertXSampeData_Id = 97, 
    eRetrieveXSampaData_Id = 98, 
    eCheckExistenceOfXSampaData_Id = 99, 
    eGetMusicMetaDataInfoFromUrlListQuery_Id = 100, 
	eRetrieveAllXSampaData_Id = 101, 
    eRetrieveXSampaDataForType_Id = 102,
	eRemoveAllMediaDataFromVolumeQuery_Id = 103, 
    eRetrieveCoverArtOnDemand_Id = 104, 
    eGetAllAlbumsWithUrlFromVolumeQuery_Id = 105, 
	eGetAllSongsByArtistFromVolumeQuery_Id = 106,      /**< All Songs By Aritst from volume query id */
	eGetAllSongsByGenreFromVolumeQuery_Id = 107,
    eGetAllSongsCountFromVolumeQuery_Id = 108,		            /** Count of All songs from volume */
    eGetAllSongsWithTitleCountFromVolumeQuery_Id = 109,			/** The number of all songs with title from volume query id */
    eGetAllAlbumsCountFromVolumeQuery_Id = 110,					/** The number of all blbums from volume query id */
    eGetAllMusicArtistsCountFromVolumeQuery_Id = 111,				/** The number of all music artitsts from volume query id */
    eGetAllMusicComposersCountFromVolumeQuery_Id = 112,			/** The number of all music composers from volume query id */
    eGetAllMusicGenresCountFromVolumeQuery_Id = 113,				/** The number of all music genres from volume query id */
    eGetAllAlbumsWithCoverArtCountFromVolumeQuery_Id = 114,		/** The number of all albums with cover art from volume query id */
    eGetAllSongsWithLimitFromVolumeQuery_Id = 115, 
    eGetAllAlbumsWithLimitFromVolumeQuery_Id = 116, 
    eGetAllMusicArtistsWithLimitFromVolumeQuery_Id = 117, 
    eGetAllMusicComposersWithLimitFromVolumeQuery_Id = 118, 
    eGetAllMusicGenresWithLimitFromVolumeQuery_Id = 119, 
    eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id = 120, 
    eGetAllSongsWithTitleIndexFromVolumeQuery_Id = 121, 
    eGetAllMusicArtistsIndexFromVolumeQuery_Id = 122, 
    eGetAllMusicGenresIndexFromVolumeQuery_Id = 123, 
    eGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Id = 124, 
	eGetMultipleAlbumSongsByArtistFromVolumeQuery_Id = 125,  /**< Songs by arstist from multiple albums */
    eGetMultipleGenreSongsFromVolumeQuery_Id = 126,        /**< Songs by multiple genre */
    eGetMultipleArtistSongsByGenreFromVolumeQuery_Id = 127,        /**< Songs by genre from multiple artist */
    eGetMultipleAlbumSongsByGenreArtistFromVolumeQuery_Id = 128,       /**< Songs by genre and artist from multiple albums */
    eGetMultipleFolderSongsQuery_Id = 129,     /**< Songs by multiple folder paths */
    eGetMultipleAlbumSongsFromVolumeQuery_Id = 130,        /** <All Songs by list of Albums */
    eGetMultipleArtistSongsFromVolumeQuery_Id = 131,    /** <All Songs by list of Artists */
    eGetAlliTunesUByTitleQuery_Id = 132, 
    eGetAlliTunesUEpisodeNamesQuery_Id = 133, 
    eGetFolderTabList_Id = 134,        /** <music folder/file list */
    eGetXSampaId3NameQuery_Id = 135, 
    eGetSongIndexFromVolumeQuery_Id = 136, 
    eGetAlbumIndexFromVolumeQuery_Id = 137, 
    eGetArtistIndexFromVolumeQuery_Id = 138, 
    eGetAllMetaDataWithLimitFromVolumeQuery_Id = 139, 
    eGetAllMusicByConnectedVolumesQuery_Id = 140,  /** <All music by connected voulme */
    eUpdateCIDInfoInXSampaDB = 141, /**Updates CID info in XSampa DB  */
    eTrackerExtractMetadataQuery_Id = 142,  
    eGetPhotoMetaDataInfoFromUrlQuery_Id = 143, 
	/* ---- PhoneBook Data Provider - SQLite ---- */
    eGetAllPhoneContactsQuery_Id = 300,
	eGetAllRecentCallQuery_Id = 301,
	eGetAllReceivedCallQuery_Id = 302,
	eGetAllMissedCallQuery_Id = 303,
	eGetAllCallHistoryQuery_Id = 304,
	eGetAllFavoriteContactsQuery_Id = 305,
	/* ---- PhoneBook Data Search - SQLite ---- */
    eGetContactDetailsByNameQuery_Id = 310,
    eGetAllContactsByKeywordForBDAdressQuery_Id = 311,
    //eGetContactByNumberQuery_Id = 312,
    eGetContactByNumberForForeignQuery_Id = 312,
    eGetContactByNumberForDomesticQuery_Id = 313,
    eGetFavoriteContactByNameAndNumberQuery_Id = 314,
    eGetContactByCallHistoryInfoQuery_Id = 315,
        //eGetContactByNumberForCallHistoryNameQuery_Id = 315,
    eGetContactByNumberForForeignCallHistoryNameQuery_Id = 316,
    eGetContactByNumberForDomesticCallHistoryNameQuery_Id = 317,
    eGetContactByCallHistoryInfoQuery_Id_KR_NA = 318,
        //eGetContactByNumberForCallHistoryNameQuery_Id_KR_NA = 317,
    eGetContactByNumberForCallHistoryNameQuery_Id_KR = 319,
    eGetContactByNumberForCallHistoryNameQuery_Id_CN = 320,
    eGetContactByNumberForCallHistoryNameQuery_Id_NA = 321,
    eGetPhoneBookHashValueByBDAddressQuery_Id = 322,

    eGetContactByNumberQuery_Id_KR = 323,
    eGetContactByNumberQuery_Id_CN = 324,
    eGetContactByNumberQuery_Id_NA = 325,
    /* ---- PhoneBook Data Count - SQLite ---- */

    /////SUPPORT_MIDDLE_NAME
    eMidGetContactByNumberForDomesticQuery_Id = 306,
    eMidGetContactByNumberForDomesticCallHistoryNameQuery_Id = 307,
    //eMidGetContactByNumberForForeignCallHistoryNameQuery_Id = 308,
    eMidGetAllFavoriteContactsForForeignQuery_Id = 308,
    eMidGetAllFavoriteContactsForDomesticQuery_Id = 309,
    eMidGetAllPhoneContactsForForeignQuery_Id = 326,
    eMidGetAllPhoneContactsForDomesticQuery_Id = 327,
    eMidGetContactByCallHistoryInfoQuery_Id = 328,
    eMidGetFavoriteContactByNameAndNumberQuery_Id = 329,
    //eMidGetContactByCallHistoryInfoQuery_Id_KR_NA = 329,
    /////SUPPORT_MIDDLE_NAME
    ePinyinGetAllSelectedPhoneContactsForDomesticQuery_id = 357,
    /* FEATURE_ORDER_BY_PINYIN */

    eGetAllPhoneContactCountQuery_Id = 330,
    eGetAllRecentCallCountQuery_Id = 331,
    eGetAllReceivedCallCountQuery_Id = 332,
    eGetAllMissedCallCountQuery_Id = 333,
    eGetAllCallHistoryCountQuery_Id = 334,
    eGetAllFavoriteContactCountQuery_Id = 335,
    eGetAllPhoneContactsForForeignQuery_Id = 336,
    eGetAllPhoneContactsForDomesticQuery_Id = 337,
	eGetAllFavoriteContactsForForeignQuery_Id = 338,
	eGetAllFavoriteContactsForDomesticQuery_Id = 339,    
	/* ---- PhoneBook Data Updater - SQLite ---- */
    eInsertContactsToDBaseQuery_Id = 340,
    eRemovePhoneBookDataBySelectionQuery_Id = 341,
    eRemoveContactDataQuery_Id = 342,
    eRemoveContactDataByIndexQuery_Id = 343,
    /* etc */
    eGetVideoFolderTabList_Id = 344,
    eGetPhotoFolderTabList_Id = 345,
    eGetMusicSimpleMetaDataQuery_Id = 346, //added by junam 2013.03.27 for ISV72425
    eGetFirstImagePathByGivenFolder = 347, // added by kihyung 2013.07.23 for ITS 0178868
    eGetImageCountByGivenFolder = 348, // added by kihyung 2013.07.23 for ITS 0178868
    eGetFirstImageAndCountByGivenFolder = 349, // added by kihyung 2013.07.23 for ITS 0178868
    eGetSongIndexWithTitleCountFromVolumeQuery_Id = 350,    // { added by sungha.choi 2013.07.23
    eGetAlbumIndexWithAlbumCountFromVolumeQuery_Id = 351,
    eGetArtistIndexWithArtistCountFromVolumeQuery_Id = 352,
    eGetAllSongsWithLimitCacheFromVolumeQuery_Id = 353,
    eGetAllAlbumsWithCoverArtWithLimitCacheFromVolumeQuery_Id = 354,
    eGetAllMusicArtistsWithLimitCacheFromVolumeQuery_Id = 355,  // } added by sungha.choi 2013.07.23
    eGetMusicNormalMetaDataQuery_Id = 356,  //  added by sungha.choi 2013.09.06 for ITS 0188361
};

enum ETrackerAbstarctor_RequestStates
{
    eDefaultRequest = 0,
    eProcessRequest,
    eWaitRequest,
    eCancelRequest,
    eProcesscomplete
};

enum ETrackerAbstractor_RequestData
{
    eRequest_default = 0,
    eRequest_NoVol_NoParam ,
    eRequest_NoVol_StringParam,
    eRequest_NoVol_List_StringParam,
    eRequest_NoVol_singleListParam,
    eRequest_NoVol_TwoListParams,
    eRequest_NoVol_ThreeListParams,
    eRequest_NoVol_TwoStringParams,
    eRequest_NoVol_TwoIntParams,
    eRequest_Vol_NoParam,
    eRequest_Vol_SingleListParam,
    eRequest_Vol_StringParam,
    eRequest_Vol_OrdParam,
    eRequest_Vol_TwoListParams,
    eRequest_NoVol_String_List_boolParam

};

// Unicode Table for a letter
const ushort  Letter_Tbl[NUM_UNICODE_TBL] = {
    0x0020, // ' ' //0x0000, //changed by junam 2013.07.25 for ITS_EU_180117
    0x0023, // '#' (Numeric)
    0x0000, // NULL
	0x0041, // 'A
	0x0042, // 'B
	0x0043, // 'C'
	0x0044, // 'D'
	0x0045, // 'E'
	0x0046, // 'F'
	0x0047, // 'G'
	0x0048, // 'H'
	0x0049, // 'I'
	0x004A, // 'J'
	0x004B, // 'K'
	0x004C, // 'L'
	0x004D, // 'M'
	0x004E, // 'N'
	0x004F, // 'O'
	0x0050, // 'P'
	0x0051, // 'Q'
	0x0052, // 'R'
	0x0053, // 'S'
	0x0054, // 'T'
	0x0055, // 'U'
	0x0056, // 'V'
	0x0057, // 'W'
	0x0058, // 'X'
	0x0059, // 'Y'
	0x005A,	// 'Z'
	0x0000, // NULL
    0x3131, // 'ㄱ'
    0x3134, // 'ㄴ'
    0x3137, // 'ㄷ'
    0x3139, // 'ㄹ'
    0x3141, // 'ㅁ'
    0x3142, // 'ㅂ'
    0x3145, // 'ㅅ'
    0x3147, // 'ㅇ'
    0x3148, // 'ㅈ'
    0x314A, // 'ㅊ'
    0x314B, // 'ㅋ'
    0x314C, // 'ㅌ'
    0x314D, // 'ㅍ'
    0x314E, // 'ㅎ'
    0x0000  // NULL
};		

// Unicode Range Table for a letter
const ushort Letter_Unicode_Map_Tbl[NUM_UNICODE_TBL][2] = {
	{0x0000, 0x002F},	// First ~ '0'
	{0x0030, 0x0039},	// '0' ~ '9' : Numeric
	{0x003A, 0x0040},	// '9' ~ 'A'
    {0x0041, 0x0041},	// 'A' => Alphabet Begin
	{0x0042, 0x0042},	// 'B'
	{0x0043, 0x0043},	// 'C'
	{0x0044, 0x0044},	// 'D'
	{0x0045, 0x0045},	// 'E'
	{0x0046, 0x0046},	// 'F'
	{0x0047, 0x0047},	// 'G'
	{0x0048, 0x0048},	// 'H'
	{0x0049, 0x0049},	// 'I'
	{0x004A, 0x004A},	// 'G'
	{0x004B, 0x004B},	// 'K'
	{0x004C, 0x004C},	// 'L'
	{0x004D, 0x004D},	// 'M'
	{0x004E, 0x004E},	// 'N'
	{0x004F, 0x004F},	// 'O'
	{0x0050, 0x0050},	// 'P'
	{0x0051, 0x0051},	// 'Q'
	{0x0052, 0x0052},	// 'R'
	{0x0053, 0x0053},	// 'S'
	{0x0054, 0x0054},	// 'T'
	{0x0055, 0x0055},	// 'U'
	{0x0056, 0x0056},	// 'V'
	{0x0057, 0x0057},	// 'W'
	{0x0058, 0x0058},	// 'X'
	{0x0059, 0x0059},	// 'Y'
    {0x005A, 0x005A},	// 'Z' <= Alphabet End
    {0x005B, 0xABFF},	//
    {0xac00, 0xb097},	// '가~'
    {0xb098, 0xb2e3},	// '나~'
    {0xb2e4, 0xb77b},	// '다~'
    {0xb77c, 0xb9c7},	// '라~'
    {0xb9c8, 0xbc13},	// '마~'
    {0xbc14, 0xc0ab},	// '바~'
    {0xc0ac, 0xc543},	// '사~'
    {0xc544, 0xc78f},	// '아~'
    {0xc790, 0xcc27},	// '자~'
    {0xcc28, 0xce73},	// '차~'
    {0xce74, 0xd0bf},	// '카~'
    {0xd0c0, 0xd30b},	// '타~'
    {0xd30c, 0xd557},	// '파~'
    {0xd558, 0xd7a3},	// '하~'
    {0xD7A4, 0xFFFF}	// 
};

// { added by jaehwan.lee  13.01.04 for audio contents sort issue 

const QChar  gKorBegin1(0x3131); //modified by honggi.shin 2013.12.05
const QChar  gKorEnd1(0xD7A3);

//{ added by jaehwan.lee 2013.12.27
const QChar  gZhBegin1(0x963F);
const QChar  gZhEnd1(0x95E7);
//} added by jaehwan

//{ modified by honggi.shin 2013.12.05
const QChar  gArbBegin1(0x0621); 
const QChar  gArbEnd1(0x0623);
const QChar  gArbSpecial1(0x0625);
const QChar  gArbBegin2(0x0627);
const QChar  gArbEnd2(0x064A);
const QChar  gArbSpecial2(0x0624);
const QChar  gArbSpecial3(0x0626);
//} modified by honggi.shin

//{ modified by honggi.shin 2013.12.05
const QString  gOrderByKor = "!(?%1 < 'A') "
                             "!((?%1>='%2')&&(?%1 <= '%3%3%3%3%3')) ";  //modified by honggi.shin 2013.12.05
//} modified by honggi.shin


//{ modified by honggi.shin 2013.12.05
const QString  gOrderByArb = "!(?%1 < 'A') "
                             "!((?%1 >= '%2') && (?%1 <= '%3%6%6%6%6%6%6%6%6%6')) "
                             "!((?%1 >= '%4') && (?%1 <= '%4%6%6%6%6%6%6%6%6%6')) "
                             "!((?%1 >= '%5') && (?%1 <= '%6%6%6%6%6%6%6%6%6%6')) "
                             "!((?%1 >= '%7') && (?%1 <= '%7%6%6%6%6%6%6%6%6%6')) "
                             "!((?%1 >= '%8') && (?%1 <= '%8%6%6%6%6%6%6%6%6%6')) ";  //modified by honggi.shin 2013.12.05
//} modified by honggi.shin 

//{ modified by jaeh7.lee 2013.12.27
const QString  gOrderByZh = "!(?%1 < 'A') "
                             "!((?%1>='%2')&&(?%1 <= '%3%3%3%3%3')) ";  //modified by honggi.shin 2013.12.05
//} modified by honggi.shin


/**
   * Query strings for media related queries
   */

const QString gSongsByFolderQuery = ("SELECT ?title ?url "
                                                         "WHERE {"
                                                         " ?song a nmm:MusicPiece ; "
                                                         "         nie:title ?title ; "
                                                         "         nie:isStoredAs ?as . "
                                                         " ?as nie:url ?url . FILTER(REGEX(?url, \"%1\", \"i\")) . "
                                                         "} "
                                                         "ORDER BY ?title");
const int SONGBYFOLDERQUERY_DATATYPECOUNT = 2;

const QString gSongsByEmptyFolderQuery ="SELECT ?title ?url "
                                                               "WHERE {"
                                                               " ?song a nmm:MusicPiece ; "
                                                               "         nie:title ?title ; "
                                                               "         nie:isStoredAs ?as . "
                                                               " ?as nie:url ?url . "
                                                               "} "
                                                               "ORDER BY ?title";
const int SONGBYEMPTYFOLDERQUERY_DATATYPECOUNT = 2;

const QString gAlbumByEmptyFolderQuery = "SELECT ?album ?performer ?url  "
                                                                 "WHERE {"
                                                                 " ?song a nmm:MusicPiece ;"
                                                                 "         nmm:performer [ nmm:artistName ?performer ] ;"
                                                                 "         nmm:musicAlbum [ nie:title ?album ] ;"
                                                                 "         nie:isStoredAs ?as . "
                                                                 " ?as nie:url ?url .  "
                                                                 "} "
                                                                 "GROUP BY ?album";
const int ALBUMBYEMPTYFOLDERQUERY_DATATYPECOUNT = 3;

const QString gAlbumByFolderQuery = ( "SELECT ?album ?performer ?url "
                                                            "WHERE {"
                                                        " ?song a nmm:MusicPiece ;"
                                                        "         nie:title ?title ;"
                                                        "         nmm:performer [ nmm:artistName ?performer ] ;"
                                                        "         nmm:musicAlbum [ nie:title ?album ]; "
                                                        "         nie:isStoredAs ?as . "
                                                        " ?as nie:url ?url . FILTER(REGEX(?url, \"%1\", \"i\")) . } GROUP BY ?album"
                                                        );
const int ALBUMBYFOLDERQUERY_DATATYPECOUNT = 3;

const QString gArtistByEmptyFolderQuery = "SELECT ?performer ?url  "
                                                        "WHERE {"
                                                        " ?song a nmm:MusicPiece ;"
                                                        "         nmm:performer [ nmm:artistName ?performer ] ;"
                                                        "         nmm:musicAlbum [ nie:title ?album ] ."
                                                        "         nie:isStoredAs ?as . "
                                                        " ?as nie:url ?url .  "
                                                        "} "
                                                        "GROUP BY ?performer";
const int ARTISTBYEMPTYFOLDERQUERY_DATATYPECOUNT = 2;

const QString gArtistByFolderQuery = ( "SELECT ?performer ?url "
                                                        "WHERE {"
                                                        " ?song a nmm:MusicPiece ;"
                                                        "         nie:title ?title ;"
                                                        "         nmm:performer [ nmm:artistName ?performer ] ;"
                                                        "         nmm:musicAlbum [ nie:title ?album ]; "
                                                        "         nie:isStoredAs ?as . "
                                                        " ?as nie:url ?url . FILTER(REGEX(?url, \"%1\", \"i\")) . } GROUP BY ?performer"
                                                        );
const int ARTISTBYFOLDERQUERY_DATATYPECOUNT = 2;

const QString gGetAllSongsQuery = "SELECT  ?url ?title "
                                           "WHERE {"
                                           " ?song a nmm:MusicPiece ; "
                                           "         nie:title ?title ; "
                                           "         nie:isStoredAs ?as . "
                                           " ?as nie:url ?url . "
                                           "} "
                                           "ORDER BY ?title ";

const int GETALLSONGSQUERY_DATATYPECOUNT = 2;

const QString gGetAllPodcastsByTitleQuery = ("SELECT ?podcast "
                                                                "WHERE {"
                                                                " ?song a nmm:MusicPiece ; "
                                                                "nmm:musicAlbum [ nie:title ?podcast ] ;"
                                                                "ipod:TypeName \"%1\" ; "
                                                                "ipod:TypeIndex ?index ; nie:url ?url . %2 }"
                                                                " ORDER BY ?podcast");

const int GETALLPODCASTSBYTITLEQUERY_DATATYPECOUNT = 1;

const QString gGetAllAudiobooksByTitleQuery = ("SELECT ?AudioBook  "
                                                        "WHERE { ?song a nmm:MusicPiece ; "
                                                        "nmm:musicAlbum [ nie:title ?AudioBook ] ;"
                                                        "ipod:TypeName \"%1\" ; ipod:TypeIndex ?index ; nie:url ?url . %2 }"
                                                        "ORDER BY ?AudioBook");


const int GETALLAUDIOBOOKSBYTITLEQUERY_DATATYPECOUNT = 1;

const int GETALLITUNESUBYTITLEQUERY_DATATYPECOUNT=1;

const QString gGetAllPodcastsEpisodeNamesQuery = ("SELECT ?title ?url "
                                                        "WHERE {?song a nmm:MusicPiece ; "
                                                        "nie:title ?title ; "
                                                        "nmm:musicAlbum [ nie:title \"%1\" ] ; "
                                                        "ipod:TypeName \"%2\" ; ipod:TypeIndex ?index ; "
                                                        "nie:url ?url . %3 } "
                                                        "ORDER BY ?title");

const int GETALLPODCASTSEPISODENAMESQUERY_DATATYPECOUNT = 2;

const QString gGetAllAudiobookChapterNamesQuery = ("SELECT ?title ?url "
                                                        "WHERE {?song a nmm:MusicPiece ; "
                                                        "nie:title ?title ; "
                                                        "nmm:musicAlbum [ nie:title \"%1\" ] ; "
                                                        "ipod:TypeName \"%2\" ; ipod:TypeIndex ?index ; "
                                                        "nie:url ?url . %4 } "
                                                        "ORDER BY ?title");

const int GETALLAUDIOBOOKCHAPTERNAMESQUERY_DATATYPECOUNT = 2;
const int GETALLITUNESUEPISODENAMESQUERY_DATATYPECOUNT=2;

const QString gGetAllSongsIDQuery = "SELECT ?url ?title tracker:id(?song)"
                                           "WHERE {"
                                           " ?song a nmm:MusicPiece ; "
                                           "         nie:title ?title ; "
                                           "         nie:isStoredAs ?as . "
                                           " ?as nie:url ?url . "
                                           "} "
                                           "ORDER BY ?title";
const int GETALLSONGSIDQUERY_DATATYPECOUNT = 3;

const QString gFolderListEmptyQuery = "SELECT  ?url  "
                                      "WHERE {"
                                      " ?folder a nfo:Folder ; "
                                      " nie:url ?url  "
                                      "} ";

const QString gFolderListQuery = "SELECT  ?url  "
                                           "WHERE {"
                                           " ?folder a nfo:Folder ; "
                                           " nie:url ?url FILTER(REGEX(?url , \"%1\" )) "
                                           "} ";

const int FOLDERLISTQUERY_DATATYPECOUNT = 1;

const QString gSongsByAlbumQuery = "SELECT ?title ?url "
                                                        "WHERE {"
                                                        " ?song a nmm:MusicPiece ; "
                                                        " nie:title ?title ;  "
                                                        "nmm:musicAlbum [ nie:title \"%1\" ] ; "
                                                        "nie:isStoredAs ?as . "
                                                        "?as nie:url ?url . "
                                                        " } "
                                                        "ORDER BY ?title";
const int SONGSBYALBUMQUERY_DATATYPECOUNT = 2;

const QString gMusicAlbumListQuery = ( "SELECT DISTINCT ?album "
                                                            "WHERE {"
                                                        " ?song a nmm:MusicPiece ;"
                                                        "         nie:title ?title ;"
                                                        "         nmm:performer [ nmm:artistName ?performer ] ;"
                                                        "         nmm:musicAlbum [ nie:title ?album ]; "
                                                        "         nie:isStoredAs ?as . "
                                                        " ?as nie:url ?url . FILTER(REGEX(?url, \"%1\", \"i\")) . } GROUP BY ?album"
                                                        );
const int MUSICALBUMLISTQUERY_DATATYPECOUNT = 1;

const QString gGetAllAlbumsQuery = "SELECT  DISTINCT ?title "
                                             "WHERE {"
                                             " ?album a nmm:MusicAlbum . "
                                             " ?album nie:title ?title "
                                             "} "
                                             "ORDER BY ?title";

const int GETALLALBUMSQUERY_DATATYPECOUNT = 1;

const QString  gGetAllMusicArtistsQuery = "SELECT DISTINCT ?performer "
                                                             "WHERE {"
                                                             " ?song a nmm:MusicPiece ; "
                                                             "         nie:title ?title ; "
                                                             "         nmm:performer [nmm:artistName ?performer ] ; "
                                                             "         nie:isStoredAs ?as . "
                                                             " ?as nie:url ?url .  "
                                                             "} "
                                                             "ORDER BY ?performer";

const int GETALLMUISCARTISTSQUERY_DATATYPECOUNT = 1;

const QString gGetAllMusicComposersQuery = "SELECT DISTINCT ?composer "
                                                                   "WHERE {"
                                                                   " ?song a nmm:MusicPiece . "
                                                                   " ?song nmm:composer [nmm:artistName ?composer ] "
                                                                   "} "
                                                                   "ORDER BY ?composer";

const int GETALLMUSICCOMPOSERSQUERY_DATATYPECOUNT = 1;

const QString gGetAllMusicGenresQuery = "SELECT  DISTINCT ?genre  "
                                                                "WHERE {"
                                                                " ?song a nmm:MusicPiece . "
                                                                " ?song nfo:genre ?genre "
                                                                "} "
                                                                "ORDER BY ?genre";

const int GETALLMUSICGENRESQUERY_DATATYPECOUNT = 1;

const QString gGetMusicMetaDataQuery =" SELECT  ?title ?performer ?album  ?genre ?url ?composer ?created ?modified ?bitrate "
                                                               " WHERE { "
                                                               " ?song a nmm:MusicPiece ; "
                                                               " nie:title ?title ; "
                                                               " nie:url ?url . "
                                                               " OPTIONAL {?song nmm:performer [nmm:artistName ?performer] } . "
                                                               " OPTIONAL { ?song nmm:musicAlbum [nie:title ?album] } . "
                                                               " OPTIONAL { ?song nfo:genre ?genre } . "
							       " OPTIONAL { ?song nmm:composer [nmm:artistName ?composer] } . "
                                                               " OPTIONAL { ?song nie:contentCreated ?created } . "
                                                               " OPTIONAL { ?song nfo:fileLastModified ?modified } . "
                                                               " OPTIONAL { ?song nfo:averageBitrate ?bitrate } . "
							       " filter(?url=\"%1\") }"
                                                               "ORDER BY ?title";

const int GETMUSICMETADATAQUERY_DATATYPECOUNT = 9;

const QString gGetMusicMetaDataInfoFromURLListQuery =" SELECT ?title ?album ?performer ?genre "
                                                             " WHERE { "
                                                             " ?song a nmm:MusicPiece ; "
                                                             " nie:title ?title ; "
                                                             " nie:url ?url . "
                                                             " OPTIONAL { ?song nmm:musicAlbum [nie:title ?album] } . "
                                                             " OPTIONAL { ?song nmm:performer  [nmm:artistName ?performer] } . "
                                                             " OPTIONAL { ?song nfo:genre ?genre } . "
                                                             " %1 } ORDER BY ?title";

const int GETMUSICMETADATAINFOFROMURLLISTQUERY_DATATYPECOUNT = 4;

// { added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup
const QString gGetPhotoMetaDataInfoFromURLQuery =" SELECT ?title ?url ?contentCreated "
                                                             " WHERE { "
                                                             " ?photo a nmm:Photo ; "
                                                             " nie:title ?title ; "
                                                             " nie:contentCreated ?contentCreated ; "
                                                             " nie:url ?url . "                                                   
                                                             " %1 } ORDER BY ?title";

const int GETPHOTOMETADATAINFOFROMURLQUERY_DATATYPECOUNT = 3;
// } added eugene.seo 2012.12.05 for adding contentCreated date into photo file info popup

const QString gGetAllSongsByArtistQuery = " SELECT ?title ?url "
                                                        " WHERE { "
                                                        " ?song a nmm:MusicPiece ; "
                                                        " nie:title ?title ;  "
                                                        " nmm:performer[ nmm:artistName \"%1\"] ; "
                                                        " nie:isStoredAs ?as . "
                                                        " ?as nie:url ?url . } "
                                                        "ORDER BY ?title";

const int GETALLSONGSBYARTISTQUERY_DATATYPECOUNT = 2;

const QString gGetAllSongsByGenreQuery = " SELECT ?title ?url "
                                                                   " WHERE { "
                                                                   " ?song a nmm:MusicPiece ; "
                                                                   " nfo:genre \"%1\" ; "
                                                                   " nmm:musicAlbum [ nie:title ?album] ; "
                                                                   " nmm:performer [nmm:artistName ?performer ] ; "
                                                                   " nie:title ?title ; "
                                                                   " nie:isStoredAs ?as . "
                                                                   " ?as nie:url ?url . }"
                                                                   "ORDER BY ?title";
const int GETALLSONGSBYGENREQUERY_DATATYPECOUNT = 2;

const QString gGetAllSongsByComposerQuery = " SELECT ?title ?url  "
                                                                        " WHERE { "
                                                                        " ?song a nmm:MusicPiece . "
                                                                        " ?song nmm:composer [nmm:artistName \"%1\" ] ; "
                                                                        " nie:title ?title ;  "
                                                                        " nie:isStoredAs ?as .  "
                                                                        " ?as nie:url ?url . } "
                                                                        "ORDER BY ?title";

const int GETALLSONGSBYCOMPOSERQUERY_DATATYPECOUNT = 2;

const QString gGetImagesByFolderQuery = " SELECT DISTINCT ?file ?url "
                                                                 " WHERE {  "
                                                                 " ?image a nfo:Image , nmm:Photo . "
                                                                 " OPTIONAL { ?image nfo:fileName ?file } . "
                                                                 " ?image nie:url ?url . "
                                                                 " Filter(REGEX(?url , \"%1\" , \"i\"))}"
                                                                 "ORDER BY ?file";

const int GETIMAGESBYFOLDERQUERY_DATATYPECOUNT = 2;

const QString gGetVideosByFolderQuery = " SELECT DISTINCT ?file ?url ?title "
                                                                " WHERE {  "
                                                                " ?video a nmm:Video . "
                                                                " OPTIONAL { ?video nfo:fileName ?file } . "
                                                                " OPTIONAL {?video nie:title ?title } "
                                                                " ?video nie:url ?url . "
                                                                " Filter(REGEX(?url , \"%1\" , \"i\"))}"
                                                                "ORDER BY ?file";

const int GETVIDEOSBYFOLDERQUERY_DATATYPECOUNT = 3;

const QString gGetAllMusicByKeywordQuery = " SELECT ?title ?album ?performer ?genre ?url  "
                                                                     " WHERE { "
                                                                     " ?song a nmm:MusicPiece  ;  "
                                                                     " nie:url ?url  .  "
                                                                     " OPTIONAL { ?song nie:title ?title }  .  "
                                                                     " OPTIONAL { ?song  nmm:performer [nmm:artistName ?performer ] }  .  "
                                                                     " OPTIONAL { ?song  nfo:genre ?genre }  . "
                                                                     " OPTIONAL { ?song  nmm:musicAlbum [nie:title ?album] }  .  "
                                                                     " FILTER( (REGEX(?album, \"%1\", \"i\") || "
                                                                     " REGEX(?album, \"%2\", \"i\") ||"
                                                                     " REGEX(?performer, \"%1\", \"i\") || "
                                                                     " REGEX(?performer, \"%2\", \"i\") || "
                                                                     " REGEX(?title, \"%1\", \"i\") || "
                                                                     " REGEX(?title, \"%2\", \"i\") "
                                                                     " )  &&  %3  ) }"
                                                                     "ORDER BY ?title";

const int GETALLMUSICBYKEYWORDQUERY_DATATYPECOUNT = 5;

const QString gGetMultipleArtistSongsQuery = "SELECT ?title ?url "
                                             "WHERE{"
                                             "?song a nmm:MusicPiece ; "
                                             "nie:title ?title ; "
                                             "nie:url ?url ; "
                                             "nmm:performer [nmm:artistName ?performer] ."
                                             "%1 }"
                                             "ORDER BY ?title";

const int GETMULTIPLEARTISTSONGSQUERY_DATATYPECOUNT = 2;

const QString gGetMultipleAlbumSongsQuery = "SELECT ?title ?url "
                                             "WHERE{"
                                             "?song a nmm:MusicPiece ; "
                                             "nie:title ?title ; "
                                             "nie:url ?url ; "
                                             "nmm:musicAlbum [nie:title ?album] . "
                                             "%1 }"
                                             "ORDER BY ?title";

const int GETMULTIPLEALBUMSONGSQUERY_DATATYPECOUNT = 2;

const QString gGetAllImagesByKeywordQuery = " SELECT ?title ?url "
                                                                        " WHERE { "
                                                                        " ?urn a nfo:Image ; "
                                                                        " nie:url ?url ;"
                                                                        " nfo:fileName ?title ."
                                                                        " FILTER( REGEX(?title, \"%1\", \"i\") || "
                                                                        " REGEX(?title, \"%2\", \"i\") )"
                                                                        " }"
                                                                        "ORDER BY ?title";

const int IMAGES_BYKEYWORD_DATATYPECOUNT = 2;

const QString gGetAllVideosByKeywrodQuery =" SELECT ?title ?url "
                                                                       " WHERE {  "
                                                                       " ?urn a nmm:Video ;  "
                                                                       " nie:url ?url ; "
                                                                       " nfo:fileName ?title ."
                                                                       " FILTER( REGEX(?title, \"%1\", \"i\") || "
                                                                       " REGEX(?title, \"%2\", \"i\") )"
                                                                       " }"
                                                                       "ORDER BY ?title";
const int VIDEOS_BYKEYWORD_DATATYPECOUNT = 2;

const QString gSongsCountByFolderQuery = ("SELECT COUNT(?song) "
                                                         "WHERE {"
                                                         " ?song a nmm:MusicPiece ; "
                                                         "         nie:title ?title ; "
                                                         "         nie:isStoredAs ?as . "
                                                         " ?as nie:url ?url . FILTER(REGEX(?url, \"%1\", \"i\")) . "
                                                         "} ");
const int SONGSCOUNTBYFOLDERQUERY_DATATYPECOUNT = 1;

const QString gGetImagesCountByFolderQuery = " SELECT DISTINCT COUNT(?image) "
                                                                 " WHERE {  "
                                                                 " ?image a nfo:Image , nmm:Photo . "
                                                                 " OPTIONAL { ?image nfo:fileName ?file } . "
                                                                 " ?image nie:url ?url . "
                                                                 " Filter(REGEX(?url , \"%1\" , \"i\"))}";

const int GETIMAGESCOUNTBYFOLDERQUERY_DATATYPECOUNT = 1;

const QString gGetVideosCountByFolderQuery = " SELECT DISTINCT COUNT(?video) "
                                                                " WHERE {  "
                                                                " ?video a nmm:Video . "
                                                                " OPTIONAL { ?video nfo:fileName ?file } . "
                                                                " OPTIONAL {?video nie:title ?title } "
                                                                " ?video nie:url ?url . "
                                                                " Filter(REGEX(?url , \"%1\" , \"i\"))}";

const int GETVIDEOSCOUNTBYFOLDERQUERY_DATATYPECOUNT = 1;

const QString gGetAllSongsCountQuery = "SELECT  COUNT(?song) "
                                           "WHERE {"
                                           " ?song a nmm:MusicPiece ; "
                                           "         nie:title ?title ; "
                                           "         nie:isStoredAs ?as . "
                                           " ?as nie:url ?url . "
                                           "} ";

const int GETALLSONGSCOUNTQUERY_DATATYPECOUNT = 1;

const QString gGetAllSongsCountFromVolumeQuery = "SELECT COUNT(?song) "
                                                            " WHERE { "
                                                            " ?song a nmm:MusicPiece ;"
                                                            " nie:url ?url . "
                                                            "%1"
                                                            "} ";
const int GETALLSONGSCOUNTFROMVOLUMEQUERY_DATATYPECOUNT = 1;

const QString gGetSongsCountWithMetaInfoFromVolumeQuery = "SELECT COUNT(?song) "
                                                                "WHERE {"
                                                                " ?song a nmm:MusicPiece ; "
                                                                "  nie:title ?title ; "
                                                                " nmm:musicAlbum [nie:title ?album] ; "
                                                                " nie:url ?url . "
                                                                "%1"
                                                                "} ";
const int GETSONGSCOUNTWITHMETAINFOFROMVOLUMEQUERY_DATATYPECOUNT = 1;

const QString gGetAllSongsWithoutMetaInfoFromVolumeQuery = "SELECT DISTINCT ?url ?title "
                                                                 "WHERE {"
                                                                 " ?song a nmm:MusicPiece ; "
                                                                 "  nie:url  ?url . "
                                                                 " OPTIONAL { ?song nie:title ?title } . "
                                                                 " OPTIONAL { ?song nmm:musicAlbum [nie:title ?album] } . "                                                                 
                                                                 "%1"
                                                                 "} ORDER BY ?album  OFFSET %2 LIMIT %3";

const int GETALLSONGSWITHOUTMETAINFOFROMVOLUMEQUERY_DATATYPECOUNT = 2;

const QString gGetAllSongsFromVolumeQuery = "SELECT  ?url ?title "
                                           "WHERE {"
                                           " ?song a nmm:MusicPiece ; "
                                           "         nie:title ?title ; "                                           
                                           " nie:url ?url . "
                                           "%1"
                                           "} "
                                           " ORDER BY ?title ";
//{ added by jaehwan.lee for audio list sorting issue 
const QString gGetAllSongsFromVolumeExtOrderQuery = "SELECT  ?url ?title "
                                           "WHERE {"
                                           " ?song a nmm:MusicPiece ; "
                                           "         nie:title ?title ; "                                           
                                           " nie:url ?url . "
                                           "%1 "
                                           "} "
                                           " ORDER BY " 
                                           "%2 "
                                           "?title ";

//}                                           
const int GETALLSONGSFROMVOLUMEQUERY_DATATYPECOUNT = 2;

const QString gGetAllAlbumsFromVolumeQuery = " SELECT DISTINCT ?album ?performer "
                                             " WHERE { "
                                             " ?song a nmm:MusicPiece ; "
                                             " nmm:performer [nmm:artistName ?performer ] ; "
                                             " nmm:musicAlbum [nie:title ?album] ;"
                                             " nie:url ?url . %1 } ORDER BY ?album";

const int GETALLALBUMSFROMVOLUMEQUERY_DATATYPECOUNT = 2;

const QString  gGetAllMusicArtistsFromVolumeQuery = " SELECT DISTINCT  ?performer "
                                                               " WHERE {"
                                                               " ?song a nmm:MusicPiece ; "
                                                               "         nie:title ?title ; "
                                                               "         nmm:performer [nmm:artistName ?performer ] ; "                                                               
                                                               " nie:url ?url .  %1"
                                                               " } "
                                                               "ORDER BY ?performer";

//{ added by jaehwan.lee for audio list sorting issue  
const QString  gGetAllMusicArtistsFromVolumeExtOrderQuery = " SELECT DISTINCT  ?performer "
                                                               " WHERE {"
                                                               " ?song a nmm:MusicPiece ; "
                                                               "         nie:title ?title ; "
                                                               "         nmm:performer [nmm:artistName ?performer ] ; "                                                               
                                                               " nie:url ?url .  %1"
                                                               " } "
                                                               "ORDER BY " 
                                                               "%2 "
                                                               "?performer";
                                      
//}

const int GETALLMUISCARTISTSFROMVOLUMEQUERY_DATATYPECOUNT = 1;

const QString gGetAllMusicComposersFromVolumeQuery = "SELECT DISTINCT  ?composer "
                                                                   "WHERE {"
                                                                   " ?song a nmm:MusicPiece . "
                                                                   " ?song nmm:composer [nmm:artistName ?composer ]  ;"                                                                   
                                                                   " nie:url ?url . %1"
                                                                   "} "
                                                                   "ORDER BY ?composer";

const int GETALLMUSICCOMPOSERSFORMVOLUMEQUERY_DATATYPECOUNT = 1;

const QString gGetAllMusicGenresFromVolumeQuery = " SELECT  DISTINCT  ?genre  "
                                                                                  " WHERE {"
                                                                                  " ?song a nmm:MusicPiece . "
                                                                                  " ?song nfo:genre ?genre ; "                                                                                  
                                                                                  " nie:url ?url . %1"
                                                                                  " } "
                                                                                  " ORDER BY ?genre";

const int GETALLMUSICGENRESFROMVOLUMEQUERY_DATATYPECOUNT = 1;

const QString gGetAllImagesFromVolumeQuery = " SELECT DISTINCT ?file ?url "
                                                                 " WHERE {  "
                                                                 " ?image a nfo:Image , nmm:Photo . "
                                                                 " OPTIONAL { ?image nfo:fileName ?file } . "
                                                                 " ?image nie:url ?url . "
                                                                 " %1 }"
                                                                 " ORDER BY ?file ";

const int GETALLIMAGESFROMVOLUMEQUERY_DATATYPECOUNT = 2;

const QString gGetAllVideosFromVolumeQuery = " SELECT DISTINCT ?file ?url ?title "
                                                                " WHERE {  "
                                                                " ?video a nmm:Video . "
                                                                " OPTIONAL { ?video nfo:fileName ?file } . "
                                                                " OPTIONAL {?video nie:title ?title } "
                                                                " ?video nie:url ?url . "
                                                                " %1}"
                                                                " ORDER BY ?file ";

const int GETALLVIDEOSFROMVOLUMEQUERY_DATATYPECOUNT = 3;

const QString gGetMountedVolumeListQuery = " SELECT nie:url(?m) "
                                                                      " WHERE { "
                                                                      " ?v a tracker:Volume; "
                                                                      " tracker:mountPoint ?m "
                                                                      " }";
const QString gGetDeviceUrlQuery = " SELECT nie:url(?m) "
                                   " WHERE { "
                                   " ?v a tracker:Volume; "
                                   " tracker:mountPoint ?m  . "
                                   " Filter(REGEX(?v , \"%1\" , \"i\")) "
                                   " }";

const QString gGetAllSongsWithMetaDataFromVolumeQuery = " SELECT ?title ?url ?album ?artist ?genre ?comp"
                                                                                            " WHERE { "
                                                                                            " ?song a nmm:MusicPiece ; "
                                                                                            " nie:title ?title ; nmm:musicAlbum [nie:title ?album]; "
                                                                                            " nmm:performer[ nmm:artistName ?artist] ; "                                                                                            
                                                                                            " nie:url ?url . %1 . OPTIONAL {?song "
                                                                                             " nmm:composer [nmm:artistName ?comp] } . "
                                                                                             "OPTIONAL {?song nfo:genre ?genre} "
                                                                                            " } ORDER BY %2";

const int GETALLSONGSWITHMETADATAFROMVOLUMEQUERY_DATATYPECOUNT = 6;

const QString gGetAllSongsByGenreArtistAlbumQuery = " SELECT ?title ?url "
                                                                                   " WHERE { "
                                                                                   " ?song a nmm:MusicPiece ; "
                                                                                   " nfo:genre \"%1\" ; "
                                                                                   " nmm:performer [nmm:artistName \"%2\" ] ; "
                                                                                   " nmm:musicAlbum [ nie:title \"%3\"] ; "
                                                                                   " nie:title ?title ; "
                                                                                   " nie:url ?url .  %4 } ORDER BY ?title";

const int GETALLSONGSBYGENREARTISTALBUMQUERY_DATATYPECOUNT = 2;

const QString gGetAllSongsByGenreComposerAlbumQuery = "SELECT ?title ?url "
                                                                                           "WHERE {"
                                                                                           " ?song a nmm:MusicPiece ; "
                                                                                           " nfo:genre \"%1\" ; "
                                                                                           " nmm:composer [nmm:artistName \"%2\" ]  ;"
                                                                                           " nmm:musicAlbum [ nie:title \"%3\"] ; "
                                                                                           " nie:title ?title; "
                                                                                           " nie:url ?url . %4"
                                                                                           "} "
                                                                                           "ORDER BY ?title";

const int GETALLSONGSBYGENRECOMPOSERALBUMQUERY_DATATYPECOUNT = 2;

const QString gGetAllSongsByArtistAlbumQuery = "SELECT DISTINCT ?url ?title "
                                                                                                   "WHERE {"
                                                                                                   "?song a nmm:MusicPiece ;"
                                                                                                   " nie:title ?title ; nie:url ?url ; "
                                                                                                   " nmm:performer [nmm:artistName ?artist] ;"
                                                                                                   " nmm:musicAlbum [nie:title ?album] ."
                                                                                                   " OPTIONAL { ?song nmm:trackNumber ?trackno } "
                                                                                                   " FILTER(?artist = \"%1\") ."
                                                                                                   " FILTER(?album = \"%2\") ."
                                                                                                   " %3"
                                                                                                   "} "
                                                                                                   "ORDER BY ?trackno ?title";

const int GETALLSONGSBYARTISTALBUMQUERY_DATATYPECOUNT = 2;

const QString gGetAllAlbumByGenreArtistQuery = "SELECT DISTINCT ?album "
                                                                                                   "WHERE {"
                                                                                                   "?song a nmm:MusicPiece ;"
                                                                                                   " nie:url ?url ;"
                                                                                                   " nmm:performer [nmm:artistName ?artist] ;"
                                                                                                   " nfo:genre ?genre ;"
                                                                                                   " nmm:musicAlbum [nie:title ?album] ."
                                                                                                   " FILTER(?genre = \"%1\") ."
                                                                                                   " FILTER(?artist = \"%2\") ."
                                                                                                   " %3"
                                                                                                   "} "
                                                                                                   "ORDER BY ?album";

const int GETALLALBUMBYGENREARTISTQUERY_DATATYPECOUNT = 1;
//add url by junam 2013.10.28 for 2nd depth coverart
const QString gGetAllAlbumByArtistQuery = "SELECT DISTINCT ?album ?url "
                                                                                                   "WHERE {"
                                                                                                   "?song a nmm:MusicPiece ;"
                                                                                                   " nie:url ?url ;"
                                                                                                   " nmm:performer [nmm:artistName ?artist] ;"
                                                                                                   " nmm:musicAlbum [nie:title ?album] ."
                                                                                                   " FILTER(?artist = \"%1\") ."
                                                                                                   " %2"
                                                                                                   "} "
                                                                                                   " GROUP BY ?album ORDER BY ?album";


const int GETALLALBUMBYARTISTQUERY_DATATYPECOUNT = 2;

const QString gGetAllArtistByGenreQuery = "SELECT DISTINCT ?artist "
                                                                                                   "WHERE {"
                                                                                                   "?song a nmm:MusicPiece ;"
                                                                                                   " nie:url ?url ;"
                                                                                                   " nfo:genre ?genre ;"
                                                                                                   " nmm:performer [ nmm:artistName ?artist ] ."
                                                                                                   " FILTER(?genre = \"%1\") ."
                                                                                                   " %2"
                                                                                                   "} "
                                                                                                   "ORDER BY ?artist";

const int GETALLARTISTBYGENREQUERY_DATATYPECOUNT = 1;

const QString gGetAllsongsByKeywordFromVolumeQuery =" SELECT DISTINCT ?title ?album ?performer ?genre ?url "
                                                                        "WHERE {  ?song a nmm:MusicPiece ;"
                                                                        " nie:title ?title ;"
                                                                        "nmm:performer [nmm:artistName ?performer ] ;"
                                                                        " nmm:musicAlbum [nie:title ?album] ; "
                                                                        "nie:url ?url . "
                                                                        "OPTIONAL { ?song nfo:genre ?genre } ."
                                                                        "FILTER( REGEX(?title, \"%1\", \"i\") || REGEX(?title, \"%2\", \"i\") "
                                                                        "&& REGEX (?url, \"^%3\", \"i\") ) } ORDER BY ?title";

const int GETALLSONGSBYKEYWORDFROMVOLUME_DATATYPECOUNT = 5;

const QString gGetAllArtistsByKeywordFromVolumeQuery =" SELECT DISTINCT ?title ?album ?performer ?genre ?url "
                                                                          " WHERE { "
                                                                          " ?song a nmm:MusicPiece ; "
                                                                          " nie:title ?title ; "
                                                                          " nmm:performer [nmm:artistName ?performer ] ; "
                                                                          " nmm:musicAlbum [nie:title ?album] ; "
                                                                          " nie:url ?url . "
                                                                          "OPTIONAL { ?song nfo:genre ?genre } ."
                                                                          " FILTER( REGEX(?performer, \"%1\", \"i\") || REGEX(?performer, \"%2\", \"i\")"
                                                                          " && REGEX (?url, \"^%3\", \"i\") ) } ORDER BY ?performer";
const int GETALLARTISTBYKEYWORDFROMVOLUME_DATATYPECOUNT = 5;
const QString gGetAllAlbumsByKeywordFromVolumeQuery =" SELECT DISTINCT ?title ?album ?performer ?genre ?url "
                                                                        " WHERE { "
                                                                        " ?song a nmm:MusicPiece ; "
                                                                        " nie:title ?title ; "
                                                                        " nie:url ?url ; "
                                                                        " nmm:performer [nmm:artistName ?performer ] ; "
                                                                        " nmm:musicAlbum [nie:title ?album] . "
                                                                        "OPTIONAL { ?song nfo:genre ?genre } ."
                                                                        " FILTER( REGEX(?album, \"%1\", \"i\") || REGEX(?album, \"%2\", \"i\")"
                                                                        " && REGEX (?url, \"^%3\", \"i\") ) } ORDER BY ?album";
const int GETALLALBUMSYKEYWORDFROMVOLUME_DATATYPECOUNT = 5;
const QString gGetAllGenresByKeywordFromVolumeQuery=" SELECT DISTINCT ?title ?album ?performer ?genre ?url "
                                                                        " WHERE { "
                                                                        " ?song a nmm:MusicPiece ; "
                                                                        " nie:title ?title ; "
                                                                        " nie:url ?url ; "
                                                                        " nmm:performer [nmm:artistName ?performer ] ; "
                                                                        " nmm:musicAlbum [nie:title ?album] ; "
                                                                        " nfo:genre ?genre . "
                                                                        " FILTER(REGEX(?genre, \"%1\", \"i\") || REGEX(?genre, \"%2\", \"i\")"
                                                                        " && REGEX (?url, \"^%3\", \"i\")) }ORDER BY ?genre";
const int GETALLGENREBYKEYWORDFROMVOLUME_DATATYPECOUNT = 5;
const QString gGetMusicDetailFromIpodOnIndexBasisQuery="select ?title ?url ?TrackIndex ?artist ?composer ?album ?genre ?TrackUid ?TrackIndex where { ?m a nmm:MusicPiece ; nfo:genre ?genre ; nmm:performer[nmm:artistName ?artist] ; nmm:composer[nmm:artistName ?composer] ; nmm:musicAlbum[nie:title ?album] ; ipod:TrackUid ?TrackUid ; ipod:TrackIndex ?TrackIndex ; nie:title ?title ; nie:url ?url ; ipod:TypeName \"AllSongs\" . filter(?TrackIndex >= %1 && ?TrackIndex <=%2) }";
const int GETMUSICDETAILFROMIPODONINDEXBASIS_DATATYPECOUNT = 9;
/**
  * QSparql query for retrieving playlist Data for each media file in the Playlist
  *  from the Tracker Store.
  */
const QString gGetAllSongsbyPlaylistQuery = "SELECT ?etitle ?entryurl "
                                                                    "WHERE {?playlist a nmm:Playlist ; "
                                                                    "nie:title \"%1\" ; ipod:TypeName \"%2\"; "
                                                                    "nfo:hasMediaFileListEntry ?entry . "
                                                                    "?entry a nfo:MediaFileListEntry ; "
                                                                    "nfo:entryUrl ?entryurl ; "
                                                                    "ipod:entryTitle ?etitle }"
                                                                    "ORDER BY ?etitle";


const int GETPLAYLISTDATA = 2;

const QString gGetlistofPlaylistsQuery="SELECT ?title ?playlist WHERE {?playlist a nmm:Playlist ; nie:title ?title }" "ORDER BY ?playlist" ;

const int GETLISTOFPLAYLIST=2;

const QString gGetMusicPlaylistByKeywordQuery="SELECT ?title WHERE {?playlist a nmm:Playlist ; nie:title ?title ;ipod:TypeName \"%2\" . FILTER(REGEX(?title , \"%1\")) }";

const int GETMUSICPLAYLISTBYKEYWORD=1;

const QString gGetMostPlayedMusicSongsQuery ="SELECT  ?title ?count ?url WHERE {  ?song nie:usageCounter ?count ; nie:title ?title ; nie:url ?url . %1 } ORDER BY DESC(?count) LIMIT 10";

const int GETMOSTPLAYEDMUSICSONGS=3;

const QString gGetMostRecentlyPlayedSongsQuery="SELECT  ?title ?time ?url WHERE { ?song nie:contentAccessed ?time ; nie:title ?title ; nie:isStoredAs ?as . ?as nie:url ?url . %1 } ORDER BY DESC(?time) LIMIT 10";

const int GETMOSTRECENTPLAYEDSONGS=3;

// Queries for fetching CD data

const QString gGetMusicFromCDQuery = " SELECT ?title ?album ?performer ?genre ?trackNum ?trackCount ?albumArtist "
                                                                     " WHERE { "
                                                                     " ?song a nmm:MusicPiece ; "
                                                                     " nie:title ?title ; "
                                                                     " nmm:performer [nmm:artistName ?performer ] ; "
                                                                     " nfo:genre ?genre; "
									      " nmm:trackNumber ?trackNum; "	
                                                                     " nmm:musicAlbum [nie:title ?album; nmm:albumTrackCount ?trackCount; nmm:albumArtist [nmm:artistName ?albumArtist]]; "
                                                                     " nmm:musicAlbumDisc [nmm:musicCDIdentifier \"%1\"] }";

const int GETALLMUSICFROMCDQUERY_DATATYPECOUNT = 7;

const QString gGetMusicMetaDataFromCDQuery = " SELECT ?title ?album ?performer ?genre "
                                                                             " WHERE { "
                                                                             " ?song a nmm:MusicPiece ; "
                                                                             " nie:title ?title ; "
                                                                             " nmm:performer [nmm:artistName ?performer ] ; "
                                                                             " nfo:genre ?genre; "
                                                                             " nmm:musicAlbum [nie:title ?album] ;"
                                                                             " nmm:musicAlbumDisc [nmm:musicCDIdentifier \"%1\"] ;"
                                                                              " nmm:trackNumber %2 }"
                                                                              " ORDER BY ?title ";
const int GETMUSICMETADATAFROMCDQUERY_DATATYPECOUNT = 4;

/*Since coverart is not a Query A dummy variable is created to not
distruct the current tracker Architecture flow*/

//const QString gGetMusicAlbumCoverArt = "Null";

/**
  * QSparql query for retrieving playlist Data for each media file in the Playlist
  *  from the Tracker Store.
  */
const QString gGetPlaylistDataQuery = "SELECT ?playlist ?url WHERE {?playlist a nmm:Playlist ; nie:title \"%1\" .?playlist nfo:hasMediaFileListEntry ?entry . ?entry nfo:entryUrl ?url}";

const QString gRemoveAllMediaDataFromVolumeQuery = "DELETE { ?song a rdfs:Resource . }"
                                                     "WHERE {"
                                                     " ?song a nfo:Media ; "
                                                     " nie:url ?url . "
                                                     "%1 }";

//{added by junam 2013.03.27 for ISV72425
const QString gGetMusicSimpleMetaDataQuery =" SELECT  ?title ?performer ?album ?url"
                                                               " WHERE { "
                                                               " ?song a nmm:MusicPiece ; "
                                                               " nie:title ?title ; "
                                                               " nie:url ?url . "
                                                               " OPTIONAL {?song nmm:performer [nmm:artistName ?performer] } . "
                                                               " OPTIONAL { ?song nmm:musicAlbum [nie:title ?album] } . "
                                                               " filter(?url=\"%1\") }"
                                                               "ORDER BY ?title";

const int GETMUSICSIMPLEMETADATAQUERY_DATATYPECOUNT = 4;
//}added by junam

// added by sungha.choi 2013.09.06 for ITS 0188361
const QString gGetMusicNormalMetaDataQuery =" SELECT  ?title ?performer ?album ?url ?genre"
                                                                " WHERE { "
                                                                " ?song a nmm:MusicPiece ; "
                                                                " nie:title ?title ; "
                                                                " nie:url ?url . "
                                                                " OPTIONAL {?song nmm:performer [nmm:artistName ?performer] } . "
                                                                " OPTIONAL { ?song nmm:musicAlbum [nie:title ?album] } . "
                                                                " OPTIONAL { ?song nfo:genre ?genre } . "
                                                                " filter(?url=\"%1\") }"
                                                                "ORDER BY ?title";

const int GETMUSICNORMALMETADATAQUERY_DATATYPECOUNT = 5;


/**
  * Contact Data Structures
  */
enum BT_PBAP_DB_TYPE{
        DB_PHONE_BOOK,
        DB_RECENT_CALLS,
        DB_RECEIVED_CALLS,
        DB_MISSED_CALLS,
        DB_COMBINED_CALLS,
        DB_CALL_HISTORY,
        DB_FAVORITE_PHONE_BOOK,
        DB_ALL_TYPE
/////SUPPORT_MIDDLE_NAME
		, DB_MID_PHONEBOOK
		, DB_MID_FAVORITE
/////SUPPORT_MIDDLE_NAME
        , DB_PINYIN_PHONEBOOK
    /* FEATURE_ORDER_BY_PINYIN */
};

enum BT_PBAP_DB_MEMORY{
        DB_PHONE_MEMORY,
        DB_SIM_MEMORY,
        DB_ALL_MEMORY
};

enum BT_PBAP_DB_SORTING{
        DB_SORT_BY_INDEX,
        DB_SORT_BY_FIRST_NAME,
        DB_SORT_BY_LAST_NAME,
        DB_SORT_BY_FORMMATED_NAME,
        DB_SORT_BY_TIME_STAMP
};

struct BT_PBAP_DB_CONTACT{
        QString bd_address;
        quint16 index;
        BT_PBAP_DB_TYPE type;
        BT_PBAP_DB_MEMORY memory_Type;
        QString first_name;
        QString crypto_hash;
        QString last_name;
        QString formatted_name;
        quint8 num_type1;
        QString number1;
        quint8 num_type2;
        QString number2;
        quint8 num_type3;
        QString number3;
        quint8 num_type4;
        QString number4;
        quint8 num_type5;
        QString number5;
        QString time_stamp;         // time stamp should be in "2011-11-14T17:40:00Z"  format
        QString middle_name;
        QString pinyin;
        quint8 use_pinyin;
        QString pinyin_order;
    /* FEATURE_ORDER_BY_PINYIN */
};

/**
  * Remove BT contact Structure
  */
struct SRemoveBTContact
{
	QString bd_address;
	quint16 index;
	QString first_name;
	BT_PBAP_DB_MEMORY memoryType;
	BT_PBAP_DB_TYPE dbType;
};

/**
  * IPOD volume insertion Query
  */
const QString gIpodVolumeInsertQuery = ("INSERT { _:ipod a tracker:Volume ; "
                                        "ipod:SerialNumber '%1' ; "
                                        "tracker:mountPoint _:mp . "
                                        "_:mp a nie:DataObject ; "
                                        "nie:url '%2' }");

/**
  * IPOD track insertion query
  */
const QString gIpodMusicTrackInsertQuery = ("INSERT OR REPLACE {_:ipodm a nmm:MusicPiece, nie:DataObject; "
                                             "nie:title '%1';"
                                             "nmm:performer <urn:artist:%2>; "
                                             "nmm:composer <urn:artist:%3>; "
                                             "nmm:musicAlbum <urn:album:%4>; "
                                             "nfo:genre '%5' ; "
                                             "ipod:TrackUid %6 ; "
                                             "ipod:TrackIndex %7 ;"
                                             " nie:url '%8' ;"
                                             " ipod:TypeName '%9' "
                                         " }");


/**
  * QSparql query for inserting the IPOD playlist details in the Tracker Store.
  * This query is also used for inserting Gracenote Playlists with ipod:TypeName = "MoreLikeThis"
  * ipod:TypeIndex = 0
  */

const QString gIpodPlaylistInsertQuery="INSERT OR REPLACE { _:pl a nmm:Playlist ; nie:title '%1'; nfo:entryCounter %2 ; "
                                       "ipod:TypeName '%3' ; ipod:TypeIndex %4 ; ";


const QString gIpodMusicDataQuery=" nfo:hasMediaFileListEntry [ a nfo:MediaFileListEntry; nfo:entryUrl '%1'; nfo:listPosition %2 ; ipod:entryTitle '%3'] " ;

/**
  * IPOD Podcast insertion query
  */
const QString gIpodPodcastAudioBookInsertQuery = ("INSERT {_:ipodm a nmm:MusicPiece, nie:DataObject; "
                                         "nie:title '%1';"
                                         "nmm:performer <urn:artist:%2>; "
                                         "nmm:musicAlbum <urn:album:%3>; "
                                         "nfo:genre '%4' ; "
                                         "ipod:TrackUid %5 ; "
                                         "ipod:TrackIndex %6 ;"
                                         " nie:url '%7' ;"
                                         " ipod:TypeName '%8' ;"
                                         " ipod:TypeIndex %9"
                                     " }");

/**
  * IPOD Track Removal query
  */
const QString gIpodRemoveTrackFromDBQuery = ("DELETE {?song a rdfs:Resource . }"
                                         "WHERE { ?song a nmm:MusicPiece ;"
                                         "nie:url ?url . "
                                         " FILTER( REGEX(?url, '%1')) "
                                         " }");

/**
  * IPOD AudioBook/Podcast/Playlist Removal query
  */

const QString gIpodRemovePodcastAudioBookFromDBQuery = ("DELETE {?song a rdfs:Resource . }"
                                         "WHERE { ?song a nmm:MusicPiece ;"
                                         " nie:title ?title ; "
                                         " ipod:TypeName ?type ; "
                                         " nie:url ?url . "
                                         " FILTER( REGEX(?type, '%1')) . "
                                         " FILTER( REGEX(?title, '%2')) . "
                                         " FILTER(REGEX(?url, '%3'))"
                                         " }");

/**
  * IPOD  Removal of all information query
  */
const QString gIpodRemoveAllInfoFromDBQuery = ("DELETE {?song a rdfs:Resource . }"
                                         "WHERE { ?song a nmm:MusicPiece ;"
                                         " nie:url ?url . "
                                         " FILTER(REGEX(?url, '%3'))"
                                         " }");
/**
  * QSparql query for updating the Artist name for the media file records
  * into the Tracker Store.
  * This query is also used to insert Gracenote MetaData
  */
const QString gIpodArtistNameUpdateQuery = (" INSERT OR REPLACE {<urn:artist:%1> a nmm:Artist ;nmm:artistName '%2' }");

/**
  * QSparql query for updating the Album details for the media file records
  * into the Tracker Store.
  */
const QString gIpodAlbumUpdateQuery =  ("INSERT OR REPLACE {<urn:album:%1> a nmm:MusicAlbum ;nmm:albumTitle '%2' ; nie:title '%2' }");

/**
  * IPOD Data Structure
  */

//Type of the info
const QString gIPODAllSongType("AllSongs");
const QString gIPODPlayListType("PlayList");
const QString gIPODAudioBookType("AudioBook");
const QString gIPODPodcastType("Podcast");

// IPOD track info structure
struct SAppleTrackInfo
{
       quint64  m_UID;
       // m_Type values : AllSongs,Audiobook,Podcasts,iTunesU, Playlist
       QString m_Type;
       QString m_Title;
       QString m_Artist;
       QString m_Album;
       QString m_Genre;
       QString m_Composer;
       int m_trackIndex;

       SAppleTrackInfo () : m_UID(-1),
                            m_Type(""),
                            m_Title(""),
                            m_Artist(""),
                            m_Album(""),
                            m_Genre(""),
                             m_Composer(""),
                            m_trackIndex(0){}
};
struct SAppleInfoList
{
    int count;
    QString m_ipodSerialNumber;
    QList<SAppleTrackInfo> m_AppleInfoList;
};

//IPOD AudioBook/ PodCast/ PlayList Structure
struct SAppleHierarchyInfo // for Audiobook, PodCast, PlayList
{
    //UID
    quint64  m_UID;
    // AudioBook, Podcast, Playlist, iTunesU, AllSongs
    QString m_Type;
    // name of chapter or episode
    QString m_Title;
    QString m_Artist;
    QString m_Album;
    QString m_Genre;
    QString m_Composer;
    // Audiobook,Podcasts,iTunesU,playlist name
    QString m_TopLevelName;
    // Audiobook,Podcasts,iTunesU, number for chapter/episode
    int     m_ChapterOrEpisodeNumber;
    // Number of tracks in PodCast/ AudioBook/Album
    int m_TrackCount;
    //Index of the episode, chaptor or track
    int m_trackIndex;
    // Podcasts,Playlist,iTunesU,AudioBook Index
    int m_TopLevelIndex;

    SAppleHierarchyInfo () : m_UID(-1),
                                m_Title(""),
                                m_Artist(""),
                                m_Album(""),
                                m_Genre(""),
                                m_Composer(""),
                                m_TopLevelName(""),
                                m_ChapterOrEpisodeNumber(0),
                                m_TrackCount(0),
                                m_trackIndex(0),
                                m_TopLevelIndex(0){}
};
struct SAppleHierarchyInfoList
{
    QString m_ipodSerialNumber;
    QList<SAppleHierarchyInfo> m_AppleHierarchyInfoList;
};

//Track remove struct
struct SIPodTrackRemoveStruct
{
QString SerialNumber;
int TrackIndex;
double TrackUID;
};

struct SCoverartMusicData{

    QString imagePath;
    QString track_artist;
    QString album;

    };

//AudioBook / podcast/Playlist remove struct

struct SIPodTypeRemoveInfo
{
QString SerialNumber;
QString IPodTypeName;
};

//Queries for updating Gracenote data into Tracker Store.

/**
  * QSparql query for updating the Artist name for the media file records
  * into the Tracker Store.
  */
const QString gArtistNameUpdateQuery = (" INSERT OR REPLACE {<urn:artist:%1> a nmm:Artist ;nmm:artistName '%2' }");

/**
  * QSparql query for updating the Album details for the media file records
  * into the Tracker Store.
  */
const QString gAlbumUpdateQuery =  ("INSERT OR REPLACE {<urn:album:%1> a nmm:MusicAlbum ;nmm:albumTitle '%2' }");

/**
  * QSparql query for updating the CD Album details for the media file records
  * into the Tracker Store.
  */
const QString gCDAlbumUpdateQuery =  ("INSERT OR REPLACE {<urn:album:%1> a nmm:MusicAlbum ;nmm:albumTitle '%2' ;  nmm:albumArtist <urn:artist:%3>; nmm:albumTrackCount %4}");

/**
  * QSparql query for updating the MusicAlbumDisc details for the media file records
  * into the Tracker Store.
  */
const QString gMusicAlbumDiscUpdateQuery =  ("INSERT OR REPLACE {<urn:album-disc:%1> a nmm:MusicAlbumDisc ;nmm:musicCDIdentifier '%2'}");

/**
  * QSparql query for updating the Music Album CD details for the media file records
  * into the Tracker Store.
  */
const QString gCDAlbumInfoUpdateQuery =  ( "INSERT OR REPLACE {<urn:uid:%1> a nmm:MusicPiece, nfo:Audio,nie:DataObject;"
                                                                            "nie:title '%2';"
                                                                            "nmm:performer <urn:artist:%3>; "
                                                                            "nmm:musicAlbum <urn:album:%4>; "
                                                                            "nfo:genre '%5';"
                                                                            "nmm:trackNumber %6;"
                                                                            "nmm:musicAlbumDisc <urn:album-disc:%7> ;"
                                                                            "nfo:duration %8 ; "
                                                                            " nie:url '%9' } ");

/**
  * QSparql query for getting the Urn for the media file records
  * into the Tracker Store based on a CD TOC
  */
const QString gCDFileUrnQuery =("SELECT ?song WHERE {?song a nmm:MusicPiece ; nmm:musicAlbumDisc [nmm:musicCDIdentifier '%1'] } ");

/**
  * QSparql query for deleting an entry from tracker store
  */
const QString gDeleteCDFilesQuery =("DELETE {<%1> a nmm:MusicPiece} ");

/**
  * QSparql query for getting the Urn for the media file records
  * into the Tracker Store.
  */
const QString gFileUrnQuery =("SELECT ?song WHERE {?song a nmm:MusicPiece ; nie:url '%1' } ");

/**
  * QSparql query for updating the basic information of the media file records
  * into the Tracker Store.
  */
const QString gBasicMusicUpdateQuery  = ( "INSERT OR REPLACE {<%1> a nmm:MusicPiece, nie:DataObject;"
                         "nie:title '%2';"
                         "nmm:performer <urn:artist:%3>; "
                         "nmm:composer <urn:artist:%4>; "
                         "nmm:musicAlbum <urn:album:%5>; "
                         "nfo:genre '%6' }");

/**
  * QSparql query for updating the advanced information of the media file records
  * into the Tracker Store.
  */
const QString gAdvancedMusicInfoUpdateQuery = ("INSERT OR REPLACE { <%1> a nmm:MusicPiece ;"
                                               "nie:contentCreated '%1' ;' "
                                               "nie:created '%2' ; }");



const QString gGetAllVideosCountFromVolumeQuery = " SELECT count(?video) "
                                                                                 " WHERE {  "
                                                                                 " ?video a nmm:Video; "
                                                                                 " nie:url ?url . "
                                                                                 " %1 }";
const int GETALLVIDEOSCOUNTFROMVOLUME_DATATYPECOUNT = 1;

const QString gGetAllImagesCountFromVolumeQuery = " SELECT count(?image) "
                                                                                 " WHERE {  "
                                                                                 " ?image a nfo:Image; "
                                                                                 " nie:url ?url . "
                                                                                 " %1 }";
const int GETALLIMAGESCOUNTFROMVOLUME_DATATYPECOUNT = 1;


// { modified by eugene 2012.12.21
/*
const QString gGetAllSongsByAlbumFromVolumeQuery = " SELECT ?title ?url ?performer "
                                                                                   " WHERE { "
                                                                                   " ?song a nmm:MusicPiece ; "
                                                                                   " nmm:musicAlbum [ nie:title \"%1\"] ; "
                                                                                   " nie:title ?title ; "
                                                                                   " nmm:performer [nmm:artistName ?performer ] ; "
                                                                                   " nie:url ?url .  %2 } ORDER BY ?title";
*/

//{ modified by jaehwan.lee 2014.01.13 for track number sorting in the album depth_1
const QString gGetAllSongsByAlbumFromVolumeQuery = " SELECT ?title ?url ?performer "
                                                   " WHERE { "
                                                   " ?song a nmm:MusicPiece ; "
                                                   " nmm:musicAlbum [ nie:title \"%1\"] ; "
                                                   " nie:title ?title ; "
                                                   " nie:url ?url . "
                                                   " OPTIONAL { ?song nmm:performer [nmm:artistName ?performer ] } . "
                                                   " OPTIONAL { ?song nmm:trackNumber ?trackno } "
                                                   " %2 } ORDER BY !BOUND(?trackno) ?trackno ?title ?url";
//}

//{ added by jaehwan.lee 2013.10.06, modified by jaehwan.lee 2014.01.13 for track number sorting in the album depth_1
const QString gGetAllSongsByAlbumFromVolumeExtOrderQuery = " SELECT ?title ?url ?performer "
                                                   " WHERE { "
                                                   " ?song a nmm:MusicPiece ; "
                                                   " nmm:musicAlbum [ nie:title \"%1\"] ; "
                                                   " nie:title ?title ; "
                                                   " nie:url ?url . "
                                                   " OPTIONAL { ?song nmm:performer [nmm:artistName ?performer ] } . "
                                                   " OPTIONAL { ?song nmm:trackNumber ?trackno } "
                                                   " %2 } ORDER BY !BOUND(?trackno) ?trackno %3 ?title ?url";
//}

// } modified by eugene 2012.12.21

const int GETALLSONGSBYALBUMFROMVOLUME=3;

const QString gGetAllSongsByComposerAlbumQuery =("SELECT DISTINCT ?url ?title "
                                                                     "WHERE {"
                                                                     "?song a nmm:MusicPiece ;"
                                                                     " nie:title ?title ; nie:url ?url ; "
                                                                     " nmm:composer [nmm:artistName ?artist] ;"
                                                                     " nmm:musicAlbum [nie:title ?album] ."
                                                                     " FILTER(?artist = \"%1\") ."
                                                                     " FILTER(?album = \"%2\") ."
                                                                     " %3"
                                                                     "} "
                                                                     "ORDER BY ?title");

const int GETALLSONGSBYCOMPOSERALBUM=2;

const QString gGetAllAlbumByGenreComposerQuery="SELECT DISTINCT ?album "
                                                               "WHERE {"
                                                               "?song a nmm:MusicPiece ;"
                                                               " nie:url ?url ;"
                                                               " nmm:composer [nmm:artistName ?artist] ;"
                                                               " nfo:genre ?genre ;"
                                                               " nmm:musicAlbum [nie:title ?album] ."
                                                               " FILTER(?genre = \"%1\") ."
                                                               " FILTER(?artist = \"%2\") ."
                                                               " %3"
                                                               "} "
                                                               "ORDER BY ?album";

const int GETALLALBUMBYGENERECOMPOSER=1;
//TODO - item increase from 1 to 3 for iPod but this query did not changed because it dose not use now.
//Need to change if it will using from jukebox
const QString gGetAllAlbumByComposerQuery="SELECT DISTINCT ?album "
                                                            "WHERE {"
                                                            "?song a nmm:MusicPiece ;"
                                                            " nie:url ?url ;"
                                                            " nmm:composer [nmm:artistName ?artist] ;"
                                                            " nmm:musicAlbum [nie:title ?album] ."
                                                            " FILTER(?artist = \"%1\") ."
                                                            " %2"
                                                            "} "
                                                            "ORDER BY ?album";

const int GETALLALBUMBYCOMPOSER=3; //changed by junam 2013.10.28 for 2nd depth coverart

const QString gGetAllComposerByGenreQuery="SELECT DISTINCT ?artist "
                                                                    "WHERE {"
                                                                    "?song a nmm:MusicPiece ;"
                                                                    " nie:url ?url ;"
                                                                    " nfo:genre ?genre ;"
                                                                    " nmm:composer [ nmm:artistName ?artist ] ."
                                                                    " FILTER(?genre = \"%1\") ."
                                                                    " %2"
                                                                    "} "
                                                                    "ORDER BY ?artist";

const int GETALLCOMPOSERBYGENER=1;

const QString gGetImagesBySubFolderQuery = " SELECT DISTINCT ?file ?url "
" WHERE {  "
" ?image a nfo:Image , nmm:Photo . "
" OPTIONAL { ?image nfo:fileName ?file } . "
" ?image nie:url ?url . "
" Filter(REGEX(?url , \"%1\" , \"i\"))}";

const int GETIMAGESBYSUBFOLDERQUERY_DATATYPECOUNT = 2;

const QString gGetVideosBySubFolderQuery = " SELECT DISTINCT ?file ?url ?title "
" WHERE {  "
" ?video a nmm:Video . "
" OPTIONAL { ?video nfo:fileName ?file } . "
" OPTIONAL {?video nie:title ?title } "
" ?video nie:url ?url . "
" Filter(REGEX(?url , \"%1\" , \"i\"))}";

const int GETVIDEOSBYSUBFOLDERQUERY_DATATYPECOUNT = 3;

const QString gGetAllAlbumsWithCoverArtFromVolumeQuery = " SELECT DISTINCT ?album ?performer "
                                             " WHERE { "
                                             " ?song a nmm:MusicPiece ; "
                                             " nmm:performer [nmm:artistName ?performer ] ; "
                                             " nmm:musicAlbum [nie:title ?album] ;"
                                             " nie:url ?url . %1 } GROUP BY ?album ORDER BY ?album";

//{ added by jaehwan.lee 13.10.02 for audio list sorting issue 
const QString gGetAllAlbumsWithCoverArtFromVolumeExtOrderQuery = " SELECT DISTINCT ?album ?performer "
                                             " WHERE { "
                                             " ?song a nmm:MusicPiece ; "
                                             " nmm:performer [nmm:artistName ?performer ] ; "
                                             " nmm:musicAlbum [nie:title ?album] ;"
                                             " nie:url ?url . %1 } "
                                             " GROUP BY "
                                             " %2 "
                                             " ?album "
                                             " ORDER BY "
                                             " %2 "
                                             " ?album";

//} 
const int GETALLALBUMSWITHCOVERARTFROMVOLUMEQUERY_DATATYPECOUNT = 2;

const QString gGetAllMusicByKeywordFromVolumeQuery = " SELECT ?title ?album ?performer ?genre ?url  "
                                                                     " WHERE { "
                                                                     " ?song a nmm:MusicPiece  ;  "
                                                                     " nie:url ?url  .  "
                                                                     " OPTIONAL { ?song nie:title ?title }  .  "
                                                                     " OPTIONAL { ?song  nmm:performer [nmm:artistName ?performer ] }  .  "
                                                                     " OPTIONAL { ?song  nfo:genre ?genre }  . "
                                                                     " OPTIONAL { ?song  nmm:musicAlbum [nie:title ?album] }  .  "
                                                                     " FILTER( ( REGEX(?album, \"%1\", \"i\") || "
                                                                     " REGEX(?album, \"%2\", \"i\") ||"
                                                                     " REGEX(?performer, \"%1\", \"i\") || "
                                                                     " REGEX(?performer, \"%2\", \"i\") || "
                                                                     " REGEX(?title, \"%1\", \"i\") || "
                                                                     " REGEX(?title, \"%2\", \"i\") || "
                                                                     " REGEX(?genre, \"%1\", \"i\") || "
                                                                     " REGEX(?genre, \"%2\", \"i\") ) && "
                                                                     " REGEX (?url, \"^%3\", \"i\") ) }";

const int GETALLMUSICBYKEYWORDFROMVOLUMEQUERY_DATATYPECOUNT = 5;

//modified by jaehwan 2013.11.27 to fix ITS 210480, modified by jaehwan.lee 2014.01.13 for track number sorting in the album depth_1
const QString gGetAllAlbumsWithUrlFromVolumeQuery = " SELECT DISTINCT ?album ?performer ?url "
                                             " WHERE { "
                                             " ?song a nmm:MusicPiece ; "
                                             " nmm:performer [nmm:artistName ?performer ] ; "
                                             " nmm:musicAlbum [nie:title ?album] ;"
                                             " nie:title ?title ;"
                                             " nie:url ?url . "
                                             " OPTIONAL { ?song nmm:trackNumber ?trackno } "
                                             " %1 } "
                                             " GROUP BY ?album !BOUND(?trackno) ?trackno ?title ?url "
                                             " ORDER BY ?album !BOUND(?trackno) ?trackno ?title ?url ";
//modified by jaehwan 2013.11.27 to fix ITS 210480
//{ added by jaehwan.lee for audio list sorting issue, modified by jaehwan.lee 2014.01.13 for track number sorting in the album depth_1
const QString gGetAllAlbumsWithUrlFromVolumeExtOrderQuery = " SELECT DISTINCT ?album ?performer ?url "
                                             " WHERE { "
                                             " ?song a nmm:MusicPiece ; "
                                             " nmm:performer [nmm:artistName ?performer ] ; "
                                             " nmm:musicAlbum [nie:title ?album] ;"
                                             " nie:title ?title ;"
                                             " nie:url ?url . "
                                             " OPTIONAL { ?song nmm:trackNumber ?trackno } "
                                             " %1 }"
                                             " GROUP BY %2 ?url "
                                             " ORDER BY %2 ?url";

//} 
const int GETALLALBUMSWITHURLFROMVOLUMEQUERY_DATATYPECOUNT = 3;


const QString gGetAllSongsByArtistFromVolumeQuery = " SELECT ?title ?url "
                                                        " WHERE { "
                                                        " ?song a nmm:MusicPiece ; "
                                                        " nie:title ?title ;  "
                                                        " nmm:performer[ nmm:artistName \"%1\"] ; "
                                                        " nie:isStoredAs ?as . "
                                                        " ?as nie:url ?url . %2 } "
                                                        "ORDER BY ?title";

//{ added by jaehwan.lee for audio list sorting issue  
const QString gGetAllSongsByArtistFromVolumeExtOrderQuery = " SELECT ?title ?url "
                                                        " WHERE { "
                                                        " ?song a nmm:MusicPiece ; "
                                                        " nie:title ?title ;  "
                                                        " nmm:performer[ nmm:artistName \"%1\"] ; "
                                                        " nie:isStoredAs ?as . "
                                                        " ?as nie:url ?url . %2 } "
                                                        "ORDER BY "
                                                        "%3 "
                                                        "?title";
                                 
//}

const int GETALLSONGSBYARTISTFROMVOLUMEQUERY_DATATYPECOUNT = 2;

// { added eugene.seo 2013.02.13
const QString gGetAllSongsByGenreFromVolumeQuery = " SELECT ?title ?url "
                                                        " WHERE { "
                                                        " ?song a nmm:MusicPiece ; "
                                                        " nie:title ?title ;  "
                                                        " nfo:genre \"%1\" ; "
                                                        " nie:isStoredAs ?as . "
                                                        " ?as nie:url ?url . %2 } "
                                                        "ORDER BY ?title";
//{ added by jaehwan.lee for audio list sorting issue  
const QString gGetAllSongsByGenreFromVolumeExtOrderQuery = " SELECT ?title ?url "
                                                        " WHERE { "
                                                        " ?song a nmm:MusicPiece ; "
                                                        " nie:title ?title ;  "
                                                        " nfo:genre \"%1\" ; "
                                                        " nie:isStoredAs ?as . "
                                                        " ?as nie:url ?url . %2 } "
                                                        "ORDER BY %3 ?title";
//}
const int GETALLSONGSBYGENREFROMVOLUMEQUERY_DATATYPECOUNT = 2;
// } added eugene.seo 2013.02.13


const QString gGetAllSongsWithTitleCountFromVolumeQuery = " SELECT COUNT(?title) "
                                                          " WHERE { "
                                                          " ?song a nmm:MusicPiece ; "
                                                          " nie:title ?title ; " 
                                                          " nie:url ?url . "
                                                          " %1 "
                                                          " } ";

const int GETALLSONGSWITHTITLECOUNTFROMVOLUMEQUERY_DATATYPECOUNT = 1;

// { modified by eugene 2012.12.21
/*
const QString gGetAllAlbumsCountFromVolumeQuery = " SELECT COUNT(DISTINCT ?album) "
                                                  " WHERE { "
                                                  " ?song a nmm:MusicPiece ; "
                                                  " nmm:performer [nmm:artistName ?performer ] ; "
                                                  " nmm:musicAlbum [nie:title ?album] ; "
                                                  " nie:url ?url . "
                                                  " %1 "
                                                  " } ";
*/
const QString gGetAllAlbumsCountFromVolumeQuery = " SELECT COUNT(DISTINCT ?album) "
                                                  " WHERE { "
                                                  " ?song a nmm:MusicPiece ; "                                                 
                                                  " nmm:musicAlbum [nie:title ?album] ; "
                                                  " nie:url ?url . "
                                                  " %1 "
                                                  " } ";
// } modified by eugene 2012.12.21
const int GETALLALBUMSCOUNTFROMVOLUMEQUERY_DATATYPECOUNT = 1;


const QString gGetAllMusicArtistsCountFromVolumeQuery = " SELECT COUNT(DISTINCT ?performer) "
                                                        " WHERE { "
                                                        " ?song a nmm:MusicPiece ; "
                                                        " nie:title ?title ; " 
                                                        " nmm:performer [nmm:artistName ?performer ] ; "
                                                        " nie:url ?url . "
                                                        " %1 "
                                                        " } ";

const int GETALLMUSICARTISTSCOUNTFROMVOLUMEQUERY_DATATYPECOUNT = 1;

const QString gGetAllMusicComposersCountFromVolumeQuery = " SELECT COUNT(DISTINCT ?composer) "
                                                          " WHERE { "
                                                          " ?song a nmm:MusicPiece . "
                                                          " ?song nmm:composer [nmm:artistName ?composer ] ; "
                                                          " nie:url ?url . "
                                                          " %1 "
                                                          " } ";

const int GETALLMUSICCOMPOSERSCOUNTFROMVOLUMEQUERY_DATATYPECOUNT = 1;

const QString gGetAllMusicGenresCountFromVolumeQuery = " SELECT COUNT(DISTINCT ?genre) "
                                                       " WHERE { "
                                                       " ?song a nmm:MusicPiece . "
                                                       " ?song nfo:genre ?genre ; "
                                                       " nie:url ?url . "
                                                       " %1 "
                                                       " } "; 

const int GETALLMUSICGENRESCOUNTFROMVOLUMEQUERY_DATATYPECOUNT = 1;

const QString gGetAllAlbumsWithCoverArtCountFromVolumeQuery = " SELECT COUNT(DISTINCT ?album) "
                                             				  " WHERE { "
                                             				  " ?song a nmm:MusicPiece ; "
                                             				  " nmm:performer [nmm:artistName ?performer ] ; "
                                                              " nmm:musicAlbum [nie:title ?album] ;"
                                                              " nie:url ?url . %1 }";

const int GETALLALBUMSWITHCOVERARTCOUNTFROMVOLUMEQUERY_DATATYPECOUNT = 1;

const QString gGetAllSongsWithLimitFromVolumeQuery = "SELECT  ?url ?title "
                                           "WHERE {"
                                           " ?song a nmm:MusicPiece ; "
                                           "         nie:title ?title ; "                                           
                                           " nie:url ?url . "
                                           "%1"
                                           "} ORDER BY ?title OFFSET %2 LIMIT %3 ";
//{ added by jaehwan.lee for audio list sorting issue 
/*const QString gGetAllSongsKorWithLimitFromVolumeQuery = "SELECT  ?url ?title "
                                           "WHERE {"
                                           " ?song a nmm:MusicPiece ; "
                                           "         nmm:titleWeightKor ?weight; "  
                                           "         nie:title ?title ; "                                           
                                           " nie:url ?url . "
                                           "%1"
                                           "} ORDER BY ?weight ?title OFFSET %2 LIMIT %3 ";

const QString gGetAllSongsArbWithLimitFromVolumeQuery = "SELECT  ?url ?title "
                                           "WHERE {"
                                           " ?song a nmm:MusicPiece ; "
                                           "         nmm:titleWeightArb ?weight; "  
                                           "         nie:title ?title ; "                                           
                                           " nie:url ?url . "
                                           "%1"
                                           "} ORDER BY ?weight ?title OFFSET %2 LIMIT %3 ";

*/
const QString gGetAllSongsWithLimitFromVolumeExtOrderQuery  = "SELECT  ?url ?title "
                                           "WHERE {"
                                           " ?song a nmm:MusicPiece ; "
                                           "         nie:title ?title ; "                                           
                                           " nie:url ?url . "
                                           "%1"
                                           "} ORDER BY  "
                                           "%2 "
                                           " ?title OFFSET %3 LIMIT %4 ";
//}                                           
const int GETALLSONGSWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT = 2;

const QString gGetAllAlbumsWithLimitFromVolumeQuery = " SELECT DISTINCT ?album ?performer "
                                             " WHERE { "
                                             " ?song a nmm:MusicPiece ; "
                                             " nmm:performer [nmm:artistName ?performer ] ; "
                                             " nmm:musicAlbum [nie:title ?album] ;"
                                             " nie:url ?url . %1 } ORDER BY ?album  OFFSET %2 LIMIT %3 ";

//{ added by jaehwan.lee for audio list sorting issue 

 const QString gGetAllAlbumsWithLimitFromVolumeExtOrderQuery = " SELECT DISTINCT ?album ?performer "
                                             " WHERE { "
                                             " ?song a nmm:MusicPiece ; "
                                             " nmm:performer [nmm:artistName ?performer ] ; "
                                             " nmm:musicAlbum [nie:title ?album] ;"
                                             " nie:url ?url . %1 } "
                                             " ORDER BY  "
                                             " %2 "
                                             " ?album  OFFSET %3 LIMIT %4 ";

//}
const int GETALLALBUMSWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT = 2;

const QString  gGetAllMusicArtistsWithLimitFromVolumeQuery = " SELECT DISTINCT  ?performer "
                                                               " WHERE {"
                                                               " ?song a nmm:MusicPiece ; "
                                                               "         nie:title ?title ; "
                                                               "         nmm:performer [nmm:artistName ?performer ] ; "                                                               
                                                               " nie:url ?url .  %1"
                                                               " } "
                                                               "ORDER BY ?performer OFFSET %2 LIMIT %3 ";

//{ added by jaehwan.lee for audio list sorting issue 

const QString  gGetAllMusicArtistsWithLimitFromVolumeExtOrderQuery = " SELECT DISTINCT  ?performer "
                                                               " WHERE {"
                                                               " ?song a nmm:MusicPiece ; "
                                                               "         nie:title ?title ; "
                                                               "         nmm:performer [nmm:artistName ?performer ] ; "                                                               
                                                               " nie:url ?url .  %1"
                                                               " } "
                                                               " ORDER BY "
                                                               " %2 "
                                                               " ?performer OFFSET %3 LIMIT %4 ";
//}
const int GETALLMUISCARTISTSWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT = 1;

const QString gGetAllMusicComposersWithLimitFromVolumeQuery = "SELECT DISTINCT  ?composer "
                                                                   "WHERE {"
                                                                   " ?song a nmm:MusicPiece . "
                                                                   " ?song nmm:composer [nmm:artistName ?composer ]  ;"                                                                   
                                                                   " nie:url ?url . %1"
                                                                   "} "
                                                                   "ORDER BY ?composer OFFSET %2 LIMIT %3 ";

const int GETALLMUSICCOMPOSERSWITHLIMITFORMVOLUMEQUERY_DATATYPECOUNT = 1;

const QString gGetAllMusicGenresWithLimitFromVolumeQuery = " SELECT  DISTINCT  ?genre  "
                                                                                  " WHERE {"
                                                                                  " ?song a nmm:MusicPiece . "
                                                                                  " ?song nfo:genre ?genre ; "                                                                                  
                                                                                  " nie:url ?url . %1"
                                                                                  " } "
                                                                                  " ORDER BY ?genre OFFSET %2 LIMIT %3 ";

const int GETALLMUSICGENRESWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT = 1;

// { modified by eugene 2012.12.21
/*
const QString gGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery = " SELECT DISTINCT ?album ?performer ?url"
                                             " WHERE { "
                                             " ?song a nmm:MusicPiece ; "
                                             " nmm:performer [nmm:artistName ?performer ] ; "
                                             " nmm:musicAlbum [nie:title ?album] ;"
                                             " nie:url ?url . %1 } GROUP BY ?album ORDER BY ?album OFFSET %2 LIMIT %3";
*/
const QString gGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery = " SELECT DISTINCT ?album ?performer ?url"
                                             " WHERE { "
                                             " ?song a nmm:MusicPiece ; "                                            
                                             " nmm:musicAlbum [nie:title ?album] ;"
                                             " nie:url ?url . "
											 " OPTIONAL { ?song nmm:performer [nmm:artistName ?performer ] } . "
											 " %1 } GROUP BY ?album ORDER BY ?album OFFSET %2 LIMIT %3";


const QString gGetAllAlbumsWithCoverArtWithLimitFromVolumeExtOrderQuery = " SELECT DISTINCT ?album ?performer ?url"
                                             " WHERE { "
                                             " ?song a nmm:MusicPiece ; "                                            
                                             " nmm:musicAlbum [nie:title ?album] ;"
                                             " nie:url ?url . "
						   " OPTIONAL { ?song nmm:performer [nmm:artistName ?performer ] } . "
						   " %1 } GROUP BY "
						   " %2 "
						   " ?album "
						   " ORDER BY "
						   " %2 "
						   " ?album "
						   " OFFSET %3 LIMIT %4";


// } modified by eugene 2012.12.21
const int GETALLALBUMSWITHCOVERARTWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT = 3;

const QString gGetAllSongsWithTitleIndexFromVolumeQuery = "SELECT ?title "
                                           "WHERE {"
                                           " ?song a nmm:MusicPiece ; "
                                           "         nie:title ?title ; "                                           
                                           " nie:url ?url . "
                                           "%1"
                                           "} "
                                           " ORDER BY ?title";
                                           
//{ added by jaehwan.lee for audio list sorting issue 
const QString gGetAllSongsWithTitleIndexFromVolumeExtOrderQuery = "SELECT ?title "
                                           "WHERE {"
                                           " ?song a nmm:MusicPiece ; "
                                           "         nie:title ?title ; "                                           
                                           " nie:url ?url . "
                                           "%1"
                                           "} "
                                           " ORDER BY "
                                           "%2 "
                                           "?title";

//}                                          
const int GETALLSONGSWITHTITLEINDEXFROMVOLUMEQUERY_DATATYPECOUNT = 1;

const QString gGetAllAlbumsWithCoverArtIndexFromVolumeQuery = " SELECT DISTINCT ?album "
                                             " WHERE { "
                                             " ?song a nmm:MusicPiece ; "
                                             " nmm:performer [nmm:artistName ?performer ] ; "
                                             " nmm:musicAlbum [nie:title ?album] ;"
                                             " nie:url ?url . %1 } GROUP BY ?album ORDER BY ?album";

//{ added by jaehwan.lee for audio list sorting issue 
const QString gGetAllAlbumsWithCoverArtIndexFromVolumeExtOrderQuery = " SELECT DISTINCT ?album "
                                             " WHERE { "
                                             " ?song a nmm:MusicPiece ; "
                                             " nmm:performer [nmm:artistName ?performer ] ; "
                                             " nmm:musicAlbum [nie:title ?album] ;"
                                             " nie:url ?url . %1 } "
                                             " GROUP BY %2 ?album "
                                             " ORDER BY %2 ?album";
                                            
//} added by jaehwan.lee

const int GETALLALBUMSWITHCOVERARTINDEXFROMVOLUMEQUERY_DATATYPECOUNT = 1;

/**
   * Mapping function to map query with respective query ids
   */
static QMap<QString, ETrackerAbstarctor_Query_Ids> GetMapQueryIds() {
    QMap<QString, ETrackerAbstarctor_Query_Ids> mapQueryIds;
    mapQueryIds.insert("gSongsByFolderQuery", eSongsByFolderQuery_Id);
    mapQueryIds.insert("gSongsByEmptyFolderQuery", eSongsByFolderQuery_Id);
    mapQueryIds.insert("gAlbumByFolderQuery",eAlbumByFolderQuery_Id);
    mapQueryIds.insert("gAlbumByEmptyFolderQuery",eAlbumByFolderQuery_Id);
    mapQueryIds.insert("gArtistByFolderQuery", eArtistByFolderQuery_Id);
    mapQueryIds.insert("gArtistByEmptyFolderQuery", eArtistByFolderQuery_Id);
    mapQueryIds.insert("gGetAllSongsQuery", eGetAllSongsQuery_Id);
    mapQueryIds.insert("gFolderListQuery",eFolderListQuery_Id);
    mapQueryIds.insert("gMusicAlbumListQuery",eMusicAlbumListQuery_Id);
    mapQueryIds.insert("gGetAllAlbumsQuery", eGetAllAlbumsQuery_Id);
    mapQueryIds.insert("gGetAllMusicArtistsQuery", eGetAllMusicArtistsQuery_Id);
    mapQueryIds.insert("gGetAllMusicComposersQuery", eGetAllMusicComposersQuery_Id);
    mapQueryIds.insert("gGetAllMusicGenresQuery", eGetAllMusicGenresQuery_Id);
    mapQueryIds.insert("gGetMusicMetaDataQuery", eGetMusicMetaDataQuery_Id);
    mapQueryIds.insert("gGetMusicMetaDataInfoFromUrlListQuery", eGetMusicMetaDataInfoFromUrlListQuery_Id);
    mapQueryIds.insert("gSongsByAlbumQuery", eGetAllSongsByAlbumQuery_Id);
    mapQueryIds.insert("gGetAllSongsByArtistQuery", eGetAllSongsByArtistQuery_Id);
    mapQueryIds.insert("gGetAllSongsByGenreQuery", eGetAllSongsByGenreQuery_Id);
    mapQueryIds.insert("gGetAllSongsByComposerQuery", eGetAllSongsByComposerQuery_Id);
    mapQueryIds.insert("gGetImagesByFolderQuery", eGetImagesByFolderQuery_Id);
    mapQueryIds.insert("gGetVideosByFolderQuery", eGetVideosByFolderQuery_Id);
    mapQueryIds.insert("gGetAllMusicByKeywordQuery", eGetAllMusicByKeywordQuery_Id);
    mapQueryIds.insert("gGetMultipleArtistSongsQuery", eGetMultipleArtistSongsQuery_Id);
    mapQueryIds.insert("gGetMultipleAlbumSongsQuery", eGetMultipleAlbumSongsQuery_Id);
    mapQueryIds.insert("gGetAllImagesByKeywordQuery" ,eGetAllImagesByKeywordQuery_Id);
    mapQueryIds.insert("gGetAllVideosByKeywrodQuery" ,eGetAllVideosByKeywrodQuery_Id);
    mapQueryIds.insert("gSongsCountByFolderQuery" ,eSongsCountByFolderQuery_Id);
    mapQueryIds.insert("gGetImagesCountByFolderQuery" ,eGetImagesCountByFolderQuery_Id);
    mapQueryIds.insert("gGetVideosCountByFolderQuery" ,eGetVideosCountByFolderQuery_Id);
    mapQueryIds.insert("gGetAllSongsCountQuery" ,eGetAllSongsCountQuery_Id);
    mapQueryIds.insert("gGetAllSongsFromVolumeQuery" ,eGetAllSongsFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllAlbumsFromVolumeQuery" ,eGetAllAlbumsFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllMusicArtistsFromVolumeQuery" ,eGetAllMusicArtistsFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllMusicComposersFromVolumeQuery" ,eGetAllMusicComposersFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllMusicGenresFromVolumeQuery" ,eGetAllMusicGenresFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllImagesFromVolumeQuery" ,eGetAllImagesFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllVideosFromVolumeQuery" ,eGetAllVideosFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllSongsWithMetaDataFromVolumeQuery" ,eGetAllSongsWithMetaDataFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllSongsIDQuery" ,eGetAllSongsIDQuery_id);
    mapQueryIds.insert("gGetAllSongsByGenreArtistAlbumQuery" , eGetAllSongsByGenreArtistAlbumQuery_Id);
    mapQueryIds.insert("gGetAllSongsByGenreComposerAlbumQuery" , eGetAllSongsByGenreComposerAlbumQuery_Id);
    mapQueryIds.insert("gGetAllSongsByArtistAlbumQuery" , eGetAllSongsByArtistAlbumQuery_Id);
    mapQueryIds.insert("gGetAllAlbumByGenreArtistQuery" , eGetAllAlbumByGenreArtistQuery_Id);
    mapQueryIds.insert("gGetAllAlbumByArtistQuery" , eGetAllAlbumByArtistQuery_Id);
    mapQueryIds.insert("gGetAllArtistByGenreQuery" , eGetAllArtistByGenreQuery_Id);
    mapQueryIds.insert("gGetAllSongsByKeywordFromVolumeQuery_key",eGetAllsongsByKeywordFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllArtistsByKeywordFromVolumeQuery_key",eGetAllArtistsByKeywordFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllAlbumsByKeywordFromVolumeQuery_key",eGetAllAlbumsByKeywordFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllGenresByKeywordFromVolumeQuery_key",eGetAllGenresByKeywordFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllSongsByAlbumFromVolumeQuery_key" ,eGetAllSongsByAlbumFromVolumeQuery_Id);

	mapQueryIds.insert("gGetAllPodcastsByTitleQuery",eGetAllPodcastsByTitleQuery_Id);
	mapQueryIds.insert("gGetAllAudiobooksByTitleQuery",eGetAllAudiobooksByTitleQuery_Id);
	mapQueryIds.insert("gGetAllPodcastsEpisodeNamesQuery",eGetAllPodcastsEpisodeNamesQuery_Id);
	mapQueryIds.insert("gGetAllAudiobookChapterNamesQuery",eGetAllAudiobookChapterNamesQuery_Id);
	
	mapQueryIds.insert("gGetMusicPlaylistByKeywordQuery" ,eGetMusicPlaylistByKeywordQuery_Id);
    mapQueryIds.insert("gGetAllSongsbyPlaylistQuery" ,eGetAllSongsbyPlaylistQuery_Id);
    mapQueryIds.insert("gGetlistofPlaylistsQuery" ,eGetlistofPlaylistsQuery_Id);
    mapQueryIds.insert("gGetMostPlayedMusicSongsQuery" ,eGetMostPlayedMusicSongsQuery_Id);
    mapQueryIds.insert("gGetMostRecentlyPlayedSongsQuery" ,eGetMostRecentlyPlayedSongsQuery_Id);
    mapQueryIds.insert("gGetAllVideosCountFromVolumeQuery",eGetAllVideosCountFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllImagesCountFromVolumeQuery",eGetAllImagesCountFromVolumeQuery_Id);
    mapQueryIds.insert("gGetMusicFromCDQuery" ,eGetMusicFromCDQuery_Id);
    mapQueryIds.insert("gGetMusicMetaDataFromCDQuery" ,eGetMusicMetaDataFromCDQuery_Id);
    mapQueryIds.insert("gGetAllSongsByComposerAlbumQuery",eGetAllSongsByComposerAlbumQuery_Id);
    mapQueryIds.insert("gGetAllAlbumByGenreComposerQuery",eGetAllAlbumByGenreComposerQuery_Id);
    mapQueryIds.insert("gGetAllAlbumByComposerQuery",eGetAllAlbumByComposerQuery_Id);
    mapQueryIds.insert("gGetAllComposerByGenreQuery" ,eGetAllComposerByGenreQuery_Id);
    mapQueryIds.insert("gGetImagesBySubFolderQuery",eGetImagesBySubFolderQuery_Id);
    mapQueryIds.insert("gGetVideosBySubFolderQuery" ,eGetVideosBySubFolderQuery_Id);
    mapQueryIds.insert("gGetAllAlbumsWithCoverArtFromVolumeQuery" , eGetAllAlbumsWithCoverArtFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllSongsWithoutMetaInfoFromVolumeQuery",eGetAllSongsWithoutAlbumFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllMusicByKeywordFromVolumeQuery",eGetAllMusicByKeywordFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllAlbumsWithUrlFromVolumeQuery",eGetAllAlbumsWithUrlFromVolumeQuery_Id );
    mapQueryIds.insert("gGetAllSongsCountFromVolumeQuery" ,eGetAllSongsCountFromVolumeQuery_Id);

    mapQueryIds.insert("gGetAllSongsWithTitleCountFromVolumeQuery" ,eGetAllSongsWithTitleCountFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllAlbumsCountFromVolumeQuery" ,eGetAllAlbumsCountFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllMusicArtistsCountFromVolumeQuery" ,eGetAllMusicArtistsCountFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllMusicComposersCountFromVolumeQuery" ,eGetAllMusicComposersCountFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllMusicGenresCountFromVolumeQuery" ,eGetAllMusicGenresCountFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllAlbumsWithCoverArtCountFromVolumeQuery" ,eGetAllAlbumsWithCoverArtCountFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllSongsWithLimitFromVolumeQuery" ,eGetAllSongsWithLimitFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllAlbumsWithLimitFromVolumeQuery" ,eGetAllAlbumsWithLimitFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllMusicArtistsWithLimitFromVolumeQuery" ,eGetAllMusicArtistsWithLimitFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllMusicComposersWithLimitFromVolumeQuery" ,eGetAllMusicComposersWithLimitFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllMusicGenresWithLimitFromVolumeQuery" ,eGetAllMusicGenresWithLimitFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery" , eGetAllAlbumsWithCoverArtWithLimitFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllSongsWithTitleIndexFromVolumeQuery" , eGetAllSongsWithTitleIndexFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllMusicArtistsIndexFromVolumeQuery" , eGetAllMusicArtistsIndexFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllMusicGenresIndexFromVolumeQuery" , eGetAllMusicGenresIndexFromVolumeQuery_Id);
    mapQueryIds.insert("gGetAllAlbumsWithCoverArtIndexFromVolumeQuery" , eGetAllAlbumsWithCoverArtIndexFromVolumeQuery_Id);
    return mapQueryIds;
};

static const QMap<QString, ETrackerAbstarctor_Query_Ids> MAP_QUERY_IDS = GetMapQueryIds();

const QString gGetMultipleAlbumSongsByArtistFromVolumeQuery = " SELECT ?title ?url"
                                                                                                     " WHERE{ "
                                                                                                     "?song a nmm:MusicPiece ; "
                                                                                                     "nie:title ?title ; nie:url ?url ; "
                                                                                                     "nmm:musicAlbum [nie:title ?album] ; "
                                                                                                     "nmm:performer[nmm:artistName ?artist]  "
                                                                                                     ". %1 }ORDER BY ?title";

const int GETMULTIPLEALBUMSONGSBYARTISTFROMVOLUMEQUERY_DATATYPECOUNT = 2;

const QString gGetMultipleGenreSongsFromVolumeQuery = " SELECT  ?title  ?url "
                                                                                         " WHERE { "
                                                                                         " ?m a nmm:MusicPiece; "
                                                                                         " nie:url ?url ; "
                                                                                         " nie:title ?title ; "
                                                                                         " nfo:genre ?genre . "
                                                                                         "%1 } order by ?title";

const int GETMULTIPLEGENRESSONGSFROMVOLUMEQUERY_DATATYPECOUNT = 2;

const QString gGetMultipleArtistSongsByGenreFromVolumeQuery = " SELECT ?title ?url "
                                                                                                     " WHERE { "
                                                                                                     " ?m a nmm:MusicPiece; "
                                                                                                     " nie:title ?title ; "
                                                                                                     " nie:url ?url ; "
                                                                                                     " nmm:performer[nmm:artistName ?artist] ; "
                                                                                                     " nfo:genre ?genre . "
                                                                                                     " %1 } order by ?title";

const int GETMULTIPLEARTISTSONGSBYGENREFROMVOLUMEQUERY_DATATYPECOUNT = 2;

const QString gGetMultipleAlbumSongsByGenreArtistFromVolumeQuery = " SELECT ?title ?url "
                                                                                                             " WHERE { "
                                                                                                             " ?song a nmm:MusicPiece ; "
                                                                                                             " nie:title ?title ; "
                                                                                                             " nie:url ?url ; "
                                                                                                             " nmm:musicAlbum [nie:title ?album] ; "
                                                                                                             " nmm:performer[nmm:artistName ?artist] ; "
                                                                                                             " nfo:genre ?genre . "
                                                                                                             " %1 }ORDER BY ?title";

const int GETMULTIPLEALBUMSONGSBYGENREARTISTFROMVOLUMEQUERY_DATATYPECOUNT =  2;

const QString gGetMultipleFolderSongsQuery = " SELECT ?title ?url "
                                                                      " WHERE { "
                                                                      " ?s a nmm:MusicPiece; "
                                                                      " nie:title ?title; "
                                                                      " nie:url ?url . "
                                                                      " %1 }order by ?title";

const int GETMULTIPLEFOLDERSONGSQUERY_DATATYPECOUNT = 2;

const QString gGetMultipleArtistSongsFromVolumeQuery = " SELECT ?title ?url "
                                                                                         " WHERE{"
                                                                                         " ?music a nmm:MusicPiece ; "
                                                                                         " nie:title ?title ; "
                                                                                         " nie:url ?url ; "
                                                                                         " nmm:performer [nmm:artistName ?performer] ."
                                                                                         " %1 }"
                                                                                         " ORDER BY ?title";

const int GETMULTIPLEARTISTSONGSFROMVOLUMEQUERY_DATATYPECOUNT = 2;

const QString gGetMultipleAlbumSongsFromVolumeQuery = " SELECT ?title ?url "
                                                                                         " WHERE{"
                                                                                         " ?music a nmm:MusicPiece ; "
                                                                                         " nie:title ?title ; "
                                                                                         " nie:url ?url ; "
                                                                                         " nmm:musicAlbum [nie:title ?album] . "
                                                                                         " %1 }"
                                                                                         " ORDER BY ?title";

const int GETMULTIPLEALBUMSONGSFROMVOLUMEQUERY_DATATYPECOUNT = 2;

const QString gGetAllMusicByKeywordForVRQuery = " SELECT ?title ?album ?performer ?genre ?url  "
                                                                     " WHERE { "
                                                                     " ?song a nmm:MusicPiece  ;  "
                                                                     " nie:url ?url  .  "
                                                                     " OPTIONAL { ?song nie:title ?title }  .  "
                                                                     " OPTIONAL { ?song  nmm:performer [nmm:artistName ?performer ] }  .  "
                                                                     " OPTIONAL { ?song  nfo:genre ?genre }  . "
                                                                     " OPTIONAL { ?song  nmm:musicAlbum [nie:title ?album] }  .  "
                                                                     " FILTER( REGEX(?album, \"%1\", \"i\") || "
                                                                     " REGEX(?album, \"%2\", \"i\") ||"
                                                                     " REGEX(?performer, \"%1\", \"i\") || "
                                                                     " REGEX(?performer, \"%2\", \"i\") || "
                                                                     " REGEX(?title, \"%1\", \"i\") || "
                                                                     " REGEX(?title, \"%2\", \"i\") || "
                                                                     " REGEX(?genre, \"%1\", \"i\") || "
                                                                     " REGEX(?genre, \"%2\", \"i\")  ) }"
                                                                     "ORDER BY ?title";

const int GETALLMUSICBYKEYWORDFORVRQUERY_DATATYPECOUNT = 5;

const QString gGetAllSongsFromVolumeQueryForIndex = "SELECT ?url "
                                           "WHERE {"
                                           " ?song a nmm:MusicPiece ; "
                                           " nie:title ?title ; "
                                           " nie:url ?url . "
                                           "%1"
                                           "} ORDER BY ?title ";

//{ added by jaehwan.lee for audio list sorting issue 
const QString gGetAllSongsFromVolumeExtOrderQueryForIndex = "SELECT ?url "
                                           "WHERE {"
                                           " ?song a nmm:MusicPiece ; "
                                           " nie:title ?title ; "
                                           " nie:url ?url . "
                                           "%1"
                                           "} ORDER BY "
                                           "%2 "
                                           "?title ";
//}

const QString gGetAllAlbumsFromVolumeQueryForIndex = "SELECT ?album "
                                           "WHERE {"
                                           " ?song a nmm:MusicPiece ; "
                                           " nmm:musicAlbum [nie:title ?album] ; "
                                           " nie:url ?url . "
                                           "%1"
                                           "} GROUP BY ?album ORDER BY ?album ";

//{ added by jaehwan.lee for audio list sorting issue 
const QString gGetAllAlbumsFromVolumeExtOrderQueryForIndex = "SELECT ?album "
                                           "WHERE {"
                                           " ?song a nmm:MusicPiece ; "
                                           " nmm:musicAlbum [nie:title ?album] ; "
                                           " nie:url ?url . "
                                           "%1"
                                           "} GROUP BY "
                                           "%2 "
                                           "?album "
                                           "ORDER BY "
                                           "%2 "
                                           "?album ";
//}
const QString gGetAllArtistsFromVolumeQueryForIndex = "SELECT ?artist "
                                           "WHERE {"
                                           " ?song a nmm:MusicPiece ; "
                                           " nmm:performer [nmm:artistName ?artist ] ; "
                                           " nie:url ?url . "
                                           "%1"
                                           "} GROUP BY ?artist ORDER BY ?artist ";

//{ added by jaehwan.lee for audio list sorting issue 
const QString gGetAllArtistsFromVolumeExtOrderQueryForIndex = "SELECT ?artist "
                                           "WHERE {"
                                           " ?song a nmm:MusicPiece ; "
                                           " nmm:performer [nmm:artistName ?artist ] ; "
                                           " nie:url ?url . "
                                           "%1"
                                           "} GROUP BY "
                                           "%2 "
                                           "?artist "
                                           "ORDER BY "
                                           " %2 "
                                           "?artist ";

//} 

const QString gGetAllMetaDataWithLimitFromVolumeQuery = " SELECT ?title ?url ?album ?artist ?genre ?comp"
                                                                                            " WHERE { "
                                                                                            " ?song a nmm:MusicPiece ; "
                                                                                            " nie:title ?title ;  "
                                                                                            " nie:url ?url . %1 . OPTIONAL {?song "
                                                                                             " nmm:composer [nmm:artistName ?comp] } . "
                                                                                             " OPTIONAL {?song nfo:genre ?genre} "
                                                                                             " . OPTIONAL { ?song nmm:musicAlbum [nie:title ?album] } "
                                                                                             " . OPTIONAL {?song nmm:performer [nmm:artistName ?artist ] } "
                                                                                            " } ORDER BY %2 LIMIT 1 OFFSET %3";

const int GETALLMETADATAWITHLIMITFROMVOLUMEQUERY_DATATYPECOUNT = 6;

const QString gGetAllMusicByConnectedVolumesQuery = " SELECT ?title ?album ?performer ?genre ?url  "
                                                                     " WHERE { "
                                                                     " ?song a nmm:MusicPiece  ;  "
                                                                     " nie:url ?url  .  "
                                                                     " OPTIONAL { ?song nie:title ?title }  .  "
                                                                     " OPTIONAL { ?song  nmm:performer [nmm:artistName ?performer ] }  .  "
                                                                     " OPTIONAL { ?song  nfo:genre ?genre }  . "
                                                                     " OPTIONAL { ?song  nmm:musicAlbum [nie:title ?album] }  .  "
                                                                     " %1  }"
                                                                     "ORDER BY ?title";

const int GETALLMUSICBYCONNECTEDVOLUMESQUERY_DATATYPECOUNT = 5;


/* ------------------------------------ PhoneBook Data Provider - SQLite ------------------------------------ */
const QString gGetAllSelectedPhoneContactsFromSQLiteDBQuery
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5"
      " FROM PhoneTable "
      " WHERE bd_address=\"%1\" AND db_type=\"%2\" "
      " ORDER BY %3 ";
const int PHONE_BOOK_DATA_TYPE_COUNT = 14;

/////SUPPORT_MIDDLE_NAME1
const QString gMidGetAllSelectedPhoneContactsFromSQLiteDBQuery
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5"
      ",middle_name"
      " FROM PhoneTable "
      " WHERE bd_address=\"%1\" AND db_type=\"%2\" "
      " ORDER BY %3 ";
/////SUPPORT_MIDDLE_NAME

const QString gGetAllSelectedPhoneContactsForForeignFromSQLiteDBQuery
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,idx"
      ",COALESCE(NULLIF((first_name||last_name), \"\"), NULLIF(formatted_name,\"\"), number1) 'contact_name' "
      " FROM PhoneTable "
      " WHERE bd_address=\"%1\" AND db_type=\"%2\" "
      " ORDER BY contact_name COLLATE NOCASE";

/////ORDER BY idx
const QString gIdxGetAllSelectedPhoneContactsForForeignFromSQLiteDBQuery
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,idx"
      ",COALESCE(NULLIF((first_name||last_name), \"\"), NULLIF(formatted_name,\"\"), number1) 'contact_name' "
      " FROM PhoneTable "
      " WHERE bd_address=\"%1\" AND db_type=\"%2\" "
      " ORDER BY idx ";
/////ORDER BY idx

/////SUPPORT_MIDDLE_NAME2
const QString gMidGetAllSelectedPhoneContactsForForeignFromSQLiteDBQuery
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,idx"
      ",COALESCE(NULLIF((first_name||middle_name||last_name), \"\"), NULLIF(formatted_name,\"\"), number1) 'contact_name' "
      ",middle_name"
      " FROM PhoneTable "
      " WHERE bd_address=\"%1\" AND db_type=\"%2\" "
      " ORDER BY contact_name COLLATE NOCASE";
/////SUPPORT_MIDDLE_NAME

const QString gGetAllSelectedPhoneContactsForDomesticFromSQLiteDBQuery
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,idx"
      ",COALESCE(NULLIF((last_name||first_name), \"\"), NULLIF(formatted_name,\"\"), number1) 'contact_name' "
      " FROM PhoneTable "
      " WHERE bd_address=\"%1\" AND db_type=\"%2\" "
      " ORDER BY contact_name COLLATE NOCASE";

/////ORDER BY idx
const QString gIdxGetAllSelectedPhoneContactsForDomesticFromSQLiteDBQuery
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,idx"
      ",COALESCE(NULLIF((last_name||first_name), \"\"), NULLIF(formatted_name,\"\"), number1) 'contact_name' "
      " FROM PhoneTable "
      " WHERE bd_address=\"%1\" AND db_type=\"%2\" "
      " ORDER BY idx ";
/////ORDER BY idx

/////SUPPORT_MIDDLE_NAME3
const QString gMidGetAllSelectedPhoneContactsForDomesticFromSQLiteDBQuery
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,idx"
      ",COALESCE(NULLIF((last_name||middle_name||first_name), \"\"), NULLIF(formatted_name,\"\"), number1) 'contact_name' "
      ",middle_name"
      " FROM PhoneTable "
      " WHERE bd_address=\"%1\" AND db_type=\"%2\" "
      " ORDER BY contact_name COLLATE NOCASE";
/////SUPPORT_MIDDLE_NAME

const QString gPinyinGetAllSelectedPhoneContactsForDomesticFromSQLiteDBQuery
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,idx"
      ",pinyin,middle_name,use_pinyin"
      " FROM PhoneTable "
      " WHERE bd_address=\"%1\" AND db_type=\"%2\" "
      " ORDER BY use_pinyin, pinyin_order, pinyin COLLATE NOCASE";
    /* FEATURE_ORDER_BY_PINYIN */

/////ORDER BY idx
const QString gIdxMidGetAllSelectedPhoneContactsForDomesticFromSQLiteDBQuery
= "SELECT memory_type,first_name,last_name,formatted_name,"
  "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,idx"
  ",COALESCE(NULLIF((last_name||middle_name||first_name), \"\"), NULLIF(formatted_name,\"\"), number1) 'contact_name' "
  ",middle_name"
  " FROM PhoneTable "
  " WHERE bd_address=\"%1\" AND db_type=\"%2\" "
  " ORDER BY idx ";
/////ORDER BY idx

const int PHONE_CONTACTS_DATA_TYPE_COUNT = 16;

const QString gGetAllSelectedCallHistoryFromSQLiteDBQuery
    = "SELECT idx,db_type,memory_type,bd_address,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,time_stamp "
      " FROM PhoneTable "
      " WHERE bd_address=\"%1\" AND db_type=\"%2\" "
      " ORDER BY %3 ";


/////SUPPORT_MIDDLE_NAME4
const QString gMidGetAllSelectedCallHistoryFromSQLiteDBQuery
    = "SELECT idx,db_type,memory_type,bd_address,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,time_stamp "
      ",middle_name"
      " FROM PhoneTable "
      " WHERE bd_address=\"%1\" AND db_type=\"%2\" "
      " ORDER BY %3 ";
/////SUPPORT_MIDDLE_NAME


const QString gGetAllCallHistoryFromSQLiteDBQuery
/////IQS_15MY
#if 0
    = "SELECT idx,db_type,memory_type,bd_address,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,time_stamp "
      " FROM PhoneTable "
      " WHERE ( db_type='1' OR db_type='2' OR db_type='3' ) AND bd_address=\"%1\" "
      " ORDER BY %2 ";
#else
    = "SELECT idx,db_type,memory_type,bd_address,first_name,last_name,formatted_name,"
      " num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,time_stamp "
      " FROM PhoneTable "
      " WHERE bd_address=\"%1\" AND db_type > '0'"
          " ORDER BY CASE"
      " WHEN time_stamp=\"\" AND db_type='1' THEN 3"
      "     WHEN time_stamp=\"\" AND db_type='3' THEN 2"
      " WHEN time_stamp=\"\" AND db_type='2' THEN 1"
          "     ELSE"
      "         time_stamp"
      "     END DESC";
#endif  /* 0 */
/////IQS_15MY

const int CALL_HISTORY_DATA_TYPE_COUNT = 18;


/////SUPPORT_MIDDLE_NAME5
const QString gMidGetAllCallHistoryFromSQLiteDBQuery
    = "SELECT idx,db_type,memory_type,bd_address,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,time_stamp "
      ",middle_name"
      " FROM PhoneTable "
      " WHERE ( db_type='1' OR db_type='2' OR db_type='3' ) AND bd_address=\"%1\" "
      " ORDER BY %2 ";
/////SUPPORT_MIDDLE_NAME


/* ------------------------------------ PhoneBook Data Search - SQLite ------------------------------------ */
const QString gGetContactDetailsByNameFromSQLiteDBQuery
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5 "
      " FROM PhoneTable "
      " WHERE db_type='0' AND bd_address=\"%1\" AND (first_name=\"%2\" OR last_name=\"%2\" OR formatted_name=\"%2\") "
      " ORDER BY first_name ";

const QString gGetAllContactsByKeywordForBDAdressFromSQLiteDBQuery
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5 "
      " FROM PhoneTable "
      " WHERE db_type='0' AND bd_address=\"%1\" AND "
      " ( first_name LIKE \"%2\" OR last_name LIKE \"%2\" OR formatted_name LIKE \"%2\" OR "
      " number1 LIKE \"%2\" OR number2 LIKE \"%2\" OR "
      " number3 LIKE \"%2\" OR number4 LIKE \"%2\" OR "
      " number5 LIKE \"%2\" ) "
      " ORDER BY first_name ";

const QString gGetContactByNumberForForeignFromSQLiteDBQuery 
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5"
      ",COALESCE(NULLIF((first_name||last_name), \"\"), NULLIF(formatted_name,\"\"), number1) 'contact_name' "
      " FROM PhoneTable "
      " WHERE db_type='0' AND bd_address=\"%1\" AND ( number1=\"%2\" OR number2=\"%2\" OR number3=\"%2\" OR number4=\"%2\" OR number5=\"%2\" ) "
      " ORDER BY contact_name COLLATE NOCASE";

const QString gGetContactByNumberForDomesticFromSQLiteDBQuery
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,COALESCE(NULLIF((last_name||first_name), \"\"), NULLIF(formatted_name,\"\"), number1) 'contact_name' "
      " FROM PhoneTable "
      " WHERE db_type='0' AND bd_address=\"%1\" AND ( number1=\"%2\" OR number2=\"%2\" OR number3=\"%2\" OR number4=\"%2\" OR number5=\"%2\" ) "
      " ORDER BY contact_name COLLATE NOCASE";

/////SUPPORT_MIDDLE_NAME6
const QString gMidGetContactByNumberForDomesticFromSQLiteDBQuery
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,idx"
      ",COALESCE(NULLIF((last_name||middle_name||first_name), \"\"), NULLIF(formatted_name,\"\"), number1) 'contact_name' "
      ",middle_name"
      " FROM PhoneTable "
      " WHERE db_type='0' AND bd_address=\"%1\" AND ( number1=\"%2\" OR number2=\"%2\" OR number3=\"%2\" OR number4=\"%2\" OR number5=\"%2\" ) "
      " ORDER BY pinyin, use_pinyin, contact_name COLLATE NOCASE";
    /* FEATURE_ORDER_BY_PINYIN */

const QString gGetContactByNumberFromSQLiteDBQuery_KR
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5"
      ",COALESCE(NULLIF((last_name||first_name), \"\"), NULLIF(formatted_name,\"\"), number1) 'contact_name' "
      " FROM PhoneTable "
      " WHERE db_type='0' AND bd_address=\"%1\" AND ( substr(number1, \"%3\")=\"%2\" OR substr(number2, \"%3\")=\"%2\" OR substr(number3, \"%3\")=\"%2\" OR substr(number4, \"%3\")=\"%2\" OR substr(number5, \"%3\")=\"%2\" ) "
      " ORDER BY contact_name COLLATE NOCASE";

const QString gGetContactByNumberFromSQLiteDBQuery_CN
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5"
      ",COALESCE(NULLIF((first_name||middle_name||last_name), \"\"), NULLIF(formatted_name,\"\"), number1) 'contact_name' "
      ",middle_name"
      " FROM PhoneTable "
      " WHERE db_type='0' AND bd_address=\"%1\" AND ( substr(number1, \"%3\")=\"%2\" OR substr(number2, \"%3\")=\"%2\" OR substr(number3, \"%3\")=\"%2\" OR substr(number4, \"%3\")=\"%2\" OR substr(number5, \"%3\")=\"%2\" ) "
      " ORDER BY contact_name COLLATE NOCASE";

const QString gGetFavoriteContactByNameAndNumberFromSQLiteDBQuery
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5 "
      " FROM PhoneTable "
      " WHERE db_type='6' AND bd_address=\"%1\" AND first_name=\"%2\" AND last_name=\"%3\" AND ( number1=\"%4\" OR number2=\"%4\" OR number3=\"%4\" OR number4=\"%4\" OR number5=\"%4\" ) "
      " ORDER BY first_name ";
const int PHONE_BOOK_SEARCH_RESULT_DATA_TYPE = 14;

/////SUPPORT_MIDDLE_NAME7
const QString gMidGetFavoriteContactByNameAndNumberFromSQLiteDBQuery
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5 "
      "middle_name"
      " FROM PhoneTable "
      " WHERE db_type='6' AND bd_address=\"%1\" AND first_name=\"%2\" AND last_name=\"%3\" AND middle_name=\"%4\" AND ( number1=\"%5\" OR number2=\"%5\" OR number3=\"%5\" OR number4=\"%5\" OR number5=\"%5\" ) "
      " ORDER BY first_name ";
/////SUPPORT_MIDDLE_NAME

const QString gGetContactByNumberForForeignCallHistoryNameFromSQLiteDBQuery
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,idx"
      ",COALESCE(NULLIF((first_name||last_name), \"\"), NULLIF(formatted_name,\"\"), number1) 'contact_name' "
      " FROM PhoneTable "
      " WHERE db_type='0' AND bd_address=\"%1\" AND ( number1=\"%2\" OR number2=\"%2\" OR number3=\"%2\" OR number4=\"%2\" OR number5=\"%2\" ) "
      " ORDER BY contact_name COLLATE NOCASE";


/////SUPPORT_MIDDLE_NAME8
const QString gMidGetContactByNumberForForeignCallHistoryNameFromSQLiteDBQuery
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,idx"
      ",COALESCE(NULLIF((first_name||middle_name||last_name), \"\"), NULLIF(formatted_name,\"\"), number1) 'contact_name' "
      ",middle_name"
      " FROM PhoneTable "
      " WHERE db_type='0' AND bd_address=\"%1\" AND ( number1=\"%2\" OR number2=\"%2\" OR number3=\"%2\" OR number4=\"%2\" OR number5=\"%2\" ) "
      " ORDER BY contact_name COLLATE NOCASE";
/////SUPPORT_MIDDLE_NAME


const QString gGetContactByNumberForDomesticCallHistoryNameFromSQLiteDBQuery
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,idx"
      ",COALESCE(NULLIF((last_name||first_name), \"\"), NULLIF(formatted_name,\"\"), number1) 'contact_name' "
      " FROM PhoneTable "
      " WHERE db_type='0' AND bd_address=\"%1\" AND ( substr(number1, \"%3\")=\"%2\" OR substr(number2, \"%3\")=\"%2\" OR substr(number3, \"%3\")=\"%2\" OR substr(number4, \"%3\")=\"%2\" OR substr(number5, \"%3\")=\"%2\" ) "
      " ORDER BY contact_name COLLATE NOCASE";

/////SUPPORT_MIDDLE_NAME9
const QString gMidGetContactByNumberForDomesticCallHistoryNameFromSQLiteDBQuery
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,idx"
      ",COALESCE(NULLIF((last_name||middle_name||first_name), \"\"), NULLIF(formatted_name,\"\"), number1) 'contact_name' "
      ",middle_name"
      " FROM PhoneTable "
      " WHERE db_type='0' AND bd_address=\"%1\" AND ( substr(number1, \"%3\")=\"%2\" OR substr(number2, \"%3\")=\"%2\" OR substr(number3, \"%3\")=\"%2\" OR substr(number4, \"%3\")=\"%2\" OR substr(number5, \"%3\")=\"%2\" ) "
      " ORDER BY pinyin, use_pinyin, contact_name COLLATE NOCASE";
    /* FEATURE_ORDER_BY_PINYIN */

const QString gGetContactByNumberForCallHistoryNameFromSQLiteDBQuery_KR
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,idx"
      ",COALESCE(NULLIF((last_name||first_name), \"\"), NULLIF(formatted_name,\"\"), number1) 'contact_name' "
      " FROM PhoneTable "
      " WHERE db_type='0' AND bd_address=\"%1\" AND ( substr(number1, \"%3\")=\"%2\" OR substr(number2, \"%3\")=\"%2\" OR substr(number3, \"%3\")=\"%2\" OR substr(number4, \"%3\")=\"%2\" OR substr(number5, \"%3\")=\"%2\" ) "
      " ORDER BY contact_name COLLATE NOCASE";

const QString gGetContactByNumberForCallHistoryNameFromSQLiteDBQuery_CN
    = "SELECT memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,idx"
      ",COALESCE(NULLIF((first_name||middle_name||last_name), \"\"), NULLIF(formatted_name,\"\"), number1) 'contact_name' "
      ",middle_name"
      " FROM PhoneTable "
      " WHERE db_type='0' AND bd_address=\"%1\" AND ( substr(number1, \"%3\")=\"%2\" OR substr(number2, \"%3\")=\"%2\" OR substr(number3, \"%3\")=\"%2\" OR substr(number4, \"%3\")=\"%2\" OR substr(number5, \"%3\")=\"%2\" ) "
      " ORDER BY contact_name COLLATE NOCASE";

const QString gGetContactByCallHistoryInfoFromSQLiteDBQuery
    = "SELECT idx,memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5 "
      " FROM PhoneTable "
      " WHERE db_type='0' AND bd_address=\"%1\" AND first_name=\"%2\" AND last_name=\"%3\" AND ( number1=\"%4\" OR number2=\"%4\" OR number3=\"%4\" OR number4=\"%4\" OR number5=\"%4\" ) "
      " ORDER BY first_name ";

/////SUPPORT_MIDDLE_NAME10
const QString gMidGetContactByCallHistoryInfoFromSQLiteDBQuery
    = "SELECT idx,memory_type,first_name,last_name,formatted_name,"
      "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5 "
      ", middle_name"
      " FROM PhoneTable "
      " WHERE db_type='0' AND bd_address=\"%1\" AND first_name=\"%2\" AND last_name=\"%3\" AND middle_name=\"%4\" AND ( number1=\"%5\" OR number2=\"%5\" OR number3=\"%5\" OR number4=\"%5\" OR number5=\"%5\" ) "
      " ORDER BY first_name ";
/////SUPPORT_MIDDLE_NAME

const QString gGetContactByCallHistoryInfoFromSQLiteDBQuery_KR_NA
    =  "SELECT idx,memory_type,first_name,last_name,formatted_name,"
       "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5 "
       " FROM PhoneTable "
       " WHERE db_type='0' AND bd_address=\"%1\" AND first_name=\"%2\" AND last_name=\"%3\" AND ( substr(number1, \"%5\")=\"%4\" OR substr(number2, \"%5\")=\"%4\" OR substr(number3, \"%5\")=\"%4\" OR substr(number4, \"%5\")=\"%4\" OR substr(number5, \"%5\")=\"%4\" ) "
       " ORDER BY first_name ";
const int CALL_HISTORY_SEARCH_RESULT_DATA_TYPE = 15;

const QString gGetPhoneBookHashValueByBDAddressFromSQLiteDBQuery =  "SELECT crypto_hash "
        " FROM PhoneTable "
        " WHERE db_type='0' AND bd_address=\"%1\" ";
const int PHONE_BOOK_CRYPTO_HASH_RESULT_DATA_TYPE = 1;

/* ------------------------------------ PhoneBook Data Count - SQLite ------------------------------------ */
const QString gGetAllSelectedPhoneContactsCountFromSQLiteDBQuery = " SELECT COUNT(*) "
                                                           " FROM PhoneTable "
                                                           " WHERE  bd_address=\"%1\" AND db_type=\"%2\" ";

const QString gGetAllCallHistoryCountFromSQLiteDBQuery = " SELECT COUNT(*) "
															" FROM PhoneTable "
                                                     		" WHERE (db_type='1' OR db_type='2' OR db_type='3') AND bd_address=\"%1\" ";
const int PHONE_BOOK_COUNT_DATA_TYPE = 1;

/* ------------------------------------ PhoneBook Data Updater - SQLite ------------------------------------ */
const QString gInsertContactIntoDBFromSQLiteDBQuery = " INSERT INTO PhoneTable "
	"(idx,db_type,memory_type,bd_address,first_name,crypto_hash,last_name,formatted_name,"
    "num_type1,number1,num_type2,number2,num_type3,number3,num_type4,number4,num_type5,number5,time_stamp,middle_name,"
    "pinyin,use_pinyin,pinyin_order)";
    /* FEATURE_ORDER_BY_PINYIN */

const QString gRemoveAllPhoneBookFromSQLiteDBQuery = " DELETE FROM PhoneTable ";

const QString gRemoveAllDataByBDAddressFromSQLiteDBQuery = " DELETE FROM PhoneTable "
	" WHERE bd_address=\"%1\" ";

const QString gRemoveSelectedDataOfBDAddressFromSQLiteDBQuery = " DELETE FROM PhoneTable "
	" WHERE bd_address=\"%1\" AND db_type=\"%2\" ";

const QString gRemoveAllCallHistoryOfBDAddressFromSQLiteDBQuery = " DELETE FROM PhoneTable "
	" WHERE (db_type='1' OR db_type='2' OR db_type='3') AND bd_address=\"%1\"";

const QString gRemoveContactDataByNameFromSQLiteDBQuery = " DELETE FROM PhoneTable "
	" WHERE db_type='0' AND bd_address=\"%1\" AND first_name=\"%2\" AND memory_type=\"%3\" ";

const QString gRemoveContactDataByIndexFromSQLiteDBQuery = " DELETE FROM PhoneTable "
	" WHERE db_type='0' AND bd_address=\"%1\" AND idx=\"%2\" AND memory_type=\"%3\" ";

#endif // DHAVN_TRACKERABSTRACTOR_CONSTANTS_H
