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

// Методы для получения измененных данных

// Пример для Unit
QList<Unit> Storage::getModifiedUnits()
{
    QList<Unit> modifiedUnits;

    for (const Unit &unit : units) {
        if (unit.getId() < 0) {
            // Удаленные объекты обрабатываются отдельно
            continue;
        }

        bool foundInBackup = false;
        for (const Unit &backupUnit : backupUnits) {
            if (unit.getId() == backupUnit.getId()) {
                if (!(unit == backupUnit)) {
                    // Объект изменен
                    modifiedUnits.append(unit);
                }
                foundInBackup = true;
                break;
            }
        }
        if (!foundInBackup) {
            // Новый объект
            modifiedUnits.append(unit);
        }
    }

    return modifiedUnits;
}

// Аналогично для остальных типов данных

QList<Tool> Storage::getModifiedTools()
{
    QList<Tool> modifiedTools;

    for (const Tool &tool : tools) {
        if (tool.getId() < 0) {
            continue;
        }

        bool foundInBackup = false;
        for (const Tool &backupTool : backupTools) {
            if (tool.getId() == backupTool.getId()) {
                if (!(tool == backupTool)) {
                    modifiedTools.append(tool);
                }
                foundInBackup = true;
                break;
            }
        }
        if (!foundInBackup) {
            modifiedTools.append(tool);
        }
    }

    return modifiedTools;
}

QList<Sensor> Storage::getModifiedSensors()
{
    QList<Sensor> modifiedSensors;

    for (const Sensor &sensor : sensors) {
        if (sensor.getId() < 0) {
            continue;
        }

        bool foundInBackup = false;
        for (const Sensor &backupSensor : backupSensors) {
            if (sensor.getId() == backupSensor.getId()) {
                if (!(sensor == backupSensor)) {
                    modifiedSensors.append(sensor);
                }
                foundInBackup = true;
                break;
            }
        }
        if (!foundInBackup) {
            modifiedSensors.append(sensor);
        }
    }

    return modifiedSensors;
}

QList<Producer> Storage::getModifiedProducers()
{
    QList<Producer> modifiedProducers;

    for (const Producer &producer : producers) {
        if (producer.getId() < 0) {
            continue;
        }

        bool foundInBackup = false;
        for (const Producer &backupProducer : backupProducers) {
            if (producer.getId() == backupProducer.getId()) {
                if (!(producer == backupProducer)) {
                    modifiedProducers.append(producer);
                }
                foundInBackup = true;
                break;
            }
        }
        if (!foundInBackup) {
            modifiedProducers.append(producer);
        }
    }

    return modifiedProducers;
}

QList<UnitType> Storage::getModifiedUnitTypes()
{
    QList<UnitType> modifiedUnitTypes;

    for (const UnitType &unitType : unitTypes) {
        if (unitType.getId() < 0) {
            continue;
        }

        bool foundInBackup = false;
        for (const UnitType &backupUnitType : backupUnitTypes) {
            if (unitType.getId() == backupUnitType.getId()) {
                if (!(unitType == backupUnitType)) {
                    modifiedUnitTypes.append(unitType);
                }
                foundInBackup = true;
                break;
            }
        }
        if (!foundInBackup) {
            modifiedUnitTypes.append(unitType);
        }
    }

    return modifiedUnitTypes;
}

QList<ToolSensor> Storage::getModifiedToolSensors()
{
    QList<ToolSensor> modifiedToolSensors;

    for (const ToolSensor &toolSensor : toolSensors) {
        if (toolSensor.getId() < 0) {
            continue;
        }

        bool foundInBackup = false;
        for (const ToolSensor &backupToolSensor : backupToolSensors) {
            if (toolSensor.getId() == backupToolSensor.getId()) {
                if (!(toolSensor == backupToolSensor)) {
                    modifiedToolSensors.append(toolSensor);
                }
                foundInBackup = true;
                break;
            }
        }
        if (!foundInBackup) {
            modifiedToolSensors.append(toolSensor);
        }
    }

    return modifiedToolSensors;
}

QList<AdditionalMnemonic> Storage::getModifiedAdditionalMnemonics()
{
    QList<AdditionalMnemonic> modifiedAdditionalMnemonics;

    for (const AdditionalMnemonic &additionalMnemonic : additionalMnemonics) {
        if (additionalMnemonic.getId() < 0) {
            continue;
        }

        bool foundInBackup = false;
        for (const AdditionalMnemonic &backupAdditionalMnemonic : backupAdditionalMnemonics) {
            if (additionalMnemonic.getId() == backupAdditionalMnemonic.getId()) {
                if (!(additionalMnemonic == backupAdditionalMnemonic)) {
                    modifiedAdditionalMnemonics.append(additionalMnemonic);
                }
                foundInBackup = true;
                break;
            }
        }
        if (!foundInBackup) {
            modifiedAdditionalMnemonics.append(additionalMnemonic);
        }
    }

    return modifiedAdditionalMnemonics;
}

