#include "btc_price_fetcher.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

BTCPriceFetcher::BTCPriceFetcher(QLabel* priceField)
    : QObject(nullptr),
      priceField(priceField) {
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,
            this, &BTCPriceFetcher::onResult);
}

void BTCPriceFetcher::fetchPrice() {
    QUrl url("https://api.binance.com/api/v3/ticker/price?symbol=BTCUSDT");
    QNetworkRequest request(url);
    manager->get(request);
}

void BTCPriceFetcher::onResult(QNetworkReply* reply) {

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Ошибка получения цены BTC:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObj = jsonDoc.object();
    double price = jsonObj["price"].toString().toDouble();
    m_last_fetched_btc_price = price;
    QLocale locale = QLocale::system();
    QString formattedPrice = locale.toString(price, 'f', 2);
    priceField->setText(formattedPrice);
    reply->deleteLater();
}

double BTCPriceFetcher::last_fetched_btc_price() const
{
    return m_last_fetched_btc_price;
}

