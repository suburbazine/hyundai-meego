/*
 ******************************************************************
 *        COPYRIGHT © Teleca AB                                   *
 *----------------------------------------------------------------*
 * MODULE     :  IPodAppController                                *
 *
 * PROGRAMMERS : Pawan Gupta                                      *
 * DATE       :  16 jan 2012                                      *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 *                                                                *
 * MODULE SUMMARY :

 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 *
 ******************************************************************
 */
#ifndef DHAVN_IPODAPPCONTROLLER_H
#define DHAVN_IPODAPPCONTROLLER_H

/**
  * includes
  */
#include <qobject.h>
#include <DHAVN_IPodAppController_CommonVars.h>
#include <DHAVN_LogUtil.h>

/**
  * Forward Declaration
  */
class CIPodAppControllerPriv;
/**
  * This class is the interface of the IPOD App Controller
  * This class exposes the functions to connect disconnet
  * Send, receive the data from/to IPOD
  */
class CIPodAppController : public QObject
{
    USE_LOG_UTIL

    Q_OBJECT
public:
    /**
      * Constructor of the class
      */
    CIPodAppController(QObject* parant=NULL);

    /**
      * Destructure of the class
      */
    ~CIPodAppController();

    /**
      * Initialization function of the class.
      * After constructor call this API must be called to
      * Initialize the library properly for the specific
      * IPOD app.
      * The parameter must be passed with
      * proper App Id "com.pandora.link.v1"(ex. "com.pandora.link.v1"),
      * and BundleSeedId (ex. "6KVQ6HHK5F").
      *
      *
      * Note: Caller must wait on the InitComplete() signal before
      * issuing any further command.
      */
    EIPodAppErrors Init(const QString& AppId, const QString& BundleSeedId, int ipodNumber, int protocolIndex); // added by Tan 2013.01.05

    /**
      * API to De-Initialize the library.
      * This function will clear all the data (ex. handle)
      * and will disconnect from IPOD App.
      */
    EIPodAppErrors D_Init();

    /**
      * API to be used to send the commands to the
      * IPOD Application.
      *
      * Return value as IPOD_APPSUCCESS means the command is sent.
      * One must wait on the Acknowledgement() signal for the
      * status of the command execution.
      */
    EIPodAppErrors Send(const QByteArray& dataArray);

    EIPodAppErrors LaunchApplication(QString appBundleID);

signals:

    /**
      * This is the signal of the class
      * This will be emitted when the initialization of the class
      * is completed and its ready to be used.
      */
    void InitComplete();

    /**
      * This is the signal of the class
      * This will be emitted when the data is received
      * from the IPOD app.
      * Note: This is not the acknoledgement of any sent command.
      */
    void DataReceived(const QByteArray& dataArray);

    /**
      * This the signal of the class
      * This will be emitted when the acknowledgement of
      * any issued command is received.
      * status = true - command is successful
      * status = false - command was unsuccessful
      */
    void Acknowledgement(EIPodAppErrors status);

    /**
      * This is the signal of the class
      * This will be emitted when the Session between
      * Library and the IPOD app is closed by some reason.
      * D_Init() will be called internally and the
      * Lib will be under non initialize mode
      *
      * App Must call the Init() function for the further
      * communication
      */
    void SessionClosed(EIPodAppErrors error);

    void SessionOpened(int protocolIndex); // added by Tan 2012.12.15
    void NotifySessionClosed(); // added by Tan 2013.01.05
    void AudioSampleRateChanged(int  nSampleRate); //added by Tan for Aha sampling rate changed,

    /**
      * This signal will be emitted when the
      * device will be disconnected from the
      * HU
      * D_Init() will be called internally and the
      * Lib will be under non initialize mode
      *
      * App Must call the Init() function for the further
      * communication
      */
    void DeviceDisconnected(int ipodNumber);

private:
    CIPodAppControllerPriv* m_IPodAppControllerPriv;
};

#endif // DHAVN_IPODAPPCONTROLLER_H
