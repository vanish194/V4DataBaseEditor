#include "producer.h"

// Конструктор
Producer::Producer(int id, const QString &name)
    : m_id(id)
    , m_name(name)
{}

// Оператор сравнения
bool Producer::operator==(const Producer &other) const
{
    return m_id == other.m_id && m_name == other.m_name;
}

// Геттеры
int Producer::getId() const
{
    return m_id;
}

QString Producer::getName() const
{
    return m_name;
}

// Сеттеры
void Producer::setId(int id)
{
    m_id = id;
}

void Producer::setName(const QString &name)
{
    m_name = name;
}

void Producer::markAsDeleted()
{
    if (m_id > 0) {
        m_id = -m_id;
    }
}
