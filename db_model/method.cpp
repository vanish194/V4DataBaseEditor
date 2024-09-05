#include "method.h"

// Конструктор
Method::Method(int id, const QString &name)
    : m_id(id)
    , m_name(name)
{}

// Оператор сравнения
bool Method::operator==(const Method &other) const
{
    return m_id == other.m_id && m_name == other.m_name;
}

// Геттеры
int Method::getId() const
{
    return m_id;
}

QString Method::getName() const
{
    return m_name;
}

// Сеттеры
void Method::setId(int id)
{
    m_id = id;
}

void Method::setName(const QString &name)
{
    m_name = name;
}

void Method::markAsDeleted()
{
    if (m_id > 0) {
        m_id = -m_id;
    }
}
