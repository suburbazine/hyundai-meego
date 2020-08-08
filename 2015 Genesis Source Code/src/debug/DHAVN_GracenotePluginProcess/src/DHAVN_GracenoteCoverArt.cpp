/*
 ******************************************************************
 *    COPYRIGHT ?Teleca AB      *
 *----------------------------------------------------------------*
 * MODULE     	:  CGracenoteCoverArt                             	  *
 *
 * PROGRAMMERS 	:  Vishal Jain
 * DATE       	:  17 April 2012                             	  *
 * VERSION    	:                                             	  *
 *
 *----------------------------------------------------------------*
 *                                                                *
 *  MODULE SUMMARY : CGracenoteCoverArt supports feature for
       retrieving cover art from Gracenote ELDB
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                     *
 *
 *     DATE         PROGRAMMER               COMMENT
 ******************************************************************
 * History
 * 17-April-2011      Vishal Jain               Create new class and API's
 ******************************************************************
 */

#include "DHAVN_GracenoteCoverArt.h"

#include <QImage>
#include <QPixmap>
#include <MediaArtInfo>
#include <QDebug>

#include <gn_coverart.h>
#include <gnepal_string.h>
#include <gnepal_memory.h>
#include <gn_error_display.h>
#include <gn_abs_errors.h>
#include <QTextCodec>


/*
 *******************************************************************
 * Function	: CGracenoteCoverArt::CGracenoteCoverArt
 *
 * Description 	: Constructor for CGracenoteCoverArt

 * Parameters 	: parent (IN) - pointer to parent object

 * Returns 	: None
 *
 *******************************************************************
 */
CGracenoteCoverArt::CGracenoteCoverArt(QObject *parent)
{
    Q_UNUSED(parent);
    LOG_INIT_V2("Media_GracenotePluginProcess");

    m_Album.clear();
    m_Artist.clear();
}

/*
 *******************************************************************
 * Function	: CGracenoteCoverArt::~CGracenoteCoverArt
 *
 * Description 	: Destructor for CGracenoteCoverArt

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
CGracenoteCoverArt::~CGracenoteCoverArt()
{
    //No implementation required
}

/*
 *******************************************************************
 * Function	: CGracenoteCoverArt::RetrieveCoverArtForMP
 *
 * Description 	: Retrieve cover art with the meta data sent by Music
                          player by doing text id lookup

 * Parameters 	: metaData - List of meta data

 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteCoverArt::RetrieveCoverArtForMP(QStringList metaData)
{
    LOG_TRACE <<("CGracenoteCoverArt::RetrieveCoverArtForMP Enter") << LOG_ENDL;

    bool result = false;

    if(metaData.count() != 4)
    {
        emit coverArtRetrieved(false);
        return;
    }

    result = CheckIfCoverArtExists(metaData);
    // Cover art signal is already emitted, so return
    if(result)
        return;

    LOG_TRACE << "Going to ret from Gracenote" << LOG_ENDL;

    gn_pfile_data_t fileData;
    gn_error_t nError = gn_textid_file_data_init(&fileData);
    if(nError != GN_SUCCESS)
    {
        emit coverArtRetrieved(false);
        return;
    }

    // Set the ID3 information to file data structure
    result = SetID3Information(metaData, fileData);
    if(!result)
    {
        //Emit error signal
        emit coverArtRetrieved(false);
        return;
    }

    PerformLookup(fileData);

    // Free memory
    gn_textid_file_data_smart_free( &fileData );

    // Text id lookup complete
    gn_textid_lookup_end();
    gn_textid_cleanup_lookup();

    LOG_TRACE <<("CGracenoteCoverArt::RetrieveCoverArtForMP End") << LOG_ENDL;
}

/*
 *******************************************************************
 * Function	: CGracenoteCoverArt::SetID3Information
 *
 * Description 	: Set the ID3 information to file data structure

 * Parameters 	: id3List - List of meta data
                          fileData -  file data pointer

 * Returns 	: None
 *
 *******************************************************************
 */

