#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QSplitter>
#include <QTextEdit>
#include "databasemanager.h"
#include "editor_windows/comparedatadialog.h"
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
    void onCompareCurrentData();

    // Слот для обновления деталей и изображения
    void onTreeSelectionChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    Ui::MainWindow *ui;
    DatabaseManager dbManager;
    Storage *storage;
    ToolSensorMnemonicTreeView *treeView;
    QTextEdit *detailView;
    QLabel *imageLabel;       // Для отображения изображения
    QSplitter *mainSplitter;  // Главный сплиттер
    QSplitter *rightSplitter; // Сплиттер для правой половины

    void setViewsForToolSensorMnemonic();
};

#endif // MAINWINDOW_H
