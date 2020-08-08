#ifndef IFILEINTERFACE_H
#define IFILEINTERFACE_H

#include "mediaplayer_global.h"
#include "DHAVN_TrackerAbstractor_constants.h"

#include <QString>

#define FMANAGER_SUCCESS    (1)
#define FMANAGER_FAILED     (0)

typedef ETrackerAbstarctor_VolumeType ETAVolumeType;

class MEDIAPLAYERSHARED_EXPORT IFileInterface 
{
public:
    virtual int         CreateFolder(const QString &createFolderName) = 0;
    virtual int         EditedFolder(const QString &strOldName, const QString &strNewName) = 0;
    virtual int         EditedFile(const QString &strOldName, const QString &strNewName) = 0;
    virtual int         DeleteFileFolder(const QString &absoluteFileFolderName, int typemode = -1, ETAVolumeType volume = eJukeBox) = 0;
    virtual int         MoveFolder(const QString &absoluteFolderName, const QString &destPath) = 0;
    virtual int         CopyFolder(const QString &absoluteFolderName, const QString &destPath) = 0;
    virtual int         CopyFolder(const QString &absoluteFolderName, const QString &destPath, int typemode, ETAVolumeType volume = eUSB) = 0;
    
    virtual int         DeleteFileList(const QStringList &strList, int typemode = -1, ETAVolumeType volume = eJukeBox) = 0;
    virtual int         MoveFileList(const QStringList &strList, const QString &destPath) = 0;
    virtual int         CopyFileList(const QStringList &strList, const QString &destPath) = 0;
    virtual int         CopyFileList(const QStringList &strList, const QString &destPath, int typemode, ETAVolumeType volume = eUSB) = 0;
    
    virtual int         TotalFileForCopy() = 0;
    virtual int         TotalFileForMove() = 0;
    virtual int         TotalFileForDeletion() = 0;
    virtual qint64      GetTotalCopySize() = 0;
    virtual qint64      GetTotalMoveSize() = 0;
    virtual qint64      GetTotalDeleteSize() = 0;
    
    virtual qint64      GetTotalDiscSpace(const QString &strVolumeName) = 0;
    virtual qint64      GetTotalUsedDiscSpace(const QString &strVolumeName) = 0;
    virtual int         StartDiscFormat(const QString &strVolumeName) = 0;
    
    virtual int         CancelCopyOperation() = 0;
    virtual int         CancelDeleteOperation() = 0;
    virtual int         CancelFormatOperation() = 0;
    virtual int         SetOverWriteCurrentExistFileMode(int event) = 0;
    virtual int         ContinueCopying() = 0;
    virtual int         PauseCopying() = 0;

    virtual int         CancelMoveOperation() = 0;
    virtual int         ContinueMoving() = 0;
    virtual int         PauseMoving() = 0;
};

#endif // IMEDIAINTERFACE_H
