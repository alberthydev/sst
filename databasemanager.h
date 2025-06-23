#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QString>

class DatabaseManager {
public:
    // O método estático que retorna a única instância da classe.
    static DatabaseManager& getInstance();

    // Método para obter a conexão com o banco de dados.
    QSqlDatabase getConnection();

    // Deleta os construtores de cópia e operadores de atribuição
    // para garantir que nenhuma cópia da instância seja feita.
    DatabaseManager(const DatabaseManager&) = delete;
    void operator=(const DatabaseManager&) = delete;

private:
    // Construtor privado para forçar o uso do getInstance().
    DatabaseManager();
    ~DatabaseManager();
    void createTables();

    QSqlDatabase m_database;
    const QString CONNECTION_NAME = "SST_CONNECTION";
};

#endif // DATABASEMANAGER_H
