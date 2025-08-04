/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_12;
    QComboBox *Time_com;
    QPushButton *ZDSC;
    QPushButton *YX;
    QPushButton *JZ;
    QPushButton *HL;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_18;
    QComboBox *CPXZ;
    QSpacerItem *horizontalSpacer_3;
    QCheckBox *CheckBox_SaveSerialData_WithTime;
    QPushButton *pushButton;
    QPushButton *KSBC;
    QSpacerItem *horizontalSpacer_2;
    QLabel *Label_SaveSerialData_StateMsg;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_10;
    QLineEdit *W;
    QLabel *label_16;
    QLineEdit *ZT;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_12;
    QLineEdit *V;
    QLabel *label_11;
    QLineEdit *I;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *baud;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *com;
    QPushButton *open_port;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QComboBox *bit;
    QPushButton *close_port;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QComboBox *stopbit;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QComboBox *jiaoyan;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_19;
    QLineEdit *lineEdit;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_7;
    QLineEdit *X;
    QLabel *label_13;
    QLabel *label_8;
    QLineEdit *Y;
    QLabel *label_14;
    QLabel *label_9;
    QLineEdit *Z;
    QLabel *label_15;
    QLabel *label_6;
    QComboBox *SD;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_17;
    QLabel *State_Anylise;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QString::fromUtf8("Widget"));
        Widget->resize(701, 323);
        Widget->setMinimumSize(QSize(701, 300));
        Widget->setMaximumSize(QSize(701, 323));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        Widget->setFont(font);
        gridLayout_2 = new QGridLayout(Widget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        Time_com = new QComboBox(Widget);
        Time_com->addItem(QString());
        Time_com->addItem(QString());
        Time_com->addItem(QString());
        Time_com->addItem(QString());
        Time_com->addItem(QString());
        Time_com->addItem(QString());
        Time_com->addItem(QString());
        Time_com->addItem(QString());
        Time_com->addItem(QString());
        Time_com->addItem(QString());
        Time_com->setObjectName(QString::fromUtf8("Time_com"));

        horizontalLayout_12->addWidget(Time_com);

        ZDSC = new QPushButton(Widget);
        ZDSC->setObjectName(QString::fromUtf8("ZDSC"));
        ZDSC->setMinimumSize(QSize(100, 30));
        ZDSC->setMaximumSize(QSize(100, 30));

        horizontalLayout_12->addWidget(ZDSC);

        YX = new QPushButton(Widget);
        YX->setObjectName(QString::fromUtf8("YX"));
        YX->setMinimumSize(QSize(100, 30));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(11);
        YX->setFont(font1);

        horizontalLayout_12->addWidget(YX);

        JZ = new QPushButton(Widget);
        JZ->setObjectName(QString::fromUtf8("JZ"));
        JZ->setMinimumSize(QSize(100, 30));
        JZ->setFont(font1);

        horizontalLayout_12->addWidget(JZ);

        HL = new QPushButton(Widget);
        HL->setObjectName(QString::fromUtf8("HL"));
        HL->setMinimumSize(QSize(100, 30));
        HL->setFont(font1);

        horizontalLayout_12->addWidget(HL);


        gridLayout_2->addLayout(horizontalLayout_12, 4, 2, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_8->setSizeConstraint(QLayout::SetMaximumSize);
        horizontalLayout_8->setContentsMargins(-1, -1, -1, 5);
        label_18 = new QLabel(Widget);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setMinimumSize(QSize(80, 0));
        label_18->setMaximumSize(QSize(80, 30));

        horizontalLayout_8->addWidget(label_18);

        CPXZ = new QComboBox(Widget);
        CPXZ->addItem(QString());
        CPXZ->addItem(QString());
        CPXZ->addItem(QString());
        CPXZ->addItem(QString());
        CPXZ->addItem(QString());
        CPXZ->addItem(QString());
        CPXZ->setObjectName(QString::fromUtf8("CPXZ"));
        CPXZ->setMinimumSize(QSize(150, 30));
        CPXZ->setMaximumSize(QSize(150, 30));
        QFont font2;
        font2.setPointSize(10);
        CPXZ->setFont(font2);

        horizontalLayout_8->addWidget(CPXZ);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_3);

        CheckBox_SaveSerialData_WithTime = new QCheckBox(Widget);
        CheckBox_SaveSerialData_WithTime->setObjectName(QString::fromUtf8("CheckBox_SaveSerialData_WithTime"));

        horizontalLayout_8->addWidget(CheckBox_SaveSerialData_WithTime);

        pushButton = new QPushButton(Widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(80, 30));
        pushButton->setMaximumSize(QSize(80, 30));

        horizontalLayout_8->addWidget(pushButton);

        KSBC = new QPushButton(Widget);
        KSBC->setObjectName(QString::fromUtf8("KSBC"));
        KSBC->setMinimumSize(QSize(80, 30));
        KSBC->setMaximumSize(QSize(80, 30));

        horizontalLayout_8->addWidget(KSBC);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_8);

        Label_SaveSerialData_StateMsg = new QLabel(Widget);
        Label_SaveSerialData_StateMsg->setObjectName(QString::fromUtf8("Label_SaveSerialData_StateMsg"));
        Label_SaveSerialData_StateMsg->setMinimumSize(QSize(0, 30));
        Label_SaveSerialData_StateMsg->setMaximumSize(QSize(16777215, 30));

        verticalLayout->addWidget(Label_SaveSerialData_StateMsg);


        gridLayout_2->addLayout(verticalLayout, 5, 2, 2, 1);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        horizontalLayout_9->setContentsMargins(-1, -1, -1, 0);
        label_10 = new QLabel(Widget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setMinimumSize(QSize(100, 30));
        label_10->setMaximumSize(QSize(100, 30));
        label_10->setFont(font);
        label_10->setStyleSheet(QString::fromUtf8("border-width: 1px;\n"
"border-style:solid;\n"
"border-color:rgb(222,222,222);"));

        horizontalLayout_9->addWidget(label_10);

        W = new QLineEdit(Widget);
        W->setObjectName(QString::fromUtf8("W"));
        W->setMinimumSize(QSize(100, 30));
        W->setMaximumSize(QSize(100, 30));
        W->setFont(font);
        W->setReadOnly(true);

        horizontalLayout_9->addWidget(W);

        label_16 = new QLabel(Widget);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setMinimumSize(QSize(100, 30));
        label_16->setMaximumSize(QSize(100, 30));
        label_16->setFont(font);
        label_16->setStyleSheet(QString::fromUtf8("border-width: 1px;\n"
"border-style:solid;\n"
"border-color:rgb(222,222,222);"));

        horizontalLayout_9->addWidget(label_16);

        ZT = new QLineEdit(Widget);
        ZT->setObjectName(QString::fromUtf8("ZT"));
        ZT->setMinimumSize(QSize(100, 30));
        ZT->setMaximumSize(QSize(100, 30));
        ZT->setFont(font);
        ZT->setReadOnly(true);

        horizontalLayout_9->addWidget(ZT);


        gridLayout_2->addLayout(horizontalLayout_9, 1, 2, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(-1, 0, -1, 0);
        label_12 = new QLabel(Widget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setMinimumSize(QSize(100, 30));
        label_12->setMaximumSize(QSize(100, 30));
        label_12->setFont(font);
        label_12->setStyleSheet(QString::fromUtf8("border-width: 1px;\n"
"border-style:solid;\n"
"border-color:rgb(222,222,222);"));

        horizontalLayout_6->addWidget(label_12);

        V = new QLineEdit(Widget);
        V->setObjectName(QString::fromUtf8("V"));
        V->setMinimumSize(QSize(100, 30));
        V->setMaximumSize(QSize(100, 30));
        V->setFont(font);
        V->setMouseTracking(true);
        V->setReadOnly(true);

        horizontalLayout_6->addWidget(V);

        label_11 = new QLabel(Widget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setMinimumSize(QSize(100, 30));
        label_11->setMaximumSize(QSize(100, 30));
        label_11->setFont(font);
        label_11->setStyleSheet(QString::fromUtf8("border-width: 1px;\n"
"border-style:solid;\n"
"border-color:rgb(222,222,222);"));

        horizontalLayout_6->addWidget(label_11);

        I = new QLineEdit(Widget);
        I->setObjectName(QString::fromUtf8("I"));
        I->setMinimumSize(QSize(100, 30));
        I->setMaximumSize(QSize(100, 30));
        I->setFont(font);
        I->setMouseTracking(true);
        I->setReadOnly(true);

        horizontalLayout_6->addWidget(I);


        gridLayout_2->addLayout(horizontalLayout_6, 0, 2, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(-1, -1, 5, -1);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(Widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);
        label_2->setStyleSheet(QString::fromUtf8("border-width: 1px;\n"
"border-style:solid;\n"
"border-color:rgb(222,222,222);"));

        horizontalLayout_2->addWidget(label_2);

        baud = new QComboBox(Widget);
        baud->addItem(QString());
        baud->addItem(QString());
        baud->setObjectName(QString::fromUtf8("baud"));
        baud->setFont(font);

        horizontalLayout_2->addWidget(baud);


        gridLayout->addLayout(horizontalLayout_2, 3, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(Widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("border-width: 1px;\n"
"border-style:solid;\n"
"border-color:rgb(222,222,222);"));

        horizontalLayout->addWidget(label);

        com = new QComboBox(Widget);
        com->setObjectName(QString::fromUtf8("com"));
        com->setFont(font);

        horizontalLayout->addWidget(com);


        gridLayout->addLayout(horizontalLayout, 1, 0, 1, 1);

        open_port = new QPushButton(Widget);
        open_port->setObjectName(QString::fromUtf8("open_port"));
        open_port->setMinimumSize(QSize(0, 30));
        open_port->setFont(font1);

        gridLayout->addWidget(open_port, 9, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(Widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);
        label_3->setStyleSheet(QString::fromUtf8("border-width: 1px;\n"
"border-style:solid;\n"
"border-color:rgb(222,222,222);"));

        horizontalLayout_3->addWidget(label_3);

        bit = new QComboBox(Widget);
        bit->addItem(QString());
        bit->addItem(QString());
        bit->addItem(QString());
        bit->addItem(QString());
        bit->setObjectName(QString::fromUtf8("bit"));
        bit->setFont(font);

        horizontalLayout_3->addWidget(bit);


        gridLayout->addLayout(horizontalLayout_3, 4, 0, 1, 1);

        close_port = new QPushButton(Widget);
        close_port->setObjectName(QString::fromUtf8("close_port"));
        close_port->setMinimumSize(QSize(0, 30));
        close_port->setFont(font1);

        gridLayout->addWidget(close_port, 8, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_5 = new QLabel(Widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font);
        label_5->setStyleSheet(QString::fromUtf8("border-width: 1px;\n"
"border-style:solid;\n"
"border-color:rgb(222,222,222);"));

        horizontalLayout_5->addWidget(label_5);

        stopbit = new QComboBox(Widget);
        stopbit->addItem(QString());
        stopbit->setObjectName(QString::fromUtf8("stopbit"));
        stopbit->setFont(font);

        horizontalLayout_5->addWidget(stopbit);


        gridLayout->addLayout(horizontalLayout_5, 6, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(Widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font);
        label_4->setStyleSheet(QString::fromUtf8("border-width: 1px;\n"
"border-style:solid;\n"
"border-color:rgb(222,222,222);"));

        horizontalLayout_4->addWidget(label_4);

        jiaoyan = new QComboBox(Widget);
        jiaoyan->addItem(QString());
        jiaoyan->setObjectName(QString::fromUtf8("jiaoyan"));
        jiaoyan->setFont(font);

        horizontalLayout_4->addWidget(jiaoyan);


        gridLayout->addLayout(horizontalLayout_4, 5, 0, 1, 1);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_19 = new QLabel(Widget);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        horizontalLayout_11->addWidget(label_19);

        lineEdit = new QLineEdit(Widget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMinimumSize(QSize(50, 30));
        lineEdit->setMaximumSize(QSize(50, 30));

        horizontalLayout_11->addWidget(lineEdit);


        gridLayout->addLayout(horizontalLayout_11, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 7, 2);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        horizontalLayout_7->setContentsMargins(-1, 0, -1, 0);
        label_7 = new QLabel(Widget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMinimumSize(QSize(50, 30));
        label_7->setMaximumSize(QSize(60, 30));
        label_7->setFont(font);
        label_7->setStyleSheet(QString::fromUtf8("border-width: 1px;\n"
"border-style:solid;\n"
"border-color:rgb(222,222,222);"));

        horizontalLayout_7->addWidget(label_7);

        X = new QLineEdit(Widget);
        X->setObjectName(QString::fromUtf8("X"));
        X->setMinimumSize(QSize(50, 30));
        X->setMaximumSize(QSize(50, 30));
        X->setFont(font);

        horizontalLayout_7->addWidget(X);

        label_13 = new QLabel(Widget);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setMinimumSize(QSize(22, 0));
        label_13->setFont(font);

        horizontalLayout_7->addWidget(label_13);

        label_8 = new QLabel(Widget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(50, 30));
        label_8->setMaximumSize(QSize(60, 30));
        label_8->setFont(font);
        label_8->setStyleSheet(QString::fromUtf8("border-width: 1px;\n"
"border-style:solid;\n"
"border-color:rgb(222,222,222);"));

        horizontalLayout_7->addWidget(label_8);

        Y = new QLineEdit(Widget);
        Y->setObjectName(QString::fromUtf8("Y"));
        Y->setMinimumSize(QSize(50, 30));
        Y->setMaximumSize(QSize(50, 30));
        Y->setFont(font);

        horizontalLayout_7->addWidget(Y);

        label_14 = new QLabel(Widget);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setMinimumSize(QSize(22, 0));
        label_14->setFont(font);

        horizontalLayout_7->addWidget(label_14);

        label_9 = new QLabel(Widget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setMinimumSize(QSize(50, 30));
        label_9->setMaximumSize(QSize(60, 30));
        label_9->setFont(font);
        label_9->setStyleSheet(QString::fromUtf8("border-width: 1px;\n"
"border-style:solid;\n"
"border-color:rgb(222,222,222);"));

        horizontalLayout_7->addWidget(label_9);

        Z = new QLineEdit(Widget);
        Z->setObjectName(QString::fromUtf8("Z"));
        Z->setMinimumSize(QSize(50, 30));
        Z->setMaximumSize(QSize(50, 30));
        Z->setFont(font);

        horizontalLayout_7->addWidget(Z);

        label_15 = new QLabel(Widget);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setMinimumSize(QSize(22, 0));
        label_15->setFont(font);

        horizontalLayout_7->addWidget(label_15);

        label_6 = new QLabel(Widget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setMinimumSize(QSize(30, 30));
        label_6->setMaximumSize(QSize(25, 30));
        label_6->setStyleSheet(QString::fromUtf8("border-width: 1px;\n"
"border-style:solid;\n"
"border-color:rgb(222,222,222);"));

        horizontalLayout_7->addWidget(label_6);

        SD = new QComboBox(Widget);
        SD->addItem(QString());
        SD->addItem(QString());
        SD->addItem(QString());
        SD->setObjectName(QString::fromUtf8("SD"));
        SD->setMinimumSize(QSize(55, 30));
        SD->setFont(font1);

        horizontalLayout_7->addWidget(SD);


        gridLayout_2->addLayout(horizontalLayout_7, 3, 2, 1, 1);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        label_17 = new QLabel(Widget);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setMinimumSize(QSize(100, 30));
        label_17->setMaximumSize(QSize(100, 30));
        label_17->setFont(font);
        label_17->setStyleSheet(QString::fromUtf8("border-width: 1px;\n"
"border-style:solid;\n"
"border-color:rgb(222,222,222);"));

        horizontalLayout_10->addWidget(label_17);

        State_Anylise = new QLabel(Widget);
        State_Anylise->setObjectName(QString::fromUtf8("State_Anylise"));
        State_Anylise->setMinimumSize(QSize(350, 30));
        State_Anylise->setMaximumSize(QSize(350, 30));
        State_Anylise->setFont(font);
        State_Anylise->setStyleSheet(QString::fromUtf8("border-width: 1px;\n"
"border-style:solid;\n"
"border-color:rgb(222,222,222);"));

        horizontalLayout_10->addWidget(State_Anylise);


        gridLayout_2->addLayout(horizontalLayout_10, 2, 2, 1, 1);


        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        Time_com->setItemText(0, QCoreApplication::translate("Widget", "\351\227\264\351\232\224\357\274\2321min", nullptr));
        Time_com->setItemText(1, QCoreApplication::translate("Widget", "\351\227\264\351\232\224\357\274\2322min", nullptr));
        Time_com->setItemText(2, QCoreApplication::translate("Widget", "\351\227\264\351\232\224\357\274\2324min", nullptr));
        Time_com->setItemText(3, QCoreApplication::translate("Widget", "\351\227\264\351\232\224\357\274\2326min", nullptr));
        Time_com->setItemText(4, QCoreApplication::translate("Widget", "\351\227\264\351\232\224\357\274\2328min", nullptr));
        Time_com->setItemText(5, QCoreApplication::translate("Widget", "\351\227\264\351\232\224\357\274\23210min", nullptr));
        Time_com->setItemText(6, QCoreApplication::translate("Widget", "\351\227\264\351\232\224\357\274\23220min", nullptr));
        Time_com->setItemText(7, QCoreApplication::translate("Widget", "\351\227\264\351\232\224\357\274\23230min", nullptr));
        Time_com->setItemText(8, QCoreApplication::translate("Widget", "\351\227\264\351\232\224\357\274\2321h", nullptr));
        Time_com->setItemText(9, QCoreApplication::translate("Widget", "\351\227\264\351\232\224\357\274\2322h", nullptr));

        ZDSC->setText(QCoreApplication::translate("Widget", "\345\274\200\345\220\257\350\207\252\345\212\250\350\277\220\350\241\214", nullptr));
        YX->setText(QCoreApplication::translate("Widget", "\345\215\225\346\255\245\350\277\220\350\241\214", nullptr));
        JZ->setText(QCoreApplication::translate("Widget", "\345\210\235\345\247\213\346\240\241\345\207\206", nullptr));
        HL->setText(QCoreApplication::translate("Widget", "\345\233\236\351\233\266", nullptr));
        label_18->setText(QCoreApplication::translate("Widget", "  \344\272\247\345\223\201\351\200\211\346\213\251\357\274\232", nullptr));
        CPXZ->setItemText(0, QCoreApplication::translate("Widget", "300W\350\207\252\347\204\266\346\225\243\347\203\255TX", nullptr));
        CPXZ->setItemText(1, QCoreApplication::translate("Widget", "300W\350\207\252\347\204\266\346\225\243\347\203\255RX", nullptr));
        CPXZ->setItemText(2, QCoreApplication::translate("Widget", "300W\351\243\216\345\206\267\346\225\243\347\203\255TX", nullptr));
        CPXZ->setItemText(3, QCoreApplication::translate("Widget", "300W\351\243\216\345\206\267\346\225\243\347\203\255RX", nullptr));
        CPXZ->setItemText(4, QCoreApplication::translate("Widget", "1200W_TX", nullptr));
        CPXZ->setItemText(5, QCoreApplication::translate("Widget", "1200W_RX", nullptr));

        CheckBox_SaveSerialData_WithTime->setText(QCoreApplication::translate("Widget", "\345\242\236\345\212\240\346\227\266\351\227\264\346\210\263", nullptr));
        pushButton->setText(QCoreApplication::translate("Widget", "\351\200\211\346\213\251\350\267\257\345\276\204", nullptr));
        KSBC->setText(QCoreApplication::translate("Widget", "\345\274\200\345\247\213\344\277\235\345\255\230", nullptr));
        Label_SaveSerialData_StateMsg->setText(QCoreApplication::translate("Widget", "  \347\202\271\345\207\273\351\200\211\346\213\251\350\267\257\345\276\204\357\274\214\351\200\211\346\213\251\344\277\235\345\255\230\350\267\257\345\276\204\357\274\214\346\226\207\344\273\266\344\277\235\345\255\230\344\270\272txt\346\226\207\344\273\266\343\200\202", nullptr));
        label_10->setText(QCoreApplication::translate("Widget", "\345\212\237\347\216\207(W)         \357\274\232", nullptr));
        label_16->setText(QCoreApplication::translate("Widget", "\347\212\266\346\200\201               \357\274\232", nullptr));
        label_12->setText(QCoreApplication::translate("Widget", "\350\276\223\345\207\272\347\224\265\345\216\213(mV) \357\274\232", nullptr));
        V->setInputMask(QString());
        label_11->setText(QCoreApplication::translate("Widget", "\350\276\223\345\207\272\347\224\265\346\265\201(mA) \357\274\232", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "\346\263\242\347\211\271\347\216\207\357\274\232", nullptr));
        baud->setItemText(0, QCoreApplication::translate("Widget", "115200", nullptr));
        baud->setItemText(1, QCoreApplication::translate("Widget", "9600", nullptr));

        label->setText(QCoreApplication::translate("Widget", "\347\253\257\345\217\243\345\217\267\357\274\232", nullptr));
        open_port->setText(QCoreApplication::translate("Widget", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        label_3->setText(QCoreApplication::translate("Widget", "\346\225\260\346\215\256\344\275\215\357\274\232", nullptr));
        bit->setItemText(0, QCoreApplication::translate("Widget", "8", nullptr));
        bit->setItemText(1, QCoreApplication::translate("Widget", "5", nullptr));
        bit->setItemText(2, QCoreApplication::translate("Widget", "6", nullptr));
        bit->setItemText(3, QCoreApplication::translate("Widget", "7", nullptr));

        close_port->setText(QCoreApplication::translate("Widget", "\345\205\263\351\227\255\344\270\262\345\217\243", nullptr));
        label_5->setText(QCoreApplication::translate("Widget", "\345\201\234\346\255\242\344\275\215\357\274\232", nullptr));
        stopbit->setItemText(0, QCoreApplication::translate("Widget", "1", nullptr));

        label_4->setText(QCoreApplication::translate("Widget", "\346\240\241\351\252\214\344\275\215\357\274\232", nullptr));
        jiaoyan->setItemText(0, QCoreApplication::translate("Widget", "0", nullptr));

        label_19->setText(QCoreApplication::translate("Widget", "RS485\345\234\260\345\235\200\357\274\232", nullptr));
        lineEdit->setText(QCoreApplication::translate("Widget", "255", nullptr));
        label_7->setText(QCoreApplication::translate("Widget", "\344\275\215\347\275\256  X\357\274\232", nullptr));
        label_13->setText(QCoreApplication::translate("Widget", "mm", nullptr));
        label_8->setText(QCoreApplication::translate("Widget", "\344\275\215\347\275\256  Y:", nullptr));
        label_14->setText(QCoreApplication::translate("Widget", "mm", nullptr));
        label_9->setText(QCoreApplication::translate("Widget", "\344\275\215\347\275\256  Z:", nullptr));
        label_15->setText(QCoreApplication::translate("Widget", "mm", nullptr));
        label_6->setText(QCoreApplication::translate("Widget", "\351\200\237\345\272\246:", nullptr));
        SD->setItemText(0, QCoreApplication::translate("Widget", "\344\275\216\351\200\237", nullptr));
        SD->setItemText(1, QCoreApplication::translate("Widget", "\344\270\255\351\200\237", nullptr));
        SD->setItemText(2, QCoreApplication::translate("Widget", "\351\253\230\351\200\237", nullptr));

        label_17->setText(QCoreApplication::translate("Widget", "\347\212\266\346\200\201\346\217\220\347\244\272        \357\274\232", nullptr));
        State_Anylise->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
