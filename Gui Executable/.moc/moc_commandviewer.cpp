/****************************************************************************
** Meta object code from reading C++ file 'commandviewer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../commandviewer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'commandviewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CommandViewer_t {
    QByteArrayData data[11];
    char stringdata[177];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CommandViewer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CommandViewer_t qt_meta_stringdata_CommandViewer = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CommandViewer"
QT_MOC_LITERAL(1, 14, 17), // "fileStatusChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 15), // "triggerPointMap"
QT_MOC_LITERAL(4, 49, 26), // "triggerCommandEditorUpdate"
QT_MOC_LITERAL(5, 76, 8), // "QString*"
QT_MOC_LITERAL(6, 85, 14), // "CommandEditor*"
QT_MOC_LITERAL(7, 100, 14), // "MoveUp_clicked"
QT_MOC_LITERAL(8, 115, 16), // "MoveDown_clicked"
QT_MOC_LITERAL(9, 132, 21), // "DeleteCommand_clicked"
QT_MOC_LITERAL(10, 154, 22) // "on_EditCommand_clicked"

    },
    "CommandViewer\0fileStatusChanged\0\0"
    "triggerPointMap\0triggerCommandEditorUpdate\0"
    "QString*\0CommandEditor*\0MoveUp_clicked\0"
    "MoveDown_clicked\0DeleteCommand_clicked\0"
    "on_EditCommand_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CommandViewer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    0,   50,    2, 0x06 /* Public */,
       4,    3,   51,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   58,    2, 0x08 /* Private */,
       8,    0,   59,    2, 0x08 /* Private */,
       9,    0,   60,    2, 0x08 /* Private */,
      10,    0,   61,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 5, 0x80000000 | 6,    2,    2,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CommandViewer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CommandViewer *_t = static_cast<CommandViewer *>(_o);
        switch (_id) {
        case 0: _t->fileStatusChanged(); break;
        case 1: _t->triggerPointMap(); break;
        case 2: _t->triggerCommandEditorUpdate((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString*(*)>(_a[2])),(*reinterpret_cast< CommandEditor*(*)>(_a[3]))); break;
        case 3: _t->MoveUp_clicked(); break;
        case 4: _t->MoveDown_clicked(); break;
        case 5: _t->DeleteCommand_clicked(); break;
        case 6: _t->on_EditCommand_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< CommandEditor* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CommandViewer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommandViewer::fileStatusChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (CommandViewer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommandViewer::triggerPointMap)) {
                *result = 1;
            }
        }
        {
            typedef void (CommandViewer::*_t)(QString , QString * , CommandEditor * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommandViewer::triggerCommandEditorUpdate)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject CommandViewer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CommandViewer.data,
      qt_meta_data_CommandViewer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CommandViewer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CommandViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CommandViewer.stringdata))
        return static_cast<void*>(const_cast< CommandViewer*>(this));
    return QWidget::qt_metacast(_clname);
}

int CommandViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void CommandViewer::fileStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void CommandViewer::triggerPointMap()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void CommandViewer::triggerCommandEditorUpdate(QString _t1, QString * _t2, CommandEditor * _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
