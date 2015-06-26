/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[30];
    char stringdata[509];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 12), // "recievePoint"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 1), // "x"
QT_MOC_LITERAL(4, 27, 1), // "y"
QT_MOC_LITERAL(5, 29, 10), // "pointCount"
QT_MOC_LITERAL(6, 40, 26), // "on_actionSave_As_triggered"
QT_MOC_LITERAL(7, 67, 23), // "on_actionOpen_triggered"
QT_MOC_LITERAL(8, 91, 33), // "on_actionDraw_Point_Map_trigg..."
QT_MOC_LITERAL(9, 125, 8), // "closeTab"
QT_MOC_LITERAL(10, 134, 5), // "index"
QT_MOC_LITERAL(11, 140, 23), // "on_actionSave_triggered"
QT_MOC_LITERAL(12, 164, 15), // "fileChangedTrue"
QT_MOC_LITERAL(13, 180, 22), // "on_actionNew_triggered"
QT_MOC_LITERAL(14, 203, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(15, 225, 22), // "on_actionRun_triggered"
QT_MOC_LITERAL(16, 248, 18), // "noticeCommandAdded"
QT_MOC_LITERAL(17, 267, 23), // "on_actionStop_triggered"
QT_MOC_LITERAL(18, 291, 24), // "on_actionPause_triggered"
QT_MOC_LITERAL(19, 316, 23), // "on_actionNext_triggered"
QT_MOC_LITERAL(20, 340, 27), // "on_actionPrevious_triggered"
QT_MOC_LITERAL(21, 368, 10), // "tabChanged"
QT_MOC_LITERAL(22, 379, 10), // "callUpdate"
QT_MOC_LITERAL(23, 390, 8), // "fileName"
QT_MOC_LITERAL(24, 399, 8), // "QString*"
QT_MOC_LITERAL(25, 408, 11), // "ProjectName"
QT_MOC_LITERAL(26, 420, 14), // "CommandEditor*"
QT_MOC_LITERAL(27, 435, 12), // "loadedEditor"
QT_MOC_LITERAL(28, 448, 29), // "on_actionColors_exe_triggered"
QT_MOC_LITERAL(29, 478, 30) // "on_actionCapture_exe_triggered"

    },
    "MainWindow\0recievePoint\0\0x\0y\0pointCount\0"
    "on_actionSave_As_triggered\0"
    "on_actionOpen_triggered\0"
    "on_actionDraw_Point_Map_triggered\0"
    "closeTab\0index\0on_actionSave_triggered\0"
    "fileChangedTrue\0on_actionNew_triggered\0"
    "on_pushButton_clicked\0on_actionRun_triggered\0"
    "noticeCommandAdded\0on_actionStop_triggered\0"
    "on_actionPause_triggered\0"
    "on_actionNext_triggered\0"
    "on_actionPrevious_triggered\0tabChanged\0"
    "callUpdate\0fileName\0QString*\0ProjectName\0"
    "CommandEditor*\0loadedEditor\0"
    "on_actionColors_exe_triggered\0"
    "on_actionCapture_exe_triggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,  109,    2, 0x0a /* Public */,
       6,    0,  116,    2, 0x08 /* Private */,
       7,    0,  117,    2, 0x08 /* Private */,
       8,    0,  118,    2, 0x08 /* Private */,
       9,    1,  119,    2, 0x08 /* Private */,
      11,    0,  122,    2, 0x08 /* Private */,
      12,    0,  123,    2, 0x08 /* Private */,
      13,    0,  124,    2, 0x08 /* Private */,
      14,    0,  125,    2, 0x08 /* Private */,
      15,    0,  126,    2, 0x08 /* Private */,
      16,    1,  127,    2, 0x08 /* Private */,
      17,    0,  130,    2, 0x08 /* Private */,
      18,    0,  131,    2, 0x08 /* Private */,
      19,    0,  132,    2, 0x08 /* Private */,
      20,    0,  133,    2, 0x08 /* Private */,
      21,    1,  134,    2, 0x08 /* Private */,
      22,    3,  137,    2, 0x08 /* Private */,
      28,    0,  144,    2, 0x08 /* Private */,
      29,    0,  145,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 24, 0x80000000 | 26,   23,   25,   27,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->recievePoint((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->on_actionSave_As_triggered(); break;
        case 2: _t->on_actionOpen_triggered(); break;
        case 3: _t->on_actionDraw_Point_Map_triggered(); break;
        case 4: _t->closeTab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_actionSave_triggered(); break;
        case 6: _t->fileChangedTrue(); break;
        case 7: _t->on_actionNew_triggered(); break;
        case 8: _t->on_pushButton_clicked(); break;
        case 9: _t->on_actionRun_triggered(); break;
        case 10: _t->noticeCommandAdded((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->on_actionStop_triggered(); break;
        case 12: _t->on_actionPause_triggered(); break;
        case 13: _t->on_actionNext_triggered(); break;
        case 14: _t->on_actionPrevious_triggered(); break;
        case 15: _t->tabChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->callUpdate((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString*(*)>(_a[2])),(*reinterpret_cast< CommandEditor*(*)>(_a[3]))); break;
        case 17: _t->on_actionColors_exe_triggered(); break;
        case 18: _t->on_actionCapture_exe_triggered(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 16:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< CommandEditor* >(); break;
            }
            break;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
