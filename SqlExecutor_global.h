#ifndef SQLEXECUTOR_GLOBAL_H
#define SQLEXECUTOR_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QString>
#include <QObject>
#include <QSettings>
#include <QThread>
#include <QScopedPointer>


#define NO_COPY(Class)\
    Class(const Class&) = delete;\
    Class& operator=(const Class&) = delete;

#define NO_MOVE(Class)\
    Class(Class&&) noexcept = delete;\
    Class& operator=(Class&&) noexcept = delete;

#define NO_COPY_MOVE(Class)\
    NO_COPY(Class)\
    NO_MOVE(Class)

#define SINGLETON(Class)\
    NO_COPY_MOVE(Class)\
    public:\
    inline static Class* instance() \
    {\
        static Class object;\
        return &object;\
    }\
    private:\
    Class()



#if defined(SQLEXECUTOR_LIBRARY)
#  define SQLEXECUTOR_EXPORT Q_DECL_EXPORT
#else
#  define SQLEXECUTOR_EXPORT Q_DECL_IMPORT
#endif


#endif // SQLEXECUTOR_GLOBAL_H
