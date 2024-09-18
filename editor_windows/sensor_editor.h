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
    void setSensor(const Sensor &sensor); // Устанавливает данные для редактирования сенсора
    Sensor getSensor() const; // Возвращает обновленный сенсор после редактирования

private slots:
    void onSaveButtonClicked(); // Обработка нажатия кнопки Save

private:
    Ui::SensorEditor ui;
    Sensor currentSensor; // Текущий редактируемый сенсор
};

#endif // SENSOREDITOR_H
