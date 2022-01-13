#ifndef SQLEXECUTOR_GLOBAL_H
#define SQLEXECUTOR_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QString>
#include <QObject>
#include <QThread>
#include <QScopedPointer>
#include <QVariant>


#include <global.h>



#if defined(SQLEXECUTOR_LIBRARY)
#  define SQLEXECUTOR_EXPORT Q_DECL_EXPORT
#else
#  define SQLEXECUTOR_EXPORT Q_DECL_IMPORT
#endif


#endif // SQLEXECUTOR_GLOBAL_H
