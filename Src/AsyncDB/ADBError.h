#ifndef ADBERROR_H
#define ADBERROR_H

#include <QString>
#include <memory>

class ADBError
{
    QString m_title;
    QString m_description;

private:
    ADBError() = default;
    ADBError(QString title, QString description);

public:
    typedef std::shared_ptr<ADBError> Ptr;
    typedef std::weak_ptr<ADBError> WPtr;

    static Ptr make(QString title, QString description);


    QString title() const;
    void setTitle(const QString &title);
    QString description() const;
    void setDescription(const QString &description);
};

#endif // ADBERROR_H
