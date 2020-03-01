/****************************************************************************
** Meta object code from reading C++ file 'qanalogico.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qanalogico.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qanalogico.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QMLabel_t {
    QByteArrayData data[1];
    char stringdata0[8];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QMLabel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QMLabel_t qt_meta_stringdata_QMLabel = {
    {
QT_MOC_LITERAL(0, 0, 7) // "QMLabel"

    },
    "QMLabel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QMLabel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void QMLabel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject QMLabel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QMLabel.data,
      qt_meta_data_QMLabel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QMLabel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QMLabel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QMLabel.stringdata0))
        return static_cast<void*>(const_cast< QMLabel*>(this));
    return QWidget::qt_metacast(_clname);
}

int QMLabel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_QAnalogico_t {
    QByteArrayData data[7];
    char stringdata0[70];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QAnalogico_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QAnalogico_t qt_meta_stringdata_QAnalogico = {
    {
QT_MOC_LITERAL(0, 0, 10), // "QAnalogico"
QT_MOC_LITERAL(1, 11, 13), // "CambiaSegnale"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 7), // "newslot"
QT_MOC_LITERAL(4, 34, 8), // "newpoint"
QT_MOC_LITERAL(5, 43, 10), // "newsegnale"
QT_MOC_LITERAL(6, 54, 15) // "editingFinished"

    },
    "QAnalogico\0CambiaSegnale\0\0newslot\0"
    "newpoint\0newsegnale\0editingFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QAnalogico[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   31,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void QAnalogico::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QAnalogico *_t = static_cast<QAnalogico *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->CambiaSegnale((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->editingFinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QAnalogico::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QAnalogico::CambiaSegnale)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject QAnalogico::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QAnalogico.data,
      qt_meta_data_QAnalogico,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QAnalogico::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QAnalogico::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QAnalogico.stringdata0))
        return static_cast<void*>(const_cast< QAnalogico*>(this));
    return QWidget::qt_metacast(_clname);
}

int QAnalogico::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void QAnalogico::CambiaSegnale(int _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_QDigitale_t {
    QByteArrayData data[7];
    char stringdata0[64];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QDigitale_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QDigitale_t qt_meta_stringdata_QDigitale = {
    {
QT_MOC_LITERAL(0, 0, 9), // "QDigitale"
QT_MOC_LITERAL(1, 10, 13), // "CambiaSegnale"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 7), // "newslot"
QT_MOC_LITERAL(4, 33, 8), // "newpoint"
QT_MOC_LITERAL(5, 42, 10), // "newsegnale"
QT_MOC_LITERAL(6, 53, 10) // "mousePress"

    },
    "QDigitale\0CambiaSegnale\0\0newslot\0"
    "newpoint\0newsegnale\0mousePress"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QDigitale[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   31,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void QDigitale::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QDigitale *_t = static_cast<QDigitale *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->CambiaSegnale((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->mousePress((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QDigitale::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QDigitale::CambiaSegnale)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject QDigitale::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QDigitale.data,
      qt_meta_data_QDigitale,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QDigitale::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QDigitale::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QDigitale.stringdata0))
        return static_cast<void*>(const_cast< QDigitale*>(this));
    return QWidget::qt_metacast(_clname);
}

int QDigitale::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void QDigitale::CambiaSegnale(int _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_QSensore_t {
    QByteArrayData data[6];
    char stringdata0[52];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QSensore_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QSensore_t qt_meta_stringdata_QSensore = {
    {
QT_MOC_LITERAL(0, 0, 8), // "QSensore"
QT_MOC_LITERAL(1, 9, 13), // "CambiaSegnale"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 7), // "newslot"
QT_MOC_LITERAL(4, 32, 8), // "newpoint"
QT_MOC_LITERAL(5, 41, 10) // "newsegnale"

    },
    "QSensore\0CambiaSegnale\0\0newslot\0"
    "newpoint\0newsegnale"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QSensore[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,

       0        // eod
};

void QSensore::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QSensore *_t = static_cast<QSensore *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->CambiaSegnale((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QSensore::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QSensore::CambiaSegnale)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject QSensore::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QSensore.data,
      qt_meta_data_QSensore,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QSensore::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QSensore::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QSensore.stringdata0))
        return static_cast<void*>(const_cast< QSensore*>(this));
    return QWidget::qt_metacast(_clname);
}

int QSensore::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void QSensore::CambiaSegnale(int _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
