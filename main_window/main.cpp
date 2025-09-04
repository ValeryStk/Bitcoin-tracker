#include "bitcoin_biper_mainwindow.h"

#include <QApplication>
#include <QPalette>
#include <QStyleFactory>

void setDarkTheme(QApplication& app) {
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(42, 42, 42));
    darkPalette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Highlight, QColor(142, 45, 197).lighter());
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    app.setStyle(QStyleFactory::create("Fusion"));
    app.setPalette(darkPalette);
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setDarkTheme(a);
    BitcoinBiperMainWindow w;
    w.show();
    return a.exec();
}
