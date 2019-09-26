#ifndef ADBITEMMODEL_H
#define ADBITEMMODEL_H

#include <QObject>
#include <AsyncDB/ADBListModel.h>

class ADBItemModel: public QObject
{
    Q_OBJECT

    Q_PROPERTY(ADBListModel* model
               READ model
               WRITE setModel
               NOTIFY modelChanged)
    Q_PROPERTY(int rowIndex
               READ rowIndex
               WRITE setRowIndex
               NOTIFY rowIndexChanged)

public:
    ADBItemModel(QObject* parent=nullptr);

    Q_INVOKABLE QVariant at(QString value);
    Q_INVOKABLE void write(QString name, QVariant value);

    Q_INVOKABLE void commit();
    Q_INVOKABLE void reset();

    ADBListModel* model() const;

    int rowIndex() const;

public slots:
    void setModel(ADBListModel* model);
    void setRowIndex(int rowIndex);

signals:
    void modelChanged(ADBListModel* model);
    void rowIndexChanged(int rowIndex);

    void changed();

private:
    QMap<int, QVariant> m_cache;
    ADBListModel* m_model = nullptr;
    int m_rowIndex = 0;
};

#endif // ADBITEMMODEL_H
