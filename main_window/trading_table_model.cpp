#include "trading_table_model.h"

#include "qbrush.h"
#include "qcolor.h"
#include "qdebug.h"
#include "qfont.h"

constexpr int column_size = 3;

TradingTableModel::TradingTableModel() { trade_summary = {0, 0, 0}; }

int TradingTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return trades.size() + 1;
}

int TradingTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return column_size;  // Цена закупки, Сумма в BTC, Сумма в USDT
}

QVariant TradingTableModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::EditRole && index.row() < trades.size()) {
        const TradeEntry &entry = trades.at(index.row());
        switch (index.column()) {
            case 0:
                return entry.purchasePrice;
            case 1:
                return entry.amountBTC;
            case 2:
                return entry.amountUSDT;
        }
    }

    // Выделение последней строки
    if (role == Qt::BackgroundRole && index.row() == trades.size()) {
        return QBrush(QColor(255, 255, 200));  // мягкий жёлтый
    }
    // Тёмный цвет шрифта
    if (role == Qt::ForegroundRole && index.row() == trades.size()) {
        return QBrush(QColor(50, 50, 50));  // тёмно-серый
    }

    // Жирный шрифт
    if (role == Qt::FontRole && index.row() == trades.size()) {
        QFont font;
        font.setBold(true);
        return font;
    }

    if (!index.isValid() || role != Qt::DisplayRole) return QVariant();
    if (index.row() < trades.size()) {
        const TradeEntry &entry = trades.at(index.row());
        switch (index.column()) {
            case 0:
                return entry.purchasePrice;
            case 1:
                return entry.amountBTC;
            case 2:
                return entry.amountUSDT;
            default:
                return QVariant();
        }
    } else {
        switch (index.column()) {
            case 0:
                return trade_summary.totalBTC > 0
                           ? trade_summary.totalUSDT / trade_summary.totalBTC
                           : QVariant("—");
            case 1:
                return QString::number(trade_summary.totalBTC, 'f', 6);
            case 2:
                return QString::number(trade_summary.totalUSDT, 'f', 2);
            default:
                return QVariant();
        }
    }
}

QVariant TradingTableModel::headerData(int section, Qt::Orientation orientation,
                                       int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    switch (section) {
        case 0:
            return QString("Entry Price");
        case 1:
            return QString("BTC");
        case 2:
            return QString("USDT");
        default:
            return QVariant();
    }
}

bool TradingTableModel::setData(const QModelIndex &index, const QVariant &value,
                                int role) {
    if (!index.isValid() || role != Qt::EditRole) return false;

    if (index.row() >= trades.size())
        return false;  // Результирующая строка — не редактируется

    TradeEntry &entry = trades[index.row()];
    bool ok = false;
    double newValue = value.toDouble(&ok);
    if (!ok || newValue <= 0) return false;

    if (index.column() == 0) {
        entry.purchasePrice = newValue;
        entry.amountBTC = entry.amountUSDT / entry.purchasePrice;
    } else if (index.column() == 2) {
        entry.amountUSDT = newValue;
        entry.amountBTC = entry.amountUSDT / entry.purchasePrice;
    } else {
        return false;
    }
    recalculateSummary();
    emit dataChanged(index, index);
    emit dataChanged(this->index(index.row(), 1),
                     this->index(index.row(), 1));  // Обновляем BTC
    emit tableUpdated(QPrivateSignal{});
    return true;
}

Qt::ItemFlags TradingTableModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) return Qt::NoItemFlags;

    if (index.row() >= trades.size())
        return Qt::ItemIsEnabled;  // Результирующая строка — только для чтения

    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);
    if (index.column() == 0 ||
        index.column() == 2)  // Цена закупки и Сумма в USDT
        return defaultFlags | Qt::ItemIsEditable;

    return defaultFlags;
}

void TradingTableModel::addTrade(const TradeEntry &entry) {
    // Проверка на положительные значения
    if (entry.purchasePrice <= 0 || entry.amountUSDT <= 0) return;

    // Пересчёт количества BTC
    TradeEntry adjustedEntry = entry;
    adjustedEntry.amountBTC =
        adjustedEntry.amountUSDT / adjustedEntry.purchasePrice;

    beginInsertRows(QModelIndex(), trades.size(), trades.size());
    trades.append(adjustedEntry);
    endInsertRows();
    recalculateSummary();
    emit tableUpdated(QPrivateSignal{});
}

void TradingTableModel::removeTrade(int row) {
    qDebug() << "remove row with index: " << row;
    if (row < 0 || row == trades.size())  // не удаляем результирующую строку
        return;

    beginRemoveRows(QModelIndex(), row, row);
    trades.removeAt(row);
    endRemoveRows();
    recalculateSummary();
    emit tableUpdated(QPrivateSignal{});
}

TradeSummary TradingTableModel::getLastSummary() const { return trade_summary; }

QList<TradeEntry> TradingTableModel::getTradesEntries() const { return trades; }

void TradingTableModel::recalculateSummary() {
    double totalBTC = 0.0;
    double totalUSDT = 0.0;
    for (const TradeEntry &entry : qAsConst(trades)) {
        totalBTC += entry.amountBTC;
        totalUSDT += entry.amountUSDT;
    }
    trade_summary.totalUSDT = totalUSDT;
    trade_summary.totalBTC = totalBTC;
    trade_summary.averagePrice = totalUSDT / totalBTC;
}