bool CGracenoteCoverArt::SetID3Information(QStringList id3List, gn_pfile_data_t &fileData)
{

    LOG_TRACE <<("CGracenoteCoverArt::SetID3Information Start");

    if(id3List.count() != 4)
        return false;

    if((id3List.at(0).length() != 0))
    {
        //Title
        gn_uchar_t* title = gnepal_strdup((gn_uchar_t *) (id3List.at(0).toUtf8().constData()));
        LOG_TRACE <<("CGracenoteCoverArt::SetID3Information: title");
        LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(title)));
        gn_textid_file_data_set_track_title(fileData, title);
        if(title != NULL) gnepal_mem_free(title); 
        title = NULL;
    }

    // variables to be used for storing album art
    m_Album.clear();
    m_Artist.clear();

    m_Artist = id3List.at(1);

    if(m_Artist.length() != 0)
    {
        //Track arist
        gn_uchar_t* trackartist = gnepal_strdup((gn_uchar_t *) (m_Artist.toUtf8().constData()));

        QString result = encodeString(trackartist);
        LOG_TRACE <<("CGracenoteCoverArt::SetID3Information: trackartist");
        gn_uchar_t* utf8artist = gnepal_strdup((gn_uchar_t *)(result.toUtf8().constData()));
        LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(utf8artist))) << LOG_ENDL;
        gn_textid_file_data_set_track_artist(fileData, utf8artist);
        if(trackartist != NULL) gnepal_mem_free(trackartist); trackartist = NULL;
        if(utf8artist != NULL) gnepal_mem_free(utf8artist); utf8artist = NULL;
    }

    m_Album = id3List.at(2);



    if(m_Album.length() != 0)
    {
        //Album title
        gn_uchar_t* album = gnepal_strdup((gn_uchar_t *) (m_Album.toUtf8().constData()));

        QString result = encodeString(album);
        LOG_TRACE <<("CGracenoteCoverArt::SetID3Information: album");
        gn_uchar_t* utf8album = gnepal_strdup((gn_uchar_t *)(result.toUtf8().constData()));
        LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(utf8album))) << LOG_ENDL;
        gn_textid_file_data_set_disc_title(fileData, utf8album);
        if(album != NULL) gnepal_mem_free(album); album = NULL;
        if(utf8album != NULL) gnepal_mem_free(utf8album); utf8album = NULL;
    }

    if((id3List.at(3)).length() != 0)
    {
        //genre
        gn_uchar_t* genre= gnepal_strdup((gn_uchar_t *) (id3List.at(3).toUtf8().constData()));
        LOG_TRACE <<("CGracenoteCoverArt::SetID3Information: genre");
        LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(genre)));
        gn_textid_file_data_set_genre(fileData, genre);
        if(genre != NULL) gnepal_mem_free(genre); genre = NULL;
    }

    LOG_TRACE <<("CGracenoteCoverArt::SetID3Information End");

    return true;

}

