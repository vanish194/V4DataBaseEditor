#ifndef STORAGE_H
#define STORAGE_H

#include <QList>
#include "db_model/AdditionalMnemonic.h"
#include "db_model/Company.h"
#include "db_model/ConversionFormula.h"
#include "db_model/MainMnemonic.h"
#include "db_model/Method.h"
#include "db_model/Producer.h"
#include "db_model/Sensor.h"
#include "db_model/SensorMnemonic.h"
#include "db_model/Tool.h"
#include "db_model/ToolDescription.h"
#include "db_model/ToolMnemonic.h"
#include "db_model/ToolSensor.h"
#include "db_model/Unit.h"
#include "db_model/UnitType.h"

// Структура для хранения всех измененных данных
struct ModifiedData
{
    // Списки добавленных объектов
    QList<Unit> addedUnits;
    QList<Tool> addedTools;
    QList<Sensor> addedSensors;
    QList<Producer> addedProducers;
    QList<UnitType> addedUnitTypes;
    QList<ToolSensor> addedToolSensors;
    QList<ToolMnemonic> addedToolMnemonics;
    QList<SensorMnemonic> addedSensorMnemonics;
    QList<AdditionalMnemonic> addedAdditionalMnemonics;
    QList<MainMnemonic> addedMainMnemonics;
    QList<ConversionFormula> addedConversionFormulas;
    QList<Company> addedCompanies;
    QList<Method> addedMethods;
    QList<ToolDescription> addedToolDescriptions;

    // Списки измененных объектов
    QList<Unit> modifiedUnits;
    QList<Tool> modifiedTools;
    QList<Sensor> modifiedSensors;
    QList<Producer> modifiedProducers;
    QList<UnitType> modifiedUnitTypes;
    QList<ToolSensor> modifiedToolSensors;
    QList<ToolMnemonic> modifiedToolMnemonics;
    QList<SensorMnemonic> modifiedSensorMnemonics;
    QList<AdditionalMnemonic> modifiedAdditionalMnemonics;
    QList<MainMnemonic> modifiedMainMnemonics;
    QList<ConversionFormula> modifiedConversionFormulas;
    QList<Company> modifiedCompanies;
    QList<Method> modifiedMethods;
    QList<ToolDescription> modifiedToolDescriptions;

    // Списки удаленных объектов
    QList<Unit> deletedUnits;
    QList<Tool> deletedTools;
    QList<Sensor> deletedSensors;
    QList<Producer> deletedProducers;
    QList<UnitType> deletedUnitTypes;
    QList<ToolSensor> deletedToolSensors;
    QList<ToolMnemonic> deletedToolMnemonics;
    QList<SensorMnemonic> deletedSensorMnemonics;
    QList<AdditionalMnemonic> deletedAdditionalMnemonics;
    QList<MainMnemonic> deletedMainMnemonics;
    QList<ConversionFormula> deletedConversionFormulas;
    QList<Company> deletedCompanies;
    QList<Method> deletedMethods;
    QList<ToolDescription> deletedToolDescriptions;
};

class Storage
{
private:
    static Storage *instance;
    Storage();

    // Основные данные
    QList<Unit> units;
    QList<Tool> tools;
    QList<Sensor> sensors;
    QList<Producer> producers;
    QList<UnitType> unitTypes;
    QList<ToolSensor> toolSensors;
    QList<ToolMnemonic> toolMnemonics;
    QList<SensorMnemonic> sensorMnemonics;
    QList<AdditionalMnemonic> additionalMnemonics;
    QList<MainMnemonic> mainMnemonics;
    QList<ConversionFormula> conversionFormulas;
    QList<Company> companies;
    QList<Method> methods;
    QList<ToolDescription> toolDescriptions;

