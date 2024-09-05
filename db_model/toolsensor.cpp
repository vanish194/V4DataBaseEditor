#include "toolsensor.h"

// Constructor
ToolSensor::ToolSensor(int id, int toolId, int sensorId, int offsetMm)
    : m_id(id)
    , m_toolId(toolId)
    , m_sensorId(sensorId)
    , m_offsetMm(offsetMm)
{}

// Comparison operator
bool ToolSensor::operator==(const ToolSensor &other) const
{
    return m_id == other.m_id && m_toolId == other.m_toolId && m_sensorId == other.m_sensorId
           && m_offsetMm == other.m_offsetMm;
}

// Getters
int ToolSensor::getId() const
{
    return m_id;
}

int ToolSensor::getToolId() const
{
    return m_toolId;
}

int ToolSensor::getSensorId() const
{
    return m_sensorId;
}

int ToolSensor::getOffsetMm() const
{
    return m_offsetMm;
}

// Setters
void ToolSensor::setId(int id)
{
    m_id = id;
}

void ToolSensor::setToolId(int toolId)
{
    m_toolId = toolId;
}

void ToolSensor::setSensorId(int sensorId)
{
    m_sensorId = sensorId;
}

void ToolSensor::setOffsetMm(int offsetMm)
{
    m_offsetMm = offsetMm;
}

void ToolSensor::markAsDeleted()
{
    if (m_id > 0) {
        m_id = -m_id;
    }
}
