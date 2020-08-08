#ifndef DHAVN_APPSETTINGS_SAVESETTINGS_H
#define DHAVN_APPSETTINGS_SAVESETTINGS_H

#include <QObject>
#include <QString>
#include <persistman.h>
#include <DHAVN_QRPMClient.h>
//#include <DHAVN_StatusControl_Adapter.h>

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusInterface>

#include <DHAVN_AppFramework_AppEngineQML_Def.h>
#include "DHAVN_AppSettings_Def.h"
#include <DHAVN_SharedDataProvider.h>
#include <DHAVN_AppSettings_SaveSettings_ResetThread.h>

#include <QSharedMemory>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QList>

const quint16 c_CntlAudioBeepCommand = 0x0115;
const quint16 c_CntlAudioSetupInfoGetCommand = 0x0210;
const quint16 c_CntlAudioSetupValueSetCommand = 0x0114;
const quint16 c_ResAudioSetupInfoGetCommand = 0x8210;

const quint16 c_VALAnCEGetCommand = 0x8314;

class SettingsAppEngine;

extern int glo_balance; // add for ISV 100925
extern int glo_fade; // add for ISV 100925
extern int glo_high; // add for ISV 100925
extern int glo_mid; // add for ISV 100925
extern int glo_low; // add for ISV 100925
extern int glo_front_bright; // add for ISV 100925
extern int glo_rear_bright; // add for ISV 100925
extern bool factory; // add for its 243706

class SettingsStorage : public QObject
{
    Q_OBJECT
public:

    SettingsStorage(SettingsAppEngine* engine, QObject *parent = 0);
    ~SettingsStorage();

    /////////////////////////////////CREATE Q_PROPERTY//////////////////////////////////////////////////
    Q_PROPERTY (int balance READ balance WRITE setBalance NOTIFY balanceChanged)
    Q_PROPERTY (int fader READ fader WRITE setFader NOTIFY faderChanged)
    Q_PROPERTY (bool balanceHorizental READ balanceHorizental WRITE setBalanceHorizental NOTIFY balanceHorizentalChanged )//added for ITS 269208 Fader/Balance bar Issue

    Q_PROPERTY (int highTone READ highTone WRITE setHighTone NOTIFY highToneChanged)
    Q_PROPERTY (int midTone READ midTone WRITE setMidTone NOTIFY midToneChanged)
    Q_PROPERTY (int lowTone READ lowTone WRITE setLowTone NOTIFY lowToneChanged)

    Q_PROPERTY (bool vipSound READ vipSound WRITE setVipSound NOTIFY vipSoundChanged)
    Q_PROPERTY (bool avc READ avc WRITE setAVC NOTIFY avcChanged)
    Q_PROPERTY (bool beepSound READ beepSound WRITE setBeepSound NOTIFY beepSoundChanged)

    Q_PROPERTY (int variableEQ READ variableEQ WRITE setVariableEQ NOTIFY variableEQChanged)
    Q_PROPERTY (int volumeRatio READ volumeRatio WRITE setVolumeRatio NOTIFY volumeRatioChanged)
    Q_PROPERTY (bool surround READ surround WRITE setSurround NOTIFY surroundChanged)

    Q_PROPERTY (int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged)
    Q_PROPERTY (int contrast READ contrast WRITE setContrast NOTIFY contrastChanged)
    Q_PROPERTY (int saturation READ saturation WRITE setSaturation NOTIFY saturationChanged)
    Q_PROPERTY (int hue READ hue WRITE setHue NOTIFY hueChanged)
    Q_PROPERTY (int frontScreenBrightness READ frontScreenBrightness WRITE setFrontScreenBrightness NOTIFY frontScreenBrightnessChanged)
    Q_PROPERTY (int rearScreenBrightness READ rearScreenBrightness WRITE setRearScreenBrightness NOTIFY rearScreenBrightnessChanged)

    Q_PROPERTY (int exposure READ exposure WRITE setExposure NOTIFY exposureChanged)
    Q_PROPERTY (int aspectRatio READ aspectRatio  WRITE setAspectRatio NOTIFY aspectRatioChanged)

