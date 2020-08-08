
/*
 ******************************************************************
 */

#include "DHAVN_GracenoteOperationUtil.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QVariant>
#include <QSqlQuery>
#include <gn_id3_reader.h>
#include <gn_id3_util.h>
#include <gnepal_string.h>
#include <gn_abs_errors.h>
#include <gn_error_codes.h>
#include <gn_error_values.h>
#include <gnepal_memory.h>
#include <QTextCodec>
#include <QDir>
//deleted by aettie.ji 20130904
#include <gn_error_display.h>
#include <gnex_error_codes.h>
#include <gn_transcript_accessors.h>
#include <QDomDocument>
#include <DHAVN_GracenoteInterface.h>
#include <QTime>
#include <QSqlError>
#include <DHAVN_GracenoteDBconnection.h>

/* supported speech languages */
/*const char* g_langs_id[LANG_COUNT] =
{
    "USA_eng",
    "MEX_spa",
    "CAN_fre",
    "GBR_eng",
    "FRA_fre",
    "ESP_spa",
    "DEU_ger",
    "ITA_ita",
    "JPN_jpn",
    "CHN_qad",
    "RUS_rus",
    "NLD_dut",
    "KOR_kor",
    "BRA_por",
    "PRT_por",
    "TUR_tur"
};*/

/* supported speech languages */
#define LANG_COUNT		4
const char* g_langs_id[LANG_COUNT] =
{
    "USA_eng",
    "MEX_spa",
    "CAN_fre",
    "KOR_kor"
};

extern int IPODDisconnected;
//Number for MLDBs to be maintained in the system for USB
extern int USB_MLDB_count;
//Number for MLDBs to be maintained in the system for IPOD
extern int IPOD_MLDB_count;

/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::CGracenoteOperationUtil
 *
 * Description 	: Constructor for CGracenoteOperationUtil

 * Parameters 	: parent (IN) -

 * Returns 	: None
 *
 *******************************************************************
 */
//added by Puneet for fixing GPP crash issue 2013.06.12
CGracenoteOperationUtil :: CGracenoteOperationUtil(QObject *parent):sem(1)
{
    Q_UNUSED(parent);
    LOG_INIT_V2("Media_GracenotePluginProcess");

    m_XSData.clear();
    m_TypeID3Name.clear();
    m_TypeName.clear();
    m_AlternatePhrase.clear();
    m_LanguageId = NULL;     // added by aettie.ji 2012.10.30 for static analysis #5423
    m_Type = eAlbumType;     // added by aettie.ji 2012.10.30 for static analysis #5423
}

/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::~CGracenoteOperationUtil
 *
 * Description 	: Destructor for CGracenoteOperationUtil

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */

CGracenoteOperationUtil :: ~CGracenoteOperationUtil()
{
}

/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::ParseFilePath
 *
 * Description 	: parse the file path and set parameters to start
       text id look up process

 * Parameters 	: filePath(IN) - media file path
       fileData(IN) - gn_pfile_data_t structure, in which
       various paremeters will be set

 * Returns 	: int - error, if any
 *
 *******************************************************************
 */
void CGracenoteOperationUtil :: ParseFilePath(QString filePath, gn_pfile_data_t &fileData)
{
    QString tempBuf;
    gn_error_t nError = GN_FAILURE;

    // Set file name
    tempBuf = filePath.section('/', -1, -1);
    gn_uchar_t* gfileName = gnepal_strdup( (gn_uchar_t *)  (tempBuf.toUtf8().constData()) );
    if(gfileName != GN_NULL)
    {
        gn_textid_file_data_set_filename(fileData, gfileName);

        // Parse file name
        nError = gn_textid_file_data_parse_filename(gfileName, fileData);
    }

    //Set path
    gn_uchar_t* gfilePath = gnepal_strdup( (gn_uchar_t *)  (filePath.toUtf8().constData()));
    if(gfilePath !=GN_NULL)
    {
        gn_textid_file_data_set_path(fileData,  gfilePath);
    }

    //Set parent folder
    tempBuf.clear();
    tempBuf = filePath.section('/', -4, -2);
    gn_uchar_t* parentFolder = gnepal_strdup( (gn_uchar_t *)  (tempBuf.toUtf8().constData()) );
    if(parentFolder != GN_NULL)
    {
        gn_textid_file_data_set_parent_folder(fileData, parentFolder);
    }

    //set grand parent folder
    tempBuf.clear();
    tempBuf = filePath.section('/', -4 , -3);
    gn_uchar_t* gparentFolder = gnepal_strdup( (gn_uchar_t *)  (tempBuf.toUtf8().constData()) );
    if(gparentFolder != GN_NULL)
    {
        gn_textid_file_data_set_g_parent_folder(fileData, gparentFolder);
    }

    // Set great grand parent folder
    tempBuf.clear();
    tempBuf = filePath.section('/', -4, -4);
    gn_uchar_t* ggparentFolder = gnepal_strdup( (gn_uchar_t *)  (tempBuf.toUtf8().constData()) );
    if(ggparentFolder != GN_NULL)
    {
        gn_textid_file_data_set_gg_parent_folder(fileData, ggparentFolder);
    }

    // Free the allocations
    if(gfileName != NULL) gnepal_mem_free(gfileName); gfileName = NULL;
    if(gfilePath != NULL) gnepal_mem_free(gfilePath); gfilePath = NULL;
    if(parentFolder != NULL) gnepal_mem_free(parentFolder); parentFolder = NULL;
    if(gparentFolder != NULL) gnepal_mem_free(gparentFolder); gparentFolder = NULL;
    if(ggparentFolder != NULL) gnepal_mem_free(ggparentFolder); ggparentFolder = NULL;
}

/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::ExtractID3Information
 *
 * Description 	: Extract ID3 metadata information from a media file

 * Parameters 	: filePath(IN) - media file path
       fileData(IN) - gn_pfile_data_t structure, in which
       various paremeters will be set

 * Returns 	: int - error, if any
 *
 *******************************************************************
 */
