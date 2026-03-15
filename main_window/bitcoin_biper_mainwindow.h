#ifndef BITCOIN_BIPER_MAINWINDOW_H
#define BITCOIN_BIPER_MAINWINDOW_H

#include <QMainWindow>
#include <QShortcut>
#include <QTimer>

#include "btc_price_fetcher.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class BitcoinBiperMainWindow;
}
QT_END_NAMESPACE

class BitcoinBiperMainWindow : public QMainWindow {
    Q_OBJECT

public:
    BitcoinBiperMainWindow(QWidget *parent = nullptr);
    ~BitcoinBiperMainWindow();

private:
    Ui::BitcoinBiperMainWindow *ui;
    QTimer *m_btc_timer;
    BTCPriceFetcher *btc_fetcher;
    QShortcut *addTradeShortcut;

private slots:
    void updateBtcPrice();
    void updateProfit();
    void saveDataToJson();
    void showContextMenu(const QPoint &pos);
    void addTrade();
};
#endif  // BITCOIN_BIPER_MAINWINDOW_H
