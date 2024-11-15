#include "toolsensorrelationeditor.h"
#include "ui_toolsensorrelationeditor.h"

#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSet>
#include <QTableWidget>
#include <QVBoxLayout>

ToolSensorRelationEditor::ToolSensorRelationEditor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ToolSensorRelationEditor)
    , storage(Storage::getInstance())
    , selectedToolId(-1) // Initialization
{
    ui->setupUi(this);

    loadTools();
    loadSensors();

    // Connect signal for updating selection
    connect(ui->treeWidgetTools,
            &QTreeWidget::itemSelectionChanged,
            this,
            &ToolSensorRelationEditor::onToolSelectionChanged);

    // Set up context menu for tools
    ui->treeWidgetTools->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidgetTools,
            &QTreeWidget::customContextMenuRequested,
            this,
            &ToolSensorRelationEditor::showToolContextMenu);

    // Disable context menu for sensors
    ui->treeWidgetSensors->setContextMenuPolicy(Qt::NoContextMenu);
}

ToolSensorRelationEditor::~ToolSensorRelationEditor()
{
    delete ui;
}

// Method to set the selected tool
void ToolSensorRelationEditor::setTool(int toolId)
{
    selectedToolId = toolId; // Store the tool ID

    // Select the tool in treeWidgetTools
    for (int i = 0; i < ui->treeWidgetTools->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->treeWidgetTools->topLevelItem(i);
        if (item->data(0, Qt::UserRole).toInt() == toolId) {
            ui->treeWidgetTools->setCurrentItem(item);
            break;
        }
    }

    updateSensorSelectionForTool(toolId);
}

void ToolSensorRelationEditor::loadTools()
{
    ui->treeWidgetTools->clear();
    ui->treeWidgetTools->setHeaderLabels(QStringList() << tr("Tool"));

    // Получаем список инструментов
    QList<Tool> tools = storage->getTools();

    // Сортируем по имени инструмента
    std::sort(tools.begin(), tools.end(), [](const Tool &a, const Tool &b) {
        return a.getName() < b.getName();
    });

    // Добавляем элементы в виджет
    for (const Tool &tool : tools) {
        if (tool.getId() < 0)
            continue; // Пропускаем удалённые инструменты

        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidgetTools);
        item->setText(0, tool.getName());
        item->setData(0, Qt::UserRole, tool.getId());
    }
}

void ToolSensorRelationEditor::loadSensors()
{
    ui->treeWidgetSensors->clear();
    ui->treeWidgetSensors->setHeaderLabels(QStringList() << tr("Sensor") << tr("Offset (mm)"));

    // Получаем список сенсоров
    QList<Sensor> sensors = storage->getSensors();

    // Сортируем по имени сенсора
    std::sort(sensors.begin(), sensors.end(), [](const Sensor &a, const Sensor &b) {
        return a.getName() < b.getName();
    });

    // Добавляем элементы в виджет
    for (const Sensor &sensor : sensors) {
        if (sensor.getId() < 0)
            continue; // Пропускаем удалённые сенсоры

        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidgetSensors);
        item->setText(0, sensor.getName());
        item->setData(0, Qt::UserRole, sensor.getId());
    }
}

void ToolSensorRelationEditor::onToolSelectionChanged()
{
    QList<QTreeWidgetItem *> selectedItems = ui->treeWidgetTools->selectedItems();
    if (selectedItems.isEmpty()) {
        ui->treeWidgetSensors->clear();
        return;
    }

    int toolId = selectedItems.first()->data(0, Qt::UserRole).toInt();
    selectedToolId = toolId;
    updateSensorSelectionForTool(toolId);
}

void ToolSensorRelationEditor::updateSensorSelectionForTool(int toolId)
{
    ui->treeWidgetSensors->clear();

    // Получаем все сенсоры, связанные с выбранным инструментом
    QList<const ToolSensor *> relatedToolSensors;
    for (const ToolSensor &toolSensor : storage->getToolSensors()) {
        // Пропускаем удалённые связи
        if (toolSensor.getId() < 0) {
            continue;
        }
        if (toolSensor.getToolId() == toolId) {
            relatedToolSensors.append(&toolSensor);
        }
    }

    for (const ToolSensor *toolSensor : relatedToolSensors) {
        const Sensor *sensor = findSensorById(toolSensor->getSensorId());
        if (sensor) {
            // Пропускаем удалённые сенсоры
            if (sensor->getId() < 0) {
                continue;
            }
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidgetSensors);
            item->setText(0, sensor->getName());
            item->setText(1, QString::number(toolSensor->getOffsetMm()));
            item->setData(0, Qt::UserRole, sensor->getId());
        }
    }
}

