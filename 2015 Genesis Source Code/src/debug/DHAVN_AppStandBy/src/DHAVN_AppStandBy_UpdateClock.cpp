#include <DHAVN_AppStandBy_UpdateClock.h>
#include <QTime>
#include <QDate>
#include "DHAVN_AppStandBy_LoadSettings.h"
#include "DHAVN_AppSettings_Def.h"
#include "DHAVN_AppSettings_Shared.h"
#include <DHAVN_AppStandBy_AppEngine.h>

#include <QDebug>

StandByApp::StandByApp( StandByAppEngine *app, QObject *parent )
   : QObject(parent)
 , m_pApplicationEngine(app)
 , m_pBackToScreen(false)
{
//   m_timer.reset(new QTimer());       // appEngineTimer 사용
    m_pLoadStandBySetting = NULL;
   m_pKeyType12hTime= 0;
   m_pKeyPathImage = "";
   m_pFileLoad = "";
   m_pKeyTypeDateFormat = 0;
   m_pPhotoFrame = 1;
   m_pKeyTypeClock = 0;
   m_pIsSummerTimeSet = 0;      // 0: false     1: true(set)
   m_bGPSTime = true;
   m_bSendInvalid = false;

//   connect(m_timer.data(), SIGNAL(timeout()), this, SLOT(updateTimeAndDate()));       // appEngien timer 사용
   connect(m_pApplicationEngine, SIGNAL(sendTimeoutToUpdateClock()), this, SLOT(updateTimeAndDate()));
   connect(m_pApplicationEngine, SIGNAL(afterBoot()), this, SLOT(afterBoot()));
   connect(m_pApplicationEngine, SIGNAL(appFG(bool)), this, SLOT(updateTimer(bool)));
   connect(m_pApplicationEngine, SIGNAL(setGPSTime(bool)), this, SLOT(setGPSTime(bool)));
   connect(m_pApplicationEngine, SIGNAL(setFG()), this, SLOT(getSystemSettings()));
   connect(m_pApplicationEngine, SIGNAL(settingsChanged( EVENT_SETTINGS_CHANGED_DATA_T *)), this, SLOT(updateSystemSettings( EVENT_SETTINGS_CHANGED_DATA_T *)));
   connect(m_pApplicationEngine, SIGNAL(setFile( bool )), this, SLOT(fileLoad( bool )));

   switch ( m_pApplicationEngine->getCountryVariant() )
   {
   case 0:
   case 2:
       setKeyTypeDateFormat(CLOCK_DATEFORMAT_YYYYMMDD_DASH);
       break;
   case 1:
   case 3:
   case 4:
   case 6:
       setKeyTypeDateFormat(CLOCK_DATEFORMAT_MMDDYYYY_DASH);
       break;
   case 5:
   case 7:
   default:
       setKeyTypeDateFormat(CLOCK_DATEFORMAT_DDMMYYYY_DASH);
       break;
   }

   /* Beep */
   msg = QDBusMessage::createMethodCall("com.lge.car.micom", "/", "com.lge.car.micom.audioif", "ProcessAudioControlMessage");
   QList<QVariant> args;
   args.append( (quint16) 0x0115 );
   args.append( (quint8) 0x0 );
   msg.setArguments( args );

   isFG = false;
   hour = -1;
   minute = -1;
   update12H = false;
}

StandByApp::~StandByApp()
{
}

void StandByApp::afterBoot()
{
    LW_LOG( "afterBoot() " );
    m_pPhotoFrame = m_pApplicationEngine->getPhotoFrame();
    getSystemSettings();
//    m_timer->start(1000);     // appEngien timer 사용
}

void
StandByApp::updateTimeAndDate()
{
    LW_LOG( "updateTimeAndDate() " );
      getSystemTime();
      getSystemDate();
}

void
StandByApp::fileLoad(bool val)
{
    QString file_name = "";
    LW_LOG(  "==> fileLoad(bool val) - val : " << val  );
   if (val)
   {
      LW_LOG( "==>  fileLoad() - before m_pPhotoFrame" << m_pPhotoFrame );
      switch ( m_pPhotoFrame )
      {
      case 0:               // Off
          file_name = "DHAVN_AppClock_PhotoFrame_OFF.qml";
          break;
      case 1:               // Clock
      {
          if ( m_pKeyTypeClock == 1 ) {
              file_name = "DHAVN_AppClock_Digital.qml" ;
              setKeyType12hTime( m_pKeyType12hTime );
          }
          else file_name = "DHAVN_AppClock_Analog.qml" ;
          break;
      }
      case 2:               // Photo
          file_name = "DHAVN_AppClock_PhotoFrame_Image.qml";
          break;
      default: break;
      }
   }
   else {
       LW_LOG(  "==> load Black Qml" );
       file_name = "DHAVN_AppClock_PhotoFrame_OFF.qml";
   }

   HS_LOG(  "==> fileLoad(bool val) - file_name : " << file_name  );
   getSystemSettings();
   setFileLoad(file_name);
   updateTimer( true );
}

