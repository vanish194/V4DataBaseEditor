#ifndef COMPANY_H
#define COMPANY_H

#include <QString>

class Company
{
public:
    // Конструктор
    Company(int id = 0, const QString &name = "");

    // Оператор сравнения
    bool operator==(const Company &other) const;

    // Геттеры
    int getId() const;
    QString getName() const;

    // Сеттеры
    void setId(int id);
    void setName(const QString &name);

    void markAsDeleted();

private:
    int m_id;
    QString m_name;
};

#endif // COMPANY_H
