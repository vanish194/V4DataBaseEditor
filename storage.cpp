#include "storage.h"

Storage *Storage::instance = nullptr;

Storage *Storage::getInstance()
{
    if (instance == nullptr) {
        instance = new Storage();
    }
    return instance;
}

Storage::Storage() {}

Storage::~Storage() {}

// Методы для работы с данными
QList<Unit> &Storage::getUnits()
{
    return units;
}
QList<Tool> &Storage::getTools()
{
    return tools;
}
QList<Sensor> &Storage::getSensors()
{
    return sensors;
}
QList<Producer> &Storage::getProducers()
{
    return producers;
}
QList<UnitType> &Storage::getUnitTypes()
{
    return unitTypes;
}
QList<ToolSensor> &Storage::getToolSensors()
{
    return toolSensors;
}
QList<AdditionalMnemonic> &Storage::getAdditionalMnemonics()
{
    return additionalMnemonics;
}
QList<MainMnemonic> &Storage::getMainMnemonics()
{
    return mainMnemonics;
}
QList<ConversionFormula> &Storage::getConversionFormulas()
{
    return conversionFormulas;
}
QList<Company> &Storage::getCompanies()
{
    return companies;
}
QList<Method> &Storage::getMethods()
{
    return methods;
}
QList<ToolDescription> &Storage::getToolDescriptions()
{
    return toolDescriptions;
}

// Методы для работы с резервными копиями данных
void Storage::createBackup()
{
    backupUnits = units;
    backupTools = tools;
    backupSensors = sensors;
    backupProducers = producers;
    backupUnitTypes = unitTypes;
    backupToolSensors = toolSensors;
    backupAdditionalMnemonics = additionalMnemonics;
    backupMainMnemonics = mainMnemonics;
    backupConversionFormulas = conversionFormulas;
    backupCompanies = companies;
    backupMethods = methods;
    backupToolDescriptions = toolDescriptions;

    qDebug() << "Backup created.";
}

void Storage::restoreFromBackup()
{
    if (backupUnits.isEmpty()) {
        qDebug() << "Backup is empty. Cannot restore.";
        return;
    }

    units = backupUnits;
    tools = backupTools;
    sensors = backupSensors;
    producers = backupProducers;
    unitTypes = backupUnitTypes;
    toolSensors = backupToolSensors;
    additionalMnemonics = backupAdditionalMnemonics;
    mainMnemonics = backupMainMnemonics;
    conversionFormulas = backupConversionFormulas;
    companies = backupCompanies;
    methods = backupMethods;
    toolDescriptions = backupToolDescriptions;

    qDebug() << "Data restored from backup.";
}

void Storage::clearBackup()
{
    backupUnits.clear();
    backupTools.clear();
    backupSensors.clear();
    backupProducers.clear();
    backupUnitTypes.clear();
    backupToolSensors.clear();
    backupAdditionalMnemonics.clear();
    backupMainMnemonics.clear();
    backupConversionFormulas.clear();
    backupCompanies.clear();
    backupMethods.clear();
    backupToolDescriptions.clear();

    qDebug() << "Backup cleared.";
}

// Методы для получения данных из резервной копии
QList<Unit> &Storage::getBackupUnits()
{
    return backupUnits;
}
QList<Tool> &Storage::getBackupTools()
{
    return backupTools;
}
QList<Sensor> &Storage::getBackupSensors()
{
    return backupSensors;
}
QList<Producer> &Storage::getBackupProducers()
{
    return backupProducers;
}
QList<UnitType> &Storage::getBackupUnitTypes()
{
    return backupUnitTypes;
}
QList<ToolSensor> &Storage::getBackupToolSensors()
{
    return backupToolSensors;
}
QList<AdditionalMnemonic> &Storage::getBackupAdditionalMnemonics()
{
    return backupAdditionalMnemonics;
}
QList<MainMnemonic> &Storage::getBackupMainMnemonics()
{
    return backupMainMnemonics;
}
QList<ConversionFormula> &Storage::getBackupConversionFormulas()
{
    return backupConversionFormulas;
}
QList<Company> &Storage::getBackupCompanies()
{
    return backupCompanies;
}
QList<Method> &Storage::getBackupMethods()
{
    return backupMethods;
}
QList<ToolDescription> &Storage::getBackupToolDescriptions()
{
    return backupToolDescriptions;
}

