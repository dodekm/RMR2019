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
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
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
    QLineEdit *lineEdit;
    QLCDNumber *lcdNumber;
    QLCDNumber *lcdNumber_2;
    QPushButton *pushButton_clear_map;
    QLabel *label;
    QPushButton *pushButton_find_path;
    QPushButton *pushButton_reset;
    QLCDNumber *lcdNumber_3;
    QPushButton *pushButton_auto;
    QPushButton *pushButton_stop;
    QSpinBox *spinBox;
    QSpinBox *spinBox_2;
    QLabel *label_2;
    QPushButton *pushButton_load_map;
    QPushButton *pushButton_save_map;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton_go;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtGuiApplication1Class)
    {
        if (QtGuiApplication1Class->objectName().isEmpty())
            QtGuiApplication1Class->setObjectName(QString::fromUtf8("QtGuiApplication1Class"));
        QtGuiApplication1Class->resize(739, 406);
        centralWidget = new QWidget(QtGuiApplication1Class);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        startButton = new QPushButton(centralWidget);
        startButton->setObjectName(QString::fromUtf8("startButton"));
        startButton->setGeometry(QRect(110, 20, 75, 23));
        pushButton_forward = new QPushButton(centralWidget);
        pushButton_forward->setObjectName(QString::fromUtf8("pushButton_forward"));
        pushButton_forward->setGeometry(QRect(170, 70, 50, 50));
        pushButton_back = new QPushButton(centralWidget);
        pushButton_back->setObjectName(QString::fromUtf8("pushButton_back"));
        pushButton_back->setGeometry(QRect(170, 170, 50, 50));
        pushButton_right = new QPushButton(centralWidget);
        pushButton_right->setObjectName(QString::fromUtf8("pushButton_right"));
        pushButton_right->setGeometry(QRect(220, 120, 50, 50));
        pushButton_left = new QPushButton(centralWidget);
        pushButton_left->setObjectName(QString::fromUtf8("pushButton_left"));
        pushButton_left->setGeometry(QRect(120, 120, 50, 50));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(20, 20, 81, 21));
        lcdNumber = new QLCDNumber(centralWidget);
        lcdNumber->setObjectName(QString::fromUtf8("lcdNumber"));
        lcdNumber->setGeometry(QRect(20, 80, 64, 23));
        lcdNumber_2 = new QLCDNumber(centralWidget);
        lcdNumber_2->setObjectName(QString::fromUtf8("lcdNumber_2"));
        lcdNumber_2->setGeometry(QRect(20, 110, 64, 23));
        pushButton_clear_map = new QPushButton(centralWidget);
        pushButton_clear_map->setObjectName(QString::fromUtf8("pushButton_clear_map"));
        pushButton_clear_map->setGeometry(QRect(340, 50, 81, 41));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 60, 51, 21));
        pushButton_find_path = new QPushButton(centralWidget);
        pushButton_find_path->setObjectName(QString::fromUtf8("pushButton_find_path"));
        pushButton_find_path->setGeometry(QRect(20, 300, 91, 41));
        pushButton_reset = new QPushButton(centralWidget);
        pushButton_reset->setObjectName(QString::fromUtf8("pushButton_reset"));
        pushButton_reset->setGeometry(QRect(120, 300, 81, 41));
        lcdNumber_3 = new QLCDNumber(centralWidget);
        lcdNumber_3->setObjectName(QString::fromUtf8("lcdNumber_3"));
        lcdNumber_3->setGeometry(QRect(20, 140, 64, 23));
        pushButton_auto = new QPushButton(centralWidget);
        pushButton_auto->setObjectName(QString::fromUtf8("pushButton_auto"));
        pushButton_auto->setGeometry(QRect(210, 300, 81, 41));
        pushButton_stop = new QPushButton(centralWidget);
        pushButton_stop->setObjectName(QString::fromUtf8("pushButton_stop"));
        pushButton_stop->setGeometry(QRect(170, 120, 51, 51));
        spinBox = new QSpinBox(centralWidget);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setGeometry(QRect(20, 270, 42, 22));
        spinBox->setMinimum(-20);
        spinBox->setMaximum(20);
        spinBox_2 = new QSpinBox(centralWidget);
        spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
        spinBox_2->setGeometry(QRect(70, 270, 42, 22));
        spinBox_2->setMinimum(-20);
        spinBox_2->setMaximum(20);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(50, 250, 47, 13));
        pushButton_load_map = new QPushButton(centralWidget);
        pushButton_load_map->setObjectName(QString::fromUtf8("pushButton_load_map"));
        pushButton_load_map->setGeometry(QRect(340, 100, 81, 41));
        pushButton_save_map = new QPushButton(centralWidget);
        pushButton_save_map->setObjectName(QString::fromUtf8("pushButton_save_map"));
        pushButton_save_map->setGeometry(QRect(340, 150, 81, 41));
        lineEdit_2 = new QLineEdit(centralWidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(340, 20, 81, 21));
        pushButton_go = new QPushButton(centralWidget);
        pushButton_go->setObjectName(QString::fromUtf8("pushButton_go"));
        pushButton_go->setGeometry(QRect(20, 210, 41, 31));
        QtGuiApplication1Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtGuiApplication1Class);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 739, 21));
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
        startButton->setText(QApplication::translate("QtGuiApplication1Class", "Connect", nullptr));
        pushButton_forward->setText(QApplication::translate("QtGuiApplication1Class", "/\\", nullptr));
        pushButton_back->setText(QApplication::translate("QtGuiApplication1Class", "V", nullptr));
        pushButton_right->setText(QApplication::translate("QtGuiApplication1Class", "->", nullptr));
        pushButton_left->setText(QApplication::translate("QtGuiApplication1Class", "<-", nullptr));
        lineEdit->setText(QApplication::translate("QtGuiApplication1Class", "192.168.1.11", nullptr));
        pushButton_clear_map->setText(QApplication::translate("QtGuiApplication1Class", "ClearMap", nullptr));
        label->setText(QApplication::translate("QtGuiApplication1Class", "Odometria", nullptr));
        pushButton_find_path->setText(QApplication::translate("QtGuiApplication1Class", "Find", nullptr));
        pushButton_reset->setText(QApplication::translate("QtGuiApplication1Class", "Reset", nullptr));
        pushButton_auto->setText(QApplication::translate("QtGuiApplication1Class", "Auto", nullptr));
        pushButton_stop->setText(QApplication::translate("QtGuiApplication1Class", "stop", nullptr));
        label_2->setText(QApplication::translate("QtGuiApplication1Class", "Target", nullptr));
        pushButton_load_map->setText(QApplication::translate("QtGuiApplication1Class", "LoadMap", nullptr));
        pushButton_save_map->setText(QApplication::translate("QtGuiApplication1Class", "SaveMap", nullptr));
        lineEdit_2->setText(QApplication::translate("QtGuiApplication1Class", "map.txt", nullptr));
        pushButton_go->setText(QApplication::translate("QtGuiApplication1Class", "Go", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtGuiApplication1Class: public Ui_QtGuiApplication1Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTGUIAPPLICATION1_H
