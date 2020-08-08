
#include <QtGui/QApplication>
#include <QtDBus/qdbusconnection.h>
#include <cdbuswrapper.h>
#include <QObject>
#include <QUrl>
#include <DHAVN_GracenoteCDLookupAdaptor.h>
#include "DHAVN_ApplicationInterface.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString variant;

    if(argc > 0)
    {
	// To support changes for CR 9792 in UISH, the variant will come in second arg
	// There 0 to modified to 1
        variant = argv[1];
    }

   CApplicationInterface* interface = new CApplicationInterface(variant);

    if (interface->Init()) {

        new CDBusWrapper(interface);

        QDBusConnection connection = QDBusConnection::sessionBus();
        bool ret = connection.registerService("org.lge.moreLikeThisWrapper");

        if(ret)
        {
            qDebug() << "MLT service registered";
        }
        else
            qDebug() << "MLT service not registered";


        //CD lookup D-Bus interaction from Media App
        new CGracenoteCDLookupDBusAdaptor(interface);
        
        ret = connection.registerService("org.lge.GracenoteCDLookupDBus");
        if(ret)
        {
            qDebug() << "CD Look up service registered";
        }
        else
            qDebug() << "CD Look up service not registered";        

        ret = connection.registerObject("/", interface);

        if(ret)
        {
            qDebug() << "object registered";
        }
        else
            qDebug() << "object not registered";       

    }

    a.exec();

    delete interface;
    interface = NULL;

    return 0;
}

// End of file
