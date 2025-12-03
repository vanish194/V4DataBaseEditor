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

    // Устанавливаем модель для таблицы
    ui->tableView->setModel(model);

    // Устанавливаем заголовки столбцов
    model->setHorizontalHeaderLabels(QStringList() << tr("Revert") << tr("Type") << tr("ID")
                                                   << tr("Change Type"));

    // Заполняем данные
    populateData();

    // Подключаем кнопку "Revert Selected"
    connect(ui->buttonRevert, &QPushButton::clicked, this, &CompareDataDialog::onRevertSelected);
}

CompareDataDialog::~CompareDataDialog()
{
    delete ui;
}

void CompareDataDialog::populateData()
{
    ModifiedData modifiedData = storage->getAllModifiedData();

    // Список всех изменений для обработки
    struct ChangeItem
    {
        QString typeName;
        int id;
        QString changeType; // "Added", "Modified", или "Deleted"
    };

    QList<ChangeItem> changes;

    // Лямбда-функция для добавления элементов в список изменений
    auto addChanges =
        [&changes](const QString &typeName, const QList<int> &ids, const QString &changeType) {
            for (int id : ids) {
                changes.append({typeName, id, changeType});
            }
        };

    // Функции для извлечения ID
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

    // Добавляем изменения для всех типов данных
    // Units
    addChanges(tr("Unit"), toIds(modifiedData.addedUnits), tr("Added"));
    addChanges(tr("Unit"), toIds(modifiedData.modifiedUnits), tr("Modified"));
    addChanges(tr("Unit"), toNegativeIds(modifiedData.deletedUnits), tr("Deleted"));

    // Tools
    addChanges(tr("Tool"), toIds(modifiedData.addedTools), tr("Added"));
    addChanges(tr("Tool"), toIds(modifiedData.modifiedTools), tr("Modified"));
    addChanges(tr("Tool"), toNegativeIds(modifiedData.deletedTools), tr("Deleted"));

    // Sensors
    addChanges(tr("Sensor"), toIds(modifiedData.addedSensors), tr("Added"));
    addChanges(tr("Sensor"), toIds(modifiedData.modifiedSensors), tr("Modified"));
    addChanges(tr("Sensor"), toNegativeIds(modifiedData.deletedSensors), tr("Deleted"));

    // Producers
    addChanges(tr("Producer"), toIds(modifiedData.addedProducers), tr("Added"));
    addChanges(tr("Producer"), toIds(modifiedData.modifiedProducers), tr("Modified"));
    addChanges(tr("Producer"), toNegativeIds(modifiedData.deletedProducers), tr("Deleted"));

    // UnitTypes
    addChanges(tr("UnitType"), toIds(modifiedData.addedUnitTypes), tr("Added"));
    addChanges(tr("UnitType"), toIds(modifiedData.modifiedUnitTypes), tr("Modified"));
    addChanges(tr("UnitType"), toNegativeIds(modifiedData.deletedUnitTypes), tr("Deleted"));

    // ToolSensors
    addChanges(tr("ToolSensor"), toIds(modifiedData.addedToolSensors), tr("Added"));
    addChanges(tr("ToolSensor"), toIds(modifiedData.modifiedToolSensors), tr("Modified"));
    addChanges(tr("ToolSensor"), toNegativeIds(modifiedData.deletedToolSensors), tr("Deleted"));

    // ToolMnemonics
    addChanges(tr("ToolMnemonic"), toIds(modifiedData.addedToolMnemonics), tr("Added"));
    addChanges(tr("ToolMnemonic"), toIds(modifiedData.modifiedToolMnemonics), tr("Modified"));
    addChanges(tr("ToolMnemonic"), toNegativeIds(modifiedData.deletedToolMnemonics), tr("Deleted"));

    // SensorMnemonics
    addChanges(tr("SensorMnemonic"), toIds(modifiedData.addedSensorMnemonics), tr("Added"));
    addChanges(tr("SensorMnemonic"), toIds(modifiedData.modifiedSensorMnemonics), tr("Modified"));
    addChanges(tr("SensorMnemonic"),
               toNegativeIds(modifiedData.deletedSensorMnemonics),
               tr("Deleted"));

    // MainMnemonics
    addChanges(tr("MainMnemonic"), toIds(modifiedData.addedMainMnemonics), tr("Added"));
    addChanges(tr("MainMnemonic"), toIds(modifiedData.modifiedMainMnemonics), tr("Modified"));
    addChanges(tr("MainMnemonic"), toNegativeIds(modifiedData.deletedMainMnemonics), tr("Deleted"));

    // AdditionalMnemonics
    addChanges(tr("AdditionalMnemonic"), toIds(modifiedData.addedAdditionalMnemonics), tr("Added"));
    addChanges(tr("AdditionalMnemonic"),
               toIds(modifiedData.modifiedAdditionalMnemonics),
               tr("Modified"));
    addChanges(tr("AdditionalMnemonic"),
               toNegativeIds(modifiedData.deletedAdditionalMnemonics),
               tr("Deleted"));

    // ConversionFormulas
    addChanges(tr("ConversionFormula"), toIds(modifiedData.addedConversionFormulas), tr("Added"));
    addChanges(tr("ConversionFormula"),
               toIds(modifiedData.modifiedConversionFormulas),
               tr("Modified"));
    addChanges(tr("ConversionFormula"),
               toNegativeIds(modifiedData.deletedConversionFormulas),
               tr("Deleted"));

    // Companies
    addChanges(tr("Company"), toIds(modifiedData.addedCompanies), tr("Added"));
    addChanges(tr("Company"), toIds(modifiedData.modifiedCompanies), tr("Modified"));
    addChanges(tr("Company"), toNegativeIds(modifiedData.deletedCompanies), tr("Deleted"));

    // Methods
    addChanges(tr("Method"), toIds(modifiedData.addedMethods), tr("Added"));
    addChanges(tr("Method"), toIds(modifiedData.modifiedMethods), tr("Modified"));
    addChanges(tr("Method"), toNegativeIds(modifiedData.deletedMethods), tr("Deleted"));

    // ToolDescriptions
    addChanges(tr("ToolDescription"), toIds(modifiedData.addedToolDescriptions), tr("Added"));
    addChanges(tr("ToolDescription"), toIds(modifiedData.modifiedToolDescriptions), tr("Modified"));
    addChanges(tr("ToolDescription"),
               toNegativeIds(modifiedData.deletedToolDescriptions),
               tr("Deleted"));

    // Заполняем модель данными
    for (const ChangeItem &item : changes) {
        QList<QStandardItem *> rowItems;

        // Checkbox для выбора
        QStandardItem *checkItem = new QStandardItem();
        checkItem->setCheckable(true);
        checkItem->setCheckState(Qt::Unchecked);
        checkItem->setData(item.typeName, Qt::UserRole + 1); // Сохраняем тип
        checkItem->setData(item.id, Qt::UserRole + 2);       // Сохраняем ID
        checkItem->setData(item.changeType, Qt::UserRole + 3); // Сохраняем тип изменения

        rowItems.append(checkItem);
        rowItems.append(new QStandardItem(item.typeName));
        rowItems.append(new QStandardItem(QString::number(item.id)));
        rowItems.append(new QStandardItem(item.changeType));

        model->appendRow(rowItems);
    }
}

