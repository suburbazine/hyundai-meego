#include <QtCore/QCoreApplication>
#include <QtGlobal>
#include <QtDBus>

#include "comm_interface/spi_comm_thread.h"
#include "msg_processor.h"
#include "util.h"

#include "micomradiocontroladaptor.h"
#include "micomaudiocontroladaptor.h"
#include "powermanagement.h"
#include "sysinfo.h"
#include "system.h"
#include "lcd.h"
#include "tmc.h"
#include "dtc.h"
#include "dab.h"

#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>

#define SERVICE_NAME                "com.lge.car.micom"

#include "micom_manager_table.h"

#ifdef	_SIMULATION
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#define UNIX_DOMAIN_PATH	"/tmp/ywpark_udsock"
#endif


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    int spi_fd;
	MMusUtil * mmusUtil = MMusUtil::instance();
#ifdef _SIMULATION
	struct sockaddr_un	servaddr;

	spi_fd = socket(AF_LOCAL, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_LOCAL;
	strcpy(servaddr.sun_path, UNIX_DOMAIN_PATH);
    connect(spi_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
#else
    spi_fd = open("/dev/micom", O_RDWR);
    if ( spi_fd < 0 ){
        mmusUtil->gf_err(SYS_ERR, "spi open fail");
        exit(1);
    }
	mmusUtil->gf_debug("Start MMUS--------------");
#endif
    
    
    CSpiCommThread * spiThread = new CSpiCommThread(spi_fd);

    CMsgProcessor * msgProcessor = CMsgProcessor::instance(spi_fd);

    QObject::connect(spiThread, SIGNAL(dataArrived(const QByteArray &)),
                msgProcessor, SLOT(parseIncomingSpiData(const QByteArray &)));
	QObject::connect(spiThread, SIGNAL(printDebug(const QByteArray &)),
				msgProcessor, SLOT(outputDebug(const QByteArray &)));

    spiThread->start(); // SPI Read Thread Start

    QDBusConnection sessionBus = QDBusConnection::sessionBus();

    if ( !sessionBus.isConnected() )
    {
        mmusUtil->gf_err(MY_ERR, "Cannot connect to the D-Bus session : %s",
               qPrintable(QDBusConnection::sessionBus().lastError().message()));
        exit(1);
    }

    if ( !sessionBus.registerService( SERVICE_NAME ) )
    {
        mmusUtil->gf_err(MY_ERR, "Cannot register service : %s",
               qPrintable(QDBusConnection::sessionBus().lastError().message()));
        exit(1);
    }

    new MicomAudioControlAdaptor( &app );
    new MicomRadioControlAdaptor( &app );

    PowerManagement powerManagement;
    SysInfo         sysInfo;
    System          system;
    Lcd             lcd;
    Tmc		    tmc;
    DTC		    dtc;
	DAB			dab;

    if ( !sessionBus.registerObject( "/", &app) )
    {
        mmusUtil->gf_err(MY_ERR, "Cannot register object : %s",
               qPrintable(QDBusConnection::sessionBus().lastError().message()));
    }

    if ( !sessionBus.registerObject( "/PowerManagement", &powerManagement)){
        mmusUtil->gf_err(MY_ERR, "Cannot register powermanagement object : %s",
               qPrintable(QDBusConnection::sessionBus().lastError().message()));
    }

    if ( !sessionBus.registerObject( "/SysInfo", &sysInfo)){
        mmusUtil->gf_err(MY_ERR, "Cannot register sysinfo object : %s",
               qPrintable(QDBusConnection::sessionBus().lastError().message()));
    }

    if ( !sessionBus.registerObject( "/System", &system)){
        mmusUtil->gf_err(MY_ERR, "Cannot register sysinfo object : %s",
               qPrintable(QDBusConnection::sessionBus().lastError().message()));
    }

    if ( !sessionBus.registerObject( "/Lcd", &lcd)){
        mmusUtil->gf_err(MY_ERR, "Cannot register sysinfo object : %s",
               qPrintable(QDBusConnection::sessionBus().lastError().message()));
    }

    if ( !sessionBus.registerObject( "/Tmc", &tmc)){
        mmusUtil->gf_err(MY_ERR, "Cannot register sysinfo object : %s",
               qPrintable(QDBusConnection::sessionBus().lastError().message()));
    }
    
	if ( !sessionBus.registerObject( "/Dtc", &dtc)){
        mmusUtil->gf_err(MY_ERR, "Cannot register dtc object : %s",
               qPrintable(QDBusConnection::sessionBus().lastError().message()));
    }

	if ( !sessionBus.registerObject( "/Dab", &dab)){
        mmusUtil->gf_err(MY_ERR, "Cannot register dab object : %s",
               qPrintable(QDBusConnection::sessionBus().lastError().message()));
    }

    return app.exec();
}
