#include "bitcoin_biper_mainwindow.h"
#include "ui_bitcoin_biper_mainwindow.h"

#include <QSvgRenderer>
#include <QPixmap>
#include <QPainter>

constexpr char svg_btc[] = R"BitcoinSvg(<svg xmlns="http://www.w3.org/2000/svg" width="63.993" height="64.001"><path d="M258.845 82.989c-4.274 17.143-21.637 27.576-38.782 23.301-17.138-4.274-27.571-21.638-23.295-38.78 4.272-17.145 21.635-27.579 38.775-23.305 17.144 4.274 27.576 21.64 23.302 38.784z" style="fill:#f7931a" transform="translate(-195.81 -43.247)"/><path d="M241.91 70.689c.637-4.258-2.605-6.547-7.038-8.074l1.438-5.768-3.511-.875-1.4 5.616c-.923-.23-1.871-.447-2.813-.662l1.41-5.653-3.509-.875-1.439 5.766c-.764-.174-1.514-.346-2.242-.527l.004-.018-4.842-1.209-.934 3.75s2.605.597 2.55.634c1.422.355 1.679 1.296 1.636 2.042l-1.638 6.571c.098.025.225.061.365.117l-.371-.092-2.296 9.205c-.174.432-.615 1.08-1.609.834.035.051-2.552-.637-2.552-.637l-1.743 4.019 4.569 1.139c.85.213 1.683.436 2.503.646l-1.453 5.834 3.507.875 1.439-5.772c.958.26 1.888.5 2.798.726l-1.434 5.745 3.511.875 1.453-5.823c5.987 1.133 10.489.676 12.384-4.739 1.527-4.36-.076-6.875-3.226-8.515 2.294-.529 4.022-2.038 4.483-5.155zm-8.022 11.249c-1.085 4.36-8.426 2.003-10.806 1.412l1.928-7.729c2.38.594 10.012 1.77 8.878 6.317zm1.086-11.312c-.99 3.966-7.1 1.951-9.082 1.457l1.748-7.01c1.982.494 8.365 1.416 7.334 5.553z" style="fill:#fff" transform="translate(-195.81 -43.247)"/></svg>)BitcoinSvg";

namespace {

QPixmap renderSvgToPixmap(const QByteArray& svgData,
                          QSize size = QSize(64, 64)) {
    QSvgRenderer renderer(svgData);
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    renderer.render(&painter);
    return pixmap;
}


double calculateAverageEntryPrice(QTableWidget* table, int columnIndex) {
    double sum = 0.0;
    int count = 0;

    for (int row = 0; row < table->rowCount(); ++row) {
        QTableWidgetItem* item = table->item(row, columnIndex);
        if (item && !item->text().isEmpty()) {
            bool ok;
            // Удаляем разделители тысяч, если есть
            QString cleanText = item->text().remove(',');
            double value = cleanText.toDouble(&ok);
            if (ok) {
                sum += value;
                count++;
            }
        }
    }

    return (count > 0) ? (sum / count) : 0.0;
}

void updateBTCAmount(QTableWidget* table, int row, int usdtColumn, int btcColumn, double btcPriceUSD) {
    QTableWidgetItem* usdtItem = table->item(row, usdtColumn);
    if (!usdtItem) return;

    bool ok;
    QString cleanText = usdtItem->text().remove(',');  // если есть разделители
    double usdtAmount = cleanText.toDouble(&ok);
    if (!ok) return;

    double btcAmount = usdtAmount / btcPriceUSD;

    // Форматируем BTC с 8 знаками после запятой
    QString btcText = QString::number(btcAmount, 'f', 8);
    table->setItem(row, btcColumn, new QTableWidgetItem(btcText));
}


double calculateTotalProfitLoss(QTableWidget* table,
                                int entryPriceColumn,
                                int btcAmountColumn,
                                double currentBTCPrice) {
    double totalProfitLoss = 0.0;

    for (int row = 0; row < table->rowCount(); ++row) {
        QTableWidgetItem* entryItem = table->item(row, entryPriceColumn);
        QTableWidgetItem* btcItem = table->item(row, btcAmountColumn);

        if (!entryItem || !btcItem) continue;

        bool ok1, ok2;
        double entryPrice = entryItem->text().remove(',').toDouble(&ok1);
        double btcAmount = btcItem->text().remove(',').toDouble(&ok2);

        if (ok1 && ok2) {
            double profitLoss = (currentBTCPrice - entryPrice) * btcAmount;
            totalProfitLoss += profitLoss;
        }
    }

    return totalProfitLoss;
}



}


BitcoinBiperMainWindow::BitcoinBiperMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BitcoinBiperMainWindow)
    ,m_btc_timer(new QTimer)
{
    ui->setupUi(this);
    btc_fetcher = new BTCPriceFetcher(ui->label_btc_price);
    connect(m_btc_timer,SIGNAL(timeout()),this ,SLOT(updateBtcPrice()));
    m_btc_timer->start(10000);
    ui->label_btc_logo->setPixmap(renderSvgToPixmap(svg_btc,ui->label_btc_logo->size()));

        ui->tableWidget_transactions->setRowCount(5);      // Количество строк
        ui->tableWidget_transactions->setColumnCount(3);   // Количество столбцов

        // Устанавливаем заголовок столбца
        QStringList headers;
        headers << "Цена входа" << "Количество в BTC" << "Количество в USDT";
        ui->tableWidget_transactions->setHorizontalHeaderLabels(headers);

        // Настройка внешнего вида
        ui->tableWidget_transactions->horizontalHeader()->setStretchLastSection(true);
}

BitcoinBiperMainWindow::~BitcoinBiperMainWindow()
{
    delete ui;
}

void BitcoinBiperMainWindow::updateBtcPrice()
{
    btc_fetcher->fetchPrice();
    double average_price = calculateAverageEntryPrice(ui->tableWidget_transactions,0);

    double profit = calculateTotalProfitLoss(ui->tableWidget_transactions,0,1,btc_fetcher->last_fetched_btc_price());
    ui->label_average_price->setText(QString::number(profit));
}


void BitcoinBiperMainWindow::on_tableWidget_transactions_cellChanged(int row,
                                                                     int column)
{
   if(column!=2)return;
   double price = ui->tableWidget_transactions->item(row,0)->text().toDouble();
   updateBTCAmount(ui->tableWidget_transactions,row,column,1,price);
}

