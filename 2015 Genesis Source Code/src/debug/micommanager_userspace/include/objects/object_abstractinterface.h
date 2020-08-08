#ifndef OBJECT_ABSTRACTINTERFACE_H
#define OBJECT_ABSTRACTINTERFACE_H

#include <QObject>
#include "msg_processor.h"

class ObjectAbstractInterface : public QObject{
    Q_OBJECT
public:
    ObjectAbstractInterface(QObject * obj = 0) : QObject(obj){
        CMsgProcessor::instance()->addObjects(this);
    }
    virtual bool handleEvent(quint16, const QByteArray & data = QByteArray()) = 0;

};

#endif // OBJECT_ABSTRACTINTERFACE_H
