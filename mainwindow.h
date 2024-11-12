#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QCloseEvent>
#include <QLabel>
#include <QMainWindow>
#include <QSplitter>
#include <QTextEdit>
#include "databasemanager.h"
#include "databasesaver.h"
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

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onOpenDatabase();
    void onCompareCurrentData();
    void onApplyChanges();

    void onTreeSelectionChanged(const QModelIndex &current, const QModelIndex &previous);

    void on_actionToolMode_triggered();

    void on_actionSensorMode_triggered();

    void on_actionMainMnemonicMode_triggered();

    void on_actionAdditionalMnemonicMode_triggered();

private:
    Ui::MainWindow *ui;
    DatabaseManager dbManager;
    Storage *storage;
    ToolSensorMnemonicTreeView *treeView;
    QTextEdit *detailView;
    QLabel *imageLabel;
    QSplitter *mainSplitter;
    QSplitter *rightSplitter;

    QPixmap originalPixmap;

    void setViewsForToolSensorMnemonic();
};

#endif // MAINWINDOW_H
