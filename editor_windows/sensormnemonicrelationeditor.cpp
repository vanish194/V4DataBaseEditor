#include "sensormnemonicrelationeditor.h"
#include "ui_sensormnemonicrelationeditor.h"

#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSet>
#include <QTableWidget>
#include <QVBoxLayout>

SensorMnemonicRelationEditor::SensorMnemonicRelationEditor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SensorMnemonicRelationEditor)
    , storage(Storage::getInstance())
    , selectedSensorId(-1)
{
    ui->setupUi(this);

    loadSensors();
    loadMnemonics();

    connect(ui->treeWidgetSensors,
            &QTreeWidget::itemSelectionChanged,
            this,
            &SensorMnemonicRelationEditor::onSensorSelectionChanged);

    ui->treeWidgetSensors->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidgetSensors,
            &QTreeWidget::customContextMenuRequested,
            this,
            &SensorMnemonicRelationEditor::showSensorContextMenu);

    ui->treeWidgetMnemonics->setContextMenuPolicy(Qt::NoContextMenu);
}

SensorMnemonicRelationEditor::~SensorMnemonicRelationEditor()
{
    delete ui;
}

void SensorMnemonicRelationEditor::setSensor(int sensorId)
{
    selectedSensorId = sensorId;

    // Выбираем сенсор в treeWidgetSensors
    for (int i = 0; i < ui->treeWidgetSensors->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->treeWidgetSensors->topLevelItem(i);
        if (item->data(0, Qt::UserRole).toInt() == sensorId) {
            ui->treeWidgetSensors->setCurrentItem(item);
            break;
        }
    }

    updateMnemonicSelectionForSensor(sensorId);
}

void SensorMnemonicRelationEditor::loadSensors()
{
    ui->treeWidgetSensors->clear();
    ui->treeWidgetSensors->setHeaderLabels(QStringList() << "Sensor");

    for (const Sensor &sensor : storage->getSensors()) {
        if (sensor.getId() < 0) {
            continue;
        }
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidgetSensors);
        item->setText(0, sensor.getName());
        item->setData(0, Qt::UserRole, sensor.getId());
    }
}

void SensorMnemonicRelationEditor::loadMnemonics()
{
    ui->treeWidgetMnemonics->clear();
    ui->treeWidgetMnemonics->setHeaderLabels(QStringList() << "Mnemonic");

    for (const MainMnemonic &mnemonic : storage->getMainMnemonics()) {
        if (mnemonic.getId() < 0) {
            continue;
        }
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidgetMnemonics);
        item->setText(0, mnemonic.getName());
        item->setData(0, Qt::UserRole, mnemonic.getId());
    }
}

void SensorMnemonicRelationEditor::onSensorSelectionChanged()
{
    QList<QTreeWidgetItem *> selectedItems = ui->treeWidgetSensors->selectedItems();
    if (selectedItems.isEmpty()) {
        ui->treeWidgetMnemonics->clear();
        return;
    }

    int sensorId = selectedItems.first()->data(0, Qt::UserRole).toInt();
    selectedSensorId = sensorId;
    updateMnemonicSelectionForSensor(sensorId);
}

void SensorMnemonicRelationEditor::updateMnemonicSelectionForSensor(int sensorId)
{
    ui->treeWidgetMnemonics->clear();

    QList<const SensorMnemonic *> relatedSensorMnemonics;
    for (const SensorMnemonic &sensorMnemonic : storage->getSensorMnemonics()) {
        if (sensorMnemonic.getSensorId() == sensorId && sensorMnemonic.getId() > 0) {
            relatedSensorMnemonics.append(&sensorMnemonic);
        }
    }

    for (const SensorMnemonic *sensorMnemonic : relatedSensorMnemonics) {
        const MainMnemonic *mnemonic = findMainMnemonicById(sensorMnemonic->getMnemonicId());
        if (mnemonic) {
            QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidgetMnemonics);
            item->setText(0, mnemonic->getName());
            item->setData(0, Qt::UserRole, mnemonic->getId());
        }
    }
}

const Sensor *SensorMnemonicRelationEditor::findSensorById(int sensorId) const
{
    for (const Sensor &sensor : storage->getSensors()) {
        if (sensor.getId() == sensorId) {
            return &sensor;
        }
    }
    return nullptr;
}

