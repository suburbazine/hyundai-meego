#include "DHAVN_AppSettings_TimeInfo.h"
#include "DHAVN_AppSettings_Def.h"
#include <QTime>
#include <QDate>
#include <QDebug>

TimeInfo::TimeInfo(QObject *parent) :
    QObject(parent),
    currentYear(0),
    currentMonth(0),
    currentDay(0),
    currentHour(0),
    currentMinute(0)
{
    //connect(&this->timeChangeTimer, SIGNAL(timeout()), SLOT(onTimeTick()));

    //this->timeChangeTimer.setInterval(1000); // Wait 1 sec.
}

void TimeInfo::onTimeTick(int screen)
{
    bool needNotify = false;
    int year, month, day, hour, minute;
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();

    date.getDate(&year,&month,&day);
    hour = time.hour();
    minute = time.minute();
    int dayOfWeek = date.dayOfWeek();

    if (year != this->currentYear)
    {
        this->currentYear = year;
        needNotify = true;
    }

    if (month != this->currentMonth)
    {
        this->currentMonth = month;
        needNotify = true;
    }

    if (day != this->currentDay)
    {
        this->currentDay = day;
        needNotify = true;
    }

    if (hour != this->currentHour)
    {
        this->currentHour = hour;
        needNotify = true;
    }

    if (minute != this->currentMinute)
    {
        this->currentMinute = minute;
        needNotify = true;
    }

    int timeType;
    if (this->currentHour >= 12)
    {
        timeType = SettingsDef::SETTINGS_TIME_TYPE_PM;
    }
    else
    {
        timeType = SettingsDef::SETTINGS_TIME_TYPE_AM;
    }

    emit timeUpdated(this->currentYear, this->currentMonth, this->currentDay,
                     hour, this->currentMinute, timeType, dayOfWeek, screen);

}

void TimeInfo::initTimeInfo(int screen)
{
    //qDebug() << "TimeInfo::initTimeInfo start time monitoring";

    onTimeTick(screen);
    //this->timeChangeTimer.start(); //timer-start  should be deleted according to comment of HFE
}

int TimeInfo::getDaysInMonth(int year, int month)
{
    switch (month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        return 31;
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    case 2:
        return isLeapYear(year) ? 29 : 28;
    default:
        return 0;
    }
}

bool TimeInfo::isLeapYear(int year)
{
    if (((year % 4) == 0 && (year % 100) != 0) || ((year % 400) == 0))
    {
        return true;
    }

    return false;
}
