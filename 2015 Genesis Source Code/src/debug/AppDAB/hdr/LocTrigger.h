#ifndef LOCTRIGGER_H
#define LOCTRIGGER_H
#include <QDeclarativeView>
#include <QObject>
class LocTrigger : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString empty READ empty NOTIFY retrigger);

public:
    QString empty() { return QString(); }

signals:
   void retrigger();
};
#endif // LOCTRIGGER_H
