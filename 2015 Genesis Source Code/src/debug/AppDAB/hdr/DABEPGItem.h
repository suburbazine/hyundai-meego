#ifndef DABEPGITEM_H
#define DABEPGITEM_H
/**
 * FileName: DABEPGItem.h
 * Author: David.Bae
 * Time: 2011-09-29 14:11
 * Description:
 *          This file is used for EPG Data.
 */
#include <QObject>
#include <QTextStream>
#include <QTime>
#include "ListModel.h"
#include "DABProtocol.h"

class DABEPGItem : public ListItem
{
    Q_OBJECT
public:
    enum Roles { //this list is defined in DAB_HOST_IF_SPEC_VER[0.5].pdf
        HourRole = Qt::UserRole+1,
        MinuteRole,
        SecondRole,
        DurationRole,
        ProgramTitleRole,
        DescriptionRole
    };

    DABEPGItem( QObject *parent=0);
    DABEPGItem(DABEPGItem* a, QObject *parent = 0);

    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;
    inline QString id() const { return QString("%1:%2:%3:%4").arg(Hour).arg(Minute).arg(Second).arg(ProgramTitle); }
    inline quint32 key() const { return frequencyID; }
    inline quint8 subKey() const { return subChId; }
    inline quint8 ausFlag() const { return frequencyID; }
    inline QString ensemble() const { return ProgramTitle; }
    inline int hour(void) const { return this->Hour; }
    inline int minute(void) const { return this->Minute; }
    inline int second(void) const { return this->Second; }
    inline int duration(void) const { return this->Duration; }
    inline QTime getStartTime(void) const { return this->startTime;}
    inline QTime getEndTime(void) const { return this->endTime; }
    inline QString programtitle(void) const { return this->ProgramTitle;}
    inline QString description(void) const { return this->Description;}
    inline quint32 getServiceID(void) const { return this->serviceID;}
    inline quint32 getFrequencyID(void) const { return this->frequencyID;}

    void setHour(int h) { this->Hour = h; }
    void setMinute(int m) { this->Minute = m; }
    void setSecond(int s) { this->Second = s; }
    void setDuration(int d) { this->Duration = d; }
    void setTime(int h, int m, int s, int d);
    void setProgramTitle(QString title) { this->ProgramTitle = title; }
    void setDescription(QString desc) { this->Description = desc; }
    void setServiceID(quint32 serviceID){ this->serviceID = serviceID; }
    void setFrequencyID(quint32 frequencyID){ this->frequencyID = frequencyID; }

    DABEPGItem & operator =(DABEPGItem& a);

    void washOffData(void);
    void saveData(stEVT_EPG_SAVE_ITEM *pData);

signals:

public slots:

private:
    //
    int Hour;
    int Minute;
    int Second;
    int Duration;
    QTime startTime;
    QTime endTime;
    QString ProgramTitle;
    QString Description;
    quint32 serviceID;
    quint32 frequencyID;
    quint8  subChId;
};
#endif // DABEPGITEM_H
