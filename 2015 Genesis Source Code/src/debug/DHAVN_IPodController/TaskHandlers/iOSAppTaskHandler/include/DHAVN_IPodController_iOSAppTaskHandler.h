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
#ifndef PANDORATASKHANDLER_H
#define PANDORATASKHANDLER_H
#include <QString>
#include <QEventLoop>
#include <DHAVN_IPodController_taskHandler.h>
#include <DHAVN_IPodController_PrivateData.h>
#include <DHAVN_LogUtil.h>
#include <DHAVN_IPodController_eventloopTimer.h>    // added by sungha.choi 2013.04.12
#include <DHAVN_iPodController_Logger.h>

//#endif

class CIPodDBIndexFetchTaskHandler;

class CiOSAppTaskHandler : public CTaskHandler
{
    Q_OBJECT
public:
    bool isReadyToDelete(); // added by sungha.choi 2013.04.12

private:
    USE_LOG_UTIL
public:
        CiOSAppTaskHandler(QObject* parent, iPodLogger *iLog);
        ~CiOSAppTaskHandler();

    void     stopCurrentTask();
    void     stopTaskHandler();
    bool     isNowPlayingApplicationIsMPE();
    bool     RestartPlay();

    bool isBasicControlEnabled();
    void delayBasicControlEnable(bool enable, QString AppName);

    void resetCurrentBasicControlStateVariable();

    QString m_ApplicationBundleName1;

private:
    int     waitForResponceFromExitCode(int timeOut, int nExitCode);
    bool    isWaitingForResponse();    // added by sungha.choi 2013.04.12
    int     isWaitingForResponseFromSignal();
public slots:
    virtual void packetAvailable(int lingoID,int commandID,const QByteArray& buf );


signals:
    void    mainLoopQuit();
    void    mainLoopQuit(int nExitCode);
    void    OnBasicControlEnableStatusChange(bool, QString);
    void OnTrackChanged(int);
#ifdef SIRI_TIMEOUT_TEST
    void    LaunchedDefaultMusicApp(bool, QString);
#endif

private:
    void SetIscurrentPlayingMPFlag(QString bundleName);

    iPodControllerPrivateData*      m_iPodPrivateData;
    CIPodDBIndexFetchTaskHandler*   m_iPodDBIndexFetchTaskHandler;

    QEventLoop      *m_pLoop;    // added by sungha.choi 2013.04.12
    CEventloopTimer *m_pWaitTimer;

    bool    m_isRunningLoop;   // added by sungha.choi 2013.04.13
    bool    m_CommandStatus;

    bool    m_isDisconnected;
    iPodLogger   *piPodLog;
    bool    m_ReadyToDelete;    // added by sungha.choi 2013.04.12
    bool    m_ReadyToDeleteFromSignal;
    int     m_nRunningLoopFromSignal;


    bool m_bBasicControlStatusToBeSend;
    QString m_strBasicControlerAppName;
    QTimer* m_delayBasicControlEnableTimer;
    bool m_bCurrentBasicControlState;
public slots:
    void delayBasicControlEnableSlot();
};

#endif // PANDORATASKHANDLER_H