void
StandByApp::updateTimer(bool val)
{
    LW_LOG(  "==>updateTimer()  m_pFileLoad : " << m_pFileLoad  );
    isFG = val;
    getSystemDate();
    getSystemTime();
}

void StandByApp::setGPSTime(bool val)
{
    LW_LOG(  "==>setGPSTime()  isGPSTime : " << val  );
    m_bGPSTime = val;
}

void
StandByApp::updateSystemSettings( EVENT_SETTINGS_CHANGED_DATA_T * message )
{
    int key = message->sKey ;
    LW_LOG(  "key  : " << key << ",   value : " <<  message->sValue );
    QString path_name = "";

    if ( m_pLoadStandBySetting == NULL )
    {
        LW_LOG( "m_pLoadStandBySetting.reset()" );
        m_pLoadStandBySetting = new LoadStandBySetting();
//        m_pLoadStandBySetting.reset(new LoadStandBySetting());
    }
    switch ( key )
    {
        case SettingsDef::DB_KEY_CLOCK_TYPE:        // analog / digital
        {
            m_pKeyTypeClock = message->sValue;
            LW_LOG("m_pKeyTypeClock : " << m_pKeyTypeClock );
            break;
        }
        case SettingsDef::DB_KEY_TIME_TYPE:         // 12h / 24h
        {
            m_pKeyType12hTime = message->sValue;
            setKeyType12hTime( m_pKeyType12hTime ) ;
            update12H = true;
            LW_LOG(  "m_pKeyType12hTime : " << m_pKeyType12hTime );
            break;
        }
        case SettingsDef::DB_KEY_PHOTO_FRAME:   // not see / clock / photo
        {
                m_pPhotoFrame = message->sValue;
                LW_LOG(  "m_pPhotoFrame : " << m_pPhotoFrame );
                if ( m_pPhotoFrame == 2 ) {
                    path_name = m_pLoadStandBySetting->LoadSettingString(SettingsDef::DB_KEY_PHOTO_FRAME_IMAGE);
                    LW_LOG(  "path_name : " << path_name );
                    setKeyPathImage( path_name );
                }
                break;
        }
        case SettingsDef::DB_KEY_SUMMER_TIME:
        {
            m_pIsSummerTimeSet = message->sValue;
            LW_LOG(  "m_pIsSummerTimeSet : " << m_pIsSummerTimeSet );
            break;
        }
        default: break;
    }
}

void StandByApp::getSystemTime()
{
   QTime time = time.currentTime();
   QDate curDate = curDate.currentDate();

   // setRTC
   if ( curDate.year() > 2011 && !m_bGPSTime && ( time.second() == 20 || time.second() == 50 ) ){       // 2012년도 이전 값일 경우 RTC 전송 안함
       const char data[7] = { 0,0,0,0,0,0,0 };
       QByteArray barray = QByteArray( data, 7 );
       if (  QDBusConnection::sessionBus().isConnected() )
       {
           QDBusInterface MicomRTCInterface( "com.lge.car.micom","/System", "com.lge.car.micom.system" );
           MicomRTCInterface.setTimeout(1000);
           MicomRTCInterface.call("LgeDBus_AskRTCInfo");
       }
   }

   // sendOSD
   QString strOSD = "@EVT^TIME_CHANGE^";
   if ( curDate.year() > 2011 ) {
       time = time.currentTime();
       int newH = time.hour();
       int newM  = time.minute();
       if ( hour != newH || minute != newM || update12H ) {
           update12H = false;
           hour = newH;
           minute = newM;
           QString ampm = "AM";
           if ( m_pKeyType12hTime == 0 && hour > 12 ) {
               newH -= 12;
               ampm = "PM";
           }
           if ( hour == 0) ampm = "AM";
           else if ( hour == 12 ) ampm = "PM";
           QString tmp;
           if ( m_pKeyType12hTime == 0 && newH == 0 ) newH = 12;
           strOSD.append( tmp.setNum(newH) );
           strOSD.append("^");
           strOSD.append(tmp.setNum( newM ) );
           if ( m_pKeyType12hTime == 0 ) {
               strOSD.append("^");
               strOSD.append(ampm);
           }
           OSDWriter::SendOSD(strOSD);
           m_bSendInvalid = false;
           LW_LOG("SendOSD(Valid Time) : " << strOSD << ", time.hour(): " << time.hour() << ", time.minute() : " << time.minute() << ", m_pKeyType12hTime : " << m_pKeyType12hTime );
       }
   }
   else {       // 2012년도 이전 값일 경우 statusbar에 inavlide(-1)한 값 전송
       if ( m_bSendInvalid ) return;
       QString str;
       int hyphen = -1;
       strOSD.append( str.setNum(hyphen));
       strOSD.append("^");
       strOSD.append( str.setNum(hyphen));
       OSDWriter::SendOSD(strOSD);
       m_bSendInvalid = true;
       LW_LOG("SendOSD(Invalide Time) : " << strOSD << ", curDate.year(): " << curDate.year() << ", curDate.month() : " << curDate.month() << ", curDate.day() : " << curDate.day() );
   }
   if ( isFG ) {
       if ( curDate.year() > 2011 ) {
           emit dataTimeSync(time.hour(), time.minute());
       }
       else {       // 2012년도 이전일 경우 대기화면에 invalide한 값 전송
           emit dataTimeSync( -1, -1 );
       }
   }
}

