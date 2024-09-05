#include "unit.h"

// Constructor
Unit::Unit(int id, const QString &name, int typeId, const QString &shortName)
    : m_id(id)
    , m_name(name)
    , m_typeId(typeId)
    , m_shortName(shortName)
{}

// Comparison operator
bool Unit::operator==(const Unit &other) const
{
    return m_id == other.m_id && m_name == other.m_name && m_typeId == other.m_typeId
           && m_shortName == other.m_shortName;
}

// Getters
int Unit::getId() const
{
    return m_id;
}

QString Unit::getName() const
{
    return m_name;
}

int Unit::getTypeId() const
{
    return m_typeId;
}

QString Unit::getShortName() const
{
    return m_shortName;
}

// Setters
void Unit::setId(int id)
{
    m_id = id;
}

void Unit::setName(const QString &name)
{
    m_name = name;
}

void Unit::setTypeId(int typeId)
{
    m_typeId = typeId;
}

void Unit::setShortName(const QString &shortName)
{
    m_shortName = shortName;
}

void Unit::markAsDeleted()
{
    if (m_id > 0) {
        m_id = -m_id;
    }
}
