#include "hdr/DHAVN_AppSettings_CMOSTManager.h"
#include "DHAVN_AppFramework_Logger_Def.h"

#include <QDBusReply>
#include <QByteArray>
#include <QStringList>
#include <QDebug>

QScopedPointer<CMOSTManager> CMOSTManager::instance;

CMOSTManager::CMOSTManager(QObject *parent) :
    QObject(parent)
  , mostInterface("com.dhavn.mostmanager.qtdbus",
                  "/hmi", "", QDBusConnection::sessionBus())
  , diagInterface("com.dhavn.mostmanager.qtdbus",
                  "/diagnosis", "", QDBusConnection::sessionBus())
{
    if (!this->mostInterface.isValid())
    {
        Logger::Instance()->Log(
                    "can't create MOST bus");
    }
}

CMOSTManager* CMOSTManager::getInstance()
{
    if (instance == NULL)
    {
        instance.reset(new CMOSTManager);
    }

    return instance.data();
}

bool CMOSTManager::sendLanguageToIBox(LANGUAGE_T languageId, uint unit)
{
    QString iboxLangId;

    switch (languageId)
    {
    case LANGUAGE_EN:
    case LANGUAGE_EN_US:
        iboxLangId = "EN1";
        break;
    case LANGUAGE_KO:
        iboxLangId = "KO0";
        break;
    case LANGUAGE_ZH_YUE:
    case LANGUAGE_ZH_CN:
        iboxLangId = "ZH1";
        break;
    case LANGUAGE_PT:
        iboxLangId = "PT0";
        break;
    case LANGUAGE_EN_GB:
        iboxLangId = "EN2";
        break;
    case LANGUAGE_FR:
        iboxLangId = "FR0";
        break;
    case LANGUAGE_IT:
        iboxLangId = "IT0";
        break;
    case LANGUAGE_DE:
        iboxLangId = "DE0";
        break;
    case LANGUAGE_ES:
        iboxLangId = "ES0";
        break;
    case LANGUAGE_RU:
        iboxLangId = "RU0";
        break;
    case LANGUAGE_NL:
        iboxLangId = "NL0";
        break;
    case LANGUAGE_SV:
        iboxLangId = "SV0";
        break;
    case LANGUAGE_PL:
        iboxLangId = "PL0";
        break;
    case LANGUAGE_TR:
        iboxLangId = "TR0";
        break;
    case LANGUAGE_CS:
        iboxLangId = "CS0";
        break;
    case LANGUAGE_DA:
        iboxLangId = "DA0";
        break;
    case LANGUAGE_SK:
        iboxLangId = "SK0";
        break;
    case LANGUAGE_AR:
        iboxLangId = "AR0";
        break;
    case LANGUAGE_FR_NA:
        iboxLangId = "FR0";
        break;
    case LANGUAGE_ES_NA:
        iboxLangId = "ES0";
        break;
    default:
        Logger::Instance()->Log(
                    QString("CMOSTManager:unknown language code=%1"
                    ).arg(languageId));
        return false;
    }

    Logger::Instance()->Log(QString(
                        "CMOSTManager::language to iBox unit=%1 languageId=%2"
                        ).arg(unit).arg(iboxLangId));

    QDBusReply<bool> reply = this->mostInterface.call("SetLanguage", iboxLangId);
    return reply.value();
}

bool CMOSTManager::sendDistanceUnitToIBox(uint distanceUnit)
{
  bool result = true;
  QDBusReply<bool> reply = this->mostInterface.call("SetDistanceUnit", distanceUnit);

  if (!reply.value())
  {
      Logger::Instance()->Log("CMOSTManager::unable to setup distanceUnit");
      result = false;
  }

  return result;
}

