/*
 * micomradiocontroladapter.h
 *
 *  Created on: 2011. 9. 3.
 *      Author: yongwoo1.park
 */

#ifndef MICOMRADIOCONTROLADAPTER_H_
#define MICOMRADIOCONTROLADAPTER_H_



#include <QObject>
#include <QDBusAbstractAdaptor>
#include <QDBusMessage>
#include <QDBusVariant>
#include <QDBusReply>
#include <protocol_common.h>


class MicomRadioControlAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO( "D-Bus Interface", "com.lge.car.micom.radioif" )

public:
    explicit MicomRadioControlAdaptor( QObject *parent = 0 );

public slots:
    void ProcessRadioControlMessage(const QDBusMessage& msg );
};


#endif /* MICOMRADIOCONTROLADAPTER_H_ */
