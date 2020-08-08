#include <QDebug>
#include <QFile>
#include <QProcess>
#include "DABEPGReservationList.h"
#include "DABProtocol.h"
#include "DABApplication_Def.h"
#include "DABLogger.h"
#include "DABController.h"

#define TIMERINTERVAL 1000 // 1 second

DABEPGReservationList::DABEPGReservationList(DABController &aDABController, QObject *parent) :
    QObject(parent),
    m_DABController(aDABController)
{
    this->reservationCount = 0;
    pReservationList = NULL;
    pReservationList = new QList<DABEPGReservationItem*>;
}

DABEPGReservationList::~DABEPGReservationList(void)
{
    qDebug() << Q_FUNC_INFO;
}

void
DABEPGReservationList::init()
{
    qDebug() << Q_FUNC_INFO;
}

void
DABEPGReservationList::onCheckReservationItem(void)
{
    if(pReservationList->count() == 0)
    {
        return;
    }

    qDebug() << Q_FUNC_INFO << " :: m_DABController.getBroadcastingTimeActive() = " << m_DABController.getBroadcastingTimeActive();
#if 1
    QDate currentDate;
    QTime currentTime;
    if(m_DABController.getBroadcastingTimeActive())
    {
        QDateTime dateTime = DABLogger::instance()->getBroadcastingTime();
        currentDate = dateTime.date();
        currentTime = dateTime.time();
    }
    else
    {
        currentDate = QDate::currentDate();
        currentTime = QTime::currentTime();
    }
#else
    QDateTime dateTime = DABLogger::instance()->getBroadcastingTime();
    QDate currentDate = dateTime.date();
    QTime currentTime = dateTime.time();
#endif
    quint32 mjd = DABLogger::instance()->convertDateToMJD(currentDate);

    DABEPGReservationItem *pItem = (DABEPGReservationItem*)pReservationList->first();
    QTime reserveTime(pItem->getHour(), pItem->getMinute(), pItem->getSecond());

    qDebug() << __FUNCTION__ << "     Current MJD = " << mjd << ", Time :" << currentTime.toString();
    qDebug() << __FUNCTION__ << " Reservation MJD = " << pItem->getMJD() << " Time :" << reserveTime.toString();

    if(mjd == pItem->getMJD())
    {
        if((currentTime.hour() == reserveTime.hour()) && (currentTime.minute() == reserveTime.minute()))
        {
            qDebug() << " =================== onCheckReservationItem() :: CurrentTime and ReserveTime is Same!!";
            emit cmdEvtPreservedPopup(pItem->getProgramtitle(), pItem->getLabel());
            m_DABController.setReservationCount(0);
        }
        else if(currentTime > reserveTime)
        {
            qDebug() << " =================== onCheckReservationItem() :: CurrentTime is big than ReserveTime. so remove reserve item";
            removeItem();
        }
        else
        {
            qDebug() << " =================== onCheckReservationItem() :: CurrentTime and ReserveTime is Different!!";
        }
    }
    else
    {
        qDebug() << " CurrentDate and ReserveDate is Different!!";
    }
}

DABEPGReservationItem*
DABEPGReservationList::getReservationItem(void)
{
    qDebug() << Q_FUNC_INFO;
    DABEPGReservationItem *pItem = (DABEPGReservationItem*)pReservationList->first();
    return pItem;
}

bool
DABEPGReservationList::isEmpty(void)
{
    bool ret = pReservationList->isEmpty();
    return ret;
}

int
DABEPGReservationList::isAlreadyExistReservationList(QDate date, int serviceID, int hour, int minute, int second)
{
    qDebug() << Q_FUNC_INFO;         
    DABEPGReservationItem* reservation = NULL;
    QTime startTime(0,0,0);

    quint32 mjd = DABLogger::instance()->convertDateToMJD(date);

    QList<DABEPGReservationItem*>::iterator i;
    for(i = pReservationList->begin(); i != pReservationList->end(); i++)
    {
        reservation = i[0];
        if(mjd == reservation->getMJD())
        {
            startTime.setHMS(hour, minute, second);
            if(startTime == reservation->getStartTime() && (reservation->getServiceID() == serviceID))
            {
                return DAB_EPG_RESERVE_DUP_STARTTIME_SERVICE_ID;
            }
        }
    }
    return DAB_EPG_RESERVE_DUP_NO;
}

bool
DABEPGReservationList::removeItem(void)
{
    qDebug() << Q_FUNC_INFO;

    if(pReservationList->count() == 0)
    {
        qDebug() << __FUNCTION__ << " Reserviation count is zero!!. so return !!";
        return false;
    }

    DABEPGReservationItem *pItem = (DABEPGReservationItem*)pReservationList->first();
    pReservationList->removeFirst();
    reservationCount = pReservationList->count();
    m_DABController.setReservationCount(reservationCount);
    saveReservationItem();
    pItem->washOffData();
    m_DABController.setEPGReservationItem(pItem);
    return true;
}

void
DABEPGReservationList::addEPGReservation(DABEPGReservationItem *pItem, bool isSave)
{
    qDebug() << Q_FUNC_INFO;

    pReservationList->append(pItem);
    reservationCount = pReservationList->count();
    m_DABController.setReservationCount(reservationCount);

    if(isSave)
        saveReservationItem();

#if 0 //For Debugging
    DABEPGReservationItem *testItem;
    QList<DABEPGReservationItem*>::iterator k;
    int j=0;
    qDebug() << "============= Reserve Time List =============";
    for(k = pReservationList->begin(); k != pReservationList->end(); k++)
    {
        testItem = k[0];
        qDebug() << "ReserveList["<<j<<"] : " <<  testItem->getStartTime().toString("hh:mm:ss");
        j++;
    }
    qDebug() << "=============================================";
#endif
}

