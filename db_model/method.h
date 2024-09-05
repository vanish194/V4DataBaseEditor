#ifndef METHOD_H
#define METHOD_H

#include <QString>

class Method
{
public:
    // Конструктор
    Method(int id = 0, const QString &name = "");

    // Оператор сравнения
    bool operator==(const Method &other) const;

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

#endif // METHOD_H