int CGracenoteOperationUtil :: ExtractID3Information(QString filePath, gn_pfile_data_t &fileData)
{
    gn_error_t nError = GN_FAILURE;

    gn_uchar_t* localFilePath = gnepal_strdup((gn_uchar_t *) (filePath.toUtf8().constData()));

    if(localFilePath == GN_NULL)
        return 0;

    gn_bool_t v1tag, v2tag;
    nError = gnid3_util_has_tags(localFilePath, &v2tag, &v1tag);

    LOG_TRACE <<("CGracenoteOperationUtil:localFilePath") << LOG_ENDL;
    LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(localFilePath))) << LOG_ENDL;

    LOG_TRACE <<("CGracenoteOperationUtil:gnid3_util_has_tags error")<< LOG_ENDL;
    LOG_INFO <<(nError) << LOG_ENDL;

    if(nError != GN_SUCCESS)
    {        
        //Free allocations
        if(localFilePath != NULL) gnepal_mem_free(localFilePath); localFilePath = NULL;

        return nError;
    }

    gnid3_reader_t id3reader = GN_NULL;
    nError = gnid3_reader_init(&id3reader, localFilePath, v1tag, v2tag, GN_TRUE, NULL);
    if( nError == GNEX_SUCCESS)
    {
        //Title
        gn_uchar_t* title = NULL;
        nError = gnid3_reader_get_track_title(id3reader, &title);
        if(nError == GNEX_SUCCESS)
        {
            QString result = encodeString(title);
            LOG_TRACE <<("result") << LOG_ENDL;
            LOG_INFO <<(result) << LOG_ENDL;
            gn_uchar_t* utf8title = gnepal_strdup((gn_uchar_t *)(result.toUtf8().constData()));
            LOG_TRACE <<("utf8title") << LOG_ENDL;
            LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(utf8title))) << LOG_ENDL;

            gn_textid_file_data_set_track_title(fileData, utf8title);
            if(utf8title != NULL) gnepal_mem_free(utf8title); utf8title = NULL;
            if(title != NULL) gnepal_mem_free(title); title = NULL;
        }
        else
        {

            // Set file name as the track tile, use base name so that extension doesnt appear
            QString tempBuf = QFileInfo(filePath).baseName();
            gn_uchar_t* fileName = gnepal_strdup( (gn_uchar_t *)  (tempBuf.toUtf8().constData()) );
            if(fileName != GN_NULL)
            {
                gn_textid_file_data_set_track_title(fileData, fileName);
            }
            if(fileName != NULL) gnepal_mem_free(fileName); fileName = NULL;
            LOG_TRACE << "File name being used as track title" << tempBuf << LOG_ENDL;
        }

        //Tag id
        gn_uchar_t* tagid= NULL;
        nError = gnid3_reader_get_tagid(id3reader, &tagid);
        if(nError == GNEX_SUCCESS)
        {
            gn_textid_file_data_set_tagid(fileData, tagid);
            if(tagid != NULL) gnepal_mem_free(tagid); tagid = NULL;
        }

        //Track arist
        gn_uchar_t* trackartist = NULL;
        nError = gnid3_reader_get_lead_performer(id3reader, &trackartist);
        if(nError == GNEX_SUCCESS)
        {
            QString result = encodeString(trackartist);
            LOG_TRACE <<("result") << LOG_ENDL;
            LOG_INFO <<(result) << LOG_ENDL;
            gn_uchar_t* utf8trackartist= gnepal_strdup((gn_uchar_t *)(result.toUtf8().constData()));
            LOG_INFO <<("utf8trackartist") << LOG_ENDL;
            LOG_TRACE <<(QString::fromUtf8(reinterpret_cast<const char*>(utf8trackartist))) << LOG_ENDL;

            gn_textid_file_data_set_track_artist(fileData, utf8trackartist);
            if(utf8trackartist != NULL) gnepal_mem_free(utf8trackartist); utf8trackartist = NULL;
            if(trackartist != NULL) gnepal_mem_free(trackartist); trackartist = NULL;
        }

        //Album title
        gn_uchar_t* album = NULL;
        nError = gnid3_reader_get_album_title(id3reader, &album);
        if(nError == GNEX_SUCCESS)
        {
            QString result = encodeString(album);
            LOG_TRACE <<("result") << LOG_ENDL;
            LOG_INFO <<(result) << LOG_ENDL;
            gn_uchar_t* utf8album= gnepal_strdup((gn_uchar_t *)(result.toUtf8().constData()));
            LOG_TRACE <<("utf8album") << LOG_ENDL;
            LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(utf8album))) << LOG_ENDL;

            gn_textid_file_data_set_disc_title(fileData, utf8album);
            if(utf8album != NULL) gnepal_mem_free(utf8album); utf8album = NULL;
            if(album != NULL) gnepal_mem_free(album); album = NULL;
        }

        //track num
        gn_uint32_t track;
        gn_uint32_t numtrack;
        nError = gnid3_reader_get_track_num(id3reader, &track, &numtrack);
        if(nError == GNEX_SUCCESS)
        {
            QString trackNum;
            trackNum.setNum(track);

            gn_uchar_t* trackNumber = gnepal_strdup((gn_uchar_t*) (trackNum.toUtf8().constData()));
            gn_textid_file_data_set_track_num(fileData, trackNumber);

            if(trackNumber != NULL) gnepal_mem_free(trackNumber); trackNumber = NULL;
        }
        //Genre
        gn_uchar_t* genre;
        nError = gnid3_reader_get_genre(id3reader, &genre);
        if(nError == GNEX_SUCCESS)
        {
            QString result = encodeString(genre);
            LOG_TRACE <<("result") << LOG_ENDL;
            LOG_INFO <<(result) << LOG_ENDL;
            gn_uchar_t* utf8genre= gnepal_strdup((gn_uchar_t *)(result.toUtf8().constData()));
            LOG_TRACE <<("utf8genre") << LOG_ENDL;
            LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(utf8genre))) << LOG_ENDL;

            gn_textid_file_data_set_genre(fileData, utf8genre);
            if(utf8genre != NULL) gnepal_mem_free(utf8genre);  utf8genre = NULL;
            if(genre != NULL) gnepal_mem_free(genre);  genre = NULL;
        }


        //Disc artist
        gn_uchar_t* discartist = NULL;
        nError = gnid3_reader_get_disc_artist(id3reader, &discartist);
        if(nError == GNEX_SUCCESS)
        {
            QString result = encodeString(discartist);
            LOG_TRACE <<("result") << LOG_ENDL;
            LOG_INFO <<(result) << LOG_ENDL;
            gn_uchar_t* utf8discartist= gnepal_strdup((gn_uchar_t *)(result.toUtf8().constData()));
            LOG_TRACE <<("utf8discartist") << LOG_ENDL;
            LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(utf8discartist))) << LOG_ENDL;

            gn_textid_file_data_set_disc_artist(fileData, utf8discartist);
            if(utf8discartist != NULL) gnepal_mem_free(utf8discartist); utf8discartist = NULL;
            if(discartist != NULL) gnepal_mem_free(discartist); discartist = NULL;
        }

        //Composer
        gn_uchar_t* composer = NULL;
        nError = gnid3_reader_get_composer(id3reader, &composer);
        if(nError == GNEX_SUCCESS)
        {
            QString result = encodeString(composer);
            LOG_TRACE <<("result") << LOG_ENDL;
            LOG_INFO <<(result) << LOG_ENDL;
            gn_uchar_t* utf8composer= gnepal_strdup((gn_uchar_t *)(result.toUtf8().constData()));

            LOG_TRACE <<("utf8composer")<< LOG_ENDL;
            LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(utf8composer))) << LOG_ENDL;

            gn_textid_file_data_set_composer(fileData, utf8composer);
            if(utf8composer != NULL) gnepal_mem_free(utf8composer); utf8composer = NULL;
            if(composer != NULL) gnepal_mem_free(composer); composer = NULL;
        }

        // Close the reader
        gnid3_reader_shutdown(&id3reader);
    }
    else
    {
        //Free allocations
        if(localFilePath != NULL) gnepal_mem_free(localFilePath); localFilePath = NULL;
        return nError;
    }

    //Free allocations
    if(localFilePath != NULL) gnepal_mem_free(localFilePath); localFilePath = NULL;
    return 0;
}

/*
 *******************************************************************
 * Function		: CGracenoteOperationUtil::GetDatabasePathForRegion
 *
 * Description 	: Select the correct database path as per the region
                          information received from UISH

 * Parameters 	: None

 * Returns 		: QString - database path
 *
 *******************************************************************
 */
