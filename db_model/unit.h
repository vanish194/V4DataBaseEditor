#ifndef UNIT_H
#define UNIT_H

#include <QString>

class Unit
{
public:
    // Constructor
    Unit(int id = 0, const QString &name = "", int typeId = 0, const QString &shortName = "");

    // Comparison operator
    bool operator==(const Unit &other) const;

    // Getters
    int getId() const;
    QString getName() const;
    int getTypeId() const;
    QString getShortName() const;

    // Setters
    void setId(int id);
    void setName(const QString &name);
    void setTypeId(int typeId);
    void setShortName(const QString &shortName);

    void markAsDeleted();

private:
    int m_id;
    QString m_name;
    int m_typeId;
    QString m_shortName;
};

#endif // UNIT_H
