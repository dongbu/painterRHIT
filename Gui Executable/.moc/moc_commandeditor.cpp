/****************************************************************************
** Meta object code from reading C++ file 'commandeditor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../commandeditor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'commandeditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CommandEditor_t {
    QByteArrayData data[12];
    char stringdata[169];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CommandEditor_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CommandEditor_t qt_meta_stringdata_CommandEditor = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CommandEditor"
QT_MOC_LITERAL(1, 14, 17), // "fileStatusChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 18), // "tell_Command_Added"
QT_MOC_LITERAL(4, 52, 5), // "index"
QT_MOC_LITERAL(5, 58, 19), // "signal_Info_Changed"
QT_MOC_LITERAL(6, 78, 18), // "sendUpdateToDrawOn"
QT_MOC_LITERAL(7, 97, 14), // "CommandEditor*"
QT_MOC_LITERAL(8, 112, 6), // "editor"
QT_MOC_LITERAL(9, 119, 19), // "Add_Command_Clicked"
QT_MOC_LITERAL(10, 139, 17), // "Add_Point_Clicked"
QT_MOC_LITERAL(11, 157, 11) // "InfoChanged"

    },
    "CommandEditor\0fileStatusChanged\0\0"
    "tell_Command_Added\0index\0signal_Info_Changed\0"
    "sendUpdateToDrawOn\0CommandEditor*\0"
    "editor\0Add_Command_Clicked\0Add_Point_Clicked\0"
    "InfoChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CommandEditor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    1,   55,    2, 0x06 /* Public */,
       3,    0,   58,    2, 0x06 /* Public */,
       5,    0,   59,    2, 0x06 /* Public */,
       6,    1,   60,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   63,    2, 0x0a /* Public */,
      10,    0,   64,    2, 0x0a /* Public */,
      11,    0,   65,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CommandEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CommandEditor *_t = static_cast<CommandEditor *>(_o);
        switch (_id) {
        case 0: _t->fileStatusChanged(); break;
        case 1: _t->tell_Command_Added((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->tell_Command_Added(); break;
        case 3: _t->signal_Info_Changed(); break;
        case 4: _t->sendUpdateToDrawOn((*reinterpret_cast< CommandEditor*(*)>(_a[1]))); break;
        case 5: _t->Add_Command_Clicked(); break;
        case 6: _t->Add_Point_Clicked(); break;
        case 7: _t->InfoChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< CommandEditor* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CommandEditor::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommandEditor::fileStatusChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (CommandEditor::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommandEditor::tell_Command_Added)) {
                *result = 1;
            }
        }
        {
            typedef void (CommandEditor::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommandEditor::tell_Command_Added)) {
                *result = 2;
            }
        }
        {
            typedef void (CommandEditor::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommandEditor::signal_Info_Changed)) {
                *result = 3;
            }
        }
        {
            typedef void (CommandEditor::*_t)(CommandEditor * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommandEditor::sendUpdateToDrawOn)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject CommandEditor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CommandEditor.data,
      qt_meta_data_CommandEditor,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CommandEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CommandEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CommandEditor.stringdata))
        return static_cast<void*>(const_cast< CommandEditor*>(this));
    return QObject::qt_metacast(_clname);
}

int CommandEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void CommandEditor::fileStatusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void CommandEditor::tell_Command_Added(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CommandEditor::tell_Command_Added()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void CommandEditor::signal_Info_Changed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void CommandEditor::sendUpdateToDrawOn(CommandEditor * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
