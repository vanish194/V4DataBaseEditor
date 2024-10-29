#include "mainmnemoniceditor.h"
#include "storageeditor.h"
#include <QMessageBox>

MainMnemonicEditor::MainMnemonicEditor(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    connect(ui.pushButton_save, &QPushButton::clicked, this, &MainMnemonicEditor::onSaveButtonClicked);

    loadUnits();
}

MainMnemonicEditor::~MainMnemonicEditor()
{
}

void MainMnemonicEditor::setMainMnemonic(const MainMnemonic &mnemonic)
{
    currentMainMnemonic = mnemonic;

    ui.lineEdit_name->setText(mnemonic.getName());
    ui.lineEdit_description->setText(mnemonic.getDescription());
    ui.checkBox_service->setChecked(mnemonic.isService());

    int unitIndex = ui.comboBox_unit->findData(mnemonic.getUnitId());
    if (unitIndex != -1) {
        ui.comboBox_unit->setCurrentIndex(unitIndex);
    }
}

void MainMnemonicEditor::loadUnits()
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

MainMnemonic MainMnemonicEditor::getMainMnemonic() const
{
    return currentMainMnemonic;
}

void MainMnemonicEditor::onSaveButtonClicked()
{
    if (ui.lineEdit_name->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a mnemonic name.");
        return;
    }
    if (ui.comboBox_unit->currentData().toInt() == -1) {
        QMessageBox::warning(this, "Input Error", "Please select a unit.");
        return;
    }

    currentMainMnemonic.setName(ui.lineEdit_name->text());
    currentMainMnemonic.setDescription(ui.lineEdit_description->text());
    currentMainMnemonic.setUnitId(ui.comboBox_unit->currentData().toInt());
    currentMainMnemonic.setService(ui.checkBox_service->isChecked());

    // Обновляем запись в хранилище
    StorageEditor editor(Storage::getInstance());
    editor.insertOrReplace(Storage::getInstance()->getMainMnemonics(), currentMainMnemonic);

    accept();
}
