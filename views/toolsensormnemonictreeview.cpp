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
    setHeaderHidden(false); // Показывать заголовок
    model->setHorizontalHeaderLabels(QStringList() << "Tool -> Sensor -> Mnemonics");
    setModel(model);
    buildTree();

    // Расширить все узлы по умолчанию
    expandAll();

    // Настроить заголовок для растягивания
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

        // **Добавляем описание инструмента**
        int toolDescriptionId = tool.getToolDescriptionId();
        const ToolDescription *toolDescription = nullptr;

        for (const ToolDescription &desc : storage->getToolDescriptions()) {
            if (desc.getId() == toolDescriptionId) {
                toolDescription = &desc;
                break;
            }
        }

        if (toolDescription) {
            QString descriptionText = "Description: " + toolDescription->getDescription();
            QStandardItem *toolDescriptionItem = new QStandardItem(descriptionText);
            toolDescriptionItem->setData(QVariant(toolDescription->getId()), Qt::UserRole + 1);
            toolDescriptionItem->setData(QVariant(ToolDescriptionType), Qt::UserRole + 2);

            // Добавляем описание как дочерний элемент инструмента
            toolItem->appendRow(toolDescriptionItem);
        } else {
            // Если описание не найдено, добавляем затычку
            QStandardItem *naItem = new QStandardItem("Description: N/A");
            toolItem->appendRow(naItem);
        }

        // Далее строим дерево сенсоров и мнемоник как раньше

        // Получаем все ToolSensor для данного инструмента
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

        // Если нет связанных сенсоров, добавляем затычку "N/A"
        if (toolSensors.isEmpty()) {
            QStandardItem *naItem = new QStandardItem("N/A");
            toolItem->appendRow(naItem);
        }

        for (const ToolSensor &toolSensor : toolSensors) {
            const Sensor *sensor = findSensorById(toolSensor.getSensorId());
            if (sensor) {
                // Пропускаем удалённые сенсоры
                if (sensor->getId() < 0) {
                    continue;
                }
                QStandardItem *sensorItem = new QStandardItem(sensor->getName());
                sensorItem->setData(QVariant(sensor->getId()),
                                    Qt::UserRole + 1); // Сохраняем ID сенсора
                sensorItem->setData(QVariant(SensorType),
                                    Qt::UserRole + 2); // Сохраняем тип элемента

                // **Добавляем описание сенсора как дочерний элемент**
                QStandardItem *sensorDescriptionItem = new QStandardItem(
                    "Description: " + sensor->getDescription());
                sensorDescriptionItem->setData(QVariant(sensor->getId()), Qt::UserRole + 1);
                sensorDescriptionItem->setData(QVariant(SensorDescriptionType), Qt::UserRole + 2);

                // Добавляем описание под сенсором
                sensorItem->appendRow(sensorDescriptionItem);

                // Получаем все MainMnemonic для данного сенсора
                QList<MainMnemonic> mainMnemonics;
                for (const MainMnemonic &mainMnemonic : storage->getMainMnemonics()) {
                    // Пропускаем удалённые мнемоники
                    if (mainMnemonic.getId() < 0) {
                        continue;
                    }
                    if (mainMnemonic.getSensorId() == sensor->getId()) {
                        mainMnemonics.append(mainMnemonic);
                    }
                }

                // Если нет связанных MainMnemonic, добавляем затычку "N/A"
                if (mainMnemonics.isEmpty()) {
                    QStandardItem *naItem = new QStandardItem("N/A");
                    sensorItem->appendRow(naItem);
                }

                for (const MainMnemonic &mainMnemonic : mainMnemonics) {
                    QStandardItem *mainMnemonicItem = new QStandardItem(mainMnemonic.getName());
                    mainMnemonicItem->setData(QVariant(mainMnemonic.getId()),
                                              Qt::UserRole + 1); // ID main mnemonic
                    mainMnemonicItem->setData(QVariant(MainMnemonicType),
                                              Qt::UserRole + 2); // Тип элемента

                    // Получаем все AdditionalMnemonic для данного main mnemonic
                    QList<AdditionalMnemonic> additionalMnemonics;
                    for (const AdditionalMnemonic &additionalMnemonic :
                         storage->getAdditionalMnemonics()) {
                        // Пропускаем удалённые дополнительные мнемоники
                        if (additionalMnemonic.getId() < 0) {
                            continue;
                        }
                        if (additionalMnemonic.getMainMnemonicId() == mainMnemonic.getId()) {
                            additionalMnemonics.append(additionalMnemonic);
                        }
                    }

                    // Если нет связанных AdditionalMnemonic, добавляем затычку "N/A"
                    if (additionalMnemonics.isEmpty()) {
                        QStandardItem *naItem = new QStandardItem("N/A");
                        mainMnemonicItem->appendRow(naItem);
                    }

                    for (const AdditionalMnemonic &additionalMnemonic : additionalMnemonics) {
                        QStandardItem *additionalMnemonicItem = new QStandardItem(
                            additionalMnemonic.getName());
                        additionalMnemonicItem->setData(QVariant(additionalMnemonic.getId()),
                                                        Qt::UserRole + 1); // ID additional mnemonic
                        additionalMnemonicItem->setData(QVariant(AdditionalMnemonicType),
                                                        Qt::UserRole + 2); // Тип элемента
                        mainMnemonicItem->appendRow(additionalMnemonicItem);
                    }

                    sensorItem->appendRow(mainMnemonicItem);
                }

                toolItem->appendRow(sensorItem);
            } else {
                // Если сенсор не найден, добавляем затычку "N/A"
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

    if (!(elementType == ToolType || elementType == SensorType || elementType == MainMnemonicType
          || elementType == AdditionalMnemonicType)) {
        return; // Просто выходим из метода, контекстное меню не будет показано
    }

    if (elementId == 0) {
        qDebug() << "Invalid selection: N/A item chosen";
        return;
    }

    QMenu contextMenu(this);
    QAction *addAction = contextMenu.addAction("Add");
    QAction *editAction = contextMenu.addAction("Edit");
    QAction *deleteAction = contextMenu.addAction("Delete");

    //пункт меню "Relation Editor" для инструментов и сенсоров
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
