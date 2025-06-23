#include "requester.h"
#include "ui_requester.h"
#include "session_manager.h"
#include <QMessageBox>
#include <QTextBrowser>

requester::requester(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::requester)
{
    ui->setupUi(this);

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

    // Default Colors
    QColor whiteBk = "#f9f9f9";
    QColor blueBk = "#3383ff";
    QColor lightBlueBk = "#5597ff";

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
    // 1. Pega o ID do usuário logado na sessão
    int myId = SessionManager::getInstance().getCurrentUserId();

    // Verificação de segurança
    if (!SessionManager::getInstance().isUserLoggedIn()) {
        QMessageBox::critical(this, "Erro", "Nenhum usuário logado para buscar os chamados.");
        return;
    }

    qDebug() << "Botão 'Meus Chamados' clicado. Buscando chamados para o solicitante ID:" << myId;

    // 2. Pede para a Facade a lista filtrada pelo ID do solicitante
    //    Parâmetros: (status="", technicianId=0, requesterId=myId)
    m_facade->getCallList("", 0, myId);

    // 3. Muda para a tela da tabela
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
        ui->requesterCallsTableWidget->setItem(currentRow, 3, new QTableWidgetItem(call.data_abertura.toString("dd/MM/yy hh:mm")));
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
    // Pega os dados dos campos do formulário
    QString title = ui->titleLineEdit->text().trimmed();
    QString type = ui->typeComboBox->currentText();
    QString priority = ui->priorityComboBox->currentText();
    QString description = ui->descTextEdit->toPlainText().trimmed();

    // Validação simples para garantir que o título não está vazio
    if (title.isEmpty()) {
        QMessageBox::warning(this, "Atenção", "O campo 'Título' é obrigatório.");
        return;
    }

    // Pede para a Facade criar o chamado com os dados do formulário
    m_facade->createNewCall(title, type, priority, description);
}

void requester::onCallCreationSuccess(const QString &message)
{
    // Mostra uma mensagem de confirmação para o usuário
    QMessageBox::information(this, "Sucesso", message);

    // Limpa o formulário para que o usuário possa criar outro chamado se quiser
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

    // Pega o item da coluna 0 (que está oculta), onde guardamos o ID
    QTableWidgetItem *idItem = ui->requesterCallsTableWidget->item(row, 0);

    if (!idItem) {
        qWarning() << "Não foi possível encontrar o item de ID na linha selecionada.";
        return;
    }

    int ticketId = idItem->text().toInt();

    qDebug() << "Solicitante deu duplo clique no chamado ID:" << ticketId << ". Buscando detalhes...";

    // Pede para a facade buscar todos os detalhes daquele chamado
    m_facade->getCallDetails(ticketId);
}

void requester::displayCallDetails(const CallInfo &info)
{
    qDebug() << "Recebendo detalhes para preencher a tela do chamado:" << info.titulo;

    m_currentCallId = info.id;

    ui->titleLabel->setText(info.titulo);
    ui->descTextBrowser->setPlainText(info.descricao);
    ui->responLabel->setText(info.nome_tecnico);
    if(info.status != "Fechado") {
        // Se não está fechado, mostra o status atual.
        ui->statusClosedDateLabel->setText("Status: " + info.status);
    } else {
        ui->messageLineEdit->setHidden(true);
        ui->sendMessageBtn->setHidden(true);
        // Se está fechado, pega a data de fechamento e formata.
        // Usamos .isValid() para garantir que a data não é nula.
        if (info.data_fechamento.isValid()) {
            QString formattedDate = info.data_fechamento.toString("dd/MM/yyyy 'às' hh:mm");
            ui->statusClosedDateLabel->setText("Concluído em: " + formattedDate);
        } else {
            // Caso de segurança se a data for nula por algum motivo
            ui->statusClosedDateLabel->setText("Status: Fechado");
        }
    }

    // Carrega o histórico do chat para este chamado
    m_facade->requestChatMessages(info.id);

    // Finalmente, muda para a tela de detalhes do chamado
    ui->main->setCurrentIndex(3);
}

