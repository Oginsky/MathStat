#ifndef CORE_H
#define CORE_H

#include <QString>
#include <QVector>
#include <QStringList>

#include "commonstruct.h"

namespace Core {

    constexpr size_t DISTRIBUTIONS_COUNT(14);
    constexpr size_t CRITERIALSECTION_COUNT(2);

    enum class GraphicsType { BarsGraph = 0, Empirical, Plot };

    enum class DistributionType { Arcsine = 0, Bernoulli, Beta, Bimomial, Cantor, Chi, Exponential, FisherF,
                                  Gamma, Geometric, Laplace, Normal, StudentT, Zeta};

    enum class CriterialSection { ShiftProblem = 0, SignRank = 1, DichotomicData = 2 };


    struct ParametersInfo {
        QString name;
        bool is_real;
        double min = std::numeric_limits<decltype(min)>::min();
        double max = std::numeric_limits<decltype(max)>::max();

        ParametersInfo(QString n, bool r = true) :
            name(n), is_real(r) {

        }
        ParametersInfo(QString n, decltype(min) _min,
                            decltype(max) _max = std::numeric_limits<decltype(max)>::max(), bool r = true) :
            name(n), is_real(r), min(_min), max(_max) {

        }
        ParametersInfo(QString n, decltype(min) _min, bool r) :
            name(n), is_real(r), min(_min) {

        }
    };
    struct DistributionInfo {
        DistributionType type;
        QString name;
        size_t parameters_count;
        QVector<ParametersInfo> parameters;

        // Класс распределения

        DistributionInfo(DistributionType t, QString n, QVector<ParametersInfo>&& p) :
            type(t), name(n), parameters(p) {
            parameters_count = parameters.size();
        }
//       DistributionInfo(DistributionType t, QString n, DistrParametersInfo&& p) :
//           type(t), name(n) {
//           parameters.push_back(p);
//           parameters_count = parameters.size();
//       }
    };
    struct CriterialInfo {
        CriterialSection section_type;
        QString name;
        QVector<ParametersInfo> parameters;
        QVector<QString> samples;
        QVector<Pair> aproximations;
        QVector<Pair> corrects;


        CriterialInfo(CriterialSection type, QString&& name, QVector<QString>&& sample = {},
                      QVector<ParametersInfo>&& parameters = {}, QVector<Pair>&& aproximation = {}, QVector<Pair>&& corrects = {}) {
            section_type = type;
            this->name = name;
            this->parameters = parameters;
            this->samples = sample;
            this->aproximations = aproximation;
            this-> corrects = corrects;
        }
    };
}

namespace Core::Distribution {

    const DistributionInfo Arcsine  {DistributionType::Arcsine, "Распределение арксинуса", {{"a"}, {"b"}} };
    const DistributionInfo Bernoulli{DistributionType::Bernoulli, "Распределение Бернулли", {{"Вероятность p", 0.0, 1.0}} };
    const DistributionInfo Beta     {DistributionType::Beta, "Бета распределение", {} };
    const DistributionInfo Bimomial {DistributionType::Bimomial, "Биномиальное распределение", {} };
    const DistributionInfo Cantor   {DistributionType::Cantor, "Канторовское распределение", {{"Количество", 0, false}, {"Вероятность успеха", 0.0, 1.0}} };
    const DistributionInfo Chi      {DistributionType::Chi, "Хи-Квадрат распределение", {{"Степени свободы", 0, false}} };
    const DistributionInfo Exponential{DistributionType::Exponential, "Показательное распределение", {{"Лямда", 0, false}} };
    const DistributionInfo FisherF  {DistributionType::FisherF, "Распределение Фишера", {{"Степени 1", 0, false}, {"Степени 2", 0, false}}};
    const DistributionInfo Gamma    {DistributionType::Gamma, "Гамма распределение", {}};
    const DistributionInfo Geometric{DistributionType::Geometric, "Геометрическое распределение", {{"Вероятность", 0.0, 1.0}} };
    const DistributionInfo Laplace  {DistributionType::Laplace, "Распределение Лаппласа", {} };
    const DistributionInfo Normal   {DistributionType::Normal, "Нормальное распределение", {{"Среднее"}, {"Дисперсия", 0.0}} };
    const DistributionInfo StudentT {DistributionType::StudentT, "Распределение Стьюдента", {}};
    const DistributionInfo Zeta     {DistributionType::Zeta, "Распределение Зета", {}};



