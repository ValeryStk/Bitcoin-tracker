#include "trading_table_model.h"

constexpr int column_size = 3;

TradingTableModel::TradingTableModel()
{

}

int TradingTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return trades.size() + 1;
}

int TradingTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return column_size; // Цена закупки, Сумма в BTC, Сумма в USDT
}

QVariant TradingTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();
    if (index.row() < trades.size()) {
        const TradeEntry &entry = trades.at(index.row());
        switch (index.column()) {
        case 0: return entry.purchasePrice;
        case 1: return entry.amountBTC;
        case 2: return entry.amountUSDT;
        default: return QVariant();
        }
    }else{
        // Результирующая строка
        double totalBTC = 0.0;
        double totalUSDT = 0.0;
        for (const TradeEntry &entry : trades) {
            totalBTC += entry.amountBTC;
            totalUSDT += entry.amountUSDT;
        }
        switch (index.column()) {
        case 0:
            return totalBTC > 0 ? totalUSDT / totalBTC : QVariant("—");
        case 1:
            return totalBTC;
        case 2:
            return totalUSDT;
        default:
            return QVariant();
        }
    }
}

QVariant TradingTableModel::headerData(int section,
                                       Qt::Orientation orientation,
                                       int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return QVariant();

    switch (section) {
    case 0: return QString("Цена закупки");
    case 1: return QString("Сумма в BTC");
    case 2: return QString("Сумма в USDT");
    default: return QVariant();
    }
}

bool TradingTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    if (index.row() >= trades.size())
         return false; // Результирующая строка — не редактируется

    TradeEntry &entry = trades[index.row()];
    bool ok = false;
    double newValue = value.toDouble(&ok);
    if (!ok || newValue <= 0)
        return false;

    if (index.column() == 0) {
        entry.purchasePrice = newValue;
        entry.amountBTC = entry.amountUSDT / entry.purchasePrice;
    } else if (index.column() == 2) {
        entry.amountUSDT = newValue;
        entry.amountBTC = entry.amountUSDT / entry.purchasePrice;
    } else {
        return false;
    }

    emit dataChanged(index, index);
    emit dataChanged(this->index(index.row(), 1), this->index(index.row(), 1)); // Обновляем BTC
    return true;
}

Qt::ItemFlags TradingTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    if (index.row() >= trades.size())
            return Qt::ItemIsEnabled; // Результирующая строка — только для чтения

    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);
    if (index.column() == 0 || index.column() == 2) // Цена закупки и Сумма в USDT
        return defaultFlags | Qt::ItemIsEditable;

    return defaultFlags;
}

void TradingTableModel::addTrade(const TradeEntry &entry)
{
    // Проверка на положительные значения
    if (entry.purchasePrice <= 0 || entry.amountBTC <= 0 || entry.amountUSDT <= 0)
        return;

    // Проверка согласованности: USDT ≈ BTC × Цена
    double expectedUSDT = entry.amountBTC * entry.purchasePrice;
    double tolerance = 0.01; // допустимая погрешность

    if (std::abs(expectedUSDT - entry.amountUSDT) > tolerance)
        return; // несогласованные данные

    beginInsertRows(QModelIndex(), trades.size(), trades.size());
    trades.append(entry);
    endInsertRows();
}