bool CMOSTManager::sendDateTimeToIBox(uchar timeFormat,
                                      uint year,
                                      uint month,
                                      uint day,
                                      uint hour,
                                      uint minute,
                                      uint second,
                                      uint dateFormat)
{
    Logger::Instance()->Log(
                        "CMOSTManager::sendDateTimeToIBox "
                        );

    bool result = true;

    //qDebug("sendDateTimeToIBox, timeFormat = %d,", timeFormat);
    //qDebug("sendDateTimeToIBox, year = %d,", year);
    //qDebug("sendDateTimeToIBox, month = %d,", month);
    //qDebug("sendDateTimeToIBox, day = %d,", day);
    //qDebug("sendDateTimeToIBox, hour = %d,", hour);
    //qDebug("sendDateTimeToIBox, minute = %d,", minute);
    //qDebug("sendDateTimeToIBox, second = %d,", second);

    QByteArray timeArray;
    timeArray.append( (char) timeFormat );
    timeArray.append( (char) 0xFF & hour );
    timeArray.append( (char) 0xFF & minute );
    timeArray.append( (char) 0xFF & second );


    for (int i = 0; i < timeArray.size(); ++i) {
            qDebug("** timeArray[i] = %d,", (int)timeArray.at(i));
    }

    QDBusReply<bool> reply = this->mostInterface.call("SetTime", timeArray);

    if (!reply.value())
    {
        Logger::Instance()->Log(
                            "CMOSTManager::unable to setup new time"
                            );
        result = false;
    }
    /*
    QStringList dateArray;

    dateArray.append(QString::number(year));
    dateArray.append(QString::number(month));
    dateArray.append(QString::number(day));

    reply = this->mostInterface.call("SetDate", dateArray, dateFormat);
    */

    // Add for send date to IBox immediately//
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    QDBusVariant systemDate;

    QVariant var;
    QDBusArgument vArg;

    vArg.beginStructure();
    vArg << year << month << day << dateFormat;
    vArg.endStructure();
    var.setValue(vArg);
    systemDate.setVariant(var);

    reply = this->mostInterface.call("SetDate",QVariant::fromValue(systemDate));

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (!reply.value())
    {
        Logger::Instance()->Log(
                            "CMOSTManager::unable to setup new date"
                            );
        result = false;
    }

    return result;
}
// added for modified ibox time set 02/05
bool CMOSTManager::NewsendDateTimeToIBox(uchar timeFormat,
                                      uint year,
                                      uint month,
                                      uint day,
                                      uint hour,
                                      uint minute,
                                      uint second,
                                      uint dateFormat,
                                      uint nSummerTime,
                                      int nTimeZone)
{
    Logger::Instance()->Log(
                        "CMOSTManager::NewsendDateTimeToIBox"
                        );

    bool result = true;

    //qDebug("sendDateTimeToIBox, timeFormat = %d,", timeFormat);
    //qDebug("sendDateTimeToIBox, year = %d,", year);
    //qDebug("sendDateTimeToIBox, month = %d,", month);
    //qDebug("sendDateTimeToIBox, day = %d,", day);
    //qDebug("sendDateTimeToIBox, hour = %d,", hour);
    //qDebug("sendDateTimeToIBox, minute = %d,", minute);
    //qDebug("sendDateTimeToIBox, second = %d,", second);

    QByteArray timeArray;
    timeArray.append( (char) timeFormat );
    timeArray.append( (char) 0xFF & hour );
    timeArray.append( (char) 0xFF & minute );
    timeArray.append( (char) 0xFF & second );


    /*
    for (int i = 0; i < timeArray.size(); ++i) {
            qDebug("** timeArray[i] = %d,", (int)timeArray.at(i));
    }
    */

    QDBusReply<bool> reply = this->mostInterface.call("SetTime", timeArray, nTimeZone, nSummerTime );

    if (!reply.value())
    {
        Logger::Instance()->Log(
                            "CMOSTManager::sendDateTimeToIBox unable to setup new time"
                            );
        result = false;
    }
    /*
    QStringList dateArray;

    dateArray.append(QString::number(year));
    dateArray.append(QString::number(month));
    dateArray.append(QString::number(day));

    reply = this->mostInterface.call("SetDate", dateArray, dateFormat);
    */

    // Add for send date to IBox immediately//
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    QDBusVariant systemDate;

    QVariant var;
    QDBusArgument vArg;


    vArg.beginStructure();
    vArg << year << month << day << dateFormat;
    vArg.endStructure();
    var.setValue(vArg);
    systemDate.setVariant(var);

    reply = this->mostInterface.call("SetDate",QVariant::fromValue(systemDate));

    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (!reply.value())
    {
        Logger::Instance()->Log(
                            "CMOSTManager::sendDateTimeToIBox unable to setup new date"
                            );
        result = false;
    }

    return result;
}
// added for modified ibox time set 02/05

