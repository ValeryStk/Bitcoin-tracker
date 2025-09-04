#ifndef BTC_PRICE_FETCHER_H
#define BTC_PRICE_FETCHER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QLabel>


class BTCPriceFetcher : public QObject
{
    Q_OBJECT
public:
    explicit BTCPriceFetcher(QLabel *priceField);

    void fetchPrice();

    double last_fetched_btc_price() const;

private slots:
    void onResult(QNetworkReply* reply);

private:
    QNetworkAccessManager* manager;
    QLabel* priceField;
    double m_last_fetched_btc_price;
};

#endif // BTC_PRICE_FETCHER_H
