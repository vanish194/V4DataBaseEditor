#include "comparedatadialog.h"
#include "ui_comparedatadialog.h"

#include <QCheckBox>
#include <QMessageBox>

CompareDataDialog::CompareDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompareDataDialog),
    storage(Storage::getInstance()),
    model(new QStandardItemModel(this))
{
    ui->setupUi(this);

    // Set up the model for the table
    ui->tableView->setModel(model);

    // Set column headers
    model->setHorizontalHeaderLabels(QStringList() << "Revert" << "Type" << "ID" << "Change Type");

    // Populate data
    populateData();

    // Connect the "Revert Selected" button
    connect(ui->buttonRevert, &QPushButton::clicked, this, &CompareDataDialog::onRevertSelected);
}

CompareDataDialog::~CompareDataDialog()
{
    delete ui;
}

void CompareDataDialog::populateData()
{
    ModifiedData modifiedData = storage->getAllModifiedData();

    // List of all changes for processing
    struct ChangeItem {
        QString typeName;
        int id;
        QString changeType; // "Modified" or "Deleted"
    };

    QList<ChangeItem> changes;

    // Add all modified and deleted objects to the list
    // For Units
    for (const Unit &unit : modifiedData.modifiedUnits) {
        changes.append({"Unit", unit.getId(), "Modified"});
    }
    for (const Unit &unit : modifiedData.deletedUnits) {
        changes.append({"Unit", -unit.getId(), "Deleted"});
    }

    // Repeat for all other data types
    // For Tools
    for (const Tool &tool : modifiedData.modifiedTools) {
        changes.append({"Tool", tool.getId(), "Modified"});
    }
    for (const Tool &tool : modifiedData.deletedTools) {
        changes.append({"Tool", -tool.getId(), "Deleted"});
    }

    // For Sensors
    for (const Sensor &sensor : modifiedData.modifiedSensors) {
        changes.append({"Sensor", sensor.getId(), "Modified"});
    }
    for (const Sensor &sensor : modifiedData.deletedSensors) {
        changes.append({"Sensor", -sensor.getId(), "Deleted"});
    }

    // ... Repeat for all other data types (Producers, UnitTypes, etc.)

    // For Producers
    for (const Producer &producer : modifiedData.modifiedProducers) {
        changes.append({"Producer", producer.getId(), "Modified"});
    }
    for (const Producer &producer : modifiedData.deletedProducers) {
        changes.append({"Producer", -producer.getId(), "Deleted"});
    }

    // For UnitTypes
    for (const UnitType &unitType : modifiedData.modifiedUnitTypes) {
        changes.append({"UnitType", unitType.getId(), "Modified"});
    }
    for (const UnitType &unitType : modifiedData.deletedUnitTypes) {
        changes.append({"UnitType", -unitType.getId(), "Deleted"});
    }

    // For ToolSensors
    for (const ToolSensor &toolSensor : modifiedData.modifiedToolSensors) {
        changes.append({"ToolSensor", toolSensor.getId(), "Modified"});
    }
    for (const ToolSensor &toolSensor : modifiedData.deletedToolSensors) {
        changes.append({"ToolSensor", -toolSensor.getId(), "Deleted"});
    }

    // For MainMnemonics
    for (const MainMnemonic &mainMnemonic : modifiedData.modifiedMainMnemonics) {
        changes.append({"MainMnemonic", mainMnemonic.getId(), "Modified"});
    }
    for (const MainMnemonic &mainMnemonic : modifiedData.deletedMainMnemonics) {
        changes.append({"MainMnemonic", -mainMnemonic.getId(), "Deleted"});
    }

    // For AdditionalMnemonics
    for (const AdditionalMnemonic &additionalMnemonic : modifiedData.modifiedAdditionalMnemonics) {
        changes.append({"AdditionalMnemonic", additionalMnemonic.getId(), "Modified"});
    }
    for (const AdditionalMnemonic &additionalMnemonic : modifiedData.deletedAdditionalMnemonics) {
        changes.append({"AdditionalMnemonic", -additionalMnemonic.getId(), "Deleted"});
    }

    // For ConversionFormulas
    for (const ConversionFormula &formula : modifiedData.modifiedConversionFormulas) {
        changes.append({"ConversionFormula", formula.getId(), "Modified"});
    }
    for (const ConversionFormula &formula : modifiedData.deletedConversionFormulas) {
        changes.append({"ConversionFormula", -formula.getId(), "Deleted"});
    }

    // For Companies
    for (const Company &company : modifiedData.modifiedCompanies) {
        changes.append({"Company", company.getId(), "Modified"});
    }
    for (const Company &company : modifiedData.deletedCompanies) {
        changes.append({"Company", -company.getId(), "Deleted"});
    }

    // For Methods
    for (const Method &method : modifiedData.modifiedMethods) {
        changes.append({"Method", method.getId(), "Modified"});
    }
    for (const Method &method : modifiedData.deletedMethods) {
        changes.append({"Method", -method.getId(), "Deleted"});
    }

    // For ToolDescriptions
    for (const ToolDescription &description : modifiedData.modifiedToolDescriptions) {
        changes.append({"ToolDescription", description.getId(), "Modified"});
    }
    for (const ToolDescription &description : modifiedData.deletedToolDescriptions) {
        changes.append({"ToolDescription", -description.getId(), "Deleted"});
    }

    // Populate the model with data
    for (const ChangeItem &item : changes) {
        QList<QStandardItem*> rowItems;

        // Checkbox for selection
        QStandardItem *checkItem = new QStandardItem();
        checkItem->setCheckable(true);
        checkItem->setCheckState(Qt::Unchecked);
        checkItem->setData(item.typeName, Qt::UserRole + 1);   // Store type name
        checkItem->setData(item.id, Qt::UserRole + 2);         // Store ID
        checkItem->setData(item.changeType, Qt::UserRole + 3); // Store change type

        rowItems.append(checkItem);
        rowItems.append(new QStandardItem(item.typeName));
        rowItems.append(new QStandardItem(QString::number(item.id)));
        rowItems.append(new QStandardItem(item.changeType));

        model->appendRow(rowItems);
    }
}

