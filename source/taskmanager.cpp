#include "taskmanager.h"

#include <QStringList>
#include <QString>
#include <QVariant>

#include "core.h"

/* Вызываемые методы */
#include "Stats/shiftproblem.h"
#include "Stats/anaysisdichotomousdata.h"


void fillFlags(const QMap<QString, QVariant>& configs, const QVector<Pair>& names, QSet<FLAG>& flags) {
    for(auto val: names)
        if(configs.contains(val.name))
            flags.insert(val.flag);
}

TaskManager::TaskManager() {

}

void TaskManager::handle(const Core::CriterialInfo* criterial, QMap<QString, QVariant> configs, const QList<Sample>& samples, OutputData& result) {
    switch (criterial->section_type) {
        case Core::CriterialSection::ShiftProblem:
            handleShiftProblem(configs, samples, result);
        break;

        case Core::CriterialSection::SignRank:
            handleSignRank(configs, samples, result);
        break;

        case Core::CriterialSection::DichotomicData:
            handleDichotomicData(configs, samples, result);
        break;


        default:
            // Пока что сюда невозможно попасть
        break;
    }
}


/*
 *  Знаковый критерий сдвига Уилкоксона
*/
void TaskManager::handleShiftProblem(QMap<QString, QVariant>& configs, const QList<Sample>& samples, OutputData& result) {
    using Core::Criterial::WilcoxonSignRank;
    using Core::Criterial::WilcoxonSignRankEstimation;
    using Core::Criterial::WilcoxonSignRankInterval;

    QString name = Core::Criterial::criterialName(Core::CriterialSection::ShiftProblem, configs["name"].toString());
    const Core::CriterialInfo* const criterial = Core::Criterial::getCriterialByName(name);
    int first_idx = configs[criterial->samples[0]].toUInt();
    int second_idx = configs[criterial->samples[1]].toUInt();
    double level;
    // Здесь у всех критериев ShiftProblem первым параметром идет уровень доверия (если он есть)
    if(!criterial->parameters.isEmpty() && configs.contains(criterial->parameters.front().name))
            level = configs[criterial->parameters.front().name].toDouble();

    QSet<FLAG> flags;
    fillFlags(configs, criterial->aproximations, flags);
    fillFlags(configs, criterial->corrects, flags);

    if(configs["name"].toString() == WilcoxonSignRank.name)
        ShiftProblem::wilcoxonSignRank(samples.at(first_idx), samples.at(second_idx), level, flags, result);

    else if(configs["name"].toString() == WilcoxonSignRankEstimation.name)
        ShiftProblem::wilcoxonSignRankEstimation(samples.at(first_idx), samples.at(second_idx), result);

    else if(configs["name"].toString() == WilcoxonSignRankInterval.name)
        ShiftProblem::wilcoxonSignRankInterval(samples.at(first_idx), samples.at(second_idx), level, result);

}

void TaskManager::handleSignRank(QMap<QString, QVariant>& configs, const QList<Sample>& samples, OutputData& result) {
    using Core::Criterial::SignRank;
    using Core::Criterial::SignRankEstimation;
    using Core::Criterial::SignRangInterval;

    QString name = Core::Criterial::criterialName(Core::CriterialSection::SignRank, configs["name"].toString());
    const Core::CriterialInfo* const criterial = Core::Criterial::getCriterialByName(name);
    int first_idx = configs[criterial->samples[0]].toUInt();
    int second_idx = configs[criterial->samples[1]].toUInt();
    double level;
    if(!criterial->parameters.isEmpty() && configs.contains(criterial->parameters.front().name))
            level = configs[criterial->parameters.front().name].toDouble();

    QSet<FLAG> flags;
    fillFlags(configs, criterial->aproximations, flags);
    fillFlags(configs, criterial->corrects, flags);

    if(configs["name"].toString() == SignRank.name)
        SignRank::signRank(samples.at(first_idx), samples.at(second_idx), level, flags, result);

    else if(configs["name"].toString() == SignRankEstimation.name)
        SignRank::signRankEstimation(samples.at(first_idx), samples.at(second_idx), result);

    else if(configs["name"].toString() == SignRangInterval.name)
        SignRank::signRankInterval(samples.at(first_idx), samples.at(second_idx), level, flags, result);

}

void TaskManager::handleDichotomicData(QMap<QString, QVariant>& configs, const QList<Sample>& samples, OutputData& result) {

}