/*
 *******************************************************************
 * Function	: CGracenoteCoverArt::PerformLookup
 *
 * Description 	: Perform text id lookup

 * Parameters 	: fileData -  file data pointer

 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteCoverArt::PerformLookup(gn_pfile_data_t &fileData)
{
    LOG_TRACE <<("CGracenoteCoverArt::PerformLookup Enter") << LOG_ENDL;

    gn_error_t nError = gn_textid_lookup_begin();

    LOG_TRACE <<("CGracenoteCoverArt : PerformLookup :: gn_textid_lookup_begin Error:");
    LogError(nError);

    if(nError != GN_SUCCESS)
    {
        emit coverArtRetrieved(false);
        return;
    }

    gn_textid_lu_flags_t lu_flag = GN_TEXTID_LU_FLAG_DFLT;
    gn_textid_match_source_t matchsource = GN_TEXTID_MATCH_SOURCE_NONE;

    gn_uint32_t nmatchcount = 0;
    gn_textid_match_type_t matchtype = GN_TEXTID_MATCH_TYPE_NOT_AVAILABLE;

    nError = gn_textid_local_lookup(fileData, lu_flag, &matchsource, &matchtype, &nmatchcount);

    LOG_TRACE <<("CGracenoteCoverArt : PerformLookup :: gn_textid_local_lookup Error:");
    LogError(nError);

    LOG_TRACE << "matchsource = " << matchsource << LOG_ENDL;
    LOG_TRACE << "nmatchcount = " << nmatchcount << LOG_ENDL;
    LOG_TRACE << "matchtype = " << matchtype << LOG_ENDL;


    if(nError == GN_SUCCESS)
    {
        if(nmatchcount <= 0 && fileData != GN_NULL)
        {
            LOG_TRACE <<("CGracenoteCoverArt : PerformLookup :: match count is < 0") << LOG_ENDL;
            emit coverArtRetrieved(false);
            return;
        }
        else
        {
            LOG_TRACE <<("CGracenoteCoverArt : PerformLookup :: Else condition and match count > 0") << LOG_ENDL;
            //Get the result
            gn_textid_presult_data_t displayResult;

            for(gn_uint32_t i = 1; i <= nmatchcount; i++)
            {

                nError = gn_textid_get_result( i, &displayResult );
                if(nError != GN_SUCCESS)
                {
                    LOG_TRACE << "gn_textid_get_result Error" << LOG_ENDL;
                    emit coverArtRetrieved(false);
                    return;
                }

                gn_uint32_t confidence = 0;
                if(matchsource == GN_TEXTID_MATCH_SOURCE_ALBUM)
                {
                    nError =  gn_textid_get_album_artist_match_confidence(fileData,displayResult,&confidence);

                    if(nError == GN_SUCCESS )
                    {
                        if(confidence < 66)
                        {
                            LOG_TRACE <<("CGracenoteCoverArt ::Album artist match confidence is ") << LOG_ENDL;
                            LOG_INFO <<(confidence) << LOG_ENDL;

                            // Free the result structure
                            gn_textid_free_result(&displayResult);

                            emit coverArtRetrieved(false);
                            return;
                        }
                    }
                }

                if(displayResult != GN_NULL)
                {
                    LOG_TRACE << "displayResult not null" << LOG_ENDL;
                    //Retrieving CoverArt
                    RetrieveCoverArt(displayResult);
                    break;
                }
                else
                {
                    LOG_TRACE << "displayResult NULL" << LOG_ENDL;
                }
            }
            // Free the look up result
            gn_textid_free_result(&displayResult);
        }
    }

}

/*
 *******************************************************************
 * Function	: CGracenoteCoverArt::RetrieveCoverArt
 *
 * Description 	: Perform text id lookup

 * Parameters 	: fileData -  file data pointer

 * Returns 	: None
 *
 *******************************************************************
 */

