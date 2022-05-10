#ifndef SQLEXECUTOR_H
#define SQLEXECUTOR_H
#include <QObject>
#include <QSqlDatabase>

class QThread;

class SqlExecutor final : public QObject
{
    Q_OBJECT
    using VariantVector = QVector<QVariant>;
    using SqlResult = QVector<VariantVector>;
    using StringVector = QVector<QString>;
public:
    explicit SqlExecutor() noexcept;
    explicit SqlExecutor(const QString &driver, const QString &connectionName,
                         const QString &hostname, const QString &databaseName,
                         const QString &username, const QString &password,
                         uint32_t port) noexcept;
    ~SqlExecutor() noexcept;

    [[nodiscard]] QString driver () const noexcept;
    [[nodiscard]] QString connectionName() const noexcept;
    [[nodiscard]] QString hostname() const noexcept;
    [[nodiscard]] QString databaseName() const noexcept;
    [[nodiscard]] QString username() const noexcept;
    [[nodiscard]] QString password() const noexcept;
    [[nodiscard]] uint32_t port() const noexcept;

    void setDriver(const QString &driver) noexcept;
    void setConnectionName(const QString &connectionName) noexcept;
    void setHostname(const QString &hostname) noexcept;
    void setDatabaseName(const QString &databaseName) noexcept;
    void setUsername(const QString &username) noexcept;
    void setPassword(const QString password) noexcept;
    void setPort(uint32_t port) noexcept;

    void executeQuery(const QString &query) noexcept;
    QString query() const noexcept;

signals:
    void finished(const std::optional<StringVector> &headers,
                  const std::optional<SqlResult> &result);
    void error(const QSqlError &error);

private:
    bool connectToDatase() noexcept;
    void execute() noexcept;
    void initThread();
private:
    QString m_driver;
    QString m_connectionName;
    QString m_hostname;
    QString m_databaseName;
    QString m_username;
    QString m_password;
    uint32_t m_port;

    QString m_query;

    QSqlDatabase m_database;
    QScopedPointer<QThread> m_thread;
};

#endif // SQLEXECUTOR_H