const ToolSensor *ToolSensorRelationEditor::findToolSensor(int toolId, int sensorId) const
{
    for (const ToolSensor &toolSensor : storage->getToolSensors()) {
        if (toolSensor.getToolId() == toolId && toolSensor.getSensorId() == sensorId) {
            return &toolSensor;
        }
    }
    return nullptr;
}

const Sensor *ToolSensorRelationEditor::findSensorById(int sensorId) const
{
    for (const Sensor &sensor : storage->getSensors()) {
        if (sensor.getId() == sensorId) {
            return &sensor;
        }
    }
    return nullptr;
}

const Tool *ToolSensorRelationEditor::findToolById(int toolId) const
{
    for (const Tool &tool : storage->getTools()) {
        if (tool.getId() == toolId) {
            return &tool;
        }
    }
    return nullptr;
}

void ToolSensorRelationEditor::showToolContextMenu(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->treeWidgetTools->itemAt(pos);
    if (!item)
        return;

    QMenu contextMenu;
    QAction *addAction = contextMenu.addAction(tr("Add Sensor"));
    QAction *removeAction = contextMenu.addAction(tr("Remove Sensor"));
    QAction *editOffsetAction = contextMenu.addAction(tr("Edit Offset"));

    connect(addAction, &QAction::triggered, this, &ToolSensorRelationEditor::addRelationFromTool);
    connect(removeAction,
            &QAction::triggered,
            this,
            &ToolSensorRelationEditor::removeRelationFromTool);
    connect(editOffsetAction,
            &QAction::triggered,
            this,
            &ToolSensorRelationEditor::editOffsetFromTool);

    contextMenu.exec(ui->treeWidgetTools->mapToGlobal(pos));
}

void ToolSensorRelationEditor::addRelationFromTool()
{
    QList<QTreeWidgetItem *> selectedTools = ui->treeWidgetTools->selectedItems();
    if (selectedTools.isEmpty())
        return;

    int toolId = selectedTools.first()->data(0, Qt::UserRole).toInt();

    // Get sensors not related to the selected tool
    QSet<int> relatedSensorIds;
    for (const ToolSensor &toolSensor : storage->getToolSensors()) {
        if (toolSensor.getToolId() == toolId) {
            relatedSensorIds.insert(toolSensor.getSensorId());
        }
    }

    QList<const Sensor *> availableSensors;
    for (const Sensor &sensor : storage->getSensors()) {
        if (!relatedSensorIds.contains(sensor.getId())) {
            availableSensors.append(&sensor);
        }
    }

    if (availableSensors.isEmpty()) {
        QMessageBox::information(this, tr("Add Sensor"), tr("No available sensors to add."));
        return;
    }

    // Show dialog to select sensors and specify offsetMm
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Add Sensors"));
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *label = new QLabel(tr("Select Sensors and specify Offset (mm):"), &dialog);
    layout->addWidget(label);

    QTableWidget *tableWidget = new QTableWidget(&dialog);
    tableWidget->setColumnCount(2);
    tableWidget->setHorizontalHeaderLabels(QStringList() << tr("Sensor") << tr("Offset (mm)"));
    tableWidget->setRowCount(availableSensors.size());
    tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    for (int i = 0; i < availableSensors.size(); ++i) {
        const Sensor *sensor = availableSensors.at(i);
        QTableWidgetItem *itemSensor = new QTableWidgetItem(sensor->getName());
        itemSensor->setData(Qt::UserRole, sensor->getId());
        tableWidget->setItem(i, 0, itemSensor);

        QTableWidgetItem *itemOffset = new QTableWidgetItem("0");
        tableWidget->setItem(i, 1, itemOffset);
    }
    layout->addWidget(tableWidget);
    tableWidget->sortItems(Qt::AscendingOrder);

    QPushButton *okButton = new QPushButton(tr("OK"), &dialog);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"), &dialog);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        StorageEditor storageEditor(storage);
        for (int i = 0; i < tableWidget->rowCount(); ++i) {
            QTableWidgetItem *itemSensor = tableWidget->item(i, 0);
            if (itemSensor->isSelected()) {
                int sensorId = itemSensor->data(Qt::UserRole).toInt();
                QTableWidgetItem *itemOffset = tableWidget->item(i, 1);
                int offsetMm = itemOffset->text().toInt();

                int newId = storage->generateNewToolSensorId();
                ToolSensor newRelation(newId, toolId, sensorId, offsetMm);
                storageEditor.insertOrReplace(storage->getToolSensors(), newRelation);
            }
        }
        refreshRelations();
    }
}

