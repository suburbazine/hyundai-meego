/*
 ******************************************************************
 *    COPYRIGHT ?Teleca AB      *
 *----------------------------------------------------------------*
 * MODULE     	:  CApplicationInterface                             	  *
 *
 * PROGRAMMERS 	:  Vishal Jain
 * DATE       	:  15 May 2012                             	  *
 * VERSION    	:                                             	  *
 *
 *----------------------------------------------------------------*
 *                                                                *
 *  MODULE SUMMARY : CApplicationInterface supports all the interfaces
 from application to Gracenote process
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                     *
 *
 *     DATE         PROGRAMMER               COMMENT
 ******************************************************************
 * History
 * 15-May-2011      Vishal Jain               Create new class and API's
 ******************************************************************
 */

#include "DHAVN_ApplicationInterface.h"
#include "DHAVN_GracenotePluginProcessThread.h"
#include "DHAVN_GracenoteRequestQueue.h"

#include <QCoreApplication>
#include <stdio.h>

/*
 *******************************************************************
 * Function	: CApplicationInterface::CApplicationInterface
 *
 * Description 	: Constructor for CApplicationInterface

 * Parameters 	: parent - parent object, if any

 * Returns 	: None
 *
 *******************************************************************
 */
CApplicationInterface::CApplicationInterface(QString variant, QObject *parent)
{
    Q_UNUSED(parent);
    LOG_INIT_V2("Media_GracenotePluginProcess");

    m_pThread = NULL;
    m_pQueue = NULL;
    m_Variant = variant;
}

