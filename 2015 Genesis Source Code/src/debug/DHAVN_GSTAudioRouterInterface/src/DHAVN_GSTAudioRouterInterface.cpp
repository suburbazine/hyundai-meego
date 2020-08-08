/*
 ******************************************************************
 *      COPYRIGHT  2013, LG Electronics.
 *	    All Rights Reserved
 *----------------------------------------------------------------*
 * MODULE     :   D-Bus Interface between AudioRouterWraper                                                *
 * PROGRAMMER : Radhakrushna Mohanty                              *
 * DATE       : 21/12/2011                                        *
 * VERSION    : 1.0                                               *
 *----------------------------------------------------------------*
 * MODULE SUMMARY :                                               *
 *  This module is responsible for communicating from AVP/PANDORA/AHA
 *  to AudioRouterWraper for audio pipeline aquire and release.
 *----------------------------------------------------------------*
 */

#include "DHAVN_GSTAudioRouterInterface.h"

/*
 * Implementation of interface class GSTAudioRouterDBus
 */

GSTAudioRouterDBus::GSTAudioRouterDBus(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
      qDBusRegisterMetaType<EAudioSource>();
}

GSTAudioRouterDBus::~GSTAudioRouterDBus()
{
}

