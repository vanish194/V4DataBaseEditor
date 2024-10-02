#include "comparedatadialog.h"
#include "ui_comparedatadialog.h"

#include <QCheckBox>
#include <QMessageBox>

CompareDataDialog::CompareDataDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CompareDataDialog)
    , storage(Storage::getInstance())
    , model(new QStandardItemModel(this))
{
    ui->setupUi(this);

    // Set up the model for the table
    ui->tableView->setModel(model);

    // Set column headers
    model->setHorizontalHeaderLabels(QStringList() << "Revert"
                                                   << "Type"
                                                   << "ID"
                                                   << "Change Type");

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
    struct ChangeItem
    {
        QString typeName;
        int id;
        QString changeType; // "Added", "Modified", or "Deleted"
    };

    QList<ChangeItem> changes;

    // Helper lambda to add items to the changes list
    auto addChanges =
        [&changes](const QString &typeName, const QList<int> &ids, const QString &changeType) {
            for (int id : ids) {
                changes.append({typeName, id, changeType});
            }
        };

    // Helper functions to extract IDs
    auto toIds = [](const auto &list) -> QList<int> {
        QList<int> ids;
        for (const auto &item : list) {
            ids.append(item.getId());
        }
        return ids;
    };

    auto toNegativeIds = [](const auto &list) -> QList<int> {
        QList<int> ids;
        for (const auto &item : list) {
            ids.append(-item.getId());
        }
        return ids;
    };

    // For Units
    addChanges("Unit", toIds(modifiedData.addedUnits), "Added");
    addChanges("Unit", toIds(modifiedData.modifiedUnits), "Modified");
    addChanges("Unit", toNegativeIds(modifiedData.deletedUnits), "Deleted");

    // For Tools
    addChanges("Tool", toIds(modifiedData.addedTools), "Added");
    addChanges("Tool", toIds(modifiedData.modifiedTools), "Modified");
    addChanges("Tool", toNegativeIds(modifiedData.deletedTools), "Deleted");

    // For Sensors
    addChanges("Sensor", toIds(modifiedData.addedSensors), "Added");
    addChanges("Sensor", toIds(modifiedData.modifiedSensors), "Modified");
    addChanges("Sensor", toNegativeIds(modifiedData.deletedSensors), "Deleted");

    // For Producers
    addChanges("Producer", toIds(modifiedData.addedProducers), "Added");
    addChanges("Producer", toIds(modifiedData.modifiedProducers), "Modified");
    addChanges("Producer", toNegativeIds(modifiedData.deletedProducers), "Deleted");

    // For UnitTypes
    addChanges("UnitType", toIds(modifiedData.addedUnitTypes), "Added");
    addChanges("UnitType", toIds(modifiedData.modifiedUnitTypes), "Modified");
    addChanges("UnitType", toNegativeIds(modifiedData.deletedUnitTypes), "Deleted");

    // For ToolSensors
    addChanges("ToolSensor", toIds(modifiedData.addedToolSensors), "Added");
    addChanges("ToolSensor", toIds(modifiedData.modifiedToolSensors), "Modified");
    addChanges("ToolSensor", toNegativeIds(modifiedData.deletedToolSensors), "Deleted");

    // For MainMnemonics
    addChanges("MainMnemonic", toIds(modifiedData.addedMainMnemonics), "Added");
    addChanges("MainMnemonic", toIds(modifiedData.modifiedMainMnemonics), "Modified");
    addChanges("MainMnemonic", toNegativeIds(modifiedData.deletedMainMnemonics), "Deleted");

    // For AdditionalMnemonics
    addChanges("AdditionalMnemonic", toIds(modifiedData.addedAdditionalMnemonics), "Added");
    addChanges("AdditionalMnemonic", toIds(modifiedData.modifiedAdditionalMnemonics), "Modified");
    addChanges("AdditionalMnemonic",
               toNegativeIds(modifiedData.deletedAdditionalMnemonics),
               "Deleted");

    // For ConversionFormulas
    addChanges("ConversionFormula", toIds(modifiedData.addedConversionFormulas), "Added");
    addChanges("ConversionFormula", toIds(modifiedData.modifiedConversionFormulas), "Modified");
    addChanges("ConversionFormula",
               toNegativeIds(modifiedData.deletedConversionFormulas),
               "Deleted");

    // For Companies
    addChanges("Company", toIds(modifiedData.addedCompanies), "Added");
    addChanges("Company", toIds(modifiedData.modifiedCompanies), "Modified");
    addChanges("Company", toNegativeIds(modifiedData.deletedCompanies), "Deleted");

    // For Methods
    addChanges("Method", toIds(modifiedData.addedMethods), "Added");
    addChanges("Method", toIds(modifiedData.modifiedMethods), "Modified");
    addChanges("Method", toNegativeIds(modifiedData.deletedMethods), "Deleted");

    // For ToolDescriptions
    addChanges("ToolDescription", toIds(modifiedData.addedToolDescriptions), "Added");
    addChanges("ToolDescription", toIds(modifiedData.modifiedToolDescriptions), "Modified");
    addChanges("ToolDescription", toNegativeIds(modifiedData.deletedToolDescriptions), "Deleted");

    // Populate the model with data
    for (const ChangeItem &item : changes) {
        QList<QStandardItem *> rowItems;

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

template<typename T>
void revertAddedData(QList<T> &dataList, int id)
{
    for (int i = 0; i < dataList.size(); ++i) {
        if (dataList[i].getId() == id) {
            dataList.removeAt(i);
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
            if (changeType == "Added") {
                revertAddedData<Unit>(storage->getUnits(), id);
            } else if (changeType == "Modified") {
                revertModifiedData<Unit>(storage->getUnits(), storage->getBackupUnits(), id);
            } else if (changeType == "Deleted") {
                revertDeletedData<Unit>(storage->getUnits(), id);
            }
        } else if (typeName == "Tool") {
            if (changeType == "Added") {
                revertAddedData<Tool>(storage->getTools(), id);
            } else if (changeType == "Modified") {
                revertModifiedData<Tool>(storage->getTools(), storage->getBackupTools(), id);
            } else if (changeType == "Deleted") {
                revertDeletedData<Tool>(storage->getTools(), id);
            }
        } else if (typeName == "Sensor") {
            if (changeType == "Added") {
                revertAddedData<Sensor>(storage->getSensors(), id);
            } else if (changeType == "Modified") {
                revertModifiedData<Sensor>(storage->getSensors(), storage->getBackupSensors(), id);
            } else if (changeType == "Deleted") {
                revertDeletedData<Sensor>(storage->getSensors(), id);
            }
        } else if (typeName == "Producer") {
            if (changeType == "Added") {
                revertAddedData<Producer>(storage->getProducers(), id);
            } else if (changeType == "Modified") {
                revertModifiedData<Producer>(storage->getProducers(),
                                             storage->getBackupProducers(),
                                             id);
            } else if (changeType == "Deleted") {
                revertDeletedData<Producer>(storage->getProducers(), id);
            }
        } else if (typeName == "UnitType") {
            if (changeType == "Added") {
                revertAddedData<UnitType>(storage->getUnitTypes(), id);
            } else if (changeType == "Modified") {
                revertModifiedData<UnitType>(storage->getUnitTypes(),
                                             storage->getBackupUnitTypes(),
                                             id);
            } else if (changeType == "Deleted") {
                revertDeletedData<UnitType>(storage->getUnitTypes(), id);
            }
        } else if (typeName == "ToolSensor") {
            if (changeType == "Added") {
                revertAddedData<ToolSensor>(storage->getToolSensors(), id);
            } else if (changeType == "Modified") {
                revertModifiedData<ToolSensor>(storage->getToolSensors(),
                                               storage->getBackupToolSensors(),
                                               id);
            } else if (changeType == "Deleted") {
                revertDeletedData<ToolSensor>(storage->getToolSensors(), id);
            }
        } else if (typeName == "MainMnemonic") {
            if (changeType == "Added") {
                revertAddedData<MainMnemonic>(storage->getMainMnemonics(), id);
            } else if (changeType == "Modified") {
                revertModifiedData<MainMnemonic>(storage->getMainMnemonics(),
                                                 storage->getBackupMainMnemonics(),
                                                 id);
            } else if (changeType == "Deleted") {
                revertDeletedData<MainMnemonic>(storage->getMainMnemonics(), id);
            }
        } else if (typeName == "AdditionalMnemonic") {
            if (changeType == "Added") {
                revertAddedData<AdditionalMnemonic>(storage->getAdditionalMnemonics(), id);
            } else if (changeType == "Modified") {
                revertModifiedData<AdditionalMnemonic>(storage->getAdditionalMnemonics(),
                                                       storage->getBackupAdditionalMnemonics(),
                                                       id);
            } else if (changeType == "Deleted") {
                revertDeletedData<AdditionalMnemonic>(storage->getAdditionalMnemonics(), id);
            }
        } else if (typeName == "ConversionFormula") {
            if (changeType == "Added") {
                revertAddedData<ConversionFormula>(storage->getConversionFormulas(), id);
            } else if (changeType == "Modified") {
                revertModifiedData<ConversionFormula>(storage->getConversionFormulas(),
                                                      storage->getBackupConversionFormulas(),
                                                      id);
            } else if (changeType == "Deleted") {
                revertDeletedData<ConversionFormula>(storage->getConversionFormulas(), id);
            }
        } else if (typeName == "Company") {
            if (changeType == "Added") {
                revertAddedData<Company>(storage->getCompanies(), id);
            } else if (changeType == "Modified") {
                revertModifiedData<Company>(storage->getCompanies(),
                                            storage->getBackupCompanies(),
                                            id);
            } else if (changeType == "Deleted") {
                revertDeletedData<Company>(storage->getCompanies(), id);
            }
        } else if (typeName == "Method") {
            if (changeType == "Added") {
                revertAddedData<Method>(storage->getMethods(), id);
            } else if (changeType == "Modified") {
                revertModifiedData<Method>(storage->getMethods(), storage->getBackupMethods(), id);
            } else if (changeType == "Deleted") {
                revertDeletedData<Method>(storage->getMethods(), id);
            }
        } else if (typeName == "ToolDescription") {
            if (changeType == "Added") {
                revertAddedData<ToolDescription>(storage->getToolDescriptions(), id);
            } else if (changeType == "Modified") {
                revertModifiedData<ToolDescription>(storage->getToolDescriptions(),
                                                    storage->getBackupToolDescriptions(),
                                                    id);
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
