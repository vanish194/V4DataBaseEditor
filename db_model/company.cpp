#include "company.h"

Company::Company(int id, const QString &name)
    : m_id(id)
    , m_name(name)
{}

bool Company::operator==(const Company &other) const
{
    return m_id == other.m_id && m_name == other.m_name;
}

int Company::getId() const
{
    return m_id;
}

QString Company::getName() const
{
    return m_name;
}

void Company::setId(int id)
{
    m_id = id;
}

void Company::setName(const QString &name)
{
    m_name = name;
}

void Company::markAsDeleted()
{
    if (m_id > 0) {
        m_id = -m_id;
    }
}