void CGracenoteCoverArt::RetrieveCoverArt(gn_textid_presult_data_t resultData)
{

    LOG_TRACE <<("CGracenoteCoverArt::RetrieveCoverArt start");

    if (resultData == GN_NULL)
    {
        emit coverArtRetrieved(false);
        return;
    }

    gn_coverart_t coverart = NULL;
    gn_error_t nError = gn_coverart_prepare_from_textid_lookup(resultData, &coverart);

    LOG_TRACE <<("CGracenoteCoverArt::RetrieveCoverArt :: gn_coverart_prepare_from_textid_lookup error:");
    LogError(nError);

    QImage image;

    if( nError == GN_SUCCESS )
    {
        nError = GN_FAILURE;
        gn_bool_t coverart_available=GN_FALSE;
        gn_uchar_t* imageBuf = NULL;
        gn_uint32_t image_size;

        //Retrieve album art
        nError = gn_coverart_retrieve_local_album_art(coverart, &coverart_available);

        LOG_TRACE <<("CGracenoteCoverArt::RetrieveCoverArt :: gn_coverart_retrieve_local_album_art error:") << LOG_ENDL;
        LogError(nError);

        //if album art not available, try to retrieve artist image
        if( !coverart_available || coverart == NULL)
        {
            //Retrieve artist image
            nError = gn_coverart_retrieve_local_artist_image(coverart, &coverart_available);

            LOG_TRACE <<("CGracenoteCoverArt::RetrieveCoverArt :: gn_coverart_retrieve_local_artist_image error:");
            LogError(nError);
        }

        //if album art not available, try to retrieve genre art
        if( !coverart_available || coverart == NULL)
        {
            //Retrieve Genre Art
            nError = gn_coverart_retrieve_local_genre_art(coverart, &coverart_available);

            LOG_TRACE <<("CGracenoteCoverArt::RetrieveCoverArt :: gn_coverart_retrieve_local_genre_art error:");
            LogError(nError);
        }

        if(nError == GN_SUCCESS && coverart_available && coverart != NULL)
        {
            //Get image size
            nError = gn_coverart_get_image_size(coverart, &image_size);
            if(nError != GN_SUCCESS || image_size == 0)
            {
                emit coverArtRetrieved(false);
                return;
            }

            //Get image buffer
            nError = gn_coverart_get_image(coverart, &imageBuf);

            LOG_TRACE <<("CGracenoteCoverArt::RetrieveCoverArt :: gn_coverart_get_image error:") << LOG_ENDL;
            LogError(nError);

            if( nError != GN_SUCCESS || imageBuf == NULL)
            {
                emit coverArtRetrieved(false);
                return;
            }

            image = QImage::fromData(reinterpret_cast<const uchar*>(imageBuf), image_size);
        }
        else
        {
            LOG_TRACE <<("CGracenoteCoverArt::RetrieveCoverArt :: Not available from Gracenote");
        }
    }

    // Free the cover art structure
    gn_coverart_smart_free(&coverart);

    // Insert cover art
    InsertCoverArt(image);

    LOG_TRACE <<("CGracenoteCoverArt::RetrieveCoverArt End");
}