bool CMOSTManager::requestIBoxVersion()
{
   //qDebug() << "CMOSTManager::requestIBoxVersion()";
   if ( diagInterface.isValid() )
   {
      //qDebug() << "CMOSTManager::requestIBoxVersion() interface is valid";
      connect( &diagInterface, SIGNAL(MainSWVersionStatus(const QStringList)), this, SIGNAL(iboxVersionUpdated(const QStringList)) );

   }

   QDBusReply<bool> reply1 = diagInterface.call("GetMainSWVersion");
   //qDebug() << "diagInterface.call(GetMainSWVersion) = " << reply1.value();

   return reply1.value();
}

bool CMOSTManager::sendKeyboardToIBox(uint korean, uint english_us, uint inglish_eu, uint chinese,
                                      uint activeKeyboard)
{
    //qDebug() << "CMOSTManager::sendKeydoeadToIBox try to send keyboard to iBox";

    bool result = true;

    QVariant keyBoardDataVar;
    QDBusVariant dBuskeyBoardDataVar;
    QList<uint> keyboardList;

    /*
    //added for iBox -> HU Default Keyboard Settings Issue
    keyboardList.append(activeKeyboard); //At 0 position  keyboard  setting value.
    keyboardList.append(korean); // At 1 position Korean keyboard value.
    keyboardList.append(english_us);//At 2 position English US keyboard value.
    keyboardList.append(inglish_eu); //At 3 position English EU keyboard value.
    keyboardList.append(chinese); //At 4 position Chinese keyboard value.
    //added for iBox -> HU Default Keyboard Settings Issue

    */

    
    keyboardList.append(korean); // At 0 position Korean keyboard value.
    keyboardList.append(english_us);//At 1 position English US keyboard value.
    keyboardList.append(inglish_eu); //At 2 position English EU keyboard value.
    keyboardList.append(chinese); //At 3 position Chinese keyboard value.
    keyboardList.append(activeKeyboard); //At 4 position  keyboard  setting value.
    QDBusArgument keyBoardDataArg;
    keyBoardDataArg << keyboardList;
    keyBoardDataVar.setValue(keyBoardDataArg);
    dBuskeyBoardDataVar.setVariant(keyBoardDataVar);

    QDBusReply<bool> reply = this->mostInterface.call("SetKeyBoard",QVariant::fromValue(dBuskeyBoardDataVar));

    if (!reply.value())
    {
        //Logger::Instance()->Log("CMOSTManager::sendDateTimeToIBox unable to setup new values");
        result = false;
    }
    return result;
 }

bool CMOSTManager::sendBeepSettingToIBox(const uint beepSetting)
{
    //qDebug() << "CMOSTManager::sendBeepSettingToIBox Value = " << beepSetting;

    bool result = true;

    QDBusReply<bool> reply = this->mostInterface.call("SetBeepSetting", beepSetting);

    if (!reply.value())
    {
        //Logger::Instance()->Log("CMOSTManager::sendBeepSettingToIBox unable to setup new values");
        result = false;
    }

    return result;
}

bool CMOSTManager::sendPhonePrioritySettingToIBox(const uint phonePriority)
{
    //qDebug() << "CMOSTManager::sendPhonePrioritySettingToIBox Value = " << phonePriority;

    bool result = true;

    QDBusReply<bool> reply = this->mostInterface.call("SetPhonePriority", phonePriority);

    if (!reply.value())
    {
        //Logger::Instance()->Log("CMOSTManager::sendPhonePrioritySettingToIBox unable to setup new values");
        result = false;
    }

    return result;
}

void CMOSTManager::ConnectToMostBoardSetting(QObject* listener)
{
    connect(&this->mostInterface,SIGNAL(KeyBoardSetting(QDBusVariant)),listener,SLOT(KeyBoardSettingChangedSlot(QDBusVariant)));
}

bool CMOSTManager::sendResetSettingToIBox (const uint senderHandle)
{
    bool result = true;
    QDBusReply<bool> reply = this->mostInterface.call("SetSystemDefaultSet", senderHandle);

    if (!reply.value())
    {
        //Logger::Instance()->Log("return false SetSystemDefaultSet", Logger::PRIORITY_HI);
        result = false;
    }

    return result;
}
