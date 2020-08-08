#ifndef DHAVN_APPSTANDBY_UPDATECLOCK_H
#define DHAVN_APPSTANDBY_UPDATECLOCK_H

#include <QDeclarativeView>
#include <QTranslator>
#include <QApplication>
#include <QGraphicsObject>

#include <DHAVN_AppFramework_AppEngineQML_Def.h>
#include <QDateTime>
#include <QString>
#include <DHAVN_AppStandBy_Log_Def.h>
#include "DHAVN_AppStandBy_LoadSettings.h"
#include <uistate_manager_def.h>
#include <QtDBus/QtDBus>

class StandByAppEngine;
class StandByApp: public QObject
{
   Q_OBJECT
   Q_ENUMS( CLOCK_DATEFORMAT_T);

public:
   StandByApp(){
       m_pLoadStandBySetting = NULL;
       m_pApplicationEngine = 0;
       m_pKeyType12hTime= 0;
       m_pIsSummerTime = 0;
       m_pBackToScreen = 0;
       m_pKeyPathImage = "";
       m_pFileLoad = "";
       m_pKeyTypeDateFormat = 0;
       m_pPhotoFrame = 1;
       m_pKeyTypeClock = 1;
       m_pIsSummerTimeSet = 0;      // 0: false     1: true(set)
       isFG = false;
       hour = -1;
       minute = -1;
       update12H = false;
       m_bGPSTime = true;
       m_bSendInvalid = false;
};
   StandByApp(StandByAppEngine *m_pApplicationEngine, QObject *parent = NULL);
   ~StandByApp();

    Q_INVOKABLE void TapOnScreen();

   enum CLOCK_DATEFORMAT_T
   {
      CLOCK_DATEFORMAT_YYYYMMDD_DASH = 0,
      CLOCK_DATEFORMAT_MMDDYYYY_DASH,
       CLOCK_DATEFORMAT_YYYYMMDD_SLASH,
       CLOCK_DATEFORMAT_MMDDYYYY_SLASH,
       CLOCK_DATEFORMAT_YYYYMMDD_DOT,
       CLOCK_DATEFORMAT_MMDDYYYY_DOT,
       CLOCK_DATEFORMAT_DDMMYYYY_DASH,
       CLOCK_DATEFORMAT_DDMMYYYY_SLASH,
       CLOCK_DATEFORMAT_DDMMYYYY_DOT
   };

private:
   StandByAppEngine* m_pApplicationEngine;

signals:
   void dataTimeSync(int cur_hour,int cur_minute);
   void setImage(QString m_pKeyPathImage);
   void dataDateSync(int cur_day,int cur_dayOfWeek,int cur_month,int cur_year);
   void dataLocationSync(QString cur_area);

public slots:
   void updateTimeAndDate();
   void updateTimer(bool val);
   void afterBoot();
   void setGPSTime(bool);
   void getSystemSettings();
   void updateSystemSettings( EVENT_SETTINGS_CHANGED_DATA_T *  message );
   void fileLoad( bool );

public:
   int m_pKeyType12hTime;
   bool m_pIsSummerTime;
   bool m_bGPSTime;
   bool m_bSendInvalid;
   int m_pIsSummerTimeSet;
   QString m_pKeyPathImage;
   QString m_pFileLoad;
   bool m_pBackToScreen;
   bool isFG;
   int hour;
   int minute;
   bool update12H;

   int m_pKeyTypeDateFormat;
   int m_pPhotoFrame;
   int m_pKeyTypeClock;

   /* Beep */
   QDBusMessage msg;

//   QScopedPointer<QTimer> m_timer;      // appEnginee timer 사용
//   QScopedPointer<LoadStandBySetting> m_pLoadStandBySetting;
   LoadStandBySetting * m_pLoadStandBySetting;

   void getSystemTime();
   void getSystemDate();

   Q_PROPERTY(QString fileLoad READ getFileLoad NOTIFY updatedFileLoad);
   Q_PROPERTY(int keyType12hTime READ getKeyType12hTime NOTIFY updatedKeyType12hTime);
   Q_PROPERTY(int keyTypeDateFormat READ getKeyTypeDateFormat NOTIFY updatedKeyTypeDateFormat);
   Q_PROPERTY(QString keyPathImage READ getKeyPathImage NOTIFY updatedKeyPathImage);

   int getKeyType12hTime();
   QString getKeyPathImage();
   QString getFileLoad();
   int getKeyTypeDateFormat();
   void setKeyType12hTime(int v);
//   void setKeyCalendar(int v);
   void setKeyPathImage(const QString & v);
   void setFileLoad(const QString & v);
   void setKeyTypeDateFormat(int v);

signals:
   void updatedKeyType12hTime();
   void updatedKeyPathImage();
   void updatedFileLoad();
   void updatedKeyTypeDateFormat();

};

#endif // DHAVN_APPSTANDBY_UPDATECLOCK_H
