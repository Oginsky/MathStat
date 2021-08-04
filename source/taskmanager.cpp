#include "taskmanager.h"

#include <QStringList>
#include <QString>
#include <QVariant>

#include "core.h"

/* Вызываемые методы */
#include "Stats/shiftproblem.h"
#include "Stats/anaysisdichotomousdata.h"
#include "Stats/twosampleshiftproblem.h"
#include "Stats/twosamplescaleproblem.h"


void fillFlags(const QMap<QString, QVariant>& configs, const QVector<Pair>& names, QSet<FLAG>& flags);
void fillOrders(const QMap<QString, QVariant>& configs, const QVector<Pair>& names, QSet<QString>& orders);


TaskManager::TaskManager() {

}

void TaskManager::handle(const Core::CriterialInfo* criterial, QMap<QString, QVariant> configs, const QList<Sample>& samples, OutputData& result) {
    switch (criterial->section_type) {
        case Core::CriterialSection::ShiftProblem:          handleShiftProblem(configs, samples, result);           break;
        case Core::CriterialSection::SignRank:              handleSignRank(configs, samples, result);               break;
        case Core::CriterialSection::TwoSampleShiftProblem: handleTwoSampleShiftProblem(configs, samples, result);  break;
        case Core::CriterialSection::TwoSampleScaleProblem: handleTwoSampleScaleProblem(configs, samples, result);  break;
        case Core::CriterialSection::DichotomicData:        handleDichotomicData(configs, samples, result);         break;

        default:
            // Пока что сюда невозможно попасть
        break;
    }
}


/* Знаковый критерий сдвига Уилкоксона */
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

