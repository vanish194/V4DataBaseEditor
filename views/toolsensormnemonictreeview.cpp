#include "toolsensormnemonictreeview.h"
#include <QAction>
#include <QContextMenuEvent>
#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>
#include <QStandardItem>

ToolSensorMnemonicTreeView::ToolSensorMnemonicTreeView(QWidget *parent)
    : QTreeView(parent)
    , model(new QStandardItemModel(this))
{
    setHeaderHidden(false);
    model->setHorizontalHeaderLabels(QStringList() << "Tool -> Sensor -> Mnemonics");
    setModel(model);
    buildTree();

    expandAll();

    header()->setSectionResizeMode(QHeaderView::Stretch);
}

void ToolSensorMnemonicTreeView::buildTree()
{
    Storage *storage = Storage::getInstance();
    model->clear();
    model->setHorizontalHeaderLabels(QStringList() << "Tool -> Sensor -> Mnemonics");

    for (const Tool &tool : storage->getTools()) {
        // Пропускаем удалённые инструменты
        if (tool.getId() < 0) {
            continue;
        }

        QStandardItem *toolItem = new QStandardItem(tool.getName());
        toolItem->setData(QVariant(tool.getId()), Qt::UserRole + 1); // Сохраняем ID инструмента
        toolItem->setData(QVariant(ToolType), Qt::UserRole + 2); // Сохраняем тип элемента

        // Встроенные мнемоники для прибора
        for (const ToolMnemonic &toolMnemonic : storage->getToolMnemonics()) {
            // Пропускаем удалённые связи
            if (toolMnemonic.getId() < 0) {
                continue;
            }
            if (toolMnemonic.getToolId() == tool.getId()) {
                const MainMnemonic *mnemonic = storage->findMainMnemonicById(
                    toolMnemonic.getMnemonicId());
                if (mnemonic && mnemonic->getId() > 0) { // Проверяем, что мнемоника не удалена
                    QStandardItem *mnemonicItem = new QStandardItem(mnemonic->getName());
                    mnemonicItem->setData(QVariant(mnemonic->getId()),
                                          Qt::UserRole + 1); // ID мнемоники
                    mnemonicItem->setData(QVariant(MainMnemonicType),
                                          Qt::UserRole + 2); // Тип элемента

                    // Обработка дополнительных мнемоник для данной основной мнемоники
                    for (const AdditionalMnemonic &additionalMnemonic :
                         storage->getAdditionalMnemonics()) {
                        // Пропускаем удалённые дополнительные мнемоники
                        if (additionalMnemonic.getId() < 0) {
                            continue;
                        }
                        if (additionalMnemonic.getMainMnemonicId() == mnemonic->getId()) {
                            QStandardItem *additionalMnemonicItem = new QStandardItem(
                                additionalMnemonic.getName());
                            additionalMnemonicItem->setData(QVariant(additionalMnemonic.getId()),
                                                            Qt::UserRole
                                                                + 1); // ID дополнительной мнемоники
                            additionalMnemonicItem->setData(QVariant(AdditionalMnemonicType),
                                                            Qt::UserRole + 2); // Тип элемента
                            mnemonicItem->appendRow(additionalMnemonicItem);
                        }
                    }

                    toolItem->appendRow(mnemonicItem);
                }
            }
        }

        // Получение всех сенсоров для данного инструмента
        QList<ToolSensor> toolSensors;
        for (const ToolSensor &toolSensor : storage->getToolSensors()) {
            // Пропускаем удалённые связи
            if (toolSensor.getId() < 0) {
                continue;
            }
            if (toolSensor.getToolId() == tool.getId()) {
                toolSensors.append(toolSensor);
            }
        }

        // Если нет сенсоров, добавляем "N/A"
        if (toolSensors.isEmpty()) {
            QStandardItem *naItem = new QStandardItem("N/A");
            naItem->setData(QVariant(0), Qt::UserRole + 1);        // ID элемента N/A
            naItem->setData(QVariant(NoneType), Qt::UserRole + 2); // Тип элемента
            toolItem->appendRow(naItem);
        }

        for (const ToolSensor &toolSensor : toolSensors) {
            const Sensor *sensor = findSensorById(toolSensor.getSensorId());
            if (sensor && sensor->getId() > 0) { // Проверяем, что сенсор не удалён
                QStandardItem *sensorItem = new QStandardItem(sensor->getName());
                sensorItem->setData(QVariant(sensor->getId()),
                                    Qt::UserRole + 1); // Сохраняем ID сенсора
                sensorItem->setData(QVariant(SensorType),
                                    Qt::UserRole + 2); // Сохраняем тип элемента

                // Мнемоники, связанные с этим сенсором (через SensorMnemonic)
                QList<const MainMnemonic *> sensorMnemonics;
                for (const SensorMnemonic &sensorMnemonic : storage->getSensorMnemonics()) {
                    // Пропускаем удалённые связи
                    if (sensorMnemonic.getId() < 0) {
                        continue;
                    }
                    if (sensorMnemonic.getSensorId() == sensor->getId()) {
                        const MainMnemonic *mnemonic = storage->findMainMnemonicById(
                            sensorMnemonic.getMnemonicId());
                        if (mnemonic
                            && mnemonic->getId() > 0) { // Проверяем, что мнемоника не удалена
                            sensorMnemonics.append(mnemonic);
                        }
                    }
                }

                // Если нет связанных мнемоник, добавляем "N/A"
                if (sensorMnemonics.isEmpty()) {
                    QStandardItem *naItem = new QStandardItem("N/A");
                    naItem->setData(QVariant(0), Qt::UserRole + 1); // ID элемента N/A
                    naItem->setData(QVariant(NoneType), Qt::UserRole + 2); // Тип элемента
                    sensorItem->appendRow(naItem);
                }

                for (const MainMnemonic *mnemonic : sensorMnemonics) {
                    QStandardItem *mnemonicItem = new QStandardItem(mnemonic->getName());
                    mnemonicItem->setData(QVariant(mnemonic->getId()),
                                          Qt::UserRole + 1); // ID мнемоники
                    mnemonicItem->setData(QVariant(MainMnemonicType),
                                          Qt::UserRole + 2); // Тип элемента

                    // Обработка дополнительных мнемоник для данной основной мнемоники
                    for (const AdditionalMnemonic &additionalMnemonic :
                         storage->getAdditionalMnemonics()) {
                        // Пропускаем удалённые дополнительные мнемоники
                        if (additionalMnemonic.getId() < 0) {
                            continue;
                        }
                        if (additionalMnemonic.getMainMnemonicId() == mnemonic->getId()) {
                            QStandardItem *additionalMnemonicItem = new QStandardItem(
                                additionalMnemonic.getName());
                            additionalMnemonicItem->setData(QVariant(additionalMnemonic.getId()),
                                                            Qt::UserRole
                                                                + 1); // ID дополнительной мнемоники
                            additionalMnemonicItem->setData(QVariant(AdditionalMnemonicType),
                                                            Qt::UserRole + 2); // Тип элемента
                            mnemonicItem->appendRow(additionalMnemonicItem);
                        }
                    }

                    sensorItem->appendRow(mnemonicItem);
                }

                toolItem->appendRow(sensorItem);
            } else {
                // Если сенсор не найден или удалён, добавляем "N/A"
                QStandardItem *naItem = new QStandardItem("N/A");
                naItem->setData(QVariant(0), Qt::UserRole + 1);        // ID элемента N/A
                naItem->setData(QVariant(NoneType), Qt::UserRole + 2); // Тип элемента
                toolItem->appendRow(naItem);
            }
        }

        model->appendRow(toolItem);
    }

    // Расширить все узлы
    expandAll();
}

