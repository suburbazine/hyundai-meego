/*
 * micomaudiocontroladaptor.h
 *
 *  Created on: 2011. 9. 12.
 *      Author: yongwoo1.park
 */

#ifndef MICOMAUDIOCONTROLADAPTOR_H_
#define MICOMAUDIOCONTROLADAPTOR_H_

#include <QObject>
#include <QDBusAbstractAdaptor>
#include <QDBusMessage>
#include <QDBusVariant>
#include <QDBusReply>
#include <protocol_common.h>


class MicomAudioControlAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO( "D-Bus Interface", "com.lge.car.micom.audioif" )

public:
    explicit MicomAudioControlAdaptor( QObject *parent = 0 );

public slots:
    void ProcessAudioControlMessage(const QDBusMessage& msg );
};




#endif /* MICOMAUDIOCONTROLADAPTOR_H_ */
