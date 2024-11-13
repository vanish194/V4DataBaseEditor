#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , storage(Storage::getInstance())
{
    ui->setupUi(this);

    // Отключаем действия по умолчанию
    ui->actionCompareCurrentData->setEnabled(false);
    ui->actionApplyChanges->setEnabled(false);

    // Подключаем действия к слотам
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpenDatabase);
    connect(ui->actionCompareCurrentData,
            &QAction::triggered,
            this,
            &MainWindow::onCompareCurrentData);
    connect(ui->actionApplyChanges, &QAction::triggered, this, &MainWindow::onApplyChanges);

    // Инициализация виджетов
    mainSplitter = ui->mainSplitter;
    rightSplitter = ui->rightSplitter;
    detailView = ui->detailView;
    imageLabel = ui->imageLabel;
    treeView = ui->treeView;

    imageLabel->installEventFilter(this);

    mainSplitter->setStretchFactor(0, 1);
    mainSplitter->setStretchFactor(1, 3);
    mainSplitter->setCollapsible(false, false);
    rightSplitter->setStretchFactor(0, 1);
    rightSplitter->setStretchFactor(1, 2);
    rightSplitter->setCollapsible(false, false);
    // Подключение сигнала выбора элемента к слоту обновления деталей и изображения
    connect(treeView->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            &MainWindow::onTreeSelectionChanged);

    // Подключение меню к слотам
    connect(ui->actionToolMode, &QAction::triggered, this, &MainWindow::on_actionToolMode_triggered);
    connect(ui->actionSensorMode,
            &QAction::triggered,
            this,
            &MainWindow::on_actionSensorMode_triggered);
    connect(ui->actionMainMnemonicMode,
            &QAction::triggered,
            this,
            &MainWindow::on_actionMainMnemonicMode_triggered);
    connect(ui->actionAdditionalMnemonicMode,
            &QAction::triggered,
            this,
            &MainWindow::on_actionAdditionalMnemonicMode_triggered);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == imageLabel && event->type() == QEvent::Resize) {
        if (!originalPixmap.isNull()) {
            // Применяем поворот и масштабирование при изменении размера
            QTransform transform;
            transform.rotate(90); // Поворачиваем на 90 градусов
            QPixmap rotatedPixmap = originalPixmap.transformed(transform);

            // Масштабируем повернутое изображение до нового размера imageLabel
            QPixmap scaledPixmap = rotatedPixmap.scaled(imageLabel->size(),
                                                        Qt::KeepAspectRatio,
                                                        Qt::SmoothTransformation);
            imageLabel->setPixmap(scaledPixmap);
        }
        return true; // Событие полностью обработано, не передаем дальше
    }
    // Передаём остальные события стандартному обработчику
    return QMainWindow::eventFilter(obj, event);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (storage->isConnected) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,
                                      "Save Changes",
                                      "Do you want to save changes before exiting?",
                                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (reply == QMessageBox::Yes) {
            onApplyChanges();
            event->accept(); // Закрытие разрешено
        } else if (reply == QMessageBox::No) {
            event->accept(); // Закрытие разрешено без сохранения
        } else {
            event->ignore(); // Отмена закрытия
        }
    } else {
        event->accept(); // Закрытие разрешено, если нет изменений
    }
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

            // Включаем действия после успешного открытия базы данных

            ui->actionCompareCurrentData->setEnabled(true);
            ui->actionApplyChanges->setEnabled(true);

        } else {
            QMessageBox::critical(
                this,
                "Database Error",
                "Failed to open database. Check if the file is correct and try again.");
        }
    }
}

void MainWindow::setViewsForToolSensorMnemonic()
{
    // Rebuilding the tree
    treeView->buildTree();
    // Clearing the detail area and the image
    detailView->clear();
    imageLabel->clear();
    imageLabel->setText("No Image");
}

void MainWindow::onCompareCurrentData()
{
    if (!storage->isDataLoaded()) {
        QMessageBox::warning(this, "No Data", "No data is loaded. Please open a database first.");
        return;
    }

    CompareDataDialog dialog(this);
    dialog.exec();

    // Updating the tree after rolling back the changes
    if (treeView) {
        treeView->buildTree();
    }
}

