#include "facade_call_system.h"
#include "databasemanager.h"
#include "session_manager.h"
#include <QSqlQuery>

facade_call_system::facade_call_system(QObject *parent) : QObject(parent) {}

void facade_call_system::getCallList(const QString &status, int techId, int requesterId) {
    QList<CallInfo> list;
    QSqlQuery query(DatabaseManager::getInstance().getConnection());

    // SQL CORRIGIDO: Adicionamos o LEFT JOIN para o técnico, igual na outra função
    QString sql = "SELECT "
                  "    c.id, c.titulo, c.descricao, c.status, c.data_abertura, c.data_fechamento, c.tipo, c.prioridade, "
                  "    u_sol.nome AS nome_solicitante, "
                  "    COALESCE(u_tec.nome, 'Não Atribuído') AS nome_tecnico "
                  "FROM "
                  "    Chamado c "
                  "INNER JOIN "
                  "    Usuario u_sol ON c.id_solicitante = u_sol.id "
                  "LEFT JOIN "
                  "    Usuario u_tec ON c.id_tecnico = u_tec.id ";

    QStringList filters;
    if (!status.isEmpty()) {
        filters.append("c.status = :status");
    }
    if (techId > 0) {
        filters.append("c.id_tecnico = :technicianId");
    }
    if (requesterId > 0) {
        filters.append("c.id_solicitante = :requesterId");
    }
    if (!filters.isEmpty()) {
        sql += " WHERE " + filters.join(" AND ");
    }

    sql += " ORDER BY c.data_abertura DESC";

    query.prepare(sql);

    // Bloco que faz os binds
    if (!status.isEmpty()) {
        query.bindValue(":status", status);
    }
    if (techId > 0) {
        query.bindValue(":technicianId", techId);
    }
    if (requesterId > 0) {
        query.bindValue(":requesterId", requesterId);
    }

    if (query.exec()) {
        while (query.next()) {
            CallInfo info;
            info.id = query.value("id").toInt();
            info.titulo = query.value("titulo").toString();
            info.descricao = query.value("descricao").toString();
            info.status = query.value("status").toString();
            info.data_abertura = query.value("data_abertura").toDateTime();
            info.data_fechamento = query.value("data_fechamento").toDateTime();
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
                  "    c.id, c.titulo, c.descricao, c.status, c.data_abertura, c.data_fechamento, c.tipo, c.prioridade, "
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
        info.data_fechamento = query.value("data_fechamento").toDateTime();
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

// Função para buscar o histórico de mensagens de um chamado
void facade_call_system::requestChatMessages(int callId)
{
    QList<ChatMessageInfo> messages;
    QSqlQuery query(DatabaseManager::getInstance().getConnection());

    // Query com JOIN para pegar o nome do remetente
    QString sql = "SELECT m.mensagem, m.data_envio, u.nome AS senderName "
                  "FROM ChatMessage m "
                  "INNER JOIN Usuario u ON m.id_remetente = u.id "
                  "WHERE m.id_chamado = :id_chamado "
                  "ORDER BY m.data_envio ASC"; // Ordena da mais antiga para a mais nova

    query.prepare(sql);
    query.bindValue(":id_chamado", callId);

    if (query.exec()) {
        while(query.next()) {
            ChatMessageInfo info;
            info.senderName = query.value("senderName").toString();
            info.message = query.value("mensagem").toString();
            info.timestamp = query.value("data_envio").toDateTime();
            messages.append(info);
        }
        emit chatMessagesReady(messages);
    } else {
        qWarning() << "Falha ao buscar mensagens do chat:" << query.lastError().text();
    }
}

// Função para enviar uma nova mensagem
void facade_call_system::sendChatMessage(int callId, const QString &message)
{
    // Verifica se a mensagem não está vazia
    if (message.trimmed().isEmpty()) {
        return;
    }

    // Pega o ID do usuário logado do nosso SessionManager!
    int senderId = SessionManager::getInstance().getCurrentUserId();
    if (!SessionManager::getInstance().isUserLoggedIn()) {
        qWarning() << "Tentativa de enviar mensagem sem estar logado.";
        return;
    }

    QSqlQuery query(DatabaseManager::getInstance().getConnection());
    query.prepare("INSERT INTO ChatMessage (id_chamado, id_remetente, mensagem) "
                  "VALUES (:id_chamado, :id_remetente, :mensagem)");
    query.bindValue(":id_chamado", callId);
    query.bindValue(":id_remetente", senderId);
    query.bindValue(":mensagem", message);

    if (!query.exec()) {
        qWarning() << "Falha ao inserir nova mensagem no chat:" << query.lastError().text();
    } else {
        // Se a mensagem foi enviada com sucesso, pede para recarregar o chat
        qDebug() << "Mensagem enviada com sucesso. Recarregando histórico do chat.";
        requestChatMessages(callId);
    }
}

void facade_call_system::closeCall(int callId){
    // --- Step 1: Carregar o estado atual do ticket ---
    // (O código para carregar o Ticket do banco é igual ao de assignTechnicianToTicket)
    QSqlQuery query(DatabaseManager::getInstance().getConnection());
    query.prepare("SELECT * FROM Chamado WHERE id = :id");
    query.bindValue(":id", callId);

    Call call;
    if (query.exec() && query.next()) {
        call.setId(query.value("id").toInt());
        call.setStatus(query.value("status").toString());
        call.startState();
    } else {
        emit operationFailed("Chamado não encontrado para conclusão.");
        return;
    }

    // --- Step 2: VERIFICAR A REGRA DE NEGÓCIO ---
    // Verificamos o estado ANTES de tentar a ação.
    if (call.status() != "Em Andamento") {
        emit operationFailed("Para concluir, o chamado precisa ter um técnico atribuído.");
        return;
    }

    // --- Step 3: Usar o Padrão State para executar a ação ---
    call.close();

    // --- Step 4: Salvar o estado atualizado no banco ---
    // (Aqui podemos adicionar o update da data de fechamento no futuro)
    query.prepare("UPDATE Chamado SET status = :status, data_fechamento = :closingDate WHERE id = :id");
    query.bindValue(":status", call.status());
    query.bindValue(":closingDate", call.closingDate());
    query.bindValue(":id", call.id());

    if (!query.exec()) {
        emit operationFailed("Não foi possível salvar a conclusão do chamado.");
        return;
    }

    // --- Step 5: Notificar a UI sobre o sucesso ---
    emit callUpdatedSuccessfully("Chamado concluído com sucesso!");
}

void facade_call_system::requestUserLists()
{
    QList<UserInfo> technicians;
    QList<UserInfo> requesters;
    QSqlQuery query(DatabaseManager::getInstance().getConnection());

    QString sql = "SELECT id, nome, email, tipo FROM Usuario WHERE tipo != 'Admin' ORDER BY nome";

    if (query.exec(sql)) {
        while (query.next()) {
            UserInfo info;
            info.id = query.value("id").toInt();
            info.name = query.value("nome").toString();
            info.email = query.value("email").toString();
            info.type = query.value("tipo").toString();

            if (info.type == "Tecnico") {
                technicians.append(info);
            } else if (info.type == "Solicitante") {
                requesters.append(info);
            }
        }
        emit userListsReady(technicians, requesters);
    } else {
        qWarning() << "Falha ao buscar listas de usuários:" << query.lastError().text();
        emit operationFailed("Não foi possível carregar os usuários.");
    }
}

void facade_call_system::deleteUser(int userId)
{
    // Regra de negócio: não permitir deletar o admin padrão
    if (userId == 1) {
        emit operationFailed("O usuário Administrador padrão não pode ser removido.");
        return;
    }

    QSqlQuery query(DatabaseManager::getInstance().getConnection());
    query.prepare("DELETE FROM Usuario WHERE id = :id");
    query.bindValue(":id", userId);

    if (query.exec()) {
        if (query.numRowsAffected() > 0) {
            emit userDeletedSuccessfully("Usuário removido com sucesso.");
        } else {
            emit operationFailed("Usuário não encontrado ou já removido.");
        }
    } else {
        qWarning() << "Falha ao remover usuário:" << query.lastError().text();
        emit operationFailed("Erro ao remover usuário. Verifique se ele não tem chamados associados.");
    }
}

void facade_call_system::requestUserDetails(int userId)
{
    QSqlQuery query(DatabaseManager::getInstance().getConnection());
    query.prepare("SELECT id, nome, email, tipo FROM Usuario WHERE id = :id");
    query.bindValue(":id", userId);

    if (query.exec() && query.next()) {
        UserInfo info;
        info.id = query.value("id").toInt();
        info.name = query.value("nome").toString();
        info.email = query.value("email").toString();
        info.type = query.value("tipo").toString();
        emit userDetailsReady(info);
    } else {
        emit operationFailed("Usuário não encontrado.");
    }
}

void facade_call_system::saveUser(int userId, const QString &name, const QString &email, const QString &password, const QString &type)
{
    QSqlQuery query(DatabaseManager::getInstance().getConnection());

    if (userId == 0) {
        // --- LÓGICA DE INSERT (continua a mesma) ---
        query.prepare("INSERT INTO Usuario (nome, email, senha, tipo) "
                      "VALUES (:name, :email, :password, :type)");
        query.bindValue(":name", name);
        query.bindValue(":email", email);
        query.bindValue(":password", password);
        query.bindValue(":type", type);

    } else {
        // --- LÓGICA DE UPDATE DINÂMICO (A MUDANÇA ESTÁ AQUI) ---

        // 1. Começamos a montar a query SQL apenas com os campos que sempre mudam.
        QString sql = "UPDATE Usuario SET nome = :name, email = :email, tipo = :type";

        // 2. SÓ adicionamos a atualização de senha SE uma nova senha foi fornecida.
        if (!password.isEmpty()) {
            sql += ", senha = :password";
        }

        // 3. Terminamos a query com a cláusula WHERE.
        sql += " WHERE id = :id";

        query.prepare(sql);

        // 4. Fazemos o bind dos valores. Note que o bind da senha também é condicional.
        query.bindValue(":name", name);
        query.bindValue(":email", email);
        query.bindValue(":type", type);
        query.bindValue(":id", userId);

        if (!password.isEmpty()) {
            query.bindValue(":password", password);
        }
    }

    if (query.exec()) {
        emit userSavedSuccessfully("Usuário salvo com sucesso!");
    } else {
        qWarning() << "Falha ao salvar usuário:" << query.lastError().text();
        if (query.lastError().nativeErrorCode() == "19") { // UNIQUE constraint
            emit operationFailed("Falha ao salvar: o e-mail informado já está em uso.");
        } else {
            emit operationFailed("Falha ao salvar usuário.");
        }
    }
}

void facade_call_system::createNewCall(const QString &title, const QString &type, const QString &priority, const QString &description)
{
    // Pega o ID do usuário da sessão atual. A mágica acontece aqui!
    int requesterId = SessionManager::getInstance().getCurrentUserId();

    // Verificação de segurança
    if (!SessionManager::getInstance().isUserLoggedIn()) {
        emit operationFailed("Nenhum usuário logado. Não é possível abrir o chamado.");
        return;
    }

    QSqlQuery query(DatabaseManager::getInstance().getConnection());
    query.prepare("INSERT INTO Chamado (titulo, descricao, id_solicitante, tipo, prioridade) "
                  "VALUES (:title, :description, :requesterId, :type, :priority)");

    query.bindValue(":title", title);
    query.bindValue(":description", description);
    query.bindValue(":requesterId", requesterId);
    query.bindValue(":type", type);
    query.bindValue(":priority", priority);

    if (query.exec()) {
        qDebug() << "Novo chamado criado com sucesso pelo usuário ID:" << requesterId;
        emit callCreatedSuccessfully("Seu chamado foi aberto com sucesso!");
    } else {
        qWarning() << "Falha ao criar novo chamado:" << query.lastError().text();
        emit operationFailed("Ocorreu um erro ao tentar abrir seu chamado.");
    }
}

void facade_call_system::validateLogin(const QString &email, const QString &password)
{
    QSqlQuery query(DatabaseManager::getInstance().getConnection());
    query.prepare("SELECT id, nome, senha, tipo FROM Usuario WHERE email = :email");
    query.bindValue(":email", email);

    if (query.exec() && query.next()) {
        // Usuário com este email foi encontrado, agora checamos a senha
        QString storedPassword = query.value("senha").toString();

        if (storedPassword == password) {
            // Senha correta! Login bem-sucedido.
            int userId = query.value("id").toInt();
            QString name = query.value("nome").toString();
            QString userType = query.value("tipo").toString();

            qDebug() << "Login bem-sucedido para o usuário:" << name;
            emit loginSuccessful(userId, name, userType);
        } else {
            // Senha incorreta
            qWarning() << "Tentativa de login com senha incorreta para o email:" << email;
            emit operationFailed("Usuário ou senha inválidos.");
        }
    } else {
        // Usuário com este email não foi encontrado
        qWarning() << "Tentativa de login com email não existente:" << email;
        emit operationFailed("Usuário ou senha inválidos.");
    }
}
