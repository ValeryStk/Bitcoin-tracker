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

private slots:
    void onResult(QNetworkReply* reply);

private:
    QNetworkAccessManager* manager;
    QLabel* priceField;
};

#endif // BTC_PRICE_FETCHER_H
