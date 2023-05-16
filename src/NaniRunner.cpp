//
// Created by cheesekun on 5/17/23.
//

#include <QSet>
#include <QSharedPointer>

#include "NaniRunner.hpp"
#include "NaverDict.hpp"

const QSet<QString> SUPPORTED_LANGUAGES {
    QStringLiteral("en"),
    QStringLiteral("ja"),
    QStringLiteral("zh"),
};

NaniRunner::NaniRunner(QObject *parent, const QVariantList &args):
    AbstractRunner(parent, args)
{
    setPriority(NormalPriority);
}

bool NaniRunner::isNaniQuery(const QString &query) const {
    for (const auto &lang: SUPPORTED_LANGUAGES) {
        if (query.startsWith(lang + QStringLiteral(":"))) {
            return true;
        }
    }
    
    return false;
}

void NaniRunner::match(Plasma::RunnerContext &context) {
    QString query = context.query();
    
    if (!isNaniQuery(query)) {
        return;
    }
    
    QEventLoop loop;
    QSharedPointer<NaverDict> naverDict(new NaverDict);
    
    QString queryLang = query.mid(0, 2);
    QString queryText = query.mid(3);
    
    connect(naverDict.get(), &NaverDict::completed, this, [this, &query, &context, &loop](QJsonArray items) {
        
        QList<Plasma::QueryMatch> matches;
        int i = 0;
        
        for (auto item: items) {
            auto itemArr = item.toArray();
            
            auto word = itemArr[0].toArray()[0].toString();
            
            auto ja_yomi = itemArr[1].toArray()[0].toString();
            auto zh_pinyin = itemArr[2].toArray()[0].toString();
            
            auto meaning = itemArr[3].toArray()[0].toString();
            
            Plasma::QueryMatch match(this);
            
            if (!ja_yomi.isEmpty()) {
                match.setText(QStringLiteral("%1 (%2) %3").arg(ja_yomi).arg(word).arg(meaning));
            } else if (!zh_pinyin.isEmpty()) {
                match.setText(QStringLiteral("%1 %2 %3").arg(word).arg(zh_pinyin).arg(meaning));
            } else {
                match.setText(QStringLiteral("%1 %2").arg(word).arg(meaning));
            }
            
            match.setId(QStringLiteral("%1_%2").arg(query).arg(i));
            match.setRelevance(1.0f - ((float) (i + 1) / items.size()));
            match.setType(Plasma::QueryMatch::InformationalMatch);
            
            matches.push_back(match);
            i++;
        }
        
        context.addMatches(matches);
        
        // TODO: matches.push_back("{queryText}로 더 찾아보기..")
        
        
        loop.quit();
    });

    
    connect(naverDict.get(), &NaverDict::error, this, [this, &context, &loop](QString errorText) {
        Plasma::QueryMatch match(this);
        match.setText(errorText);
        match.setId(errorText);
        
        match.setRelevance(0.1);
        match.setType(Plasma::QueryMatch::InformationalMatch);
        
        context.addMatch(match);
        
        loop.quit();
    });
    
    naverDict->query(queryLang, queryText);
    
    loop.exec();
}

void NaniRunner::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) {
}



void NaniRunner::reloadConfiguration() {
}

void NaniRunner::init() {
    reloadConfiguration();
    
    connect(this, &NaniRunner::prepare, this, []() {
    
    });
    connect(this, &NaniRunner::teardown, this, []() {
    
    });
}