void ToolSensorRelationEditor::removeRelationFromTool()
{
    QList<QTreeWidgetItem *> selectedTools = ui->treeWidgetTools->selectedItems();
    if (selectedTools.isEmpty())
        return;

    int toolId = selectedTools.first()->data(0, Qt::UserRole).toInt();

    QList<const ToolSensor *> relatedToolSensors;
    for (const ToolSensor &toolSensor : storage->getToolSensors()) {
        // Пропускаем удалённые связи
        if (toolSensor.getId() < 0) {
            continue;
        }
        if (toolSensor.getToolId() == toolId) {
            relatedToolSensors.append(&toolSensor);
        }
    }

    if (relatedToolSensors.isEmpty()) {
        QMessageBox::information(this, tr("Remove Sensor"), tr("No sensors to remove."));
        return;
    }

    // Показываем диалог для выбора сенсоров для удаления
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Remove Sensors"));
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *label = new QLabel(tr("Select Sensors to remove:"), &dialog);
    layout->addWidget(label);

    QListWidget *listWidget = new QListWidget(&dialog);
    listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    for (const ToolSensor *toolSensor : relatedToolSensors) {
        const Sensor *sensor = findSensorById(toolSensor->getSensorId());
        if (sensor) {
            // Пропускаем удалённые сенсоры
            if (sensor->getId() < 0) {
                continue;
            }
            QListWidgetItem *item = new QListWidgetItem(sensor->getName());
            item->setData(Qt::UserRole, toolSensor->getId());
            listWidget->addItem(item);
        }
    }
    layout->addWidget(listWidget);

    QPushButton *okButton = new QPushButton(tr("OK"), &dialog);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"), &dialog);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        StorageEditor storageEditor(storage);
        QList<QListWidgetItem *> selectedItems = listWidget->selectedItems();
        for (QListWidgetItem *item : selectedItems) {
            int toolSensorId = item->data(Qt::UserRole).toInt();
            for (ToolSensor &toolSensor : storage->getToolSensors()) {
                if (toolSensor.getId() == toolSensorId) {
                    // Помечаем связь как удалённую, установив отрицательный ID
                    toolSensor.setId(-abs(toolSensor.getId()));
                    storageEditor.insertOrReplace(storage->getToolSensors(), toolSensor);
                    break;
                }
            }
        }
        refreshRelations();
    }
}