/*
 *******************************************************************
 * Function	: CGracenoteInterface::InsertCoverArt
 *
 * Description 	: Inserts Gracenote ELDB coverart into Tracker Store
 *
 * Parameters 	: trackerMusicFiledata(TrackerMusicData) - Structure containing
 *                       music metadata from Gracenote ELDB
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteCoverArt::InsertCoverArt(QImage &qimage)
{

    LOG_TRACE <<("CGracenoteCoverArt::InsertCoverArt Start") << LOG_ENDL;

    QSize size = qimage.size();

    if(size.height() == 0 || size.width() == 0)
    {
        emit coverArtRetrieved(false);
        return;
    }

    MediaArt::Info albumArt;

    albumArt = MediaArt::Album(m_Artist, m_Album);

    QString fileName = albumArt.potentialPath().toString();
	 //deleted by aettie 20131010 unused code
//{added by aettie.ji 20130904 for gracenote logo
    //QString gnstr = m_Artist;
    //gnstr.insert(0, QString("gn_"));

    //MediaArt::Info gn_albumArt;
    //gn_albumArt = MediaArt::Album(gnstr, m_Album);
    //QString gN_fileName = gn_albumArt.potentialPath().toString();

    //gN_fileName.remove("file://");
    fileName.remove("file://");
    
    LOG_TRACE <<"TestInsertCoverArt stripped : " <<fileName<< LOG_ENDL;

    if(albumArt.exists())
    {
        LOG_TRACE <<("CGracenoteCoverArt::InsertCoverArt Media art already present");
    }
    else
    {
        LOG_TRACE << ("Cache the file:") << fileName << LOG_ENDL;
        SaveImageToCache(qimage, fileName);
        //LOG_TRACE << ("Cache the file w/ gn_ prefix:") << gN_fileName << LOG_ENDL;
        //SaveImageToCache(qimage, gN_fileName); 
    }
//}added by aettie.ji 20130904 for gracenote logo

    emit coverArtRetrieved(true, albumArt.potentialPath().toString());

    LOG_TRACE <<("CGracenoteCoverArt::InsertCoverArt End") << LOG_ENDL;
}


/*
 *******************************************************************
 * Function	: CGracenoteInterface::SaveImageToCache
 *
 * Description 	: saves the image to cache
 *
 * Parameters 	: source - actual QImage
 *                fileName - Random file name in which the image gets copied
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteCoverArt::SaveImageToCache( const QImage& source, QString fileName)
{
    LOG_TRACE <<("CGracenoteCoverArt::SaveImageToCache Start") << LOG_ENDL;

    //save the image into the new file (in jpeg file format)
    if(!source.save(fileName, "JPEG")) {
        LOG_TRACE <<"*********saveimage4********"<< LOG_ENDL;
            return;
    }

    LOG_TRACE <<("CGracenoteCoverArt::SaveImageToCache End") << LOG_ENDL;
}

/*
 *******************************************************************
 * Function	: CGracenoteCoverArt::LogError
 *
 * Description 	: Log error
 *
 * Parameters 	: error - Error code
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteCoverArt::LogError(gn_error_t error)
{
    const gn_uchar_t* errorStr = gnerr_get_code_desc( error);

    QString txt;
    txt.append (QString("Error: %1").arg(reinterpret_cast<const char*>(errorStr)));

    LOG_TRACE << txt << LOG_ENDL;
}
/*
 *******************************************************************
 * Function	: CGracenoteCoverArt::RetrieveMultipleCoverArts
 *
 * Description 	: Retrieve cover arts for multiple songs
 *
 * Parameters 	: metaDataList - list of various metadata
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteCoverArt::RetrieveMultipleCoverArts()
{
    LOG_TRACE <<("CGracenoteCoverArt::RetrieveMultipleCoverArts Start") << LOG_ENDL;
    LOG_INFO << "RetrieveMultipleCoverArts Album and Artist Count" << m_AlbumList.count() << m_ArtistList.count() << LOG_ENDL;

    //qDebug() << "RetrieveMultipleCoverArts: ALBUM LIST COUNT" << m_AlbumList.count();
    //qDebug() << "RetrieveMultipleCoverArts: Artist LIST COUNT" << m_ArtistList.count();

    QStringList metaData;
    for(int index = 0; index < m_AlbumList.count(); index++)
    {

        metaData.clear();
        metaData.append("");
        metaData.append(m_ArtistList.at(index));
        metaData.append(m_AlbumList.at(index));
        metaData.append("");

        RetrieveCoverArtForMP(metaData);
    }

    LOG_TRACE <<("CGracenoteCoverArt::RetrieveMultipleCoverArts End") << LOG_ENDL;
}

bool CGracenoteCoverArt::CheckIfCoverArtExists(QStringList list)
{

    MediaArt::Info albumArt;

    albumArt = MediaArt::Album(list.at(1), list.at(2));

    QString fileName = albumArt.potentialPath().toString();

    if(albumArt.exists())
    {
        LOG_TRACE << "Already exists in caches" << LOG_ENDL;
        emit coverArtRetrieved(true, fileName);
        return true;
    }
    else
        return false;
}

/*
 *******************************************************************
 * Function	: CGracenoteCoverArt::encodeString
 *
 * Description 	: This functions checks if the string is in UTF-8 format.
     If not in UTF-8, then encodes it in UTF-8 format.

 * Parameters 	: inputString - to be converted to UTF-8

 * Returns 	: QString - String encoded in UTF-8 format
 *
 *******************************************************************
 */
