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
    void setTool(const Tool &tool); // Устанавливает данные для редактирования инструмента
    Tool getTool() const; // Возвращает обновленный инструмент после редактирования

private slots:
    void onSaveButtonClicked(); // Обработка нажатия кнопки Save

private:
    Ui::ToolEditor ui;
    Tool currentTool; // Текущий редактируемый инструмент
};

#endif // TOOLEDITOR_H
