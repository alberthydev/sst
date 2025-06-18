#include "technician.h"
#include "ui_technician.h"

technician::technician(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::technician)
{
    ui->setupUi(this);

    ui->newCalls->setColors(QColor("#ffa2b3"), QColor("#ff8aa0"));
    ui->newCalls->setText("Teste\n Teste");
    ui->newCalls->setTextColor("#ff2d55");
    ui->newCalls->setStyleSheet
        (R"(
            color: black;
            font-size: 30pt;
            font-weight: bold;
        )");
    ui->analysisCalls->setColors(QColor("#faffac"), QColor("#f7ff7e"));
    ui->completedCalls->setColors(QColor("#d0ffa3"), QColor("#b5ff6e"));
}

technician::~technician()
{
    delete ui;
}
