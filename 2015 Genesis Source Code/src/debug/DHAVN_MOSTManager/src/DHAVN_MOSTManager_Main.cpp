/*
 ******************************************************************
 * COPYRIGHT © <2011>, 
 * All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :  MOSTManager                                      *
 * PROGRAMMER :  Neelima Khedad,Venkatesh Aeturi                  *
 * DATE       :  28 July 2011                                     *
 * VERSION    :                                                   *
 *
 *----------------------------------------------------------------*
 * MODULE SUMMARY : MOST Manager is a middle ware component which *
 *                  talks to the applications and controller over *
 *                  QtDbus.The data sent over QtDbus is further   *
 *                  communicated to other MOST devices through ACS*
 *                  daemon.This module is also responsible for    *
 *                  commands from ACS daemon.                     *
 *                  This file contains the main Function          *
 *                  instanciates the MOSTManager and also creates *
 *                  adaptor Thread for for receiving QtDbus events*
 *                                                                *
 *----------------------------------------------------------------*
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 * 28 Jul 2011  Neelima Khedad           Draft version            *
 * 13 Jan 2012  Shiva Kumar              Removed Daemonize part in the main*
 * 08 Feb 2011  Neelima Khedad           Using the latest Version of LOG_INIT function Logger module
 * 30 Oct 2012  Shiva Kumar              changes for CR15173
 * 19 Feb 2013  Hanhong Keum             Added check_most_driver_is_ready function
 ******************************************************************
 */

#include <QtCore/QCoreApplication>

#include "DHAVN_MOSTManager_MOSTManager.h"
#include "DHAVN_MOSTManager_MOSTDebug.h"
#include "DHAVN_MOSTManager_Config.h"

/*
 *******************************************************************
 * Function:main
 *
 * Description :Main Function instanciates the MOSTManager and     *
 *              also creates adaptor Thread for for receiving      *
 *              QtDbus events                                      *
 *
 * Parameters :
 *    argc     rw   command line argument.
 *    argv     rw   command line argument.
 *
 * Returns :returns 0
 *
 *******************************************************************
 */

int main(int argc, char *argv[])
{
    /** Creating this instance to run an event loopn in background for Qt events */
    QCoreApplication app(argc, argv);

    /** Instance of main MOSTmanager class */
    CMOSTManager *pMostManager;
    pMostManager = new CMOSTManager();

    /** Execute k2lacsdaemon */
    pMostManager->Launch();

    /** Configures all the function blocks */
    pMostManager->Configure();

    /** Instance of Adaptor thread */
    CAdaptorThread *pAdaptorThread;
    pAdaptorThread = new CAdaptorThread(pMostManager);

    /** Register MOSTManager to UISH by using AppFrameworkIPC instead of the whole AppFramework */
    DECLARE_META_EVENT();
    pAdaptorThread->ConnectToUISH();

    /** Starting adaptor thread which process the evnts on QtDbus */
    pAdaptorThread->Execute();

#ifdef _K2L_DEBUG_
    CTraceThread *pTraceThread;
    pTraceThread = new CTraceThread();
    pTraceThread->Configure();
    pTraceThread->start(QThread::NormalPriority);
#endif

    int result = app.exec();

    delete pMostManager;
    pMostManager = NULL;

    delete pAdaptorThread;
    pAdaptorThread = NULL;

    return result;
}
