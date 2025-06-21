#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString usuario=ui->lineEdit->text();
    QString senha=ui->lineEdit_2->text();
}


void MainWindow::on_pushButton_2_clicked()
{
    UserWindow *userWin = new UserWindow(this);  // cria a janela
    userWin->show();                   // exibe a nova janela
    this->hide();
}

