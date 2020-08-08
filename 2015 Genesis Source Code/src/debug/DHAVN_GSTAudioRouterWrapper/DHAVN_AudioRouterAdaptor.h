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

#ifndef CAUDIOROUTERADAPTOR_H
#define CAUDIOROUTERADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include <QtCore/QMetaObject>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <DHAVN_LogUtil.h>
#include <DHAVN_AudioSourceType.h>

/*
 * Adaptor class for interface org.lge.GSTAudioRouterDBus
 */
class GSTAudioRouterDBusAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    USE_LOG_UTIL
    Q_CLASSINFO("D-Bus Interface", "org.lge.GSTAudioRouterDBus")
    Q_CLASSINFO("D-Bus Introspection", "" "")
public:
    GSTAudioRouterDBusAdaptor(QObject *parent);
    virtual ~GSTAudioRouterDBusAdaptor();

public Q_SLOTS: // METHODS
    Q_INVOKABLE void AquireAudioSource( const QString &inSrc, const QString& inRate,
                                        const EAudioSource& inSrcType);
    Q_INVOKABLE void ReleaseAudioSource(const EAudioSource& inSrcType);

Q_SIGNALS: // SIGNALS
    void AquireStatus(bool status , const EAudioSource& inSrcType);
    void ReleaseStatus(bool status , const EAudioSource& inSrcType);
};

#endif //
