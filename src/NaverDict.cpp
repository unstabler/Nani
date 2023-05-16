//
// Created by cheesekun on 5/17/23.
//

#include <QString>
#include <QNetworkReply>

#include <QUrl>
#include <QUrlQuery>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "NaverDict.hpp"

NaverDict::NaverDict(QObject *parent):
    QObject(parent),
    _networkAccessManager(new QNetworkAccessManager(this))
{

}

QUrl NaverDict::buildQueryUrl(const QString &lang, const QString &query) {
    QUrl url("https://ac-dict.naver.com");
   
    url.setPath(QStringLiteral("/%1ko/ac").arg(lang));
    url.setQuery(QUrlQuery({
        {"st", "11"},
        {"r_lt", "11"},
        {"q", query},
        {"_callback", "cb"}
    }));
    
    return url;
}

void NaverDict::query(const QString &lang, const QString &query) {
    QUrl url = buildQueryUrl(lang, query);
    QNetworkRequest request(url);
    
    auto *reply = _networkAccessManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        auto jsonp = QString::fromUtf8(reply->readAll());
        QStringRef json(&jsonp, 3, jsonp.length() - 3 - 1);
        
        auto document = QJsonDocument::fromJson(json.toUtf8());
        auto items = document.object().value(QStringLiteral("items")).toArray()[0].toArray();
        
        Q_EMIT completed(items);
    });
    
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply](QNetworkReply::NetworkError code) {
        auto errorString = reply->errorString();
        
        Q_EMIT error(errorString);
    });
}
