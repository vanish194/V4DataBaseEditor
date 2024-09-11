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
        QStandardItem *toolItem = new QStandardItem(tool.getName());
        toolItem->setData(QVariant(tool.getId()), Qt::UserRole + 1); // Сохраняем ID инструмента
        toolItem->setData(QVariant(ToolType), Qt::UserRole + 2); // Сохраняем тип элемента

        // Получаем все ToolSensor для данного инструмента
        QList<ToolSensor> toolSensors;
        for (const ToolSensor &toolSensor : storage->getToolSensors()) {
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
                QStandardItem *sensorItem = new QStandardItem(sensor->getName());
                sensorItem->setData(QVariant(sensor->getId()),
                                    Qt::UserRole + 1); // Сохраняем ID сенсора
                sensorItem->setData(QVariant(SensorType),
                                    Qt::UserRole + 2); // Сохраняем тип элемента

                // Получаем все MainMnemonic для данного сенсора
                QList<MainMnemonic> mainMnemonics;
                for (const MainMnemonic &mainMnemonic : storage->getMainMnemonics()) {
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
                                              Qt::UserRole + 1); // Сохраняем ID main mnemonic
                    mainMnemonicItem->setData(QVariant(MainMnemonicType),
                                              Qt::UserRole + 2); // Сохраняем тип элемента

                    // Получаем все AdditionalMnemonic для данного main mnemonic
                    QList<AdditionalMnemonic> additionalMnemonics;
                    for (const AdditionalMnemonic &additionalMnemonic :
                         storage->getAdditionalMnemonics()) {
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
                                                        Qt::UserRole
                                                            + 1); // Сохраняем ID additional mnemonic
                        additionalMnemonicItem->setData(QVariant(AdditionalMnemonicType),
                                                        Qt::UserRole + 2); // Сохраняем тип элемента
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

    // Извлекаем ID и тип элемента
    int elementId = index.data(Qt::UserRole + 1).toInt();
    ElementType elementType = static_cast<ElementType>(index.data(Qt::UserRole + 2).toInt());

    // Проверяем, выбрано ли недопустимое значение (например, "N/A")
    if (elementId == 0) {
        qDebug() << "Invalid selection: N/A item chosen";
        return; // Игнорируем "N/A" элементы
    }

    // Создаем контекстное меню
    QMenu contextMenu(this);

    QAction *addAction = contextMenu.addAction("Add");
    QAction *editAction = contextMenu.addAction("Edit");
    QAction *deleteAction = contextMenu.addAction("Delete");

    // Соединяем действия с обработчиками, передавая ID и тип элемента
    connect(addAction, &QAction::triggered, this, [=]() { onAddItem(elementId, elementType); });
    connect(editAction, &QAction::triggered, this, [=]() { onEditItem(elementId, elementType); });
    connect(deleteAction, &QAction::triggered, this, [=]() {
        onDeleteItem(elementId, elementType);
    });

    contextMenu.exec(mapToGlobal(event->pos()));
}

void ToolSensorMnemonicTreeView::onAddItem(int elementId, ElementType elementType)
{
    qDebug() << "Add action triggered for ID:" << elementId << "Type:" << elementType;
    // Логика добавления нового элемента
}

void ToolSensorMnemonicTreeView::onEditItem(int elementId, ElementType elementType)
{
    qDebug() << "Edit action triggered for ID:" << elementId << "Type:" << elementType;
    // Логика редактирования элемента
}

void ToolSensorMnemonicTreeView::onDeleteItem(int elementId, ElementType elementType)
{
    qDebug() << "Delete action triggered for ID:" << elementId << "Type:" << elementType;
    // Логика удаления элемента
}
