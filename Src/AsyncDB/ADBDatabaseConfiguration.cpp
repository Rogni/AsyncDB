#include <Include/AsyncDB/ADBDatabaseConfiguration.h>

ADBDatabaseConfiguration::ADBDatabaseConfiguration(QObject *parent): QObject(parent) {}

QString ADBDatabaseConfiguration::type() const
{
    return m_type;
}

QString ADBDatabaseConfiguration::databaseName() const
{
    return m_connectionName;
}

void ADBDatabaseConfiguration::setType(QString type)
{
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged(m_type);
}

void ADBDatabaseConfiguration::setConnectionName(QString connectionName)
{
    if (m_connectionName == connectionName)
        return;

    m_connectionName = connectionName;
    emit connectionNameChanged(m_connectionName);
}
