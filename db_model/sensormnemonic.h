#ifndef SENSORMNEMONIC_H
#define SENSORMNEMONIC_H

#include <QString>

class SensorMnemonic
{
public:
    // Constructor
    SensorMnemonic(int id = 0, int sensorId = 0, int mnemonicId = 0);

    // Comparison operator
    bool operator==(const SensorMnemonic &other) const;

    // Getters
    int getId() const;
    int getSensorId() const;
    int getMnemonicId() const;

    // Setters
    void setId(int id);
    void setSensorId(int sensorId);
    void setMnemonicId(int mnemonicId);

    void markAsDeleted();

private:
    int m_id;
    int m_sensorId;
    int m_mnemonicId;
};

#endif // SENSORMNEMONIC_H
