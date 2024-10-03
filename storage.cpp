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

bool Storage::isDataLoaded()
{
    // Checking if any data has been uploaded
    return !units.isEmpty() || !tools.isEmpty() || !sensors.isEmpty() || !producers.isEmpty()
           || !unitTypes.isEmpty() || !toolSensors.isEmpty() || !additionalMnemonics.isEmpty()
           || !mainMnemonics.isEmpty() || !conversionFormulas.isEmpty() || !companies.isEmpty()
           || !methods.isEmpty() || !toolDescriptions.isEmpty();
}

// Data access methods
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

// Methods for searching for objects by ID
const Tool *Storage::findToolById(int id) const
{
    for (const Tool &tool : tools) {
        if (tool.getId() == id && tool.getId() > 0) {
            return &tool;
        }
    }
    return nullptr;
}

const Sensor *Storage::findSensorById(int id) const
{
    for (const Sensor &sensor : sensors) {
        if (sensor.getId() == id && sensor.getId() > 0) {
            return &sensor;
        }
    }
    return nullptr;
}

const MainMnemonic *Storage::findMainMnemonicById(int id) const
{
    for (const MainMnemonic &mnemonic : mainMnemonics) {
        if (mnemonic.getId() == id && mnemonic.getId() > 0) {
            return &mnemonic;
        }
    }
    return nullptr;
}

const AdditionalMnemonic *Storage::findAdditionalMnemonicById(int id) const
{
    for (const AdditionalMnemonic &mnemonic : additionalMnemonics) {
        if (mnemonic.getId() == id && mnemonic.getId() > 0) {
            return &mnemonic;
        }
    }
    return nullptr;
}

const ToolDescription *Storage::findToolDescriptionById(int id) const
{
    for (const ToolDescription &desc : toolDescriptions) {
        if (desc.getId() == id && desc.getId() > 0) {
            return &desc;
        }
    }
    return nullptr;
}

const Company *Storage::findCompanyById(int id) const
{
    for (const Company &company : companies) {
        if (company.getId() == id && company.getId() > 0) {
            return &company;
        }
    }
    return nullptr;
}

const Producer *Storage::findProducerById(int id) const
{
    for (const Producer &producer : producers) {
        if (producer.getId() == id && producer.getId() > 0) {
            return &producer;
        }
    }
    return nullptr;
}

const Method *Storage::findMethodById(int id) const
{
    for (const Method &method : methods) {
        if (method.getId() == id && method.getId() > 0) {
            return &method;
        }
    }
    return nullptr;
}
// Methods of working with backups
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
}

void Storage::restoreFromBackup()
{
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
}

// Methods of accessing backup data
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

// Template functions for getting added, modified, and deleted data
template<typename T>
QList<T> getAddedData(const QList<T> &dataList, const QList<T> &backupList)
{
    QList<T> addedData;

    for (const T &item : dataList) {
        if (item.getId() < 0) {
            // Deleted objects are processed separately
            continue;
        }

        bool foundInBackup = false;
        for (const T &backupItem : backupList) {
            if (item.getId() == backupItem.getId()) {
                foundInBackup = true;
                break;
            }
        }
        if (!foundInBackup) {
            // New object
            addedData.append(item);
        }
    }

    return addedData;
}

template<typename T>
QList<T> getModifiedData(const QList<T> &dataList, const QList<T> &backupList)
{
    QList<T> modifiedData;

    for (const T &item : dataList) {
        if (item.getId() < 0) {
            // Deleted objects are processed separately
            continue;
        }

        for (const T &backupItem : backupList) {
            if (item.getId() == backupItem.getId()) {
                if (!(item == backupItem)) {
                    // The object has been changed
                    modifiedData.append(item);
                }
                break;
            }
        }
    }

    return modifiedData;
}

template<typename T>
QList<T> getDeletedData(const QList<T> &dataList)
{
    QList<T> deletedData;

    for (const T &item : dataList) {
        if (item.getId() < 0) {
            deletedData.append(item);
        }
    }

    return deletedData;
}

// Methods for obtaining added data
QList<Unit> Storage::getAddedUnits()
{
    return getAddedData(units, backupUnits);
}

QList<Tool> Storage::getAddedTools()
{
    return getAddedData(tools, backupTools);
}

QList<Sensor> Storage::getAddedSensors()
{
    return getAddedData(sensors, backupSensors);
}

QList<Producer> Storage::getAddedProducers()
{
    return getAddedData(producers, backupProducers);
}

QList<UnitType> Storage::getAddedUnitTypes()
{
    return getAddedData(unitTypes, backupUnitTypes);
}

QList<ToolSensor> Storage::getAddedToolSensors()
{
    return getAddedData(toolSensors, backupToolSensors);
}

