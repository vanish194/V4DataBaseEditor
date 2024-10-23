#include "toolmnemonicrelationeditor.h"
#include "ui_toolmnemonicrelationeditor.h"

#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSet>
#include <QTableWidget>
#include <QVBoxLayout>

ToolMnemonicRelationEditor::ToolMnemonicRelationEditor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ToolMnemonicRelationEditor)
    , storage(Storage::getInstance())
    , selectedToolId(-1)
{
    ui->setupUi(this);

    loadTools();
    loadMnemonics();

    connect(ui->treeWidgetTools,
            &QTreeWidget::itemSelectionChanged,
            this,
            &ToolMnemonicRelationEditor::onToolSelectionChanged);

    ui->treeWidgetTools->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidgetTools,
            &QTreeWidget::customContextMenuRequested,
            this,
            &ToolMnemonicRelationEditor::showToolContextMenu);

    ui->treeWidgetMnemonics->setContextMenuPolicy(Qt::NoContextMenu);
}

ToolMnemonicRelationEditor::~ToolMnemonicRelationEditor()
{
    delete ui;
}

void ToolMnemonicRelationEditor::setTool(int toolId)
{
    selectedToolId = toolId;

    for (int i = 0; i < ui->treeWidgetTools->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->treeWidgetTools->topLevelItem(i);
        if (item->data(0, Qt::UserRole).toInt() == toolId) {
            ui->treeWidgetTools->setCurrentItem(item);
            break;
        }
    }

    updateMnemonicSelectionForTool(toolId);
}

void ToolMnemonicRelationEditor::loadTools()
{
    ui->treeWidgetTools->clear();
    ui->treeWidgetTools->setHeaderLabels(QStringList() << "Tool");

    for (const Tool &tool : storage->getTools()) {
        if (tool.getId() < 0) {
            continue;
        }
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidgetTools);
        item->setText(0, tool.getName());
        item->setData(0, Qt::UserRole, tool.getId());
    }
}

void ToolMnemonicRelationEditor::loadMnemonics()
{
    ui->treeWidgetMnemonics->clear();
    ui->treeWidgetMnemonics->setHeaderLabels(QStringList() << "Mnemonic"
                                                           << "Offset (mm)");

    for (const MainMnemonic &mnemonic : storage->getMainMnemonics()) {
        if (mnemonic.getId() < 0) {
            continue;
        }
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidgetMnemonics);
        item->setText(0, mnemonic.getName());
        item->setData(0, Qt::UserRole, mnemonic.getId());
    }
}

void ToolMnemonicRelationEditor::onToolSelectionChanged()
{
    QList<QTreeWidgetItem *> selectedItems = ui->treeWidgetTools->selectedItems();
    if (selectedItems.isEmpty()) {
        ui->treeWidgetMnemonics->clear();
        return;
    }

    int toolId = selectedItems.first()->data(0, Qt::UserRole).toInt();
    selectedToolId = toolId;
    updateMnemonicSelectionForTool(toolId);
}

void ToolMnemonicRelationEditor::updateMnemonicSelectionForTool(int toolId)
{
    ui->treeWidgetMnemonics->clear();

    QList<const ToolMnemonic *> relatedToolMnemonics;
    for (const ToolMnemonic &toolMnemonic : storage->getToolMnemonics()) {
        if (toolMnemonic.getToolId() == toolId && toolMnemonic.getId() > 0) {
            relatedToolMnemonics.append(&toolMnemonic);
        }
    }

    for (const ToolMnemonic *toolMnemonic : relatedToolMnemonics) {
        const MainMnemonic *mnemonic = findMainMnemonicById(toolMnemonic->getMnemonicId());
        if (mnemonic) {
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidgetMnemonics);
            item->setText(0, mnemonic->getName());
            item->setText(1, QString::number(toolMnemonic->getOffsetMm()));
            item->setData(0, Qt::UserRole, mnemonic->getId());
        }
    }
}

const Tool *ToolMnemonicRelationEditor::findToolById(int toolId) const
{
    for (const Tool &tool : storage->getTools()) {
        if (tool.getId() == toolId) {
            return &tool;
        }
    }
    return nullptr;
}

const MainMnemonic *ToolMnemonicRelationEditor::findMainMnemonicById(int mnemonicId) const
{
    for (const MainMnemonic &mnemonic : storage->getMainMnemonics()) {
        if (mnemonic.getId() == mnemonicId) {
            return &mnemonic;
        }
    }
    return nullptr;
}