QString CGracenoteOperationUtil::GetDatabasePathForRegion(QString countryVariant)
{
    LOG_TRACE <<("CGracenoteOperationUtil::GetDatabasePathForRegion Start") << countryVariant << LOG_ENDL;

    int country = 0;

    // Check if variant value exists
    if(countryVariant.length() == 0)
    {
        LOG_TRACE <<("CGracenoteOperationUtil::GetDatabasePathForRegion: Empty Country variant") << LOG_ENDL;

        QString regionFileName = "/gracenote/region.txt";
        // Check if region.txt file is present
        if(QFile::exists(regionFileName))
        {
            QFile file(regionFileName);
            if(file.open(QIODevice::ReadOnly))
            {
                LOG_TRACE <<("CGracenoteOperationUtil::GetDatabasePathForRegion: Read region.text") << LOG_ENDL;

                QTextStream in(&file);
                QString countryStr = in.readLine();
                LOG_TRACE <<(countryStr);
                country = countryStr.toInt();
            }
        }
        else
        {
            LOG_TRACE <<("CGracenoteOperationUtil::GetDatabasePathForRegion: Set default case") << LOG_ENDL;
            //Set the default country as Korea
            country = 0;
        }
    }
    else
    {
        LOG_TRACE <<("CGracenoteOperationUtil::GetDatabasePathForRegion: variant value came") << LOG_ENDL;
        countryVariant = countryVariant.remove("-cv:");
        country = countryVariant.toInt();
    }

    QString databasePath;
    databasePath = "/gracenote/ELDB/";

	//modified for DB country variant 20131023
    switch(country)
    {
	    case 0:
	        {
	            databasePath.append("KOR/");
	        }
	        break;
	    case 1:
	    case 6:
	        {
	            databasePath.append("NA/");
	        }
	        break;
	    case 2:
	        {
	             databasePath.append("CN/");
	         }
	        break;
	    case 3:
	    case 4:
	        {
	             databasePath.append("GLOBAL/");
	        }
	        break;
	    case 5:
	    case 7:
	        {
	             databasePath.append("EU/");
	        }
	        break;
	    default:
	       LOG_TRACE <<("You do not have proper gracenote databases for your country code.") << LOG_ENDL;
	       break;
    }
    LOG_TRACE <<("Final database path") << LOG_ENDL;
    LOG_INFO <<(databasePath) << LOG_ENDL;

    return databasePath;
}


/*
 *******************************************************************
 * Function		: CGracenoteOperationUtil::encodeString
 *
 * Description 	: This functions checks if the string is in UTF-8 format.
     If not in UTF-8, then encodes it in UTF-8 format.

 * Parameters 	: None

 * Returns 		: QString - String encoded in UTF-8 format
 *
 *******************************************************************
 */
QString CGracenoteOperationUtil::encodeString(gn_uchar_t* inputString)
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
 * Function	: CGracenoteOperationUtil::isUtf8
 *
 * Description 	: This functions checks if the string is in UTF-8 format.

 * Parameters 	: None

 * Returns 	: bool - True if encoded in UTF-8 format
 *			  False if not in UTF-8 format
 *
 *******************************************************************
 */

bool CGracenoteOperationUtil::isUtf8(const char * str )
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
 * Function	: CGracenoteOperationUtil::SetIPODID3Info
 *
 * Description 	: This functions sets the IPOD metadata recevied to
                          file data structure

 * Parameters 	: iPodMetadata - Metadata list of iPOD
                          fileData - file data structure

 * Returns 	: bool - True if information is set properly
 *			  False otherwise
 *
 *******************************************************************
 */
bool CGracenoteOperationUtil::SetIPODID3Info(QStringList iPodMetadata, gn_pfile_data_t &fileData)
{
    if(iPodMetadata.length() == 6)
    {
        LOG_TRACE <<("SetIPODID3Info: filename") << LOG_ENDL;
        LOG_INFO <<iPodMetadata.at(5) << LOG_ENDL;


        /*qDebug() <<"Title :"<< iPodMetadata.at(0).toUtf8().constData();
        qDebug() <<"Album :"<< iPodMetadata.at(1).toUtf8().constData();
        qDebug() <<"Artist :"<< iPodMetadata.at(2).toUtf8().constData();
        qDebug() <<"Genre :"<< iPodMetadata.at(3).toUtf8().constData();
        qDebug() <<"Composer :"<< iPodMetadata.at(4).toUtf8().constData();*/
        if((iPodMetadata.at(0).length() != 0))
        {
            //Title and filename
            gn_uchar_t* title = gnepal_strdup((gn_uchar_t *) (iPodMetadata.at(0).toUtf8().constData()));
            QString result = encodeString(title);

            gn_uchar_t* utf8Title = gnepal_strdup((gn_uchar_t *)(result.toUtf8().constData()));
            LOG_TRACE <<("SetIPODID3Info: title") << LOG_ENDL;
            LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(utf8Title))) << LOG_ENDL;
            gn_textid_file_data_set_track_title(fileData, utf8Title);

            if(utf8Title != NULL) gnepal_mem_free(utf8Title); utf8Title = NULL;
            if(title != NULL) gnepal_mem_free(title); title = NULL;
        }

        if((iPodMetadata.at(0).length() != 0))
        {
            //Filename
            gn_uchar_t* title = gnepal_strdup((gn_uchar_t *) (iPodMetadata.at(0).toUtf8().constData()));
            QString result = encodeString(title);

            gn_uchar_t* utf8filename = gnepal_strdup((gn_uchar_t *)(result.toUtf8().constData()));
            LOG_TRACE <<("SetIPODID3Info: filename") << LOG_ENDL;
            LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(utf8filename))) << LOG_ENDL;
            gn_textid_file_data_set_filename(fileData, utf8filename);

            if(utf8filename != NULL) gnepal_mem_free(utf8filename); utf8filename = NULL;
            if(title != NULL) gnepal_mem_free(title); title = NULL;
        }

        if((iPodMetadata.at(1)).length() != 0)
        {
            //Album title
            gn_uchar_t* album = gnepal_strdup((gn_uchar_t *) (iPodMetadata.at(1).toUtf8().constData()));

            QString result = encodeString(album);
            gn_uchar_t* utf8Album = gnepal_strdup((gn_uchar_t *)(result.toUtf8().constData()));

            LOG_TRACE <<("SetIPODID3Info: album")<< LOG_ENDL;
            LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(utf8Album))) << LOG_ENDL;
            gn_textid_file_data_set_disc_title(fileData, utf8Album);
            if(utf8Album != NULL) gnepal_mem_free(utf8Album); utf8Album = NULL;
            if(album != NULL) gnepal_mem_free(album); album = NULL;
        }


        if((iPodMetadata.at(2)).length() != 0)
        {
            //Track arist
            gn_uchar_t* trackartist = gnepal_strdup((gn_uchar_t *) (iPodMetadata.at(2).toUtf8().constData()));
            QString result = encodeString(trackartist);
            gn_uchar_t* utf8Artist = gnepal_strdup((gn_uchar_t *)(result.toUtf8().constData()));

            LOG_TRACE <<("SetIPODID3Info: trackartist") << LOG_ENDL;
            LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(utf8Artist))) << LOG_ENDL;
            gn_textid_file_data_set_track_artist(fileData, utf8Artist);
            if(utf8Artist != NULL) gnepal_mem_free(utf8Artist); utf8Artist = NULL;
            if(trackartist != NULL) gnepal_mem_free(trackartist); trackartist = NULL;
        }

        if((iPodMetadata.at(3)).length() != 0)
        {
            //genre
            gn_uchar_t* genre= gnepal_strdup((gn_uchar_t *) (iPodMetadata.at(3).toUtf8().constData()));
            QString result = encodeString(genre);
            gn_uchar_t* utf8Genre = gnepal_strdup((gn_uchar_t *)(result.toUtf8().constData()));

            LOG_TRACE <<("SetIPODID3Info: genre") << LOG_ENDL;
            LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(utf8Genre))) << LOG_ENDL;
            gn_textid_file_data_set_genre(fileData, utf8Genre);
            if(utf8Genre != NULL) gnepal_mem_free(utf8Genre); utf8Genre = NULL;
            if(genre != NULL) gnepal_mem_free(genre); genre = NULL;
        }

        if((iPodMetadata.at(4)).length() != 0)
        {
            //composer
            gn_uchar_t* composer= gnepal_strdup((gn_uchar_t *) (iPodMetadata.at(4).toUtf8().constData()));

            QString result = encodeString(composer);
            gn_uchar_t* utf8Composer = gnepal_strdup((gn_uchar_t *)(result.toUtf8().constData()));

            LOG_TRACE <<("SetIPODID3Info: composer") << LOG_ENDL;
            LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(utf8Composer))) << LOG_ENDL;
            gn_textid_file_data_set_composer (fileData, utf8Composer);
            if(utf8Composer != NULL) gnepal_mem_free(utf8Composer);  utf8Composer = NULL;
            if(composer != NULL) gnepal_mem_free(composer);  composer = NULL;
        }

        //Set path
        if((iPodMetadata.at(5)).length() != 0)
        {
            gn_uchar_t* filePath = gnepal_strdup( (gn_uchar_t *)  (iPodMetadata.at(5).toUtf8().constData()));
            if(filePath !=GN_NULL)
            {
                gn_textid_file_data_set_path(fileData,  filePath);
            }
            if(filePath != NULL) gnepal_mem_free(filePath); filePath = NULL;
        }
        return true;
    }

    return false;
}

