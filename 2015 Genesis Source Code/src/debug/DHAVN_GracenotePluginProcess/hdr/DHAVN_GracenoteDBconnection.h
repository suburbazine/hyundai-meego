#ifndef DHAVN_GRACENOTEDBCONNECTION_H
#define DHAVN_GRACENOTEDBCONNECTION_H

#include <QSqlDatabase>
#include <QString>
#include <DHAVN_LogUtil.h>
//added by Puneet for fixing GPP crash issue 2013.06.12
#include <QMutex>
#include <QSemaphore>

//added by Puneet for fixing GPP crash issue 2013.06.12
static QSemaphore sem(1);
bool createConnection(QString dbPath, QString connectionName, QSqlDatabase &dBHandle);
//added by Puneet for fixing GPP crash issue 2013.06.12
void closeConnection(QSqlDatabase &dBHandle); //by aettie for Static Test Defect 201300711

#endif // DHAVN_GRACENOTEDBCONNECTION_H
