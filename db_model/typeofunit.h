#ifndef TYPEOFUNIT_H
#define TYPEOFUNIT_H

#include <QString>

class TypeOfUnit
{
public:
    // Constructor
    TypeOfUnit(int id = 0, const QString &name = "");

    // Comparison operator
    bool operator==(const TypeOfUnit &other) const;

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

#endif // TYPEOFUNIT_H
