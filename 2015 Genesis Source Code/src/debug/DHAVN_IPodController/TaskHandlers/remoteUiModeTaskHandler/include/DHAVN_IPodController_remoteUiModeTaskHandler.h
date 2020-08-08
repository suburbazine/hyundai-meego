#ifndef DHAVN_IPODCONTROLLER_REMOTEUIMODETASKHANDLER_H
#define DHAVN_IPODCONTROLLER_REMOTEUIMODETASKHANDLER_H
/*
 ******************************************************************
 *              *
 *----------------------------------------------------------------*
 * MODULE     :  IPodController                                   *
 *
 * PROGRAMMERS : Pawan Gupta/ Sowmini / Venkat                    *
 * DATE       :  21 Sept 2011                                     *
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

#include <DHAVN_IPodController_enterRemoteUiMode.h>
#include <DHAVN_IPodController_GetColorDisplayImageLimits.h>
#include <DHAVN_IPodController_RetColorDisplayImageLimits.h>
#include <DHAVN_IPodController_GetMonoDisplayImageLimits.h>
#include <DHAVN_IPodController_ReturnMonoDisplayImageLimits.h>
#include <DHAVN_IPodController_SetDisplayImage.h>
#include <DHAVN_IPodController_logoImageTransmitter.h>
#include <DHAVN_IPodController_RetAccSampleRateCaps.h>
#include <DHAVN_IPodController_requestLingoProtocolVersion.h>
#include <DHAVN_IPodController_returnLingoProtocolVersion.h>
#include <DHAVN_IPodController_PrivateData.h>
#include <QTimer>


#include <QtCore/QCoreApplication>
#include <QThread>
#include <QGlib/Connect>
#include <QGst/Pipeline>
#include <QGst/ElementFactory>
#include <QGst/Bus>
#include <QGst/Message>
#include <QGst/Event>
#include <QGst/Init>
#include <DHAVN_LogUtil.h>

using namespace GeneralLingo;
using namespace ExtendedLingo;
using namespace DigitalAudioLingo;


class CRemoteUIModeTaskHandler : public CTaskHandler
{
    enum ERemoteUiState{
        eExtStateIdle = 0,
        eExtStateIdentified,
        eExtStateAuthorised,
        eExtStateInitComplete,
        eExtStateExtMode
    };

    Q_OBJECT
private:
    USE_LOG_UTIL
public:
    CRemoteUIModeTaskHandler(QObject *parent);
    ~CRemoteUIModeTaskHandler();
    /* Stops the current task */
        void stopCurrentTask();
    /* Stops the entair taskhandler. This will disconnect the taskhandler form iAP channel*
     * This can be called if the task handler is not required to operate.
     * This will improve the performance.
    */
        void stopTaskHandler();
        void setRemoteUIProductID(int productID);

private:
    void SetIPODOptionsForExtendedLingo();
    void executeSuccessfulPostUiModeOperation();

signals:
    void mainLoopQuit();
    void EnteredInUiMode(bool status);
    void GetDeviceName(QString name); // added by jungae 2012.08.16 to get the iPod device name
    void identify_timeout_signal();   // added by SK 2012. 12. 18. for identify retry(CR 13200)
    void identify_restart_signal();
    void reconnection();    // added by sungha.choi 2013.04.03

public slots:
    virtual void packetAvailable(int lingoID,int commandID,const QByteArray& buf );
    void authorisationFinished(bool);
    void IdentificationProcessFinished(bool);
    void EnterUiModeTimeOut();
    void enterRemoteUiMode();

private slots:
    void identify_timeout_slot();   // for identify retry(CR 13200)

private:
    ERemoteUiState m_remoteUIState;
    iPodControllerPrivateData* m_iPodPrivateData;
    qint16 m_LocalTransID;

    bool m_isUiModeRequested;

    /* Logo relate variables */
    int   m_logoWidth;
    int   m_logoHeight;
    int m_logoFormat;
    int m_logoIndex;
    int m_countOf0A02;
    CLogoImageTrasmitter* m_logoTransmitter;
    QByteArray m_CommandBuffer;

    QTimer m_timer;
    QString m_softwareVersion;
    QString m_ipodName; // added by jungae 2012.08.16 to get the iPod device name
    // { deleted by jungae 2012.12.26, get the iPoddevice name about all devices
    // bool m_nanoTypeFlag; // added by jungae 2012.08.17 to get the device product id
    // } deleted by jungae
    int m_productID; // added by jungae 2012.08.17 to get the device product id
	//CR14645: remove the delay as the Kernel has fixed the issue
    bool m_AlreadyRequested;
    int m_IdentifyRetryCount; // for identify retry(CR 13200)

    bool m_bSupportGetIPodOptionsForLingo;
};

#endif // DHAVN_IPODCONTROLLER_REMOTEUIMODETASKHANDLER_H