    Q_PROPERTY (bool frontScreenOnOff READ frontScreenOnOff WRITE setFrontScreenOnOff NOTIFY frontScreenOnOffChanged)
    Q_PROPERTY (bool LeftRearScreen READ leftRearScreen WRITE setLeftRearScreen NOTIFY leftRearScreenChanged)
    Q_PROPERTY (bool RightRearScreen READ rightRearScreen WRITE setRightRearScreen NOTIFY rightRearScreenChanged)

    Q_PROPERTY (int voiceCommand READ voiceCommand WRITE setVoiceCommand NOTIFY voiceCommandChanged)

    Q_PROPERTY (bool gpsTime READ gpsTime WRITE setGPSTime NOTIFY gpsTimeChanged)

    Q_PROPERTY (bool isNaviVariant READ isNaviVariant WRITE setIsNaviVariant NOTIFY isNaviVariantChanged)

    Q_PROPERTY (bool isIBoxVariant READ isIBoxVariant WRITE setIsIBoxVariant NOTIFY isIBoxVariantChanged)

    Q_PROPERTY (int autoTimeZone READ autoTimeZone WRITE setAutoTimeZone NOTIFY autoTimeZoneChanged)
    Q_PROPERTY (bool autoTimeZoneSet READ autoTimeZoneSet WRITE setAutoTimeZoneSet NOTIFY autoTimeZoneSetChanged)
    Q_PROPERTY (bool summerTime READ summerTime WRITE setSummerTime NOTIFY summerTimeChanged)
    Q_PROPERTY (bool summerTimeSet READ summerTimeSet WRITE setSummerTimeSet NOTIFY summerTimeSetChanged)
    Q_PROPERTY (int clockType READ clockType WRITE setClockType NOTIFY clockTypeChanged)
    Q_PROPERTY (int timeType READ timeType WRITE setTimeType NOTIFY timeTypeChanged)
    Q_PROPERTY (int calendarType READ calendarType WRITE setCalendarType NOTIFY calendarTypeChanged)
    Q_PROPERTY (int photoFrame READ photoFrame WRITE setPhotoFrame NOTIFY photoFrameChanged)

    Q_PROPERTY (QString photoPath READ photoPath WRITE setPhotoPath NOTIFY photoPathChanged)
    Q_PROPERTY (int languageType READ languageType WRITE setLanguageType NOTIFY languageTypeChanged)
    Q_PROPERTY (bool isArabicLanguage READ isArabicLanguage WRITE setIsArabicLanguage NOTIFY isArabicLanguageChanged)

    Q_PROPERTY (int distanceUnit READ distanceUnit WRITE setDistanceUnit NOTIFY distanceUnitChanged)

    Q_PROPERTY (int hyunDaiKeypad READ hyunDaiKeypad WRITE setHyunDaiKeypad NOTIFY hyunDaiKeypadChanged)
    Q_PROPERTY (int englishKeypad READ englishKeypad WRITE setEnglishKeypad NOTIFY englishKeypadChanged)
    Q_PROPERTY (int koreanKeypad READ koreanKeypad WRITE setKoreanKeypad NOTIFY koreanKeypadChanged)
    Q_PROPERTY (int chinaKeypad READ chinaKeypad WRITE setChinaKeypad NOTIFY chinaKeypadChanged)
    Q_PROPERTY (int europeKeypad READ europeKeypad WRITE setEuropeKeypad NOTIFY europeKeypadChanged)
    Q_PROPERTY (int arabicKeypad READ arabicKeypad WRITE setArabicKeypad NOTIFY arabicKeypadChanged)
    Q_PROPERTY (int russianKeypad READ russianKeypad WRITE setRussianKeypad NOTIFY russianKeypadChanged)

    Q_PROPERTY (bool approval READ approval WRITE setApproval NOTIFY approvalChanged)
    Q_PROPERTY (bool rearLockScreen READ rearLockScreen WRITE setRearLockScreen NOTIFY rearLockScreenChanged)