/*
 *******************************************************************
 * Function	: CApplicationInterface::~CApplicationInterface
 *
 * Description 	: Destructor for CApplicationInterface

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
CApplicationInterface::~CApplicationInterface()
{
    if(m_pQueue)
    {
        delete m_pQueue;
        m_pQueue = NULL;
    }

    if(m_pThread)
    {
        delete m_pThread;
        m_pThread = NULL;
    }
}

/*
 *******************************************************************
 * Function	: CApplicationInterface::Init
 *
 * Description 	: Initialize all the variables and connect signal, slot

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
bool CApplicationInterface::Init()
{
    LOG_TRACE << "CApplicationInterface::Init Start" << LOG_ENDL;

    bool result = false;

    m_pQueue = new CGracenoteRequestQueue;
    if(!m_pQueue)
    {
        return result;
    }

    m_pThread = new GNThread();
    if(m_pThread)
    {
        LOG_TRACE << "CApplicationInterface::Init Move to thread and start" << LOG_ENDL;

        connect(m_pThread,SIGNAL(started()), this, SLOT(onChildStarted())); //added by Puneet for fixing GPP crash issue 2013.06.12
        m_pQueue->moveToThread(m_pThread);
        m_pThread->start();
    }
    else
    {
        return result;
    }

    connect(this, SIGNAL(start(QString)), m_pQueue, SLOT(onStart(QString)), Qt::DirectConnection);

    connect(this, SIGNAL(fetchCDData(QString)), m_pQueue, SLOT(onFetchCDData(QString)), Qt::DirectConnection);
    connect(this, SIGNAL(deleteCDData(QString)), m_pQueue, SLOT(onDeleteCDData(QString)),Qt::DirectConnection);

    connect(this, SIGNAL(retrieveCoverArt(QStringList)), m_pQueue, SLOT(onRetrieveCoverArt(QStringList)),Qt::DirectConnection);

    connect(this, SIGNAL(enableMLT(int,QString,QString)), m_pQueue, SLOT(onEnableMLT(int,QString,QString)),Qt::DirectConnection);
    connect(this, SIGNAL(usbConnected(QString,QString,int)), m_pQueue, SLOT(onUsbConnected(QString,QString,int)),Qt::DirectConnection);

    connect(this, SIGNAL(ipodConnected(QString,int,int,int,int)), m_pQueue, SLOT(onIPodConnected(QString,int,int,int,int)),Qt::DirectConnection);
    connect(this, SIGNAL(highLightedVolume(int,QString,QString)), m_pQueue,
                    SLOT(onHighLightedVolume(int,QString,QString)),Qt::DirectConnection);

    connect(this, SIGNAL(usbDisconnected(QString,int)), m_pQueue, SLOT(onUsbDisconnected(QString,int)),Qt::DirectConnection);
    connect(this, SIGNAL(ipodDisConnected(QString,int)), m_pQueue, SLOT(onIPodDisConnected(QString,int)),Qt::DirectConnection);
    connect(this, SIGNAL(closeCollections()), m_pQueue, SLOT(onCloseCollections()),Qt::DirectConnection);
    connect(this, SIGNAL(retrieveMultipleCoverArts(QStringList,QStringList)), m_pQueue,
                SLOT(onRetrieveMultipleCoverArts(QStringList,QStringList)),Qt::DirectConnection);
    connect(this , SIGNAL(jukeBoxMLTSignal()),m_pQueue , SLOT(onjukeBoxMLTSignal()),Qt::DirectConnection); //added by Puneet for fixing MLT not higlited for JB/USB1/USB2 issue 2013.06.12
    //added by aettie.ji 20130904 for gracenote version
	connect(this, SIGNAL(getSDKVer()), m_pQueue, SLOT(onGetSDKVer()),Qt::DirectConnection);
    connect(this, SIGNAL(getDBVer()), m_pQueue, SLOT(onGetDBVer()),Qt::DirectConnection);
    connect(this, SIGNAL(getDBRegionVer()), m_pQueue, SLOT(onGetDBRegionVer()),Qt::DirectConnection); //20131023 System info
 //deleted by aettie 20131010 unused code
	//connect(this, SIGNAL(requestEmitSignalForGracenoteLogo()), m_pQueue, SLOT(onRequestEmitSignalForGracenoteLogo()),Qt::DirectConnection);


    connect(m_pQueue, SIGNAL(OnMusicIDCDDataReceived(bool)),
            this, SIGNAL(OnMusicIDCDDataReceived(bool)));
    connect(m_pQueue, SIGNAL(CoverArtRetrieved(bool,QString)), this, SIGNAL(CoverArtRetrieved(bool,QString)));
    connect(m_pQueue, SIGNAL(CoverArtsRetrieved(QStringList)), this, SIGNAL(CoverArtsRetrieved(QStringList)));
    connect(m_pQueue, SIGNAL(ExitFromApp()), this, SLOT(onExitFromApp()));

    connect(this, SIGNAL(resyncAfterCopyOperation(int)), m_pQueue, SLOT(onResyncAfterCopyOperation(int)), Qt::DirectConnection);


    connect(m_pQueue, SIGNAL(GracenoteIndexingStatus(bool,int)), this, SLOT(OnGracenoteIndexingStatus(bool,int)), Qt::DirectConnection);
    connect(m_pQueue, SIGNAL(GracenoteKilled()), this, SLOT(onGracenoteKilled()), Qt::DirectConnection); //added by aettie for Gracenote killed handling
    //added by aettie.ji 20130904 for gracenote version
	connect(m_pQueue, SIGNAL(GNSDKVer(QString)), this, SLOT(onGNSDKVer(QString)), Qt::DirectConnection);
    connect(m_pQueue, SIGNAL(GNDBVer(int)), this, SLOT(onGNDBVer(int)), Qt::DirectConnection);
    connect(m_pQueue, SIGNAL(GNDBRegionVer(QString)), this, SLOT(onGNDBRegionVer(QString)), Qt::DirectConnection); //20131023 System info

    connect(m_pQueue, SIGNAL(GracenoteLaunchComplete(bool)), this, SIGNAL(GracenoteLaunchComplete(bool)), Qt::DirectConnection);

    LOG_TRACE << "CApplicationInterface::Init End" << LOG_ENDL;
    return true;
}

/*
 *******************************************************************
 * Function	: CApplicationInterface::FetchMediaDataFromCDTOCLookup
 *
 * Description 	: Emit signal to Fetch data for CD
 *
 *******************************************************************
 */
void CApplicationInterface::FetchMediaDataFromCDTOCLookup(QString tocInfo)
{
    LOG_TRACE << "CApplicationInterface::FetchMediaDataFromCDTOCLookup" << LOG_ENDL;
    emit fetchCDData(tocInfo);
}

/*
 *******************************************************************
 * Function	: CApplicationInterface::DeleteMediaDataForCD
 *
 * Description 	: Emit signal to delete CD data
 *
 *******************************************************************
 */
void CApplicationInterface::DeleteMediaDataForCD(QString tocInfo)
{
    LOG_TRACE << "CApplicationInterface::DeleteMediaDataForCD" << LOG_ENDL;
    emit deleteCDData(tocInfo);
}

