#include "principal.h"
#include "ui_principal.h"
#include "session_manager.h"
#include "technician.h"
#include "requester.h"
#include <QMessageBox>
#include <QIcon>

Principal::Principal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principal)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/sst.png"));

    m_facade = new facade_call_system(this);

    connect(m_facade, &facade_call_system::loginSuccessful, this, &Principal::onLoginSuccess);
    connect(m_facade, &facade_call_system::operationFailed, this, &Principal::onLoginFailed);
    connect(ui->userLoginLineEdit, &QLineEdit::returnPressed, this, &Principal::on_loginBtn_clicked);
    connect(ui->userPassLineEdit, &QLineEdit::returnPressed, this, &Principal::on_loginBtn_clicked);

    // Colors
    QColor whiteBk = 0xf9f9f9;
    QColor blueBk = 0x3383ff;
    QColor lightBlueBk = 0x5597ff;

    // Login Button
    ui->loginBtn->setConfigs("ENTRAR", whiteBk, blueBk, lightBlueBk, "loginBtn");
    ui->loginBtn->setDefault(true);

    // Show Password Button
    ui->showPassBtn->setConfigs("O", whiteBk, blueBk, lightBlueBk, "showPassBtn");
}

Principal::~Principal()
{
    delete ui;
}

void Principal::on_loginBtn_clicked()
{
    QString email = ui->userLoginLineEdit->text().trimmed();
    QString password = ui->userPassLineEdit->text();

    if (email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Atenção", "Por favor, preencha o e-mail e a senha.");
        return;
    }

    m_facade->validateLogin(email, password);
}


void Principal::on_showPassBtn_clicked()
{
    if (ui->userPassLineEdit->echoMode() == QLineEdit::Password) {
        ui->userPassLineEdit->setEchoMode(QLineEdit::Normal);
        ui->showPassBtn->setText("I");
    } else {
        ui->userPassLineEdit->setEchoMode(QLineEdit::Password);
        ui->showPassBtn->setText("O");
    }
}

void Principal::onLoginSuccess(int userId, const QString &name, const QString &userType)
{
    SessionManager::getInstance().login(userId, name, userType);

    if (userType == "Tecnico" || userType == "Admin") {
        technician *techWindow = new technician(); // Cria a janela de técnico
        techWindow->show();
    } else { // if (userType == "Solicitante")
        requester *reqWindow = new requester(); // Cria a janela de solicitante
        reqWindow->show();
    }

    this->close();
}

void Principal::onLoginFailed(const QString &errorMessage)
{
    QMessageBox::critical(this, "Falha no Login", errorMessage);
}
