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

private:
    Ui::technician *ui;
};

#endif // TECHNICIAN_H
