#ifndef TOOLSENSORMNEMONICTREEVIEW_H
#define TOOLSENSORMNEMONICTREEVIEW_H

#include <QMenu>
#include <QStandardItemModel>
#include <QTreeView>
#include "storage.h"

class ToolSensorMnemonicTreeView : public QTreeView
{
    Q_OBJECT

public:
    enum ElementType { ToolType, SensorType, MainMnemonicType, AdditionalMnemonicType };

    explicit ToolSensorMnemonicTreeView(QWidget *parent = nullptr);
    void buildTree();

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    QStandardItemModel *model;

    const Sensor *findSensorById(int sensorId);

    // Методы для контекстного меню
    void onAddItem(int elementId, ElementType elementType);
    void onEditItem(int elementId, ElementType elementType);
    void onDeleteItem(int elementId, ElementType elementType);
};

#endif // TOOLSENSORMNEMONICTREEVIEW_H
