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
    // Конструктор и деструктор
    DatabaseManager();
    ~DatabaseManager();

    // Метод для подключения к базе данных
    bool connect(const QString &path);

    // Методы для загрузки данных
    void loadAllData();

private:
    QSqlDatabase db;

    int getDatabaseVersion() const;
    // Приватные методы для загрузки данных из каждой таблицы
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
