#include "mainmnemonic.h"

// Конструктор
MainMnemonic::MainMnemonic(
    int id, const QString &name, const QString &description, int unitId, bool service)
    : m_id(id)
    , m_name(name)
    , m_description(description)
    , m_unitId(unitId)
    , m_service(service)
{}

bool MainMnemonic::operator==(const MainMnemonic &other) const
{
    return m_id == other.m_id && m_name == other.m_name && m_description == other.m_description
           && m_unitId == other.m_unitId && m_service == other.m_service;
}

// Геттеры
int MainMnemonic::getId() const
{
    return m_id;
}

QString MainMnemonic::getName() const
{
    return m_name;
}

QString MainMnemonic::getDescription() const
{
    return m_description;
}

int MainMnemonic::getUnitId() const
{
    return m_unitId;
}

bool MainMnemonic::isService() const
{
    return m_service;
}

// Сеттеры
void MainMnemonic::setId(int id)
{
    m_id = id;
}

void MainMnemonic::setName(const QString &name)
{
    m_name = name;
}

void MainMnemonic::setDescription(const QString &description)
{
    m_description = description;
}

void MainMnemonic::setUnitId(int unitId)
{
    m_unitId = unitId;
}

void MainMnemonic::setService(bool service)
{
    m_service = service;
}

void MainMnemonic::markAsDeleted()
{
    if (m_id > 0) {
        m_id = -m_id;
    }
}