QList<MainMnemonic> Storage::getModifiedMainMnemonics()
{
    QList<MainMnemonic> modifiedMainMnemonics;

    for (const MainMnemonic &mainMnemonic : mainMnemonics) {
        if (mainMnemonic.getId() < 0) {
            continue;
        }

        bool foundInBackup = false;
        for (const MainMnemonic &backupMainMnemonic : backupMainMnemonics) {
            if (mainMnemonic.getId() == backupMainMnemonic.getId()) {
                if (!(mainMnemonic == backupMainMnemonic)) {
                    modifiedMainMnemonics.append(mainMnemonic);
                }
                foundInBackup = true;
                break;
            }
        }
        if (!foundInBackup) {
            modifiedMainMnemonics.append(mainMnemonic);
        }
    }

    return modifiedMainMnemonics;
}

QList<ConversionFormula> Storage::getModifiedConversionFormulas()
{
    QList<ConversionFormula> modifiedConversionFormulas;

    for (const ConversionFormula &conversionFormula : conversionFormulas) {
        if (conversionFormula.getId() < 0) {
            continue;
        }

        bool foundInBackup = false;
        for (const ConversionFormula &backupConversionFormula : backupConversionFormulas) {
            if (conversionFormula.getId() == backupConversionFormula.getId()) {
                if (!(conversionFormula == backupConversionFormula)) {
                    modifiedConversionFormulas.append(conversionFormula);
                }
                foundInBackup = true;
                break;
            }
        }
        if (!foundInBackup) {
            modifiedConversionFormulas.append(conversionFormula);
        }
    }

    return modifiedConversionFormulas;
}

QList<Company> Storage::getModifiedCompanies()
{
    QList<Company> modifiedCompanies;

    for (const Company &company : companies) {
        if (company.getId() < 0) {
            continue;
        }

        bool foundInBackup = false;
        for (const Company &backupCompany : backupCompanies) {
            if (company.getId() == backupCompany.getId()) {
                if (!(company == backupCompany)) {
                    modifiedCompanies.append(company);
                }
                foundInBackup = true;
                break;
            }
        }
        if (!foundInBackup) {
            modifiedCompanies.append(company);
        }
    }

    return modifiedCompanies;
}

QList<Method> Storage::getModifiedMethods()
{
    QList<Method> modifiedMethods;

    for (const Method &method : methods) {
        if (method.getId() < 0) {
            continue;
        }

        bool foundInBackup = false;
        for (const Method &backupMethod : backupMethods) {
            if (method.getId() == backupMethod.getId()) {
                if (!(method == backupMethod)) {
                    modifiedMethods.append(method);
                }
                foundInBackup = true;
                break;
            }
        }
        if (!foundInBackup) {
            modifiedMethods.append(method);
        }
    }

    return modifiedMethods;
}

QList<ToolDescription> Storage::getModifiedToolDescriptions()
{
    QList<ToolDescription> modifiedToolDescriptions;

    for (const ToolDescription &toolDescription : toolDescriptions) {
        if (toolDescription.getId() < 0) {
            continue;
        }

        bool foundInBackup = false;
        for (const ToolDescription &backupToolDescription : backupToolDescriptions) {
            if (toolDescription.getId() == backupToolDescription.getId()) {
                if (!(toolDescription == backupToolDescription)) {
                    modifiedToolDescriptions.append(toolDescription);
                }
                foundInBackup = true;
                break;
            }
        }
        if (!foundInBackup) {
            modifiedToolDescriptions.append(toolDescription);
        }
    }

    return modifiedToolDescriptions;
}

// Методы для получения удаленных данных

QList<Unit> Storage::getDeletedUnits()
{
    QList<Unit> deletedUnits;

    for (const Unit &unit : units) {
        if (unit.getId() < 0) {
            deletedUnits.append(unit);
        }
    }

    return deletedUnits;
}

QList<Tool> Storage::getDeletedTools()
{
    QList<Tool> deletedTools;

    for (const Tool &tool : tools) {
        if (tool.getId() < 0) {
            deletedTools.append(tool);
        }
    }

    return deletedTools;
}

QList<Sensor> Storage::getDeletedSensors()
{
    QList<Sensor> deletedSensors;

    for (const Sensor &sensor : sensors) {
        if (sensor.getId() < 0) {
            deletedSensors.append(sensor);
        }
    }

    return deletedSensors;
}

QList<Producer> Storage::getDeletedProducers()
{
    QList<Producer> deletedProducers;

    for (const Producer &producer : producers) {
        if (producer.getId() < 0) {
            deletedProducers.append(producer);
        }
    }

    return deletedProducers;
}

