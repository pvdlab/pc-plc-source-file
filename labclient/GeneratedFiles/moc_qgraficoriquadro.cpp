/****************************************************************************
** Meta object code from reading C++ file 'qgraficoriquadro.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qgraficoriquadro.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qgraficoriquadro.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QGraficoLabel_t {
    QByteArrayData data[1];
    char stringdata0[14];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QGraficoLabel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QGraficoLabel_t qt_meta_stringdata_QGraficoLabel = {
    {
QT_MOC_LITERAL(0, 0, 13) // "QGraficoLabel"

    },
    "QGraficoLabel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QGraficoLabel[] = {

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

void QGraficoLabel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject QGraficoLabel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QGraficoLabel.data,
      qt_meta_data_QGraficoLabel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QGraficoLabel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QGraficoLabel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QGraficoLabel.stringdata0))
        return static_cast<void*>(const_cast< QGraficoLabel*>(this));
    return QWidget::qt_metacast(_clname);
}

int QGraficoLabel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_QGrafico_t {
    QByteArrayData data[4];
    char stringdata0[29];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QGrafico_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QGrafico_t qt_meta_stringdata_QGrafico = {
    {
QT_MOC_LITERAL(0, 0, 8), // "QGrafico"
QT_MOC_LITERAL(1, 9, 9), // "gotoTempo"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 8) // "newtempo"

    },
    "QGrafico\0gotoTempo\0\0newtempo"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QGrafico[] = {

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
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void QGrafico::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QGrafico *_t = static_cast<QGrafico *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->gotoTempo((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QGrafico::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QGrafico::gotoTempo)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject QGrafico::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QGrafico.data,
      qt_meta_data_QGrafico,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QGrafico::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QGrafico::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QGrafico.stringdata0))
        return static_cast<void*>(const_cast< QGrafico*>(this));
    return QWidget::qt_metacast(_clname);
}

int QGrafico::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void QGrafico::gotoTempo(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_QGraficoRiquadro_t {
    QByteArrayData data[8];
    char stringdata0[63];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QGraficoRiquadro_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QGraficoRiquadro_t qt_meta_stringdata_QGraficoRiquadro = {
    {
QT_MOC_LITERAL(0, 0, 16), // "QGraficoRiquadro"
QT_MOC_LITERAL(1, 17, 9), // "gotoTempo"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 1), // "x"
QT_MOC_LITERAL(4, 30, 8), // "newtempo"
QT_MOC_LITERAL(5, 39, 4), // "goTo"
QT_MOC_LITERAL(6, 44, 12), // "valueChanged"
QT_MOC_LITERAL(7, 57, 5) // "value"

    },
    "QGraficoRiquadro\0gotoTempo\0\0x\0newtempo\0"
    "goTo\0valueChanged\0value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QGraficoRiquadro[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   34,    2, 0x08 /* Private */,
       6,    1,   37,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    7,

       0        // eod
};

void QGraficoRiquadro::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QGraficoRiquadro *_t = static_cast<QGraficoRiquadro *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->gotoTempo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->goTo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QGraficoRiquadro::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QGraficoRiquadro::gotoTempo)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject QGraficoRiquadro::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QGraficoRiquadro.data,
      qt_meta_data_QGraficoRiquadro,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QGraficoRiquadro::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QGraficoRiquadro::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QGraficoRiquadro.stringdata0))
        return static_cast<void*>(const_cast< QGraficoRiquadro*>(this));
    return QWidget::qt_metacast(_clname);
}

int QGraficoRiquadro::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void QGraficoRiquadro::gotoTempo(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
