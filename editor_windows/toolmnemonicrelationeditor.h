#ifndef TOOLMNEMONICRELATIONEDITOR_H
#define TOOLMNEMONICRELATIONEDITOR_H

#include <QDialog>
#include <QTreeWidget>
#include "storage.h"
#include "storageeditor.h"
namespace Ui {
class ToolMnemonicRelationEditor;
}

class ToolMnemonicRelationEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ToolMnemonicRelationEditor(QWidget *parent = nullptr);
    ~ToolMnemonicRelationEditor();

    void setTool(int toolId);

private slots:
    void onToolSelectionChanged();
    void showToolContextMenu(const QPoint &pos);
    void addRelationFromTool();
    void removeRelationFromTool();
    void editOffsetFromTool();
    void refreshRelations();

private:
    Ui::ToolMnemonicRelationEditor *ui;
    Storage *storage;
    int selectedToolId;

    const Tool *findToolById(int toolId) const;
    const MainMnemonic *findMainMnemonicById(int mnemonicId) const;

    void loadTools();
    void loadMnemonics();
    void updateMnemonicSelectionForTool(int toolId);
};

#endif // TOOLMNEMONICRELATIONEDITOR_H
