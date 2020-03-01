/****************************************************************************
** Meta object code from reading C++ file 'qconnettidb.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qconnettidb.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qconnettidb.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QConnettiDB_t {
    QByteArrayData data[13];
    char stringdata0[133];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QConnettiDB_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QConnettiDB_t qt_meta_stringdata_QConnettiDB = {
    {
QT_MOC_LITERAL(0, 0, 11), // "QConnettiDB"
QT_MOC_LITERAL(1, 12, 12), // "mostraEvento"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 3), // "msg"
QT_MOC_LITERAL(4, 30, 12), // "mostraErrore"
QT_MOC_LITERAL(5, 43, 8), // "Connesso"
QT_MOC_LITERAL(6, 52, 13), // "QSqlDatabase*"
QT_MOC_LITERAL(7, 66, 9), // "newDbDati"
QT_MOC_LITERAL(8, 76, 13), // "newDbProcesso"
QT_MOC_LITERAL(9, 90, 10), // "QSqlQuery*"
QT_MOC_LITERAL(10, 101, 11), // "queryEvento"
QT_MOC_LITERAL(11, 113, 9), // "queryData"
QT_MOC_LITERAL(12, 123, 9) // "queryLeek"

    },
    "QConnettiDB\0mostraEvento\0\0msg\0"
    "mostraErrore\0Connesso\0QSqlDatabase*\0"
    "newDbDati\0newDbProcesso\0QSqlQuery*\0"
    "queryEvento\0queryData\0queryLeek"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QConnettiDB[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       4,    1,   32,    2, 0x06 /* Public */,
       5,    5,   35,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 6, 0x80000000 | 9, 0x80000000 | 9, 0x80000000 | 9,    7,    8,   10,   11,   12,

       0        // eod
};

void QConnettiDB::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QConnettiDB *_t = static_cast<QConnettiDB *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->mostraEvento((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->mostraErrore((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->Connesso((*reinterpret_cast< QSqlDatabase*(*)>(_a[1])),(*reinterpret_cast< QSqlDatabase*(*)>(_a[2])),(*reinterpret_cast< QSqlQuery*(*)>(_a[3])),(*reinterpret_cast< QSqlQuery*(*)>(_a[4])),(*reinterpret_cast< QSqlQuery*(*)>(_a[5]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QConnettiDB::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QConnettiDB::mostraEvento)) {
                *result = 0;
            }
        }
        {
            typedef void (QConnettiDB::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QConnettiDB::mostraErrore)) {
                *result = 1;
            }
        }
        {
            typedef void (QConnettiDB::*_t)(QSqlDatabase * , QSqlDatabase * , QSqlQuery * , QSqlQuery * , QSqlQuery * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QConnettiDB::Connesso)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject QConnettiDB::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_QConnettiDB.data,
      qt_meta_data_QConnettiDB,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QConnettiDB::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QConnettiDB::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QConnettiDB.stringdata0))
        return static_cast<void*>(const_cast< QConnettiDB*>(this));
    return QThread::qt_metacast(_clname);
}

int QConnettiDB::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void QConnettiDB::mostraEvento(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QConnettiDB::mostraErrore(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QConnettiDB::Connesso(QSqlDatabase * _t1, QSqlDatabase * _t2, QSqlQuery * _t3, QSqlQuery * _t4, QSqlQuery * _t5)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
