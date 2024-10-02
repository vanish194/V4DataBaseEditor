#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
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

    // Отключаем действие "Compare Current Data" по умолчанию
    ui->actionCompareCurrentData->setEnabled(false);

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpenDatabase);
    connect(ui->actionCompareCurrentData,
            &QAction::triggered,
            this,
            &MainWindow::onCompareCurrentData);

    // Инициализация splitter'ов, detailView и imageLabel
    mainSplitter = ui->mainSplitter;
    rightSplitter = ui->rightSplitter;
    detailView = ui->detailView;
    imageLabel = ui->imageLabel;
    treeView = ui->treeView; // Это теперь QTreeView*

    mainSplitter->setStretchFactor(4, 5); // Левая часть дерева
    mainSplitter->setStretchFactor(1, 2); // Правая часть (описание и изображение)

    // Подключаем сигнал выбора элемента к слоту обновления деталей и изображения
    connect(treeView->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            &MainWindow::onTreeSelectionChanged);
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

            // Включаем действие "Compare Current Data" после успешного открытия базы данных
            ui->actionCompareCurrentData->setEnabled(true);

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
    // Перестраиваем дерево
    treeView->buildTree(); // Предположим, у вашего treeView есть этот метод
    // Очищаем область детализации и изображение
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

    // Обновляем дерево после отката изменений
    if (treeView) {
        treeView->buildTree();
    }
}

void MainWindow::onTreeSelectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);

    // Очищаем область детализации и изображения, если элемент не выбран
    if (!current.isValid()) {
        detailView->clear();
        imageLabel->clear();
        imageLabel->setText("No Image");
        return;
    }

    // Получаем тип элемента и его ID из выбранного элемента
    int elementId = current.data(Qt::UserRole + 1).toInt();
    ToolSensorMnemonicTreeView::ElementType elementType
        = static_cast<ToolSensorMnemonicTreeView::ElementType>(
            current.data(Qt::UserRole + 2).toInt());

    // Очищаем изображение перед обновлением
    imageLabel->clear();

    QString description;

    // Обработка различных типов элементов
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

                // Загрузка изображения, если доступно
                if (!toolDescription->getImage().isEmpty()) {
                    QPixmap pixmap;
                    if (pixmap.loadFromData(toolDescription->getImage())) {
                        // Улучшение качества изображения при масштабировании
                        imageLabel->setPixmap(pixmap.scaled(imageLabel->size(),
                                                            Qt::KeepAspectRatio,
                                                            Qt::SmoothTransformation));
                    } else {
                        imageLabel->setText("Error loading image");
                    }
                } else {
                    imageLabel->setText("No Image Available");
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

    // Отображаем описание в текстовом виджете
    detailView->setText(description);
}
