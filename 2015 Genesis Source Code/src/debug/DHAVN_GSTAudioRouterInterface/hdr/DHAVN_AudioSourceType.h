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
 * MODULE SUMMARY : Source type enums are declared here to be used
                    in Client and RouterWraper
 *----------------------------------------------------------------*
 */

#ifndef DHAVN_AUDIOSOURCETYPE_H
#define DHAVN_AUDIOSOURCETYPE_H

#include <QDBusArgument>

enum EAudioSource
{
    ENone = 0,
    EOther ,
    EIpodFront,
    EIpodRear,
    EPandoraFront,
    EPandoraRear,
    EAhaFront,
    EAhaRear,
    EAllSource // added by kihyung 2013.06.01
};

QDBusArgument &operator<<(QDBusArgument &argument, const EAudioSource& source);

const QDBusArgument &operator>>(const QDBusArgument &argument, EAudioSource &dest);

Q_DECLARE_METATYPE(EAudioSource);

#endif // DHAVN_AUDIOSOURCETYPE_H
