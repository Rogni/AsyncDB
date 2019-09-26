#ifndef ADBDATABASE_H
#define ADBDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <AsyncDB/ADBDatabaseConfiguration.h>
#include <Threads/ThreadManager.h>

class ADBDatabase: public QObject
{
    Q_OBJECT
    Q_PROPERTY(ADBDatabaseConfiguration* configuration
               READ configuration
               WRITE setConfiguration
               NOTIFY configurationChanged)

    ADBDatabaseConfiguration* m_configuration = nullptr;

public:
    ADBDatabase(QObject *parent = nullptr);
    ADBDatabaseConfiguration* configuration() const;

    void execute(std::function<std::function<void()>(QSqlDatabase)>);

public slots:
    void setConfiguration(ADBDatabaseConfiguration* configuration);

signals:
    void configurationChanged(ADBDatabaseConfiguration* configuration);


private:
    QSqlDatabase m_db;
    ThreadManager::Executor m_threadExecutor = ThreadManager::makeThread();
};

#endif // ADBDATABASE_H
