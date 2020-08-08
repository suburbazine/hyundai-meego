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
#ifndef DHAVN_IPODCONTROLLER_IPODDBINDEXFETCHTASKHANDLER_H
#define DHAVN_IPODCONTROLLER_IPODDBINDEXFETCHTASKHANDLER_H

#include <DHAVN_IPodController_iPodDBFetchTaskHandler.h>

class CIPodDBIndexFetchTaskHandler : public CIPodDBFetchTaskHandler
{
    Q_OBJECT
private:
    USE_LOG_UTIL

    public:
        CIPodDBIndexFetchTaskHandler(QObject* parent, iPodControllerPrivateData* privateData, CIAPPacketWriter* iapWriter, CIAPPacketReader* iapReader);
        virtual SiPodDBTrackInfo getIndexedTrackMetaDataE(int index, int metaDataCount=6);
        ~CIPodDBIndexFetchTaskHandler();
        //CR 13730: ATS Warning Fix S
        bool playCommandCall(int);
        void stopTaskHandler();
        //CR 13730: ATS Warning Fix E

public slots:
    void packetAvailable(int lingoID,int commandID,const QByteArray& buf );
private:
        QList<QByteArray> m_metaDataList_IndexedBased;
        int m_MetaDataToFecthCount;
        bool m_metadata_Flag;

};
#endif // DHAVN_IPODCONTROLLER_IPODDBINDEXFETCHTASKHANDLER_H
