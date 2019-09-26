#ifndef ADBABSTRACTLISTMODELCONFIGURATION_H
#define ADBABSTRACTLISTMODELCONFIGURATION_H

#include <QObject>
#include <AsyncDB/ADBDatabase.h>
#include <QVariant>

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
    virtual void update(QVariantMap from, QVariantMap to,std::function<void()>);
    virtual void insert(QVariantMap item, std::function<void()>);

public slots:
    void setDatabase(ADBDatabase* database);

signals:
    void databaseChanged(ADBDatabase* database);
    void reselectItems();

};

#endif // ADBABSTRACTLISTMODELCONFIGURATION_H
