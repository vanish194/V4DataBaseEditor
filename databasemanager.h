#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include "storage.h"

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();

    bool connect(const QString &path);

    void loadAllData();
    QSqlDatabase &getDatabase();

private:
    QSqlDatabase db;

    int getDatabaseVersion() const;
    // Private methods for loading data from each table
    void loadUnits();
    void loadUnitTypes();
    void loadMethods();
    void loadProducers();
    void loadSensors();
    void loadMainMnemonics();
    void loadAdditionalMnemonics();
    void loadToolDescriptions();
    void loadTools();
    void loadToolSensors();
    void loadToolMnemonics();
    void loadSensorMnemonics();
    void loadConversionFormulas();
    void loadCompanies();
};

#endif // DATABASEMANAGER_H
