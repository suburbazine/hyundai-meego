/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta                                      *
 * DATE       :  21 Spet 2012                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                                                                *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef DHAVN_IPODCONTROLLER_IPODDBUIDFETCHTASKHANDLER_H
#define DHAVN_IPODCONTROLLER_IPODDBUIDFETCHTASKHANDLER_H

#include <DHAVN_IPodController_iPodDBFetchTaskHandler.h>

class CIPodDBUIDFetchTaskHandler : public CIPodDBFetchTaskHandler
{
    Q_OBJECT
private:
    USE_LOG_UTIL

    public:
        ~CIPodDBUIDFetchTaskHandler();
        CIPodDBUIDFetchTaskHandler(QObject* parent, iPodControllerPrivateData* privateData, CIAPPacketWriter* iapWriter, CIAPPacketReader* iapReader);
        virtual void returnAllTracksInfoE(int,int);
        virtual QList <SiPodDBTrackInfo> getUIDBasedTrackList(){return m_TrackList;}
        virtual EIPodErrors getiTunesDBInfoE(SITunesDBInfo *dbInfo);// Will fetch iPod iTunes DB info
        void stopCurrentTask();

public slots:
        void packetAvailable(int lingoID,int commandID,const QByteArray& buf );
signals:
        void updateFileNameWithTrackIndex(int nTrackIndex, quint64 uid);

private:
        QMap<int,SiPodDBTrackInfo> m_tracksInfoMap;
        QList <QByteArray> m_MetaDataArray;
        int m_GetDBTrackCountForMetaData;
        SITunesDBInfo m_itunesDBInfo;
};
#endif // DHAVN_IPODCONTROLLER_IPODDBUIDFETCHTASKHANDLER_H
