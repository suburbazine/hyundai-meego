
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
 * MODULE SUMMARY : D-Bus adaptor to listen d-bus command from
                     clients
 *----------------------------------------------------------------*
 */
#include "DHAVN_AudioRouterAdaptor.h"
#include <QDebug>

/*
 * Implementation of adaptor class GSTAudioRouterDBusAdaptor
 */

GSTAudioRouterDBusAdaptor::GSTAudioRouterDBusAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    USE_LOG_UTIL;
    LOG_INIT_V2("Media_IPodController");
    setAutoRelaySignals(true);
}

GSTAudioRouterDBusAdaptor::~GSTAudioRouterDBusAdaptor()
{
    // destructor
}

void GSTAudioRouterDBusAdaptor::AquireAudioSource( const QString& inSrc, const QString& inRate, const EAudioSource& inSrcType)
{
    LOG_TRACE << "GSTAudioRouterDBusAdaptor::AquireAudioSource() IN"<<LOG_ENDL;
    QMetaObject::invokeMethod(parent(), "AquireAudioSource", Q_ARG(QString, inSrc) ,Q_ARG(QString, inRate),Q_ARG(EAudioSource, inSrcType) );
}

void GSTAudioRouterDBusAdaptor::ReleaseAudioSource(const EAudioSource& inSrcType)
{
    LOG_TRACE << "GSTAudioRouterDBusAdaptor::ReleaseAudioSource() IN"<<LOG_ENDL;
    QMetaObject::invokeMethod(parent(), "ReleaseAudioSource",Q_ARG(EAudioSource, inSrcType));
}
//End of file
