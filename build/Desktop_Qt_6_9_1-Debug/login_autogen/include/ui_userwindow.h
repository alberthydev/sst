/********************************************************************************
** Form generated from reading UI file 'userwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERWINDOW_H
#define UI_USERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserWindow
{
public:
    QFrame *frameChamado;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *lineEdit;
    QGroupBox *groupBoxPrioridade;
    QRadioButton *radioButtonBaixo;
    QRadioButton *radioButtonMedio;
    QRadioButton *radioButtonUrgente;
    QRadioButton *radioButtonAlta;
    QScrollArea *scrollAreaProblema;
    QWidget *scrollAreaWidgetContents;
    QCheckBox *checkBoxSistema;
    QCheckBox *checkBoxImpressora;
    QCheckBox *checkBoxHardware;
    QCheckBox *checkBoxRede;
    QCheckBox *checkBoxOutro;
    QPushButton *pushButton;
    QLineEdit *lineEditOutro;

    void setupUi(QDialog *UserWindow)
    {
        if (UserWindow->objectName().isEmpty())
            UserWindow->setObjectName("UserWindow");
        UserWindow->resize(982, 637);
        UserWindow->setStyleSheet(QString::fromUtf8("background-color: #3465f0;"));
        frameChamado = new QFrame(UserWindow);
        frameChamado->setObjectName("frameChamado");
        frameChamado->setGeometry(QRect(30, 20, 921, 601));
        frameChamado->setMinimumSize(QSize(400, 400));
        frameChamado->setSizeIncrement(QSize(0, 0));
        frameChamado->setStyleSheet(QString::fromUtf8("background-color: #ffffff;\n"
"border-radius: 15px;\n"
"border: 1px solid #cccccc;\n"
"box-shadow: 2px 2px 10px rgba(0, 0, 0, 0.25);\n"
""));
        frameChamado->setFrameShape(QFrame::Shape::StyledPanel);
        frameChamado->setFrameShadow(QFrame::Shadow::Raised);
        label = new QLabel(frameChamado);
        label->setObjectName("label");
        label->setGeometry(QRect(310, 0, 281, 41));
        QFont font;
        font.setFamilies({QString::fromUtf8("Arimo")});
        font.setPointSize(15);
        font.setBold(true);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("color: #333333;\n"
"font-weight: bold;\n"
""));
        label_2 = new QLabel(frameChamado);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(150, 110, 151, 21));
        label_2->setStyleSheet(QString::fromUtf8("color: #333333;\n"
"font-weight: bold;\n"
""));
        label_3 = new QLabel(frameChamado);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(270, 190, 81, 21));
        label_3->setStyleSheet(QString::fromUtf8("color: #333333;\n"
"font-weight: bold;\n"
""));
        label_4 = new QLabel(frameChamado);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(560, 190, 91, 21));
        label_4->setStyleSheet(QString::fromUtf8("color: #333333;\n"
"font-weight: bold;\n"
""));
        lineEdit = new QLineEdit(frameChamado);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(320, 110, 351, 26));
        lineEdit->setStyleSheet(QString::fromUtf8("QLineEdit {\n"
"    border: 1px solid #ccc;\n"
"    border-radius: 10px;\n"
"    padding: 5px;\n"
"    background-color: #ffffff;\n"
"    color: #222222;\n"
"}\n"
""));
        groupBoxPrioridade = new QGroupBox(frameChamado);
        groupBoxPrioridade->setObjectName("groupBoxPrioridade");
        groupBoxPrioridade->setEnabled(true);
        groupBoxPrioridade->setGeometry(QRect(530, 220, 141, 151));
        groupBoxPrioridade->setStyleSheet(QString::fromUtf8("background-color: #ffffff;\n"
"border-radius: 15px;\n"
"border: 1px solid #cccccc;\n"
"box-shadow: 2px 2px 10px rgba(0, 0, 0, 0.25);\n"
""));
        radioButtonBaixo = new QRadioButton(groupBoxPrioridade);
        radioButtonBaixo->setObjectName("radioButtonBaixo");
        radioButtonBaixo->setGeometry(QRect(10, 20, 102, 24));
        radioButtonBaixo->setStyleSheet(QString::fromUtf8("color: #333333;\n"
"font-weight: bold;\n"
"\n"
""));
        radioButtonMedio = new QRadioButton(groupBoxPrioridade);
        radioButtonMedio->setObjectName("radioButtonMedio");
        radioButtonMedio->setGeometry(QRect(10, 50, 102, 24));
        radioButtonMedio->setStyleSheet(QString::fromUtf8("color: #333333;\n"
"font-weight: bold;"));
        radioButtonUrgente = new QRadioButton(groupBoxPrioridade);
        radioButtonUrgente->setObjectName("radioButtonUrgente");
        radioButtonUrgente->setGeometry(QRect(10, 110, 102, 24));
        radioButtonUrgente->setStyleSheet(QString::fromUtf8("color: #333333;\n"
"font-weight: bold;\n"
""));
        radioButtonAlta = new QRadioButton(groupBoxPrioridade);
        radioButtonAlta->setObjectName("radioButtonAlta");
        radioButtonAlta->setGeometry(QRect(10, 80, 102, 24));
        radioButtonAlta->setStyleSheet(QString::fromUtf8("color: #333333;\n"
"font-weight: bold;"));
        scrollAreaProblema = new QScrollArea(frameChamado);
        scrollAreaProblema->setObjectName("scrollAreaProblema");
        scrollAreaProblema->setGeometry(QRect(230, 220, 211, 151));
        scrollAreaProblema->setStyleSheet(QString::fromUtf8("background-color: #ffffff;\n"
"border-radius: 15px;\n"
"border: 1px solid #cccccc;\n"
"box-shadow: 2px 2px 10px rgba(0, 0, 0, 0.25);\n"
""));
        scrollAreaProblema->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 209, 149));
        checkBoxSistema = new QCheckBox(scrollAreaWidgetContents);
        checkBoxSistema->setObjectName("checkBoxSistema");
        checkBoxSistema->setGeometry(QRect(10, 0, 191, 24));
        checkBoxSistema->setStyleSheet(QString::fromUtf8("color: #333333;\n"
"font-weight: bold;\n"
""));
        checkBoxImpressora = new QCheckBox(scrollAreaWidgetContents);
        checkBoxImpressora->setObjectName("checkBoxImpressora");
        checkBoxImpressora->setGeometry(QRect(10, 30, 191, 24));
        checkBoxImpressora->setStyleSheet(QString::fromUtf8("color: #333333;\n"
"font-weight: bold;\n"
""));
        checkBoxHardware = new QCheckBox(scrollAreaWidgetContents);
        checkBoxHardware->setObjectName("checkBoxHardware");
        checkBoxHardware->setGeometry(QRect(10, 60, 191, 24));
        checkBoxHardware->setStyleSheet(QString::fromUtf8("color: #333333;\n"
"font-weight: bold;\n"
""));
        checkBoxRede = new QCheckBox(scrollAreaWidgetContents);
        checkBoxRede->setObjectName("checkBoxRede");
        checkBoxRede->setGeometry(QRect(10, 90, 191, 24));
        checkBoxRede->setStyleSheet(QString::fromUtf8("color: #333333;\n"
"font-weight: bold;\n"
""));
        checkBoxOutro = new QCheckBox(scrollAreaWidgetContents);
        checkBoxOutro->setObjectName("checkBoxOutro");
        checkBoxOutro->setGeometry(QRect(10, 120, 191, 24));
        checkBoxOutro->setStyleSheet(QString::fromUtf8("color: #333333;\n"
"font-weight: bold;\n"
""));
        scrollAreaProblema->setWidget(scrollAreaWidgetContents);
        pushButton = new QPushButton(frameChamado);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(410, 480, 121, 31));
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"  background-color: #1976D2;\n"
"  color: white;\n"
"  border-radius: 10px;\n"
"  font-weight: bold;\n"
"}\n"
"QPushButton:hover {\n"
"  background-color: #1565C0;\n"
"}\n"
""));
        lineEditOutro = new QLineEdit(frameChamado);
        lineEditOutro->setObjectName("lineEditOutro");
        lineEditOutro->setEnabled(false);
        lineEditOutro->setGeometry(QRect(230, 380, 211, 51));

        retranslateUi(UserWindow);

        QMetaObject::connectSlotsByName(UserWindow);
    } // setupUi

    void retranslateUi(QDialog *UserWindow)
    {
        UserWindow->setWindowTitle(QCoreApplication::translate("UserWindow", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("UserWindow", "ABERTURA DE CHAMADOS", nullptr));
        label_2->setText(QCoreApplication::translate("UserWindow", "CLIENTE SOLICITANTE", nullptr));
        label_3->setText(QCoreApplication::translate("UserWindow", "PROBLEMA", nullptr));
        label_4->setText(QCoreApplication::translate("UserWindow", "PRIORIDADE", nullptr));
        groupBoxPrioridade->setTitle(QString());
        radioButtonBaixo->setText(QCoreApplication::translate("UserWindow", "Baixo", nullptr));
        radioButtonMedio->setText(QCoreApplication::translate("UserWindow", "Medio", nullptr));
        radioButtonUrgente->setText(QCoreApplication::translate("UserWindow", "Urgente", nullptr));
        radioButtonAlta->setText(QCoreApplication::translate("UserWindow", "Alta", nullptr));
        checkBoxSistema->setText(QCoreApplication::translate("UserWindow", "Sistema", nullptr));
        checkBoxImpressora->setText(QCoreApplication::translate("UserWindow", "Impressora", nullptr));
        checkBoxHardware->setText(QCoreApplication::translate("UserWindow", "Hardware", nullptr));
        checkBoxRede->setText(QCoreApplication::translate("UserWindow", "Rede", nullptr));
        checkBoxOutro->setText(QCoreApplication::translate("UserWindow", "Outro", nullptr));
        pushButton->setText(QCoreApplication::translate("UserWindow", "ENVIAR", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserWindow: public Ui_UserWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERWINDOW_H