    Q_PROPERTY (int dateFormatType READ dateFormatType WRITE setDateFormatType NOTIFY dateFormatTypeChanged)

    Q_PROPERTY (int vrPhonePriority READ vrPhonePriority WRITE setVRPhonePriority NOTIFY vrPhonePriorityChanged)
    Q_PROPERTY (int currentRegion READ currentRegion WRITE setCurrentRegion NOTIFY currentRegionChanged)

    Q_PROPERTY (bool appleSiri READ appleSiri WRITE setAppleSiri NOTIFY appleSiriChanged)

    Q_PROPERTY (bool rearMonitor READ rearMonitor WRITE setRearMonitor NOTIFY rearMonitorChanged)
    Q_PROPERTY (bool rearRRCVariant READ rearRRCVariant WRITE setRearRRCVariant NOTIFY rearRRCVariantChanged)

    Q_PROPERTY (bool naviSDCard READ naviSDCard WRITE setNaviSDCard NOTIFY naviSDCardChanged)

    /////////////////////////////////END Q_PROPERTY//////////////////////////////////////////////////

    /////////////////////////////////////Sound MMUS//////////////////////////////////////////////
    void connectDBUSInterface();
    int requestSoundSettigValue();
    void requestDisplaySettigValue();

    Q_INVOKABLE bool NotifyBassChanged(int value); // add for ISV 100925
    Q_INVOKABLE bool NotifyMidChanged(int value); // add for ISV 100925
    Q_INVOKABLE bool NotifyTrebleChanged(int value); // add for ISV 100925
    Q_INVOKABLE bool NotifyBalanceChanged(int value); // add for ISV 100925
    Q_INVOKABLE bool NotifyFaderChanged(int value); // add for ISV 100925
    bool NotifyVariableEQChanged(int value);
    bool NotifyAVCChanged(int value);
    bool NotifySurroundChanged(int value);
    bool NotifyBeepChanged(int value);
    bool NotifyVipSoundChanged(int value);
    bool NotifyVolumeRatioChanged(int value);

    Q_INVOKABLE bool NotifySoundToneResetChanged();
    Q_INVOKABLE bool NotifyBalanceFaderResetChanged();
    Q_INVOKABLE bool NotifyAllResetChanged();

    QString getCurrentTime();//added for DB write Fail Issue

    void getAudioSetupResponse(QList<QVariant> list);
    /////////////////////////////////////End Sound MMUS/////////////////////////////////////////////////

    bool NotifyPhotoFrameChanged(int value);

    Q_INVOKABLE void SaveSetting( int value, int key, bool setMMUS = true );
    Q_INVOKABLE void SaveSettingString( QString, int, int );
    Q_INVOKABLE int LoadSetting( int );
    Q_INVOKABLE QString LoadSettingString( int );
    Q_INVOKABLE void SetSystemTimeByGPStime(int Year,int Month,int Day,int Hour,int Min,int Sec, QString ampm);
    Q_INVOKABLE int SetDefaultSetting(int);
    Q_INVOKABLE int GetDefaultSetting(int);
    Q_INVOKABLE void ResetSoundSettings(bool isRequestFromQml, bool factoryReset = false);
    Q_INVOKABLE void ResetScreenSettings(bool isRequestFromQml, bool factoryReset = false);
    //modified for ITS 248479 Auto Time Zone Issue EU Variant
    Q_INVOKABLE void ResetGeneralSettings(bool isRequestFromQml, bool factoryReset = false, bool countryVariantChanged = false);



    Q_INVOKABLE int GetDefaultLanguage();
    Q_INVOKABLE int GetDefaultSummerTime();
    Q_INVOKABLE int GetDefaultDateTimeFormat();
    //Q_INVOKABLE int GetDefaultDistanceUnit();
    Q_INVOKABLE int GetDefaultTimeZone();
    Q_INVOKABLE int getDefaultHyundaiKeyboard();
    Q_INVOKABLE void GetDefaultKeyPad( );
    Q_INVOKABLE void UpdateKeyboardData();
    Q_INVOKABLE void SendVideoDefaultSetMost( int type, bool isSetDispOnOff = false );

