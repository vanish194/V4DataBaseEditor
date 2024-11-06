#include "additionalmnemoniceditor.h"
#include "storageeditor.h"
#include <QMessageBox>

AdditionalMnemonicEditor::AdditionalMnemonicEditor(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    connect(ui.pushButton_save, &QPushButton::clicked, this, &AdditionalMnemonicEditor::onSaveButtonClicked);

    loadCompanies();
    loadMainMnemonics();
    loadUnits();
}

AdditionalMnemonicEditor::~AdditionalMnemonicEditor()
{
}

void AdditionalMnemonicEditor::setAdditionalMnemonic(const AdditionalMnemonic &mnemonic)
{
    currentAdditionalMnemonic = mnemonic;

    ui.lineEdit_name->setText(mnemonic.getName());

    int companyIndex = ui.comboBox_company->findData(mnemonic.getCompanyId());
    if (companyIndex != -1) {
        ui.comboBox_company->setCurrentIndex(companyIndex);
    }

    int mainMnemonicIndex = ui.comboBox_mainMnemonic->findData(mnemonic.getMainMnemonicId());
    if (mainMnemonicIndex != -1) {
        ui.comboBox_mainMnemonic->setCurrentIndex(mainMnemonicIndex);
    }

    int unitIndex = ui.comboBox_unit->findData(mnemonic.getUnitId());
    if (unitIndex != -1) {
        ui.comboBox_unit->setCurrentIndex(unitIndex);
    }
}

void AdditionalMnemonicEditor::loadCompanies()
{
    ui.comboBox_company->clear();
    const auto &companies = Storage::getInstance()->getCompanies();
    for (const Company &company : companies) {
        if (company.getId() > 0) {
            ui.comboBox_company->addItem(company.getName(), company.getId());
        }
    }
    if (ui.comboBox_company->count() == 0) {
        ui.comboBox_company->addItem("No Companies Available", -1);
    }
    ui.comboBox_company->model()->sort(0);
}

void AdditionalMnemonicEditor::loadMainMnemonics()
{
    ui.comboBox_mainMnemonic->clear();
    const auto &mainMnemonics = Storage::getInstance()->getMainMnemonics();
    for (const MainMnemonic &mnemonic : mainMnemonics) {
        if (mnemonic.getId() > 0) {
            ui.comboBox_mainMnemonic->addItem(mnemonic.getName(), mnemonic.getId());
        }
    }
    if (ui.comboBox_mainMnemonic->count() == 0) {
        ui.comboBox_mainMnemonic->addItem("No Main Mnemonics Available", -1);
    }
    ui.comboBox_mainMnemonic->model()->sort(0);
}

void AdditionalMnemonicEditor::loadUnits()
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
    ui.comboBox_unit->model()->sort(0);
}

AdditionalMnemonic AdditionalMnemonicEditor::getAdditionalMnemonic() const
{
    return currentAdditionalMnemonic;
}

void AdditionalMnemonicEditor::onSaveButtonClicked()
{
    if (ui.lineEdit_name->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a mnemonic name.");
        return;
    }
    if (ui.comboBox_company->currentData().toInt() == -1) {
        QMessageBox::warning(this, "Input Error", "Please select a company.");
        return;
    }
    if (ui.comboBox_mainMnemonic->currentData().toInt() == -1) {
        QMessageBox::warning(this, "Input Error", "Please select a main mnemonic.");
        return;
    }
    if (ui.comboBox_unit->currentData().toInt() == -1) {
        QMessageBox::warning(this, "Input Error", "Please select a unit.");
        return;
    }

    currentAdditionalMnemonic.setName(ui.lineEdit_name->text());
    currentAdditionalMnemonic.setCompanyId(ui.comboBox_company->currentData().toInt());
    currentAdditionalMnemonic.setMainMnemonicId(ui.comboBox_mainMnemonic->currentData().toInt());
    currentAdditionalMnemonic.setUnitId(ui.comboBox_unit->currentData().toInt());

    // Обновляем запись в хранилище
    StorageEditor editor(Storage::getInstance());
    editor.insertOrReplace(Storage::getInstance()->getAdditionalMnemonics(), currentAdditionalMnemonic);

    accept();
}
