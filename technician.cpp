#include "technician.h"
#include "select_technician_dialog.h"
#include "ui_technician.h"
#include "session_manager.h"
#include <QMessageBox>
#include <QTextBrowser>
#include <QIcon>

technician::technician(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::technician)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/sst.png"));

    // PAGES INDEX
    // 0 - Main
    // 1 - Calls Generico
    // 2 - Users
    // 3 - Criar/Editar User
    // 4 - Chamado Generico

    // Facade Object
    m_facade = new facade_call_system(this);

    connect(m_facade, &facade_call_system::readyCallList, this, &technician::populateTable);
    connect(m_facade, &facade_call_system::readyCallDetails, this, &technician::displayCallDetails);
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
    connect(ui->messageLineEdit, &QLineEdit::returnPressed, this, &technician::on_addComentBtn_clicked);

    // Colors
    QColor whiteBk = 0xf9f9f9;
    QColor blueBk = 0x3383ff;
    QColor lightBlueBk = 0x5597ff;

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
    ui->rmvUserBtn->setConfigs("Alterar Status", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->editUserBtn->setConfigs("Editar Usuário", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->createUserBtn->setConfigs("Criar Usuário", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->okCallsBtn->setConfigs("\u2190", whiteBk, blueBk, lightBlueBk, "defaultBtn");

    // User Type ComboBox
    ui->typeComboBox->addItems({"Tecnico", "Solicitante"});

    // Chat
    ui->chatListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->chatListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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
        QString statusLabel = user.isActive ? "[ATIVO]" : "[INATIVO]";
        QString displayText = QString("%1 %2").arg(user.name).arg(statusLabel);
        QListWidgetItem *item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, user.id);
        item->setData(Qt::UserRole + 1, user.isActive);

        if (!user.isActive) {
            item->setForeground(Qt::gray);
        }

        ui->techUserList->addItem(item);
    }

    ui->requestUserList->clear();
    for (const UserInfo &user : requesters) {
        QString statusLabel = user.isActive ? "[ATIVO]" : "[INATIVO]";
        QString displayText = QString("%1 %2").arg(user.name).arg(statusLabel);
        QListWidgetItem *item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, user.id);
        item->setData(Qt::UserRole + 1, user.isActive);

        if (!user.isActive) {
            item->setForeground(Qt::gray);
        }

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

void technician::displayCallDetails(const CallInfo &info){
    qDebug() << "Recebendo detalhes para o chamado:" << info.titulo;

    m_currentCallId = info.id;

    ui->titleCall->setText(info.titulo);
    ui->responsibleLabel->setText(info.nome_tecnico);
    ui->problemDesc->setPlainText(info.descricao);

    if (info.status == "Fechado") {
        qDebug() << "Chamado está Fechado, escondendo o botão de atribuir.";
        ui->closedDate->setText("Concluído em: " + info.data_fechamento.toLocalTime().toString("dd/MM/yyyy 'às' hh:mm"));
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

        // Populating Table
        ui->callsTable->setItem(currentRow, 0, new QTableWidgetItem(QString::number(call.id)));
        ui->callsTable->setItem(currentRow, 1, new QTableWidgetItem(call.titulo));
        ui->callsTable->setItem(currentRow, 2, new QTableWidgetItem(call.nome_solicitante));
        ui->callsTable->setItem(currentRow, 3, new QTableWidgetItem(call.data_abertura.toLocalTime().toString("dd/MM/yy hh:mm")));
        ui->callsTable->setItem(currentRow, 4, new QTableWidgetItem(call.tipo));
        ui->callsTable->setItem(currentRow, 5, new QTableWidgetItem(call.prioridade));

        QDateTime closingDate = call.data_fechamento;

        QString closingDateStr = closingDate.isValid() ? closingDate.toLocalTime().toString("dd/MM/yy hh:mm") : "";
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
    m_facade->assignTechnicianToCall(m_currentCallId, technicianId);
}

void technician::onCallUpdateSuccess(const QString &message)
{
    QMessageBox::information(this, "Sucesso", message);
    m_facade->getCallDetails(m_currentCallId);
    updateCountersDashboard();
}

void technician::displayChatMessages(const QList<ChatMessageInfo> &messages)
{
    qDebug() << "Recebendo" << messages.size() << "mensagens para exibir no chat.";
    ui->chatListWidget->setUpdatesEnabled(false);
    ui->chatListWidget->clear();

    ui->mainInfo->setCurrentIndex(4);

    // Coloquei aqui para testar ver se para de bugar essa porcaria
    QCoreApplication::processEvents();

    // Populando com as informações
    ui->chatListWidget->setUpdatesEnabled(false);
    ui->chatListWidget->clear();

    for (const ChatMessageInfo &msg : messages) {
        // Checa se a mensagem é do usuario logado
        bool isCurrentUser = (msg.senderName == SessionManager::getInstance().getCurrentUserName());

        // Teste de balao do chat
        QTextBrowser *messageBrowser = new QTextBrowser();
        messageBrowser->setFrameStyle(QFrame::NoFrame);
        messageBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        messageBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        QString html = QString("<b>%1</b><br>%2<br><span style='font-size: 8pt; color: #888;'>%3</span>")
                           .arg(msg.senderName)
                           .arg(msg.message.toHtmlEscaped()) // .toHtmlEscaped() para segurança
                           .arg(msg.timestamp.toLocalTime().toString("dd/MM/yy hh:mm"));
        messageBrowser->setHtml(html);

        // Estilização
        QString bubbleStyle = isCurrentUser
                                  ? "background-color: #dcf8c6; border-radius: 10px; padding: 8px;"
                                  : "background-color: #ffffff; border-radius: 10px; padding: 8px;";
        messageBrowser->setStyleSheet(bubbleStyle);

        // Container para ficar o balão
        QWidget *containerWidget = new QWidget();
        containerWidget->setStyleSheet("border:none;");
        QHBoxLayout *layout = new QHBoxLayout(containerWidget);
        layout->setContentsMargins(5, 5, 5, 5);

        // Alinhamento para imitar um chat real
        if (isCurrentUser) {
            layout->addStretch();
            layout->addWidget(messageBrowser);
        } else {
            layout->addWidget(messageBrowser);
            layout->addStretch();
        }

        QListWidgetItem *item = new QListWidgetItem();
        ui->chatListWidget->addItem(item);
        ui->chatListWidget->setItemWidget(item, containerWidget);

        // Teste de calculo de altura com limite de 75% para melhorar a visualização
        messageBrowser->document()->setTextWidth(ui->chatListWidget->viewport()->width() * 0.75);
        QSize size = messageBrowser->document()->size().toSize();
        item->setSizeHint(QSize(size.width(), size.height() + 50));
    }

    ui->chatListWidget->setUpdatesEnabled(true);
    ui->chatListWidget->scrollToBottom();
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
    // Só pede para o facade buscar os dados novamente.
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
    QString userRole = "";

    if (!ui->techUserList->selectedItems().isEmpty()) {
        selectedItem = ui->techUserList->selectedItems().first();
        userRole = "técnico";
    }

    else if (!ui->requestUserList->selectedItems().isEmpty()) {
        selectedItem = ui->requestUserList->selectedItems().first();
        userRole = "solicitante";
    }

    if (!selectedItem) {
        QMessageBox::warning(this, "Atenção", "Por favor, selecione um usuário para alterar o status.");
        return;
    }

    // Verificando se ta correto a seleção, por que tava bugando tudo
    int userId = selectedItem->data(Qt::UserRole).toInt();
    bool isActive = selectedItem->data(Qt::UserRole+1).toBool();
    QString actionText = isActive ? "desativar" : "ativar";
    QString userName = selectedItem->text().section(" [", 0, 0);

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmar Remoção",
                                  QString("Tem certeza que deseja alterar o status do %1 '%2'?")
                                      .arg(userRole)
                                      .arg(userName),
                                  QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_facade->toggleUserStatus(userId);
    }
}


void technician::on_editUserBtn_clicked()
{
    QListWidgetItem* selectedItem = nullptr;
    if (!ui->techUserList->selectedItems().isEmpty()) {
        selectedItem = ui->techUserList->selectedItems().first();
    }
    else if (!ui->requestUserList->selectedItems().isEmpty()) {
        selectedItem = ui->requestUserList->selectedItems().first();
    }

    if (!selectedItem) {
        QMessageBox::warning(this, "Atenção", "Por favor, selecione um usuário para editar.");
        return;
    }

    int userId = selectedItem->data(Qt::UserRole).toInt();

    qDebug() << "Botão Editar clicado para o usuário ID:" << userId << ". Solicitando detalhes...";
    m_currentUserEditingId = userId;
    m_facade->requestUserDetails(userId);
}


void technician::on_createUserBtn_clicked()
{
    // ID = 0 significa que esta criando um novo usuario
    m_currentUserEditingId = 0;

    ui->nameLineEdit->clear();
    ui->loginLineEdit->clear();
    ui->passLineEdit->clear();
    ui->passLineEdit->setPlaceholderText("Digite uma senha (obrigatório)");
    ui->typeComboBox->setCurrentIndex(0);

    ui->titleUser->setText("Novo Usuário");

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

    ui->mainInfo->setCurrentIndex(3);
}

void technician::on_okEditUserBtn_clicked()
{
    if (ui->nameLineEdit->text().isEmpty() || ui->loginLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Atenção", "Nome e E-mail são campos obrigatórios.");
        return;
    }
    // ID=0 a senha é obrigatória por que é um novo usuario
    if (m_currentUserEditingId == 0 && ui->passLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Atenção", "A senha é obrigatória para novos usuários.");
        return;
    }

    // Chama o Facade
    m_facade->saveUser(m_currentUserEditingId,
                       ui->nameLineEdit->text(),
                       ui->loginLineEdit->text(),
                       ui->passLineEdit->text(),
                       ui->typeComboBox->currentText());
}

void technician::onTechnicianSelectionChanged()
{
    // Tentativa de resolver o bug de seleção de usuário
    if (!ui->techUserList->selectedItems().isEmpty()) {
        ui->requestUserList->clearSelection();
    }
}

void technician::onRequesterSelectionChanged()
{
    // Tentativa de resolver o bug de seleção de usuário
    if (!ui->requestUserList->selectedItems().isEmpty()) {
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

