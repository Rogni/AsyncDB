#include <Src/AsyncDB/ADBQueryHelpers.h>
#include <QSqlError>
#include <QSqlRecord>

#include <QDebug>

namespace
{

QStringList columnsFromQuery(QSqlQuery query)
{
    QStringList columns;
    auto fieldsCount = query.record().count();
    columns.reserve(fieldsCount);
    for (int i = 0; i < fieldsCount; ++i) {
        columns += query.record().fieldName(i);
    }
    return columns;
}

QString makeInsertQueryString(const QStringList &columns, const int recordsCount, const QString &table)
{
    QString result;
    if (! (columns.isEmpty() || recordsCount < 1)) {
        QString columnsTemplate = " ( " + QString("?, ").repeated(columns.size() - 1) + "? ) ";
        QString comaColumnsTemplae = columnsTemplate + ",";
        QString insert("INSERT INTO ");
        QString joinedColumns = columns.join(", ");
        QString columnsValues = comaColumnsTemplae.repeated(recordsCount - 1) + columnsTemplate;
        result.reserve(insert.size() + table.size() + joinedColumns.size() + 20 /*buff*/);
        result = insert + table + " ( " + joinedColumns + " ) VALUES "
                + columnsValues;
    }
    return result;
}


QString makeUpdateQueryString(const QString &table, const QStringList &columns)
{
    QString r;
    if (!columns.isEmpty()) {
        r = "UPDATE " + table + " SET " + columns.join("= ? , ") + "= ?";
    }
    return r;
}



QString selectFrom(QStringList columns, QStringList tables)
{
    return "SELECT " + columns.join(", ") + " FROM " + tables.join(", ");
}

QString where(QString && query, QString condition)
{
    return query + " WHERE " + condition;
}

QString whereFieldsIs(QString && query, QStringList columns)
{

    return where(std::move(query),  columns.join(" = ? AND ") + " = ?");
}

QString groubBy(QString && query, QString column)
{
    return query + " GROUP BY " + column;
}

QString orderBy(QString && query, QString column, bool desc)
{
    return query + " ORDER BY " + column + (desc ?  " DESC": "");
}

}

QueryFunctorMaker parseSelectQuery(std::function<void (QStringList &&, QVector<QVariantMap> &&)> resultHandler)
{
    return [resultHandler] (QueryFunctor functor) {
        return [resultHandler, functor](QSqlQuery query) {
            QVector<QVariantMap> result;
            QStringList columns = columnsFromQuery(query);
            auto s = query.size();
            result.reserve( s > -1 ? s : 0 );
            while (query.next()) {
                QVariantMap record;
                for (const auto &name: columns) {
                    record[name] = query.value(name);
                }
                result.push_back(std::move(record));
            }
            resultHandler(std::move(columns), std::move(result));
            functor(query);
        };
    };
}

QueryFunctorMaker makeSelectQuery(
                             QStringList columns,
                             QStringList tables,
                             QString w,
                             QString g,
                             QString o,
                             bool desc)
{
    QString result = selectFrom(columns, tables);
    if (!w.isEmpty()) result = where(std::move(result), w);
    if (!g.isEmpty()) result = groubBy(std::move(result), g);
    if (!o.isEmpty()) result = orderBy(std::move(result), o, desc);
    return [qstr = std::move(result)] (QueryFunctor functor) {
        return [functor, qstr](QSqlQuery q) {
            q.prepare(qstr);
            functor(q);
        };
    };
}



QueryFunctorMaker makeInsertQuery(
        QString table,
        QStringList columns,
        QVector<QVariantMap> values)
{
    QString queryStr(makeInsertQueryString(columns, values.size(), table));
    return [qstr = std::move(queryStr),
            columns = std::move(columns),
            values = std::move(values)] (QueryFunctor functor) {
        return [functor, qstr, columns, values](QSqlQuery q) {
            q.prepare(qstr);
            for (auto &&r : values) {
                for (auto i = columns.begin(); i != columns.end(); ++i) {
                    q.addBindValue(r.value(*i));
                }
            }
            functor(q);
        };
    };
}

QueryFunctorMaker makeUpdateQuery(QString table, QVariantMap from, QVariantMap to)
{
    QString queryStr(whereFieldsIs(makeUpdateQueryString(table, to.keys()), from.keys()));
    return [qstr = std::move(queryStr),
            from = std::move(from),
            to = std::move(to)] (QueryFunctor functor) {
        return [functor, qstr, from, to](QSqlQuery q) {

            q.prepare(qstr);
            for (auto r = to.begin(); r != to.end(); ++r) {
                q.addBindValue(r.value());
            }
            for (auto r = from.begin(); r != from.end(); ++r) {
                q.addBindValue(r.value());
            }
            functor(q);
        };
    };
}

QueryFunctorMaker executeQuery()
{
    return [](QueryFunctor f) {
        return [f] (QSqlQuery q) {
            q.exec();
            f(q);
        };
    };
}

QueryFunctor queryExit()
{
    return [](QSqlQuery q){
        return q;
    };
}

QueryFunctorMaker handleSqlError(QueryFunctor errorHandler)
{
    return [errorHandler] (QueryFunctor functor) {
        return [errorHandler, functor] (QSqlQuery query) {
            if (query.lastError().type() != QSqlError::NoError) {
                errorHandler(query);
            } else {
                functor(query);
            }
        };
    };
}
