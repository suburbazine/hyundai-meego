#ifndef DHAVN_APPHOMESCREEN_APPDATA_H
#define DHAVN_APPHOMESCREEN_APPDATA_H

#include <QHash>
#include <QVariant>
#include <QSettings>
#include <DHAVN_AppHomeScreen_Def_Private.h>
#include <DHAVN_AppHomeScreen_Log_Def.h>

struct MEDIA_CONTENT_MAP_T
{
   HomeScreenDef::MEDIA_SOURCE_T nSource;
   HSDefP::APP_DATA_T nDevice;
   HSDefP::APP_DATA_T nVideo;
   HSDefP::APP_DATA_T nPhoto;
   HSDefP::APP_DATA_T nAudio;
   QString sDeviceName;
};

class AppData : public QObject
{
   Q_OBJECT

public:
   AppData();
   ~AppData();

   QVariant GetValue( HSDefP::APP_DATA_T nId );

   bool GetSettingsVRIconInfo(LANGUAGE_T language);
   bool GetSettingsHELPIconInfo(LANGUAGE_T language);

   Q_INVOKABLE bool GetParkingMode();

   void SetMediaContentStatus( HomeScreenDef::MEDIA_SOURCE_T nSource,
                               bool bDeviceConnected = false,
                               bool bVideo = false,
                               bool bPhoto = false,
                               bool bAudio = false );

signals:
   void ValueChanged( HSDefP::APP_DATA_T nID, QVariant data );

public slots:
   void SetValue( HSDefP::APP_DATA_T nId, QVariant value );

private:
   MEDIA_CONTENT_MAP_T GetDataFields( HomeScreenDef::MEDIA_SOURCE_T nSource );
   bool NeedToSetConnectedDevice( HomeScreenDef::MEDIA_SOURCE_T nSource, bool bDeviceConnected );


private:
   void UpdateDependentId( HSDefP::APP_DATA_T nId );
   HSDefP::APP_DATA_T GetDependentId( HSDefP::APP_DATA_T nId );
   QHash< HSDefP::APP_DATA_T, QVariant> m_AppData;
   QList< HomeScreenDef::MEDIA_SOURCE_T > m_USBList;
   //QSettings *m_pSettings;
};

#endif // DHAVN_APPHOMESCREEN_APPDATA_H
