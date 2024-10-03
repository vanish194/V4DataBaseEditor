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
        // Skip deleted tools
        if (tool.getId() < 0) {
            continue;
        }
        QStandardItem *toolItem = new QStandardItem(tool.getName());
        toolItem->setData(QVariant(tool.getId()), Qt::UserRole + 1); // Save tool ID
        toolItem->setData(QVariant(ToolType), Qt::UserRole + 2);     // Save item type

        // Get all ToolSensor for this tool
        QList<ToolSensor> toolSensors;
        for (const ToolSensor &toolSensor : storage->getToolSensors()) {
            // Skip deleted relations
            if (toolSensor.getId() < 0) {
                continue;
            }
            if (toolSensor.getToolId() == tool.getId()) {
                toolSensors.append(toolSensor);
            }
        }

        // If no associated sensors, add a "N/A" placeholder
        if (toolSensors.isEmpty()) {
            QStandardItem *naItem = new QStandardItem("N/A");
            toolItem->appendRow(naItem);
        }

        for (const ToolSensor &toolSensor : toolSensors) {
            const Sensor *sensor = findSensorById(toolSensor.getSensorId());
            if (sensor) {
                // Skip deleted sensors
                if (sensor->getId() < 0) {
                    continue;
                }
                QStandardItem *sensorItem = new QStandardItem(sensor->getName());
                sensorItem->setData(QVariant(sensor->getId()),
                                    Qt::UserRole + 1); // Save sensor ID
                sensorItem->setData(QVariant(SensorType),
                                    Qt::UserRole + 2); // Save item type

                // Get all MainMnemonic for this sensor
                QList<MainMnemonic> mainMnemonics;
                for (const MainMnemonic &mainMnemonic : storage->getMainMnemonics()) {
                    // Skip deleted mnemonics
                    if (mainMnemonic.getId() < 0) {
                        continue;
                    }
                    if (mainMnemonic.getSensorId() == sensor->getId()) {
                        mainMnemonics.append(mainMnemonic);
                    }
                }

                // If no associated MainMnemonics, add a "N/A" placeholder
                if (mainMnemonics.isEmpty()) {
                    QStandardItem *naItem = new QStandardItem("N/A");
                    sensorItem->appendRow(naItem);
                }

                for (const MainMnemonic &mainMnemonic : mainMnemonics) {
                    QStandardItem *mainMnemonicItem = new QStandardItem(mainMnemonic.getName());
                    mainMnemonicItem->setData(QVariant(mainMnemonic.getId()),
                                              Qt::UserRole + 1); // Main mnemonic ID
                    mainMnemonicItem->setData(QVariant(MainMnemonicType),
                                              Qt::UserRole + 2); // Save item type

                    // Get all AdditionalMnemonic for this main mnemonic
                    QList<AdditionalMnemonic> additionalMnemonics;
                    for (const AdditionalMnemonic &additionalMnemonic :
                         storage->getAdditionalMnemonics()) {
                        // Skip deleted additional mnemonics
                        if (additionalMnemonic.getId() < 0) {
                            continue;
                        }
                        if (additionalMnemonic.getMainMnemonicId() == mainMnemonic.getId()) {
                            additionalMnemonics.append(additionalMnemonic);
                        }
                    }

                    // If no associated AdditionalMnemonics, add a "N/A" placeholder
                    if (additionalMnemonics.isEmpty()) {
                        QStandardItem *naItem = new QStandardItem("N/A");
                        mainMnemonicItem->appendRow(naItem);
                    }

                    for (const AdditionalMnemonic &additionalMnemonic : additionalMnemonics) {
                        QStandardItem *additionalMnemonicItem = new QStandardItem(
                            additionalMnemonic.getName());
                        additionalMnemonicItem->setData(QVariant(additionalMnemonic.getId()),
                                                        Qt::UserRole + 1); // Additional mnemonic ID
                        additionalMnemonicItem->setData(QVariant(AdditionalMnemonicType),
                                                        Qt::UserRole + 2); // Save item type
                        mainMnemonicItem->appendRow(additionalMnemonicItem);
                    }

                    sensorItem->appendRow(mainMnemonicItem);
                }

                toolItem->appendRow(sensorItem);
            } else {
                // If sensor is not found, add a "N/A" placeholder
                QStandardItem *naItem = new QStandardItem("N/A");
                toolItem->appendRow(naItem);
            }
        }

        model->appendRow(toolItem);
    }

    // Expand all nodes
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