/*
 *******************************************************************
 * Function	: CApplicationInterface::RetrieveCoverArt
 *
 * Description 	: Emit signal to retrieveCoverArt
 *
 *******************************************************************
 */
void CApplicationInterface::RetrieveCoverArt(QStringList metaDataList)
{
    LOG_TRACE << "CApplicationInterface::RetrieveCoverArt" << LOG_ENDL;
    emit retrieveCoverArt(metaDataList);
}

/*
 *******************************************************************
 * Function	: CApplicationInterface::EnableMLTForVolume
 *
 * Description 	: Emit signal to Enable MLT for specified volume
 *
 *******************************************************************
 */
void CApplicationInterface::EnableMLTForVolume(int volume, QString serialNum,
                                               QString mountPath)
{
    LOG_TRACE << "CApplicationInterface::EnableMLTForVolume" << LOG_ENDL;
    emit enableMLT(volume, serialNum, mountPath);
}

	//added by Puneet for fixing MLT not higlited for JB/USB1/USB2 issue 2013.06.12
void CApplicationInterface::jukeBoxMLT()
{
    LOG_TRACE << "CApplicationInterface::jukeBoxMLT" << LOG_ENDL;
    emit jukeBoxMLTSignal();
}

/*
 *******************************************************************
 * Function	: CApplicationInterface::USBConnected
 *
 * Description 	: Emit signal to indicate USB is connected
 *
 *******************************************************************
 */
void CApplicationInterface::USBConnected(QString serialNum, QString mountPath, int volume)
{
    LOG_TRACE << "CApplicationInterface::USBConnected" << " " << serialNum << " " << mountPath << LOG_ENDL;
    emit usbConnected(serialNum, mountPath, volume);
}

/*
 *******************************************************************
 * Function	: CApplicationInterface::IPODConnected
 *
 * Description 	: Emit signal to indicate IPOD is connected
 *
 *******************************************************************
 */
void CApplicationInterface::IPODConnected(QString serialNum, int startIndex, int endIndex,
                   int syncState, int volume)
{
    LOG_TRACE << "CApplicationInterface::IPODConnected " <<LOG_ENDL;
    emit ipodConnected(serialNum, startIndex, endIndex,
                       syncState, volume);
}

/*
 *******************************************************************
 * Function	: CApplicationInterface::HighLightedVolume
 *
 * Description 	: Emit signal to indicate change in the highlighted
                           volume
 *
 *******************************************************************
 */
void CApplicationInterface::HighLightedVolume(int volume, QString serialNum, QString mountPath)
{
    LOG_TRACE << "CApplicationInterface::HighLightedVolume= " << volume << LOG_ENDL;
    emit highLightedVolume(volume, serialNum, mountPath);
}

/*
 *******************************************************************
 * Function	: CApplicationInterface::USBDisConnected
 *
 * Description 	: Emit signal to indicate USB is disconnected
 *
 *******************************************************************
 */
void CApplicationInterface::USBDisConnected(QString serialNum, int volume)
{
    LOG_TRACE << "CApplicationInterface::USBDisConnected" << LOG_ENDL;
    emit usbDisconnected(serialNum, volume);
}
/*
 *******************************************************************
 * Function	: CApplicationInterface::IPODDisConnected
 *
 * Description 	: Emit signal to indicate IPOD is disconnected
 *
 *******************************************************************
 */
void CApplicationInterface::IPODDisConnected(QString serialNum, int volume)
{
    LOG_TRACE << "CApplicationInterface::IPODDisConnected" << LOG_ENDL;
    emit ipodDisConnected(serialNum, volume);
}

/*
 *******************************************************************
 * Function	: CApplicationInterface::CloseCollectionIDs
 *
 * Description 	: Emit signal to close all the collections
 *
 *******************************************************************
 */
void CApplicationInterface::CloseCollectionIDs()
{
    LOG_TRACE << "CApplicationInterface::CloseCollectionIDs()" << LOG_ENDL;

    //BackupXsampaToFS(); //deleted by aettie Xsampa DB is not used.

    emit closeCollections();

    LOG_TRACE << "CApplicationInterface::CloseCollectionIDs() Exit" << LOG_ENDL;
}

