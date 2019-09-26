#include <AsyncDB/ADBListModel.h>
#include <QSqlQuery>

const QString SELECT_QUERY_STR_TEMPLATE = "SELECT %1 FROM :tablename ;";
const QString UPDATE_QUERY_STR_TEMPLATE = "UPDATE :tablename "
                                          "SET %1 "
                                          "WHERE %2 ";
const QString INSERT_QUERY_STR_TEMPLATE = "INSERT INTO :tablename ( %1 )"
                                          "VALUES( %2 );";
const QString OLD_VALUE_TEMPLATE = "&1 = :old_%1";
const QString NEW_VALUE_TEMPLATE = "%1 = :new_%1";

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
    QHash<int, QByteArray> roles;
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
                emit ptr->model->endResetModel();
            }
        };
        m_configuration->select(resolve);
    }
}

int ADBListModel::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    return static_cast<int>(m_p->records.size());
}

QVariant ADBListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= m_p->records.size() || !m_p->roles.contains(role)) return QVariant();
    auto record = m_p->records[ index.row() ];
    QString key = QString::fromUtf8(m_p->roles[role]);
    if (!record.contains(key)) return QVariant();
    return record[key];
}

QHash<int, QByteArray> ADBListModel::roleNames() const
{
    return m_p->roles;
}

bool ADBListModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &newValues)
{
    if (m_configuration) {
        auto old = m_p->records.at(index.row());
        auto roles = roleNames();
        auto newRecord = makeRoleNamesMap(roles, newValues);
        auto exec = [w = std::weak_ptr(m_p), index, newRecord]() {
            auto ptr = w.lock();
            if (ptr) {
                ptr->records[index.row()] = newRecord;
                emit ptr->model->dataChanged(index, index);
            }
        };

        m_configuration->update(old, newRecord, exec);
    }
    return true;
}

ADBAbstractListModelConfiguration *ADBListModel::configuration() const
{
    return m_configuration;
}
