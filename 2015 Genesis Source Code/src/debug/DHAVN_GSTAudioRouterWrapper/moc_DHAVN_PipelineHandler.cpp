/****************************************************************************
** Meta object code from reading C++ file 'DHAVN_PipelineHandler.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "DHAVN_PipelineHandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DHAVN_PipelineHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PipeLinehandler[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      40,   17,   16,   16, 0x0a,
      98,   88,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PipeLinehandler[] = {
    "PipeLinehandler\0\0inSrc,inRate,inSrcType\0"
    "AquireAudioSource(QString,QString,EAudioSource)\0"
    "inSrcType\0ReleaseAudioSource(EAudioSource)\0"
};

void PipeLinehandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PipeLinehandler *_t = static_cast<PipeLinehandler *>(_o);
        switch (_id) {
        case 0: _t->AquireAudioSource((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const EAudioSource(*)>(_a[3]))); break;
        case 1: _t->ReleaseAudioSource((*reinterpret_cast< const EAudioSource(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PipeLinehandler::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PipeLinehandler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PipeLinehandler,
      qt_meta_data_PipeLinehandler, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PipeLinehandler::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PipeLinehandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PipeLinehandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PipeLinehandler))
        return static_cast<void*>(const_cast< PipeLinehandler*>(this));
    return QObject::qt_metacast(_clname);
}

int PipeLinehandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
