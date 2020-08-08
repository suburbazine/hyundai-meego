#ifndef DABEPGRESERVATIONLIST_H
#define DABEPGRESERVATIONLIST_H

#include <QObject>
#include <qtimer.h>
#include <QtAlgorithms>
#include <QList>
#include <QTime>
#include "DABEPGItem.h"
#include "DABEPGReservationItem.h"
#ifndef __DAB_DEBUG_MSG_OUTPUT_FILE__
#include <DHAVN_LogUtil.h>
#endif

class DABController;
class DABEPGReservationList : public QObject
{
#ifndef __DAB_DEBUG_MSG_OUTPUT_FILE__
    USE_LOG_UTIL
#endif
    Q_OBJECT

private:
    QList<DABEPGReservationItem*> *pReservationList;
    QTime currentTime;
    DABController &m_DABController;

public:
    explicit DABEPGReservationList(DABController &aDABController, QObject *parent=0);
    ~DABEPGReservationList();
    void init();
    void addEPGReservation(DABEPGReservationItem *pItem, bool isSave);
    void addRemoveEPGReservation(DABEPGReservationItem *pItem);
    int isAlreadyExistReservationList(QDate date, int serviceID, int hour, int minute, int second);
    bool removeItem(void);
    static bool reserveTimelessThan(void *arg1, void *arg2);
    int reservationCount;  //#KEH - (20130108)
    void saveReservationItem(void);
    void loadReservationItem(void);
    DABEPGReservationItem* getReservationItem(void);
    bool isEmpty(void);

signals:
    void cmdEvtPreservedPopup(QString title, QString serivceName);

public slots:
    void onCheckReservationItem(void);

};

#endif // DABEPGRESERVATIONLIST_H
