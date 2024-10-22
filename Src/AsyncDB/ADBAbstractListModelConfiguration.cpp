#include <AsyncDB/ADBAbstractListModelConfiguration.h>


ADBAbstractListModelConfiguration::ADBAbstractListModelConfiguration(QObject *parent): QObject(parent) {}

ADBDatabase *ADBAbstractListModelConfiguration::database() const
{
    return m_database;
}

void ADBAbstractListModelConfiguration::select(std::function<void (QVector<QVariantMap>, QStringList)> callback)
{

}

ADBAbstractListModelConfiguration::DbFunctor ADBAbstractListModelConfiguration::updateFunctor(QVariantMap from, QVariantMap to)
{
    return [] (QSqlDatabase) {};
}

ADBAbstractListModelConfiguration::DbFunctor ADBAbstractListModelConfiguration::insertFunctor(QVector<QVariantMap> items)
{
    return [] (QSqlDatabase) {};
}

void ADBAbstractListModelConfiguration::execute(std::function<std::function<void ()> (QSqlDatabase)> f)
{
    if (database()) {
        database()->execute(f);
    }
}

void ADBAbstractListModelConfiguration::setDatabase(ADBDatabase *database)
{
    if (m_database == database)
        return;

    m_database = database;
    emit databaseChanged(m_database);
    emit reselectItems();
}
