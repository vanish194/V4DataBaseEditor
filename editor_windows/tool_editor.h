#ifndef TOOLEDITOR_H
#define TOOLEDITOR_H

#include <QDialog>
#include "ui_tool_editor.h"

#include "storage.h"

class ToolEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ToolEditor(QWidget *parent = nullptr);
    void setTool(const Tool &tool); // Sets the data for editing the tool
    Tool getTool() const;           // Returns the updated tool after editing

private slots:
    void onSaveButtonClicked(); // Handling the Save button click

private:
    Ui::ToolEditor ui;
    Tool currentTool; // Current editable tool
};

#endif // TOOLEDITOR_H
