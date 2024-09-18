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

    // Методы для установки выбранного инструмента и сенсора
    void setTool(int toolId);
    void setSensor(int sensorId);

private slots:
    void onToolSelectionChanged();
    void onSensorSelectionChanged();
    void showToolContextMenu(const QPoint &pos);
    void showSensorContextMenu(const QPoint &pos);
    void addRelationFromTool();
    void removeRelationFromTool();
    void editOffsetFromTool();
    void addRelationFromSensor();
    void removeRelationFromSensor();
    void editOffsetFromSensor();
    void updateRelation(int toolId, int sensorId, int offsetMm);
    void removeRelation(int toolId, int sensorId);
    void refreshRelations();

private:
    Ui::ToolSensorRelationEditor *ui;
    Storage *storage;

    // Переменные для хранения выбранного инструмента и сенсора
    int selectedToolId;
    int selectedSensorId;

    const Sensor *findSensorById(int sensorId) const;
    const ToolSensor *findToolSensor(int toolId, int sensorId) const;
    const Tool *findToolById(int toolId) const;

    void loadTools();
    void loadSensors();
    void updateSensorSelectionForTool(int toolId);
    void updateToolSelectionForSensor(int sensorId);
};

#endif // TOOLSENSORRELATIONEDITOR_H
