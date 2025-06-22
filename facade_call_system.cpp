#include "facade_call_system.h"
#include "databasemanager.h"
#include <QSqlQuery>

facade_call_system::facade_call_system(QObject *parent) : QObject(parent) {}

void facade_call_system::getCallList(const QString &status) {
    QList<CallInfo> list;
    QSqlQuery query(DatabaseManager::getInstance().getConnection());

    // SQL CORRIGIDO: Adicionamos o LEFT JOIN para o técnico, igual na outra função
    QString sql = "SELECT "
                  "    c.id, c.titulo, c.descricao, c.status, c.data_abertura, c.tipo, c.prioridade, "
                  "    u_sol.nome AS nome_solicitante, "
                  "    COALESCE(u_tec.nome, 'Não Atribuído') AS nome_tecnico "
                  "FROM "
                  "    Chamado c "
                  "INNER JOIN "
                  "    Usuario u_sol ON c.id_solicitante = u_sol.id "
                  "LEFT JOIN "
                  "    Usuario u_tec ON c.id_tecnico = u_tec.id ";

    if (!status.isEmpty()) {
        sql += " WHERE c.status = :status";
    }
    sql += " ORDER BY c.data_abertura DESC";

    query.prepare(sql);

    if (!status.isEmpty()) {
        query.bindValue(":status", status);
    }

    if (query.exec()) {
        while (query.next()) {
            CallInfo info;
            info.id = query.value("id").toInt();
            info.titulo = query.value("titulo").toString();
            info.descricao = query.value("descricao").toString();
            info.status = query.value("status").toString();
            info.data_abertura = query.value("data_abertura").toDateTime();
            info.tipo = query.value("tipo").toString();
            info.prioridade = query.value("prioridade").toString();
            info.nome_solicitante = query.value("nome_solicitante").toString();

            // Agora esta linha vai funcionar, pois a query retorna o campo
            info.nome_tecnico = query.value("nome_tecnico").toString();

            list.append(info);
        }
        emit readyCallList(list);
    } else {
        // Renomeei o sinal de erro para o nome que você usou, operationFailed
        emit operationFailed("Erro ao carregar chamados: " + query.lastError().text());
    }
}

void facade_call_system::getCallDetails(int callId){
    QSqlQuery query(DatabaseManager::getInstance().getConnection());

    QString sql = "SELECT "
                  "    c.id, c.titulo, c.descricao, c.status, c.data_abertura, c.tipo, c.prioridade, "
                  "    u_sol.nome AS nome_solicitante, "
                  "    COALESCE(u_tec.nome, 'Não Atribuído') AS nome_tecnico "
                  "FROM "
                  "    Chamado c "
                  "INNER JOIN "
                  "    Usuario u_sol ON c.id_solicitante = u_sol.id "
                  "LEFT JOIN "
                  "    Usuario u_tec ON c.id_tecnico = u_tec.id "
                  "WHERE c.id = :id";

    query.prepare(sql);
    query.bindValue(":id", callId);

    if(query.exec() && query.next()){
        CallInfo info;

        info.id = query.value("id").toInt();

        qDebug() << "Lendo ID do banco:" << info.id;

        info.titulo = query.value("titulo").toString();
        info.descricao = query.value("descricao").toString();
        info.status = query.value("status").toString();
        info.data_abertura = query.value("data_abertura").toDateTime();
        info.tipo = query.value("tipo").toString();
        info.prioridade = query.value("prioridade").toString();
        info.nome_solicitante = query.value("nome_solicitante").toString();
        info.nome_tecnico = query.value("nome_tecnico").toString();

        emit readyCallDetails(info);
    } else {
        qWarning() << "Falha ao buscar detalhes do chamado ID" << callId << ":" << query.lastError().text();
        emit operationFailed("Não foi possível carregar os detalhes do chamado.");
    }
}

int facade_call_system::callCount(const QString &status){
    QSqlQuery query(DatabaseManager::getInstance().getConnection());
    QString sql = "SELECT COUNT(*) FROM Chamado";

    if(!status.isEmpty()){
        sql+= " WHERE status = :status";
    }

    query.prepare(sql);

    if(!status.isEmpty()){
        query.bindValue(":status", status);
    }

    if(query.exec()){
        if(query.next()){
            return query.value(0).toInt();
        }
    } else {
        qWarning() << "Falha ao contar chamados:" << query.lastError().text();
    }

    return 0;
}

void facade_call_system::requestTechnicianList() {
    QMap<int, QString> technicians;
    QSqlQuery query(DatabaseManager::getInstance().getConnection());

    QString sql = "SELECT id, nome FROM Usuario WHERE tipo = 'Tecnico' ORDER BY nome";

    if (query.exec(sql)) {
        while (query.next()) {
            // Adiciona ao QMap, com o ID como chave e o nome como valor
            technicians.insert(query.value("id").toInt(), query.value("nome").toString());
        }
        qDebug() << "Encontrados" << technicians.size() << "técnicos no banco de dados.";
        emit technicianListReady(technicians);
    } else {
        qWarning() << "Falha ao buscar lista de técnicos:" << query.lastError().text();
        emit operationFailed("Não foi possível carregar a lista de técnicos.");
    }
}

void facade_call_system::assignTechnicianToCall(int ticketId, int technicianId)
{
    QSqlQuery query(DatabaseManager::getInstance().getConnection());
    query.prepare("SELECT * FROM Chamado WHERE id = :id");
    query.bindValue(":id", ticketId);

    Call call;
    if (query.exec() && query.next()) {
        call.setId(query.value("id").toInt());
        call.setStatus(query.value("status").toString());

        call.startState();
    } else {
        qWarning() << "Falha ao carregar chamado para atualização:" << query.lastError().text();
        emit operationFailed("Chamado não encontrado para atualização.");
        return;
    }

    call.assignTechnician(technicianId);

    query.prepare("UPDATE Chamado SET status = :status, id_tecnico = :id_tecnico WHERE id = :id");
    query.bindValue(":status", call.status());
    query.bindValue(":id", call.id());

    if (call.techId() > 0) {
        query.bindValue(":id_tecnico", call.techId());
    } else {
        query.bindValue(":id_tecnico", QVariant(QVariant::Int));
    }

    if (!query.exec()) {
        qWarning() << "Falha ao salvar o chamado atualizado:" << query.lastError().text();
        emit operationFailed("Não foi possível salvar as alterações no chamado.");
        return;
    }

    // --- Step 4: Notificar a UI sobre o sucesso ---
    qDebug() << "Chamado" << ticketId << "atualizado com sucesso para o técnico" << technicianId;
    emit callUpdatedSuccessfully("Técnico atribuído com sucesso!");
}