void
DABEPGReservationList::addRemoveEPGReservation(DABEPGReservationItem *pItem)
{
    qDebug() << Q_FUNC_INFO  << " Reservation list is Empty ? = " << pReservationList->isEmpty();
    pReservationList->removeFirst();
    qDebug() << __FUNCTION__ << " Reservation list is Empty ? = " << pReservationList->isEmpty();
    pReservationList->append(pItem);
    reservationCount = pReservationList->count();
    m_DABController.setReservationCount(reservationCount);
    saveReservationItem();
}

bool
DABEPGReservationList::reserveTimelessThan(void *arg1, void *arg2)
{
    DABEPGReservationItem *firstItem = (DABEPGReservationItem*)arg1;
    DABEPGReservationItem *secondItem = (DABEPGReservationItem*)arg2;

    if(firstItem->getMJD() < secondItem->getMJD())
    {
        return true;
    }
    else if(firstItem->getMJD() > secondItem->getMJD())
    {
        return false;
    }
    else
    {
        return firstItem->getStartTime() < secondItem->getStartTime();
    }
}

void
DABEPGReservationList::loadReservationItem(void)
{
    qDebug() << Q_FUNC_INFO;

    stEVT_EPG_RESERVE_ITEM data;

    int readSize = sizeof(data);
    memset(&data, 0x00, readSize);

/**
  2013.07.24 Mobilus DaehyungE
  방송 time 수신 안될 경우에는, 시스템 시간 보다가 방송에서 time내려오면 방송 time 보도록 수정
  */
#if 1
    QDateTime dateTime = QDateTime::currentDateTime();
#else
    QDateTime dateTime = DABLogger::instance()->getBroadcastingTime();
#endif

    QDate currentDate = dateTime.date();
    quint32 currentMJD = DABLogger::instance()->convertDateToMJD(currentDate);


    QFile filedata(DAB_EPG_RESERVATION);
    QDataStream out;
    out.setDevice(&filedata);

    DABLogger::instance()->Log((QString("  =====EPG Load START ===================================================================")));
    if(filedata.open(QIODevice::ReadOnly))
    {
        while(out.readRawData((char*)&data,readSize) == readSize)
        {
            QString programTitle = "";
            QString description = "";
            programTitle = QString::fromUtf8(data.programTitle, strlen(data.programTitle));
            description = QString::fromUtf8(data.description, strlen(data.description));
            DABLogger::instance()->Log((QString(" loadReservationItem() : mjd = %1, hour=%2, minute=%3, second=%4").arg(data.mjd).arg(data.hour).arg(data.minute).arg(data.second)));
            DABLogger::instance()->Log((QString(" loadReservationItem() : serivceID = %1, title = %2").arg(data.serviceID).arg(programTitle)));

            if(currentMJD <= data.mjd)
            {
                DABEPGReservationItem* pItem = new DABEPGReservationItem(this);
                pItem->setTime(data.hour, data.minute, data.second, data.duration);
                pItem->setProgramTitle(programTitle);
                pItem->setDescription(description);
                pItem->setServiceID(data.serviceID);
                pItem->setMJD(data.mjd);
                pItem->setSupplementData(&data);

                DABLogger::instance()->Log((QString(" loadReservationItem() : addEPGReservation() called !!")));
                this->addEPGReservation(pItem, false);
                m_DABController.setEPGReservationItem(pItem);
            }            
            else
            {
                if(!filedata.remove(DAB_EPG_RESERVATION))
                    DABLogger::instance()->Log((QString(" loadReservationItem() : reservation file remove Fail !!")));
                else
                    DABLogger::instance()->Log((QString(" loadReservationItem() : reservation file remove success !!")));
            }
            memset(&data, 0x00, readSize);
        }
    }
    else
    {
        DABLogger::instance()->Log((QString(" loadReservationItem() : File Not found: path = %1").arg(DAB_EPG_RESERVATION)));
        return;
    }

    filedata.close();
    DABLogger::instance()->Log((QString("  =====EPG Load END ===================================================================")));
}

void
DABEPGReservationList::saveReservationItem(void)
{
    qDebug() << Q_FUNC_INFO;

    DABEPGReservationItem* reservation = NULL;
    QList<DABEPGReservationItem*>::iterator i;

    QFile file(DAB_EPG_RESERVATION);

    if(file.exists())
    {
        qDebug() << __FUNCTION__ << " :: EPG File is exists. so remove!!";
        file.remove();
    }
    else
    {
        qDebug() << __FUNCTION__ << " :: EPG File is not exists!!";
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        qDebug() << Q_FUNC_INFO << "File Not found: ";
        return; //temporaty...
    }

    QDataStream out;
    out.setDevice(&file);

    stEVT_EPG_RESERVE_ITEM saveItem;
    int sizeSaveData = 0;

    sizeSaveData = sizeof(saveItem);
    memset(&saveItem, 0x00, sizeSaveData);

    for(i = pReservationList->begin(); i != pReservationList->end(); i++)
    {
        reservation = i[0];
        qDebug() << __FUNCTION__ << " Reservation item :: frequency = " << reservation->getFrequency();
        reservation->saveData(&saveItem);
        out.writeRawData((const char*)&saveItem, sizeSaveData);
        memset(&saveItem, 0x00, sizeSaveData);
    }

    file.close();
    DABLogger::instance()->sendProcessCommand("sync");
}
