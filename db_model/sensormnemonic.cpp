#include "sensormnemonic.h"

// Constructor
SensorMnemonic::SensorMnemonic(int id, int sensorId, int mnemonicId)
    : m_id(id)
    , m_sensorId(sensorId)
    , m_mnemonicId(mnemonicId)
{}

// Comparison operator
bool SensorMnemonic::operator==(const SensorMnemonic &other) const
{
    return m_id == other.m_id && m_sensorId == other.m_sensorId
           && m_mnemonicId == other.m_mnemonicId;
}

// Getters
int SensorMnemonic::getId() const
{
    return m_id;
}

int SensorMnemonic::getSensorId() const
{
    return m_sensorId;
}

int SensorMnemonic::getMnemonicId() const
{
    return m_mnemonicId;
}

// Setters
void SensorMnemonic::setId(int id)
{
    m_id = id;
}

void SensorMnemonic::setSensorId(int sensorId)
{
    m_sensorId = sensorId;
}

void SensorMnemonic::setMnemonicId(int mnemonicId)
{
    m_mnemonicId = mnemonicId;
}

void SensorMnemonic::markAsDeleted()
{
    if (m_id > 0) {
        m_id = -m_id;
    }
}