void requester::displayChatMessages(const QList<ChatMessageInfo> &messages)
{
    qDebug() << "Recebendo" << messages.size() << "mensagens para exibir no chat.";
    ui->chatListWidget->setUpdatesEnabled(false);
    ui->chatListWidget->clear();

    // PASSO 1: TORNE A PÁGINA DO CHAT VISÍVEL PRIMEIRO.
    ui->main->setCurrentIndex(3);

    // PASSO 2: FORCE O QT A PROCESSAR TODAS AS MUDANÇAS PENDENTES.
    // Esta é a linha "mágica" que resolve problemas de timing de UI.
    // Ela garante que a página foi desenhada e seus widgets têm tamanho real.
    QCoreApplication::processEvents();

    // PASSO 3: AGORA, COM A TELA JÁ VISÍVEL, PODEMOS POPULAR A LISTA COM SEGURANÇA.
    // O seu código para preencher a lista já está correto.
    ui->chatListWidget->setUpdatesEnabled(false);
    ui->chatListWidget->clear();

    for (const ChatMessageInfo &msg : messages) {
        // --- LÓGICA FINAL COM ALINHAMENTO DE BALÃO ---

        // 1. Determina se a mensagem é do usuário logado
        bool isCurrentUser = (msg.senderName == SessionManager::getInstance().getCurrentUserName());

        // 2. Cria o nosso "balão de chat" (o QTextBrowser)
        QTextBrowser *messageBrowser = new QTextBrowser();
        messageBrowser->setFrameStyle(QFrame::NoFrame);
        messageBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        messageBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        // Formatamos o conteúdo com HTML
        QString html = QString("<b>%1</b><br>%2<br><span style='font-size: 8pt; color: #888;'>%3</span>")
                           .arg(msg.senderName)
                           .arg(msg.message.toHtmlEscaped()) // .toHtmlEscaped() para segurança
                           .arg(msg.timestamp.toString("dd/MM/yy hh:mm"));
        messageBrowser->setHtml(html);

        // Estilizamos o balão com cores de fundo diferentes
        QString bubbleStyle = isCurrentUser
                                  ? "background-color: #dcf8c6; border-radius: 10px; padding: 8px;" // Verde claro (enviado)
                                  : "background-color: #ffffff; border-radius: 10px; padding: 8px;";  // Branco (recebido)
        messageBrowser->setStyleSheet(bubbleStyle);

        // 3. Cria o widget container e o layout horizontal
        QWidget *containerWidget = new QWidget();
        containerWidget->setStyleSheet("border:none;");
        QHBoxLayout *layout = new QHBoxLayout(containerWidget);
        layout->setContentsMargins(5, 5, 5, 5); // Margem para a linha inteira

        // 4. A MÁGICA DO ALINHAMENTO
        if (isCurrentUser) {
            layout->addStretch(); // Mola elástica à ESQUERDA, empurrando o balão para a direita
            layout->addWidget(messageBrowser);
        } else {
            layout->addWidget(messageBrowser);
            layout->addStretch(); // Mola elástica à DIREITA, empurrando o balão para a esquerda
        }

        // 5. Crie e adicione o item da lista
        QListWidgetItem *item = new QListWidgetItem();
        ui->chatListWidget->addItem(item);

        // 6. Coloque o CONTAINER (e não o browser diretamente) dentro do item
        ui->chatListWidget->setItemWidget(item, containerWidget);

        // 7. Cálculo final da altura
        // Limitamos a largura do balão a 75% da tela para um visual melhor
        messageBrowser->document()->setTextWidth(ui->chatListWidget->viewport()->width() * 0.75);
        QSize size = messageBrowser->document()->size().toSize();
        item->setSizeHint(QSize(size.width(), size.height() + 50)); // +10 de padding
    }

    ui->chatListWidget->setUpdatesEnabled(true);
    ui->chatListWidget->scrollToBottom();
}

void requester::on_sendMessageBtn_clicked()
{
    // Substitua 'chatLineEdit' pelo nome real do seu QLineEdit
    QString message = ui->messageLineEdit->text().trimmed();

    // 1. Verifica se a mensagem não está vazia
    if (message.isEmpty()) {
        return;
    }

    // 2. Garante que sabemos qual chamado está aberto
    if (m_currentCallId <= 0) {
        qWarning() << "Tentativa de enviar mensagem sem um chamado aberto válido.";
        return;
    }

    qDebug() << "Solicitante enviando mensagem para o chamado ID:" << m_currentCallId;

    // 3. Pede para a Facade enviar a mensagem.
    // A Facade usará o SessionManager para saber que o REMETENTE é o solicitante logado.
    m_facade->sendChatMessage(m_currentCallId, message);

    // 4. Limpa o campo de texto após o envio para uma nova mensagem.
    ui->messageLineEdit->clear();
}


void requester::on_logoutRequesterBtn_clicked()
{
    qDebug() << "Usuário" << SessionManager::getInstance().getCurrentUserName() << "está deslogando.";

    SessionManager::getInstance().logout();

    Principal *loginWindow = new Principal();
    loginWindow->show();

    this->close();
}

