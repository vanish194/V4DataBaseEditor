#include "unittype.h"

// Constructor
UnitType::UnitType(int id, const QString &name)
    : m_id(id)
    , m_name(name)
{}

// Comparison operator
bool UnitType::operator==(const UnitType &other) const
{
    return m_id == other.m_id && m_name == other.m_name;
}

// Getters
int UnitType::getId() const
{
    return m_id;
}

QString UnitType::getName() const
{
    return m_name;
}

// Setters
void UnitType::setId(int id)
{
    m_id = id;
}

void UnitType::setName(const QString &name)
{
    m_name = name;
}

void UnitType::markAsDeleted()
{
    if (m_id > 0) {
        m_id = -m_id;
    }
}
