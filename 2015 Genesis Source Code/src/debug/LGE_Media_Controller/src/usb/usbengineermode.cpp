#include "usbengineermode.h"
#include "usbcontroller.h"
#include <QFile>
#include <QDir>
#include "common.h"
#include "util.h"

USBEngineerMode::USBEngineerMode(QObject *parent) : QObject(parent)
{
}

USBEngineerMode::~USBEngineerMode()
{
}

int USBEngineerMode::USBFileReadingFail(QString strMountPath)
{
    #if USE_USB_SINGLE_INSTANCE
    USBController *pUSB = USBController::GetInstance();
    #else
    USBController *pUSB = USBController::GetFirstInstance();
    #endif

    if(pUSB == 0) {
        WARNING() << "pUSB is NULL";
        return true;    // Fail : true
    }
        
    //if(pUSB->IsBootWithUSB() == false)
    //    return false;   // Fail : false

    QString strFileName = FindFirstFile(strMountPath);

    if(strFileName.isEmpty()) {
        WARNING() << "strFileName is empty";
        return true;    // Fail : true
    }

    QFile tSrcFile(strFileName);
    if(tSrcFile.open(QFile::ReadOnly) == false) {
        WARNING() << "tSrcFile.open == false" << strFileName;
        tSrcFile.close();
        return true;    // Fail : true
    }

    char   pBlock[1];
    qint64 nRead  = 0;
    int    bError = false;
    
    while(!tSrcFile.atEnd()) {
        nRead = tSrcFile.read(pBlock, 1);
        if(nRead <= 0) bError = true;
        break;
    }

    tSrcFile.close();

    MC_HIGH << "bError:" << bError << LOG_ENDL;
    
    return bError; // Fail : false
}

QString USBEngineerMode::FindFirstFile(QString strFolderName)
{
    QFileInfo tDirInfo(strFolderName);
    if(!tDirInfo.isDir()) {
        return strFolderName;
    }
    
    QDir          tSrcDir(strFolderName);
    QFileInfoList tFileInfoList = tSrcDir.entryInfoList(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot);
    
    for(int i = 0; i < tFileInfoList.size(); i++)
    {
        QFileInfo tFileInfo = tFileInfoList.at(i);

        if(tFileInfo.isDir() == true) {
            return FindFirstFile(tFileInfo.absoluteFilePath());
        }
        else if(tFileInfo.size() > 0) {
            return tFileInfo.absoluteFilePath();
        }
    }

    return QString();
}