void MainWindow::onTreeSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    // Очищаем детали и изображение, если элемент не выбран
    if (!current.isValid()) {
        detailView->clear();
        imageLabel->clear();
        imageLabel->setText("No Image");
        return;
    }

    int elementId = current.data(Qt::UserRole + 1).toInt();
    ToolSensorMnemonicTreeView::ElementType elementType
        = static_cast<ToolSensorMnemonicTreeView::ElementType>(
            current.data(Qt::UserRole + 2).toInt());

    imageLabel->clear();

    QString description;

    switch (elementType) {
    case ToolSensorMnemonicTreeView::ToolType: {
        const Tool *tool = storage->findToolById(elementId);
        if (tool) {
            const ToolDescription *toolDescription = storage->findToolDescriptionById(
                tool->getToolDescriptionId());
            if (toolDescription) {
                description = QString("Tool: %1\nDescription: %2\nLength: %3 mm\nOuter Diameter: "
                                      "%4 mm\nInner Diameter: %5 mm")
                                  .arg(tool->getName())
                                  .arg(toolDescription->getDescription())
                                  .arg(toolDescription->getLengthMm())
                                  .arg(toolDescription->getOuterDiameterMm())
                                  .arg(toolDescription->getInnerDiameterMm());

                if (!toolDescription->getImage().isEmpty()) {
                    QPixmap pixmap;
                    if (pixmap.loadFromData(toolDescription->getImage())) {
                        originalPixmap = pixmap; // Сохраняем оригинальный pixmap

                        // Поворот изображения на 90 градусов
                        QTransform transform;
                        transform.rotate(90);
                        QPixmap rotatedPixmap = originalPixmap.transformed(transform);

                        // Масштабируем изображение под размер imageLabel
                        QPixmap scaledPixmap = rotatedPixmap.scaled(imageLabel->size(),
                                                                    Qt::KeepAspectRatio,
                                                                    Qt::SmoothTransformation);

                        imageLabel->setPixmap(scaledPixmap);
                    } else {
                        imageLabel->setText("Error loading image");
                        originalPixmap = QPixmap(); // Очищаем оригинальный pixmap
                    }
                } else {
                    imageLabel->setText("No Image Available");
                    originalPixmap = QPixmap(); // Очищаем оригинальный pixmap
                }

                const Producer *producer = storage->findProducerById(
                    toolDescription->getProducerId());
                QString producerName = producer ? producer->getName() : "Unknown Producer";
                description += QString("\nProducer: %1").arg(producerName);
            } else {
                description = QString("Tool: %1\nNo Tool Description available.")
                                  .arg(tool->getName());
            }
        } else {
            description = "Tool not found.";
        }
        break;
    }
    case ToolSensorMnemonicTreeView::SensorType: {
        const Sensor *sensor = storage->findSensorById(elementId);
        if (sensor) {
            const Method *method = storage->findMethodById(sensor->getMethodId());
            QString methodName = method ? method->getName() : "Unknown Method";
            description = QString("Sensor: %1\nMethod: %2\nDescription: %3")
                              .arg(sensor->getName())
                              .arg(methodName)
                              .arg(sensor->getDescription());
        } else {
            description = "Sensor not found.";
        }
        imageLabel->setText("No Image");
        break;
    }
    case ToolSensorMnemonicTreeView::MainMnemonicType: {
        const MainMnemonic *mnemonic = storage->findMainMnemonicById(elementId);
        if (mnemonic) {
            description = QString("Main Mnemonic: %1\nDescription: %2")
                              .arg(mnemonic->getName())
                              .arg(mnemonic->getDescription());
        } else {
            description = "Main Mnemonic not found.";
        }
        imageLabel->setText("No Image");
        break;
    }
    case ToolSensorMnemonicTreeView::AdditionalMnemonicType: {
        const AdditionalMnemonic *mnemonic = storage->findAdditionalMnemonicById(elementId);
        if (mnemonic) {
            const Company *company = storage->findCompanyById(mnemonic->getCompanyId());
            QString companyName = company ? company->getName() : "Unknown Company";
            description = QString("Additional Mnemonic: %1\nCompany: %2")
                              .arg(mnemonic->getName())
                              .arg(companyName);
        } else {
            description = "Additional Mnemonic not found.";
        }
        imageLabel->setText("No Image");
        break;
    }
    default:
        description = "No description available.";
        imageLabel->setText("No Image");
        break;
    }
    // Displaying the description in a text widget
    detailView->setText(description);
}

void MainWindow::onApplyChanges()
{
    DatabaseSaver dbSaver(dbManager.getDatabase());

    if (dbSaver.saveAllData()) {
        QMessageBox::information(this,
                                 "Apply Changes",
                                 "Changes have been applied to the database.");

        // После успешного сохранения обновляем резервную копию данных
        storage->createBackup();

    } else {
        QMessageBox::critical(this, "Apply Changes", "Failed to apply changes to the database.");
    }
}

void MainWindow::on_actionToolMode_triggered()
{
    if (treeView) {
        treeView->setExpansionDepth(0);
    }
}

void MainWindow::on_actionSensorMode_triggered()
{
    if (treeView) {
        treeView->setExpansionDepth(1);
    }
}

void MainWindow::on_actionMainMnemonicMode_triggered()
{
    if (treeView) {
        treeView->setExpansionDepth(2);
    }
}
void MainWindow::on_actionAdditionalMnemonicMode_triggered()
{
    if (treeView) {
        treeView->setExpansionDepth(3);
    }
}