const Sensor *ToolSensorMnemonicTreeView::findSensorById(int sensorId)
{
    Storage *storage = Storage::getInstance();
    for (const Sensor &sensor : storage->getSensors()) {
        // Пропускаем удалённые сенсоры
        if (sensor.getId() < 0) {
            continue;
        }
        if (sensor.getId() == sensorId) {
            return &sensor;
        }
    }
    return nullptr;
}

void ToolSensorMnemonicTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    int elementId = 0;
    ElementType elementType = NoneType;

    if (index.isValid()) {
        elementId = index.data(Qt::UserRole + 1).toInt();
        elementType = static_cast<ElementType>(index.data(Qt::UserRole + 2).toInt());
    }

    QMenu contextMenu(this);

    // Добавляем действия "Add Tool", "Add Sensor", "Add Main Mnemonic", "Add Additional Mnemonic"
    QAction *addToolAction = contextMenu.addAction("Add Tool");
    connect(addToolAction, &QAction::triggered, this, [=]() { onAddItem(ToolType); });

    QAction *addSensorAction = contextMenu.addAction("Add Sensor");
    connect(addSensorAction, &QAction::triggered, this, [=]() { onAddItem(SensorType); });

    QAction *addMainMnemonicAction = contextMenu.addAction("Add Main Mnemonic");
    connect(addMainMnemonicAction, &QAction::triggered, this, [=]() {
        onAddItem(MainMnemonicType);
    });

    QAction *addAdditionalMnemonicAction = contextMenu.addAction("Add Additional Mnemonic");
    connect(addAdditionalMnemonicAction, &QAction::triggered, this, [=]() {
        onAddItem(AdditionalMnemonicType);
    });

    // Добавляем действие "Delete" для выбранных элементов определённых типов
    if (elementId != 0
        && (elementType == ToolType || elementType == SensorType || elementType == MainMnemonicType
            || elementType == AdditionalMnemonicType)) {
        QAction *deleteAction = contextMenu.addAction("Delete");
        connect(deleteAction, &QAction::triggered, this, [=]() {
            onDeleteItem(elementId, elementType);
        });
    }

    // Добавляем редакторы связей для инструментов и сенсоров
    if (elementId != 0 && elementType == ToolType) {
        QAction *toolSensorRelationsAction = contextMenu.addAction("Relation ToolSensor Editor");
        connect(toolSensorRelationsAction, &QAction::triggered, this, [=]() {
            onToolSensorRelations(elementId);
        });

        QAction *toolMnemonicRelationsAction = contextMenu.addAction(
            "Relation ToolMnemonic Editor");
        connect(toolMnemonicRelationsAction, &QAction::triggered, this, [=]() {
            onToolMnemonicRelations(elementId);
        });
    } else if (elementId != 0 && elementType == SensorType) {
        QAction *sensorMnemonicRelationsAction = contextMenu.addAction(
            "Relation SensorMnemonic Editor");
        connect(sensorMnemonicRelationsAction, &QAction::triggered, this, [=]() {
            onSensorMnemonicRelations(elementId);
        });
    }

    contextMenu.exec(mapToGlobal(event->pos()));
}

