#include "typeofunit.h"

// Constructor
TypeOfUnit::TypeOfUnit(int id, const QString &name)
    : m_id(id)
    , m_name(name)
{}

// Comparison operator
bool TypeOfUnit::operator==(const TypeOfUnit &other) const
{
    return m_id == other.m_id && m_name == other.m_name;
}

// Getters
int TypeOfUnit::getId() const
{
    return m_id;
}

QString TypeOfUnit::getName() const
{
    return m_name;
}

// Setters
void TypeOfUnit::setId(int id)
{
    m_id = id;
}

void TypeOfUnit::setName(const QString &name)
{
    m_name = name;
}

void TypeOfUnit::markAsDeleted()
{
    if (m_id > 0) {
        m_id = -m_id;
    }
}
