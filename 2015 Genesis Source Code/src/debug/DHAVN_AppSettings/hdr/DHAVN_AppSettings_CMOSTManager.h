#ifndef DHAVN_APPSETTINGS_CMOSTMANAGER_H
#define DHAVN_APPSETTINGS_CMOSTMANAGER_H

#include <QObject>
#include <QDBusInterface>
#include <QString>
#include <QStringList>

#include "DHAVN_AppSettings_Def.h"

class CMOSTManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(CMOSTManager)
signals:
   void iboxVersionUpdated( const QStringList version );
private:
    explicit CMOSTManager(QObject *parent = 0);

public:
    static CMOSTManager *getInstance();

    bool sendLanguageToIBox(LANGUAGE_T languageId, uint unit);
    bool sendDateTimeToIBox(uchar timeFormat,
                            uint year,
                            uint month,
                            uint day,
                            uint hour,
                            uint minute,
                            uint second,
                            uint dateFormat);
    // added for modified ibox time set 02/05
    bool NewsendDateTimeToIBox(uchar timeFormat,
                            uint year,
                            uint month,
                            uint day,
                            uint hour,
                            uint minute,
                            uint second,
                            uint dateFormat,
                            uint nSummerTime,
                            int nTimeZone);
    // added for modified ibox time set 02/05

    bool sendDistanceUnitToIBox(uint distanceUnit);

    bool requestIBoxVersion();
    bool sendKeyboardToIBox(uint korean, uint english_us, uint inglish_eu, uint chinese,
                                          uint activeKeyboard);
    void ConnectToMostBoardSetting(QObject* listener);

    bool sendBeepSettingToIBox(const uint beepSetting);
    bool sendPhonePrioritySettingToIBox(const uint phonePriority);

    bool sendResetSettingToIBox (const uint senderHandle);

private:
    static QScopedPointer<CMOSTManager> instance;

    QDBusInterface mostInterface;
    QDBusInterface diagInterface;
};

#endif // DHAVN_APPSETTINGS_CMOSTMANAGER_H
