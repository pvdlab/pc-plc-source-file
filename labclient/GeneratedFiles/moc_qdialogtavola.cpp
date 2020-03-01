/****************************************************************************
** Meta object code from reading C++ file 'qdialogtavola.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qdialogtavola.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qdialogtavola.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QDialogTavola_t {
    QByteArrayData data[6];
    char stringdata0[75];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QDialogTavola_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QDialogTavola_t qt_meta_stringdata_QDialogTavola = {
    {
QT_MOC_LITERAL(0, 0, 13), // "QDialogTavola"
QT_MOC_LITERAL(1, 14, 19), // "on_OkButton_clicked"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 9), // "RowChange"
QT_MOC_LITERAL(4, 45, 5), // "index"
QT_MOC_LITERAL(5, 51, 23) // "on_ButtonChiudi_clicked"

    },
    "QDialogTavola\0on_OkButton_clicked\0\0"
    "RowChange\0index\0on_ButtonChiudi_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QDialogTavola[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x08 /* Private */,
       3,    1,   30,    2, 0x08 /* Private */,
       5,    0,   33,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QModelIndex,    4,
    QMetaType::Void,

       0        // eod
};

void QDialogTavola::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QDialogTavola *_t = static_cast<QDialogTavola *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_OkButton_clicked(); break;
        case 1: _t->RowChange((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 2: _t->on_ButtonChiudi_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject QDialogTavola::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QDialogTavola.data,
      qt_meta_data_QDialogTavola,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QDialogTavola::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QDialogTavola::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QDialogTavola.stringdata0))
        return static_cast<void*>(const_cast< QDialogTavola*>(this));
    return QDialog::qt_metacast(_clname);
}

int QDialogTavola::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
