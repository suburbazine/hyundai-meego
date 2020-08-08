#ifndef DHAVN_APPSETTINGS_SYSTEMINFO_H
#define DHAVN_APPSETTINGS_SYSTEMINFO_H

#include <QObject>
#include <QString>
#include <QSettings>
#include <QStringList>

#include "DHAVN_AppFramework_Def.h"
#include <DHAVN_AppSettings_AppEngine.h>
#include "DHAVN_AppSettings_CNaviVersionInformer.h"

class SystemInfo : public QObject
{
   Q_OBJECT

   Q_ENUMS( SYSTEM_INFO_TYPES )

    Q_PROPERTY( QString Model READ GetModel WRITE setModel NOTIFY notifyModel )
    Q_PROPERTY( QString SWVersion READ GetSWVersion WRITE setSWVersion NOTIFY notifySWVersion )
    Q_PROPERTY( QString KernelVersion READ GetKernelVersion WRITE setKernelVersion NOTIFY notifyKernelVersion )
    //Q_PROPERTY( QString MicomVersion READ GetMicomVersion WRITE setMicomVersion NOTIFY notifyMicomVersion )
    Q_PROPERTY( QString MapVersion READ GetMapVersion WRITE setMapVersion NOTIFY notifyMapVersion )
    Q_PROPERTY( QString AppVersion READ GetAppVersion WRITE setAppVersion NOTIFY notifyAppVersion )
    Q_PROPERTY( QString Association READ GetAssociationNumber WRITE setAssociationNumber NOTIFY notifyAssociation )
    Q_PROPERTY( QString IBoxVersion READ GetIBoxVersion WRITE setIBoxVersion NOTIFY notifyIBoxVersion )
    Q_PROPERTY( QString GraceNoteSDKVersion READ GraceNoteSDKVersion WRITE setGraceNoteSDKVersion NOTIFY notifyGraceNoteSDKVersion )
    Q_PROPERTY( QString GraceNoteDBVersion READ GraceNoteDBVersion WRITE setGraceNoteDBVersion NOTIFY notifyGraceNoteDBVersion )
    Q_PROPERTY( QString BTMacAddress READ BTMacAddress WRITE setBTMacAddress NOTIFY notifyBTMacAddress )

public:
   SystemInfo(){ m_Engine = NULL; m_pNaviInformer = NULL; m_pSettingsInfo = NULL; m_Variant = eCVKorea; m_gracenoteDBVer="";}
   SystemInfo(SettingsAppEngine* engine, ECountryVariant variant, QObject *parent = 0);

   ~SystemInfo();

   Q_INVOKABLE QString GetModel();
   QString GetSWVersion();
   QString GetKernelVersion();
   //QString GetMicomVersion();
   QString GetMapVersion();
   QString GetAppVersion();
   QString GetAssociationNumber();
   QString GetIBoxVersion();
   QString carHwType; //added for DH15MY version

public:
   enum SYSTEM_INFO_TYPES
   {
      SYSTEM_INFO_MODEL,
      SYSTEM_INFO_SW_VERSION,
      SYSTEM_INFO_KERNEL_VERSION,
      SYSTEM_INFO_MICOM_VERSION,
      SYSTEM_INFO_MAP_VERSION,
      SYSTEM_INFO_ASSOCIATION_NUM,
      SYSTEM_INFO_IBOX_VERSION
   };

Q_SIGNALS:
   void notifyModel();
   void notifySWVersion();
   void notifyKernelVersion();
   //void notifyMicomVersion();
   void notifyMapVersion();
   void notifyAppVersion();
   void notifyAssociation();
   void notifyIBoxVersion();

   void notifyGraceNoteSDKVersion();
   void notifyGraceNoteDBVersion();

   void notifyBTMacAddress();

public slots:
   void setModel(QString str) { m_vehicleModel = str; emit notifyModel(); }
   void setSWVersion(QString str) { m_swVersion = str; emit notifySWVersion(); }
   void setKernelVersion(QString str) { m_kernelVersion = str; emit notifyKernelVersion(); }
   //void setMicomVersion(QString str) { m_micomVersion = str; emit notifyMicomVersion(); }
   void setMapVersion(QString str) { m_mapVersion = str; emit notifyMapVersion(); }
   void setAppVersion(QString str) { m_appVersion = str; emit notifyAppVersion(); }
   void setAssociationNumber(QString str) { m_associationNumber = str; emit notifyAssociation(); }
   void setIBoxVersion(QString str) { m_iboxVersion = str; emit notifyIBoxVersion(); }

   QString GraceNoteSDKVersion(){return m_gracenoteSDKVer;}
   QString GraceNoteDBVersion(){return m_gracenoteDBVer;}
   void setGraceNoteSDKVersion(QString str) { m_gracenoteSDKVer = str; emit notifyGraceNoteSDKVersion(); }
   void setGraceNoteDBVersion(QString value) { m_gracenoteDBVer = value; emit notifyGraceNoteDBVersion(); }

   QString BTMacAddress(){return m_sBTMacAddress;}
   void setBTMacAddress(QString value) { m_sBTMacAddress = value; emit notifyBTMacAddress(); }

   void RequestGNSDKVer();
   void RequestGNDBVer();
   void onGNSDKVer(QString ver);
   void onGNDBVer(QString ver);

   void updateNaviInfo();
private slots:
   void iboxVersionUpdated( const QStringList version );
   void clearNaviInfo ( );

private:
   SettingsAppEngine* m_Engine;

   QString m_vehicleModel;
   QString m_swVersion;
   QString m_kernelVersion;
   QString m_micomVersion;
   QString m_mapVersion;
   QString m_appVersion;
   QString m_associationNumber;
   QString m_iboxVersion;

   QString m_gracenoteSDKVer;
   QString m_gracenoteDBVer;

   QString m_sBTMacAddress;

   CNaviVersionInformer *m_pNaviInformer;
   ECountryVariant m_Variant;

   QSettings *m_pSettingsInfo;

private:
   QString getFirstSubString( const QString skipped, const QString src );
   QString getHWMainBoardVersion();
   void makeSWVersionWithBoardVersion( const QString BoardVer );
   bool isContainNumber( const QString string );

   void getModelInfo();
   void getBTMacAddressInfo();
};

#endif // DHAVN_APPSETTINGS_SYSTEMINFO_H
