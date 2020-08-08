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


#ifndef CGSTAUDIOROUTERINTERFACE_H
#define CGSTAUDIOROUTERINTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>
#include <DHAVN_AudioSourceType.h>
/*
 * Proxy class for interface org.lge.GSTAudioRouterDBus
 */
class GSTAudioRouterDBus: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.lge.GSTAudioRouterDBus"; }

public:
    GSTAudioRouterDBus(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~GSTAudioRouterDBus();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> AquireAudioSource(const QString& inSrc ,
                                                 const QString& inRate , const EAudioSource& inType)
    {
        qDebug() << "AquireAudioSource interface";
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(inSrc);
        argumentList << qVariantFromValue(inRate);
        argumentList << qVariantFromValue(inType);

        return asyncCallWithArgumentList(QLatin1String("AquireAudioSource"), argumentList);
    }

    inline QDBusPendingReply<> ReleaseAudioSource(const EAudioSource& inType)
    {
        qDebug() << "ReleaseAudioSource interface";
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(inType);
        return asyncCallWithArgumentList(QLatin1String("ReleaseAudioSource"),argumentList);
    }
Q_SIGNALS: // SIGNALS
    void AquireStatus(bool status , const EAudioSource& inType );
    void ReleaseStatus(bool status , const EAudioSource& inType );
};
#endif //CGSTAUDIOROUTERINTERFACE_H
