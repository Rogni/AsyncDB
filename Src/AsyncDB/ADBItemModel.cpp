#include <AsyncDB/ADBItemModel.h>

ADBItemModel::ADBItemModel(QObject *parent): QObject(parent) {}

QVariant ADBItemModel::at(QString value)
{
    if (model() == nullptr) return QVariant();
    auto rolesHash = model()->roleNames();
    auto roles = rolesHash.keys(value.toUtf8());
    if (roles.isEmpty()) return QVariant();
    int role = roles.first();
    if (m_cache.contains(role)) {
        return m_cache[role];
    }
    return model()->data(model()->index(m_rowIndex), role);
}

void ADBItemModel::write(QString name, QVariant value)
{
    int role = model()->roleNames().key(name.toUtf8());
    m_cache[role] = value;
}

void ADBItemModel::commit()
{
    auto index= model()->index(m_rowIndex);
    QMap<int, QVariant> record = model()->itemData(index);

    auto rolenames = model()->roleNames();

    for (auto r = m_cache.begin(); r != m_cache.end(); ++r) {
        record[r.key()] = r.value();
    }
    model()->setItemData(index, record);
}

void ADBItemModel::reset()
{
    m_cache.clear();
}

ADBListModel *ADBItemModel::model() const
{
    return m_model;
}

int ADBItemModel::rowIndex() const
{
    return m_rowIndex;
}

void ADBItemModel::setModel(ADBListModel *model)
{
    if (m_model == model)
        return;

    m_model = model;
    emit changed();
    emit modelChanged(m_model);
}

void ADBItemModel::setRowIndex(int rowIndex)
{
    if (m_rowIndex == rowIndex)
        return;

    m_rowIndex = rowIndex;
    emit changed();
    emit rowIndexChanged(m_rowIndex);
}