/* */
void TaskManager::handleSignRank(QMap<QString, QVariant>& configs, const QList<Sample>& samples, OutputData& result) {
    using Core::Criterial::SignRank;
    using Core::Criterial::SignRankEstimation;
    using Core::Criterial::SignRangInterval;

    QString name = Core::Criterial::criterialName(Core::CriterialSection::SignRank, configs["name"].toString());
    const Core::CriterialInfo* const criterial = Core::Criterial::getCriterialByName(name);
    size_t first_idx = configs[criterial->samples[0]].toUInt();
    size_t second_idx = configs[criterial->samples[1]].toUInt();
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

void TaskManager::handleTwoSampleShiftProblem(QMap<QString, QVariant>& configs, const QList<Sample>& samples, OutputData& result) {
    using Core::Criterial::MedianTestMuda;
    using Core::Criterial::WilcoxonSummRank;
    using Core::Criterial::MannWhitneyTest;
    using Core::Criterial::FlynnerPolycelloTest;

    QString name = Core::Criterial::criterialName(Core::CriterialSection::TwoSampleShiftProblem, configs["name"].toString());
    const Core::CriterialInfo* const criterial = Core::Criterial::getCriterialByName(name);
    size_t first_idx = configs[criterial->samples[0]].toUInt();
    size_t second_idx = configs[criterial->samples[1]].toUInt();

    QSet<FLAG> flags;
    fillFlags(configs, criterial->aproximations, flags);
    fillFlags(configs, criterial->corrects, flags);

    double alpha = configs[criterial->parameters.front().name].toDouble();

    if(configs["name"].toString() == MedianTestMuda.name)
        TwoSampleShiftProblem::medianTestMuda(samples.at(first_idx), samples.at(second_idx), alpha, flags, result);

    else if(configs["name"].toString() == WilcoxonSummRank.name)
        TwoSampleShiftProblem::wilcoxonSummRank(samples.at(first_idx), samples.at(second_idx), alpha, flags, result);

    else if(configs["name"] == MannWhitneyTest.name) {
        QSet<QString> orders;
        fillOrders(configs, criterial->aproximations, orders);
        fillOrders(configs, criterial->corrects, orders);

        TwoSampleShiftProblem::mannWhitneyTest(samples.at(first_idx), samples.at(second_idx), alpha, flags, orders, result);
    }

    else if(configs["name"] == FlynnerPolycelloTest.name)
        TwoSampleShiftProblem::flynnerPolycelloTest(samples.at(first_idx), samples.at(second_idx), alpha, flags, result);
}

void TaskManager::handleTwoSampleScaleProblem(QMap<QString, QVariant>& configs, const QList<Sample>& samples, OutputData& result) {
    using Core::Criterial::AnsariBradleyTest;
    using Core::Criterial::SigelTukeyTest;
    using Core::Criterial::MosesTest;

    QString name = Core::Criterial::criterialName(Core::CriterialSection::TwoSampleScaleProblem, configs["name"].toString());
    const Core::CriterialInfo* const criterial = Core::Criterial::getCriterialByName(name);
    size_t first_idx = configs[criterial->samples[0]].toUInt();
    size_t second_idx = configs[criterial->samples[1]].toUInt();

    QSet<FLAG> flags;
    fillFlags(configs, criterial->aproximations, flags);
    fillFlags(configs, criterial->corrects, flags);

    double alpha = configs[criterial->parameters.front().name].toDouble();

    if(configs["name"] == AnsariBradleyTest.name)
        TwoSampleScaleProblem::ansariBradleyTest(samples.at(first_idx), samples.at(second_idx), alpha, flags, result);

    else if(configs["name"] == SigelTukeyTest.name)
        TwoSampleScaleProblem::sigelTukeyTest(samples.at(first_idx), samples.at(second_idx), alpha, flags, result);

    else if(configs["name"] == MosesTest.name)
        TwoSampleScaleProblem::mosesTest(samples.at(first_idx), samples.at(second_idx), alpha, flags, result);

}

void TaskManager::handleDichotomicData(QMap<QString, QVariant>& configs, const QList<Sample>& samples, OutputData& result) {
    using Core::Criterial::BinomialCriterial;
    using Core::Criterial::EsvaluationProbability;
    using Core::Criterial::CompareProbability;

    QString name = Core::Criterial::criterialName(Core::CriterialSection::DichotomicData, configs["name"].toString());
    const Core::CriterialInfo* const criterial = Core::Criterial::getCriterialByName(name);
    size_t data_idx = configs[criterial->samples[0]].toUInt();

    QSet<FLAG> flags;
    fillFlags(configs, criterial->aproximations, flags);
    fillFlags(configs, criterial->corrects, flags);

    QSet<QString> orders;
    fillOrders(configs, criterial->aproximations, orders);
    fillOrders(configs, criterial->corrects, orders);

    if(configs["name"].toString() == BinomialCriterial.name) {
        double p0 = configs[criterial->parameters.at(0).name].toDouble();
        double alpha = configs[criterial->parameters.at(1).name].toDouble();

        Dichotomical::binomialCriterial(samples.at(data_idx), p0, alpha, flags, result);
    }

    else if(configs["name"].toString() == EsvaluationProbability.name) {
         double alpha = configs[criterial->parameters.at(0).name].toDouble();

         Dichotomical::esvaluationProbability(samples.at(data_idx), alpha, orders, flags, result);
    }

    else if(configs["name"].toString() == CompareProbability.name) {
         size_t first_idx = configs[criterial->samples[0]].toUInt();
         size_t second_idx = configs[criterial->samples[1]].toUInt();
         double alpha = configs[criterial->parameters.at(0).name].toDouble();

         Dichotomical::compareProbability(samples.at(first_idx), samples.at(second_idx), alpha, orders, flags, result);
     }

}




void fillFlags(const QMap<QString, QVariant>& configs, const QVector<Pair>& names, QSet<FLAG>& flags) {
    for(auto val: names)
        if(configs.contains(val.name))
            flags.insert(val.flag);
}

void fillOrders(const QMap<QString, QVariant>& configs, const QVector<Pair>& names, QSet<QString>& orders) {
    for(const auto& rec: names)
        if(configs.contains(rec.name))
            orders.insert(rec.name);
}
