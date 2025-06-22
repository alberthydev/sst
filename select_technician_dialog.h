#ifndef SELECT_TECHNICIAN_DIALOG_H
#define SELECT_TECHNICIAN_DIALOG_H

#include <QDialog>
#include <QMap>
#include "facade_call_system.h"

// O namespace Ui usará o nome da classe que você deu no Designer
namespace Ui {
class select_technician_dialog;
}

// O nome da classe foi alterado para snake_case
class select_technician_dialog : public QDialog
{
    Q_OBJECT

public:
    // Construtor com o novo nome de classe
    explicit select_technician_dialog(facade_call_system *facade, QWidget *parent = nullptr);
    ~select_technician_dialog();

signals:
    void technicianSelected(int technicianId);

private slots:
    void onTechnicianListReady(const QMap<int, QString> &technicians);
    void accept() override;

private:
    // Ponteiro para a UI com o novo nome
    Ui::select_technician_dialog *ui;
    facade_call_system *m_facade;
};

#endif // SELECT_TECHNICIAN_DIALOG_H
