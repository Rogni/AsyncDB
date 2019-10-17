#ifndef ADBQUERYHELPERS_H
#define ADBQUERYHELPERS_H

#include <QString>
#include <QStringList>
#include <QObject>
#include <QMetaProperty>
#include <QVariantMap>
#include <functional>
#include <QVariant>
#include <QVector>
#include <QSqlQuery>

using QueryFunctor = std::function<void(QSqlQuery)>;
using QueryFunctorMaker = std::function<QueryFunctor(QueryFunctor)>;


QueryFunctorMaker makeSelectQuery(
        QStringList columns,
        QStringList tables,
        QString where = QString(),
        QString groubBy = QString(),
        QString orderBy = QString(),
        bool desc = false);

QueryFunctorMaker parseSelectQuery(
        std::function<void(QStringList && columns,
                           QVector<QVariantMap> && records)>);

QueryFunctorMaker makeInsertQuery(
        QString table,
        QStringList columns,
        QVector<QVariantMap> values
        );

QueryFunctorMaker makeUpdateQuery(
        QString table,
        QVariantMap from,
        QVariantMap to
        );

QueryFunctor queryExit();
QueryFunctorMaker executeQuery();

QueryFunctorMaker handleSqlError(QueryFunctor errorHandler);


#endif // ADBQUERYHELPERS_H