void ToolMnemonicRelationEditor::showToolContextMenu(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->treeWidgetTools->itemAt(pos);
    if (!item)
        return;

    QMenu contextMenu;
    QAction *addAction = contextMenu.addAction("Add Mnemonic");
    QAction *removeAction = contextMenu.addAction("Remove Mnemonic");
    QAction *editOffsetAction = contextMenu.addAction("Edit Offset");

    connect(addAction, &QAction::triggered, this, &ToolMnemonicRelationEditor::addRelationFromTool);
    connect(removeAction,
            &QAction::triggered,
            this,
            &ToolMnemonicRelationEditor::removeRelationFromTool);
    connect(editOffsetAction,
            &QAction::triggered,
            this,
            &ToolMnemonicRelationEditor::editOffsetFromTool);

    contextMenu.exec(ui->treeWidgetTools->mapToGlobal(pos));
}

void ToolMnemonicRelationEditor::addRelationFromTool()
{
    QList<QTreeWidgetItem *> selectedTools = ui->treeWidgetTools->selectedItems();
    if (selectedTools.isEmpty())
        return;

    int toolId = selectedTools.first()->data(0, Qt::UserRole).toInt();

    QSet<int> relatedMnemonicIds;
    for (const ToolMnemonic &toolMnemonic : storage->getToolMnemonics()) {
        if (toolMnemonic.getToolId() == toolId) {
            relatedMnemonicIds.insert(toolMnemonic.getMnemonicId());
        }
    }

    QList<const MainMnemonic *> availableMnemonics;
    for (const MainMnemonic &mnemonic : storage->getMainMnemonics()) {
        if (!relatedMnemonicIds.contains(mnemonic.getId())) {
            availableMnemonics.append(&mnemonic);
        }
    }

    if (availableMnemonics.isEmpty()) {
        QMessageBox::information(this, "Add Mnemonic", "No available mnemonics to add.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Add Mnemonics");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *label = new QLabel("Select Mnemonics and specify Offset (mm):", &dialog);
    layout->addWidget(label);

    QTableWidget *tableWidget = new QTableWidget(&dialog);
    tableWidget->setColumnCount(2);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "Mnemonic"
                                                         << "Offset (mm)");
    tableWidget->setRowCount(availableMnemonics.size());
    tableWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    for (int i = 0; i < availableMnemonics.size(); ++i) {
        const MainMnemonic *mnemonic = availableMnemonics.at(i);
        QTableWidgetItem *itemMnemonic = new QTableWidgetItem(mnemonic->getName());
        itemMnemonic->setData(Qt::UserRole, mnemonic->getId());
        tableWidget->setItem(i, 0, itemMnemonic);

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
            QTableWidgetItem *itemMnemonic = tableWidget->item(i, 0);
            if (itemMnemonic->isSelected()) {
                int mnemonicId = itemMnemonic->data(Qt::UserRole).toInt();
                QTableWidgetItem *itemOffset = tableWidget->item(i, 1);
                int offsetMm = itemOffset->text().toInt();

                int newId = storage->generateNewToolMnemonicId();
                ToolMnemonic newRelation(newId, toolId, mnemonicId, offsetMm);
                storageEditor.insertOrReplace(storage->getToolMnemonics(), newRelation);
            }
        }
        refreshRelations();
    }
}

