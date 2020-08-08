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


#include "DHAVN_AudioSourceType.h"

QDBusArgument &operator<<(QDBusArgument &argument, const EAudioSource& source)
{
    argument.beginStructure();

    argument << static_cast<int>(source);

    argument.endStructure();

    return argument;
 }


const QDBusArgument &operator>>(const QDBusArgument &argument, EAudioSource &dest)
{
    int a;
    argument.beginStructure();
    argument >> a;
    argument.endStructure();
    dest = static_cast<EAudioSource>(a);
    return argument;
}
