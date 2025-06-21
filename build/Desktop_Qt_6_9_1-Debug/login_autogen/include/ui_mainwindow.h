/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QFrame *frame;
    QPushButton *pushButton_2;
    QLabel *label_3;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton;
    QLineEdit *lineEdit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(825, 623);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: #a8c8ff;\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget->setStyleSheet(QString::fromUtf8("background-color: #3465f0 ;\n"
"    "));
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(180, 70, 461, 421));
        frame->setStyleSheet(QString::fromUtf8("background-color: #ffffff;\n"
"border-radius: 15px;\n"
"border: 1px solid #cccccc;\n"
"box-shadow: 2px 2px 10px rgba(0, 0, 0, 0.25);User"));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        pushButton_2 = new QPushButton(frame);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(170, 370, 141, 16));
        pushButton_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background: transparent;\n"
"    border: none;\n"
"    color: #5555ff;\n"
"    text-decoration: underline;\n"
"}\n"
"QPushButton:hover {\n"
"    color: #0000aa;\n"
"}\n"
""));
        label_3 = new QLabel(frame);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(80, 205, 51, 20));
        label_3->setStyleSheet(QString::fromUtf8("color: #333333;\n"
"font-weight: bold;\n"
""));
        label = new QLabel(frame);
        label->setObjectName("label");
        label->setGeometry(QRect(180, 10, 100, 20));
        label->setStyleSheet(QString::fromUtf8("\n"
"      font-size: 24px;\n"
"      font-weight: bold;\n"
"      color: #333;\n"
"     "));
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_2 = new QLabel(frame);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(70, 155, 58, 20));
        label_2->setStyleSheet(QString::fromUtf8("color: #333333;\n"
"font-weight: bold;\n"
""));
        lineEdit_2 = new QLineEdit(frame);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(150, 200, 200, 30));
        lineEdit_2->setStyleSheet(QString::fromUtf8("\n"
"      QLineEdit {\n"
"		border: 1px solid #ccc;\n"
"		border-radius: 10px;\n"
"		padding: 5px;\n"
"		background-color: #ffffff;\n"
"		color: #222222;\n"
"      }\n"
"     "));
        lineEdit_2->setEchoMode(QLineEdit::EchoMode::Password);
        pushButton = new QPushButton(frame);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(170, 330, 140, 32));
        pushButton->setStyleSheet(QString::fromUtf8("\n"
"      QPushButton {\n"
"        background-color: #1976D2;\n"
"        color: white;\n"
"        border-radius: 10px;\n"
"        font-weight: bold;\n"
"      }\n"
"      QPushButton:hover {\n"
"        background-color: #1565C0;\n"
"      }\n"
"     "));
        lineEdit = new QLineEdit(frame);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(150, 150, 200, 30));
        lineEdit->setStyleSheet(QString::fromUtf8("\n"
"      QLineEdit {\n"
"        border: 1px solid #ccc;\n"
"    	 border-radius: 10px;\n"
"   		 padding: 5px;\n"
"    	 background-color: #ffffff;\n"
"    	 color: #222222;\n"
"      }\n"
"     "));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 825, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Tela de Login", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Entrar como usu\303\241rio", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Senha", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "LOGIN", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Usu\303\241rio", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "ENTRAR", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