/*
 *******************************************************************
 * Function	: CApplicationInterface::CloseCollectionIDs
 *
 * Description 	: Emit signal to retrieve cover art for various songs
 *
 *******************************************************************
 */
void CApplicationInterface::RetrieveMultipleCoverArts(QStringList albumList, QStringList artistList)
{
    LOG_TRACE << "CApplicationInterface::RetrieveMultipleCoverArts" << LOG_ENDL;
    emit retrieveMultipleCoverArts(albumList, artistList);
}

/*
 *******************************************************************
 * Function	: CApplicationInterface::onChildStarted
 *
 * Description 	: This slot will be hit, once the child thread starts
 *                        accordingly start signal should be emitted
 *
 *******************************************************************
 */
void CApplicationInterface::onChildStarted()
{
    LOG_TRACE << "CApplicationInterface::onChildStarted" << LOG_ENDL;
    //CopyXSampaToTmpfs(); //deleted by aettie Xsampa DB is not used.

    emit start(m_Variant);
}
//{added by aettie.ji 20130904 for gracenote version
void CApplicationInterface::GetSDKVer()
{
    LOG_TRACE << "CApplicationInterface::GetSDKVer" << LOG_ENDL;

    emit getSDKVer();
}

void CApplicationInterface::GetDBVer()
{
    LOG_TRACE << "CApplicationInterface::GetDBVer" << LOG_ENDL;

    emit getDBVer();
}
//20131023 System info
void CApplicationInterface::GetDBRegionVer()
{
    LOG_TRACE << "CApplicationInterface::GetDBRegionVer" << LOG_ENDL;

    emit getDBRegionVer();
}

//}added by aettie.ji 20130904 for gracenote version
 //deleted by aettie 20131010 unused code
//{added by aettie.ji 20130904 for gracenote logo
//void CApplicationInterface::RequestEmitSignalForGracenoteLogo()
//{
  //  LOG_TRACE << "CApplicationInterface::RequestEmitSignalForGracenoteLogo" << LOG_ENDL;

    //emit requestEmitSignalForGracenoteLogo();
//}
//}added by aettie.ji 20130904 for gracenote logo
/*
 *******************************************************************
 * Function	: CApplicationInterface::CopyXSampaToTmpfs
 *
 * Description 	: Copy XSampa data from File system to TMPFS
 *
 *******************************************************************
 *
//deleted by aettie Xsampa DB is not used.
void CApplicationInterface::CopyXSampaToTmpfs()
{
    LOG_TRACE << " CApplicationInterface::CopyXSampaToTmpfs() Enter" << LOG_ENDL;

//    QString xsampaFSName = "/gracenote/XSampaDB";
    QString xsampaFSName = "/app/data/gracenote/XSampaDB"; //modified by aettie.ji 2013.05.01 for Gracenote DB location change
    QString xsampaTmpfsName = "/dev/shm/XSampaDB";

    if(QFile::exists(xsampaTmpfsName))
    {
        LOG_TRACE << " CApplicationInterface :: Removing tmpfs file" << LOG_ENDL;
        QFile::remove(xsampaTmpfsName);
    }

    if(QFile::exists(xsampaFSName))
    {
        LOG_TRACE << " CApplicationInterface :: copying FS file to tmpfs " << LOG_ENDL;
        QFile::copy(xsampaFSName, xsampaTmpfsName);        
    }

    // Make sure that file copy is complete
    sync();

    LOG_TRACE << " CApplicationInterface::CopyXSampaToTmpfs() Exit" << LOG_ENDL;
    
}*/

/*
 *******************************************************************
 * Function	: CApplicationInterface::BackupXsampaToFS
 *
 * Description 	: Take backup of XSampa database to File system
 *
 *******************************************************************
 *
 //deleted by aettie Xsampa DB is not used.
void CApplicationInterface::BackupXsampaToFS()
{
   LOG_TRACE << " CApplicationInterface::BackupXsampaToFS() Exit" << LOG_ENDL;

    QString xsampaFSName = "/app/data/gracenote/XSampaDB"; //modified by aettie.ji 2013.05.01 for Gracenote DB location change
    QString xsampaTmpfsName = "/dev/shm/XSampaDB";

    if(QFile::exists(xsampaFSName))
    {
        LOG_TRACE << "CApplicationInterface :: Removing FS file " << LOG_ENDL;
        QFile::remove(xsampaFSName);
    }

    if(QFile::exists(xsampaTmpfsName))
    {
        LOG_TRACE << "CApplicationInterface :: copying TMPFS file to FS " << LOG_ENDL;
        QFile::copy(xsampaTmpfsName, xsampaFSName);        
    }

    // Make sure that file copy is complete
    sync();

    LOG_TRACE << " CApplicationInterface::BackupXsampaToFS() Exit" << LOG_ENDL;
    
}*/