void ToolSensorRelationEditor::editOffsetFromTool()
{
    QList<QTreeWidgetItem *> selectedTools = ui->treeWidgetTools->selectedItems();
    if (selectedTools.isEmpty()) {
        QMessageBox::warning(this, tr("Edit Offset"), tr("No tool selected."));
        return;
    }

    int toolId = selectedTools.first()->data(0, Qt::UserRole).toInt();

    // Get all relations for this tool
    QList<const ToolSensor *> relatedToolSensors;
    for (const ToolSensor &toolSensor : storage->getToolSensors()) {
        if (toolSensor.getToolId() == toolId) {
            relatedToolSensors.append(&toolSensor);
        }
    }

    if (relatedToolSensors.isEmpty()) {
        QMessageBox::information(this,
                                 tr("Edit Offset_"),
                                 tr("No sensors related to the selected tool."));
        return;
    }

    // Create dialog to select relation
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Select Sensor to Edit Offset"));

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *label = new QLabel(tr("Select a sensor to edit the offset:"), &dialog);
    layout->addWidget(label);

    // Table to display relations
    QTableWidget *tableWidget = new QTableWidget(&dialog);
    tableWidget->setColumnCount(3);
    tableWidget->setHorizontalHeaderLabels(QStringList()
                                           << tr("Tool") << tr("Sensor") << tr("Offset (mm)"));
    tableWidget->setRowCount(relatedToolSensors.size());
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // Fill table with data
    for (int i = 0; i < relatedToolSensors.size(); ++i) {
        const ToolSensor *toolSensor = relatedToolSensors[i];
        const Tool *tool = findToolById(toolSensor->getToolId());
        const Sensor *sensor = findSensorById(toolSensor->getSensorId());

        QTableWidgetItem *toolItem = new QTableWidgetItem(tool ? tool->getName() : tr("Unknown"));
        toolItem->setData(Qt::UserRole, toolSensor->getToolId());
        tableWidget->setItem(i, 0, toolItem);

        QTableWidgetItem *sensorItem = new QTableWidgetItem(sensor ? sensor->getName()
                                                                   : tr("Unknown"));
        sensorItem->setData(Qt::UserRole, toolSensor->getSensorId());
        tableWidget->setItem(i, 1, sensorItem);

        QTableWidgetItem *offsetItem = new QTableWidgetItem(
            QString::number(toolSensor->getOffsetMm()));
        tableWidget->setItem(i, 2, offsetItem);
    }

    layout->addWidget(tableWidget);

    // OK and Cancel buttons
    QPushButton *okButton = new QPushButton(tr("OK"), &dialog);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"), &dialog);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        // Get selected item
        QModelIndexList selectedRows = tableWidget->selectionModel()->selectedRows();
        if (selectedRows.isEmpty()) {
            QMessageBox::warning(this, tr("Edit Offset"), tr("No sensor selected."));
            return;
        }

        int selectedRow = selectedRows.first().row();
        int sensorId = tableWidget->item(selectedRow, 1)->data(Qt::UserRole).toInt();

        ToolSensor *toolSensorPtr = nullptr;
        for (ToolSensor &toolSensor : storage->getToolSensors()) {
            if (toolSensor.getToolId() == toolId && toolSensor.getSensorId() == sensorId) {
                toolSensorPtr = &toolSensor;
                break;
            }
        }

        if (!toolSensorPtr) {
            QMessageBox::warning(this,
                                 tr("Edit Offset"),
                                 tr("No relation found for the selected Tool and Sensor."));
            return;
        }

        // Show dialog to input new offset value
        bool ok;
        int newOffsetMm = QInputDialog::getInt(this,
                                               tr("Edit Offset"),
                                               tr("Enter new Offset (mm):"),
                                               toolSensorPtr->getOffsetMm(),
                                               0,
                                               1000000,
                                               1,
                                               &ok);
        if (ok) {
            toolSensorPtr->setOffsetMm(newOffsetMm);

            // Save changes to storage
            StorageEditor storageEditor(storage);
            storageEditor.insertOrReplace(storage->getToolSensors(), *toolSensorPtr);

            // Update UI
            refreshRelations();
        }
    }
}

void ToolSensorRelationEditor::updateRelation(int toolId, int sensorId, int offsetMm)
{
    // Find existing relation between tool and sensor
    ToolSensor *existingRelation = nullptr;
    for (ToolSensor &toolSensor : storage->getToolSensors()) {
        if (toolSensor.getToolId() == toolId && toolSensor.getSensorId() == sensorId) {
            existingRelation = &toolSensor;
            break;
        }
    }

    StorageEditor storageEditor(storage);

    // If relation found, update offsetMm
    if (existingRelation) {
        existingRelation->setOffsetMm(offsetMm);
        storageEditor.insertOrReplace(storage->getToolSensors(), *existingRelation);
    } else {
        // If relation not found, create new one
        int newId = storage->generateNewToolSensorId();
        ToolSensor newRelation(newId, toolId, sensorId, offsetMm);
        storageEditor.insertOrReplace(storage->getToolSensors(), newRelation);
    }

    // Refresh relations display
    refreshRelations();
}

void ToolSensorRelationEditor::removeRelation(int toolId, int sensorId)
{
    // Find relation to remove
    for (auto it = storage->getToolSensors().begin(); it != storage->getToolSensors().end(); ++it) {
        if (it->getToolId() == toolId && it->getSensorId() == sensorId) {
            StorageEditor storageEditor(storage);
            storageEditor.deleteElement(storage->getToolSensors(), *it);
            break;
        }
    }

    // Refresh relations display
    refreshRelations();
}

void ToolSensorRelationEditor::refreshRelations()
{
    if (selectedToolId != -1) {
        updateSensorSelectionForTool(selectedToolId);
    }
}
