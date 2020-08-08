/*
 ******************************************************************
 *    COPYRIGHT ?Teleca AB      *
 *----------------------------------------------------------------*
 * MODULE     	:  CGracenoteInterface                            	  *
 *
 * PROGRAMMERS 	:  GADDAM SATEESH
 * DATE       	:  27 April 2012                             	  *
 * VERSION    	:                                             	  *
 *
 *----------------------------------------------------------------*
 *                                                                *
 *  MODULE SUMMARY : CQueue interface makes all the requests of
 *  Juke,USB,IPOD will be enqueued and dequeued on prority base
 *----------------------------------------------------------------*
  ******************************************************************
 */
#include <QQueue>
#include <QDebug>
#include "DHAVN_TrackerQuery.h"
#include "DHAVN_GracenoteCDLookup.h"
#include "DHAVN_GracenoteCoverArt.h"
#include "DHAVN_GracenoteRequestQueue.h"

#include <QDBusConnection>
#include <cdbuswrapper.h>

extern int HighlightedVolume;

/*
 *******************************************************************
 * Function	: CGracenoteRequestQueue::CGracenoteRequestQueue
 *
 * Description 	: constructer registers the dequeue slot

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
 //{deleted all "LOG statement" by aettie.ji 2013.02.22 for ISV 71131 etc.
CGracenoteRequestQueue::CGracenoteRequestQueue()
{
   //LOG_INIT_V2("Media_GracenotePluginProcess"); //deleted by aettie CLOGUtil fault in GNTread

    QString dummyPath;
    m_pGracenoteInterface = new CGracenoteInterface(dummyPath);

    connect(this,SIGNAL(DeQueueStart()), this, SLOT(KeepOnDequeue()), Qt::QueuedConnection );

    connect(m_pGracenoteInterface, SIGNAL(OnMusicIDCDDataReceived(bool)),
            this, SIGNAL(OnMusicIDCDDataReceived(bool)));
    connect(m_pGracenoteInterface, SIGNAL(CoverArtRetrieved(bool,QString)), this, SIGNAL(CoverArtRetrieved(bool,QString)));
    connect(m_pGracenoteInterface, SIGNAL(CoverArtsRetrieved(QStringList)), this, SIGNAL(CoverArtsRetrieved(QStringList)));
    //{modified by esjang 2012.11.14 for XSampa DB update with Volume Info
    //connect(m_pGracenoteInterface, SIGNAL(XSampaDataUpdated()), this, SIGNAL(XSampaDataUpdated()));
    //connect(m_pGracenoteInterface, SIGNAL(XSampaDataUpdated(int)), this, SIGNAL(XSampaDataUpdated(int))); //deleted by aettie Xsampa DB is not used.
    //}modified by esjang 2012.11.14 for XSampa DB update with Volume Info
    connect(m_pGracenoteInterface, SIGNAL(GracenoteIndexingStatus(bool,int)), this, SIGNAL(GracenoteIndexingStatus(bool,int)), Qt::DirectConnection);
    //{added by aettie for Gracenote killed handling
    connect(m_pGracenoteInterface, SIGNAL(GracenoteKilled()), this, SIGNAL(GracenoteKilled()), Qt::DirectConnection);
	//added by aettie.ji 20130904 for gracenote version
    connect(m_pGracenoteInterface, SIGNAL(GNSDKVer(QString)), this, SIGNAL(GNSDKVer(QString)), Qt::DirectConnection);
    connect(m_pGracenoteInterface, SIGNAL(GNDBVer(int)), this, SIGNAL(GNDBVer(int)), Qt::DirectConnection);
    connect(m_pGracenoteInterface, SIGNAL(GNDBRegionVer(QString)), this, SIGNAL(GNDBRegionVer(QString)), Qt::DirectConnection); //20131023 System info

}


/*
 *******************************************************************
 * Function	: CGracenoteRequestQueue::~CGracenoteRequestQueue
 *
 * Description 	: Destructer

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
CGracenoteRequestQueue::~CGracenoteRequestQueue()
{
    if(m_pGracenoteInterface)
    {
        delete m_pGracenoteInterface;
        m_pGracenoteInterface = NULL;
    }
}

/*
 *******************************************************************
 * Function	: CGracenoteRequestQueue::Enqueue
 *
 * Description 	: Enqueue's the request Jukebox,USB,IPOD

 * Parameters 	: type - volme type and its priority

 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteRequestQueue::Enqueue(EVolumeType type,Priority priority)
{
   //LOG_INFO << "Queue::Enqueue Enter" << LOG_ENDL;

  // LOG_TRACE << "Queue::Enqueue Enter" << QThread::currentThreadId() << LOG_ENDL;

    Event queueEvent(type, priority);
    for(int i = 0 ; i < m_RequestQueue.count() ; i++ )
    {
        const Event& ExistingEvent = m_RequestQueue[i];
        if( priority > ExistingEvent.ReqPriority )
        {
            m_RequestQueue.insert(i,queueEvent);
            //LOG_INFO << "CGracenoteRequestQueue::inserted" << LOG_ENDL;
            emit DeQueueStart();
            return;
        }
    }
    //LOG_INFO << "CGracenoteRequestQueue::appended" << LOG_ENDL;
    m_RequestQueue.append(queueEvent);
    emit DeQueueStart();

  // LOG_INFO << "Queue::Enqueue Exit" << LOG_ENDL;
}


/*
 *******************************************************************
 * Function	: CGracenoteRequestQueue::HVDequeue
 *
 * Description 	: Dequeue's only the highlighted volume

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteRequestQueue::HVDequeue()
{
   // LOG_INFO << "Queue::HVDequeue Enter" << LOG_ENDL;

    for(int i = 0; i <m_RequestQueue.count(); i++)
    {
        //LOG_INFO << "Queue::HVDequeue" << i << LOG_ENDL;

        //LOG_INFO << "DEQUEUED HighlightedVolume Entry1" << i << m_RequestQueue.count() << HighlightedVolume << m_RequestQueue.at(i).ReqType;
        if(HighlightedVolume == m_RequestQueue.at(i).ReqType)
        {
            //LOG_INFO << "Queue::HVDequeue HV volume is same as request type" << LOG_ENDL;

	        const Event dequeueEvent = m_RequestQueue.at(i);
            m_RequestQueue.removeAt(i);
	        //LOG_INFO << "dequeueEvent != NULL" << LOG_ENDL;
            PopulateMLDB(dequeueEvent.ReqType);
           //LOG_INFO << "Queue::PopulateMLDB() called" << LOG_ENDL;
            HVDequeue();
            //LOG_INFO << "Queue::HVDequeue() called" << LOG_ENDL;
        }
    }

    //LOG_INFO << "Queue::HVDequeue Exit" << LOG_ENDL;
}


/*
 *******************************************************************
 * Function	: CGracenoteRequestQueue::RemoveItem
 *
 * Description 	: Dequeue's only the highlighted volume

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteRequestQueue::RemoveItem(int type)
{
    //LOG_INFO << "Queue::RemoveItem Enter" << LOG_ENDL;

    for(int i = 0; i <m_RequestQueue.count(); i++)
    {
        if(type == m_RequestQueue.at(i).ReqType)
        {
            m_RequestQueue.removeAt(i);
        }
    }

    //LOG_INFO << "Queue::RemoveItem Exit" << LOG_ENDL;
}


/*
 *******************************************************************
 * Function	: CGracenoteRequestQueue::Count
 *
 * Description 	: holds the number of requests in CGracenoteRequestQueue

 * Parameters 	: None

 * Returns 	: number of requests in CGracenoteRequestQueue
 *
 *******************************************************************
 */
