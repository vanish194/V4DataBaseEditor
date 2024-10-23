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
    if (!index.isValid())
        return;

    int elementId = index.data(Qt::UserRole + 1).toInt();
    ElementType elementType = static_cast<ElementType>(index.data(Qt::UserRole + 2).toInt());

    if (elementId == 0 || elementType == NoneType) {
        qDebug() << "Invalid selection: N/A or unsupported item chosen";
        return;
    }

    QMenu contextMenu(this);

    // Добавляем действие "Edit" для поддерживаемых типов
    if (elementType == ToolType || elementType == SensorType || elementType == MainMnemonicType
        || elementType == AdditionalMnemonicType) {
        QAction *editAction = contextMenu.addAction("Edit");
        connect(editAction, &QAction::triggered, this, [=]() {
            onEditItem(elementId, elementType);
        });
    }

    if (elementType == ToolType) {
        QAction *sensorRelationsAction = contextMenu.addAction("Tool Sensor Relations");
        QAction *mnemonicRelationsAction = contextMenu.addAction("Tool Mnemonic Relations");

        connect(sensorRelationsAction, &QAction::triggered, this, [=]() {
            onToolSensorRelations(elementId);
        });

        connect(mnemonicRelationsAction, &QAction::triggered, this, [=]() {
            onToolMnemonicRelations(elementId);
        });
    } else if (elementType == SensorType) {
        QAction *mnemonicRelationsAction = contextMenu.addAction("Sensor Mnemonic Relations");

        connect(mnemonicRelationsAction, &QAction::triggered, this, [=]() {
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
    } // else if (elementType == MainMnemonicType) {
      //MainMnemonicEditor editor;
      //for (const MainMnemonic &mnemonic : storage->getMainMnemonics()) {
      //    if (mnemonic.getId() == elementId) {
      //        editor.setMainMnemonic(mnemonic);
      //       if (editor.exec() == QDialog::Accepted) {
      //           MainMnemonic updatedMnemonic = editor.getMainMnemonic();
      //            StorageEditor storageEditor(storage);
      //           storageEditor.insertOrReplace(storage->getMainMnemonics(), updatedMnemonic);
      //           buildTree();
      //       }
      //        break;
      //  }
      // }

    //else if (elementType == AdditionalMnemonicType)
    //{
    // AdditionalMnemonicEditor editor;
    // for (const AdditionalMnemonic &mnemonic : storage->getAdditionalMnemonics()) {
    //  if (mnemonic.getId() == elementId) {
    //     editor.setAdditionalMnemonic(mnemonic);
    //    if (editor.exec() == QDialog::Accepted) {
    //       AdditionalMnemonic updatedMnemonic = editor.getAdditionalMnemonic();
    //       StorageEditor storageEditor(storage);
    //      storageEditor.insertOrReplace(storage->getAdditionalMnemonics(), updatedMnemonic);
    //      buildTree();
    //  }
    //    break;
    // }
    //}
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
