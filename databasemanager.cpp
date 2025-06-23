#include "databasemanager.h"
#include "qsqlquery.h"
#include <QDebug>

// O construtor é chamado apenas uma vez, na primeira chamada de getInstance().
DatabaseManager::DatabaseManager() {
    m_database = QSqlDatabase::addDatabase("QSQLITE"); // Ex: usando SQLite
    m_database.setDatabaseName("sst_db.db");

    if (!m_database.open()) {
        qCritical() << "Erro: Falha ao conectar ao banco de dados:" << m_database.lastError().text();
    } else {
        qDebug() << "Banco de dados conectado com sucesso!";
        createTables();
    }
}

DatabaseManager::~DatabaseManager() {
    if (m_database.isOpen()) {
        m_database.close();
        qDebug() << "Conexão com o banco de dados fechada.";
    }
}

DatabaseManager& DatabaseManager::getInstance() {
    // A instância estática é criada na primeira vez que esta linha é executada
    // e destruída automaticamente quando o programa termina. É thread-safe em C++11+.
    static DatabaseManager instance;
    return instance;
}

QSqlDatabase DatabaseManager::getConnection() {
    return m_database;
}

// Implementação da função que cria as tabelas
void DatabaseManager::createTables() {
    QSqlQuery query(m_database);

    // --- Tabela Usuario ---
    QString userTableSql = "CREATE TABLE IF NOT EXISTS Usuario ("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "nome TEXT NOT NULL, "
                           "email TEXT UNIQUE NOT NULL, "
                           "senha TEXT NOT NULL, "
                           "tipo TEXT CHECK(tipo IN ('Solicitante', 'Tecnico', 'Admin')) NOT NULL"
                           ");";
    if (!query.exec(userTableSql)) {
        qWarning() << "Falha ao criar tabela 'Usuario':" << query.lastError().text();
    }

    query.prepare("SELECT COUNT(*) FROM Usuario WHERE email = 'admin@sst.com'");
    if (query.exec() && query.next() && query.value(0).toInt() == 0) {
        // Se a contagem for 0, o admin não existe, então o inserimos.
        query.prepare("INSERT INTO Usuario (nome, email, senha, tipo) "
                      "VALUES ('Administrador', 'admin@sst.com', 'admin', 'Admin')");
        if (!query.exec()) {
            qWarning() << "Falha ao inserir usuário Admin padrão:" << query.lastError().text();
        } else {
            qDebug() << "Usuário Administrador padrão criado com sucesso.";
        }
    }

    // --- Tabela Ticket (Chamado) ---
    QString callTableSql = "CREATE TABLE IF NOT EXISTS Chamado ("
                             "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "titulo TEXT NOT NULL, "
                             "descricao TEXT, "
                             "status TEXT CHECK(status IN ('Aberto', 'Em Andamento', 'Fechado')) DEFAULT 'Aberto', "
                             "data_abertura DATETIME DEFAULT CURRENT_TIMESTAMP, "
                             "data_fechamento DATETIME, "
                             "id_solicitante INTEGER NOT NULL, "
                             "id_tecnico INTEGER, "
                             "tipo TEXT CHECK(tipo IN ('Sistema', 'Impressora', 'Hardware', 'Rede', 'Outros')) NOT NULL, "
                             "prioridade TEXT NOT NULL DEFAULT 'Baixo' CHECK(prioridade IN ('Baixo', 'Médio', 'Alto', 'Urgente')), "
                             "FOREIGN KEY (id_solicitante) REFERENCES Usuario(id), "
                             "FOREIGN KEY (id_tecnico) REFERENCES Usuario(id)"
                             ");";
    if (!query.exec(callTableSql)) {
        qWarning() << "Falha ao criar tabela 'Chamado':" << query.lastError().text();
    }

    // --- Tabela ChatMessage ---
    QString chatMessageTableSql = "CREATE TABLE IF NOT EXISTS ChatMessage ("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                  "id_chamado INTEGER NOT NULL, "
                                  "id_remetente INTEGER NOT NULL, "
                                  "mensagem TEXT NOT NULL, "
                                  "data_envio DATETIME DEFAULT CURRENT_TIMESTAMP, "
                                  "FOREIGN KEY (id_chamado) REFERENCES Ticket(id), "
                                  "FOREIGN KEY (id_remetente) REFERENCES Usuario(id)"
                                  ");";
    if (!query.exec(chatMessageTableSql)) {
        qWarning() << "Falha ao criar tabela 'ChatMessage':" << query.lastError().text();
    }

    qDebug() << "Verificação de tabelas concluída.";
}
