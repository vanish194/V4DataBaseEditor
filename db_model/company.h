#ifndef COMPANY_H
#define COMPANY_H

#include <QString>

class Company
{
public:
    // Constructor
    Company(int id = 0, const QString &name = "");

    // Comparison operator
    bool operator==(const Company &other) const;

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

#endif // COMPANY_H
