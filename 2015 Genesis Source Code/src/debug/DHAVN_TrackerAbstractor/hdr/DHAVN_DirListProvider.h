/*
******************************************************************
*        COPYRIGHT (C) Teleca AB      *
*----------------------------------------------------------------*
* MODULE     :  DHAVN_TrackerAbstractor                             *
*
* PROGRAMMER :  chetan nanda                                  *
* DATE       :  18 November 2011                                    *
* VERSION    :  1.0                                              *
*
*----------------------------------------------------------------*
*                                                                *
* MODULE SUMMARY : This file defines class which interact with    *
*                  Tracker database using spaql query to retrieve media related    *
*                  information  (e.g Artist, Album, Genre etc)    *
*                                                                *
*----------------------------------------------------------------*
*
* MODIFICATION RECORDS :                                         *
*
* VERSION   DATE                     PROGRAMMER              COMMENT
******************************************************************
* 1.0       18 November 2011     chetan nanda           Draft version
******************************************************************
*/

#ifndef DHAVN_DIRLISTPROVIDER_H
#define DHAVN_DIRLISTPROVIDER_H

#include <QStringList>
#include <QVector>
#include <QThread>
#include "DHAVN_TrackerAbstractor_constants.h"
#include <DHAVN_LogUtil.h>
#include <QMetaType>

#define FOLDER_KEY_LIST_SIZE    3

typedef QVector<QStringList> m_veclist;
Q_DECLARE_METATYPE(QVector<QStringList>);

class CDirListProvider : public QThread
{
	Q_OBJECT
    USE_LOG_UTIL

	public :
   
    bool GetFolderTabList(QString folderPath); // added by kihyung 2013.2.8
	bool GetMusicListByDirList(QString path);
	bool GetVideoListByDirList(QString path);
	bool GetPhotoListByDirList(QString path);
	bool GetLimitedMusicListByDirList(QString path);
	bool GetLimitedVideoListByDirList(QString path);
	bool GetLimitedPhotoListByDirList(QString path);
	
	bool GetMusicListByDirList(ETrackerAbstarctor_VolumeType  volumeType);
	bool GetVideoListByDirList(ETrackerAbstarctor_VolumeType  volumeType);
	bool GetPhotoListByDirList(ETrackerAbstarctor_VolumeType  volumeType);
	bool GetLimitedMusicListByDirList(ETrackerAbstarctor_VolumeType  volumeType);
	bool GetLimitedVideoListByDirList(ETrackerAbstarctor_VolumeType  volumeType);
	bool GetLimitedPhotoListByDirList(ETrackerAbstarctor_VolumeType  volumeType);
        bool setDeviceURL(ETrackerAbstarctor_VolumeType volumeType,QString DeviceId,QString MountPath);
    /**
       * Default Destructor
       */
    ~CDirListProvider();
	CDirListProvider();
protected:
	void run();
private :			
    QString m_path;
    int m_mode;
    QStringList m_DeviceId;
    QStringList m_DeviceURL;

    // { added by kihyung 2013.2.8
    QVector<QStringList> *m_ppFolderList[FOLDER_KEY_LIST_SIZE];
    QVector<QStringList>  m_pFolderList[FOLDER_KEY_LIST_SIZE];
    QStringList           m_FolderKeyList;
    // } added by kihyung 2013.2.8
    
//		void GetDataList(QVector<QStringList> dataList, int mode);

//private slots:
//		void HandleGetDataList(QVector<QStringList> dataList, int mode);
		
public:
	void CreateFileList(QString path, int mode);  // added by kihyung 2013.2.8
    void CreateFolderList(QString folderPath); 
	void SetPath(QString str);
	void SetCreateListMode(int mode);
	int GetCreateListMode();


//		void DataFetched(QVector<QStringList> dataList, ETrackerAbstarctor_Query_Ids queryId);

	signals:
	void DataFetched(QVector<QStringList>, ETrackerAbstarctor_Query_Ids);
	 void Error(int);

};

#endif
