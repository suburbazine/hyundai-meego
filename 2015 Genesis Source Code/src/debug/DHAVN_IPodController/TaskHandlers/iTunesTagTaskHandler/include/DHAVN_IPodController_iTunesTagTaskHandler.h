#ifndef DHAVN_IPODCONTROLLER_ITUNESTAGTASKHNDLER_H
#define DHAVN_IPODCONTROLLER_ITUNESTAGTASKHNDLER_H
/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Sowmini / Venkat                    *
 * DATE       :  2 Jan 2012                                     
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                   This is the class which will help control    *
 *                   to put the device to enter into the remote   *
 *                   UI mode                                      *
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
#include <QEventLoop>
#include <QDBusMessage>
#include <QtDBus>
#include <DHAVN_LogUtil.h>
#include <DHAVN_IPodController_eventloopTimer.h>    // added by sungha.choi 2013.04.12
#include <DHAVN_iPodController_Logger.h>

class CiTunesTagTaskHandler : public CTaskHandler
{
    Q_OBJECT
private:
    USE_LOG_UTIL

public:
    CiTunesTagTaskHandler(QObject *parent, iPodLogger *iLog);
    ~CiTunesTagTaskHandler();

public slots:
    virtual void packetAvailable(int lingoID,int commandID,const QByteArray& buf );
    void updateTagListenerE(QString pListName,QString pListPath);



//private:
    int exportTagFileE(QString plistPath);
    void closeFileHandleE(int);
private:
    int  m_nRunningLoopFromSignal;
    bool m_ReadyToDeleteFromSignal;

    int waitForResponceFromExitCode(int timeOut, int nExitCode);
    int EmitiTunesTaggingStatusE(QString plistPath);
    bool isWaitingForResponse();    // added by sungha.choi 2013.04.12
    int isWaitingForResponseFromSignal();
public:
    void startiTunesTagTaskHandlerE();
    void iTunesTagDirectoryE(QString plistPath);
    void stopCurrentTask();
    /* Stops the entair taskhandler. This will disconnect the taskhandler form iAP channel*
     * This can be called if the task handler is not required to operate.
     * This will improve the performance.
    */
    void stopTaskHandler();
    bool isReadyToDelete(); // added by sungha.choi 2013.04.12

signals:
    void mainLoopQuit();
    void mainLoopQuit(int nExitCode);
    void IPodPopUp(EIPodPopUp);

private:
//    qint16 m_TransID;
    quint64 m_FreeSpaceOnDevice;
    QByteArray m_CommandBuffer;
    QEventLoop *m_pLoop;    // added by sungha.choi 2013.04.12
    CEventloopTimer *m_pWaitTimer;
    bool m_isRunningLoop;   // added by sungha.choi 2013.04.13
    bool m_isDisconnected;
    bool m_FileHandleToBeClosed;
    int m_FileHandle ;
    QDBusConnection  m_DbusConnection;
    iPodControllerPrivateData* m_iPodPrivateData;
    bool m_firstTime;
    iPodLogger   *piPodLog;    // added by sungha.choi 2013.04.12
    bool m_ReadyToDelete;    // added by sungha.choi 2013.04.12
};




#endif // DHAVN_IPODCONTROLLER_ITUNESTAGTASKHNDLER_H
