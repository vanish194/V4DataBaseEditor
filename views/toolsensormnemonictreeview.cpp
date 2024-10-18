#include "toolsensormnemonictreeview.h"
#include <QAction>
#include <QContextMenuEvent>
#include <QDebug>
#include <QHeaderView>
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

        // Встроенные мнемоники для прибора (если есть)
        QStandardItem *embeddedMnemonicsItem = new QStandardItem("Embedded Mnemonics");
        bool hasEmbeddedMnemonics = false;

        for (const ToolMnemonic &toolMnemonic : storage->getToolMnemonics()) {
            if (toolMnemonic.getToolId() == tool.getId()) {
                const MainMnemonic *mnemonic = storage->findMainMnemonicById(
                    toolMnemonic.getMnemonicId());
                if (mnemonic) {
                    QStandardItem *mnemonicItem = new QStandardItem(mnemonic->getName());
                    mnemonicItem->setData(QVariant(mnemonic->getId()),
                                          Qt::UserRole + 1); // ID мнемоники
                    mnemonicItem->setData(QVariant(MainMnemonicType),
                                          Qt::UserRole + 2); // Тип элемента
                    embeddedMnemonicsItem->appendRow(mnemonicItem);
                    hasEmbeddedMnemonics = true;
                }
            }
        }

        // Если есть встроенные мнемоники, добавляем ветку "Embedded Mnemonics"
        if (hasEmbeddedMnemonics) {
            toolItem->appendRow(embeddedMnemonicsItem);
        }

        // Получение всех сенсоров для данного инструмента
        QList<ToolSensor> toolSensors;
        for (const ToolSensor &toolSensor : storage->getToolSensors()) {
            if (toolSensor.getToolId() == tool.getId()) {
                toolSensors.append(toolSensor);
            }
        }

        // Если нет сенсоров, добавляем "N/A"
        if (toolSensors.isEmpty()) {
            QStandardItem *naItem = new QStandardItem("N/A");
            toolItem->appendRow(naItem);
        }

        for (const ToolSensor &toolSensor : toolSensors) {
            const Sensor *sensor = findSensorById(toolSensor.getSensorId());
            if (sensor) {
                QStandardItem *sensorItem = new QStandardItem(sensor->getName());
                sensorItem->setData(QVariant(sensor->getId()),
                                    Qt::UserRole + 1); // Сохраняем ID сенсора
                sensorItem->setData(QVariant(SensorType),
                                    Qt::UserRole + 2); // Сохраняем тип элемента

                // Мнемоники, связанные с этим сенсором (через SensorMnemonic)
                QList<MainMnemonic> sensorMnemonics;
                for (const SensorMnemonic &sensorMnemonic : storage->getSensorMnemonics()) {
                    if (sensorMnemonic.getSensorId() == sensor->getId()) {
                        const MainMnemonic *mnemonic = storage->findMainMnemonicById(
                            sensorMnemonic.getMnemonicId());
                        if (mnemonic) {
                            sensorMnemonics.append(*mnemonic);
                        }
                    }
                }

                // Если нет связанных мнемоник, добавляем "N/A"
                if (sensorMnemonics.isEmpty()) {
                    QStandardItem *naItem = new QStandardItem("N/A");
                    sensorItem->appendRow(naItem);
                }

                for (const MainMnemonic &mnemonic : sensorMnemonics) {
                    QStandardItem *mnemonicItem = new QStandardItem(mnemonic.getName());
                    mnemonicItem->setData(QVariant(mnemonic.getId()),
                                          Qt::UserRole + 1); // ID мнемоники
                    mnemonicItem->setData(QVariant(MainMnemonicType),
                                          Qt::UserRole + 2); // Тип элемента
                    sensorItem->appendRow(mnemonicItem);
                }

                toolItem->appendRow(sensorItem);
            } else {
                // Если сенсор не найден, добавляем "N/A"
                QStandardItem *naItem = new QStandardItem("N/A");
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
        // Skip deleted sensors
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

    if (!(elementType == ToolType || elementType == SensorType || elementType == MainMnemonicType
          || elementType == AdditionalMnemonicType)) {
        return; // Just exit the method, context menu will not be shown
    }

    if (elementId == 0) {
        qDebug() << "Invalid selection: N/A item chosen";
        return;
    }

    QMenu contextMenu(this);
    QAction *addAction = contextMenu.addAction("Add");
    QAction *editAction = contextMenu.addAction("Edit");
    QAction *deleteAction = contextMenu.addAction("Delete");

    // "Relation Editor" menu item for tools and sensors
    if (elementType == ToolType || elementType == SensorType) {
        QAction *relationEditorAction = contextMenu.addAction("Relation Editor");
        connect(relationEditorAction, &QAction::triggered, this, [=]() {
            onEditRelations(elementId, elementType);
        });
    }

    connect(editAction, &QAction::triggered, this, [=]() { onEditItem(elementId, elementType); });
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
    }
}

void ToolSensorMnemonicTreeView::onEditRelations(int elementId, ElementType elementType)
{
    if (elementType == ToolType) {
        ToolSensorRelationEditor *relationEditor = new ToolSensorRelationEditor(this);
        // Open the relation editor for the tool
        relationEditor->setTool(elementId);

        // Connect the finished signal to rebuild the tree
        connect(relationEditor, &QDialog::finished, this, [=]() {
            buildTree(); // Rebuild the tree after closing the editor
        });

        relationEditor->exec(); // Open the editor window
    } else {
        // Relations can only be edited from tools
        QMessageBox::information(this,
                                 "Relation Editor",
                                 "Relations can only be edited from tools.");
    }
}