// Helper template functions to revert changes
template<typename T>
void revertModifiedData(QList<T> &dataList, const QList<T> &backupList, int id)
{
    for (const T &backupItem : backupList) {
        if (backupItem.getId() == id) {
            for (T &item : dataList) {
                if (item.getId() == id) {
                    item = backupItem;
                    break;
                }
            }
            break;
        }
    }
}

template<typename T>
void revertDeletedData(QList<T> &dataList, int id)
{
    for (T &item : dataList) {
        if (item.getId() == -id) {
            item.setId(id); // Restore the positive ID
            break;
        }
    }
}

void CompareDataDialog::onRevertSelected()
{
    QList<int> rowsToRevert;

    // Collect selected items
    for (int row = 0; row < model->rowCount(); ++row) {
        QStandardItem *checkItem = model->item(row, 0);
        if (checkItem->checkState() == Qt::Checked) {
            rowsToRevert.append(row);
        }
    }

    if (rowsToRevert.isEmpty()) {
        QMessageBox::information(this, "No Selection", "No items selected for revert.");
        return;
    }

    // Confirmation dialog
    int ret = QMessageBox::warning(this,
                                   "Confirm Revert",
                                   "Are you sure you want to revert the selected changes?",
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) {
        return;
    }

    // Perform revert operations
    for (int row : rowsToRevert) {
        QString typeName = model->item(row, 0)->data(Qt::UserRole + 1).toString();
        int id = model->item(row, 0)->data(Qt::UserRole + 2).toInt();
        QString changeType = model->item(row, 0)->data(Qt::UserRole + 3).toString();

        if (typeName == "Unit") {
            if (changeType == "Modified") {
                revertModifiedData<Unit>(storage->getUnits(), storage->getBackupUnits(), id);
            } else if (changeType == "Deleted") {
                revertDeletedData<Unit>(storage->getUnits(), id);
            }
        } else if (typeName == "Tool") {
            if (changeType == "Modified") {
                revertModifiedData<Tool>(storage->getTools(), storage->getBackupTools(), id);
            } else if (changeType == "Deleted") {
                revertDeletedData<Tool>(storage->getTools(), id);
            }
        } else if (typeName == "Sensor") {
            if (changeType == "Modified") {
                revertModifiedData<Sensor>(storage->getSensors(), storage->getBackupSensors(), id);
            } else if (changeType == "Deleted") {
                revertDeletedData<Sensor>(storage->getSensors(), id);
            }
        } else if (typeName == "Producer") {
            if (changeType == "Modified") {
                revertModifiedData<Producer>(storage->getProducers(), storage->getBackupProducers(), id);
            } else if (changeType == "Deleted") {
                revertDeletedData<Producer>(storage->getProducers(), id);
            }
        } else if (typeName == "UnitType") {
            if (changeType == "Modified") {
                revertModifiedData<UnitType>(storage->getUnitTypes(), storage->getBackupUnitTypes(), id);
            } else if (changeType == "Deleted") {
                revertDeletedData<UnitType>(storage->getUnitTypes(), id);
            }
        } else if (typeName == "ToolSensor") {
            if (changeType == "Modified") {
                revertModifiedData<ToolSensor>(storage->getToolSensors(), storage->getBackupToolSensors(), id);
            } else if (changeType == "Deleted") {
                revertDeletedData<ToolSensor>(storage->getToolSensors(), id);
            }
        } else if (typeName == "MainMnemonic") {
            if (changeType == "Modified") {
                revertModifiedData<MainMnemonic>(storage->getMainMnemonics(), storage->getBackupMainMnemonics(), id);
            } else if (changeType == "Deleted") {
                revertDeletedData<MainMnemonic>(storage->getMainMnemonics(), id);
            }
        } else if (typeName == "AdditionalMnemonic") {
            if (changeType == "Modified") {
                revertModifiedData<AdditionalMnemonic>(storage->getAdditionalMnemonics(), storage->getBackupAdditionalMnemonics(), id);
            } else if (changeType == "Deleted") {
                revertDeletedData<AdditionalMnemonic>(storage->getAdditionalMnemonics(), id);
            }
        } else if (typeName == "ConversionFormula") {
            if (changeType == "Modified") {
                revertModifiedData<ConversionFormula>(storage->getConversionFormulas(), storage->getBackupConversionFormulas(), id);
            } else if (changeType == "Deleted") {
                revertDeletedData<ConversionFormula>(storage->getConversionFormulas(), id);
            }
        } else if (typeName == "Company") {
            if (changeType == "Modified") {
                revertModifiedData<Company>(storage->getCompanies(), storage->getBackupCompanies(), id);
            } else if (changeType == "Deleted") {
                revertDeletedData<Company>(storage->getCompanies(), id);
            }
        } else if (typeName == "Method") {
            if (changeType == "Modified") {
                revertModifiedData<Method>(storage->getMethods(), storage->getBackupMethods(), id);
            } else if (changeType == "Deleted") {
                revertDeletedData<Method>(storage->getMethods(), id);
            }
        } else if (typeName == "ToolDescription") {
            if (changeType == "Modified") {
                revertModifiedData<ToolDescription>(storage->getToolDescriptions(), storage->getBackupToolDescriptions(), id);
            } else if (changeType == "Deleted") {
                revertDeletedData<ToolDescription>(storage->getToolDescriptions(), id);
            }
        }
    }

    QMessageBox::information(this, "Revert Complete", "Selected changes have been reverted.");

    // Refresh the data
    model->removeRows(0, model->rowCount());
    populateData();
}
