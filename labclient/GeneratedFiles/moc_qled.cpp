/****************************************************************************
** Meta object code from reading C++ file 'qled.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qled.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qled.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QLed_t {
    QByteArrayData data[17];
    char stringdata0[159];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QLed_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QLed_t qt_meta_stringdata_QLed = {
    {
QT_MOC_LITERAL(0, 0, 4), // "QLed"
QT_MOC_LITERAL(1, 5, 10), // "mousePress"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 8), // "setState"
QT_MOC_LITERAL(4, 26, 5), // "state"
QT_MOC_LITERAL(5, 32, 11), // "toggleState"
QT_MOC_LITERAL(6, 44, 11), // "setFlashing"
QT_MOC_LITERAL(7, 56, 8), // "flashing"
QT_MOC_LITERAL(8, 65, 12), // "setFlashRate"
QT_MOC_LITERAL(9, 78, 4), // "rate"
QT_MOC_LITERAL(10, 83, 13), // "startFlashing"
QT_MOC_LITERAL(11, 97, 12), // "stopFlashing"
QT_MOC_LITERAL(12, 110, 8), // "diameter"
QT_MOC_LITERAL(13, 119, 5), // "color"
QT_MOC_LITERAL(14, 125, 9), // "alignment"
QT_MOC_LITERAL(15, 135, 13), // "Qt::Alignment"
QT_MOC_LITERAL(16, 149, 9) // "flashRate"

    },
    "QLed\0mousePress\0\0setState\0state\0"
    "toggleState\0setFlashing\0flashing\0"
    "setFlashRate\0rate\0startFlashing\0"
    "stopFlashing\0diameter\0color\0alignment\0"
    "Qt::Alignment\0flashRate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QLed[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       6,   64, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   52,    2, 0x0a /* Public */,
       5,    0,   55,    2, 0x0a /* Public */,
       6,    1,   56,    2, 0x0a /* Public */,
       8,    1,   59,    2, 0x0a /* Public */,
      10,    0,   62,    2, 0x0a /* Public */,
      11,    0,   63,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      12, QMetaType::Double, 0x00095103,
      13, QMetaType::QColor, 0x00095103,
      14, 0x80000000 | 15, 0x0009510b,
       4, QMetaType::Bool, 0x00095103,
       7, QMetaType::Bool, 0x00095103,
      16, QMetaType::Int, 0x00095103,

       0        // eod
};

void QLed::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QLed *_t = static_cast<QLed *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->mousePress((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->setState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->toggleState(); break;
        case 3: _t->setFlashing((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->setFlashRate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->startFlashing(); break;
        case 6: _t->stopFlashing(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QLed::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QLed::mousePress)) {
                *result = 0;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        QLed *_t = static_cast<QLed *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = _t->diameter(); break;
        case 1: *reinterpret_cast< QColor*>(_v) = _t->color(); break;
        case 2: *reinterpret_cast< Qt::Alignment*>(_v) = _t->alignment(); break;
        case 3: *reinterpret_cast< bool*>(_v) = _t->state(); break;
        case 4: *reinterpret_cast< bool*>(_v) = _t->isFlashing(); break;
        case 5: *reinterpret_cast< int*>(_v) = _t->flashRate(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        QLed *_t = static_cast<QLed *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setDiameter(*reinterpret_cast< double*>(_v)); break;
        case 1: _t->setColor(*reinterpret_cast< QColor*>(_v)); break;
        case 2: _t->setAlignment(*reinterpret_cast< Qt::Alignment*>(_v)); break;
        case 3: _t->setState(*reinterpret_cast< bool*>(_v)); break;
        case 4: _t->setFlashing(*reinterpret_cast< bool*>(_v)); break;
        case 5: _t->setFlashRate(*reinterpret_cast< int*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

const QMetaObject QLed::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_QLed.data,
      qt_meta_data_QLed,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QLed::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QLed::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QLed.stringdata0))
        return static_cast<void*>(const_cast< QLed*>(this));
    return QWidget::qt_metacast(_clname);
}

int QLed::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void QLed::mousePress(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
