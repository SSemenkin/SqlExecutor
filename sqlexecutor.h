#ifndef SQLEXECUTOR_H
#define SQLEXECUTOR_H

#include "sqlexecutorpool.h"
#include <QSqlDatabase>



class SQLEXECUTOR_EXPORT SqlExecutor : public QObject
{
    Q_OBJECT
public:
    explicit SqlExecutor (const QString &driver,
                          const QString &connection,
                          const QString &hostname,
                          const QString &database,
                          const QString &username,
                          const QString &password,
                          uint port);
    virtual ~SqlExecutor();

    void setQuery(const QString &query);
    const QString &query() const;

    void exec();

signals:
    void error(const QString &errorText);
    void executed(const QStringList &fieldnames, const QVector<QVariantList> &data);

protected:
    QString m_driver;
    QString m_connection;
    QString m_hostname;
    QString m_databasename;
    QString m_username;
    QString m_password;
    uint m_port;

private:
    virtual bool connectToDatabase();
    void executeQuery();

private:
    QString m_query;
    QScopedPointer<QThread> m_thread;
    QSqlDatabase m_database;
    SqlExecutorPool *m_ExecutorPool;
};

#endif // SQLEXECUTOR_H
