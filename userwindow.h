#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QDialog>

namespace Ui {
class UserWindow;
}

class UserWindow : public QDialog
{
    Q_OBJECT

public:
    explicit UserWindow(QWidget *parent = nullptr);
    ~UserWindow();

private slots:
    void on_label_5_linkActivated(const QString &link);

    void on_pushButton_clicked();

private:
    Ui::UserWindow *ui;
};

#endif // USERWINDOW_H
