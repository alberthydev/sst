#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QString>

class DatabaseManager {
public:
    static DatabaseManager& getInstance();
    QSqlDatabase getConnection();

    // Garante o padrao singleton
    DatabaseManager(const DatabaseManager&) = delete;
    void operator=(const DatabaseManager&) = delete;

private:
    DatabaseManager();
    ~DatabaseManager();
    void createTables();
    QSqlDatabase m_database;
    const QString CONNECTION_NAME = "SST_CONNECTION";

};

#endif // DATABASEMANAGER_H
