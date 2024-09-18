#include "sensor_editor.h"
#include "storage.h"
#include "storageeditor.h"

SensorEditor::SensorEditor(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    // Подключаем кнопку Save к слоту обработки
    connect(ui.pushButton_save, &QPushButton::clicked, this, &SensorEditor::onSaveButtonClicked);
}

void SensorEditor::setSensor(const Sensor &sensor)
{
    currentSensor = sensor;

    // Устанавливаем данные сенсора в элементы формы
    ui.lineEdit_name->setText(sensor.getName());
    ui.lineEdit_description->setText(sensor.getDescription());

    // Получаем метод и устанавливаем в выпадающий список
    for (const Method &method : Storage::getInstance()->getMethods()) {
        ui.comboBox_method->addItem(method.getName(), QVariant(method.getId()));
        if (method.getId() == sensor.getMethodId()) {
            ui.comboBox_method->setCurrentText(method.getName());
        }
    }
}

Sensor SensorEditor::getSensor() const
{
    // Обновляем информацию о сенсоре
    Sensor updatedSensor = currentSensor;
    updatedSensor.setName(ui.lineEdit_name->text());
    updatedSensor.setDescription(ui.lineEdit_description->text());

    int methodId = ui.comboBox_method->currentData().toInt();
    updatedSensor.setMethodId(methodId);

    // Обновляем запись в хранилище
    StorageEditor editor(Storage::getInstance());
    editor.insertOrReplace(Storage::getInstance()->getSensors(), updatedSensor);

    return updatedSensor;
}

void SensorEditor::onSaveButtonClicked()
{
    // Сохранение данных и закрытие окна
    accept();
}
