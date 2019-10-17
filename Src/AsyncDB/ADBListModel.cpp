#include <AsyncDB/ADBListModel.h>
#include <QSqlQuery>
#include <QtDebug>


QHash<int, QByteArray> makeRoleNamesHash(const QStringList &roles)
{
    QHash<int, QByteArray> result;
    for (int i = 0; i < roles.size(); ++i) {
        result[i] = roles[i].toUtf8();
    }
    return result;
}

QVariantMap makeRoleNamesMap(const QHash<int, QByteArray> &roles,
                             const QMap<int, QVariant> &record)
{
    QVariantMap result;

    for (auto i = record.begin(); i != record.end(); ++i)
    {
        result[QString::fromUtf8(roles.value(i.key()))] = i.value();
    }

    return result;
}

struct ADBListModel::Private
{
    Private(ADBListModel *m): model(m) {}
    ADBListModel *model = nullptr;
    QVector<QVariantMap> records;
    QHash<int, QVariantMap> updatedRecords;
    QVector<QVariantMap> newRecords;
    QHash<int, QByteArray> roles;

    int rowsCount() {
        return records.size() + newRecords.size();
    }

    QVariant data(int index, QString field)
    {
        QVariant result;
        if (index >= 0 && index < records.size()) {
            if (updatedRecords.contains(index)) {
                result = updatedRecords[index][field];
            } else {
                result = records[index][field];
            }
        } else if (int i = index - records.size(); i < newRecords.size()) {
            result = newRecords[i][field];
        }
        return result;
    }

    bool setData(int index, QString field, QVariant value)
    {
        if (data(index, field) == value) return false;
        if (index >= 0 && index < records.size()) {
            if (!updatedRecords.contains(index)) {
                updatedRecords[index] = records[index];
            }
            updatedRecords[index][field] = value;

        } else if (int i = index - records.size(); i < newRecords.size()) {
            newRecords[i][field] = value;
        }
        return true;
    }

};

ADBListModel::ADBListModel(QObject *parent):
    QAbstractListModel(parent),
    m_p(std::make_shared<Private>(this))
{

}

void ADBListModel::setConfiguration(ADBAbstractListModelConfiguration *configuration)
{
    if (m_configuration == configuration)
        return;

    if (m_configuration) {
        m_configuration->disconnect(this);
    }

    m_configuration = configuration;

    if (m_configuration) {
        connect(m_configuration, &ADBAbstractListModelConfiguration::reselectItems, this, [this](){
            this->select();
        });
        this->select();
    }

    emit configurationChanged(m_configuration);
}

void ADBListModel::select()
{
    if (m_configuration && m_configuration->database()) {
        auto resolve = [w = std::weak_ptr(m_p)] (QVector<QVariantMap> result, QStringList roles) {
            auto ptr = w.lock();
            if (ptr) {
                emit ptr->model->beginResetModel();
                ptr->records = result;
                ptr->roles = makeRoleNamesHash(roles);
                ptr->model->rollback();
                emit ptr->model->endResetModel();
            }
        };
        m_configuration->select(resolve);
    }
}

int ADBListModel::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    return m_p->rowsCount();
}

QVariant ADBListModel::data(const QModelIndex &index, int role) const
{
    QString key = QString::fromUtf8(m_p->roles[role]);
    return m_p->data(index.row(), key);
}

bool ADBListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    QString key = QString::fromUtf8(m_p->roles[role]);
    if (m_p->setData(index.row(), key, value)) {
        dataChanged(index, index);
        return true;
    }
    return false;
}

QHash<int, QByteArray> ADBListModel::roleNames() const
{
    return m_p->roles;
}

void ADBListModel::append(QVariantMap item)
{
    if (m_configuration) {
        beginInsertRows(index(m_p->rowsCount()-1), m_p->rowsCount(), m_p->rowsCount());
        m_p->newRecords.append(std::move(item));
        endInsertRows();
    }
}

void ADBListModel::commit()
{
    if (m_configuration) {
        auto insert = m_configuration->insertFunctor(std::move(m_p->newRecords));
        std::vector<std::function<void(QSqlDatabase)>> updateFunctors;
        updateFunctors.reserve(static_cast<size_t>(m_p->records.size()));
        for (auto i = m_p->updatedRecords.begin();
             i != m_p->updatedRecords.end();
             ++i) {
            QVariantMap from = m_p->records[i.key()];
            QVariantMap to = i.value();
            auto functor = m_configuration->updateFunctor(std::move(from), std::move(to));
            updateFunctors.push_back(std::move(functor));
        }
        auto exec = [
                w = std::weak_ptr(m_p),
                insert = std::move(insert),
                updateFunctors = std::move(updateFunctors)] (QSqlDatabase db) {
            insert(db);
            for (auto &f: updateFunctors) {
                f(db);
            }
            return [w] () {
                auto s = w.lock();
                if (s) {

                    s->model->select();
                }
            };
        };
        m_configuration->execute(exec);
    }
}

void ADBListModel::rollback()
{
    m_p->newRecords.clear();
    m_p->updatedRecords.clear();
}

ADBAbstractListModelConfiguration *ADBListModel::configuration() const
{
    return m_configuration;
}
