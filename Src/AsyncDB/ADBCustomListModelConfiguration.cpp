#include <AsyncDB/ADBCustomListModelConfiguration.h>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QVector>
#include <QDebug>
#include <QThread>

ADBCustomListModelConfiguration::ADBCustomListModelConfiguration(QObject *parent): ADBAbstractListModelConfiguration(parent) {}

QString ADBCustomListModelConfiguration::selectQuery() const
{
    return m_selectQuery;
}

QString ADBCustomListModelConfiguration::insertQuery() const
{
    return m_insertQuery;
}

QString ADBCustomListModelConfiguration::updateQuery() const
{
    return m_updateQuery;
}

void ADBCustomListModelConfiguration::setSelectQuery(QString selectQuery)
{
    if (m_selectQuery == selectQuery)
        return;

    m_selectQuery = selectQuery;
    emit selectQueryChanged(m_selectQuery);
    emit reselectItems();
}

void ADBCustomListModelConfiguration::setInsertQuery(QString insertQuery)
{
    if (m_insertQuery == insertQuery)
        return;

    m_insertQuery = insertQuery;
    emit insertQueryChanged(m_insertQuery);
}

void ADBCustomListModelConfiguration::setUpdateQuery(QString updateQuery)
{
    if (m_updateQuery == updateQuery)
        return;

    m_updateQuery = updateQuery;
    emit updateQueryChanged(m_updateQuery);
}

void ADBCustomListModelConfiguration::select(std::function<void (QVector<QVariantMap>, QStringList)> callback)
{
    if (database()) {
        auto exec = [callback, queryStr = m_selectQuery] (QSqlDatabase db) {
            QVector<QVariantMap> result;
            QStringList roles;
            if (queryStr != "" ) {
                QSqlQuery query(db);
                query.prepare(queryStr);
                query.exec();

                auto s = query.size();
                result.reserve( s > -1 ? s : 0 );

                while (query.next()) {
                    QVariantMap record;
                    auto fieldsCount =query.record().count();
                    for (int i = 0; i < fieldsCount; ++i) {
                        auto name = query.record().fieldName(i);
                        auto value = query.value(name);
                        record[name] = value;
                    }
                    result.push_back(std::move(record));
                }
                if (!result.empty()) roles = result.first().keys();
            }
            return [callback, roles = std::move(roles), result = std::move(result)] () {

                callback(result, roles);
            };
        };
        database()->execute(exec);
    }
}

void ADBCustomListModelConfiguration::update(QVariantMap from, QVariantMap to, std::function<void ()>)
{
    if (database()) {

    }
}

void ADBCustomListModelConfiguration::insert(QVector<QVariantMap> item, std::function<void ()>)
{
    if (database()) {

    }
}
