#include "conversionformula.h"

// Конструктор
ConversionFormula::ConversionFormula(int id,
                                     const QString &formula,
                                     int initialUnitId,
                                     int derivedUnitId)
    : m_id(id)
    , m_formula(formula)
    , m_initialUnitId(initialUnitId)
    , m_derivedUnitId(derivedUnitId)
{}

// Оператор сравнения
bool ConversionFormula::operator==(const ConversionFormula &other) const
{
    return m_id == other.m_id && m_formula == other.m_formula
           && m_initialUnitId == other.m_initialUnitId && m_derivedUnitId == other.m_derivedUnitId;
}

// Геттеры
int ConversionFormula::getId() const
{
    return m_id;
}

QString ConversionFormula::getFormula() const
{
    return m_formula;
}

int ConversionFormula::getInitialUnitId() const
{
    return m_initialUnitId;
}

int ConversionFormula::getDerivedUnitId() const
{
    return m_derivedUnitId;
}

// Сеттеры
void ConversionFormula::setId(int id)
{
    m_id = id;
}

void ConversionFormula::setFormula(const QString &formula)
{
    m_formula = formula;
}

void ConversionFormula::setInitialUnitId(int initialUnitId)
{
    m_initialUnitId = initialUnitId;
}

void ConversionFormula::setDerivedUnitId(int derivedUnitId)
{
    m_derivedUnitId = derivedUnitId;
}

void ConversionFormula::markAsDeleted()
{
    if (m_id > 0) {
        m_id = -m_id;
    }
}
