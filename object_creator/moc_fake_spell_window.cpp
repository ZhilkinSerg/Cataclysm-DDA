/****************************************************************************
** Meta object code from reading C++ file 'fake_spell_window.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.1.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "fake_spell_window.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fake_spell_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.1.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_creator__fake_spell_window_t {
    const uint offsetsAndSize[6];
    char stringdata0[37];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_creator__fake_spell_window_t, stringdata0) + ofs), len
static const qt_meta_stringdata_creator__fake_spell_window_t
qt_meta_stringdata_creator__fake_spell_window = {
    {
        QT_MOC_LITERAL( 0, 26 ), // "creator::fake_spell_window"
        QT_MOC_LITERAL( 27, 8 ), // "modified"
        QT_MOC_LITERAL( 36, 0 ) // ""

    },
    "creator::fake_spell_window\0modified\0"
    ""
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_creator__fake_spell_window[] = {

    // content:
    9,       // revision
    0,       // classname
    0,    0, // classinfo
    1,   14, // methods
    0,    0, // properties
    0,    0, // enums/sets
    0,    0, // constructors
    0,       // flags
    1,       // signalCount

    // signals: name, argc, parameters, tag, flags, initial metatype offsets
    1,    0,   20,    2, 0x06,    0 /* Public */,

    // signals: parameters
    QMetaType::Void,

    0        // eod
};

void creator::fake_spell_window::qt_static_metacall( QObject *_o, QMetaObject::Call _c, int _id,
        void **_a )
{
    if( _c == QMetaObject::InvokeMetaMethod ) {
        auto *_t = static_cast<fake_spell_window *>( _o );
        ( void )_t;
        switch( _id ) {
            case 0:
                _t->modified();
                break;
            default:
                ;
        }
    } else if( _c == QMetaObject::IndexOfMethod ) {
        int *result = reinterpret_cast<int *>( _a[0] );
        {
            using _t = void ( fake_spell_window::* )();
            if( *reinterpret_cast<_t *>( _a[1] ) == static_cast<_t>( &fake_spell_window::modified ) ) {
                *result = 0;
                return;
            }
        }
    }
    ( void )_a;
}

const QMetaObject creator::fake_spell_window::staticMetaObject = { {
        QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
        qt_meta_stringdata_creator__fake_spell_window.offsetsAndSize,
        qt_meta_data_creator__fake_spell_window,
        qt_static_metacall,
        nullptr,
        qt_incomplete_metaTypeArray<qt_meta_stringdata_creator__fake_spell_window_t
        , QtPrivate::TypeAndForceComplete<void, std::false_type>



        >,
        nullptr
    }
};


const QMetaObject *creator::fake_spell_window::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *creator::fake_spell_window::qt_metacast( const char *_clname )
{
    if( !_clname ) {
        return nullptr;
    }
    if( !strcmp( _clname, qt_meta_stringdata_creator__fake_spell_window.stringdata0 ) ) {
        return static_cast<void *>( this );
    }
    return QMainWindow::qt_metacast( _clname );
}

int creator::fake_spell_window::qt_metacall( QMetaObject::Call _c, int _id, void **_a )
{
    _id = QMainWindow::qt_metacall( _c, _id, _a );
    if( _id < 0 ) {
        return _id;
    }
    if( _c == QMetaObject::InvokeMetaMethod ) {
        if( _id < 1 ) {
            qt_static_metacall( this, _c, _id, _a );
        }
        _id -= 1;
    } else if( _c == QMetaObject::RegisterMethodArgumentMetaType ) {
        if( _id < 1 ) {
            *reinterpret_cast<QMetaType *>( _a[0] ) = QMetaType();
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void creator::fake_spell_window::modified()
{
    QMetaObject::activate( this, &staticMetaObject, 0, nullptr );
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
