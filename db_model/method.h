#ifndef METHOD_H
#define METHOD_H

#include <QString>

class Method
{
public:
    // Constructor
    Method(int id = 0, const QString &name = "");

    // Comparison operator
    bool operator==(const Method &other) const;

    // Getters
    int getId() const;
    QString getName() const;

    // Setters
    void setId(int id);
    void setName(const QString &name);

    void markAsDeleted();

private:
    int m_id;
    QString m_name;
};

#endif // METHOD_H