QList<UnitType> Storage::getDeletedUnitTypes()
{
    QList<UnitType> deletedUnitTypes;

    for (const UnitType &unitType : unitTypes) {
        if (unitType.getId() < 0) {
            deletedUnitTypes.append(unitType);
        }
    }

    return deletedUnitTypes;
}

QList<ToolSensor> Storage::getDeletedToolSensors()
{
    QList<ToolSensor> deletedToolSensors;

    for (const ToolSensor &toolSensor : toolSensors) {
        if (toolSensor.getId() < 0) {
            deletedToolSensors.append(toolSensor);
        }
    }

    return deletedToolSensors;
}

QList<AdditionalMnemonic> Storage::getDeletedAdditionalMnemonics()
{
    QList<AdditionalMnemonic> deletedAdditionalMnemonics;

    for (const AdditionalMnemonic &additionalMnemonic : additionalMnemonics) {
        if (additionalMnemonic.getId() < 0) {
            deletedAdditionalMnemonics.append(additionalMnemonic);
        }
    }

    return deletedAdditionalMnemonics;
}

QList<MainMnemonic> Storage::getDeletedMainMnemonics()
{
    QList<MainMnemonic> deletedMainMnemonics;

    for (const MainMnemonic &mainMnemonic : mainMnemonics) {
        if (mainMnemonic.getId() < 0) {
            deletedMainMnemonics.append(mainMnemonic);
        }
    }

    return deletedMainMnemonics;
}

QList<ConversionFormula> Storage::getDeletedConversionFormulas()
{
    QList<ConversionFormula> deletedConversionFormulas;

    for (const ConversionFormula &conversionFormula : conversionFormulas) {
        if (conversionFormula.getId() < 0) {
            deletedConversionFormulas.append(conversionFormula);
        }
    }

    return deletedConversionFormulas;
}

QList<Company> Storage::getDeletedCompanies()
{
    QList<Company> deletedCompanies;

    for (const Company &company : companies) {
        if (company.getId() < 0) {
            deletedCompanies.append(company);
        }
    }

    return deletedCompanies;
}

QList<Method> Storage::getDeletedMethods()
{
    QList<Method> deletedMethods;

    for (const Method &method : methods) {
        if (method.getId() < 0) {
            deletedMethods.append(method);
        }
    }

    return deletedMethods;
}

QList<ToolDescription> Storage::getDeletedToolDescriptions()
{
    QList<ToolDescription> deletedToolDescriptions;

    for (const ToolDescription &toolDescription : toolDescriptions) {
        if (toolDescription.getId() < 0) {
            deletedToolDescriptions.append(toolDescription);
        }
    }

    return deletedToolDescriptions;
}

// Метод для получения всех измененных данных
ModifiedData Storage::getAllModifiedData()
{
    ModifiedData modifiedData;

    // Добавленные и измененные объекты
    modifiedData.modifiedUnits = getModifiedUnits();
    modifiedData.modifiedTools = getModifiedTools();
    modifiedData.modifiedSensors = getModifiedSensors();
    modifiedData.modifiedProducers = getModifiedProducers();
    modifiedData.modifiedUnitTypes = getModifiedUnitTypes();
    modifiedData.modifiedToolSensors = getModifiedToolSensors();
    modifiedData.modifiedAdditionalMnemonics = getModifiedAdditionalMnemonics();
    modifiedData.modifiedMainMnemonics = getModifiedMainMnemonics();
    modifiedData.modifiedConversionFormulas = getModifiedConversionFormulas();
    modifiedData.modifiedCompanies = getModifiedCompanies();
    modifiedData.modifiedMethods = getModifiedMethods();
    modifiedData.modifiedToolDescriptions = getModifiedToolDescriptions();

    // Удаленные объекты
    modifiedData.deletedUnits = getDeletedUnits();
    modifiedData.deletedTools = getDeletedTools();
    modifiedData.deletedSensors = getDeletedSensors();
    modifiedData.deletedProducers = getDeletedProducers();
    modifiedData.deletedUnitTypes = getDeletedUnitTypes();
    modifiedData.deletedToolSensors = getDeletedToolSensors();
    modifiedData.deletedAdditionalMnemonics = getDeletedAdditionalMnemonics();
    modifiedData.deletedMainMnemonics = getDeletedMainMnemonics();
    modifiedData.deletedConversionFormulas = getDeletedConversionFormulas();
    modifiedData.deletedCompanies = getDeletedCompanies();
    modifiedData.deletedMethods = getDeletedMethods();
    modifiedData.deletedToolDescriptions = getDeletedToolDescriptions();

    return modifiedData;
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