const MainMnemonic *SensorMnemonicRelationEditor::findMainMnemonicById(int mnemonicId) const
{
    for (const MainMnemonic &mnemonic : storage->getMainMnemonics()) {
        if (mnemonic.getId() == mnemonicId) {
            return &mnemonic;
        }
    }
    return nullptr;
}

void SensorMnemonicRelationEditor::showSensorContextMenu(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->treeWidgetSensors->itemAt(pos);
    if (!item)
        return;

    QMenu contextMenu;
    QAction *addAction = contextMenu.addAction("Add Mnemonic");
    QAction *removeAction = contextMenu.addAction("Remove Mnemonic");

    connect(addAction,
            &QAction::triggered,
            this,
            &SensorMnemonicRelationEditor::addRelationFromSensor);
    connect(removeAction,
            &QAction::triggered,
            this,
            &SensorMnemonicRelationEditor::removeRelationFromSensor);

    contextMenu.exec(ui->treeWidgetSensors->mapToGlobal(pos));
}

void SensorMnemonicRelationEditor::addRelationFromSensor()
{
    QList<QTreeWidgetItem *> selectedSensors = ui->treeWidgetSensors->selectedItems();
    if (selectedSensors.isEmpty())
        return;

    int sensorId = selectedSensors.first()->data(0, Qt::UserRole).toInt();

    QSet<int> relatedMnemonicIds;
    for (const SensorMnemonic &sensorMnemonic : storage->getSensorMnemonics()) {
        if (sensorMnemonic.getSensorId() == sensorId) {
            relatedMnemonicIds.insert(sensorMnemonic.getMnemonicId());
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

    QLabel *label = new QLabel("Select Mnemonics to add:", &dialog);
    layout->addWidget(label);

    QListWidget *listWidget = new QListWidget(&dialog);
    listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    for (const MainMnemonic *mnemonic : availableMnemonics) {
        QListWidgetItem *item = new QListWidgetItem(mnemonic->getName());
        item->setData(Qt::UserRole, mnemonic->getId());
        listWidget->addItem(item);
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
            int mnemonicId = item->data(Qt::UserRole).toInt();

            int newId = storage->generateNewSensorMnemonicId();
            SensorMnemonic newRelation(newId, sensorId, mnemonicId);
            storageEditor.insertOrReplace(storage->getSensorMnemonics(), newRelation);
        }
        refreshRelations();
    }
}

void SensorMnemonicRelationEditor::removeRelationFromSensor()
{
    QList<QTreeWidgetItem *> selectedSensors = ui->treeWidgetSensors->selectedItems();
    if (selectedSensors.isEmpty())
        return;

    int sensorId = selectedSensors.first()->data(0, Qt::UserRole).toInt();

    QList<const SensorMnemonic *> relatedSensorMnemonics;
    for (const SensorMnemonic &sensorMnemonic : storage->getSensorMnemonics()) {
        if (sensorMnemonic.getSensorId() == sensorId && sensorMnemonic.getId() > 0) {
            relatedSensorMnemonics.append(&sensorMnemonic);
        }
    }

    if (relatedSensorMnemonics.isEmpty()) {
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
    for (const SensorMnemonic *sensorMnemonic : relatedSensorMnemonics) {
        const MainMnemonic *mnemonic = findMainMnemonicById(sensorMnemonic->getMnemonicId());
        if (mnemonic) {
            QListWidgetItem *item = new QListWidgetItem(mnemonic->getName());
            item->setData(Qt::UserRole, sensorMnemonic->getId());
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
            int sensorMnemonicId = item->data(Qt::UserRole).toInt();
            for (SensorMnemonic &sensorMnemonic : storage->getSensorMnemonics()) {
                if (sensorMnemonic.getId() == sensorMnemonicId) {
                    sensorMnemonic.setId(-abs(sensorMnemonic.getId()));
                    storageEditor.insertOrReplace(storage->getSensorMnemonics(), sensorMnemonic);
                    break;
                }
            }
        }
        refreshRelations();
    }
}

void SensorMnemonicRelationEditor::refreshRelations()
{
    if (selectedSensorId != -1) {
        updateMnemonicSelectionForSensor(selectedSensorId);
    }
}
