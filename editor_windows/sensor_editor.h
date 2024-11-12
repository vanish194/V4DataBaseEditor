#ifndef SENSOREDITOR_H
#define SENSOREDITOR_H

#include <QDialog>
#include "storage.h"
#include "ui_sensor_editor.h"

class SensorEditor : public QDialog
{
    Q_OBJECT

public:
    explicit SensorEditor(QWidget *parent = nullptr);
    void setSensor(const Sensor &sensor); // Sets the data for editing the sensor
    Sensor getSensor() const;             // Returns updated sensor after editing

private slots:
    void onSaveButtonClicked(); // Handling the Save button click

private:
    Ui::SensorEditor ui;
    Sensor currentSensor; // Current editable sensor
};

#endif // SENSOREDITOR_H