/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::SetID3InfoForCD
 *
 * Description 	: This functions sets the Audio CD metadata recevied to
                          file data structure

 * Parameters 	: cdDataList - Metadata list of CD
                          fileData - file data structure

 * Returns 	: bool - True if information is set properly
 *			  False otherwise
 *
 *******************************************************************
 */
bool CGracenoteOperationUtil::SetID3InfoForCD(QStringList cdDataList, gn_pfile_data_t &fileData)
{
    if((cdDataList.at(4)).length() != 0)
    {
        ParseFilePath(cdDataList.at(4).toUtf8().constData(), fileData);

        if((cdDataList.at(0)).length() != 0)
        {
            //Album title
            gn_uchar_t* album = gnepal_strdup((gn_uchar_t *) (cdDataList.at(0).toUtf8().constData()));
            LOG_TRACE <<("SetID3InfoForCD: album") << LOG_ENDL;
            LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(album))) << LOG_ENDL;
            gn_textid_file_data_set_disc_title(fileData, album);
            if(album != NULL) gnepal_mem_free(album);  album = NULL;
        }

        if((cdDataList.at(1)).length() != 0)
        {
            //Track arist
            gn_uchar_t* trackartist = gnepal_strdup((gn_uchar_t *) (cdDataList.at(1).toUtf8().constData()));
            LOG_TRACE <<("SetID3InfoForCD: trackartist") << LOG_ENDL;
            LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(trackartist))) << LOG_ENDL;
            gn_textid_file_data_set_track_artist(fileData, trackartist);
            if(trackartist != NULL) gnepal_mem_free(trackartist);  trackartist = NULL;
        }

        if((cdDataList.at(2).length() != 0))
        {
            //Title
            gn_uchar_t* title = gnepal_strdup((gn_uchar_t *) (cdDataList.at(2).toUtf8().constData()));
            LOG_TRACE <<("SetID3InfoForCD: title") << LOG_ENDL;
            LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(title))) << LOG_ENDL;
            gn_textid_file_data_set_track_title(fileData, title);
            if(title != NULL) gnepal_mem_free(title);  title = NULL;
        }

        if((cdDataList.at(3)).length() != 0)
        {
            //genre
            gn_uchar_t* genre= gnepal_strdup((gn_uchar_t *) (cdDataList.at(3).toUtf8().constData()));
            LOG_TRACE <<("SetID3InfoForCD: genre") << LOG_ENDL;
            LOG_INFO <<(QString::fromUtf8(reinterpret_cast<const char*>(genre))) << LOG_ENDL;
            gn_textid_file_data_set_genre(fileData, genre);
            if(genre != NULL) gnepal_mem_free(genre);  genre = NULL;
        }
        return true;
    }
    return false;
}

/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::GetXSampaDataForAlbum
 *
 * Description 	: This function fetches XSampa data for Album

 * Parameters 	: albumName - album name
                          albumID3Name - ID3 album name
                          resultData - TextID lookup result data structure
 * Returns 	: None
 *
 *******************************************************************
 *
 //deleted by aettie Xsampa DB is not used.
void CGracenoteOperationUtil::GetXSampaDataForAlbum(QString albumName,
                                                    QString albumID3Name,
                                                    gn_textid_presult_data_t resultData)
{
    LOG_TRACE << "GetXSampaDataForAlbum Start" << LOG_ENDL;

    gn_prep_array_t prepArray;
    gn_error_t error;
    gn_prepresentation_t representation = NULL;

    m_XSData.clear();

    m_TypeID3Name = albumID3Name;
    m_TypeName = albumName;
    m_Type = eAlbumType;

    LOG_TRACE << "albumName" << albumName << LOG_ENDL;

    error = gn_textid_get_album_title_representation_array(resultData, &prepArray);
    if(error == GN_SUCCESS)
    {
        gn_uint32_t count;
        error = gn_representation_get_element_count(prepArray, &count);
        if(error == GN_SUCCESS)
        {
            LOG_TRACE << "Element count is : " << count << LOG_ENDL;

            for (gn_uint32_t index = 0; index < count ; index++)
            {
                error = gn_representation_get_rep(prepArray, index, &representation);
                if((error == GN_SUCCESS) && (representation != GN_NULL))
                    ProcessRepresentation(representation);
            }
        }
    }
    else
    {
        LOG_TRACE << "Cant get album result" << LOG_ENDL ;
    }

    LOG_TRACE << "GetXSampaDataForAlbum End" << LOG_ENDL;
}*/


/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::GetXSampaDataForArtist
 *
 * Description 	: This function fetches XSampa data for Artist

 * Parameters 	: artistName - artist name
                          artistID3Name - ID3 artist name
                          resultData - TextID lookup result data structure
 * Returns 	: None
 *
 *******************************************************************
 *
 //deleted by aettie Xsampa DB is not used.
void CGracenoteOperationUtil::GetXSampaDataForArtist(QString artistName,
                                                     QString artistID3Name,
                                                     gn_textid_presult_data_t resultData)
{
    LOG_TRACE << "GetXSampaDataForArtist Start" << LOG_ENDL;

    gn_prep_array_t prepArray;
    gn_error_t error;
    gn_prepresentation_t representation = NULL;

    m_XSData.clear();

    m_TypeID3Name = artistID3Name;
    m_TypeName = artistName;
    m_Type = eArtistType;

    error = gn_textid_get_primary_artist_representation_array(resultData, &prepArray);
    if(error == GN_SUCCESS)
    {
        gn_uint32_t count;
        error = gn_representation_get_element_count(prepArray, &count);
        if(error == GN_SUCCESS)
        {
            LOG_TRACE << "Element count is : " << count << LOG_ENDL;

            for (gn_uint32_t index = 0; index < count ; index++)
            {
                error = gn_representation_get_rep(prepArray, index, &representation);
                if((error == GN_SUCCESS) && (representation != GN_NULL))
                    ProcessRepresentation(representation);
            }
        }
    }

    LOG_TRACE << "GetXSampaDataForArtist End" << LOG_ENDL;
}*/

/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::ProcessRepresentation
 *
 * Description 	:

 * Parameters 	:
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteOperationUtil::ProcessRepresentation(gn_prepresentation_t rep)
{
    LOG_TRACE << "ProcessRepresentation Start" << LOG_ENDL;
    gn_uchar_t* dispStr;
    gn_ptranscript_arr_t transcriptArr;

    gn_error_t error = gn_representation_get_display_string(rep, &dispStr);

    m_AlternatePhrase.clear();
    m_AlternatePhrase.append(QString::fromUtf8(reinterpret_cast<const char*> (dispStr)));

    if (error == GN_SUCCESS)
    {
        for(int index=0; index < LANG_COUNT; index++)
        {
            m_LanguageId = gnepal_strdup((gn_uchar_t*)g_langs_id[index]);

            error = gn_representation_get_transcript_array(rep, m_LanguageId, &transcriptArr);
            if((error == GN_SUCCESS) && (transcriptArr != GN_NULL))
            {
                ProcessTranscriptionArray(dispStr, transcriptArr);
            }
            else
            {
                LOG_TRACE << "Error in ProcessRepresentation (not available for language)" << LOG_ENDL;
            }

            //Free memory allocation
            if(m_LanguageId != NULL) gnepal_mem_free(m_LanguageId); m_LanguageId = NULL;
            gn_transcript_array_destroy(&transcriptArr);
        }
    }

    LOG_TRACE << "ProcessRepresentation End" << LOG_ENDL;
}

