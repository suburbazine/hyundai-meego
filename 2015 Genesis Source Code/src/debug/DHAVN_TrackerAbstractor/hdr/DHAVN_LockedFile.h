#ifndef DHAVN_LOCKEDFILE_H  // added this file by sungha.choi 2013.08.18 for File Lock across Process
#define DHAVN_LOCKEDFILE_H

#include <QFile>

class QtLockedFile : public QFile
{
public:
    enum LockMode { NoLock = 0, ReadLock, WriteLock };

    QtLockedFile();
    QtLockedFile(const QString &name);
    ~QtLockedFile();

    bool open(OpenMode mode);

    bool lock(LockMode mode, bool block = true);
    bool unlock();
    bool isLocked() const;
    LockMode lockMode() const;

private:
    LockMode m_lock_mode;
};


#endif // DHAVN_LOCKEDFILE_H
