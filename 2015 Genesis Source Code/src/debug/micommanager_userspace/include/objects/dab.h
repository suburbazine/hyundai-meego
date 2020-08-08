#ifndef DAB_H
#define DAB_H


#include "object_abstractinterface.h"
#include "dab_adaptor.h"
#include "util.h"
#include <QDBusContext>

class DAB : public ObjectAbstractInterface, public QDBusContext{
    Q_OBJECT
    DABAdaptor * adaptor;
public:
    DAB(QObject * obj = 0);
    bool handleEvent(quint16, const QByteArray & data = QByteArray());

public Q_SLOTS: // METHODS
    void LgeDBus_DABPICodeRequest(const QByteArray &in0);
    void LgeDBus_DABTAStatus(uchar in0);
    
Q_SIGNALS: // SIGNALS
    void LgeDBus_EventDABPISensitivity(const QByteArray &in0);
private:
	MMusUtil * mmusUtil;
};

#endif // SYSINFO_H
