/*
 ******************************************************************
 *      COPYRIGHT  2013, LG Electronics.
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :                                                   *
 * PROGRAMMER : Radhakrushna Mohanty                              *
 * DATE       : 21/12/2011                                        *
 * VERSION    : 1.0                                               *
 *----------------------------------------------------------------*
 * MODULE SUMMARY : Wraper main class .
 *----------------------------------------------------------------*
 */

#include <QtCore/QCoreApplication>
#include <DHAVN_GSTAudioRouter_Def.h>
#include <QDebug>
#include <DHAVN_LogUtil.h>
#include "DHAVN_PipelineHandler.h"
#include "DHAVN_AudioRouterAdaptor.h"
#include <unistd.h>

void signalHandler(int sig, siginfo_t *siginfo, void* /**context*/)
{
    USE_LOG_UTIL;
    LOG_INIT_V2("Media_IPodController");
    LOG_TRACE << "GST: signalHandler(). signal = " << strsignal(sig) << LOG_ENDL;
    LOG_TRACE << "GST: Sending PID: " << siginfo->si_pid << " UID: " << siginfo->si_uid << LOG_ENDL;
}

int main(int argc, char *argv[])
{
    USE_LOG_UTIL;
    LOG_INIT_V2("Media_IPodController");

    if(argc < 3)
    {
        LOG_HIGH << "GSTAudioRouterWrapper::main(), returning as the number of args are less than minimum required, atleast src, rate , srcType are required, argc = " << argc << LOG_ENDL;
        return -1;
    }

    QString      hwSrc;
    QString      hwSink;
    QString      sampleRate;
    QString      srcType;
    EAudioSource sType = ENone;

    while(argc)
    {
        QString args;
        args.append(argv[argc]);
        if(args.contains("src="))          { hwSrc.append(args.mid(4));      }
        else if(args.contains("sink="))    { hwSink.append(args.mid(5));     }
        else if(args.contains("rate="))    { sampleRate.append(args.mid(5)); }
        else if(args.contains("srcType=")) { srcType.append(args.mid(8)); sType = (EAudioSource)srcType.toInt(); }
        argc--;
    }

    if(hwSrc.length() <= 0)
    {
        LOG_TRACE << "   no src set returning error" << LOG_ENDL;
        return -1;
    }
    else if(sampleRate.length() <= 0)
    {
        LOG_TRACE << "   no rate set returning error" << LOG_ENDL;
        return -1;
    }

    LOG_HIGH << "GSTAudioRouterWrapper::main(), src = " << hwSrc << ", sink = " << hwSink << ", rate = " << sampleRate << LOG_ENDL;

    QCoreApplication a(argc, argv);
    qDBusRegisterMetaType<EAudioSource>();

    CGSTAudioRouter* router = NULL;
    router = new CGSTAudioRouter();

    PipeLinehandler* interface = new PipeLinehandler(NULL , router);
    //interface->AquireAudioSource(hwSrc ,  sampleRate ,sType);

    //D-Bus interaction from Media/Pandora/Aha App
    new GSTAudioRouterDBusAdaptor(interface);
    QDBusConnection connection = QDBusConnection::sessionBus();

    bool ret = false;
    ret = connection.registerService("org.lge.GSTAudioRouterDBus");
    if(ret){ LOG_TRACE << "   GSTAudioRouterDBus, service registered" << LOG_ENDL; }
    else   { LOG_TRACE << "   GSTAudioRouterDBus, service not registered" << LOG_ENDL; }

    ret = connection.registerObject("/", interface);
    if(ret){ LOG_TRACE << "   GSTAudioRouterDBus, object registered" << LOG_ENDL; }
    else   { LOG_TRACE << "   GSTAudioRouterDBus, object not registered" << LOG_ENDL; }

    LOG_TRACE << "   started the routing" << LOG_ENDL;
    // { added by sungha.choi 2013.03.12 for gracefully GSTAudioWrapper quit
    struct sigaction sa;
    memset (&sa, 0x00, sizeof(sa));
    /* Use the sa_sigaction field because the handles has two additional parameters */
    sa.sa_sigaction = &signalHandler;
    /* The SA_SIGINFO flag tells sigaction() to use the sa_sigaction field, not sa_handler. */
    sa.sa_flags = SA_SIGINFO;

    if (sigaction(SIGINT, &sa, NULL) != 0)  { /* error */ }
    if (sigaction(SIGTERM, &sa, NULL) != 0) { /* error */ }

    int r =  a.exec();

    delete interface; interface = NULL;
    delete router;    router    = NULL;

    return r;
}
