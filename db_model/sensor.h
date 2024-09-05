#ifndef SENSOR_H
#define SENSOR_H

#include <QString>

class Sensor
{
public:
    // Конструктор
    Sensor(int id = 0,
           int methodId = 0,
           const QString &name = QString(),
           const QString &description = QString());

    // Оператор сравнения
    bool operator==(const Sensor &other) const;

    // Геттеры
    int getId() const;
    int getMethodId() const;
    QString getName() const;
    QString getDescription() const;

    // Сеттеры
    void setId(int id);
    void setMethodId(int methodId);
    void setName(const QString &name);
    void setDescription(const QString &description);

    void markAsDeleted();

private:
    int m_id;
    int m_methodId;
    QString m_name;
    QString m_description;
};

#endif // SENSOR_H
