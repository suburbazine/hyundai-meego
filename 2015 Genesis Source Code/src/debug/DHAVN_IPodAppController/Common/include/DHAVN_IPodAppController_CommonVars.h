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
#ifndef DHAVN_IPODAPPCONTROLLER_COMMONVARS_H
#define DHAVN_IPODAPPCONTROLLER_COMMONVARS_H
#include <QString>

const QString gIPodAppControllerLogs("IPodAppController");

/**
  * Errors which will be used
  * in return from
  * this library
  */
enum EIPodAppErrors
{
    //If the command is timeout
    IPOD_APPERROR_TIMEOUT = -9,
    //If the controller is already initialized
    //for the same app id and seed id
    IPOD_APPERROR_ALREADYINITIALIZED = -8,
    //If the controller is not initialized
    IPOD_APPERROR_NOTINITIALIZED = -7,
    //If the controller is not
    //responding
    IPOD_APPERROR_NOTRESPONDING = -6,
    //If the Device is not connected
    IPOD_APPERROR_NOTCONNECTED = -5,
    //If application is not opened
    IPOD_APPERROR_APPNOTOPENED = -4,
    //Genreric error (unknown)
    IPOD_APPERROR_GENERIC = -3,
    //If the Controller is already busy
    //with previous commands
    IPOD_APPERROR_BUSY = -2,
    //
    IPOD_APPERROR_SESSIONCLOSED = -1,
    //No error
    IPOD_APPSUCCESS = 0
};

enum EIPodPorts
{
    eIPOD_PORT_NONE = 0, // Default, means single ipod supported
    eIPOD_PORT_1 = 1,    // This Controller is dedicated for iPods connected to usb port 1
    eIPOD_PORT_2 = 2     // This Controller is dedicated for iPods connected to usb port 2
};

#endif // DHAVN_IPODAPPCONTROLLER_COMMONVARS_H
