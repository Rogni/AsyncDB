#ifndef ADBDATABASECONFIGURATION_H
#define ADBDATABASECONFIGURATION_H

#include <QObject>

class ADBDatabaseConfiguration: public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString databaseName READ databaseName WRITE setConnectionName NOTIFY connectionNameChanged)

    QString m_type;
    QString m_connectionName;

public:
    ADBDatabaseConfiguration(QObject *parent = nullptr);
    QString type() const;
    QString databaseName() const;

public slots:
    void setType(QString type);
    void setConnectionName(QString databaseName);

signals:
    void typeChanged(QString type);
    void connectionNameChanged(QString databaseName);

    void configurationChanged();
};

#endif // ADBDATABASECONFIGURATION_H
