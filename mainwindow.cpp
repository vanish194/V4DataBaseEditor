#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , storage(Storage::getInstance())
{
    ui->setupUi(this);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpenDatabase);
    connect(ui->actionCompareCurrentData,
            &QAction::triggered,
            this,
            &MainWindow::onCompareCurrentData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onOpenDatabase()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open Database",
                                                    "",
                                                    "SQLite Database (*.db)");
    if (!fileName.isEmpty()) {
        if (dbManager.connect(fileName)) {
            dbManager.loadAllData();
            setViewsForToolSensorMnemonic();

        } else {
            QMessageBox::critical(
                this,
                "Database Error",
                "Failed to open database. Check if the file is correct and try again.");
        }
    }
}

void MainWindow::populateTreeToolSensorMnemonic()
{
    treeView = new ToolSensorMnemonicTreeView(this);
    ui->gridLayout->addWidget(treeView);
    ui->treeView->setParent(nullptr);
    delete ui->treeView;
    ui->treeView = treeView;
    if (treeView) {
        treeView->buildTree();
    }
}

void MainWindow::populateTableToolSensorMnemonic() {}

void MainWindow::setViewsForToolSensorMnemonic()
{
    populateTreeToolSensorMnemonic();
    populateTableToolSensorMnemonic();
}

void MainWindow::onCompareCurrentData()
{
    CompareDataDialog dialog(this);
    dialog.exec();

    // Refresh the tree view after reverting changes
    if (treeView) {
        treeView->buildTree();
    }
}
