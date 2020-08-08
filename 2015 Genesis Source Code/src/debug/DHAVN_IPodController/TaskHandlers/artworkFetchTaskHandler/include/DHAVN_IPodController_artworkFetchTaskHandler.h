#ifndef DHAVN_IPODCONTROLLER_ARTWORKFETCHTASKHANDLER_H
#define DHAVN_IPODCONTROLLER_ARTWORKFETCHTASKHANDLER_H

/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini / Venkat                    *
 * DATE       :  03 Dec 2011                                     *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                   This is the class which will fetch the       *
  *                   Artwork from iPod                           *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#include <DHAVN_IPodController_taskHandler.h>
#include <DHAVN_IPodController_eventloopTimer.h>

#include <DHAVN_IPodController_getArtworkFormats.h>
#include <DHAVN_IPodController_getTrackArtworkTimes.h>
#include <DHAVN_IPodController_getTrackArtworkData.h>
#include <DHAVN_IPodController_retArtworkFormats.h>
#include <DHAVN_IPodController_retTrackArtworkTimes.h>
#include <DHAVN_IPodController_retTrackArtworkData.h>
#include <DHAVN_IPodController_retArtworkTimesUID.h>
#include <DHAVN_IPodController_getIndexedPlayingTrackInfo.h>
#include <DHAVN_IPodController_returnIndexedPlayingTrackInfo.h>
#include <DHAVN_LogUtil.h>

#define INVALID_PACKET_INDEX -3
#define BAD_PARAM_INDEX -4
using namespace ExtendedLingo;

class CArtworkFetchTaskHandler : public CTaskHandler
{
    Q_OBJECT
private:
    USE_LOG_UTIL
public:
    CArtworkFetchTaskHandler(QObject *parent,
                             iPodControllerPrivateData* privateData,
                             CIAPPacketWriter* iapWriter,
                             CIAPPacketReader* iapReader);
    ~CArtworkFetchTaskHandler();

    /* Stops the current task */
    void stopCurrentTask();
    /* Stops the entair taskhandler. This will disconnect the taskhandler form iAP channel*
     * This can be called if the task handler is not required to operate.
     * This will improve the performance.
    */
    void stopTaskHandler();

    int fetchArtworkE(int trackIndex,QString &artPath, bool isUIDSupported, bool forceFlag, quint64 UID = 0);  // Modified by pawan 2013.05.15.
    bool cleanupAfterArtworkFetch();
    bool isReadyToDelete(); // added by sungha.choi 2013.04.12

private:/* methods*/
    int waitForResponceFromExitCode(int timeOut, int nExitCode);
    int waitForCommandE(int command, int lingo);
    int getSupportedArtworkFormatsE(int &nBestFormatID);
	// { Modified by pawan 2013.05.15.
    int fetchArtWorkForUIDDeviceE(quint64 uid);
    QByteArray getArtworkTimesUID(int formatID, quint64 uid);
    QByteArray getArtWorkUID(int timeOffset, int formatID, quint64 uid);
	// } Modified by pawan 2013.05.15.
    void retArtworkDataUID(QByteArray cmd);
    bool isWaitingForResponse();    // added by sungha.choi 2013.04.12
    int isWaitingForResponseFromSignal();

signals:
        void mainLoopQuit(int nExitCode);
        void mainLoopQuit();
public slots:
    virtual void packetAvailable(int lingoID,int commandID,const QByteArray& buf );


private:/*Variables*/
    QEventLoop *m_pLoop;    // added by sungha.choi 2013.04.12
    CEventloopTimer *m_pWaitTimer;    // added by sungha.choi 2013.04.12
    bool m_isRunningLoop;   // added by sungha.choi 2013.04.13
    int m_nRunningLoopFromSignal;
    CRetArtworkFormats *m_retArtworkFormats;
    int m_nBestFormatID;
    CRetTrackArtworkTimes *m_retTrackArtworkTimes;
    CRetTrackArtworkTimesUID *m_retArtworkTimesUID;
    iPodControllerPrivateData* m_iPodPrivateData;
    CIAPPacketWriter* m_iAPWriter;
    CIAPPacketReader* m_iAPReader;

    /* Logo relate variables */
    int m_artWidth;
    int m_artHeight;
    int m_artFormat;
    int m_rowSize;
    int m_artIndex;

    int m_previous_packet_index;

    int m_totalImageSize;
    QByteArray m_artData;
    bool m_isDisconnected;

    //CR 16144,15669 : Added for artwork S
    int  m_artWorkTotalCount;
    //CR 16144,15669 : Added for artwork E
    QList <int> m_TriedArtworkIndexList;
    bool m_ReadyToDelete;    // added by sungha.choi 2013.04.12
    bool m_ReadyToDeleteFromSignal;
};




#endif // DHAVN_IPODCONTROLLER_ARTWORKFETCHTASKHANDLER_H
