#ifndef TOOLMNEMONIC_H
#define TOOLMNEMONIC_H

#include <QString>

class ToolMnemonic
{
public:
    // Constructor
    ToolMnemonic(int id = 0, int toolId = 0, int mnemonicId = 0, int offsetMm = 0);

    // Comparison operator
    bool operator==(const ToolMnemonic &other) const;

    // Getters
    int getId() const;
    int getToolId() const;
    int getMnemonicId() const;
    int getOffsetMm() const;

    // Setters
    void setId(int id);
    void setToolId(int toolId);
    void setMnemonicId(int mnemonicId);
    void setOffsetMm(int offsetMm);

    void markAsDeleted();

private:
    int m_id;
    int m_toolId;
    int m_mnemonicId;
    int m_offsetMm;
};

#endif // TOOLMNEMONIC_H