int CGracenoteRequestQueue::Count()
{
    return m_RequestQueue.count();
}


/*
 *******************************************************************
 * Function	: CGracenoteRequestQueue::KeepOnDequeue
 *
 * Description 	: Checks for queue and dequeue's the same

 * Parameters 	: Volume type

 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteRequestQueue::KeepOnDequeue()
{
    //LOG_INFO << "CGracenoteRequestQueue::KeepOnDequeue:Entry" << LOG_ENDL;
    if(!(m_RequestQueue.isEmpty()))
    {
        //LOG_INFO << "KeepOnDequeue->HighlightedVolume" << HighlightedVolume <<  LOG_ENDL;

        HVDequeue();
        while(m_RequestQueue.count() > 0)
        {
            //LOG_INFO << "DEQUEUED NOT HighlightedVolume Entry" << LOG_ENDL;
            const Event dequeueReq = m_RequestQueue.dequeue();
            PopulateMLDB(dequeueReq.ReqType);
        }
        //LOG_INFO << "DEQUEUED NOT HighlightedVolume Exit" << LOG_ENDL;
    }
    //LOG_INFO << "CGracenoteRequestQueue::KeepOnDequeue:Exit" << LOG_ENDL;
}


/*
 *******************************************************************
 * Function	: CGracenoteRequestQueue::PopulateMLDB
 *
 * Description 	: Invokes respective dequeued request to
                          populate MLDB

 * Parameters 	: Volume type

 * Returns 	: None
 *
 *******************************************************************
 */
