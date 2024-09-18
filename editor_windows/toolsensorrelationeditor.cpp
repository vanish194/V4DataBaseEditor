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
    , selectedSensorId(-1)
{
    ui->setupUi(this);

    loadTools();
    loadSensors();

    // Connect signals for updating selection
    connect(ui->treeWidgetTools,
            &QTreeWidget::itemSelectionChanged,
            this,
            &ToolSensorRelationEditor::onToolSelectionChanged);
    connect(ui->treeWidgetSensors,
            &QTreeWidget::itemSelectionChanged,
            this,
            &ToolSensorRelationEditor::onSensorSelectionChanged);

    // Set up context menus
    ui->treeWidgetTools->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidgetTools,
            &QTreeWidget::customContextMenuRequested,
            this,
            &ToolSensorRelationEditor::showToolContextMenu);

    ui->treeWidgetSensors->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidgetSensors,
            &QTreeWidget::customContextMenuRequested,
            this,
            &ToolSensorRelationEditor::showSensorContextMenu);
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

// Method to set the selected sensor
void ToolSensorRelationEditor::setSensor(int sensorId)
{
    selectedSensorId = sensorId; // Store the sensor ID

    // Select the sensor in treeWidgetSensors
    for (int i = 0; i < ui->treeWidgetSensors->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->treeWidgetSensors->topLevelItem(i);
        if (item->data(0, Qt::UserRole).toInt() == sensorId) {
            ui->treeWidgetSensors->setCurrentItem(item);
            break;
        }
    }

    updateToolSelectionForSensor(sensorId);
}

void ToolSensorRelationEditor::loadTools()
{
    ui->treeWidgetTools->clear();
    ui->treeWidgetTools->setHeaderLabels(QStringList() << "Tool"
                                                       << "Offset (mm)");

    for (const Tool &tool : storage->getTools()) {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidgetTools);
        item->setText(0, tool.getName());
        item->setData(0, Qt::UserRole, tool.getId());
    }
}

void ToolSensorRelationEditor::loadSensors()
{
    ui->treeWidgetSensors->clear();
    ui->treeWidgetSensors->setHeaderLabels(QStringList() << "Sensor"
                                                         << "Offset (mm)");

    for (const Sensor &sensor : storage->getSensors()) {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidgetSensors);
        item->setText(0, sensor.getName());
        item->setData(0, Qt::UserRole, sensor.getId());
    }
}

void ToolSensorRelationEditor::onToolSelectionChanged()
{
    QList<QTreeWidgetItem *> selectedItems = ui->treeWidgetTools->selectedItems();
    if (selectedItems.isEmpty()) {
        ui->treeWidgetSensors->clearSelection();
        return;
    }

    int toolId = selectedItems.first()->data(0, Qt::UserRole).toInt();
    selectedToolId = toolId;
    updateSensorSelectionForTool(toolId);
}

void ToolSensorRelationEditor::onSensorSelectionChanged()
{
    QList<QTreeWidgetItem *> selectedItems = ui->treeWidgetSensors->selectedItems();
    if (selectedItems.isEmpty()) {
        ui->treeWidgetTools->clearSelection();
        return;
    }

    int sensorId = selectedItems.first()->data(0, Qt::UserRole).toInt();
    selectedSensorId = sensorId;
    updateToolSelectionForSensor(sensorId);
}

void ToolSensorRelationEditor::updateSensorSelectionForTool(int toolId)
{
    ui->treeWidgetSensors->blockSignals(true);

    for (int i = 0; i < ui->treeWidgetSensors->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->treeWidgetSensors->topLevelItem(i);
        int sensorId = item->data(0, Qt::UserRole).toInt();
        const ToolSensor *toolSensor = findToolSensor(toolId, sensorId);
        if (toolSensor) {
            item->setText(1, QString::number(toolSensor->getOffsetMm()));
            item->setSelected(true);
        } else {
            item->setText(1, "");
            item->setSelected(false);
        }
    }

    ui->treeWidgetSensors->blockSignals(false);
}

