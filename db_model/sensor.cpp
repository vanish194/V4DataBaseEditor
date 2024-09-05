#include "sensor.h"

// Конструктор
Sensor::Sensor(int id, int methodId, const QString &name, const QString &description)
    : m_id(id)
    , m_methodId(methodId)
    , m_name(name)
    , m_description(description)
{}

// Оператор сравнения
bool Sensor::operator==(const Sensor &other) const
{
    return m_id == other.m_id && m_methodId == other.m_methodId && m_name == other.m_name
           && m_description == other.m_description;
}

// Геттеры
int Sensor::getId() const
{
    return m_id;
}

int Sensor::getMethodId() const
{
    return m_methodId;
}

QString Sensor::getName() const
{
    return m_name;
}

QString Sensor::getDescription() const
{
    return m_description;
}

// Сеттеры
void Sensor::setId(int id)
{
    m_id = id;
}

void Sensor::setMethodId(int methodId)
{
    m_methodId = methodId;
}

void Sensor::setName(const QString &name)
{
    m_name = name;
}

void Sensor::setDescription(const QString &description)
{
    m_description = description;
}

void Sensor::markAsDeleted()
{
    if (m_id > 0) {
        m_id = -m_id;
    }
}
