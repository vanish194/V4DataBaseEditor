#ifndef TOOLADDWINDOW_H
#define TOOLADDWINDOW_H

#include <QDialog>
#include "storage.h"
#include "ui_tooladdwindow.h"

class ToolAddWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ToolAddWindow(QWidget *parent = nullptr);
    ~ToolAddWindow();

    void setNewToolId(int id);
    void setNewToolDescriptionId(int id);

    Tool getNewTool() const;
    ToolDescription getNewToolDescription() const;

private slots:
    void onAddButtonClicked();
    void onCancelButtonClicked();

private:
    Ui::ToolAddWindow ui;

    Tool newTool;
    ToolDescription newToolDescription;

    void loadProducers();
};

#endif // TOOLADDWINDOW_H