    // Extenal Clock
    //Q_INVOKABLE void changeToGPSMode(bool isGpsAvilable);
    Q_INVOKABLE void TimeFormatechanged(int);
    Q_INVOKABLE void SettingsModeChange(bool);
    Q_INVOKABLE void GpsCheckStateChange(bool isGpsOn);//added for GPS Time CAN Date Issue

    /** Made by Han */
    Q_INVOKABLE bool loadFromProperty(int key);

    Q_INVOKABLE void callAudioBeepCommand();

    Q_INVOKABLE void updateTimeZoneToSystem(bool isShowToastPopup);

    void showTimeZoneChangedToastPopup(bool isShowPopup);

    bool isResetAll; //added for ITS 248640

    void rewriteDB(); // add for backup DB

public slots:
    Q_INVOKABLE void StartResetScreenSetting();
    Q_INVOKABLE void StartResetAllSetting();

    // added for modified ibox time set 02/05
    void onSetTimeToIBox(uchar timeFormat, uint year, uint month,
                    uint day, uint hour, uint minute, uint second,
                    uint dateFormat);
    // added for modified ibox time set 02/05

    void deleteDBFile(); //added for DB write Fail Issue
    void InitializeDB();//added for DB write Fail Issue

    //q_property
private:
    int m_nBalance;
    int m_nFader;
    bool m_nBalanceHorizental;//added for ITS 269208 Fader/Balance bar Issue

    int m_nHighTone;
    int m_nMidTone;
    int m_nLowTone;

    bool m_bVipSound;
    bool m_bAVC;
    bool m_bBeepSound;

    int m_nVariableEQ;
    int m_nVolumeRatio;
    bool m_bSurround;

    int m_nBrightness;
    int m_nContrast;
    int m_nSaturation;
    int m_nHue;
    int m_nFrontScreenBrightness;
    int m_nRearScreenBrightness;

    int m_nExposure;
    int m_nAspectRatio;

    bool m_bRearMonitor;

    bool m_bRearRRCVariant;

    bool m_bFrontScreenOnOff;
    bool m_bLeftRearScreen;
    bool m_bRightRearScreen;

    int m_nVoiceCommand;

    bool m_bGPSTime;
    bool m_bIsNaviVariant;
    bool m_bIBoxVariant;
    int m_nAutoTimeZone;
    int m_bAutoTimeZoneSet;
    bool m_bSummerTime;
    bool m_bSummerTimeSet;
    int m_nClockType;
    int m_nTimeType;
    int m_nCalendarType;
    int m_nPhotoFrame;

    QString m_sPhotoPath;
    int m_nLanguageType;
    bool m_bArabicLanguage;
    int m_nDistanceUnit;

    int m_nHyunDaiKeypad;
    int m_nEnglishKeypad;
    int m_nKoreanKeypad;
    int m_nChinaKeypad;
    int m_nEuropeKeypad;
    int m_nArabicKeypad;
    int m_nRussianKeypad;

    bool m_bApproval;
    bool m_bRearLockScreen;

    int m_nDateFormatType;

    int m_nVRPhonePriority;
    int m_nCurrentRegion;

    bool m_nAppleSiri;

    bool m_bNaviSDCard;

    QDBusConnection conn;
    QDBusMessage soundSetupControl;

    QDBusConnection m_responseCountryVariant;
    QDBusConnection m_responseLCD;

    int retryCount; //added for DB write Fail Issue

    //q_property signals
signals:
    void balanceChanged();
    void faderChanged();
    void balanceHorizentalChanged();//added for ITS 269208 Fader/Balance bar Issue

    void highToneChanged();
    void midToneChanged();
    void lowToneChanged();

    void vipSoundChanged();
    void avcChanged();
    void beepSoundChanged();

