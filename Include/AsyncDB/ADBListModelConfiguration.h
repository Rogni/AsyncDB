#ifndef ADBLISTMODELCONFIGURATION_H
#define ADBLISTMODELCONFIGURATION_H

#include <AsyncDB/ADBDatabase.h>
#include <AsyncDB/ADBAbstractListModelConfiguration.h>

class ADBListModelConfiguration: public ADBAbstractListModelConfiguration
{
    Q_OBJECT

    Q_PROPERTY(QString tablename
               READ tablename
               WRITE setTablename
               NOTIFY tablenameChanged)
    Q_PROPERTY(QStringList columns
               READ columns
               WRITE setColumns
               NOTIFY columnsChanged)

    QStringList m_columns;
    QString m_tablename;

public:
    ADBListModelConfiguration(QObject *parent=nullptr);
    QStringList columns() const;
    QString tablename() const;

public slots:
    void setColumns(QStringList columns);
    void setTablename(QString tablename);

signals:
    void columnsChanged(QStringList columns);
    void tablenameChanged(QString tablename);

    // ADBAbstractListModelConfiguration interface
public:
    void select(std::function<void (QVector<QVariantMap>, QStringList)> callback) override;
    void update(QVariantMap from, QVariantMap to, std::function<void ()>) override;
    void insert(QVariantMap item, std::function<void ()>) override;
};

#endif // ADBLISTMODELCONFIGURATION_H
