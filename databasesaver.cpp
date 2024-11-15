#include "databasesaver.h"
#include <QLocale>
DatabaseSaver::DatabaseSaver(QSqlDatabase &database)
    : db(database)
{
    storage = Storage::getInstance();
}

DatabaseSaver::~DatabaseSaver() {}

bool DatabaseSaver::saveAllData()
{
    bool success = true;

    // Начинаем транзакцию
    if (!db.transaction()) {
        qDebug() << QObject::tr("Start Transaction Eror:") << db.lastError().text();
        return false;
    }

    success &= saveUnitTypes();
    success &= saveUnits();
    success &= saveMethods();
    success &= saveProducers();
    success &= saveSensors();
    success &= saveMainMnemonics();
    success &= saveAdditionalMnemonics();
    success &= saveCompanies();
    success &= saveToolDescriptions();
    success &= saveTools();
    success &= saveToolSensors();
    success &= saveToolMnemonics();
    success &= saveSensorMnemonics();
    success &= saveConversionFormulas();

    if (success) {
        if (!db.commit()) {
            qDebug() << QObject::tr("Commit Transaction Eror:") << db.lastError().text();
            db.rollback();
            return false;
        }
    } else {
        qDebug() << QObject::tr("Error saving data. Transaction rolled back.");
        db.rollback();
    }

    return success;
}

// Вспомогательный шаблонный метод для обработки записей
template<typename T>
bool DatabaseSaver::processItems(const QList<T> &deletedItems,
                                 const QList<T> &modifiedItems,
                                 const QList<T> &addedItems,
                                 const QString &tableName,
                                 const QString &idColumn,
                                 const QStringList &columns,
                                 const std::function<void(const T &, QSqlQuery &)> &bindValues)
{
    QSqlQuery query(db);
    bool success = true;

    // Удаление записей
    for (const T &item : deletedItems) {
        int id = positiveId(item.getId());
        query.prepare(QString("DELETE FROM %1 WHERE %2 = ?").arg(tableName, idColumn));
        query.addBindValue(id);
        if (!query.exec()) {
            qDebug() << QObject::tr("Error deleting from") << tableName << ":"
                     << query.lastError().text();
            success = false;
        }
    }

    // Обновление записей
    for (const T &item : modifiedItems) {
        QStringList setClauses;
        for (const QString &column : columns) {
            setClauses << QString("%1 = ?").arg(column);
        }
        QString sql = QString("UPDATE %1 SET %2 WHERE %3 = ?")
                          .arg(tableName, setClauses.join(", "), idColumn);
        query.prepare(sql);
        bindValues(item, query);
        query.addBindValue(item.getId());
        if (!query.exec()) {
            qDebug() << QObject::tr("Update error in") << tableName << ":"
                     << query.lastError().text();
            success = false;
        }
    }

    // Вставка новых записей
    for (const T &item : addedItems) {
        QStringList placeholders(columns.size() + 1, "?"); // +1 для idColumn
        QString sql = QString("INSERT INTO %1 (%2, %3) VALUES (%4)")
                          .arg(tableName, idColumn, columns.join(", "), placeholders.join(", "));
        query.prepare(sql);
        query.addBindValue(item.getId()); // Привязываем значение для idColumn
        bindValues(item, query); // Привязываем значения для остальных столбцов
        if (!query.exec()) {
            qDebug() << QObject::tr("Error inserting into") << tableName << ":"
                     << query.lastError().text();
            success = false;
        }
    }

    return success;
}

// Реализация методов сохранения для каждой сущности

bool DatabaseSaver::saveUnits()
{
    return processItems<Unit>(storage->getDeletedUnits(),
                              storage->getModifiedUnits(),
                              storage->getAddedUnits(),
                              "units",
                              "unit_id",
                              {"unit_name", "type_id", "unit_short_name"},
                              [](const Unit &unit, QSqlQuery &query) {
                                  query.addBindValue(unit.getName());
                                  query.addBindValue(unit.getTypeId());
                                  query.addBindValue(unit.getShortName());
                              });
}

