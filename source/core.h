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

    enum class CriterialSection { ShiftProblem = 0, SignRank, DichotomicData, TwoSampleShiftProblem,
                                  TwoSampleScaleProblem };


    struct ParametersInfo {
        QString name;
        bool is_real;
        double min = std::numeric_limits<decltype(min)>::min();
        double max = std::numeric_limits<decltype(max)>::max();

        ParametersInfo(QString n, bool r = true);
        ParametersInfo(QString n, decltype(min) _min,
                            decltype(max) _max = std::numeric_limits<decltype(max)>::max(), bool r = true);
        ParametersInfo(QString n, decltype(min) _min, bool r);
    };

    struct DistributionInfo {
        DistributionType type;
        QString name;
        size_t parameters_count;
        QVector<ParametersInfo> parameters;

        // Класс распределения

        DistributionInfo(DistributionType t, QString n, QVector<ParametersInfo>&& p);
    };

    struct CriterialInfo {
     private:
        //static size_t TOTAL_ID;

     public:
        size_t id;
        CriterialSection section_type;
        QString name;
        QVector<ParametersInfo> parameters;
        QVector<QString> samples;
        QVector<Pair> aproximations;
        QVector<Pair> corrects;


        CriterialInfo(CriterialSection type, QString&& name, QVector<QString>&& sample = {},
                      QVector<ParametersInfo>&& parameters = {}, QVector<Pair>&& aproximation = {}, QVector<Pair>&& corrects = {});

        QString fullName();

        private:
            CriterialInfo& operator=(const CriterialInfo& left) = delete;
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
     * Аппроксимации
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

    const CriterialInfo BinomialCriterial = {
        CriterialSection::DichotomicData, "Биномиальный критерий",
        {"Результат схемы Бернулли"},
        { {"Проверяемое значение", 0.0, 1.0}, {"Уровень значимости", 0.0, 1.0}},
        { {"Аппроксимация нормальным распределением", FLAG::NormalAproximate} },
        { {"Доп. характеристики: мощность критерия, вероятность ошибки", FLAG::StatisticsInfo} }
    };
    const CriterialInfo EsvaluationProbability = {
        CriterialSection::DichotomicData, "Оценка вероятности успеха",
        {"Результат схемы Бернулли"},
        { {"Уровень значимости", 0.0, 1.0} },
        { {"Приближение Вальда (для доверительного интервала)", FLAG::NormalAproximate } },
        { {Order::HodgesLehmanEstimate}, {"Доверительный интервал", FLAG::ConfidenceInterval},
          {Order::CopperPearsonInterval}, {Order::WaldApproximation}, {Order::WilsonInterval} }
    };
    const CriterialInfo CompareProbability = {
        CriterialSection::DichotomicData, "Сравнение вероятностей",
        { "Исход №1", "Исход №2" },
        { {"Уровень значимости", 0.0, 1.0} },
        {  },
        { {"Доверительный интервал", FLAG::ConfidenceInterval}, {Order::AsymptoticDifferenceInterval},
          {Order::ProbabilityRatioEstimate}, {Order::ProbabilityRatioConfidenceInterval} }
    };

    const CriterialInfo MedianTestMuda = {
        CriterialSection::TwoSampleShiftProblem, "Медианный критерий Муда",
        { "Выборка №1", "Выборка №2" },
        { {"Уровень значимости", 0.0, 1.0} },
        {  },
        { {"Ранговая форма", FLAG::Ranks}, {"Поправка Йейтса на непрерывность", FLAG::ChiSquareCorrection} }
    };
    const CriterialInfo WilcoxonSummRank = {
        CriterialSection::TwoSampleShiftProblem, "Критерий ранговых сумм Уилкоксона",
        { "Выборка №1", "Выборка №2" },
        { {"Уровень значимости", 0.0, 1.0} },
        { {"Апроксимация Имана", FLAG::ImansAproximate} },
        { {"Точечная оценка", FLAG::PointEstimate}, {"Доверительный интервал", FLAG::ConfidenceInterval} }
    };
    const CriterialInfo MannWhitneyTest = {
        CriterialSection::TwoSampleShiftProblem, "Критерий Манна-Уитни",
        { "Выборка №1", "Выборка №2" },
        { {"Уровень значимости", 0.0, 1.0} },
        {  },
        { {"Коррекция на непрерывность", FLAG::ContinuityCorrection}, {"Размер эффекта", FLAG::EffectSize},
          {Order::KliffDelta}, {Order::DominatedProbability}, {Order::DominatedProbability}}
    };
    const CriterialInfo FlynnerPolycelloTest = {
        CriterialSection::TwoSampleShiftProblem, "Критерий Флиннера-Поличелло",
        { "Выборка №1", "Выборка №2" },
        { {"Уровень значимости", 0.0, 1.0} }
    };

    const CriterialInfo AnsariBradleyTest = {
        CriterialSection::TwoSampleScaleProblem, "Критерий Ансари-Брэдли",
        { "Выборка №1", "Выборка №2" },
        { {"Уровень значимости", 0.0, 1.0} }
    };
    const CriterialInfo SigelTukeyTest = {
        CriterialSection::TwoSampleScaleProblem, "Критерий Сиджела-Тьюки",
        { "Выборка №1", "Выборка №2" },
        { {"Уровень значимости", 0.0, 1.0} },
        {  },
        { {"Коррекция на непрерывность", FLAG::ContinuityCorrection} }
    };
    const CriterialInfo MosesTest = {
        CriterialSection::TwoSampleScaleProblem, "Критерий Мозеса",
        { "Выборка №1", "Выборка №2" },
        { {"Уровень значимости", 0.0, 1.0} },
    };

    /***************************************/


    const QVector<const CriterialInfo*> criterials = {
        &WilcoxonSignRank, &WilcoxonSignRankEstimation, &WilcoxonSignRankInterval,
        &SignRank, &SignRankEstimation, &SignRangInterval,
        &BinomialCriterial, &EsvaluationProbability, &CompareProbability,
        &MedianTestMuda, &WilcoxonSummRank, &MannWhitneyTest, &FlynnerPolycelloTest,
        &AnsariBradleyTest, &SigelTukeyTest, &MosesTest
    };

    const QMap<CriterialSection, QString> CriterialSectionNameByType = {
        {CriterialSection::ShiftProblem,            "Знаковый критерий сдвига"},
        {CriterialSection::SignRank,                "Критерий знаков"},
        {CriterialSection::DichotomicData,          "Дихотомические данные"},
        {CriterialSection::TwoSampleShiftProblem,   "Двухвыборочная задача о сдвиге"},
        {CriterialSection::TwoSampleScaleProblem,   "Двухвыборочная задача о масштабе"}
    };


    static QStringList fillCriterialsList() {
        QStringList names;
        names.reserve(criterials.size());
        for(const auto& crit: criterials) {
            names << CriterialSectionNameByType[crit->section_type] + " : " + crit->name;
        }
        return names;
    }

    const QStringList CriterialsNames = fillCriterialsList();

    QString criterialName(CriterialSection type, QString name);

    const CriterialInfo* getCriterialByName(const QString& name);

    QStringList getCriterialsBySection(CriterialSection type);


}

#endif // CORE_H
