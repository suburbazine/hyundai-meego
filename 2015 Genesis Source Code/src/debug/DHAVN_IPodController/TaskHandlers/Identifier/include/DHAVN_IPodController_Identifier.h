/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini / Venkat                    *
 * DATE       :  12 August 2011                                   *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :
 *                   This is the Identification TaskHandler       *
 *                   it Starts and handle the whole iden-         *
 *                   tification process                           *                                              *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 26 Sept 2012   Pawan Gupta             Changed file to separate the
                                        identification process
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef IDENTIFIER_H
#define IDENTIFIER_H
#include <DHAVN_IPodController_taskHandler.h>
#include <QVariantMap>
#include <QBitArray>
#include <QList>
#include <DHAVN_LogUtil.h>
#include <DHAVN_IPodController_setFIDTokenValue.h>
#include <QEventLoop>
#include <DHAVN_IPodController_startIDPS.h>
#include <DHAVN_IPodController_eventloopTimer.h>    // added by sungha.choi 2013.04.12
#ifdef REQIDENTIFY
#include <DHAVN_IPodController_RequestIdentify.h>
#endif

using namespace GeneralLingo;

#ifdef DBUSBASEDAPPHANDLER
struct AppSessionId
{
    QString m_AppId;
    qint16 sessionId;

    bool operator == (const AppSessionId& rhs) const
    {
	bool ret = false;
       if(!m_AppId.compare(rhs.m_AppId) && sessionId == rhs.sessionId)
           ret = true;
	return ret;
    }
};
#endif

class CIdentifierTaskHandler : public CTaskHandler
{
    Q_OBJECT
private:
    USE_LOG_UTIL
public:
    CIdentifierTaskHandler(QObject *parent);
    virtual ~CIdentifierTaskHandler();
    virtual void identify(QBitArray supportedLingoes, QList<FIDTokenValue> fidTokenValues);
    /* Stops the current task */
    virtual void stopCurrentTask();
    /* Stops the entair taskhandler. This will disconnect the taskhandler form iAP channel*
     * This can be called if the task handler is not required to operate.
     * This will improve the performance.
    */
    virtual void stopTaskHandler();
    virtual bool isReadyToDelete(); // added by sungha.choi 2013.04.12

#ifdef DBUSBASEDAPPHANDLER
    QList <AppSessionId>& getSessionIdList() { return m_AppSessionIdList; }
#endif
    virtual int getMaxPacketSize() {return m_MaxWritePacketSize;}
    bool m_EndIdpsState;


protected:
    virtual int RequestTransPortMaxPayLoadSize();
    virtual bool isWaitingForResponse();    // added by sungha.choi 2013.04.12

signals:
    void identified(bool successful);
	#ifdef DBUSBASEDAPPHANDLER
    void AppSessionOpened(const QString& AppId, qint16 sessionId);
    void AppSessionClosed(qint16 sessionId);
	#endif
    void    mainLoopQuit();

    void identificationType(bool isIDPS);
    void reconnection();    // added by sungha.choi 2013.03.07

public slots:
    virtual void packetAvailable(int lingoID,int commandID,const QByteArray& buf );
    virtual void StartIDPS();
private slots:
    void StartIDPS_timeout_slot();   // added for CR 13200
protected:
    iPodControllerPrivateData* m_iPodPrivateData;
	#ifdef DBUSBASEDAPPHANDLER
    QList <AppSessionId> m_AppSessionIdList;
	#endif
    int m_MaxWritePacketSize;
    QString m_IPodPandoraAppSessionID;
//#ifdef DHAVN_COMPILE_FOR_AHA // added by jungae, 2012.12.01 for AhaRadio application
    QString m_IPodAhaAppSessionID;
//#endif
    bool m_isDisconnected;            // moved here from private: area for identify retry(CR 13200)
    int m_WaitingResponseOfCommand;   // for identify retry(CR 13200)
    int m_CommandRetry;               // for identify retry(CR 13200)

private:
    QTimer m_rtimer;                  // for StartIDPS retry(CR 13200)
    QEventLoop *m_pLoop;              // added by sungha.choi 2013.04.12
    CEventloopTimer *m_pWaitTimer;
    bool m_isRunningLoop;   // added by sungha.choi 2013.04.13
    bool m_isIDPS;
    bool m_ReadyToDelete;       // added by sungha.choi    2013.04.12
};

#endif // IDENTIFIER_H
