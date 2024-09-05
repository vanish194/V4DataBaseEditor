#include "tool.h"

// Constructor
Tool::Tool(int id, const QString &name, int toolDescriptionId)
    : m_id(id)
    , m_name(name)
    , m_toolDescriptionId(toolDescriptionId)
{}

// Comparison operator
bool Tool::operator==(const Tool &other) const
{
    return m_id == other.m_id && m_name == other.m_name
           && m_toolDescriptionId == other.m_toolDescriptionId;
}

// Getters
int Tool::getId() const
{
    return m_id;
}

QString Tool::getName() const
{
    return m_name;
}

int Tool::getToolDescriptionId() const
{
    return m_toolDescriptionId;
}

// Setters
void Tool::setId(int id)
{
    m_id = id;
}

void Tool::setName(const QString &name)
{
    m_name = name;
}

void Tool::setToolDescriptionId(int toolDescriptionId)
{
    m_toolDescriptionId = toolDescriptionId;
}

void Tool::markAsDeleted()
{
    if (m_id > 0) {
        m_id = -m_id;
    }
}