void ToolMnemonicRelationEditor::removeRelationFromTool()
{
    QList<QTreeWidgetItem *> selectedTools = ui->treeWidgetTools->selectedItems();
    if (selectedTools.isEmpty())
        return;

    int toolId = selectedTools.first()->data(0, Qt::UserRole).toInt();

    QList<const ToolMnemonic *> relatedToolMnemonics;
    for (const ToolMnemonic &toolMnemonic : storage->getToolMnemonics()) {
        if (toolMnemonic.getToolId() == toolId && toolMnemonic.getId() > 0) {
            relatedToolMnemonics.append(&toolMnemonic);
        }
    }

    if (relatedToolMnemonics.isEmpty()) {
        QMessageBox::information(this, "Remove Mnemonic", "No mnemonics to remove.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Remove Mnemonics");
    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *label = new QLabel("Select Mnemonics to remove:", &dialog);
    layout->addWidget(label);

    QListWidget *listWidget = new QListWidget(&dialog);
    listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    for (const ToolMnemonic *toolMnemonic : relatedToolMnemonics) {
        const MainMnemonic *mnemonic = findMainMnemonicById(toolMnemonic->getMnemonicId());
        if (mnemonic) {
            QListWidgetItem *item = new QListWidgetItem(mnemonic->getName());
            item->setData(Qt::UserRole, toolMnemonic->getId());
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
            int toolMnemonicId = item->data(Qt::UserRole).toInt();
            for (ToolMnemonic &toolMnemonic : storage->getToolMnemonics()) {
                if (toolMnemonic.getId() == toolMnemonicId) {
                    toolMnemonic.setId(-abs(toolMnemonic.getId()));
                    storageEditor.insertOrReplace(storage->getToolMnemonics(), toolMnemonic);
                    break;
                }
            }
        }
        refreshRelations();
    }
}

void ToolMnemonicRelationEditor::editOffsetFromTool()
{
    QList<QTreeWidgetItem *> selectedTools = ui->treeWidgetTools->selectedItems();
    if (selectedTools.isEmpty()) {
        QMessageBox::warning(this, "Edit Offset", "No tool selected.");
        return;
    }

    int toolId = selectedTools.first()->data(0, Qt::UserRole).toInt();

    QList<const ToolMnemonic *> relatedToolMnemonics;
    for (const ToolMnemonic &toolMnemonic : storage->getToolMnemonics()) {
        if (toolMnemonic.getToolId() == toolId) {
            relatedToolMnemonics.append(&toolMnemonic);
        }
    }

    if (relatedToolMnemonics.isEmpty()) {
        QMessageBox::information(this, "Edit Offset", "No mnemonics related to the selected tool.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Select Mnemonic to Edit Offset");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *label = new QLabel("Select a mnemonic to edit the offset:", &dialog);
    layout->addWidget(label);

    QTableWidget *tableWidget = new QTableWidget(&dialog);
    tableWidget->setColumnCount(3);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "Tool"
                                                         << "Mnemonic"
                                                         << "Offset (mm)");
    tableWidget->setRowCount(relatedToolMnemonics.size());
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    for (int i = 0; i < relatedToolMnemonics.size(); ++i) {
        const ToolMnemonic *toolMnemonic = relatedToolMnemonics[i];
        const Tool *tool = findToolById(toolMnemonic->getToolId());
        const MainMnemonic *mnemonic = findMainMnemonicById(toolMnemonic->getMnemonicId());

        QTableWidgetItem *toolItem = new QTableWidgetItem(tool ? tool->getName() : "Unknown");
        toolItem->setData(Qt::UserRole, toolMnemonic->getToolId());
        tableWidget->setItem(i, 0, toolItem);

        QTableWidgetItem *mnemonicItem = new QTableWidgetItem(mnemonic ? mnemonic->getName()
                                                                       : "Unknown");
        mnemonicItem->setData(Qt::UserRole, toolMnemonic->getMnemonicId());
        tableWidget->setItem(i, 1, mnemonicItem);

        QTableWidgetItem *offsetItem = new QTableWidgetItem(
            QString::number(toolMnemonic->getOffsetMm()));
        tableWidget->setItem(i, 2, offsetItem);
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
        QModelIndexList selectedRows = tableWidget->selectionModel()->selectedRows();
        if (selectedRows.isEmpty()) {
            QMessageBox::warning(this, "Edit Offset", "No mnemonic selected.");
            return;
        }

        int selectedRow = selectedRows.first().row();
        int mnemonicId = tableWidget->item(selectedRow, 1)->data(Qt::UserRole).toInt();

        ToolMnemonic *toolMnemonicPtr = nullptr;
        for (ToolMnemonic &toolMnemonic : storage->getToolMnemonics()) {
            if (toolMnemonic.getToolId() == toolId && toolMnemonic.getMnemonicId() == mnemonicId) {
                toolMnemonicPtr = &toolMnemonic;
                break;
            }
        }

        if (!toolMnemonicPtr) {
            QMessageBox::warning(this,
                                 "Edit Offset",
                                 "No relation found for the selected Tool and Mnemonic.");
            return;
        }

        bool ok;
        int newOffsetMm = QInputDialog::getInt(this,
                                               "Edit Offset",
                                               "Enter new Offset (mm):",
                                               toolMnemonicPtr->getOffsetMm(),
                                               0,
                                               1000000,
                                               1,
                                               &ok);
        if (ok) {
            toolMnemonicPtr->setOffsetMm(newOffsetMm);

            StorageEditor storageEditor(storage);
            storageEditor.insertOrReplace(storage->getToolMnemonics(), *toolMnemonicPtr);

            refreshRelations();
        }
    }
}

void ToolMnemonicRelationEditor::refreshRelations()
{
    if (selectedToolId != -1) {
        updateMnemonicSelectionForTool(selectedToolId);
    }
}
