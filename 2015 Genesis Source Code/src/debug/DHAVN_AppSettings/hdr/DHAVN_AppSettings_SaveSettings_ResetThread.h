#ifndef DHAVN_APPSETTINGS_SAVESETTINGS_RESETTHREAD_H
#define DHAVN_APPSETTINGS_SAVESETTINGS_RESETTHREAD_H

#include <DHAVN_AppSettings_Def.h>
#include <persistman.h>
#include <QObject>
#include <QThread>

class SettingsStorage;

class ResetSettingThread : public QThread
{
    Q_OBJECT

public:
    ResetSettingThread(QObject *parent = 0);
    ResetSettingThread(SettingsStorage* storage, SettingsDef::SETTINGS_RESET_TYPE_T resetType, QObject *parent = 0);
    ~ResetSettingThread();

    virtual void run ();

signals:
    void propertyResetCompleted(int value, int key);
    void aa(); // add for die

public slots:
    int GetDefaultSetting( int key );
    int GetDefaultSettingOfDispOnOff( int defaultDispOnOff );

    void resetScreenSettings();
    void resetGeneralSettings();

    void GetDefaultKeyPad();
    int LoadSetting( int key );
    int GetDefaultDateTimeFormat();
    int getDefaultHyundaiKeyboard();
    int GetDefaultSummerTime();
    int GetDefaultLanguage();
    int SetDefaultSetting( int key );
    void SaveSetting( int value, int key );
    bool close();

private:
    bool m_bNaviSDCard;
    bool m_bNaviSystemVariant;
    bool m_bRearRRCVariant;
    int m_nCurrentRegion;
    int m_nResetType;
    int m_nDBState;
    PMHANDLE_T *m_pDBSession;
    SettingsStorage* m_Storage;

};

#endif // DHAVN_APPSETTINGS_SAVESETTINGS_RESETTHREAD_H
