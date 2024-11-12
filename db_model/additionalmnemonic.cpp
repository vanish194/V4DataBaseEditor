#include "additionalmnemonic.h"

AdditionalMnemonic::AdditionalMnemonic(
    int id, const QString &name, int companyId, int mainMnemonicId, int unitId)
    : m_id(id)
    , m_name(name)
    , m_companyId(companyId)
    , m_mainMnemonicId(mainMnemonicId)
    , m_unitId(unitId)
{}

bool AdditionalMnemonic::operator==(const AdditionalMnemonic &other) const
{
    return m_id == other.m_id && m_name == other.m_name && m_companyId == other.m_companyId
           && m_mainMnemonicId == other.m_mainMnemonicId && m_unitId == other.m_unitId;
}

int AdditionalMnemonic::getId() const
{
    return m_id;
}

QString AdditionalMnemonic::getName() const
{
    return m_name;
}

int AdditionalMnemonic::getCompanyId() const
{
    return m_companyId;
}

int AdditionalMnemonic::getMainMnemonicId() const
{
    return m_mainMnemonicId;
}

int AdditionalMnemonic::getUnitId() const
{
    return m_unitId;
}

void AdditionalMnemonic::setId(int id)
{
    m_id = id;
}

void AdditionalMnemonic::setName(const QString &name)
{
    m_name = name;
}

void AdditionalMnemonic::setCompanyId(int companyId)
{
    m_companyId = companyId;
}

void AdditionalMnemonic::setMainMnemonicId(int mainMnemonicId)
{
    m_mainMnemonicId = mainMnemonicId;
}

void AdditionalMnemonic::setUnitId(int unitId)
{
    m_unitId = unitId;
}

void AdditionalMnemonic::markAsDeleted()
{
    if (m_id > 0) {
        m_id = -m_id;
    }
}