void ToolSensorRelationEditor::updateToolSelectionForSensor(int sensorId)
{
    ui->treeWidgetTools->blockSignals(true);

    for (int i = 0; i < ui->treeWidgetTools->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->treeWidgetTools->topLevelItem(i);
        int toolId = item->data(0, Qt::UserRole).toInt();
        const ToolSensor *toolSensor = findToolSensor(toolId, sensorId);
        if (toolSensor) {
            item->setText(1, QString::number(toolSensor->getOffsetMm()));
            item->setSelected(true);
        } else {
            item->setText(1, "");
            item->setSelected(false);
        }
    }

    ui->treeWidgetTools->blockSignals(false);
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
    QAction *addAction = contextMenu.addAction("Add Relation");
    QAction *removeAction = contextMenu.addAction("Remove Relation");
    QAction *editOffsetAction = contextMenu.addAction("Edit Offset");

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

void ToolSensorRelationEditor::showSensorContextMenu(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->treeWidgetSensors->itemAt(pos);
    if (!item)
        return;

    QMenu contextMenu;
    QAction *addAction = contextMenu.addAction("Add Relation");
    QAction *removeAction = contextMenu.addAction("Remove Relation");
    QAction *editOffsetAction = contextMenu.addAction("Edit Offset");

    connect(addAction, &QAction::triggered, this, &ToolSensorRelationEditor::addRelationFromSensor);
    connect(removeAction,
            &QAction::triggered,
            this,
            &ToolSensorRelationEditor::removeRelationFromSensor);
    connect(editOffsetAction,
            &QAction::triggered,
            this,
            &ToolSensorRelationEditor::editOffsetFromSensor);

    contextMenu.exec(ui->treeWidgetSensors->mapToGlobal(pos));
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
        QMessageBox::information(this, "Add Relation", "No available sensors to relate.");
        return;
    }

    // Show dialog to select sensors and specify offsetMm
    QDialog dialog(this);
    dialog.setWindowTitle("Add Relations");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *label = new QLabel("Select Sensors and specify Offset (mm):", &dialog);
    layout->addWidget(label);

    QTableWidget *tableWidget = new QTableWidget(&dialog);
    tableWidget->setColumnCount(2);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "Sensor"
                                                         << "Offset (mm)");
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

    QPushButton *okButton = new QPushButton("OK", &dialog);
    QPushButton *cancelButton = new QPushButton("Cancel", &dialog);
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
            if (itemSensor->isSelected()) { // Corrected line
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
    // Implementation remains the same
    // ...
}

void ToolSensorRelationEditor::editOffsetFromTool()
{
    // Implementation remains the same
    // ...
}