void
StandByApp::getSystemDate()
{
    LW_LOG( "==> getSystemDate() " );
    if ( isFG ) {
       QDate curDate = curDate.currentDate();
       if ( curDate.year() > 2011 ) {
           emit dataDateSync(curDate.day(), curDate.dayOfWeek(), curDate.month(), curDate.year());
       }
       else {
           emit dataDateSync( -1, -1, -1, -1);
       }
   }
}

void
StandByApp::getSystemSettings()
{
   LW_LOG(  "==> getSystemSettings()" );
   int value;

   if ( m_pLoadStandBySetting == NULL )
   {
       LW_LOG( "m_pLoadStandBySetting.reset()" );
       m_pLoadStandBySetting = new LoadStandBySetting();
//       m_pLoadStandBySetting.reset(new LoadStandBySetting());
   }
   if (m_pLoadStandBySetting)
   {
      value = m_pLoadStandBySetting->LoadSetting(SettingsDef::DB_KEY_CLOCK_TYPE);
      LW_LOG(  "DB_KEY_CLOCK_TYPE : " << value  );
      if ( value == 0 || value == 1 )
          m_pKeyTypeClock = value;

      value = m_pLoadStandBySetting->LoadSetting(SettingsDef::DB_KEY_TIME_TYPE);
      LW_LOG(  "DB_KEY_TIME_TYPE : " << value  );
      if ( value != -1 )
          m_pKeyType12hTime = value;

      value = m_pLoadStandBySetting->LoadSetting(SettingsDef::DB_KEY_SUMMER_TIME);
      LW_LOG(  " DB_KEY_SUMMER_TIME : " << value  );
      if ( value != -1 )
          m_pIsSummerTime = value;

      HS_LOG(  "0. m_pPhotoFrame : " << m_pPhotoFrame << " 1. m_pKeyTypeClock : " << m_pKeyTypeClock << ",    2.  m_pKeyType12hTime : " << m_pKeyType12hTime
                  << ",   4. m_pIsSummerTime : " << m_pIsSummerTime );
   }
}

int
StandByApp::getKeyTypeDateFormat()
{
    LW_LOG(  "getKeyTypeDateFormat()" );
   return m_pKeyTypeDateFormat;
}

int
StandByApp::getKeyType12hTime()
{
    LW_LOG(  "getKeyType12hTime()" );
   return m_pKeyType12hTime;
}

QString
StandByApp::getKeyPathImage()
{
    LW_LOG(  "getKeyPathImage() - path image : " << m_pKeyPathImage );
   return m_pKeyPathImage;
}

QString
StandByApp::getFileLoad()
{
    LW_LOG(  "getFileLoad()" );
   return m_pFileLoad;
}

// NOTIFY signals
void
StandByApp::setKeyTypeDateFormat(int v)
{
    LW_LOG(  "==> setKeyTypeDateFormat()" );
   m_pKeyTypeDateFormat = v;
   emit updatedKeyTypeDateFormat();
}

void
StandByApp::setKeyType12hTime(int v)
{
    LW_LOG(  "==> setKeyType12hTime()" << v );
   m_pKeyType12hTime = v;
   emit updatedKeyType12hTime();
}

void
StandByApp::setKeyPathImage(const QString & v)
{
    LW_LOG(  "==> setKeyPathImage : " << v );
   m_pKeyPathImage = v;
   emit updatedKeyPathImage();
}

void
StandByApp::setFileLoad(const QString & v)
{
    LW_LOG(  "==> setFileLoad()" << v );
   m_pFileLoad = v;
   emit updatedFileLoad();
}

void StandByApp::TapOnScreen()
{
    LW_LOG(  " TapOnScreen() - PowerOff : " << m_pApplicationEngine->getLogicOff() );

    if ( m_pApplicationEngine->getLogicOff() || m_pApplicationEngine->getPowerOff() )
        return;

    /* Beep */
    QDBusConnection::sessionBus().call(msg, QDBus::NoBlock);

    m_pApplicationEngine->HandleBackKey();
}
