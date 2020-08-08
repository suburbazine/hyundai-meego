/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_FileOperationsHelper.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "hdr/DHAVN_FileOperationsHelper.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_FileOperationsHelper.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FileOperationsHelper[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      18,       // signalCount

 // signals: signature, parameters, type, tag, flags
      57,   22,   21,   21, 0x05,
     114,   87,   21,   21, 0x25,
     161,  136,   21,   21, 0x05,
     197,  192,   21,   21, 0x05,
     230,   21,   21,   21, 0x05,
     254,   21,   21,   21, 0x05,
     305,  278,   21,   21, 0x05,
     335,  330,   21,   21, 0x05,
     370,  350,   21,   21, 0x05,
     402,  396,   21,   21, 0x05,
     428,  396,   21,   21, 0x05,
     452,  396,   21,   21, 0x05,
     483,  474,   21,   21, 0x05,
     508,  502,   21,   21, 0x05,
     534,  528,   21,   21, 0x05,
     575,   21,   21,   21, 0x05,
     595,   21,   21,   21, 0x05,
     617,   21,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
     650,  645,   21,   21, 0x0a,
     767,  666,   21,   21, 0x08,
     837,  811,   21,   21, 0x08,
     878,  867,   21,   21, 0x08,
     921,  902,   21,   21, 0x08,
     965,  942,   21,   21, 0x08,
    1017,  996,   21,   21, 0x08,
    1067, 1054,   21,   21, 0x08,
    1094,   21,   21,   21, 0x08,
    1117,   21,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FileOperationsHelper[] = {
    "FileOperationsHelper\0\0"
    "index,total,copyPercentage,strName\0"
    "progress(int,int,int,QString)\0"
    "index,total,copyPercentage\0"
    "progress(int,int,int)\0skipCount,copyFilesCount\0"
    "operationCopyComplete(int,int)\0path\0"
    "operationDeleteComplete(QString)\0"
    "signalDeleteCompleted()\0operationMoveComplete()\0"
    "fileName,totalFilesForCopy\0"
    "copyRequest(QString,int)\0type\0"
    "copyError(int)\0filesCount,fileSize\0"
    "capacityError(int,qint64)\0event\0"
    "createFolderComplete(int)\0"
    "editFolderComplete(int)\0editFileComplete(int)\0"
    "fileName\0fileMoved(QString)\0state\0"
    "setMoveStatus(bool)\0value\0"
    "operationStatusChanged(EOperationStatus)\0"
    "fileSystemChanged()\0copyOpertaionExited()\0"
    "operationDeleteIncomplete()\0disp\0"
    "setDisplay(int)\0"
    "index,skipCount,copyFilesCount,modeType,volumeType,currentProgressMode"
    "Type,currentProgressVolumeType\0"
    "onCopyComplete(int,int,int,int,int,int,int)\0"
    "index,modeType,volumeType\0"
    "onDeleteComplete(int,int,int)\0index,mode\0"
    "onMoveComplete(int,int)\0errorCode,fileName\0"
    "onError(int,QString)\0percent,size,copyCount\0"
    "onCopyProgress(int,qint64,int)\0"
    "percent,size,strName\0"
    "onDeleteProgress(int,qint64,QString)\0"
    "percent,size\0onMoveProgress(int,qint64)\0"
    "notifyDeleteComplete()\0notifyCopyCanceled()\0"
};

void FileOperationsHelper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FileOperationsHelper *_t = static_cast<FileOperationsHelper *>(_o);
        switch (_id) {
        case 0: _t->progress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4]))); break;
        case 1: _t->progress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->operationCopyComplete((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->operationDeleteComplete((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->signalDeleteCompleted(); break;
        case 5: _t->operationMoveComplete(); break;
        case 6: _t->copyRequest((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->copyError((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->capacityError((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 9: _t->createFolderComplete((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->editFolderComplete((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->editFileComplete((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->fileMoved((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->setMoveStatus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->operationStatusChanged((*reinterpret_cast< EOperationStatus(*)>(_a[1]))); break;
        case 15: _t->fileSystemChanged(); break;
        case 16: _t->copyOpertaionExited(); break;
        case 17: _t->operationDeleteIncomplete(); break;
        case 18: _t->setDisplay((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->onCopyComplete((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< int(*)>(_a[7]))); break;
        case 20: _t->onDeleteComplete((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 21: _t->onMoveComplete((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 22: _t->onError((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 23: _t->onCopyProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 24: _t->onDeleteProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 25: _t->onMoveProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 26: _t->notifyDeleteComplete(); break;
        case 27: _t->notifyCopyCanceled(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FileOperationsHelper::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FileOperationsHelper::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FileOperationsHelper,
      qt_meta_data_FileOperationsHelper, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FileOperationsHelper::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FileOperationsHelper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FileOperationsHelper::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FileOperationsHelper))
        return static_cast<void*>(const_cast< FileOperationsHelper*>(this));
    return QObject::qt_metacast(_clname);
}

int FileOperationsHelper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
    return _id;
}

// SIGNAL 0
void FileOperationsHelper::progress(int _t1, int _t2, int _t3, QString _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 2
void FileOperationsHelper::operationCopyComplete(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FileOperationsHelper::operationDeleteComplete(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void FileOperationsHelper::signalDeleteCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void FileOperationsHelper::operationMoveComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void FileOperationsHelper::copyRequest(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void FileOperationsHelper::copyError(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void FileOperationsHelper::capacityError(int _t1, qint64 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void FileOperationsHelper::createFolderComplete(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void FileOperationsHelper::editFolderComplete(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void FileOperationsHelper::editFileComplete(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void FileOperationsHelper::fileMoved(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void FileOperationsHelper::setMoveStatus(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void FileOperationsHelper::operationStatusChanged(EOperationStatus _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void FileOperationsHelper::fileSystemChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 15, 0);
}

// SIGNAL 16
void FileOperationsHelper::copyOpertaionExited()
{
    QMetaObject::activate(this, &staticMetaObject, 16, 0);
}

// SIGNAL 17
void FileOperationsHelper::operationDeleteIncomplete()
{
    QMetaObject::activate(this, &staticMetaObject, 17, 0);
}
QT_END_MOC_NAMESPACE