    static const QVector<const DistributionInfo*> distributions = {
        &Arcsine, &Bernoulli, &Beta, &Bimomial, &Cantor, &Chi, &Exponential, &FisherF, &Gamma,
        &Geometric, &Laplace, &Normal, &StudentT, &Zeta
    };

    static QMap<QString, DistributionType> fillTypeByName() {
        QMap<QString, DistributionType> typeByName;
        for(const auto& distr: distributions) typeByName.insert(distr->name, distr->type);
        return typeByName;
    }
    static QStringList fillNames() {
        QStringList names;
        names.reserve(distributions.size());
        for(const auto& distr: distributions) names << distr->name;
        return names;
    }

    static const QMap<QString, DistributionType> typeByName = fillTypeByName();
    static const QStringList names = fillNames();

}

namespace Core::Criterial {
    // Сделать данные константными в структурах

    /************** Критерии ***************/

    /* Формат:
     * Тип, имя
     * Имена выборок
     * Входные параметры
     * Апроксимации
     * Добавки
    */
    /* Затем зарегистрировать в criterials */

    const CriterialInfo WilcoxonSignRank = {
        CriterialSection::ShiftProblem, "Гипотезы о сдвиге",
        {"Выборка до", "Выборка после"},
        {{"Уровень доверия", 0.0, 1.0}},
        { {"Имана", FLAG::ImansAproximate} }
    };
    const CriterialInfo WilcoxonSignRankEstimation = {
        CriterialSection::ShiftProblem, "Точечная оценка сдвига",
        {"Выборка до", "Выборка после"}
    };
    const CriterialInfo WilcoxonSignRankInterval = {
        CriterialSection::ShiftProblem, "Доверительный интервал сдвига",
        {"Выборка до", "Выборка после"},
        {{"Уровень доверия", 0.0, 1.0}},
    };
    const CriterialInfo SignRank = {
        CriterialSection::SignRank, "Гипотезы о сдвиге",
        {"Выборка до", "Выборка после"},
        {{"Уровень доверия", 0.0, 1.0}},
        { {"Метод Пратта", FLAG::NormalAproximate}, {"Диксон и Муд", FLAG::ZeroCorrection} }
    };
    const CriterialInfo SignRankEstimation = {
        CriterialSection::SignRank, "Точечная оценка сдвига",
        {"Выборка до", "Выборка после"}
    };
    const CriterialInfo SignRangInterval = {
        CriterialSection::SignRank, "Доверительный интервал для сдвига",
        {"Выборка до", "Выборка после"},
        {{"Уровень доверия", 0.0, 1.0}},
        { {"Аппроксимация нормальным распределением", FLAG::NormalAproximate} }
    };


    /***************************************/


    const QVector<const CriterialInfo*> criterials = {
        &WilcoxonSignRank, &WilcoxonSignRankEstimation, &WilcoxonSignRankInterval,
        &SignRank, &SignRankEstimation, &SignRangInterval,
    };

    const QMap<CriterialSection, QString> CriterialSectionNameByType = {
        {CriterialSection::ShiftProblem,    "Знаковый критерий сдвига"},
        {CriterialSection::SignRank,        "Критерий знаков"},
        {CriterialSection::DichotomicData,  "Дихотомические данные"},
    };


    static QStringList fillCriterialsList() {
        QStringList names;
        for(const auto& crit: criterials) {
            names << CriterialSectionNameByType[crit->section_type] + " : " + crit->name;
        }
        return names;
    }

    const QStringList CriterialsNames = fillCriterialsList();

    static QString criterialName(CriterialSection type, QString name) {
        return CriterialSectionNameByType[type] + " : " + name;
    }

    static const CriterialInfo* getCriterialByName(const QString& name) {
        return criterials[CriterialsNames.indexOf(name)];
    }


}


#endif // CORE_H
