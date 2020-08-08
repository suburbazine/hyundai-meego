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
 *                   This is the Autherizor TaskHandler           *
 *                   it Starts and handle the whole auth-         *
 *                   ntication process,
 *                   and helps ipod to enter into UI mode         *
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef AUTHORISOR_H
#define AUTHORISOR_H
#include <DHAVN_IPodController_taskHandler.h>
#include <DHAVN_IPodController_getDevAuthenticationInfo.h>
#include <DHAVN_LogUtil.h>
#include <QTimer>
using namespace GeneralLingo;

class CAuthorisorTaskHandler : public CTaskHandler
{
    Q_OBJECT
private:
    USE_LOG_UTIL
public:
    CAuthorisorTaskHandler(QObject *parent);
    ~CAuthorisorTaskHandler();
    /* Stops the current task */
        void stopCurrentTask();
    /* Stops the entair taskhandler. This will disconnect the taskhandler form iAP channel*
     * This can be called if the task handler is not required to operate.
     * This will improve the performance.
    */
    virtual void stopTaskHandler();

    bool m_IsTimeToRespond;

private:
    void retDevAuthenticationNonIDPS();
    void retDevAuthenticationSignature(QByteArray chal);
signals:
    void authorised(bool sucessful);
public slots:
    virtual void packetAvailable(int lingoID,int commandID,const QByteArray& buf );
    void RemoteUiModeActivated(bool status);
    void retDevAuthentication();
    void Authorisor_restart_slot(); // added for identify retry(CR 13200)
private:
    int m_CertIncrementCount;
	int m_CertIncrementCountNonIDPS;
    iPodControllerPrivateData* m_iPodPrivateData;
    int m_authCount;
    QTimer m_authTimer;
    qint16 m_LocalTransId;
    int m_WaitingResponseOfCommand; // added for identify retry(CR 13200)
};

#endif // AUTHORISOR_H
