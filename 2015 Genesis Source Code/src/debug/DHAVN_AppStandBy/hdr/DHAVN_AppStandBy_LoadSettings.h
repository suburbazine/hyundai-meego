#ifndef STANDBY_APP_LOADSETTING_H
#define STANDBY_APP_LOADSETTING_H

#include <QObject>
#include <QString>
#include <persistman.h>

#include <DHAVN_AppStandBy_Log_Def.h>
//#include "DHAVN_AppSettings_Def.h"

class LoadStandBySetting : public QObject
{
    Q_OBJECT
public:
    LoadStandBySetting(QObject *parent = 0);
    ~LoadStandBySetting();

    int LoadSetting( int );
    QString LoadSettingString( int );

private:
   bool Close();

   const char *m_pDBPath;
   PMHANDLE_T *m_pDBSession;
   int m_nDBState;
};

#endif //STANDBY_APP_LOADSETTING_H
