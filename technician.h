#ifndef TECHNICIAN_H
#define TECHNICIAN_H

#include <QDialog>

namespace Ui {
class technician;
}

class technician : public QDialog
{
    Q_OBJECT

public:
    explicit technician(QWidget *parent = nullptr);
    ~technician();

private slots:
    void on_myCallsBtn_clicked();

    void on_goBackBtn_clicked();

    void on_backBtn_clicked();

    void on_backUserBtn_clicked();

    void on_usersBtn_clicked();

    void on_backUserEditBtn_clicked();

    void on_okCallsBtn_clicked();

private:
    Ui::technician *ui;
};

#endif // TECHNICIAN_H
