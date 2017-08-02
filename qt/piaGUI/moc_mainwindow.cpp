/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MyWidget_t {
    QByteArrayData data[16];
    char stringdata0[195];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyWidget_t qt_meta_stringdata_MyWidget = {
    {
QT_MOC_LITERAL(0, 0, 8), // "MyWidget"
QT_MOC_LITERAL(1, 9, 16), // "overAreaCal_Type"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 15), // "QVector<double>"
QT_MOC_LITERAL(4, 43, 16), // "QVector<double>&"
QT_MOC_LITERAL(5, 60, 11), // "overAreaCal"
QT_MOC_LITERAL(6, 72, 8), // "openfile"
QT_MOC_LITERAL(7, 81, 8), // "readfile"
QT_MOC_LITERAL(8, 90, 9), // "writefile"
QT_MOC_LITERAL(9, 100, 13), // "allSelectFunc"
QT_MOC_LITERAL(10, 114, 8), // "IA_check"
QT_MOC_LITERAL(11, 123, 8), // "progress"
QT_MOC_LITERAL(12, 132, 14), // "do_calibration"
QT_MOC_LITERAL(13, 147, 21), // "do_feature_extraction"
QT_MOC_LITERAL(14, 169, 13), // "alignmentName"
QT_MOC_LITERAL(15, 183, 11) // "featureName"

    },
    "MyWidget\0overAreaCal_Type\0\0QVector<double>\0"
    "QVector<double>&\0overAreaCal\0openfile\0"
    "readfile\0writefile\0allSelectFunc\0"
    "IA_check\0progress\0do_calibration\0"
    "do_feature_extraction\0alignmentName\0"
    "featureName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   74,    2, 0x09 /* Protected */,
       5,    0,   81,    2, 0x09 /* Protected */,
       6,    0,   82,    2, 0x09 /* Protected */,
       7,    1,   83,    2, 0x09 /* Protected */,
       8,    0,   86,    2, 0x09 /* Protected */,
       9,    0,   87,    2, 0x09 /* Protected */,
      10,    1,   88,    2, 0x09 /* Protected */,
      11,    0,   91,    2, 0x09 /* Protected */,
      12,    0,   92,    2, 0x09 /* Protected */,
      13,    0,   93,    2, 0x09 /* Protected */,
      14,    1,   94,    2, 0x09 /* Protected */,
      15,    1,   97,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 4,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QString, QMetaType::Int,    2,
    QMetaType::QString, QMetaType::Int,    2,

       0        // eod
};

void MyWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyWidget *_t = static_cast<MyWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->overAreaCal_Type((*reinterpret_cast< QVector<double>(*)>(_a[1])),(*reinterpret_cast< QVector<double>(*)>(_a[2])),(*reinterpret_cast< QVector<double>(*)>(_a[3]))); break;
        case 1: _t->overAreaCal(); break;
        case 2: _t->openfile(); break;
        case 3: _t->readfile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->writefile(); break;
        case 5: _t->allSelectFunc(); break;
        case 6: _t->IA_check((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->progress(); break;
        case 8: _t->do_calibration(); break;
        case 9: _t->do_feature_extraction(); break;
        case 10: { QString _r = _t->alignmentName((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 11: { QString _r = _t->featureName((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<double> >(); break;
            }
            break;
        }
    }
}

const QMetaObject MyWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MyWidget.data,
      qt_meta_data_MyWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MyWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MyWidget.stringdata0))
        return static_cast<void*>(const_cast< MyWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int MyWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
