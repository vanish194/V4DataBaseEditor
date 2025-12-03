#ifndef SENSORMNEMONICRELATIONEDITOR_H
#define SENSORMNEMONICRELATIONEDITOR_H

#include <QDialog>
#include <QTreeWidget>
#include "storage.h"
#include "storageeditor.h"
namespace Ui {
class SensorMnemonicRelationEditor;
}

class SensorMnemonicRelationEditor : public QDialog
{
    Q_OBJECT

public:
    explicit SensorMnemonicRelationEditor(QWidget *parent = nullptr);
    ~SensorMnemonicRelationEditor();

    void setSensor(int sensorId);

private slots:
    void onSensorSelectionChanged();
    void showSensorContextMenu(const QPoint &pos);
    void addRelationFromSensor();
    void removeRelationFromSensor();
    void refreshRelations();

private:
    Ui::SensorMnemonicRelationEditor *ui;
    Storage *storage;
    int selectedSensorId;

    const Sensor *findSensorById(int sensorId) const;
    const MainMnemonic *findMainMnemonicById(int mnemonicId) const;

    void loadSensors();
    void loadMnemonics();
    void updateMnemonicSelectionForSensor(int sensorId);
};

#endif // SENSORMNEMONICRELATIONEDITOR_H
