#include "bitcoin_biper_mainwindow.h"

#include <trading_table_model.h>

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>
#include <QPixmap>
#include <QSvgRenderer>
#include <QTableView>

#include "ui_bitcoin_biper_mainwindow.h"

constexpr char svg_btc[] =
    R"BitcoinSvg(<svg xmlns="http://www.w3.org/2000/svg" width="63.993" height="64.001"><path d="M258.845 82.989c-4.274 17.143-21.637 27.576-38.782 23.301-17.138-4.274-27.571-21.638-23.295-38.78 4.272-17.145 21.635-27.579 38.775-23.305 17.144 4.274 27.576 21.64 23.302 38.784z" style="fill:#f7931a" transform="translate(-195.81 -43.247)"/><path d="M241.91 70.689c.637-4.258-2.605-6.547-7.038-8.074l1.438-5.768-3.511-.875-1.4 5.616c-.923-.23-1.871-.447-2.813-.662l1.41-5.653-3.509-.875-1.439 5.766c-.764-.174-1.514-.346-2.242-.527l.004-.018-4.842-1.209-.934 3.75s2.605.597 2.55.634c1.422.355 1.679 1.296 1.636 2.042l-1.638 6.571c.098.025.225.061.365.117l-.371-.092-2.296 9.205c-.174.432-.615 1.08-1.609.834.035.051-2.552-.637-2.552-.637l-1.743 4.019 4.569 1.139c.85.213 1.683.436 2.503.646l-1.453 5.834 3.507.875 1.439-5.772c.958.26 1.888.5 2.798.726l-1.434 5.745 3.511.875 1.453-5.823c5.987 1.133 10.489.676 12.384-4.739 1.527-4.36-.076-6.875-3.226-8.515 2.294-.529 4.022-2.038 4.483-5.155zm-8.022 11.249c-1.085 4.36-8.426 2.003-10.806 1.412l1.928-7.729c2.38.594 10.012 1.77 8.878 6.317zm1.086-11.312c-.99 3.966-7.1 1.951-9.082 1.457l1.748-7.01c1.982.494 8.365 1.416 7.334 5.553z" style="fill:#fff" transform="translate(-195.81 -43.247)"/></svg>)BitcoinSvg";

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

void saveTableToJson(QList<TradeEntry> trades, const QString& filename) {
    QJsonArray dataArray;

    for (int i = 0; i < trades.size(); ++i) {
        QJsonObject rowObject;
        auto te = trades[i];
        rowObject["Entry Price"] = QString::number(te.purchasePrice);
        rowObject["BTC"] = QString::number(te.amountBTC);
        rowObject["USDT"] = QString::number(te.amountUSDT);
        dataArray.append(rowObject);
    }

    QJsonDocument doc(dataArray);
    QFile file(QApplication::applicationDirPath() + "/" + filename);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }
}

void loadTableFromJson(TradingTableModel* model, const QString& filename) {
    QFile file(QApplication::applicationDirPath() + "/" + filename);
    if (!file.open(QIODevice::ReadOnly)) return;

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    QJsonArray dataArray = doc.array();

    for (int row = 0; row < dataArray.size(); ++row) {
        QJsonObject obj = dataArray[row].toObject();
        TradeEntry te;
        te.amountBTC = obj["BTC"].toString().toDouble();
        te.amountUSDT = obj["USDT"].toString().toDouble();
        te.purchasePrice = obj["Entry Price"].toString().toDouble();
        model->addTrade(te);
    }
}

void updateProfitLabel(QLabel* label, double profit) {
    QString text =
        QString("Profit: %1")
            .arg(profit, 0, 'f', 2);  // формат до 2 знаков после запятой
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

}  // namespace
TradingTableModel* ttm;
QTableView* tv;

BitcoinBiperMainWindow::BitcoinBiperMainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::BitcoinBiperMainWindow),
      m_btc_timer(new QTimer) {
    ui->setupUi(this);
    ttm = new TradingTableModel;
    tv = ui->tableView_bitcoin_entry_points;
    tv->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tv->setWindowTitle("Entry points");
    tv->setContextMenuPolicy(Qt::CustomContextMenu);
    tv->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    tv->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    connect(tv, &QTableView::customContextMenuRequested, this,
            &BitcoinBiperMainWindow::showContextMenu);

    tv->setModel(ttm);
    loadTableFromJson(ttm, "transactions");
    tv->show();

    btc_fetcher = new BTCPriceFetcher(ui->label_btc_price);
    connect(m_btc_timer, SIGNAL(timeout()), this, SLOT(updateBtcPrice()));
    m_btc_timer->start(10000);
    ui->label_btc_logo->setPixmap(
        renderSvgToPixmap(svg_btc, ui->label_btc_logo->size()));
}

BitcoinBiperMainWindow::~BitcoinBiperMainWindow() { delete ui; }

void BitcoinBiperMainWindow::updateBtcPrice() {
    btc_fetcher->fetchPrice();
    double btc_price = btc_fetcher->last_fetched_btc_price();
    auto profit = (btc_price - ttm->getLastSummary().averagePrice) *
                  ttm->getLastSummary().totalBTC;
    qDebug() << "-->" << profit;
    updateProfitLabel(ui->label_profit, profit);
}

void BitcoinBiperMainWindow::on_pushButton_save_to_json_clicked() {
    saveTableToJson(ttm->getTradesEntries(), "transactions");
}

void BitcoinBiperMainWindow::showContextMenu(const QPoint& pos) {
    QModelIndex index = tv->indexAt(pos);

    QMenu contextMenu;
    QAction* addAction = contextMenu.addAction("Add entry");
    QAction* removeAction = contextMenu.addAction("Delete entry");

    // Отключаем удаление, если клик по результирующей строке
    if (index.row() == ttm->rowCount(index) - 1) {
        qDebug() << "-------------Delete row---------------" << index.row();
        removeAction->setEnabled(false);
    }

    QAction* selectedAction =
        contextMenu.exec(tv->viewport()->mapToGlobal(pos));
    if (selectedAction == addAction) {
        TradeEntry newEntry{btc_fetcher->last_fetched_btc_price(), 0, 100};
        ttm->addTrade(newEntry);
    } else if (selectedAction == removeAction) {
        ttm->removeTrade(index.row());
    }
}
