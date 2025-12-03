#include "tooladdwindow.h"
#include <QMessageBox>

ToolAddWindow::ToolAddWindow(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    connect(ui.pushButton_add, &QPushButton::clicked, this, &ToolAddWindow::onAddButtonClicked);
    connect(ui.pushButton_cancel,
            &QPushButton::clicked,
            this,
            &ToolAddWindow::onCancelButtonClicked);

    loadProducers();
}

ToolAddWindow::~ToolAddWindow() {}

void ToolAddWindow::setNewToolId(int id)
{
    newTool.setId(id);
}

void ToolAddWindow::setNewToolDescriptionId(int id)
{
    newToolDescription.setId(id);
    newTool.setToolDescriptionId(id);
}

void ToolAddWindow::loadProducers()
{
    ui.comboBox_producer->clear();
    const auto &producers = Storage::getInstance()->getProducers();
    for (const Producer &producer : producers) {
        if (producer.getId() > 0) {
            ui.comboBox_producer->addItem(producer.getName(), producer.getId());
        }
    }
    if (ui.comboBox_producer->count() == 0) {
        ui.comboBox_producer->addItem(tr("No Producers Available"), -1);
    }
}

void ToolAddWindow::onAddButtonClicked()
{
    if (ui.lineEdit_name->text().isEmpty()) {
        QMessageBox::warning(this, tr("Input Error"), tr("Please enter a tool name."));
        return;
    }
    if (ui.comboBox_producer->currentData().toInt() == -1) {
        QMessageBox::warning(this, tr("Input Error"), tr("Please select a producer."));
        return;
    }

    newTool.setName(ui.lineEdit_name->text());

    newToolDescription.setDescription(ui.lineEdit_description->text());
    newToolDescription.setLengthMm(ui.spinBox_length->value());
    newToolDescription.setOuterDiameterMm(ui.spinBox_outerDiameter->value());
    newToolDescription.setInnerDiameterMm(ui.spinBox_innerDiameter->value());

    int producerId = ui.comboBox_producer->currentData().toInt();
    newToolDescription.setProducerId(producerId);

    accept();
}

void ToolAddWindow::onCancelButtonClicked()
{
    reject();
}

Tool ToolAddWindow::getNewTool() const
{
    return newTool;
}

ToolDescription ToolAddWindow::getNewToolDescription() const
{
    return newToolDescription;
}
