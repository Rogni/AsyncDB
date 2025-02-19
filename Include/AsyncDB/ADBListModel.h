#ifndef ADBLISTMODEL_H
#define ADBLISTMODEL_H

#include <QAbstractListModel>
#include <AsyncDB/ADBDatabase.h>
#include <AsyncDB/ADBListModelConfiguration.h>

class ADBListModel: public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ADBAbstractListModelConfiguration* configuration
               READ configuration
               WRITE setConfiguration
               NOTIFY configurationChanged)

public:
    ADBListModel(QObject* parent = nullptr);

public slots:
    void setConfiguration(ADBAbstractListModelConfiguration* configuration);
    Q_INVOKABLE void select();

signals:
    void configurationChanged(ADBAbstractListModelConfiguration* configuration);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Q_INVOKABLE void append(QVariantMap item);
    Q_INVOKABLE void commit();
    Q_INVOKABLE void rollback();


    ADBAbstractListModelConfiguration* configuration() const;

private:
    struct Private;
    std::shared_ptr<Private> m_p;
    ADBAbstractListModelConfiguration* m_configuration = nullptr;


};

#endif // ADBLISTMODEL_H
