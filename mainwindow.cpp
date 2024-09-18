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
    // Получаем изменённые данные из хранилища
    ModifiedData modifiedData = storage->getAllModifiedData();

    // Строка для вывода результатов
    QString result;

    // Проверяем каждый тип данных и собираем информацию об изменениях
    if (!modifiedData.modifiedUnits.isEmpty()) {
        result += "Modified Units:\n";
        for (const Unit &unit : modifiedData.modifiedUnits) {
            result += QString(" - Unit ID: %1\n").arg(unit.getId());
        }
    }
    if (!modifiedData.deletedUnits.isEmpty()) {
        result += "Deleted Units:\n";
        for (const Unit &unit : modifiedData.deletedUnits) {
            result += QString(" - Unit ID: %1\n")
                          .arg(-unit.getId()); // ID отрицательный, выводим положительный
        }
    }
    if (!modifiedData.modifiedTools.isEmpty()) {
        result += "Modified Tools:\n";
        for (const Tool &tool : modifiedData.modifiedTools) {
            result += QString(" - Tool ID: %1\n").arg(tool.getId());
        }
    }
    if (!modifiedData.deletedTools.isEmpty()) {
        result += "Deleted Tools:\n";
        for (const Tool &tool : modifiedData.deletedTools) {
            result += QString(" - Tool ID: %1\n").arg(-tool.getId());
        }
    }
    if (!modifiedData.modifiedSensors.isEmpty()) {
        result += "Modified Sensors:\n";
        for (const Sensor &sensor : modifiedData.modifiedSensors) {
            result += QString(" - Sensor ID: %1\n").arg(sensor.getId());
        }
    }
    if (!modifiedData.deletedSensors.isEmpty()) {
        result += "Deleted Sensors:\n";
        for (const Sensor &sensor : modifiedData.deletedSensors) {
            result += QString(" - Sensor ID: %1\n").arg(-sensor.getId());
        }
    }
    // Повторяем для всех остальных типов данных...

    // Обработка Producers
    if (!modifiedData.modifiedProducers.isEmpty()) {
        result += "Modified Producers:\n";
        for (const Producer &producer : modifiedData.modifiedProducers) {
            result += QString(" - Producer ID: %1\n").arg(producer.getId());
        }
    }
    if (!modifiedData.deletedProducers.isEmpty()) {
        result += "Deleted Producers:\n";
        for (const Producer &producer : modifiedData.deletedProducers) {
            result += QString(" - Producer ID: %1\n").arg(-producer.getId());
        }
    }

    // Обработка UnitTypes
    if (!modifiedData.modifiedUnitTypes.isEmpty()) {
        result += "Modified UnitTypes:\n";
        for (const UnitType &unitType : modifiedData.modifiedUnitTypes) {
            result += QString(" - UnitType ID: %1\n").arg(unitType.getId());
        }
    }
    if (!modifiedData.deletedUnitTypes.isEmpty()) {
        result += "Deleted UnitTypes:\n";
        for (const UnitType &unitType : modifiedData.deletedUnitTypes) {
            result += QString(" - UnitType ID: %1\n").arg(-unitType.getId());
        }
    }

    // Обработка ToolSensors
    if (!modifiedData.modifiedToolSensors.isEmpty()) {
        result += "Modified ToolSensors:\n";
        for (const ToolSensor &toolSensor : modifiedData.modifiedToolSensors) {
            result += QString(" - ToolSensor ID: %1\n").arg(toolSensor.getId());
        }
    }
    if (!modifiedData.deletedToolSensors.isEmpty()) {
        result += "Deleted ToolSensors:\n";
        for (const ToolSensor &toolSensor : modifiedData.deletedToolSensors) {
            result += QString(" - ToolSensor ID: %1\n").arg(-toolSensor.getId());
        }
    }

    // Обработка AdditionalMnemonics
    if (!modifiedData.modifiedAdditionalMnemonics.isEmpty()) {
        result += "Modified AdditionalMnemonics:\n";
        for (const AdditionalMnemonic &mnemonic : modifiedData.modifiedAdditionalMnemonics) {
            result += QString(" - AdditionalMnemonic ID: %1\n").arg(mnemonic.getId());
        }
    }
    if (!modifiedData.deletedAdditionalMnemonics.isEmpty()) {
        result += "Deleted AdditionalMnemonics:\n";
        for (const AdditionalMnemonic &mnemonic : modifiedData.deletedAdditionalMnemonics) {
            result += QString(" - AdditionalMnemonic ID: %1\n").arg(-mnemonic.getId());
        }
    }

    // Обработка MainMnemonics
    if (!modifiedData.modifiedMainMnemonics.isEmpty()) {
        result += "Modified MainMnemonics:\n";
        for (const MainMnemonic &mnemonic : modifiedData.modifiedMainMnemonics) {
            result += QString(" - MainMnemonic ID: %1\n").arg(mnemonic.getId());
        }
    }
    if (!modifiedData.deletedMainMnemonics.isEmpty()) {
        result += "Deleted MainMnemonics:\n";
        for (const MainMnemonic &mnemonic : modifiedData.deletedMainMnemonics) {
            result += QString(" - MainMnemonic ID: %1\n").arg(-mnemonic.getId());
        }
    }

    // Обработка ConversionFormulas
    if (!modifiedData.modifiedConversionFormulas.isEmpty()) {
        result += "Modified ConversionFormulas:\n";
        for (const ConversionFormula &formula : modifiedData.modifiedConversionFormulas) {
            result += QString(" - ConversionFormula ID: %1\n").arg(formula.getId());
        }
    }
    if (!modifiedData.deletedConversionFormulas.isEmpty()) {
        result += "Deleted ConversionFormulas:\n";
        for (const ConversionFormula &formula : modifiedData.deletedConversionFormulas) {
            result += QString(" - ConversionFormula ID: %1\n").arg(-formula.getId());
        }
    }

    // Обработка Companies
    if (!modifiedData.modifiedCompanies.isEmpty()) {
        result += "Modified Companies:\n";
        for (const Company &company : modifiedData.modifiedCompanies) {
            result += QString(" - Company ID: %1\n").arg(company.getId());
        }
    }
    if (!modifiedData.deletedCompanies.isEmpty()) {
        result += "Deleted Companies:\n";
        for (const Company &company : modifiedData.deletedCompanies) {
            result += QString(" - Company ID: %1\n").arg(-company.getId());
        }
    }

    // Обработка Methods
    if (!modifiedData.modifiedMethods.isEmpty()) {
        result += "Modified Methods:\n";
        for (const Method &method : modifiedData.modifiedMethods) {
            result += QString(" - Method ID: %1\n").arg(method.getId());
        }
    }
    if (!modifiedData.deletedMethods.isEmpty()) {
        result += "Deleted Methods:\n";
        for (const Method &method : modifiedData.deletedMethods) {
            result += QString(" - Method ID: %1\n").arg(-method.getId());
        }
    }

    // Обработка ToolDescriptions
    if (!modifiedData.modifiedToolDescriptions.isEmpty()) {
        result += "Modified ToolDescriptions:\n";
        for (const ToolDescription &description : modifiedData.modifiedToolDescriptions) {
            result += QString(" - ToolDescription ID: %1\n").arg(description.getId());
        }
    }
    if (!modifiedData.deletedToolDescriptions.isEmpty()) {
        result += "Deleted ToolDescriptions:\n";
        for (const ToolDescription &description : modifiedData.deletedToolDescriptions) {
            result += QString(" - ToolDescription ID: %1\n").arg(-description.getId());
        }
    }

    // Проверяем, есть ли изменения
    if (result.isEmpty()) {
        result = "No changes detected.";
    }

    // Выводим результаты в сообщении
    QMessageBox::information(this, "Modified and Deleted Data", result);
}
