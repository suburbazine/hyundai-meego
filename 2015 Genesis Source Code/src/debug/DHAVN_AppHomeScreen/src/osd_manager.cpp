#include "osd_manager.h"
#include <osdwriter.h>
#include <QCoreApplication>


OSD_manager::OSD_manager(QObject *parent) :
    QObject(parent)
{
}

void OSD_manager::sendOSD(QString osd_message)
{
    HS_LOG(osd_message);
    OSDWriter::SendOSD(osd_message);
}

void OSD_manager::OSD_AppsBooting(int display)
{
    QString str = "@OSD:";
    appendScreenString(display, str);
    str.append("^COMMON^WARN^");
    str.append( qApp->translate( "main", "STR_APP_INITIALIZING" ) );
    sendOSD(str);
}

inline void OSD_manager::appendScreenString(int d, QString & str)
{
    if ( d == 1 )
    {
        str.append("F");
    }

    else if ( d == 2 )
    {
        str.append("R");
    }
}