// Пример метода для получения измененных данных
// Метод для получения изменённых Units
QList<Unit> Storage::getModifiedUnits()
{
    QList<Unit> modifiedUnits;

    // Проходим по рабочим данным и сравниваем с данными из резервной копии
    for (const Unit &unit : units) {
        if (!backupUnits.contains(unit)) {
            modifiedUnits.append(unit);
        }
    }

    return modifiedUnits;
}

// Метод для получения изменённых Tools
QList<Tool> Storage::getModifiedTools()
{
    QList<Tool> modifiedTools;

    // Проходим по рабочим данным и сравниваем с данными из резервной копии
    for (const Tool &tool : tools) {
        if (!backupTools.contains(tool)) {
            modifiedTools.append(tool);
        }
    }

    return modifiedTools;
}

// Метод для получения изменённых Sensors
QList<Sensor> Storage::getModifiedSensors()
{
    QList<Sensor> modifiedSensors;

    // Проходим по рабочим данным и сравниваем с данными из резервной копии
    for (const Sensor &sensor : sensors) {
        if (!backupSensors.contains(sensor)) {
            modifiedSensors.append(sensor);
        }
    }

    return modifiedSensors;
}

// Метод для получения изменённых Producers
QList<Producer> Storage::getModifiedProducers()
{
    QList<Producer> modifiedProducers;

    // Проходим по рабочим данным и сравниваем с данными из резервной копии
    for (const Producer &producer : producers) {
        if (!backupProducers.contains(producer)) {
            modifiedProducers.append(producer);
        }
    }

    return modifiedProducers;
}

QList<UnitType> Storage::getModifiedUnitTypes()
{
    QList<UnitType> modifiedUnitTypes;

    // Проходим по рабочим данным и сравниваем с данными из резервной копии
    for (const UnitType &unitType : unitTypes) {
        if (!backupUnitTypes.contains(unitType)) {
            modifiedUnitTypes.append(unitType);
        }
    }

    return modifiedUnitTypes;
}

// Метод для получения изменённых ToolSensors
QList<ToolSensor> Storage::getModifiedToolSensors()
{
    QList<ToolSensor> modifiedToolSensors;

    // Проходим по рабочим данным и сравниваем с данными из резервной копии
    for (const ToolSensor &toolSensor : toolSensors) {
        if (!backupToolSensors.contains(toolSensor)) {
            modifiedToolSensors.append(toolSensor);
        }
    }

    return modifiedToolSensors;
}

// Метод для получения изменённых AdditionalMnemonics
QList<AdditionalMnemonic> Storage::getModifiedAdditionalMnemonics()
{
    QList<AdditionalMnemonic> modifiedAdditionalMnemonics;

    // Проходим по рабочим данным и сравниваем с данными из резервной копии
    for (const AdditionalMnemonic &additionalMnemonic : additionalMnemonics) {
        if (!backupAdditionalMnemonics.contains(additionalMnemonic)) {
            modifiedAdditionalMnemonics.append(additionalMnemonic);
        }
    }

    return modifiedAdditionalMnemonics;
}

// Метод для получения изменённых MainMnemonics
QList<MainMnemonic> Storage::getModifiedMainMnemonics()
{
    QList<MainMnemonic> modifiedMainMnemonics;

    // Проходим по рабочим данным и сравниваем с данными из резервной копии
    for (const MainMnemonic &mainMnemonic : mainMnemonics) {
        if (!backupMainMnemonics.contains(mainMnemonic)) {
            modifiedMainMnemonics.append(mainMnemonic);
        }
    }

    return modifiedMainMnemonics;
}

