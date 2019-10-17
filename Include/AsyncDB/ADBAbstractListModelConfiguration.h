#ifndef ADBABSTRACTLISTMODELCONFIGURATION_H
#define ADBABSTRACTLISTMODELCONFIGURATION_H

#include <QObject>
#include <AsyncDB/ADBDatabase.h>
#include <QVariant>
#include <QVector>

class ADBAbstractListModelConfiguration: public QObject
{
    Q_OBJECT
    Q_PROPERTY(ADBDatabase* database
               READ database
               WRITE setDatabase
               NOTIFY databaseChanged)
    ADBDatabase* m_database = nullptr;

public:
    explicit ADBAbstractListModelConfiguration(QObject *parent = nullptr);

    ADBDatabase* database() const;



    virtual void select(std::function<void(QVector<QVariantMap>, QStringList)> callback);

    using DbFunctor = std::function<void(QSqlDatabase)>;
    virtual DbFunctor updateFunctor(QVariantMap from, QVariantMap to);
    virtual DbFunctor insertFunctor(QVector<QVariantMap> items);

    void execute(std::function<std::function<void()>(QSqlDatabase)> f);

public slots:
    void setDatabase(ADBDatabase* database);

signals:
    void databaseChanged(ADBDatabase* database);
    void reselectItems();

};

#endif // ADBABSTRACTLISTMODELCONFIGURATION_H