/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::ProcessTranscriptionArray
 *
 * Description 	:

 * Parameters 	:
 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteOperationUtil::ProcessTranscriptionArray(
        const gn_uchar_t*	orthography,
        const gn_ptranscript_arr_t transcriptionArray)
{
    LOG_TRACE << "ProcessTranscriptionArray Start" << LOG_ENDL;

    gn_error_t	 error = GN_SUCCESS;
    gn_uint32_t      transcription_array_size = 0;
    gn_ptranscript_t transcript = GN_NULL;

    if ((GN_NULL == transcriptionArray) ||  (GN_NULL == orthography))
    {
        return;
    }

    error = gn_transcript_array_get_array_size(transcriptionArray, &transcription_array_size);
    if (GN_SUCCESS != error)
    {
        LOG_TRACE << "Error getting transcription array size" << LOG_ENDL;
    }

    for (gn_uint32_t index = 0; index < transcription_array_size; index++)
    {
        error = gn_transcript_array_get_element(transcriptionArray, index, &transcript);
        if (GN_SUCCESS != error)
        {
            LOG_TRACE << "Error getting gn_transcript_array_get_element" << LOG_ENDL;
        }

        if (GN_NULL != transcript)
        {
            ProcessTranscription(orthography, transcript);
        }
    }

    LOG_TRACE << "ProcessTranscriptionArray End" << LOG_ENDL;
}

/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::ProcessTranscription
 *
 * Description 	:

 * Parameters 	:
 * Returns 	: None
 *
 *******************************************************************
 */
 //modified by aettie for Static Defect 20131007
void CGracenoteOperationUtil::ProcessTranscription(const gn_uchar_t* orthography,
                                                         const gn_ptranscript_t transcription)
{
    /* MediaVOCS may supply multiple transcriptions for the same orthography,   */
    /* including common mispronunciations. You should add all transcriptions to */
    /* your Automatic Speech Recognition (ASR) system in order to recognize all */
    /* the variants for a particular orthography                                */

    /* But since the Text-to-Speech (TTS) system produces audible words for     */
    /* a given orthography it is important to only add transcriptions which     */
    /* indicate that they are a correct pronunciation.                          */

    LOG_TRACE << "ProcessTranscription Start" << LOG_ENDL;

//deleted by aettie.ji 20130904 

}

/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::GetXSampaList
 *
 * Description 	:

 * Parameters 	:
 * Returns 	: None
 *
 *******************************************************************
 *
 //deleted by aettie Xsampa DB is not used.
void CGracenoteOperationUtil::GetXSampaList(QVector<QStringList>& phoneticData)
{
    phoneticData = m_XSData;
}*/

/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::ReadDataFromIPODSqlDB()
 *
 * Description 	: This functions reads the data from IPod SQL Master DB
                          at tempfs

 * Parameters 	: None

 * Returns 	: QVector of data whatever read from IPod SQL Master DB
 *
 *******************************************************************
 */

GNVList CGracenoteOperationUtil::ReadDataFromIPODSqlDB(QString iPODSerialNum,
                                                       int startIndex, int endIndex, bool allData)
{
    LOG_TRACE << "CGracenoteOperationUtil::ReadDataFromIPODSqlDB() START" << LOG_ENDL;
    GNVList dataList;
    dataList.clear();

    QString IPODDBPath("/dev/shm/");
    IPODDBPath.append(iPODSerialNum);

    LOG_TRACE << "Start Index"<< startIndex << "End Index" << endIndex << LOG_ENDL;

    QString index1, index2;
    index1.setNum(startIndex);
    index2.setNum(endIndex);

    QFile dbpath(IPODDBPath);
    // { Added by Neelima, 2013.01.03 for CR13965
    //If DB is absent in /dev/shm then fetch data from FileSystem
    if(!dbpath.exists())
    {
        IPODDBPath.clear();
        IPODDBPath.append("/app/share/iPodDBPath/");
        IPODDBPath.append(iPODSerialNum);
    }
	// } Added by Neelima
    if(dbpath.exists())
    {
        LOG_TRACE <<("CGracenoteOperationUtil:ReadDataFromIPODSqlDB dbpath exists") << LOG_ENDL;

        QSqlDatabase db;

        QString connectionName;
        connectionName.append("MasterDB");
   		
        if(createConnection(IPODDBPath, connectionName, db))
        {
            LOG_TRACE <<("CGracenoteOperationUtil:ReadDataFromIPODSqlDB db opened") << LOG_ENDL;

            QSqlQuery* query = new QSqlQuery(db);
            if(allData)
            {
                qDebug() <<"All case";
                query->prepare("SELECT T_Title, T_Album, T_Artist, T_Genre, T_Composer, T_FilePath FROM MasterTable");
            }
            else
            {
                qDebug() <<"Between case";
                query->prepare("SELECT T_Title, T_Album, T_Artist, T_Genre, T_Composer, T_FilePath FROM MasterTable WHERE T_TypeIndex between ? and ?");
                query->addBindValue(index1);
                query->addBindValue(index2);
            }

            bool status = query->exec();
            if(status)
            {
                LOG_TRACE <<("CGracenoteOperationUtil:ReadDataFromIPODSqlDB Select query success") << LOG_ENDL;
                while (query->next())
                {
                    QStringList columnList;
                    for (int i = 0; i < 6; i++)
                    {
                        QString value = query->value(i).toString();
                        columnList.append(value);
                    }
                    dataList.append(columnList);
                }//end of while
            }//end of if(status)
            else
            {
                LOG_TRACE <<("CGracenoteOperationUtil:ReadDataFromIPODSqlDB Select query failed !!!") << LOG_ENDL;
            }
            delete query;
            closeConnection(db); //added by Puneet for fixing GPP crash issue 2013.06.12
//            db.close(); //Removed by Puneet for fixing GPP crash issue 2013.06.12
        }
    }//end of dbpath.exists
    else
    {
        LOG_TRACE <<("CGracenoteOperationUtil:ReadDataFromIPODSqlDB IPOD DB deos not exist") << LOG_ENDL;
    }

    LOG_TRACE << "CGracenoteOperationUtil::ReadDataFromIPODSqlDB() END" << LOG_ENDL;
    return dataList;
}


/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::GetMLDBConfiguration()
 *
 * Description 	: This functions reads the MLDB configurations set by the
 *                        customer from the config xml file.
 *
 * Parameters 	: None

 * Returns  	: Bool value, False if Configuration reading failed
 *                       True if values are set in the config file.
 *
 *******************************************************************
 */