    void variableEQChanged();
    void volumeRatioChanged();
    void surroundChanged();

    void brightnessChanged();
    void contrastChanged();
    void saturationChanged();
    void hueChanged();
    void frontScreenBrightnessChanged();
    void rearScreenBrightnessChanged();

    void exposureChanged();
    void aspectRatioChanged();

    void frontScreenOnOffChanged();
    void leftRearScreenChanged();
    void rightRearScreenChanged();

    void voiceCommandChanged();

    void gpsTimeChanged();
    void isNaviVariantChanged();
    void isIBoxVariantChanged();
    void autoTimeZoneChanged();
    void autoTimeZoneSetChanged();
    void summerTimeChanged();
    void summerTimeSetChanged();
    void clockTypeChanged();
    void timeTypeChanged();
    void calendarTypeChanged();
    void photoFrameChanged();

    void photoPathChanged();
    void languageTypeChanged();
    void isArabicLanguageChanged();
    void distanceUnitChanged();

    void hyunDaiKeypadChanged();
    void englishKeypadChanged();
    void koreanKeypadChanged();
    void chinaKeypadChanged();
    void europeKeypadChanged();
    void arabicKeypadChanged();
    void russianKeypadChanged();

    void approvalChanged();
    void rearLockScreenChanged();

    void dateFormatTypeChanged();

    void vrPhonePriorityChanged();

    void currentRegionChanged();

    void appleSiriChanged();

    void rearMonitorChanged();
    void naviSDCardChanged();
    void rearRRCVariantChanged();

public:

    QByteArray CommandExecute(const QByteArray cmd);//added for DB write Fail Issue
    QByteArray syncCommandExecuteReturn(const QByteArray cmd);//added for DB write Fail Issue
    void checkDBAndDelete();//added for DB write Fail Issue
    void checkResetComplete();
    void getSharedMemoryFromUISH();
    void setSHMForKeypad();

public slots:
    //q_property slots

    //Q_INVOKABLE void SetDefaultWatingImage(bool bForce = false);
    Q_INVOKABLE bool SetSharedMemoryInfo(int offset, unsigned char * pBuf, int size);
    Q_INVOKABLE bool SetSharedMemoryInfoForKeypad(int offset, unsigned char * pBuf, int size);

    Q_INVOKABLE bool GetSharedMemoryInfo(int offset, unsigned char * pBuf, int size);

    int balance() { return m_nBalance; }
    void setBalance(int value) { m_nBalance = value; glo_balance = m_nBalance; emit balanceChanged(); } // add for ISV 100925

    int fader() { return m_nFader; }
    void setFader(int value) { m_nFader = value; glo_fade = m_nFader; emit faderChanged(); } // add for ISV 100925

    bool balanceHorizental() { return m_nBalanceHorizental;  }//added for ITS 269208 Fader/Balance bar Issue
    void setBalanceHorizental(bool value) { m_nBalanceHorizental = value; emit balanceHorizentalChanged(); }

    int highTone() { return m_nHighTone; }
    void setHighTone(int value) { m_nHighTone = value; glo_high = m_nHighTone; emit highToneChanged(); } // add for ISV 100925

    int midTone() { return m_nMidTone; }
    void setMidTone(int value) { m_nMidTone = value; glo_mid = m_nMidTone; emit midToneChanged(); } // add for ISV 100925

    int lowTone() { return m_nLowTone; }
    void setLowTone(int value) { m_nLowTone = value; glo_low = m_nLowTone; emit lowToneChanged(); } // add for ISV 100925

    bool vipSound() { return m_bVipSound; }
    void setVipSound(bool value) { m_bVipSound = value; emit vipSoundChanged(); }

    bool avc() { return m_bAVC; }
    void setAVC(bool value) { m_bAVC = value; emit avcChanged(); }

    bool beepSound() { return m_bBeepSound; }
    void setBeepSound(bool value) { m_bBeepSound = value; emit beepSoundChanged(); }

    int variableEQ() { return m_nVariableEQ; }
    void setVariableEQ(int value) { m_nVariableEQ = value; emit variableEQChanged(); }

