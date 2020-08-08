/*
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 * History
 * 01-Aug-2011      Divya Dsouza
 ******************************************************************
 */

#ifndef DHAVN_GRACENOTEOPERATIONUTIL_H
#define DHAVN_GRACENOTEOPERATIONUTIL_H

#define GNVList QVector<QStringList>

#include <QObject>
#include <QSqlDatabase>
#include <gn_textid_file_data.h>
#include <gn_abs_types.h>
#include <gn_textid_lookup.h>
#include <QStringList>
#include <QVector>
#include <DHAVN_LogUtil.h>
//added by Puneet for fixing GPP crash issue 2013.06.12
#include <QSemaphore>
#include <QMutex>
#include <QMutexLocker>


enum EXSampaType
{
    eAlbumType = 0,
    eArtistType,
    eGenreType
};

/**
  * This is a helper class for TextID based ELDB lookup.
  * Has functions to parse file name path and extract ID3 information
  * from media files.
  */
class CGracenoteOperationUtil : public QObject
{
    Q_OBJECT
    USE_LOG_UTIL

public:
    CGracenoteOperationUtil(QObject *parent = 0);
    ~CGracenoteOperationUtil();

public:

    void ParseFilePath(QString filePath, gn_pfile_data_t &fileData);

    int ExtractID3Information(QString filePath, gn_pfile_data_t &fileData);

    QString GetDatabasePathForRegion(QString countryVariant);

    QString encodeString(gn_uchar_t* inputString);    

    bool isUtf8(const char * str );

    bool SetIPODID3Info(QStringList list, gn_pfile_data_t &fileData);

    bool SetID3InfoForCD(QStringList cdDataList, gn_pfile_data_t &fileData);
//deleted by aettie Xsampa DB is not used.
    //void GetXSampaDataForAlbum(QString albumName, QString albumID3Name, gn_textid_presult_data_t resultData);
    //void GetXSampaDataForArtist(QString artistName, QString artistID3Name, gn_textid_presult_data_t resultData);

    //void GetXSampaList(GNVList& phoneticData);
    /**Reads the maximum number of MLDBs to be stored in the system for USB and IPOD
       *from the configuration file, in which the user sets the desired numbers.
       */
    bool GetMLDBConfiguration();
    QStringList GetTextIDCacheInfoFomConfig();
	
    GNVList ReadDataFromIPODSqlDB(QString iPODSerialNum,
                                                          int startIndex,
                                                          int endIndex, bool allData = false);

    void GetResyncData(QString iPodSerialNum, GNVList& newList,
                                    GNVList& modifiedList,GNVList& deletedList);

    void PrepareDataForIndexBased(QString iPodSerialNum, GNVList& newList,
                                                        GNVList& modifiedList,GNVList& deletedList);

    void PrepareDataForUIDBased(QString iPodSerialNum, GNVList& newList,
                                                    GNVList& deletedList);

    GNVList ReadResyncDataForIndexBased(QString iPODSerialNum,
                                           bool oldTable);

    GNVList ReadResyncDataForUIDBased(QString iPODSerialNum, bool newEntries);

    bool CheckIPODType(QString iPodSerialNum);

    void DeleteOldTableFromIPODDB(QString iPodSerialNum);

private:
    void ProcessRepresentation(gn_prepresentation_t rep);

    void ProcessTranscriptionArray(
            const gn_uchar_t* orthography,
            const gn_ptranscript_arr_t	transcriptionArray);
	//modified by aettie for Static Defect 20131007
    void ProcessTranscription(const gn_uchar_t* orthography, const gn_ptranscript_t transcription);
	//added by Puneet for fixing GPP crash issue 2013.06.12
    bool createConnection(QString dbPath, QString connectionName, QSqlDatabase &dBHandle);
    void closeConnection(QSqlDatabase &dBHandle); //by aettie for Static Test Defect 201300711
private:
    gn_spklangid_t m_LanguageId;
    GNVList m_XSData;
    QString m_TypeID3Name;
    QString m_TypeName;
    QString m_AlternatePhrase;
    EXSampaType m_Type;
    QSemaphore sem; //added by Puneet for fixing GPP crash issue 2013.06.12
};

#endif // DHAVN_GRACENOTEOPERATIONUTIL_H
