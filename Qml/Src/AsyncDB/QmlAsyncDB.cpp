#include <Include/AsyncDB/QmlAsyncDB.h>

#include <QQmlEngine>

#include <AsyncDB/AsyncDB.h>

#define QML_ASYNC_DB_MODULE "AsyncDB"
#define QML_REGISTER_ASYNC_DB(TYPE) qmlRegisterType<TYPE>(QML_ASYNC_DB_MODULE, 1, 0, #TYPE)


void QmlAsyncDB::registerQmlType(QQmlEngine *)
{

    QML_REGISTER_ASYNC_DB(ADBDatabase);
    QML_REGISTER_ASYNC_DB(ADBListModel);
    QML_REGISTER_ASYNC_DB(ADBDatabaseConfiguration);
    QML_REGISTER_ASYNC_DB(ADBListModelConfiguration);
    QML_REGISTER_ASYNC_DB(ADBAbstractListModelConfiguration);
    QML_REGISTER_ASYNC_DB(ADBCustomListModelConfiguration);
}
