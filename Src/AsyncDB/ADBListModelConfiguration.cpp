#include <AsyncDB/ADBListModelConfiguration.h>

#include <QSqlQuery>
#include <QVector>
#include <QSqlError>
#include <QDebug>
#include <QVariant>

const QString SELECT_QUERY_STR_TEMPLATE = "SELECT %1 FROM %2 ";
const QString UPDATE_QUERY_STR_TEMPLATE = "UPDATE %1 "
                                          "SET %2 "
                                          "WHERE %3 ";
const QString INSERT_QUERY_STR_TEMPLATE = "INSERT INTO %1 ( %2 )"
                                          "VALUES( %3 )";

const QString OLD_VALUE_TEMPLATE_BIND = ":old_%1";
const QString NEW_VALUE_TEMPLATE_BIND = ":new_%1";
const QString OLD_VALUE_TEMPLATE = "%1 = :old_%1";
const QString NEW_VALUE_TEMPLATE = "%1 = :new_%1";

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
            QSqlQuery query(db);

            QString columnsStr = columns.join(", ");
            QString queryStr = SELECT_QUERY_STR_TEMPLATE.arg(columnsStr).arg(table);
            query.prepare(queryStr);
            query.exec();

            while (query.next()) {
                QVariantMap map;
                for (const auto &c: columns) {
                    map[c] = query.value(c);
                }
                records.push_back(std::move(map));
            }
        }
        return [callback, r = std::move(records), columns]() {
            callback(r, columns);
        };
    } );
}

void ADBListModelConfiguration::update(QVariantMap from, QVariantMap to, std::function<void ()> callback)
{
    if (database()) {
        database()->execute(
                    [table = tablename(),
                    columns = columns(),
                    oldRecord = std::move(from),
                    newRecord = std::move(to),
                    callback] (QSqlDatabase db) {

            QSqlQuery query(db);

            QStringList newColumns;
            QStringList oldColumns;

            newColumns.reserve(columns.size());
            oldColumns.reserve(columns.size());

            for (const auto &c: columns) {
                newColumns += NEW_VALUE_TEMPLATE.arg(c);
                oldColumns += OLD_VALUE_TEMPLATE.arg(c);
            }

            QString queryStr = UPDATE_QUERY_STR_TEMPLATE.arg(table).arg(newColumns.join(", ")).arg(oldColumns.join(" AND "));
            query.prepare(queryStr);

            for (const auto &c: columns) {
                query.bindValue(NEW_VALUE_TEMPLATE_BIND.arg(c), newRecord[c]);
                query.bindValue(OLD_VALUE_TEMPLATE_BIND.arg(c), oldRecord[c]);
            }

            query.exec();

            return [callback]() {
                callback();
            };
        } );
    }

}

void ADBListModelConfiguration::insert(QVariantMap item, std::function<void ()> callback)
{
    if (database()) {
        database()->execute(
                    [table = tablename(),
                    columns = columns(),
                    item = std::move(item),
                    callback] (QSqlDatabase db) {

            QSqlQuery query(db);



            QString queryStr = INSERT_QUERY_STR_TEMPLATE.arg(table).arg(columns.join(", ")).arg(":" + columns.join(", :"));
            query.prepare(queryStr);

            for (const auto &c: columns) {
                query.bindValue(":" + c, item[c]);
            }

            query.exec();

            return [callback]() {
                callback();
            };
        } );
    }
}