    int volumeRatio() { return m_nVolumeRatio; }
    void setVolumeRatio(int value) { m_nVolumeRatio = value; emit volumeRatioChanged(); }

    bool surround() { return m_bSurround; }
    void setSurround(bool value) { m_bSurround = value; emit surroundChanged(); }

    int brightness() { return m_nBrightness; }
    void setBrightness(int value) { m_nBrightness = value; emit brightnessChanged(); }

    int contrast() { return m_nContrast; }
    void setContrast(int value) { m_nContrast = value; emit contrastChanged(); }

    int saturation() { return m_nSaturation; }
    void setSaturation(int value) { m_nSaturation = value; emit saturationChanged(); }

    int hue() { return m_nHue; }
    void setHue(int value) { m_nHue = value; emit hueChanged(); }

    int exposure() { return m_nExposure; }
    void setExposure(int value) { m_nExposure = value; emit exposureChanged(); }

    int aspectRatio() { return m_nAspectRatio; }
    void setAspectRatio(int value) { m_nAspectRatio = value; emit aspectRatioChanged(); }

    int frontScreenBrightness() { return m_nFrontScreenBrightness; }
    void setFrontScreenBrightness(int value) { m_nFrontScreenBrightness = value; glo_front_bright = m_nFrontScreenBrightness; emit frontScreenBrightnessChanged(); } // add for ISV 100925

    int rearScreenBrightness() { return m_nRearScreenBrightness; }
    void setRearScreenBrightness(int value) { m_nRearScreenBrightness = value; glo_rear_bright = m_nRearScreenBrightness; emit rearScreenBrightnessChanged(); } // add for ISV 100925

    bool frontScreenOnOff() { return m_bFrontScreenOnOff; }
    void setFrontScreenOnOff(bool value) { m_bFrontScreenOnOff = value; emit frontScreenOnOffChanged(); }

    bool leftRearScreen() { return m_bLeftRearScreen; }
    void setLeftRearScreen(bool value) { m_bLeftRearScreen = value; emit leftRearScreenChanged(); }

    bool rightRearScreen() { return m_bRightRearScreen; }
    void setRightRearScreen(bool value) { m_bRightRearScreen = value; emit rightRearScreenChanged(); }

    int voiceCommand() { return m_nVoiceCommand; }
    void setVoiceCommand(int value) { m_nVoiceCommand = value; emit voiceCommandChanged(); }

    bool gpsTime() { return m_bGPSTime; }
    void setGPSTime(bool value) { m_bGPSTime = value; emit gpsTimeChanged(); }

    bool isNaviVariant() { return m_bIsNaviVariant; }
    void setIsNaviVariant(bool value) { m_bIsNaviVariant = value; emit isNaviVariantChanged(); }

    bool isIBoxVariant() { return m_bIBoxVariant; }
    void setIsIBoxVariant(bool value) { m_bIBoxVariant = value; emit isIBoxVariantChanged(); }

    int autoTimeZone() { return m_nAutoTimeZone; }
    void setAutoTimeZone(int value) { m_nAutoTimeZone = value; emit autoTimeZoneChanged(); }

    bool autoTimeZoneSet() { return m_bAutoTimeZoneSet; }
    void setAutoTimeZoneSet(bool value) { m_bAutoTimeZoneSet = value; emit autoTimeZoneSetChanged(); }

    bool summerTime() { return m_bSummerTime; }
    void setSummerTime(bool value) { m_bSummerTime = value; emit summerTimeChanged(); }

    bool summerTimeSet() { return m_bSummerTimeSet; }
    void setSummerTimeSet(bool value) { m_bSummerTimeSet = value; emit summerTimeSetChanged(); }

    int clockType() { return m_nClockType; }
    void setClockType(int value) { m_nClockType = value; emit clockTypeChanged(); }

    int timeType() { return m_nTimeType; }
    void setTimeType(int value) { m_nTimeType = value; emit timeTypeChanged(); }

