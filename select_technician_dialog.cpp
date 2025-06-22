#include "select_technician_dialog.h"
#include "ui_select_technician_dialog.h"
#include <QDebug>

select_technician_dialog::select_technician_dialog(facade_call_system *facade, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::select_technician_dialog),
    m_facade(facade)
{
    ui->setupUi(this);
    setWindowTitle("Atribuir Responsável");

    connect(m_facade, &facade_call_system::technicianListReady,
            this, &select_technician_dialog::onTechnicianListReady);

    m_facade->requestTechnicianList();
}

select_technician_dialog::~select_technician_dialog()
{
    delete ui;
}

void select_technician_dialog::onTechnicianListReady(const QMap<int, QString> &technicians)
{
    ui->techniciansComboBox->clear();

    ui->techniciansComboBox->addItem("Nenhum", 0);

    for (auto it = technicians.constBegin(); it != technicians.constEnd(); ++it) {
        ui->techniciansComboBox->addItem(it.value(), it.key());
    }
}

void select_technician_dialog::accept()
{
    int selectedId = ui->techniciansComboBox->currentData().toInt();

    if (selectedId >= 0) {
        qDebug() << "Técnico ID" << selectedId << "selecionado no diálogo.";
        emit technicianSelected(selectedId);
        QDialog::accept();
    } else {
        qWarning() << "Nenhum técnico selecionado.";
    }
}