bool CGracenoteOperationUtil::GetMLDBConfiguration()
{
    LOG_TRACE <<("CGracenoteOperationUtil::GetMLDBConfiguration Start") << LOG_ENDL;

    QDomDocument doc( "GracenoteMLDB_Configuration" );
    QFile file( "/app/data/gracenote/configuration/Gracenote_Configuration.xml" );
    if( !file.open( QIODevice::ReadOnly ) )
        return false;

    if( !doc.setContent( &file ) )
    {
        file.close();
        return false;
    }
    file.close();

    QDomElement root = doc.documentElement();
    if( root.tagName() != "GracenoteConfiguration" )
        return false;

    QDomNode domNode = root.firstChild();
    while( !domNode.isNull() )
    {
        QDomElement domElement = domNode.toElement();
        if( !domElement.isNull() )
        {
            if( domElement.tagName() == "IPOD" )
            {
                QString val = domElement.attribute( "MLDB", "" );
                IPOD_MLDB_count = val.toInt();
                LOG_TRACE <<("IPOD_MLDB_count") << LOG_ENDL;
                LOG_INFO <<IPOD_MLDB_count<< LOG_ENDL;
            }
            else if(domElement.tagName() == "USB")
            {
                QString val = domElement.attribute( "MLDB", "" );
                USB_MLDB_count = val.toInt();
                LOG_TRACE <<("USB_MLDB_count") << LOG_ENDL;
                LOG_INFO <<USB_MLDB_count<< LOG_ENDL;
            }
        }
        domNode =domNode.nextSibling();
    }

    LOG_TRACE <<("CGracenoteOperationUtil::GetMLDBConfiguration End") << LOG_ENDL;
    return true;
}

/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::GetTextIDCacheInfoFomConfig()
 *
 * Description 	: This functions reads the TextID cache configurations set by the
*                         customer in the config xml file.
 *
 * Parameters 	: None

 * Returns  	: QStringList
 *
 *******************************************************************
 */
QStringList CGracenoteOperationUtil::GetTextIDCacheInfoFomConfig()
{
    LOG_TRACE <<("CGracenoteOperationUtil::GetCacheSizeFomConfig Start") << LOG_ENDL;
    QStringList cachingVal;
    cachingVal.clear();
    cachingVal.append("81920"); //Cache Buffer Size
    LOG_TRACE <<("CGracenoteOperationUtil::GetCacheSizeFomConfig Start") << LOG_ENDL;
    cachingVal.append("300"); //Album Cache Size
    cachingVal.append("300"); //Composer Cache Size
    cachingVal.append("50"); //Genre Cache Size
    cachingVal.append("0"); //Caching Disabled
    LOG_TRACE <<("CGracenoteOperationUtil::GetCacheSizeFomConfig Start") << LOG_ENDL;

    QDomDocument doc( "GracenoteMLDB_Configuration" );
    QFile file( "/app/data/gracenote/configuration/Gracenote_Configuration.xml" );
    if( !file.open( QIODevice::ReadOnly ) )
        return cachingVal;

    if( !doc.setContent( &file ) )
    {
        file.close();
        return cachingVal;
    }
    file.close();

    QDomElement root = doc.documentElement();
    if( root.tagName() != "GracenoteConfiguration" )
        return cachingVal;

    QDomNode domNode = root.firstChild();
    while( !domNode.isNull() )
    {
        QDomElement domElement = domNode.toElement();
        if( !domElement.isNull() )
        {
            if( domElement.tagName() == "CACHING" )
            {
                cachingVal[4] = domElement.attribute( "SWITCH", "" );
                LOG_TRACE <<("TextIDCachingEnabledFromConfig value") << LOG_ENDL;
                LOG_INFO <<cachingVal[4]<< LOG_ENDL;
            }
            else if( domElement.tagName() == "TEXTID_BUF" )
            {
                cachingVal[0]  = domElement.attribute( "SIZE", "" );
                LOG_TRACE <<("GetTextIDBufSizeFromConfig value") << LOG_ENDL;
                LOG_INFO <<cachingVal[0] << LOG_ENDL;
            }
            else if( domElement.tagName() == "TEXTID_LEADPERFORMER_CACHE" )
            {
                cachingVal[2] = domElement.attribute( "SIZE", "" );
                LOG_TRACE <<("GetLeadPerformerCacheSizeFomConfig value") << LOG_ENDL;
                LOG_INFO <<cachingVal[2]<< LOG_ENDL;
            }
            else if( domElement.tagName() == "TEXTID_ALBUM_CACHE" )
            {
                cachingVal[1] = domElement.attribute( "SIZE", "" );
                LOG_TRACE <<("GetTextIDAlbumCacheSizeFromConfig value") << LOG_ENDL;
                LOG_INFO <<cachingVal[1]<< LOG_ENDL;
            }
            else if( domElement.tagName() == "TEXTID_GENRE_CACHE" )
            {
                cachingVal[3] = domElement.attribute( "SIZE", "" );
                LOG_TRACE <<("GetTextIDGenreCacheSizeFromConfig value") << LOG_ENDL;
                LOG_INFO << cachingVal[3] << LOG_ENDL;
            }
        }
        domNode =domNode.nextSibling();
    }

    LOG_TRACE <<("CGracenoteOperationUtil::GetCacheSizeFomConfig End") << LOG_ENDL;

    return cachingVal;
}

/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::GetResyncData()
 *
 * Description 	: Get the resync data for specific iPod. It will calculate
 *                newly added, modified and deleted files
 *
 * Parameters 	: iPodSerialNum - IPOD Serial number
 *                newList       - List of new files
 *                modifiedList  - List of modified files
 *                deletedList   - List of deleted files
 * Returns  	: None
 *
 *******************************************************************
 */
void CGracenoteOperationUtil::GetResyncData(QString iPodSerialNum, GNVList& newList,
                                            GNVList& modifiedList, GNVList& deletedList)
{
    bool isIndexBased = CheckIPODType(iPodSerialNum);

    if(isIndexBased)
    {
        PrepareDataForIndexBased(iPodSerialNum, newList, modifiedList, deletedList);
    }
    else
    {
        // There will not be any modified case for UID based iPod
        modifiedList.clear();
        PrepareDataForUIDBased(iPodSerialNum, newList, deletedList);
    }
}

/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::PrepareDataForIndexBased()
 *
 * Description 	: Prepare the list of data for Index based device
 *
 * Parameters 	: iPodSerialNum - IPOD Serial number
 *                       newList       - List of new files
 *                       modifiedList  - List of modified files
 *                       deletedList   - List of deleted files
 * Returns  	: None
 *
 *******************************************************************
 */
void CGracenoteOperationUtil::PrepareDataForIndexBased(QString iPodSerialNum, GNVList& newList,
                                                       GNVList& modifiedList,GNVList& deletedList)
{

    LOG_TRACE << "CGracenoteOperationUtil::GetResyncData Enter" << LOG_ENDL;

    // Get data of old Master table
    QVector<QStringList> oldDataList;
    QVector<QStringList> newDataList;

    oldDataList = ReadResyncDataForIndexBased(iPodSerialNum, true);

    if(oldDataList.count() == 0)
    {
        LOG_TRACE << "GetResyncData: Old table is not present, Cant Perform Resync" << LOG_ENDL;
        return;
    }

    newDataList = ReadResyncDataForIndexBased(iPodSerialNum, false);

    if(newDataList.count() == 0)
    {
        LOG_TRACE << "GetResyncData: New Master table is not present, Cant Perform Resync" << LOG_ENDL;
        return;
    }

    // Get deleted files list

    QStringList oldTitles;
    QStringList newTitles;

    for(int index = 0 ; index < oldDataList.count(); index++)
    {
        oldTitles.append(oldDataList.at(index).at(0));
    }

    for(int index = 0 ; index < newDataList.count(); index++)
    {
        newTitles.append(newDataList.at(index).at(0));
    }

    LOG_TRACE << "CGracenoteOperationUtil::Old Title Count : " << oldTitles.count() << LOG_ENDL;
    LOG_TRACE << "CGracenoteOperationUtil::New Title Count : " << newTitles.count() << LOG_ENDL;

    QStringList singleDeleteEntry;

    for (int index = 0 ; index < oldTitles.count(); index++)
    {
        // Check if the titles in the old list, are present in the new or not
        if(!newTitles.contains(oldTitles.at(index)))
        {
            singleDeleteEntry.append(oldTitles.at(index));
            // Get the corresponding file path to be deleted from MLDB
            singleDeleteEntry.append(oldDataList.at(index).at(5));

            deletedList.append(singleDeleteEntry);
            singleDeleteEntry.clear();
        }
    }

    LOG_TRACE << "CGracenoteOperationUtil::Deleted list count : " << deletedList.count() << LOG_ENDL;

    // Get the modified or new entries
    QStringList singleModifiedEntry;
    int indexInOldList;
    QString oldPath, newPath;

    for(int index = 0; index < newTitles.count(); index++)
    {
        indexInOldList = oldTitles.indexOf(newTitles.at(index));

        // Index will be -1 , in case there was no match found
        if(indexInOldList >=0)
        {
            // Need to check, if there is any update in the path
            // Index based IPOD could change the file path
            oldPath = oldDataList.at(indexInOldList).at(5);
            newPath = newDataList.at(index).at(5);

            if(oldPath != newPath)
            {
                // Append title and file path
                singleModifiedEntry.append(newTitles.at(index));
                singleModifiedEntry.append(newPath);

                modifiedList.append(singleModifiedEntry);
                singleModifiedEntry.clear();
            }
        }
        else
        {
            //New data to be populated
            newList.append(newDataList.at(index));
        }
    }

    LOG_TRACE << "CGracenoteOperationUtil::Modified list count : " << modifiedList.count() << LOG_ENDL;
    LOG_TRACE << "CGracenoteOperationUtil::New list count : " << newList.count() << LOG_ENDL;
    LOG_TRACE << "CGracenoteOperationUtil::GetResyncData Exit " << LOG_ENDL;

}


