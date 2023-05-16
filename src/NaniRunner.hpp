//
// Created by cheesekun on 5/17/23.
//

#ifndef NANI_NANIRUNNER_HPP
#define NANI_NANIRUNNER_HPP

#include <KRunner/AbstractRunner>

class NaniRunner: public Plasma::AbstractRunner {
    Q_OBJECT
    
public:
    NaniRunner(QObject *parent, const QVariantList &args);
    
    void match(Plasma::RunnerContext &context) override;
    void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) override;
    void reloadConfiguration() override;
    
    bool isNaniQuery(const QString &query) const;
    
protected Q_SLOTS:
    void init();

private:

};


#endif //NANI_NANIRUNNER_HPP
