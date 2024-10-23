#ifndef DATABASESAVER_H
#define DATABASESAVER_H

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "storage.h"
#include <functional>

class DatabaseSaver
{
public:
    DatabaseSaver(QSqlDatabase &database);
    ~DatabaseSaver();

    bool saveAllData();

private:
    QSqlDatabase &db;
    Storage *storage;

    bool saveUnits();
    bool saveUnitTypes();
    bool saveMethods();
    bool saveProducers();
    bool saveSensors();
    bool saveMainMnemonics();
    bool saveAdditionalMnemonics();
    bool saveCompanies();
    bool saveToolDescriptions();
    bool saveTools();
    bool saveToolSensors();
    bool saveToolMnemonics();
    bool saveSensorMnemonics();
    bool saveConversionFormulas();

    // Вспомогательный шаблонный метод для обработки записей
    template<typename T>
    bool processItems(const QList<T> &deletedItems,
                      const QList<T> &modifiedItems,
                      const QList<T> &addedItems,
                      const QString &tableName,
                      const QString &idColumn,
                      const QStringList &columns,
                      const std::function<void(const T &, QSqlQuery &)> &bindValues);

    // Вспомогательный метод для получения положительного ID
    int positiveId(int id) { return id < 0 ? -id : id; }
};

#endif // DATABASESAVER_H
