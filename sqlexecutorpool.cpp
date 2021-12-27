#include "sqlexecutorpool.h"
#include "sqlexecutor.h"

#include <QMutexLocker>

SqlExecutorPool::SqlExecutorPool()
{

}

bool SqlExecutorPool::isIdle() const
{
    QMutexLocker locker(&m_mutex);
    for (SqlExecutor *executor : m_executors) {
        if (!executor->thread()->isFinished()) {
            return false;
        }
    }
    return true;
}

void SqlExecutorPool::append(SqlExecutor *executor)
{
    QMutexLocker locker(&m_mutex);
    m_executors.push_back(executor);
}

bool SqlExecutorPool::remove(SqlExecutor *executor)
{
    QMutexLocker locker(&m_mutex);
    return m_executors.removeOne(executor);
}
