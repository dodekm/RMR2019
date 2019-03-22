/********************************************************************************
** Form generated from reading UI file 'QtGuiApplication1.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTGUIAPPLICATION1_H
#define UI_QTGUIAPPLICATION1_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtGuiApplication1Class
{
public:
    QWidget *centralWidget;
    QPushButton *startButton;
    QPushButton *pushButton_forward;
    QPushButton *pushButton_back;
    QPushButton *pushButton_right;
    QPushButton *pushButton_left;
    QLabel *label;
    QLineEdit *lineEdit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtGuiApplication1Class)
    {
        if (QtGuiApplication1Class->objectName().isEmpty())
            QtGuiApplication1Class->setObjectName(QString::fromUtf8("QtGuiApplication1Class"));
        QtGuiApplication1Class->resize(600, 400);
        centralWidget = new QWidget(QtGuiApplication1Class);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        startButton = new QPushButton(centralWidget);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setGeometry(QRect(250, 30, 75, 23));
        pushButton_forward = new QPushButton(centralWidget);
        pushButton_forward->setObjectName(QString::fromUtf8("pushButton_forward"));
        pushButton_forward->setGeometry(QRect(220, 140, 75, 23));
        pushButton_back = new QPushButton(centralWidget);
        pushButton_back->setObjectName(QString::fromUtf8("pushButton_back"));
        pushButton_back->setGeometry(QRect(220, 210, 75, 23));
        pushButton_right = new QPushButton(centralWidget);
        pushButton_right->setObjectName(QString::fromUtf8("pushButton_right"));
        pushButton_right->setGeometry(QRect(310, 180, 75, 23));
        pushButton_left = new QPushButton(centralWidget);
        pushButton_left->setObjectName(QString::fromUtf8("pushButton_left"));
        pushButton_left->setGeometry(QRect(130, 180, 75, 23));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(250, 90, 47, 13));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(40, 40, 113, 20));
        QtGuiApplication1Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtGuiApplication1Class);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        QtGuiApplication1Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtGuiApplication1Class);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        QtGuiApplication1Class->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QtGuiApplication1Class);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        QtGuiApplication1Class->setStatusBar(statusBar);

        retranslateUi(QtGuiApplication1Class);

        QMetaObject::connectSlotsByName(QtGuiApplication1Class);
    } // setupUi

    void retranslateUi(QMainWindow *QtGuiApplication1Class)
    {
        QtGuiApplication1Class->setWindowTitle(QApplication::translate("QtGuiApplication1Class", "QtGuiApplication1", nullptr));
        startButton->setText(QApplication::translate("QtGuiApplication1Class", "Start", nullptr));
        pushButton_forward->setText(QApplication::translate("QtGuiApplication1Class", "PushButton", nullptr));
        pushButton_back->setText(QApplication::translate("QtGuiApplication1Class", "PushButton", nullptr));
        pushButton_right->setText(QApplication::translate("QtGuiApplication1Class", "PushButton", nullptr));
        pushButton_left->setText(QApplication::translate("QtGuiApplication1Class", "PushButton", nullptr));
        label->setText(QApplication::translate("QtGuiApplication1Class", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtGuiApplication1Class: public Ui_QtGuiApplication1Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTGUIAPPLICATION1_H
