#ifndef DHAVN_APPSETTINGS_TIMEINFO_H
#define DHAVN_APPSETTINGS_TIMEINFO_H

#include <QObject>
#include <QTimer>
#include <QString>

class TimeInfo : public QObject
{
    Q_OBJECT
public:
    explicit TimeInfo(QObject *parent = 0);

    Q_INVOKABLE void initTimeInfo(int screen);
    Q_INVOKABLE int getDaysInMonth(int year, int month);

signals:
    void timeUpdated(int year, int month, int day, int hour, int minute, int timeType, int dayOfWeek, int screen);

private slots:
    void onTimeTick(int screen);

private:
    int currentYear;
    int currentMonth;
    int currentDay;
    int currentHour;
    int currentMinute;
    QTimer timeChangeTimer;

private:
    bool isLeapYear(int year);
};

#endif // DHAVN_APPSETTINGS_TIMEINFO_H