/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::PrepareDataForUIDBased()
 *
 * Description 	: Prepare the list of data for UID based device
 *
 * Parameters 	: iPodSerialNum - IPOD Serial number
 *                       newList       - List of new files *
 *                       deletedList   - List of deleted files
 * Returns  	: None
 *
 *******************************************************************
 */
void CGracenoteOperationUtil::PrepareDataForUIDBased(QString iPodSerialNum, GNVList& newList,
                                                                                          GNVList& deletedList)
{
    // There is no modified list, in case of UID based device
    newList.clear();
    deletedList.clear();

    //newList = ReadResyncDataForUIDBased(iPodSerialNum, true);
    deletedList = ReadResyncDataForUIDBased(iPodSerialNum, false);
}

//added by Puneet for fixing GPP crash issue 2013.06.12
bool CGracenoteOperationUtil::createConnection(QString dbPath, QString connectionName, QSqlDatabase &dBHandle)
{
    bool retVal = false;
    LOG_TRACE << "CGracenoteOperationUtil::createConnection: Sem trying to Acquire with available = " << sem.available() <<  LOG_ENDL;
    sem.acquire(1);
    LOG_TRACE << "CGracenoteOperationUtil::createConnection: Sem Acquired " << LOG_ENDL;

    if(QSqlDatabase::contains(connectionName))
    {
        LOG_TRACE << "CGracenoteOperationUtil::createConnection: already have this connection name"<< LOG_ENDL;

        dBHandle = QSqlDatabase::database(connectionName);
        dBHandle.setDatabaseName(dbPath);
        if(dBHandle.isOpen())
        {
            LOG_TRACE << "CGracenoteOperationUtil::createConnection: already opened successfully"<< LOG_ENDL;
            retVal = true;
        }
        else
        {
            if (!dBHandle.open())
            {
                sem.release(1);
//                m_mutex_db.unlockInline();
                LOG_TRACE << "CGracenoteOperationUtil::createConnection: Sem Released on Error Avail = " << sem.available()<< LOG_ENDL;
                LOG_TRACE << "CGracenoteOperationUtil::createConnection: not opened successfully"<< LOG_ENDL;
                retVal = false;
            }
            else
            {
                LOG_TRACE << "CGracenoteOperationUtil::createConnection: opened successfully"<< LOG_ENDL;
                retVal = true;
            }
        }
    }
    else
    {
        LOG_TRACE << "CGracenoteOperationUtil::createConnection: not already have this connection name"<< LOG_ENDL;
        dBHandle = QSqlDatabase::addDatabase("QSQLITE",connectionName);
        dBHandle.setDatabaseName(dbPath);
        if (!dBHandle.open())
        {
            sem.release(1);
            LOG_TRACE << "CGracenoteOperationUtil::createConnection: Sem Released on Error, Avail = " << sem.available() << LOG_ENDL;
            LOG_TRACE << "CGracenoteOperationUtil::createConnection: not opened successfully"<< LOG_ENDL;
            retVal = false;
        }
        else
        {

            LOG_TRACE << "CGracenoteOperationUtil::createConnection: opened successfully"<< LOG_ENDL;
            retVal = true;
        }
    }
    return retVal;
}

//added by Puneet for fixing GPP crash issue 2013.06.12
//by aettie for Static Test Defect 201300711
void CGracenoteOperationUtil::closeConnection(QSqlDatabase &dBHandle)
{
    dBHandle.close();
    sem.release(1);
    LOG_TRACE << "CGracenoteOperationUtil::closeConnection: Sem Released on request of close with avail = " << sem.available() << LOG_ENDL;
}


/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::ReadResyncDataForIndexBased()
 *
 * Description 	: Read the new and old entries from iPod database
 *
 * Parameters 	: iPodSerialNum - IPOD Serial number
 *                oldTable - true, if entry should be read from old
 *                table, else false for MasterTable
 * Returns  	: None
 *
 *******************************************************************
 */
GNVList CGracenoteOperationUtil::ReadResyncDataForIndexBased(QString iPODSerialNum,
                                                                                                        bool oldTable)
{
    LOG_TRACE << "CGracenoteOperationUtil::ReadResyncDataForIndexBased() START" << LOG_ENDL;
    GNVList dataList;
    dataList.clear();

    QString IPODDBPath("/app/share/iPodDBPath/");    
    IPODDBPath.append(iPODSerialNum);

    QFile dbpath(IPODDBPath);
    if(dbpath.exists())
    {
        LOG_TRACE <<("CGracenoteOperationUtil:ReadResyncDataForIndexBased dbpath exists") << LOG_ENDL;

        // Remove the old connection
        QSqlDatabase::removeDatabase("MasterDB");

        QSqlDatabase db;
        QString connectionName;
        connectionName.append("MasterDB");
   		
        if(createConnection(IPODDBPath, connectionName, db))
        {
            LOG_TRACE <<("CGracenoteOperationUtil:ReadResyncDataForIndexBased db opened") << LOG_ENDL;

            LOG_TRACE << "Getting list of tables" << LOG_ENDL;
            QStringList list = db.tables(QSql::Tables);
            for(int i = 0; i < list.count(); i++)
                LOG_TRACE << list.at(i) << LOG_ENDL;

            LOG_TRACE << "Getting list of tables end" << list.count() << LOG_ENDL;

            QSqlQuery* query = new QSqlQuery(db);
            if(oldTable)
            {
                LOG_TRACE << "Old Table case" <<LOG_ENDL;
                query->prepare("SELECT T_Title, T_Album, T_Artist, T_Genre, T_Composer, T_FilePath FROM OldTable");
            }
            else
            {
                LOG_TRACE << "Master Table case" <<LOG_ENDL;
                query->prepare("SELECT T_Title, T_Album, T_Artist, T_Genre, T_Composer, T_FilePath FROM MasterTable");
            }

            bool status = query->exec();
            if(status)
            {
                LOG_TRACE <<("CGracenoteOperationUtil:ReadResyncDataForIndexBased Select query success") << LOG_ENDL;
                while (query->next())
                {
                    QStringList columnList;
                    for (int i = 0; i < 6; i++)
                    {
                        QString value = query->value(i).toString();
                        columnList.append(value);
                    }
                    dataList.append(columnList);
                }//end of while
            }//end of if(status)
            else
            {
                LOG_TRACE <<("CGracenoteOperationUtil:ReadResyncDataForIndexBased Select query failed !!!") << LOG_ENDL;
                LOG_TRACE << query->lastError().text() << LOG_ENDL;
            }
            delete query;
            closeConnection(db); //added by Puneet for fixing GPP crash issue 2013.06.12
//            db.close();//Removed by Puneet for fixing GPP crash issue 2013.06.12
        }
    }//end of dbpath.exists
    else
    {
        LOG_TRACE <<("CGracenoteOperationUtil:ReadResyncDataForIndexBased IPOD DB deos not exist") << LOG_ENDL;
    }

    LOG_TRACE << "CGracenoteOperationUtil::ReadResyncDataForIndexBased() END" << LOG_ENDL;
    return dataList;
}


