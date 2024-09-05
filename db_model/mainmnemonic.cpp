#include "mainmnemonic.h"

// Конструктор
MainMnemonic::MainMnemonic(
    int id, const QString &name, const QString &description, int sensorId, int unitId)
    : m_id(id)
    , m_name(name)
    , m_description(description)
    , m_sensorId(sensorId)
    , m_unitId(unitId)
{}

// Оператор сравнения
bool MainMnemonic::operator==(const MainMnemonic &other) const
{
    return m_id == other.m_id && m_name == other.m_name && m_description == other.m_description
           && m_sensorId == other.m_sensorId && m_unitId == other.m_unitId;
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

int MainMnemonic::getSensorId() const
{
    return m_sensorId;
}

int MainMnemonic::getUnitId() const
{
    return m_unitId;
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

void MainMnemonic::setSensorId(int sensorId)
{
    m_sensorId = sensorId;
}

void MainMnemonic::setUnitId(int unitId)
{
    m_unitId = unitId;
}
void MainMnemonic::markAsDeleted()
{
    if (m_id > 0) {
        m_id = -m_id;
    }
}
