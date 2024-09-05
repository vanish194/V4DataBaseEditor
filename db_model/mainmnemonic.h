#ifndef MAINMNEMONIC_H
#define MAINMNEMONIC_H

#include <QString>

class MainMnemonic
{
public:
    // Конструктор
    MainMnemonic(int id = 0,
                 const QString &name = QString(),
                 const QString &description = QString(),
                 int sensorId = 0,
                 int unitId = 0);

    // Оператор сравнения
    bool operator==(const MainMnemonic &other) const;

    // Геттеры
    int getId() const;
    QString getName() const;
    QString getDescription() const;
    int getSensorId() const;
    int getUnitId() const;

    // Сеттеры
    void setId(int id);
    void setName(const QString &name);
    void setDescription(const QString &description);
    void setSensorId(int sensorId);
    void setUnitId(int unitId);

    void markAsDeleted();

private:
    int m_id;
    QString m_name;
    QString m_description;
    int m_sensorId;
    int m_unitId;
};

#endif // MAINMNEMONIC_H
