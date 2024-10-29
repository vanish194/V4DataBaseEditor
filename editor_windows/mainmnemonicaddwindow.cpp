#include "mainmnemonicaddwindow.h"
#include <QMessageBox>

MainMnemonicAddWindow::MainMnemonicAddWindow(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    connect(ui.pushButton_add, &QPushButton::clicked, this, &MainMnemonicAddWindow::onAddButtonClicked);
    connect(ui.pushButton_cancel, &QPushButton::clicked, this, &MainMnemonicAddWindow::onCancelButtonClicked);

    loadUnits();
}

MainMnemonicAddWindow::~MainMnemonicAddWindow()
{
}

void MainMnemonicAddWindow::setNewMainMnemonicId(int id)
{
    newMainMnemonic.setId(id);
}

void MainMnemonicAddWindow::loadUnits()
{
    ui.comboBox_unit->clear();
    const auto &units = Storage::getInstance()->getUnits();
    for (const Unit &unit : units) {
        if (unit.getId() > 0) {
            ui.comboBox_unit->addItem(unit.getName(), unit.getId());
        }
    }
    if (ui.comboBox_unit->count() == 0) {
        ui.comboBox_unit->addItem("No Units Available", -1);
    }
}

void MainMnemonicAddWindow::onAddButtonClicked()
{
    if (ui.lineEdit_name->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a mnemonic name.");
        return;
    }
    if (ui.comboBox_unit->currentData().toInt() == -1) {
        QMessageBox::warning(this, "Input Error", "Please select a unit.");
        return;
    }

    newMainMnemonic.setName(ui.lineEdit_name->text());
    newMainMnemonic.setDescription(ui.lineEdit_description->text());
    newMainMnemonic.setUnitId(ui.comboBox_unit->currentData().toInt());
    newMainMnemonic.setService(ui.checkBox_service->isChecked());

    accept();
}

void MainMnemonicAddWindow::onCancelButtonClicked()
{
    reject();
}

MainMnemonic MainMnemonicAddWindow::getNewMainMnemonic() const
{
    return newMainMnemonic;
}
