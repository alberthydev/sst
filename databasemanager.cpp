#include "databasemanager.h"
#include "qsqlquery.h"
#include <QDebug>

DatabaseManager::DatabaseManager() {
    m_database = QSqlDatabase::addDatabase("QSQLITE");
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
    static DatabaseManager instance;
    return instance;
}

QSqlDatabase DatabaseManager::getConnection() {
    return m_database;
}

void DatabaseManager::createTables() {
    QSqlQuery query(m_database);

    // Usuário
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

    // Adiciona o administrador para gerenciar usuários toda vez que for compilado o programa
    query.prepare("SELECT COUNT(*) FROM Usuario WHERE email = 'admin@sst.com'");
    if (query.exec() && query.next() && query.value(0).toInt() == 0) {
        query.prepare("INSERT INTO Usuario (nome, email, senha, tipo) "
                      "VALUES ('Administrador', 'admin@sst.com', 'admin', 'Admin')");
        if (!query.exec()) {
            qWarning() << "Falha ao inserir usuário Admin padrão:" << query.lastError().text();
        } else {
            qDebug() << "Usuário Administrador padrão criado com sucesso.";
        }
    }

    // Chamado
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

    // Mensagens do Chat
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
