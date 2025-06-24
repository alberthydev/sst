#include "requester.h"
#include "ui_requester.h"
#include "session_manager.h"
#include <QMessageBox>
#include <QTextBrowser>
#include <QIcon>

requester::requester(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::requester)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/sst.png"));

    // Index Pages
    // 0 - Main Window
    // 1 - Open Call
    // 2 - My Calls
    // 3 - Generic Call

    // Facade Object
    m_facade = new facade_call_system(this);

    connect(m_facade, &facade_call_system::operationFailed, this, &requester::onOperationFailed);
    connect(m_facade, &facade_call_system::callCreatedSuccessfully, this, &requester::onCallCreationSuccess);
    connect(ui->requesterCallsTableWidget, &QTableWidget::itemDoubleClicked, this, &requester::onMyCallDoubleClicked);
    connect(m_facade, &facade_call_system::readyCallDetails, this, &requester::displayCallDetails);
    connect(m_facade, &facade_call_system::readyCallList, this, &requester::populateCallsTable);
    connect(m_facade, &facade_call_system::chatMessagesReady, this, &requester::displayChatMessages);
    connect(ui->messageLineEdit, &QLineEdit::returnPressed, this, &requester::on_sendMessageBtn_clicked);
    connect(ui->titleLineEdit, &QLineEdit::returnPressed, this, &requester::on_sendCallBtn_clicked);

    // Colors
    QColor whiteBk = 0xf9f9f9;
    QColor blueBk = 0x3383ff;
    QColor lightBlueBk = 0x5597ff;

    ui->requesterUserName->setText(SessionManager::getInstance().getCurrentUserName());

    // Requester Calls Table
    ui->requesterCallsTableWidget->setColumnCount(7);
    ui->requesterCallsTableWidget->verticalHeader()->setVisible(false);

    QStringList headers = {"Título", "Solicitante", "Abertura", "Tipo", "Prioridade", "Status"};
    ui->requesterCallsTableWidget->setColumnHidden(0, true);
    ui->requesterCallsTableWidget->setHorizontalHeaderLabels(QStringList() << "" << headers);

    ui->requesterCallsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->requesterCallsTableWidget->setWordWrap(true);
    ui->requesterCallsTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->requesterCallsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->requesterCallsTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // Left Menu Container
    ui->requesterCallsBtn->setConfigs("Meus Chamados", whiteBk, blueBk, lightBlueBk, "admin");
    ui->logoutRequesterBtn->setConfigs("Sair", whiteBk, blueBk, lightBlueBk, "admin");

    // Buttons
    ui->openCallBtn->setConfigs("Novo Chamado", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->goBackRequesterCallsBtn->setConfigs("\u2190", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->goBackNewCallBtn->setConfigs("\u2190", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->sendCallBtn->setConfigs("Enviar Chamado", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->goBackGenericCallBtn->setConfigs("\u2190", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->sendMessageBtn->setConfigs("\u2191", whiteBk, blueBk, lightBlueBk, "defaultBtn");

    // Placeholders
    ui->titleLineEdit->setPlaceholderText("Digite um título breve do problema");
    ui->descTextEdit->setPlaceholderText("Descreva o problema em detalhes aqui!");

    // Populate ComboBox
    ui->typeComboBox->addItems({"Sistema", "Impressora", "Hardware", "Rede", "Outros"});
    ui->priorityComboBox->addItems({"Baixo", "Médio", "Alto", "Urgente"});

    // Chat
    ui->chatListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->chatListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

requester::~requester()
{
    delete ui;
}

void requester::onOperationFailed(const QString &errorMessage)
{
    QMessageBox::warning(this, "Atenção", errorMessage);
}

void requester::on_openCallBtn_clicked()
{
    ui->main->setCurrentIndex(1);
}


void requester::on_goBackNewCallBtn_clicked()
{
    ui->main->setCurrentIndex(0);
}


void requester::on_goBackRequesterCallsBtn_clicked()
{
    ui->main->setCurrentIndex(0);
}


void requester::on_requesterCallsBtn_clicked()
{
    int myId = SessionManager::getInstance().getCurrentUserId();

    if (!SessionManager::getInstance().isUserLoggedIn()) {
        QMessageBox::critical(this, "Erro", "Nenhum usuário logado para buscar os chamados.");
        return;
    }

    qDebug() << "Botão 'Meus Chamados' clicado. Buscando chamados para o solicitante ID:" << myId;

    m_facade->getCallList("", 0, myId);

    ui->main->setCurrentIndex(2);
}

void requester::populateCallsTable(const QList<CallInfo> &calls)
{
    qDebug() << "[UI DEBUG] Slot populateCallsTable foi chamado. Número de chamados recebidos:" << calls.size();

    ui->requesterCallsTableWidget->setUpdatesEnabled(false);
    ui->requesterCallsTableWidget->clearContents();
    ui->requesterCallsTableWidget->setRowCount(0);

    for (const CallInfo &call : calls) {
        int currentRow = ui->requesterCallsTableWidget->rowCount();
        ui->requesterCallsTableWidget->insertRow(currentRow);

        ui->requesterCallsTableWidget->setItem(currentRow, 0, new QTableWidgetItem(QString::number(call.id)));
        ui->requesterCallsTableWidget->setItem(currentRow, 1, new QTableWidgetItem(call.titulo));
        ui->requesterCallsTableWidget->setItem(currentRow, 2, new QTableWidgetItem(call.nome_solicitante));
        ui->requesterCallsTableWidget->setItem(currentRow, 3, new QTableWidgetItem(call.data_abertura.toLocalTime().toString("dd/MM/yy hh:mm")));
        ui->requesterCallsTableWidget->setItem(currentRow, 4, new QTableWidgetItem(call.tipo));
        ui->requesterCallsTableWidget->setItem(currentRow, 5, new QTableWidgetItem(call.prioridade));
        ui->requesterCallsTableWidget->setItem(currentRow, 6, new QTableWidgetItem(call.status));
    }

    ui->requesterCallsTableWidget->setUpdatesEnabled(true);
}

void requester::on_goBackGenericCallBtn_clicked()
{
    ui->main->setCurrentIndex(2);
}


void requester::on_sendCallBtn_clicked()
{
    QString title = ui->titleLineEdit->text().trimmed();
    QString type = ui->typeComboBox->currentText();
    QString priority = ui->priorityComboBox->currentText();
    QString description = ui->descTextEdit->toPlainText().trimmed();

    if (title.isEmpty()) {
        QMessageBox::warning(this, "Atenção", "O campo 'Título' é obrigatório.");
        return;
    }

    m_facade->createNewCall(title, type, priority, description);
}

void requester::onCallCreationSuccess(const QString &message)
{
    QMessageBox::information(this, "Sucesso", message);

    ui->titleLineEdit->clear();
    ui->descTextEdit->clear();
    ui->typeComboBox->setCurrentIndex(0);
    ui->priorityComboBox->setCurrentIndex(0);

    on_requesterCallsBtn_clicked();
    ui->main->setCurrentIndex(2);
}

void requester::onMyCallDoubleClicked(QTableWidgetItem *item)
{
    if (!item) return; // Verificação de segurança

    int row = item->row();

    QTableWidgetItem *idItem = ui->requesterCallsTableWidget->item(row, 0);

    if (!idItem) {
        qWarning() << "Não foi possível encontrar o item de ID na linha selecionada.";
        return;
    }

    int callId = idItem->text().toInt();

    qDebug() << "Solicitante deu duplo clique no chamado ID:" << callId << ". Buscando detalhes...";

    m_facade->getCallDetails(callId);
}

void requester::displayCallDetails(const CallInfo &info)
{
    qDebug() << "Recebendo detalhes para preencher a tela do chamado:" << info.titulo;

    m_currentCallId = info.id;

    ui->titleLabel->setText(info.titulo);
    ui->descTextBrowser->setPlainText(info.descricao);
    ui->responLabel->setText(info.nome_tecnico);
    if(info.status != "Fechado") {
        ui->statusClosedDateLabel->setText("Status: " + info.status);
    } else {
        ui->messageLineEdit->setHidden(true);
        ui->sendMessageBtn->setHidden(true);
        if (info.data_fechamento.isValid()) {
            QString formattedDate = info.data_fechamento.toLocalTime().toString("dd/MM/yyyy 'às' hh:mm");
            ui->statusClosedDateLabel->setText("Concluído em: " + formattedDate);
        } else {
            ui->statusClosedDateLabel->setText("Status: Fechado");
        }
    }

    m_facade->requestChatMessages(info.id);

    ui->main->setCurrentIndex(3);
}

void requester::displayChatMessages(const QList<ChatMessageInfo> &messages)
{
    qDebug() << "Recebendo" << messages.size() << "mensagens para exibir no chat.";
    ui->chatListWidget->setUpdatesEnabled(false);
    ui->chatListWidget->clear();

    ui->main->setCurrentIndex(3);

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

void requester::on_sendMessageBtn_clicked()
{
    QString message = ui->messageLineEdit->text().trimmed();

    if (message.isEmpty()) {
        return;
    }

    if (m_currentCallId <= 0) {
        qWarning() << "Tentativa de enviar mensagem sem um chamado aberto válido.";
        return;
    }

    qDebug() << "Solicitante enviando mensagem para o chamado ID:" << m_currentCallId;

    m_facade->sendChatMessage(m_currentCallId, message);

    ui->messageLineEdit->clear();
}


void requester::on_logoutRequesterBtn_clicked()
{
    qDebug() << "Usuario" << SessionManager::getInstance().getCurrentUserName() << "esta deslogando.";

    SessionManager::getInstance().logout();

    Principal *loginWindow = new Principal();
    loginWindow->show();

    this->close();
}