    // Резервные копии данных
    QList<Unit> backupUnits;
    QList<Tool> backupTools;
    QList<Sensor> backupSensors;
    QList<Producer> backupProducers;
    QList<UnitType> backupUnitTypes;
    QList<ToolSensor> backupToolSensors;
    QList<ToolMnemonic> backupToolMnemonics;
    QList<SensorMnemonic> backupSensorMnemonics;
    QList<AdditionalMnemonic> backupAdditionalMnemonics;
    QList<MainMnemonic> backupMainMnemonics;
    QList<ConversionFormula> backupConversionFormulas;
    QList<Company> backupCompanies;
    QList<Method> backupMethods;
    QList<ToolDescription> backupToolDescriptions;

    // Переменные для хранения максимальных ID
    int maxUnitId = 0;
    int maxToolId = 0;
    int maxSensorId = 0;
    int maxProducerId = 0;
    int maxUnitTypeId = 0;
    int maxToolSensorId = 0;
    int maxToolMnemonicId = 0;
    int maxSensorMnemonicId = 0;
    int maxAdditionalMnemonicId = 0;
    int maxMainMnemonicId = 0;
    int maxConversionFormulaId = 0;
    int maxCompanyId = 0;
    int maxMethodId = 0;
    int maxToolDescriptionId = 0;

public:
    static Storage *getInstance();
    ~Storage();

    bool isConnected = false;
    bool isDataLoaded();

    // Методы доступа к данным
    QList<Unit> &getUnits();
    QList<Tool> &getTools();
    QList<Sensor> &getSensors();
    QList<Producer> &getProducers();
    QList<UnitType> &getUnitTypes();
    QList<ToolSensor> &getToolSensors();
    QList<ToolMnemonic> &getToolMnemonics();
    QList<SensorMnemonic> &getSensorMnemonics();
    QList<AdditionalMnemonic> &getAdditionalMnemonics();
    QList<MainMnemonic> &getMainMnemonics();
    QList<ConversionFormula> &getConversionFormulas();
    QList<Company> &getCompanies();
    QList<Method> &getMethods();
    QList<ToolDescription> &getToolDescriptions();

    // Методы поиска объектов по ID
    const Tool *findToolById(int id) const;
    const Sensor *findSensorById(int id) const;
    const MainMnemonic *findMainMnemonicById(int id) const;
    const ToolMnemonic *findToolMnemonicById(int id) const;
    const SensorMnemonic *findSensorMnemonicById(int id) const;
    const AdditionalMnemonic *findAdditionalMnemonicById(int id) const;
    const ToolDescription *findToolDescriptionById(int id) const;
    const Company *findCompanyById(int id) const;
    const Producer *findProducerById(int id) const;
    const Method *findMethodById(int id) const;

    // Методы работы с резервными копиями
    void createBackup();
    void restoreFromBackup();
    void clearBackup();

    // Методы доступа к резервным данным
    QList<Unit> &getBackupUnits();
    QList<Tool> &getBackupTools();
    QList<Sensor> &getBackupSensors();
    QList<Producer> &getBackupProducers();
    QList<UnitType> &getBackupUnitTypes();
    QList<ToolSensor> &getBackupToolSensors();
    QList<ToolMnemonic> &getBackupToolMnemonics();
    QList<SensorMnemonic> &getBackupSensorMnemonics();
    QList<AdditionalMnemonic> &getBackupAdditionalMnemonics();
    QList<MainMnemonic> &getBackupMainMnemonics();
    QList<ConversionFormula> &getBackupConversionFormulas();
    QList<Company> &getBackupCompanies();
    QList<Method> &getBackupMethods();
    QList<ToolDescription> &getBackupToolDescriptions();

    // Методы получения добавленных данных
    QList<Unit> getAddedUnits();
    QList<Tool> getAddedTools();
    QList<Sensor> getAddedSensors();
    QList<Producer> getAddedProducers();
    QList<UnitType> getAddedUnitTypes();
    QList<ToolSensor> getAddedToolSensors();
    QList<ToolMnemonic> getAddedToolMnemonics();
    QList<SensorMnemonic> getAddedSensorMnemonics();
    QList<AdditionalMnemonic> getAddedAdditionalMnemonics();
    QList<MainMnemonic> getAddedMainMnemonics();
    QList<ConversionFormula> getAddedConversionFormulas();
    QList<Company> getAddedCompanies();
    QList<Method> getAddedMethods();
    QList<ToolDescription> getAddedToolDescriptions();

