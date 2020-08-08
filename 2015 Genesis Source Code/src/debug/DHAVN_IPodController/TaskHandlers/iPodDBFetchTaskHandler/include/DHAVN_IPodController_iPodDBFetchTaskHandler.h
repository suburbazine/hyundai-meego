/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                    *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini / Venkat                    *
 * DATE       :  12 August 2011                                   *
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
#ifndef CIPODDBFETCHTASKHANDLER_H
#define CIPODDBFETCHTASKHANDLER_H
#include <DHAVN_IPodController_taskHandler.h>
#include <DHAVN_IPodController_iAPPacket.h>
#include <QVariantMap>
#include <QStack>
#include <QEventLoop>
#include <DHAVN_IPodController_TrackerInfo.h>
#include <DHAVN_TrackerAbstractor_constants.h>
#include <DHAVN_LogUtil.h>
#include <DHAVN_IPodController_retDBTrackInfo.h>
#include <DHAVN_IPodController_PrivateData.h>
#include <DHAVN_IPodController_IAPPacketReader.h>
#include <DHAVN_IPodController_IAPPacketWriter.h>
#include <QTimer>
#include <DHAVN_IPodController_eventloopTimer.h>    // added by sungha.choi 2013.04.12

#ifndef COMMON_ITUNES_FETCH_HANDLLERS
#define COMMON_ITUNES_FETCH_HANDLLERS
const int gGETDBINFOTRACKCOUNT=5;
const int gGETTRACKMETADATACOUNT=8;

#endif

typedef struct
{
    int sec;
    int min;
    int hours;
    int day;
    int month;
    int year;
}SITuneTimeStamp;

typedef struct SITunesDBInfo
{
    uint64_t DBUid;
    SITuneTimeStamp lastSyncTime;

}SITunesDBInfo;

class CIPodDBFetchTaskHandler : public CTaskHandler
{
    Q_OBJECT
private:
    USE_LOG_UTIL
public:
     //constructor
    CIPodDBFetchTaskHandler(QObject* parent,
                            iPodControllerPrivateData* privateData,
                            CIAPPacketWriter* iapWriter,
                            CIAPPacketReader* iapReader, bool connectToReader=true);
    virtual ~CIPodDBFetchTaskHandler();
    virtual void getList(QString category,int startIndex, int offset);
    virtual bool makeSelectionE(QString category,int index);
    virtual void resetPreviousSelectionsE();
    virtual int getCategorizedListCountE(QString category);
    virtual double  lingoProtocolVersion(int lingo);
    virtual void stopCurrentTask();
    virtual QString getCurrentSerialNumberE();
    virtual bool setShuffleE(int mode);
    virtual bool pauseForTracker();
    virtual int getTotalCountOfiPodTracksE();
    virtual void clearList();
    virtual bool setRepeatE(int mode);

    /* Stops the entair taskhandler. This will disconnect the taskhandler form iAP channel*
     * This can be called if the task handler is not required to operate.
     * This will improve the performance.
    */
    virtual void stopTaskHandler();



    //Index Based class must override this function
    virtual SiPodDBTrackInfo getIndexedTrackMetaDataE(int /*index*/, int metaDataCount=6){SiPodDBTrackInfo info; metaDataCount = metaDataCount; return info;}

    //UID Based class must override this function
    virtual void returnAllTracksInfoE(int,int){}
    virtual QList <SiPodDBTrackInfo> getUIDBasedTrackList(){QList <SiPodDBTrackInfo> listt; return listt; }
    virtual EIPodErrors getiTunesDBInfoE(SITunesDBInfo */*dbInfo*/){return IPOD_ERROR_UNSUPPORTED;}// Will fetch iPod iTunes DB info
    virtual bool makeSelectionSortedOnArtistE(QString category,int index, QString sortType);

    virtual bool isReadyToDelete(); // added by sungha.choi 2013.04.12


protected:
    virtual bool isWaitingForResponse();    // added by sungha.choi 2013.04.12
    virtual int isWaitingForResponseFromSignal();   // added by sungha.choi 2013.04.26
    virtual void setMutexLock();    // added by sungha.choi 2013.04.22
	//added by Tan 2013.04.26. 
	//Fixing database for 1.13 device.
    //this waitForResponceFromExitCode terminate only if emit MainLoopQuit is emitted
    //with a matching exitCode.
    virtual int waitForResponceFromExitCode(int timeOut, int nExitCode);

signals:
    void getListComplete(QMap<int,QString> namesOfCategoryContents,int length);
    void mainLoopQuit();
    void mainLoopQuit(int nExitCode);
	//added by Tan 2013.04.26. 
	//Fixing database for 1.13 device.
	//this signal is emitted when RetDBTrackInfo package is received.
    //void getDBTrackInfoReturned();

public slots:
    virtual void packetAvailable(int lingoID,int commandID,const QByteArray& buf );
    virtual void getListTimeOut();

protected:

    //Category count
    int listCount;

    QList <SiPodDBTrackInfo> m_TrackList;

    //getList() related
    QMap<int,QString> m_getListMap;
    int m_getListCount;
    QTimer m_ListFetchtimer;

    bool m_CommandStatus;
    QByteArray m_CommandBuffer;
    QEventLoop *m_pLoop;    // added by sungha.choi 2013.04.02
    CEventloopTimer *m_pWaitTimer;
    bool m_isRunningLoop;   // added by sungha.choi 2013.04.13
    int  m_nRunningLoopFromSignal;   // added by sungha.choi 2013.04.26
    bool m_isDisconnected;
    QString m_SerialNumber;
    iPodControllerPrivateData* m_iPodPrivateData;
    int m_commandID;
    CIAPPacketWriter* m_iAPWriter;
    CIAPPacketReader* m_iAPReader;
    int m_ExpectedCommandID;
    bool m_ReadyToDelete;    // added by sungha.choi 2013.04.12
    bool m_ReadyToDeleteFromSignal;    // added by sungha.choi 2013.04.26
};

#endif // CIPODDBFETCHTASKHANDLER_H
