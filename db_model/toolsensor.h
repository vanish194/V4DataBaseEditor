#ifndef TOOLSENSOR_H
#define TOOLSENSOR_H

#include <QString>

class ToolSensor
{
public:
    // Constructor
    ToolSensor(int id = 0, int toolId = 0, int sensorId = 0, int offsetMm = 0);

    // Comparison operator
    bool operator==(const ToolSensor &other) const;

    // Getters
    int getId() const;
    int getToolId() const;
    int getSensorId() const;
    int getOffsetMm() const;

    // Setters
    void setId(int id);
    void setToolId(int toolId);
    void setSensorId(int sensorId);
    void setOffsetMm(int offsetMm);

    void markAsDeleted();

private:
    int m_id;
    int m_toolId;
    int m_sensorId;
    int m_offsetMm;
};

#endif // TOOLSENSOR_H
