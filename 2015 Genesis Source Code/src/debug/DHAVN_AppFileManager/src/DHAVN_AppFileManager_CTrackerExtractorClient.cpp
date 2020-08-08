#include "hdr/DHAVN_AppFileManager_CTrackerExtractorClient.h"
#include <QDebug>
#include <QtDBus/QDBusConnectionInterface>

#define POPUP_TIMEOUT           3000
// modified by Dmitry 14.08.13 for ITS0183845
CTrackerExtractorClient::CTrackerExtractorClient(QObject *parent) :
    QObject(parent)
  , connection(QDBusConnection::connectToBus(QDBusConnection::SessionBus,
                                             "org.lge.TrackerExtractor"))
  , trackingFinished(true)
{
    // FM_LOG_INIT_V2("AppFileManager");
    if (!connection.isConnected())
    {
        MP_LOG << "CTrackerExtractorClient .ctor(): could not connect to TrackerExtractor." <<
                  "Notification will be unplugged" << LOG_ENDL;
    }
    else
    {
        //QStringList serviceNames = connection.interface()->registeredServiceNames();
        //FM_LOG << "CTrackerExtractorClient .ctor() registered DBUS services:" << serviceNames << LOG_ENDL;
// modified by Dmitry 17.08.13 for ITS0183917
        bool result;

//        result = connection.connect("org.lge.TrackerExtractor", "/", "org.lge.TrackerExtractor", "MiningStatus", this,
//                                    SLOT(onMinerStatus(QString,QString)));
//        if (!result)
//        {
//            MP_LOG << "Unable to connect signal MinerProgress" << LOG_ENDL;
//        }

//        result = connection.connect("org.lge.TrackerExtractor", "/", "org.lge.TrackerExtractor", "changeNotifier", this,
//                                    SLOT(onChangeNotify(int,bool,QVector<QStringList>)));
//        if (!result)
//        {
//            MP_LOG << "Unable to connect signal changeNotify" << LOG_ENDL;
//        }

//        result = connection.connect("org.lge.TrackerExtractor", "/", "org.lge.TrackerExtractor", "MiningStatusInDetail", this,
//                                    SLOT(onMiningStatusInDetail(QString, QString, int)));
//        if (!result)
//        {
//            MP_LOG << "Unable to connect signal MiningStatusInDetail" << LOG_ENDL;
//        }
         result = connection.connect("org.lge.TrackerExtractor", "/", "org.lge.TrackerExtractor", "ThumbnailsGenerated", this,
                                     SLOT(onThumbnailsGenerated()));

         if (!result)
         {
             MP_LOG << "Unable to connect signal ThumbnailsGenerated" << LOG_ENDL;
         }
    }
}

void CTrackerExtractorClient::onThumbnailsGenerated()
{
    MP_MEDIUM << LOG_ENDL;
    emit ThumbNailGenerated();
}
// modified by Dmitry 17.08.13 for ITS0183917

void CTrackerExtractorClient::onMinerStatus(QString minerType, QString minerStatus)
{
    MP_LOG << "CTrackerExtractorClient::onMinerStatus(): Mining status for" << minerType <<
              "is" << minerStatus << LOG_ENDL;

    if (minerStatus == "inProgress" && this->trackingFinished)
    {
        MP_LOG << "emit showNotifyPopup" << LOG_ENDL;
        this->trackingFinished = false;
        emit showNotifyPopup();

        //this->popupTimer.singleShot(POPUP_TIMEOUT, this, SLOT(onTimeout()));
    }
    else if (minerStatus == "Finished")
    {
        // FM_LOG << "emit hideNotifyPopup" << LOG_ENDL;
        //if (this->popupTimer.isActive())
        //{
        //    this->popupTimer.stop();
            emit hideNotifyPopup();
        //}

        emit MinerStatusFinished();

        this->trackingFinished = true;
    }
}

void CTrackerExtractorClient::onChangeNotify(int ChangeStatus, bool ChangedType,
                                             QVector<QStringList> UpdtaedUrlList)
{
    // FM_LOG << "CTrackerExtractorClient::onChangeNotify(): Files changed  " << ChangeStatus << "  "  << ChangedType << LOG_ENDL;

    Q_UNUSED(ChangeStatus) // added by Dmitry 17.08.13
    if ( !ChangedType )
    {
       return;
    }

    for (int i = 0; i < UpdtaedUrlList.size(); i++)
    {
        //FM_LOG << UpdtaedUrlList.at(i) << LOG_ENDL;
    }

    emit updateUIData( &UpdtaedUrlList );
}


void CTrackerExtractorClient::onMiningStatusInDetail(QString minerType, QString minerStatus, int percentage)
{
   Q_UNUSED( percentage )
   Q_UNUSED(minerStatus) // added by Dmitry 17.08.13
   Q_UNUSED(minerType) // added by Dmitry 17.08.13

   // FM_LOG << "mylog: CTrackerExtractorClient::onMinerStatusInDetail(): Mining status for" << minerType <<
   //           "is" << minerStatus << LOG_ENDL;

   emit showNotifyPopup();
}

void CTrackerExtractorClient::onTimeout()
{
    emit hideNotifyPopup();
}

// removed by Dmitry 21.08.13

