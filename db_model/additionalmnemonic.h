#ifndef ADDITIONALMNEMONIC_H
#define ADDITIONALMNEMONIC_H

#include <QString>

class AdditionalMnemonic
{
public:
    // Constructor
    AdditionalMnemonic(int id = 0,
                       const QString &name = QString(),
                       int companyId = 0,
                       int mainMnemonicId = 0,
                       int unitId = 0);

    // Comparison operator
    bool operator==(const AdditionalMnemonic &other) const;

    // Getters
    int getId() const;
    QString getName() const;
    int getCompanyId() const;
    int getMainMnemonicId() const;
    int getUnitId() const;

    // Setters
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
