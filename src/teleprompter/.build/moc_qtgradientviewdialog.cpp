/****************************************************************************
** Meta object code from reading C++ file 'qtgradientviewdialog.h'
**
** Created: Sat Dec 19 21:09:16 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../3rdparty/qtgradienteditor/qtgradientviewdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtgradientviewdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtGradientViewDialog[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      25,   22,   21,   21, 0x08,
      55,   22,   21,   21, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QtGradientViewDialog[] = {
    "QtGradientViewDialog\0\0id\0"
    "slotGradientSelected(QString)\0"
    "slotGradientActivated(QString)\0"
};

const QMetaObject QtGradientViewDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_QtGradientViewDialog,
      qt_meta_data_QtGradientViewDialog, 0 }
};

const QMetaObject *QtGradientViewDialog::metaObject() const
{
    return &staticMetaObject;
}

void *QtGradientViewDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtGradientViewDialog))
        return static_cast<void*>(const_cast< QtGradientViewDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int QtGradientViewDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slotGradientSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: slotGradientActivated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE