#include "sensoraddwindow.h"
#include <QMessageBox>

SensorAddWindow::SensorAddWindow(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    connect(ui.pushButton_add, &QPushButton::clicked, this, &SensorAddWindow::onAddButtonClicked);
    connect(ui.pushButton_cancel,
            &QPushButton::clicked,
            this,
            &SensorAddWindow::onCancelButtonClicked);

    loadMethods();
}

SensorAddWindow::~SensorAddWindow() {}

void SensorAddWindow::setNewSensorId(int id)
{
    newSensor.setId(id);
}

void SensorAddWindow::loadMethods()
{
    ui.comboBox_method->clear();
    const auto &methods = Storage::getInstance()->getMethods();
    for (const Method &method : methods) {
        if (method.getId() > 0) {
            ui.comboBox_method->addItem(method.getName(), method.getId());
        }
    }
    if (ui.comboBox_method->count() == 0) {
        ui.comboBox_method->addItem("No Methods Available", -1);
    }
}

void SensorAddWindow::onAddButtonClicked()
{
    if (ui.lineEdit_name->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a sensor name.");
        return;
    }
    if (ui.comboBox_method->currentData().toInt() == -1) {
        QMessageBox::warning(this, "Input Error", "Please select a method.");
        return;
    }

    newSensor.setName(ui.lineEdit_name->text());
    newSensor.setDescription(ui.lineEdit_description->text());
    newSensor.setMethodId(ui.comboBox_method->currentData().toInt());

    accept();
}

void SensorAddWindow::onCancelButtonClicked()
{
    reject();
}

Sensor SensorAddWindow::getNewSensor() const
{
    return newSensor;
}
