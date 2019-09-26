#include <Include/AsyncDB/ADBDatabase.h>
#include "ADBError.h"
#include <QSqlError>



ADBDatabase::ADBDatabase(QObject *parent): QObject(parent)
{

}

ADBDatabaseConfiguration *ADBDatabase::configuration() const
{
    return m_configuration;
}

void ADBDatabase::execute(std::function<std::function<void ()> (QSqlDatabase)> executed)
{
    auto exec = [db = m_db, executed] () {
        QSqlDatabase database = db;
        ADBError::Ptr error;
        if (!database.isOpen()) {
            if (!database.open()) {
                error = ADBError::make(database.lastError().text(), database.lastError().databaseText());
            }
        }
        std::function<void()> resolve;
        if (database.isOpen()) {
            resolve = executed(database);
        }

        return [resolve, error] () {
            if (error) {
                
            } else {
                resolve();
            }
        };
    };

    auto errHandler = [](std::exception_ptr) {};

    m_threadExecutor(exec, errHandler);
}

void ADBDatabase::setConfiguration(ADBDatabaseConfiguration *configuration)
{
    if (m_configuration == configuration)
        return;

    m_configuration = configuration;
    if (m_configuration) {
        connect(m_configuration, &ADBDatabaseConfiguration::configurationChanged, this, [this] () {
            m_db = QSqlDatabase::addDatabase(m_configuration->type());
            m_db.setDatabaseName(m_configuration->databaseName());
        });

        m_db = QSqlDatabase::addDatabase(m_configuration->type());
        m_db.setDatabaseName(m_configuration->databaseName());
    }

    emit configurationChanged(m_configuration);
}
