#ifndef TOOLSENSORMNEMONICTREEVIEW_H
#define TOOLSENSORMNEMONICTREEVIEW_H

#include <QMenu>
#include <QStandardItemModel>
#include <QTreeView>
#include "editor_windows/sensor_editor.h"
#include "editor_windows/tool_editor.h"
#include "editor_windows/toolsensorrelationeditor.h"
#include "storage.h"
#include "storageeditor.h"

class ToolSensorMnemonicTreeView : public QTreeView
{
    Q_OBJECT

public:
    enum ElementType {
        ToolType,
        ToolDescriptionType,
        SensorType,
        SensorDescriptionType,
        MainMnemonicType,
        AdditionalMnemonicType,
        NoneType
    };

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

    void onEditRelations(int elementId, ElementType elementType);
};

#endif // TOOLSENSORMNEMONICTREEVIEW_H
