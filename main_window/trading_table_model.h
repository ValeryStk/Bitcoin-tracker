#ifndef TRADING_TABLEMODEL_H
#define TRADING_TABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>

struct TradeEntry {
    double purchasePrice;
    double amountBTC;
    double amountUSDT;
};

struct TradeSummary {
    double averagePrice;
    double totalBTC;
    double totalUSDT;
};

class TradingTableModel:public QAbstractTableModel
{
public:
    TradingTableModel();

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void addTrade(const TradeEntry &entry);
    void removeTrade(int row);
    TradeSummary getLastSummary() const;
    QList<TradeEntry> getTradesEntries() const;

private:
    QList<TradeEntry> trades;
    mutable TradeSummary trade_summary;
};

#endif // TRADING_TABLEMODEL_H
