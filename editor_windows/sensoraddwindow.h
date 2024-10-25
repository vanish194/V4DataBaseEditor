#ifndef SENSORADDWINDOW_H
#define SENSORADDWINDOW_H

#include <QDialog>
#include "storage.h"
#include "ui_sensoraddwindow.h"

class SensorAddWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SensorAddWindow(QWidget *parent = nullptr);
    ~SensorAddWindow();

    void setNewSensorId(int id);

    Sensor getNewSensor() const;

private slots:
    void onAddButtonClicked();
    void onCancelButtonClicked();

private:
    Ui::SensorAddWindow ui;

    Sensor newSensor;

    void loadMethods();
};

#endif // SENSORADDWINDOW_H
