#include <AsyncDB/ADBAbstractListModelConfiguration.h>


ADBAbstractListModelConfiguration::ADBAbstractListModelConfiguration(QObject *parent): QObject(parent) {}

ADBDatabase *ADBAbstractListModelConfiguration::database() const
{
    return m_database;
}

void ADBAbstractListModelConfiguration::select(std::function<void (QVector<QVariantMap>, QStringList)> callback)
{

}

void ADBAbstractListModelConfiguration::update(QVariantMap from, QVariantMap to, std::function<void ()>)
{

}

void ADBAbstractListModelConfiguration::insert(QVariantMap item, std::function<void()>)
{

}

void ADBAbstractListModelConfiguration::setDatabase(ADBDatabase *database)
{
    if (m_database == database)
        return;

    m_database = database;
    emit databaseChanged(m_database);
    emit reselectItems();
}
