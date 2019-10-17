#ifndef ADBCUSTOMLISTMODELCONFIGURATION_H
#define ADBCUSTOMLISTMODELCONFIGURATION_H

#include <AsyncDB/ADBAbstractListModelConfiguration.h>

class ADBCustomListModelConfiguration: public ADBAbstractListModelConfiguration
{
    Q_OBJECT

    Q_PROPERTY(QString selectQuery READ selectQuery WRITE setSelectQuery NOTIFY selectQueryChanged)
    Q_PROPERTY(QString insertQuery READ insertQuery WRITE setInsertQuery NOTIFY insertQueryChanged)
    Q_PROPERTY(QString updateQuery READ updateQuery WRITE setUpdateQuery NOTIFY updateQueryChanged)

    QString m_selectQuery;
    QString m_insertQuery;
    QString m_updateQuery;

public:
    explicit ADBCustomListModelConfiguration(QObject *parent=nullptr);

    QString selectQuery() const;
    QString insertQuery() const;
    QString updateQuery() const;

public slots:
    void setSelectQuery(QString selectQuery);
    void setInsertQuery(QString insertQuery);
    void setUpdateQuery(QString updateQuery);

signals:
    void selectQueryChanged(QString selectQuery);
    void insertQueryChanged(QString insertQuery);
    void updateQueryChanged(QString updateQuery);

    // ADBAbstractListModelConfiguration interface
public:
    void select(std::function<void(QVector<QVariantMap>, QStringList)> callback) override;
    void update(QVariantMap from, QVariantMap to, std::function<void ()>) override;
    void insert(QVector<QVariantMap> item, std::function<void ()>) override;
};

#endif // ADBCUSTOMLISTMODELCONFIGURATION_H