void ToolSensorRelationEditor::addRelationFromSensor()
{
    QList<QTreeWidgetItem *> selectedSensors = ui->treeWidgetSensors->selectedItems();
    if (selectedSensors.isEmpty())
        return;

    int sensorId = selectedSensors.first()->data(0, Qt::UserRole).toInt();

    // Get tools not related to the selected sensor
    QSet<int> relatedToolIds;
    for (const ToolSensor &toolSensor : storage->getToolSensors()) {
        if (toolSensor.getSensorId() == sensorId) {
            relatedToolIds.insert(toolSensor.getToolId());
        }
    }

    QList<const Tool *> availableTools;
    for (const Tool &tool : storage->getTools()) {
        if (!relatedToolIds.contains(tool.getId())) {
            availableTools.append(&tool);
        }
    }

    if (availableTools.isEmpty()) {
        QMessageBox::information(this, "Add Relation", "No available tools to relate.");
        return;
    }

    // Show dialog to select tools and specify offsetMm
    QDialog dialog(this);
    dialog.setWindowTitle("Add Relations");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *label = new QLabel("Select Tools and specify Offset (mm):", &dialog);
    layout->addWidget(label);

    QTableWidget *tableWidget = new QTableWidget(&dialog);
    tableWidget->setColumnCount(2);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "Tool"
                                                         << "Offset (mm)");
    tableWidget->setRowCount(availableTools.size());
    tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    for (int i = 0; i < availableTools.size(); ++i) {
        const Tool *tool = availableTools.at(i);
        QTableWidgetItem *itemTool = new QTableWidgetItem(tool->getName());
        itemTool->setData(Qt::UserRole, tool->getId());
        tableWidget->setItem(i, 0, itemTool);

        QTableWidgetItem *itemOffset = new QTableWidgetItem("0");
        tableWidget->setItem(i, 1, itemOffset);
    }
    layout->addWidget(tableWidget);

    QPushButton *okButton = new QPushButton("OK", &dialog);
    QPushButton *cancelButton = new QPushButton("Cancel", &dialog);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        StorageEditor storageEditor(storage);
        for (int i = 0; i < tableWidget->rowCount(); ++i) {
            QTableWidgetItem *itemTool = tableWidget->item(i, 0);
            if (itemTool->isSelected()) { // Corrected line
                int toolId = itemTool->data(Qt::UserRole).toInt();
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

void ToolSensorRelationEditor::removeRelationFromSensor()
{
    QList<QTreeWidgetItem *> selectedSensors = ui->treeWidgetSensors->selectedItems();
    if (selectedSensors.isEmpty())
        return;

    int sensorId = selectedSensors.first()->data(0, Qt::UserRole).toInt();

    QList<const ToolSensor *> relatedToolSensors;
    for (const ToolSensor &toolSensor : storage->getToolSensors()) {
        if (toolSensor.getSensorId() == sensorId) {
            relatedToolSensors.append(&toolSensor);
        }
    }

    if (relatedToolSensors.isEmpty()) {
        QMessageBox::information(this, "Remove Relation", "No relations to remove.");
        return;
    }

    // Показываем диалог для выбора инструментов для удаления связей
    QDialog dialog(this);
    dialog.setWindowTitle("Remove Relations");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *label = new QLabel("Select Tools to remove relations:", &dialog);
    layout->addWidget(label);

    QListWidget *listWidget = new QListWidget(&dialog);
    listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    for (const ToolSensor *toolSensor : relatedToolSensors) {
        const Tool *tool = findToolById(toolSensor->getToolId());
        if (tool) {
            QListWidgetItem *item = new QListWidgetItem(tool->getName());
            item->setData(Qt::UserRole, toolSensor->getId());
            listWidget->addItem(item);
        }
    }
    layout->addWidget(listWidget);

    QPushButton *okButton = new QPushButton("OK", &dialog);
    QPushButton *cancelButton = new QPushButton("Cancel", &dialog);
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
            for (auto it = storage->getToolSensors().begin(); it != storage->getToolSensors().end();
                 ++it) {
                if (it->getId() == toolSensorId) {
                    storageEditor.deleteElement(storage->getToolSensors(), *it);
                    break;
                }
            }
        }
        refreshRelations();
    }
}
void ToolSensorRelationEditor::editOffsetFromSensor()
{
    // Получаем выбранный сенсор
    QList<QTreeWidgetItem *> selectedSensors = ui->treeWidgetSensors->selectedItems();
    if (selectedSensors.isEmpty()) {
        QMessageBox::warning(this, "Edit Offset", "No sensor selected.");
        return;
    }

    int sensorId = selectedSensors.first()->data(0, Qt::UserRole).toInt();

    // Получаем все связи для этого сенсора
    QList<const ToolSensor *> relatedToolSensors;
    for (const ToolSensor &toolSensor : storage->getToolSensors()) {
        if (toolSensor.getSensorId() == sensorId) {
            relatedToolSensors.append(&toolSensor);
        }
    }

    // Если нет связей для этого сенсора
    if (relatedToolSensors.isEmpty()) {
        QMessageBox::information(this, "Edit Offset", "No relations found for the selected sensor.");
        return;
    }

    // Создаём диалог для выбора связи
    QDialog dialog(this);
    dialog.setWindowTitle("Select Relation to Edit");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *label = new QLabel("Select a relation to edit the offset:", &dialog);
    layout->addWidget(label);

    // Таблица для отображения связей
    QTableWidget *tableWidget = new QTableWidget(&dialog);
    tableWidget->setColumnCount(3);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "Tool"
                                                         << "Sensor"
                                                         << "Offset (mm)");
    tableWidget->setRowCount(relatedToolSensors.size());

    // Заполнение таблицы данными
    for (int i = 0; i < relatedToolSensors.size(); ++i) {
        const ToolSensor *toolSensor = relatedToolSensors[i];
        const Tool *tool = findToolById(toolSensor->getToolId());
        const Sensor *sensor = findSensorById(toolSensor->getSensorId());

        QTableWidgetItem *toolItem = new QTableWidgetItem(tool ? tool->getName() : "Unknown");
        toolItem->setData(Qt::UserRole, toolSensor->getToolId());
        tableWidget->setItem(i, 0, toolItem);

        QTableWidgetItem *sensorItem = new QTableWidgetItem(sensor ? sensor->getName() : "Unknown");
        sensorItem->setData(Qt::UserRole, toolSensor->getSensorId());
        tableWidget->setItem(i, 1, sensorItem);

        QTableWidgetItem *offsetItem = new QTableWidgetItem(
            QString::number(toolSensor->getOffsetMm()));
        tableWidget->setItem(i, 2, offsetItem);
    }

    layout->addWidget(tableWidget);

    // Кнопки OK и Cancel
    QPushButton *okButton = new QPushButton("OK", &dialog);
    QPushButton *cancelButton = new QPushButton("Cancel", &dialog);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        // Получаем выбранный элемент
        QList<QTableWidgetItem *> selectedItems = tableWidget->selectedItems();
        if (selectedItems.isEmpty()) {
            QMessageBox::warning(this, "Edit Offset", "No relation selected.");
            return;
        }

        // Получаем выбранный toolId
        int selectedRow = tableWidget->currentRow();
        int toolId = tableWidget->item(selectedRow, 0)->data(Qt::UserRole).toInt();

        ToolSensor *toolSensorPtr = nullptr;
        for (ToolSensor &toolSensor : storage->getToolSensors()) {
            if (toolSensor.getToolId() == toolId && toolSensor.getSensorId() == sensorId) {
                toolSensorPtr = &toolSensor;
                break;
            }
        }

        if (!toolSensorPtr) {
            QMessageBox::warning(this,
                                 "Edit Offset",
                                 "No relation found for the selected Tool and Sensor.");
            return;
        }

        // Показываем диалог для ввода нового значения offset
        bool ok;
        int newOffsetMm = QInputDialog::getInt(this,
                                               "Edit Offset",
                                               "Enter new Offset (mm):",
                                               toolSensorPtr->getOffsetMm(),
                                               0,
                                               1000000,
                                               1,
                                               &ok);
        if (ok) {
            toolSensorPtr->setOffsetMm(newOffsetMm);

            // Сохраняем изменения в хранилище
            StorageEditor storageEditor(storage);
            storageEditor.insertOrReplace(storage->getToolSensors(), *toolSensorPtr);

            // Обновляем интерфейс
            refreshRelations();
        }
    }
}