bool CGracenoteRequestQueue::PopulateMLDB(EVolumeType type)
{
    switch(type)
    {
    case e_JukeBox:
        {
            //LOG_INFO << "Started JukeBox and HighlightedVolume" << HighlightedVolume << LOG_ENDL;
//deleted by aettie.ji 20130904 for gracenote logo
            HighlightedVolume = e_JukeBox;

            m_pGracenoteInterface->StartJukeboxPopulation();

           // LOG_INFO << "Finished JukeBox and m_RequestQueue.count() : " << m_RequestQueue.count() << LOG_ENDL;
//deleted by aettie.ji 20130904 for gracenote logo

            break;
        }
    case e_USB1:
        {
            //LOG_INFO << "Started USB1 and HighlightedVolume : " << HighlightedVolume << LOG_ENDL;
//deleted by aettie.ji 20130904 for gracenote logo
            m_pGracenoteInterface->StartUSBPopulation(e_USB1);
            //LOG_INFO << "Finished USB1 and m_RequestQueue.count()" << m_RequestQueue.count() << LOG_ENDL;


//deleted by aettie.ji 20130904 for gracenote logo
            break;
        }
    case e_USB2:
        {
            //LOG_INFO << "Started USB2 and HighlightedVolume : " << HighlightedVolume << LOG_ENDL;
//deleted by aettie.ji 20130904 for gracenote logo


            m_pGracenoteInterface->StartUSBPopulation(e_USB2);
            //LOG_INFO << "Finished USB2 and m_RequestQueue.count() : " << m_RequestQueue.count() << LOG_ENDL;


//deleted by aettie.ji 20130904 for gracenote logo
            break;
        }
	
    case e_Disc:
        {
            //LOG_INFO << "CGracenoteCoverArt Finished e_Disc and m_RequestQueue.count() : " << m_RequestQueue.count() << LOG_ENDL;
//deleted by aettie.ji 20130904 for gracenote logo
			m_pGracenoteInterface->PerformCDLookupForTOC();
//deleted by aettie.ji 20130904 for gracenote logo
            //LOG_INFO << "CGracenoteCoverArt Finished e_Disc and m_RequestQueue.count() : " << m_RequestQueue.count() << LOG_ENDL;
            break;
        }
    /*case e_Disc_EJECT:
        {
            LOG_INFO << " Started e_Disc_EJECT : "  << LOG_ENDL;

            QString tmpToc;
            tmpToc = GetCDToc();
            m_pGracenoteInterface->DeleteMediaDataForCD(tmpToc);
            LOG_INFO << "aettiequeue Finished e_Disc_EJECT  : "  << LOG_ENDL;
            break;
        }*/
    case e_CA:
        {
            //LOG_INFO << "CGracenoteCoverArt Finished e_CA and m_RequestQueue.count() : " << m_RequestQueue.count() << LOG_ENDL;
            m_pGracenoteInterface->RetrieveCA();
            //LOG_INFO << "CGracenoteCoverArt Finished e_CA and m_RequestQueue.count() : " << m_RequestQueue.count() << LOG_ENDL;
            break;
        }
    case e_MultiCA:
        {
            //LOG_INFO << "CGracenoteCoverArt Finished e_MultiCA and m_RequestQueue.count() : " << m_RequestQueue.count() << LOG_ENDL;
            m_pGracenoteInterface->RetrieveMultipleCA();
            //LOG_INFO << "CGracenoteCoverArt Finished e_MultiCA and m_RequestQueue.count() : " << m_RequestQueue.count() << LOG_ENDL;
            break;
        }
//}modified by aettie 2013.03.19 for Gracenote issue
    default:
        break;
    }
    return true;
}

