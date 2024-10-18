#include "toolmnemonic.h"

// Constructor
ToolMnemonic::ToolMnemonic(int id, int toolId, int mnemonicId, int offsetMm)
    : m_id(id)
    , m_toolId(toolId)
    , m_mnemonicId(mnemonicId)
    , m_offsetMm(offsetMm)
{}

// Comparison operator
bool ToolMnemonic::operator==(const ToolMnemonic &other) const
{
    return m_id == other.m_id && m_toolId == other.m_toolId && m_mnemonicId == other.m_mnemonicId
           && m_offsetMm == other.m_offsetMm;
}

// Getters
int ToolMnemonic::getId() const
{
    return m_id;
}

int ToolMnemonic::getToolId() const
{
    return m_toolId;
}

int ToolMnemonic::getMnemonicId() const
{
    return m_mnemonicId;
}

int ToolMnemonic::getOffsetMm() const
{
    return m_offsetMm;
}

// Setters
void ToolMnemonic::setId(int id)
{
    m_id = id;
}

void ToolMnemonic::setToolId(int toolId)
{
    m_toolId = toolId;
}

void ToolMnemonic::setMnemonicId(int mnemonicId)
{
    m_mnemonicId = mnemonicId;
}

void ToolMnemonic::setOffsetMm(int offsetMm)
{
    m_offsetMm = offsetMm;
}

void ToolMnemonic::markAsDeleted()
{
    if (m_id > 0) {
        m_id = -m_id;
    }
}