    int calendarType() { return m_nCalendarType; }
    void setCalendarType(int value) { m_nCalendarType = value; emit calendarTypeChanged(); }

    int photoFrame() { return m_nPhotoFrame; }
    void setPhotoFrame(int value) { m_nPhotoFrame = value; emit photoFrameChanged(); }

    QString photoPath() { return m_sPhotoPath; }
    void setPhotoPath(QString value) { m_sPhotoPath = value; emit photoPathChanged(); }

    int languageType() { return m_nLanguageType; }
    void setLanguageType(int value) { m_nLanguageType = value; emit languageTypeChanged(); }

    bool isArabicLanguage() { return m_bArabicLanguage; }
    void setIsArabicLanguage(bool value) { m_bArabicLanguage = value; emit isArabicLanguageChanged(); }

    int distanceUnit() { return m_nDistanceUnit; }
    void setDistanceUnit(int value) { m_nDistanceUnit = value; emit distanceUnitChanged(); }

    int hyunDaiKeypad() { return m_nHyunDaiKeypad; }
    void setHyunDaiKeypad(int value) { m_nHyunDaiKeypad = value; emit hyunDaiKeypadChanged(); }

    int englishKeypad() { return m_nEnglishKeypad; }
    void setEnglishKeypad(int value) { m_nEnglishKeypad = value; emit englishKeypadChanged(); }

    int koreanKeypad() { return m_nKoreanKeypad; }
    void setKoreanKeypad(int value) { m_nKoreanKeypad = value; emit koreanKeypadChanged(); }

    int chinaKeypad() { return m_nChinaKeypad; }
    void setChinaKeypad(int value) { m_nChinaKeypad = value; emit chinaKeypadChanged(); }

    int europeKeypad() { return m_nEuropeKeypad; }
    void setEuropeKeypad(int value) { m_nEuropeKeypad = value; emit europeKeypadChanged(); }

    int arabicKeypad() { return m_nArabicKeypad; }
    void setArabicKeypad(int value) { m_nArabicKeypad = value; emit arabicKeypadChanged(); }

    int russianKeypad() { return m_nRussianKeypad; }
    void setRussianKeypad(int value) { m_nRussianKeypad = value; emit russianKeypadChanged(); }

    bool approval() { return m_bApproval; }
    void setApproval(bool value) { m_bApproval = value; emit approvalChanged(); }

    bool rearLockScreen() { return m_bRearLockScreen; }
    void setRearLockScreen(bool value) { m_bRearLockScreen = value; emit rearLockScreenChanged(); }

    int dateFormatType() { return m_nDateFormatType; }
    void setDateFormatType(int value) { m_nDateFormatType = value; emit dateFormatTypeChanged(); }

    int vrPhonePriority() { return m_nVRPhonePriority; }
    void setVRPhonePriority(int value) { m_nVRPhonePriority = value; emit vrPhonePriorityChanged(); }

    int currentRegion() { return m_nCurrentRegion; }
    void setCurrentRegion(int value) { m_nCurrentRegion = value; emit currentRegionChanged(); }

    bool appleSiri() { return m_nAppleSiri; }
    void setAppleSiri(bool value) { m_nAppleSiri = value; emit appleSiriChanged(); }

    bool rearMonitor() { return m_bRearMonitor; }
    void setRearMonitor(bool value) { m_bRearMonitor = value; emit rearMonitorChanged(); }

    bool naviSDCard() { return m_bNaviSDCard; }
    void setNaviSDCard(bool value) { m_bNaviSDCard = value; emit naviSDCardChanged(); }

    bool rearRRCVariant() { return m_bRearRRCVariant; }
    void setRearRRCVariant(bool value) { m_bRearRRCVariant = value; emit rearRRCVariantChanged(); }
    ///////////////////////////////////////////// End ///////////////////////////////////////////////////////

    /** MMUS */
    void DBusMicomResponseHandler(QDBusMessage reply);
    /** Response LCD Setup from MMUS */
    void LgeDBus_EventLCDSetupInfo (QByteArray in0);
    /** Response Sound Setup from MMUS */
    void EventMessage(QDBusMessage in0);

