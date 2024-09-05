#ifndef ADDITIONALMNEMONIC_H
#define ADDITIONALMNEMONIC_H

#include <QString>

class AdditionalMnemonic
{
public:
    // Конструктор
    AdditionalMnemonic(int id = 0,
                       const QString &name = QString(),
                       int companyId = 0,
                       int mainMnemonicId = 0,
                       int unitId = 0);

    // Оператор сравнения
    bool operator==(const AdditionalMnemonic &other) const;

    // Геттеры
    int getId() const;
    QString getName() const;
    int getCompanyId() const;
    int getMainMnemonicId() const;
    int getUnitId() const;

    // Сеттеры
    void setId(int id);
    void setName(const QString &name);
    void setCompanyId(int companyId);
    void setMainMnemonicId(int mainMnemonicId);
    void setUnitId(int unitId);

    void markAsDeleted();

private:
    int m_id;
    QString m_name;
    int m_companyId;
    int m_mainMnemonicId;
    int m_unitId;
};

#endif // ADDITIONALMNEMONIC_H