/*
 *******************************************************************
 * Function	: Queue::CheckIfRequestExistsInQueue
 *
 * Description 	: Checks if the request exists in queue

 * Parameters 	: EVolumeType : Volume Type

 * Returns 	: Bool, True is exists. False if absent
 *
 *******************************************************************
 */
bool CGracenoteRequestQueue::CheckIfRequestExistsInQueue(EVolumeType type)
{
    for(int i = 0; i <m_RequestQueue.count(); i++)
    {
        if(type == m_RequestQueue.at(i).ReqType)
        {
            return true;
        }
    }

    return false;
}

/*
 *******************************************************************
 * Function	: Queue::onStart
 *
 * Description 	: Register the MLT dbus connection and start the
                          processing for Jukebox

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteRequestQueue::onStart(QString variant)
{
    //LOG_TRACE <<"CGracenoteRequestQueue::onStart" << QThread::currentThreadId() << LOG_ENDL; //deleted by aettie CLOGUtil fault in GNTread

    bool status = m_pGracenoteInterface->Init();
    if(status)
    {
        status = m_pGracenoteInterface->EMMSInit(variant);

        if(status)
        {
            m_pGracenoteInterface->SetQueueInterface(this);
			//temporally deleted for gracenote indexing spec out 2013 07 26 
            //m_pGracenoteInterface->Start();
        }
        else
        {
            // Start timer to exit the application
            QTimer::singleShot(1000, this, SLOT(onTimerFinished()));
        }
    }

    //Emit signal to notify AVP about Gracenote launching complete
    emit GracenoteLaunchComplete(status);
    //LOG_TRACE <<"Queue::onStart End" << LOG_ENDL;
}

/*
void CGracenoteRequestQueue::SetCDToc(QString toc, int volume)
{
    LOG_INFO << " Queue::SetCDToc() " <<toc<< LOG_ENDL;
    m_cdToc = toc;    
}
QString CGracenoteRequestQueue::GetCDToc()
{
    return m_cdToc;
}*/

/*
 *******************************************************************
 * Function	: Queue::onFetchCDData
 *
 * Description 	: Call  API to Fetch CD data
 *******************************************************************
 */
void CGracenoteRequestQueue::onFetchCDData(QString tocInfo)
{
    qDebug() << "CGracenoteRequestQueue::onFetchCDData";
    m_pGracenoteInterface->FetchMediaDataFromCDTOCLookup(tocInfo);
}

/*
 *******************************************************************
 * Function	: Queue::onDeleteCDData
 *
 * Description 	: Call API to delete CD data
 *******************************************************************
 */
/*void CGracenoteRequestQueue::onDeleteCDData(QString tocInfo)
{
    SetCDToc(tocInfo, e_Disc_EJECT);
    m_pGracenoteInterface->EnqueueDeleteMediaDataForCD(tocInfo);
    //HighlightedVolume = e_Disc_EJECT;
    //Enqueue(e_Disc_EJECT, DISC);
}*/
void CGracenoteRequestQueue::onDeleteCDData(QString tocInfo)
{
    m_pGracenoteInterface->DeleteMediaDataForCD(tocInfo);
}
/*
 *******************************************************************
 * Function	: Queue::onRetrieveCoverArt
 *
 * Description 	: Call API to retrieve cover art
 *******************************************************************
 */
void CGracenoteRequestQueue::onRetrieveCoverArt(QStringList metaDataList)
{
    m_pGracenoteInterface->RetrieveCoverArt(metaDataList);
}

/*
 *******************************************************************
 * Function	: Queue::onEnableMLT
 *
 * Description 	: Call API to enable MLT for specified volume
 *******************************************************************
 */
void CGracenoteRequestQueue::onEnableMLT(int volume, QString serialNum,QString mountPath)
{
    //LOG_TRACE <<"Queue::onEnableMLT Slot Enter" << LOG_ENDL;
    m_pGracenoteInterface->EnableMLTForVolume(volume, serialNum, mountPath);


    //LOG_TRACE <<"Queue::onEnableMLT Slot Exit" << LOG_ENDL;
}

