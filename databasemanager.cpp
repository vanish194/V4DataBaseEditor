#include "databasemanager.h"

DatabaseManager::DatabaseManager()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
}

DatabaseManager::~DatabaseManager()
{
    if (db.isOpen())
        db.close();
}

bool DatabaseManager::connect(const QString &path)
{
    db.setDatabaseName(path);

    if (!db.open()) {
        qDebug() << "Error: connection with database failed -" << db.lastError().text();
        return false;
    }

    qDebug() << "Database: connection ok";

    // Check the database version
    int currentVersion = getDatabaseVersion();
    int expectedVersion = 1; // Set this to the version your application expects

    if (currentVersion < 0) {
        qDebug() << "Error: Unable to determine database version.";
        return false;
    }

    if (currentVersion != expectedVersion) {
        qDebug() << "Error: Database version mismatch. Expected version" << expectedVersion
                 << "but found version" << currentVersion;
        return false;
    }

    return true;
}
int DatabaseManager::getDatabaseVersion() const
{
    int version = -1; // Default to an invalid version

    QSqlQuery query("SELECT version FROM db_information ORDER BY version DESC LIMIT 1");

    if (query.exec()) {
        if (query.next()) {
            version = query.value(0).toInt();
        }
    } else {
        qDebug() << "Error retrieving database version:" << query.lastError().text();
    }

    return version;
}

// Методы для загрузки данных
void DatabaseManager::loadAllData()
{
    loadUnits();
    loadTypesOfUnits();
    loadMethods();
    loadProducers();
    loadSensors();
    loadMainMnemonics();
    loadAdditionalMnemonics();
    loadToolDescriptions();
    loadTools();
    loadToolSensors();
    loadConversionFormulas();
    loadCompanies();
    // После загрузки всех данных создаём резервную копию
    Storage *storage = Storage::getInstance();
    storage->createBackup();
}
void DatabaseManager::loadUnits()
{
    QSqlQuery query;
    if (!query.exec("SELECT unit_id, unit_name, type_id, unit_short_name FROM units")) {
        qDebug() << "Ошибка загрузки данных из таблицы units:" << query.lastError().text();
        return;
    }

    Storage *storage = Storage::getInstance();
    int maxId = 0;
    while (query.next()) {
        int id = query.value("unit_id").toInt();
        QString name = query.value("unit_name").toString();
        int typeId = query.value("type_id").toInt();
        QString shortName = query.value("unit_short_name").toString();
        storage->getUnits().append(Unit(id, name, typeId, shortName));
        if (id > maxId) {
            maxId = id; // Запоминаем максимальный ID
        }
    }
    storage->setMaxUnitId(maxId); // Устанавливаем максимальный ID для генерации новых данных
}

void DatabaseManager::loadTypesOfUnits()
{
    QSqlQuery query;
    if (!query.exec("SELECT type_id, type_name FROM types_of_units")) {
        qDebug() << "Ошибка загрузки данных из таблицы types_of_units:" << query.lastError().text();
        return;
    }

    Storage *storage = Storage::getInstance();
    int maxId = 0;
    while (query.next()) {
        int id = query.value("type_id").toInt();
        QString name = query.value("type_name").toString();
        storage->getTypesOfUnits().append(TypeOfUnit(id, name));
        if (id > maxId) {
            maxId = id;
        }
    }
    storage->setMaxTypeOfUnitId(maxId);
}

void DatabaseManager::loadMethods()
{
    QSqlQuery query;
    if (!query.exec("SELECT method_id, method_name FROM methods")) {
        qDebug() << "Ошибка загрузки данных из таблицы methods:" << query.lastError().text();
        return;
    }

    Storage *storage = Storage::getInstance();
    int maxId = 0;
    while (query.next()) {
        int id = query.value("method_id").toInt();
        QString name = query.value("method_name").toString();
        storage->getMethods().append(Method(id, name));
        if (id > maxId) {
            maxId = id;
        }
    }
    storage->setMaxMethodId(maxId);
}