/*
 *******************************************************************
 * Function	: CApplicationInterface::onXSampaDataUpdated
 *
 * Description 	: Slot hit, once XSampa data is updated
 *
 *******************************************************************
 *
 //deleted by aettie Xsampa DB is not used.
//{modified by esjang 2012.11.14 for XSampa DB update with Volume Info
//void CApplicationInterface::onXSampaDataUpdated()
void CApplicationInterface::onXSampaDataUpdated(int volume)
//}modified by esjang 2012.11.14 for XSampa DB update with Volume Info
{    
    //LOG_TRACE << " CApplicationInterface::onXSampaDataUpdated() Enter" << volume <<LOG_ENDL;
    //BackupXsampaToFS();

    //{modified by esjang 2012.11.14 for XSampa DB update with Volume Info
    //emit XSampaDataUpdated();
    //emit XSampaDataUpdated(volume);
    //}modified by esjang 2012.11.14 for XSampa DB update with Volume Info
    
    //LOG_TRACE << " CApplicationInterface::onXSampaDataUpdated() Exit" << LOG_ENDL;
}*/

/*
 *******************************************************************
 * Function	: CApplicationInterface::onExitFromApp
 *
 * Description 	: Slot hit, once exit from application signal is emitted
 *
 *******************************************************************
 */
void CApplicationInterface::onExitFromApp()
{
    if(m_pThread)
    {
        m_pThread->quit();
    }

    QCoreApplication::quit();
}

/*
 *******************************************************************
 * Function	: CApplicationInterface::ResyncAfterCopyOperation
 *
 * Description 	: emit signal for performing resync after copy operation.
 *
 *******************************************************************
 */
void CApplicationInterface::ResyncAfterCopyOperation(int targetVolume)
{
    LOG_TRACE << "CApplicationInterface::ResyncAfterCopyOperation" << LOG_ENDL;
    emit resyncAfterCopyOperation(targetVolume);
}

void CApplicationInterface::OnGracenoteIndexingStatus(bool status, int deviceType)
{
    LOG_TRACE << "CApplicationInterface::OnGracenoteIndexingStatus" << LOG_ENDL;
    LOG_TRACE << "Status : " << status << "DeviceType : " << deviceType << LOG_ENDL;

    emit GracenoteIndexingStatus(status, deviceType);

    LOG_TRACE << "CApplicationInterface::OnGracenoteIndexingStatus Exit" << LOG_ENDL;
}
//{added by aettie for Gracenote killed handling
void CApplicationInterface::onGracenoteKilled()
{
    LOG_TRACE << "CApplicationInterface::onGracenoteKilled" << LOG_ENDL;

    emit GracenoteKilled();

    LOG_TRACE << "CApplicationInterface::onGracenoteKilled Exit" << LOG_ENDL;

}
//{added by aettie.ji 20130904 for gracenote version
void CApplicationInterface::onGNSDKVer(QString ver)
{
    LOG_TRACE << "CApplicationInterface::onGNSDKVer" << LOG_ENDL;

    emit GNSDKVer(ver);

    LOG_TRACE << "CApplicationInterface::onGNSDKVer Exit" << LOG_ENDL;

}
void CApplicationInterface::onGNDBVer(int ver)
{
    LOG_TRACE << "CApplicationInterface::onGNDBVer" << LOG_ENDL;

    emit GNDBVer(ver);

    LOG_TRACE << "CApplicationInterface::onGNDBVer Exit" << LOG_ENDL;

}
//20131023 System info
void CApplicationInterface::onGNDBRegionVer(QString ver)
{
    LOG_TRACE << "CApplicationInterface::onGNDBRegionVer" << LOG_ENDL;

    emit GNDBRegionVer(ver);

    LOG_TRACE << "CApplicationInterface::onGNDBRegionVer Exit" << LOG_ENDL;

}
//}added by aettie.ji 20130904 for gracenote version
// End of file

