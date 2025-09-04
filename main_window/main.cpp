#include "bitcoin_biper_mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BitcoinBiperMainWindow w;
    w.show();
    return a.exec();
}
