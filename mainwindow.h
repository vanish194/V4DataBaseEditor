#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include "databasemanager.h"
#include "storage.h"
#include "views/toolsensormnemonictreeview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onOpenDatabase();

private:
    Ui::MainWindow *ui;
    DatabaseManager dbManager;
    Storage *storage;
    ToolSensorMnemonicTreeView *treeView;

    void populateTreeToolSensorMnemonic();
    void populateTableToolSensorMnemonic();

    void setViewsForToolSensorMnemonic();
};

#endif // MAINWINDOW_H