void ToolSensorMnemonicTreeView::onEditItem(int elementId, ElementType elementType)
{
    Storage *storage = Storage::getInstance();

    if (elementType == ToolType) {
        ToolEditor editor;
        for (const Tool &tool : storage->getTools()) {
            if (tool.getId() == elementId) {
                editor.setTool(tool);
                if (editor.exec() == QDialog::Accepted) {
                    Tool updatedTool = editor.getTool();
                    StorageEditor storageEditor(storage);
                    storageEditor.insertOrReplace(storage->getTools(), updatedTool);
                    buildTree();
                }
                break;
            }
        }
    } else if (elementType == SensorType) {
        SensorEditor editor;
        for (const Sensor &sensor : storage->getSensors()) {
            if (sensor.getId() == elementId) {
                editor.setSensor(sensor);
                if (editor.exec() == QDialog::Accepted) {
                    Sensor updatedSensor = editor.getSensor();
                    StorageEditor storageEditor(storage);
                    storageEditor.insertOrReplace(storage->getSensors(), updatedSensor);
                    buildTree();
                }
                break;
            }
        }
    } else if (elementType == MainMnemonicType) {
        for (const MainMnemonic &mnemonic : storage->getMainMnemonics()) {
            if (mnemonic.getId() == elementId) {
                MainMnemonicEditor editor;
                editor.setMainMnemonic(mnemonic);
                if (editor.exec() == QDialog::Accepted) {
                    MainMnemonic updatedMnemonic = editor.getMainMnemonic();
                    StorageEditor storageEditor(storage);
                    storageEditor.insertOrReplace(storage->getMainMnemonics(), updatedMnemonic);
                    buildTree();
                }
                break;
            }
        }
    } else if (elementType == AdditionalMnemonicType) {
        for (const AdditionalMnemonic &mnemonic : storage->getAdditionalMnemonics()) {
            if (mnemonic.getId() == elementId) {
                AdditionalMnemonicEditor editor;
                editor.setAdditionalMnemonic(mnemonic);
                if (editor.exec() == QDialog::Accepted) {
                    AdditionalMnemonic updatedMnemonic = editor.getAdditionalMnemonic();
                    StorageEditor storageEditor(storage);
                    storageEditor.insertOrReplace(storage->getAdditionalMnemonics(),
                                                  updatedMnemonic);
                    buildTree();
                }
                break;
            }
        }
    }
}