void DatabaseManager::loadProducers()
{
    QSqlQuery query;
    if (!query.exec("SELECT producer_id, producer_name FROM producers")) {
        qDebug() << "Ошибка загрузки данных из таблицы producers:" << query.lastError().text();
        return;
    }

    Storage *storage = Storage::getInstance();
    int maxId = 0;
    while (query.next()) {
        int id = query.value("producer_id").toInt();
        QString name = query.value("producer_name").toString();
        storage->getProducers().append(Producer(id, name));
        if (id > maxId) {
            maxId = id;
        }
    }
    storage->setMaxProducerId(maxId);
}

void DatabaseManager::loadSensors()
{
    QSqlQuery query;
    if (!query.exec("SELECT sensor_id, method_id, sensor_name, sensor_description FROM sensors")) {
        qDebug() << "Ошибка загрузки данных из таблицы sensors:" << query.lastError().text();
        return;
    }

    Storage *storage = Storage::getInstance();
    int maxId = 0;
    while (query.next()) {
        int id = query.value("sensor_id").toInt();
        int methodId = query.value("method_id").toInt();
        QString name = query.value("sensor_name").toString();
        QString description = query.value("sensor_description").toString();
        storage->getSensors().append(Sensor(id, methodId, name, description));
        if (id > maxId) {
            maxId = id;
        }
    }
    storage->setMaxSensorId(maxId);
}

void DatabaseManager::loadMainMnemonics()
{
    QSqlQuery query;
    if (!query.exec("SELECT main_mnemonic_id, main_mnemonic_name, main_mnemonic_description, "
                    "sensor_id, unit_id FROM main_mnemonics")) {
        qDebug() << "Ошибка загрузки данных из таблицы main_mnemonics:" << query.lastError().text();
        return;
    }

    Storage *storage = Storage::getInstance();
    int maxId = 0;
    while (query.next()) {
        int id = query.value("main_mnemonic_id").toInt();
        QString name = query.value("main_mnemonic_name").toString();
        QString description = query.value("main_mnemonic_description").toString();
        int sensorId = query.value("sensor_id").toInt();
        int unitId = query.value("unit_id").toInt();
        storage->getMainMnemonics().append(MainMnemonic(id, name, description, sensorId, unitId));
        if (id > maxId) {
            maxId = id;
        }
    }
    storage->setMaxMainMnemonicId(maxId);
}

void DatabaseManager::loadAdditionalMnemonics()
{
    QSqlQuery query;
    if (!query.exec("SELECT additional_mnemonic_id, additional_mnemonic_name, company_id, "
                    "main_mnemonic_id, unit_id FROM additional_mnemonics")) {
        qDebug() << "Ошибка загрузки данных из таблицы additional_mnemonics:"
                 << query.lastError().text();
        return;
    }

    Storage *storage = Storage::getInstance();
    int maxId = 0;
    while (query.next()) {
        int id = query.value("additional_mnemonic_id").toInt();
        QString name = query.value("additional_mnemonic_name").toString();
        int companyId = query.value("company_id").toInt();
        int mainMnemonicId = query.value("main_mnemonic_id").toInt();
        int unitId = query.value("unit_id").toInt();
        storage->getAdditionalMnemonics().append(
            AdditionalMnemonic(id, name, companyId, mainMnemonicId, unitId));
        if (id > maxId) {
            maxId = id;
        }
    }
    storage->setMaxAdditionalMnemonicId(maxId);
}

