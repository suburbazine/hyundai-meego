#ifndef OSD_MANAGER_H
#define OSD_MANAGER_H

#include <QObject>
#include "DHAVN_AppHomeScreen_Def_Private.h"
#include <DHAVN_AppHomeScreen_Log_Def.h>

class OSD_manager : public QObject
{
    Q_OBJECT
public:
    explicit OSD_manager(QObject *parent = 0);
    
signals:
    
public slots:
    void sendOSD(QString osd_message);
    void OSD_AppsBooting (int display);

private:
    inline void appendScreenString(int d, QString &);
    
};

#endif // OSD_MANAGER_H
