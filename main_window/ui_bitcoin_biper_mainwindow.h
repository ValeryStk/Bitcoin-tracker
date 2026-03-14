/********************************************************************************
** Form generated from reading UI file 'bitcoin_biper_mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BITCOIN_BIPER_MAINWINDOW_H
#define UI_BITCOIN_BIPER_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BitcoinBiperMainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_btc_logo;
    QLabel *label_btc_price;
    QTableView *tableView_bitcoin_entry_points;
    QLabel *label_profit;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_save_to_json;
    QSpacerItem *horizontalSpacer;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *BitcoinBiperMainWindow)
    {
        if (BitcoinBiperMainWindow->objectName().isEmpty())
            BitcoinBiperMainWindow->setObjectName(QStringLiteral("BitcoinBiperMainWindow"));
        BitcoinBiperMainWindow->resize(982, 576);
        centralwidget = new QWidget(BitcoinBiperMainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_btc_logo = new QLabel(centralwidget);
        label_btc_logo->setObjectName(QStringLiteral("label_btc_logo"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_btc_logo->sizePolicy().hasHeightForWidth());
        label_btc_logo->setSizePolicy(sizePolicy);
        label_btc_logo->setMinimumSize(QSize(100, 100));
        label_btc_logo->setMaximumSize(QSize(100, 100));
        label_btc_logo->setStyleSheet(QStringLiteral(""));

        horizontalLayout->addWidget(label_btc_logo);

        label_btc_price = new QLabel(centralwidget);
        label_btc_price->setObjectName(QStringLiteral("label_btc_price"));
        QFont font;
        font.setPointSize(72);
        font.setBold(true);
        label_btc_price->setFont(font);
        label_btc_price->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_btc_price);


        verticalLayout->addLayout(horizontalLayout);

        tableView_bitcoin_entry_points = new QTableView(centralwidget);
        tableView_bitcoin_entry_points->setObjectName(QStringLiteral("tableView_bitcoin_entry_points"));

        verticalLayout->addWidget(tableView_bitcoin_entry_points);

        label_profit = new QLabel(centralwidget);
        label_profit->setObjectName(QStringLiteral("label_profit"));
        QFont font1;
        font1.setPointSize(16);
        label_profit->setFont(font1);

        verticalLayout->addWidget(label_profit);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButton_save_to_json = new QPushButton(centralwidget);
        pushButton_save_to_json->setObjectName(QStringLiteral("pushButton_save_to_json"));

        horizontalLayout_2->addWidget(pushButton_save_to_json);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_2);

        BitcoinBiperMainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(BitcoinBiperMainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 982, 25));
        BitcoinBiperMainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(BitcoinBiperMainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        BitcoinBiperMainWindow->setStatusBar(statusbar);

        retranslateUi(BitcoinBiperMainWindow);

        QMetaObject::connectSlotsByName(BitcoinBiperMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *BitcoinBiperMainWindow)
    {
        BitcoinBiperMainWindow->setWindowTitle(QApplication::translate("BitcoinBiperMainWindow", "Bitcoin profit", nullptr));
        label_btc_logo->setText(QString());
        label_btc_price->setText(QApplication::translate("BitcoinBiperMainWindow", "0", nullptr));
        label_profit->setText(QString());
        pushButton_save_to_json->setText(QApplication::translate("BitcoinBiperMainWindow", "Save", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BitcoinBiperMainWindow: public Ui_BitcoinBiperMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BITCOIN_BIPER_MAINWINDOW_H
