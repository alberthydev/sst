#include "technician.h"
#include "ui_technician.h"

technician::technician(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::technician)
{
    ui->setupUi(this);

    QColor whiteBk = "#f9f9f9";
    QColor blueBk = "#3383ff";
    QColor lightBlueBk = "#5597ff";

    // Calls Container
    ui->newCalls->setConfigs("Chamados em \nAberto \n0", "#ff2d55", "#ffa2b3", "#ff8aa0", "calls");
    ui->analysisCalls->setConfigs("Chamados em \nAnalise \n0", "#f4d700", QColor("#faffac"), QColor("#f7ff7e"), "calls");
    ui->completedCalls->setConfigs("Chamados \nConcluídos \n0", "#69d800", QColor("#d0ffa3"), QColor("#b5ff6e"),  "calls");

    // Admin Container
    ui->myCallsBtn->setConfigs("Meus Chamados", whiteBk, blueBk, lightBlueBk, "admin");
    ui->usersBtn->setConfigs("Usuários", whiteBk, blueBk, lightBlueBk, "admin");
    ui->logoutBtn->setConfigs("Sair", whiteBk, blueBk, lightBlueBk, "admin");

    // Buttons
    ui->goBackBtn->setConfigs("\u2190", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->backBtn->setConfigs("\u2190", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->finishCallBtn->setConfigs("Concluir Chamado", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->addComentBtn->setConfigs("\u2191", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->editResponBtn->setConfigs("\u270E", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->backUserEditBtn->setConfigs("\u2190", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->okEditUserBtn->setConfigs("OK", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->backUserBtn->setConfigs("\u2190", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->rmvUserBtn->setConfigs("Remover Usuário", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->editUserBtn->setConfigs("Editar Usuário", whiteBk, blueBk, lightBlueBk, "defaultBtn");
    ui->okCallsBtn->setConfigs("\u2190", whiteBk, blueBk, lightBlueBk, "defaultBtn");
}

technician::~technician()
{
    delete ui;
}

void technician::on_myCallsBtn_clicked()
{
    ui->mainInfo->setCurrentIndex(2);
}


void technician::on_goBackBtn_clicked()
{
    ui->mainInfo->setCurrentIndex(0);
}


void technician::on_backBtn_clicked()
{
    ui->mainInfo->setCurrentIndex(1);
}


void technician::on_backUserBtn_clicked()
{
    ui->mainInfo->setCurrentIndex(0);
}


void technician::on_usersBtn_clicked()
{
    ui->mainInfo->setCurrentIndex(3);
}


void technician::on_backUserEditBtn_clicked()
{
    ui->mainInfo->setCurrentIndex(2);
}


void technician::on_okCallsBtn_clicked()
{
    ui->mainInfo->setCurrentIndex(0);
}