bool DatabaseSaver::saveUnitTypes()
{
    return processItems<UnitType>(storage->getDeletedUnitTypes(),
                                  storage->getModifiedUnitTypes(),
                                  storage->getAddedUnitTypes(),
                                  "unit_types",
                                  "type_id",
                                  {"type_name"},
                                  [](const UnitType &unitType, QSqlQuery &query) {
                                      query.addBindValue(unitType.getName());
                                  });
}

bool DatabaseSaver::saveMethods()
{
    return processItems<Method>(storage->getDeletedMethods(),
                                storage->getModifiedMethods(),
                                storage->getAddedMethods(),
                                "methods",
                                "method_id",
                                {"method_name"},
                                [](const Method &method, QSqlQuery &query) {
                                    query.addBindValue(method.getName());
                                });
}

bool DatabaseSaver::saveProducers()
{
    return processItems<Producer>(storage->getDeletedProducers(),
                                  storage->getModifiedProducers(),
                                  storage->getAddedProducers(),
                                  "producers",
                                  "producer_id",
                                  {"producer_name"},
                                  [](const Producer &producer, QSqlQuery &query) {
                                      query.addBindValue(producer.getName());
                                  });
}

bool DatabaseSaver::saveSensors()
{
    return processItems<Sensor>(storage->getDeletedSensors(),
                                storage->getModifiedSensors(),
                                storage->getAddedSensors(),
                                "sensors",
                                "sensor_id",
                                {"method_id", "sensor_name", "sensor_description"},
                                [](const Sensor &sensor, QSqlQuery &query) {
                                    query.addBindValue(sensor.getMethodId());
                                    query.addBindValue(sensor.getName());
                                    query.addBindValue(sensor.getDescription());
                                });
}

bool DatabaseSaver::saveMainMnemonics()
{
    return processItems<MainMnemonic>(storage->getDeletedMainMnemonics(),
                                      storage->getModifiedMainMnemonics(),
                                      storage->getAddedMainMnemonics(),
                                      "main_mnemonics",
                                      "main_mnemonic_id",
                                      {"main_mnemonic_name",
                                       "main_mnemonic_description",
                                       "unit_id",
                                       "service"},
                                      [](const MainMnemonic &mnemonic, QSqlQuery &query) {
                                          query.addBindValue(mnemonic.getName());
                                          query.addBindValue(mnemonic.getDescription());
                                          query.addBindValue(mnemonic.getUnitId());
                                          query.addBindValue(mnemonic.isService());
                                      });
}

bool DatabaseSaver::saveAdditionalMnemonics()
{
    return processItems<AdditionalMnemonic>(storage->getDeletedAdditionalMnemonics(),
                                            storage->getModifiedAdditionalMnemonics(),
                                            storage->getAddedAdditionalMnemonics(),
                                            "additional_mnemonics",
                                            "additional_mnemonic_id",
                                            {"additional_mnemonic_name",
                                             "company_id",
                                             "main_mnemonic_id",
                                             "unit_id"},
                                            [](const AdditionalMnemonic &mnemonic,
                                               QSqlQuery &query) {
                                                query.addBindValue(mnemonic.getName());
                                                query.addBindValue(mnemonic.getCompanyId());
                                                query.addBindValue(mnemonic.getMainMnemonicId());
                                                query.addBindValue(mnemonic.getUnitId());
                                            });
}

bool DatabaseSaver::saveCompanies()
{
    return processItems<Company>(storage->getDeletedCompanies(),
                                 storage->getModifiedCompanies(),
                                 storage->getAddedCompanies(),
                                 "companies",
                                 "company_id",
                                 {"company_name"},
                                 [](const Company &company, QSqlQuery &query) {
                                     query.addBindValue(company.getName());
                                 });
}