void DatabaseManager::loadToolDescriptions()
{
    QSqlQuery query;
    if (!query.exec("SELECT tool_description_id, description, length_mm, outer_diameter_mm, "
                    "inner_diameter_mm, image, producer_id FROM tool_descriptions")) {
        qDebug() << "Ошибка загрузки данных из таблицы tool_descriptions:"
                 << query.lastError().text();
        return;
    }

    Storage *storage = Storage::getInstance();
    int maxId = 0;
    while (query.next()) {
        int id = query.value("tool_description_id").toInt();
        QString description = query.value("description").toString();
        int length = query.value("length_mm").toInt();
        int outerDiameter = query.value("outer_diameter_mm").toInt();
        int innerDiameter = query.value("inner_diameter_mm").toInt();
        QByteArray image = query.value("image").toByteArray();
        int producerId = query.value("producer_id").toInt();
        storage->getToolDescriptions().append(
            ToolDescription(id, description, length, outerDiameter, innerDiameter, image, producerId));
        if (id > maxId) {
            maxId = id;
        }
    }
    storage->setMaxToolDescriptionId(maxId);
}

void DatabaseManager::loadTools()
{
    QSqlQuery query;
    if (!query.exec("SELECT tool_id, tool_name, tool_description_id FROM tools")) {
        qDebug() << "Ошибка загрузки данных из таблицы tools:" << query.lastError().text();
        return;
    }

    Storage *storage = Storage::getInstance();
    int maxId = 0;
    while (query.next()) {
        int id = query.value("tool_id").toInt();
        QString name = query.value("tool_name").toString();
        int toolDescriptionId = query.value("tool_description_id").toInt();
        storage->getTools().append(Tool(id, name, toolDescriptionId));
        if (id > maxId) {
            maxId = id;
        }
    }
    storage->setMaxToolId(maxId);
}

void DatabaseManager::loadToolSensors()
{
    QSqlQuery query;
    if (!query.exec("SELECT tool_sensor_id, tool_id, sensor_id, offset_mm FROM tools_sensors")) {
        qDebug() << "Ошибка загрузки данных из таблицы tools_sensors:" << query.lastError().text();
        return;
    }

    Storage *storage = Storage::getInstance();
    int maxId = 0;
    while (query.next()) {
        int id = query.value("tool_sensor_id").toInt();
        int toolId = query.value("tool_id").toInt();
        int sensorId = query.value("sensor_id").toInt();
        int offsetMm = query.value("offset_mm").toInt();
        storage->getToolSensors().append(ToolSensor(id, toolId, sensorId, offsetMm));
        if (id > maxId) {
            maxId = id;
        }
    }
    storage->setMaxToolSensorId(maxId);
}

void DatabaseManager::loadConversionFormulas()
{
    QSqlQuery query;
    if (!query.exec("SELECT formula_id, formula, initial_unit_id, derived_unit_id FROM "
                    "conversion_formulas")) {
        qDebug() << "Ошибка загрузки данных из таблицы conversion_formulas:"
                 << query.lastError().text();
        return;
    }

    Storage *storage = Storage::getInstance();
    int maxId = 0;
    while (query.next()) {
        int id = query.value("formula_id").toInt();
        QString formula = query.value("formula").toString();
        int initialUnitId = query.value("initial_unit_id").toInt();
        int derivedUnitId = query.value("derived_unit_id").toInt();
        storage->getConversionFormulas().append(
            ConversionFormula(id, formula, initialUnitId, derivedUnitId));
        if (id > maxId) {
            maxId = id;
        }
    }
    storage->setMaxConversionFormulaId(maxId);
}

void DatabaseManager::loadCompanies()
{
    QSqlQuery query;
    if (!query.exec("SELECT company_id, company_name FROM companies")) {
        qDebug() << "Ошибка загрузки данных из таблицы companies:" << query.lastError().text();
        return;
    }

    Storage *storage = Storage::getInstance();
    int maxId = 0;
    while (query.next()) {
        int id = query.value("company_id").toInt();
        QString name = query.value("company_name").toString();
        storage->getCompanies().append(Company(id, name));
        if (id > maxId) {
            maxId = id;
        }
    }
    storage->setMaxCompanyId(maxId);
}
