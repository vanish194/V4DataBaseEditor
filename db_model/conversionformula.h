#ifndef CONVERSIONFORMULA_H
#define CONVERSIONFORMULA_H

#include <QString>

class ConversionFormula
{
public:
    // Constructor
    ConversionFormula(int id = 0,
                      const QString &formula = QString(),
                      int initialUnitId = 0,
                      int derivedUnitId = 0);

    // Comparison operator
    bool operator==(const ConversionFormula &other) const;

    // Getters
    int getId() const;
    QString getFormula() const;
    int getInitialUnitId() const;
    int getDerivedUnitId() const;

    // Setters
    void setId(int id);
    void setFormula(const QString &formula);
    void setInitialUnitId(int initialUnitId);
    void setDerivedUnitId(int derivedUnitId);

    void markAsDeleted();

private:
    int m_id;
    QString m_formula;
    int m_initialUnitId;
    int m_derivedUnitId;
};

#endif // CONVERSIONFORMULA_H
