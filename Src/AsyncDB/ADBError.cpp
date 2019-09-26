#include "ADBError.h"

ADBError::ADBError(QString title, QString description):
    m_title(title),
    m_description(description) {}

ADBError::Ptr ADBError::make(QString title, QString description)
{
    return ADBError::Ptr(new ADBError(title, description));
}

QString ADBError::title() const
{
    return m_title;
}

void ADBError::setTitle(const QString &title)
{
    m_title = title;
}

QString ADBError::description() const
{
    return m_description;
}

void ADBError::setDescription(const QString &description)
{
    m_description = description;
}
