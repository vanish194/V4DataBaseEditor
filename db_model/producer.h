#ifndef PRODUCER_H
#define PRODUCER_H

#include <QString>

class Producer
{
public:
    // Constructor
    Producer(int id = 0, const QString &name = "");

    // Comparison operator
    bool operator==(const Producer &other) const;

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

#endif // PRODUCER_H