bool DatabaseSaver::saveToolDescriptions()
{
    return processItems<ToolDescription>(storage->getDeletedToolDescriptions(),
                                         storage->getModifiedToolDescriptions(),
                                         storage->getAddedToolDescriptions(),
                                         "tool_descriptions",
                                         "tool_description_id",
                                         {"description",
                                          "length_mm",
                                          "outer_diameter_mm",
                                          "inner_diameter_mm",
                                          "image",
                                          "producer_id"},
                                         [](const ToolDescription &desc, QSqlQuery &query) {
                                             query.addBindValue(desc.getDescription());
                                             query.addBindValue(desc.getLengthMm());
                                             query.addBindValue(desc.getOuterDiameterMm());
                                             query.addBindValue(desc.getInnerDiameterMm());
                                             query.addBindValue(desc.getImage());
                                             query.addBindValue(desc.getProducerId());
                                         });
}

bool DatabaseSaver::saveTools()
{
    return processItems<Tool>(storage->getDeletedTools(),
                              storage->getModifiedTools(),
                              storage->getAddedTools(),
                              "tools",
                              "tool_id",
                              {"tool_name", "tool_description_id"},
                              [](const Tool &tool, QSqlQuery &query) {
                                  query.addBindValue(tool.getName());
                                  query.addBindValue(tool.getToolDescriptionId());
                              });
}

bool DatabaseSaver::saveToolSensors()
{
    return processItems<ToolSensor>(storage->getDeletedToolSensors(),
                                    storage->getModifiedToolSensors(),
                                    storage->getAddedToolSensors(),
                                    "tools_sensors",
                                    "tool_sensor_id",
                                    {"tool_id", "sensor_id", "offset_mm"},
                                    [](const ToolSensor &toolSensor, QSqlQuery &query) {
                                        query.addBindValue(toolSensor.getToolId());
                                        query.addBindValue(toolSensor.getSensorId());
                                        query.addBindValue(toolSensor.getOffsetMm());
                                    });
}

bool DatabaseSaver::saveToolMnemonics()
{
    return processItems<ToolMnemonic>(storage->getDeletedToolMnemonics(),
                                      storage->getModifiedToolMnemonics(),
                                      storage->getAddedToolMnemonics(),
                                      "tools_mnemonics",
                                      "tool_mnemonic_id",
                                      {"tool_id", "mnemonic_id", "offset_mm"},
                                      [](const ToolMnemonic &toolMnemonic, QSqlQuery &query) {
                                          query.addBindValue(toolMnemonic.getToolId());
                                          query.addBindValue(toolMnemonic.getMnemonicId());
                                          query.addBindValue(toolMnemonic.getOffsetMm());
                                      });
}

bool DatabaseSaver::saveSensorMnemonics()
{
    return processItems<SensorMnemonic>(storage->getDeletedSensorMnemonics(),
                                        storage->getModifiedSensorMnemonics(),
                                        storage->getAddedSensorMnemonics(),
                                        "sensors_mnemonics",
                                        "sensor_mnemonic_id",
                                        {"sensor_id", "mnemonic_id"},
                                        [](const SensorMnemonic &sensorMnemonic, QSqlQuery &query) {
                                            query.addBindValue(sensorMnemonic.getSensorId());
                                            query.addBindValue(sensorMnemonic.getMnemonicId());
                                        });
}

bool DatabaseSaver::saveConversionFormulas()
{
    return processItems<ConversionFormula>(storage->getDeletedConversionFormulas(),
                                           storage->getModifiedConversionFormulas(),
                                           storage->getAddedConversionFormulas(),
                                           "conversion_formulas",
                                           "formula_id",
                                           {"formula", "initial_unit_id", "derived_unit_id"},
                                           [](const ConversionFormula &formula, QSqlQuery &query) {
                                               query.addBindValue(formula.getFormula());
                                               query.addBindValue(formula.getInitialUnitId());
                                               query.addBindValue(formula.getDerivedUnitId());
                                           });
}
