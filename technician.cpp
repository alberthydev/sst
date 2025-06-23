#include "technician.h"
#include "select_technician_dialog.h"
#include "ui_technician.h"
#include "session_manager.h"
#include <QMessageBox>

technician::technician(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::technician)
{
    ui->setupUi(this);

    // PAGES
    // 0 - Main
    // 1 - Calls Generico
    // 2 - Users
    // 3 - Criar/Editar User
    // 4 - Chamado Generico

    // Facade Object
    m_facade = new facade_call_system(this);

    connect(m_facade, &facade_call_system::readyCallList, this, &technician::populateTable);
    connect(m_facade, &facade_call_system::readyCallDetails, this, &technician::showCallDetails);
    connect(ui->callsTable, &QTableWidget::itemDoubleClicked, this, &technician::onItemTableDoubleClick);
    connect(m_facade, &facade_call_system::callUpdatedSuccessfully, this, &technician::onCallUpdateSuccess);
    connect(m_facade, &facade_call_system::chatMessagesReady, this, &technician::displayChatMessages);
    connect(m_facade, &facade_call_system::operationFailed, this, &technician::onOperationFailed);
    connect(m_facade, &facade_call_system::userListsReady, this, &technician::displayUserLists);
    connect(m_facade, &facade_call_system::userDeletedSuccessfully, this, &technician::refreshUserLists);
    connect(m_facade, &facade_call_system::userDeletedSuccessfully, this, &technician::onOperationSuccess);
    connect(m_facade, &facade_call_system::userDetailsReady, this, &technician::populateUserDetailsForm);
    connect(m_facade, &facade_call_system::userSavedSuccessfully, this, &technician::refreshUserLists);
    connect(m_facade, &facade_call_system::userSavedSuccessfully, this, &technician::onOperationSuccess);
    connect(ui->techUserList, &QListWidget::itemSelectionChanged,this, &technician::onTechnicianSelectionChanged);
    connect(ui->requestUserList, &QListWidget::itemSelectionChanged,this, &technician::onRequesterSelectionChanged);

    // Default Colors
    QColor whiteBk = "#f9f9f9";
    QColor blueBk = "#3383ff";
    QColor lightBlueBk = "#5597ff";

    // User Definition
    ui->signInUser->setText(SessionManager::getInstance().getCurrentUserName());
    QString userType = SessionManager::getInstance().getCurrentUserType();
    if (userType == "Admin") {
        qDebug() << "Usuário Admin detectado. Escondendo botões não relevantes.";

        ui->myCallsBtn->setVisible(false);
    }

    // Calls Table
    ui->callsTable->setColumnCount(7);
    ui->callsTable->verticalHeader()->setVisible(false);

    QStringList headers = {"Título", "Solicitante", "Abertura", "Tipo", "Prioridade", "Data de Fechamento"};
    ui->callsTable->setColumnHidden(0, true);
    ui->callsTable->setHorizontalHeaderLabels(QStringList() << "" << headers);

    ui->callsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->callsTable->setWordWrap(true);
    ui->callsTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->callsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->callsTable->setSelectionMode(QAbstractItemView::SingleSelection);

    // Calls Container
    ui->newCalls->setConfigs("Chamados em \nAberto \n0", "#ff2d55", "#ffa2b3", "#ff8aa0", "calls");
    ui->analysisCalls->setConfigs("Chamados em \nAnálise \n0", "#f4d700", QColor("#faffac"), QColor("#f7ff7e"), "calls");
    ui->completedCalls->setConfigs("Chamados \nConcluídos \n0", "#69d800", QColor("#d0ffa3"), QColor("#b5ff6e"),  "calls");

    // Admin Container
    ui->myCallsBtn->setConfigs("Meus Chamados", whiteBk, blueBk, lightBlueBk, "admin");
    ui->usersBtn->setConfigs("Usuários", whiteBk, blueBk, lightBlueBk, "admin");
    ui->logoutTechnichianBtn->setConfigs("Sair", whiteBk, blueBk, lightBlueBk, "admin");
    updateCountersDashboard();

    // Buttons
    ui->backBtn->setConfigs("\u2190", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->finishCallBtn->setConfigs("Concluir Chamado", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->addComentBtn->setConfigs("\u2191", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->editResponBtn->setConfigs("\u270E", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->backUserEditBtn->setConfigs("\u2190", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->okEditUserBtn->setConfigs("OK", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->backUserBtn->setConfigs("\u2190", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->rmvUserBtn->setConfigs("Remover Usuário", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->editUserBtn->setConfigs("Editar Usuário", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->createUserBtn->setConfigs("Criar Usuário", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->okCallsBtn->setConfigs("\u2190", whiteBk, blueBk, lightBlueBk, "defaultBtn");

    // User Type ComboBox
    ui->typeComboBox->addItems({"Tecnico", "Solicitante"});
}

technician::~technician()
{
    delete ui;
}

void technician::on_myCallsBtn_clicked()
{
    qDebug() << "MEUS CHAMADOS: Usando Facade no endereço:" << m_facade;

    int myId = SessionManager::getInstance().getCurrentUserId();

    if(!SessionManager::getInstance().isUserLoggedIn()){
        QMessageBox::critical(this, "Erro", "Nenhum Usuário Logado");
        return;
    }

    qDebug() << "Buscando chamados para o técnico ID:" << myId;

    ui->callsTable->setColumnHidden(6, true);

    m_facade->getCallList("", myId);

    ui->mainInfo->setCurrentIndex(1);

    ui->callType->setText("Meus Chamados");
}

void technician::on_backBtn_clicked()
{
    ui->mainInfo->setCurrentIndex(0);
}


void technician::on_backUserBtn_clicked()
{
    ui->mainInfo->setCurrentIndex(0);
}


void technician::on_usersBtn_clicked()
{
    m_facade->requestUserLists();

    ui->mainInfo->setCurrentIndex(2);
}

void technician::displayUserLists(const QList<UserInfo> &technicians, const QList<UserInfo> &requesters)
{
    ui->techUserList->clear();
    for (const UserInfo &user : technicians) {
        QListWidgetItem *item = new QListWidgetItem(user.name);
        item->setData(Qt::UserRole, user.id);
        ui->techUserList->addItem(item);
    }

    ui->requestUserList->clear();
    for (const UserInfo &user : requesters) {
        QListWidgetItem *item = new QListWidgetItem(user.name);
        item->setData(Qt::UserRole, user.id);
        ui->requestUserList->addItem(item);
    }
}

void technician::on_backUserEditBtn_clicked()
{
    ui->mainInfo->setCurrentIndex(2);
}


void technician::on_okCallsBtn_clicked()
{
    ui->mainInfo->setCurrentIndex(0);
}

void technician::onItemTableDoubleClick(QTableWidgetItem *item){
    if(!item){
        return;
    }

    int selectedRow = item->row();

    QTableWidgetItem *idItem = ui->callsTable->item(selectedRow, 0);

    if(!idItem){
        qWarning() << "ERRO CRÍTICO: O item na coluna 0 (ID) é NULO! Verifique a função preencherTabela.";
        return;
    }

    bool conversaoOk = false;
    int callId = idItem->text().toInt(&conversaoOk);

    if (!conversaoOk) {
        qWarning() << "ERRO CRÍTICO: O texto na coluna 0 não é um número válido:" << idItem->text();
        return;
    }

    qDebug() << "Duplo clique no chamado ID" << callId << ". Solicitando Detalhes...";
    m_facade->getCallDetails(callId);
}

void technician::showCallDetails(const CallInfo &info){
    qDebug() << "Recebendo detalhes para o chamado:" << info.titulo;

    m_currentCallId = info.id;

    ui->titleCall->setText(info.titulo);
    ui->responsibleLabel->setText(info.nome_tecnico);
    ui->problemDesc->setPlainText(info.descricao);

    if (info.status == "Fechado") {
        qDebug() << "Chamado está Fechado, escondendo o botão de atribuir.";
        ui->closedDate->setText("Concluído em: " + info.data_fechamento.toString("dd/MM/yyyy 'às' hh:mm"));
        ui->editResponBtn->setVisible(false);
        ui->addComentBtn->setVisible(false);
        ui->messageLineEdit->setVisible(false);
        ui->finishCallBtn->setVisible(false);
    } else {
        qDebug() << "Chamado está" << info.status << ", exibindo o botão de atribuir.";
        ui->editResponBtn->setVisible(true);
        ui->addComentBtn->setVisible(true);
        ui->messageLineEdit->setVisible(true);
        ui->finishCallBtn->setVisible(true);
    }

    m_facade->requestChatMessages(m_currentCallId);

    ui->mainInfo->setCurrentIndex(4);
}

void technician::onOperationFailed(const QString &errorMessage)
{
    QMessageBox::warning(this, "Atenção", errorMessage);
}

void technician::updateCountersDashboard(){
    int totalCalls = m_facade->callCount();
    int openedCalls = m_facade->callCount("Aberto");
    int analysisCalls = m_facade->callCount("Em Andamento");
    int closedCalls = m_facade->callCount("Fechado");

    ui->totalCalls->setText(QString("%1").arg(totalCalls));
    ui->newCalls->setText(QString("Chamados em \nAberto \n%1").arg(openedCalls));
    ui->analysisCalls->setText(QString("Chamados em \nAnálise \n%1").arg(analysisCalls));
    ui->completedCalls->setText(QString("Chamados \nConcluídos \n%1").arg(closedCalls));
}

void technician::populateTable(const QList<CallInfo> &calls){
    ui->callsTable->setUpdatesEnabled(false);
    ui->callsTable->clearContents();
    ui->callsTable->setRowCount(0);

    for(const CallInfo &call : calls){
        int currentRow = ui->callsTable->rowCount();

        ui->callsTable->insertRow(currentRow);

        // Preenchendo as novas colunas
        ui->callsTable->setItem(currentRow, 0, new QTableWidgetItem(QString::number(call.id)));
        ui->callsTable->setItem(currentRow, 1, new QTableWidgetItem(call.titulo));
        ui->callsTable->setItem(currentRow, 2, new QTableWidgetItem(call.nome_solicitante));
        ui->callsTable->setItem(currentRow, 3, new QTableWidgetItem(call.data_abertura.toString("dd/MM/yy hh:mm")));
        ui->callsTable->setItem(currentRow, 4, new QTableWidgetItem(call.tipo));
        ui->callsTable->setItem(currentRow, 5, new QTableWidgetItem(call.prioridade));

        QDateTime closingDate = call.data_fechamento;

        QString closingDateStr = closingDate.isValid() ? closingDate.toString("dd/MM/yy hh:mm") : "";
        ui->callsTable->setItem(currentRow, 6, new QTableWidgetItem(closingDateStr));
    }

    ui->callsTable->setUpdatesEnabled(true);
}

void technician::on_newCalls_clicked()
{
    qDebug() << "EM ABERTO: Usando Facade no endereço:" << m_facade;

    ui->mainInfo->setCurrentIndex(1);

    ui->callType->setText("Chamados em Abertos");

    ui->callsTable->setColumnHidden(6, true);

    m_facade->getCallList("Aberto");
}


void technician::on_analysisCalls_clicked()
{
    qDebug() << "EM ANÁLISE: Usando Facade no endereço:" << m_facade;

    ui->mainInfo->setCurrentIndex(1);

    ui->callType->setText("Chamados em Análise");

    ui->callsTable->setColumnHidden(6, true);

    m_facade->getCallList("Em Andamento");
}


void technician::on_completedCalls_clicked()
{
    qDebug() << "EM CONCLUÍDOS: Usando Facade no endereço:" << m_facade;

    ui->mainInfo->setCurrentIndex(1);

    ui->callType->setText("Chamados Concluídos");

    ui->callsTable->setColumnHidden(6, false);

    m_facade->getCallList("Fechado");
}



void technician::on_editResponBtn_clicked()
{
    qDebug() << "Botão de atribuir técnico clicado para o chamado ID:" << m_currentCallId;

    select_technician_dialog* dialog = new select_technician_dialog(m_facade, this);
    connect(dialog, &select_technician_dialog::technicianSelected, this, &technician::onTechnicianSelected);
    dialog->exec();
}

void technician::onTechnicianSelected(int technicianId)
{
    qDebug() << "Janela principal recebeu o ID do técnico:" << technicianId << "para o chamado" << m_currentCallId;
    // Manda a facade executar a ação final!
    m_facade->assignTechnicianToCall(m_currentCallId, technicianId);
}

void technician::onCallUpdateSuccess(const QString &message)
{
    // Mostra uma mensagem de sucesso para o usuário
    QMessageBox::information(this, "Sucesso", message);

    // Pede os detalhes do chamado novamente para mostrar o novo responsável
    m_facade->getCallDetails(m_currentCallId);

    // Atualiza os contadores do dashboard
    updateCountersDashboard();
}

void technician::displayChatMessages(const QList<ChatMessageInfo> &messages)
{
    // **Substitua 'chatListWidget' pelo nome real da sua QListWidget**
    ui->chatList->clear(); // Limpa a lista antes de adicionar as novas mensagens

    for (const ChatMessageInfo &msg : messages) {
        QString formattedMessage = QString("[%1] - %2 - [%3]")
        .arg(msg.senderName)
            .arg(msg.message)
            .arg(msg.timestamp.toString("dd/MM/yy hh:mm"));

        QListWidgetItem *item = new QListWidgetItem(formattedMessage);

        // Opcional: Alinhar a mensagem do usuário logado à direita para um visual de chat
        if (msg.senderName == SessionManager::getInstance().getCurrentUserName()) {
            item->setTextAlignment(Qt::AlignRight);
        } else {
            item->setTextAlignment(Qt::AlignLeft);
        }

        ui->chatList->addItem(item);
    }

    // Rola automaticamente para a última mensagem
    ui->chatList->scrollToBottom();
}

void technician::on_addComentBtn_clicked()
{
    QString messageText = ui->messageLineEdit->text().trimmed();

    if (!messageText.isEmpty()) {
        // Envia o ID do chamado atual e o texto da mensagem para a facade
        m_facade->sendChatMessage(m_currentCallId, messageText);

        // Limpa o campo de texto após o envio
        ui->messageLineEdit->clear();
    }
}


void technician::on_finishCallBtn_clicked()
{
    m_facade->closeCall(m_currentCallId);
}

void technician::refreshUserLists()
{
    qDebug() << "Recebido sinal de sucesso na operação de usuário. Recarregando listas...";
    // Apenas pede para a facade buscar os dados novamente.
    // O slot 'displayUserLists' que já existe fará o resto do trabalho.
    m_facade->requestUserLists();

    ui->mainInfo->setCurrentIndex(2);
}

void technician::onOperationSuccess(const QString &message)
{
    QMessageBox::information(this, "Sucesso", message);
}

void technician::on_rmvUserBtn_clicked()
{
    QListWidgetItem* selectedItem = nullptr;
    QString userRole = ""; // Para deixar a mensagem de confirmação mais clara

    // Primeiro, verificamos se há algo selecionado na lista de técnicos
    if (!ui->techUserList->selectedItems().isEmpty()) {
        // Se a lista de itens selecionados não está vazia, pegamos o primeiro (e único) item
        selectedItem = ui->techUserList->selectedItems().first();
        userRole = "técnico";
    }
    // Se não, verificamos a lista de solicitantes
    else if (!ui->requestUserList->selectedItems().isEmpty()) {
        selectedItem = ui->requestUserList->selectedItems().first();
        userRole = "solicitante";
    }

    // Se, depois de checar as duas listas, não encontramos nada, então nada está selecionado.
    if (!selectedItem) {
        QMessageBox::warning(this, "Atenção", "Por favor, selecione um usuário para remover.");
        return;
    }

    // O resto da lógica continua igual, mas agora com a certeza de ter o item certo
    int userId = selectedItem->data(Qt::UserRole).toInt();
    QString userName = selectedItem->text();

    QMessageBox::StandardButton reply;
    // Mensagem de confirmação melhorada!
    reply = QMessageBox::question(this, "Confirmar Remoção",
                                  QString("Tem certeza que deseja remover o %1 '%2'?")
                                      .arg(userRole)
                                      .arg(userName),
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_facade->deleteUser(userId);
    }
}


void technician::on_editUserBtn_clicked()
{
    // --- Passo 1: Descobrir o item selecionado de forma robusta ---
    QListWidgetItem* selectedItem = nullptr;
    if (!ui->techUserList->selectedItems().isEmpty()) {
        selectedItem = ui->techUserList->selectedItems().first();
    }
    else if (!ui->requestUserList->selectedItems().isEmpty()) {
        selectedItem = ui->requestUserList->selectedItems().first();
    }

    // Se nada foi selecionado, exibe um aviso e para.
    if (!selectedItem) {
        QMessageBox::warning(this, "Atenção", "Por favor, selecione um usuário para editar.");
        return;
    }

    // --- Passo 2: Pegar o ID do usuário ---
    int userId = selectedItem->data(Qt::UserRole).toInt();

    // --- Passo 3 (A LÓGICA CORRETA PARA STACKEDWIDGET): ---
    // Apenas guardamos o ID e pedimos os detalhes para a Facade.
    // A tela NÃO muda aqui. Ela só vai mudar quando a Facade responder.
    qDebug() << "Botão Editar clicado para o usuário ID:" << userId << ". Solicitando detalhes...";
    m_currentUserEditingId = userId;
    m_facade->requestUserDetails(userId);
}


void technician::on_createUserBtn_clicked()
{
    // 1. Define o modo: 0 significa "criar novo"
    m_currentUserEditingId = 0;

    // 2. Limpa os campos do formulário para garantir que não há dados antigos
    ui->nameLineEdit->clear();
    ui->loginLineEdit->clear();
    ui->passLineEdit->clear();
    ui->passLineEdit->setPlaceholderText("Digite uma senha (obrigatório)");
    ui->typeComboBox->setCurrentIndex(0); // Põe de volta na primeira opção

    // 3. Muda o título da página
    ui->titleUser->setText("Novo Usuário"); // Supondo que você tenha um label para o título

    // 4. Muda para a página de detalhes do usuário
    // **Substitua '5' pelo índice correto da sua página de detalhes no QStackedWidget**
    ui->mainInfo->setCurrentIndex(3);
}

void technician::populateUserDetailsForm(const UserInfo &info)
{
    ui->nameLineEdit->setText(info.name);
    ui->loginLineEdit->setText(info.email);
    ui->passLineEdit->clear();
    ui->passLineEdit->setPlaceholderText("Deixe em branco para não alterar");
    ui->typeComboBox->setCurrentText(info.type);

    ui->titleUser->setText("Editar Usuário");

    // Agora que os dados estão prontos, mudamos de tela
    ui->mainInfo->setCurrentIndex(3);
}

void technician::on_okEditUserBtn_clicked()
{
    // Validação básica
    if (ui->nameLineEdit->text().isEmpty() || ui->loginLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Atenção", "Nome e E-mail são campos obrigatórios.");
        return;
    }
    // No modo de criação (ID=0), a senha também é obrigatória
    if (m_currentUserEditingId == 0 && ui->passLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Atenção", "A senha é obrigatória para novos usuários.");
        return;
    }

    // Pede para a facade salvar, passando o ID que guardamos
    m_facade->saveUser(m_currentUserEditingId,
                       ui->nameLineEdit->text(),
                       ui->loginLineEdit->text(),
                       ui->passLineEdit->text(),
                       ui->typeComboBox->currentText());

    // Após salvar, podemos voltar para a lista de usuários.
    // Opcional: só fazer isso no slot de sucesso (onUserSaveSuccess)
    // ui->mainStackedWidget->setCurrentIndex(4); // **Substitua 4 pelo índice da lista**
}

void technician::onTechnicianSelectionChanged()
{
    // Se o usuário realmente selecionou um item na lista de técnicos
    // (e não apenas limpou a seleção), então...
    if (!ui->techUserList->selectedItems().isEmpty()) {
        // ...limpa programaticamente a seleção da outra lista.
        ui->requestUserList->clearSelection();
    }
}

void technician::onRequesterSelectionChanged()
{
    // Se o usuário realmente selecionou um item na lista de solicitantes...
    if (!ui->requestUserList->selectedItems().isEmpty()) {
        // ...limpa programaticamente a seleção da outra lista.
        ui->techUserList->clearSelection();
    }
}


void technician::on_logoutTechnichianBtn_clicked()
{
    qDebug() << "Usuário" << SessionManager::getInstance().getCurrentUserName() << "está deslogando.";

    SessionManager::getInstance().logout();

    Principal *loginWindow = new Principal();
    loginWindow->show();

    this->close();
}

