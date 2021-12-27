#include "sqlexecutor.h"
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>



SqlExecutor::SqlExecutor(const QString &driver,
                         const QString &connection,
                         const QString &hostname,
                         const QString &database,
                         const QString &username,
                         const QString &password,
                         uint port) :
      m_driver(driver),
      m_connection(connection),
      m_hostname(hostname),
      m_databasename(database),
      m_username(username),
      m_password(password),
      m_port(port),
      m_thread(new QThread)
{
    m_ExecutorPool = SqlExecutorPool::instance();
    m_ExecutorPool->append(this);
    moveToThread(m_thread.data());
    connect(m_thread.data(), &QThread::started, this, &SqlExecutor::executeQuery);
}

SqlExecutor::~SqlExecutor()
{
    while(!thread()->isFinished());
    m_ExecutorPool->remove(this);
}

void SqlExecutor::setQuery(const QString &query)
{
    m_query = query;
}

const QString &SqlExecutor::query() const
{
    return m_query;
}

void SqlExecutor::exec()
{
    thread()->start();
}

bool SqlExecutor::connectToDatabase()
{
    if (QSqlDatabase::contains(m_connection)) {
        m_database = QSqlDatabase::database(m_connection);
    } else {
        m_database = QSqlDatabase::addDatabase(m_driver, m_connection);
        m_database.setDatabaseName(m_databasename);
        m_database.setHostName(m_hostname);
        m_database.setUserName(m_username);
        m_database.setPassword(m_password);
        m_database.setPort(m_port);
    }
    return m_database.open();
}

void SqlExecutor::executeQuery()
{
    if (connectToDatabase()) {

        QSqlQuery query(m_database);

        if (query.exec(m_query)) {

            QStringList fieldnames;
            QVector<QVariantList> data;
            QVariantList row;

            while(query.next()) {

                if (fieldnames.isEmpty()) {
                    for (int i = 0; i < query.record().count(); ++i) {
                        fieldnames.push_back(query.record().fieldName(i));
                    }
                }


                for (int i = 0; i < query.record().count(); ++i) {
                    row << query.value(i);
                }

                data.push_back(row);
                row.clear();

            }
            emit executed(fieldnames, data);
        } else {
            emit error(query.lastError().text());
        }
    } else {
        emit error(m_database.lastError().text());
    }
    m_thread->quit();
}
