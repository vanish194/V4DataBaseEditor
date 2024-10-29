#ifndef TOOLSENSORMNEMONICTREEVIEW_H
#define TOOLSENSORMNEMONICTREEVIEW_H

#include <QMenu>
#include <QStandardItemModel>
#include <QTreeView>
#include "editor_windows/additionalmnemonicaddwindow.h"
#include "editor_windows/additionalmnemoniceditor.h"
#include "editor_windows/mainmnemonicaddwindow.h"
#include "editor_windows/mainmnemoniceditor.h"
#include "editor_windows/sensor_editor.h"
#include "editor_windows/sensoraddwindow.h"
#include "editor_windows/sensormnemonicrelationeditor.h"
#include "editor_windows/tool_editor.h"
#include "editor_windows/tooladdwindow.h"
#include "editor_windows/toolmnemonicrelationeditor.h"
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
private slots:
    void onAddItem(ElementType elementType);

private:
    QStandardItemModel *model;
    const Sensor *findSensorById(int sensorId);

    void onEditItem(int elementId, ElementType elementType);
    void onDeleteItem(int elementId, ElementType elementType);
    void onToolSensorRelations(int toolId);
    void onEditRelations(int elementId, ElementType elementType);
    void onToolMnemonicRelations(int toolId);
    void onSensorMnemonicRelations(int sensorId);
};

#endif // TOOLSENSORMNEMONICTREEVIEW_H
