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
                 int unitId = 0,
                 bool service = false);

    // Оператор сравнения
    bool operator==(const MainMnemonic &other) const;

    // Геттеры
    int getId() const;
    QString getName() const;
    QString getDescription() const;
    int getUnitId() const;
    bool isService() const; // Добавляем геттер для service

    // Сеттеры
    void setId(int id);
    void setName(const QString &name);
    void setDescription(const QString &description);
    void setUnitId(int unitId);
    void setService(bool service); // Добавляем сеттер для service

    void markAsDeleted();

private:
    int m_id;
    QString m_name;
    QString m_description;
    int m_unitId;
    bool m_service; // Добавляем поле service
};

#endif // MAINMNEMONIC_H
