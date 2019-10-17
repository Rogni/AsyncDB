#include <AsyncDB/ADBListModelConfiguration.h>

#include <QSqlQuery>
#include <QVector>
#include <QSqlError>
#include <QDebug>
#include <QVariant>

#include <Src/AsyncDB/ADBQueryHelpers.h>


ADBListModelConfiguration::ADBListModelConfiguration(QObject *parent): ADBAbstractListModelConfiguration(parent) {}

QStringList ADBListModelConfiguration::columns() const
{
    return m_columns;
}

QString ADBListModelConfiguration::tablename() const
{
    return m_tablename;
}

void ADBListModelConfiguration::setColumns(QStringList columns)
{
    if (m_columns == columns)
        return;

    m_columns = columns;
    emit reselectItems();
    emit columnsChanged(m_columns);
}

void ADBListModelConfiguration::setTablename(QString tablename)
{
    if (m_tablename == tablename)
        return;

    m_tablename = tablename;
    emit reselectItems();
    emit tablenameChanged(m_tablename);
}


void ADBListModelConfiguration::select(std::function<void (QVector<QVariantMap>, QStringList)> callback)
{
    auto columnsTemp = columns();

    database()->execute(
                [callback,
                table = tablename(),
                columns = std::move(columnsTemp)] (QSqlDatabase db) {



        QVector<QVariantMap> records;
        if (!columns.isEmpty()) {
            auto parser = [&records] (QStringList && , QVector<QVariantMap> && r) {
                records = std::move(r);
            };

            auto handleSelectResponse = parseSelectQuery(parser)(queryExit());
            auto handleErr = handleSqlError([](QSqlQuery q) { qDebug() << Q_FUNC_INFO << q.lastError().text(); })(handleSelectResponse);
            auto exec = executeQuery()(handleErr);
            auto setupQuery = makeSelectQuery(columns, {table})(exec);

            QSqlQuery query(db);
            setupQuery(query);
        }
        return [callback, r = std::move(records), columns]() {
            callback(r, columns);
        };
    } );
}


ADBAbstractListModelConfiguration::DbFunctor ADBListModelConfiguration::updateFunctor(QVariantMap from, QVariantMap to)
{
    return [table = tablename(),
            columns = columns(),
            oldRecord = std::move(from),
            newRecord = std::move(to)] (QSqlDatabase db) {
        auto handleErr = handleSqlError([](QSqlQuery q) { qDebug()<< Q_FUNC_INFO << q.lastError().text(); })(queryExit());
        auto exec = executeQuery()(handleErr);
        auto setupQuery = makeUpdateQuery(std::move(table), std::move(oldRecord), std::move(newRecord))(exec);

        QSqlQuery query(db);
        setupQuery(query);
    };
}

ADBAbstractListModelConfiguration::DbFunctor ADBListModelConfiguration::insertFunctor(QVector<QVariantMap> items)
{
    return [
            items = std::move(items),
            table = tablename(),
            columns = columns()
            ] (QSqlDatabase db) {
        if (!items.isEmpty()) {
            auto handleErr = handleSqlError([](QSqlQuery q) { qDebug()<< Q_FUNC_INFO << q.lastError().text(); })(queryExit());
            auto exec = executeQuery()(handleErr);
            auto setupQuery = makeInsertQuery(std::move(table), std::move(columns), std::move(items))(exec);
            QSqlQuery query(db);
            setupQuery(query);
        }

    };
}