    void UpdateDateTime( QDateTime *dateTime );
    void SaveSettingsFromEngine (int value, int key, int screen);
    void SaveStringSettingsFromEngine (QString value, int key, int target_screen);
    void SaveVideoImageSettingsFromIBox( int brightness, int saturation, int contrast, int hue );
    void KeyBoardSettingChangedSlot(QDBusVariant keyboard);
    //void frontScreenStatusChanged( bool isScreenOff );
    void onTimeoutProcessingVDS();

    //! Rear State Value storage
    void onSetLRearState(int value);
    void onSetRRearState(int value);

    /** Response Country Information from Engineering Mode */
    void responseCountryVariantInfo(int countryInfo);

    //void updateFrontScreenOnOffButton(bool isFrontScreenOn);

    void notifyResetSoundCompleted();
    void notifyResetScreenCompleted();
    void notifyResetGeneralCompleted();

    void notifyFactoryResetCompleted();//added for DB File Crash Issue

    void mountedNaviSDCard();
    void removedNaviSDCard();

    Q_INVOKABLE void setTimeZone(int timezone, bool isShowToastPopup);

    void updateProperty(int value, int key);
    void initScreenThread();
    void initGeneralThread();
    int GetUishLanguage();
    int variantCountry();

    Q_INVOKABLE void showToastPopup(int toastPopupType);

    void chineKeypadChanged(int chineseKeypadType);

signals:
    void displayGPSUnavailablePopup();
    void changeDistanceUnit();
    void changeKeypadSettings();
    //ExternalClock
    void timeFormateDataChanged(int);
    void SettingsModeChanged(bool);

    void resetSoundCompleted();
    void resetScreenCompleted();
    void resetGeneralCompleted();

    void showLanguageChangingToastPopup();
    void showDaylightTimeChangingToastPopup(); //added for ITS 217706 daylight savings time not sync

private:
    void checkDBFile();
    bool Close();

    void initPropertyValue();
    void initRearMonitor();
    void initGpsVariant();
    void initIBoxVariant();

    void InitialTimeformatAndDateformat();
    void InitialDistanceUnit();
    void ConnectToMostBoardSetting();

    void initIBoxSettings();
    void initDisplayStateSettings();
    void startProcessingVDSTimer();
    void sendResultVDS( uint sTarget, bool isSetDispOnOff = false );


    //! Display On/Off
    void setSendDispControlToIBoxFlag( SettingsDef::SETTINGS_DISPLAY_MASK_T displayType, bool flag );
    bool getSendDispControlToIBoxFlag( SettingsDef::SETTINGS_DISPLAY_MASK_T displayType );
    int GetDefaultSettingOfDispOnOff(int);
    bool isDiffWithCurDisplayState( SettingsDef::SETTINGS_DISPLAY_MASK_T displayType, int value );
    void SaveSettingsFromIBox( int value, int key );

    SettingsAppEngine* m_Engine;
    const char *m_pDBPath;
    PMHANDLE_T *m_pDBSession;
    int m_nDBState;
    int m_key;

    QDateTime *m_dateTime;

    int m_brightness;
    int m_saturation;
    int m_contrast;
    int m_hue;

    //! Use for VideoDefaultSet
    uint m_uiVDS_DeviceID;
    ushort m_usVDS_SenderHandle;
    QTimer *m_pProcessingVDSTimer;

    int m_iSendDispControlToIBox;

    SharedDataProvider m_sharedVideoPlayer;
    SharedDataProvider m_sharedMobileTv;

    ResetSettingThread *m_pResetScreenThread;
    ResetSettingThread *m_pResetAllThread;
    QSharedMemory *m_pSharedMemory;
    QSharedMemory *m_pSharedMemoryForKeypad;

private slots:
    void updateDisplaySettingUI(int AppID);
    void factoryReset();

};

#endif // DHAVN_APPHOMESCREEN_SETTINGS_H
