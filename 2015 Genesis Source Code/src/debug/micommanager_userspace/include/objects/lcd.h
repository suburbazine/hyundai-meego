#ifndef LCD_H
#define LCD_H

#include "object_abstractinterface.h"
#include "lcd_adaptor.h"
#include "util.h"
#include <QDBusContext>

class Lcd : public ObjectAbstractInterface, public QDBusContext{
    Q_OBJECT
    LcdAdaptor * adaptor;
public:
    Lcd(QObject * obj = 0);
    bool handleEvent(quint16, const QByteArray & data = QByteArray());

public Q_SLOTS: // METHODS
    QByteArray LgeDBus_AskLcdSetInfoValue();
    void LgeDBus_SetBacklightControl(uchar in0);
    void LgeDBus_SetLcdPWMControl(uchar in0, uchar in1, uchar in2, uchar in3);
Q_SIGNALS: // SIGNALS
    void LgeDBus_EventLCDSetupInfo(const QByteArray &in0);

private:
	MMusUtil * mmusUtil;
};

#endif // LCD_H