QString CGracenoteCoverArt::encodeString(gn_uchar_t* inputString)
{

    QString inputQStr = QString::fromUtf8(reinterpret_cast<const char*>(inputString));

    if ( isUtf8( inputQStr.toAscii() ) )
    {
        return inputQStr;
    }

    QTextCodec *codec = QTextCodec::codecForName("EUC-KR");
    QString result;

    LOG_TRACE <<("CGracenoteOperationUtil:encodeString inputString = ")<< LOG_ENDL;
    LOG_INFO <<(inputQStr) << LOG_ENDL;

    QTextStream in(inputQStr.toAscii());

    in.setAutoDetectUnicode(false);
    in.setCodec(codec);

    result = in.readAll();

    LOG_TRACE <<("CGracenoteOperationUtil:encodeString output String = ") << LOG_ENDL;
    LOG_INFO <<(result) << LOG_ENDL;
    return result;
}


/*
 *******************************************************************
 * Function	: CGracenoteCoverArt::isUtf8
 *
 * Description 	: This functions checks if the string is in UTF-8 format.

 * Parameters 	: str - string pointer

 * Returns 	: bool - True if encoded in UTF-8 format
 *			  False if not in UTF-8 format
 *
 *******************************************************************
 */

bool CGracenoteCoverArt::isUtf8(const char * str )
{
    int str_bytes = 0;
    if( str == NULL )
        return false;

    str_bytes = strlen(str);
    bool isUtf8(true);
    int index = 0;

    while( index < str_bytes && isUtf8 )
    {
        char achar = str[index];
        int supplemental_bytes = 0;
        if( (achar & 0x80) == 0 )
        {
            // 0xxxxxxx
            ++index;
        }
        else if( (achar &0xF8) == 0xF0 )
        {
            // 11110uuu 10uuzzzz 10yyyyyy 10xxxxxx
            ++index;
            supplemental_bytes = 3;
        }
        else if( (achar &0xE0) == 0xE0 )
        {
            // 1110zzzz 10yyyyyy 10xxxxxx
            ++index;
            supplemental_bytes = 2;
        }
        else if( (achar &0xE0) == 0xC0 )
        {
            // 10yyyyyy 10xxxxxx
            ++index;
            supplemental_bytes = 1;
        }
        else
        {
            isUtf8 = false;
        }

        while( isUtf8 && supplemental_bytes-- )
        {
            if( index >= str_bytes )
            {
                isUtf8 = false;
            }
            else if( (str[index++] & 0xC0) != 0x80 )
            {
                // 10uuzzzz
                isUtf8 = false;
            }
        }
    }

    return isUtf8;
}


/*
 *******************************************************************
 * Function	: CGracenoteCoverArt::SetAlbumArtistForCA
 *
 * Description 	: Set the album and artist lists
 *
 * Parameters 	: albumlist and artistlist
 *
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteCoverArt::SetAlbumArtistForCA(QStringList albumList, QStringList artistList)
{
    m_AlbumList.clear();
    m_ArtistList.clear();
    m_AlbumList = albumList;
    m_ArtistList = artistList;
}

/*
 *******************************************************************
 * Function	: CGracenoteCoverArt::SetMetadataForCA / GetMetadataForCA
 *
 * Description 	: Set/Get metadata list 
 *
 * Parameters 	: metadate for getting single coverart
 *
 * Returns 	: None
 *
 *******************************************************************
 */
 //added by aettie 2013.03.19 for gracenote issue
void CGracenoteCoverArt::SetMetadataForCA(QStringList mataDataList)
{
    m_Metadata.clear();
    m_Metadata = mataDataList;
}

QStringList CGracenoteCoverArt::GetMetadataForCA()
{
    return m_Metadata;
}
/*
 *******************************************************************
 * Function	: CGracenoteCoverArt::GetAlbumListCount
 *
 * Description 	: Returns the album list count
 *
 * Parameters 	: None
 *
 * Returns 	: Album list count
 *
 *******************************************************************
 */
int CGracenoteCoverArt::GetAlbumListCount()
{
    return m_AlbumList.count();
}
//End of file
