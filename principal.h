#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <QMainWindow>
#include <QMessageBox>
#include "facade_call_system.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Principal;
}
QT_END_NAMESPACE

class Principal : public QMainWindow
{
    Q_OBJECT

public:
    Principal(QWidget *parent = nullptr);
    ~Principal();

private slots:
    void on_loginBtn_clicked();
    void on_showPassBtn_clicked();
    void onLoginSuccess(int userId, const QString &name, const QString &userType);
    void onLoginFailed(const QString &errorMessage);

private:
    Ui::Principal *ui;
    facade_call_system *m_facade;
};
#endif // PRINCIPAL_H