void ToolSensorRelationEditor::updateRelation(int toolId, int sensorId, int offsetMm)
{
    // Найдем существующую связь между инструментом и сенсором
    ToolSensor *existingRelation = nullptr;
    for (ToolSensor &toolSensor : storage->getToolSensors()) {
        if (toolSensor.getToolId() == toolId && toolSensor.getSensorId() == sensorId) {
            existingRelation = &toolSensor;
            break;
        }
    }

    StorageEditor storageEditor(storage);

    // Если связь найдена, обновляем offsetMm
    if (existingRelation) {
        existingRelation->setOffsetMm(offsetMm);
        storageEditor.insertOrReplace(storage->getToolSensors(), *existingRelation);
    } else {
        // Если связь не найдена, создаем новую
        int newId = storage->generateNewToolSensorId();
        ToolSensor newRelation(newId, toolId, sensorId, offsetMm);
        storageEditor.insertOrReplace(storage->getToolSensors(), newRelation);
    }

    // Обновляем отображение связей
    refreshRelations();
}

void ToolSensorRelationEditor::removeRelation(int toolId, int sensorId)
{
    // Найдем связь для удаления
    for (auto it = storage->getToolSensors().begin(); it != storage->getToolSensors().end(); ++it) {
        if (it->getToolId() == toolId && it->getSensorId() == sensorId) {
            StorageEditor storageEditor(storage);
            storageEditor.deleteElement(storage->getToolSensors(), *it);
            break;
        }
    }

    // Обновляем отображение связей
    refreshRelations();
}

void ToolSensorRelationEditor::refreshRelations()
{
    if (selectedToolId != -1) {
        updateSensorSelectionForTool(selectedToolId);
    }

    if (selectedSensorId != -1) {
        updateToolSelectionForSensor(selectedSensorId);
    }
}
