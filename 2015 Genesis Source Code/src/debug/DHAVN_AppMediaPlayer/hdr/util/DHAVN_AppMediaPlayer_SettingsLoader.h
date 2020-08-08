#ifndef SETTINGS_LOADER_H
#define SETTINGS_LOADER_H

#include <QObject>
#include <QString>
#include <persistman.h>

class SettingsLoader
{
public:
    static SettingsLoader* instance()
    {
        if( m_instance == NULL )
        {
            m_instance = new SettingsLoader();
        }
        return m_instance;
    }
    static void release()
    {
        if( m_instance != NULL ) {
            delete m_instance;
            m_instance = NULL;
        }
    }

    int LoadSetting( int );
    QString LoadSettingString( int );

private:
    SettingsLoader();
    SettingsLoader(const SettingsLoader& t) {}
    const SettingsLoader& operator =(const SettingsLoader& t) {}
    ~SettingsLoader();

    bool Close();

    const char *m_pDBPath;
    PMHANDLE_T *m_pDBSession;
    int m_nDBState;

    static SettingsLoader* m_instance;
};

#endif //SETTINGS_LOADER_H
