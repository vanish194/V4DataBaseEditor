#ifndef TOOL_H
#define TOOL_H

#include <QString>

class Tool
{
public:
    // Constructor
    Tool(int id = 0, const QString &name = "", int toolDescriptionId = 0);

    // Comparison operator
    bool operator==(const Tool &other) const;

    // Getters
    int getId() const;
    QString getName() const;
    int getToolDescriptionId() const;

    // Setters
    void setId(int id);
    void setName(const QString &name);
    void setToolDescriptionId(int toolDescriptionId);

    void markAsDeleted();

private:
    int m_id;
    QString m_name;
    int m_toolDescriptionId;
};

#endif // TOOL_H
