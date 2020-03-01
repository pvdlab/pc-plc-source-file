/****************************************************************************
** Meta object code from reading C++ file 'cthreadclient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cthreadclient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cthreadclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CThreadClient_t {
    QByteArrayData data[8];
    char stringdata0[60];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CThreadClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CThreadClient_t qt_meta_stringdata_CThreadClient = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CThreadClient"
QT_MOC_LITERAL(1, 14, 8), // "Connesso"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 3), // "val"
QT_MOC_LITERAL(4, 28, 11), // "StepSpediti"
QT_MOC_LITERAL(5, 40, 6), // "Errore"
QT_MOC_LITERAL(6, 47, 3), // "msg"
QT_MOC_LITERAL(7, 51, 8) // "nuovoMsg"

    },
    "CThreadClient\0Connesso\0\0val\0StepSpediti\0"
    "Errore\0msg\0nuovoMsg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CThreadClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    2,   37,    2, 0x06 /* Public */,
       5,    1,   42,    2, 0x06 /* Public */,
       7,    0,   45,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,

       0        // eod
};

void CThreadClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CThreadClient *_t = static_cast<CThreadClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Connesso((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->StepSpediti((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->Errore((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->nuovoMsg(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CThreadClient::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CThreadClient::Connesso)) {
                *result = 0;
            }
        }
        {
            typedef void (CThreadClient::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CThreadClient::StepSpediti)) {
                *result = 1;
            }
        }
        {
            typedef void (CThreadClient::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CThreadClient::Errore)) {
                *result = 2;
            }
        }
        {
            typedef void (CThreadClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CThreadClient::nuovoMsg)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject CThreadClient::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CThreadClient.data,
      qt_meta_data_CThreadClient,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CThreadClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CThreadClient::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CThreadClient.stringdata0))
        return static_cast<void*>(const_cast< CThreadClient*>(this));
    return QThread::qt_metacast(_clname);
}

int CThreadClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void CThreadClient::Connesso(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CThreadClient::StepSpediti(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CThreadClient::Errore(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CThreadClient::nuovoMsg()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
