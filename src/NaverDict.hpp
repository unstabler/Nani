//
// Created by cheesekun on 5/17/23.
//

#ifndef NANI_NAVERDICT_HPP
#define NANI_NAVERDICT_HPP

#include <QObject>
#include <QString>

#include <QNetworkAccessManager>
#include <QUrl>

#include <QJsonArray>

class NaverDict: public QObject {
Q_OBJECT
public:
    NaverDict(QObject *parent = nullptr);

    QUrl buildQueryUrl(const QString &lang, const QString &query);

public Q_SLOTS:
    void query(const QString &lang, const QString &query);

Q_SIGNALS:
    void completed(QJsonArray);
    void error(QString);

private:
    QNetworkAccessManager *_networkAccessManager;
};


#endif //NANI_NAVERDICT_HPP