void ToolSensorMnemonicTreeView::onDeleteItem(int elementId, ElementType elementType)
{
    Storage *storage = Storage::getInstance();
    StorageEditor storageEditor(storage);

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "Confirm Delete",
                                  "Are you sure you want to delete this item?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {
        return;
    }

    if (elementType == ToolType) {
        // Удаление инструмента
        for (Tool &tool : storage->getTools()) {
            if (abs(tool.getId()) == elementId) {
                tool.setId(-abs(tool.getId())); // Отмечаем инструмент как удалённый
                break;
            }
        }
        // Также нужно удалить связанные ToolDescriptions
        for (ToolDescription &desc : storage->getToolDescriptions()) {
            if (abs(desc.getId()) == elementId) {
                desc.setId(-abs(desc.getId())); // Отмечаем описание как удалённое
                break;
            }
        }
        // Удаляем все связи ToolSensor и ToolMnemonic
        for (ToolSensor &toolSensor : storage->getToolSensors()) {
            if (abs(toolSensor.getToolId()) == elementId) {
                toolSensor.setId(-abs(toolSensor.getId()));
            }
        }
        for (ToolMnemonic &toolMnemonic : storage->getToolMnemonics()) {
            if (abs(toolMnemonic.getToolId()) == elementId) {
                toolMnemonic.setId(-abs(toolMnemonic.getId()));
            }
        }
    } else if (elementType == SensorType) {
        // Удаление сенсора
        for (Sensor &sensor : storage->getSensors()) {
            if (abs(sensor.getId()) == elementId) {
                sensor.setId(-abs(sensor.getId())); // Отмечаем сенсор как удалённый
                break;
            }
        }
        // Удаляем все связи ToolSensor и SensorMnemonic
        for (ToolSensor &toolSensor : storage->getToolSensors()) {
            if (abs(toolSensor.getSensorId()) == elementId) {
                toolSensor.setId(-abs(toolSensor.getId()));
            }
        }
        for (SensorMnemonic &sensorMnemonic : storage->getSensorMnemonics()) {
            if (abs(sensorMnemonic.getSensorId()) == elementId) {
                sensorMnemonic.setId(-abs(sensorMnemonic.getId()));
            }
        }
    } else if (elementType == MainMnemonicType) {
        // Удаление основной мнемоники
        for (MainMnemonic &mnemonic : storage->getMainMnemonics()) {
            if (abs(mnemonic.getId()) == elementId) {
                mnemonic.setId(-abs(mnemonic.getId())); // Отмечаем мнемонику как удалённую
                break;
            }
        }
        // Удаляем связанные AdditionalMnemonics
        for (AdditionalMnemonic &additionalMnemonic : storage->getAdditionalMnemonics()) {
            if (abs(additionalMnemonic.getMainMnemonicId()) == elementId) {
                additionalMnemonic.setId(-abs(additionalMnemonic.getId()));
            }
        }
        // Удаляем все связи ToolMnemonic и SensorMnemonic
        for (ToolMnemonic &toolMnemonic : storage->getToolMnemonics()) {
            if (abs(toolMnemonic.getMnemonicId()) == elementId) {
                toolMnemonic.setId(-abs(toolMnemonic.getId()));
            }
        }
        for (SensorMnemonic &sensorMnemonic : storage->getSensorMnemonics()) {
            if (abs(sensorMnemonic.getMnemonicId()) == elementId) {
                sensorMnemonic.setId(-abs(sensorMnemonic.getId()));
            }
        }
    } else if (elementType == AdditionalMnemonicType) {
        // Удаление дополнительной мнемоники
        for (AdditionalMnemonic &additionalMnemonic : storage->getAdditionalMnemonics()) {
            if (abs(additionalMnemonic.getId()) == elementId) {
                additionalMnemonic.setId(-abs(additionalMnemonic.getId()));
                break;
            }
        }
    }

    buildTree(); // Обновляем дерево после удаления
}

