#include "sqlexecutor.h"
#include <QSqlQuery>
#include <QThread>
#include <QSqlError>
#include <QSqlField>
#include <QSqlRecord>


SqlExecutor::SqlExecutor() noexcept
{
    initThread();
}

SqlExecutor::SqlExecutor(const QString &driver, const QString &connectionName,
                         const QString &hostname, const QString &databaseName,
                         const QString &username, const QString &password,
                         uint32_t port) noexcept :
    m_driver(driver)
  , m_connectionName(connectionName)
  , m_hostname(hostname)
  , m_databaseName(databaseName)
  , m_username(username)
  , m_password(password)
  , m_port(port)
{
    initThread();
}

SqlExecutor::~SqlExecutor() noexcept
{
    if (!m_thread->isFinished()) {
        m_thread->quit();
        m_thread->wait();
    }
}

QString SqlExecutor::driver() const noexcept
{
    return m_driver;
}

QString SqlExecutor::connectionName() const noexcept
{
    return m_connectionName;
}

QString SqlExecutor::hostname() const noexcept
{
    return m_hostname;
}

QString SqlExecutor::databaseName() const noexcept
{
    return m_databaseName;
}

QString SqlExecutor::username() const noexcept
{
    return m_username;
}

QString SqlExecutor::password() const noexcept
{
    return m_password;
}

uint32_t SqlExecutor::port() const noexcept
{
    return m_port;
}

void SqlExecutor::setDriver(const QString &driver) noexcept
{
    m_driver = driver;
}

void SqlExecutor::setConnectionName(const QString &connectionName) noexcept
{
    m_connectionName = connectionName;
}

void SqlExecutor::setHostname(const QString &hostname) noexcept
{
    m_hostname = hostname;
}

void SqlExecutor::setDatabaseName(const QString &databaseName) noexcept
{
    m_databaseName = databaseName;
}

void SqlExecutor::setUsername(const QString &username) noexcept
{
    m_username = username;
}

void SqlExecutor::setPassword(const QString password) noexcept
{
    m_password = password;
}

void SqlExecutor::setPort(uint32_t port) noexcept
{
    m_port = port;
}

void SqlExecutor::executeQuery(const QString &query) noexcept
{
    if (m_thread->isRunning()) {
        return;
    }
    m_query = query;
    m_thread->start();
}

QString SqlExecutor::query() const noexcept
{
    return m_query;
}

bool SqlExecutor::connectToDatase() noexcept
{
   if (QSqlDatabase::contains(m_databaseName)) {
       m_database = QSqlDatabase::database(m_connectionName);
       return m_database.open();
   }

   m_database = QSqlDatabase::addDatabase(m_driver, m_connectionName);
   m_database.setDatabaseName(m_databaseName);
   m_database.setHostName(m_hostname);
   m_database.setUserName(m_username);
   m_database.setPassword(m_password);
   m_database.setPort(m_port);
   return m_database.open();
}

void SqlExecutor::execute() noexcept
{
    if (!m_database.isOpen()) {
        bool isConnected = connectToDatase();
        if (!isConnected) {
            emit error(m_database.lastError());
            return;
        }
    }

    QSqlQuery query(m_query, m_database);
    if (query.exec()) {
        if (query.isSelect()) {
            StringVector headers;
            SqlResult data;

            VariantVector record;
            while(query.next()) {
                if (headers.isEmpty()) {
                    for (int i = 0; i < query.record().count(); ++i) {
                        headers.push_back(query.record().fieldName(i));
                    }
                }

                for (int i = 0; i < query.record().count(); ++i) {
                    record.push_back(query.record().value(i));
                }
                data.push_back(record);
                record.clear();
            }

            emit finished(headers, data);
        } else {
            emit finished(std::nullopt, std::nullopt);
        }
    } else {
        emit error(query.lastQuery());
    }

    m_thread->quit();
}

void SqlExecutor::initThread()
{
    m_thread.reset(new QThread);
    moveToThread(m_thread.data());
    connect(m_thread.data(), &QThread::started, this, &SqlExecutor::execute);
}