QList<AdditionalMnemonic> Storage::getAddedAdditionalMnemonics()
{
    return getAddedData(additionalMnemonics, backupAdditionalMnemonics);
}

QList<MainMnemonic> Storage::getAddedMainMnemonics()
{
    return getAddedData(mainMnemonics, backupMainMnemonics);
}

QList<ConversionFormula> Storage::getAddedConversionFormulas()
{
    return getAddedData(conversionFormulas, backupConversionFormulas);
}

QList<Company> Storage::getAddedCompanies()
{
    return getAddedData(companies, backupCompanies);
}

QList<Method> Storage::getAddedMethods()
{
    return getAddedData(methods, backupMethods);
}

QList<ToolDescription> Storage::getAddedToolDescriptions()
{
    return getAddedData(toolDescriptions, backupToolDescriptions);
}

// Methods for obtaining modified data
QList<Unit> Storage::getModifiedUnits()
{
    return getModifiedData(units, backupUnits);
}

QList<Tool> Storage::getModifiedTools()
{
    return getModifiedData(tools, backupTools);
}

QList<Sensor> Storage::getModifiedSensors()
{
    return getModifiedData(sensors, backupSensors);
}

QList<Producer> Storage::getModifiedProducers()
{
    return getModifiedData(producers, backupProducers);
}

QList<UnitType> Storage::getModifiedUnitTypes()
{
    return getModifiedData(unitTypes, backupUnitTypes);
}

QList<ToolSensor> Storage::getModifiedToolSensors()
{
    return getModifiedData(toolSensors, backupToolSensors);
}

QList<AdditionalMnemonic> Storage::getModifiedAdditionalMnemonics()
{
    return getModifiedData(additionalMnemonics, backupAdditionalMnemonics);
}

QList<MainMnemonic> Storage::getModifiedMainMnemonics()
{
    return getModifiedData(mainMnemonics, backupMainMnemonics);
}

QList<ConversionFormula> Storage::getModifiedConversionFormulas()
{
    return getModifiedData(conversionFormulas, backupConversionFormulas);
}

QList<Company> Storage::getModifiedCompanies()
{
    return getModifiedData(companies, backupCompanies);
}

QList<Method> Storage::getModifiedMethods()
{
    return getModifiedData(methods, backupMethods);
}

QList<ToolDescription> Storage::getModifiedToolDescriptions()
{
    return getModifiedData(toolDescriptions, backupToolDescriptions);
}

// Methods for obtaining deleted data
QList<Unit> Storage::getDeletedUnits()
{
    return getDeletedData(units);
}

QList<Tool> Storage::getDeletedTools()
{
    return getDeletedData(tools);
}

QList<Sensor> Storage::getDeletedSensors()
{
    return getDeletedData(sensors);
}

QList<Producer> Storage::getDeletedProducers()
{
    return getDeletedData(producers);
}

QList<UnitType> Storage::getDeletedUnitTypes()
{
    return getDeletedData(unitTypes);
}

QList<ToolSensor> Storage::getDeletedToolSensors()
{
    return getDeletedData(toolSensors);
}

QList<AdditionalMnemonic> Storage::getDeletedAdditionalMnemonics()
{
    return getDeletedData(additionalMnemonics);
}

QList<MainMnemonic> Storage::getDeletedMainMnemonics()
{
    return getDeletedData(mainMnemonics);
}

QList<ConversionFormula> Storage::getDeletedConversionFormulas()
{
    return getDeletedData(conversionFormulas);
}

QList<Company> Storage::getDeletedCompanies()
{
    return getDeletedData(companies);
}

QList<Method> Storage::getDeletedMethods()
{
    return getDeletedData(methods);
}

QList<ToolDescription> Storage::getDeletedToolDescriptions()
{
    return getDeletedData(toolDescriptions);
}

// The method for getting all the changed data
ModifiedData Storage::getAllModifiedData()
{
    ModifiedData modifiedData;

    // Added objects
    modifiedData.addedUnits = getAddedUnits();
    modifiedData.addedTools = getAddedTools();
    modifiedData.addedSensors = getAddedSensors();
    modifiedData.addedProducers = getAddedProducers();
    modifiedData.addedUnitTypes = getAddedUnitTypes();
    modifiedData.addedToolSensors = getAddedToolSensors();
    modifiedData.addedAdditionalMnemonics = getAddedAdditionalMnemonics();
    modifiedData.addedMainMnemonics = getAddedMainMnemonics();
    modifiedData.addedConversionFormulas = getAddedConversionFormulas();
    modifiedData.addedCompanies = getAddedCompanies();
    modifiedData.addedMethods = getAddedMethods();
    modifiedData.addedToolDescriptions = getAddedToolDescriptions();

    // Modified objects
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

    // Deleted objects
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

// Methods for generating new IDs
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

// Methods for setting maximum IDs when uploading data
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
