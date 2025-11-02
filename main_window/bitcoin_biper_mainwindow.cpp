#include "bitcoin_biper_mainwindow.h"
#include "ui_bitcoin_biper_mainwindow.h"

#include <QSvgRenderer>
#include <QPixmap>
#include <QPainter>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QTableView>
#include <trading_table_model.h>

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

void updateBTCAmount(QTableWidget* table,
                     int row,
                     int сolumn) {

    if(!table)return;
    QTableWidgetItem* usdtItem = table->item(row, 2);
    if (!usdtItem) return;
    QTableWidgetItem* entryPriceItem = table->item(row, 0);
    QTableWidgetItem* btcAmountItem = table->item(row, 1);

    bool ok;
    QString cleanText = usdtItem->text().remove(',');
    double usdtAmount = cleanText.toDouble(&ok);
    if (!ok) return;


    cleanText = entryPriceItem->text().remove(',');
    double entryPrice = cleanText.toDouble(&ok);
    if (!ok) return;


    cleanText = btcAmountItem->text().remove(',');
    double btcAmount = cleanText.toDouble(&ok);
    if (!ok) return;

    if(сolumn==2){
    double newBtcAmount = usdtAmount / entryPrice;
    QString btcText = QString::number(newBtcAmount, 'f', 8);
    table->setItem(row, 1, new QTableWidgetItem(btcText));
    }else if(сolumn==1){
    usdtAmount = btcAmount * entryPrice;
    QString usdtAmountText = QString::number(usdtAmount, 'f', 8);
    table->setItem(row, 2, new QTableWidgetItem(usdtAmountText));
    }
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

void saveTableToJson(QTableWidget* table, const QString& filename) {
    QJsonArray dataArray;

    for (int row = 0; row < table->rowCount(); ++row) {
        QJsonObject rowObject;
        rowObject["цена входа"] = table->item(row, 0) ? table->item(row, 0)->text() : "";
        rowObject["количество BTC"] = table->item(row, 1) ? table->item(row, 1)->text() : "";
        rowObject["количество USDT"] = table->item(row, 2) ? table->item(row, 2)->text() : "";
        dataArray.append(rowObject);
    }

    QJsonDocument doc(dataArray);
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }
}

void loadTableFromJson(QTableWidget* table, const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) return;

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonArray dataArray = doc.array();

    table->setRowCount(dataArray.size());
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"цена входа", "количество BTC", "количество USDT"});

    for (int row = 0; row < dataArray.size(); ++row) {
        QJsonObject obj = dataArray[row].toObject();
        table->setItem(row, 0, new QTableWidgetItem(obj["цена входа"].toString()));
        table->setItem(row, 1, new QTableWidgetItem(obj["количество BTC"].toString()));
        table->setItem(row, 2, new QTableWidgetItem(obj["количество USDT"].toString()));
    }
}

void updateProfitLabel(QLabel* label, double profit) {
    QString text = QString("Профит: %1").arg(profit, 0, 'f', 2); // формат до 2 знаков после запятой
    label->setText(text);

    QPalette palette = label->palette();
    if (profit > 0) {
        palette.setColor(QPalette::WindowText, Qt::green);
    } else if (profit < 0) {
        palette.setColor(QPalette::WindowText, Qt::red);
    } else {
        palette.setColor(QPalette::WindowText, Qt::white);
    }
    label->setPalette(palette);
}

}
TradingTableModel* ttm;
QTableView* tv;

BitcoinBiperMainWindow::BitcoinBiperMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BitcoinBiperMainWindow)
    ,m_btc_timer(new QTimer)
{
    ui->setupUi(this);
    ttm = new TradingTableModel;
    tv = new QTableView;
    tv->setWindowTitle("Entry points");
    tv->setContextMenuPolicy(Qt::CustomContextMenu);
    tv->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    tv->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    connect(tv, &QTableView::customContextMenuRequested,
            this, &BitcoinBiperMainWindow::showContextMenu);

    tv->setModel(ttm);
    tv->show();
    ttm->addTrade({100000,0,100});
    ttm->addTrade({106500,0,100});
    ttm->addTrade({108500,0,100});
    ttm->addTrade({102500,0,100});
    ttm->addTrade({101500,0,100});
    ttm->addTrade({96500,0,100});

    btc_fetcher = new BTCPriceFetcher(ui->label_btc_price);
    connect(m_btc_timer,SIGNAL(timeout()),this ,SLOT(updateBtcPrice()));
    m_btc_timer->start(10000);
    ui->label_btc_logo->setPixmap(renderSvgToPixmap(svg_btc,ui->label_btc_logo->size()));
    loadTableFromJson(ui->tableWidget_transactions,"transactions");

    // Устанавливаем заголовок столбца
    QStringList headers;
    headers << "Цена входа" << "Количество в BTC" << "Количество в USDT";
    ui->tableWidget_transactions->setHorizontalHeaderLabels(headers);

    // Настройка внешнего вида
    ui->tableWidget_transactions->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_transactions->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_transactions->verticalHeader()->setVisible(false);

    for (int row = 0; row < ui->tableWidget_transactions->rowCount(); ++row) {
        for (int col = 0; col < ui->tableWidget_transactions->columnCount(); ++col) {
            QTableWidgetItem* item = ui->tableWidget_transactions->item(row, col);
            if (item) {
                item->setTextAlignment(Qt::AlignCenter);
            }
        }
    }
}

BitcoinBiperMainWindow::~BitcoinBiperMainWindow()
{
    delete ui;
}

void BitcoinBiperMainWindow::updateBtcPrice()
{
    btc_fetcher->fetchPrice();
    double btc_price = btc_fetcher->last_fetched_btc_price();
    double profit = calculateTotalProfitLoss(ui->tableWidget_transactions,
                                             0,
                                             1,
                                             btc_price);

    updateProfitLabel(ui->label_average_price,profit);
    qDebug()<<"profit: "<<(btc_price - ttm->getLastSummary().averagePrice)*ttm->getLastSummary().totalBTC;
}


void BitcoinBiperMainWindow::on_tableWidget_transactions_cellChanged(int row,
                                                                     int column)
{
    int rowCount = ui->tableWidget_transactions->rowCount();
    int columnCount = ui->tableWidget_transactions->columnCount();

    if (row >= 0 && row < rowCount && column >= 0 && column < columnCount) {
                ui->tableWidget_transactions->blockSignals(true);
                updateBTCAmount(ui->tableWidget_transactions, row, column);
                ui->tableWidget_transactions->blockSignals(false);

    }

}

void BitcoinBiperMainWindow::on_pushButton_save_to_json_clicked()
{
    saveTableToJson(ui->tableWidget_transactions,"transactions");
}

void BitcoinBiperMainWindow::showContextMenu(const QPoint &pos)
{
    QModelIndex index = tv->indexAt(pos);

    QMenu contextMenu;
    QAction *addAction = contextMenu.addAction("Добавить сделку");
    QAction *removeAction = contextMenu.addAction("Удалить сделку");

    // Отключаем удаление, если клик по результирующей строке
    if (index.row() == ttm->rowCount(index)-1){
        qDebug()<<"-------------Delete row---------------"<<index.row();
        removeAction->setEnabled(false);
    }

    QAction *selectedAction = contextMenu.exec(tv->viewport()->mapToGlobal(pos));
    if (selectedAction == addAction) {
        TradeEntry newEntry{100000,0,100};
        ttm->addTrade(newEntry);
    } else if (selectedAction == removeAction) {
        ttm->removeTrade(index.row());
    }
}

