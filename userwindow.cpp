#include "userwindow.h"
#include "ui_userwindow.h"

UserWindow::UserWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UserWindow)
{
    ui->setupUi(this);

    connect(ui->checkBoxOutro, &QCheckBox::toggled, this, [=](bool checked){
        ui->lineEditOutro->setEnabled(checked);
    });
}

UserWindow::~UserWindow()
{
    delete ui;
}

void UserWindow::on_label_5_linkActivated(const QString &link)
{

}


void UserWindow::on_pushButton_clicked()
{
    this->close();
    parentWidget()->show();  // isso reexibe a MainWindow escondida

}