void ToolSensorMnemonicTreeView::onToolSensorRelations(int toolId)
{
    ToolSensorRelationEditor *relationEditor = new ToolSensorRelationEditor(this);
    relationEditor->setTool(toolId);
    connect(relationEditor, &QDialog::finished, this, [=]() {
        buildTree(); // Перестраиваем дерево после закрытия редактора
    });
    relationEditor->exec();
}

void ToolSensorMnemonicTreeView::onToolMnemonicRelations(int toolId)
{
    ToolMnemonicRelationEditor *relationEditor = new ToolMnemonicRelationEditor(this);
    relationEditor->setTool(toolId);
    connect(relationEditor, &QDialog::finished, this, [=]() {
        buildTree(); // Перестраиваем дерево после закрытия редактора
    });
    relationEditor->exec();
}

void ToolSensorMnemonicTreeView::onSensorMnemonicRelations(int sensorId)
{
    SensorMnemonicRelationEditor *relationEditor = new SensorMnemonicRelationEditor(this);
    relationEditor->setSensor(sensorId);
    connect(relationEditor, &QDialog::finished, this, [=]() {
        buildTree(); // Перестраиваем дерево после закрытия редактора
    });
    relationEditor->exec();
}
void ToolSensorMnemonicTreeView::onAddItem(ElementType elementType)
{
    Storage *storage = Storage::getInstance();
    StorageEditor storageEditor(storage);

    if (elementType == ToolType) {
        // Добавление нового инструмента
        ToolAddWindow addWindow;

        int newToolId = storage->generateNewToolId();
        int newToolDescriptionId = storage->generateNewToolDescriptionId();

        addWindow.setNewToolId(newToolId);
        addWindow.setNewToolDescriptionId(newToolDescriptionId);

        if (addWindow.exec() == QDialog::Accepted) {
            Tool newTool = addWindow.getNewTool();
            ToolDescription newToolDescription = addWindow.getNewToolDescription();

            storageEditor.insertOrReplace(storage->getTools(), newTool);
            storageEditor.insertOrReplace(storage->getToolDescriptions(), newToolDescription);

            buildTree();
        } else {
            // Если пользователь отменил, уменьшаем счётчики ID
            storage->setMaxToolId(storage->getMaxToolId() - 1);
            storage->setMaxToolDescriptionId(storage->getMaxToolDescriptionId() - 1);
        }
    } else if (elementType == SensorType) {
        // Добавление нового сенсора
        SensorAddWindow addWindow;

        int newSensorId = storage->generateNewSensorId();
        addWindow.setNewSensorId(newSensorId);

        if (addWindow.exec() == QDialog::Accepted) {
            Sensor newSensor = addWindow.getNewSensor();

            storageEditor.insertOrReplace(storage->getSensors(), newSensor);

            buildTree();
        } else {
            storage->setMaxSensorId(storage->getMaxSensorId() - 1);
        }
    } else if (elementType == MainMnemonicType) {
        // Добавление новой основной мнемоники
        MainMnemonicAddWindow addWindow;

        int newMainMnemonicId = storage->generateNewMainMnemonicId();
        addWindow.setNewMainMnemonicId(newMainMnemonicId);

        if (addWindow.exec() == QDialog::Accepted) {
            MainMnemonic newMainMnemonic = addWindow.getNewMainMnemonic();

            storageEditor.insertOrReplace(storage->getMainMnemonics(), newMainMnemonic);

            buildTree();
        } else {
            storage->setMaxMainMnemonicId(storage->getMaxMainMnemonicId() - 1);
        }
    } else if (elementType == AdditionalMnemonicType) {
        // Добавление новой дополнительной мнемоники
        AdditionalMnemonicAddWindow addWindow;

        int newAdditionalMnemonicId = storage->generateNewAdditionalMnemonicId();
        addWindow.setNewAdditionalMnemonicId(newAdditionalMnemonicId);

        if (addWindow.exec() == QDialog::Accepted) {
            AdditionalMnemonic newAdditionalMnemonic = addWindow.getNewAdditionalMnemonic();

            storageEditor.insertOrReplace(storage->getAdditionalMnemonics(), newAdditionalMnemonic);

            buildTree();
        } else {
            storage->setMaxAdditionalMnemonicId(storage->getMaxAdditionalMnemonicId() - 1);
        }
    }
}