// Метод для получения изменённых ConversionFormulas
QList<ConversionFormula> Storage::getModifiedConversionFormulas()
{
    QList<ConversionFormula> modifiedConversionFormulas;

    // Проходим по рабочим данным и сравниваем с данными из резервной копии
    for (const ConversionFormula &conversionFormula : conversionFormulas) {
        if (!backupConversionFormulas.contains(conversionFormula)) {
            modifiedConversionFormulas.append(conversionFormula);
        }
    }

    return modifiedConversionFormulas;
}

// Метод для получения изменённых Companies
QList<Company> Storage::getModifiedCompanies()
{
    QList<Company> modifiedCompanies;

    // Проходим по рабочим данным и сравниваем с данными из резервной копии
    for (const Company &company : companies) {
        if (!backupCompanies.contains(company)) {
            modifiedCompanies.append(company);
        }
    }

    return modifiedCompanies;
}

// Метод для получения изменённых Methods
QList<Method> Storage::getModifiedMethods()
{
    QList<Method> modifiedMethods;

    // Проходим по рабочим данным и сравниваем с данными из резервной копии
    for (const Method &method : methods) {
        if (!backupMethods.contains(method)) {
            modifiedMethods.append(method);
        }
    }

    return modifiedMethods;
}

// Метод для получения изменённых ToolDescriptions
QList<ToolDescription> Storage::getModifiedToolDescriptions()
{
    QList<ToolDescription> modifiedToolDescriptions;

    // Проходим по рабочим данным и сравниваем с данными из резервной копии
    for (const ToolDescription &toolDescription : toolDescriptions) {
        if (!backupToolDescriptions.contains(toolDescription)) {
            modifiedToolDescriptions.append(toolDescription);
        }
    }

    return modifiedToolDescriptions;
}

// Реализация методов генерации новых ID
int Storage::generateNewUnitId()
{
    return ++maxUnitId;
}
int Storage::generateNewToolId()
{
    return ++maxToolId;
}
int Storage::generateNewSensorId()
{
    return ++maxSensorId;
}
int Storage::generateNewProducerId()
{
    return ++maxProducerId;
}
int Storage::generateNewUnitTypeId()
{
    return ++maxUnitTypeId;
}
int Storage::generateNewToolSensorId()
{
    return ++maxToolSensorId;
}
int Storage::generateNewAdditionalMnemonicId()
{
    return ++maxAdditionalMnemonicId;
}
int Storage::generateNewMainMnemonicId()
{
    return ++maxMainMnemonicId;
}
int Storage::generateNewConversionFormulaId()
{
    return ++maxConversionFormulaId;
}
int Storage::generateNewCompanyId()
{
    return ++maxCompanyId;
}
int Storage::generateNewMethodId()
{
    return ++maxMethodId;
}
int Storage::generateNewToolDescriptionId()
{
    return ++maxToolDescriptionId;
}

// Реализация методов для установки максимальных ID
void Storage::setMaxUnitId(int id)
{
    maxUnitId = id;
}
void Storage::setMaxToolId(int id)
{
    maxToolId = id;
}
void Storage::setMaxSensorId(int id)
{
    maxSensorId = id;
}
void Storage::setMaxProducerId(int id)
{
    maxProducerId = id;
}
void Storage::setMaxUnitTypeId(int id)
{
    maxUnitTypeId = id;
}
void Storage::setMaxToolSensorId(int id)
{
    maxToolSensorId = id;
}
void Storage::setMaxAdditionalMnemonicId(int id)
{
    maxAdditionalMnemonicId = id;
}
void Storage::setMaxMainMnemonicId(int id)
{
    maxMainMnemonicId = id;
}
void Storage::setMaxConversionFormulaId(int id)
{
    maxConversionFormulaId = id;
}
void Storage::setMaxCompanyId(int id)
{
    maxCompanyId = id;
}
void Storage::setMaxMethodId(int id)
{
    maxMethodId = id;
}
void Storage::setMaxToolDescriptionId(int id)
{
    maxToolDescriptionId = id;
}
