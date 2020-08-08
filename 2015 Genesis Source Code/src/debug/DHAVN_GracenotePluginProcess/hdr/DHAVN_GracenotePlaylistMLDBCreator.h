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

#ifndef DHAVN_GRACENOTEPLAYLISTMLDBCREATOR_H
#define DHAVN_GRACENOTEPLAYLISTMLDBCREATOR_H

#include <QString>
#include <QObject>

#include <gn_abs_errors.h>
#include <gn_playlist_types.h>

#include <DHAVN_LogUtil.h>
#include <QDateTime>
#include "DHAVN_NormalizedDataUtil.h" //added by Divya 2012.01.07 for gracenote normalized db
//#include "DHAVN_GracenoteInterface.h" //added by aettie 20130518 ISV 83208
//added by Puneet for fixing GPP crash issue 2013.06.12
#include <QSemaphore>
#include <QMutex>
#include <QMutexLocker>


/**
  * This is a class for performing various operations which help in
  * getting the Collection ID created for a particular volume, Registering
  * a Collection ID, Populating the MLDB, Opening and Closing the
  * Collection IDs when the volume is unplugged, Modifying the MLDB entry.
  */
class CPlaylistMLDBCreator : public QObject
{
    Q_OBJECT
    USE_LOG_UTIL

public :
    CPlaylistMLDBCreator(QObject *parent = 0);
    ~CPlaylistMLDBCreator();

    bool Init();
    //void SetGracenoteInterface(CGracenoteInterface* aInterface); //added by aettie 20130518 ISV 83208

    unsigned long RegisterCollectionIDForVolumeObject(int volume, QString serialNum = QString());
    unsigned long RetrieveCollectionIDforVolume(int volume, QString serialNum = QString());
    gn_error_t PopulateCollectionID(gn_collection_id_t CID, gnplaylist_entrydata_t entrydata,
                                                      QString filePath);

    int DeleteMusicFileEntryInCollectionID(int CID, QString filePath);

    gn_error_t CloseCollectionID(int CID);
    void CloseCollections();
    gn_error_t OpenCollectionID(int CID);
    void DeleteCollectionID(int CID);
	 
    bool EnableForPlaylisting(int volume, QString serialNum, QString mountPath);
    void DisableAllCollectionsForPlaylisting();

    int GetConnectStatus(QString serialNum = QString());

    void UpdateConnectStatus(int ConnectStatus, int nVolume = -1, QString serialNum = QString()); //modified by aettie 2013.03.22 for ITS 148383
    void UpdateMountPathInDB(QString mountPath, int nCID );

    void DeleteLeastRecentlyUsedMLDB(QList<int> CIDList, QList<QDateTime> accessDates, int nVolume);
    void MLDBMaintenance(int nVolume );

    void SetMLDBLocation(QString mldbLocation);
    void CopyMLDBToRAM(int collectionId);
    void BackupMLDBOnFS(int collectionId);

    void UpdateMLDBState(int state, QString serialNum = QString());
    int GetMLDBState(QString serialNum = QString(), QString mountPath = QString());
	
	void DeleteIPODEntryFromMLDB(int collectionId, QString fileName, QString filePath);
    void UpdateIPODEntryFromMLDB(int collectionId, QString fileName, QString filePath);

private:
    void LogError(gn_error_t error);

private:
     bool m_isDbValid;
     QString m_MLDBLocation;
     CNormalizedData *m_NormalizedData; //added by Divya 2012.01.07 for gracenote normalized db
     //CGracenoteInterface *m_pGracenoteInterface; //added by aettie 20130518 ISV 83208
};

#endif // DHAVN_GRACENOTEPLAYLISTMLDBCREATOR_H