    // Методы получения измененных данных
    QList<Unit> getModifiedUnits();
    QList<Tool> getModifiedTools();
    QList<Sensor> getModifiedSensors();
    QList<Producer> getModifiedProducers();
    QList<UnitType> getModifiedUnitTypes();
    QList<ToolSensor> getModifiedToolSensors();
    QList<ToolMnemonic> getModifiedToolMnemonics();
    QList<SensorMnemonic> getModifiedSensorMnemonics();
    QList<AdditionalMnemonic> getModifiedAdditionalMnemonics();
    QList<MainMnemonic> getModifiedMainMnemonics();
    QList<ConversionFormula> getModifiedConversionFormulas();
    QList<Company> getModifiedCompanies();
    QList<Method> getModifiedMethods();
    QList<ToolDescription> getModifiedToolDescriptions();

    // Методы получения удаленных данных
    QList<Unit> getDeletedUnits();
    QList<Tool> getDeletedTools();
    QList<Sensor> getDeletedSensors();
    QList<Producer> getDeletedProducers();
    QList<UnitType> getDeletedUnitTypes();
    QList<ToolSensor> getDeletedToolSensors();
    QList<ToolMnemonic> getDeletedToolMnemonics();
    QList<SensorMnemonic> getDeletedSensorMnemonics();
    QList<AdditionalMnemonic> getDeletedAdditionalMnemonics();
    QList<MainMnemonic> getDeletedMainMnemonics();
    QList<ConversionFormula> getDeletedConversionFormulas();
    QList<Company> getDeletedCompanies();
    QList<Method> getDeletedMethods();
    QList<ToolDescription> getDeletedToolDescriptions();

    // Метод для получения всех измененных данных
    ModifiedData getAllModifiedData();

    // Методы для генерации новых ID
    int generateNewUnitId();
    int generateNewToolId();
    int generateNewSensorId();
    int generateNewProducerId();
    int generateNewUnitTypeId();
    int generateNewToolSensorId();
    int generateNewToolMnemonicId();
    int generateNewSensorMnemonicId();
    int generateNewAdditionalMnemonicId();
    int generateNewMainMnemonicId();
    int generateNewConversionFormulaId();
    int generateNewCompanyId();
    int generateNewMethodId();
    int generateNewToolDescriptionId();

    // Методы для установки максимальных ID при загрузке данных
    void setMaxUnitId(int id);
    void setMaxToolId(int id);
    void setMaxSensorId(int id);
    void setMaxProducerId(int id);
    void setMaxUnitTypeId(int id);
    void setMaxToolSensorId(int id);
    void setMaxToolMnemonicId(int id);
    void setMaxSensorMnemonicId(int id);
    void setMaxAdditionalMnemonicId(int id);
    void setMaxMainMnemonicId(int id);
    void setMaxConversionFormulaId(int id);
    void setMaxCompanyId(int id);
    void setMaxMethodId(int id);
    void setMaxToolDescriptionId(int id);

    // Методы для получения максимальных ID
    int getMaxUnitId() const;
    int getMaxToolId() const;
    int getMaxSensorId() const;
    int getMaxProducerId() const;
    int getMaxUnitTypeId() const;
    int getMaxToolSensorId() const;
    int getMaxToolMnemonicId() const;
    int getMaxSensorMnemonicId() const;
    int getMaxAdditionalMnemonicId() const;
    int getMaxMainMnemonicId() const;
    int getMaxConversionFormulaId() const;
    int getMaxCompanyId() const;
    int getMaxMethodId() const;
    int getMaxToolDescriptionId() const;

    // Запрет копирования и присваивания
    Storage(const Storage &) = delete;
    Storage &operator=(const Storage &) = delete;
};

#endif // STORAGE_H