/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::ReadResyncDataForUIDBased()
 *
 * Description 	: Read the new and deleted entries from iPod database
 *
 * Parameters 	: iPodSerialNum - IPOD Serial number
 *                       newEntries - true, if new entries to be read, else false
 * Returns  	: None
 *
 *******************************************************************
 */
GNVList CGracenoteOperationUtil::ReadResyncDataForUIDBased(QString iPODSerialNum, bool newEntries)
{

    LOG_TRACE << "CGracenoteOperationUtil::ReadResyncDataForUIDBased() START" << LOG_ENDL;
    GNVList dataList;
    dataList.clear();

    QString IPODDBPath("/app/share/iPodDBPath/");
    IPODDBPath.append(iPODSerialNum);

    QFile dbpath(IPODDBPath);
    if(dbpath.exists())
    {
        LOG_TRACE <<("CGracenoteOperationUtil:ReadResyncDataForUIDBased dbpath exists") << LOG_ENDL;

        // Remove the old connection
        QSqlDatabase::removeDatabase("MasterDB");

        QSqlDatabase db;
        QString connectionName;
        connectionName.append("MasterDB");
   		
        if(createConnection(IPODDBPath, connectionName, db))
        {
            LOG_TRACE <<("CGracenoteOperationUtil:ReadResyncDataForUIDBased db opened") << LOG_ENDL;

            QSqlQuery* query = new QSqlQuery(db);
            if(newEntries)
            {
                LOG_TRACE << "New Entry case" <<LOG_ENDL;
                query->prepare("SELECT T_Title, T_FilePath FROM MasterTable except SELECT T_Title, T_FilePath FROM OldTable");
            }
            else
            {
                LOG_TRACE << "Deleted Entry case" <<LOG_ENDL;
                query->prepare("SELECT T_Title, T_FilePath FROM OldTable except SELECT T_Title, T_FilePath FROM MasterTable");
            }

            bool status = query->exec();
            if(status)
            {
                LOG_TRACE <<("CGracenoteOperationUtil:ReadResyncDataForUIDBased Select query success") << LOG_ENDL;
                 while (query->next())
                {
                     QStringList columnList;
                     for (int i = 0; i < 2; i++)
                     {
                         QString value = query->value(i).toString();
                         columnList.append(value);
                     }
                     dataList.append(columnList);
                 }//end of while
             }//end of if(status)
             else
            {
                 LOG_TRACE <<("CGracenoteOperationUtil:ReadResyncDataForUIDBased Select query failed !!!") << LOG_ENDL;
                 LOG_TRACE << query->lastError().text() << LOG_ENDL;
             }
             delete query;
             closeConnection(db); //added by Puneet for fixing GPP crash issue 2013.06.12
//            db.close(); //Removed by Puneet for fixing GPP crash issue 2013.06.12
         }
    }//end of dbpath.exists
    else
    {
        LOG_TRACE <<("CGracenoteOperationUtil:ReadResyncDataForUIDBased IPOD DB deos not exist") << LOG_ENDL;
    }

    LOG_TRACE << "CGracenoteOperationUtil::ReadResyncDataForUIDBased() END" << LOG_ENDL;
    return dataList;
}

/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::CheckIPODType()
 *
 * Description 	: Check the iPod Type
 *
 * Parameters 	: iPodSerialNum - IPOD Serial number
 *
 * Returns  	: bool, true if index based else false, UID based
 *
 *******************************************************************
 */
bool CGracenoteOperationUtil::CheckIPODType(QString iPodSerialNum)
{
    LOG_TRACE << "CGracenoteOperationUtil::CheckIPODType Enter" << LOG_ENDL;

    bool isIndexBased = false;

    QString IPODDBPath("/app/share/iPodDBPath/");
    IPODDBPath.append(iPodSerialNum);

    QFile dbpath(IPODDBPath);
    if(dbpath.exists())
    {
        LOG_TRACE <<("CGracenoteOperationUtil:CheckIPODType dbpath exists") << LOG_ENDL;

        // Remove the old connection
        QSqlDatabase::removeDatabase("MasterDB");

        QSqlDatabase db;
        QString connectionName;
        connectionName.append("MasterDB");
   		
        if(createConnection(IPODDBPath, connectionName, db))
        {
            QSqlQuery* query = new QSqlQuery(db);
            query->prepare("SELECT T_UID FROM MasterTable where T_index = 1");

            QString value;
            bool status = query->exec();

            if(status)
            {
                while (query->next())
                {
                    value = query->value(0).toString();
                }
            }

            delete query;
            closeConnection(db); //added by Puneet for fixing GPP crash issue 2013.06.12
            // If result is 0, means device is Index based
            if(value.compare("0") == 0)
            {
                LOG_TRACE << "CGracenoteOperationUtil::CheckIPODType Index based" << LOG_ENDL;
                isIndexBased = true;
            }
            else
            {
                LOG_TRACE << "CGracenoteOperationUtil::CheckIPODType UID based" << LOG_ENDL;
                isIndexBased = false;
            }
        }
        //db.close(); //Removed  by Puneet for fixing GPP crash issue 2013.06.12
    }

    LOG_TRACE << "CGracenoteOperationUtil::CheckIPODType Exit" << LOG_ENDL;

    return isIndexBased;
}

/*
 *******************************************************************
 * Function	: CGracenoteOperationUtil::DeleteOldTableFromIPODDB()
 *
 * Description 	: Deleting the old table from the IPOD Database
 *
 * Parameters 	: iPodSerialNum - IPOD Serial number
 *
 * Returns  	: None
 *
 *******************************************************************
 */
void CGracenoteOperationUtil::DeleteOldTableFromIPODDB(QString iPodSerialNum)
{
    LOG_TRACE << "CGracenoteOperationUtil::DeleteOldTableFromIPODDB Enter" << LOG_ENDL;

    QString IPODDBPath("/app/share/iPodDBPath/");
    IPODDBPath.append(iPodSerialNum);

    QFile dbpath(IPODDBPath);
    if(dbpath.exists())
    {
        LOG_TRACE <<("CGracenoteOperationUtil:DeleteOldTableFromIPODDB dbpath exists") << LOG_ENDL;

        // Remove the old connection
        QSqlDatabase::removeDatabase("MasterDB");
        QSqlDatabase db;
        QString connectionName;
        connectionName.append("MasterDB");
   		
        if(createConnection(IPODDBPath, connectionName, db))
        {
            QSqlQuery* query = new QSqlQuery(db);
            query->prepare("DROP TABLE OldTable");

            bool status = query->exec();
            if(!status)
            {
                LOG_TRACE <<("CGracenoteOperationUtil:DeleteOldTableFromIPODDB DROP table query failed !!!") << LOG_ENDL;
            }
            delete query;
            closeConnection(db);    //added by Puneet for fixing GPP crash issue 2013.06.12
//            db.close();   //Removed  by Puneet for fixing GPP crash issue 2013.06.12
        }

    }
    LOG_TRACE << "CGracenoteOperationUtil::DeleteOldTableFromIPODDB Exit" << LOG_ENDL;
}

//End of file
