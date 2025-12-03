#include "additionalmnemonicaddwindow.h"
#include <QMessageBox>

AdditionalMnemonicAddWindow::AdditionalMnemonicAddWindow(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    connect(ui.pushButton_add, &QPushButton::clicked, this, &AdditionalMnemonicAddWindow::onAddButtonClicked);
    connect(ui.pushButton_cancel, &QPushButton::clicked, this, &AdditionalMnemonicAddWindow::onCancelButtonClicked);

    loadCompanies();
    loadMainMnemonics();
    loadUnits();
}

AdditionalMnemonicAddWindow::~AdditionalMnemonicAddWindow()
{
}

void AdditionalMnemonicAddWindow::setNewAdditionalMnemonicId(int id)
{
    newAdditionalMnemonic.setId(id);
}

void AdditionalMnemonicAddWindow::loadCompanies()
{
    ui.comboBox_company->clear();
    const auto &companies = Storage::getInstance()->getCompanies();
    for (const Company &company : companies) {
        if (company.getId() > 0) {
            ui.comboBox_company->addItem(company.getName(), company.getId());
        }
    }
    if (ui.comboBox_company->count() == 0) {
        ui.comboBox_company->addItem(tr("No Companies Available"), -1);
    }
    ui.comboBox_company->model()->sort(0);
}

void AdditionalMnemonicAddWindow::loadMainMnemonics()
{
    ui.comboBox_mainMnemonic->clear();
    const auto &mainMnemonics = Storage::getInstance()->getMainMnemonics();
    for (const MainMnemonic &mnemonic : mainMnemonics) {
        if (mnemonic.getId() > 0) {
            ui.comboBox_mainMnemonic->addItem(mnemonic.getName(), mnemonic.getId());
        }
    }
    if (ui.comboBox_mainMnemonic->count() == 0) {
        ui.comboBox_mainMnemonic->addItem(tr("No Main Mnemonics Available"), -1);
    }
    ui.comboBox_mainMnemonic->model()->sort(0);
}

void AdditionalMnemonicAddWindow::loadUnits()
{
    ui.comboBox_unit->clear();
    const auto &units = Storage::getInstance()->getUnits();
    for (const Unit &unit : units) {
        if (unit.getId() > 0) {
            ui.comboBox_unit->addItem(unit.getName(), unit.getId());
        }
    }
    if (ui.comboBox_unit->count() == 0) {
        ui.comboBox_unit->addItem(tr("No Units Available"), -1);
    }
    ui.comboBox_unit->model()->sort(0);
}

void AdditionalMnemonicAddWindow::onAddButtonClicked()
{
    if (ui.lineEdit_name->text().isEmpty()) {
        QMessageBox::warning(this, tr("Input Error"), tr("Please enter a mnemonic name."));
        return;
    }
    if (ui.comboBox_company->currentData().toInt() == -1) {
        QMessageBox::warning(this, tr("Input Error"), tr("Please select a company."));
        return;
    }
    if (ui.comboBox_mainMnemonic->currentData().toInt() == -1) {
        QMessageBox::warning(this, tr("Input Error"), tr("Please select a main mnemonic."));
        return;
    }
    if (ui.comboBox_unit->currentData().toInt() == -1) {
        QMessageBox::warning(this, tr("Input Error"), tr("Please select a unit."));
        return;
    }

    newAdditionalMnemonic.setName(ui.lineEdit_name->text());
    newAdditionalMnemonic.setCompanyId(ui.comboBox_company->currentData().toInt());
    newAdditionalMnemonic.setMainMnemonicId(ui.comboBox_mainMnemonic->currentData().toInt());
    newAdditionalMnemonic.setUnitId(ui.comboBox_unit->currentData().toInt());

    accept();
}

void AdditionalMnemonicAddWindow::onCancelButtonClicked()
{
    reject();
}

AdditionalMnemonic AdditionalMnemonicAddWindow::getNewAdditionalMnemonic() const
{
    return newAdditionalMnemonic;
}
