#ifndef TOOLSENSORRELATIONEDITOR_H
#define TOOLSENSORRELATIONEDITOR_H

#include <QDialog>
#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QMessageBox>
#include <QTableWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include "storage.h"
#include "storageeditor.h"

namespace Ui {
class ToolSensorRelationEditor;
}

class ToolSensorRelationEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ToolSensorRelationEditor(QWidget *parent = nullptr);
    ~ToolSensorRelationEditor();

    // Methods to set the selected tool
    void setTool(int toolId);

private slots:
    void onToolSelectionChanged();
    void showToolContextMenu(const QPoint &pos);
    void addRelationFromTool();
    void removeRelationFromTool();
    void editOffsetFromTool();
    void updateRelation(int toolId, int sensorId, int offsetMm);
    void removeRelation(int toolId, int sensorId);
    void refreshRelations();

private:
    Ui::ToolSensorRelationEditor *ui;
    Storage *storage;

    // Variable to store selected tool ID
    int selectedToolId;

    const Sensor *findSensorById(int sensorId) const;
    const ToolSensor *findToolSensor(int toolId, int sensorId) const;
    const Tool *findToolById(int toolId) const;

    void loadTools();
    void loadSensors();
    void updateSensorSelectionForTool(int toolId);
};

#endif // TOOLSENSORRELATIONEDITOR_H