// Вспомогательные шаблонные функции для отката изменений
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
            item.setId(id); // Восстанавливаем положительный ID
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

    // Собираем выбранные элементы
    for (int row = 0; row < model->rowCount(); ++row) {
        QStandardItem *checkItem = model->item(row, 0);
        if (checkItem->checkState() == Qt::Checked) {
            rowsToRevert.append(row);
        }
    }

    if (rowsToRevert.isEmpty()) {
        QMessageBox::information(this, tr("No Selection"), tr("No items selected for revert."));
        return;
    }

    // Диалог подтверждения
    int ret = QMessageBox::warning(this,
                                   tr("Confirm Revert"),
                                   tr("Are you sure you want to revert the selected changes?"),
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) {
        return;
    }

    // Выполняем операции отката
    for (int row : rowsToRevert) {
        QString typeName = model->item(row, 0)->data(Qt::UserRole + 1).toString();
        int id = model->item(row, 0)->data(Qt::UserRole + 2).toInt();
        QString changeType = model->item(row, 0)->data(Qt::UserRole + 3).toString();

        if (typeName == tr("Unit")) {
            if (changeType == tr("Added")) {
                revertAddedData<Unit>(storage->getUnits(), id);
            } else if (changeType == tr("Modified")) {
                revertModifiedData<Unit>(storage->getUnits(), storage->getBackupUnits(), id);
            } else if (changeType == tr("Deleted")) {
                revertDeletedData<Unit>(storage->getUnits(), id);
            }
        } else if (typeName == tr("Tool")) {
            if (changeType == tr("Added")) {
                revertAddedData<Tool>(storage->getTools(), id);
            } else if (changeType == tr("Modified")) {
                revertModifiedData<Tool>(storage->getTools(), storage->getBackupTools(), id);
            } else if (changeType == tr("Deleted")) {
                revertDeletedData<Tool>(storage->getTools(), id);
            }
        } else if (typeName == tr("Sensor")) {
            if (changeType == tr("Added")) {
                revertAddedData<Sensor>(storage->getSensors(), id);
            } else if (changeType == tr("Modified")) {
                revertModifiedData<Sensor>(storage->getSensors(), storage->getBackupSensors(), id);
            } else if (changeType == tr("Deleted")) {
                revertDeletedData<Sensor>(storage->getSensors(), id);
            }
        } else if (typeName == tr("Producer")) {
            if (changeType == tr("Added")) {
                revertAddedData<Producer>(storage->getProducers(), id);
            } else if (changeType == tr("Modified")) {
                revertModifiedData<Producer>(storage->getProducers(),
                                             storage->getBackupProducers(),
                                             id);
            } else if (changeType == tr("Deleted")) {
                revertDeletedData<Producer>(storage->getProducers(), id);
            }
        } else if (typeName == tr("UnitType")) {
            if (changeType == tr("Added")) {
                revertAddedData<UnitType>(storage->getUnitTypes(), id);
            } else if (changeType == tr("Modified")) {
                revertModifiedData<UnitType>(storage->getUnitTypes(),
                                             storage->getBackupUnitTypes(),
                                             id);
            } else if (changeType == tr("Deleted")) {
                revertDeletedData<UnitType>(storage->getUnitTypes(), id);
            }
        } else if (typeName == tr("ToolSensor")) {
            if (changeType == tr("Added")) {
                revertAddedData<ToolSensor>(storage->getToolSensors(), id);
            } else if (changeType == tr("Modified")) {
                revertModifiedData<ToolSensor>(storage->getToolSensors(),
                                               storage->getBackupToolSensors(),
                                               id);
            } else if (changeType == tr("Deleted")) {
                revertDeletedData<ToolSensor>(storage->getToolSensors(), id);
            }
        } else if (typeName == tr("ToolMnemonic")) {
            if (changeType == tr("Added")) {
                revertAddedData<ToolMnemonic>(storage->getToolMnemonics(), id);
            } else if (changeType == tr("Modified")) {
                revertModifiedData<ToolMnemonic>(storage->getToolMnemonics(),
                                                 storage->getBackupToolMnemonics(),
                                                 id);
            } else if (changeType == tr("Deleted")) {
                revertDeletedData<ToolMnemonic>(storage->getToolMnemonics(), id);
            }
        } else if (typeName == tr("SensorMnemonic")) {
            if (changeType == tr("Added")) {
                revertAddedData<SensorMnemonic>(storage->getSensorMnemonics(), id);
            } else if (changeType == tr("Modified")) {
                revertModifiedData<SensorMnemonic>(storage->getSensorMnemonics(),
                                                   storage->getBackupSensorMnemonics(),
                                                   id);
            } else if (changeType == tr("Deleted")) {
                revertDeletedData<SensorMnemonic>(storage->getSensorMnemonics(), id);
            }
        } else if (typeName == tr("MainMnemonic")) {
            if (changeType == tr("Added")) {
                revertAddedData<MainMnemonic>(storage->getMainMnemonics(), id);
            } else if (changeType == tr("Modified")) {
                revertModifiedData<MainMnemonic>(storage->getMainMnemonics(),
                                                 storage->getBackupMainMnemonics(),
                                                 id);
            } else if (changeType == tr("Deleted")) {
                revertDeletedData<MainMnemonic>(storage->getMainMnemonics(), id);
            }
        } else if (typeName == tr("AdditionalMnemonic")) {
            if (changeType == tr("Added")) {
                revertAddedData<AdditionalMnemonic>(storage->getAdditionalMnemonics(), id);
            } else if (changeType == tr("Modified")) {
                revertModifiedData<AdditionalMnemonic>(storage->getAdditionalMnemonics(),
                                                       storage->getBackupAdditionalMnemonics(),
                                                       id);
            } else if (changeType == tr("Deleted")) {
                revertDeletedData<AdditionalMnemonic>(storage->getAdditionalMnemonics(), id);
            }
        } else if (typeName == tr("ConversionFormula")) {
            if (changeType == tr("Added")) {
                revertAddedData<ConversionFormula>(storage->getConversionFormulas(), id);
            } else if (changeType == tr("Modified")) {
                revertModifiedData<ConversionFormula>(storage->getConversionFormulas(),
                                                      storage->getBackupConversionFormulas(),
                                                      id);
            } else if (changeType == tr("Deleted")) {
                revertDeletedData<ConversionFormula>(storage->getConversionFormulas(), id);
            }
        } else if (typeName == tr("Company")) {
            if (changeType == tr("Added")) {
                revertAddedData<Company>(storage->getCompanies(), id);
            } else if (changeType == tr("Modified")) {
                revertModifiedData<Company>(storage->getCompanies(),
                                            storage->getBackupCompanies(),
                                            id);
            } else if (changeType == tr("Deleted")) {
                revertDeletedData<Company>(storage->getCompanies(), id);
            }
        } else if (typeName == tr("Method")) {
            if (changeType == tr("Added")) {
                revertAddedData<Method>(storage->getMethods(), id);
            } else if (changeType == tr("Modified")) {
                revertModifiedData<Method>(storage->getMethods(), storage->getBackupMethods(), id);
            } else if (changeType == tr("Deleted")) {
                revertDeletedData<Method>(storage->getMethods(), id);
            }
        } else if (typeName == tr("ToolDescription")) {
            if (changeType == tr("Added")) {
                revertAddedData<ToolDescription>(storage->getToolDescriptions(), id);
            } else if (changeType == tr("Modified")) {
                revertModifiedData<ToolDescription>(storage->getToolDescriptions(),
                                                    storage->getBackupToolDescriptions(),
                                                    id);
            } else if (changeType == tr("Deleted")) {
                revertDeletedData<ToolDescription>(storage->getToolDescriptions(), id);
            }
        }
    }

    QMessageBox::information(this,
                             tr("Revert Complete"),
                             tr("Selected changes have been reverted."));

    // Обновляем данные
    model->removeRows(0, model->rowCount());
    populateData();
}
