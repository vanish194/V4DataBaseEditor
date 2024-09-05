#ifndef STORAGE_H
#define STORAGE_H

#include <QDebug>
#include <QList>
#include "db_model/AdditionalMnemonic.h"
#include "db_model/Company.h"
#include "db_model/ConversionFormula.h"
#include "db_model/MainMnemonic.h"
#include "db_model/Method.h"
#include "db_model/Producer.h"
#include "db_model/Sensor.h"
#include "db_model/Tool.h"
#include "db_model/ToolDescription.h"
#include "db_model/ToolSensor.h"
#include "db_model/TypeOfUnit.h"
#include "db_model/Unit.h"

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
    QList<TypeOfUnit> typesOfUnits;
    QList<ToolSensor> toolSensors;
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
    QList<TypeOfUnit> backupTypesOfUnits;
    QList<ToolSensor> backupToolSensors;
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
    int maxTypeOfUnitId = 0;
    int maxToolSensorId = 0;
    int maxAdditionalMnemonicId = 0;
    int maxMainMnemonicId = 0;
    int maxConversionFormulaId = 0;
    int maxCompanyId = 0;
    int maxMethodId = 0;
    int maxToolDescriptionId = 0;

public:
    static Storage *getInstance();
    ~Storage();
    //1.удаление(значений с отрацательным id) -> добавление/редактивование
    // Методы для работы с данными
    QList<Unit> &getUnits();
    QList<Tool> &getTools();
    QList<Sensor> &getSensors();
    QList<Producer> &getProducers();
    QList<TypeOfUnit> &getTypesOfUnits();
    QList<ToolSensor> &getToolSensors();
    QList<AdditionalMnemonic> &getAdditionalMnemonics();
    QList<MainMnemonic> &getMainMnemonics();
    QList<ConversionFormula> &getConversionFormulas();
    QList<Company> &getCompanies();
    QList<Method> &getMethods();
    QList<ToolDescription> &getToolDescriptions();

    // Методы для работы с резервными копиями данных
    void createBackup();
    void restoreFromBackup();
    void clearBackup();

    // Методы для получения данных из резервной копии
    QList<Unit> &getBackupUnits();
    QList<Tool> &getBackupTools();
    QList<Sensor> &getBackupSensors();
    QList<Producer> &getBackupProducers();
    QList<TypeOfUnit> &getBackupTypesOfUnits();
    QList<ToolSensor> &getBackupToolSensors();
    QList<AdditionalMnemonic> &getBackupAdditionalMnemonics();
    QList<MainMnemonic> &getBackupMainMnemonics();
    QList<ConversionFormula> &getBackupConversionFormulas();
    QList<Company> &getBackupCompanies();
    QList<Method> &getBackupMethods();
    QList<ToolDescription> &getBackupToolDescriptions();

    // Методы для сравнения данных
    QList<Unit> getModifiedUnits();
    QList<Tool> getModifiedTools();
    QList<Sensor> getModifiedSensors();
    QList<Producer> getModifiedProducers();
    QList<TypeOfUnit> getModifiedTypesOfUnits();
    QList<ToolSensor> getModifiedToolSensors();
    QList<AdditionalMnemonic> getModifiedAdditionalMnemonics();
    QList<MainMnemonic> getModifiedMainMnemonics();
    QList<ConversionFormula> getModifiedConversionFormulas();
    QList<Company> getModifiedCompanies();
    QList<Method> getModifiedMethods();
    QList<ToolDescription> getModifiedToolDescriptions();

    // Методы для получения максимальных ID
    int generateNewUnitId();
    int generateNewToolId();
    int generateNewSensorId();
    int generateNewProducerId();
    int generateNewTypeOfUnitId();
    int generateNewToolSensorId();
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
    void setMaxTypeOfUnitId(int id);
    void setMaxToolSensorId(int id);
    void setMaxAdditionalMnemonicId(int id);
    void setMaxMainMnemonicId(int id);
    void setMaxConversionFormulaId(int id);
    void setMaxCompanyId(int id);
    void setMaxMethodId(int id);
    void setMaxToolDescriptionId(int id);

    Storage(const Storage &) = delete;
    Storage &operator=(const Storage &) = delete;
};

#endif // STORAGE_H