//added by Puneet for MLT not highlighting for JB issue 2013.06.12
void CGracenoteRequestQueue::onjukeBoxMLTSignal()
{
///temporally deleted for gracenote indexing spec out 2013 07 26 
    //m_pGracenoteInterface->Start();
}
/*
 *******************************************************************
 * Function	: Queue::onUsbConnected
 *
 * Description 	: Call API to indicate USB connected
 *******************************************************************
 */
 void CGracenoteRequestQueue::onUsbConnected(QString serialNum, QString mountPath, int volume)
{
    qDebug() << "CGracenoteRequestQueue::onUsbConnected";
    //LOG_TRACE <<"Queue::onUsbConnected Slot Enter" << LOG_ENDL;
//temporally deleted for gracenote indexing spec out 2013 07 26 
    //m_pGracenoteInterface->USBConnected(serialNum, mountPath, volume);
    //LOG_TRACE <<"Queue::onUsbConnected Slot Exit" << LOG_ENDL;
}
/*void CGracenoteRequestQueue::onUsbConnected(QString serialNum, QString mountPath, int volume)
{
    LOG_INFO << "aettiequeue Queue::onUsbConnected() called" << LOG_ENDL;
    SetUSBConnectedInfo(serialNum, mountPath, volume);
    //m_pGracenoteInterface->USBConnected(serialNum, mountPath, volume);
    m_pGracenoteInterface->EnqueueUSBConnected(serialNum, mountPath, volume);
    LOG_TRACE <<"Queue::onUsbConnected Slot Exit" << LOG_ENDL;
}
    void CGracenoteRequestQueue::SetUSBConnectedInfo(QString serialNum, QString mountPath, int volume)
    {
            LOG_INFO << "aettiequeue Queue::SetUSBConnectedInfo() " <<serialNum<< LOG_ENDL;
            LOG_INFO << "aettiequeue Queue::SetUSBConnectedInfo() " <<mountPath<< LOG_ENDL;
            LOG_INFO << "aettiequeue Queue::SetUSBConnectedInfo() " <<volume<< LOG_ENDL;
        if (volume == e_USB1)
        {
            m_USBInfo01.serialNum = serialNum;
            m_USBInfo01.mountPath = mountPath;
            m_USBInfo01.volume = volume;
        }
        else if (volume == e_USB2)
        {
            m_USBInfo02.serialNum = serialNum;
            m_USBInfo02.mountPath = mountPath;
            m_USBInfo02.volume = volume;

        }
        else if (volume == e_USB1_DISCONN)
        {
            m_USBInfoDisConn01.serialNum = serialNum;
            m_USBInfoDisConn01.mountPath = "";
            m_USBInfoDisConn01.volume = volume;
        }
        else if (volume == e_USB2_DISCONN)
        {
            m_USBInfoDisConn02.serialNum = serialNum;
            m_USBInfoDisConn02.mountPath = "";
            m_USBInfoDisConn02.volume = volume;

        }
    }
    USBInfo* CGracenoteRequestQueue::GetUSBConnectedInfo(int volume)
    {
        if (volume == e_USB1)
        {
            return &m_USBInfo01;
        }
        else if (volume == e_USB2)
        {
            return &m_USBInfo02;
        }
        else if (volume == e_USB1_DISCONN)
        {
            return &m_USBInfoDisConn01;
        }
        else if (volume == e_USB2_DISCONN)
        {
            return &m_USBInfoDisConn02;
        }
    }*/

/*
 *******************************************************************
 * Function	: Queue::onIPodConnected
 *
 * Description 	: Call API to indicate IPOD connected
 *******************************************************************
 */
void CGracenoteRequestQueue::onIPodConnected(QString serialNum, int startIndex, int endIndex,
                                                   int syncState, int volume)
{
    m_pGracenoteInterface->IPODConnected(serialNum, startIndex,endIndex,
                                                                      syncState, volume);
}

/*
 *******************************************************************
 * Function	: Queue::onHighLightedVolume
 *
 * Description 	: Call API to indicate highlighted volume
 *******************************************************************
 */
void CGracenoteRequestQueue::onHighLightedVolume(int volume, QString serialNum, QString mountPath)
{
    m_pGracenoteInterface->HighLightedVolume(volume, serialNum, mountPath);
}

/*
 *******************************************************************
 * Function	: Queue::onUsbDisconnected
 *
 * Description 	: Call API to indicate USB disconnected
 *******************************************************************
 */
