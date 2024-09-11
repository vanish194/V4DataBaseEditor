#ifndef UNITTYPE_H
#define UNITTYPE_H

#include <QString>

class UnitType
{
public:
    // Constructor
    UnitType(int id = 0, const QString &name = "");

    // Comparison operator
    bool operator==(const UnitType &other) const;

    // Getters
    int getId() const;
    QString getName() const;

    // Setters
    void setId(int id);
    void setName(const QString &name);

    void markAsDeleted();

private:
    int m_id;
    QString m_name;
};

#endif // UNITTYPE_H
