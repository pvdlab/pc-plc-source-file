/****************************************************************************
** Meta object code from reading C++ file 'qlabservernet.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qlabservernet.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qlabservernet.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QLabserverNet_t {
    QByteArrayData data[8];
    char stringdata0[78];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QLabserverNet_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QLabserverNet_t qt_meta_stringdata_QLabserverNet = {
    {
QT_MOC_LITERAL(0, 0, 13), // "QLabserverNet"
QT_MOC_LITERAL(1, 14, 6), // "Evento"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 10), // "onConnesso"
QT_MOC_LITERAL(4, 33, 13), // "onDisconnesso"
QT_MOC_LITERAL(5, 47, 8), // "onErrore"
QT_MOC_LITERAL(6, 56, 10), // "RiConnetti"
QT_MOC_LITERAL(7, 67, 10) // "onNuovoMsg"

    },
    "QLabserverNet\0Evento\0\0onConnesso\0"
    "onDisconnesso\0onErrore\0RiConnetti\0"
    "onNuovoMsg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QLabserverNet[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   47,    2, 0x0a /* Public */,
       4,    0,   50,    2, 0x0a /* Public */,
       5,    1,   51,    2, 0x0a /* Public */,
       6,    0,   54,    2, 0x0a /* Public */,
       7,    0,   55,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QLabserverNet::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QLabserverNet *_t = static_cast<QLabserverNet *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Evento((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->onConnesso((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->onDisconnesso(); break;
        case 3: _t->onErrore((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->RiConnetti(); break;
        case 5: _t->onNuovoMsg(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QLabserverNet::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLabserverNet::Evento)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject QLabserverNet::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QLabserverNet.data,
      qt_meta_data_QLabserverNet,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QLabserverNet::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QLabserverNet::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QLabserverNet.stringdata0))
        return static_cast<void*>(const_cast< QLabserverNet*>(this));
    return QObject::qt_metacast(_clname);
}

int QLabserverNet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void QLabserverNet::Evento(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
