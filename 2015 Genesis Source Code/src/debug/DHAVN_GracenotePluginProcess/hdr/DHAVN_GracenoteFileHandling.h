/*
 *----------------------------------------------------------------*
 *
 * MODIFICATION RECORDS :                                         *
 *
 * DATE         PROGRAMMER               COMMENT
 ******************************************************************
 * History
 * 07-Mar-2012      Vishal Jain             Create new file
 ******************************************************************
 */

#ifndef DHAVN_GRACENOTEDELETEHANDLING_H
#define DHAVN_GRACENOTEDELETEHANDLING_H

#include <QObject>
#include <QSqlDatabase>
#include <QStringList>
#include <DHAVN_LogUtil.h>
//added by Puneet for fixing GPP crash issue 2013.06.12
#include <QSemaphore>
#include <QMutex>
#include <QMutexLocker>


//Enum indicating files in USB or JB
enum EFileType
{
    eJukebox,
    eUsb
};

class CGracenoteFileHandling : public QObject
{
    Q_OBJECT
    USE_LOG_UTIL

public:
    CGracenoteFileHandling(QObject* parent = 0);
    ~CGracenoteFileHandling();
    bool Init();

    bool CreateTable(QSqlDatabase db);

    bool InsertFilesInformation(QStringList filesList);

    bool GetJukeBoxFiles(QStringList& filesList);
    bool GetUSBFiles(QStringList& filesList, QString mountPath);

    bool DeleteJukeboxFiles();
    bool DeleteUSBFiles(QString mountPath);

    bool DeleteListOfFiles(QStringList deletedFileList);

private:
    bool GetListOfFiles(EFileType type, QStringList& filesList, QString mountPath = QString());
	//added by Puneet for fixing GPP crash issue 2013.06.12
    bool createConnection(QString dbPath, QString connectionName, QSqlDatabase &dBHandle);
    void closeConnection(QSqlDatabase &dBHandle); //by aettie for Static Test Defect 201300711
    QSemaphore sem;
};

#endif // DHAVN_GRACENOTEDELETEHANDLING_H
