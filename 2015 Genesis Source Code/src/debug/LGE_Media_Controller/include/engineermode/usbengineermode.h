#ifndef _USBENGINEERMODE_H
#define _USBENGINEERMODE_H

#include <QObject>

class USBEngineerMode : public QObject
{
    Q_OBJECT
public:
    USBEngineerMode(QObject *parent = 0);
    ~USBEngineerMode();

    int USBFileReadingFail(QString strMountPath);
    
private:
    QString FindFirstFile(QString strFolderName);
};

#endif