/*void CGracenoteRequestQueue::onUsbDisconnected(QString serialNum, int volume)
{
    LOG_INFO << "aettiequeue Queue::onUsbDisconnected() " << LOG_ENDL;
    SetUSBConnectedInfo(serialNum, "", volume);
    m_pGracenoteInterface->EnqueueUSBDisConnected(serialNum, volume);
   LOG_TRACE <<"Queue::OnUsbDisconnected Slot Exit" << LOG_ENDL;
}*/
void CGracenoteRequestQueue::onUsbDisconnected(QString serialNum, int volume)
{
    //LOG_TRACE <<"Queue::OnUsbDisconnected Slot Enter" << LOG_ENDL;
    //Wainting until indexing finished
	//added by aettie for Gracenote killed handling

   
    m_pGracenoteInterface->USBDisConnected(serialNum, volume);

   //LOG_TRACE <<"Queue::OnUsbDisconnected Slot Exit" << LOG_ENDL;
}



/*
 *******************************************************************
 * Function	: Queue::onIPodDisConnected
 *
 * Description 	: Call API to indicate IPOD disconnected
 *******************************************************************
 */
void CGracenoteRequestQueue::onIPodDisConnected(QString serialNum, int volume)
{
    m_pGracenoteInterface->IPODDisConnected(serialNum, volume);
}

/*
 *******************************************************************
 * Function	: Queue::onCloseCollections
 *
 * Description 	: Call API to close collections
 *******************************************************************
 */
void CGracenoteRequestQueue::onCloseCollections()
{
    m_pGracenoteInterface->CloseCollectionIDs();
}

/*
 *******************************************************************
 * Function	: Queue::onAVOff
 *
 * Description 	: 
 *******************************************************************
 */
//void CGracenoteRequestQueue::onAVOff()
//{
//    m_pGracenoteInterface->AVOffHandling();
//}

/*
 *******************************************************************
 * Function	: Queue::onRetrieveMultipleCoverArts
 *
 * Description 	: Call API to retrieve multiple cover arts
 *******************************************************************
 */
void CGracenoteRequestQueue::onRetrieveMultipleCoverArts(QStringList albumList, QStringList artistList)
{
    m_pGracenoteInterface->RetrieveMultipleCoverArts(albumList, artistList);
}

/*
 *******************************************************************
 * Function	: Queue::onTimerFinished
 *
 * Description 	: Emit signal for application to exit
 *******************************************************************
 */
void CGracenoteRequestQueue::onTimerFinished()
{
    //LOG_INFO << "CGracenoteRequestQueue::onTimerFinished " << LOG_ENDL;

    emit ExitFromApp();
}

/*
 *******************************************************************
 * Function	: Queue::onResyncAfterCopyOperation
 *

 * Description 	: Call API for performing resync when there is a copy or
 *				  move opreration.
 *******************************************************************
 */
void CGracenoteRequestQueue::onResyncAfterCopyOperation(int targetVolume)
{
    //LOG_INFO << "CGracenoteRequestQueue::onResyncAfterCopyOperation" << LOG_ENDL;
    m_pGracenoteInterface->ResyncAfterCopyOperation(targetVolume);
}
 //}deleted all "LOG statement" by aettie.ji 2013.02.22 for ISV 71131 etc.
//{added by aettie.ji 20130904 for gracenote logo
/*
 *******************************************************************
 * Function	: CGracenoteRequestQueue::emitSignalForGracenoteLogo
 *
 * Description 	: Destructer

 * Parameters 	: None

 * Returns 	: None
 *
 *******************************************************************
 */
void CGracenoteRequestQueue::emitSignalForGracenoteLogo(EVolumeType type, bool status)
{
        emit GracenoteIndexingStatus(status, type); 
 }
  //deleted by aettie 20131010 unused code
//void CGracenoteRequestQueue::onRequestEmitSignalForGracenoteLogo()
//{
//    emitSignalForGracenoteLogo(e_MultiCA, 1);
// }
//}added by aettie.ji 20130904 for gracenote logo
//{added by aettie.ji 20130904 for gracenote version

 void CGracenoteRequestQueue::onGetSDKVer()
 {
    m_pGracenoteInterface->GetSDKVer();
 }
 void CGracenoteRequestQueue::onGetDBVer()
 {
    m_pGracenoteInterface->GetDBVer();
 }
 //20131023 System info
 void CGracenoteRequestQueue::onGetDBRegionVer()
 {
    m_pGracenoteInterface->GetDBRegionVer();
 }
//}added by aettie.ji 20130904 for gracenote version

//End of file
