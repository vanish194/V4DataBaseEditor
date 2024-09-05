#ifndef PRODUCER_H
#define PRODUCER_H

#include <QString>

class Producer
{
public:
    // Конструктор
    Producer(int id = 0, const QString &name = "");

    // Оператор сравнения
    bool operator==(const Producer &other) const;

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

#endif // PRODUCER_H
