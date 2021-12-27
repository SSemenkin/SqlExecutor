#ifndef SQLEXECUTORPOOL_H
#define SQLEXECUTORPOOL_H
#include "SqlExecutor_global.h"

#include <QMutex>

class SqlExecutor;

class SQLEXECUTOR_EXPORT SqlExecutorPool
{
public:

    bool isIdle() const;
    void append(SqlExecutor *executor);
    bool remove(SqlExecutor *executor);

private:
    QList<SqlExecutor*> m_executors;
    mutable QMutex m_mutex;

    SINGLETON(SqlExecutorPool);

};

#endif // SQLEXECUTORPOOL_H
