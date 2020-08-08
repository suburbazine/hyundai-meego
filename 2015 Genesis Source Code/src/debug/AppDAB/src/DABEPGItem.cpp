/**
 * FileName: DABEPGItem.cpp
 * Author: David.Bae
 * Time: 2011-09-29 14:23
 * Description:
 *          This file is used for EPG Data.
 */

#include <QTextStream>
#include <QDebug>
#include "DABEPGItem.h"

DABEPGItem::DABEPGItem( QObject *parent):
    ListItem(parent)
{
    washOffData();
}

DABEPGItem::DABEPGItem(DABEPGItem* a, QObject *parent):
ListItem(parent)
{
//    this->setHour(a->hour());
//    this->setMinute(a->minute());
//    this->setSecond(a->second());
//    this->setDuration(a->duration());
    this->setTime(a->hour(), a->minute(), a->second(), a->duration());
    this->setProgramTitle(a->programtitle());
    this->setDescription(a->description());
    this->setServiceID(a->getServiceID());
    this->setFrequencyID(a->getFrequencyID());

    this->subChId = 0;
}

QHash<int, QByteArray>
DABEPGItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[HourRole] = "hour";
    names[MinuteRole] = "minute";
    names[SecondRole] = "second";
    names[DurationRole] = "duration";
    names[ProgramTitleRole] = "programtitle";
    names[DescriptionRole] = "description";
    return names;
}

QVariant
DABEPGItem::data(int role) const
{
    switch(role) {
    case HourRole:
        return hour();
    case SecondRole:
        return second();
    case MinuteRole:
        return minute();
    case DurationRole:
        return duration();
    case ProgramTitleRole:
        return programtitle();

    case DescriptionRole:
        return description();
    default:
        return QVariant();
    }
}
void
DABEPGItem::setTime(int h, int m, int s, int d)
{
    setHour(h);
    setMinute(m);
    setSecond(s);
    setDuration(d);
    this->startTime.setHMS(h,m,s);
    this->endTime = startTime.addSecs(d);
}

DABEPGItem &
DABEPGItem::operator =(DABEPGItem& a)
{
//    this->setHour(a.hour());
//    this->setMinute(a.minute());
//    this->setSecond(a.second());
//    this->setDuration(a.duration());
    qDebug() << Q_FUNC_INFO << " serviceID = " << a.getServiceID();
    this->setTime(a.hour(), a.minute(), a.second(), a.duration());
    this->setProgramTitle(a.programtitle());
    this->setDescription(a.description());
    this->setServiceID(a.getServiceID());
    this->setFrequencyID(a.getFrequencyID());
    return *this;
}

void
DABEPGItem::washOffData(void)
{
    this->Hour = 0;
    this->Minute = 0;
    this->Second = 0;
    this->Duration = 0;
    this->startTime.setHMS(0,0,0);
    this->endTime.setHMS(0,0,0);
    this->ProgramTitle = "";
    this->Description = "";
    this->serviceID = 0;
    this->frequencyID = 0; //# 20130327 prevent
    this->subChId = 0;
}

void
DABEPGItem::saveData(stEVT_EPG_SAVE_ITEM *pData)
{
    pData->sHour = this->Hour;
    pData->sMinute = this->Minute;
    pData->sSecond = this->Second;
    pData->duration = this->Duration;
    pData->serviceID = this->serviceID;
    pData->frequencyID = this->frequencyID;

    QByteArray title = this->ProgramTitle.toUtf8();
    snprintf(pData->programTitle, sizeof(pData->programTitle), "%s", title.data());  //# 20130327 prevent

    QByteArray desc = this->Description.toUtf8();
    snprintf(pData->description, sizeof(pData->description), "%s", desc.data());    //# 20130327 prevent
}
